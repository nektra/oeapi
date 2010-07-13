/* $Id: folder.cpp,v 1.33 2008/09/07 18:37:23 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"
#include "folder.h"
#include "message.h"
#include "manager.h"
#include "oestore.h"

#include "../lib/oeapi.h"

#include "oeapi_utils.h"

#include "se_debug.h"

#ifdef EVALUATION_VERSION
#include "eval_utils.h"
#endif // EVALUATION_VERSION

#define OE_VALID_FLAGS 0xFFFF

#include <comet/safearray.h>

extern HINSTANCE hInst;

LRESULT CALLBACK FolderWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND CreateFolderCallbackWindow()
{
	WNDCLASSEX wndclass;
	const TCHAR szFolderSink[] = _T("OEAPI Folder sink");
	HWND hWnd;

	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= 0;
	wndclass.lpfnWndProc	= &FolderWndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInst;
	wndclass.hIcon			= NULL;
	wndclass.hCursor		= NULL;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szFolderSink;
	wndclass.hIconSm		= NULL;

	// Register it
	RegisterClassEx(&wndclass);

	// And create a window
	hWnd = CreateWindow(szFolderSink,
				_T("Folder"),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				400, 200,
				NULL,
				NULL,
				hInst,
				NULL);

	if (hWnd == NULL) {
		debug_print(DEBUG_ERROR, _T("CreateFolderCallbackWindow: CreateWindow() failed.\n"));
		return NULL;
	}

	return hWnd;
}

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
LRESULT CALLBACK FolderWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TOEFolder *pThis = (TOEFolder *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

//	// BEGIN DEBUG
//	CHAR msg[256];
//	sprintf(msg,"FolderWndProc: Message %x wParam %x lParam %x\n", uMsg, wParam, lParam);
//	dbgprint(msg);
//	// END DEBUG
	switch (uMsg)
	{
		case WM_NEWMSGS:
		{
			if(IsWMail()) {
				LPDWORD pId = (LPDWORD)wParam;
				for(int i=0; i<(int)lParam; i++)
				{
					PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, pId[i], lParam);
				}
			}
			else {
				DWORD id;
				id = (DWORD)wParam;
				PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, id, lParam);
			}
			break;
		}
		case WM_MARKEDASREAD:
		case WM_MARKEDASUNREAD:
		case WM_DELETEMSGS:
		{
			if(IsWMail()) {
				LPDWORD pId = (LPDWORD)wParam;
				for(int i=0; i<(int)lParam; i++)
				{
					PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, pId[i], lParam);
				}
			//	if(!CopyFromWinMail(&id, (const void *) wParam, sizeof(DWORD))) {
			//		debug_print(DEBUG_ERROR, _T("StoreWndProc: CopyFromWinMail Error.\n"));
			//		break;
			//	}
			}
			else {
				DWORD id;
				id = *(DWORD*) wParam;
				PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, id, 0);
			}
			break;
		}
		case WM_DELETEFOLDER:
			pThis->OnDeleteEvent();
			break;
		case OEAPI_MSG_BASE + WM_NEWMSGS:
			pThis->OnNewMessageEvent((LONG) wParam);
			break;
		case OEAPI_MSG_BASE + WM_DELETEMSGS:
		{
 			pThis->OnMessageDeletedEvent((LONG)wParam);
			break;
		}
		case OEAPI_MSG_BASE + WM_MARKEDASUNREAD:
			pThis->OnMessageUnreadChangeEvent((LONG)wParam, TRUE);
			break;
		case OEAPI_MSG_BASE + WM_MARKEDASREAD:
			pThis->OnMessageUnreadChangeEvent((LONG)wParam, FALSE);
			break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#if _MSC_VER >= 1400
#pragma warning(push)
// disable warning C4812: 
// obsolete declaration style: please use 'coclass_implementation<comet::OESTORE::OEFolder>::coclass_implementation' instead
#pragma warning(disable:4812) 
#endif

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEFolder::TOEFolder()
{
	OEStoreManager::Get()->AddFolder(this);
	refCount_ = 0;
	oeapiId_ = -1;
	specialId_ = -1;
	id_ = 0;
	pSF_ = NULL;
	hEnumFolder_ = NULL;
	hEnumMsg_ = HENUMSTORE_INVALID;
	hWnd_ = NULL;
//	reporter_ = NULL;
//	recursiveEvents_ = FALSE;
	msgFolder_ = NULL;
}

//---------------------------------------------------------------------------//
TOEFolder::~TOEFolder()
{
	Close();
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::Close() 
{
//	SetRecursive(FALSE);

	if(hEnumMsg_ != HENUMSTORE_INVALID) {
		pSF_->GetMessageClose(hEnumMsg_);
		hEnumMsg_ = HENUMSTORE_INVALID;
	}

	if(hEnumFolder_) {
		OEStoreManager::Get()->GetSN()->GetSubFolderClose(hEnumFolder_);
		hEnumFolder_ = NULL;
	}

	if(pSF_) {
		if(hWnd_) {
			pSF_->UnregisterNotification(0, hWnd_);
			DestroyWindow(hWnd_);
			hWnd_ = NULL;
		}
		pSF_->Release();
		pSF_ = NULL;
	}

	if(msgFolder_) 
	{
		msgFolder_->Release();
		msgFolder_ = NULL;
	}

	if(OEStoreManager::Get(false)) {
		OEStoreManager::Get()->RemoveFolder(this);
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
void TOEFolder::RegisterNotifier()
{
	debug_print(DEBUG_INFO, _T("TOEFolder::RegisterNotifier: Called\n"));
	if(pSF_ != NULL && hWnd_ == NULL && connection_point.is_connected()) {
		hWnd_ = CreateFolderCallbackWindow();
		if(hWnd_) {
			::SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR)this);
			HRESULT hr = pSF_->RegisterNotification(0, hWnd_);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEFolder::RegisterNotifier: RegisterNotification failed\n"));
			}
		}
	}
}

//---------------------------------------------------------------------------//
void TOEFolder::UnregisterNotifier()
{
	debug_print(DEBUG_INFO, _T("TOEFolder::UnregisterNotifier: Called\n"));
	if(pSF_ != NULL && hWnd_ != NULL && !connection_point.is_connected())
	{
		HRESULT hr = pSF_->UnregisterNotification(0, hWnd_);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::UnregisterNotifier: UnregisterNotification failed\n"));
		}
		::DestroyWindow(hWnd_);
		hWnd_ = NULL;
	}
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::SetID(DWORD id, IStoreFolder* pSF)
{
	id_ = id;

	if(id != FOLDERID_ROOT) {
		OEStoreManager::Get()->OpenFolder(id, 0, pSF, this);

		/* HRESULT hr;
		if(pSF == NULL) {
			hr = OEStoreManager::Get()->GetSN()->OpenFolder(id, 0, &pSF_);
		}
		else {
			hr = S_OK;
			pSF_ = pSF;
			pSF_->AddRef();
		}

		FOLDERPROPS props;

		if(FAILED(hr)) {
			pSF_ = NULL;
			debug_print(DEBUG_ERROR, _T("OEFolder::SetID: OpenFolder\n"));
			return FALSE;
		}

		props.cbSize = sizeof(FOLDERPROPS);

		hr = pSF_->GetFolderProps(0, &props);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::SetID: GetFolderProps\n"));
			return FALSE;
		}

		specialId_ = props.sfType;

		//[]RegisterNotifier(); */
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
void TOEFolder::SetSpecialID(DWORD specialId)
{
	OEStoreManager::Get()->OpenFolder(0, specialId, NULL, this);

	/*HRESULT hr = OEStoreManager::Get()->GetSN()->OpenSpecialFolder((SPECIALFOLDER) specialId, 0, &pSF_);
	if(FAILED(hr)) {
		pSF_ = NULL;
		debug_print(DEBUG_ERROR, _T("OEFolder::SetSpecialID: OpenSpecialFolder\n"));
		return;
	}
	FOLDERPROPS props;
	props.cbSize = sizeof(FOLDERPROPS);

	hr = pSF_->GetFolderProps(0, &props);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::SetSpecialID: GetFolderProps\n"));
		return;
	}

	id_ = props.dwFolderId;
	specialId_ = specialId;

	//[] RegisterNotifier(); */
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolder::GetFirstChild()
{
	if(pSF_ == NULL && id_ != FOLDERID_ROOT) {
		TOEFolderPtr ret;
		return ret;
	}

	if(hEnumFolder_) {
		OEStoreManager::Get()->GetSN()->GetSubFolderClose(hEnumFolder_);
		hEnumFolder_ = NULL;
	}

	std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(OEStoreManager::Get()->GetSN()));

	HRESULT hr = props->GetFirstSubFolder(id_, &hEnumFolder_);
	if(FAILED(hr) || hr == 1 || hEnumFolder_ == NULL) {
		TOEFolderPtr ret;
		return ret;
	}

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	folder->SetID(props->GetID());

	return ret;
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolder::GetNextChild()
{
	if(pSF_ == NULL && id_ != FOLDERID_ROOT) {
		TOEFolderPtr ret;
		return ret;
	}

	if(hEnumFolder_ == NULL) {
		return GetFirstChild();
	}

	std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(OEStoreManager::Get()->GetSN()));

	HRESULT hr = props->GetNextSubFolder(hEnumFolder_);
	if(FAILED(hr) || hr == 1 || hEnumFolder_ == NULL) {
		if(hEnumFolder_) {
			OEStoreManager::Get()->GetSN()->GetSubFolderClose(hEnumFolder_);
			hEnumFolder_ = NULL;
		}

		TOEFolderPtr ret;
		return ret;
	}

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	folder->SetID(props->GetID());

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEFolder::Name()
{
	bstr_t ret = OLESTR("");

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return ret;
	}
#endif // EVALUATION_VERSION

	if(id_ == FOLDERID_ROOT) {
		ret = OLESTR("Local Folders");
	}
	else if(pSF_) {
		std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(pSF_));

		HRESULT hr;
		hr = props->GetFolderProps();
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::Name: GetFolderProps\n"));
			return OLESTR("");
		}

		ret = props->GetName();
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::Rename(const bstr_t &newName)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

	if(id_ == FOLDERID_ROOT) {
		debug_print(DEBUG_ERROR, _T("OEFolder::Rename: Root folder cannot be renamed.\n"));
		return FALSE;
	}

	HRESULT hr = OEStoreManager::Get()->GetSN()->RenameFolder(GetID(), 0, newName.s_str().c_str());
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::Rename: RenameFolder\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::Move(DWORD newParentId)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

	if(id_ == FOLDERID_ROOT) {
		debug_print(DEBUG_ERROR, _T("OEFolder::Move: Root folder cannot be moved.\n"));
		return FALSE;
	}

	HRESULT hr = OEStoreManager::Get()->GetSN()->MoveFolder(GetID(), newParentId, 0);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::Move: MoveFolder\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::Delete()
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

	if(id_ == FOLDERID_ROOT) {
		debug_print(DEBUG_ERROR, _T("OEFolder::Delete: Root folder cannot be deleted.\n"));
		return FALSE;
	}

	HRESULT hr = OEStoreManager::Get()->GetSN()->DeleteFolder(GetID(), 0);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::Delete: DeleteFolder\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
long TOEFolder::CreateFolder(const bstr_t &name)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return -1;
	}
#endif // EVALUATION_VERSION

	DWORD newId;

	HRESULT hr = OEStoreManager::Get()->GetSN()->CreateFolder(id_, name.s_str().c_str(), 0, &newId);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::CreateFolder: CreateFolder\n"));
		return -1;
	}

	return newId;
}

//---------------------------------------------------------------------------//
TOEMessagePtr TOEFolder::GetFirstMessage()
{
	TOEMessagePtr ret;

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return ret;
	}
#endif // EVALUATION_VERSION

	if(id_ == FOLDERID_ROOT) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetFirstMessage: Root doesn't have messages.\n"));
		return ret;
	}

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetFirstMessage: invalid pointer\n"));
		return ret;
	}

	if(hEnumMsg_ != HENUMSTORE_INVALID) {
		pSF_->GetMessageClose(hEnumMsg_);
		hEnumMsg_ = HENUMSTORE_INVALID;
	}

	NktMessageProps* pMsgProps = NULL;
	pMsgProps = NktMessageProps::Create(pSF_);

	HRESULT hr;
	hr = pMsgProps->GetFirstMessage(0, MESSAGEID_FIRST, &hEnumMsg_);
	if(FAILED(hr) || hr == 1) {
		hEnumMsg_ = HENUMSTORE_INVALID;
		delete pMsgProps;
		return ret;
	}

	ret = TOEMessagePtr(TOEMessage::newInstance());
	TOEMessage *msg = (TOEMessage *) ret.get();
	if(msg == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetFirstMessage: Invalid pointer\n"));
		delete pMsgProps;
		ret = NULL;
		return ret;
	}

	msg->SetMessageProps(id_, pSF_, pMsgProps, msgFolder_);

	return ret;
}

//---------------------------------------------------------------------------//
TOEMessagePtr TOEFolder::GetNextMessage()
{
	TOEMessagePtr ret;

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return ret;
	}
#endif // EVALUATION_VERSION

	if(id_ == FOLDERID_ROOT) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetNextMessage: Root doesn't have messages.\n"));
		return ret;
	}

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetNextMessage: invalid pointer\n"));
		return ret;
	}

	if(hEnumMsg_ == HENUMSTORE_INVALID) {
		return GetFirstMessage();
	}

	NktMessageProps* pMsgProps = NULL;
	pMsgProps = NktMessageProps::Create(pSF_); 

	HRESULT hr;
	hr = pMsgProps->GetNextMessage(hEnumMsg_, 0);
	if(FAILED(hr) || hr == 1) {
		pSF_->GetMessageClose(hEnumMsg_);
		hEnumMsg_ = HENUMSTORE_INVALID;
		delete pMsgProps;
		return ret;
	}

	ret = TOEMessagePtr(TOEMessage::newInstance());
	TOEMessage *msg = (TOEMessage *) ret.get();
	if(msg == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetNextMessage: Invalid pointer\n"));
		delete pMsgProps;
		ret = NULL;
		return ret;
	}

	msg->SetMessageProps(id_, pSF_, pMsgProps, msgFolder_);

	return ret;
}

//---------------------------------------------------------------------------//
TOEMessagePtr TOEFolder::GetMessage(DWORD msgId)
{
	TOEMessagePtr ret;

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return ret;
	}
#endif // EVALUATION_VERSION

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetMessage: invalid pointer\n"));
		return ret;
	}

	NktMessageProps* pMsgProps;
	pMsgProps = NktMessageProps::Create(pSF_);

	HRESULT hr;
	hr = pMsgProps->GetMessageProps(msgId, 0);
	if(FAILED(hr)) {
		delete pMsgProps;
		return ret;
	}

	ret = TOEMessagePtr(TOEMessage::newInstance());
	TOEMessage *msg = (TOEMessage *) ret.get();
	if(msg == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetMessage: Invalid msg pointer\n"));
		delete pMsgProps;
		ret = NULL;
		return ret;
	}

	msg->SetMessageProps(id_, pSF_, pMsgProps, msgFolder_);

	return ret;
}

//---------------------------------------------------------------------------//
TOEMessagePtr TOEFolder::CreateMessage(const bstr_t &msgSource, DWORD status)
{
	TOEMessagePtr ret;

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return ret;
	}
#endif // EVALUATION_VERSION

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: invalid pointer\n"));
		return ret;
	}

	DWORD flags = HIWORD(status);

	if((flags & OE_VALID_FLAGS) != flags)
	{
		// Assume it is a BOOL
		flags = (flags) ? MSG_UNREAD : 0;
	}

	HRESULT hr;
	MESSAGEID msgId;

	if(IsWMail()) {
		IStream* pStream = NULL;
		hr = ::CreateStreamOnHGlobal(NULL, TRUE, &pStream);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: CreateStreamOnHGlobal.\n"));
			return ret;
		}

		ULONG len;
		hr = pStream->Write((void *) msgSource.s_str().c_str(), (ULONG)msgSource.length(), &len);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: Write.\n"));
			return ret;
		}

		hr = pStream->Commit(0); 

		hr = pSF_->SaveMessage(IID_IStream, pStream, flags, &msgId);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: SaveMessage.\n"));
			return ret;
		}

		pStream->Release();
	}
	else {
		IStream *newMail = NULL;
		HRESULT hr = pSF_->CreateStream(0, 0, &newMail, &msgId);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: CreateStream.\n"));
			return ret;
		}

		ULONG len;

		hr = newMail->Write((void *) msgSource.s_str().c_str(), (ULONG)msgSource.length(), &len);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: Write\n"));
			newMail->Release();
			return ret;
		}

		hr = pSF_->CommitStream(0, 0, flags, newMail, msgId, NULL);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: CommitStream.\n"));
			newMail->Release();
			return ret;
		}

		newMail->Release();
	}

	ret = TOEMessagePtr(TOEMessage::newInstance());
	TOEMessage *msg = (TOEMessage *) ret.get();
	if(msg == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::CreateMessage: Invalid msg pointer\n"));
		ret = NULL;
		return ret;
	}

	msg->SetMessageProps(id_, pSF_, msgId, msgFolder_);

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::DeleteMessage(DWORD msgId, BOOL permanent)
{
	BOOL ret = FALSE;

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

	TOEMessagePtr msg = GetMessage(msgId);

	if(!msg.is_null()) {
		ret = msg->Delete(permanent);
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEFolder::DeleteMessage: invalid message id.\n"));
		return FALSE;
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::MarkAsRead(DWORD msgId)
{
	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::MarkAsRead: invalid pointer\n"));
		return FALSE;
	}

	MESSAGEIDLIST msgIdList;
	DWORD msgIds[] = {msgId};

	msgIdList.cbSize = sizeof(MESSAGEIDLIST);
	msgIdList.cMsgs = 1;
	msgIdList.prgdwMsgId = msgIds;

	HRESULT hr = pSF_->MarkMessagesAsRead(TRUE, 0, &msgIdList);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::MarkAsRead: MarkMessagesAsRead\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL TOEFolder::MarkAsUnread(DWORD msgId)
{
	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::MarkAsUnread: invalid pointer\n"));
		return FALSE;
	}

	MESSAGEIDLIST msgIdList;
	DWORD msgIds[] = {msgId};

	msgIdList.cbSize = sizeof(MESSAGEIDLIST);
	msgIdList.cMsgs = 1;
	msgIdList.prgdwMsgId = msgIds;

	HRESULT hr = pSF_->MarkMessagesAsRead(FALSE, 0, &msgIdList);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::MarkAsUnread: MarkMessagesAsRead\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
DWORD TOEFolder::GetSubFolderCount()
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return -1;
	}
#endif // EVALUATION_VERSION

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetSubFolderCount: invalid pointer\n"));
		return -1;
	}

	std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(pSF_));

	HRESULT hr;
	hr = props->GetFolderProps();
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetUnreadCount: GetFolderProps\n"));
		return -1;
	}

	return props->GetSubFolders();
}

//---------------------------------------------------------------------------//
DWORD TOEFolder::GetUnreadCount()
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return -1;
	}
#endif // EVALUATION_VERSION

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetUnreadCount: invalid pointer\n"));
		return -1;
	}

	std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(pSF_));

	HRESULT hr = props->GetFolderProps();
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetUnreadCount: GetFolderProps\n"));
		return -1;
	}

	return props->GetUnreadCount();
}

//---------------------------------------------------------------------------//
//STDMETHODIMP TOEFolder::FindConnectionPoint(REFIID riid, IConnectionPoint** ppCP)
//{
//	HRESULT hr = implement_cpc<OEFolder::source_interfaces>::FindConnectionPoint(riid, ppCP);
//	if(SUCCEDDED(hr))
//	{
//	}
//}

//---------------------------------------------------------------------------//
DWORD TOEFolder::GetMessageCount()
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return -1;
	}
#endif // EVALUATION_VERSION

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetMessageCount: invalid pointer\n"));
		return -1;
	}

	std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(pSF_));

	HRESULT hr = props->GetFolderProps();
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetMessageCount: GetFolderProps\n"));
		return -1;
	}

	return props->GetMessageCount();
}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEFolder *TOEFolder::newInstance()
{
	return new TOEFolder;
}
#else
com_ptr<IOEFolder> TOEFolder::newInstance()
{
	com_ptr<IOEFolder> ret(uuidof<OEFolder>());
	return ret;
}
#endif // STATIC_LIBRARY


//---------------------------------------------------------------------------//
void TOEFolder::OnNewMessageEvent(LONG msgId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnNewMessage(msgId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolder::OnNewMessageEvent: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEFolder::OnMessageDeletedEvent(LONG msgId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnMessageDeleted(msgId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolder::OnMessageDeletedEvent: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEFolder::OnMessageUnreadChangeEvent(LONG msgId, BOOL unread)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnMessageUnreadChange(msgId, unread);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolder::OnMessageUnreadChange: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}


//---------------------------------------------------------------------------//
void TOEFolder::OnDeleteEvent()
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnDeletion();
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolder::OnDeleteEvent: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEFolderManager::TOEFolderManager()
{
	refCount_ = 0;
//	lastReturnedId_ = -1;
//	globalNewMsgActive_ = FALSE;

	OEStoreManager::Get()->AddFolderManager(this);
}

//---------------------------------------------------------------------------//
TOEFolderManager::~TOEFolderManager()
{
	Close();
}

//---------------------------------------------------------------------------//
BOOL TOEFolderManager::Close() 
{
	curFolder_ = NULL;

	CloseAllEnumHandles();

	if(OEStoreManager::Get(false)) {
		OEStoreManager::Get()->RemoveFolderManager(this);
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
void TOEFolderManager::ActivateGlobalNotification()
{
	OEStoreManager::Get()->ActivateGlobalNotification();
//	DWORD maxId;
//	// functions GetFirstFolder / GetNextFolder changes curFolder_
//	TOEFolderPtr curFolderBackup = curFolder_;
//	TOEFolderPtr folder;
//
//	maxMsgIds_.clear();
//
//	folder = GetFirstFolder();
//
//	while(!folder.is_null()) {
//		maxId = GetMaxMsgID(folder);
//		if(maxId != -1) {
//			maxMsgIds_[folder->GetID()] = maxId;
//		}
//
//		folder = GetNextFolder();
//	}
//
//	globalNewMsgActive_ = TRUE;
//
//	curFolder_ = curFolderBackup;
}
//
////---------------------------------------------------------------------------//
//void TOEFolderManager::Restart()
//{
//	OEStoreManager::Get()->Restart();
//}
//
////---------------------------------------------------------------------------//
//DWORD TOEFolderManager::GetMaxMsgID(TOEFolderPtr folderPtr)
//{
//	DWORD maxId = -1;
//	TOEFolder *folder;
//	HENUMSTORE hEnum;
//	HRESULT hr;
//	MESSAGEPROPS msgProps;
//
//	folder = (TOEFolder *) folderPtr.get();
//
//	if(folder->GetSF() != NULL) {
//		msgProps.cbSize = sizeof(MESSAGEPROPS);
//
//		hr = folder->GetSF()->GetFirstMessage(MSGPROPS_FAST, 0, MESSAGEID_FIRST, &msgProps, &hEnum);
//
//		while(SUCCEEDED(hr) && hr != S_FALSE) {
//			maxId = msgProps.dwMessageId;
//
//			folder->GetSF()->FreeMessageProps(&msgProps);
//
//			hr = folder->GetSF()->GetNextMessage(hEnum, MSGPROPS_FAST, &msgProps);
//		}
//
//		if(SUCCEEDED(hr)) {
//			folder->GetSF()->GetMessageClose(hEnum);
//		}
//	}
//
//	return maxId;
//}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetSpecialFolder(DWORD specialFolder)
{
	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	folder->SetSpecialID(specialFolder);
	return ret;
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetInboxFolder()
{
	return GetSpecialFolder(FOLDER_INBOX);
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetOutboxFolder()
{
	return GetSpecialFolder(FOLDER_OUTBOX);
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetSentFolder()
{
	return GetSpecialFolder(FOLDER_SENT);
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetDeletedFolder()
{
	return GetSpecialFolder(FOLDER_DELETED);
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetDraftFolder()
{
	return GetSpecialFolder(FOLDER_DRAFT);
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetFolder(DWORD folderId)
{
	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	if(folder) {
		if(!folder->SetID(folderId)) {
			ret = NULL;
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetFirstFolder()
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		TOEFolderPtr ret;
		return ret;
	}
#endif // EVALUATION_VERSION

	CloseAllEnumHandles();
	curFolder_ = TOEFolderPtr(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) curFolder_.get();
	if(folder == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::GetFirstFolder: invalid pointer\n"));
		return curFolder_;
	}
	else {
		folder->SetID(FOLDERID_ROOT);
	}

	return curFolder_;
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetNextFolder()
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		TOEFolderPtr ret;
		return ret;
	}
#endif // EVALUATION_VERSION

	TOEFolder *folder = (TOEFolder *) curFolder_.get();

	if(folder == NULL) {
		return GetFirstFolder();
	}

	//FOLDERPROPS props;
	//props.cbSize = sizeof(FOLDERPROPS);
	HENUMSTORE hEnum;
	DWORD oldFolderId = folder->GetID();

	// the reference count of the curFolder must be at least 1 while we are using it and it
	// must be released
	curFolder_ = NULL;

	// first, try to go in depth. If no childs, return the next sibling. If no next sibling return the sibling of the
	// parent and then.
	std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(OEStoreManager::Get()->GetSN()));
	HRESULT hr = props->GetFirstSubFolder(oldFolderId, &hEnum);
	if(FAILED(hr) || hr == 1 || hEnum == NULL) {
		while(!hEnumList_.empty() && curFolder_.get() == NULL) {
			HENUMSTORE hEnum = hEnumList_.back();
			std::auto_ptr<NktFolderProps> props2(NktFolderProps::Create(OEStoreManager::Get()->GetSN()));

			HRESULT hr = props2->GetNextSubFolder(hEnum);
			if(FAILED(hr) || hr == 1 || hEnum == NULL) {
				OEStoreManager::Get()->GetSN()->GetSubFolderClose(hEnum);
				hEnumList_.pop_back();
			}
			else {
				curFolder_ = TOEFolderPtr(TOEFolder::newInstance());
				folder = (TOEFolder *) curFolder_.get();
				if(folder == NULL) {
					CloseAllEnumHandles();
					debug_print(DEBUG_ERROR, _T("OEFolder::GetNextFolder: GetNextSubFolder invalid pointer\n"));
					return curFolder_;
				}
				else {
					folder->SetID(props2->GetID());
				}
			}
		}
	}
	else {
		hEnumList_.push_back(hEnum);
		curFolder_ = TOEFolderPtr(TOEFolder::newInstance());
		TOEFolder *folder = (TOEFolder *) curFolder_.get();
		if(folder == NULL) {
			CloseAllEnumHandles();
			debug_print(DEBUG_ERROR, _T("OEFolder::GetNextFolder: invalid pointer\n"));
			return curFolder_;
		}
		else {
			folder->SetID(props->GetID());
		}
	}

	return curFolder_;
}

//---------------------------------------------------------------------------//
TOEFolderPtr TOEFolderManager::GetNextSpecialFolder(DWORD specialId)
{
	TOEFolderPtr ret;

	// if FOLDER_DRAFT it doesn't have next
	switch(specialId) {
	case FOLDER_INBOX:
		ret = GetOutboxFolder();
		break;
	case FOLDER_OUTBOX:
		ret = GetSentFolder();
		break;
	case FOLDER_SENT:
		ret = GetDeletedFolder();
		break;
	case FOLDER_DELETED:
		ret = GetDraftFolder();
		break;
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEFolderManager::CloseAllEnumHandles()
{
	while(!hEnumList_.empty()) {
		HENUMSTORE hEnum = hEnumList_.back();
		if(FAILED(OEStoreManager::Get()->GetSN()->GetSubFolderClose(hEnum)))
		{
			debug_print(0, _T("TOEFolderManager::CloseAllEnumHandles: "));
		}
		hEnumList_.pop_back();
	}
}

//---------------------------------------------------------------------------//
BOOL TOEFolderManager::RenameFolder(DWORD folderId, const bstr_t &newName)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	if(folder == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::RenameFolder: invalid pointer\n"));
		return FALSE;
	}

	folder->SetID(folderId);
	return folder->Rename(newName);
}

//---------------------------------------------------------------------------//
BOOL TOEFolderManager::MoveFolder(DWORD folderId, DWORD newParentId)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	if(folder == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::MoveFolder: invalid pointer\n"));
		return FALSE;
	}

	folder->SetID(folderId);
	return folder->Move(newParentId);
}

//---------------------------------------------------------------------------//
BOOL TOEFolderManager::DeleteFolder(DWORD folderId)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	if(folder == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::DeleteFolder: invalid pointer\n"));
		return FALSE;
	}
	folder->SetID(folderId);
	if(!curFolder_.is_null() && curFolder_->GetID() == folderId) {
		curFolder_ = GetNextFolder();
	}

	return folder->Delete();
}

//---------------------------------------------------------------------------//
long TOEFolderManager::CreateFolder(DWORD parentFolderId, const bstr_t &newName)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return -1;
	}
#endif // EVALUATION_VERSION

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	if(folder == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::DeleteFolder: invalid pointer\n"));
		return FALSE;
	}
	folder->SetID(parentFolderId);

	return folder->CreateFolder(newName);
}

//---------------------------------------------------------------------------//
BOOL TOEFolderManager::CopyMoveMessagesSynch(BOOL move, DWORD srcFolderId, DWORD destFolderId,
																	const safearray_t<long> &msgIds)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return FALSE;
	}
#endif // EVALUATION_VERSION

//	CProgress progNotify;
	IStoreFolder *pSFsrc, *pSFdest;
	BOOL ret = TRUE;

	HRESULT hr = OEStoreManager::Get()->GetSN()->OpenFolder(srcFolderId, 0, &pSFsrc);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolderManager::CopyMoveMessages: OpenFolder\n"));
		return FALSE;
	}

	hr = OEStoreManager::Get()->GetSN()->OpenFolder(destFolderId, 0, &pSFdest);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEFolderManager::CopyMoveMessages: OpenFolder\n"));
		return FALSE;
	}

	MESSAGEIDLIST msgIdList;
	DWORD id;
	MESSAGEPROPS msgProps;

	for(safearray_t<long>::const_iterator it=msgIds.begin(); it!=msgIds.end(); it++) {
		DWORD props = -1;

		msgIdList.cbSize = sizeof(MESSAGEIDLIST);
		msgIdList.cMsgs = 1;
		msgIdList.prgdwMsgId = &id;
		id = *it;

		// Remove all the properties that are not set in the message
		msgProps.cbSize = sizeof(MESSAGEPROPS);
		hr = pSFsrc->GetMessageProps(id, 0, &msgProps);
		if(SUCCEEDED(hr)) {
			props &= ~msgProps.dwFlags;
			hr = OEStoreManager::Get()->GetSN()->CopyMoveMessages(pSFsrc, pSFdest, &msgIdList,
															(move ? CMF_MOVE : 0), props, NULL/*&progNotify*/);

			pSFsrc->FreeMessageProps(&msgProps);
		}
		if(FAILED(hr)) {
			ret = FALSE;
		}
	}

	pSFsrc->Release();
	pSFdest->Release();

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEFolderManager::CopyMessage(DWORD srcFolderId, DWORD destFolderId,
															DWORD msgId)
{
	safearray_t<long> msgIds;
	msgIds.push_back(msgId);
	return CopyMoveMessagesSynch(FALSE, srcFolderId, destFolderId, msgIds);
}

//---------------------------------------------------------------------------//
BOOL TOEFolderManager::MoveMessage(DWORD srcFolderId, DWORD destFolderId,
															DWORD msgId)
{
	safearray_t<long> msgIds;
	msgIds.push_back(msgId);
	return CopyMoveMessagesSynch(TRUE, srcFolderId, destFolderId, msgIds);
}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEFolderManager *TOEFolderManager::newInstance()
{
	return new TOEFolderManager;
}
#else
com_ptr<IOEFolderManager> TOEFolderManager::newInstance()
{
	com_ptr<IOEFolderManager> ret(uuidof<OEFolderManager>());
	return ret;
}
#endif // STATIC_LIBRARY

//---------------------------------------------------------------------------//
void TOEFolderManager::OnEvent(LPFOLDERNOTIFYEX folderNotify)
{
	switch(folderNotify->type) {
	case NEW_FOLDER:
		OnFolderCreated(folderNotify->idFolderNew);
		break;
	case DELETE_FOLDER:
		OnFolderDeleted(folderNotify->idFolderNew);
		break;
	case RENAME_FOLDER:
		OnFolderRenamed(folderNotify->idFolderNew);
		break;
	case MOVE_FOLDER:
		OnFolderMoved(folderNotify->idFolderNew);
		break;
	case UNREAD_CHANGE:
		OnFolderUnreadChanged(folderNotify->idFolderNew);
		break;
	case IMAPFLAG_CHANGE:
		break;
	case UPDATEFLAG_CHANGE:
		break;
	}
}

//---------------------------------------------------------------------------//
void TOEFolderManager::OnFolderCreated(DWORD folderId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnFolderCreated(folderId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolderManager::OnFolderCreated: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEFolderManager::OnFolderDeleted(DWORD folderId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnFolderDeleted(folderId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolderManager::OnFolderDeleted: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEFolderManager::OnFolderRenamed(DWORD folderId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnFolderRenamed(folderId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolderManager::OnFolderRenamed: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEFolderManager::OnFolderMoved(DWORD folderId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnFolderMoved(folderId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolderManager::OnFolderMoved: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEFolderManager::OnFolderUnreadChanged(DWORD folderId)
{
//	TOEFolderPtr folderPtr;
//	DWORD maxMsgId, oldMaxMsgId = -1;
//	BOOL newMsg = FALSE;
//	TOEMessagePtr msg;
//	TOEFolder *pFolder;
//
//	if(globalNewMsgActive_) {
//		folderPtr = GetFolder(folderId);
//
//		if(!folderPtr.is_null()) {
//			pFolder = (TOEFolder *) folderPtr.get();
//
//			// if it is a special folder different from Inbox do not notify
//			if(!pFolder->IsDeleted() && !pFolder->IsOutbox() && !pFolder->IsSent()
//				&& !pFolder->IsDraft()) {
//				maxMsgId = GetMaxMsgID(folderPtr);
//
//				if(maxMsgId != -1) {
////					// BEGIN DEBUG
////					{
////						char msgStr[1024];
////						TOEMessagePtr aamsg = folderPtr->GetMessage(maxMsgId);
////						sprintf(msgStr, "OnFolderUnreadChanged: %x %s.\n", maxMsgId,
////							(aamsg.is_null() ? "" : aamsg->GetSubject().t_str().c_str()));
////						dbgprint(msgStr);
////					}
////					// END DEBUG
//
//					// if the folder is not in the list, it has no messages, so any message is new
//					// otherwise, the new max id must be greater that the old max
//					if(maxMsgIds_.find(folderId) != maxMsgIds_.end()) {
//						oldMaxMsgId = maxMsgIds_[folderId];
//
//						if(oldMaxMsgId < maxMsgId) {
//							newMsg = TRUE;
//						}
//					}
//					else {
//						newMsg = TRUE;
//					}
//
//					if(newMsg) {
//						maxMsgIds_[folderId] = maxMsgId;
//
//						if(oldMaxMsgId == -1) {
//							oldMaxMsgId = maxMsgId-1;
//						}
//
//						// NOTE: new messages can be notified in any order so if OE is receiving
//						// more than 1 message, it could notify first the second and then the
//						// first. In that situation, the first message won't be notified by
//						// OEAPI if I just notify the last one. To fix this, I notify all the
//						// messages between the oldMaxMsgId and the last one.
//						// Then, notifications of the previous messages won't enter here because
//						// maxMsgIds_[folderId] > msgId
//						for(int i=oldMaxMsgId+1; i<=maxMsgId; i++) {
//							// notify only if it is a unread message
//							msg = pFolder->GetMessage(i);
//							if(!msg.is_null() && (msg->GetState() & comet::OESTORE::OE_MSG_UNREAD)) {
//								OnNewMessage(folderId, i);
//							}
//
////							// BEGIN DEBUG
////							{
////								char msgStr[1024];
////								sprintf(msgStr, "OnFolderUnreadChanged: %x %s.\n", maxMsgId,
////									(msg.is_null() ? "" : msg->GetSubject().t_str().c_str()));
////								dbgprint(msgStr);
////							}
////							// END DEBUG
//						}
//					}
//				}
//			}
//		}
//	}

#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnFolderUnreadChanged(folderId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolderManager::OnFolderUnreadChanged: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEFolderManager::OnNewMessage(DWORD folderId, DWORD newMsgId)
{
#ifndef STATIC_LIBRARY
	try {
//		// BEGIN DEBUG
//		{
//			char msgStr[1024];
//			sprintf(msgStr, "OnNewMessage: Folder %x Message %x.\n", folderId,
//				newMsgId);
//			dbgprint(msgStr);
//		}
		// END DEBUG
		connection_point.Fire_OnNewMessage(folderId, newMsgId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEFolderManager::OnNewMessage: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

#if _MSC_VER >= 1400
#pragma warning(pop)
#endif

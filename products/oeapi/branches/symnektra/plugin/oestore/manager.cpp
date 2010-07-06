/* $Id: manager.cpp,v 1.26 2008/10/28 20:27:44 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "oeapi_config.h"
#include "std.h"
#include "message.h"

#ifndef STATIC_LIBRARY
#include <comet/comet.h>
#include <comet/exe_server.h>
#include <comet/cmd_line_parser.h>

#pragma warning(disable : 4800)

typedef com_server< OESTORE::type_library > SERVER;
#endif // STATIC_LIBRARY

///////////////////////////////////////////////////////////////////////////
#include <list>

#include "manager.h"

#include "se_debug.h"
#include "msoeapi.h"
#include "oeapi_utils.h"

#ifdef EVALUATION_VERSION
#include "eval_utils.h"
#endif // EVALUATION_VERSION

// there is a auto-generated oeapi.h in this project, so we must specify the path
#include "../lib/oeapi.h"

//static DWORD serverID = 0;

// HINSTANCE of the dll
//HINSTANCE hInst = NULL;
extern HINSTANCE hInst;


const UINT BUTTON_CLICKED_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnButtonClick"));
const UINT MENU_ITEM_CLICKED_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnMenuItemClick"));
const UINT MESSAGE_SELECTION_CHANGED_CODE = RegisterWindowMessage(_T("OEAPI.OnMessageSelectionChanged"));

INT __cdecl ExitServerCallback();

OEStoreManager *pThis = NULL;

LRESULT CALLBACK StoreWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Create a window
HWND CreateStoreCallbackWindow();


LRESULT CALLBACK DeletedWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
HWND CreateDeletedCallbackWindow()
{
	WNDCLASSEX wndclass;
	const TCHAR szFolderSink[] = _T("OEAPI Outbox sink");
	HWND hWnd;

	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= 0;
	wndclass.lpfnWndProc	= &DeletedWndProc;
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
		debug_print(DEBUG_ERROR, _T("CreateDeletedCallbackWindow: CreateWindow() failed.\n"));
		return NULL;
	}

	return hWnd;
}

//---------------------------------------------------------------------------//
LRESULT CALLBACK DeletedWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IStoreFolder *deletedFolder = (IStoreFolder *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
	DWORD msgId = (DWORD) wParam;
	IMimeMessage *pMimeMsg;
	HRESULT hr;
	PROPVARIANT value;

	// Delete all the messages that were deleted using OEAPI with the flag permanent
	// The DELETE_MSG_HEADER indicates if the flag was set
	if(uMsg == WM_NEWMSGS) {
		if(deletedFolder) {
			// Open it as an IMimeMessage interface
			hr = deletedFolder->OpenMessage(msgId,
											IID_IMimeMessage,
											(LPVOID*)&pMimeMsg);
			if(SUCCEEDED(hr)) {

				value.vt = VT_LPSTR;

				hr = pMimeMsg->GetBodyProp(HBODY_ROOT, DELETE_MSG_HEADER, 0, &value);
				if(SUCCEEDED(hr)) {
					if(hr != S_FALSE) {
						if(!strcmp(value.pszVal, "True")) {
							MESSAGEIDLIST msgIdList;
							DWORD msgIds[] = {msgId};

							msgIdList.cbSize = sizeof(MESSAGEIDLIST);
							msgIdList.cMsgs = 1;
							msgIdList.prgdwMsgId = msgIds;

							hr = deletedFolder->DeleteMessages(&msgIdList, 0, 0);
							if(FAILED(hr)) {
								debug_print(DEBUG_ERROR, _T("DeletedWndProc: DeleteMessages.\n"));
							}

						}

						PropVariantClear(&value);
					}

					pMimeMsg->Release();
				}
			}
//			else {
//				debug_print(DEBUG_ERROR, _T("DeletedWndProc: deletedFolder NULL.\n"));
//			}
		}
		else {
			debug_print(DEBUG_ERROR, _T("DeletedWndProc: deletedFolder NULL.\n"));
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
OEStoreManager::OEStoreManager()
{
	InitializeCriticalSection(&folderCS_);

	pDeletedFolder_ = NULL;
	pSN_ = NULL;
	hDeletedWnd_ = NULL;
	hWnd_ = NULL;
	hThread_ = NULL;
	msgStore_ = NULL;

	Restart();
}

//---------------------------------------------------------------------------//
OEStoreManager::~OEStoreManager()
{
	DestroyObjects();

	DeleteCriticalSection(&folderCS_);
}

//---------------------------------------------------------------------------//
void OEStoreManager::Restart()
{
	DestroyObjects();

	HRESULT hr = CoCreateInstance(CLSID_StoreNamespace, NULL,
		CLSCTX_SERVER, IID_IStoreNamespace, (LPVOID*)&pSN_);
	if(FAILED(hr)) {
		debug_print(DEBUG_INFO, _T("OEStoreManager::OEStoreManager: Error CoCreateInstance\n"));
		return;
	}

//	hr = pSN_->Initialize(NULL,NULL);
	hr = pSN_->Initialize(NULL, 1);
	if(FAILED(hr)) {
		pSN_->Release();
		pSN_ = NULL;
		debug_print(DEBUG_ERROR, _T("OEStoreManager::OEStoreManager: Error Initialize\n"));
		return;
	}

	//FOLDERPROPS props;
	//props.cbSize = sizeof(FOLDERPROPS);

	//IStoreFolder *outbox = NULL;
	//hr = GetSN()->OpenSpecialFolder(FOLDER_OUTBOX, 0, &outbox);
	//if(FAILED(hr)) {
	//	debug_print(DEBUG_ERROR, _T("OEStoreManager::OEStoreManager: Error OpenSpecialFolder Outbox.\n"));
	//	return;
	//}
	//outbox->Release();

	// Create the window that handles the button clicks
	hWnd_ = CreateStoreCallbackWindow();

	hr = pSN_->RegisterNotification(0, hWnd_);
	if(FAILED(hr)) {
		debug_print(DEBUG_INFO, _T("OEStoreManager::OEStoreManager: Error RegisterNotification Store Namespace.\n"));
	}

	// new messages in the deleted folder are deleted if there were deleted using OEAPI
	// and the flag permanent was set (IOEMessage::Delete)
	hr = pSN_->OpenSpecialFolder(FOLDER_DELETED, 0, &pDeletedFolder_);
	if(FAILED(hr)) {
		debug_print(DEBUG_INFO, _T("OEStoreManager::OEStoreManager: Error OpenSpecialFolder FOLDER_DELETED.\n"));
		return;
	}

	hDeletedWnd_ = CreateDeletedCallbackWindow();
	if(hDeletedWnd_) {
		SetWindowLongPtr(hDeletedWnd_, GWLP_USERDATA, (LONG_PTR)pDeletedFolder_);
	}
	else {
		debug_print(DEBUG_INFO, _T("OEStoreManager::OEStoreManager: Error CreateDeletedCallbackWindow.\n"));
		return;
	}

	hr = pDeletedFolder_->RegisterNotification(0, hDeletedWnd_);
	if(FAILED(hr)) {
		debug_print(DEBUG_INFO, _T("OEStoreManager::OEStoreManager: Error RegisterNotification Deleted Folder.\n"));
	}

	if(IsOE()) { // && IsWMail()) {
		HWND hOEWnd = FindWindowRecursive(NULL, OEAPI_OECOM_CALLBACK_CLASS); //, GetCurrentProcessId());
		if(hOEWnd) {
			msgStore_ = (OEAPIMessageStore*)GetProp(hOEWnd, _T("NKTHANDLE"));
		}
		if(!msgStore_) {
			debug_print(DEBUG_INFO, _T("+++++++OEStoreManager::Restart: GetMessageStore failed!!!!\n"));
		}
		else {
			debug_print(DEBUG_INFO, _T("+++++++OEStoreManager::Restart: GetMessageStore success!!!!\n"));
		}
	}
}

//---------------------------------------------------------------------------//
void OEStoreManager::DestroyObjects()
{
	HRESULT hr;

	msgStore_ = NULL;

	if(pSN_) {
		hr = pSN_->UnregisterNotification(0, hWnd_);
		if(FAILED(hr)) {
			debug_print(DEBUG_INFO, _T("OEStoreManager::~OEStoreManager: Error UnregisterNotification Store Namespace.\n"));
		}

		if(hWnd_) {
			DestroyWindow(hWnd_);
			hWnd_ = NULL;
		}

		pSN_->Release();
		pSN_ = NULL;
	}

//	UnhookFolder();

	if(pDeletedFolder_) {
		hr = pDeletedFolder_->UnregisterNotification(0, hDeletedWnd_);
		if(FAILED(hr)) {
			debug_print(DEBUG_INFO, _T("OEStoreManager::~OEStoreManager: Error UnregisterNotification Deleted Folder.\n"));
		}

		if(hDeletedWnd_) {
			DestroyWindow(hDeletedWnd_);
			hDeletedWnd_ = NULL;
		}

		pDeletedFolder_->Release();

		pDeletedFolder_ = NULL;
	}
}

//---------------------------------------------------------------------------//
void OEStoreManager::ActivateGlobalNotification()
{
	if(hThread_ == NULL) {
		if((hThread_ = CreateThread(0, 0, ActivateGlobalNotificationThread, this, 0, &dwThreadId_)) == NULL) {
			debug_print(DEBUG_INFO, _T("OEStoreManager::OEStoreManager: CreateThread failed.\n"));
		}
	}
}

//---------------------------------------------------------------------------//
DWORD OEStoreManager::ActivateGlobalNotificationThread(LPVOID lpParameter)
{
	CoInitialize(0);

	while(Get(false) == NULL) {
		Sleep(0);
	}

	DWORD maxId;
	TOEFolderPtr folder;
	TOEFolderManagerPtr fm = TOEFolderManager::newInstance();
	OEStoreManager *pThis = (OEStoreManager *) lpParameter;
	int reps = 0;

	FolderIdMessageIdMap &maxMsgIds = pThis->GetMaxMsgIDsMap();

	maxMsgIds.clear();

	folder = fm->GetFirstFolder();

	while(!folder.is_null()) {

		maxId = pThis->GetMaxMsgID(folder);

		if(maxId != -1) {
			maxMsgIds[folder->GetID()] = maxId;
		}

		folder = fm->GetNextFolder();

		if(reps++ == 100) {
			reps = 0;
			Sleep(10);
		}
	}

	CoUninitialize();

	return 0;
}

//---------------------------------------------------------------------------//
DWORD OEStoreManager::GetMaxMsgID(TOEFolderPtr folderPtr)
{
	DWORD maxId = -1;
	TOEFolder *folder;
	HENUMSTORE hEnum = HENUMSTORE_INVALID;
	HRESULT hr;
	NktMessageProps* msgProps = NULL;

	folder = (TOEFolder *) folderPtr.get();

	if(folder->GetSF() != NULL) {
		msgProps = NktMessageProps::Create(folder->GetSF());

		hr = msgProps->GetFirstMessage(MSGPROPS_FAST, MESSAGEID_FIRST, &hEnum);

		while(SUCCEEDED(hr) && hr != S_FALSE) {
			maxId = msgProps->GetID();

			hr = msgProps->GetNextMessage(hEnum, MSGPROPS_FAST);
		}

		if(hEnum != HENUMSTORE_INVALID) {
			folder->GetSF()->GetMessageClose(hEnum);
		}
	}

	if(msgProps) {
		delete msgProps;
	}

	return maxId;
}
//
////---------------------------------------------------------------------------//
//void OEStoreManager::OnHookedFolderEvent(DWORD uMsg, LPARAM lParam, WPARAM wParam)
//{
//	switch(uMsg) {
//		case WM_NEWMSGS:
//		{
//			MESSAGEPROPS props;
//			HRESULT hr;
//
//			props.cbSize = sizeof(MESSAGEPROPS);
//
//			hr = hookedFolder_->GetMessageProps(wParam, MSGPROPS_FAST, &props);
//			if(SUCCEEDED(hr)) {
//				// notify only if it is a unread message to keep compatibility
//				if(props.dwState & comet::OESTORE::OE_MSG_UNREAD) {
//					RaiseOnNewMessageEvent(hookedFolder_, wParam);
//				}
//
//				hookedFolder_->FreeMessageProps(&props);
//			}
//
//			break;
//		}
//		case WM_MARKEDASREAD:
//		case WM_MARKEDASUNREAD:
//		case WM_DELETEMSGS:
//		case WM_DELETEFOLDER:
//		{
//			break;
//		}
//	}
//
//}

//---------------------------------------------------------------------------//
void OEStoreManager::OnFolderUnreadChanged(DWORD folderId)
{
	TOEFolderPtr folderPtr;
	DWORD maxMsgId, oldMaxMsgId = -1;
	BOOL newMsg = FALSE;
	TOEMessagePtr msg;
	TOEFolder *pFolder;
	TOEFolderManagerPtr fm = TOEFolderManager::newInstance();

	if(hThread_) {
		WaitForSingleObject(hThread_, INFINITE);
	}

	folderPtr = fm->GetFolder(folderId);

	if(!folderPtr.is_null()) {
		pFolder = (TOEFolder *) folderPtr.get();

		// if it is a special folder different from Inbox do not notify
		if(!pFolder->IsDeleted() && !pFolder->IsOutbox() && !pFolder->IsSent()
			&& !pFolder->IsDraft()) {
			maxMsgId = GetMaxMsgID(folderPtr);

			if(maxMsgId != -1) {
				// if the folder is not in the list, it has no messages, so any message is new
				// otherwise, the new max id must be greater that the old max
				if(maxMsgIds_.find(folderId) != maxMsgIds_.end()) {
					oldMaxMsgId = maxMsgIds_[folderId];

					if(oldMaxMsgId < maxMsgId) {
						newMsg = TRUE;
					}
				}
				else {
					newMsg = TRUE;
				}

				if(newMsg) {
					maxMsgIds_[folderId] = maxMsgId;

					if(oldMaxMsgId == -1) {
						oldMaxMsgId = maxMsgId-1;
					}

					// NOTE: new messages can be notified in any order so if OE is receiving
					// more than 1 message, it could notify first the second and then the
					// first. In that situation, the first message won't be notified by
					// OEAPI if I just notify the last one. To fix this, I notify all the
					// messages between the oldMaxMsgId and the last one.
					// Then, notifications of the previous messages won't enter here because
					// maxMsgIds_[folderId] > msgId
					for(DWORD i=oldMaxMsgId+1; i<=maxMsgId; i++) {
						// notify only if it is a unread message
						msg = pFolder->GetMessage(i);
						if(!msg.is_null() && (msg->GetState() & comet::OESTORE::OE_MSG_UNREAD)) {
							RaiseOnNewMessageEvent(folderId, i);
						}
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------//
void OEStoreManager::AddFolder(TOEFolder *folder)
{
	EnterCriticalSection(&folderCS_);
	folders_.push_back(folder);
	LeaveCriticalSection(&folderCS_);
}

//---------------------------------------------------------------------------//
void OEStoreManager::RemoveFolder(TOEFolder *folder)
{
	EnterCriticalSection(&folderCS_);
	folders_.remove(folder);
	LeaveCriticalSection(&folderCS_);

	// don't kill manager from the ActivateGlobalNotification thread
	if(GetCurrentThreadId() != dwThreadId_) {
		if(hThread_) {
			WaitForSingleObject(hThread_, INFINITE);
		}

		// we don't have to lock here because if we are at this point the other thread is
		// dead
		if(folders_.size() == 0 && folderManagers_.size() == 0) {
			// TODO: Check under which condition this are triggered
			delete pThis;
			pThis = NULL;
		}
	}
}

//---------------------------------------------------------------------------//
void OEStoreManager::AddFolderManager(TOEFolderManager *folderMan)
{
	EnterCriticalSection(&folderCS_);
	folderManagers_.push_back(folderMan);
	LeaveCriticalSection(&folderCS_);
}

//---------------------------------------------------------------------------//
void OEStoreManager::RemoveFolderManager(TOEFolderManager *folderMan)
{
	EnterCriticalSection(&folderCS_);
	folderManagers_.remove(folderMan);
	LeaveCriticalSection(&folderCS_);

	// don't kill manager from the ActivateGlobalNotification thread
	if(GetCurrentThreadId() != dwThreadId_) {
		if(hThread_) {
			WaitForSingleObject(hThread_, INFINITE);
		}

		// we don't have to lock here because if we are at this point the other thread is
		// dead
		if(folders_.size() == 0 && folderManagers_.size() == 0) {
			// TODO: Check under which conditions this are triggered
			delete pThis;
			pThis = NULL;
		}
	}
}
//
////---------------------------------------------------------------------------//
//void OEStoreManager::HookFolder(DWORD folderId)
//{
//	UnhookFolder();
//
//	HRESULT hr = pSN_->OpenFolder(folderId, 0, &hookedFolder_);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEStoreManager::HookFolder: Cannot open folder %d.\n"), folderId);
//		return;
//	}
//
//	hr = hookedFolder_->RegisterNotification(0, hWnd_);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEStoreManager::HookFolder: Error RegisterNotification folder %d.\n"), folderId);
//		hookedFolder_ = NULL;
//		hookedFolderId_ = -1;
//		return;
//	}
//}
//
////---------------------------------------------------------------------------//
//void OEStoreManager::UnhookFolder()
//{
//	if(hookedFolder_) {
//		hookedFolder_->UnregisterNotification(0, hWnd_);
//		hookedFolder_ = NULL;
//		hookedFolderId_ = -1;
//	}
//}

//---------------------------------------------------------------------------//
void OEStoreManager::RaiseOnNewMessageEvent(DWORD folderId, DWORD newMsgId)
{
	EnterCriticalSection(&folderCS_);
	for(FolderManagerList::iterator it=folderManagers_.begin(); it!=folderManagers_.end(); it++) {
		TOEFolderManager *fm = *it;

		fm->OnNewMessage(folderId, newMsgId);
	}
	LeaveCriticalSection(&folderCS_);
}

//---------------------------------------------------------------------------//
void OEStoreManager::RaiseOnFolderManagerEvent(LPFOLDERNOTIFYEX folderNotify)
{
	EnterCriticalSection(&folderCS_);
	for(FolderManagerList::iterator it=folderManagers_.begin(); it!=folderManagers_.end(); it++) {
		TOEFolderManager *fm = *it;

		fm->OnEvent(folderNotify);
	}
	LeaveCriticalSection(&folderCS_);

#ifndef DISCONNECT_GLOBAL_NOTIFICATION
	if(folderNotify->type == UNREAD_CHANGE) {
		OnFolderUnreadChanged(folderNotify->idFolderNew);
	}
#endif // DISCONNECT_GLOBAL_NOTIFICATION
}

//---------------------------------------------------------------------------//
//LRESULT CALLBACK StoreWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch(uMsg) {
//		// from this event cannot generate COM events because the thread is marked as
//		// user-input. To finally generate the event a message must be posted.
//		case WM_FOLDERNOTIFY:
//		{
//			LPFOLDERNOTIFYEX folderNot;
//
//			if(IsWMail()) {
//				folderNot = (LPFOLDERNOTIFYEX) CoTaskMemAlloc(sizeof(FOLDERNOTIFYEX));
//
//				if(CopyFromWinMail(folderNot, (const void *) lParam, sizeof(FOLDERNOTIFYEX))) {
//					if(folderNot->type != FOLDER_PROPS_CHANGED) {
//						OEStoreManager::Get()->HookFolder(folderNot->idFolderNew);
//						PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, wParam, (LPARAM) folderNot);
//					}
//				}
//				else {
//					debug_print(DEBUG_ERROR, _T("StoreWndProc: CopyFromWinMail Error.\n"));
//				}
//
//			}
//			else {
//				folderNot =  (LPFOLDERNOTIFYEX) lParam;
////				if(folderNot->type != FOLDER_PROPS_CHANGED) {
//					OEStoreManager::Get()->HookFolder(folderNot->idFolderNew);
//					PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, wParam, lParam);
////				}
//			}
//			break;
//		}
//		case WM_FOLDERNOTIFY+OEAPI_MSG_BASE:
//		{
//			LPFOLDERNOTIFYEX folderNot = (LPFOLDERNOTIFYEX) lParam;
//			OEStoreManager::Get()->RaiseOnFolderManagerEvent(folderNot);
//
//			CoTaskMemFree((LPVOID) lParam);
//			break;
//		}
//
//		case WM_NEWMSGS:
//		case WM_MARKEDASREAD:
//		case WM_MARKEDASUNREAD:
//		case WM_DELETEMSGS:
//		case WM_DELETEFOLDER:
//		{
//			OEStoreManager::Get()->OnHookedFolderEvent(uMsg, wParam, lParam);
//			break;
//		}
//	}
//
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
LRESULT CALLBACK StoreWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// from this event cannot generate COM events because the thread is marked as
	// user-input. To finally generate the event a message must be posted.
	if(uMsg == WM_FOLDERNOTIFY) {
		LPFOLDERNOTIFYEX folderNot;

//		if(IsWMail()) {
//			folderNot = (LPFOLDERNOTIFYEX) CoTaskMemAlloc(sizeof(FOLDERNOTIFYEX));
//
//			if(CopyFromWinMail(folderNot, (const void *) lParam, sizeof(FOLDERNOTIFYEX))) {
//				if(folderNot->type != FOLDER_PROPS_CHANGED) {
//					PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, wParam, (LPARAM) folderNot);
//				}
//			}
//			else {
//				debug_print(DEBUG_ERROR, _T("StoreWndProc: CopyFromWinMail Error.\n"));
//			}
//		}
//		else {
			folderNot =  (LPFOLDERNOTIFYEX) lParam;
			if(folderNot->type != FOLDER_PROPS_CHANGED) {
//				if(folderNot->type == DELETE_FOLDER) {
//					printf("");
//				}
				PostMessage(hwnd, uMsg+OEAPI_MSG_BASE, wParam, lParam);
			}
			else {
				CoTaskMemFree((LPVOID) lParam);
			}
// 		}
	}
	else if(uMsg == WM_FOLDERNOTIFY+OEAPI_MSG_BASE) {
		LPFOLDERNOTIFYEX folderNot = (LPFOLDERNOTIFYEX) lParam;
		OEStoreManager::Get()->RaiseOnFolderManagerEvent(folderNot);

		CoTaskMemFree((LPVOID) lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//---------------------------------------------------------------------------//
HWND CreateStoreCallbackWindow()
{
	WNDCLASSEX wndclass;
	const TCHAR szStoreSink[] = OEAPI_OESTORE_CALLBACK_CLASS;

	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= 0;
	wndclass.lpfnWndProc	= &StoreWndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInst;
	wndclass.hIcon			= NULL;
	wndclass.hCursor		= NULL;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szStoreSink;
	wndclass.hIconSm		= NULL;

	// Register it
	RegisterClassEx(&wndclass);

	// And create a window
	HWND hWnd = CreateWindow(szStoreSink,
				OEAPI_OESTORE_CALLBACK_NAME,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				400, 200,
				NULL,
				NULL,
				hInst,
				NULL);

	if (hWnd == NULL) {
		debug_print(DEBUG_ERROR, _T("CreateStoreCallbackWindow: CreateWindow() failed.\n"));
		return NULL;
	}

	return hWnd;
}

//---------------------------------------------------------------------------//
BOOL OEStoreManager::OpenFolder(INT id, INT specialId, IStoreFolder* pSF, TOEFolder* folder)
{
	if(id != 0)	// FOLDERID_ROOT
	{
		HRESULT hr;

		if(pSF == NULL) {
			hr = GetSN()->OpenFolder(id, 0, &folder->pSF_);
			if(FAILED(hr)) {
				folder->pSF_ = NULL;
				debug_print(DEBUG_ERROR, _T("OEStoreManager::OpenFolder: OpenFolder %08x.\n"), hr);
				return FALSE;
			}
		}
		else {
			hr = S_OK;
			folder->pSF_ = pSF;
			folder->pSF_->AddRef();
		}

		std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(folder->pSF_));

		hr = props->GetFolderProps();
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEStoreManager::OpenFolder: GetFolderProps %08x.\n"), hr);
			return FALSE;
		}

		folder->id_ = id;
		folder->specialId_ = props->GetSpecialID();

		OpenStorageFolder(folder->id_, &folder->msgFolder_);		
		//if(msgStore_) {
		//	msgStore_->OpenFolder((FOLDERID)folder->id_, &folder->msgFolder_);
		//}
	}
	else
	{
		HRESULT hr = GetSN()->OpenSpecialFolder((SPECIALFOLDER) specialId, 0, &folder->pSF_);
		if(FAILED(hr)) {
			folder->pSF_ = NULL;
			debug_print(DEBUG_ERROR, _T("OEStoreManager::OpenFolder: OpenSpecialFolder %08x.\n"), hr);
			return FALSE;
		}
		
		std::auto_ptr<NktFolderProps> props(NktFolderProps::Create(folder->pSF_));

		hr = props->GetFolderProps();
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEStoreManager::OpenFolder: GetFolderProps %08x.\n"), hr);
			return FALSE;
		}

		folder->id_ = props->GetID();
		folder->specialId_ = specialId;

		OpenStorageFolder(folder->id_, &folder->msgFolder_);
		//if(msgStore_) {
		//	msgStore_->OpenFolder((FOLDERID)folder->id_, &folder->msgFolder_);
		//}
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL OEStoreManager::OpenStorageFolder(INT folderId, IMessageFolder** pFolder)
{
	HRESULT hr = E_FAIL;

	if(msgStore_) {
		hr = msgStore_->OpenFolder((FOLDERID)folderId, pFolder);
	}

	return SUCCEEDED(hr);
}

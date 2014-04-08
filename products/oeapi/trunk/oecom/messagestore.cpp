/* $Id: messagestore.cpp,v 1.36 2008/11/16 19:39:06 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"

#include <initguid.h>
#include "oeundoc.h"
#include "messagestore.h"

#include "oeapi_utils.h"

#include <algorithm>

#define OEAPI_FOLDER_EVENT 0x1111111
#define OEAPI_REGISTER_FOLDER WM_USER + 0x1010

//////////////////////////////////////////////////////////////////////////
//typedef NktHookedIUnknown<IMessageStore> HookedIMessageStore;
//typedef NktHookedIUnknown<IMessageStoreWMail> HookedIMessageStoreWM;

//HookedIMessageStore hookIMessageStore;
//HookedIMessageStoreWM hookIMessageStoreWM;

//----------------------------------------------------------------------------
class NktNullStoreCallback : public IStoreCallback
{
public:
	// NktNullStoreCallback() {}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject) 
	{
		return S_OK; 
	}
	virtual ULONG STDMETHODCALLTYPE AddRef( void) 
	{
		return S_OK;
	}
	virtual ULONG STDMETHODCALLTYPE Release( void) 
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE OnBegin(tagSTOREOPERATIONTYPE,tagSTOREOPERATIONINFO *,IOperationCancel *)	
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE OnProgress(tagSTOREOPERATIONTYPE,ULONG,ULONG,char const *) 
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE OnTimeout(INETSERVER *,ULONG *,tagIXPTYPE) 
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE CanConnect(char const *,ULONG) 
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE OnLogonPrompt(INETSERVER *,tagIXPTYPE) 
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE OnComplete(tagSTOREOPERATIONTYPE,long,tagSTOREOPERATIONINFO *,tagSTOREERROR *) 
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE OnPrompt(long,char const *,char const *,UINT,int *) 
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE GetParentWindow(ULONG,HWND__ * *)
	{
		return S_OK;
	}
};

NktNullStoreCallback nullStoreCallback;

#include <initguid.h>
DEFINE_GUID(CLSID_MessageDatabase, 0x06F74FDC6, 0xE366, 0x11d1, 0x9A, 0x4E, 0x0, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);
//{40FC6ED3-2438-11CF-A3DB-080036F12502}
//{06BE7323-EF34-11d1-ACD8-00C04FA31009}
//{6F74FDC5-E366-11d1-9A4E-00C04FA309D4}
//{6F74FDC6-E366-11d1-9A4E-00C04FA309D4}


DEFINE_GUID(IID_IMessageStore2, 0x7157F0AE, 0x967B, 0x11D1, 0x9A, 0x08, 0x00, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);
DEFINE_GUID(IID_IMessageStore3, 0x6B966885, 0xD30E, 0x4ADB, 0xB8, 0x5A, 0xE3, 0xDF, 0xF5, 0xCE, 0x01, 0x4E);
DEFINE_GUID(IID_IMessageStore4, 0xB58A0F80, 0x0702, 0x11D2, 0x9A, 0x63, 0x00, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);


OEAPIMessageStore* OEAPIMessageStore::_OEAPIMessageStore = NULL;


//////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
class DatabaseNotifierTrampoline : public IDatabaseNotify 
{
public:
	DatabaseNotifierTrampoline() : _lock(true), _listener(NULL) {}
	~DatabaseNotifierTrampoline() {}

	void SetListener(IDatabaseNotify* listener) { _lock.Enter(); _listener = listener; _lock.Leave(); }
	IDatabaseNotify* GetListener() { return _listener; }

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject) 
	{
		return S_OK; 
	}
	virtual ULONG STDMETHODCALLTYPE AddRef( void) 
	{
		return S_OK;
	}
	virtual ULONG STDMETHODCALLTYPE Release( void) 
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnTransaction(HTRANSACTION__ *hTransaction, ULONG dwUser, IDatabase *database)
	{
		HRESULT hr = E_FAIL;
		_lock.Enter();
		if(_listener) {
			hr = _listener->OnTransaction(hTransaction, dwUser, database);
		}
		_lock.Leave();
		return hr;
	}

private:
	IDatabaseNotify* _listener;
	NktLock _lock;
};

DatabaseNotifierTrampoline tramp; 


//////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
OEAPIMessageStore* OEAPIMessageStore::CreateMessageStore()
{
	OEAPIMessageStore* msgStore = NULL;
	if(IsWMail()) {
		msgStore = new OEAPIMessageStoreWM();
	}
	else {
		msgStore = new OEAPIMessageStoreOE();
	}
	return msgStore;
}

//----------------------------------------------------------------------------
OEAPIMessageStore::OEAPIMessageStore()
{
	debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OEAPIMessageStore: Enter.\n"));
	_listener = NULL;
	_registered = FALSE;
	_folderLock.Init();

	if(!_OEAPIMessageStore) {
		_OEAPIMessageStore = this;
	}

	tramp.SetListener(this);
	debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OEAPIMessageStore: Leave.\n"));
}

//----------------------------------------------------------------------------
OEAPIMessageStore::~OEAPIMessageStore()
{
	debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::~OEAPIMessageStore: Enter.\n"));
	tramp.SetListener(NULL);
	if(_OEAPIMessageStore == this) {
		_OEAPIMessageStore = NULL;
	}

	_folderLock.Term();
	debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::~OEAPIMessageStore: Leave.\n"));
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::QueryInterface(const IID &iid, void **ppv)
{
	if(iid == IID_IUnknown) {
		*ppv = this;
	}
	else if(iid == IID_IDatabaseNotify ) {
		*ppv = (IDatabaseNotify*) this;
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
    return S_OK;
}

//----------------------------------------------------------------------------
void OEAPIMessageStore::Init()
{
}

//----------------------------------------------------------------------------
void OEAPIMessageStore::Close()
{
	//UnregisterAllFoldersNotification();
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::OpenFolder(NktFOLDERID folderId, IMessageFolder **ppfInfo)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::FreeRecord(void *ptr)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::GetEnumChildren(NktFOLDERID folderId, IEnumerateFolders **ppEnum)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::GetNextFolderId(IEnumerateFolders *pEnum, ULONG next, NktFOLDERID *folderId, DWORD *pcount)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::GetFolderType(NktFOLDERID folderId, FolderType *folderType)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::GetParentId(NktFOLDERID folderId, NktFOLDERID* parentId)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
BOOL OEAPIMessageStore::MapFolderType(BYTE type, FolderType *folderType)
{
	switch(type) {
		case 0:
			*folderType = folder_newsgroup;
			break;
		case 1:
			*folderType = folder_online_mail;
			break;
		case 3:
			*folderType = folder_local_mail;
			break;
		default:
			*folderType = folder_unknown;
			return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::RegisterMessageNotification(NktFOLDERID folderId)
{
	HRESULT hr;
	::SendMessage(_hwndCallback, OEAPI_REGISTER_FOLDER, (WPARAM)folderId, (LPARAM)&hr);
	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::InternalRegisterMessageNotification(NktFOLDERID folderId)
{
	if(!_registered) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::RegisterMessageNotification: Registration is not enabled %d.\n"), folderId);
		return E_FAIL;
	}

	VOID* param = NULL;
	HRESULT hr;

	NktAutoLock lock(_folderLock);

	OEAPIFolderList::iterator it = _folders.find(folderId);
	if(it != _folders.end()) {
		//debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::RegisterMessageNotification: already %d.\n"), folderId);
		// Already registered
		// Update LRU list
		OEAPIFolderLRUList::iterator it2;
		it2 = std::find(_foldersLRU.begin(), _foldersLRU.end(), folderId);
		if(it2 != _foldersLRU.end()) {
			_foldersLRU.erase(it2);
			_foldersLRU.push_back(folderId);
		}
		else {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::RegisterMessageNotification: Can't update LRU list %d.\n"), folderId);
		}
		return S_OK;
	}

	//debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::RegisterMessageNotification: registering %d th: %d.\n"), folderId, ::GetCurrentThreadId());

	if(_folders.size() > 30) {
		// Remove LRU
		for(int i=0; i < 5; i++) {
			NktFOLDERID oldId = _foldersLRU.front();
			//_foldersLRU.pop_front();
			UnregisterMessageNotification(oldId);
		}
	}

	hr = RegisterMessageNotification(folderId, &tramp, &param);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::RegisterMessageNotification: Can't register message notification %d : %08x\n."), folderId, hr);
		return hr;
	}

	_folders[folderId] = param;
	_foldersLRU.push_back(folderId);

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::UnregisterMessageNotification(NktFOLDERID folderId)
{
	OEAPIFolderList::iterator it;
	//std::map<DWORD, VOID*>::iterator it;

	NktAutoLock lock(_folderLock);

	it = _folders.find(folderId);
	if(it != _folders.end())
	{
		//debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::UnregisterMessageNotification: Unregister folder id %d.\n"), folderId);
		VOID* param = it->second;
		HRESULT hr = UnregisterMessageNotification(&tramp, param);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::UnregisterMessageNotification: Unregister failed %d %08x.\n"), folderId, hr);
		}
		OEAPIFolderLRUList::iterator it2 = std::find(_foldersLRU.begin(), _foldersLRU.end(), folderId);
		if(it2 != _foldersLRU.end()) {
			_foldersLRU.erase(it2);
		}
		else {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::UnregisterMessageNotification: Can't update LRU list %d.\n"), folderId);
		}
		_folders.erase(it);
		return hr;
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::UnregisterMessageNotification: Can't find folder id %d.\n"), folderId);
		return E_FAIL;
	}
}


//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::RegisterMessageNotification(NktFOLDERID folderId, IDatabaseNotify* listener, VOID** param)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::UnregisterMessageNotification(IDatabaseNotify* listener, VOID* param)
{
	return E_NOTIMPL;
}


//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::RegisterFoldersNotification(IDatabaseNotify* listener)
{
	return E_NOTIMPL;
}

//------------------------------------------------------------------//
HRESULT OEAPIMessageStore::UnregisterFoldersNotification(IDatabaseNotify* listener)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
void OEAPIMessageStore::RegisterAllFoldersNotification()
{
	if(!_registered) {
		_registered = TRUE;
		_hwndCallback = CreateStoreCallbackWindow();
		RegisterFoldersNotification(&tramp);
	}
	//RegisterChildrenFoldersNotification(1, this);
}

//------------------------------------------------------------------//
void OEAPIMessageStore::UnregisterAllFoldersNotification()
{
	if(_registered) {
		_registered = FALSE;
		if(_hwndCallback) {
			::DestroyWindow(_hwndCallback);
		}
		UnregisterChildrenFoldersNotification(&tramp);
		UnregisterFoldersNotification(&tramp);
	}
}


//----------------------------------------------------------------------------
void OEAPIMessageStore::RegisterChildrenFoldersNotification(NktFOLDERID folderId, IDatabaseNotify* listener)
{
	HRESULT hr;
	IEnumerateFoldersPtr enumFolders;
	NktFOLDERID childId;
	VOID* param;

	hr = GetEnumChildren(folderId, &enumFolders);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::RegisterChildrenFoldersNotification: Can't get enumeration %08x.\n"), hr);
		return;
	}

	hr = GetNextFolderId(enumFolders, 1, &childId);

	while(hr == S_OK) {
		param = NULL;
		hr = RegisterMessageNotification(childId);
		if(SUCCEEDED(hr)) {
			RegisterChildrenFoldersNotification(childId, listener);
		}
		hr = GetNextFolderId(enumFolders, 1, &childId);
	}

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::RegisterChildrenFoldersNotification: Enumeration failed %08x.\n"), hr);
	}
}

//----------------------------------------------------------------------------
void OEAPIMessageStore::UnregisterChildrenFoldersNotification(IDatabaseNotify* listener)
{
	NktAutoLock lock(_folderLock);

	OEAPIFolderList::iterator it = _folders.begin();
	OEAPIFolderList newFolders;
	while(it != _folders.end())
	{
		NktFOLDERID folderId = it->first;
		VOID* param = it->second;
		HRESULT hr = UnregisterMessageNotification(&tramp, param);
		if(FAILED(hr)) {
			debug_print(DEBUG_TRACE, _T(">> %ld -> %08x : %08x - %08x .\n"), folderId, param, (IDatabaseNotify*)&tramp, hr);
			newFolders[it->first] = it->second;
		}
		it++;
	}
	_folders = newFolders; 
}

//
//----------------------------------------------------------------------------
void OEAPIMessageStore::MapTransactionType(BOOL isFolder, TRANSACTIONTYPE transType, TransactionType *tt)
{
	DWORD ret;
	ret = oeapi_tx_invalid;

	if(isFolder) {
		ret = oeapi_tx_folder;
	}
	else {
		ret = oeapi_tx_message;
	}

	switch(transType) {
		case tr_insertrecord:
		{
			ret |= oeapi_tx_insert;
			break;
		}
		case tr_updaterecord:
		{
			ret |= oeapi_tx_update;
			break;
		}
		case tr_deleterecord:
		{
			ret |= oeapi_tx_delete;
			break;
		}
		default:
		{
			ret = oeapi_tx_invalid;
			break;
		}
	}

	*tt = (TransactionType) ret;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::OpenMimeMessage(NktFOLDERID folderId, NktMESSAGEID msgId, IMimeMessage** ppMimeMsg)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::SaveMimeMessage(NktFOLDERID folderId, ULONG state, IMimeMessage* pMimeMsg, NktMESSAGEID* pNewMsgId)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::DeleteMessage(NktFOLDERID folderId, NktMESSAGEID msgId, BOOL permanent)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
//HRESULT OEAPIMessageStore::GetMessageState(FOLDERID folderId, MESSAGEID msgId, LPDWORD pState)
//{
//	return E_NOTIMPL;
//}

//----------------------------------------------------------------------------
//HRESULT OEAPIMessageStore::SetMessageState(FOLDERID folderId, MESSAGEID msgId, DWORD state)
//{
//	return E_NOTIMPL;
//}

//----------------------------------------------------------------------------
LONG OEAPIMessageStore::GetMaxMessageId(NktFOLDERID folderId)
{
	return -1;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::OnTransaction(HTRANSACTION__ *hTransaction, ULONG dwUser, IDatabase *database)
{
	//debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: st %08x.\n"), dwUser);

	HRESULT result;

	// folder event
	if(dwUser == OEAPI_FOLDER_EVENT) {
		result = NotifyFolderTransaction(hTransaction, dwUser, database);
	}
	else {
		result = NotifyMessageTransaction(hTransaction, dwUser, database);
	}

	//debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: en.\n"));
    return result;
}


//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::NotifyFolderTransaction(HTRANSACTION__ *hTransaction, ULONG dwUser, IDatabase *database)
{
	HRESULT hr;
	OEAPITransInfo transInfo;
	int count = 0;
	debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::NotifyFolderTransaction: beg %08x %08x %08x.\n"), hTransaction, dwUser, database);
	while(hTransaction) {
		hr = GetFolderTransactionInfo(database, &hTransaction, &transInfo);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::OnTransaction: Error GetFolderTransactionInfo %08x.\n"), hr);
			return S_FALSE;
		}

		FolderType ft;
		TransactionType txType;

		// we don't need to get the folder type but we use it to workaround the delete
		hr = GetFolderType(transInfo.folderId1, &ft);
		if(FAILED(hr)) {
			MapFolderType(transInfo.folderType, &ft);

			// WORKAROUND: delete folder notification doesn't arrive so we assume that an
			// if a folder fails to return its type in an update event -> folder deleted
			if(ft == folder_local_mail) {
				transInfo.transType = tr_deleterecord;
			}

	//			debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: GetFolderType folder %d"), dwUser);
	//			return S_OK;
		}

		//debug_print(DEBUG_TRACE, _T(">> --%d, %d, %d, %d--\n"), transInfo.parentId1, transInfo.folderId, transInfo.folderId1, transInfo.folderId2 );

		// Register message notificatios for this folder
		//RegisterMessageNotification(transInfo.folderId1);

		MapTransactionType(TRUE, transInfo.transType, &txType);
		if(txType == oeapi_tx_folder_update && transInfo.parentId1 != transInfo.parentId2) {
			// Folder move 
			txType = oeapi_tx_folder_move;
		}
		else if(txType == oeapi_tx_folder_update && transInfo.rename) {
			txType = oeapi_tx_folder_rename;
		}
		if(txType != oeapi_tx_invalid) {
			if(_listener) {
				_listener->OnDatabaseTransaction(txType, ft, transInfo.folderId1, transInfo.parentId1, 0, transInfo.parentId2);
			}
		}

		switch(transInfo.transType) {
			case tr_insertrecord:
			{
				//RegisterMessageNotification(transInfo.folderId1);
				debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: New folder %d Parent %d.\n"), transInfo.folderId1, transInfo.parentId1);
				break;
			}
			case tr_updaterecord:
			{
				if(transInfo.parentId1 != transInfo.parentId2 && (transInfo.parentId1 + transInfo.parentId2) > 0) {
					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: Folder Moved %d Old Parent %d New Parent %d.\n"), transInfo.folderId1, transInfo.parentId1, transInfo.parentId2);
				}
				else {
					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: Unknown update folder %d Old Parent %d New Parent %d.\n"), transInfo.folderId1, transInfo.parentId1, transInfo.parentId2);
				}
				break;
			}
			case tr_deleterecord:
			{
				UnregisterMessageNotification(transInfo.folderId1);
				debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: Deleted folder %d: parent id=%d.\n"), transInfo.folderId1, transInfo.parentId1);
				break;
			}
			default:
			{
				debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: unknown transaction type %d.\n"), transInfo.transType);
				break;
			}
		}
		++count;
		break;
	}
	debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::NotifyFolderTransaction: end %08x %08x %08x [%d].\n"), hTransaction, dwUser, database, count);
	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStore::NotifyMessageTransaction(HTRANSACTION__ *hTransaction, ULONG dwUser, IDatabase *database)
{
	HRESULT hr;
	OEAPITransInfo transInfo;

	while(hTransaction) {
		hr = GetMessageTransactionInfo(database, &hTransaction, &transInfo);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::OnTransaction: Error GetFolderTransactionInfo %08x.\n"), hr);
			return S_FALSE;
		}
		//if(transInfo.transType == tr_updaterecord) {
		//	debug_print(DEBUG_TRACE, _T("...........1:%08x, 2:%08x ->\n"), transInfo.arf1, transInfo.arf2);
		//}

		FolderType ft;
		TransactionType txType;
		NktFOLDERID folderId = (transInfo.folderId == -1) ? dwUser : transInfo.folderId;

		hr = GetFolderType(folderId, &ft);
		if(FAILED(hr)) {
			//debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: GetFolderType folder %d.\n"), folderId);
			return S_OK;
		}
		
		MapTransactionType(FALSE, transInfo.transType, &txType);
		
		// TODO: Merge into MapTransactionType
		if(transInfo.transType == tr_updaterecord) {
			if(IsWMail()) {
				if((transInfo.arf1 & 0x80)) {
					txType = oeapi_tx_message_read;
				} else if(!(transInfo.arf1 & 0x80)) {
					txType = oeapi_tx_message_unread;
				}
			}
			else {
				if((transInfo.arf1 & 0x80) && !(transInfo.arf2 & 0x80)) {
					txType = oeapi_tx_message_unread;
				} else if(!(transInfo.arf1 & 0x80) && (transInfo.arf2 & 0x80)) {
					txType = oeapi_tx_message_read;
				}
			}
		}


		if(txType != oeapi_tx_invalid) {
			if(_listener) {
				_listener->OnDatabaseTransaction(txType, ft, folderId, 
					transInfo.messageId1, transInfo.arf1, 
					transInfo.messageId2, transInfo.arf2);
			}
		}

		if(SUCCEEDED(hr)) {
			switch(transInfo.transType) {
				case tr_insertrecord:
				{
					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: New Message in folder %d: id=%d.\n"), folderId, transInfo.messageId1);
					break;
				}
				case tr_updaterecord:
				{
					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: Message updated in folder %d: id=%d arf=0x%08X/0x%08X.\n"), folderId, transInfo.messageId1, transInfo.arf1, transInfo.arf2);
					break;
				}
				case tr_deleterecord:
				{
					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: Deleted Message in folder %d: id=%d.\n"), folderId, transInfo.messageId1);
					break;
				}
//				case tr_rebuildindex:
//				{
//					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: index rebuilt"));
//					break;
//				}
//				case tr_deleteindex:
//				{
//					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: index deleted"));
//					break;
//				}
//				case tr_basecompacted:
//				{
//					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: base compacted"));
//					break;
//				}
				default:
				{
					debug_print(DEBUG_TRACE, _T("OEAPIMessageStore::OnTransaction: unknown transaction type %d.\n"), transInfo.transType);
					break;
				}
			}
		}
	}
	return S_OK;
}

//---------------------------------------------------------------------------//
LRESULT CALLBACK OEAPIMessageStore::OeapiMsgStoreWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == OEAPI_REGISTER_FOLDER) {
		HRESULT hr;
		hr = OEAPIMessageStore::Get()->InternalRegisterMessageNotification((DWORD)wParam);
		if(lParam) {
			*(HRESULT*)lParam = hr;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//---------------------------------------------------------------------------//
HWND OEAPIMessageStore::CreateStoreCallbackWindow()
{
	WNDCLASSEX wndclass;
	const TCHAR szStoreSink[] = _T("OEAPIMessageStoreCallback");

	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= 0;
	wndclass.lpfnWndProc	= &OeapiMsgStoreWndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= NULL;
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
				_T("OEAPIMessageStoreCallback Wnd"),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				400, 200,
				NULL,
				NULL,
				NULL,
				NULL);

	if (hWnd == NULL) {
		debug_print(DEBUG_ERROR, _T("CreateStoreCallbackWindow: CreateWindow() failed.\n"));
		return NULL;
	}

	return hWnd;
}


//////////////////////////////////////////////////////////////////////////
//
NktOpenFolderOE OEAPIMessageStoreOE::_oldOpenFolder = NULL;
NktAddRefFunction OEAPIMessageStoreOE::_oldAddRef = NULL;
IMessageStore* OEAPIMessageStoreOE::_msgStoreStatic = NULL;

//----------------------------------------------------------------------------
OEAPIMessageStoreOE::OEAPIMessageStoreOE()
{
}

OEAPIMessageStoreOE::~OEAPIMessageStoreOE()
{
	Close();
	_msgStoreStatic = NULL;
}

//----------------------------------------------------------------------------
void OEAPIMessageStoreOE::Init()
{
	OEAPIMessageStore::Init();

	// hookIMessageStore.Init(CLSID_MessageStore, IID_IMessageStore);

	IUnknown *pUnknown = NULL;
	HRESULT hr = E_FAIL;
	hr = ::CoCreateInstance(CLSID_MessageStore, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnknown);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::Init: CoCreateInstance failed %08x.\n"), hr);
		return;
	}

	IMessageStore* msgStore = NULL;
	hr = pUnknown->QueryInterface(IID_IMessageStore, (LPVOID *)&msgStore);
	pUnknown->Release();

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::Init: QueryInterface failed %08x.\n"), hr);
		return;
	}

	if(msgStore) {
		_oldAddRef = _hookAddRef.Init((NktIMessageStore*)msgStore, AddRefHook, &NktIMessageStoreVtbl::AddRef);
		msgStore->Release();
	}
}

//----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE 
OEAPIMessageStoreOE::AddRefHook(NktIUnknown *This)
{
	if(_msgStoreStatic == NULL) {
		_msgStoreStatic = (IMessageStore*)This;
	}
	return _oldAddRef(This);
}

//----------------------------------------------------------------------------
void OEAPIMessageStoreOE::Close()
{
	_hookOpenFolder.Uninit();
	_hookAddRef.Uninit();

	// hookIMessageStore.Uninit();
	OEAPIMessageStore::Close();
}

//----------------------------------------------------------------------------
bool OEAPIMessageStoreOE::CheckStore()
{
	HRESULT hr;
	IMessageStore* msgStore = NULL;

	hr = GetMessageStore((void**)&msgStore);

	if(SUCCEEDED(hr) && msgStore != NULL) {
		//_oldOpenFolder = _hookOpenFolder.Init((NktIMessageStore*)msgStore, (NktOpenFolderOE)OpenFolderHook, &NktIMessageStoreVtbl::OpenFolder);
		_hookAddRef.Uninit();
		msgStore->Release();
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE 
OEAPIMessageStoreOE::OpenFolderHook(IMessageStore * This, FOLDERID__ folderId, IMessageServer * server, ULONG reserved, IMessageFolder** folder)
{
	HRESULT hr = _oldOpenFolder(This, folderId, server, reserved, folder);
	if(SUCCEEDED(hr)) {
		HRESULT hr2 = S_OK;
		hr2 = OEAPIMessageStoreOE::Get()->RegisterMessageNotification(folderId);
		if(FAILED(hr2)) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::OpenFolderHook: Can't register notify %d %08x.\n"), folderId, hr2);
		}
	}
	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::GetMessageStore(void** msgStore)
{
	if(_msgStore != NULL)
	{
		*(IMessageStore**)msgStore = _msgStore;
		_msgStore->AddRef();
	}
	else
	{
		*msgStore = _msgStore = _msgStoreStatic;

		if(_msgStore == NULL) {
			return E_FAIL;
		}

		_msgStore->AddRef(); 
	}
    return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::OpenFolder(NktFOLDERID folderId, IMessageFolder **ppfInfo)
{
	HRESULT hr;
	IMessageStorePtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::OpenFolder: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	if(!_oldOpenFolder) {
		hr = msgStore->OpenFolder(folderId, 0, 0, ppfInfo);
	}
	else {
		hr = _oldOpenFolder(msgStore, folderId, 0, 0, ppfInfo);
	}

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::FreeRecord(void *ptr)
{
	HRESULT hr;
	IMessageStorePtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::FreeRecord: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->FreeRecord(ptr);

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::GetEnumChildren(NktFOLDERID folderId, IEnumerateFolders **ppEnum)
{
	HRESULT hr;
	IMessageStorePtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::EnumChildren: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->EnumChildren(folderId, 1, ppEnum);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::EnumChildren: Error EnumChildren %08x.\n"), hr);
	}

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::GetNextFolderId(IEnumerateFolders *pEnum, ULONG next, NktFOLDERID *folderId, DWORD *pcount)
{
	HRESULT hr = E_FAIL;
	FOLDERINFO folderInfo = {0};
	ULONG count = 0;

	hr = pEnum->Next(next, &folderInfo, &count);
	if(SUCCEEDED(hr) && hr != S_FALSE) {
		*folderId = folderInfo.dwFolderId;
		FreeRecord(&folderInfo);
	}
	else if(hr != S_FALSE) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetNextFolderId: Error Next %08x.\n"), hr);
	}

	if(SUCCEEDED(hr) && pcount) {
		*pcount = count;
	}

	return hr;
}

//----------------------------------------------------------------------------
LONG OEAPIMessageStoreOE::GetMaxMessageId(NktFOLDERID folderId)
{
	LONG maxId = -1;
	HRESULT hr;
	IMessageFolderPtr folder;

	hr = OpenFolder(folderId, &folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetMaxMessageId: Open folder failed %08x.\n"), hr);
		return maxId;
	}

	HROWSET__* rowset;
	hr = folder->CreateRowset(0, 0, &rowset);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetMaxMessageId: Can't create enumeration %08x.\n"), hr);
		return maxId;
	}

	ULONG pos = 0;
	hr = folder->SeekRowset(rowset, (enum tagSEEKROWSETTYPE)2, 0, &pos);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetMaxMessageId: Can't seek enumeration %08x.\n"), hr);
		folder->CloseRowset(&rowset);
		return maxId;
	}

	MESSAGEINFO msgInfo;
	hr = folder->QueryRowset(rowset, pos, (LPVOID*)&msgInfo, NULL);
	if(hr == S_OK) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetMaxMessageId: Can't query enumeration %08x.\n"), hr);
		folder->CloseRowset(&rowset);
		return maxId;
	}

	maxId = msgInfo.dwMessageId;

	folder->FreeRecord(&msgInfo);
	folder->CloseRowset(&rowset);

	return maxId;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::GetFolderType(NktFOLDERID folderId, FolderType *folderType)
{
	HRESULT hr;
	DWORD type = 0;

	FOLDERINFO folderInfo;
	IMessageStorePtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetFolderType: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->GetFolderInfo(folderId, &folderInfo);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetFolderType: Error GetFolderInfo %08x.\n"), hr);
		return hr;
	}

	type = folderInfo.type;

	hr = msgStore->FreeRecord(&folderInfo);

	if(folderType) {
		MapFolderType((BYTE)type, folderType);
	}

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::GetParentId(NktFOLDERID folderId, NktFOLDERID* parentId)
{
	HRESULT hr;

	FOLDERINFO folderInfo;
	IMessageStorePtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetFolderType: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->GetFolderInfo(folderId, &folderInfo);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::GetFolderType: Error GetFolderInfo %08x.\n"), hr);
	}
	else if(parentId != NULL) {
		*parentId = folderInfo.dwParentFolderId;
	}

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::RegisterFoldersNotification(IDatabaseNotify* listener)
{
	HRESULT hr;
	IMessageStorePtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::RegisterFoldersNotification: Can't get message store %08x"), hr);
		return hr;
	}

	hr = msgStore->RegisterNotify(2, 1, OEAPI_FOLDER_EVENT, listener);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::RegisterFoldersNotification: Can't register global notification %08x.\n"), hr);
		return hr;
	}

	_oldOpenFolder = _hookOpenFolder.Init((NktIMessageStore*)(IMessageStore*)msgStore, (NktOpenFolderOE)OpenFolderHook, &NktIMessageStoreVtbl::OpenFolder);

	return S_OK;
}

//------------------------------------------------------------------//
HRESULT OEAPIMessageStoreOE::UnregisterFoldersNotification(IDatabaseNotify* listener)
{
	HRESULT hr;
	IMessageStorePtr msgStore;

	_hookOpenFolder.Uninit();

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::UnregisterFoldersNotification: Can't get message store %08x.\n"), hr);
		return hr;
	}

	 hr = msgStore->UnregisterNotify(listener);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::UnregisterFoldersNotification: Can't unregister global notification %08x.\n"), hr);
		return hr;
	}

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::RegisterMessageNotification(NktFOLDERID folderId, IDatabaseNotify* listener, VOID** pParam)
{
	HRESULT hr;
	IMessageFolder* msgFolder;

	hr = OpenFolder(folderId, &msgFolder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::RegisterMessageNotification: Can't open folder %d %08x.\n"), folderId, hr);
		return hr;
	}
	
	hr = msgFolder->RegisterNotify(0, 1, folderId, listener);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::RegisterMessageNotification: Can't register folder %d %08x.\n"), folderId, hr);
		msgFolder->Release();
		pParam = NULL;
		return hr;
	}

	if(pParam != NULL) {
		*pParam = (VOID*)msgFolder;
	}

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::UnregisterMessageNotification(IDatabaseNotify* listener, VOID* param)
{
	IMessageFolder* folder = (IMessageFolder*)param;
	if(folder != NULL) {
		HRESULT hr = folder->UnregisterNotify(listener);
		if(FAILED(hr)) {
			//debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::UnregisterMessageNotification: UnregisterNotify failed %08x : %08x - %08x.\n"), param, listener, hr);
		}
		else {
			folder->Release();
		}
		return hr;
	}
	return E_FAIL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::OpenMimeMessage(NktFOLDERID folderId, NktMESSAGEID msgId, IMimeMessage** ppMimeMsg)
{
	HRESULT hr;
	IMessageFolderPtr folder;

	hr = OpenFolder(folderId, &folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::OpenMimeMessage: Can't open folder %08x.\n"), hr);
		return hr;
	}

	hr = folder->OpenMessage(msgId, 0, ppMimeMsg, NULL);

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::SaveMimeMessage(NktFOLDERID folderId, ULONG state, IMimeMessage* pMimeMsg, NktMESSAGEID* pNewMsgId)
{
	HRESULT hr;
	IMessageFolderPtr folder;

	hr = OpenFolder(folderId, &folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::OpenMimeMessage: Can't open folder %08x.\n"), hr);
		return hr;
	}

	MESSAGEID msgId = 0;
	hr = folder->SaveMessage(&msgId, 0, state, NULL, pMimeMsg, NULL);
	*pNewMsgId = msgId;
	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreOE::DeleteMessage(NktFOLDERID folderId, NktMESSAGEID msgId, BOOL permanent)
{
	HRESULT hr;
	IMessageFolderPtr folder;

	hr = OpenFolder(folderId, &folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::DeleteMessage: Can't open folder %08x.\n"), hr);
		return hr;
	}

	MESSAGEID msgId2 = msgId;

	MESSAGEIDLIST msgList;
	msgList.cbSize = sizeof(MESSAGEIDLIST);
	msgList.cMsgs = 1;
	msgList.prgdwMsgId = &msgId2;

	if(permanent) {
		hr = folder->DeleteMessages(1, &msgList, NULL, &nullStoreCallback);
	}
	else {
		hr = folder->DeleteMessages(0, &msgList, NULL, NULL);
	}
	return hr;
}

//------------------------------------------------------------------//
//HRESULT OEAPIMessageStoreOE::GetMessageState(FOLDERID folderId, MESSAGEID msgId, LPDWORD pState)
//{
//	HRESULT hr;
//	IMessageFolderPtr folder;
//	MESSAGEINFOWMAIL msgInfo;
//
//	hr = OpenFolder(folderId, &folder);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::GetMessageState: Can't open folder %08x.\n"), hr);
//		return hr;
//	}
//
//	ZeroMemory(&msgInfo, sizeof(msgInfo));
//	msgInfo.dwMessageId = msgId;
//	hr = folder->FindRecord(0, -1, &msgInfo, NULL);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::GetMessageState: Can't find msgId %d:%d %08x.\n"), folderId, msgId, hr);
//		return hr;
//	}
//
//	if(pState) {
//		*pState = msgInfo.dwARF;
//	}
//
//	folder->FreeRecord(&msgInfo);
//
//	return hr;
//}

//------------------------------------------------------------------//
//HRESULT OEAPIMessageStoreOE::SetMessageState(FOLDERID folderId, MESSAGEID msgId, DWORD state)
//{
//	HRESULT hr;
//	IMessageFolderPtr folder;
//	MESSAGEINFO msgInfo;
//
//	hr = OpenFolder(folderId, &folder);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::SetMessageState: Can't open folder %08x.\n"), hr);
//		return hr;
//	}
//
//	ZeroMemory(&msgInfo, sizeof(msgInfo));
//	msgInfo.dwMessageId = msgId;
//	hr = folder->FindRecord(0, -1, &msgInfo, NULL);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::SetMessageState: Can't find msgId %d:%d %08x.\n"), folderId, msgId, hr);
//		return hr;
//	}
//
//	msgInfo.dwARF = state;
//
//	hr = folder->UpdateRecord(&msgInfo);
//
//	folder->FreeRecord(&msgInfo);
//
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEAPIMessageStore::SetMessageState: Can't update record %08x.\n"), hr);
//		return hr;
//	}
//
//	return hr;
//}

//------------------------------------------------------------------//
HRESULT OEAPIMessageStoreOE::GetFolderTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo)
{
	HRESULT hr;
	FOLDERINFO folInfo1 = {0};
	FOLDERINFO folInfo2 = {0};
	ULONG dummy = 0;

	hr = database->GetTransaction(hTransaction, &transInfo->transType, &folInfo1,
							&folInfo2, &dummy, &transInfo->ordList);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::OnTransaction: Error GetTransaction %08x.\n"), hr);
		return hr;
	}

	transInfo->folderId1 = folInfo1.dwFolderId;
	transInfo->folderId2 = folInfo2.dwFolderId;
	transInfo->parentId1 = folInfo1.dwParentFolderId;
	transInfo->parentId2 = folInfo2.dwParentFolderId;
	transInfo->folderType = folInfo1.type;

	transInfo->rename = FALSE;
	if(transInfo->parentId1 == transInfo->parentId2 && transInfo->folderId1 == transInfo->folderId2) {
		if(folInfo1.szFolderName != NULL && folInfo2.szFolderName != NULL && strcmp(folInfo1.szFolderName, folInfo2.szFolderName) != 0) {
			transInfo->rename = TRUE;
		}
	}

	database->FreeRecord(&folInfo1);
	database->FreeRecord(&folInfo2);

	return S_OK;
}

//------------------------------------------------------------------//
HRESULT OEAPIMessageStoreOE::GetMessageTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo)
{
	HRESULT hr;
	MESSAGEINFO msgInfo1 = {0};
	MESSAGEINFO msgInfo2 = {0};
	ULONG dummy = 0;

	hr = database->GetTransaction(hTransaction, &transInfo->transType, &msgInfo1,
							&msgInfo2, &dummy, &transInfo->ordList);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreOE::OnTransaction: Error GetTransaction folderId: %08x.\n"), hr);
		return hr;
	}

	transInfo->folderId = (NktFOLDERID) -1;
	transInfo->messageId1 = msgInfo1.dwMessageId;
	transInfo->messageId2 = msgInfo2.dwMessageId;
	transInfo->arf1 = msgInfo1.dwARF;
	transInfo->arf2 = msgInfo2.dwARF;

	database->FreeRecord(&msgInfo1);
	database->FreeRecord(&msgInfo2);

	return S_OK;
}


//HRESULT (STDMETHODCALLTYPE *OpenFolder)(IMessageStore * This, FOLDERID__ ,IMessageServer *,ULONG,IMessageFolder**);

// __declspec (naked) 
//HRESULT NktOpenFolder(IMessageStore * This, FOLDERID__ folderId, IMessageServer * server, ULONG reserved, IMessageFolder** folder)
//{
//	debug_print(DEBUG_TRACE, _T("NktOpenFolder: %08x -> %d : %08x - %d - %08x.\n"), This, folderId, server, reserved, folder);
//	//_asm {
//	//	jmp [_oldOpenFolder]
//	//}
//	HRESULT hr = _oldOpenFolder(This, folderId, server, reserved, folder);
//	if(SUCCEEDED(hr)) {
//		nktStoreListener->RegisterMessageNotification(folderId);
//		//if(*folder) {
//		//	HRESULT hr2 = (*folder)->RegisterNotify(0, 1, folderId, nktStoreListener);
//		//	debug_print(DEBUG_TRACE, _T("+++ %d : %08x %08x.\n"), folderId, *folder, hr2);
//		//}
//	}
//	return hr;
//}

//----------------------------------------------------------------------------
//int OEAPIMessageStoreOE::HookOpenFolder(NktIMessageStore* msgStore)
//{
//	OpenFolderHooker::Init(msgStore, (NktOpenFolderOE)OpenFolderHook, fun);
//	/* NktOpenFolderFunction* address = (NktOpenFolderFunction*)&(((NktIMessageStore*)msgStore)->lpVtbl->OpenFolder);
//	//NktOpenFolderFunction* address = &(((NktIMessageStore*)msgStore)->lpVtbl->OpenFolder);
//	_oldOpenFolder = *address;
//	DWORD oldProtect, thunkProtect;
//	::VirtualProtect(address, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
//	*address = (NktOpenFolderFunction)NktOpenFolder;
//	::VirtualProtect(address, 4, oldProtect, &thunkProtect); */
//	
//	return 1;
//}

//////////////////////////////////////////////////////////////////////////
//
NktOpenFolderWM OEAPIMessageStoreWM::_oldOpenFolder = NULL;
NktAddRefFunction OEAPIMessageStoreWM::_oldAddRef = NULL;
IMessageStoreWMail* OEAPIMessageStoreWM::_msgStoreStatic = NULL;

//----------------------------------------------------------------------------
OEAPIMessageStoreWM::OEAPIMessageStoreWM()
{
}

OEAPIMessageStoreWM::~OEAPIMessageStoreWM()
{
	Close();
	_msgStoreStatic = NULL;
}

//----------------------------------------------------------------------------
void OEAPIMessageStoreWM::Init()
{
	OEAPIMessageStore::Init();
	//hookIMessageStoreWM.Init(CLSID_MessageStoreWMail, IID_IMessageStoreWMail);

	IUnknown *pUnknown = NULL;
	HRESULT hr = E_FAIL;
	hr = ::CoCreateInstance(CLSID_MessageStoreWMail, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnknown);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::Init: CoCreateInstance failed %08x.\n"), hr);
		return;
	}

	IMessageStore* msgStore = NULL;
	hr = pUnknown->QueryInterface(IID_IMessageStoreWMail, (LPVOID *)&msgStore);
	pUnknown->Release();

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::Init: QueryInterface failed %08x.\n"), hr);
		return;
	}

	if(msgStore) {
		_oldAddRef = _hookAddRef.Init((NktIMessageStoreWMail*)msgStore, AddRefHook, &NktIMessageStoreWMailVtbl::AddRef);
		msgStore->Release();
	}
}

//----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE 
OEAPIMessageStoreWM::AddRefHook(NktIUnknown *This)
{
	if(_msgStoreStatic == NULL) {
		_msgStoreStatic = (IMessageStoreWMail*)This;
	}
	return _oldAddRef(This);
}

//----------------------------------------------------------------------------
void OEAPIMessageStoreWM::Close()
{
	_hookAddRef.Uninit();
	_hookOpenFolder.Uninit();

	//hookIMessageStoreWM.Uninit();
	OEAPIMessageStore::Close();
}

//----------------------------------------------------------------------------
bool OEAPIMessageStoreWM::CheckStore()
{
	HRESULT hr;
	IMessageStoreWMail* msgStore = NULL;

	hr = GetMessageStore((void**)&msgStore);

	if(SUCCEEDED(hr) && msgStore != NULL) {
		//_oldOpenFolder = _hookOpenFolder.Init((NktIMessageStoreWMail*)msgStore, (NktOpenFolderWM)OpenFolderHook, &NktIMessageStoreWMailVtbl::OpenFolder);
		_hookAddRef.Uninit();
		msgStore->Release();
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE 
OEAPIMessageStoreWM::OpenFolderHook(IMessageStoreWMail * This, ULONGLONG folderId, IMessageServer * server, ULONG reserved, IMessageFolder** folder)
{
	HRESULT hr = _oldOpenFolder(This, folderId, server, reserved, folder);
	if(SUCCEEDED(hr)) { //  && (folderId >> 32) == 0) {
		HRESULT hr2 = S_OK;
		hr2 = OEAPIMessageStoreOE::Get()->RegisterMessageNotification(folderId);
		if(FAILED(hr2)) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::OpenFolderHook: Can't register notify %d %08x.\n"), folderId, hr2);
		}
	}
	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::GetMessageStore(void** msgStore)
{
	if(_msgStore != NULL)
	{
		*(IMessageStoreWMail**)msgStore = _msgStore;
		_msgStore->AddRef();
	}
	else
	{
		*msgStore = _msgStore = _msgStoreStatic;

		if(_msgStore == NULL) {
			return E_FAIL;
		}

		*(IMessageStoreWMail**)msgStore = _msgStore;

		_msgStore->AddRef();
	}
    return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::OpenFolder(NktFOLDERID folderId, IMessageFolder **ppfInfo)
{
	HRESULT hr;
	IMessageStoreWMailPtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::OpenFolder: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	if(_oldOpenFolder) {
		hr = _oldOpenFolder(msgStore, folderId, NULL, 0, ppfInfo);
	}
	else {
		//debug_print(DEBUG_TRACE, _T("-%d-%d.\n"),sizeof(folderId), sizeof(long));
		hr = msgStore->OpenFolder(folderId, NULL, 0, ppfInfo);
	}

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::FreeRecord(void *ptr)
{
	HRESULT hr;
	IMessageStoreWMailPtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::FreeRecord: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->FreeRecord(ptr);

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::GetEnumChildren(NktFOLDERID folderId, IEnumerateFolders **ppEnum)
{
	HRESULT hr;
	IMessageStoreWMailPtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::EnumChildren: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->EnumChildren(folderId, 1, ppEnum);

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::GetNextFolderId(IEnumerateFolders *pEnum, ULONG next, NktFOLDERID *folderId, DWORD *pcount)
{
	HRESULT hr = E_FAIL;
	FOLDERINFOWMAIL folderInfo = {0};
	ULONG count = 0;

	hr = pEnum->Next(next, &folderInfo, &count);
	if(SUCCEEDED(hr) && hr != S_FALSE) {
		*folderId = folderInfo.dwFolderId;
		FreeRecord(&folderInfo);
	}
	else if(hr != S_FALSE) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetNextFolderId: Error Next %08x.\n"), hr);
	}

	if(SUCCEEDED(hr) && pcount) {
		*pcount = count;
	}

	return hr;
}

//----------------------------------------------------------------------------
LONG OEAPIMessageStoreWM::GetMaxMessageId(NktFOLDERID folderId)
{
	LONG maxId = -1;
	HRESULT hr;
	IMessageFolderPtr folder;

	hr = OpenFolder(folderId, &folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetMaxMessageId: Open folder failed %08x.\n"), hr);
		return maxId;
	}

	HROWSET__* rowset;
	hr = folder->CreateRowset(0, 0, &rowset);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetMaxMessageId: Can't create enumeration %08x.\n"), hr);
		return maxId;
	}

	MESSAGEINFOWMAIL msgInfo = {0};

	while(1) {
		hr = folder->QueryRowset(rowset, 1, (LPVOID*)&msgInfo, NULL);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetMaxMessageId: Can't query enumeration %08x.\n"), hr);
			folder->CloseRowset(&rowset);
			return maxId;
		}

		if(hr == S_FALSE) {
			break;
		}

		if((INT)msgInfo.dwMessageId > maxId) {
			maxId = msgInfo.dwMessageId;
		}

		folder->FreeRecord(&msgInfo);
	}

	folder->CloseRowset(&rowset);

	return maxId;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::GetFolderType(NktFOLDERID folderId, FolderType *folderType)
{
	HRESULT hr;
	DWORD type = 0;

	FOLDERINFOWMAIL folderInfo;
	IMessageStoreWMailPtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetFolderType: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->GetFolderInfo(folderId, &folderInfo);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetFolderType: Error GetFolderInfo %08x.\n"), hr);
		return hr;
	}
	else {
		type = folderInfo.type;
	}

	msgStore->FreeRecord(&folderInfo);

	if(folderType) {
		MapFolderType((BYTE)type, folderType);
	}

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::GetParentId(NktFOLDERID folderId, NktFOLDERID* parentId)
{
	HRESULT hr;

	FOLDERINFOWMAIL folderInfo;
	IMessageStoreWMailPtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetFolderType: Error GetMessageStore %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->GetFolderInfo(folderId, &folderInfo);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::GetFolderType: Error GetFolderInfo %08x.\n"), hr);
		return hr;
	}

	if(parentId != NULL) {
		*parentId = folderInfo.dwParentFolderId;
	}

	return hr;
}


//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::RegisterFoldersNotification(IDatabaseNotify* listener)
{
	HRESULT hr;
	IMessageStoreWMailPtr msgStore;

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::RegisterFoldersNotification: Can't get message store %08x.\n"), hr);
		return hr;
	}

	hr = msgStore->RegisterNotify(2, 1, OEAPI_FOLDER_EVENT, listener);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::RegisterFoldersNotification: Can't register global notification %08x.\n"), hr);
		return hr;
	}

	//debug_print(DEBUG_TRACE, _T("OEAPIMessageStoreWM::RegisterFoldersNotification: Hooking open folder th:%d.\n"), hookThread);
	_oldOpenFolder = _hookOpenFolder.Init((NktIMessageStoreWMail*)(IMessageStoreWMail*)msgStore, (NktOpenFolderWM)OpenFolderHook, &NktIMessageStoreWMailVtbl::OpenFolder);

	return S_OK;
}

//------------------------------------------------------------------//
HRESULT OEAPIMessageStoreWM::UnregisterFoldersNotification(IDatabaseNotify* listener)
{
	HRESULT hr;
	IMessageStoreWMailPtr msgStore;

	_hookOpenFolder.Uninit();

	hr = GetMessageStore((void**)&msgStore);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::UnregisterFoldersNotification: Can't get message store %08x.\n"), hr);
		return hr;
	}

	 hr = msgStore->UnregisterNotify(listener);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::UnregisterFoldersNotification: Can't unregister global notification %08x.\n"), hr);
		return hr;
	}

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::RegisterMessageNotification(NktFOLDERID folderId, IDatabaseNotify* listener, VOID** pParam)
{
	HRESULT hr;
	IMessageFolder* msgFolder;

	hr = OpenFolder(folderId, &msgFolder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::RegisterMessageNotification: Can't open folder %d %08x.\n"), folderId, hr);
		return hr;
	}

	hr = msgFolder->RegisterNotify(0, 1, folderId, listener);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::RegisterMessageNotification: Can't register folder %d %08x.\n"), folderId, hr);
		*pParam = NULL;
		msgFolder->Release();
		return hr;
	}

	if(pParam != NULL) {
		*pParam = (VOID*)msgFolder;
	}

	return S_OK;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::UnregisterMessageNotification(IDatabaseNotify* listener, VOID* param)
{
	IMessageFolder* folder = (IMessageFolder*)param;
	if(folder != NULL) {
		HRESULT hr = folder->UnregisterNotify(listener);
		if(FAILED(hr)) {
			//debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::UnregisterMessageNotification: UnregisterNotify failed %08x : %08x - %08x.\n"), param, listener, hr);
		}
		else {
			folder->Release();
			//debug_print(DEBUG_ERROR, _T("<<  %08x : %08x - %08x.\n"), param, listener, hr);
		}
		return hr;
	}
	return E_FAIL;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::OpenMimeMessage(NktFOLDERID folderId, NktMESSAGEID msgId, IMimeMessage** ppMimeMsg)
{
	HRESULT hr;
	IMessageFolderWMailPtr folder;

	hr = OpenFolder(folderId, (IMessageFolder**)&folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::OpenMimeMessage: Can't open folder %08x.\n"), hr);
		return hr;
	}

	hr = folder->OpenMessage(msgId, 0, ppMimeMsg, NULL);

	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::SaveMimeMessage(NktFOLDERID folderId, ULONG state, IMimeMessage* pMimeMsg, NktMESSAGEID* pNewMsgId)
{
	HRESULT hr;
	IMessageFolderWMailPtr folder;

	hr = OpenFolder(folderId, (IMessageFolder**)&folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::OpenMimeMessage: Can't open folder %08x.\n"), hr);
		return hr;
	}

	//MESSAGEID msgId = 0;
	ULONGLONG msgId = 0;
	hr = folder->SaveMessage(&msgId, 1, state, NULL, pMimeMsg, NULL);
	*pNewMsgId = msgId;
	return hr;
}

//----------------------------------------------------------------------------
HRESULT OEAPIMessageStoreWM::DeleteMessage(NktFOLDERID folderId, NktMESSAGEID msgId, BOOL permanent)
{
	HRESULT hr;
	IMessageFolderWMailPtr folder;

	hr = OpenFolder(folderId, (IMessageFolder**)&folder);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::DeleteMessage: Can't open folder %08x.\n"), hr);
		return hr;
	}

	MESSAGEID msgId2[2] = { msgId, 0 };

	MESSAGEIDLIST msgList;
	msgList.cbSize = sizeof(MESSAGEIDLIST);
	msgList.cMsgs = 1;
	msgList.prgdwMsgId = msgId2;

	// deletedFolderId_ == 6 in WinMail
	if(folderId == 6) 
	{
		permanent = TRUE;
	}

	if(permanent) {
		hr = folder->DeleteMessages(15, &msgList, NULL, &nullStoreCallback);
	}
	else {
		hr = folder->DeleteMessages(0, &msgList, NULL, NULL);
	}
	return hr;
}


//------------------------------------------------------------------//
HRESULT OEAPIMessageStoreWM::GetFolderTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo)
{
	HRESULT hr;
	FOLDERINFOWMAIL folInfo1 = {0};
	FOLDERINFOWMAIL folInfo2 = {0};
	ULONG dummy = 0;

	hr = database->GetTransaction(hTransaction, &transInfo->transType, &folInfo1,
								&folInfo2, &dummy, &transInfo->ordList);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::OnTransaction: Error GetTransaction %08x.\n"), hr);
		return hr;
	}

	transInfo->folderId1 = folInfo1.dwFolderId;
	transInfo->folderId2 = folInfo2.dwFolderId;
	transInfo->parentId1 = folInfo1.dwParentFolderId;
	transInfo->parentId2 = folInfo2.dwParentFolderId;
	transInfo->folderType = folInfo1.type;

	transInfo->rename = FALSE;
	if(transInfo->parentId1 == transInfo->parentId2 && transInfo->folderId1 == transInfo->folderId2) {
		if(folInfo1.szFolderName != NULL && folInfo2.szFolderName != NULL && strcmp(folInfo1.szFolderName, folInfo2.szFolderName) != 0) {
			transInfo->rename = TRUE;
		}
	}

	database->FreeRecord(&folInfo1);
	database->FreeRecord(&folInfo2);

	return S_OK;
}

//------------------------------------------------------------------//
HRESULT OEAPIMessageStoreWM::GetMessageTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo)
{
	HRESULT hr;
	MESSAGEINFOWMAIL msgInfo1 = {0};
	MESSAGEINFOWMAIL msgInfo2 = {0};
	ULONG dummy = 0;

	hr = database->GetTransaction(hTransaction, &transInfo->transType, &msgInfo1,
								&msgInfo2, &dummy, &transInfo->ordList);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageStoreWM::OnTransaction: Error GetTransaction folderId: %08x.\n"), hr);
		return E_FAIL;
	}

	transInfo->folderId = msgInfo1.dwFolderId;
	transInfo->messageId1 = msgInfo1.dwMessageId;
	transInfo->messageId2 = msgInfo2.dwMessageId;
	transInfo->arf1 = msgInfo1.dwARF;
	transInfo->arf2 = msgInfo2.dwARF;

	database->FreeRecord(&msgInfo1);
	database->FreeRecord(&msgInfo2);

	return S_OK;
}

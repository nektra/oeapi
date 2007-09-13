/* $Id: messagestore.h,v 1.19.6.8 2007/08/08 22:39:36 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef MESSAGE_STORE_H____
#define MESSAGE_STORE_H____


#include "nkt_utils.h"

#include "oeundoc.h"

#include "nkt_lock.h"
#include "nkt_hook.h"

#include <map>
#include <list>

typedef NKTComPtr<IMessageStore, &IID_IMessageStore> IMessageStorePtr;
typedef NKTComPtr<IMessageStoreWMail, &IID_IMessageStoreWMail> IMessageStoreWMailPtr;
typedef NKTComPtr<IEnumerateFolders, &IID_IEnumerateFolders> IEnumerateFoldersPtr;
typedef NKTComPtr<IMessageFolder, &IID_IMessageFolder> IMessageFolderPtr;
typedef NKTComPtr<IMessageFolderWMail, &IID_IMessageFolder> IMessageFolderWMailPtr;
typedef NKTComPtr<IMimeMessage, &IID_IMimeMessage> IMimeMessagePtr;
typedef NKTComPtr<IStream, &IID_IStream> IStreamPtr;

typedef enum {
	folder_local_mail = 1,
	folder_newsgroup = 2,
	folder_online_mail = 3,
	folder_unknown = 4
} OEStoreFolderType;

typedef enum {
	oeapi_tx_invalid = 0x00,
	oeapi_tx_insert = 0x01,
	oeapi_tx_update = 0x02,
	oeapi_tx_delete = 0x04,
	oeapi_tx_unread = 0x08,
	oeapi_tx_read = 0x010,
	oeapi_tx_rename = 0x020,
	oeapi_tx_move = 0x040,

	oeapi_tx_message = 0x100,
	oeapi_tx_folder = 0x200,

	oeapi_tx_message_insert = oeapi_tx_message | oeapi_tx_insert,
	oeapi_tx_message_update = oeapi_tx_message | oeapi_tx_update,
	oeapi_tx_message_delete = oeapi_tx_message | oeapi_tx_delete,
	oeapi_tx_message_unread = oeapi_tx_message | oeapi_tx_unread,
	oeapi_tx_message_read = oeapi_tx_message | oeapi_tx_read,

	oeapi_tx_folder_insert = oeapi_tx_folder | oeapi_tx_insert,
	oeapi_tx_folder_update = oeapi_tx_folder | oeapi_tx_update,
	oeapi_tx_folder_delete = oeapi_tx_folder | oeapi_tx_delete,
	oeapi_tx_folder_rename = oeapi_tx_folder | oeapi_tx_rename,
	oeapi_tx_folder_move = oeapi_tx_folder | oeapi_tx_move,
} OEStoreTransactionType;

////////////////////////////////////////////////////////////////////////
typedef DWORD NktFOLDERID;
typedef DWORD NktMESSAGEID;

////////////////////////////////////////////////////////////////////////
//
typedef struct OEAPITransInfo__
{
	TRANSACTIONTYPE transType;
	union  {
		struct {
			NktFOLDERID folderId1;
			NktFOLDERID folderId2;
			NktFOLDERID parentId1;
			NktFOLDERID parentId2;
			BYTE folderType;
		};
		struct {
			NktFOLDERID folderId;
			NktMESSAGEID messageId1;
			NktMESSAGEID messageId2;
			ULONG arf1;
			ULONG arf2;
		};
	};
	ORDINALLIST ordList;
} OEAPITransInfo;

////////////////////////////////////////////////////////////////////////
//
struct OEAPIStoreListener {
public:
	virtual void OnDatabaseTransaction(OEStoreTransactionType txType, OEStoreFolderType ft, 
		NktFOLDERID folId, NktMESSAGEID msgId1=0, DWORD arf1=0, NktMESSAGEID msgId2=0, DWORD arf2=0) {};
};

////////////////////////////////////////////////////////////////////////
//
typedef std::map<NktFOLDERID, VOID*> OEAPIFolderList;
typedef std::list<NktFOLDERID> OEAPIFolderLRUList;

////////////////////////////////////////////////////////////////////////
//
class OEAPIMessageStore : public IDatabaseNotify 
{
public:
	typedef OEStoreFolderType FolderType;
	typedef OEStoreTransactionType TransactionType;

	OEAPIMessageStore();
	virtual ~OEAPIMessageStore();

	static OEAPIMessageStore* Get() { return _OEAPIMessageStore; }

	// IUnknown interface
	// OEAPI keeps the object alive until it closes
	virtual DWORD __stdcall AddRef() { return 1; }
	virtual DWORD __stdcall Release() { return 1; }
	virtual HRESULT __stdcall QueryInterface( const IID &iid, void **ppv );

	// IDatabaseNotify interface
    virtual HRESULT __stdcall OnTransaction(HTRANSACTION__ *trans, ULONG dwUser, IDatabase *db);

	// Split OnTransaction into message and folders
	HRESULT NotifyFolderTransaction(HTRANSACTION__ *trans, ULONG dwUser, IDatabase *db);
	HRESULT NotifyMessageTransaction(HTRANSACTION__ *trans, ULONG dwUser, IDatabase *db);

	virtual void Init();
	virtual void Close();

	virtual bool CheckStore() { return false; }

//	HRESULT GetFolderInfo(FOLDERID folderId, LPFOLDERINFO pfInfo);
	virtual HRESULT OpenFolder(NktFOLDERID folderId, IMessageFolder**);
	virtual HRESULT FreeRecord(void *);
	virtual HRESULT GetEnumChildren(NktFOLDERID folderId, IEnumerateFolders **ppEnum);
	virtual HRESULT GetNextFolderId(IEnumerateFolders *pEnum, ULONG next, NktFOLDERID* folderId, DWORD *pcount=NULL);
	virtual HRESULT GetFolderType(NktFOLDERID folderId, FolderType *folderType);
	virtual HRESULT GetParentId(NktFOLDERID folderId, NktFOLDERID* parentId);

	void RegisterAllFoldersNotification();
	void UnregisterAllFoldersNotification();
	void RegisterChildrenFoldersNotification(NktFOLDERID folderId, IDatabaseNotify* listener);
	void UnregisterChildrenFoldersNotification(IDatabaseNotify* listener);
	HRESULT RegisterMessageNotification(NktFOLDERID folderId);
	HRESULT UnregisterMessageNotification(NktFOLDERID folderId);

	OEAPIStoreListener* GetListener() { return _listener; }
	void SetListener(OEAPIStoreListener* listener) { _listener = listener; }

	virtual HRESULT OpenMimeMessage(NktFOLDERID folderId, NktMESSAGEID msgId, IMimeMessage** ppMimeMsg);
	virtual HRESULT SaveMimeMessage(NktFOLDERID folderId, ULONG state, IMimeMessage* pMimeMsg, NktMESSAGEID* pNewMsgId);
	virtual HRESULT DeleteMessage(NktFOLDERID folderId, NktMESSAGEID msgId, BOOL permanent = FALSE);

	//virtual HRESULT GetMessageState(FOLDERID folderId, MESSAGEID msgId, LPDWORD pState);
	//virtual HRESULT SetMessageState(FOLDERID folderId, MESSAGEID msgId, DWORD state);

	virtual LONG GetMaxMessageId(NktFOLDERID folderId);
	HWND GetHwndCallback() { return _hwndCallback; }

protected:
	virtual HRESULT RegisterFoldersNotification(IDatabaseNotify* listener);
	virtual HRESULT UnregisterFoldersNotification(IDatabaseNotify* listener);
	virtual HRESULT RegisterMessageNotification(NktFOLDERID folderId, IDatabaseNotify* listener, VOID** lpParam);
	virtual HRESULT UnregisterMessageNotification(IDatabaseNotify* listener, VOID* param);

protected:
	/**
	Map the low-level TRANSACTIONTYPE to a OEAPI TransactionType
	*/
	void MapTransactionType(BOOL isFolder, TRANSACTIONTYPE transType, TransactionType *tt);

	/**
	Map the low-level folder type to a OEAPI FolderType
	*/
	BOOL MapFolderType(BYTE type, FolderType *ft);

	//void AddRegisteredFolder(FOLDERID folderId, IMessageFolder* folder);
	//void RemoveRegisteredFolder(FOLDERID folderId, IMessageFolder* folder);

	HRESULT InternalRegisterMessageNotification(NktFOLDERID folderId);

private:
	virtual HRESULT GetMessageStore(LPVOID* target) = 0;
	virtual HRESULT GetFolderTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo) { return E_NOTIMPL; }
	virtual HRESULT GetMessageTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo) { return E_NOTIMPL; }

	HWND CreateStoreCallbackWindow();
	static LRESULT CALLBACK OeapiMsgStoreWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static OEAPIMessageStore* _OEAPIMessageStore;


private:
	BOOL _registered;
	NktLock _folderLock;
	//std::map<DWORD, VOID*> _folders;
	OEAPIFolderList _folders;
	OEAPIFolderLRUList _foldersLRU;
	OEAPIStoreListener* _listener;
	HWND _hwndCallback;
};


////////////////////////////////////////////////////////////////////////
//
typedef HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NktOpenFolderOE)(IMessageStore * This, FOLDERID__ , IMessageServer *, ULONG, IMessageFolder**);

class OEAPIMessageStoreOE : public OEAPIMessageStore
{
	//typedef NktMethodHooker<NktIMessageStore, NktOpenFolderOE> OpenFolderHooker;

public:
	OEAPIMessageStoreOE();
	virtual ~OEAPIMessageStoreOE();

	virtual void Init();
	virtual void Close();

	virtual bool CheckStore();

	virtual HRESULT OpenFolder(NktFOLDERID folderId, IMessageFolder**);
	virtual HRESULT FreeRecord(void *);
	virtual HRESULT GetEnumChildren(NktFOLDERID folderId, IEnumerateFolders **ppEnum);
	virtual HRESULT GetNextFolderId(IEnumerateFolders *pEnum, ULONG next, NktFOLDERID* folderId, DWORD *pcount=NULL);
	virtual HRESULT GetFolderType(NktFOLDERID folderId, FolderType *folderType);
	virtual HRESULT GetParentId(NktFOLDERID folderId, NktFOLDERID* parentId);

	virtual HRESULT OpenMimeMessage(NktFOLDERID folderId, NktMESSAGEID msgId, IMimeMessage** ppMimeMsg);
	virtual HRESULT SaveMimeMessage(NktFOLDERID folderId, ULONG state, IMimeMessage* pMimeMsg, NktMESSAGEID* pNewMsgId);
	virtual HRESULT DeleteMessage(NktFOLDERID folderId, NktMESSAGEID msgId, BOOL permanent = FALSE);

	//virtual HRESULT GetMessageState(FOLDERID folderId, MESSAGEID msgId, LPDWORD pState);
	//virtual HRESULT SetMessageState(FOLDERID folderId, MESSAGEID msgId, DWORD state);

	virtual LONG GetMaxMessageId(NktFOLDERID folderId);

protected:
	virtual HRESULT RegisterFoldersNotification(IDatabaseNotify* listener);
	virtual HRESULT UnregisterFoldersNotification(IDatabaseNotify* listener);
	virtual HRESULT RegisterMessageNotification(NktFOLDERID folderId, IDatabaseNotify* listener, VOID** pParam);
	virtual HRESULT UnregisterMessageNotification(IDatabaseNotify* listener, VOID* param);

private:
	virtual HRESULT GetMessageStore(LPVOID* target);
	//HRESULT GetMessageStore(IMessageStore** target);
	virtual HRESULT GetFolderTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo);
	virtual HRESULT GetMessageTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo);

	static HRESULT STDMETHODCALLTYPE OpenFolderHook(IMessageStore * This, FOLDERID__ , IMessageServer *, ULONG, IMessageFolder**);
	static ULONG STDMETHODCALLTYPE AddRefHook(NktIUnknown * This);

private:
	NktMethodHooker<NktIMessageStore> _hookOpenFolder;
	NktMethodHooker<NktIMessageStore> _hookAddRef;
	static NktOpenFolderOE _oldOpenFolder;
	static NktAddRefFunction _oldAddRef;
	static IMessageStore* _msgStoreStatic;

private:
	IMessageStorePtr _msgStore;
};

////////////////////////////////////////////////////////////////////////
//
typedef HRESULT ( STDMETHODCALLTYPE *NktOpenFolderWM)(IMessageStoreWMail * This, ULONG, ULONG, IMessageServer *, ULONG, IMessageFolder**);

class OEAPIMessageStoreWM : public OEAPIMessageStore
{
	//typedef NktMethodHooker<NktIMessageStoreWMail, NktOpenFolderWM> OpenFolderHooker;

public:
	OEAPIMessageStoreWM();
	virtual ~OEAPIMessageStoreWM();

	virtual void Init();
	virtual void Close();

	virtual bool CheckStore();

	virtual HRESULT OpenFolder(NktFOLDERID folderId, IMessageFolder**);
	virtual HRESULT FreeRecord(void *);
	virtual HRESULT GetEnumChildren(NktFOLDERID folderId, IEnumerateFolders **ppEnum);
	virtual HRESULT GetNextFolderId(IEnumerateFolders *pEnum, ULONG next, NktFOLDERID* folderId, DWORD *pcount=NULL);
	virtual HRESULT GetFolderType(NktFOLDERID folderId, FolderType *folderType);
	virtual HRESULT GetParentId(NktFOLDERID folderId, NktFOLDERID* parentId);

	virtual HRESULT OpenMimeMessage(NktFOLDERID folderId, NktMESSAGEID msgId, IMimeMessage** ppMimeMsg);
	virtual HRESULT SaveMimeMessage(NktFOLDERID folderId, ULONG state, IMimeMessage* pMimeMsg, NktMESSAGEID* pNewMsgId);
	virtual HRESULT DeleteMessage(NktFOLDERID folderId, NktMESSAGEID msgId, BOOL permanent = FALSE);

	// virtual HRESULT SetMessageState(FOLDERID folderId, MESSAGEID msgId, DWORD state);

	virtual LONG GetMaxMessageId(NktFOLDERID folderId);

protected:
	virtual HRESULT RegisterFoldersNotification(IDatabaseNotify* listener);
	virtual HRESULT UnregisterFoldersNotification(IDatabaseNotify* listener);
	virtual HRESULT RegisterMessageNotification(NktFOLDERID folderId, IDatabaseNotify* listener, VOID** pParam);
	virtual HRESULT UnregisterMessageNotification(IDatabaseNotify* listener, VOID* param);

private:
	virtual HRESULT GetMessageStore(LPVOID* target);
	//HRESULT GetMessageStore(IMessageStoreWMail** target);
	virtual HRESULT GetFolderTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo);
	virtual HRESULT GetMessageTransactionInfo(IDatabase* database, HTRANSACTION__ ** hTransaction, OEAPITransInfo* transInfo);

	static HRESULT STDMETHODCALLTYPE OpenFolderHook(IMessageStoreWMail * This, ULONG, NktFOLDERID, IMessageServer *, ULONG, IMessageFolder**);
	static ULONG STDMETHODCALLTYPE AddRefHook(NktIUnknown * This);

private:
	NktMethodHooker<NktIMessageStoreWMail> _hookOpenFolder;
	NktMethodHooker<NktIMessageStoreWMail> _hookAddRef;
	static NktOpenFolderWM _oldOpenFolder;
	static NktAddRefFunction _oldAddRef;
	static IMessageStoreWMail* _msgStoreStatic;

private:
	IMessageStoreWMailPtr _msgStore;
};

typedef CountedPtr<OEAPIMessageStore> OEAPIMessageStorePtr;


#endif // MESSAGE_STORE_H____
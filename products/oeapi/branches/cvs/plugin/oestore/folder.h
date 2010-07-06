/* $Id: folder.h,v 1.19 2007/05/28 19:31:17 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#pragma once

#include "msoeapi.h"
#include "message.h"
#include "oeundoc.h"


//struct OEAPIFolderInfo
//{
//public:
//	OEAPIFolderInfo()
//		:id_(-1), specialId_(-1), pSF_(NULL), msgFolder_(NULL)
//	{
//	}
//
//	INT id_;
//	INT specialId_;
//	IStoreFolder* pSF_;
//	IMessageFolder* msgFolder_;
//};
//


typedef std::list<HENUMSTORE> HENUMSTOREList;

#ifdef STATIC_LIBRARY
	#define _TEMPLATE
#else
	#define _TEMPLATE template<>
#endif



#ifdef STATIC_LIBRARY

class TOEFolder;
typedef AutoPtr<TOEFolder> TOEFolderPtr;

typedef std::list<TOEFolder *> FolderList;

class TOEFolder

#else

#define TOEFolderPtr com_ptr<IOEFolder>
#define TOEFolder coclass_implementation<OEFolder>

typedef std::list<TOEFolder *> FolderList;

///////////////////////////////////////////////////////////////////////
//[1]
struct IFakeOEFolderEvents : public comet::OESTORE::IOEFolderEvents {
    typedef comet::OESTORE::IOEFolderEvents interface_is;
};

template<> class comet::connection_point<IFakeOEFolderEvents> : public comet::connection_point<comet::OESTORE::IOEFolderEvents>
{
public:
    connection_point(IUnknown* pUnk) : connection_point<OESTORE::IOEFolderEvents>(pUnk) {}

	inline STDMETHOD(Advise)(::IUnknown* pUnkSink, DWORD* pdwCookie);

	inline STDMETHOD(Unadvise)(DWORD dwCookie);
};
//[1]
///////////////////////////////////////////////////////////////////////

//[2]
//struct FakeOEFolderEvents : public OESTORE::IOEFolderEvents {
//};
//
//
//struct DECLSPEC_UUID("12BAF0A1-264C-464B-9D58-C83B3781DD4B") IFakeOEFolder : OESTORE::IOEFolder {
//	//typedef IOEFolder interface_is;
//};
//
//struct DECLSPEC_UUID("12BAF070-264C-464B-9D58-C83B3781DD4B") FakeOEFolder : public  OESTORE::OEFolder {
//    //typedef IFakeOEFolder interface_is;
//    //typedef make_list<IFakeOEFolder>::result interfaces;
//    typedef make_list<IFakeOEFolderEvents>::result  source_interfaces;
//    typedef make_list<IOEFolderImpl< coclass_implementation<FakeOEFolder>, IOEFolder, OESTORE_type_library > , implement_cpc<  source_interfaces > >::result interface_impls;
//    //typedef make_list<FakeOEFolderEvents>::result  source_interfaces;
//};
//
//template<> class connection_point<FakeOEFolderEvents> 
//	: public connection_point<OESTORE::IOEFolderEvents> {
//public:
//    connection_point(IUnknown* pUnk) : connection_point<OESTORE::IOEFolderEvents>(pUnk) {}
//
//	STDMETHOD(Advise)(::IUnknown* pUnkSink, DWORD* pdwCookie)
//	{
//		HRESULT hr = connection_point<OESTORE::IOEFolderEvents>::Advise(pUnkSink, pdwCookie);
//		if(SUCCEEDED(hr))
//		{
//			TOEFolder* p = (TOEFolder*)(connection_point_for<FakeOEFolderEvents>*)get_parent();
//			if(p && p->connection_point.is_connected())
//				p->RegisterNotifier();
//		}
//		return hr;
//	}
//	STDMETHOD(Unadvise)(DWORD dwCookie)
//	{
//		HRESULT hr = connection_point<OESTORE::IOEFolderEvents>::Unadvise(dwCookie);
//		if(SUCCEEDED(hr))
//		{
//			TOEFolder* p = (TOEFolder*)(connection_point_for<FakeOEFolderEvents>*)get_parent();
//			if(p && !p->connection_point.is_connected())
//				p->UnregisterNotifier();
//		}
//	}
//};
//[2]


_TEMPLATE class TOEFolder : 
		//public coclass<FakeOEFolder>
		public simple_object< IOEFolderImpl< TOEFolder >, simple_cpc< IFakeOEFolderEvents > > //[1]
		//public coclass<OEFolder>
#endif // STATIC_LIBRARY
{
public:
	//[1]
	typedef TOEFolder coclass_type;
	enum { factory_type = impl::ft_standard };
	enum { thread_model = thread_model::Apartment };
	//[1]
public:
	TOEFolder();
	~TOEFolder();

	friend class OEStoreManager;

	static TCHAR *get_progid() { return _T("OESTORE.OEFolder"); }

	TOEFolderPtr GetFirstChild();
	TOEFolderPtr GetNextChild();

	bstr_t Name();

    BOOL Rename(const bstr_t &newName);
    BOOL Move(DWORD newParentId);
    BOOL Delete();

	/**
	Create a folder as child of this one.
	Return -1 if an error or the new folder id.
	*/
	long CreateFolder(const bstr_t &name);

//	void MarkAsRead(const safearray_t<DWORD> &msgIds);
//	void MarkAsUnread(const safearray_t<DWORD> &msgIds);
	
	BOOL IsInbox() { 
		return specialId_ == FOLDER_INBOX; 
	}
	BOOL IsOutbox() { 
		return specialId_ == FOLDER_OUTBOX; 
	}
	BOOL IsSent() { 
		return specialId_ == FOLDER_SENT; 
	}
	BOOL IsDeleted() { 
		return specialId_ == FOLDER_DELETED; 
	}
	BOOL IsDraft() { 
		return specialId_ == FOLDER_DRAFT; 
	}

	DWORD GetID() { return id_; }

//	void SetRecursive(BOOL enable);
//	BOOL GetRecursive() { return recursiveEvents_; }

	BOOL IsNull() { return pSF_ == NULL; }

	// Messages
	TOEMessagePtr GetFirstMessage();
	TOEMessagePtr GetNextMessage();
	TOEMessagePtr GetMessage(DWORD msgId);
	TOEMessagePtr CreateMessage(const bstr_t &msgSource, BOOL unread);
	BOOL DeleteMessage(DWORD msgId, BOOL permanent);

	BOOL MarkAsRead(DWORD msgId);
	BOOL MarkAsUnread(DWORD msgId);

	/**
	Get the number of sub folders of the folder.
	*/
    DWORD GetSubFolderCount();

	/**
	Get the number of unread messages of the folder.
	*/
    DWORD GetUnreadCount();

	/**
	Get the number of messages of the folder.
	*/
    DWORD GetMessageCount();

	BOOL Close();

	// Internal
	BOOL SetID(DWORD id, IStoreFolder* pSF = NULL);
	void SetSpecialID(DWORD specialId);
	DWORD GetSpecialID() { return specialId_; }

	void SetOEAPIID(DWORD id) { oeapiId_ = id; }
	int OEAPIID() { return oeapiId_; }

	void RegisterNotifier();

	void UnregisterNotifier();

	//class __declspec(novtable) OEFolderEventsProxy : connection_point_impl<IOEFolderEvents>
	//{
	//}

	//STDMETHOD(FindConnectionPoint)(REFIID riid, IConnectionPoint** ppCP)
	//{
	//	IConnectionPoint* pCP;
	//	HRESULT hr = implement_cpc<OEFolder::source_interfaces>::FindConnectionPoint(riid, &pCP);
	//	if(SUCCEDDED(hr))
	//	{
	//		ppCP = new OEFolderEventsProxy(this, pCP);
	//		return hr;
	//	}
	//	return hr;
	//}

	IStoreFolder *GetSF() { return pSF_; }

//	virtual void OnNewMessageRecursiveEvent(DWORD folderId, DWORD msgId);

//	virtual void OnMessageDeletedRecursiveEvent(DWORD folderId, DWORD msgId);

//	virtual void OnDeleteRecursiveEvent(DWORD folderId);

	virtual void OnNewMessageEvent(LONG msgId);

	virtual void OnMessageDeletedEvent(LONG msgId);

	virtual void OnMessageUnreadChangeEvent(LONG msgId, BOOL unread);

	virtual void OnDeleteEvent();

#ifdef STATIC_LIBRARY
	static TOEFolder *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static com_ptr<IOEFolder> newInstance();
#endif // STATIC_LIBRARY

protected:
//	void AddChildren(TOEFolder *folder);
//	void AddChildren(DWORD id);

	/**
	The reporter folder is the folder that will be called when any folder event ocurr.
	It is used to handle recursive events.
	*/
//	void SetReportEvents(TOEFolder *reporter) { reporter_ = reporter; }

private:
	INT id_, specialId_;
	INT oeapiId_;
	IStoreFolder *pSF_;
	HENUMSTORE hEnumFolder_;
	HENUMSTORE hEnumMsg_;
	HWND hWnd_;
	INT refCount_;
//	FolderList children_;
//	TOEFolder *reporter_;
//	BOOL recursiveEvents_;
	IMessageFolder* msgFolder_;
};

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY

class TOEFolderManager;
typedef AutoPtr<TOEFolderManager> TOEFolderManagerPtr;

typedef std::list<TOEFolderManager *> FolderManagerList;

class TOEFolderManager

#else

#define TOEFolderManagerPtr com_ptr<IOEFolderManager>
#define TOEFolderManager coclass_implementation<OEFolderManager>

typedef std::list<TOEFolderManager *> FolderManagerList;

_TEMPLATE class TOEFolderManager : public coclass<OEFolderManager>

#endif // STATIC_LIBRARY
{
	friend class OEStoreManager;
public:
	TOEFolderManager();
	~TOEFolderManager();

	static TCHAR *get_progid() { return _T("OESTORE.OEFolderManager"); }

	/**
	Activate the IOEFolderManagerEvents::OnNewMessage notification.
	Browse all folders and get the max ids of each folder to know when a new message 
	arrives to each one.
	Deprecated: it's in the manager now.
	*/
	void ActivateGlobalNotification();

	/**
	Restart the folder manager to use current identity.
	NOTE: this function should be used with special care. If you want to manage identities
	you should call this function after IOEInitEvents::OnInitOEAPI to use the current
	identity of OE.
	*/
//	void Restart();

	// COM Interface
	TOEFolderPtr GetInboxFolder();
	TOEFolderPtr GetOutboxFolder();
	TOEFolderPtr GetSentFolder();
	TOEFolderPtr GetDeletedFolder();
	TOEFolderPtr GetDraftFolder();
	
	TOEFolderPtr GetFolder(DWORD folderId);

	// Iterator
	TOEFolderPtr GetFirstFolder();
	TOEFolderPtr GetNextFolder();
	TOEFolderPtr GetCurrentFolder() { return curFolder_; }

	// Folder Operations
    BOOL RenameFolder(DWORD folderId, const bstr_t &newName);
    BOOL MoveFolder(DWORD folderId, DWORD newParentId);
    BOOL DeleteFolder(DWORD folderId);
	long CreateFolder(DWORD parentFolderId, const bstr_t &name);

	// Messages
	BOOL CopyMessage(DWORD srcFolderId, DWORD destFolderId, DWORD msgId);
	BOOL MoveMessage(DWORD srcFolderId, DWORD destFolderId, DWORD msgId);

	BOOL Close();
	
	/**
	Event triggered when a folder is created
	*/
	virtual void OnFolderCreated(DWORD folderId);

	/**
	Event triggered when a folder is deleted
	*/
	virtual void OnFolderDeleted(DWORD folderId);

	/**
	Event triggered when a folder is renamed
	*/
	virtual void OnFolderRenamed(DWORD folderId);

	/**
	Event triggered when a folder is moved
	*/
	virtual void OnFolderMoved(DWORD folderId);

	/**
	Event triggered when a message stored in a folder changes its state to unread
	*/
	virtual void OnFolderUnreadChanged(DWORD folderId);

	/**
	Event triggered when a message new message is stored in a folder.
	This is a global event and can be used to know when a message is created in any folder.
	*/
	virtual void OnNewMessage(DWORD folderId, DWORD newMsgId);

#ifdef STATIC_LIBRARY
	static TOEFolderManager *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static com_ptr<IOEFolderManager> newInstance();
#endif // STATIC_LIBRARY

protected:
	// Internal
	/**
	Get the max id of the specified folder.
	*/
//	DWORD GetMaxMsgID(TOEFolderPtr folderPtr);

	TOEFolderPtr GetSpecialFolder(DWORD specialFolder);

	TOEFolderPtr GetNextSpecialFolder(DWORD specialFolder);

	/**
	Copy or move messages and wait it finished (synchronic)
	*/
	BOOL CopyMoveMessagesSynch(BOOL move, DWORD srcFolderId, DWORD destFolderId, 
								const safearray_t<long> &msgIds);

	void CloseAllEnumHandles();

	/**
	General event. Triggered from the manager
	*/
	void OnEvent(LPFOLDERNOTIFYEX folderNotify);

private:
	TOEFolderPtr curFolder_;
	HENUMSTOREList hEnumList_;
	DWORD curSpecialFolder_;
//	DWORD lastReturnedId_;
//	BOOL globalNewMsgActive_;

//	// last message id of each folder. 
//	// When OE creates a new message in a folder, it uses a number greater that the last
//	// greatest msg id. So, if we remember the last msg id of each folder, we can know
//	// when a new message arrives using the OnFolderUnreadChanged
//	std::map<DWORD, DWORD> maxMsgIds_;
	int refCount_;
};



inline STDMETHODIMP comet::connection_point<IFakeOEFolderEvents>::Advise(::IUnknown* pUnkSink, DWORD* pdwCookie)
{
	HRESULT hr = connection_point<OESTORE::IOEFolderEvents>::Advise(pUnkSink, pdwCookie);
	if(SUCCEEDED(hr))
	{
		TOEFolder* p = (TOEFolder*)(simple_cpc<IFakeOEFolderEvents>*)get_parent();
		if(p) 
			p->RegisterNotifier();
	}
	return hr;
}

inline STDMETHODIMP comet::connection_point<IFakeOEFolderEvents>::Unadvise(DWORD dwCookie)
{
	HRESULT hr = connection_point<OESTORE::IOEFolderEvents>::Unadvise(dwCookie);
	if(SUCCEEDED(hr))
	{
		TOEFolder* p = (TOEFolder*)(simple_cpc<IFakeOEFolderEvents>*)get_parent();
		if(p)
			p->UnregisterNotifier();
	}
	return hr;
}

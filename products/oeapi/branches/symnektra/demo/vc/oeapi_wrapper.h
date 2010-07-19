/* $Id: oeapi_wrapper.h,v 1.14 2008/02/21 05:59:44 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#pragma once


#include "oeapiinitcom.h"
#include "oestore.h"
#include "oeapi.h"

using namespace comet;
using namespace OESTORE;
using namespace OEAPI;
using namespace OEAPIINITCOM;


#include <comet/uuid.h>
#include <comet/bstr.h>
#include <comet/cp.h>

/**
These wrapper classes work in this way:

- Constructors:

* ClassWrapper(BOOL create=FALSE):

If create is FALSE (default) the COM object is not created. 
Otherwise (create is TRUE), the COM object is create. 
NOTE: If you create a OEAPIObj, OEToolbar, OEButton, OEMenu or OEMenuItem you will launch 
OE if it isn't running. 
To create the COM object use Create if you didn't use create = TRUE

* ClassWrapper(com_ptr<OEAPIINITCOM::IOEAPIInit> iface):

Constructor from a interface pointer.
This constructor is used when a function returns a interface pointer (e.g.: CreateToolbar,
GetFolder, etc.).

* ClassWrapper(const ClassWrapper &obj):

Constructor from a class wrapper.


operator=:

These operator functions are implemented to receive objects from other functions 
(e.g.: CreateToolbar, etc.).
NOTE: ClassWrapper &operator=(int null); must receive 0 (or NULL) and is used only to 
release the COM object that the wrapper contains.

CAUTION: Remember to release OEAPI (assign NULL) Library objects (OEOAPIObj, OEToolbar, OEButton, 
OEMenu and OEMenuItem) when the OE shutdowns (OnShutdownOEAPI in OEAPIInit Library). If you 
don't do it the objects will be no longer valid, so DO NOT USE THEM.

*/
//--------------------------------------------------------------------------------//
class OEAPIInitWrapper : public sink_impl< OEAPIINITCOM::IOEInitEventsImpl<OEAPIInitWrapper> >
{
public:
	OEAPIInitWrapper(BOOL create=FALSE);
	OEAPIInitWrapper(com_ptr<OEAPIINITCOM::IOEAPIInit> iface);
	OEAPIInitWrapper(const OEAPIInitWrapper &obj);
	~OEAPIInitWrapper();

	void Create();

	wrap_t<OEAPIINITCOM::IOEAPIInit> *operator->() { return iface_.operator->(); }

	OEAPIInitWrapper &operator=(com_ptr<OEAPIINITCOM::IOEAPIInit> &iface);
	OEAPIInitWrapper &operator=(const OEAPIInitWrapper &obj);
	OEAPIInitWrapper &operator=(int null);

	com_ptr<OEAPIINITCOM::IOEAPIInit> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnInitOEAPI() {}
	virtual void OnShutdownOEAPI() {}

	com_ptr<OEAPIINITCOM::IOEAPIInit> iface_;

};

//--------------------------------------------------------------------------------//
class OEIdentityWrapper : public com_ptr<IOEIdentity>
{
public:
	OEIdentityWrapper(BOOL create=FALSE);
	OEIdentityWrapper(com_ptr<OEAPIINITCOM::IOEIdentity> iface);
	OEIdentityWrapper(const OEIdentityWrapper &obj);
	~OEIdentityWrapper();

	void Create();

	wrap_t<OEAPIINITCOM::IOEIdentity> *operator->() { return iface_.operator->(); }

	OEIdentityWrapper &operator=(com_ptr<OEAPIINITCOM::IOEIdentity> &iface);
	OEIdentityWrapper &operator=(const OEIdentityWrapper &obj);
	OEIdentityWrapper &operator=(int null);

	com_ptr<OEAPIINITCOM::IOEIdentity> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	com_ptr<OEAPIINITCOM::IOEIdentity> iface_;

};

//--------------------------------------------------------------------------------//
class OEFolderWrapper : public sink_impl< OESTORE::IOEFolderEventsImpl<OEFolderWrapper> >
{
public:
	OEFolderWrapper(BOOL create=FALSE);
	OEFolderWrapper(com_ptr<OESTORE::IOEFolder> &iface);
	OEFolderWrapper(const OEFolderWrapper &obj);
	~OEFolderWrapper();

	void Create();

	wrap_t<OESTORE::IOEFolder> *operator->() { return iface_.operator->(); }

	OEFolderWrapper &operator=(const OEFolderWrapper &obj);
	OEFolderWrapper &operator=(com_ptr<OESTORE::IOEFolder> &iface);
	OEFolderWrapper &operator=(int null);

	com_ptr<OESTORE::IOEFolder> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnNewMessage(long msgId) {}
	virtual void OnMessageDeleted(long msgId) {}
	virtual void OnDeletion() {}
	virtual void OnMessageUnreadChange(long msgId, long unread) {}

	com_ptr<OESTORE::IOEFolder> iface_;

};


//--------------------------------------------------------------------------------//
class OEFolderManagerWrapper : public sink_impl< OESTORE::IOEFolderManagerEventsImpl<OEFolderManagerWrapper> >
{
public:
	OEFolderManagerWrapper(BOOL create=FALSE);
	OEFolderManagerWrapper(com_ptr<OESTORE::IOEFolderManager> &iface);
	OEFolderManagerWrapper(const OEFolderManagerWrapper &obj);
	~OEFolderManagerWrapper();

	void Create();

	wrap_t<OESTORE::IOEFolderManager> *operator->() { return iface_.operator->(); }

	OEFolderManagerWrapper &operator=(const OEFolderManagerWrapper &obj);
	OEFolderManagerWrapper &operator=(com_ptr<OESTORE::IOEFolderManager> &iface);
	OEFolderManagerWrapper &operator=(int null);

	com_ptr<OESTORE::IOEFolderManager> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnFolderCreated(long folderId) {}
	virtual void OnFolderDeleted(long folderId) {}
	virtual void OnFolderRenamed(long folderId) {}
	virtual void OnFolderMoved(long folderId) {}
	virtual void OnFolderUnreadChanged(long folderId) {}

	virtual void OnNewMessage(long folderId, long msgId) {}

	com_ptr<OESTORE::IOEFolderManager> iface_;
};

typedef com_ptr<IOEMessage> OEMessageWrapper;


class MyOEMenuItemSilk;

//--------------------------------------------------------------------------------//
class OEMenuItemWrapper : public sink_impl< OEAPI::IOEMenuItemEventsImpl<OEMenuItemWrapper> >
{
public:
	OEMenuItemWrapper(BOOL create=FALSE);
	OEMenuItemWrapper(com_ptr<OEAPI::IOEMenuItem> &iface);
	OEMenuItemWrapper(const OEMenuItemWrapper &obj);
	~OEMenuItemWrapper();

	void Create();

	void advise(const com_ptr<::IUnknown>& t);
	void unadvise();

	wrap_t<OEAPI::IOEMenuItem> *operator->() { return iface_.operator->(); }

	OEMenuItemWrapper &operator=(const OEMenuItemWrapper &obj);
	OEMenuItemWrapper &operator=(com_ptr<OEAPI::IOEMenuItem> &iface);
	OEMenuItemWrapper &operator=(int null);

	com_ptr<OEAPI::IOEMenuItem> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnClick() {}
	virtual void OnMsgWndClick(long msgWndId) {}

	MyOEMenuItemSilk *silk_;
	com_ptr<OEAPI::IOEMenuItem> iface_;
};

class MyOEMenuSilk;

//--------------------------------------------------------------------------------//
class OEMenuWrapper : public sink_impl< OEAPI::IOEMenuEventsImpl<OEMenuWrapper> >
{
public:
	OEMenuWrapper(BOOL create=FALSE);
	OEMenuWrapper(com_ptr<OEAPI::IOEMenu> &iface);
	OEMenuWrapper(const OEMenuWrapper &obj);
	~OEMenuWrapper();

	void Create();

	void advise(const com_ptr<::IUnknown>& t);
	void unadvise();

	wrap_t<OEAPI::IOEMenu> *operator->() { return iface_.operator->(); }

	OEMenuWrapper &operator=(const OEMenuWrapper &obj);
	OEMenuWrapper &operator=(com_ptr<OEAPI::IOEMenu> &iface);
	OEMenuWrapper &operator=(int null);

	com_ptr<OEAPI::IOEMenu> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnItemClicked(long itemId) {}
	virtual void OnMsgWndItemClicked(long itemId, long msgWndId) {}

	com_ptr<OEAPI::IOEMenu> iface_;
	MyOEMenuSilk *silk_;
};


class MyOEButtonSilk;

//--------------------------------------------------------------------------------//
class OEButtonWrapper : public sink_impl< OEAPI::IOEButtonEventsImpl<OEButtonWrapper> >
{
public:
	OEButtonWrapper(BOOL create=FALSE);
	OEButtonWrapper(com_ptr<OEAPI::IOEButton> &iface);
	OEButtonWrapper(const OEButtonWrapper &obj);
	~OEButtonWrapper();

	void Create();

	void advise(const com_ptr<::IUnknown>& t);
	void unadvise();

	wrap_t<OEAPI::IOEButton> *operator->() { return iface_.operator->(); }

	OEButtonWrapper &operator=(const OEButtonWrapper &obj);
	OEButtonWrapper &operator=(com_ptr<OEAPI::IOEButton> &iface);
	OEButtonWrapper &operator=(int null);

	com_ptr<OEAPI::IOEButton> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnClick() {}
	virtual void OnMsgWndClick(long msgWndId) {}

	com_ptr<OEAPI::IOEButton> iface_;
	MyOEButtonSilk *silk_;
};

class MyOEToolbarSilk;

//--------------------------------------------------------------------------------//
class OEToolbarWrapper : public sink_impl< OEAPI::IOEToolbarEventsImpl<OEToolbarWrapper> >
{
public:
	OEToolbarWrapper(BOOL create=FALSE);
	OEToolbarWrapper(com_ptr<OEAPI::IOEToolbar> &iface);
	OEToolbarWrapper(const OEToolbarWrapper &obj);
	~OEToolbarWrapper();

	void Create();

	void advise(const com_ptr<::IUnknown>& t);
	void unadvise();

	wrap_t<OEAPI::IOEToolbar> *operator->() { return iface_.operator->(); }

	OEToolbarWrapper &operator=(const OEToolbarWrapper &obj);
	OEToolbarWrapper &operator=(com_ptr<OEAPI::IOEToolbar> &iface);
	OEToolbarWrapper &operator=(int null);

	com_ptr<OEAPI::IOEToolbar> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnButtonClicked(long buttonId) {}
	virtual void OnButtonMsgWndClicked(long buttonId, long msgWndId) {}

	com_ptr<OEAPI::IOEToolbar> iface_;
	MyOEToolbarSilk *silk_;
};



class MyOEAPIObjSilk;

//--------------------------------------------------------------------------------//
class OEAPIObjWrapper {
public:
	OEAPIObjWrapper(BOOL create=FALSE);
	OEAPIObjWrapper(com_ptr<OEAPI::IOEAPIObj> &iface);
	OEAPIObjWrapper(const OEAPIObjWrapper &obj);
	~OEAPIObjWrapper();

	void Create();

	void advise(const com_ptr<::IUnknown>& t);
	void unadvise();

	wrap_t<OEAPI::IOEAPIObj> *operator->() { return iface_.operator->(); }

	OEAPIObjWrapper &operator=(const OEAPIObjWrapper &obj);
	OEAPIObjWrapper &operator=(com_ptr<OEAPI::IOEAPIObj> &iface);
	OEAPIObjWrapper &operator=(int null);

	com_ptr<OEAPI::IOEAPIObj> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	virtual void OnToolbarButtonClicked(long toolbarId, long buttonId) {}
	virtual void OnToolbarButtonMsgWndClicked(long toolbarId, long buttonId, long msgWndId) {}
	virtual void OnMenuItemClicked(long menuIndex, long menuItemId) {}
	virtual void OnFolderSelectionChanged(long folderId) {}
	virtual void OnMessageSelectionChanged() {}
	virtual void OnCurrentMessageChanged(long msgId) {}
	virtual void OnMessageDownloadStarted() {}
	virtual void OnMessageDownloadFinished() {}

	// Only Enterprise Version
	virtual void OnNewMessageInOutbox(long msgId) {}
	virtual void OnNewMsgWnd(long msgWndId) {}
	virtual void OnMsgWndClosed(long msgWndId, BOOL isMainWindow) {}
	virtual void OnSendButtonMsgWndClicked(long msgWndId) {}
	virtual void OnDatabaseChange(tagOE_DATABASE_TRANSACTION dt, long folderId, long msgId, long parentId) {}
	
	virtual void OnMsgWndMenuItemClicked(long menuIndex, long menuItemId, long msgWndId) {}


	MyOEAPIObjSilk *silk_;
	com_ptr<OEAPI::IOEAPIObj> iface_;
};



//--------------------------------------------------------------------------------//
class OEMsgWndWrapper {
public:
	OEMsgWndWrapper(BOOL create=FALSE);
	OEMsgWndWrapper(com_ptr<OEAPI::IOEMsgWnd> &iface);
	OEMsgWndWrapper(const OEMsgWndWrapper &obj);
	~OEMsgWndWrapper();

	void Create();

	wrap_t<OEAPI::IOEMsgWnd> *operator->() { return iface_.operator->(); }

	OEMsgWndWrapper &operator=(const OEMsgWndWrapper &obj);
	OEMsgWndWrapper &operator=(com_ptr<OEAPI::IOEMsgWnd> &iface);
	OEMsgWndWrapper &operator=(int null);

	com_ptr<OEAPI::IOEMsgWnd> GetInterface() { return iface_; }
	BOOL is_null() { return iface_.is_null(); }

	// Only Enterprise Version
	virtual void OnToolbarButtonClicked(long toolbarId, long buttonId) {}
	virtual void OnMenuItemClicked(long menuIndex, long menuItemId) {}
	virtual void OnClosed(BOOL isMainWindow) {}

	com_ptr<OEAPI::IOEMsgWnd> iface_;
};


/* $Id: oeapi_wrapper.cpp,v 1.11 2008/02/21 05:59:44 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "stdafx.h"

#include "oeapi_wrapper.h"

#define UNADVISE() try { unadvise(); } catch(...) {}


////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEAPIInitWrapper::OEAPIInitWrapper(BOOL create)
{
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEAPIInitWrapper::OEAPIInitWrapper(com_ptr<OEAPIINITCOM::IOEAPIInit> iface)
{
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEAPIInitWrapper::OEAPIInitWrapper(const OEAPIInitWrapper &obj)
{
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEAPIInitWrapper::~OEAPIInitWrapper()
{
	if(!iface_.is_null()) {
		UNADVISE();
	}
	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEAPIInitWrapper::Create()
{
	iface_ = OEAPIINITCOM::OEAPIInit::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEAPIInitWrapper &OEAPIInitWrapper::operator=(const OEAPIInitWrapper &obj)
{
	UNADVISE();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEAPIInitWrapper &OEAPIInitWrapper::operator=(com_ptr<OEAPIINITCOM::IOEAPIInit> &iface)
{
	UNADVISE();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEAPIInitWrapper &OEAPIInitWrapper::operator=(int null)
{
	UNADVISE();

	iface_ = null;
	return *this;
}

////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEIdentityWrapper::OEIdentityWrapper(BOOL create)
{
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEIdentityWrapper::OEIdentityWrapper(com_ptr<OEAPIINITCOM::IOEIdentity> iface)
{
	CoInitialize(0);
	iface_ = iface;
}

//--------------------------------------------------------------------//
OEIdentityWrapper::OEIdentityWrapper(const OEIdentityWrapper &obj)
{
	CoInitialize(0);
	iface_ = obj.iface_;
}

//--------------------------------------------------------------------//
OEIdentityWrapper::~OEIdentityWrapper()
{
	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEIdentityWrapper::Create()
{
	iface_ = OEAPIINITCOM::OEIdentity::create();
}

//--------------------------------------------------------------------//
OEIdentityWrapper &OEIdentityWrapper::operator=(const OEIdentityWrapper &obj)
{
	iface_ = obj.iface_;
	return *this;
}

//--------------------------------------------------------------------//
OEIdentityWrapper &OEIdentityWrapper::operator=(com_ptr<OEAPIINITCOM::IOEIdentity> &iface)
{
	iface_ = iface;
	return *this;
}

//--------------------------------------------------------------------//
OEIdentityWrapper &OEIdentityWrapper::operator=(int null)
{
	iface_ = null;
	return *this;
}

////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEFolderWrapper::OEFolderWrapper(BOOL create)
{
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEFolderWrapper::OEFolderWrapper(com_ptr<OESTORE::IOEFolder> &iface)
{
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEFolderWrapper::OEFolderWrapper(const OEFolderWrapper &obj)
{
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEFolderWrapper::~OEFolderWrapper()
{
	if(!iface_.is_null()) {
		UNADVISE();
	}
	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEFolderWrapper::Create()
{
	iface_ = OESTORE::OEFolder::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEFolderWrapper &OEFolderWrapper::operator=(const OEFolderWrapper &obj)
{
	UNADVISE();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEFolderWrapper &OEFolderWrapper::operator=(com_ptr<OESTORE::IOEFolder> &iface)
{
	UNADVISE();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEFolderWrapper &OEFolderWrapper::operator=(int null)
{
	UNADVISE();

	iface_ = null;
	return *this;
}


////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEFolderManagerWrapper::OEFolderManagerWrapper(BOOL create)
{
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEFolderManagerWrapper::OEFolderManagerWrapper(com_ptr<OESTORE::IOEFolderManager> &iface)
{
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEFolderManagerWrapper::OEFolderManagerWrapper(const OEFolderManagerWrapper &obj)
{
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEFolderManagerWrapper::~OEFolderManagerWrapper()
{
	if(!iface_.is_null()) {
		UNADVISE();
	}
	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEFolderManagerWrapper::Create()
{
	iface_ = OESTORE::OEFolderManager::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEFolderManagerWrapper &OEFolderManagerWrapper::operator=(const OEFolderManagerWrapper &obj)
{
	UNADVISE();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEFolderManagerWrapper &OEFolderManagerWrapper::operator=(com_ptr<OESTORE::IOEFolderManager> &iface)
{
	UNADVISE();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEFolderManagerWrapper &OEFolderManagerWrapper::operator=(int null)
{
	UNADVISE();

	iface_ = null;
	return *this;
}


//--------------------------------------------------------------------//
class MyOEMenuItemSilk : public sink_impl< OEAPI::IOEMenuItemEventsImpl<MyOEMenuItemSilk> > {
public:
	MyOEMenuItemSilk(OEMenuItemWrapper *obj)
	{
		obj_ = obj;
	}

	void OnClick()
	{
		obj_->OnClick();
	}

	void OnMsgWndClick(long msgWndId)
	{
		obj_->OnMsgWndClick(msgWndId);
	}

private:
	OEMenuItemWrapper *obj_;
};

////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEMenuItemWrapper::OEMenuItemWrapper(BOOL create)
{
	silk_ = NULL;
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEMenuItemWrapper::OEMenuItemWrapper(com_ptr<OEAPI::IOEMenuItem> &iface)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEMenuItemWrapper::OEMenuItemWrapper(const OEMenuItemWrapper &obj)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEMenuItemWrapper::~OEMenuItemWrapper()
{
	unadvise();

	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEMenuItemWrapper::advise(const com_ptr<::IUnknown>& t)
{
	if(silk_) {
		delete silk_;
	}

	silk_ = new MyOEMenuItemSilk(this);
	silk_->advise(iface_);
}

//--------------------------------------------------------------------//
void OEMenuItemWrapper::unadvise()
{
	if(silk_) {
		// if the COM was destroyed (OE was unloaded), this line throw a C++ exception as the
		// connection with the COM object is broken (it doesn't exist)
//		try {
			delete silk_;
//		}
//		catch(...) {}

		silk_ = NULL;
	}
}

//--------------------------------------------------------------------//
void OEMenuItemWrapper::Create()
{
	iface_ = OEAPI::OEMenuItem::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEMenuItemWrapper &OEMenuItemWrapper::operator=(const OEMenuItemWrapper &obj)
{
	unadvise();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEMenuItemWrapper &OEMenuItemWrapper::operator=(com_ptr<OEAPI::IOEMenuItem> &iface)
{
	unadvise();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEMenuItemWrapper &OEMenuItemWrapper::operator=(int null)
{
	unadvise();

	iface_ = null;
	return *this;
}


//--------------------------------------------------------------------//
class MyOEMenuSilk : public sink_impl< OEAPI::IOEMenuEventsImpl<MyOEMenuSilk> > {
public:
	MyOEMenuSilk(OEMenuWrapper *obj)
	{
		obj_ = obj;
	}

	void OnItemClicked(long itemId)
	{
		obj_->OnItemClicked(itemId);
	}

	void OnMsgWndItemClicked(long itemId, long msgWndId)
	{
		obj_->OnMsgWndItemClicked(itemId, msgWndId);
	}

private:
	OEMenuWrapper *obj_;
};


////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEMenuWrapper::OEMenuWrapper(BOOL create)
{
	silk_ = NULL;
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEMenuWrapper::OEMenuWrapper(com_ptr<OEAPI::IOEMenu> &iface)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEMenuWrapper::OEMenuWrapper(const OEMenuWrapper &obj)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEMenuWrapper::~OEMenuWrapper()
{
	unadvise();

	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEMenuWrapper::advise(const com_ptr<::IUnknown>& t)
{
	if(silk_) {
		delete silk_;
	}

	silk_ = new MyOEMenuSilk(this);
	silk_->advise(iface_);
}

//--------------------------------------------------------------------//
void OEMenuWrapper::unadvise()
{
	if(silk_) {
		// if the COM was destroyed (OE was unloaded), this line throw a C++ exception as the
		// connection with the COM object is broken (it doesn't exist)
//		try {
			delete silk_;
//		}
//		catch(...) {}

		silk_ = NULL;
	}
}

//--------------------------------------------------------------------//
void OEMenuWrapper::Create()
{
	iface_ = OEAPI::OEMenu::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEMenuWrapper &OEMenuWrapper::operator=(const OEMenuWrapper &obj)
{
	unadvise();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEMenuWrapper &OEMenuWrapper::operator=(com_ptr<OEAPI::IOEMenu> &iface)
{
	unadvise();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEMenuWrapper &OEMenuWrapper::operator=(int null)
{
	unadvise();

	iface_ = null;
	return *this;
}


//--------------------------------------------------------------------//
class MyOEButtonSilk : public sink_impl< OEAPI::IOEButtonEventsImpl<MyOEButtonSilk> > {
public:
	MyOEButtonSilk(OEButtonWrapper *obj)
	{
		obj_ = obj;
	}

	void OnClick()
	{
		obj_->OnClick();
	}

	void OnMsgWndClick(long msgWndId)
	{
		obj_->OnMsgWndClick(msgWndId);
	}

private:
	OEButtonWrapper *obj_;
};


////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEButtonWrapper::OEButtonWrapper(BOOL create)
{
	silk_ = NULL;
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEButtonWrapper::OEButtonWrapper(com_ptr<OEAPI::IOEButton> &iface)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEButtonWrapper::OEButtonWrapper(const OEButtonWrapper &obj)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEButtonWrapper::~OEButtonWrapper()
{
	unadvise();

	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEButtonWrapper::advise(const com_ptr<::IUnknown>& t)
{
	if(silk_) {
		delete silk_;
	}

	silk_ = new MyOEButtonSilk(this);
	silk_->advise(iface_);
}

//--------------------------------------------------------------------//
void OEButtonWrapper::unadvise()
{
	if(silk_) {
		// if the COM was destroyed (OE was unloaded), this line throw a C++ exception as the
		// connection with the COM object is broken (it doesn't exist)
//		try {
			delete silk_;
//		}
//		catch(...) {}

		silk_ = NULL;
	}
}

//--------------------------------------------------------------------//
void OEButtonWrapper::Create()
{
	iface_ = OEAPI::OEButton::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEButtonWrapper &OEButtonWrapper::operator=(const OEButtonWrapper &obj)
{
	unadvise();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEButtonWrapper &OEButtonWrapper::operator=(com_ptr<OEAPI::IOEButton> &iface)
{
	unadvise();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEButtonWrapper &OEButtonWrapper::operator=(int null)
{
	unadvise();

	iface_ = null;
	return *this;
}


//--------------------------------------------------------------------//
class MyOEToolbarSilk : public sink_impl< OEAPI::IOEToolbarEventsImpl<MyOEToolbarSilk> > {
public:
	MyOEToolbarSilk(OEToolbarWrapper *obj)
	{
		obj_ = obj;
	}

	void OnButtonClicked(long buttonId)
	{
		obj_->OnButtonClicked(buttonId);
	}
	void OnButtonMsgWndClicked(long buttonId, long msgWndId)
	{
		obj_->OnButtonMsgWndClicked(buttonId, msgWndId);
	}

private:
	OEToolbarWrapper *obj_;
};


////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEToolbarWrapper::OEToolbarWrapper(BOOL create)
{
	silk_ = NULL;
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEToolbarWrapper::OEToolbarWrapper(com_ptr<OEAPI::IOEToolbar> &iface)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEToolbarWrapper::OEToolbarWrapper(const OEToolbarWrapper &obj)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEToolbarWrapper::~OEToolbarWrapper()
{
	unadvise();

	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEToolbarWrapper::advise(const com_ptr<::IUnknown>& t)
{
	if(silk_) {
		delete silk_;
	}

	silk_ = new MyOEToolbarSilk(this);
	silk_->advise(iface_);
}

//--------------------------------------------------------------------//
void OEToolbarWrapper::unadvise()
{
	if(silk_) {
		// if the COM was destroyed (OE was unloaded), this line throw a C++ exception as the
		// connection with the COM object is broken (it doesn't exist)
//		try {
			delete silk_;
//		}
//		catch(...) {}

		silk_ = NULL;
	}
}

//--------------------------------------------------------------------//
void OEToolbarWrapper::Create()
{
	iface_ = OEAPI::OEToolbar::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEToolbarWrapper &OEToolbarWrapper::operator=(const OEToolbarWrapper &obj)
{
	unadvise();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEToolbarWrapper &OEToolbarWrapper::operator=(com_ptr<OEAPI::IOEToolbar> &iface)
{
	unadvise();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEToolbarWrapper &OEToolbarWrapper::operator=(int null)
{
	unadvise();

	iface_ = null;
	return *this;
}


//--------------------------------------------------------------------//
class MyOEAPIObjSilk : public sink_impl< OEAPI::IOEAPIObjEventsImpl<MyOEAPIObjSilk> > {
public:
	MyOEAPIObjSilk(OEAPIObjWrapper *oeapiObj)
	{
		oeapiObj_ = oeapiObj;
	}
	void OnToolbarButtonClicked(long toolbarId, long btnId)
	{
		oeapiObj_->OnToolbarButtonClicked(toolbarId, btnId);
	}
	void OnToolbarButtonMsgWndClicked(long toolbarId, long btnId, long msgWndId)
	{
		oeapiObj_->OnToolbarButtonMsgWndClicked(toolbarId, btnId, msgWndId);
	}
	void OnMenuItemClicked(long menuIndex, long menuItemId)
	{
		oeapiObj_->OnMenuItemClicked(menuIndex, menuItemId);
	}
	void OnFolderSelectionChanged(long folderId)
	{
		oeapiObj_->OnFolderSelectionChanged(folderId);
	}
	void OnMessageSelectionChanged()
	{
		oeapiObj_->OnMessageSelectionChanged();
	}
	void OnCurrentMessageChanged(long msgId)
	{
		oeapiObj_->OnCurrentMessageChanged(msgId);
	}
	void OnMessageDownloadStarted()
	{
		oeapiObj_->OnMessageDownloadStarted();
	}
	void OnMessageDownloadFinished()
	{
		oeapiObj_->OnMessageDownloadFinished();
	}
	void OnNewMessageInOutbox(long msgWnd)
	{
		oeapiObj_->OnNewMessageInOutbox(msgWnd);
	}
	void OnNewMsgWnd(long msgWndId)
	{
		oeapiObj_->OnNewMsgWnd(msgWndId);
	}
	void OnMsgWndClosed(long msgWndId, BOOL isMainWindow)
	{
		oeapiObj_->OnMsgWndClosed(msgWndId, isMainWindow);
	}
	void OnSendButtonMsgWndClicked(long msgWndId)
	{
		oeapiObj_->OnSendButtonMsgWndClicked(msgWndId);
	}
	void OnDatabaseChange(tagOE_DATABASE_TRANSACTION dt, long folderId, long msgId, long parentId)
	{
		oeapiObj_->OnDatabaseChange(dt, folderId, msgId, parentId);
	}
	void OnMsgWndMenuItemClicked(long menuIndex, long menuItemId, long msgWndId)
	{
		oeapiObj_->OnMsgWndMenuItemClicked(menuIndex, menuItemId, msgWndId);
	}

private:
	OEAPIObjWrapper *oeapiObj_;
};

////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEAPIObjWrapper::OEAPIObjWrapper(BOOL create)
{
	silk_ = NULL;
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEAPIObjWrapper::OEAPIObjWrapper(com_ptr<OEAPI::IOEAPIObj> &iface)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEAPIObjWrapper::OEAPIObjWrapper(const OEAPIObjWrapper &obj)
{
	silk_ = NULL;
	CoInitialize(0);
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
}

//--------------------------------------------------------------------//
OEAPIObjWrapper::~OEAPIObjWrapper()
{
	unadvise();

	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEAPIObjWrapper::advise(const com_ptr<::IUnknown>& t)
{
	if(silk_) {
		delete silk_;
	}

	silk_ = new MyOEAPIObjSilk(this);
	silk_->advise(iface_);
}

//--------------------------------------------------------------------//
void OEAPIObjWrapper::unadvise()
{
	if(silk_) {
		// if the COM was destroyed (OE was unloaded), this line throw a C++ exception as the
		// connection with the COM object is broken (it doesn't exist)
//		try {
			delete silk_;
//		}
//		catch(...) {}

		silk_ = NULL;
	}
}

//--------------------------------------------------------------------//
void OEAPIObjWrapper::Create()
{
	iface_ = OEAPI::OEAPIObj::create();
	advise(iface_);
}

//--------------------------------------------------------------------//
OEAPIObjWrapper &OEAPIObjWrapper::operator=(const OEAPIObjWrapper &obj)
{
	unadvise();
	iface_ = obj.iface_;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEAPIObjWrapper &OEAPIObjWrapper::operator=(com_ptr<OEAPI::IOEAPIObj> &iface)
{
	unadvise();
	iface_ = iface;
	if(!iface_.is_null()) {
		advise(iface_);
	}
	return *this;
}

//--------------------------------------------------------------------//
OEAPIObjWrapper &OEAPIObjWrapper::operator=(int null)
{
	unadvise();
//	sink_impl< OEAPI::IOEAPIObjEventsImpl<OEAPIObjWrapper> >::operator=(NULL);// *this = null;

	iface_ = null;
	return *this;
}



////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------//
OEMsgWndWrapper::OEMsgWndWrapper(BOOL create)
{
	CoInitialize(0);
	if(create) {
		Create();
	}
}

//--------------------------------------------------------------------//
OEMsgWndWrapper::OEMsgWndWrapper(com_ptr<OEAPI::IOEMsgWnd> &iface)
{
	CoInitialize(0);
	iface_ = iface;
}

//--------------------------------------------------------------------//
OEMsgWndWrapper::OEMsgWndWrapper(const OEMsgWndWrapper &obj)
{
	CoInitialize(0);
	iface_ = obj.iface_;
}

//--------------------------------------------------------------------//
OEMsgWndWrapper::~OEMsgWndWrapper()
{
	CoUninitialize();
}

//--------------------------------------------------------------------//
void OEMsgWndWrapper::Create()
{
	iface_ = OEAPI::OEMsgWnd::create();
}

//--------------------------------------------------------------------//
OEMsgWndWrapper &OEMsgWndWrapper::operator=(const OEMsgWndWrapper &obj)
{
	iface_ = obj.iface_;
	return *this;
}

//--------------------------------------------------------------------//
OEMsgWndWrapper &OEMsgWndWrapper::operator=(com_ptr<OEAPI::IOEMsgWnd> &iface)
{
	iface_ = iface;
	return *this;
}

//--------------------------------------------------------------------//
OEMsgWndWrapper &OEMsgWndWrapper::operator=(int null)
{
	iface_ = null;
	return *this;
}

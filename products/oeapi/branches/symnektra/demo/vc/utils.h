/* $Id: utils.h,v 1.8 2008/02/21 05:59:45 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __DEMO_UTILS__
#define __DEMO_UTILS__

#include "oeapi_wrapper.h"

class CDemoDlg;

void SetTopMostWindow(HWND hwnd, BOOL topMost);

//-----------------------------------------------------//
class MyOEAPIInit : public OEAPIInitWrapper {
public:
	MyOEAPIInit(BOOL create=FALSE) : OEAPIInitWrapper(create) { m_dlg = NULL; }

	void OnInitOEAPI();
	void OnShutdownOEAPI();

	void SetDialog(CDemoDlg *dlg);
private:
	CDemoDlg *m_dlg;
};

//-----------------------------------------------------//
class MyOEFolderManager : public OEFolderManagerWrapper {
public:
	MyOEFolderManager(BOOL create=FALSE) : OEFolderManagerWrapper(create) { m_dlg = NULL; }
	void SetDialog(CDemoDlg *dlg) { m_dlg = dlg; }

	void OnFolderCreated(long folderId);
	void OnFolderDeleted(long folderId);
	void OnFolderRenamed(long folderId);
	void OnFolderMoved(long folderId);
	void OnFolderUnreadChanged(long folderId);

	void OnNewMessage(long folderId, long msgId);

private:
	CDemoDlg *m_dlg;
};

//-----------------------------------------------------//
class InboxFolder : public OEFolderWrapper {
public:
	InboxFolder(BOOL create=FALSE) : OEFolderWrapper(create) { m_dlg = NULL; }
	void SetDialog(CDemoDlg *dlg) { m_dlg = dlg; }

	void OnNewMessage(long msgId);
	void OnMessageDeleted(long msgId);
	void OnDeletion();

	InboxFolder &operator=(const InboxFolder &obj) { OEFolderWrapper::operator=(obj); return *this; }
	InboxFolder &operator=(com_ptr<OESTORE::IOEFolder> &iface) { OEFolderWrapper::operator=(iface); return *this; }
	InboxFolder &operator=(int null) { OEFolderWrapper::operator=(null); return *this; }

private:
	CDemoDlg *m_dlg;
};

//-----------------------------------------------------//
class MyOEMenuItem : public OEMenuItemWrapper {
public:
	MyOEMenuItem(BOOL create=FALSE) : OEMenuItemWrapper(create) { m_dlg = NULL; }

	void OnClick();

	MyOEMenuItem &operator=(const MyOEMenuItem &obj) { OEMenuItemWrapper::operator=(obj); return *this; }
	MyOEMenuItem &operator=(com_ptr<OEAPI::IOEMenuItem> &iface) { OEMenuItemWrapper::operator=(iface); return *this; }
	MyOEMenuItem &operator=(int null) { OEMenuItemWrapper::operator=(null); return *this; }

	void SetDialog(CDemoDlg *dlg) { m_dlg = dlg; }
private:
	CDemoDlg *m_dlg;
};

//-----------------------------------------------------//
class MyOEMenu : public OEMenuWrapper {
public:
	MyOEMenu(BOOL create=FALSE) : OEMenuWrapper(create) { m_dlg = NULL; }

	void OnItemClicked(long itemId);

	MyOEMenu &operator=(const MyOEMenu &obj) { OEMenuWrapper::operator=(obj); return *this; }
	MyOEMenu &operator=(com_ptr<OEAPI::IOEMenu> &iface) { OEMenuWrapper::operator=(iface); return *this; }
	MyOEMenu &operator=(int null) { OEMenuWrapper::operator=(null); return *this; }

	void SetDialog(CDemoDlg *dlg) { m_dlg = dlg; }
private:
	CDemoDlg *m_dlg;
};


//-----------------------------------------------------//
class MyOEButton : public OEButtonWrapper {
public:
	MyOEButton(BOOL create=FALSE) : OEButtonWrapper(create) { m_dlg = NULL; }

	void OnClick();

	MyOEButton &operator=(const MyOEButton &obj) { OEButtonWrapper::operator=(obj); return *this; }
	MyOEButton &operator=(com_ptr<OEAPI::IOEButton> &iface) { OEButtonWrapper::operator=(iface); return *this; }
	MyOEButton &operator=(int null) { OEButtonWrapper::operator=(null); return *this; }

	void SetDialog(CDemoDlg *dlg) { m_dlg = dlg; }
private:
	CDemoDlg *m_dlg;
};


//-----------------------------------------------------//
class MyOEToolbar : public OEToolbarWrapper {
public:
	MyOEToolbar(BOOL create=FALSE) : OEToolbarWrapper(create) { m_dlg = NULL; }

	void OnButtonClicked(long buttonId);

	MyOEToolbar &operator=(const OEToolbarWrapper &obj) { OEToolbarWrapper::operator=(obj); return *this; }
	MyOEToolbar &operator=(com_ptr<OEAPI::IOEToolbar> &iface) { OEToolbarWrapper::operator=(iface); return *this; }
	MyOEToolbar &operator=(int null) { OEToolbarWrapper::operator=(null); return *this; }

	void SetDialog(CDemoDlg *dlg) { m_dlg = dlg; }
private:
	CDemoDlg *m_dlg;
};


//-----------------------------------------------------//
class MyOEAPIObj : public OEAPIObjWrapper {
public:
	MyOEAPIObj(BOOL create=FALSE) : OEAPIObjWrapper(create) { m_dlg = NULL; }

	void OnToolbarButtonClicked(long toolbarId, long buttonId);
	void OnMenuItemClicked(long menuIndex, long menuItemId);
	void OnFolderSelectionChanged(long folderId);
	void OnMessageSelectionChanged();
	void OnDatabaseChange(tagOE_DATABASE_TRANSACTION dt, long folderId, long msgId, long parId);

	MyOEAPIObj &operator=(const OEAPIObjWrapper &obj) { OEAPIObjWrapper::operator=(obj); return *this; }
	MyOEAPIObj &operator=(com_ptr<OEAPI::IOEAPIObj> &iface) { OEAPIObjWrapper::operator=(iface); return *this; }
	MyOEAPIObj &operator=(int null) { OEAPIObjWrapper::operator=(null); return *this; }

	void SetDialog(CDemoDlg *dlg) { m_dlg = dlg; }
private:
	CDemoDlg *m_dlg;
};


#endif // __DEMO_UTILS__

/* $Id: utils.cpp,v 1.10 2008/02/21 05:59:45 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "stdafx.h"

#include "demo.h"
#include "demoDlg.h"
#include "InputDlg.h"

#include "utils.h"

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void SetTopMostWindow(HWND hwnd, BOOL topMost)
{
	if(topMost) {
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	else {
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
}

//-------------------------------------------------------------------//
void FolderMessageBox(CDemoDlg *dlg, long folderId, TCHAR *operation)
{
	OEFolderManagerWrapper fm(TRUE);

	CString name = _T("Unknown");
	CString msg = _T("Folder ");
	msg += operation;
	msg += " ";

	if(folderId != -1) {
		OEFolderWrapper folder = fm->GetFolder(folderId);
		if(!folder.is_null()) {
			name = folder->Name().t_str().c_str();
		}
	}

	msg += name;
	dlg->MessageBox((LPCSTR) msg, _T("Folder Operation"), MB_OK);
}

//-------------------------------------------------------------------//
void MessageMessageBox(CDemoDlg *dlg, long folderId, TCHAR *operation)
{
	OEFolderManagerWrapper fm(TRUE);

	CString name = _T("Unknown");
	CString msg = _T("Message ");
	msg += operation;
	msg += " ";

	if(folderId != -1) {
		OEFolderWrapper folder = fm->GetFolder(folderId);
		name = folder->Name().t_str().c_str();
	}

	msg += name;
	dlg->MessageBox((LPCSTR) msg, _T("Message Operation"), MB_OK);
}

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void MyOEAPIInit::OnInitOEAPI()
{
	if(m_dlg) {
		m_dlg->OnInitOEAPI();
	}
}

//-------------------------------------------------------------------//
void MyOEAPIInit::OnShutdownOEAPI()
{
	if(m_dlg) {
		m_dlg->OnShutdownOEAPI();
	}
}

//-------------------------------------------------------------------//
void MyOEAPIInit::SetDialog(CDemoDlg *dlg)
{
	m_dlg = dlg;
	m_dlg->SetWindowText(_T("OE Not Loaded"));
}

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void MyOEFolderManager::OnFolderCreated(long folderId)
{
	if(m_dlg) {
		FolderMessageBox(m_dlg, folderId, _T("Created"));
	}
}

//-------------------------------------------------------------------//
void MyOEFolderManager::OnFolderDeleted(long folderId)
{
	if(m_dlg) {
		FolderMessageBox(m_dlg, folderId, _T("Deleted"));
	}
}

//-------------------------------------------------------------------//
void MyOEFolderManager::OnFolderRenamed(long folderId)
{
	if(m_dlg) {
		FolderMessageBox(m_dlg, folderId, _T("Renamed"));
	}
}

//-------------------------------------------------------------------//
void MyOEFolderManager::OnFolderMoved(long folderId)
{
	if(m_dlg) {
		FolderMessageBox(m_dlg, folderId, _T("Moved"));
	}
}

//-------------------------------------------------------------------//
void MyOEFolderManager::OnFolderUnreadChanged(long folderId)
{
	if(m_dlg) {
		FolderMessageBox(m_dlg, folderId, _T("Unread Changed"));
	}
}

//-------------------------------------------------------------------//
void MyOEFolderManager::OnNewMessage(long folderId, long msgId)
{
	// desactivated
//	if(m_dlg) {
//		FolderMessageBox(m_dlg, folderId, _T("New message"));
//	}
}

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void InboxFolder::OnNewMessage(long msgId)
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("New Message."), _T("Inbox Folder"), MB_OK);
	}
}

//-------------------------------------------------------------------//
void InboxFolder::OnMessageDeleted(long msgId)
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Message deleted."), _T("Inbox Folder"), MB_OK);
	}
}

//-------------------------------------------------------------------//
void InboxFolder::OnDeletion()
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Folder deleted."), _T("Inbox Folder"), MB_OK);
	}
}


///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void MyOEMenuItem::OnClick()
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Menu Item Clicked"), _T("Menu Event"), MB_OK);
	}
}

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void MyOEMenu::OnItemClicked(long itemId)
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Menu Item Clicked"), _T("Menu Item Event"), MB_OK);
	}
}

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void MyOEButton::OnClick()
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Toolbar Button Clicked"), _T("Button Event"), MB_OK);
	}
}

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void MyOEToolbar::OnButtonClicked(long buttonId)
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Toolbar Button Clicked"), _T("Toolbar Event"), MB_OK);
	}
}

///////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
void MyOEAPIObj::OnToolbarButtonClicked(long toolbarId, long buttonId)
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Toolbar Button Clicked"), _T("OEAPI Event"), MB_OK);
	}
}

//-------------------------------------------------------------------//
void MyOEAPIObj::OnMenuItemClicked(long menuIndex, long menuItemId)
{
	if(m_dlg) {
		m_dlg->MessageBox(_T("Menu Item Clicked"), _T("OEAPI Event"), MB_OK);
	}
}

//-------------------------------------------------------------------//
void MyOEAPIObj::OnFolderSelectionChanged(long folderId)
{
	if(m_dlg) {
		FolderMessageBox(m_dlg, folderId, _T("Selection Changed"));
	}
}

//-------------------------------------------------------------------//
void MyOEAPIObj::OnMessageSelectionChanged()
{
}

//-------------------------------------------------------------------//
void MyOEAPIObj::OnDatabaseChange(tagOE_DATABASE_TRANSACTION dt, long folderId, long objId, long parId)
{
	if(m_dlg) {
		switch(dt) {
			case OE_TR_INSERT_MESSAGE:
			{
				MessageMessageBox(m_dlg, folderId, _T("Inserted. Folder"));
				break;
			}
			case OE_TR_DELETE_MESSAGE:
			{
				MessageMessageBox(m_dlg, folderId, _T("Deleted. Folder"));
				break;
			}
			case OE_TR_INSERT_FOLDER:
			{
				FolderMessageBox(m_dlg, folderId, _T("New Folder"));
				break;
			}
			case OE_TR_DELETE_FOLDER:
			{
				FolderMessageBox(m_dlg, objId, _T("Folder Deleted. Parent:"));
				break;
			}
		}
	}
}


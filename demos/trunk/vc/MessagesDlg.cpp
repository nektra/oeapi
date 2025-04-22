/* $Id: MessagesDlg.cpp,v 1.6 2007/04/19 16:53:28 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

// MessagesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "MessagesDlg.h"
#include "MsgDlg.h"

#include "FolderSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//---------------------------------------------------------------------//
void MsgOEAPIObj::OnFolderSelectionChanged(long folderId)
{
	if(m_dlg) {
		m_dlg->OnFolderSelectionChanged(folderId);
	}
}

//---------------------------------------------------------------------//
void MsgOEAPIObj::OnMessageSelectionChanged()
{
	if(m_dlg) {
		m_dlg->OnMessageSelectionChanged();
	}		
}

/////////////////////////////////////////////////////////////////////////////
// CMessagesDlg dialog


CMessagesDlg::CMessagesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessagesDlg::IDD, pParent) , m_fm(TRUE)
{
	//{{AFX_DATA_INIT(CMessagesDlg)
	m_msgListValue = _T("");
	//}}AFX_DATA_INIT

	m_folderId = -1;
//	m_folderSelectionChanged = FALSE;
//	m_msgSelectionChanged = FALSE;
}


void CMessagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessagesDlg)
	DDX_Control(pDX, IDC_COPY, m_copyBtn);
	DDX_Control(pDX, IDC_MSG_MOVE, m_moveBtn);
	DDX_Control(pDX, IDC_MSG_SELECT, m_selBtn);
	DDX_Control(pDX, IDC_MSG_LIST, m_msgList);
	DDX_LBString(pDX, IDC_MSG_LIST, m_msgListValue);
	//}}AFX_DATA_MAP
	SetTopMostWindow(m_hWnd, TRUE);
}


BEGIN_MESSAGE_MAP(CMessagesDlg, CDialog)
	//{{AFX_MSG_MAP(CMessagesDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_MSG_MOVE, OnMsgMove)
	ON_BN_CLICKED(IDC_MARK_READ, OnMarkRead)
	ON_BN_CLICKED(IDC_MARK_UNREAD, OnMarkUnread)
	ON_BN_CLICKED(IDC_MSG_VIEW, OnMsgView)
	ON_BN_CLICKED(IDC_MSG_SELECT, OnMsgSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

OEMessageWrapper CMessagesDlg::GetCurMessage()
{
	OEMessageWrapper ret;

	long id = m_msgList.GetCurSel();

	if(id != -1) {
		id = (LONG)m_msgList.GetItemData(id);
		OEFolderWrapper folder = m_fm->GetFolder(m_folderId);
		ret = folder->GetMessage(id);
	}

	return ret;
}	

long CMessagesDlg::GetMessageID(CString &itemValue)
{
	int id = -1, where;

	if(!itemValue.IsEmpty()) {
		where = itemValue.Find(" ");
		if(where != -1) {
			CString num = itemValue.Mid(0, where);
			id = _ttoi((LPCTSTR) num);
		}
	}

	return id;
}

void CMessagesDlg::AddMessage(OEMessageWrapper &m)
{
	CString item;

	item.Format("%d %s %s %s", m->GetID(), m->GetDisplayFrom().s_str().c_str(),
		m->GetDisplayTo().s_str().c_str(), m->GetNormalSubject().s_str().c_str());
	int index = m_msgList.AddString(item);
	m_msgList.SetItemData(index, m->GetID());
}

void CMessagesDlg::SetFolderID(long id)
{
	m_folderId = id;
	if(m_hWnd) {
		m_msgList.ResetContent();
		if(m_folderId != -1) {
			Refresh();
			if(!m_oeapi.is_null()) {
				UpdateMsgSelection();
			}
		}
	}
}

void CMessagesDlg::UpdateMsgSelection()
{
#if !defined(EVALUATION_VERSION) && !defined(STANDARD_VERSION)
	if(m_hWnd) {
		int count = m_msgList.GetCount();
		for(int i=0; i<count; i++) {
			m_msgList.SetSel(i, FALSE);
		}

		long id = m_oeapi->GetFirstSelectedMessageID();
		while(id != -1) {
			for(int i=0; i<m_msgList.GetCount(); i++) {
				if(m_msgList.GetItemData(i) == id) {
					m_msgList.SetSel(i, TRUE);
					break;
				}
			}
			id = m_oeapi->GetNextSelectedMessageID();
		}
	}
#endif // !EVALUATION_VERSION && !STANDARD_VERSION
}

void CMessagesDlg::SetHandleSelection()
{
	m_oeapi.Create();
	m_oeapi.SetDialog(this);
#if !defined(EVALUATION_VERSION) && !defined(STANDARD_VERSION)
	SetFolderID(m_oeapi->GetSelectedFolderID());
#endif // !EVALUATION_VERSION && !STANDARD_VERSION
}

void CMessagesDlg::Refresh()
{
	if(m_hWnd) {
		m_msgList.ResetContent();
		OEFolderWrapper folder = m_fm->GetFolder(m_folderId);
		OEMessageWrapper msg = folder->GetFirstMessage();

		while(!msg.is_null()) {
			AddMessage(msg);
			msg = folder->GetNextMessage();
		}
	}
}

OEFolderWrapper CMessagesDlg::GetFolder(LPCTSTR name)
{
	OEFolderWrapper folder = m_fm->GetFirstFolder();

	while(!folder.is_null()) {
		if(folder->Name().t_str() == name) {
			break;
		}
		folder = m_fm->GetNextFolder();
	}

	return folder;
}

void CMessagesDlg::OnFolderSelectionChanged(long folderId)
{
	SetFolderID(folderId);
}

void CMessagesDlg::OnMessageSelectionChanged()
{
	UpdateMsgSelection();
}

/////////////////////////////////////////////////////////////////////////////
// CMessagesDlg message handlers

void CMessagesDlg::OnDelete() 
{
	int items[100];
	int count = m_msgList.GetSelItems(100, items);
	OEFolderWrapper folder = m_fm->GetFolder(m_folderId);

	for(int i=0; i<count;i++) {
		folder->DeleteMessage((LONG)m_msgList.GetItemData(items[i]), TRUE);
	}
}

void CMessagesDlg::OnCopy() 
{
	OEMessageWrapper msg = GetCurMessage();
	OEFolderWrapper folder = m_fm->GetFolder(m_folderId);

	CFolderSelect dlg;
	if(dlg.DoModal() == IDOK) {
		OEFolderWrapper folderDest = GetFolder(dlg.m_folderSelection);
		m_fm->CopyMessage(folder->GetID(), folderDest->GetID(), msg->GetID());
	}
}

void CMessagesDlg::OnMsgMove() 
{
	int items[100];
	int count = m_msgList.GetSelItems(100, items);
	OEFolderWrapper folder = m_fm->GetFolder(m_folderId);

	CFolderSelect dlg;
	if(dlg.DoModal() == IDOK) {
		OEFolderWrapper folderDest = GetFolder(dlg.m_folderSelection);
		if(!folderDest.is_null()) {
			for(int i=0; i<count;i++) {
				m_fm->MoveMessage(folder->GetID(), folderDest->GetID(), (LONG)m_msgList.GetItemData(items[i]));
			}
		}
	}
}

void CMessagesDlg::OnMarkRead() 
{
	int items[100];
	int count = m_msgList.GetSelItems(100, items);
	OEFolderWrapper folder = m_fm->GetFolder(m_folderId);

	for(int i=0; i<count;i++) {
		folder->MarkAsRead((LONG)m_msgList.GetItemData(items[i]));
	}
}

void CMessagesDlg::OnMarkUnread() 
{
	int items[100];
	int count = m_msgList.GetSelItems(100, items);
	OEFolderWrapper folder = m_fm->GetFolder(m_folderId);

	for(int i=0; i<count;i++) {
		folder->MarkAsUnread((LONG)m_msgList.GetItemData(items[i]));
	}
}

void CMessagesDlg::OnMsgView() 
{
	int items[1];
	int count = m_msgList.GetSelItems(1, items);
	
	if(count == 1) {
		OEFolderWrapper folder = m_fm->GetFolder(m_folderId);
		OEMessageWrapper m = folder->GetMessage((LONG)m_msgList.GetItemData(items[0]));

		CMsgDlg dlg;
        dlg.m_from = m->GetDisplayFrom().t_str().c_str();
        dlg.m_to = m->GetDisplayTo().t_str().c_str();
        dlg.m_subject = m->GetSubject().t_str().c_str();
        dlg.m_body = m->GetAllBody().t_str().c_str();
		dlg.DoModal();
	}
}

void CMessagesDlg::OnMsgSelect() 
{
#if !defined(EVALUATION_VERSION) && !defined(STANDARD_VERSION)
	int items[100];
	int count = m_msgList.GetSelItems(100, items);
	OEFolderWrapper folder = m_fm->GetFolder(m_folderId);

	for(int i=0; i<count;i++) {
		if(i) {
			m_oeapi->AddSelectedMessageID((LONG)m_msgList.GetItemData(items[i]));
		}
		else {
			m_oeapi->SetSelectedMessageID((LONG)m_msgList.GetItemData(items[i]));
		}
	}
#endif // !EVALUATION_VERSION && !STANDARD_VERSION
}

BOOL CMessagesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_selBtn.EnableWindow(!m_oeapi.is_null());

#if defined(EVALUATION_VERSION) || defined(STANDARD_VERSION)
	m_selBtn.EnableWindow(FALSE);
#endif // !EVALUATION_VERSION && !STANDARD_VERSION

	SetFolderID(m_folderId);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

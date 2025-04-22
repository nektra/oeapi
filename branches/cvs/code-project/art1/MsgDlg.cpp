/* $Id: MsgDlg.cpp,v 1.6 2007/01/03 11:09:50 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 * You may modify and/or integrate this code into your commercial software
 * keeping this notice.
 *
 **/

#include "stdafx.h"
#include "Demo.h"
#include "MsgDlg.h"
#include "SelectFolderDlg.h"
#include "MsgSrcDlg.h"
#include "CreateMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog


CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStoreFolder = NULL;
	m_pStoreNamespace = NULL;
}

CMsgDlg::~CMsgDlg()
{
	if(m_pStoreFolder) {
		m_pStoreFolder->Release();
	}
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgDlg)
	DDX_Control(pDX, IDC_LIST_MSG, m_listMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgDlg)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnCopy)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, OnMove)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnMarkRead)
	ON_BN_CLICKED(IDC_BUTTON_UNREAD, OnMarkUnread)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg message handlers

BOOL CMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	UpdateMessages();

	SetWindowText(m_dlgTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgDlg::SetFolder(IStoreNamespace *pSN, STOREFOLDERID dwFolderId)
{
	HRESULT hr;
	FOLDERPROPS folderProps;

	m_pStoreNamespace = pSN;

	hr = m_pStoreNamespace->OpenFolder(dwFolderId, 0, &m_pStoreFolder);
	if(FAILED(hr)) {
		MessageBox(_T("Error opening folder."), _T("Demo Error"));
		return;
	}

	folderProps.cbSize = sizeof(FOLDERPROPS);

	hr = m_pStoreNamespace->GetFolderProps(dwFolderId, 0, &folderProps);
	if(FAILED(hr)) {
		MessageBox(_T("Error getting Folder Properties."), _T("Demo Error"));
		return;
	}

	m_dlgTitle = folderProps.szName;
	m_dlgTitle += _T(" - Messages");
}

void CMsgDlg::UpdateMessages()
{
	m_listMsg.ResetContent();

	if(m_pStoreFolder) {
		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg;
		CString item;
		int nIndex;

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS);

		// as we want the subject and other staff we get all the properties.
		// you can use MSGPROPS_FAST as first parameter to get only a few properties of the message.
		HRESULT hr = m_pStoreFolder->GetFirstMessage(0,
													0,
													MESSAGEID_FIRST,
													&msgProps,
													&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) {
			item = msgProps.pszDisplayFrom;
			item += _T("      ");
			item += msgProps.pszNormalSubject;

			// message subject and from is displayed in the list box.
			// data of each item is the message id.
			nIndex = m_listMsg.AddString(item);

			if(nIndex != LB_ERR && nIndex != LB_ERRSPACE) {
				m_listMsg.SetItemData(nIndex, msgProps.dwMessageId);
			}

			// free the message properties as they are allocated by IStoreFolder.
			m_pStoreFolder->FreeMessageProps(&msgProps);

			hr = m_pStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
		}

		// close the enum
		if(hEnumMsg) {
			m_pStoreFolder->GetMessageClose(hEnumMsg);
		}
	}
}

void CMsgDlg::OnCopy()
{
	CopyMoveMessage(FALSE);
}

void CMsgDlg::OnMove()
{
	CopyMoveMessage(TRUE);
	UpdateMessages();
}

void CMsgDlg::CopyMoveMessage(BOOL move)
{
	int nIndex;
	STOREFOLDERID dwSelMsg;
	HRESULT hr;
	IStoreFolder *pDestFolder;
	MESSAGEIDLIST msgIdList;
	DWORD id;
	MESSAGEPROPS msgProps;
	STOREFOLDERID dwDestFolder;

	DWORD props = -1;

	// Get selected folder id
	nIndex = m_listMsg.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a message first."), _T("Demo Error"));
		return;
	}

	dwSelMsg = m_listMsg.GetItemData(nIndex);

	// Remove all the properties that are not set in the message
	msgProps.cbSize = sizeof(MESSAGEPROPS);
	hr = m_pStoreFolder->GetMessageProps(dwSelMsg, MSGPROPS_FAST, &msgProps);
	if(SUCCEEDED(hr)) {
		CSelectFolderDlg folderDlg;

		folderDlg.SetSN(m_pStoreNamespace);

		if(folderDlg.DoModal() == IDOK) {
			dwDestFolder = folderDlg.GetSelectedFolderID();

			if(dwDestFolder != -1) {
				hr = m_pStoreNamespace->OpenFolder(dwDestFolder, 0, &pDestFolder);
				if(FAILED(hr)) {
					MessageBox(_T("Error opening folder."), _T("Demo Error"));
				}
				else {
					// remove flags that are not present in the message.
					props &= ~msgProps.dwFlags;

					msgIdList.cbSize = sizeof(MESSAGEIDLIST);
					msgIdList.cMsgs = 1;
					msgIdList.prgdwMsgId = &id;
					id = dwSelMsg;

					hr = m_pStoreNamespace->CopyMoveMessages(m_pStoreFolder,
															pDestFolder, &msgIdList,
															(move ? CMF_MOVE : 0),
															props,
															NULL);
					if(FAILED(hr)) {
						MessageBox(_T("Error copying / moving message."), _T("Demo Error"));
					}

					pDestFolder->Release();
				}
			}
		}

		m_pStoreFolder->FreeMessageProps(&msgProps);
	}

}

void CMsgDlg::OnCreate()
{
	CCreateMsgDlg createDlg;

	createDlg.SetFolder(m_pStoreFolder);
	if(createDlg.DoModal() == IDOK) {
		UpdateMessages();
	}
}

void CMsgDlg::OnDelete()
{
	int nIndex;
	STOREFOLDERID dwSelMsg;
	HRESULT hr;
	MESSAGEIDLIST msgIdList;

	// Get selected folder id
	nIndex = m_listMsg.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a message first."), _T("Demo Error"));
		return;
	}

	dwSelMsg = m_listMsg.GetItemData(nIndex);

	DWORD msgIds[] = {dwSelMsg};

	msgIdList.cbSize = sizeof(MESSAGEIDLIST);
	msgIdList.cMsgs = 1;
	msgIdList.prgdwMsgId = msgIds;

	hr = m_pStoreFolder->DeleteMessages(&msgIdList, 0, 0);
	if(FAILED(hr)) {
		MessageBox(_T("Error deleting message."), _T("Demo Error"));
		return;
	}

	UpdateMessages();
}

void CMsgDlg::OnMarkRead()
{
	MarkSelection(TRUE);
}

void CMsgDlg::OnMarkUnread()
{
	MarkSelection(FALSE);
}

void CMsgDlg::MarkSelection(BOOL asRead)
{
	int nIndex;
	STOREFOLDERID dwSelMsg;
	HRESULT hr;

	// Get selected folder id
	nIndex = m_listMsg.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a message first."), _T("Demo Error"));
		return;
	}

	dwSelMsg = m_listMsg.GetItemData(nIndex);

	MESSAGEIDLIST msgIdList;
	DWORD msgIds[] = {dwSelMsg};

	msgIdList.cbSize = sizeof(MESSAGEIDLIST);
	msgIdList.cMsgs = 1;
	msgIdList.prgdwMsgId = msgIds;

	hr = m_pStoreFolder->MarkMessagesAsRead(asRead, 0, &msgIdList);
	if(FAILED(hr)) {
		MessageBox(_T("Error marking message."), _T("Demo Error"));
		return;
	}
}

void CMsgDlg::OnView()
{
	ULONG ulReaded = 0;
	int nIndex;
	STOREFOLDERID dwSelMsg;
	HRESULT hr;
	IStream *pTextStream;
	char buffer[4096];

	// Get selected folder id
	nIndex = m_listMsg.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a message first."), _T("Demo Error"));
		return;
	}

	dwSelMsg = m_listMsg.GetItemData(nIndex);

	// create a IStream from the message
	hr = m_pStoreFolder->OpenMessage(dwSelMsg, IID_IStream, (VOID **) &pTextStream);
	if(FAILED(hr)) {
		MessageBox(_T("Error opening message."), _T("Demo Error"));
		return;
	}

	CMsgSrcDlg msgSrcDlg;

	// read all the message
	do {
		hr = pTextStream->Read(buffer, sizeof(buffer)-1, &ulReaded);
		if(FAILED(hr)) {
			MessageBox(_T("Error reading message."), _T("Demo Error"));
		}
		else {
			buffer[ulReaded] = 0;

			msgSrcDlg.AddMessageSource(buffer);
		}
	} while(SUCCEEDED(hr) && ulReaded != 0);

	if(SUCCEEDED(hr)) {
		// display message
		msgSrcDlg.DoModal();
	}

	pTextStream->Release();
}

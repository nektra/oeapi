/* $Id: SelectFolderDlg.cpp,v 1.3 2007/01/03 11:09:51 ibejarano Exp $
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
#include "SelectFolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDlg dialog


CSelectFolderDlg::CSelectFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectFolderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectFolderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pStoreNamespace = NULL;
	m_selFolder = -1;
}


void CSelectFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectFolderDlg)
	DDX_Control(pDX, IDC_FOLDER_LIST, m_listFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectFolderDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectFolderDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDlg message handlers

BOOL CSelectFolderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AddFolders();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectFolderDlg::AddFolders(STOREFOLDERID dwFolderId)
{
	FOLDERPROPS props;
	HENUMSTORE hEnum;
	int nIndex;

	hEnum = NULL;
	props.cbSize = sizeof(FOLDERPROPS);

	HRESULT hr = m_pStoreNamespace->GetFirstSubFolder(dwFolderId, &props, &hEnum);

	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) {
		nIndex = m_listFolder.AddString(props.szName);
		if(nIndex != LB_ERR && nIndex != LB_ERRSPACE) {
			m_listFolder.SetItemData(nIndex, props.dwFolderId);
			AddFolders(props.dwFolderId);
		}

		hr = m_pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	if(hEnum) {
		m_pStoreNamespace->GetSubFolderClose(hEnum);
	}
}

STOREFOLDERID CSelectFolderDlg::GetSelectedFolderID()
{
	return m_selFolder;
}

void CSelectFolderDlg::OnOK() 
{
	int nIndex;

	// Get selected folder id
	nIndex = m_listFolder.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a folder or cancel."), _T("Demo Error"));
		return;
	}

	m_selFolder = m_listFolder.GetItemData(nIndex);

	CDialog::OnOK();
}

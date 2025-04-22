/* $Id: FolderSelect.cpp,v 1.3 2007/01/03 10:34:27 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

// FolderSelect.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "FolderSelect.h"

#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderSelect dialog


CFolderSelect::CFolderSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderSelect)
	m_folderSelection = _T("");
	//}}AFX_DATA_INIT
}


void CFolderSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderSelect)
	DDX_Control(pDX, IDC_FOLDER_LIST, m_folderList);
	DDX_LBString(pDX, IDC_FOLDER_LIST, m_folderSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderSelect, CDialog)
	//{{AFX_MSG_MAP(CFolderSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderSelect message handlers

BOOL CFolderSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetTopMostWindow(m_hWnd, TRUE);

	OEFolderManagerWrapper fm(TRUE);
	
	m_folderList.ResetContent();
	OEFolderWrapper folder = fm->GetFirstFolder();

	while(!folder.is_null()) {
		m_folderList.AddString(folder->Name().t_str().c_str());
		folder = fm->GetNextFolder();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

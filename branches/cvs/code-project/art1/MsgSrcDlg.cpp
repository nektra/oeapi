/* $Id: MsgSrcDlg.cpp,v 1.5 2007/01/03 11:09:51 ibejarano Exp $
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
#include "MsgSrcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgSource dialog


CMsgSrcDlg::CMsgSrcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgSrcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgSrcDlg)
	//}}AFX_DATA_INIT
}


void CMsgSrcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgSrcDlg)
	DDX_Control(pDX, IDC_EDIT_MSG_SRC, m_editMsgSrc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgSrcDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgSrcDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgSrcDlg message handlers

BOOL CMsgSrcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editMsgSrc.SetWindowText(m_msgSource);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgSrcDlg::AddMessageSource(CString msgSource)
{
	m_msgSource += msgSource;
}

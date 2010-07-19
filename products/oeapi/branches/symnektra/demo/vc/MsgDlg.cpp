/* $Id: MsgDlg.cpp,v 1.3 2007/01/03 10:34:28 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

// MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "MsgDlg.h"
#include "utils.h"

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
	m_body = _T("");
	m_from = _T("");
	m_subject = _T("");
	m_to = _T("");
	//}}AFX_DATA_INIT
}


void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgDlg)
	DDX_Text(pDX, IDC_BODY, m_body);
	DDX_Text(pDX, IDC_FROM, m_from);
	DDX_Text(pDX, IDC_SUBJECT, m_subject);
	DDX_Text(pDX, IDC_TO, m_to);
	//}}AFX_DATA_MAP
	SetTopMostWindow(m_hWnd, TRUE);
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg message handlers

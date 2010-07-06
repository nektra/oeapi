/* $Id: CreateMsgDlg.cpp,v 1.3 2007/01/03 11:09:49 ibejarano Exp $
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
#include "demo.h"
#include "CreateMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateMsgDlg dialog


CCreateMsgDlg::CCreateMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateMsgDlg)
	m_bodyValue = _T("");
	m_fromValue = _T("");
	m_subjectValue = _T("");
	m_toValue = _T("");
	//}}AFX_DATA_INIT
}


void CCreateMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateMsgDlg)
	DDX_Text(pDX, IDC_EDIT_BODY, m_bodyValue);
	DDX_Text(pDX, IDC_EDIT_FROM, m_fromValue);
	DDX_Text(pDX, IDC_EDIT_SUBJECT, m_subjectValue);
	DDX_Text(pDX, IDC_EDIT_TO, m_toValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCreateMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateMsgDlg message handlers


void CCreateMsgDlg::SetFolder(IStoreFolder *pFolder)
{
	m_pFolder = pFolder;
}


void CCreateMsgDlg::OnOK() 
{
	IStream *newMail = NULL;
	MESSAGEID msgId;
	HRESULT hr;
	ULONG len;
	CString msgSource;

	UpdateData();

    msgSource = _T("From: ");
	msgSource += m_fromValue + _T("\r\n");
	msgSource += _T("To:");
	msgSource += m_toValue + _T("\r\n");
	msgSource += _T("Subject:");
	msgSource += m_subjectValue + _T("\r\n\r\n");
	msgSource += m_bodyValue;
	
	hr = m_pFolder->CreateStream(0, 0, &newMail, &msgId);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot Create Stream."), _T("Demo Error"));
		return;
	}

	hr = newMail->Write((const char *) msgSource, msgSource.GetLength(), &len);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot Write message."), _T("Demo Error"));
		newMail->Release();
		return;
	}

	hr = m_pFolder->CommitStream(0, 0, 0, newMail, msgId, NULL);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot Commit Stream."), _T("Demo Error"));
		newMail->Release();
		return;
	}

	newMail->Release();

	CDialog::OnOK();
}

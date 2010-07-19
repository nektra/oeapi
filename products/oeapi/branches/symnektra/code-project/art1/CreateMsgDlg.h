/* $Id: CreateMsgDlg.h,v 1.3 2007/01/03 11:09:49 ibejarano Exp $
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

#if !defined(AFX_CREATEMSGDLG_H__DCBA09C6_0333_42D6_A35F_F6C9BA8158D0__INCLUDED_)
#define AFX_CREATEMSGDLG_H__DCBA09C6_0333_42D6_A35F_F6C9BA8158D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateMsgDlg.h : header file
//

#include "msoeapi.h"


/////////////////////////////////////////////////////////////////////////////
// CCreateMsgDlg dialog

class CCreateMsgDlg : public CDialog
{
// Construction
public:
	CCreateMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateMsgDlg)
	enum { IDD = IDD_CREATE_MSG_DIALOG };
	CString	m_bodyValue;
	CString	m_fromValue;
	CString	m_subjectValue;
	CString	m_toValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	/**
	Set the folder where the message should be created.
	The pointer is owned by the caller and it will not be released by this class.
	*/
	void SetFolder(IStoreFolder *pFolder);

protected:
	IStoreFolder *m_pFolder;

	// Generated message map functions
	//{{AFX_MSG(CCreateMsgDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEMSGDLG_H__DCBA09C6_0333_42D6_A35F_F6C9BA8158D0__INCLUDED_)

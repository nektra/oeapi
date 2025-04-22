/* $Id: MsgDlg.h,v 1.3 2007/01/03 10:34:28 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#if !defined(AFX_MSGDLG_H__6E0272FD_EE91_4DBF_8843_75296F50D5BA__INCLUDED_)
#define AFX_MSGDLG_H__6E0272FD_EE91_4DBF_8843_75296F50D5BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog

class CMsgDlg : public CDialog
{
// Construction
public:
	CMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgDlg)
	enum { IDD = IDD_MESSAGE_DLG };
	CString	m_body;
	CString	m_from;
	CString	m_subject;
	CString	m_to;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGDLG_H__6E0272FD_EE91_4DBF_8843_75296F50D5BA__INCLUDED_)

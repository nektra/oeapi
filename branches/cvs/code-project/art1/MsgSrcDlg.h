/* $Id: MsgSrcDlg.h,v 1.4 2007/01/03 11:09:51 ibejarano Exp $
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

#if !defined(AFX_MSGSOURCE_H__9FAC14D9_9326_41C1_91F1_C87207A527D5__INCLUDED_)
#define AFX_MSGSOURCE_H__9FAC14D9_9326_41C1_91F1_C87207A527D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgSource.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgSrcDlg dialog

class CMsgSrcDlg : public CDialog
{
// Construction
public:
	CMsgSrcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgSrcDlg)
	enum { IDD = IDD_MSG_SRC_DIALOG };
	CEdit	m_editMsgSrc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgSrcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	/**
	Set the source of the message displayed.
	*/
	void AddMessageSource(CString msgSource);

protected:
	CString m_msgSource;

	// Generated message map functions
	//{{AFX_MSG(CMsgSrcDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGSOURCE_H__9FAC14D9_9326_41C1_91F1_C87207A527D5__INCLUDED_)

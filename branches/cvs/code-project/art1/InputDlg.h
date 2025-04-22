/* $Id: InputDlg.h,v 1.3 2007/01/03 11:09:50 ibejarano Exp $
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

#if !defined(AFX_INPUTDLG_H__3EE1C780_7BEE_4FE1_976C_35F2D32D6222__INCLUDED_)
#define AFX_INPUTDLG_H__3EE1C780_7BEE_4FE1_976C_35F2D32D6222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog

class CInputDlg : public CDialog
{
// Construction
public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

	void SetCaption(LPCTSTR caption);
	
// Dialog Data
	//{{AFX_DATA(CInputDlg)
	enum { IDD = IDD_INPUT_DIALOG };
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	CString m_caption;

protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__3EE1C780_7BEE_4FE1_976C_35F2D32D6222__INCLUDED_)

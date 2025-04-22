/* $Id: FolderSelect.h,v 1.3 2007/01/03 10:34:27 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#if !defined(AFX_FOLDERSELECT_H__802A4C4D_8878_4079_AD79_29121AA1A808__INCLUDED_)
#define AFX_FOLDERSELECT_H__802A4C4D_8878_4079_AD79_29121AA1A808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderSelect dialog

class CFolderSelect : public CDialog
{
// Construction
public:
	CFolderSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFolderSelect)
	enum { IDD = IDD_SEL_FOLDER };
	CListBox	m_folderList;
	CString	m_folderSelection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFolderSelect)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERSELECT_H__802A4C4D_8878_4079_AD79_29121AA1A808__INCLUDED_)

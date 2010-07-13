/* $Id: demoDlg.h,v 1.9 2008/02/21 05:59:44 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

// demoDlg.h : header file
//

#if !defined(AFX_DEMODLG_H__3ABF181C_9154_447D_BD59_9037ED616D0B__INCLUDED_)
#define AFX_DEMODLG_H__3ABF181C_9154_447D_BD59_9037ED616D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDemoDlg;

#include "utils.h"

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

class CDemoDlg : public CDialog
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemoDlg)
	enum { IDD = IDD_DEMO_DIALOG };
	CButton	m_addButtonBtn;
	CButton	m_addToolbarBtn;
	CButton	m_addMenuItemsBtn;
	CButton	m_createMsgBtn;
	CButton	m_msgSelBtn;
	CListBox	m_folderList;
	//}}AFX_DATA

	void OnInitOEAPI();
	void OnShutdownOEAPI();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void RefreshFolders();
	OEFolderWrapper GetFolder(const TCHAR *name);

protected:
	HICON m_hIcon;


	MyOEFolderManager m_fm;
	MyOEAPIInit m_oeapiInit;
	MyOEAPIObj m_oeapi;
	MyOEToolbar m_toolbar;
	MyOEMenu m_menu;
	MyOEButton m_button;
	MyOEMenuItem m_menuItem;
//	InboxFolder m_inbox;

	CString m_previousIdentity;

	// Generated message map functions
	//{{AFX_MSG(CDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnToolbar();
	afx_msg void OnAddButton();
	afx_msg void OnAddMenuItems();
	afx_msg void OnRename();
	afx_msg void OnDelete();
	afx_msg void OnMove();
	afx_msg void OnCreate();
	afx_msg void OnPrintMsgs();
	afx_msg void OnCreateMsg();
	afx_msg void OnMsgSelection();
	afx_msg void OnClose();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODLG_H__3ABF181C_9154_447D_BD59_9037ED616D0B__INCLUDED_)

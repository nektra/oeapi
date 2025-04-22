/* $Id: DemoDlg.h,v 1.4 2007/01/03 11:09:50 ibejarano Exp $
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


#if !defined(AFX_DEMODLG_H__9F708468_A593_453B_9679_99EA37F82E2E__INCLUDED_)
#define AFX_DEMODLG_H__9F708468_A593_453B_9679_99EA37F82E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "msoeapi.h"


/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

class CDemoDlg : public CDialog
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CDemoDlg();

// Dialog Data
	//{{AFX_DATA(CDemoDlg)
	enum { IDD = IDD_DEMO_DIALOG };
	CListBox	m_listFolder;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	/**
	Add the child folders of the specified folder recursively to the list box.
	*/
	void AddFolders(STOREFOLDERID dwFolderId = FOLDERID_ROOT);


	IStoreNamespace *m_pStoreNamespace;

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCreateFolder();
	afx_msg void OnRemoveFolder();
	afx_msg void OnRenameFolder();
	afx_msg void OnViewMessages();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODLG_H__9F708468_A593_453B_9679_99EA37F82E2E__INCLUDED_)

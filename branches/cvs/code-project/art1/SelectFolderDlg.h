/* $Id: SelectFolderDlg.h,v 1.3 2007/01/03 11:09:51 ibejarano Exp $
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

#if !defined(AFX_SELECTFOLDERDLG_H__9D1F447E_6C9F_4060_9916_339D479660DA__INCLUDED_)
#define AFX_SELECTFOLDERDLG_H__9D1F447E_6C9F_4060_9916_339D479660DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectFolderDlg.h : header file
//

#include "msoeapi.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDlg dialog

class CSelectFolderDlg : public CDialog
{
// Construction
public:
	CSelectFolderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectFolderDlg)
	enum { IDD = IDD_FOLDER_SEL_DIALOG };
	CListBox	m_listFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectFolderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	/**
	Set the IStoreNamespace.
	*/
	void SetSN(IStoreNamespace *pSN) { m_pStoreNamespace = pSN; }

	/**
	Get the id of the selected folder.
	*/
	STOREFOLDERID GetSelectedFolderID();

	/**
	Add the child folders of the specified folder recursively to the list box.
	*/
	void AddFolders(STOREFOLDERID dwFolderId = FOLDERID_ROOT);

protected:
	IStoreNamespace *m_pStoreNamespace;
	STOREFOLDERID m_selFolder;

protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectFolderDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTFOLDERDLG_H__9D1F447E_6C9F_4060_9916_339D479660DA__INCLUDED_)

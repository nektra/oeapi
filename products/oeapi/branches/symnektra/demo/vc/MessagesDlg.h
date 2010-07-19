/* $Id: MessagesDlg.h,v 1.3 2007/01/03 10:34:28 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#if !defined(AFX_MESSAGESDLG_H__BDD0D30B_64C2_438B_B4AE_B5DFF9F2FAEF__INCLUDED_)
#define AFX_MESSAGESDLG_H__BDD0D30B_64C2_438B_B4AE_B5DFF9F2FAEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessagesDlg.h : header file
//

#include "utils.h"

class CMessagesDlg;

//-----------------------------------------------------//
class MsgOEAPIObj : public OEAPIObjWrapper {
public:
	MsgOEAPIObj() : OEAPIObjWrapper(FALSE) { m_dlg = NULL; }

	void OnFolderSelectionChanged(long folderId);
	void OnMessageSelectionChanged();

	void SetDialog(CMessagesDlg *dlg) { m_dlg = dlg; }
private:
	CMessagesDlg *m_dlg;
};

/////////////////////////////////////////////////////////////////////////////
// CMessagesDlg dialog

class CMessagesDlg : public CDialog
{
// Construction
public:
	CMessagesDlg(CWnd* pParent = NULL);   // standard constructor

	void SetFolderID(long id);
	void SetHandleSelection();

	void OnFolderSelectionChanged(long folderId);
	void OnMessageSelectionChanged();
	void UpdateMsgSelection();

// Dialog Data
	//{{AFX_DATA(CMessagesDlg)
	enum { IDD = IDD_MESSAGES };
	CButton	m_copyBtn;
	CButton	m_moveBtn;
	CButton	m_selBtn;
	CListBox	m_msgList;
	CString	m_msgListValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessagesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void AddMessage(OEMessageWrapper &msg);
	void Refresh();
	OEMessageWrapper GetCurMessage();
	long GetMessageID(CString &itemValue);
	OEFolderWrapper GetFolder(LPCTSTR name);

	long m_folderId;
	MsgOEAPIObj m_oeapi;
	OEFolderManagerWrapper m_fm;
	BOOL m_folderSelectionChanged;
	BOOL m_msgSelectionChanged;

protected:

	// Generated message map functions
	//{{AFX_MSG(CMessagesDlg)
	afx_msg void OnDelete();
	afx_msg void OnCopy();
	afx_msg void OnMsgMove();
	afx_msg void OnMarkRead();
	afx_msg void OnMarkUnread();
	afx_msg void OnMsgView();
	afx_msg void OnMsgSelect();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGESDLG_H__BDD0D30B_64C2_438B_B4AE_B5DFF9F2FAEF__INCLUDED_)

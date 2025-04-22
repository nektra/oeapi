/* $Id: MsgDlg.h,v 1.3 2007/01/03 11:09:50 ibejarano Exp $
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

#if !defined(AFX_MSGDIALOG_H__59268260_5287_4F43_9DF9_3BA3E797EA3D__INCLUDED_)
#define AFX_MSGDIALOG_H__59268260_5287_4F43_9DF9_3BA3E797EA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgDialog.h : header file
//

#include "msoeapi.h"


/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog

class CMsgDlg : public CDialog
{
// Construction
public:
	CMsgDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CMsgDlg)
	enum { IDD = IDD_MSG_DIALOG };
	CListBox	m_listMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	~CMsgDlg();

	/**
	Set the folder associated to this dialog.
	The IStoreNamespace is not released.
	*/
	void SetFolder(IStoreNamespace *pSN, STOREFOLDERID dwFolderId);

	/**
	Put messages of the folder in the list box.
	*/
	void UpdateMessages();

	/**
	Mark the selected message as read or unread.
	*/
	void MarkSelection(BOOL asRead);

	/**
	Copy or Move the selected message.
	*/
	void CopyMoveMessage(BOOL move);

protected:
	IStoreFolder *m_pStoreFolder;
	IStoreNamespace *m_pStoreNamespace;
	CString m_dlgTitle;

	// Generated message map functions
	//{{AFX_MSG(CMsgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCopy();
	afx_msg void OnCreate();
	afx_msg void OnDelete();
	afx_msg void OnMove();
	afx_msg void OnMarkRead();
	afx_msg void OnMarkUnread();
	afx_msg void OnView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGDIALOG_H__59268260_5287_4F43_9DF9_3BA3E797EA3D__INCLUDED_)

// FolderListDlg.h : Declaration of the CFolderListDlg

#pragma once

#include "stdafx.h"
#include "resource.h"

#include <atlhost.h>
#include <string>

#include <commctrl.h>
#include <atlwin.h>

// CFolderListDlg

class CFolderListDlg : 
	public CDialogImpl<CFolderListDlg>
{
public:
    CFolderListDlg(CComPtr<IOEAPIObj> oeapi, CComPtr<IOEFolderManager> foldermanager);
	~CFolderListDlg();

	enum { IDD = IDD_FOLDERS_DLG };

	BEGIN_MSG_MAP(CFolderListDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOk)
		//CHAIN_MSG_MAP(CDialogImpl<CFolderListDlg>)
		NOTIFY_HANDLER(IDC_TREE_FOLDERS, TVN_SELCHANGED, OnTvnSelchangedTreeFolders)
	END_MSG_MAP()

private:

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void LoadFolders();
	void LoadFoldersRecursively(HWND hTree, HTREEITEM hParent, IOEFolderPtr &folder);

	void InsertMessage(IOEMessagePtr message);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CComPtr<IOEAPIObj> m_oeapi;
	CComPtr<IOEFolderManager> m_foldermanager;
	CComPtr<IOEFolder> m_folder;
	COLORREF m_defColor;
public:
	LRESULT OnTvnSelchangedTreeFolders(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};



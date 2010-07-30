#include "stdafx.h"
#include "CFolderListDlg.h"
#include <string>
#include <sstream>

#define MESSAGE_TIMER_ID 0x4345

CFolderListDlg::CFolderListDlg(CComPtr<IOEAPIObj> oeapi, CComPtr<IOEFolderManager> foldermanager):
    m_oeapi(oeapi),m_foldermanager(foldermanager)
{
	//AtlAxWinInit();
	INITCOMMONCONTROLSEX InitCtrlEx;
	InitCtrlEx.dwICC = ICC_UPDOWN_CLASS;
	InitCommonControlsEx(&InitCtrlEx);
}

CFolderListDlg::~CFolderListDlg()
{

}

LRESULT CFolderListDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	CenterWindow(m_hWnd);

	LoadFolders();

	HWND hList = GetDlgItem(IDC_LIST_MESSAGES);


	m_defColor = ListView_GetTextColor(hList);

	LVCOLUMN lvc;
	ZeroMemory(&lvc, sizeof(LVCOLUMN));
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.pszText = _T("From");
	lvc.cx = 100;
	ListView_InsertColumn(hList, lvc.iSubItem, &lvc);

	ZeroMemory(&lvc, sizeof(LVCOLUMN));
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 1;
	lvc.pszText = _T("Subject");
	lvc.cx = 170;
	ListView_InsertColumn(hList, lvc.iSubItem, &lvc);

	ZeroMemory(&lvc, sizeof(LVCOLUMN));
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 2;
	lvc.pszText = _T("Received");
	lvc.cx = 125;
	ListView_InsertColumn(hList, lvc.iSubItem, &lvc);

	return 1;  // Let the system set the focus
}

void CFolderListDlg::LoadFolders()
{
	HWND hTree = this->GetDlgItem(IDC_TREE_FOLDERS);
	TreeView_DeleteAllItems(hTree);

	IOEFolderPtr folder = m_foldermanager->GetFolder(0);
	LoadFoldersRecursively(hTree, NULL, folder);
}

void CFolderListDlg::LoadFoldersRecursively(HWND hTree, HTREEITEM hParent, IOEFolderPtr &folder)
{
	std::basic_string<TCHAR> name = folder->Name();

	if(name.empty())
	{
		return;
	}

	TCHAR unreadCount[100];

	_stprintf_s(unreadCount, sizeof(unreadCount) / sizeof(unreadCount[0]), _T(" (%d)"), folder->GetUnreadCount());
	name += unreadCount;
	TVINSERTSTRUCT tvis;
	tvis.hParent = hParent;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_PARAM;
	tvis.item.state = tvis.item.stateMask = TVIS_EXPANDED;
	tvis.item.pszText = (TCHAR*) name.c_str();
	tvis.item.lParam = folder->GetID();
	HTREEITEM hItem = TreeView_InsertItem(hTree, &tvis);



	IOEFolderPtr child = folder->GetFirstChild();
	while(child != NULL)
	{
		LoadFoldersRecursively(hTree, hItem, child);
		child = folder->GetNextChild();
	}
}

LRESULT CFolderListDlg::OnClickedOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    this->DestroyWindow();
	return 0;
}

LRESULT CFolderListDlg::OnTvnSelchangedTreeFolders(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	HWND hList = GetDlgItem(IDC_LIST_MESSAGES);

	ListView_DeleteAllItems(hList);

	DWORD folderId = pNMTreeView->itemNew.lParam;
	IOEFolderPtr folder = m_foldermanager->GetFolder(folderId);
	if(folder != NULL)
	{
		m_folder = folder;
		IOEMessagePtr message = m_folder->GetFirstMessage();
		if(message != NULL)
		{
			InsertMessage(message);
		}

		SetTimer(MESSAGE_TIMER_ID, USER_TIMER_MINIMUM , NULL);
	}

	return 0;
}

void CFolderListDlg::InsertMessage(IOEMessagePtr message)
{
	HWND hList = GetDlgItem(IDC_LIST_MESSAGES);

	BOOL exist = FALSE;
	std::basic_string<TCHAR> subject = message->GetSubject();
	std::basic_string<TCHAR> from = message->GetDisplayFrom();
	std::basic_string<TCHAR> received;
	long bodyRoot = message->GetBodyHandle(0, OE_IBL_ROOT);

	if(bodyRoot != 0)
	{
		bstr_t rec = message->GetBodyPropByName(bodyRoot, _T("Date"), (long*) &exist);
		if(exist)
			received = rec;
	}
	if((message->GetState() & OE_MSG_UNREAD) != 0)
	{
		ListView_SetTextColor(hList, RGB(0, 0, 0));
	}
	else
	{
		ListView_SetTextColor(hList, m_defColor);
	}

	LVITEM item;

	item.mask = LVIF_TEXT;
	item.iItem = ListView_GetItemCount(hList);
	item.iSubItem = 0;
	item.pszText = (TCHAR*) from.c_str();

	ListView_InsertItem(hList, &item);

	item.mask = LVIF_TEXT;
	item.iSubItem = 1;
	item.pszText = (TCHAR*) subject.c_str();

	ListView_SetItem(hList, &item);

	if(exist)
	{
		item.mask = LVIF_TEXT;
		item.iSubItem = 2;
		item.pszText = (TCHAR*) received.c_str();

		ListView_SetItem(hList, &item);
	}
}

LRESULT CFolderListDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam == MESSAGE_TIMER_ID)
	{
		bHandled = TRUE;
		IOEMessagePtr message = m_folder->GetNextMessage();
		if(message == NULL)
		{
			m_folder = NULL;
			KillTimer(wParam);
		}
		else
		{
			InsertMessage(message);
		}
	}
	return 0;
}

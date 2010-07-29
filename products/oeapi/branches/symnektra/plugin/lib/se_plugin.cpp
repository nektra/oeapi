/* $Id: se_plugin.cpp,v 1.45 2009/01/27 19:29:58 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "oeapi_config.h"

#include <windows.h>
#include <commctrl.h>

#include <shellapi.h>

#include <tchar.h>

#include "se_plugin.h"
#include "se_debug.h"
#include "oe_toolbar.h"
#include "oe_msgwnd.h"
#include "oe_menu.h"

//#include "..\oehook\se_hook.h"

#ifdef EVALUATION_VERSION
#include "eval_utils.h"
#endif // EVALUATION_VERSION

#include "oeapi_utils.h"

#include "oe_msgwnd.h"

#define MAX_COLUMN_TEXT_SIZE 150

// notify in the OE main window that arrives when OE Send/Receive
#define WM_POPACTION 0x08B6
#define WM_STOREEVENT 0x13A0

const UINT CREATE_MSG_TABLES_CODE = RegisterWindowMessage(_T("OEAPI.CreateMsgTables"));
const UINT WM_OEAPI_CLOSE = RegisterWindowMessage(_T("OEAPI_WM_CLOSE"));

// Plugin is information of OE
OEPlugin Plugin; 

// Timer used to create toolbars
static UINT_PTR timerID = 0;

// Pre-defined menus
//extern PREDEF_MENU PredefMenus[MAX_MENUS];


HWND GetMessageWindow(HWND parent)
{
	HWND msgView, msgList, msgWindow = NULL;
		
	msgView = FindWindowRecursive(Plugin.OEMainWindow, _T("Outlook Express Message View"));
	if(msgView) {
		msgList = FindWindowRecursive(msgView, _T("Outlook Express Message List"));
		if(msgList) {
			if(IsWMail()) {
				msgWindow = FindWindowRecursive(msgView, _T("SysListView32"));
			}
			else {
				msgWindow = FindWindowRecursive(msgView, _T("ATL:SysListView32"));
			}
		}
	}

	return msgWindow;

}

VOID UpdateMsgSelection()
{
	if(IsWindow(Plugin.OEMsgWindow)) {
		INT selCount = ListView_GetSelectedCount(Plugin.OEMsgWindow);
		INT i=0;
		INT *selItems = new int[selCount];
		INT itemIndex = ListView_GetNextItem(Plugin.OEMsgWindow, -1, LVNI_SELECTED);
		INT focusItemInx;

		while(itemIndex != -1) {
			int col = 0;

			selItems[i++] = itemIndex;

			itemIndex = ListView_GetNextItem(Plugin.OEMsgWindow, itemIndex, LVNI_SELECTED);
		}

		focusItemInx = ListView_GetNextItem(Plugin.OEMsgWindow, -1, LVNI_FOCUSED);

		if(Plugin.messageCb) {
			(*Plugin.messageCb)(i, selItems, focusItemInx, Plugin.RegenerateMsgList);
			Plugin.RegenerateMsgList = FALSE;
		}

		delete [] selItems;
	}
}

//static INT msgList[1024];
//static INT msgCount = 0;


void OEAPI_SetSelectedMessage(INT count, INT *indexs)
{
	if(IsWindow(Plugin.OEMsgWindow) && count > 0) {
		Plugin.selectionBlocked = TRUE;

		INT itemIndex = ListView_GetNextItem(Plugin.OEMsgWindow, -1, LVNI_SELECTED);
		while(itemIndex != -1) {
			ListView_SetItemState(Plugin.OEMsgWindow, itemIndex, 0, LVIS_SELECTED|LVIS_FOCUSED);
			itemIndex = ListView_GetNextItem(Plugin.OEMsgWindow, itemIndex, LVNI_SELECTED);
		}

		if(indexs[0] != -1) {
			ListView_SetItemState(Plugin.OEMsgWindow, indexs[0], LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}

		for(int i=1; i<count; i++) {
			ListView_SetItemState(Plugin.OEMsgWindow, indexs[i], LVIS_SELECTED, LVIS_SELECTED);
		}

		if(Plugin.messageCb) {
			(*Plugin.messageCb)(count, indexs, indexs[0], Plugin.RegenerateMsgList);
			Plugin.RegenerateMsgList = FALSE;
		}

		Plugin.selectionBlocked = FALSE;
	}
}

void OEAPI_AddSelectedMessage(INT index)
{
	ListView_SetItemState(Plugin.OEMsgWindow, index, LVIS_SELECTED, LVIS_SELECTED);
}


int lastHighlightMenu = -1;
HMENU lastInitMenu = NULL;

LRESULT CALLBACK MainWndProc(HWND hWnd, 
							 UINT msg,
							 WPARAM wParam, 
							 LPARAM lParam)
{
	WNDPROC lpOldProc;

	lpOldProc = Plugin.OldWinProc;

//	for(int j=0; j<msgCount; j++) {
//		if(msgList[j] == msg)
//			break;
//	}
//	if(j==msgCount) {
//		msgList[msgCount++] = msg;
//	}
//
//	if(msg == 11690 || msg == 49297 || msg == 2230) {
//		return TRUE;
//	}

	switch(msg)
	{
		case WM_SETTINGCHANGE:
		case WM_PALETTECHANGED:
		case WM_FONTCHANGE:
		case WM_SYSCOLORCHANGE:
		{
			break;
		}

		case WM_PARENTNOTIFY:
		{
			if (LOWORD(wParam) == WM_CREATE) {
				if(timerID == 0) {
					//debug_print(DEBUG_INFO, _T("MainWndProc: Set timer\n"));
					timerID = ::SetTimer(hWnd, 1, 100, NULL);
				}

				if(OEPluginToolbarMgr::Get()->GetMainWndMgr()->Initialized()) {
					// create again the buttons in OE toolbar because they can be destroyed
					OEPluginToolbarMgr::Get()->GetMainWndMgr()->Initialize(Plugin.OEMainWindow, TRUE);
				}
			}

			break;
		}

		case WM_POPACTION:
		{
			if(Plugin.messageDownloadCb) {
				(*Plugin.messageDownloadCb)((INT)wParam);
			}
			break;
		}
		case WM_TIMER:
		{
			if(!OEPluginToolbarMgr::Get()->GetMainWndMgr()->Initialized()) {
				OEPluginToolbarMgr::Get()->GetMainWndMgr()->Initialize(Plugin.OEMainWindow, TRUE);
			}

			// when a new folder is selected the WM_SETTEXT set the flag and this event is
			// produced to notify that the selection finished, so the new message window was
			// created (could be NULL too)
			if(Plugin.folderSelChangedEventPending) {
				HWND msgList, msgWindow;

				while(1) {
					// first get the message view and then the list view to prevent getting contact
					// list view
					Plugin.hMessageView = FindWindowRecursive(Plugin.OEMainWindow, _T("Outlook Express Message View"));
					if(Plugin.hMessageView == NULL) {
						break;
					}

					msgList = FindWindowRecursive(Plugin.hMessageView, _T("Outlook Express Message List"));
					if(msgList == NULL) {
						break;
					}

					if(IsWMail()) {
						msgWindow = FindWindowRecursive(Plugin.hMessageView, _T("SysListView32"));
					}
					else {
						msgWindow = FindWindowRecursive(Plugin.hMessageView, _T("ATL:SysListView32"));
					}

					if(msgWindow == NULL) {
						break;
					}

					if(msgWindow != Plugin.OEMsgWindow) {
						Plugin.OEMsgWindow = msgWindow;
					}

					break;
				}
			}

			if(!Plugin.selectionBlocked) {
				Plugin.MsgListViewSelPending = FALSE;
				UpdateMsgSelection();
			}

			break;
		}

		case WM_ENDSESSION:
		{
			if(wParam == TRUE) {
				// if the user is logging off or the computer is shutting down do the same 
				// that we do when a WM_CLOSE comes and a WM_DESTROY.
				// this object must be destroyed before all the windows are destroyed

				OEPluginToolbarMgr::Get()->DestroyOEMainWindowTopLevelWndMgr();
				
				::KillTimer(Plugin.OEMainWindow, timerID);
				timerID = 0;

				NotificationDetach(TRUE);
			}

			break;
		}
		case WM_DESTROY:
		{
			NotificationDetach(FALSE);
			break;
		}
		case WM_CLOSE:
		{
			// Check if we have to send wm_oeapi_close
			if(GetProp(hWnd, _T("OEAPI_MainWndDestroying")) != (HANDLE)TRUE)
			{
				SetProp(hWnd, _T("OEAPI_MainWndDestroying"), (HANDLE)TRUE);
				LRESULT lresult = CallWindowProc(MainWndProc, hWnd, WM_OEAPI_CLOSE, NULL, NULL);
			}
			// this object must be destroyed before all the windows are destroyed
			//OEPluginToolbarMgr::Get()->DestroyOEMainWindowTopLevelWndMgr();
			if(Plugin.storeRegisterEventsCb) {
				Plugin.storeRegisterEventsCb(TRUE);
			}
			
//			delete OEPluginToolbarMgr::Get();
			::KillTimer(Plugin.OEMainWindow, timerID);
			timerID = 0;

			break;
		}
		case WM_MENUSELECT:
		{
			lastHighlightMenu = LOWORD(wParam);
			break;
		}
		// use this message to verify if the popup menu isn't a sub item
		case WM_INITMENU:
		{
			lastInitMenu = (HMENU) wParam;
			break;
		}
		case WM_INITMENUPOPUP:
		{
			int i = 0;
			HMENU menu = (HMENU) wParam;

			LRESULT lResult;

			lResult = CallWindowProc(lpOldProc, hWnd, msg, wParam, 
										lParam);

			// if it's equal it's a menu. Otherwise, it's a menu item with sub items.
			// only insert items if it is a menu
			if(menu == lastInitMenu) {
				// InsertMenuItems(menu, lastHighlightMenu);
				OEPluginMenuMgr::Get()->InsertMenuItems(menu, lastHighlightMenu);
			}

			OEPluginMenuMgr::Get()->EnableAllMenus();
			// Menu_EnableAllMenus();

			return lResult;
		}
		// This message is sent after the folder is changed to change the title of the window
		// Handling this event let us know ALWAYS when the current folder is changed, even
		// when it is a external folder (hotmail, IMAP, etc.).
		// It is used to correct the mistakes of FOLDER_PROPS_CHANGED IStoreFolder message.
		case WM_SETTEXT:
		{
			if(Plugin.OEFoldersWindow == NULL) {
				Plugin.OEFoldersWindow = FindWindowRecursive(Plugin.OEMainWindow, _T("SysTreeView32"));

				if(Plugin.OEFoldersWindow) {
					// set that the folder list view will receive the keys sent using SendKey
					OEPluginToolbarMgr::Get()->GetMainWndMgr()->SetKeyMsgWnd(Plugin.OEFoldersWindow);
				}
			}

			if(Plugin.OEFoldersWindow) {
				if(Plugin.folderCb) {
					(*Plugin.folderCb)(Plugin.OEFoldersWindow);
				}

				Plugin.folderSelChangedEventPending = TRUE;
			}

			break;
		}		
	}
	if(msg == WM_OEAPI_CLOSE)
	{
		// OEAPI is closing main window
		SetProp(hWnd, _T("OEAPI_MainWndDestroying"), (HANDLE)TRUE);
		LRESULT lResult = CommonWndProc(Plugin.OldWinProc, hWnd, msg, wParam, lParam);
		OEPluginToolbarMgr::Get()->DestroyOEMainWindowTopLevelWndMgr();
		return lResult;
	}
	else if(msg == WM_STOREEVENT && wParam == 0 && lParam == 0) {
		//if(IsWMail()) {
		if(Plugin.storeRegisterEventsCb) {
			Plugin.storeRegisterEventsCb(FALSE);
		}
		//}
	}


	return CommonWndProc(Plugin.OldWinProc, hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CommonWndProc(WNDPROC lpOldProc, HWND hDlg, 
							   UINT msg, WPARAM wParam, 
							   LPARAM lParam)
{
	switch(msg)
	{
		case WM_COMMAND: // Called if we clicked on a menu or toolbar button
		{
			WORD id = LOWORD(wParam);

			OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id, FALSE);
			if(btn) {
				HWND msgWndId = INVALID_MSGWND_ID;
				if(hDlg != Plugin.OEMainWindow) {
					msgWndId = hDlg;
				}

				btn->NotifyButtonClick(msgWndId, id);
			}
			else {
				HWND msgWndId = INVALID_MSGWND_ID;

				// keep the value of msgWndId if the toolbar is not in the OE main wnd
				if(hDlg != Plugin.OEMainWindow) {
					msgWndId = hDlg;
				}
				OEPluginMenuMgr::Get()->NotifyMenuItemClick(msgWndId, id);
			}

			break;

		}
//		case WM_NOTIFY:
//		{
//			LPNMHDR lpnm = (LPNMHDR) lParam;
//
//			if(lpnm->code == TBN_DROPDOWN) {
//				LPNMTOOLBAR pnmtb = (LPNMTOOLBAR) lParam;
//
//				OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButtonInstance(lpnm->hwndFrom, pnmtb->iItem, FALSE);
//				if(btn) {
//					btn->CreateSubButtons();
//				}
//			}
//			break;
//		}
	}

	LRESULT res = CallWindowProc(lpOldProc, hDlg, msg, wParam, lParam);

	return res;
}

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HCBT_CREATEWND) {
		LPCBT_CREATEWND cw = (LPCBT_CREATEWND) lParam;
		LPCREATESTRUCT cs = (LPCREATESTRUCT) cw->lpcs;
		TCHAR *className = (TCHAR *) cs->lpszClass;
		HWND hWnd = (HWND) wParam;
		TCHAR aux[1024];

		if(!HIWORD(cs->lpszClass)) {
			if(GetClassName(hWnd, aux, sizeof(aux)/sizeof(TCHAR)-1) == 0) {
				className = NULL;
			}
			else {
				className = aux;
			}
		}

#ifdef ENTERPRISE_VERSION
		if(className && !_tcscmp(className, _T("ATH_Note"))) {
			MsgWndMgr.AddWindow(hWnd);
		}
		else if(className && cs->hwndParent && !_tcscmp(className, _T("##MimeEdit_Server"))) {
			if(IsChild(Plugin.OEMainWindow, cs->hwndParent)) {
				// verify if the window is a message window or the html object displayed
				// when OE root folder is selected.
				// if it has message listview it's a folder
				HWND msgWindow = GetMessageWindow(Plugin.OEMainWindow);

				// the current messages has changed and the html object that shows it
				// is created now.
				OEPluginMsgWnd *msgWnd = MsgWndMgr.AddWindow(hWnd, TRUE);

				if(msgWindow && msgWnd) {
					// call the creation callback if any
					if(Plugin.msgWndCb) {
						(*Plugin.msgWndCb)(hWnd, 0);
					}

					msgWnd->SetPreviewMsgWnd();
				}
			}
		}
		else if(className && !_tcscmp(className, _T("ToolbarWindow32"))) {
			// GUI thread doesn't need lock
			OEPluginMsgWnd *msgWnd = MsgWndMgr.GetCurrentMsgWnd(FALSE);

			if(msgWnd && !OEAPI_IsAnyMessageSelected()) {
				msgWnd->SetEmptyMsgWnd(TRUE);
			}
		}

#endif // ENTERPRISE_VERSION
		// if OEAPI was attached with a ATH_NOTE window the main window wasn't created
		// at that moment so it can be created after
		else if(className && Plugin.OEMainWindow == NULL && 
			(!_tcscmp(className, _T("ThorBrowserWndClass")) || 
			!_tcscmp(className, _T("Outlook Express Browser Class"))))	{				
			Plugin.OldWinProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
			Plugin.OEMainWindow = hWnd;
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)MainWndProc);
			debug_print(DEBUG_TRACE, _T("CallWndProc: Hook ThorBrowserWndClass: %08x : %08x -> %08x.\n"), hWnd, Plugin.OldWinProc, MainWndProc);
		}
	}

	// Call the next handler in the chain
    return CallNextHookEx(Plugin.hookHandle, nCode, wParam, lParam);
}


VOID Plugin_Init(VOID)
{
	memset(&Plugin, 0, sizeof(Plugin));
}

VOID Plugin_Shutdown (VOID)
{
}


HINSTANCE Plugin_GetInstance()
{
	return Plugin.hInstance;
}

HWND Plugin_GetMainWindow()
{
	return Plugin.OEMainWindow;
}


void AttachPlugin(HWND hwnd, HINSTANCE hInstance)
{
/////////////////////////// DLL MAIN
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return;
	}
#endif // EVALUATION_VERSION

	Plugin.hInstance = hInstance;

	Plugin_Init();

	// Menu_Initialize();
	//OEPluginMenuMgr::Get(hwnd)->Initialize(NULL, (OEPluginMsgWnd::WNDSTYLE)0);	

	NotificationAttach(hInstance, DLL_PROCESS_ATTACH);

	TCHAR szWndClass[1024];

	GetClassName(hwnd, szWndClass, sizeof(szWndClass)/sizeof(TCHAR)-1);

	// Set our own WinProc
	if ((!_tcscmp(szWndClass, _T("ThorBrowserWndClass")) || 
		!_tcscmp(szWndClass, _T("Outlook Express Browser Class"))))	{
		OEPluginMenuMgr::Get(hwnd)->Initialize(NULL, (OEPluginMsgWnd::WNDSTYLE)0);

		Plugin.OldWinProc = (WNDPROC) GetWindowLongPtr(hwnd, GWLP_WNDPROC);
		Plugin.OEMainWindow = hwnd;
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MainWndProc);
	}
	else if(!_tcscmp(szWndClass, _T("ATH_Note"))) {
		MsgWndMgr.AddWindow(hwnd);
	}

//	EnumWindows(CBTHookWindowsProc, GetCurrentProcessId());

	// hook to get the column changes dialog
//	Plugin.hookHandle = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC) CallWndRetProc, 0, GetCurrentThreadId());
	Plugin.hookHandle = SetWindowsHookEx(WH_CBT, (HOOKPROC) CallWndProc, 0, GetCurrentThreadId());
	if(Plugin.hookHandle == NULL) {
		debug_print(DEBUG_ERROR, _T("AttachPlugin: SetWindowsHookEx\n"));
	}
}


void DetachPlugin()
{
	if(Plugin.mainMsgWnd) {
		delete Plugin.mainMsgWnd;
		Plugin.mainMsgWnd = NULL;
	}

	if(Plugin.hookHandle) {
		UnhookWindowsHookEx(Plugin.hookHandle);
	}

	// Menu_Finalize();
	OEPluginMenuMgr::Get()->Finalize();
}

INT NotificationAttach(HINSTANCE hinstDll, DWORD fdwReason)
{
	return 0;
}

INT NotificationDetach(BOOL logOff)
{
	Plugin.OEMainWindow = NULL;
	Plugin.OEFoldersWindow = NULL;
	Plugin.OEMsgWindow = NULL;

	if(Plugin.detachCb) {
		(*Plugin.detachCb)(logOff);
	}
	
	return 0;
}

BOOL OEAPI_IsAnyMessageSelected()
{
	if(!IsWindow(Plugin.OEMsgWindow)) {
		return FALSE;
	}

	return (ListView_GetSelectedCount(Plugin.OEMsgWindow) != 0);
}

HWND OEAPI_GetOEMainWindowHandle()
{
	return Plugin.OEMainWindow;
}

HWND OEAPI_GetMsgListViewHandle()
{
	return Plugin.OEMsgWindow;
}

void OEAPI_SetDetachCallback(OEAPI_DetachCallback cb)
{
	Plugin.detachCb = cb;
}

void OEAPI_SetFolderSelChangedCallback(OEAPI_FolderSelChangedCallback cb)
{
	Plugin.folderCb = cb;
}

void OEAPI_SetMessageSelChangedCallback(OEAPI_MessageSelChangedCallback cb)
{
	Plugin.messageCb = cb;
}

void OEAPI_SetMessageDownloadCallback(OEAPI_MessageDownloadCallback cb)
{
	Plugin.messageDownloadCb = cb;
}

void OEAPI_SetMsgWndCallback(OEAPI_MsgWndCallback cb)
{
	Plugin.msgWndCb = cb;
}

void OEAPI_SetMsgWndSendButtonCallback(OEAPI_MsgWndSendBtnCallback cb)
{
	Plugin.msgWndSendButtonCb = cb;
}

void OEAPI_SetRegisterMessageStoreEvents(OEAPI_RegisterStoreEventsCallback cb)
{
	Plugin.storeRegisterEventsCb = cb;
}

void OEAPI_SetSendWindowCallback(OEAPI_SendWindowCallback cb)
{
	Plugin.sendWindowCb = cb;
}

HWND OEAPI_GetTreeViewHandle()
{
	return Plugin.OEFoldersWindow;
}

HWND OEAPI_GetMessageViewHandle()
{
	return Plugin.hMessageView;
}


void LockSendWindow(BOOL lock)
{
	if(Plugin.sendWindowCb) {
		(*Plugin.sendWindowCb)(lock ? SendWindow_Lock : SendWindow_Unlock);
	}
}

BOOL IsSendWindowLocked()
{
	if(Plugin.sendWindowCb) {
		(*Plugin.sendWindowCb)(SendWindow_Status);
	}
	return FALSE;
}

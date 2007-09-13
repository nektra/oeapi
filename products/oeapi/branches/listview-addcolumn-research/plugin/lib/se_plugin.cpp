/* $Id: se_plugin.cpp,v 1.36.4.4 2007/08/13 17:56:36 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
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

#include "..\oehook\se_hook.h"

#ifdef EVALUATION_VERSION
#include "eval_utils.h"
#endif // EVALUATION_VERSION

#include "oeapi_utils.h"

#include "oe_msgwnd.h"

#define MAX_COLUMN_TEXT_SIZE 150

// notify in the OE main window that arrives when OE Send/Receive
#define WM_POPACTION 0x08B6

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

// Unused
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

		ListView_SetItemState(Plugin.OEMsgWindow, indexs[0], LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

		for(int i=1; i<count; i++) {
			ListView_SetItemState(Plugin.OEMsgWindow, indexs[i], LVIS_SELECTED, LVIS_SELECTED|LVIS_FOCUSED);
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
	else if(msg == 0x13A0 && wParam == 0 && lParam == 0) {
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

HWND oldLH = NULL;
WNDPROC oldListHeaderWndProc = NULL;
HWND oldMV = NULL;
WNDPROC oldMessageViewWndProc = NULL;
HWND oldML = NULL;
WNDPROC oldMessageListWndProc = NULL;
HWND oldLV = NULL;
WNDPROC oldSysListViewWndProc = NULL;

int depth = 0;

class DebugSentry {
public:
	DebugSentry(LPCTSTR stag, LPCTSTR etag) : etag_(etag)
	{
		//OutputDebugString(stag); 
	}
	~DebugSentry() 
	{ 
		//OutputDebugString(etag_); 
	}
private:
	LPCTSTR etag_;
};

DWORD oeapiColumns[32];
DWORD oeapiColumns2[32];

//---------------------------------------------------------------------------------//
LRESULT CALLBACK ListHeaderWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case HDM_ORDERTOINDEX:
		{
			//debug_print(DEBUG_TRACE, _T("^^^^^^^> ListHeaderWndProc: %08x, HDM_ORDERTOINDEX %d, %08x <--- zero.\n"), hWnd, wParam, lParam);
			break;
		}
	case HDM_GETITEMRECT:
		{
			LPRECT pRect = (LPRECT)lParam;
			//debug_print(DEBUG_TRACE, _T("^^^^^^^> ListHeaderWndProc: %08x, HDM_GETITEMRECT %d, %08x .\n"), hWnd, wParam, lParam);
			//DWORD res = CallWindowProc(oldListHeaderWndProc, hWnd, msg, wParam, lParam);
			////if(wParam == 3) {
			//if(wParam == 0 && pRect->right - pRect->left > 100) {
			//	//debug_print(DEBUG_TRACE, _T("<<><>>%d.\n"), wParam);
			//	//pRect->left += 20;
			//}
			//return res;
			break;
		}
	case HDM_LAYOUT:
		{
			//debug_print(DEBUG_TRACE, _T("^^^^^^^> ListHeaderWndProc: %08x, HDM_LAYOUT %d, %08x .\n"), hWnd, wParam, lParam);
			break;
		}
	case HDM_INSERTITEMW:
		{
			//debug_print(DEBUG_TRACE, _T("^^^^^^^> ListHeaderWndProc: %08x, HDM_INSERTITEMW %d, %08x .\n"), hWnd, wParam, lParam);
			break;
		}
	case HDM_GETORDERARRAY:
		{
			static int q = 0;
			//debug_print(DEBUG_TRACE, _T("^^^^^^^> ListHeaderWndProc: %08x, HDM_GETORDERARRAY %d, %08x .\n"), hWnd, wParam, lParam);
			DWORD count = Header_GetItemCount(hWnd);
			if(count != wParam) {
				LPDWORD newOrder = oeapiColumns; //new DWORD[wParam+1];		// assumes wParam == 6
				LPDWORD prevOrder = (LPDWORD)lParam;
				if(q < 2) {
					LRESULT res = CallWindowProc(oldListHeaderWndProc, hWnd, msg, wParam+1, (LPARAM)newOrder);
					/* int col2 = newOrder[2];
					newOrder[2] = newOrder[wParam];
					newOrder[wParam] = col2; */
					//memcpy((LPVOID)lParam, newOrder, wParam * sizeof(DWORD));
					int i = 0;
					while(i<wParam && newOrder[i] != wParam) {
						prevOrder[i] = newOrder[i];
						i++;
					}
					while(i<wParam) {
						prevOrder[i] = newOrder[i+1];
						i++;
					}
					memcpy(oeapiColumns2, prevOrder, sizeof(DWORD) * wParam);
					q++;
					//delete [] newOrder;
					return res;
				}
				else {
					memcpy(prevOrder, oeapiColumns2, sizeof(DWORD) * wParam);
					return TRUE;
				}
			}
			//else {
			//}
			break;
		}
	case HDM_SETORDERARRAY:
		{
			//debug_print(DEBUG_TRACE, _T("^^^^^^^> ListHeaderWndProc: %08x, HDM_GETORDERARRAY %d, %08x .\n"), hWnd, wParam, lParam);
			break;
		}
	default:
		{
			//debug_print(DEBUG_TRACE, _T("^^^^^^^> ListHeaderWndProc: %08x, %08x %08x, %08x.\n"), hWnd, msg, wParam, lParam);
			break;
		}
	}
	//if(msg == WM_DESTROY) {
	//	LRESULT res = CallWindowProc(oldListHeaderWndProc, hWnd, msg, wParam, lParam);
	//	::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oldListHeaderWndProc);
	//	oldListHeaderWndProc = NULL;
	//	return res;
	//}
	return CallWindowProc(oldListHeaderWndProc, hWnd, msg, wParam, lParam);
}

//---------------------------------------------------------------------------------//
LRESULT CALLBACK SysListViewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//volatile DebugSentry s;
	switch(msg)	
	{
	case LVM_GETITEMCOUNT:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_GETITEMCOUNT %d %d.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_INSERTCOLUMNA:
		{
			debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_INSERTCOLUMNA %d %08x.\n"), hWnd, wParam, lParam);
			//* 
			if(wParam == 5) 
			{
				LRESULT res = CallWindowProc(oldSysListViewWndProc, hWnd, msg, wParam, lParam);
				LVCOLUMN column = {0};
				column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT; // | LVCF_IMAGE; // 
				column.cx = 24;
				column.fmt = LVCFMT_CENTER;
				column.iImage = 0;
				column.pszText = _T("q"); // _T("qq");
				CallWindowProc(oldSysListViewWndProc, hWnd, msg, 6, (LPARAM)&column);

				HIMAGELIST hImageList = ListView_GetImageList(hWnd, LVSIL_SMALL); //ListView_GetImageList(hWnd, LVSIL_NORMAL); //::SendMessage(hWnd, LVM_GETIMAGELIST, NULL, NULL);
				//HIMAGELIST h1, h2, h3, h4;
				//h1 = ListView_GetImageList(hWnd, LVSIL_NORMAL);
				//h2 = ListView_GetImageList(hWnd, LVSIL_SMALL);
				//h3 = ListView_GetImageList(hWnd, LVSIL_STATE);
				//h4 = ListView_GetImageList(hWnd, LVSIL_GROUPHEADER);
				//int count = ImageList_GetImageCount(hImageList);
				//HBITMAP hBitmap = (HBITMAP)::LoadImage(Plugin.hInstance, MAKEINTRESOURCE(104), IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
				//DWORD ind = ImageList_Add(hImageList, hBitmap, hBitmap);
				//count = ImageList_GetImageCount(hImageList);
				return res;
			} // */
			break;
		}
	case LVM_DELETECOLUMN:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_DELETECOLUMN %d %08x <-- zero.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_GETNEXTITEM:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_GETNEXTITEM %d %08x.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_SETIMAGELIST:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_SETIMAGELIST %d %08x.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_GETSELECTEDCOUNT:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_GETSELECTEDCOUNT %d %d.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_SETITEMSTATE:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_SETITEMSTATE %d %08x.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_SETCOLUMNORDERARRAY:
		{
			debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_SETCOLUMNORDERARRAY %d %08x.\n"), hWnd, wParam, lParam);
			// Add our column to the order array
			//* 
			LPDWORD newOrder = new DWORD[wParam+1];	// assumes wParam == 6
			LPDWORD prevOrder = (LPDWORD)lParam;

			//*
			for(int i = 0; i < 3; i++) {
				newOrder[i] = prevOrder[i];
			}
			newOrder[3] = wParam;
			for(int i = 3; i < wParam; i++) {
				newOrder[1+i] = prevOrder[i];
			} // */
			/* 
			int i = 0;
			while(i < wParam && prevOrder[i] != 0) {
				newOrder[i] = prevOrder[i];
				i++;
			}
			newOrder[i] = wParam;
			while(i < wParam) {
				newOrder[i+1] = prevOrder[i];
				i++;
			} // */

			//memcpy(newOrder, (LPVOID)lParam, wParam * sizeof(DWORD));
			//newOrder[wParam] = wParam;
			//int col2 = newOrder[2];
			//if(col2 != wParam) {
			//	newOrder[2] = wParam;
			//	newOrder[wParam] = col2;
			//}
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_SETCOLUMNORDERARRAY %d %08x.\n"), hWnd, wParam, lParam);
			LRESULT res = CallWindowProc(oldSysListViewWndProc, hWnd, msg, wParam+1, (LPARAM)newOrder);
			//LRESULT res = CallWindowProc(oldSysListViewWndProc, hWnd, msg, wParam, lParam);
			delete [] newOrder;
			return res; //*/
		}
	case LVM_GETCOLUMNORDERARRAY:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_GETCOLUMNORDERARRAY %d %08x.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_SETITEMCOUNT:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_SETITEMCOUNT %d %08x.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_GETITEMSTATE:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_GETITEMSTATE %d %08x.\n"), hWnd, wParam, lParam);
			break;
		}
	case LVM_GETHEADER:
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x LVM_GETHEADER %d %d <--- zero.\n"), hWnd, wParam, lParam);
			break;
		}
	case WM_NOTIFY:
		{
			//LPNMHDR pnmhdr = (LPNMHDR)lParam;
			////debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: %08x, %08x %08x, %08x.\n"), hWnd, msg, , wParam, lParam);
			//
			//switch(pnmhdr->code) 
			//{
			//	case NM_CUSTOMDRAW: 
			//	default:
			//		{
			//			debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x, %08x, %08x, %d, %08x, %08x.\n"), hWnd, msg, wParam, pnmhdr->code, pnmhdr->idFrom, pnmhdr->hwndFrom);
			//			break;
			//		}
			//}			
			break;
		}
	default: 
		{
			//debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x, %08x %08x, %08x.\n"), hWnd, msg, wParam, lParam);
			break;
		}
	}
	//if(msg == WM_DESTROY) {
	//	LRESULT res = CallWindowProc(oldSysListViewWndProc, hWnd, msg, wParam, lParam);
	//	::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oldSysListViewWndProc);
	//	oldSysListViewWndProc = NULL;
	//	return res;
	//}
	//if(msg == LVM_INSERTCOLUMNW || msg == LVM_SETCOLUMNW) {
	//if(msg == LVM_INSERTCOLUMNA || msg == LVM_SETCOLUMNA) {
	//	debug_print(DEBUG_TRACE, _T(".......> SysListViewWndProc: %08x, %08x %08x, %08x.\n"), hWnd, msg, wParam, lParam);
	//}
	//if(msg == LVM_INSERTCOLUMNA) {
	//}
	return CallWindowProc(oldSysListViewWndProc, hWnd, msg, wParam, lParam);
}

bool bf = false;

//---------------------------------------------------------------------------------//
LRESULT CALLBACK MessageListWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//volatile DebugSentry s(_T("]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]\n"), _T("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n"));
	//if(msg == WM_NOTIFY) {
	//	LPNMHDR pnmhdr = (LPNMHDR)lParam;
	//	//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: %08x, %08x %08x, %08x.\n"), hWnd, msg, , wParam, lParam);
	//	debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: %08x, %08x, %08x, %d, %08x, %08x.\n"), hWnd, msg, wParam, pnmhdr->code, pnmhdr->idFrom, pnmhdr->hwndFrom);
	//	/* switch(pnmhdr->code) {
	//		case NM_CUSTOMDRAW:
	//			break;
	//	} */
	//}
	//else {
	//	debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: %08x, %08x, %08x, %08x.\n"), hWnd, msg, wParam, lParam);
	//}
	if(msg == WM_NOTIFY) {
		LPNMHDR pnmhdr = (LPNMHDR)lParam;
		// I've to catch some messages but I dont know what exactly yet
		switch(pnmhdr->code)
		{
		case NM_CUSTOMDRAW: {
			LPNMLVCUSTOMDRAW pCustomDraw = (LPNMLVCUSTOMDRAW)lParam;			
			//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: NM_CUSTOMDRAW %08x %08x %d %d.\n"), pCustomDraw->nmcd.hdr.hwndFrom, pCustomDraw->nmcd.dwDrawStage, pCustomDraw->nmcd.dwItemSpec, pCustomDraw->iSubItem);
			////int oldSubItem = pCustomDraw->iSubItem;
			////if(pCustomDraw->iSubItem > 5) {
			////	pCustomDraw->iSubItem--;
			////}
			////if(pCustomDraw->nmcd.dwItemSpec >= 2) {
			////	pCustomDraw->nmcd.dwItemSpec--;
			////}
			////COLORREF clrTextBk = pCustomDraw->clrTextBk;
			////if((pCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) ||
			////	(pCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT | CDDS_SUBITEM)) {

			////	/* if(pCustomDraw->nmcd.dwItemSpec % 2) {
			////		pCustomDraw->clrTextBk = RGB(0x88, 0x22, 0xaa);
			////		if(pCustomDraw->nmcd.uItemState == CDIS_HOT) {
			////			pCustomDraw->clrText = RGB(0x00, 0x00, 0x00);
			////		}
			////		else {
			////			pCustomDraw->clrText = RGB(0xff, 0xff, 0xff);
			////		}
			////	} */
			////	//else {
			////	//	pCustomDraw->clrTextBk = RGB(0xff, 0xff, 0xff); 
			////	//	pCustomDraw->clrText = RGB(0xaa, 0x22, 0x88);
			////	//}
			////	//LRESULT res = CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);
			////	//pCustomDraw->nmcd.dwItemSpec = oldItem;
			////	//return res;
			////}
			////void* p = (LPVOID)pCustomDraw->nmcd.lItemlParam;
			////pCustomDraw->nmcd.lItemlParam
			////pCustomDraw->clrTextBk = clrTextBk;
			//LRESULT res;

			//// debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: NM_CUSTOMDRAW %08x %08x %d %d.\n"), pCustomDraw->nmcd.hdr.hwndFrom, pCustomDraw->nmcd.dwDrawStage, pCustomDraw->nmcd.dwItemSpec, pCustomDraw->iSubItem);


			////if(pCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
			////	if(pCustomDraw->iSubItem == 4) {
			////		pCustomDraw->nmcd.rc.left += 20;
			////	}
			////}
			////if(pCustomDraw->iSubItem == 6) {
			////	res = CDRF_NOTIFYITEMDRAW; // CDRF_NOTIFYITEMDRAW | CDRF_NEWFONT;
			////}
			////else {
			//	res = CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);

			////if(pCustomDraw->nmcd.dwDrawStage == CDDS_ITEM) {
			////	//if(pCustomDraw->iSubItem == 4) {
			////	//	pCustomDraw->nmcd.rc.left += 20;
			////	//}
			////}
			////}
			////CDRF_NOTIFYITEMDRAW
			////pCustomDraw->iSubItem = oldSubItem;
			//return res;
			break;
		}
		//case NM_CLICK: case NM_HOVER: case LVN_ODCACHEHINT: {
		//	//return FALSE;
		//	//break;
		//}
		case LVN_GETDISPINFOA:
			{
				LPNMLVDISPINFOA pdi = (LPNMLVDISPINFOA)lParam;
				//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: LVN_GETDISPINFO: %08x %d %d %08x.\n"), pdi->hdr.hwndFrom, pdi->item.iItem, pdi->item.iSubItem, pdi->item.mask);
				//* 
				if(pdi->item.iSubItem == 6) {
					if(pdi->item.mask & LVIF_IMAGE) {
						if(pdi->item.iItem % 5 == 2) {
							pdi->item.iImage = 37; // 0x1234;
						}
					}
					if(pdi->item.mask & LVIF_TEXT) {
						strcpy_s(pdi->item.pszText, pdi->item.cchTextMax, _T(""));
						//pdi->item.pszText = ""; // (LPWSTR)0xabcd;
						//pdi->item.cchTextMax = 
					}
					if(pdi->item.mask & LVIF_STATE) {
						pdi->item.stateMask = LVIS_STATEIMAGEMASK; //0x5678;	//
						pdi->item.state = 0; //0x9abc;
					}
					return FALSE;
				}
				else {
					LRESULT res = CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);
					if(pdi->item.iSubItem >= 1 && pdi->item.iSubItem <= 3) {
						debug_print(DEBUG_TRACE, _T("       > MessageListWndProc: LVN_GETDISPINFO: ret %08x %d.\n"), res, res);
					}
					return res;
				} // */
				//LRESULT res = CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);
				//if(pdi->item.iSubItem == 4) {
				//	LPSTR pstr = new TCHAR[pdi->item.cchTextMax];
				//	strcpy_s(pstr, pdi->item.cchTextMax, pdi->item.pszText);
				//	strcpy_s(pdi->item.pszText, pdi->item.cchTextMax, _T("__"));
				//	strcat_s(pdi->item.pszText, pdi->item.cchTextMax, pstr);
				//	delete [] pstr;
				//}
				//return res;
				break;
			}
		case LVN_GETDISPINFOW: {
			LPNMLVDISPINFOW pdi = (LPNMLVDISPINFOW)lParam;
			//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: LVN_GETDISPINFOW: %08x %d %d %08x.\n"), pdi->hdr.hwndFrom, pdi->item.iItem, pdi->item.iSubItem, pdi->item.mask);
			/* LPNMLVDISPINFOW info = (LPNMLVDISPINFOW)lParam;
			int oldSubItem = info->item.iSubItem;
			if(info->item.iSubItem > 0) {
				if(info->item.iSubItem > 1) {
					info->item.iSubItem--;
				}
			}
			LRESULT res = CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);
			info->item.iSubItem = oldSubItem;
			return res; */
			
			/* 
			if(pdi->item.iSubItem == 4) {
				if(pdi->item.mask & LVIF_IMAGE) {
					pdi->item.iImage = -1; // 0x1234;
				}
				if(pdi->item.mask & LVIF_TEXT) {
					pdi->item.pszText = L""; // (LPWSTR)0xabcd;
				}
				if(pdi->item.mask & LVIF_STATE) {
					pdi->item.stateMask = 0; //0x5678;	//
					pdi->item.state = 0; //0x9abc;
				}
				if(pdi->item.mask & LVIF_PARAM) {
					//pdi->item.lParam = 0xef01;
				}
				if(pdi->item.mask & LVIF_INDENT) {
					//pdi->item.iIndent = 0x4321;
				}
				//if(pdi->item.mask & 0x00000100) {
				//	//pdi->item
				//}
				return FALSE;
			}
			else {
				LRESULT res = CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);
				return res;
			} // */
			break;
		}
		/* case LVN_GETINFOTIPW: {
			LPNMLVGETINFOTIPW pdi = (LPNMLVGETINFOTIPW)lParam;
			debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: LVN_GETINFOTIPW %d %d.\n"), pdi->iItem, pdi->iSubItem);
			if(pdi->iSubItem > 5) {
				pdi->pszText = NULL;
				return FALSE;
			}
			break;
		} */
		/* case LVN_ITEMCHANGED: {
			LPNMLISTVIEW pdi = (LPNMLISTVIEW)lParam;
			debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: LVN_ITEMCHANGED %d %d.\n"), pdi->iItem, pdi->iSubItem);
			if(pdi->iSubItem > 5) {
				return FALSE;
			}
			break;
		} */
		case LVN_HOTTRACK: {
			LPNMLISTVIEW pdi = (LPNMLISTVIEW)lParam;
			//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: LVN_HOTTRACK %d %d.\n"), pdi->iItem, pdi->iSubItem);
			/* if(pdi->iSubItem > 5) {
				return FALSE;
			} */
			break;
		}
		case NM_SETFOCUS: 
			{
				//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: %08x, NM_SETFOCUS %08x, %d, %08x, %08x.\n"), hWnd, wParam, pnmhdr->code, pnmhdr->idFrom, pnmhdr->hwndFrom);
				/* if(!bf) {
					// Doesn't seems to be safe toi add a header here
					bf = true;
					HWND hListView = ::FindWindowEx(hWnd, NULL, !IsWMail() ? _T("ATL:SysListView32") : _T("SysListView32"), NULL);
					if(hListView) {
						LVCOLUMN column = {0};
						column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
						column.cx = 50;
						column.fmt = LVCFMT_LEFT;
						column.pszText = _T("mono");
						INT index = (INT)::SendMessage(hListView, LVM_INSERTCOLUMN, 6, (LPARAM)&column);
						debug_print(DEBUG_TRACE, _T("```````> MessageListWndProc: %08x %d.\n"), hListView, index);
					}
				} */
				break;
			}
		case HDN_ITEMCHANGINGW: {
			LPNMHEADERW phi = (LPNMHEADERW)lParam;
			//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: HDN_ITEMCHANGINGW %d.\n"), phi->iItem);
			/* if(phi->iItem > 5) {
				return TRUE;
			} */
			break;
		}
		case HDN_ITEMCHANGEDW: {
			LPNMHEADERW phi = (LPNMHEADERW)lParam;
			//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: HDN_ITEMCHANGINGW %d.\n"), phi->iItem);
			/* if(phi->iItem > 5) {
				return TRUE;
			} */
			break;
		}
		default: {
			//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: %08x, %08x, %08x, %d, %08x, %08x.\n"), hWnd, msg, wParam, pnmhdr->code, pnmhdr->idFrom, pnmhdr->hwndFrom);
		}
		}
	}
	else {
		//debug_print(DEBUG_TRACE, _T("+++++++> MessageListWndProc: %08x, %08x, %08x, %08x.\n"), hWnd, msg, wParam, lParam);
	}
	//if(msg == WM_DESTROY) {
	//	LRESULT res = CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);
	//	::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oldMessageListWndProc);
	//	oldMessageListWndProc = NULL;
	//	return res;
	//}
	return CallWindowProc(oldMessageListWndProc, hWnd, msg, wParam, lParam);
}

HWND hPrevML = NULL;

//---------------------------------------------------------------------------------//
LRESULT CALLBACK MessageViewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//volatile DebugSentry s(_T(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"), _T("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"));
	//LRESULT res;
	debug_print(DEBUG_TRACE, _T("-------> MessageViewWndProc: %08x, %08x %08x, %08x.\n"), hWnd, msg, wParam, lParam);

	switch(msg) { // == WM_PARENTNOTIFY) {
		case WM_PARENTNOTIFY: 
			{
				if(LOWORD(wParam) == WM_CREATE) {
					LRESULT res = ::CallWindowProc(oldMessageViewWndProc, hWnd, msg, wParam, lParam);

					HWND hMsgList = (HWND)lParam;
					if(hMsgList) {
						if(hPrevML) {
							::SetWindowLongPtr(hPrevML, GWL_WNDPROC, (LONG_PTR)oldMessageListWndProc);
							oldMessageListWndProc = NULL;
						}
						hPrevML = hMsgList;
						//TCHAR className[1024];
						//::GetClassName(hMsgList, className, sizeof(className));
						//debug_print(DEBUG_TRACE, _T("=======> MessageViewWndProc: %s.\n"), className);
						oldMessageListWndProc = (WNDPROC)::GetWindowLongPtr(hMsgList, GWLP_WNDPROC);
						::SetWindowLongPtr(hMsgList, GWL_WNDPROC, (LONG_PTR)MessageListWndProc);

						HWND hSysListView = ::FindWindowEx(hMsgList, NULL, !IsWMail() ? _T("ATL:SysListView32") : _T("SysListView32"), NULL);
						if(hSysListView) {
							//if(!oldSysListViewWndProc) {
							if(oldLV) {
								::SetWindowLongPtr(oldLV, GWLP_WNDPROC, (LONG_PTR)oldSysListViewWndProc);
								oldSysListViewWndProc = NULL;
							}
							oldSysListViewWndProc = (WNDPROC)::GetWindowLongPtr(hSysListView, GWLP_WNDPROC);
							::SetWindowLongPtr(hSysListView, GWL_WNDPROC, (LONG_PTR)SysListViewWndProc);
							oldLV = hSysListView;

							if(oldLH) {
								::SetWindowLongPtr(oldLH, GWLP_WNDPROC, (LONG_PTR)oldListHeaderWndProc);
								oldListHeaderWndProc = NULL;
							}
							HWND hListHeader = (HWND)::SendMessage(hSysListView, LVM_GETHEADER, 0, 0);
							oldListHeaderWndProc = (WNDPROC)::GetWindowLongPtr(hListHeader, GWLP_WNDPROC);
							::SetWindowLongPtr(hListHeader, GWL_WNDPROC, (LONG_PTR)ListHeaderWndProc);
							oldLH = hListHeader;
							//}
						}// */
					}

					return res;
				}
				break;
			}
	}
	//if(msg == WM_DESTROY) {
	//	LRESULT res = CallWindowProc(oldMessageViewWndProc, hWnd, msg, wParam, lParam);
	//	::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oldMessageViewWndProc);
	//	oldMessageViewWndProc = NULL;
	//	return res;
	//}
	return CallWindowProc(oldMessageViewWndProc, hWnd, msg, wParam, lParam);
}

HWND hPrevList = NULL;

//---------------------------------------------------------------------------------//
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
		//else if(className && !_tcscmp(className, _T("Outlook Express Message List"))) {
		else if(className && !_tcscmp(className, _T("Outlook Express Message View"))) {
			if(hPrevList) {
				SetWindowLongPtr(hPrevList, GWL_WNDPROC, (LONG_PTR)oldMessageViewWndProc);
				oldMessageViewWndProc = NULL;
			}
			// Call the next handler in the chain
			//LRESULT res = CallNextHookEx(Plugin.hookHandle, nCode, wParam, lParam);
			hPrevList = hWnd;
			oldMessageViewWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
			SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG_PTR)MessageViewWndProc);
			//return res;
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
			SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG_PTR)MainWndProc);
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

		Plugin.OldWinProc = (WNDPROC) GetWindowLongPtr(hwnd, GWL_WNDPROC);
		Plugin.OEMainWindow = hwnd;
		SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)MainWndProc);
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

HWND OEAPI_GetTreeViewHandle()
{
	return Plugin.OEFoldersWindow;
}

HWND OEAPI_GetMessageViewHandle()
{
	return Plugin.hMessageView;
}


/* $Id: se_plugin.h,v 1.25 2009/01/27 19:29:59 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef _SE_OE6_PLUGIN_H_
#define _SE_OE6_PLUGIN_H_

#include <commctrl.h>
#include <tchar.h>

//#include "se_menu.h"
#include "oeapi.h"

class OEPluginMsgWnd;

typedef
struct _Plugin
{
	WNDPROC OldWinProc; // old window proc of the main window
//	HWND ATLSystListView32; // List Tree view window handle
	WNDPROC ATLSystListView32OldProc; // ATLSystListView32 Window Proc
	WNDPROC ATLSystListView32ParentOldProc; // ATLSystListView32 Parent Window Proc
	BOOL MsgListViewSelPending;		// If true the initial selection of the msgs wasn't got
	BOOL RegenerateMsgList;			// If true a message was deleted and the message list must be regeerated
	WNDPROC ATLSystListView32HeaderOldProc;
	WNDPROC ColumnsOldProc;
	WNDPROC HeaderPopupOldProc;
	HWND ColumnsListViewHandle;
	HWND OEMainWindow; // OE main window
	HWND OEFoldersWindow; // OE Folders window
	HWND OEMsgWindow; // OE Message List window
	HWND hMessageView; // "Outlook Express Message View" window
	HINSTANCE hInstance;   // hintance of the dll
	// this event is triggered when the main window is destroyed to unload the plug-in and
	// when there are no more message windows. The parameter indicates if the main window
	// was destroyed.
	OEAPI_DetachCallback detachCb;
	OEAPI_FolderSelChangedCallback folderCb;
	OEAPI_MessageSelChangedCallback messageCb;
//	OEAPI_MessageOrderChangedCallback orderChangedCb;
//	OEAPI_HeaderItemsOrderChangedCallback headerOrderCb;
//	OEAPI_HeaderOrderRegeneratedCallback headerOrderRegeneratedCb;
	OEAPI_MessageDownloadCallback messageDownloadCb;
	// this event is triggered when a new message window is created.
	OEAPI_MsgWndCallback msgWndCb;
	// this event is triggered when a Send button of the msg wnd is pressed.
	OEAPI_MsgWndSendBtnCallback msgWndSendButtonCb;
	BOOL folderSelChangedEventPending;  // flag indicates that a new folder was selected and the message window wasn't create yet
	TCHAR lastSelFolder[1024];   // last selected folder
	HHOOK hookHandle;			// handle to a hook of the applications messages
//	HHOOK hookHandleFind;		// handle to a hook of the 'Find Message' window that is in another thread.
	BOOL selectionBlocked;

	// this object represents the message displayed in the preview panel in the main window
	OEPluginMsgWnd *mainMsgWnd;

	// hook handle of the WH_CBT hook
//	HHOOK hhook;
	// flag indicates if the dialog that is shown when the user reorganizes the columns
	// was created. It is used to find the SysListView32 window that is create after
	BOOL colDlgCreated;
	OEAPI_RegisterStoreEventsCallback storeRegisterEventsCb;
	OEAPI_SendWindowCallback sendWindowCb;
} OEPlugin, *POEPlugin, **PPOEPlugin;


/* ------------------- */
/* Function Prototypes */
/* ------------------- */

// Callbacks

// Function called to process WM_COMMAND msgs to verify if it was triggered by a button
// of OEAPI or a menu item.
// It is used for sub buttons to display the sub buttons when the user press the arrow.
LRESULT CALLBACK CommonWndProc(WNDPROC lpOldProc, HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI CBTProc (int nCode, WPARAM wParam, LPARAM lParam);

// Others
VOID Plugin_Init (VOID);
VOID Plugin_Shutdown (VOID);

VOID Plugin_SimulateSelectAll(HWND hwnd);
					   
HINSTANCE Plugin_GetInstance();
HWND Plugin_GetMainWindow();

int error(void);

void LockSendWindow(BOOL lock);

BOOL IsSendWindowLocked();


// Global Plugin

extern OEPlugin Plugin;

#endif /* _SE_OE6_PLUGIN_H_ */

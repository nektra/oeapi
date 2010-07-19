/* $Id: oeapi.h,v 1.32 2009/01/27 19:29:58 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __OEAPI_H__
#define __OEAPI_H__

#include <windows.h>
#include <tchar.h>

#define TRANSPARENT_COLOR	RGB(255, 0, 0)

const HWND INVALID_MSGWND_ID = ((HWND)-1L);

struct MsgWndStyles {
	typedef  enum tagWNDSTYLE {
		OE_MSG_INVALID = 0,
		/**
		Window opened when the user dbl clicks a message.
		*/
		OE_MSG_DETAIL_WND = 0x0001,
		/**
		Windows opened when the user press 'Create Mail' / Forward / Reply / Reply All.
		*/
		OE_MSG_SEND_WND = 0x0002,
		/**
		Window displayed in the bottom part of OE that shows the current message in OE main window.
		*/
		OE_CURRENT_MSG_WND = 0x0004,
		/**
		Window displayed in the bottom part of OE that shows a text that says that there is no message selected.
		*/
		OE_EMPTY_MSG_WND = 0x0008,
		/**
		Window displayed when OE root folder is selected.
		*/
		OE_OTHER_WND = 0x0010,
		/**
		All message windows.
		*/
		OE_ALL_MSG_WND = OE_MSG_DETAIL_WND | OE_CURRENT_MSG_WND | OE_MSG_SEND_WND | OE_OTHER_WND
	} WNDSTYLE ;
};

enum {
	SendWindow_Lock = 1,
	SendWindow_Unlock = 2,
	SendWindow_Status = 3,
};

extern const UINT CREATE_MSG_TABLES_CODE;

void AttachPlugin(HWND, HINSTANCE);
void DetachPlugin();


INT NotificationAttach(HINSTANCE hinstDll, DWORD fdwReason);
INT NotificationDetach(BOOL logOff);

///////////////////////////// NEW OEAPI
typedef INT (__cdecl *OEAPI_MenuItemCallBack)(HWND, INT, INT);
typedef INT (__cdecl *OEAPI_ButtonCallBack)(INT, INT, HWND);
// logOff: TRUE if OEAPI is detaching because of the user is logging off or shutdowning
typedef INT (__cdecl *OEAPI_DetachCallback)(BOOL logOff);

// first: is the folder name.
// second: the depth of the item. if it is 1, it is a root folder of a branch (like 'Local folders').
// third: TRUE if it is a local folder.
//typedef INT (__cdecl *OEAPI_FolderSelChangedCallback)(TCHAR *, INT, BOOL);
typedef INT (__cdecl *OEAPI_FolderSelChangedCallback)(HWND);

// the first parameter is the length of the selection and the second a array of indexed of
// the selected items.
// the third parameter is the focus item.
// if the forth parameter is true the message list should be regenerated as a delete ocurred
typedef INT (__cdecl *OEAPI_MessageSelChangedCallback)(INT,INT *,INT,BOOL);

// the first is the item clicked and the second the direction
// if the direction if -1 means that the item was clicked so if the item is the same that
// was active, the order just change the direction. If it is another, the item is the new
// order item and the direction is the same that it was.
// if the direction is 1 the order is ascendant, or 0 if descendant
typedef INT (__cdecl *OEAPI_MessageOrderChangedCallback)(INT, INT);

// first integer is the column number dragged and the second where it was dropped
typedef INT (__cdecl *OEAPI_HeaderItemsOrderChangedCallback)(INT, INT);

// the integer specifies the wParam of the download event:
//
typedef INT (__cdecl *OEAPI_MessageDownloadCallback)(INT);

// the integer specifies the id of the msg wnd.
// the second parameter:
// 0: creation
// 1: initialization
// 2: close
typedef INT (__cdecl *OEAPI_MsgWndCallback)(HWND, INT);

// the integer specifies the id of the msg wnd.
//
typedef INT (__cdecl *OEAPI_MsgWndSendBtnCallback)(HWND, BOOL);

// first integer is the length of the new order array and the second the order array itself.
// This order array is not the header order array, it's the internal order array of OE.
// all the columns in this order array are absolute numbers that identify always the same
// column
typedef INT (__cdecl *OEAPI_HeaderOrderRegeneratedCallback)(INT,INT *);

typedef VOID (__cdecl *OEAPI_RegisterStoreEventsCallback)(BOOL);

typedef INT (__cdecl *OEAPI_SendWindowCallback)(INT);

/**
Return TRUE if any message is selected int the Messages List View
*/
BOOL OEAPI_IsAnyMessageSelected();

/**
Return OE main window handle
*/
HWND OEAPI_GetOEMainWindowHandle();

/**
Return the Messages List View handle
*/
HWND OEAPI_GetMsgListViewHandle();

/**
Add a item to a popup menu.
before specifies the name of the item before the new one. If empty, the item is inserted at the end.
cb is the function called when the button is pressed.
subString (only valid if before is not empty) indicates that the string specified in before
is a substring of the search item.
*/
//INT OEAPI_AddMenuItem(INT menuIndex, const PTCHAR name, const PTCHAR before, BOOL subString, OEAPI_MenuItemCallBack cb);

/**
Add a item as a sub item of a added item.
cb is the function called when the button is pressed.
*/
//INT OEAPI_AddMenuSubItem(INT parentItemId, const PTCHAR name, OEAPI_MenuItemCallBack cb);

/**
Remove a menu item.
*/
//VOID OEAPI_RemoveMenuItem(INT itemId);

/**
Set the enabled property of the menu item.
*/
//VOID OEAPI_SetMenuItemEnabled(INT menuItemId, BOOL enabled);

/**
Get the enabled property of the menu item.
*/
//BOOL OEAPI_GetMenuItemEnabled(INT menuItemId);

/**
Set the check property of the menu item.
If true the item is of check style.
*/
//VOID OEAPI_SetMenuItemCheck(INT menuItemId, BOOL check);

/**
Set the checked property of the menu item.
This property is only available when the item is of check style.
*/
//VOID OEAPI_SetMenuItemChecked(INT menuItemId, BOOL checked);

/**
Get the checked property of the menu item.
This property is only available when the item is of check style.
*/
//BOOL OEAPI_GetMenuItemChecked(INT menuItemId);

/**
Set the callback that will be called when the plugin is detached
*/
void OEAPI_SetDetachCallback(OEAPI_DetachCallback cb);

/**
Set the callback called when the selected folder change
*/
void OEAPI_SetFolderSelChangedCallback(OEAPI_FolderSelChangedCallback cb);

/**
Set the callback called when the selected message change
*/
void OEAPI_SetMessageSelChangedCallback(OEAPI_MessageSelChangedCallback cb);

/**
Set the callback called when the order item of the messages list view changes.
*/
void OEAPI_SetMessageListViewOrderItemChangedCallback(OEAPI_MessageOrderChangedCallback cb);

/**
Set the callback called when the header of the message list view changes the order of the
columns.
*/
void OEAPI_SetMessageListViewHeaderColumnOrderChangedCallback(OEAPI_HeaderItemsOrderChangedCallback cb);

/**
Set the callback called when the header of the message list view changes the order of the
columns radically. This callback is called after the columns dialogs is displayed and the
user press ok. The new internal OE order array is sent in the callback.
*/
void OEAPI_SetMessageListViewHeaderColumnOrderRegeneratedCallback(OEAPI_HeaderOrderRegeneratedCallback cb);

/**
Set the callback called when OE starts downloaded or uploading messages.
*/
void OEAPI_SetMessageDownloadCallback(OEAPI_MessageDownloadCallback cb);

/**
Set the callback called when a new message window is created.
*/
void OEAPI_SetMsgWndCallback(OEAPI_MsgWndCallback cb);

/**
Set the callback called when OE starts downloaded or uploading messages.
If this callback is set, the responsability of pressing the 'Send' button is from the caller
to this callback.
*/
void OEAPI_SetMsgWndSendButtonCallback(OEAPI_MsgWndSendBtnCallback cb);

/**
This function clears the selection and select these items.
*/
void OEAPI_SetSelectedMessage(INT count, INT *indexs);

/**
This function keeps the selection and adds index to it.
*/
void OEAPI_AddSelectedMessage(INT index);

/**
Set message store events callback
*/
void OEAPI_SetRegisterMessageStoreEvents(OEAPI_RegisterStoreEventsCallback cb);

/**
Set send window events callback
*/
void OEAPI_SetSendWindowCallback(OEAPI_SendWindowCallback cb);

/**
Returns the folder tree view handle.
*/
HWND OEAPI_GetTreeViewHandle();

/**
Returns the message view window handle.
*/
HWND OEAPI_GetMessageViewHandle();

///////////////////////////// NEW OEAPI END

#endif

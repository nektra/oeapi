/* $Id: oe_menu.h,v 1.7.8.2 2007/08/13 17:56:36 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2006-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __OE_MENU_H
#define __OE_MENU_H


#include <windows.h> // for windows types
#include <vector>
#include <string>


#include "oeapi.h"

typedef INT (__cdecl *OEAPI_CallBack)(INT, INT, INT);

enum InsertState {
	item_pending_insert,
	item_inserted,
	item_pending_delete,
	item_deleted,
	item_pending_hide,
	item_pending_show,
	item_pending_update,
};

class OEPluginMenuMgr;
class OEPluginMenuItem;

////////////////////////////////////////////////////////////////////////////
// Menu Item
class OEPluginMenuItem
{
public:
	typedef MsgWndStyles::WNDSTYLE WNDSTYLE;

	friend class OEPluginMenuMgr;
	friend class OEPluginMenuItemListMgr;
	friend class OEPluginMenuMgr;

	OEPluginMenuItem(INT menuID = -1,
		INT menuIndex = -1, 
		LPCTSTR name = NULL);
	~OEPluginMenuItem();

	BOOL IsEnabled() { return _enabled; }
	VOID SetEnabled(BOOL enabled) { _enabled = enabled; }

	BOOL IsItemCheck() { return _isCheck; }
	VOID SetItemCheck(BOOL isCheck) { _isCheck = isCheck; }

	BOOL IsChecked() { return _checked; }
	VOID SetChecked(BOOL checked) { _checked = checked; }

	OEAPI_MenuItemCallBack GetCallBack() { return _cb; }
	VOID SetCallback(OEAPI_MenuItemCallBack cb) { _cb = cb; }

	const std::basic_string<TCHAR>& GetName() { return _name; }
	VOID SetName(LPCTSTR name) { _name = name; }

	INT GetIndex() { return _index; }
	VOID SetIndex(INT index) { _index = index; }

	HMENU GetItemMenu() { return _itemMenu; }
	VOID SetItemMenu(HMENU itemMenu) { _itemMenu = itemMenu;}

	InsertState GetInsState() { return _insState; }
	VOID SetInsState(InsertState insState) { _insState = insState; }

	INT GetParentId() { return _parentId; } 
	VOID SetParentId(INT parentId) { _parentId = parentId; }

	HMENU GetParentMenu() { return _parentMenu; }
	VOID SetParentMenu(HMENU parentMenu) { _parentMenu = parentMenu; }

	const std::basic_string<TCHAR>& GetBefore() { return _before; }
	VOID SetBefore(LPCTSTR before) { _before = before; }

	BOOL IsBefore() { return !_before.empty(); }

	BOOL IsBeforeSubstr() { return _beforeSubStr; }
	VOID SetBeforeSubstr(BOOL beforeSubstr) { _beforeSubStr = beforeSubstr; }

	VOID SetId(INT Id) { _id = Id;}
	INT GetId() { return _id; }

	VOID SetWndStyle(WNDSTYLE style) { _style = style; }
	WNDSTYLE GetWndStyle() { return _style; }

	VOID SetVisible(BOOL visible); // { _visible = visible; }
	BOOL IsVisible() { return _visible; }

	LPCTSTR GetText(LPTSTR text, DWORD size);
	VOID SetText(LPCTSTR text);

	INT GetPosition() { return _position; }
	VOID SetPosition(INT position) { _position = position; } 

	/**
	Make a clone of the menu item
	*/
	OEPluginMenuItem* Clone(WNDSTYLE style);

protected:

private:
	WNDSTYLE _style;
	// HWND _msgWnd;	// handle to the message window
	INT _index;
	InsertState _insState;
	HMENU _parentMenu;
	HMENU _itemMenu;  // used for items that have subItems
	INT _parentId;
	INT _position;
	//TCHAR _before[256];
	std::basic_string<TCHAR> _before;
	BOOL _beforeSubStr;
	BOOL _enabled;
	BOOL _isCheck;   // is the button a check button
	BOOL _checked;   // if the above is true it indicates the check state
	BOOL _visible;
	//TCHAR _name[256];
	std::basic_string<TCHAR> _name;
	OEAPI_MenuItemCallBack _cb;
	INT _id;
};

typedef std::vector<OEPluginMenuItem*> OEPluginMenuItemList;


////////////////////////////////////////////////////////////////////////////
//
class OEPluginMenuItemListMgr
{
public:
	typedef MsgWndStyles::WNDSTYLE WNDSTYLE;

	friend class OEPluginMenuMgr;

	~OEPluginMenuItemListMgr();

	void EnableAllMenus(HWND hWnd);
	
	/**
	Remove all menu items from the list
	*/
	void Clear();

	void AddItem(OEPluginMenuItem* item);
	
	/**
	Remove item from list return true if removed
	*/
	bool RemoveItem(INT itemId);

	void RemoveFromList(INT itemId);
	
	VOID InsertMenuItems(WNDSTYLE wndStyle, HMENU hMenu, INT index);

	void Lock();
	void Unlock();

	void Clone(WNDSTYLE wndStyle, OEPluginMenuItemListMgr* itemList);

	OEPluginMenuItem* GetMenuItemByName(PTCHAR MenuName);
	OEPluginMenuItem* GetMenuItemById(INT id);

	/**
	Check if menu item is valid
	*/
	BOOL PrepareItem(OEPluginMenuItem* item, HMENU hMenu);

	/**
	Insert menu item from hMenu
	*/
	BOOL InsertItem(OEPluginMenuItem* item, HMENU hMenu);

	/**
	Delete menu item from hMenu
	*/
	BOOL DeleteItem(OEPluginMenuItem* item, HMENU hMenu);

	/**
	Hide menu item
	*/
	BOOL HideItem(OEPluginMenuItem* item, HMENU hMenu);

	/**
	Show menu item
	*/
	BOOL ShowItem(OEPluginMenuItem* item, HMENU hMenu);

	/**
	*/
	INT FindItemPosition(OEPluginMenuItem* item, HMENU hMenu);

	/**
	*/
	BOOL CheckItem(OEPluginMenuItem* item, WNDSTYLE wndStyle, INT index);

	/**
	*/
	VOID ProcessMenuItem(OEPluginMenuItem* item, HMENU hMenu);

protected:
	// OEPluginMenuItemListMgr();
	OEPluginMenuItemListMgr(MsgWndStyles::WNDSTYLE wndStyle = MsgWndStyles::OE_MSG_INVALID, OEPluginMenuItemListMgr* itemList = NULL);

private:
	CRITICAL_SECTION _menuItemsCS;
	OEPluginMenuItemList _menuItemList;
};


// There's only one plugin manager
extern OEPluginMenuMgr* __MenuManager;

////////////////////////////////////////////////////////////////////////////
//
class OEPluginMenuMgr
{
public:
	typedef MsgWndStyles::WNDSTYLE WNDSTYLE;

	OEPluginMenuMgr(HWND hWnd, MsgWndStyles::WNDSTYLE wndStyle = MsgWndStyles::OE_MSG_INVALID, OEPluginMenuMgr* mgr = NULL);
	~OEPluginMenuMgr();

	/**
	Get menu manager
	*/
	static OEPluginMenuMgr* Get(HWND hWnd = NULL) {
		if(__MenuManager == NULL) {
			__MenuManager = new OEPluginMenuMgr(hWnd);
		}
		return __MenuManager;
	}

	void Initialize(HWND hWnd, WNDSTYLE style);
	void Finalize();

	void InsertMenuItems(HMENU hMenu, INT index);

	void EnableAllMenus(HWND hWnd = NULL);

	/**
	Add a menu item
	*/
	INT AddMenuItem(WNDSTYLE wndStyle, INT menuIndex, LPCTSTR name, LPCTSTR before, BOOL subString, OEAPI_MenuItemCallBack cb, INT position = -1);

	/**
	Add a menu sub item
	*/
	INT AddMenuSubItem(INT parentItemId, LPCTSTR name, OEAPI_MenuItemCallBack cb);

	/**
	Obtain a menu item by id
	*/
	OEPluginMenuItem* GetMenuItem(INT itemId);

	//VOID RemoveFromList(INT itemId);	
	VOID RemoveMenuItem(INT itemId);

	VOID SetMenuItemEnabled(INT menuItemId, BOOL enabled);
	BOOL GetMenuItemEnabled(INT menuItemId);

	VOID SetMenuItemCheck(INT menuItemId, BOOL check);
	BOOL GetMenuItemCheck(INT menuItemId);

	VOID SetMenuItemChecked(INT menuItemId, BOOL checked);
	BOOL GetMenuItemChecked(INT menuItemId);

	VOID SetMenuItemVisible(INT menuItemId, BOOL visible);
	BOOL GetMenuItemVisible(INT menuItemId);

	VOID SetMenuItemText(INT menuItemId, LPCTSTR text);
	LPCTSTR GetMenuItemText(INT menuItemId, LPTSTR test, DWORD size);

	int GetNewMenuItemId() { return _index++; }

	/**
	Notify menu item click, return TRUE if proccesed
	*/
	BOOL NotifyMenuItemClick(HWND msgWndId, INT id);

	VOID DestroyMenuItem(INT itemId);

	OEPluginMenuMgr* CreateMsgWndMgr(HWND hWnd, WNDSTYLE wndStyle);

	void Lock();
	void Unlock();

protected:
	OEPluginMenuItemListMgr* GetMenuItemListMgr() { return &_menuItemList; }

private:
	CRITICAL_SECTION _menuMgrCS;
	OEPluginMenuItemListMgr _menuItemList;
	WNDSTYLE _wndStyle;
	LONG _index;
};





#endif // __OE_MENU_H


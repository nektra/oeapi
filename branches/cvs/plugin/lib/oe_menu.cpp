/* $Id: oe_menu.cpp,v 1.15 2008/09/07 16:56:45 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2006-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/


#include <windows.h>
#include "oeapi.h"

#include "oe_menu.h"
#include "se_debug.h"

#include <algorithm>
#include <functional>
#include <comet/assert.h>

/**
Menu manager
*/
OEPluginMenuMgr* __MenuManager;


////////////////////////////////////////////////////////////////////////////
// OEPluginMenuItem

//-----------------------------------------------------------//
OEPluginMenuItem::OEPluginMenuItem(INT menuId, INT menuIndex, LPCTSTR name)
{
	_style = MsgWndStyles::OE_MSG_INVALID;
	//_msgWnd = NULL;
	_id = menuId;
	_name = (name != NULL) ? name : _T("");
	//_before[0] = '\0';
	_before = _T("");
	_beforeSubStr = FALSE;
	_cb = NULL;
	_index = menuIndex;
	_insState = item_pending_insert;
	_isCheck = FALSE;
	_checked = FALSE;
	_enabled = TRUE;
	_visible = TRUE;
	_parentMenu = NULL;
	_parentId = -1;
	_itemMenu = NULL;
	_position = -1;
}

//-----------------------------------------------------------//
OEPluginMenuItem::~OEPluginMenuItem()
{
}

//-----------------------------------------------------------//
OEPluginMenuItem* OEPluginMenuItem::Clone(WNDSTYLE style)
{
	OEPluginMenuItem* item = NULL;
	item = new OEPluginMenuItem(_id, _index, _name.c_str());
	if(item)
	{
		item->_before = _before;
		item->_style = _style;
		item->_beforeSubStr = _beforeSubStr;
		item->_insState = item_pending_insert;
		item->_isCheck = _isCheck;
		item->_checked = _checked;
		item->_enabled = _enabled;
		item->_visible = _visible;
		item->_parentMenu = NULL;
		item->_parentId = _parentId;
		item->_itemMenu = NULL;
		item->_position = _position;
		item->_cb = _cb;
	}
	return item;
}

//-----------------------------------------------------------//
LPCTSTR OEPluginMenuItem::GetText(LPTSTR text, DWORD size) 
{
	_tcscpy_s(text, size, _name.c_str());
	return text; 
}

//-----------------------------------------------------------//
void OEPluginMenuItem::SetText(LPCTSTR text) 
{ 
	_name = text;
	if(_insState == item_inserted)
	{
		if(::IsMenu(_parentMenu)) {
			::ModifyMenu(_parentMenu, _id, MF_BYCOMMAND | MF_STRING, _id, text);
		}
	}
	//_insState = item_pending_update;
}

//-----------------------------------------------------------//
VOID OEPluginMenuItem::SetVisible(BOOL visible)
{ 
	if(visible == _visible)
	{
		return;
	}

	_visible = visible; 
	_insState = item_pending_update;
}

////////////////////////////////////////////////////////////////////////////
// OEPluginMenuItemListMgr
//-----------------------------------------------------------//
OEPluginMenuItemListMgr::OEPluginMenuItemListMgr(WNDSTYLE wndStyle /* = menu_main_wnd */, OEPluginMenuItemListMgr* itemList /* = NULL */)
{
	InitializeCriticalSection(&_menuItemsCS);

	Lock();

	if(itemList)
	{
		itemList->Clone(wndStyle, this);
	}

	Unlock();
}

//-----------------------------------------------------------//
OEPluginMenuItemListMgr::~OEPluginMenuItemListMgr()
{
	Clear();	
	DeleteCriticalSection(&_menuItemsCS);
}

//-----------------------------------------------------------//
void OEPluginMenuItemListMgr::EnableAllMenus(HWND hWnd)
{
	MENUITEMINFO menuInfo;

	Lock();

	OEPluginMenuItemList::iterator it = _menuItemList.begin();

	for(; it != _menuItemList.end(); it++)
	{
		OEPluginMenuItem* item = *it;
		menuInfo.cbSize = sizeof(MENUITEMINFO);
		menuInfo.fMask = MIIM_STATE;
		
		menuInfo.fState = (item->IsEnabled() ? MFS_ENABLED : MFS_DISABLED);
		if(item->IsItemCheck()) {
			menuInfo.fState |= (item->IsChecked() ? MFS_CHECKED : MFS_UNCHECKED);
		}

		::SetMenuItemInfo(item->GetParentMenu(), item->GetId(), 
						FALSE, &menuInfo);		
	}

	Unlock();
}

//-----------------------------------------------------------//
void OEPluginMenuItemListMgr::AddItem(OEPluginMenuItem* item)
{
	Lock();

	_menuItemList.push_back(item);

	Unlock();
}

//-----------------------------------------------------------//
bool OEPluginMenuItemListMgr::RemoveItem(INT itemId)
{
	bool ret = false;

	Lock();

	OEPluginMenuItem* item = GetMenuItemById(itemId);

	if(item)
	{
		if(item->GetInsState() == item_pending_insert)
		{
			RemoveFromList(itemId);
		}
		// is a child?
		else if(item->GetParentId() != -1)
		{
			OEPluginMenuItem* parent = GetMenuItemById(item->GetParentId());
			if(parent)
			{
				if(parent->GetItemMenu() != NULL)
				{
					if(::GetMenuItemCount(parent->GetItemMenu()) == 1)
					{
						MENUITEMINFO menuInfo;

						menuInfo.cbSize = sizeof(menuInfo);
						menuInfo.fMask = MIIM_SUBMENU;
						menuInfo.hSubMenu = NULL;
						::SetMenuItemInfo(parent->GetParentMenu(), 
							parent->GetId(),
							FALSE,
							&menuInfo);

						::DestroyMenu(parent->GetItemMenu());
						parent->SetItemMenu(NULL);							
					}
				}
				else
				{
					::DeleteMenu(parent->GetItemMenu(), 
						item->GetId(),
						MF_BYCOMMAND);
				}
			}
			RemoveFromList(itemId);
		}
		else
		{
			item->SetInsState(item_pending_delete);
		}

		ret = true;
	}

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
VOID OEPluginMenuItemListMgr::RemoveFromList(INT itemId)
{
	Lock();

	OEPluginMenuItemList::iterator it = _menuItemList.begin();
	OEPluginMenuItemList::iterator last = it;

	for(; it != _menuItemList.end(); it++)
	{
		if((*it)->GetId() == itemId || (*it)->GetParentId() == itemId)
		{
			delete *it;
			*it = NULL;
		}
		else
		{
			*last++ = *it;
		}
	}

	_menuItemList.erase(last, _menuItemList.end());

	Unlock();
}

//-----------------------------------------------------------//
void OEPluginMenuItemListMgr::Clear()
{
	Lock();

	OEPluginMenuItemList::iterator it = _menuItemList.begin();
	for(; it != _menuItemList.end(); it++)
	{
		OEPluginMenuItem* item = *it;
		delete item;
		*it = NULL;
	}

	Unlock();
}

//-----------------------------------------------------------//
OEPluginMenuItem* OEPluginMenuItemListMgr::GetMenuItemByName(PTCHAR menuName)
{
	OEPluginMenuItem* ret = NULL;

	Lock();

	OEPluginMenuItemList::iterator it = _menuItemList.begin();

	for(; it != _menuItemList.end(); it++)
	{
		if((*it)->GetName() == menuName)
		{
			ret = *it;
			break;
		}
	}

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
OEPluginMenuItem* OEPluginMenuItemListMgr::GetMenuItemById(INT id)
{
	OEPluginMenuItem* ret = NULL;

	Lock();

	OEPluginMenuItemList::iterator it = _menuItemList.begin();

	for(; it != _menuItemList.end(); it++)
	{
		if((*it)->GetId() == id)
		{
			ret = *it;
			break;
		}
	}

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
BOOL OEPluginMenuItemListMgr::PrepareItem(OEPluginMenuItem* item, HMENU hMenu)
{
	MENUITEMINFO menuInfo;
	HMENU parentMenu;

	if(item->GetInsState() == item_pending_insert)
	{
		return TRUE;
	}
	else
	{
		// When switching views the menu items can be destroyed
		// we must ensure the menu items is present

		parentMenu = (item->GetParentId() == -1) ? hMenu : item->GetParentMenu();

		menuInfo.cbSize = sizeof(menuInfo);
		menuInfo.fMask = MIIM_ID;

		// Check if the item is valid
		BOOL valid = ::GetMenuItemInfo(hMenu, item->GetId(), FALSE, &menuInfo);

		if(!valid || item->GetInsState() == item_pending_update)
		{
			// non valid item, insert it
			item->SetInsState(item_pending_insert);

			if(valid) 
			{
				::DeleteMenu(item->GetParentMenu(), item->GetId(), MF_BYCOMMAND);				
			}

			if(item->GetItemMenu())
			{
				DestroyMenu(item->GetItemMenu());
				item->SetItemMenu(NULL);
			}
			else if(item->GetParentId() != -1)
			{
				OEPluginMenuItem* parent = GetMenuItemById(item->GetParentId());
				if(parent)
				{
					item->SetParentMenu(parent->GetItemMenu());
				}
			}
		}
	}

	return TRUE;
}

//-----------------------------------------------------------//
BOOL OEPluginMenuItemListMgr::InsertItem(OEPluginMenuItem* item, HMENU hMenu)
{
	//debug_print(DEBUG_INFO, _T("OEPluginMenuItemListMgr::InsertItem: %d\n"), item->GetId());
	//int pos = menuItemCount;
	int position = -1;
	MENUITEMINFO menuInfo;
	

	if(item->GetParentId() != -1)
	{
		OEPluginMenuItem* parent = GetMenuItemById(item->GetParentId());

		// PredefMenus[parentIndex].insState must be item_inserted because the sub item is
		// inserted after its parent
		if(parent && parent->GetInsState() == item_inserted)
		{
			// the item is a simple item or a popup. If not set as a popup
			if(parent->GetItemMenu() == NULL)
			{
				menuInfo.cbSize = sizeof(menuInfo);
				menuInfo.fMask = MIIM_SUBMENU;
				menuInfo.hSubMenu = CreatePopupMenu();

				parent->SetItemMenu(menuInfo.hSubMenu);

				::SetMenuItemInfo(parent->GetParentMenu(), 
					parent->GetId(), 
					FALSE, 
					&menuInfo);
			}
		}

		position = ::GetMenuItemCount(parent->GetItemMenu());
		item->SetParentMenu(parent->GetItemMenu());
	}
	else
	{
		if(item->IsBefore())
		{
			position = FindItemPosition(item, hMenu);

			if(position == -1)
			{
				debug_print(DEBUG_INFO, _T("InsertMenuItems: Cannot find before entry\n. Item inserted at the end."));
				// pos = menuItemCount;
			}
		}
		else if(item->GetPosition() >= 0)
		{
			position = item->GetPosition();
		}
		item->SetParentMenu(hMenu);
	}

	int count = GetMenuItemCount(item->GetParentMenu());
	if(position < 0 || position > count)
	{
		position = count;
	}

	TCHAR szMenu[256];
	if(!item->GetName().empty()) 
	{
#if _MSC_VER >= 1400
		_tcscpy_s(szMenu, sizeof(szMenu), item->GetName().c_str());
#else
		_tcsncpy(szMenu, item->GetName().c_str(), sizeof(szMenu)/sizeof(TCHAR)-1);
#endif
	}
	else
	{
		szMenu[0] = '\0';
	}

	menuInfo.cbSize = sizeof(menuInfo);
	menuInfo.fMask = MIIM_TYPE | MIIM_ID | MIIM_STATE;
	menuInfo.fType = (!item->GetName().empty()) ? MFT_STRING : MFT_SEPARATOR;
	menuInfo.fState = MFS_ENABLED | (item->IsChecked() ? MFS_CHECKED : 0);
	menuInfo.wID = item->GetId();
	menuInfo.dwTypeData = szMenu;

	::InsertMenuItem(item->GetParentMenu(), position, TRUE, &menuInfo);
	item->SetInsState(item_inserted);

	return TRUE;
}

//-----------------------------------------------------------//
INT OEPluginMenuItemListMgr::FindItemPosition(OEPluginMenuItem* item, HMENU hMenu)
{
	INT pos = -1;
	MENUITEMINFO menuInfo;
	INT menuItemCount = ::GetMenuItemCount(hMenu);

	TCHAR szMenu[1024];

	for(int j=0; j<menuItemCount; j++)
	{
		szMenu[0] = '\0';

		menuInfo.cbSize = sizeof(menuInfo);
		menuInfo.fMask = MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
		menuInfo.dwTypeData = szMenu;
		menuInfo.cch = sizeof(szMenu)/sizeof(szMenu[0]) - 1; // 255

		::GetMenuItemInfo(hMenu, j, TRUE, &menuInfo);
		if((item->IsBeforeSubstr() && item->GetBefore() == szMenu)
			|| (item->IsBeforeSubstr() && _tcsstr(szMenu, item->GetBefore().c_str())))
		{
			// Found!
			pos = j;
			break;
		}
	}

	return pos;
}

//-----------------------------------------------------------//
BOOL OEPluginMenuItemListMgr::DeleteItem(OEPluginMenuItem* item, HMENU hMenu)
{
	::DeleteMenu(hMenu, item->GetId(), MF_BYCOMMAND);
	// item->SetInsState()

	return TRUE;
}

//-----------------------------------------------------------//
BOOL OEPluginMenuItemListMgr::HideItem(OEPluginMenuItem* item, HMENU hMenu)
{
	//::DeleteMenu(hMenu, item->GetId(), MF_BYCOMMAND);
	DeleteItem(item, hMenu);

	return TRUE;
}

//-----------------------------------------------------------//
BOOL OEPluginMenuItemListMgr::ShowItem(OEPluginMenuItem* item, HMENU hMenu)
{
	InsertItem(item, hMenu);
	return TRUE;
}

//-----------------------------------------------------------//
BOOL OEPluginMenuItemListMgr::CheckItem(OEPluginMenuItem* item, WNDSTYLE wndStyle, INT index)
{
	BOOL ret = TRUE;
	if(item->GetIndex() != index)
	{
		ret = FALSE;
	}
	// check if main windows
	else if(wndStyle == MsgWndStyles::OE_MSG_INVALID && item->GetWndStyle() != wndStyle)
	{
		ret = FALSE;
	}
	else if((item->GetWndStyle() & wndStyle) != wndStyle)
	{
		ret = FALSE;
	}
	return ret;
}

//-----------------------------------------------------------//
VOID OEPluginMenuItemListMgr::ProcessMenuItem(OEPluginMenuItem* item, HMENU hMenu)
{
	// Check if menu item is valid mark to insert it is not
	PrepareItem(item, hMenu);

	if(item->GetInsState() == item_pending_insert && item->IsVisible())
	{
		InsertItem(item, hMenu);
	}
	else if(item->GetInsState() == item_pending_delete)
	{
		DeleteItem(item, hMenu);
	}
}


//-----------------------------------------------------------//
VOID OEPluginMenuItemListMgr::InsertMenuItems(WNDSTYLE wndStyle, HMENU hMenu, INT index)
{
	Lock();

	// Process first menuitems by position
	OEPluginMenuItemList::iterator it = _menuItemList.begin();
	for(; it != _menuItemList.end(); it++)
	{
		OEPluginMenuItem* item = *it;
		if(item->GetPosition() >= 0 && CheckItem(item, wndStyle, index) )
		{
			ProcessMenuItem(item, hMenu);
		}
	}

	it = _menuItemList.begin();
	for(; it != _menuItemList.end(); it++)
	{
		OEPluginMenuItem* item = *it;
		if(item->GetPosition() < 0 && CheckItem(item, wndStyle, index) )
		{
			ProcessMenuItem(item, hMenu);
		}
	}

	Unlock();
}

//-----------------------------------------------------------//
void OEPluginMenuItemListMgr::Clone(WNDSTYLE wndStyle, OEPluginMenuItemListMgr* itemList)
{
	Lock();

	OEPluginMenuItem* item;
	OEPluginMenuItemList::iterator it = _menuItemList.begin();

	for(; it != _menuItemList.end(); it++)
	{
		item = (*it)->Clone(wndStyle);
		if(item)
		{
			itemList->AddItem(item);
		}
	}

	Unlock();
}

//-----------------------------------------------------------//
void OEPluginMenuItemListMgr::Lock()
{
	EnterCriticalSection(&_menuItemsCS);
}

//-----------------------------------------------------------//
void OEPluginMenuItemListMgr::Unlock()
{
	LeaveCriticalSection(&_menuItemsCS);
}


////////////////////////////////////////////////////////////////////////////
// OEPluginMenuMgr

//-----------------------------------------------------------//
OEPluginMenuMgr::OEPluginMenuMgr(HWND hWnd, WNDSTYLE wndStyle /* = menu_main_wnd */, OEPluginMenuMgr* mgr /* = NULL */)
	:_menuItemList(wndStyle, mgr ? mgr->GetMenuItemListMgr() : NULL)
{
	InitializeCriticalSection(&_menuMgrCS);
	if(mgr)	{
		_index = mgr->_index;
	}
	else {
		COMET_ASSERT(hWnd != NULL);
		LONG index = (LONG)GetProp(hWnd, _T("OEAPI_MenuNextId"));
		
		if(index == 0) {
			index = 50101;
		}
		SetProp(hWnd, _T("OEAPI_MenuNextId"), (HANDLE)(index + 1000));
		_index = index;
	}
	_wndStyle = wndStyle;
}

//-----------------------------------------------------------//
OEPluginMenuMgr::~OEPluginMenuMgr()
{
	Lock();

	_menuItemList.Clear();

	Unlock();

	DeleteCriticalSection(&_menuMgrCS);

	// if we are deallocation main menu manager
	// remove reference
	if(this == __MenuManager)
	{
		__MenuManager = NULL;
	}
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::Initialize(HWND hWnd, WNDSTYLE style)
{
	_wndStyle = style;
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::Finalize()
{
	delete this;
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::InsertMenuItems(HMENU hMenu, INT index)
{
	Lock();
	
	_menuItemList.InsertMenuItems(_wndStyle, hMenu, index);

	Unlock();
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::EnableAllMenus(HWND hWnd /* = NULL */)
{
	Lock();

	_menuItemList.EnableAllMenus(hWnd);

	Unlock();
}

//-----------------------------------------------------------//
INT OEPluginMenuMgr::AddMenuItem(WNDSTYLE wndStyle, INT menuIndex, LPCTSTR name, LPCTSTR before, BOOL subString, OEAPI_MenuItemCallBack cb, INT position)
{
	INT ret = -1;

	Lock();

	OEPluginMenuItem* item;
	INT mID = GetNewMenuItemId();
	item = new OEPluginMenuItem(mID, menuIndex, name);
	if(item)
	{
		item->SetWndStyle(wndStyle);
		if(position >= 0) 
		{
			item->SetPosition(position);
		}
		else 
		{
			item->SetBefore(before);
			item->SetBeforeSubstr(subString);
		}
		item->SetCallback(cb);
		_menuItemList.AddItem(item);
		ret = item->GetId();
	}

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
INT OEPluginMenuMgr::AddMenuSubItem(INT parentItemId, LPCTSTR name, OEAPI_MenuItemCallBack cb)
{
	INT ret = -1;

	Lock();

	OEPluginMenuItem* parentItem = _menuItemList.GetMenuItemById(parentItemId);
	if(parentItem)
	{
		OEPluginMenuItem* item;
		INT mID = GetNewMenuItemId();
		item = new OEPluginMenuItem(mID, parentItem->GetIndex(), name); //, NULL, FALSE, cb);
		item->SetCallback(cb);
		item->SetParentId(parentItem->GetId());
		item->SetWndStyle(parentItem->GetWndStyle()); // same style as parent

		_menuItemList.AddItem(item);

		ret = item->GetId();
	}

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::RemoveMenuItem(INT itemId)
{
	Lock();

	_menuItemList.RemoveItem(itemId);

	Unlock();
}

//-----------------------------------------------------------//
OEPluginMenuItem* OEPluginMenuMgr::GetMenuItem(INT itemId)
{
	OEPluginMenuItem* ret = NULL;

	Lock();

	ret = _menuItemList.GetMenuItemById(itemId);

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::SetMenuItemEnabled(INT menuItemId, BOOL enabled)
{
	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);

	if(item)
	{
		item->SetEnabled(enabled);
	}

	Unlock();
}

//-----------------------------------------------------------//
BOOL OEPluginMenuMgr::GetMenuItemEnabled(INT menuItemId)
{
	BOOL ret = FALSE;

	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item)
	{
		ret = item->IsEnabled();
	}

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::SetMenuItemCheck(INT menuItemId, BOOL check)
{
	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item)
	{
		item->SetItemCheck(check);
	}

	Unlock();
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::SetMenuItemChecked(INT menuItemId, BOOL checked)
{
	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item)
	{
		item->SetChecked(checked);
	}

	Unlock();
}

//-----------------------------------------------------------//
BOOL OEPluginMenuMgr::GetMenuItemChecked(INT menuItemId)
{
	BOOL ret = FALSE;

	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item)
	{
		ret = item->IsChecked();
	}

	Unlock();

	return ret;
}

//-----------------------------------------------------------//
VOID OEPluginMenuMgr::SetMenuItemVisible(INT menuItemId, BOOL visible)
{
	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item)
	{
		item->SetVisible(visible);
	}

	Unlock();
}

//------------------------------------------------------------------//
BOOL OEPluginMenuMgr::GetMenuItemVisible(INT menuItemId)
{
	BOOL ret = FALSE;
	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item)
	{
		ret = item->IsVisible();
	}

	Unlock();
	return ret;
}

//------------------------------------------------------------------//
LPCTSTR OEPluginMenuMgr::GetMenuItemText(INT menuItemId, LPTSTR text, DWORD size)
{
	LPCTSTR ret = NULL;
	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item) {
		ret = item->GetText(text, size);
	}

	Unlock();
	return ret;
}

//------------------------------------------------------------------//
VOID OEPluginMenuMgr::SetMenuItemText(INT menuItemId, LPCTSTR text)
{
	Lock();

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(menuItemId);
	if(item) {
		item->SetText(text);
	}

	Unlock();
}

//-----------------------------------------------------------//
BOOL OEPluginMenuMgr::NotifyMenuItemClick(HWND msgWndId, INT id)
{
	BOOL ret = FALSE;
	LONG param = 0;

	OEPluginMenuItem* item = _menuItemList.GetMenuItemById(id);
	OEAPI_MenuItemCallBack cb = NULL;
	if(item)
	{
		cb = item->GetCallBack();
		if(item->IsItemCheck())
		{
			// only change state for menu items that are displayed
			// ie there are in a message window msgWndId != -1
			//    or there main window items GetWndStyle() == menu_main_wnd
			//if(msgWndId != -1 || (msgWndId == -1 && item->GetWndStyle() == menu_main_wnd))
			//{
				item->SetChecked(!item->IsChecked());
			//}
		}
		param = item->GetIndex();
	}

	if(cb)
	{
		(*cb)(msgWndId, (INT) param, id);
		ret = TRUE;
	}

	return ret;
}

//-----------------------------------------------------------//
VOID OEPluginMenuMgr::DestroyMenuItem(INT itemId)
{
	RemoveMenuItem(itemId);
	// OEAPIManager::Get()->DestroyMenuItem(itemId);
}

//-----------------------------------------------------------//
OEPluginMenuMgr* OEPluginMenuMgr::CreateMsgWndMgr(HWND hWnd, WNDSTYLE wndStyle)
{
	return new OEPluginMenuMgr(hWnd, wndStyle, this);
}

//-----------------------------------------------------------//
void OEPluginMenuMgr::Lock()
{
	EnterCriticalSection(&_menuMgrCS);
}


//-----------------------------------------------------------//
void OEPluginMenuMgr::Unlock()
{
	LeaveCriticalSection(&_menuMgrCS);
}


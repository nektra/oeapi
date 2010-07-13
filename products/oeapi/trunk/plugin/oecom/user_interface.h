/* $Id: user_interface.h,v 1.36 2008/09/07 16:56:54 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#pragma once

#include "auto_ptr.h"

#include <list>
#include <comet/bstr.h>

typedef std::list<int> IntList;

class OEPluginToolbar;
class OEPluginMenuMgr;

//using namespace comet;

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY

class TOEMenuItem;
typedef AutoPtr<TOEMenuItem> TOEMenuItemPtr;

class TOEMenuItem

#else

#define TOEMenuItemPtr comet::com_ptr<comet::OEAPI::IOEMenuItem>
#define TOEMenuItem coclass_implementation<comet::OEAPI::OEMenuItem>

template <> class TOEMenuItem : public comet::coclass<comet::OEAPI::OEMenuItem>

#endif // STATIC_LIBRARY
{
	friend class TOEMenu;
public:
	TOEMenuItem();
	~TOEMenuItem();

	static TCHAR *get_progid() { return _T("OEAPI.OEMenuItem"); }

	void Create(int menuID, const comet::bstr_t &name);

	void CreateBefore(int menuID, const comet::bstr_t &name, const comet::bstr_t &before, long subString);

	void CreateAt(long menuID, const comet::bstr_t& name, long par_position);

	TOEMenuItemPtr CreateSubItem(const comet::bstr_t &name);

	void Destroy();

	void SetEnabled(BOOL enabled);

	/**
	Create a separator as child of this menu item.
	*/
	TOEMenuItemPtr CreateSeparator();

	/**
	Set that the item is a check.
	*/
	void SetCheck(BOOL check);

	/**
	Set the checked property for check style items.
	*/
	void SetChecked(BOOL checked);

	/**
	If the item is of check style this flag indicates if it is checked.
	*/
	BOOL IsChecked();

	/**
	Set item as visible
	*/
	void SetVisible(BOOL visible);

	int GetID() { return id_; }

	void SetID(int id) { id_ = id; }

	int GetMenuID() { return menuId_; }

	void SetMenuID(int id) { menuId_ = id; }

	int GetMenuIndex() { return menuIndex_; }

	void SetMenuIndex(int index) { menuIndex_ = index; }

	void OnClick();

	void OnMsgWndClick(int wndMsgId);

	void SetWndStyle(int wndStyle) { wndStyle_ = wndStyle; }
	int GetWndStyle() { return wndStyle_; }

	BOOL IsVisible();
	BOOL IsEnabled();

	comet::bstr_t GetText();
	void SetText(const comet::bstr_t& text);

	void SetMenuMgr(OEPluginMenuMgr* menuMgr) { menuMgr_ = menuMgr; }
	OEPluginMenuMgr* GetMenuMgr();

#ifdef STATIC_LIBRARY
	static TOEMenuItem *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static comet::com_ptr<comet::OEAPI::IOEMenuItem> newInstance();
#endif // STATIC_LIBRARY

private:
	int id_;
	int menuId_;
	int menuIndex_;
	int refCount_;
	int wndStyle_;
	OEPluginMenuMgr* menuMgr_;
};

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY


class TOEMenu;
typedef AutoPtr<TOEMenu> TOEMenuPtr;

class TOEMenu

#else

#define TOEMenuPtr comet::com_ptr<comet::OEAPI::IOEMenu>
#define TOEMenu coclass_implementation<comet::OEAPI::OEMenu>

template <> class TOEMenu : public comet::coclass<comet::OEAPI::OEMenu>

#endif // STATIC_LIBRARY
{
public:
	TOEMenu();
	~TOEMenu();

	static TCHAR *get_progid() { return _T("OEAPI.OEMenu"); }

	TOEMenuItemPtr CreateItem(const comet::bstr_t &name);

	TOEMenuItemPtr CreateItemBefore(const comet::bstr_t &name, const comet::bstr_t &before, long subString);

    TOEMenuItemPtr CreateItemAt(const comet::bstr_t& name, long position);

	TOEMenuItemPtr CreateSeparator();

	TOEMenuItemPtr CreateSeparatorBefore(const comet::bstr_t &before, long subString);

    TOEMenuItemPtr CreateSeparatorAt(long position);

	int GetIndex() { return index_; }

	int GetOEAPIID() { return oeapiId_; }

	void SetOEAPIID(int id) { oeapiId_ = id; }

	void SetIndex(int index) { index_ = index; }

	void SetID(int id) { id_ = id; }

	int GetID() { return id_; }

	void SetWndStyle(int wndStyle) { wndStyle_ = wndStyle; }
	int GetWndStyle() { return wndStyle_; }

	void SetMenuMgr(OEPluginMenuMgr* menuMgr) {  menuMgr_ = menuMgr; }
	OEPluginMenuMgr* GetMenuMgr();

//	bstr_t Name();

	void OnItemClicked(int menuItemId);

	void OnMsgWndItemClicked(int menuItemId, int wndMsgId);

#ifdef STATIC_LIBRARY
	static TOEMenu *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static comet::com_ptr<comet::OEAPI::IOEMenu> newInstance();
#endif // STATIC_LIBRARY

private:
	int oeapiId_;
	int index_;
	int id_;
	IntList menuItems_;
	int refCount_;
	int wndStyle_;
	OEPluginMenuMgr* menuMgr_;
};

// forward declare
class OEPluginTopLevelWndMgr;
class OEPluginToolbar;
class OEPluginToolbarBtn;

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY

class TOEButton;
typedef AutoPtr<TOEButton> TOEButtonPtr;

class TOEButton

#else

#define TOEButtonPtr comet::com_ptr<comet::OEAPI::IOEButton>
#define TOEButton coclass_implementation<comet::OEAPI::OEButton>

template <> class TOEButton : public comet::coclass<comet::OEAPI::OEButton>

#endif // STATIC_LIBRARY
{
	friend class TOEToolbar;
public:
	TOEButton();
	~TOEButton();

	static TCHAR *get_progid() { return _T("OEAPI.OEButton"); }

	/**
	Creates the button in the toolbar
	*/
	void Create(int toolbarId, const comet::bstr_t &name, const comet::bstr_t &bmpNormal, const comet::bstr_t &bmpMouseOver);

	/**
	Create a button with separator style.
	*/
	void CreateSeparator(int toolbarId);

	/**
	Destroy the button
	*/
	void Destroy();

	/**
	Creates a button inside this button.
	The new button is displayed if the user presses the arrow to see the options of the
	top-level button.
	*/
	TOEButtonPtr CreateSubButton(const comet::bstr_t &name, const comet::bstr_t &bmpNormal, const comet::bstr_t &bmpMouseOver);

	/**
	Add a separator to the drop down menu.
	*/
	TOEButtonPtr CreateSubSeparator();

	/**
	Set the button name.
	*/
	void SetName(const comet::bstr_t &name);

	/**
	Set the image displayed in the button.
	*/
	void SetImageNormal(const comet::bstr_t &bmp);

	/**
	Set the image displayed in the button when the mouse pointer in on the button.
	*/
	void SetImageMouseOver(const comet::bstr_t &bmp);

	/**
	Returns TRUE if it is a separator.
	*/
	BOOL IsSeparator();

	/**
	Set if the button has the popup style.
	Only for buttons with sub buttons: if the button has popup style when is pressed
	it shows the sub buttons instead of generate a OnClick event.
	*/
	void SetPopupStyle(BOOL popup);

	/**
	Set tooltip.
	*/
	void SetTooltip(const comet::bstr_t& tooltip);

	void SetID(int id) { id_ = id; }
	int GetID() { return id_; }

	comet::bstr_t GetName();

	int GetToolbarID() { return toolbarId_; }
	void SetToolbarID(int toolbarId) { toolbarId_ = toolbarId; }

	void SetEnabled(BOOL enabled);
	void SetShown(BOOL show);

	// Iterator
	int GetFirstSubButtonID();
	int GetNextSubButtonID();

	TOEButtonPtr GetSubButtonByIndex(int index);

	void OnClick();

//#ifdef ENTERPRISE_VERSION
	/**
	Event triggered when the button is clicked in a message window.<br>
	Only useful when the button is clicked in a msg window.
	*/
	void OnMsgWndClick(int msgWndId);
//#endif // ENTERPRISE_VERSION

	void SetMsgWndMgr(OEPluginTopLevelWndMgr* msgWndMgr) { msgWndMgr_ = msgWndMgr; }

	void SetCheckStyle(BOOL check);
	void SetChecked(BOOL checked);
	BOOL IsChecked();

	BOOL IsVisible();
	BOOL IsEnabled();

	void SetPicture(const comet::com_ptr<::IPictureDisp>& normal);
	void SetPictureMouseOver(const comet::com_ptr<::IPictureDisp>& over);

	OEPluginToolbarBtn* GetButtonInternal();
	OEPluginToolbar* GetToolbarInternal();

#ifdef STATIC_LIBRARY
	static TOEButton *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static comet::com_ptr<comet::OEAPI::IOEButton> newInstance();
#endif // STATIC_LIBRARY

private:
	int id_;
	int curSubBtnIndex_;
	int toolbarId_;
	int refCount_;
	OEPluginToolbarBtn* button_;
	OEPluginTopLevelWndMgr* msgWndMgr_;
};

// forward reference
class OEPluginTopLevelWndMgr;

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY

class TOEToolbar;
typedef AutoPtr<TOEToolbar> TOEToolbarPtr;

class TOEToolbar

#else

#define TOEToolbarPtr comet::com_ptr<comet::OEAPI::IOEToolbar>
#define TOEToolbar coclass_implementation<comet::OEAPI::OEToolbar>

template <> class TOEToolbar : public comet::coclass<comet::OEAPI::OEToolbar>

#endif // STATIC_LIBRARY
{
public:
	TOEToolbar();
	~TOEToolbar();

	static TCHAR *get_progid() { return _T("OEAPI.OEToolbar"); }

	void Create(int oeapiId);

	void Create();

	/**
	Get / Create function of the plugin object.
	*/
	OEPluginToolbar *GetToolbarObject(BOOL unlockObj=FALSE);

	/**
	Destroy the toolbar
	*/
	void Destroy();

	/**
	Set that the toolbar is the default OE toolbar.
	if wndstyle if OE_MSG_INVALID the toolbar is from the main window.
	*/
	void SetOEDefaultToolbar(int wndStyle, int oeapiId);

	/**
	Enable tooltips
	*/
	void EnableTooltips(BOOL bEnable);

	int GetID() { return id_; }

	void SetID(int id) { id_ = id; }

	int GetOEAPIID() { return oeapiId_; }

	void SetOEAPIID(int id) { oeapiId_ = id; }

	void SetShown(BOOL show);

	void SetLargeButtons(BOOL largeBtns);

	void SetMaxTextRows(INT rows);

	void SetMaskColor(LONG color);
	LONG GetMaskColor();

	BOOL GetPictureSize(long& cx, long& cy);
	BOOL SetPictureSize(long cx, long cy);

	/**
	Create a new button in the toolbar
	*/
	TOEButtonPtr CreateButton(const comet::bstr_t &name, const comet::bstr_t &bmpNormal, const comet::bstr_t &bmpMouseOver);

	/**
	Create an empty button
	*/
	//TOEButtonPtr AddButton();

	/**
	Create a toolbar separator.
	*/
	TOEButtonPtr CreateSeparator();

	// Iterator
	int GetFirstButtonID();
	int GetNextButtonID();

	TOEButtonPtr GetButtonByIndex(int index);

	TOEButtonPtr GetButton(int butId);

	/**
	Raise COM Event
	*/
	void OnButtonClicked(int buttonId);

//#ifdef ENTERPRISE_VERSION
	/**
	Create a toolbar in send message window.
	It will appear when the user reply / create / forward a mail.
	*/
	void CreateInMsgWnd(int wndStyle, int oeapiId);

	/**
	Event triggered when a button of this toolbar is clicked in a message window.<br>
	Only useful when the toolbar is defined in a msg window
	*/
	void OnButtonMsgWndClicked(int buttonId, int msgWndId);
//#endif // ENTERPRISE_VERSION

	long GetHandle();

	void SetMsgWndMgr(OEPluginTopLevelWndMgr* msgWndMgr) { msgWndMgr_ = msgWndMgr; }

	OEPluginToolbar *GetToolbarInternal();

#ifdef STATIC_LIBRARY
	static TOEToolbar *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static comet::com_ptr<comet::OEAPI::IOEToolbar> newInstance();
#endif // STATIC_LIBRARY

private:
	int id_;
	int oeapiId_;
//	IntList buttons_;
	int curButIndex_;
	int refCount_;
	OEPluginTopLevelWndMgr* msgWndMgr_;
};

//#endif // __USER_INTERFACE_H
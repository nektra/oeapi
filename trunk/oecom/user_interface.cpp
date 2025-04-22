/* $Id: user_interface.cpp,v 1.51 2009/01/28 18:17:12 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"
#include <oe_toolbar.h>
#include <oe_msgwnd.h>
#include <oe_menu.h>

#include "user_interface.h"
#include "main.h"

#include "../plugin/oeapi.h"

#include "se_debug.h"

#ifdef EVALUATION_VERSION
#include "eval_utils.h"
#endif // EVALUATION_VERSION

TOEMenuItemPtr miptr;

using namespace comet;
using namespace OEAPI;

#if _MSC_VER >= 1400
#pragma warning(push)
// disable warning C4812: 
// obsolete declaration style: please use 'coclass_implementation<comet::OEAPI::OEMenuItem>::coclass_implementation' instead
#pragma warning(disable:4812) 
#endif

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEMenuItem::TOEMenuItem()
{
	refCount_ = 0;
	menuIndex_ = menuId_ = id_ = -1;
	wndStyle_ = MsgWndStyles::OE_MSG_INVALID;
	menuMgr_ = NULL;
	OEAPIManager::Get()->AddMenuItem(this);
}

//---------------------------------------------------------------------------//
TOEMenuItem::~TOEMenuItem()
{
	if(OEAPIManager::Get(false)) {
		OEAPIManager::Get()->RemoveMenuItem(this);
	}
}

//---------------------------------------------------------------------------//
void TOEMenuItem::Create(int menuID, const bstr_t &name)
{
	TOEMenu *menu = OEAPIManager::Get()->GetMenu(menuID);
	if(menu) {
		menuIndex_ = menu->GetIndex();
		menuId_ = menuID;
		menuMgr_ = menu->GetMenuMgr();
		wndStyle_ = menu->GetWndStyle();
		id_ = GetMenuMgr()->AddMenuItem((MsgWndStyles::WNDSTYLE)wndStyle_, 
			menuIndex_, 
			name.t_str().c_str(), 
			_T(""), 
			TRUE, 
			MenuItemClickedCallback);
		// id_ = OEAPI_AddMenuItem(menuIndex_, (const PTCHAR) name.t_str().c_str(), _T(""), true, MenuItemClickedCallback);
		OEAPIManager::Get()->MenuItemCreated(this);
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEMenuItem::Create: Cannot find parent menu\n"));
	}
}

//---------------------------------------------------------------------------//
void TOEMenuItem::CreateBefore(int menuID, const bstr_t &name, const bstr_t &before, long subString)
{
	TOEMenu *menu = OEAPIManager::Get()->GetMenu(menuID);
	if(menu) {
		menuIndex_ = menu->GetIndex();
		menuId_ = menuID;
		menuMgr_ = menu->GetMenuMgr();
		wndStyle_ = menu->GetWndStyle();
		id_ = GetMenuMgr()->AddMenuItem((MsgWndStyles::WNDSTYLE)wndStyle_, 
			menuIndex_, 
			name.t_str().c_str(), 
			before.t_str().c_str(), 
			subString, 
			MenuItemClickedCallback);
		// id_ = OEAPI_AddMenuItem(menuIndex_, (const PTCHAR) name.t_str().c_str(), (const PTCHAR) before.t_str().c_str(), subString, MenuItemClickedCallback);
		OEAPIManager::Get()->MenuItemCreated(this);
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEMenuItem::CreateBefore: Cannot find parent menu\n"));
	}
}

//---------------------------------------------------------------------------//
void TOEMenuItem::CreateAt(long menuID, const bstr_t &name, long position)
{
	TOEMenu *menu = OEAPIManager::Get()->GetMenu(menuID);
	if(menu) {
		menuIndex_ = menu->GetIndex();
		menuId_ = menuID;
		menuMgr_ = menu->GetMenuMgr();
		wndStyle_ = menu->GetWndStyle();
		id_ = GetMenuMgr()->AddMenuItem((MsgWndStyles::WNDSTYLE)wndStyle_, 
			menuIndex_, 
			name.t_str().c_str(), 
			NULL, 
			FALSE, 
			MenuItemClickedCallback, 
			position);
		OEAPIManager::Get()->MenuItemCreated(this);
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEMenuItem::Create: Cannot find parent menu\n"));
	}
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenuItem::CreateSubItem(const bstr_t &name)
{
	TOEMenuItemPtr ret(TOEMenuItem::newInstance());

	TOEMenuItem *item = (TOEMenuItem *) ret.get();
	item->SetWndStyle(wndStyle_);
	item->SetMenuID(menuId_);
	item->SetMenuIndex(menuIndex_);
	item->SetID(
		// OEAPI_AddMenuSubItem(id_, (const PTCHAR) name.t_str().c_str(), MenuItemClickedCallback)
		GetMenuMgr()->AddMenuSubItem(id_, (const PTCHAR) name.t_str().c_str(), MenuItemClickedCallback)
		);
	OEAPIManager::Get()->MenuItemCreated(item, id_);

	return ret;
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenuItem::CreateSeparator()
{
	return CreateSubItem(_T(""));
}

//---------------------------------------------------------------------------//
void TOEMenuItem::Destroy()
{
	OEAPIManager::Get()->DestroyMenuItem(id_);
	GetMenuMgr()->DestroyMenuItem(id_);
}

//---------------------------------------------------------------------------//
void TOEMenuItem::SetEnabled(BOOL enabled)
{
	// OEAPI_SetMenuItemEnabled(id_, enabled);
	GetMenuMgr()->SetMenuItemEnabled(id_, enabled);
}

//---------------------------------------------------------------------------//
void TOEMenuItem::SetCheck(BOOL check)
{
	// OEAPI_SetMenuItemCheck(id_, check);
	GetMenuMgr()->SetMenuItemCheck(id_, check);
}

//---------------------------------------------------------------------------//
void TOEMenuItem::SetChecked(BOOL checked)
{
	//OEAPI_SetMenuItemChecked(id_, checked);
	GetMenuMgr()->SetMenuItemChecked(id_, checked);
}

//---------------------------------------------------------------------------//
BOOL TOEMenuItem::IsChecked()
{
	//return OEAPI_GetMenuItemChecked(id_);
	return GetMenuMgr()->GetMenuItemChecked(id_);
}

//-----------------------------------------------------------//
void TOEMenuItem::SetVisible(BOOL visible)
{
	GetMenuMgr()->SetMenuItemVisible(id_, visible);
}

//------------------------------------------------------------------//
BOOL TOEMenuItem::IsVisible()
{
	return GetMenuMgr()->GetMenuItemVisible(id_);
}

//------------------------------------------------------------------//
BOOL TOEMenuItem::IsEnabled()
{
	return GetMenuMgr()->GetMenuItemEnabled(id_);
}

//------------------------------------------------------------------//
bstr_t TOEMenuItem::GetText()
{
	TCHAR text[2048];
	GetMenuMgr()->GetMenuItemText(id_, text, sizeof(text));
	return text;
}

//------------------------------------------------------------------//
void TOEMenuItem::SetText(const bstr_t& text)
{
	GetMenuMgr()->SetMenuItemText(id_, text.t_str().c_str());
}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEMenuItem *TOEMenuItem::newInstance()
{
	
    return new TOEMenuItem;
}
#else

TOEMenuItemPtr TOEMenuItem::newInstance()
{
    return TOEMenuItemPtr(new TOEMenuItem);
}
#endif // STATIC_LIBRARY

//---------------------------------------------------------------------------//
void TOEMenuItem::OnClick()
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnClick();
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEMenuItem::OnClick: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//-----------------------------------------------------------//
void TOEMenuItem::OnMsgWndClick(int wndMsgId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnMsgWndClick(wndMsgId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEMenuItem::OnClick: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//-----------------------------------------------------------//
OEPluginMenuMgr* TOEMenuItem::GetMenuMgr()
{
	return menuMgr_ ? menuMgr_ : OEPluginMenuMgr::Get();
}

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEMenu::TOEMenu()
{
	refCount_ = 0;
	oeapiId_ = index_ = -1;
	wndStyle_ = OE_MSG_INVALID;
	menuMgr_ = NULL;
	OEAPIManager::Get()->AddMenu(this);
}

//---------------------------------------------------------------------------//
TOEMenu::~TOEMenu()
{
	if(OEAPIManager::Get(false)) {
		OEAPIManager::Get()->RemoveMenu(this);
	}
}
//
////---------------------------------------------------------------------------//
//void TOEMenu::Create(int oeapiId, const bstr_t &name)
//{
//	oeapiId_ = oeapiId;
////	id_ = OEAPI_AddToolbarButton(toolbarId, (PTCHAR) name.t_str().c_str(), (PTCHAR) bmpLocation.t_str().c_str(), (OEAPI_CallBack) ButtonClickedCallback);
//}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenu::CreateItem(const bstr_t &name)
{
	TOEMenuItemPtr ret(TOEMenuItem::newInstance());
	ret->Create(id_, name);
	return ret;
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenu::CreateItemBefore(const bstr_t &name, const bstr_t &before, long subString)
{
	TOEMenuItemPtr ret(TOEMenuItem::newInstance());
	ret->CreateBefore(id_, name, before, subString);
	return ret;
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenu::CreateItemAt(const bstr_t& name, long position)
{
	TOEMenuItemPtr ret(TOEMenuItem::newInstance());
	ret->CreateAt(id_, name, position);
	return ret;
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenu::CreateSeparator()
{
	return CreateItem(_T(""));
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenu::CreateSeparatorBefore(const bstr_t &before, long subString)
{
	return CreateItemBefore(_T(""), before, subString);
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEMenu::CreateSeparatorAt(long position)
{
	return CreateItemAt(_T(""), position);
}


////---------------------------------------------------------------------------//
//bstr_t TOEMenu::Name()
//{
//	return "";
//}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEMenu *TOEMenu::newInstance()
{
	return new TOEMenu;
}
#else
TOEMenuPtr TOEMenu::newInstance()
{
    return TOEMenuPtr(new TOEMenu);
}
#endif // STATIC_LIBRARY

//---------------------------------------------------------------------------//
void TOEMenu::OnItemClicked(int menuItemId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnItemClicked(menuItemId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEMenu::OnItemClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEMenu::OnMsgWndItemClicked(int menuItemId, int msgWndId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnMsgWndItemClicked(menuItemId, msgWndId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEMenu::OnItemClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//-----------------------------------------------------------//
OEPluginMenuMgr* TOEMenu::GetMenuMgr()
{
	return menuMgr_ ? menuMgr_ : OEPluginMenuMgr::Get();
}


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEButton::TOEButton()
{
	refCount_ = 0;
	toolbarId_ = id_ = -1;
	msgWndMgr_ = NULL;
	button_ = NULL;
	OEAPIManager::Get()->AddButton(this);
}

//---------------------------------------------------------------------------//
TOEButton::~TOEButton()
{
	if(button_) {
		delete button_;
		button_ = NULL;
	}
	if(OEAPIManager::Get(false)) {
		OEAPIManager::Get()->RemoveButton(this);
	}
}

//---------------------------------------------------------------------------//
void TOEButton::Create(int toolbarId, const bstr_t &name, const bstr_t &bmpNormal, const bstr_t &bmpMouseOver)
{
	toolbarId_ = toolbarId;
	//INT butId = -1;

	//OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(toolbarId);
	OEPluginToolbar *toolbar = GetToolbarInternal();
	if(toolbar) {
		OEPluginToolbarBtn *button = GetButtonInternal();
		if(button != NULL) {
			button->SetName(name.t_str().c_str());
			button->SetBMPNormal(bmpNormal.t_str().c_str());
			button->SetBMPMouseOver(bmpMouseOver.t_str().c_str());
			button->SetCallback(ButtonClickedCallback);
			toolbar->AddButton(button);
			button_ = NULL;
			button->Unlock();
		}
		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::Create: Cannot find toolbar object\n"));
	}
}

//---------------------------------------------------------------------------//
void TOEButton::CreateSeparator(int toolbarId)
{
	toolbarId_ = toolbarId;
	
	//OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(toolbarId);
	OEPluginToolbar *toolbar = GetToolbarInternal();
	if(toolbar) {
		OEPluginToolbarBtn *button  = GetButtonInternal(); // = new OEPluginToolbarBtn;
		if(button) {
			button->SetSeparator(TRUE);
			toolbar->AddButton(button);
			//id_ = button->GetID();
			button_ = NULL;
			button->Unlock();
		}
		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::CreateSeparator: Cannot find toolbar object\n"));
	}
}

//---------------------------------------------------------------------------//
BOOL TOEButton::IsSeparator()
{
	BOOL ret = FALSE;
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		ret = btn->IsSeparator();
		btn->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::IsSeparator: Cannot find button object\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEButton::SetPopupStyle(BOOL popup)
{
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetPopupStyle(popup);
		btn->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::SetPopupStyle: Cannot find button object\n"));
	}
}

//---------------------------------------------------------------------------//
void TOEButton::Destroy()
{
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		delete btn;
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::Destroy: Cannot find button object\n"));
	}
}

//---------------------------------------------------------------------------//
TOEButtonPtr TOEButton::CreateSubButton(const bstr_t &name, const bstr_t &bmpNormal, const bstr_t &bmpMouseOver)
{
	TOEButtonPtr ret;

	if(id_ == -1) {
		debug_print(DEBUG_ERROR, _T("OEButton::CreateSubButton: Parent button is not created\n"));
		return ret;
	}

	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		ret = TOEButton::newInstance();

		OEPluginToolbarBtn *subButton = new OEPluginToolbarBtn;
		subButton->SetName(name.t_str().c_str());
		subButton->SetBMPNormal(bmpNormal.t_str().c_str());
		subButton->SetBMPMouseOver(bmpMouseOver.t_str().c_str());
		subButton->SetCallback(ButtonClickedCallback);
		btn->AddButton(subButton);

		TOEButton *newBtn = (TOEButton *) ret.get();
		newBtn->SetID(subButton->GetID());
		newBtn->SetToolbarID(GetToolbarID());

		btn->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::CreateSubButton: Cannot find button object\n"));
	}

	return ret;
}

//------------------------------------------------------------------//
TOEButtonPtr TOEButton::CreateSubSeparator()
{
	TOEButtonPtr ret;

	if(id_ == -1) {
		debug_print(DEBUG_ERROR, _T("TOEButton::CreateSubSeparator: Parent button is not created\n"));
		return ret;
	}

	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		ret = TOEButton::newInstance();

		OEPluginToolbarBtn *subButton = new OEPluginToolbarBtn;
		subButton->SetSeparator(TRUE);
		btn->AddButton(subButton);

		TOEButton *newBtn = (TOEButton *) ret.get();
		newBtn->SetID(subButton->GetID());
		newBtn->SetToolbarID(GetToolbarID());

		btn->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("TOEButton::CreateSubSeparator: Cannot find button object\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEButton::SetEnabled(BOOL enabled)
{
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetEnabled(enabled);
		btn->Unlock();
	}
}

//---------------------------------------------------------------------------//
void TOEButton::SetShown(BOOL show)
{
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetShown(show);
		btn->Unlock();
	}
}

//---------------------------------------------------------------------------//
void TOEButton::SetCheckStyle(BOOL check)
{
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetCheckStyle(check);
		btn->Unlock();
	}
}

//---------------------------------------------------------------------------//
void TOEButton::SetChecked(BOOL checked)
{
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetChecked(checked);
		btn->Unlock();
	}
}

//---------------------------------------------------------------------------//
BOOL TOEButton::IsChecked()
{
	BOOL ret = FALSE;
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		ret = btn->IsChecked();
		btn->Unlock();
	}
	return ret;
}

//---------------------------------------------------------------------------//
void TOEButton::SetName(const bstr_t &name)
{
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetName((PTCHAR) name.t_str().c_str());
		btn->Unlock();
	}
}

//-----------------------------------------------------------//
void TOEButton::SetTooltip(const bstr_t& tooltip)
{
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetTooltip((PTCHAR) tooltip.t_str().c_str());
		btn->Unlock();
	}
}

//---------------------------------------------------------------------------//
void TOEButton::SetImageNormal(const bstr_t &bmp)
{
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetBMPNormal((PTCHAR) bmp.t_str().c_str());
		btn->Unlock();
	}
}

//---------------------------------------------------------------------------//
void TOEButton::SetImageMouseOver(const bstr_t &bmp)
{
	//OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButton(id_);
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		btn->SetBMPMouseOver((PTCHAR) bmp.t_str().c_str());
		btn->Unlock();
	}
}

//------------------------------------------------------------------//
BOOL TOEButton::IsVisible()
{
	BOOL ret = FALSE;
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		ret = !btn->IsHidden();
		btn->Unlock();
	}
	return ret;
}

//------------------------------------------------------------------//
BOOL TOEButton::IsEnabled()
{
	BOOL ret = FALSE;
	OEPluginToolbarBtn *btn = GetButtonInternal();
	if(btn) {
		ret = btn->IsEnabled();
		btn->Unlock();
	}
	return ret;
}

//---------------------------------------------------------------------------//
int TOEButton::GetFirstSubButtonID()
{
	curSubBtnIndex_ = 0;
	return GetNextSubButtonID();
}

//---------------------------------------------------------------------------//
int TOEButton::GetNextSubButtonID()
{
	int ret = -1;

	OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(toolbarId_);
	if(toolbar) {
		OEPluginToolbarBtn *btn = toolbar->GetButton(id_);
		if(btn) {
			OEPluginToolbarBtn *subBtn = btn->GetSubButtonByIndex(curSubBtnIndex_);
			if(subBtn) {
				ret = subBtn->GetID();
				subBtn->Unlock();
			}

			btn->Unlock();
		}
		else {
			debug_print(DEBUG_ERROR, _T("OEButton::GetNextSubButtonID: Invalid button.\n"));
			curSubBtnIndex_ = -1;
		}

		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::GetNextSubButtonID: Cannot find toolbar object.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
TOEButtonPtr TOEButton::GetSubButtonByIndex(int index)
{
	TOEButtonPtr ret;

	//OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);
	OEPluginToolbar *toolbar = GetToolbarInternal();
	if(toolbar) {
		OEPluginToolbarBtn *btn = toolbar->GetButtonByIndex(index);
		if(btn) {
			ret = TOEButton::newInstance();
			TOEButton *tbtn = (TOEButton *) ret.get();
			if(tbtn) {
				tbtn->SetID(btn->GetID());
				tbtn->SetToolbarID(btn->GetToolbarID());
			}

			btn->Unlock();
		}

		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEButton::GetButtonByIndex: Cannot find object.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEButton *TOEButton::newInstance()
{
	return new TOEButton;
}
#else
TOEButtonPtr TOEButton::newInstance()
{
    return TOEButtonPtr(new TOEButton);
}
#endif // STATIC_LIBRARY

//---------------------------------------------------------------------------//
void TOEButton::OnClick()
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnClick();
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEButton::OnClick: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
OEPluginToolbarBtn* TOEButton::GetButtonInternal()
{
	if(id_ != -1) {
		if(button_ == 0) {
			return msgWndMgr_ ? msgWndMgr_->GetButton(id_) :
				OEPluginToolbarMgr::Get()->GetButton(id_);
		}
		else {
			button_->Lock();
			return button_;
		}
	}
	else {
		button_ = new OEPluginToolbarBtn;
		id_ = button_->GetID();
		button_->Lock();
		return button_;
	}
}

//------------------------------------------------------------------//
bstr_t TOEButton::GetName()
{
	bstr_t name = _T("");
	OEPluginToolbarBtn* button = GetButtonInternal();
	if(button) {
		name = button->GetName();
		button->Unlock();
	}
	return name;
}

//------------------------------------------------------------------//
void TOEButton::SetPicture(const com_ptr<::IPictureDisp>& normal)
{
	OEPluginToolbarBtn* btn = GetButtonInternal();
	if(btn != NULL) {
		IPicture* pict = NULL;
		if(!normal.is_null()) {
			HRESULT hr = normal->QueryInterface(IID_IPicture, (void**)&pict);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("TOEButton::SetPicture: Can't retrieve normal picture.\n"));
				return;
			}
		}
		btn->SetPictureNormal(pict);
		if(pict) {
			pict->Release();
		}
		btn->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("TOEButton::SetPicture: Can't retrieve  button\n"));
	}
}

//------------------------------------------------------------------//
void TOEButton::SetPictureMouseOver(const com_ptr<::IPictureDisp>& over)
{
	OEPluginToolbarBtn* btn = GetButtonInternal();
	if(btn != NULL) {
		IPicture* pict = NULL;
		if(!over.is_null()) {
			HRESULT hr = over->QueryInterface(IID_IPicture, (void**)&pict);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("TOEButton::SetMouseOverPicture: Can't retrieve normal picture.\n"));
				return;
			}
		}
		btn->SetPictureOver(pict);
		if(pict) {
			pict->Release();
		}
		btn->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("TOEButton::SetMouseOverPicture: Can't retrieve  button\n"));
	}
}

//---------------------------------------------------------------------------//
OEPluginToolbar* TOEButton::GetToolbarInternal()
{
	return msgWndMgr_ ? msgWndMgr_->GetToolbar(toolbarId_) :
		OEPluginToolbarMgr::Get()->GetToolbar(toolbarId_);
}


#ifdef ENTERPRISE_VERSION
//---------------------------------------------------------------------------//
void TOEButton::OnMsgWndClick(int msgWndId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnMsgWndClick(msgWndId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEButton::OnMsgWndClick: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}
#endif // ENTERPRISE_VERSION


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEToolbar::TOEToolbar()
{
	refCount_ = 0;
	oeapiId_ = id_ = -1;
	msgWndMgr_ = NULL;
	OEAPIManager::Get()->AddToolbar(this);
}

//---------------------------------------------------------------------------//
TOEToolbar::~TOEToolbar()
{
	if(OEAPIManager::Get(false)) {
		OEAPIManager::Get()->RemoveToolbar(this);
	}
}

//---------------------------------------------------------------------------//
void TOEToolbar::Create(int oeapiId)
{
	OEPluginToolbar *toolbar = NULL;

	oeapiId_ = oeapiId;

//	if(id_ == -1) {
//		OEPluginToolbar *toolbar = new OEPluginToolbar(OEPluginToolbarMgr::Get()->GetMainWndMgr());
//		toolbar->Lock();
//		id_ = (INT) toolbar->GetID();
//	}
//	else {
//		toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);
//	}

	toolbar = GetToolbarObject();

	if(toolbar) {
		toolbar->Create(oeapiId_);
		toolbar->Unlock();
	}
}

//---------------------------------------------------------------------------//
void TOEToolbar::Create()
{
	Create(-1);
}

//---------------------------------------------------------------------------//
OEPluginToolbar *TOEToolbar::GetToolbarObject(BOOL unlockObj)
{
	OEPluginToolbar *toolbar = NULL;

	if(id_ == -1) {
		toolbar = new OEPluginToolbar(OEPluginToolbarMgr::Get()->GetMainWndMgr());
		toolbar->Lock();
		id_ = (INT) toolbar->GetID();
	}
	else {
		//toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);
		toolbar = GetToolbarInternal();
	}

	if(toolbar && unlockObj) {
		toolbar->Unlock();
	}

	return toolbar;
}

//---------------------------------------------------------------------------//
long TOEToolbar::GetHandle()
{
	OEPluginToolbar *toolbar = GetToolbarObject();
	HWND hToolbar = toolbar->GetHandle();

	toolbar->Unlock();

	return (LONG)hToolbar;
}

//---------------------------------------------------------------------------//
void TOEToolbar::Destroy()
{
	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar) {
		delete toolbar;
	}
}

//---------------------------------------------------------------------------//
void TOEToolbar::SetOEDefaultToolbar(int wndStyle, int oeapiId)
{
	OEPluginToolbar *toolbar;

	oeapiId_ = oeapiId;

	if(wndStyle == OE_MSG_INVALID) {
		toolbar = new OEPluginToolbar(OEPluginToolbarMgr::Get()->GetMainWndMgr());
	}
	else {
		toolbar = new OEPluginToolbar(OEPluginToolbarMgr::Get()->GetMsgWndMgr());
	}

	id_ = (INT) toolbar->GetID();
	toolbar->SetOEDefaultToolbar(TRUE);
	toolbar->SetWndStyle(wndStyle);
	toolbar->Create(oeapiId_);
}

//---------------------------------------------------------------------------//
void TOEToolbar::SetShown(BOOL show)
{
//	OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);

	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar != NULL) {
		toolbar->SetShown(show);
		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEToolbar::SetShown: Cannot find object.\n"));
	}
}

//---------------------------------------------------------------------------//
void TOEToolbar::SetLargeButtons(BOOL largeBtns)
{
//	OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);

	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar != NULL) {
		toolbar->SetLargeButtons(largeBtns);
		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEToolbar::SetLargeButtons: Cannot find object.\n"));
	}
}

//---------------------------------------------------------------------------//
void TOEToolbar::SetMaxTextRows(INT rows)
{
	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar != NULL) {
		toolbar->SetMaxTextRows(rows);
		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEToolbar::SetMaxTextRows: Cannot find object.\n"));
	}
}


//-----------------------------------------------------------//
void TOEToolbar::EnableTooltips(BOOL bEnable)
{
	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar != NULL) {
		toolbar->EnableTooltips(bEnable);
		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("TOEToolbar::EnableTooltips: Cannot find object.\n"));
	}
}

//---------------------------------------------------------------------------//
TOEButtonPtr TOEToolbar::CreateButton(const bstr_t &name, const bstr_t &bmpNormal, const bstr_t &bmpMouseOver)
{
	GetToolbarObject(TRUE);

	TOEButtonPtr ret(TOEButton::newInstance());
	ret->Create(id_, name, bmpNormal, bmpMouseOver);
	//if(msgWndMgr_) // ret->SetMsgWndMgr(msgWndMgr_);
	//	ret->SetMsgWndMgr(msgWndMgr_);
	return ret;
}

//------------------------------------------------------------------//
//TOEButtonPtr TOEToolbar::AddButton()
//{
//	GetToolbarObject(TRUE);
//
//	TOEButtonPtr ret(TOEButton::newInstance());
//	ret->Create(id_, _T(""), _T(""), _T(""));
//	return ret;
//}

//---------------------------------------------------------------------------//
TOEButtonPtr TOEToolbar::CreateSeparator()
{
	GetToolbarObject(TRUE);

	TOEButtonPtr ret(TOEButton::newInstance());
	ret->CreateSeparator(id_);
	return ret;
}

//---------------------------------------------------------------------------//
int TOEToolbar::GetFirstButtonID()
{
	curButIndex_ = 0;
	return GetNextButtonID();
}

//---------------------------------------------------------------------------//
int TOEToolbar::GetNextButtonID()
{
	int ret = -1;

	//OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);
	OEPluginToolbar *toolbar = GetToolbarInternal();
	if(toolbar) {
		OEPluginToolbarBtn *btn = toolbar->GetButtonByIndex(curButIndex_++);
		if(btn) {
			ret = btn->GetID();
			btn->Unlock();
		}
		else {
			curButIndex_ = -1;
		}

		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEToolbar::GetNextButtonID: Cannot find object.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
TOEButtonPtr TOEToolbar::GetButtonByIndex(int index)
{
	TOEButtonPtr ret;

	//OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);
	OEPluginToolbar *toolbar = GetToolbarInternal();

	if(toolbar) {
		OEPluginToolbarBtn *btn = toolbar->GetButtonByIndex(index);
		if(btn) {
			ret = TOEButton::newInstance();
			TOEButton *tbtn = (TOEButton *) ret.get();
			if(tbtn) {
				tbtn->SetID(btn->GetID());
				tbtn->SetToolbarID(id_);
				if(msgWndMgr_) tbtn->SetMsgWndMgr(msgWndMgr_);
			}

			btn->Unlock();
		}

		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEToolbar::GetButtonByIndex: Cannot find object.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
TOEButtonPtr TOEToolbar::GetButton(int butId)
{
	TOEButtonPtr ret;
//	OEPluginToolbarMgr::Get()->Lock();
	//OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(id_);
	OEPluginToolbar *toolbar = GetToolbarInternal();
	if(toolbar) {
		OEPluginToolbarBtn *btn = toolbar->GetButton(butId);
		if(btn) {
			ret = TOEButton::newInstance();
			TOEButton *tbtn = (TOEButton *) ret.get();
			if(tbtn) {
				tbtn->SetID(butId);
				tbtn->SetToolbarID(id_);
				if(msgWndMgr_) tbtn->SetMsgWndMgr(msgWndMgr_);
			}

			btn->Unlock();
		}

		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEToolbar::GetButton: Cannot find toolbar object.\n"));
	}
//	OEPluginToolbarMgr::Get()->Unlock();
	return ret;
}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEToolbar *TOEToolbar::newInstance()
{
	return new TOEToolbar;
}
#else
TOEToolbarPtr TOEToolbar::newInstance()
{
    return TOEToolbarPtr(new TOEToolbar);
}
#endif // STATIC_LIBRARY

//---------------------------------------------------------------------------//
void TOEToolbar::OnButtonClicked(int buttonId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnButtonClicked(buttonId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEToolbar::OnButtonClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//#ifdef ENTERPRISE_VERSION

//---------------------------------------------------------------------------//
void TOEToolbar::CreateInMsgWnd(int wndStyle, int oeapiId)
{
	oeapiId_ = oeapiId;

	OEPluginToolbar *toolbar = new OEPluginToolbar(OEPluginToolbarMgr::Get()->GetMsgWndMgr());
	id_ = (INT) toolbar->GetID();
	toolbar->SetWndStyle(wndStyle);
	toolbar->Create(oeapiId_);
}

//------------------------------------------------------------------//
void TOEToolbar::SetMaskColor(LONG color)
{
	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar) {
		toolbar->SetMaskColor(color);
		toolbar->Unlock();
	}
}

//------------------------------------------------------------------//
LONG TOEToolbar::GetMaskColor()
{
	LONG color = CLR_DEFAULT;
	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar) {
		color = toolbar->GetMaskColor();
		toolbar->Unlock();
	}
	return color;
}

//------------------------------------------------------------------//
BOOL TOEToolbar::SetPictureSize(long cx, long cy)
{
	BOOL ret = FALSE;
	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar) {
		ret = toolbar->SetBitmapSize(cx, cy);
		toolbar->Unlock();
	}
	return ret;
}

//------------------------------------------------------------------//
BOOL TOEToolbar::GetPictureSize(long& cx, long& cy)
{
	BOOL ret = FALSE;
	OEPluginToolbar *toolbar = GetToolbarObject();
	if(toolbar) {
		ret = toolbar->GetBitmapSize((INT*)&cx, (INT*)&cy);
		toolbar->Unlock();
	}
	return ret;
}

//---------------------------------------------------------------------------//
void TOEToolbar::OnButtonMsgWndClicked(int buttonId, int msgWndId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnButtonMsgWndClicked(buttonId, msgWndId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("TOEToolbar::OnButtonMsgWndClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}


//---------------------------------------------------------------------------//
OEPluginToolbar* TOEToolbar::GetToolbarInternal()
{
	return msgWndMgr_ ? msgWndMgr_->GetToolbar(id_) : OEPluginToolbarMgr::Get()->GetToolbar(id_);
}


#if _MSC_VER >= 1400
#pragma warning(pop)
#endif

//#endif // ENTERPRISE_VERSION

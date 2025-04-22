/* $Id: oemsgwnd.cpp,v 1.16.6.1 2007/06/25 19:05:42 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"

#include "oe_msgwnd.h"
#include "oe_toolbar.h"
#include "oemsgwnd.h"

#include "main.h"

#include "oe_menu.h"
#include "se_debug.h"


using namespace comet;
using namespace OEAPI;

#if _MSC_VER >= 1400
#pragma warning(push)
// disable warning C4812: 
// obsolete declaration style: please use 'coclass_implementation<comet::OEAPI::OEMsgWnd>::coclass_implementation' instead
#pragma warning(disable:4812) 
#endif


//#ifdef ENTERPRISE_VERSION

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEMsgWnd::TOEMsgWnd()
{
	msgWnd_ = NULL;
	OEAPIManager::Get()->AddMsgWnd(this);
}

//---------------------------------------------------------------------------//
TOEMsgWnd::~TOEMsgWnd()
{
	Close();
}
//---------------------------------------------------------------------------//
BOOL TOEMsgWnd::Close()
{
	if(msgWnd_) {
		msgWnd_->Unlock();
	}

	if(OEAPIManager::Get(false)) {
		OEAPIManager::Get()->RemoveMsgWnd(this);
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
bstr_t TOEMsgWnd::GetTo()
{
	bstr_t ret;

	if(msgWnd_) {
		ret = msgWnd_->GetTo();
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SetTo(const bstr_t &address)
{
	if(msgWnd_) {
		msgWnd_->SetTo(address);
	}
}

//---------------------------------------------------------------------------//
bstr_t TOEMsgWnd::GetCc()
{
	bstr_t ret;

	if(msgWnd_) {
		ret = msgWnd_->GetCc();
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SetCc(const bstr_t &address)
{
	if(msgWnd_) {
		msgWnd_->SetCc(address);
	}
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::CancelSend()
{
	if(msgWnd_) {
		msgWnd_->SetSendCancelled(TRUE);
	}
}

//---------------------------------------------------------------------------//
bstr_t TOEMsgWnd::GetSubject()
{
	bstr_t ret;

	if(msgWnd_) {
		ret = msgWnd_->GetSubject();
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SetSubject(const bstr_t &subject)
{
	if(msgWnd_) {
		msgWnd_->SetSubject(subject);
	}
}

//---------------------------------------------------------------------------//
bstr_t TOEMsgWnd::GetBody()
{
	bstr_t ret;

	if(msgWnd_) {
		ret = msgWnd_->GetBody();
	}

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEMsgWnd::GetBodyHTML()
{
	bstr_t ret;

	if(msgWnd_) {
		ret = msgWnd_->GetBodyHTML();
	}

	return ret;
}

//---------------------------------------------------------------------------//
int TOEMsgWnd::GetBodyLength()
{
	int ret = 0;

	if(msgWnd_) {
		ret = msgWnd_->GetBodyLength();
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SetBody(const bstr_t &body)
{
	if(msgWnd_) {
		msgWnd_->SetBody(body);
	}
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SetBodyHTML(const bstr_t &body)
{
	if(msgWnd_) {
		msgWnd_->SetBodyHTML(body);
	}
}

//---------------------------------------------------------------------------//
comet::OEAPI::tagWNDSTYLE TOEMsgWnd::GetWndStyle()
{
	comet::OEAPI::tagWNDSTYLE wndStyle = OE_MSG_INVALID;

	if(msgWnd_) {
		if(msgWnd_->IsMainWndMsgWnd()) {
			if(msgWnd_->IsPreviewMsgWnd()) {
				if(msgWnd_->IsEmptyMsg()) {
					wndStyle = comet::OEAPI::OE_EMPTY_MSG_WND;
				}
				else {
					wndStyle = comet::OEAPI::OE_CURRENT_MSG_WND;
				}
			}
			else {
				wndStyle = comet::OEAPI::OE_OTHER_WND;
			}
		}
		else {
			wndStyle = (comet::OEAPI::tagWNDSTYLE) msgWnd_->GetTopLevelWndMgr()->GetWndStyle();
		}
	}

	return wndStyle;
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SendMail()
{
	if(msgWnd_ && msgWnd_->GetTopLevelWndMgr()) {
		msgWnd_->GetTopLevelWndMgr()->SendMail();
	}
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SendKey(long keyCode, long controlCode, BOOL setForeground)
{
	if(msgWnd_ && msgWnd_->GetTopLevelWndMgr()) {
		msgWnd_->GetTopLevelWndMgr()->SendKey((BYTE)keyCode, (BYTE)controlCode, setForeground);
	}
}

//---------------------------------------------------------------------------//
TOEToolbarPtr TOEMsgWnd::GetToolbarByIndex(int toolbarIdx)
{
	TOEToolbarPtr ret;
	if(msgWnd_)
	{
		OEPluginToolbar *toolbar = msgWnd_->GetTopLevelWndMgr()->GetToolbarByIndex(toolbarIdx);
		if(toolbar)
		{
			ret = TOEToolbar::newInstance();
			TOEToolbar *ttool = (TOEToolbar *) ret.get();
			if(ttool) {
				ttool->SetID(toolbar->GetID());
				ttool->SetMsgWndMgr(msgWnd_->GetTopLevelWndMgr());
				// ttool->SetOEAPIID(id_);
			}
			toolbar->Unlock();
		}
	}
	return ret;
}

//---------------------------------------------------------------------------//
TOEToolbarPtr TOEMsgWnd::GetToolbar(int toolbarId)
{
	TOEToolbarPtr ret;
	if(msgWnd_)
	{
		OEPluginToolbar *toolbar = msgWnd_->GetTopLevelWndMgr()->GetToolbar(toolbarId);
		if(toolbar)
		{
			ret = TOEToolbar::newInstance();
			TOEToolbar *ttool = (TOEToolbar *) ret.get();
			if(ttool) {
				ttool->SetID(toolbar->GetID());
				ttool->SetMsgWndMgr(msgWnd_->GetTopLevelWndMgr());
				ttool->SetOEAPIID(oeapiId_);
			}
			toolbar->Unlock();
		}
	}
	return ret;
}

//-----------------------------------------------------------//
TOEMenuPtr TOEMsgWnd::GetMenu(int menuIdx)
{
	TOEMenuPtr ret(TOEMenu::newInstance());
	if(msgWnd_)
	{
		TOEMenu *menu = (TOEMenu *) ret.get();
		menu->SetIndex(menuIdx);
		menu->SetWndStyle(msgWnd_->GetWndStyle());
		menu->SetID(OEAPIManager::Get()->GetNewMenuID());
		menu->SetMenuMgr(msgWnd_->GetMenuMgr());
		menu->SetOEAPIID(oeapiId_);
	}
	return ret;
}

//-----------------------------------------------------------//
TOEMenuItemPtr TOEMsgWnd::GetMenuItem(int itemId)
{
	TOEMenuItemPtr ret;
	OEPluginMenuItem* item = msgWnd_->GetMenuMgr()->GetMenuItem(itemId);
	if(item != NULL) {
		ret = TOEMenuItem::newInstance();
		TOEMenuItem *menuItem = (TOEMenuItem *) ret.get();
		if(menuItem) {
			menuItem->SetID(item->GetId());
			//menuItem->SetWndStyle(menu_msg_wnd);
			menuItem->SetWndStyle(msgWnd_->GetWndStyle());
			// This is used for subbuttons
			// menuItem->SetMenuID(item->);
			menuItem->SetMenuIndex(item->GetIndex());
			menuItem->SetMenuMgr(msgWnd_->GetMenuMgr());
			//menuItem->SetOEAPIID(oeapiId_);
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SetID(int id)
{
	// GetMsgWnd return a locked msg wnd
	msgWnd_ = MsgWndMgr.GetMsgWnd((HWND)LongToHandle(id));
	if(msgWnd_ == NULL) {
//		debug_print(DEBUG_ERROR, _T("TOEMsgWnd::SetID: Invalid window.\n"));
	}
//	else {
//		// do not let the window die.
//		if(!msgWnd_->Lock()) {
//			// the window is destroyed.
//			msgWnd_ = NULL;
//		}
//	}
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::OnToolbarButtonClicked(int toolbarId, int buttonId)
{
#if !defined(STATIC_LIBRARY) && defined(ENTERPRISE_VERSION)
	try {
		connection_point.Fire_OnToolbarButtonClicked(toolbarId, buttonId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnToolbarButtonClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::OnMenuItemClicked(int menuId, int menuItemId)
{
#if !defined(STATIC_LIBRARY) && defined(ENTERPRISE_VERSION)
	try {
		connection_point.Fire_OnMenuItemClicked(menuId, menuItemId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnMenuItemClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::OnClose(int isMainWnd)
{
#if !defined(STATIC_LIBRARY) && defined(ENTERPRISE_VERSION)
	try {
		connection_point.Fire_OnClose(isMainWnd);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnClose: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}


//---------------------------------------------------------------------------//
int TOEMsgWnd::GetID()
{
	if(msgWnd_ == NULL) {
		return -1;
	}

	return msgWnd_->GetID();
}

//---------------------------------------------------------------------------//
com_ptr<::IDispatch> TOEMsgWnd::GetIHTMLDocument2()
{
	IDispatch* html = NULL;
#if defined(ENTERPRISE_VERSION)
	if(msgWnd_) {
		msgWnd_->GetIHTMLDocument2(&html);
	}
#endif
	return html;
}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEMsgWnd *TOEMsgWnd::newInstance()
{
	return new TOEMsgWnd;
}
#else
com_ptr<IOEMsgWnd> TOEMsgWnd::newInstance()
{
	com_ptr<IOEMsgWnd> ret(uuidof<OEMsgWnd>());
	return ret;
}
#endif // STATIC_LIBRARY

//#endif // ENTERPRISE_VERSION

#if _MSC_VER >= 1400
#pragma warning(pop)
#endif


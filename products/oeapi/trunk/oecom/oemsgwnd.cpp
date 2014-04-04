/* $Id: oemsgwnd.cpp,v 1.22 2009/01/28 18:17:55 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
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
bstr_t TOEMsgWnd::GetBcc()
{
	bstr_t ret;

	if(msgWnd_) {
		ret = msgWnd_->GetBcc();
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEMsgWnd::SetBcc(const bstr_t &address)
{
	if(msgWnd_) {
		msgWnd_->SetBcc(address);
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
	if(msgWnd_) {
		OEPluginTopLevelWndMgr* mgr = msgWnd_->GetTopLevelWndMgr();
		OEPluginToolbar *toolbar = mgr ? mgr->GetToolbarByIndex(toolbarIdx) : NULL;
		if(toolbar) {
			ret = TOEToolbar::newInstance();
			TOEToolbar *ttool = (TOEToolbar *) ret.get();
			if(ttool) {
				ttool->SetID(toolbar->GetID());
				ttool->SetMsgWndMgr(mgr);
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
	if(msgWnd_) {
		OEPluginTopLevelWndMgr* mgr = msgWnd_->GetTopLevelWndMgr();
		OEPluginToolbar *toolbar = mgr ? mgr->GetToolbar(toolbarId) : NULL;
		if(toolbar)
		{
			ret = TOEToolbar::newInstance();
			TOEToolbar *ttool = (TOEToolbar *) ret.get();
			if(ttool) {
				ttool->SetID(toolbar->GetID());
				ttool->SetMsgWndMgr(mgr);
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
	msgWnd_ = MsgWndMgr.GetMsgWnd((HWND)id);
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
VOID TOEMsgWnd::SaveDraft()
{
	// Temporarily disable the SaveDraft  dialog to avoid deadlock
	//

	HKEY hKeyIdent;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Identities", 0, KEY_READ | KEY_WRITE, &hKeyIdent) == ERROR_SUCCESS)
	{
		DWORD cbData = 128;
		wchar_t identityUID[64] = { 0 };
		if (RegQueryValueEx(hKeyIdent, L"Last User ID", 0, 0, (BYTE*) identityUID, &cbData) == ERROR_SUCCESS)
		{
			wchar_t regPath[1024] = {0};
			lstrcpy(regPath, L"Identities\\");
			lstrcat(regPath, identityUID);
			lstrcat(regPath, L"\\Software\\Microsoft\\Outlook Express\\5.0\\Dont Show Dialogs");

			HKEY hKeyDontShow;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_READ | KEY_WRITE, &hKeyDontShow) ==ERROR_SUCCESS)
			{

				DWORD data = 1;
				DWORD cbData = sizeof(DWORD);
				DWORD oldData = 0;

				RegQueryValueEx(hKeyDontShow, L"Saved in Saved Items", 0 , 0, (BYTE*) &oldData, &cbData);

				if (ERROR_SUCCESS == RegSetValueExW(hKeyDontShow, L"Saved in Saved Items", 0, REG_DWORD, (const BYTE*)&data, sizeof(DWORD)))
				{
					SendMessage(msgWnd_->GetWindowHandle(), WM_COMMAND, MAKEWPARAM(OE_SAVEMESSAGE,1), 0);

					// Restore Old value if was present
					// If it was not present , assumes 0

					RegSetValueExW(hKeyDontShow, L"Saved in Saved Items", 0, REG_DWORD, (BYTE*)&oldData, sizeof(DWORD));
				}

			}
		}
		
		RegCloseKey(hKeyIdent);
	}
}

//---------------------------------------------------------------------------//
ULONG TOEMsgWnd::GetHandle()
{
	return (ULONG) msgWnd_->GetWindowHandle();
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
TOEMsgWndPtr TOEMsgWnd::newInstance()
{
    return TOEMsgWndPtr(new TOEMsgWnd);
}
#endif // STATIC_LIBRARY

//#endif // ENTERPRISE_VERSION

#if _MSC_VER >= 1400
#pragma warning(pop)
#endif


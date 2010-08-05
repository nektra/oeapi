/* $Id: oeapiobj.cpp,v 1.32 2009/01/28 14:09:19 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"

#include "oeapiobj.h"
#include "main.h"
#include "oe_toolbar.h"

#include "se_debug.h"

// there is a auto-generated oeapi.h in this project, so we must specify the path
#include "../plugin/oeapi.h"

#ifdef EVALUATION_VERSION
#include "eval_utils.h"
#endif // EVALUATION_VERSION

#include "oe_msgwnd.h"

int counter = 1;


using namespace comet;
using namespace OEAPI;


#if _MSC_VER >= 1400
#pragma warning(push)
// disable warning C4812: 
// obsolete declaration style: please use 'coclass_implementation<comet::OEAPI::OEAPIObj>::coclass_implementation' instead
#pragma warning(disable:4812) 
#endif



///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEAPIObj::TOEAPIObj()
{
	refCount_ = 0;
	id_ = counter++;
	lastMsgId_ = -1;
	OEAPIManager::Get()->AddOEAPI(this);
}

//---------------------------------------------------------------------------//
TOEAPIObj::~TOEAPIObj()
{
	if(OEAPIManager::Get(false)) {
		OEAPIManager::Get()->RemoveOEAPI(this);
	}
}

//---------------------------------------------------------------------------//
TOEToolbarPtr TOEAPIObj::CreateToolbar()
{
	TOEToolbarPtr ret(TOEToolbar::newInstance());
	ret->Create(id_);
	return ret;
}

//---------------------------------------------------------------------------//
TOEToolbarPtr TOEAPIObj::GetOEToolbar()
{
	TOEToolbarPtr ret(TOEToolbar::newInstance());

	TOEToolbar *tool = (TOEToolbar *) ret.get();
	if(tool) {
		tool->SetOEDefaultToolbar(MsgWndStyles::OE_MSG_INVALID, id_);
	}

	return ret;
}

//---------------------------------------------------------------------------//
int TOEAPIObj::GetFirstToolbarID()
{
	curToolbarIndex_ = 0;
	return GetNextToolbarID();
}

//---------------------------------------------------------------------------//
int TOEAPIObj::GetNextToolbarID()
{
	int ret = -1;

//	OEPluginToolbarMgr::Get()->Lock();
	OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbarByIndex(curToolbarIndex_++, id_);
	if(toolbar != NULL) {
		ret = toolbar->GetID();
		toolbar->Unlock();
	}
	else {
		curToolbarIndex_ = -1;
	}

//	OEPluginToolbarMgr::Get()->Unlock();

	return ret;
}

//---------------------------------------------------------------------------//
TOEToolbarPtr TOEAPIObj::GetToolbar(int toolbarId)
{
	TOEToolbarPtr ret;
//	OEPluginToolbarMgr::Get()->Lock();

	OEPluginToolbar *toolbar = OEPluginToolbarMgr::Get()->GetToolbar(toolbarId);
	if(toolbar) {
		if(toolbar->GetOEAPIID() == id_) {
			ret = TOEToolbar::newInstance();
			TOEToolbar *ttool = (TOEToolbar *) ret.get();
			if(ttool) {
				ttool->SetID(toolbarId);
				ttool->SetOEAPIID(id_);
			}
		}

		toolbar->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::GetButton: Cannot find toolbar object.\n"));
	}

//	OEPluginToolbarMgr::Get()->Unlock();
	return ret;
}

//---------------------------------------------------------------------------//
void TOEAPIObj::RemoveAllToolbars()
{
	OEPluginToolbarMgr::Get()->DestroyAllToolbars(id_);
}

//---------------------------------------------------------------------------//
void TOEAPIObj::SendAndReceiveMessages()
{
	OEAPIManager::Get()->SendAndReceiveMessages();
}

//---------------------------------------------------------------------------//
void TOEAPIObj::SendMessage(int folderId, int msgId)
{
	OEAPIManager::Get()->SendMailMessage(folderId, msgId);
}

//---------------------------------------------------------------------------//
DWORD TOEAPIObj::GetOEMainWindow()
{
	return (DWORD) OEAPI_GetOEMainWindowHandle();
}

//---------------------------------------------------------------------------//
LONGLONG TOEAPIObj::GetOEMainWindow2()
{
	return (LONGLONG) OEAPI_GetOEMainWindowHandle();
}

//---------------------------------------------------------------------------//
int TOEAPIObj::GetFirstMenuItemID()
{
	curItemIndex_ = 0;
	return GetNextMenuItemID();
}

//---------------------------------------------------------------------------//
int TOEAPIObj::GetNextMenuItemID()
{
	int ret = -1;
	OEAPIManager *mgr = OEAPIManager::Get();
	mgr->LockUIObjects();

	int i=0;
	MenuItemReprList &itemList = mgr->GetMenuItemsRepr();
	for(MenuItemReprList::iterator it=itemList.begin(); it!=itemList.end(); it++) {
		OEMenuItemRepr *item = *it;

		if(id_ == item->GetOEAPIID()) {
			if(i++ == curItemIndex_) {
				ret = item->GetID();
				break;
			}
		}
	}

	mgr->UnlockUIObjects();

	curItemIndex_++;

	return ret;
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEAPIObj::GetMenuItem(int itemId)
{
	TOEMenuItemPtr ret;
	OEMenuItemRepr *item = OEAPIManager::Get()->GetMenuItemRepr(itemId);

	if(item != NULL) {
		ret = TOEMenuItem::newInstance();
		TOEMenuItem *menuItem = (TOEMenuItem *) ret.get();
		if(menuItem) {
			menuItem->SetID(item->GetID());
			menuItem->SetMenuID(item->GetMenuID());
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
TOEMenuItemPtr TOEAPIObj::GetMsgWndMenuItem(int wndStyle, int itemId)
{
	TOEMenuItemPtr ret;
	OEMenuItemRepr *item = OEAPIManager::Get()->GetMenuItemRepr(itemId);

	if(item != NULL) {
		ret = TOEMenuItem::newInstance();
		TOEMenuItem *menuItem = (TOEMenuItem *) ret.get();
		if(menuItem) {
			menuItem->SetID(item->GetID());
			menuItem->SetWndStyle(wndStyle);
			menuItem->SetMenuID(item->GetMenuID());
			menuItem->SetMenuMgr(NULL);
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEAPIObj::RemoveAllMenuItems()
{
	OEAPIManager::Get()->DestroyAllMenuItemsByOEAPI(id_);
}

//---------------------------------------------------------------------------//
TOEMenuPtr TOEAPIObj::GetMenu(int index)
{
	TOEMenuPtr ret(TOEMenu::newInstance());
	TOEMenu *menu = (TOEMenu *) ret.get();
	menu->SetIndex(index);
	// menu->SetWndStyle(menu_main_wnd);
	menu->SetMenuMgr(NULL);
	menu->SetID(OEAPIManager::Get()->GetNewMenuID());
	menu->SetOEAPIID(id_);
	return ret;
}

//-----------------------------------------------------------//
TOEMenuPtr TOEAPIObj::GetMsgWndMenu(int wndStyle, int index)
{
	TOEMenuPtr ret(TOEMenu::newInstance());
	TOEMenu *menu = (TOEMenu *) ret.get();
	menu->SetIndex(index);
	menu->SetWndStyle(wndStyle);
	menu->SetMenuMgr(NULL);
	menu->SetID(OEAPIManager::Get()->GetNewMenuID());
	menu->SetOEAPIID(id_);
	return ret;
}


//---------------------------------------------------------------------------//
DWORD TOEAPIObj::GetMessageID(DWORD index)
{
	return OEAPIManager::Get()->GetMessageID(index);
}

//---------------------------------------------------------------------------//
DWORD TOEAPIObj::GetMessageIndex(DWORD msgId)
{
	return OEAPIManager::Get()->GetMessageIndex(msgId);
}

//---------------------------------------------------------------------------//
DWORD TOEAPIObj::GetSelectedFolderID()
{
#if defined(STANDARD_VERSION)
	return -1;
#endif // STANDARD_VERSION

	return OEAPIManager::Get()->GetSelectedFolderID();
}

//---------------------------------------------------------------------------//
void TOEAPIObj::SetSelectedFolderID(long selFolderId)
{
#if defined(STANDARD_VERSION)
	return;
#endif // STANDARD_VERSION

	OEAPIManager::Get()->SelectFolderID(selFolderId);
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OpenCurrentMessage()
{
#if defined(STANDARD_VERSION)
	return;
#endif // STANDARD_VERSION

	OEAPIManager::Get()->OpenCurrentMessage();
}

//---------------------------------------------------------------------------//
void TOEAPIObj::ComposeMessage()
{
#if defined(STANDARD_VERSION)
	return;
#endif // STANDARD_VERSION

	OEAPIManager::Get()->ComposeMessage();
}

//---------------------------------------------------------------------------//
DWORD TOEAPIObj::GetFirstSelectedMessageID()
{
#if defined(STANDARD_VERSION)
	return -1;
#endif // STANDARD_VERSION

	lastMsgId_ = OEAPIManager::Get()->GetFirstSelectedMessage();
	return lastMsgId_;
}

//---------------------------------------------------------------------------//
DWORD TOEAPIObj::GetNextSelectedMessageID()
{
#if defined(STANDARD_VERSION)
	return -1;
#endif // STANDARD_VERSION

	lastMsgId_ = OEAPIManager::Get()->GetNextSelectedMessage(lastMsgId_);
	return lastMsgId_;
}

//---------------------------------------------------------------------------//
void TOEAPIObj::SetSelectedMessageID(DWORD msgId)
{
#if defined(STANDARD_VERSION)
	return;
#endif // STANDARD_VERSION

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return;
	}
#endif // EVALUATION_VERSION

	int index = OEAPIManager::Get()->GetMessageIndex(msgId);
	if(index != -1) {
		OEAPI_SetSelectedMessage(1, &index);
	}
	else if(msgId == -1)
	{
		//index = 0;
		OEAPI_SetSelectedMessage(1, &index);
	}
}

//---------------------------------------------------------------------------//
void TOEAPIObj::AddSelectedMessageID(DWORD msgId)
{
#if defined(STANDARD_VERSION)
	return;
#endif // STANDARD_VERSION

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return;
	}
#endif // EVALUATION_VERSION

	int index = OEAPIManager::Get()->GetMessageIndex(msgId);
	if(index != -1) {
		OEAPI_AddSelectedMessage(index);
	}
}

//---------------------------------------------------------------------------//
DWORD TOEAPIObj::GetCurrentMessageID()
{
#if defined(STANDARD_VERSION)
	return -1;
#endif // STANDARD_VERSION

	return OEAPIManager::Get()->GetCurrentMessageID();
}

//---------------------------------------------------------------------------//
BOOL TOEAPIObj::IsAnyMessageSelected()
{
#if defined(STANDARD_VERSION)
	return FALSE;
#endif // STANDARD_VERSION

	return OEAPI_IsAnyMessageSelected();
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnFolderSelectionChangedPrivate(DWORD newSelFolderId)
{
	lastMsgId_ = -1;
	OnFolderSelectionChanged(newSelFolderId);
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnToolbarButtonClicked(int toolbarId, int buttonId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnToolbarButtonClicked(toolbarId, buttonId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnToolbarButtonClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnMenuItemClicked(int menuId, int menuItemId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnMenuItemClicked(menuId, menuItemId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnMenuItemClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnMsgWndMenuItemClicked(int menuId, int menuItemId, int msgWndId)
{
#ifndef STATIC_LIBRARY
	try {
		connection_point.Fire_OnMsgWndMenuItemClicked(menuId, menuItemId, msgWndId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnMenuItemClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}
//---------------------------------------------------------------------------//
void TOEAPIObj::OnFolderSelectionChanged(DWORD newSelFolderId)
{
#if (!defined(STATIC_LIBRARY) && !defined(STANDARD_VERSION))
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return;
	}
#endif // EVALUATION_VERSION

	try {
		connection_point.Fire_OnFolderSelectionChanged(newSelFolderId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnFolderSelectionChanged: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY && !STANDARD_VERSION
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnMessageSelectionChanged()
{
#if (!defined(STATIC_LIBRARY) && !defined(STANDARD_VERSION))
	try {
		connection_point.Fire_OnMessageSelectionChanged();
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnMessageSelectionChanged: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY && !STANDARD_VERSION
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnCurrentMessageChanged(DWORD msgId)
{
#if (!defined(STATIC_LIBRARY) && !defined(STANDARD_VERSION))
	try {
		connection_point.Fire_OnCurrentMessageChanged(msgId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnCurrentMessageChanged: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY && !STANDARD_VERSION
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnMessageDownloadStarted()
{
#if (!defined(STATIC_LIBRARY) && !defined(STANDARD_VERSION))
	try {
		connection_point.Fire_OnMessageDownloadStarted();
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnMessageDownloadStarted: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY && !STANDARD_VERSION
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnMessageDownloadFinished()
{
#if (!defined(STATIC_LIBRARY) && !defined(STANDARD_VERSION))
	try {
		connection_point.Fire_OnMessageDownloadFinished();
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnMessageDownloadFinished: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY && !STANDARD_VERSION
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnNewMessageInOutbox(DWORD msgId)
{
#if (!defined(STATIC_LIBRARY) && !defined(STANDARD_VERSION))
	try {
		connection_point.Fire_OnNewMessageInOutbox(msgId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnNewMessageInOutbox: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY && !STANDARD_VERSION
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnDatabaseChange(comet::OEAPI::tagOE_DATABASE_TRANSACTION dt, int folderId,
									int msgId, int parentId)
{
#if (!defined(STATIC_LIBRARY) && !defined(STANDARD_VERSION))
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return;
	}
#endif // EVALUATION_VERSION

	try {
		connection_point.Fire_OnDatabaseChange(dt, folderId, msgId, parentId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnDatabaseChange: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY && !STANDARD_VERSION
}

//---------------------------------------------------------------------------//
TOEMsgWndPtr TOEAPIObj::GetMsgWnd(int msgWndId)
{
	TOEMsgWndPtr ret;

	ret = TOEMsgWnd::newInstance();
	TOEMsgWnd *mwnd = (TOEMsgWnd *) ret.get();
	if(mwnd) {
		mwnd->SetID(msgWndId);
		mwnd->SetOEAPIID(id_);
		if(mwnd->GetID() == -1) {
			ret = NULL;
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
int TOEAPIObj::GetFirstMsgWndID()
{
	curMsgWndIndex_ = 0;
	return GetNextMsgWndID();
}

//---------------------------------------------------------------------------//
int TOEAPIObj::GetNextMsgWndID()
{
	int ret = -1;

#ifdef ENTERPRISE_VERSION
	if(curMsgWndIndex_ == -1) {
		return ret;
	}

	OEPluginMsgWnd *msgWnd = MsgWndMgr.GetMsgWndByIndex(curMsgWndIndex_++);
	if(msgWnd != NULL) {
		ret = msgWnd->GetID();
		msgWnd->Unlock();
	}
	else {
		curMsgWndIndex_ = -1;
	}
#endif // (EVALUATION_VERSION || STANDARD_VERSION) && !ENTERPRISE_VERSION

	return ret;
}

//---------------------------------------------------------------------------//
int TOEAPIObj::GetActiveMsgWndID()
{
	int ret = -1;

#ifdef ENTERPRISE_VERSION
	ret = MsgWndMgr.GetActiveMsgWndID();
#endif // (EVALUATION_VERSION || STANDARD_VERSION) && !ENTERPRISE_VERSION

	return ret;
}

//---------------------------------------------------------------------------//
TOEToolbarPtr TOEAPIObj::CreateToolbarInMsgWnd(int wndStyle)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		TOEToolbarPtr ret;
		return ret;
	}
#endif // EVALUATION_VERSION

	TOEToolbarPtr ret(TOEToolbar::newInstance());
	ret->CreateInMsgWnd((comet::OEAPI::tagWNDSTYLE) wndStyle, id_);
	return ret;
}

//---------------------------------------------------------------------------//
TOEToolbarPtr TOEAPIObj::GetOEToolbarInMsgWnd(int wndStyle)
{
#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		TOEToolbarPtr ret;
		return ret;
	}
#endif // EVALUATION_VERSION

	TOEToolbarPtr ret(TOEToolbar::newInstance());

	TOEToolbar *tool = (TOEToolbar *) ret.get();
	if(tool) {
		tool->SetOEDefaultToolbar(wndStyle, id_);
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEAPIObj::SendKey(long keyCode, long controlCode, BOOL setForeground)
{
	OEPluginToolbarMgr::Get()->GetMainWndMgr()->SendKey((BYTE)keyCode, (BYTE)controlCode, setForeground);
}

//---------------------------------------------------------------------------//
bstr_t TOEAPIObj::GetSubjectOfCurrentMsg()
{
	bstr_t ret;
	OEPluginMsgWnd *msgWnd;

	msgWnd = MsgWndMgr.GetCurrentMsgWnd();
	if(msgWnd) {
		ret = msgWnd->GetSubject();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::GetSubjectOfCurrentMsg: Msg Wnd NULL.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEAPIObj::SetSubjectOfCurrentMsg(const bstr_t &subject)
{
	BOOL ret = FALSE;
	OEPluginMsgWnd *msgWnd;

	msgWnd = MsgWndMgr.GetCurrentMsgWnd();
	if(msgWnd) {
		ret = msgWnd->SetSubject(subject);
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::SetSubjectOfCurrentMsg: Msg Wnd NULL.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEAPIObj::GetBodyInMainWnd()
{
	bstr_t ret;
	OEPluginMsgWnd *msgWnd;

	msgWnd = MsgWndMgr.GetCurrentMsgWnd();
	if(msgWnd) {
		ret = msgWnd->GetBody();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::GetBodyOfCurrentMsg: Msg Wnd NULL.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEAPIObj::SetBodyInMainWnd(const bstr_t &body)
{
	BOOL ret = FALSE;
	OEPluginMsgWnd *msgWnd;

	msgWnd = MsgWndMgr.GetCurrentMsgWnd();
	if(msgWnd) {
		ret = msgWnd->SetBody(body);
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::SetBodyOfCurrentMsg: Msg Wnd NULL.\n"));
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnToolbarButtonMsgWndClicked(int toolbarId, int buttonId, int msgWndId)
{
#if !defined(STATIC_LIBRARY) && defined(ENTERPRISE_VERSION)
	try {
		connection_point.Fire_OnToolbarButtonMsgWndClicked(toolbarId, buttonId, msgWndId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnToolbarButtonMsgWndClicked: Error Connection point.\n"));
	}
#endif // STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnSendButtonMsgWndClicked(long msgWndId)
{
#if !defined(STATIC_LIBRARY)
	try {
		connection_point.Fire_OnSendButtonMsgWndClicked(msgWndId);
	}
	catch(...) {
		debug_print(DEBUG_ERROR, _T("OnSendButtonMsgWndClicked: Error Connection point.\n"));
	}
#endif // !STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnNewMsgWnd(long msgWndId)
{
#if !defined(STATIC_LIBRARY)
	TOEMsgWndPtr msgWnd = GetMsgWnd(msgWndId);

	if(!msgWnd.is_null()) {
		try {
			connection_point.Fire_OnNewMsgWnd(msgWndId);
		}
		catch(...) {
			debug_print(DEBUG_ERROR, _T("OnNewMsgWnd: Error Connection point.\n"));
		}
	}
#endif // !STATIC_LIBRARY
}

//---------------------------------------------------------------------------//
void TOEAPIObj::OnMsgWndClosed(long msgWndId, BOOL isMainWindow)
{
#if !defined(STATIC_LIBRARY)
	TOEMsgWndPtr msgWnd = GetMsgWnd(msgWndId);

	if(isMainWindow || !msgWnd.is_null()) {
		try {
			connection_point.Fire_OnMsgWndClosed(msgWndId, isMainWindow);
		}
		catch(...) {
			debug_print(DEBUG_ERROR, _T("OnMsgWndClosed: Error Connection point.\n"));
		}
	}
#endif // !STATIC_LIBRARY
}


//-----------------------------------------------------------//
//void TOEAPIObj::OnMsgWndMenuItemClicked(int menuId, int menuItemId, int msgWndId)
//{
//}

#if _MSC_VER >= 1400
#pragma warning(pop)
#endif

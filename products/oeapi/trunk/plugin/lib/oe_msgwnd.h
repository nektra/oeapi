/* $Id: oe_msgwnd.h,v 1.10.8.4 2007/08/22 20:10:16 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __OE_MSG_WND_H
#define __OE_MSG_WND_H

#pragma warning(disable:4786)

#include <windows.h>
#include <list>
#include <tchar.h>
#include <comet/bstr.h>
#include <mshtml.h>			// for IHTMLDocument2
#include "nkt_lock.h"
#include "oeapi.h"

class OEPluginTopLevelWndMgr;
class OEPluginMsgWnd;
class OEPluginMsgWndMgr;

class OEPluginMenuMgr;

extern OEPluginMsgWndMgr MsgWndMgr;

typedef std::list<OEPluginMsgWnd*> OEPluginMsgWndList;

//using namespace comet;



/**
This class contains all created message windows in OE.
It is a unique object that store all the information of the created message windows.
*/
//---------------------------------------------------------------------------------//
class OEPluginMsgWndMgr {
public:
	OEPluginMsgWndMgr();
	~OEPluginMsgWndMgr();

	/**
	Add a message window.
	If isPreviewMsgWnd the OEPluginMsgWnd is created without a top level object as it is
	the message window of OE main window.
	*/
	OEPluginMsgWnd *AddWindow(HWND hMsg, BOOL isPreviewMsgWnd = FALSE);

	/**
	Remove a message window.
	*/
	VOID RemoveWindow(HWND hMsg);

	/**
	Get OEPluginMsgWnd by handle.
	If lock is TRUE the returned object is locked and must be unlocked by the caller.
	*/
	OEPluginMsgWnd *GetMsgWnd(HWND hMsg, BOOL lock = TRUE);

	/**
	Get OEPluginMsgWnd by index.
	If lock is TRUE the returned object is locked and must be unlocked by the caller.
	*/
	OEPluginMsgWnd *GetMsgWndByIndex(UINT index, BOOL lock = TRUE);

	/**
	Get message window of the current message.
	If lock is TRUE the returned object is locked and must be unlocked by the caller.
	*/
	OEPluginMsgWnd *GetCurrentMsgWnd(BOOL lock = TRUE);

	/**
	Get the number of active message windows.
	*/
	int GetMsgWndCount() { return (int)msgWndList_.size(); }

	/**
	Get the msg wnd id of the msg wnd that is in the front. The returned msg wnd
	could be the msg wnd of the main window.
	*/
	int GetActiveMsgWndID();

protected:
	/**
	Lock msg wnd list.
	*/
	void Lock();

	/**
	Unlock msg wnd list.
	*/
	void Unlock();

	/**
	Try lock msg wnd list.
	*/
	BOOL TryLock();

	/**
	Lock msg wnd AND wnd list
	*/
	BOOL TryLockMsgWnd(OEPluginMsgWnd* msgWnd);

	/**
	Unlock msg wnd AND wnd list
	*/
	void UnlockMsgWnd(OEPluginMsgWnd* msgWnd);

private:
	OEPluginMsgWndList msgWndList_;

	//CRITICAL_SECTION lock_;
	NktLock lock_;
	//CRITICAL_SECTION lockAux_;
	//LONG lockCount_;
};


/**
This class represent a message window in OE.
Message windows are the windows created when the user double-click a message or press
reply / forward / create mail buttons.
*/
//---------------------------------------------------------------------------------//
class OEPluginMsgWnd {
public:
	typedef MsgWndStyles::WNDSTYLE WNDSTYLE;

	OEPluginMsgWnd(HWND hMsg);
	~OEPluginMsgWnd();


	/**
	Return TRUE if this object represents the html object displayed in OE main window.
	*/
	BOOL IsMainWndMsgWnd() { return (topLevelMgr_ == NULL); }

	/**
	Return TRUE if this object represent the msg wnd that is displayed in OE main window
	to show the current message.
	This is a particular case of the MainWndMgsWnd
	*/
	BOOL IsPreviewMsgWnd() { return isPreviewMsgWnd_; }

	/**
	Only useful for message windows of OE main window.
	This function return TRUE when there wasn't any message selected when this msg wnd
	was created.
	This flag is useful to know if this msg wnd represents a empty msg.
	*/
	BOOL IsEmptyMsg() { return emptyMsgWnd_; }

	/**
	Set that this msg wnd represents the empty selection message.
	*/
	VOID SetEmptyMsgWnd(BOOL empty) { emptyMsgWnd_ = empty; }

	/**
	Set that this object represents a preview msg wnd.
	*/
	VOID SetPreviewMsgWnd() { isPreviewMsgWnd_ = TRUE; }

	/**
	Get the msg body.
	*/
	comet::bstr_t GetBody();

	/**
	Get the msg html body.
	*/
	comet::bstr_t GetBodyHTML();

	/**
	Get the length of the msg body.
	*/
	int GetBodyLength();

	/**
	Set the msg body.
	This function send a msg to set the body. In the wnd proc of this wnd 
	SetBodyGUIThread is called to set the body content.
	*/
	BOOL SetBody(const comet::bstr_t &body);

	/**
	Set the msg body using put_innerHTML function.
	This function send a msg to set the body. In the wnd proc of this wnd 
	SetBodyGUIThread is called to set the body content.
	*/
	BOOL SetBodyHTML(const comet::bstr_t &body);

	/**
	Get the first address of the msg wnd. By default is 'To:'.
	Does not work.
	*/
	comet::bstr_t GetTo();

	/**
	Set the first address of the msg wnd. By default is 'To:'.
	Does not work.
	*/
	BOOL SetTo(const comet::bstr_t &address);

	/**
	Get the second address of the msg wnd. By default is 'Cc:'.
	Does not work.
	*/
	comet::bstr_t GetCc();

	/**
	Set the second address of the msg wnd. By default is 'To:'.
	Does not work.
	*/
	BOOL SetCc(const comet::bstr_t &address);

	/**
	Cancel send process of the message window.
	When the user presses 'Send' button or it is pressed by code, the flag retrieved in
	function SendCancelled() it set FALSE and this function set it to TRUE.
	If the flag is TRUE after calling the 'Send' event handlers the message is not finally sent.
	*/
	void SetSendCancelled(BOOL cancelled=FALSE) { sendCancelled_ = cancelled; }

	/**
	*/
	void SetSendEvent(BOOL sent) { sentEvent_ = sent; }

	/**
	*/
	BOOL GetSendEvent() { return sentEvent_; }

	/**
	Return TRUE if the Send process wasn cancelled.
	*/
	BOOL SendCancelled() { return sendCancelled_; }

	/**
	Get the subject of the msg wnd.
	*/
	comet::bstr_t GetSubject();

	/**
	Set the subject of the msg wnd.
	*/
	BOOL SetSubject(const comet::bstr_t &subject);

	/**
	Get the text of the control.
	*/
	comet::bstr_t GetControlText(UINT ctrlId);

	/**
	Set the text of the control.
	*/
	BOOL SetControlText(UINT ctrlId, const comet::bstr_t &str);

	/**
	Lock the wnd to prevent the wnd close.
	If it returns FALSE, the object is about to be destroyed.
	If processEvents = TRUE, the function will process thread messages while it waits until 
	there are not locks taken. It is used from the wnd procs to prevent a dead lock if the
	plugin do a SendMessage while it is waiting a lock.
	*/
	BOOL Lock(BOOL processMsgs = FALSE);

	/**
	Unlock the object.
	*/
	VOID Unlock();

	/**
	Try Lock 
	*/
	BOOL TryLock();

	/**
	Call this function ONLY from the GUI thread.
	if usePutInnerMethod is TRUE the body is set using put_innerHTML function.
	*/
	BOOL SetBodyGUIThread(LPCWSTR body, BOOL usePutInnerMethod);

	/**
	Set the toolbar mgr that have the toolbar info.
	*/
	VOID SetTopLevelWndMgr(OEPluginTopLevelWndMgr *mgr) { topLevelMgr_ = mgr; }

	/**
	Get the toolbar mgr of the window.
	*/
	OEPluginTopLevelWndMgr *GetTopLevelWndMgr() { return topLevelMgr_; }

	/**
	Get the handle of the window associated to the object as a integer.
	*/
	INT GetID() { return (INT) HandleToLong(GetWindowHandle()); }

	/**
	Get the handle of the window associated to the object.
	*/
	HWND GetWindowHandle() { return hWnd_; }

	/**
	Get the handle window associated to the object. If the window is in the main window
	it returns the handle of the top-level window.
	*/
	HWND GetTopLevelWindowHandle();

	/**
	Set the old proc of the window.
	*/
	VOID SetOldWndProc(WNDPROC oldProc) { oldProc_ = oldProc; }

	/**
	Get the old proc of the window.
	*/
	WNDPROC GetOldWndProc() { return oldProc_; }

	/**
	Set the main wnd and create the defined toolbars for the window.
	*/
	VOID Initialize(HWND hMainWnd);

	/**
	Get the rebar wnd.
	*/
	BOOL Initialized() { return init_; }

	/**
	Set the timer id
	*/
	VOID SetTimerId(UINT timerId) { timerId_ = timerId; }

	/**
	Get the timer id or 0 if not set
	*/
	DWORD GetTimerId() { return timerId_; }

	/**
	Set menu manager
	*/
	VOID SetMenuMgr(OEPluginMenuMgr* menuMgr) { menuMgr_ = menuMgr; }

	/**
	Get menu manager
	*/
	OEPluginMenuMgr* GetMenuMgr() { return menuMgr_; }

	VOID SetLastMenuSelect(int menuSelect) { lastMenuSelect_ = menuSelect; }
	INT GetLastMenuSelect() { return lastMenuSelect_; }

	VOID SetLastInitMenu(HMENU initMenu) { lastInitMenu_ = initMenu; }
	HMENU GetLastInitMenu() { return lastInitMenu_; }

	WNDSTYLE GetWndStyle() { return wndStyle_; } 
	VOID SetWndStyle(WNDSTYLE style) { wndStyle_ = style; }

	VOID GetIHTMLDocument2(IDispatch** htmldoc);

private:
	BOOL GetHTMLDocument();

	HWND FindBodyCtrl();

	static DWORD CALLBACK ReadEditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK WriteEditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

	HWND hWnd_;

	// window where the HTML object is located.
	HWND ieServer_;
	
	BOOL init_;
	WNDPROC oldProc_;

	BOOL isPreviewMsgWnd_;
	OEPluginTopLevelWndMgr *topLevelMgr_;
	UINT timerId_;
	IHTMLDocument2 *pHTMLDocument2_;

	OEPluginMenuMgr *menuMgr_;
	INT lastMenuSelect_;
	HMENU lastInitMenu_;

	// lock used to prevent a wnd closed while the plugin user has the object.
	//CRITICAL_SECTION lock_;
	NktLock lock_;

	// lock used to keep the lock count to prevent lock
	//CRITICAL_SECTION lockAux_;

	//volatile int lockCount_;

	// if TRUE the object is in the destructor
	BOOL destroyed_;
	// count the number of threads trying to lock. Used to know if the object can be destroyed
	// or not.
	volatile int locking_;

	// flag used to know if the Send process was cancelled
	BOOL sendCancelled_;

	// flag useful for msg windows of OE main window. This flag is TRUE if this msg wnd
	// represents a empty message or the window displayed when no message is selected.
	BOOL emptyMsgWnd_;

	WNDSTYLE wndStyle_;

	BOOL sentEvent_;
};


#endif // __OE_MSG_WND_H
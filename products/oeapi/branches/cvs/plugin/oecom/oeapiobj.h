/* $Id: oeapiobj.h,v 1.20.8.1 2007/07/23 19:56:37 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#pragma once

#include "user_interface.h"
#include "oemsgwnd.h"

// forward declaration
//class TOEMsgWndPtr;
//class TOEToolbarPtr;
//class TOEMenuItemPtr;
//class TOEMenuPtr;

//typedef std::list<int> IntList;

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY

class TOEAPIObj;
typedef AutoPtr<TOEAPIObj> TOEAPIObjPtr;

class TOEAPIObj

#else

#define TOEAPIObj coclass_implementation<comet::OEAPI::OEAPIObj>

template<> class TOEAPIObj : public comet::coclass<comet::OEAPI::OEAPIObj>

#endif // STATIC_LIBRARY
{
	friend class OEAPIManager;
public:
	TOEAPIObj();
	~TOEAPIObj();

	static TCHAR *get_progid() { return _T("OEAPI.OEAPIObj"); }

//#ifdef ENTERPRISE_VERSION
	/**
	Get the message window by id.
	*/
	TOEMsgWndPtr GetMsgWnd(int msgWndId);

	/**
	Get first msg wnd id of the OEAPIObj.
	*/
	int GetFirstMsgWndID();

	/**
	Get next msg wnd id of the OEAPIObj.
	It returns -1 when finished.
	*/
	int GetNextMsgWndID();

	/**
	Only in Enterprise Edition.
	Get the msg wnd id of the msg wnd that is in the front. The returned msg wnd
	could be the msg wnd of the main window.
	*/
	int GetActiveMsgWndID();

	/**
	Create a toolbar in send message window.
	It will appear when the user reply / create / forward a mail.
	*/
	TOEToolbarPtr CreateToolbarInMsgWnd(INT wndStyle);

	/**
	Get the OE toolbar in msg wnd.<br>
	It will appear when the user reply / create / forward a mail.
	*/
	TOEToolbarPtr GetOEToolbarInMsgWnd(int wndStyle);

	/**
	Send a Key to the OE main window.
	keyCode contains the code of the key and controlCode the control combination (e.g.: VK_CONTROL).
	If setForeground is TRUE, the window is set as foreground window before sending the key.
	*/
	void SendKey(long keyCode, long controlCode, BOOL setForeground);

	/**
	Get the subject of current message.
	*/
	comet::bstr_t GetSubjectOfCurrentMsg();

	/**
	Set the subject of current message.
	*/
	BOOL SetSubjectOfCurrentMsg(const comet::bstr_t &subject);

	/**
	Get the body of current message.
	*/
	comet::bstr_t GetBodyInMainWnd();

	/**
	Set the body of current message.
	*/
	BOOL SetBodyInMainWnd(const comet::bstr_t &body);

	/**
	Raise the event
	*/
	virtual void OnToolbarButtonMsgWndClicked(int toolbarId, int buttonId, int msgWndId);

	/**
	Event triggered when the 'Send' button in a message window is clicked.
	*/
	virtual void OnSendButtonMsgWndClicked(long msgWndId);

	/**
	Event triggered when a message window is created.
	*/
	virtual void OnNewMsgWnd(long msgWndId);

	/**
	Event triggered when a message window is closed.
	*/
	virtual void OnMsgWndClosed(long msgWndId, BOOL isMainWindow);

//#endif // ENTERPRISE_VERSION

	/**
	Create a toolbar
	*/
	TOEToolbarPtr CreateToolbar();

	/**
	Get the OE toolbar.
	*/
	TOEToolbarPtr GetOEToolbar();

	/**
	Get the first toolbar owned by this object
	*/
	int GetFirstToolbarID();

	/**
	Get next toolbar owned by this object
	*/
	int GetNextToolbarID();

	/**
	Get toolbar by id
	*/
	TOEToolbarPtr GetToolbar(int toolbarId);

	/**
	Get the first item owned by this object
	*/
	int GetFirstMenuItemID();

	/**
	Get next item owned by this object
	*/
	int GetNextMenuItemID();

	/**
	Get item by id
	*/
	TOEMenuItemPtr GetMenuItem(int itemId);

	/**
	Get item by id
	*/
	TOEMenuItemPtr GetMsgWndMenuItem(int wndStyle, int itemId);

	/**
	Remove all the menu items owned by this OEAPI
	*/
	void RemoveAllMenuItems();

	/**
	Remove all the toolbars owned by this OEAPI
	*/
	void RemoveAllToolbars();

	/**
	Send messages in Outbox folder and receive pending messages in server.<br>
	Equivalent to press 'Send/Recv' button but it doesn't display the dialog.
	*/
	void SendAndReceiveMessages();

	/**
	Create a message in Outbox folder to be sent.<br>
	To send it immediately use call SendAndReceiveMessages after this function.
	*/
	void SendMessage(int folderId, int msgId);

	/**
	Get the handle of OE main window.
	*/
	DWORD GetOEMainWindow();

	/**
	Get the id of the oeapi
	*/
	int GetID() { return id_; }

	/**
	Get menu by index
	*/
	TOEMenuPtr GetMenu(int index);

	/**
	Get menu by index
	*/
	TOEMenuPtr GetMsgWndMenu(int wndStyle, int index);

	/**
	Return the id of the message specified by the index of the list view
	or -1 if fail.
	*/
	DWORD GetMessageID(DWORD index);

	/**
	Return the index in the list view of the message specified by the msg id
	or -1 if fail.
	*/
	DWORD GetMessageIndex(DWORD msgId);

	/**
	Return the selected folder id. It returns -1 if the folder is not local (hotmail or
	a newsgroup)
	*/
	DWORD GetSelectedFolderID();

	/**
	Select folder id.
	*/
	void SetSelectedFolderID(long selFolderId);

	/**
	Only in Professional / Enterprise Edition.<br>
	Display current message in a new window: equivalent to double-click the message.<br>
	Current message is displayed in the bottom part of OE.
	*/
	void OpenCurrentMessage();

	/**
	Only in Professional / Enterprise Edition.<br>
	Open a new message window to compose a message: equivalent to press 'Create Mail' button.
	*/
	void ComposeMessage();

	/**
	Return the first selected message.
	*/
	DWORD GetFirstSelectedMessageID();

	/**
	Return next selected message.
	*/
	DWORD GetNextSelectedMessageID();

	/**
	Set the next selected message in the current folder. This function clears the preious
	selection, after calling this function, only msgId will be selected.
	This function also changes the current message.
	*/
	void SetSelectedMessageID(DWORD msgId);

	/**
	Add a message to the current selection. After calling this function, the previous
	selection is kept and msgId is added to the selection.
	*/
	void AddSelectedMessageID(DWORD msgId);

	/**
	Get the current message id of the current folder. Current message is displayed in the bottom part of OE.
	*/
	DWORD GetCurrentMessageID();

	/**
	Return TRUE if there is any message selected in the message list view.<br>
	NOTE: this function returns valid values even if a hotmail or IMAP folder is selected.
	*/
	BOOL IsAnyMessageSelected();

	/**
	Raise the event
	*/
	virtual void OnToolbarButtonClicked(int toolbarId, int buttonId);

	/**
	Raise the event
	*/
	virtual void OnMenuItemClicked(int menuId, int menuItemId);

	/**
	Raise the event
	*/
	virtual void OnMsgWndMenuItemClicked(int menuId, int menuItemId, int wndMsgId);

	/**
	Raise this event when the selected folder changes.
	*/
	virtual void OnFolderSelectionChanged(DWORD newSelFolderId);

	/**
	Raise this event when the selected messages changes.
	*/
	virtual void OnMessageSelectionChanged();

	/**
	Raise this event when the current message that is displayed in the bottom part of OE changes.
	*/
	virtual void OnCurrentMessageChanged(DWORD msgId);

	/**
	Raise this event when the message download starts.
	*/
	virtual void OnMessageDownloadStarted();

	/**
	Raise this event when the message download finishes.
	*/
	virtual void OnMessageDownloadFinished();

	/**
	Raise this event when a message is created in Outbox folder to send it.
	*/
	virtual void OnNewMessageInOutbox(DWORD msgId);

	/**
	Raise event that is triggered when a message or folder changes.
	*/
	virtual void OnDatabaseChange(comet::OEAPI::tagOE_DATABASE_TRANSACTION dt, int folderId,
									int msgId, int parentId);

#ifdef STATIC_LIBRARY
	static TOEAPIObj *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static comet::com_ptr<comet::OEAPI::IOEAPIObj> newInstance();
#endif // STATIC_LIBRARY

protected:
	/**
	Raise this event OnFolderSelectionChanged and clear the current message
	*/
	void OnFolderSelectionChangedPrivate(DWORD newSelFolderId);

private:
	int id_;
//	IntList buttons_;
//	IntList toolbars_;
//	IntList menus_;
//	IntList menuItems_;
	DWORD lastMsgId_;
	int curToolbarIndex_;
	int curMsgWndIndex_;
	int curItemIndex_;
	int refCount_;
};


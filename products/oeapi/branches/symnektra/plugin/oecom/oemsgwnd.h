/* $Id: oemsgwnd.h,v 1.15 2008/09/07 16:56:54 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#pragma once

#include "user_interface.h"

class OEPluginMsgWnd;


//#ifdef ENTERPRISE_VERSION
//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY

class TOEMsgWnd;
typedef AutoPtr<TOEMsgWnd> TOEMsgWndPtr;

class TOEMsgWnd

#else

#define TOEMsgWndPtr comet::com_ptr<comet::OEAPI::IOEMsgWnd>
#define TOEMsgWnd coclass_implementation<comet::OEAPI::OEMsgWnd>

template<> class TOEMsgWnd : public comet::coclass<comet::OEAPI::OEMsgWnd>

#endif // STATIC_LIBRARY
{
public:
	TOEMsgWnd();
	~TOEMsgWnd();

	static TCHAR *get_progid() { return _T("OEAPI.OEMsgWnd"); }

	/**
	Get the 'To' address list of the message.
	*/
	comet::bstr_t GetTo();

	/**
	Set the 'To' address list of the message.
	*/
	void SetTo(const comet::bstr_t &subject);

	/**
	Get the 'Cc' address list of the message.
	*/
	comet::bstr_t GetCc();

	/**
	Set the 'Cc' address list of the message.
	*/
	void SetCc(const comet::bstr_t &subject);

	/**
	Get the 'Cc' address list of the message.
	*/
	comet::bstr_t GetBcc();

	/**
	Set the 'Cc' address list of the message.
	*/
	void SetBcc(const comet::bstr_t &subject);

	/**
	Cancel send process of the message window.<br>
	This function should be called inside the event IOEAPIObjEvents::OnSendButtonMsgWndClicked
	to cancel the 'Send' action started by the user or by code.<br>
	After calling this function other handlers of the event are not called and the message is not sent.
	*/
	void CancelSend();

	/**
	Get the subject of the message.
	*/
	comet::bstr_t GetSubject();

	/**
	Set the subject of the message.
	*/
	void SetSubject(const comet::bstr_t &subject);

	/**
	Get the body of the message part of the window.
	*/
	comet::bstr_t GetBody();

	/**
	Get the body of the message part of the window.
	*/
	comet::bstr_t GetBodyHTML();

	/**
	Get the body length of the message part of the window.
	*/
	int GetBodyLength();

	/**
	Set the body of the message part of the window.
	*/
	void SetBody(const comet::bstr_t &body);

	/**
	Set the body of the message part of the window.
	This function works in the same way of the SetBody function but it sets the body in
	a different way. This function is recommended if you want to modify an outgoing message
	because some mail clients experience problems showing html messages set with the
	SetBody function.
	*/
	void SetBodyHTML(const comet::bstr_t &body);

	/**
	Get the window style.
	*/
	comet::OEAPI::tagWNDSTYLE GetWndStyle();

	/**
	Press the 'Send' button of the window.<br>
	Only valid for windows with wnd style different than OE_MSG_DETAIL_WND.
	*/
	void SendMail();

	/**
	Send a Key to the msg window.
	keyCode contains the code of the key and controlCode the control combination (e.g.: VK_CONTROL).
	If setForeground is TRUE, the window is set as foreground window before sending the key.
	*/
	void SendKey(long keyCode, long controlCode, BOOL setForeground);

	/**
	Get toolbar by index
	*/
	TOEToolbarPtr GetToolbarByIndex(int toolbarIdx);

	/**
	Get toolbar by id
	*/
	TOEToolbarPtr GetToolbar(int toolbarId);

	/**
	Get menu by idx
	*/
	TOEMenuPtr GetMenu(int menuIdx);

	/**
	Get menu item by id
	*/
	TOEMenuItemPtr GetMenuItem(int itemId);

	/**
	Set the id of the msg wnd.
	*/
	void SetID(int id);

	/**
	Get the id of the msg wnd.
	*/
	int GetID();

	void SetOEAPIID(int oeapiId) { oeapiId_ = oeapiId; }
	int GetOEAPIID() { return oeapiId_;  }

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
	virtual void OnClose(int isMainWnd);

	/**
	Get IHTMLDocument2
	*/
	comet::com_ptr<::IDispatch> GetIHTMLDocument2();

	BOOL Close();


#ifdef STATIC_LIBRARY
	static TOEMsgWnd *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static comet::com_ptr<comet::OEAPI::IOEMsgWnd> newInstance();
#endif // STATIC_LIBRARY

protected:

private:
	int oeapiId_;
	OEPluginMsgWnd *msgWnd_;
};

//#endif // ENTERPRISE_VERSION

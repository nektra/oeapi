/* $Id: atl_addin.cpp,v 1.6 2008/09/07 16:57:07 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2006-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

// atl_addin.cpp : Implementation of Catl_addin

#include "stdafx.h"
#include "atl_addin.h"
#include <sstream>
#include <iostream>
#include <string>
#include <HtmlHelp.h>

#ifdef _DEBUG
# pragma comment(lib, "comsuppwd.lib")
#else
# pragma comment(lib, "comsuppw.lib")
#endif 

_ATL_FUNC_INFO NoParamInfo = { CC_STDCALL, VT_EMPTY, 0 };
_ATL_FUNC_INFO OneLongParamInfo = { CC_STDCALL, VT_EMPTY, 1, { VT_UINT } };
_ATL_FUNC_INFO TwoLongParamInfo = { CC_STDCALL, VT_EMPTY, 2, { VT_UINT, VT_UINT } };
_ATL_FUNC_INFO ThreeLongParamInfo = { CC_STDCALL, VT_EMPTY, 3, { VT_UINT, VT_UINT, VT_UINT } };
_ATL_FUNC_INFO FourLongParamInfo = { CC_STDCALL, VT_EMPTY, 4, { VT_UINT, VT_UINT, VT_UINT, VT_UINT } };

// Catl_addin

STDMETHODIMP Catl_addin::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_Iatl_addin
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
//

//------------------------------------------------------------------//
HRESULT Catl_addin::FinalConstruct()
{
	OutputDebugString(_T("Catl_addin::FinalConstruct\n"));
	HRESULT hr;

	m_identity = _T("");

	// get dll path
	GetModuleFileName(_AtlBaseModule.m_hInst, m_path, sizeof(m_path)/sizeof(TCHAR));
	PathRemoveFileSpec(m_path);

	// create oeapi init notification object
	hr = m_init.CoCreateInstance(CLSID_OEAPIInit); //, NULL, CLSCTX_INPROC_SERVER);
	if(SUCCEEDED(hr))
	{
		_OEAPIInitEvents::DispEventAdvise((IUnknown*)m_init, &__uuidof(OEAPIINITCOM::IOEInitEvents));
	}
	return S_OK;
}

//------------------------------------------------------------------//
void Catl_addin::FinalRelease()
{
	OutputDebugString(_T("Catl_addin::FinalRelease\n"));
	// _OEAPIInitEvents::DispEventUnadvise((IUnknown*)m_init);
	m_init = NULL;
}


//-----------------------------------------------------------//
STDMETHODIMP Catl_addin::OnInitOEAPI()
{
	OutputDebugString(_T("Catl_addin::OnInitOEAPI\n"));
	// oeapi is up now you can create all oeapi objects

	//* // Inproc servers are unloaded on identity switchs
	CComPtr<IOEIdentity> identity;
	if(SUCCEEDED(identity.CoCreateInstance(CLSID_OEIdentity)))
	{
		m_identity = (LPCTSTR)identity->GetCurrentIdentity(); 
	}
	// */

	//* 
	if(SUCCEEDED(m_oeapi.CoCreateInstance(CLSID_OEAPIObj)))
	{
		// sink events
		_OEAPIObjEvents::DispEventAdvise((IUnknown*)m_oeapi, &__uuidof(OEAPI::IOEAPIObjEvents)); 


		// images path
		std::basic_string<TCHAR> normal = m_path;
		normal += _T("\\..\\images\\img_normal.bmp");
		std::basic_string<TCHAR> over = m_path;
		over += _T("\\..\\images\\img_mouseover.bmp");

		CComPtr<IOEMenu> menu;
		//menu = m_oeapi->GetMsgWndMenu(OE_ALL_MSG_WND, 2);
		menu = m_oeapi->GetMenu(2);

		CComPtr<IOEMenuItem> item;
		menu->CreateSeparator();
		//item.CoCreateInstance(CLSID_OEMenuItem);
		item = menu->CreateItem(_T("item 1"));
		item->SetCheck(TRUE);
		item->SetChecked(TRUE);
		//item->Create(menu->GetID(), _T("item 1"));
		menu->CreateItem(_T("item 2"));
		item = menu->CreateItem(_T("item 3"));
		menu->CreateSeparator();

		CComPtr<IOEMenuItem> subitem;
		subitem = item->CreateSubItem(_T("subitem 1"));
		subitem = item->CreateSubItem(_T("subitem 2"));
		subitem->SetCheck(TRUE);
		subitem->SetChecked(TRUE);
		item->CreateSeparator();
		subitem = item->CreateSubItem(_T("subitem 3"));
		subitem = item->CreateSubItem(_T("subitem 4"));
		item = subitem->CreateSubItem(_T("susubbitem 1"));
		int mnuId = item->GetID();
		subitem = subitem->CreateSubItem(_T("susubbitem 2"));
		subitem->SetCheck(TRUE);
		subitem->SetChecked(TRUE);
		subitem->CreateSubItem(_T("susubsubbitem 1"));
		subitem = subitem->CreateSubItem(_T("susubsubbitem 2"));
		subitem->SetCheck(TRUE);
		subitem->SetChecked(TRUE);
		menu->CreateItem(_T("item 4"));
		item = m_oeapi->GetMenuItem(mnuId);
		if(item) 
		{
			item->Destroy();
		}
		//subitem->SetEnabled(TRUE);

		// create a toolbar
		CComPtr<IOEToolbar> toolbar;
		//toolbar = m_oeapi->GetOEToolbarInMsgWnd(OE_ALL_MSG_WND); // CreateToolbarInMsgWnd(OE_ALL_MSG_WND);
		toolbar = m_oeapi->CreateToolbar(); // 
		//toolbar = m_oeapi->CreateToolbarInMsgWnd(OE_ALL_MSG_WND);
		//toolbar = m_oeapi->GetOEToolbar(); // CreateToolbarInMsgWnd(OE_ALL_MSG_WND);
		//toolbar->SetLargeButtons(FALSE);
		if(toolbar != NULL)
		{
			//toolbar->SetLargeButtons(TRUE);
			//toolbar->EnableTooltips(TRUE);
			m_toolbarId = toolbar->GetID();

			CComPtr<IOEButton> button;
			//IOEButton* button = NULL;
			//toolbar->raw_CreateSeparator(&button);
			button = toolbar->CreateSeparator();
			button->SetShown(FALSE);
			//button->Release();
			//toolbar->raw_CreateButton(CComBSTR("Test"), CComBSTR(""), CComBSTR(""), &button);
			CComBSTR name = _T("test");
			CComBSTR empty = _T("");
			button = toolbar->CreateButton((BSTR)name, (BSTR)empty, (BSTR)empty);
			m_button = button->GetID();
			//button->CreateSubButton(_T("Subbutton1"), _T(""), _T(""));
			//button->CreateSubButton(_T("Subbutton2"), _T(""), _T(""));

			//CComPtr<IOETextBox> edit;
			//edit.CoCreateInstance(CLSID_OETextBox);
			//edit->Create(toolbar->GetID(), L"tag");
			//toolbar->CreateSeparator();
			//button->Release();

			// create a botton
			/* CComPtr<IOEButton> button;
			//button = toolbar->CreateButton(_T("OEAPI Help"), _T(""), _T(""));
			button = toolbar->CreateButton(_T("OEAPI Help"), normal.c_str(), over.c_str());
			//toolbar->SetMaskColor(0xff000000);
			button->SetTooltip("Mongo");
			if(button != NULL)
				m_showHelp = button->GetID();

			CComPtr<IOEButton> subbutton;

			IPictureDisp* pict = NULL;
			PICTDESC picDesc;
			picDesc.cbSizeofstruct = sizeof(PICTDESC);
			picDesc.picType = PICTYPE_BITMAP;
			picDesc.bmp.hbitmap = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			HRESULT hr = ::OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, TRUE, (LPVOID*)&pict);
			
			button = toolbar->CreateButton(_T("Button"), _T(""), _T(""));
			m_button = button->GetID();
			button->SetPicture(pict);
			subbutton = button->CreateSubButton(_T("Subbutton1"), _T(""), _T(""));
			subbutton->SetPicture(pict);
			subbutton = button->CreateSubButton(_T("Subbutton2"), _T(""), _T(""));
			subbutton->SetPicture(pict);
			subbutton = button->CreateSubButton(_T("Subbutton3"), _T(""), _T(""));
			subbutton->SetPicture(pict);

			pict->Release();

			button = toolbar->CreateButton(_T("Popup Button"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subbutton1"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subbutton2"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subutton3"), normal.c_str(), over.c_str());
			button->SetPopupStyle(TRUE); //*/
		} // */
		//toolbar = m_oeapi->CreateToolbar(); // CreateToolbarInMsgWnd(OE_ALL_MSG_WND);
		////toolbar = m_oeapi->CreateToolbarInMsgWnd(OE_ALL_MSG_WND);
		//toolbar->SetLargeButtons(FALSE);
		//toolbar->EnableTooltips(TRUE);
		//CComPtr<IOEButton> button;
		//button = toolbar->CreateSeparator();
		//button->SetShown(FALSE);
		//CComBSTR name = _T("test");
		//CComBSTR empty = _T("");
		//button = toolbar->CreateButton((BSTR)name, (BSTR)empty, (BSTR)empty);
		//m_button = button->GetID();
		//button->CreateSubButton(_T("Subbutton1"), _T(""), _T(""));
		//button->CreateSubButton(_T("Subbutton2"), _T(""), _T(""));
			//CComPtr<IOETextBox> edit; 
			//edit.CoCreateInstance(CLSID_OETextBox);
			//edit->Create(toolbar->GetID(), L"tag");
			//toolbar->CreateSeparator();
			//edit = NULL;
			//edit.CoCreateInstance(CLSID_OETextBox);
			//edit->Create(toolbar->GetID(), L"tag");
			//toolbar->CreateSeparator();
			//edit = NULL;
			//edit.CoCreateInstance(CLSID_OETextBox);
			//edit->Create(toolbar->GetID(), L"tag");
			//toolbar->CreateSeparator();
			//edit = NULL;
			/* edit.CoCreateInstance(CLSID_OEEdit);
			edit->Create(toolbar->GetID(), L"tag");
			toolbar->CreateSeparator();
			edit = NULL;
			edit.CoCreateInstance(CLSID_OEEdit);
			edit->Create(toolbar->GetID(), L"tag");
			toolbar->CreateSeparator();
			edit = NULL;
			edit.CoCreateInstance(CLSID_OEEdit);
			edit->Create(toolbar->GetID(), L"tag");
			toolbar->CreateSeparator();
			edit = NULL;
			edit.CoCreateInstance(CLSID_OEEdit);
			edit->Create(toolbar->GetID(), L"tag");
			toolbar->CreateSeparator(); */
		//toolbar->SetLargeButtons(TRUE);
		//toolbar->SetLargeButtons(FALSE);	
	}

	// 
	//CComPtr<IOEFolderManager> fm;
	if(SUCCEEDED(m_fm.CoCreateInstance(CLSID_OEFolderManager)))
	{
		/* m_inbox = m_fm->GetInboxFolder();
		if(m_inbox != NULL)
		{
			// start listening folder events for Inbox 
			_OEFolderEvents::DispEventAdvise((IUnknown*)m_inbox);
		} */
	}

	return S_OK;
}

//-----------------------------------------------------------//
STDMETHODIMP Catl_addin::OnShutdownOEAPI()
{
	OutputDebugString(_T("Catl_addin::OnShutdownOEAPI\n"));

	/* if(m_inbox != NULL)
	{
		_OEFolderEvents::DispEventUnadvise((IUnknown*)m_inbox);
		m_inbox = NULL;
	} */

	// stop listening oeapi events
	if(m_oeapi != NULL)
	{
		_OEAPIObjEvents::DispEventUnadvise((IUnknown*)m_oeapi, &__uuidof(OEAPI::IOEAPIObjEvents));
		m_oeapi = NULL;
	}

	// stop listening init events
	_OEAPIInitEvents::DispEventUnadvise((IUnknown*)m_init, &__uuidof(OEAPIINITCOM::IOEInitEvents));

	return S_OK;
}
//com_ptr<IPictureDisp> CAddinMgr::LoadPicture(INT bmpId)
//{
//	IPictureDisp* pict = NULL;
//	PICTDESC picDesc;
//	picDesc.cbSizeofstruct = sizeof(PICTDESC);
//	picDesc.picType = PICTYPE_BITMAP;
//	picDesc.bmp.hbitmap = ::LoadBitmap(_Module.GetModuleInstance(), MAKEINTRESOURCE(bmpId));
//	HRESULT hr = ::OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, TRUE, (LPVOID*)&pict);
//	if(FAILED(hr))
//	{
//		debug_print(DEBUG_ERROR, _T("CAddinMgr::LoadPicture: Can't load picture %08x\n"), hr);
//	}
//	return auto_attach(pict);
// }

TCHAR text[] = _T(
	"From: ismael.bejarano@nektra.com\n"
	"To: ismael.bejarano@nektra.com\n"
	"Date: Sun, 4 Jun 2006 06:50:31 -0300\n"
	"Subject: test %s\n"
	"Content-Type: text/plain; charset=ISO-8859-1; format=flowed\n"
	"Content-Transfer-Encoding: 7bit\n"
	"\nLorem ipsum dolor sit amet, consectetur adipisicing elit, "
	"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
	"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
	"nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
	"reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
	"pariatur. Excepteur sint occaecat cupidatat non proident, sunt in "
	"culpa qui officia deserunt mollit anim id est laborum.\n"
	"Tempuri -- \nhttp://www.nektra.com\n\n");


//------------------------------------------------------------------//
STDMETHODIMP Catl_addin::OnToolbarButtonClicked(long toolbarId, long buttonId)
{
	if(m_toolbarId == toolbarId && buttonId == m_button)
	{
		DWORD folderId = m_oeapi->GetSelectedFolderID();
		CComPtr<IOEFolder> folder = m_fm->GetFolder(folderId);
		DWORD msgId = m_oeapi->GetCurrentMessageID();
		folder->DeleteMessage(msgId, TRUE);

		//CComPtr<IOEFolder> inbox = m_fm->GetInboxFolder();
		//std::basic_string<TCHAR> s = text;
		//CComPtr<IOEMessage> msg = inbox->CreateMessage(s.c_str(), FALSE);
		//long msgId = msg->GetID();
		//inbox->DeleteMessage(msgId, TRUE);

		//m_oeapi->SetSelectedMessageID(-1);

		//CComPtr<IOEFolder> outbox = m_fm->GetOutboxFolder();
		//CComPtr<IOEMessage> msg = outbox->GetFirstMessage();
		//CComPtr<IOEMessage> msg2 = msg->Clone(outbox->GetID());
		//msg2->Commit();

		//CComPtr<IOEFolder> deleted = m_fm->GetDeletedFolder();
		//CComPtr<IOEFolder> inbox = m_fm->GetInboxFolder();
		//CComPtr<IOEMessage> msg = inbox->GetFirstMessage();
		//msg->Delete(FALSE);
		//msg = deleted->GetFirstMessage();
		//msg->Delete(TRUE);


		//::MessageBox((HWND)m_oeapi->GetOEMainWindow(), _T("text"), _T("caption"), MB_OK);
		//CComPtr<IOEFolder> drafts = m_fm->GetDraftFolder();
		//LONG id = drafts->GetID();
		//m_oeapi->SetSelectedFolderID(id);

		//CComPtr<IOEFolder> folder = m_fm->GetInboxFolder();
		//CComPtr<IOEMessage> msg = folder->GetFirstMessage();
//		LONG root = msg->GetBodyHandle(0, OE_IBL_ROOT);
		//LONG htmlHandle = msg->GetHTMLBody();
		//msg->SetBodyText(htmlHandle, _T("<html><body></body></html>"), _T(""), _T(""));
		//msg->Commit();
		// msg->SetBodyProp
		//while(msg) {
			//OutputDebugString("Catl_addin::OnToolbarButtonClicked: ");
			//OutputDebugString((LPCTSTR)msg->GetSubject());
			//OutputDebugString("\n");
		//	msg = drafts->GetNextMessage();
		//}

		/* TCHAR buffer[256];
		_stprintf(buffer, _T("%d"), GetTickCount());
		std::basic_string<TCHAR> s = text;
		s.replace(s.find(_T("%s")), 2, buffer);
		CComPtr<IOEFolder> drafts = m_fm->GetDraftFolder();
		CComPtr<IOEMessage> msg = drafts->CreateMessage(s.c_str(), FALSE);
		m_oeapi->OESendMessage(drafts->GetID(), msg->GetID()); */

		//m_oeapi->ge
		//////CComPtr<IOEToolbar> toolbar = m_oeapi->GetToolbar(toolbarId);
		//////CComPtr<IOEButton> button = toolbar->GetButton(buttonId);
		/* IPictureDisp* pict = NULL;
		PICTDESC picDesc;
		picDesc.cbSizeofstruct = sizeof(PICTDESC);
		picDesc.picType = PICTYPE_BITMAP;
		picDesc.bmp.hbitmap = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		HRESULT hr = ::OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, TRUE, (LPVOID*)&pict);
		button->SetPicture(pict); */
		//button->SetPicture(NULL);

		////button->SetName("large title for a button"); // SetTooltip("Pyon");

		/*std::basic_string<TCHAR> helpFile = m_path;
		helpFile += _T("\\..\\..\\..\\doc\\OEAPI-Help.chm");

		::HtmlHelp(NULL, helpFile.c_str(), HH_DISPLAY_TOC, NULL);*/
	}
	return S_OK;
}


//------------------------------------------------------------------//
STDMETHODIMP Catl_addin::OnNewMessage(long msgId)
{
	// NOTE:
	// You should keep this event simple, to support a large number of events
	// if you should do a more complex task then you better
	// keep a list of msgId and re-post this event to other thread 
	// so it can be processed later
	CComPtr<IOEToolbar> toolbar = m_oeapi->GetToolbar(m_toolbarId);
	if(toolbar != NULL)
	{
		CComPtr<IOEButton> button = toolbar->GetButton(m_button);
		if(button != NULL)
		{
			std::basic_stringstream<TCHAR> buffer;
			buffer << _T("Button (") << msgId << _T(")") << std::ends;
			button->SetName(buffer.str().c_str());
		}
	}
	return S_OK;
}


//------------------------------------------------------------------//
STDMETHODIMP Catl_addin::OnDatabaseChange(long tr, long toolbarId, long buttonId, long other)
{
	std::basic_stringstream<TCHAR> buffer;
	buffer << _T("++database change (") << tr << _T(", ") << toolbarId << _T(", ") << buttonId << _T("\n") << std::ends;
	OutputDebugString(buffer.str().c_str());
	return S_OK;
}
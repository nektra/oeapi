/* $Id: atl_addin.cpp,v 1.6 2007/05/07 20:02:54 ibejarano Exp $
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


_ATL_FUNC_INFO NoParamInfo = { CC_STDCALL, VT_EMPTY, 0 };
_ATL_FUNC_INFO OneLongParamInfo = { CC_STDCALL, VT_EMPTY, 1, { VT_INT } };
_ATL_FUNC_INFO TwoLongParamInfo = { CC_STDCALL, VT_EMPTY, 2, { VT_INT, VT_INT } };

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

		// create a toolbar
		CComPtr<IOEToolbar> toolbar;
		toolbar = m_oeapi->CreateToolbar();
		if(toolbar != NULL)
		{
			toolbar->SetLargeButtons(FALSE);
			m_toolbarId = toolbar->GetID();

			// create a botton
			CComPtr<IOEButton> button;
			button = toolbar->CreateButton(_T("OEAPI Help"), _T(""), _T(""));
			if(button != NULL)
				m_showHelp = button->GetID();

			button = toolbar->CreateButton(_T("Button"), _T(""), _T(""));
			m_button = button->GetID();
			button->CreateSubButton(_T("Subbutton1"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subbutton2"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subbutton3"), normal.c_str(), over.c_str());

			button = toolbar->CreateButton(_T("Popup Button"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subbutton1"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subbutton2"), normal.c_str(), over.c_str());
			button->CreateSubButton(_T("Subbutton3"), normal.c_str(), over.c_str());
			button->SetPopupStyle(TRUE);
		}
	}

	//

	if(SUCCEEDED(m_foldermanager.CoCreateInstance(CLSID_OEFolderManager)))
	{
		m_inbox = m_foldermanager->GetInboxFolder();
		if(m_inbox != NULL)
		{
			// start listening folder events for Inbox
			_OEFolderEvents::DispEventAdvise((IUnknown*)m_inbox);
		}
	}

	return S_OK;
}

//-----------------------------------------------------------//
STDMETHODIMP Catl_addin::OnShutdownOEAPI()
{
	OutputDebugString(_T("Catl_addin::OnShutdownOEAPI\n"));

	if(m_inbox != NULL)
	{
		_OEFolderEvents::DispEventUnadvise((IUnknown*)m_inbox);
		m_inbox = NULL;
	}

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

//------------------------------------------------------------------//



long FindTextBody(IOEMessagePtr msg, long hBody)
{
    if (!msg->IsBodyAttachment(hBody) && 
        msg->IsBodyContentType(hBody,L"text","plain"))
    {
        return hBody;
    }

    long ret = NULL;
    
    _bstr_t bContentType = msg->GetBodyPrimaryContentType(hBody);

    if (bContentType == _bstr_t("multipart"))
    {
        long hChild = msg->GetBodyHandle(hBody,OE_IBL_FIRST);
        while (hChild!=NULL && ret == NULL)
        {
            ret = FindTextBody(msg, hChild);
            hChild = msg->GetBodyHandle(hChild,OE_IBL_NEXT);
        }
    }
    return ret;
}


STDMETHODIMP Catl_addin::OnToolbarButtonClicked(long toolbarId, long buttonId)
{
	if(m_toolbarId == toolbarId && buttonId == m_showHelp)
	{
		/*std::basic_string<TCHAR> helpFile = m_path;
		helpFile += _T("\\..\\..\\..\\doc\\OEAPI-Help.chm");

		::HtmlHelp(NULL, helpFile.c_str(), HH_DISPLAY_TOC, NULL);*/


        long msgid = m_oeapi->GetCurrentMessageID();
        long folderid = m_oeapi->GetSelectedFolderID();
        //m_oeapi->OESendMessage(folderid,msgid);

        IOEFolderPtr folder = m_foldermanager->GetFolder(folderid);
        IOEMessagePtr msg = folder->OEGetMessage(msgid);

   
        long contentHandle = FindTextBody(msg, msg->GetBodyHandle(NULL,OE_IBL_ROOT));
        _bstr_t bstrTemp = msg->GetBodyText(contentHandle);
        
        if (bstrTemp.GetBSTR() != NULL)
        {
          
            /*wstring sBody = (wchar_t *)bstrTemp;
            sBody = TrimString(sBody);

            if(sBody.length() > 0)
            {
                item.bContent = true;
            }
            else
            {
                item.bContent = false;
            }*/
        }
        else
        {
            //item.bContent = false;
        }

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


#include "stdafx.h"
#include "CSymDemo.h"
#include <sstream>
#include <libsymnektra.h>

#include "CFolderListDlg.h"
#include <string>

using namespace std;


CSymDemo::~CSymDemo()
{
    OutputDebugString(_T("Catl_addin::FinalRelease\n"));
    // _OEAPIInitEvents::DispEventUnadvise((IUnknown*)m_init);
    m_init = NULL;
}

//------------------------------------------------------------------//
HRESULT CSymDemo::InitializePlugin()
{
    OutputDebugString(_T("Catl_addin::FinalConstruct\n"));
    HRESULT hr;

    m_identity = _T("");

    // get dll path
    GetModuleFileName(_AtlBaseModule.m_hInst, m_path, sizeof(m_path)/sizeof(TCHAR));
    PathRemoveFileSpec(m_path);

    // create oeapi init notification object
    
    hr = SNGetInit((void**)&m_init);
    
    if(SUCCEEDED(hr))
    {
        _OEAPIInitEvents::DispEventAdvise((IUnknown*)m_init, &__uuidof(OEAPIINITCOM::IOEInitEvents));
    }
    return S_OK;
}

//------------------------------------------------------------------//

STDMETHODIMP CSymDemo::OnInitOEAPI()
{
	OutputDebugString(_T("Catl_addin::OnInitOEAPI\n"));
	// oeapi is up now you can create all oeapi objects

	//* // Inproc servers are unloaded on identity switchs
	CComPtr<IOEIdentity> identity;

	if(SUCCEEDED(SNGetIdentity((void**)&identity)))
	{
		m_identity = (LPCTSTR)identity->GetCurrentIdentity();
	}
	// */

	//*
	if(SUCCEEDED(SNGetApiCore((void**)&m_oeapi)))
	{
		// sink events
		_OEAPIObjEvents::DispEventAdvise((IUnknown*)m_oeapi, &__uuidof(OEAPI::IOEAPIObjEvents));


		// images path
		std::basic_string<TCHAR> normal = m_path;
		normal += _T("\\images\\img_normal.bmp");
		std::basic_string<TCHAR> over = m_path;
		over += _T("\\images\\img_mouseover.bmp");

		// create a toolbar
		CComPtr<IOEToolbar> toolbar;
		toolbar = m_oeapi->CreateToolbar();
		if(toolbar != NULL)
		{
			toolbar->SetLargeButtons(FALSE);
			m_toolbarId = toolbar->GetID();

			CComPtr<IOEButton> button;
            button = toolbar->CreateButton(_T("Send Message"), _T(""), _T(""));
			if (button != NULL) m_SendButton = button->GetID();

            button = toolbar->CreateButton(_T("Show Folders"), _T(""), _T(""));
            if (button != NULL) m_ShowFoldersButton = button->GetID();

			button = toolbar->CreateButton(_T("Button"), normal.c_str(), over.c_str());
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

        toolbar = m_oeapi->CreateToolbarInMsgWnd(OE_ALL_MSG_WND);
        if(toolbar != NULL)
        {
            toolbar->SetLargeButtons(FALSE);
            m_msgwndtoolbarId = toolbar->GetID();

            // create a botton
            CComPtr<IOEButton> button;
            button = toolbar->CreateButton(_T("Message window button"), _T(""), _T(""));
        }
	}

	//

	if(SUCCEEDED(SNGetFolderManager((void**)&m_foldermanager)))
	{
		m_inbox = m_foldermanager->GetInboxFolder();
		if(m_inbox != NULL)
		{
			// start listening folder events for Inbox
			_OEFolderEvents::DispEventAdvise((IUnknown*)m_inbox);
		}
	}

    m_folderDlg = new CFolderListDlg(m_oeapi,m_foldermanager);

	return S_OK;
}

//-----------------------------------------------------------//
STDMETHODIMP CSymDemo::OnShutdownOEAPI()
{
	OutputDebugString(_T("Catl_addin::OnShutdownOEAPI\n"));
    
    delete m_folderDlg;

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

STDMETHODIMP CSymDemo::OnMessageDownloadStarted()
{
    MessageBox(0,L"OnMessageDownloadStarted",L"",0);
    return S_OK;
}

STDMETHODIMP CSymDemo::OnMessageDownloadFinished()
{
    MessageBox(0,L"OnMessageDownloadFinished",L"",0);
    return S_OK;
}

STDMETHODIMP CSymDemo::OnNewMessageInOutbox(long msgId)
{
    IOEFolderPtr outbox = m_foldermanager->GetOutboxFolder();
    IOEMessagePtr msg = outbox->OEGetMessage(msgId);
    _bstr_t s = msg->GetDisplayTo();
    s = L"OnNewMessageInOutbox: To: " + s;
    MessageBox(0,s,L"",0);
    return S_OK;   
}

//------------------------------------------------------------------//
STDMETHODIMP CSymDemo::OnToolbarButtonClicked(long toolbarId, long buttonId)
{
	if(m_toolbarId == toolbarId )
    {
        if (buttonId == m_SendButton)
        {
            long msgid = m_oeapi->GetCurrentMessageID();
            long folderid = m_oeapi->GetSelectedFolderID();
            m_oeapi->OESendMessage(folderid,msgid);
            
           /* IOEFolderPtr folder = m_foldermanager->GetFolder(folderid);
            IOEMessagePtr msg = folder->OEGetMessage(msgid);

            long bodyHandle = msg->GetPlainBody();
            _bstr_t bstrContent = msg->GetBodyText(bodyHandle); // here
            if(bstrContent.length() != 0) // here
            {
                MessageBox(0,L"not null",L"",0);
            }
            else
            {
                MessageBox(0,L"null",L"",0);
            }*/
        } 
        else if (buttonId == m_ShowFoldersButton)
        {
    
            HWND mainWnd = (HWND) m_oeapi->GetOEMainWindow();
            if(NULL != m_folderDlg->Create(mainWnd))
            {
                m_folderDlg->ShowWindow(SW_SHOWNORMAL);
            }
    
        }
	}
    
	return S_OK;
}


//------------------------------------------------------------------//
STDMETHODIMP CSymDemo::OnNewMessage(long msgId)
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

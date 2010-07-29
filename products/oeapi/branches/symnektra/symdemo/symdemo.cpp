// oeapi_sample.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include <sstream>
#include <libsymnektra.h>

_ATL_FUNC_INFO NoParamInfo = { CC_STDCALL, VT_EMPTY, 0 };
_ATL_FUNC_INFO OneLongParamInfo = { CC_STDCALL, VT_EMPTY, 1, { VT_INT } };
_ATL_FUNC_INFO TwoLongParamInfo = { CC_STDCALL, VT_EMPTY, 2, { VT_INT, VT_INT } };

class CSymDemo : 
    public IDispEventSimpleImpl<1, CSymDemo, &__uuidof(OEAPIINITCOM::IOEInitEvents)>,
    public IDispEventSimpleImpl<2, CSymDemo, &__uuidof(OEAPI::IOEAPIObjEvents)>,
    public IDispEventSimpleImpl<3, CSymDemo, &__uuidof(OESTORE::IOEFolderEvents)>
{
    public :

        virtual ~CSymDemo();

        typedef IDispEventSimpleImpl<1, CSymDemo, &__uuidof(OEAPIINITCOM::IOEInitEvents)> _OEAPIInitEvents;
        typedef IDispEventSimpleImpl<2, CSymDemo, &__uuidof(OEAPI::IOEAPIObjEvents)> _OEAPIObjEvents;
        typedef IDispEventSimpleImpl<3, CSymDemo, &__uuidof(OESTORE::IOEFolderEvents)> _OEFolderEvents;

        virtual HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo(REFIID riid){return E_FAIL;};

        BEGIN_SINK_MAP(CSymDemo)
            SINK_ENTRY_INFO(1, __uuidof(OEAPIINITCOM::IOEInitEvents), 0x0001, OnInitOEAPI, &NoParamInfo)
            SINK_ENTRY_INFO(1, __uuidof(OEAPIINITCOM::IOEInitEvents), 0x0002, OnShutdownOEAPI, &NoParamInfo)
            SINK_ENTRY_INFO(2, __uuidof(OEAPI::IOEAPIObjEvents), 0x0001, OnToolbarButtonClicked, &TwoLongParamInfo)
            SINK_ENTRY_INFO(3, __uuidof(OESTORE::IOEFolderEvents), 0x0001, OnNewMessage, &OneLongParamInfo)
        END_SINK_MAP()

        STDMETHOD(OnInitOEAPI)();
        STDMETHOD(OnShutdownOEAPI)();
        STDMETHOD(OnToolbarButtonClicked)(long toolbarId, long buttonId);
        STDMETHOD(OnNewMessage)(long msgId);

        HRESULT InitializePlugin();

    private:
        CComPtr<IOEAPIInit> m_init;
        CComPtr<IOEAPIObj> m_oeapi;
        CComPtr<IOEFolder> m_inbox;

        std::basic_string<TCHAR> m_identity;

        TCHAR m_path[1024];

        long m_toolbarId;
        long m_showHelp;
        long m_button;

};

CSymDemo _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}


extern "C" BOOL WINAPI InitializePlugin(void)
{
    _AtlModule.InitializePlugin();
    return TRUE;
}


//////////////////////////////////////////////////////////////////////
//


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
	CComPtr<IOEFolderManager> fm;
	if(SUCCEEDED(SNGetFolderManager((void**)&fm)))
	{
		m_inbox = fm->GetInboxFolder();
		if(m_inbox != NULL)
		{
			// start listening folder events for Inbox
			_OEFolderEvents::DispEventAdvise((IUnknown*)m_inbox);
		}
	}

	return S_OK;
}

//-----------------------------------------------------------//
STDMETHODIMP CSymDemo::OnShutdownOEAPI()
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
STDMETHODIMP CSymDemo::OnToolbarButtonClicked(long toolbarId, long buttonId)
{
	if(m_toolbarId == toolbarId && buttonId == m_showHelp)
	{
	
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

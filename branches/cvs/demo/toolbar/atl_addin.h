/* $Id: atl_addin.h,v 1.6 2007/05/07 20:02:54 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2006-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

// atl_addin.h : Declaration of the Catl_addin

#pragma once
#include "resource.h"       // main symbols

#include <string>

#include "oeapi_sample.h"

extern _ATL_FUNC_INFO NoParamInfo;
extern _ATL_FUNC_INFO OneLongParamInfo;
extern _ATL_FUNC_INFO TwoLongParamInfo;

// Catl_addin

class ATL_NO_VTABLE Catl_addin :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Catl_addin, &CLSID_atl_addin>,
	public ISupportErrorInfo,
	public IDispatchImpl<Iatl_addin, &IID_Iatl_addin, &LIBID_oeapi_sampleLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventSimpleImpl<1, Catl_addin, &__uuidof(OEAPIINITCOM::IOEInitEvents)>,
	public IDispEventSimpleImpl<2, Catl_addin, &__uuidof(OEAPI::IOEAPIObjEvents)>,
	public IDispEventSimpleImpl<3, Catl_addin, &__uuidof(OESTORE::IOEFolderEvents)>
{
public:
	typedef IDispEventSimpleImpl<1, Catl_addin, &__uuidof(OEAPIINITCOM::IOEInitEvents)> _OEAPIInitEvents;
	typedef IDispEventSimpleImpl<2, Catl_addin, &__uuidof(OEAPI::IOEAPIObjEvents)> _OEAPIObjEvents;
	typedef IDispEventSimpleImpl<3, Catl_addin, &__uuidof(OESTORE::IOEFolderEvents)> _OEFolderEvents;

	Catl_addin()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATL_ADDIN)


BEGIN_COM_MAP(Catl_addin)
	COM_INTERFACE_ENTRY(Iatl_addin)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_SINK_MAP(Catl_addin)
		SINK_ENTRY_INFO(1, __uuidof(OEAPIINITCOM::IOEInitEvents), 0x0001, OnInitOEAPI, &NoParamInfo)
		SINK_ENTRY_INFO(1, __uuidof(OEAPIINITCOM::IOEInitEvents), 0x0002, OnShutdownOEAPI, &NoParamInfo)
		SINK_ENTRY_INFO(2, __uuidof(OEAPI::IOEAPIObjEvents), 0x0001, OnToolbarButtonClicked, &TwoLongParamInfo)
		SINK_ENTRY_INFO(3, __uuidof(OESTORE::IOEFolderEvents), 0x0001, OnNewMessage, &OneLongParamInfo)
	END_SINK_MAP()

	HRESULT FinalConstruct();

	void FinalRelease() ;

	//------------------------------------------------------------------//
	// Init event, called when OEAPI is up
	STDMETHOD(OnInitOEAPI)();

	// Shutdown event, called when OEAPI is terminating
	STDMETHOD(OnShutdownOEAPI)();

	//------------------------------------------------------------------//
	STDMETHOD(OnToolbarButtonClicked)(long toolbarId, long buttonId);

	//------------------------------------------------------------------//
	STDMETHOD(OnNewMessage)(long msgId);

public:

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

OBJECT_ENTRY_AUTO(__uuidof(atl_addin), Catl_addin)

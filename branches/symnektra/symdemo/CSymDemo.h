#pragma once

#include "stdafx.h"
#include <string>
#include "CFolderListDlg.h"

static _ATL_FUNC_INFO NoParamInfo = { CC_STDCALL, VT_EMPTY, 0 };
static _ATL_FUNC_INFO OneLongParamInfo = { CC_STDCALL, VT_EMPTY, 1, { VT_INT } };
static _ATL_FUNC_INFO TwoLongParamInfo = { CC_STDCALL, VT_EMPTY, 2, { VT_INT, VT_INT } };

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
        SINK_ENTRY_INFO(2, __uuidof(OEAPI::IOEAPIObjEvents), 0x0006, OnMessageDownloadStarted, &NoParamInfo)
        SINK_ENTRY_INFO(2, __uuidof(OEAPI::IOEAPIObjEvents), 0x0007, OnMessageDownloadFinished, &NoParamInfo)
        SINK_ENTRY_INFO(2, __uuidof(OEAPI::IOEAPIObjEvents), 0x0008, OnNewMessageInOutbox, &OneLongParamInfo)
        SINK_ENTRY_INFO(3, __uuidof(OESTORE::IOEFolderEvents), 0x0001, OnNewMessage, &OneLongParamInfo)            
    END_SINK_MAP()

    STDMETHOD(OnInitOEAPI)();
    STDMETHOD(OnShutdownOEAPI)();
    STDMETHOD(OnMessageDownloadStarted)();
    STDMETHOD(OnMessageDownloadFinished)();
    STDMETHOD(OnNewMessageInOutbox)(long msgId);

    STDMETHOD(OnToolbarButtonClicked)(long toolbarId, long buttonId);
    STDMETHOD(OnNewMessage)(long msgId);

    HRESULT InitializePlugin();

private:
    CComPtr<IOEAPIInit> m_init;
    CComPtr<IOEAPIObj> m_oeapi;
    CComPtr<IOEFolder> m_inbox;
    CComPtr<IOEFolderManager> m_foldermanager;
    CFolderListDlg* m_folderDlg;

    std::basic_string<TCHAR> m_identity;

    TCHAR m_path[MAX_PATH];

    long m_toolbarId;
    long m_msgwndtoolbarId;
    long m_SendButton;
    long m_ShowFoldersButton;
    long m_button;

};

#include "OeApiInit.h"
#include <Windows.h>
#include <oeapi_config.h>
#include <se_debug.h>
#include "globals.h"
#include "oeapi_utils.h"
#include "oeapi_event.h"
#include "NktMutex.h"
#include <sstream>


const UINT OEAPI_INIT_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnInit"));
const UINT OEAPI_SHUTDOWN_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnShutdown"));

/**
This function is necessary because the serverProc runs in another thread and the events
must be triggered in the same thread that the COM object was created or the thread
of the constructor.
*/

DWORD _stdcall OEAPIInitServerProc(void *obj)
{
    TOEAPIInit *oeapi = (TOEAPIInit *) obj;
    HANDLE initEventHandle;
    HANDLE shutdownEventHandle;

    while(true) {
        initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_INIT_EVENT_NAME);
        if(initEventHandle == NULL)
        {
            debug_print(DEBUG_ERROR, _T("ServerProc: Error CreateEvent OEAPI_INIT_EVENT_NAME\n"));
            return 1;
        }

        //debug_print(DEBUG_ERROR, _T("ServerProc: WaitForSingleObject OEAPI_INIT_EVENT_NAME\n"));
        WaitForSingleObject(initEventHandle, INFINITE);

        oeapi->TriggerOnInitOEAPI();

        // notify oecom.dll that we are triggering the event
        oeapi->NotifyInitComplete();

        shutdownEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_SHUTDOWN_EVENT_NAME);
        if(shutdownEventHandle == NULL) {
            debug_print(DEBUG_ERROR, _T("ServerProc: Error CreateEvent OEAPI_SHUTDOWN_EVENT_NAME\n"));
            return 1;
        }

        WaitForSingleObject(shutdownEventHandle, INFINITE);
        oeapi->TriggerOnShutdownOEAPI();
        // break;
    }

    // never arrives here
    CloseHandle(initEventHandle);
    CloseHandle(shutdownEventHandle);
    return 0;
}

LRESULT CALLBACK OEAPIInitWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    TOEAPIInit *pThis = (TOEAPIInit *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if(uMsg == OEAPI_INIT_MESSAGE_CODE) {
        pThis->FireOnInitOEAPI();
        ::SetEvent((HANDLE)lParam);
    }
    else if(uMsg == OEAPI_SHUTDOWN_MESSAGE_CODE) {
        pThis->FireOnShutdownOEAPI();
        ::SetEvent((HANDLE)lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND CreateCallbackWindow()
{
    WNDCLASSEX wndclass;
    const TCHAR szOEAPIInitSink[] = OEAPI_INITCOM_CALLBACK_CLASS;
    HWND hWnd;

    wndclass.cbSize			= sizeof(wndclass);
    wndclass.style			= 0;
    wndclass.lpfnWndProc	= &OEAPIInitWndProc;
    wndclass.cbClsExtra		= 0;
    wndclass.cbWndExtra		= 0;
    wndclass.hInstance		= g_hOEAPIINITCOM;
    wndclass.hIcon			= NULL;
    wndclass.hCursor		= NULL;
    wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName	= NULL;
    wndclass.lpszClassName	= szOEAPIInitSink;
    wndclass.hIconSm		= NULL;

    // Register it
    RegisterClassEx(&wndclass);

    // And create a window
    hWnd = CreateWindow(szOEAPIInitSink,
        OEAPI_INITCOM_CALLBACK_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        400, 200,
        NULL,
        NULL,
        g_hOEAPIINITCOM,
        NULL);

    if (hWnd == NULL) {
        debug_print(DEBUG_ERROR, _T("CreateCallbackWindow: CreateWindow() failed.\n"));
        return NULL;
    }

    return hWnd;
}

TOEAPIInit::coclass_implementation()
{
    DWORD serverID;
	::InterlockedIncrement(&g_objectCounter);

    active_ = FALSE;
    hwnd_ = CreateCallbackWindow();

    ::SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);
    hThread_ = ::CreateThread(0, 0, ::OEAPIInitServerProc, this, 0, &serverID);
    if(hThread_ == NULL) {
        debug_print(DEBUG_ERROR, _T("OEAPIInit::OEAPIInit: Error CreateThread\n"));
    }
}

TOEAPIInit::~coclass_implementation()
{
    ::DestroyWindow(hwnd_);
    
    //FIXME: Don't use TerminateThread
	if(!::TerminateThread(hThread_, 0))
    {
        debug_print(DEBUG_ERROR, _T("OEAPIInit::~OEAPIInit: Error TerminateThread\n"));
    }

	::InterlockedDecrement(&g_objectCounter);
}

TCHAR * TOEAPIInit::get_progid() { return OEAPI_OEAPIINIT_PROGID; }

BOOL TOEAPIInit::IsOEAPIActive()
{
    return active_;
}

void TOEAPIInit::HelloWorld()
{
    std::basic_stringstream<TCHAR> msg;
    msg << _T("counter = ") << g_objectCounter << std::endl;
    MessageBox(0, msg.str().c_str(), 0, 0);
}

void TOEAPIInit::TriggerOnInitOEAPI()
{
    HANDLE init = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    ::PostMessage(hwnd_, OEAPI_INIT_MESSAGE_CODE, 0, (LPARAM)init);
    ::WaitForSingleObject(init, INFINITE);
    ::CloseHandle(init);
}

void TOEAPIInit::FireOnInitOEAPI()
{
    active_ = TRUE;
    connection_point.Fire_OnInitOEAPI();
}

void TOEAPIInit::TriggerOnShutdownOEAPI()
{
    active_ = FALSE;
    HANDLE shutdown = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    ::PostMessage(hwnd_, OEAPI_SHUTDOWN_MESSAGE_CODE, 0, (LPARAM)shutdown);
    ::WaitForSingleObject(shutdown, INFINITE);
    ::CloseHandle(shutdown);
}

void TOEAPIInit::FireOnShutdownOEAPI()
{
    connection_point.Fire_OnShutdownOEAPI();
}

void TOEAPIInit::NotifyInitComplete()
{
    HWND hOeapi = FindWindowRecursive(NULL, OEAPI_OECOM_CALLBACK_CLASS);

    // notify oecom.dll that the event was served
    if(hOeapi != NULL) {
        PostMessage(hOeapi, WM_OEAPI_INIT_NOTIFICATION_MSG_CODE, OEAPI_INIT_FINISH, reinterpret_cast<LPARAM>(hwnd_));
    }
    else {
        debug_print(DEBUG_ERROR, _T("ServerProc: Error FindWindowRecursive OEAPI_CALLBACK_WND_CLASS.\n"));
    }
}

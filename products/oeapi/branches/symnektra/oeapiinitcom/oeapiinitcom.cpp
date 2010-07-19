/* $Id: oeapiinitcom.cpp,v 1.23 2008/09/07 16:56:33 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "oeapi_config.h"
#include "std.h"

using namespace comet;
using namespace OEAPIINITCOM;

typedef com_server< OEAPIINITCOM::type_library > SERVER;

#include <list>
#include <sstream>

#include "oeapi_event.h"
#include "se_debug.h"

#include "oeapi_utils.h"


#pragma data_seg(OEAPI_SHARED_DATA)

// BUG: if I don't initialize the array the compiler doesn't create the buffer in shared
// memory
HWND g_hwndServerArray[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
							,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
							,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
							,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
							,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int g_hwndCount = 0;
int g_initializedObjects = 0;

#define OEAPI_SECTION_NAME "/section:" ## OEAPI_SHARED_DATA ## ",rws"
#pragma data_seg()
#pragma comment(linker, OEAPI_SECTION_NAME)


int counter = 0;
HINSTANCE hInst = NULL;

DWORD _stdcall OEAPIInitServerProc(void *obj);

const UINT OEAPI_INIT_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnInit"));
const UINT OEAPI_SHUTDOWN_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnShutdown"));

typedef coclass_implementation<OEAPIInit> TOEAPIInit;

HWND CreateCallbackWindow();

///////////////////////////////////////////////////////////////////
// Mutex functions
///////////////////////////////////////////////////////////////////
class OEAPIMutex {
public:
	OEAPIMutex()
	{
		_hMutex = CreateMutex(NULL, FALSE, OEAPI_INITCOM_MUTEX);
		if(_hMutex == NULL) {
			debug_print(DEBUG_ERROR, _T("OEAPIInit::OEAPIInit: Error CreateMutex\n"));
		}
		else {
			WaitForSingleObject(_hMutex, INFINITE);
		}
	}

	~OEAPIMutex()
	{
		if(_hMutex) {
			ReleaseMutex(_hMutex);
		}
	}

private:
	HANDLE _hMutex;
};
///////////////////////////////////////////////////////////////////

/**
This object implements the OEAPIInit. It has 2 events: one when the OEAPI inits and the
other when it shutdown.
The OEAPI set the events that this object handles in the ServerProc.
*/
template<> class coclass_implementation<OEAPIInit> : public coclass<OEAPIInit>
{
public:
	coclass_implementation<OEAPIInit>()
	{
		DWORD serverID;
		counter++;

		active_ = FALSE;
		hwnd_ = CreateCallbackWindow();
		if(hwnd_) {
			OEAPIMutex mutex;
			g_hwndServerArray[g_hwndCount++] = hwnd_;
		}

		::SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);
		hThread_ = ::CreateThread(0, 0, ::OEAPIInitServerProc, this, 0, &serverID);
		if(hThread_ == NULL) {
			debug_print(DEBUG_ERROR, _T("OEAPIInit::OEAPIInit: Error CreateThread\n"));
		}
	}
	~coclass_implementation<OEAPIInit>()
	{
		{
			OEAPIMutex mutex;
			for(int i=0; i<g_hwndCount; i++) {
				if(g_hwndServerArray[i] == hwnd_) {
					for(int j=i+1; j<g_hwndCount; j++) {
						g_hwndServerArray[j-1] = g_hwndServerArray[j];
					}

					g_hwndCount--;
					break;
				}
			}
		}

		::DestroyWindow(hwnd_);
		if(!::TerminateThread(hThread_, 0)) {
			debug_print(DEBUG_ERROR, _T("OEAPIInit::~OEAPIInit: Error TerminateThread\n"));
		}

		counter--;
	}

	static TCHAR *get_progid() { return OEAPI_OEAPIINIT_PROGID; }

	BOOL IsOEAPIActive()
	{
		return active_;
	}

	void HelloWorld()
	{
		std::basic_stringstream<TCHAR> msg;
		msg << _T("counter = ") << counter++ << std::endl;
		MessageBox(0, msg.str().c_str(), 0, 0);
	}

	void TriggerOnInitOEAPI()
	{
		HANDLE init = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		::PostMessage(hwnd_, OEAPI_INIT_MESSAGE_CODE, 0, (LPARAM)init);
		::WaitForSingleObject(init, INFINITE);
		::CloseHandle(init);
	}

	void FireOnInitOEAPI()
	{
		active_ = TRUE;
		connection_point.Fire_OnInitOEAPI();
	}

	void TriggerOnShutdownOEAPI()
	{
		active_ = FALSE;
		HANDLE shutdown = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		::PostMessage(hwnd_, OEAPI_SHUTDOWN_MESSAGE_CODE, 0, (LPARAM)shutdown);
		::WaitForSingleObject(shutdown, INFINITE);
		::CloseHandle(shutdown);
	}

	void FireOnShutdownOEAPI()
	{
		connection_point.Fire_OnShutdownOEAPI();
	}

private:
	HANDLE hThread_;
	HWND hwnd_;
	BOOL active_;
};

/**
This function is necessary because the serverProc runs in another thread and the events
must be triggered in the same thread that the COM object was created or the thread
of the constructor.
*/
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
	wndclass.hInstance		= hInst;
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
				hInst,
				NULL);

	if (hWnd == NULL) {
		debug_print(DEBUG_ERROR, _T("CreateCallbackWindow: CreateWindow() failed.\n"));
		return NULL;
	}

	return hWnd;
}


DWORD _stdcall OEAPIInitServerProc(void *obj)
{
	coclass_implementation<OEAPIInit> *oeapi = (coclass_implementation<OEAPIInit> *) obj;
	HANDLE initEventHandle;
	HANDLE shutdownEventHandle;

	while(1) {
		initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_INIT_EVENT_NAME);
		if(initEventHandle == NULL) {
			debug_print(DEBUG_ERROR, _T("ServerProc: Error CreateEvent OEAPI_INIT_EVENT_NAME\n"));
			return 1;
		}

		//debug_print(DEBUG_ERROR, _T("ServerProc: WaitForSingleObject OEAPI_INIT_EVENT_NAME\n"));
		WaitForSingleObject(initEventHandle, INFINITE);

		oeapi->TriggerOnInitOEAPI();

		// another object finished the event
		g_initializedObjects++;

		// notify oecom.dll that we are triggering the event
		HWND hOeapi = FindWindowRecursive(NULL, OEAPI_OECOM_CALLBACK_CLASS);

		// notify oecom.dll that the event was served
		if(hOeapi != NULL) {
			PostMessage(hOeapi, WM_OEAPI_INIT_NOTIFICATION_MSG_CODE, OEAPI_INIT_FINISH, 0);
		}
		else {
			debug_print(DEBUG_ERROR, _T("ServerProc: Error FindWindowRecursive OEAPI_CALLBACK_WND_CLASS.\n"));
		}

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


///////////////////////////////////////////////////////////////////////////
template<> class coclass_implementation<OEAPIInitState> : public coclass<OEAPIInitState>
{
public:
	coclass_implementation<OEAPIInitState>()
	{
	}
	~coclass_implementation<OEAPIInitState>()
	{
	}

	static TCHAR *get_progid() { return OEAPI_OEAPIINITSTATE_PROGID; }

	int GetOEAPIInitObjectsCount()
	{
		int count = 0;

		OEAPIMutex mutex;

		// verify that the windows exist.
		for(int i=0; i<g_hwndCount; i++) {
			if(IsWindow((HWND) g_hwndServerArray[i])) {
				count++;
			}
			else {
				debug_print(DEBUG_ERROR, _T("GetActiveObjectsCount: Window Deleted.\n"));
			}
		}

		return count;
	}

	int GetOEAPIInitInitializedObjectsCount()
	{
		return g_initializedObjects;
	}

	void ResetOEAPIInitInitializedObjectsCount()
	{
		g_initializedObjects = 0;
	}
};


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if(dwReason == DLL_PROCESS_ATTACH) {
		hInst = hInstance;
	}
	else if(dwReason == DLL_PROCESS_DETACH) {
		if(counter != 0) {
			debug_print(DEBUG_ERROR, _T("OEAPIINITCOM DLL_PROCESS_DETACH: There are objects leaving.\n"));
		}
	}

	return SERVER::DllMain(hInstance, dwReason, 0);
}

STDAPI DllCanUnloadNow()
{
	return SERVER::DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return SERVER::DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer()
{
	return SERVER::DllRegisterServer();
}

STDAPI DllUnregisterServer()
{
	return SERVER::DllUnregisterServer();
}

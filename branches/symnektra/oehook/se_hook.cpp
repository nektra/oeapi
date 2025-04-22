/* $Id: se_hook.cpp,v 1.28 2008/09/07 16:56:39 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include <windows.h>
#include <malloc.h>
#include <shlwapi.h>
#include <psapi.h>
#include <tchar.h>
#include <se_debug.h>
#include <oecom.h>

#define WM_REMOVEHOOK WM_USER+0
#define OEAPI_LIB_NAME _T("oecom.dll")

const TCHAR OutlookExpressClassName[] = _T("Outlook Express Browser Class");
const TCHAR ATLSysListView32ClassName[] = _T("ATL:SysListView32");
const TCHAR ATHNOTEClassName[] = _T("ATH_Note");
const CHAR StartServerProcName[] = "StartServer";

HMODULE g_hCurrentModule;
bool g_hooked;

HWINEVENTHOOK hWinEventHook = NULL;
HWND hWnd = NULL;
HHOOK hCBTHook = NULL;

void GetLibraryPath(const LPTSTR fileName, LPTSTR out_buffer, size_t bufferSize)
{
    HMODULE hModule = GetModuleHandle(_T("oehook.dll"));
    GetModuleFileName(hModule, out_buffer, (DWORD)bufferSize);
    PathRemoveFileSpec(out_buffer);

    _tcscat_s(out_buffer,bufferSize,_T("\\"));
    _tcscat_s(out_buffer,bufferSize,fileName);
}

bool LoadOeComModule(HWND hwnd)
{
    TCHAR file[MAX_PATH];
    GetLibraryPath(_T("oecom.dll"),file, MAX_PATH);
    HINSTANCE g_hSEPlugin = LoadLibrary(file);
    if(g_hSEPlugin != NULL)
    {
        StartServer_t pStartServer = (StartServer_t) GetProcAddress(g_hSEPlugin, "StartServer");
        if(pStartServer == NULL)
        {
            debug_print(DEBUG_ERROR, _T("CBTProc: Error GetProcAddress\n"));
        }
        else
        {
            pStartServer(hwnd);
            return TRUE;
        }
    }
    else
    {
        debug_print(DEBUG_ERROR, _T("CBTProc: Error LoadLibrary %d\n"), GetLastError());
    }
    return FALSE;
}


extern "C" __declspec(dllexport) 
LRESULT CALLBACK CBTHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode == HCBT_CREATEWND && !g_hooked)
    {
        LPCBT_CREATEWND lpCreateWnd = (LPCBT_CREATEWND)lParam;
        LPCREATESTRUCT lpcs = lpCreateWnd->lpcs;
        HWND hParentWindow = lpcs->hwndParent;

        HWND hCreatedWindow = (HWND)wParam;

        TCHAR wndClassName[sizeof(OutlookExpressClassName)/sizeof(OutlookExpressClassName[0])];


        GetClassName(hCreatedWindow, wndClassName, sizeof(wndClassName)/sizeof(wndClassName[0]));
        if(_tcscmp(wndClassName, OutlookExpressClassName) == 0 ||
           _tcscmp(wndClassName, ATLSysListView32ClassName) == 0 ||
           _tcscmp(wndClassName, ATHNOTEClassName) == 0 )
        {
            g_hooked = LoadOeComModule(hCreatedWindow);
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL CheckProcessName(HANDLE hProcess)
{
    TCHAR fname[MAX_PATH];

    if (GetModuleFileNameEx(hProcess,NULL,fname,MAX_PATH)==0) return FALSE;

    LPTSTR processName = PathFindFileName(fname);

    if(_tcsicmp(processName,_T("msimn.exe")) == 0 || 
       _tcsicmp(processName,_T("winmail.exe")) == 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL InstallCBTHook(DWORD dwThreadId)
{
    if ( g_hCurrentModule != NULL && dwThreadId != 0 )
    {
        debug_print(DEBUG_TRACE,_T("Installing CBT Hook on thread 0x%x"),dwThreadId);
        hCBTHook = SetWindowsHookEx(WH_CBT, CBTHookProc, g_hCurrentModule, dwThreadId);
        if (hCBTHook!=NULL)
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL RemoveCBTHook()
{
    if (hCBTHook!=NULL)
    {
        UnhookWindowsHookEx(hCBTHook);
        debug_print(DEBUG_TRACE,_T("CBT Hook Removed\n"));
        hCBTHook=NULL;
    }
    return TRUE;
}


BOOL RemoveWinEventHook()
{
    debug_print(DEBUG_TRACE,_T("Removing WinEventHook 0x%x\n"),hWinEventHook);
    if ( hWinEventHook!=NULL && UnhookWinEvent(hWinEventHook))
    {
        hWinEventHook = NULL;
        return TRUE;
    }
    return FALSE;
}

BOOL RemoveWinEventHookAndWait(HANDLE hProcess)
{
    if (RemoveWinEventHook())
    {
        PostMessage(hWnd,WM_REMOVEHOOK,(WPARAM) hProcess,0);
        return TRUE;
    }
    return FALSE;
};

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook,
                           DWORD event,
                           HWND hwnd,
                           LONG idObject,
                           LONG idChild,
                           DWORD dwEventThread,
                           DWORD dwmsEventTime)
{
    UNREFERENCED_PARAMETER(hWinEventHook);
    UNREFERENCED_PARAMETER(event);
    UNREFERENCED_PARAMETER(dwEventThread);
    UNREFERENCED_PARAMETER(dwmsEventTime);


    if (idObject==OBJID_WINDOW && idChild==CHILDID_SELF && hwnd!=NULL )	
    {
        DWORD dwProcessId;
        DWORD dwThreadId;
        dwThreadId = GetWindowThreadProcessId(hwnd,&dwProcessId);

        if (dwThreadId!=0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,dwProcessId);

            if(CheckProcessName(hProcess) && InstallCBTHook(dwThreadId))
            {
                if (!RemoveWinEventHookAndWait(hProcess))
                {
                    CloseHandle(hProcess);
                }
            }
            else
            {
                CloseHandle(hProcess);
            }
        }
    }		
    return;
}


void InstallWinEventHook()
{
    hWinEventHook = SetWinEventHook(EVENT_OBJECT_CREATE,EVENT_OBJECT_CREATE,NULL,WinEventProc,0,0,WINEVENT_OUTOFCONTEXT|WINEVENT_SKIPOWNPROCESS);
    debug_print(DEBUG_TRACE,_T("WinEventHook 0x%x\n"),hWinEventHook);
}

BOOL WaitProcess(HANDLE hProcess)
{
    debug_print(DEBUG_TRACE,_T("Waiting for process to end..."));
    WaitForSingleObject(hProcess,INFINITE);
    CloseHandle(hProcess);
    return TRUE;
}

LRESULT CALLBACK LoaderWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_REMOVEHOOK:
        WaitProcess((HANDLE) wParam);
        RemoveCBTHook();
        InstallWinEventHook();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL DllMain_ProcessAttach(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(reason);
    UNREFERENCED_PARAMETER(lpReserved);
    
    g_hooked = false;

    if (!CheckProcessName(GetCurrentProcess()))
    {
        WNDCLASSEX wcex;
        memset(&wcex,0,sizeof(wcex));
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.lpfnWndProc	= LoaderWndProc;
        wcex.lpszClassName	= L"LOADER";
        ATOM wndclass = RegisterClassEx(&wcex);

        hWnd = CreateWindow((LPCWSTR)wndclass, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

        if (!hWnd) return FALSE;

        g_hCurrentModule = hModule;
    
        InstallWinEventHook();
    }


    return TRUE;
}

BOOL DllMain_ProcessDetach(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(reason);
    UNREFERENCED_PARAMETER(lpReserved);

    if (!CheckProcessName(GetCurrentProcess()))
    {
        RemoveCBTHook();
        RemoveWinEventHook();
    }
   

    return TRUE;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        return DllMain_ProcessAttach(hModule, reason, lpReserved);
    case DLL_PROCESS_DETACH:
        return DllMain_ProcessDetach(hModule, reason, lpReserved);
    }
    return TRUE;
}

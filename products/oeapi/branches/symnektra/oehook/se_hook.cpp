/* $Id: se_hook.cpp,v 1.28 2008/09/07 16:56:39 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include <windows.h>
#include <winuser.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>
#include <tchar.h>

#include "se_hook.h"

#include "se_debug.h"
#include "oeapi_utils.h"

#include "oecom.h"

#include "oeapi_event.h"

#include "Win32Process.h"

#define OEAPI_LIB_NAME _T("oecom.dll")

/* ---------------- */
/* Global variables */
/* ---------------- */

//#pragma data_seg(".SharedData")
#pragma data_seg(OEAPI_SHARED_DATA)

BOOL g_Hooked = FALSE;
ULONG counter = 0;

// NT cannot return FALSE in DllMain so keep the dll loaded in NT.
BOOL IsNT = TRUE;
BOOL Is9x = FALSE;

HHOOK g_hCBTHook;

// oecom version
int g_OecomVersion = -1;

// this flag indicates if the dll in unloaded from other processes than OE
int g_UnloadFromOtherProcesses = -1;

#define OEAPI_SECTION_NAME "/section:" ## OEAPI_SHARED_DATA ## ",rws"
#pragma data_seg()
#pragma comment(linker, OEAPI_SECTION_NAME)
//#pragma comment(linker, "/section:.SharedData,rws")

HINSTANCE g_Hinst = NULL;

// the process that calls LoadLibrary to this library has this flag set to know when
// it detached to Unhook
BOOL g_IsLauncher = FALSE;

// oecom library
HINSTANCE g_hSEPlugin = NULL;

Win32Process W32Process;

// Set the registry key with the version of oecom
void SetMaxVersion(int version)
{
	HKEY hKey;

	if(RegOpenKeyEx(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) {
		if(RegCreateKey(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY, &hKey) != ERROR_SUCCESS) {
			debug_print(DEBUG_ERROR, _T("SetMaxVersion: Error RegCreateKey\n"));
			return;
		}
	}

	if(RegSetValueEx(hKey, _T("MaxVersion"), 0, REG_DWORD, (CONST BYTE*) &version, sizeof(version)) != ERROR_SUCCESS) {
		debug_print(DEBUG_ERROR, _T("SetMaxVersion: Error RegSetValueEx\n"));
	}

//	// reset the flag used to load only 1 instance of oecom
//	version = 0;
//	if(RegSetValueEx(hKey, _T("DllLoaded"), 0, REG_DWORD, (CONST BYTE*) &version, sizeof(version)) != ERROR_SUCCESS) {
//		debug_print(DEBUG_ERROR, _T("SetMaxVersion: Error RegSetValueEx\n"));
//	}

	RegCloseKey(hKey);
}

// Get the registry key value that contains the max version of oecom
int GetMaxVersion()
{
	DWORD version = -1;
	HKEY hKey;
	DWORD size, type;

	size = sizeof(version);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
		if(RegQueryValueEx(hKey, _T("MaxVersion"), NULL, &type, (BYTE*) &version, &size) != ERROR_SUCCESS) {
			version = -1;
		}

		RegCloseKey(hKey);
	}

	return version;
}

// Get the registry key value that contains the info to know if oecom was loaded
// this flag is set by oecom.dll
BOOL IsOecomLoaded()
{
	BOOL loaded = FALSE;
	HANDLE initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_INIT_EVENT_NAME);

	if(initEventHandle != NULL) {
		if(WaitForSingleObject(initEventHandle, 0) == WAIT_OBJECT_0) {
			loaded = TRUE;
		}

		CloseHandle(initEventHandle);
	}

//	BOOL loaded = FALSE;
//	DWORD aux;
//	HKEY hKey;
//	DWORD size, type;
//
//	if(RegOpenKeyEx(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
//		if(RegQueryValueEx(hKey, _T("DllLoaded"), NULL, &type, (BYTE*) &aux, &size) == ERROR_SUCCESS) {
//			if(aux == 1) {
//				loaded = TRUE;
//			}
//		}
//
//		RegCloseKey(hKey);
//	}

	return loaded;
}

// Remove the version info that was set in the registry
void RemoveVersionInfo(BOOL removeLoadedInfo = TRUE)
{
	HKEY hKey;

	if(RegOpenKeyEx(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
		RegDeleteValue(hKey, _T("MaxVersion"));

		if(removeLoadedInfo) {
			RegDeleteValue(hKey, _T("DllLoaded"));
		}

		RegCloseKey(hKey);
	}
}

/**
This function verify if OE is running. If it is not running and the init event is set,
it reset the event.
*/
VOID VerifyEvent()
{
	BOOL loaded = FALSE;
	HANDLE initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_INIT_EVENT_NAME);

	if(initEventHandle != NULL) {
		if(WaitForSingleObject(initEventHandle, 0) == WAIT_OBJECT_0) {
			if(W32Process.FindProcess("msimn") == 0 && W32Process.FindProcess("winmail") == 0) {
				debug_print(DEBUG_ERROR, _T("OEHOOK: Reseting event.\n"));
				ResetEvent(initEventHandle);
			}
		}

		CloseHandle(initEventHandle);
	}
}

VOID GetLibraryPath(TCHAR *path, DWORD nSize)
{
	TCHAR *lastPart;

	// put the full OEAPI library path in path by getting the path of this dll and
	// replacing the file part with OEAPI_LIB_NAME
	if(GetModuleFileName(g_Hinst, path, nSize) == 0) {
		debug_print(DEBUG_ERROR, _T("SetHook: Error GetModuleFileName\n"));
#if _MSC_VER >= 1400
		_tcscpy_s(path, nSize, OEAPI_LIB_NAME);
#else
		_tcsncpy(path, OEAPI_LIB_NAME, nSize);
#endif
	}
	else {
		lastPart = _tcsrchr(path, _T('\\'));
		if(lastPart++ != NULL) {
			*lastPart = 0;
#if _MSC_VER >= 1400
			_tcscat_s(path, nSize, OEAPI_LIB_NAME);
#else
			_tcscat(path, OEAPI_LIB_NAME);
#endif
//			dbgprint(path);
//			debug_print(DEBUG_ERROR, _T("\n"));
		}
	}
}

INT SetHook(VOID)
{
	// Now, the system will call our CBTProc callback before doing operations
	// on any window.
	g_hCBTHook = SetWindowsHookEx(WH_CBT, (HOOKPROC) CBTProc, g_Hinst, 0) ;
	if (g_hCBTHook == NULL) {
		debug_print(DEBUG_ERROR, _T("SetHook: Error SetWindowsHookEx\n"));
		return -1;
	}

	return 0;
}

VOID RemoveHook(VOID)
{
	debug_print(DEBUG_ERROR, _T("RemoveHook\n"));

	if(IsOE()) {
		// remove registry information because the loaded flag
		RemoveVersionInfo();
	}
	if(g_Hooked) {
		if (UnhookWindowsHookEx(g_hCBTHook) == 0)
		{
			debug_print(DEBUG_ERROR, _T("RemoveHook: Error UnhookWindowsHookEx\n"));
		}
		g_hCBTHook = NULL;
	}
}

StartServer_t pStartServer = NULL;
GetLibraryVersion_t pGetLibraryVersion = NULL;


LRESULT WINAPI CBTProc (int nCode, WPARAM wParam, LPARAM lParam)
{
//	if(!IsOEVerified) {
//		IsOEVerified = TRUE;
//
//		if(!IsOE()) {
//			debug_print(DEBUG_ERROR, _T("Not OE\n"));
//			HRESULT hr = CallNextHookEx(g_hCBTHook, nCode, wParam, lParam);
//
//			if(FreeLibrary(g_Hinst)) {
//				debug_print(DEBUG_ERROR, _T("Unhooked\n"));
//			}
//
//			return hr;
//		}
//		else {
//			debug_print(DEBUG_ERROR, _T("Yes OE\n"));
//		}
//	}

	if(nCode == HCBT_ACTIVATE || nCode == HCBT_CREATEWND) {
		VerifyEvent();
	}

	if(nCode >= 0) {
		TCHAR szBuf[1024];

		switch (nCode)
        {
			// We will call GetWindowText only at HCBT_ACTIVATE
//			case HCBT_ACTIVATE:

			case HCBT_CREATEWND:
			{
				LPCBT_CREATEWND pcws = (LPCBT_CREATEWND) lParam;

				if(pcws && pcws->lpcs->hwndParent == NULL) {
					if(!IsOecomLoaded() && IsOE() && GetMaxVersion() < g_OecomVersion) {
						SetMaxVersion(g_OecomVersion);
					}
				}

				memset(szBuf, 0, sizeof(szBuf));
				GetClassName((HWND) wParam, szBuf, sizeof(szBuf)/sizeof(TCHAR)-sizeof(TCHAR));

				// BEGIN DEBUG
//				TCHAR dbgBuf[1024];
//				sprintf(dbgBuf, "Class %s\n", szBuf);
//				dbgprint(dbgBuf);
				// END DEBUG

				if ((!_tcscmp(szBuf, _T("Outlook Express Browser Class"))) ||
					(!_tcscmp(szBuf, _T("ATL:SysListView32"))) ||
					(!_tcscmp(szBuf, _T("ATH_Note")))) {

					// BEGIN DEBUG
//					debug_print(DEBUG_ERROR, _T("Class Detected\n"));
					// END DEBUG

#ifdef NDEBUG
					// only load the greatest version of oecom.dll
					if(GetMaxVersion() == g_OecomVersion && !IsOecomLoaded()) {
#endif // NDEBUG
						// BEGIN DEBUG
						//debug_print(DEBUG_ERROR, _T("Version OK\n"));
						// END DEBUG

						// load COM interface
						TCHAR file[MAX_PATH*4];

						GetLibraryPath(file, MAX_PATH*4);

//						debug_print(DEBUG_ERROR, _T("CBTpROC\n"));

						// BEGIN DEBUG
						//dbgprint(file);
						//debug_print(DEBUG_ERROR, _T("\n"));
						// END DEBUG

//						debug_print(DEBUG_ERROR, _T("XXXXXXXXXXXX: g_hSEPlugin\n"));

//						if(g_hSEPlugin == NULL ) {
							g_hSEPlugin = LoadLibrary(file);

	//						dbgprint(file);
							if(g_hSEPlugin != NULL) {
	//							debug_print(DEBUG_ERROR, _T("\nOk\n"));
								pStartServer = (StartServer_t) GetProcAddress(g_hSEPlugin, "StartServer");

								if(pStartServer == NULL) {
									debug_print(DEBUG_ERROR, _T("CBTProc: Error GetProcAddress\n"));
								}
								else {
						// BEGIN DEBUG
						//debug_print(DEBUG_ERROR, _T("pStartServer\n"));
						// END DEBUG

									pStartServer((HWND) wParam);
								}
							}
							else {
								debug_print(DEBUG_ERROR, _T("CBTProc: Error LoadLibrary %d\n"), GetLastError());
							}
//						}
#ifdef NDEBUG
					}
					else if(g_hSEPlugin == NULL) {
						//char error[1024];
						TCHAR file[MAX_PATH*4];

						GetLibraryPath(file, MAX_PATH*4);
						//sprintf(error, "HCBT_CREATEWND Warning: There is a oecom newer than this (%s).\n", file);

						//dbgprint(error);
						debug_print(DEBUG_ERROR, _T("HCBT_CREATEWND Warning: There is a oecom newer than this (%s).\n"), file);
					}
#endif // NDEBUG
				}

				break;
			}
		}
	}

	return CallNextHookEx (g_hCBTHook, nCode, wParam, lParam);
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
   	switch (fdwReason)
	{
   		case DLL_PROCESS_ATTACH:
		{
			counter++;
       		g_Hinst = hinstDll;

			if(g_Hooked == FALSE) {
				OSVERSIONINFO osvi;
				TCHAR file[MAX_PATH*4];
				HMODULE hOecom;

//			// BEGIN DEBUG
//			debug_print(DEBUG_ERROR, _T("DLL_PROCESS_ATTACH 1\n"));
//			// END DEBUG

				g_OecomVersion = 0;

				// get the greatest version of oecom library
				GetLibraryPath(file, MAX_PATH*4);

				osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

				if(GetVersionEx(&osvi)) {
					IsNT = ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
						   (osvi.dwMajorVersion == 4));
					Is9x = (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
				}
				else {
					debug_print(DEBUG_ERROR, _T("OEHOOK: GetVersionEx Error.\n"));
				}

				// FIXME: I don't know why in 9x the LoadLibrary("oecom.dll"); generates
				// a exception when this library exits.
				// Workaround: skip version check
				if(Is9x) {
					g_IsLauncher = TRUE;

					if (SetHook() == -1) {
						debug_print(DEBUG_ERROR, _T("SetHook: Error.\n"));
					}
				}
				else {
					// test if this oecom.dll is newer than any other loaded (its version is greater)
					hOecom = LoadLibrary(file);
					if(hOecom != NULL) {
						pGetLibraryVersion = (GetLibraryVersion_t) GetProcAddress(hOecom, "GetLibraryVersion");
						if(pGetLibraryVersion != NULL) {
							g_OecomVersion = pGetLibraryVersion();
						}

						// we enter with equal because if launcher.exe crashed last boot, the
						// registry key was set (maybe) by this dll and it should be loaded.
						// then, in the hook only 1 dll is loaded.
	//					if(g_OecomVersion >= GetMaxVersion()) {
	//						SetMaxVersion(g_OecomVersion);
	//					}

						g_IsLauncher = TRUE;

						if (SetHook() == -1) {
							debug_print(DEBUG_ERROR, _T("SetHook: Error.\n"));
						}

#ifndef NDEBUG
						// in W98 this line generates a exception, but it's useful to prevent
						// launcher to lock oecom library.
						FreeLibrary(hOecom);
#endif // NDEBUG
					}
					else {
						debug_print(DEBUG_ERROR, _T("LoadLibrary: Cannot find oecom.dll.\n"));
					}
				}

				// all the oehooks remove registry old data.
				RemoveVersionInfo();

				g_Hooked = TRUE;
			}
//#ifndef NDEBUG
			// NT display a error message if we choose not to load the dll so this
			// optimization is not used in NT.
			else if(!IsNT) {
//			// BEGIN DEBUG
//			char szBuf[1024];
//			// double verification: verify that the exe is OE
//			if(GetModuleFileName(NULL, szBuf, sizeof(szBuf)/sizeof(TCHAR)-sizeof(TCHAR)) == 0) {
//				debug_print(DEBUG_ERROR, _T("CBTProc: Error GetModuleFileName\n"));
//				return FALSE;
//			}
//			else {
//				TCHAR *lastPart;
//				lastPart = _tcsrchr(szBuf, _T('\\'));
//				if(lastPart++ != NULL) {
//					// DEBUG BEGIN
//			//			CHAR file[1024];
//			//			sprintf(file, "%s\n", szBuf);
//			//			dbgprint(file);
//					// DEBUG END
//
//					if(_tcsicmp(lastPart, _T("mspaint.exe")) && _tcsicmp(lastPart, _T("mspaint"))) {
//						return FALSE;
//					}
//				}
//			}
//
//			debug_print(DEBUG_ERROR, _T("DLL_PROCESS_ATTACH 2\n"));
//			// END DEBUG

				g_UnloadFromOtherProcesses = 1;
//				if(g_UnloadFromOtherProcesses == -1) {
					HKEY hKey;
					DWORD dwType;
					DWORD size = sizeof(DWORD);
					DWORD dwValue;

					if(RegOpenKeyEx(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
						if(RegQueryValueEx(hKey, _T("HookAllProcesses"), NULL, &dwType,
							(LPBYTE) &dwValue, &size) == ERROR_SUCCESS) {
							g_UnloadFromOtherProcesses = !dwValue;
						}

						RegCloseKey(hKey);
					}

//				}

				// BEGIN DEBUG
				//break;
				// END DEBUG

				if(!IsOE() && !IsVistaIE() && g_UnloadFromOtherProcesses) {
//					// BEGIN DEBUG
//					debug_print(DEBUG_ERROR, _T("DLL_PROCESS_ATTACH 4\n"));
//					// END DEBUG
					//OutputDebugString(_T("Goodby, cruel world\n"));

					return FALSE;
				}
			}
//#endif // NDEBUG
		}
		break;

		case DLL_PROCESS_DETACH:
		{
//			// BEGIN DEBUG
//			debug_print(DEBUG_ERROR, _T("DLL_PROCESS_DETACH\n"));
//			// END DEBUG

			if(IsOE()) {
				// remove registry information because the loaded flag
				RemoveVersionInfo();
			}
			if(g_IsLauncher && g_Hooked) {
				if(UnhookWindowsHookEx(g_hCBTHook) == 0) {
					debug_print(DEBUG_ERROR, _T("RemoveHook: Error UnhookWindowsHookEx\n"));
				}

				g_hCBTHook = NULL;
			}
//			// BEGIN DEBUG
//			debug_print(DEBUG_ERROR, _T("DLL_PROCESS_DETACH EXIT\n"));
//			// END DEBUG

		}

		break;
   }

   return(TRUE);
}


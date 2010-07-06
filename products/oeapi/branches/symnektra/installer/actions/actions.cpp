/* $Id: actions.cpp,v 1.8 2007/04/26 18:24:18 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include <windows.h>
#include <tchar.h>

#include "Win32Process.h"


BOOL windowFound;

///////////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
	DWORD processId;

	GetWindowThreadProcessId(hwnd, &processId);
	if( processId == lParam ) {
		SendMessage(hwnd,WM_CLOSE,0,0);
		windowFound = TRUE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////
void KillProcess(const char *procName)
{
	Win32Process win32;
	int id = win32.FindProcess(procName);
	if( id ) {
		windowFound = FALSE;

		EnumWindows(EnumWindowsProc, id);
		OutputDebugString("EnumWindows\n");
		if( !windowFound ) {
			OutputDebugString("!windowFound\n");
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
										   PROCESS_VM_READ | PROCESS_TERMINATE,
										   FALSE, id );

			if( hProcess ) {
				TerminateProcess(hProcess, 1);

				CloseHandle(hProcess);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
UINT __stdcall ClosePrograms(HANDLE hInstall)
{
	KillProcess("launcher");
	KillProcess("msimn");

	Sleep(5000);

	return 1;
}

///////////////////////////////////////////////////////////////////////
UINT __stdcall DeleteRegKey(HANDLE hInstall)
{
	RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Nektra\\OEAPI\\Settings"));

	return 1;
}

///////////////////////////////////////////////////////////////////////
UINT __stdcall RebootPending(HANDLE hInstall)
{
	Win32Process win32;
	UINT reboot = FALSE;
	if(win32.IsWinNT())
	{
		// check in 
		// HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\PendingFileRenameOperations
		// HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnce 
		// HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnceEx 
		// HKLM\Software\Microsoft\Windows\CurrentVersion\RunServicesOnce 
		// HKCU\Software\Microsoft\Windows\CurrentVersion\RunOnce
	}
	return reboot;
}


///////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	return TRUE;
}
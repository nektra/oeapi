/* $Id: winmail_utils.cpp,v 1.4 2007/04/20 21:59:08 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2006-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "winmail_utils.h"
#include "Win32Process.h"

#include "se_debug.h"

//BOOL CopyFromWinMail(void *dest, const void *src, size_t dwBytes)
//{
//	Win32Process w32Process;
//	DWORD dwWinMailId;
//	HANDLE hProcess;
//	DWORD dwRead;
//	BOOL ret = FALSE;
//
//	dwWinMailId = w32Process.FindProcess("winmail");
//	if(dwWinMailId == 0) {
//		debug_print(DEBUG_ERROR, _T("CopyFromWinMail: Error FindProcess Cannot locate WinMail.exe.\n"));
//	}
//	else {
//		// if we are in WinMail.exe just copy the buffer
//		if(dwWinMailId == GetCurrentProcessId()) {
//			memcpy(dest, src, dwBytes);
//			ret = TRUE;
//		}
//		else {
//			hProcess = OpenProcess(PROCESS_VM_READ, FALSE, dwWinMailId);
//			if(hProcess == NULL) {
//				debug_print(DEBUG_ERROR, _T("StoreWndProc: Error OpenProcess.\n"));
//			}
//			else {
//				if(!ReadProcessMemory(hProcess, src, dest, dwBytes, &dwRead)) {
//					dwRead = GetLastError();
//					debug_print(DEBUG_ERROR, _T("StoreWndProc: Error ReadProcessMemory.\n"));
//				}
//				else if(dwRead == dwBytes) {
//					ret = TRUE;
//				}
//
//				CloseHandle(hProcess);
//			}
//		}
//	}
//
//	return ret;
//}

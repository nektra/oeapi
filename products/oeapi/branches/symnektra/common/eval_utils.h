/* $Id: eval_utils.h,v 1.8 2008/09/07 16:55:57 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __EVAL_UTILS_H
#define __EVAL_UTILS_H

#include <windows.h>
#include <tchar.h>

#include "oeapi_config.h"
#include "se_debug.h"

#define EVALUATION_PERIOD 30

#ifdef EVALUATION_VERSION

//#include <time.h>
#include <sstream>
#include "nkt_registry.h"

#define OEAPI_NEKTRA_DATA_KEY NKTCONCAT(OEAPI_NEKTRA_KEY, _T("\\Data"))

inline BOOL VerifyExpiration(BOOL displayMsg=FALSE)
{
	NktRegistry reg;

	//DWORD secs, size, type;
	//time_t curTime, elapsedTime, oldCurTime;
	//TCHAR expMsg[1024];
	//SYSTIME curTime, oldTime;
	LARGE_INTEGER curTime, oldTime;
	LARGE_INTEGER secs;
	DWORD size, type;

	//time(&curTime);
	SYSTEMTIME sysTime;
	FILETIME fileTime;
	GetSystemTime(&sysTime);
	SystemTimeToFileTime(&sysTime, &fileTime);
	curTime.LowPart = fileTime.dwLowDateTime;
	curTime.HighPart = fileTime.dwHighDateTime;

	if(reg.Open(HKEY_CURRENT_USER, OEAPI_NEKTRA_DATA_KEY, KEY_ALL_ACCESS) != ERROR_SUCCESS) {
		if(reg.Create(HKEY_CURRENT_USER, OEAPI_NEKTRA_DATA_KEY) != ERROR_SUCCESS) {
			debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegCreateKey\n"));
			return FALSE;
		}

		if(reg.SetValue(_T("Flags"), REG_BINARY, (CONST BYTE*)&curTime, sizeof(curTime)) != ERROR_SUCCESS) {
			debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegSetValueEx\n"));
			return FALSE;
		}

		secs = curTime;
	}

	for(int i=0; i<10; i++) {
		size = sizeof(secs);

		// sometimes this fails (???)
		LONG ret;
		if((ret = reg.QueryValue(_T("Flags"), &type, (BYTE*)&secs, &size)) != ERROR_SUCCESS) {
			debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegQueryValueEx %d - %08x\n"), i, ret);
			if(i == 9) {
				return FALSE;
			}
			::Sleep(100);
		}
		else {
			break;
		}
	}

	//1296000;
	//elapsedTime = curTime - secs;

	LARGE_INTEGER elapsedTime;
	elapsedTime.QuadPart = curTime.QuadPart - secs.QuadPart;

	// modified by hand
	if(elapsedTime.QuadPart < 0) {
		if(displayMsg) {
			MessageBox(NULL, _T("Evaluation Period Expired"), _T("OEAPI Expired"), MB_OK);
		}
		return FALSE;
	}

	 // ellapsed.QuadPart is in 100 ns so this convert to days
	int days = EVALUATION_PERIOD - (int)(elapsedTime.QuadPart / 864000000000ULL);
	//int days = EVALUATION_PERIOD - (int)(elapsedTime / 86400);

	size = sizeof(oldTime);
	if(reg.QueryValue(_T("Data"), &type, (BYTE*)&oldTime, &size) != ERROR_SUCCESS) {
		oldTime.QuadPart = 0;
	}

	// verify if the time was modified to prevent expiration
	if(oldTime.QuadPart > curTime.QuadPart) {
		if(displayMsg) {
			MessageBox(NULL, _T("Evaluation Period Expired"), _T("OEAPI Expired"), MB_OK);
		}
		return FALSE;
	}

	// only do it once
	if(displayMsg) {
		if(reg.SetValue(_T("Data"), REG_BINARY, (CONST BYTE*) &curTime, sizeof(curTime)) != ERROR_SUCCESS) {
			debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegSetValueEx\n"));
			return FALSE;
		}
	}

	// expired?
	if(days < 0) {
		if(displayMsg) {
			MessageBox(NULL, _T("Evaluation Period Expired"), _T("OEAPI Expired"), MB_OK);
		}
		return FALSE;
	}

	if(displayMsg) {
		//_stprintf(expMsg, _T("Evaluation Period: %d days remaining"), days);
		std::basic_stringstream<TCHAR> ss;
		ss << _T("Evaluation Period: ") << days << _T(" days remaining");
		MessageBox(NULL, ss.str().c_str(), _T("OEAPI Expiration"), MB_OK);
	}

	return TRUE;


	//HKEY hKey;

	//DWORD secs, size, type;
	//time_t curTime, elapsedTime, oldCurTime;
	//TCHAR expMsg[1024];

	//time(&curTime);

	//if(RegOpenKeyEx(HKEY_CURRENT_USER, OEAPI_NEKTRA_DATA_KEY, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) {
	//	if(RegCreateKey(HKEY_CURRENT_USER, OEAPI_NEKTRA_DATA_KEY, &hKey) != ERROR_SUCCESS) {
	//		debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegCreateKey\n"));
	//		return FALSE;
	//	}

	//	if(RegSetValueEx(hKey, _T("Flags"), 0, REG_DWORD, (CONST BYTE*) &curTime, sizeof(curTime)) != ERROR_SUCCESS) {
	//		debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegSetValueEx\n"));
	//		return FALSE;
	//	}

	//	secs = curTime;
	//}

	//for(int i=0;i<10;i++) {
	//	size = sizeof(secs);

	//	// sometimes this fails (???)
	//	if(RegQueryValueEx(hKey, _T("Flags"), NULL, &type, (BYTE*) &secs, &size) != ERROR_SUCCESS) {
	//		debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegQueryValueEx\n"));
	//		if(i==9) {
	//			return FALSE;
	//		}
	//		Sleep(100);
	//	}
	//	else {
	//		break;
	//	}
	//}

	////1296000;
	//elapsedTime = curTime - secs;

	//// modified by hand
	//if(elapsedTime < 0) {
	//	if(displayMsg) {
	//		MessageBox(NULL, _T("Evaluation Period Expired"), _T("OEAPI Expired"), MB_OK);
	//	}
	//	return FALSE;
	//}

	//int days = EVALUATION_PERIOD - elapsedTime / 86400;

	//if(RegQueryValueEx(hKey, _T("Data"), NULL, &type, (BYTE*) &oldCurTime, &size) != ERROR_SUCCESS) {
	//	oldCurTime = 0;
	//}

	//// verify if the time was modified to prevent expiration
	//if(oldCurTime > curTime) {
	//	if(displayMsg) {
	//		MessageBox(NULL, _T("Evaluation Period Expired"), _T("OEAPI Expired"), MB_OK);
	//	}
	//	return FALSE;
	//}

	//// only do it once
	//if(displayMsg) {
	//	if(RegSetValueEx(hKey, _T("Data"), NULL, REG_DWORD, (CONST BYTE*) &curTime, sizeof(curTime)) != ERROR_SUCCESS) {
	//		debug_print(DEBUG_ERROR, _T("VerifyExpiration: Error RegSetValueEx\n"));
	//		return FALSE;
	//	}
	//}

	//// expired?
	//if(days < 0) {
	//	if(displayMsg) {
	//		MessageBox(NULL, _T("Evaluation Period Expired"), _T("OEAPI Expired"), MB_OK);
	//	}
	//	return FALSE;
	//}

	//if(displayMsg) {
	//	_stprintf(expMsg, _T("Evaluation Period: %d days remaining"), days);
	//	MessageBox(NULL, expMsg, _T("OEAPI Expiration"), MB_OK);
	//}

	//RegCloseKey(hKey);

	//return TRUE;
}
#endif // EVALUATION_VERSION


#endif // __EVAL_UTILS_H
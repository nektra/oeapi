/* $Id: oeapi_utils.h,v 1.26 2008/09/07 16:55:58 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __OEAPI_UTILS_H
#define __OEAPI_UTILS_H

#include <windows.h>
//#include <tchar.h>
#include <string>

#include "se_debug.h"

#include "msoeapi.h"
#include "nkt_utils.h"


#define HENUMSTORE_INVALID ((HENUMSTORE)-1)

#define DELETE_MSG_HEADER "X_OEAPI_DELETED"

//---------------------------------------------------------------------------//


/**
Get the window that matches the search recursively
*/
HWND FindWindowRecursive(HWND parent, TCHAR *className, DWORD processId = -1);

typedef struct AccountInfoOE__ {
	TCHAR accountId[1024];
	TCHAR accountName[1024];
} AccountInfoOE;

BOOL GetDefaultAccountSettingsOE(AccountInfoOE* accountInfo);

typedef struct AccountInfoWM__ {
	CHAR accountId[1024];
	WCHAR accountName[2048];
	WCHAR displayFrom[2048];
	CHAR sender[1024];
} AccountInfoWM;

/**
*/
BOOL GetDefaultAccountSettingsWM(AccountInfoWM* accountInfo);

/**
Returns TRUE if the process currently running is msimn.exe
*/
BOOL IsOE();

/**
Returns TRUE if OS is WMail
*/
BOOL IsWMail();

/**
Returns TRUE if the process currently running is iexplore.exe
*/
BOOL IsVistaIE();

/**
Copies a buffer inside WinMail.exe to a buffer in actual process.
Useful for standalone exes because early versions of WinMail reports a pointer to a 
buffer inside WinMail.exe
*/
// BOOL CopyFromWinMail(void *dest, const void *src, size_t dwBytes);

/**
Load a string from resources given its id
*/
std::basic_string<TCHAR> GetResourceString(HINSTANCE hInstance, int stringId);

//---------------------------------------------------------------------------//

typedef NKTComPtr<IStoreFolder, &IID_IStoreFolder> IStoreFolderPtr;
typedef NKTComPtr<IStoreNamespace, &IID_IStoreNamespace> IStoreNamespacePtr;

#endif // __OEAPI_UTILS_H
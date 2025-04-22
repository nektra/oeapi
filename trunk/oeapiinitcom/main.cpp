 /* $Id: oeapiinitcom.cpp,v 1.23 2008/09/07 16:56:33 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/


#include "std.h"

#include "OEAPIINITCOM.h"
#include "se_debug.h"
#include "oeapi_utils.h"
//#include "globals.h"
#include <oeapi_config.h>

using namespace comet;
using namespace OEAPIINITCOM;


volatile LONG g_objectCounter = 0;
HINSTANCE g_hOEAPIINITCOM = NULL;


typedef com_server<OEAPIINITCOM::type_library> SERVER;

///////////////////////////////////////////////////////////////////////////
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
    {
		g_hOEAPIINITCOM = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
    {
		if (g_objectCounter != 0)
        {
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



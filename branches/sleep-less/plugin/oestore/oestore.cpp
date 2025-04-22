/* $Id: oestore.cpp,v 1.5 2007/04/20 22:10:23 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"

using namespace comet;
using namespace OESTORE;

#include "manager.h"
#include "message.h"
#include "folder.h"
#include "manager.h"

#include "oeapi_utils.h"

typedef com_server< OESTORE::type_library > SERVER;

HINSTANCE hInst = NULL;

// Declare and implement DllMain, DllGetClassObject, DllRegisterServer,
// DllUnregisterServer and DllCanUnloadNow.
// These are implemented by simple forwarding onto a static method of
// the same name on SERVER:
// i.e. DllMain simply calls SERVER::DllMain.
//COMET_DECLARE_DLL_FUNCTIONS(SERVER)

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if(dwReason == DLL_PROCESS_ATTACH) {
		hInst = hInstance;
		// TODO: Check if we need to create a manager???
		//OEStoreManager::Get();
	}
	else if(dwReason == DLL_PROCESS_DETACH) {
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

/* $Id: oeidentity.cpp,v 1.5 2007/05/15 17:17:36 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"

#include "se_debug.h"
#include "nkt_registry.h"


bstr_t coclass_implementation<OEIdentity>::GetCurrentIdentity()
{
	NktRegistry reg;
	TCHAR buf[512];
	DWORD type, size;
	bstr_t identity;
	LONG ret;

	if(reg.Open(HKEY_CURRENT_USER, _T("Identities"), KEY_QUERY_VALUE) != ERROR_SUCCESS) {
		debug_print(DEBUG_ERROR, _T("GetCurrentIdentity: Error RegOpenKeyEx\n"));
	}
	else {
		size = sizeof(buf);
		ret = reg.QueryValue(_T("Last User ID"), &type, (BYTE*) buf, &size);
		if(ret == ERROR_SUCCESS) {
			identity = buf;
		}
	}

	return identity;
}

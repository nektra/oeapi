/* $Id: se_debug.cpp,v 1.1 2007/07/17 17:27:41 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include <windows.h>
#include <tchar.h>

#include <se_debug.h>

#include <stdio.h>
//#include <sstream>

int nkt_debug_level = 5;

//---------------------------------------------------------------------------//
void nkt_debug_print(int level, LPCTSTR format, ...)
{
	if(nkt_debug_level >= level)
	{
		va_list ap;
		TCHAR buffer[2048];
		va_start(ap, format);
#if _MSC_VER >= 1400
		_vsntprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, ap);
#else
		_vsntprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), format, ap);
#endif
		va_end(ap);
#ifdef OEAPI_DEBUG_PREFIX
		OutputDebugString(OEAPI_DEBUG_PREFIX);
#endif

		OutputDebugString(buffer);
	}
}


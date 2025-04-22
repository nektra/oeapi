// launcher.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <tchar.h>

#include "oeapi_utils.h"
#include "se_debug.h"


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	HMODULE hOehook = LoadLibrary("oehook.dll");
	if(hOehook == NULL) {
		debug_print(DEBUG_ERROR, _T("Launcher: Cannot load oehook.dll\n"));
		return 1;
	}

	MSG msg;

    while (GetMessage(&msg, (HWND) NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	return 0;
}

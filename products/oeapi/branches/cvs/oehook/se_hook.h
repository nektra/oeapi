#ifndef _SE_HOOK_
#define _SE_HOOK_

#define PLUGIN_PROP		_T("OEAPI Plugin")

INT SetHook (VOID) ;
VOID RemoveHook(VOID) ;

LRESULT WINAPI CBTProc (int nCode, WPARAM wParam, LPARAM lParam) ;

//#define OEAPI_TRIAL

#endif /* _SE_HOOK_ */

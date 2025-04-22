#include <windows.h>


typedef UINT (__stdcall *function_t)(HANDLE);

void main(void)
{
	HINSTANCE hInst = LoadLibrary("actions.dll");
	if(hInst) {
		function_t pClosePrograms, pDeleteKey;
		pClosePrograms = (function_t) GetProcAddress(hInst, "ClosePrograms");
		pDeleteKey = (function_t) GetProcAddress(hInst, "DeleteRegKey");
		pClosePrograms(0);
		pDeleteKey(0);
	}
}
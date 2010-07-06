#include <windows.h>
#include <tchar.h>

#include <time.h>

void main(void)
{
//	HKEY hKey;
//
//	DWORD secs, size, type;
//	time_t curTime, elapsedTime, oldCurTime;
//	TCHAR expMsg[1024];
//
//	time(&curTime);
//
//	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Nektra\\OEAPI\\Data"), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) {
//		if(RegCreateKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Nektra\\OEAPI\\Data"), &hKey) != ERROR_SUCCESS) {
//			return;
//		}
//
//		if(RegSetValueEx(hKey, _T("Flags"), 0, REG_DWORD, (CONST BYTE*) &curTime, sizeof(curTime)) != ERROR_SUCCESS) {
//			return;
//		}
//
//		secs = curTime;
//	}
//
//	RegCloseKey(hKey);

	HKEY hKey;

	DWORD secs, size, type;
	time_t curTime;
	TCHAR expMsg[1024];

	time(&curTime);

	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Nektra\\OEAPI\\Data2"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
		MessageBox(NULL, "You have already reset the evaluation period.", "OEAPI Error", MB_OK);
		return;
	}

	RegDeleteKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Nektra\\OEAPI\\Data"));
	if(RegCreateKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Nektra\\OEAPI\\Data2"), &hKey) != ERROR_SUCCESS) {
		MessageBox(NULL, "Error creating key.", "OEAPI Error", MB_OK);
	}
}

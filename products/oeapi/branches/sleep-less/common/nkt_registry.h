/* $Id: nkt_registry.h,v 1.6 2007/05/23 16:54:28 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2006-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __NKTREGISTRY_H__
#define __NKTREGISTRY_H__ 1


/**
Quick and dirty HKEY Helper class 
*/
class NktRegistry
{
public:
	NktRegistry(HKEY hKey = NULL):m_hKey(hKey) {}
	~NktRegistry() { Close(); }

	inline LONG Open(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired = KEY_READ) 
	{
		Close();
		return RegOpenKeyEx(hKey, lpSubKey, 0, samDesired, &m_hKey);
	}

	inline LONG Create(HKEY hKey, LPCTSTR lpSubKey, DWORD dwOptions = REG_OPTION_NON_VOLATILE, REGSAM samDesired = KEY_ALL_ACCESS, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, LPDWORD lpdwDisposition = NULL) 
	{
		return RegCreateKeyEx(hKey, lpSubKey, 0, NULL, dwOptions, samDesired, lpSecurityAttributes, &m_hKey, lpdwDisposition);
	}

	inline VOID Close() 
	{ 
		if(m_hKey) 
		{ 
			RegCloseKey(m_hKey); 
			m_hKey = NULL; 
		} 
	}

	const NktRegistry& operator = (HKEY hKey) {
		Close();
		m_hKey = hKey;
	}

	inline LONG SetValue(LPCTSTR lpValueName, DWORD dwType, LPCBYTE lpData, DWORD cbData) 
	{
		return RegSetValueEx(m_hKey, lpValueName, 0, dwType, lpData, cbData);
	}

	//inline LONG StoreString(LPCTSTR lpValueName, LPCTSTR lpData)
	//{
	//	DWORD dwSize = (_tcslen(lpData) + 1) * sizeof(TCHAR);
	//	return SetValue(lpValueName, REG_SZ, (LPCBYTE)lpData, dwSize);
	//}

	inline LONG QueryValue(LPCTSTR lpValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
	{
		return RegQueryValueEx(m_hKey, lpValueName, NULL, lpType, lpData, lpcbData);
	}

	//inline LONG QueryString(LPCTSTR lpValueName, LPCTSTR lpDefaultValue, LPTSTR lpData, DWORD cbData)
	//{
	//	DWORD dwSize = cbData;
	//	DWORD dwType;
	//	if(QueryValue(lpValueName, &dwType, (LPBYTE)lpData, &dwSize) != ERROR_SUCCESS)
	//	{
	//		_tcsncpy(lpData, lpDefaultValue, cbData);
	//		return cbData;
	//	}
	//	else
	//	{
	//		return dwSize;
	//	}
	//}

	inline LONG EnumValue(DWORD index, 
							LPTSTR lpValueName, 
							LPDWORD lpcValueName, 
							LPDWORD lpType, 
							LPBYTE lpData, 
							LPDWORD lpcbData) {
		return RegEnumValue(m_hKey, 
								index, 
								lpValueName, 
								lpcValueName, 
								NULL, // reserved
								lpType, 
								lpData, 
								lpcbData);
	}

	inline LONG EnumKey(DWORD index,
							LPTSTR lpName,
							LPDWORD lpcName,
							LPTSTR lpClass,
							LPDWORD lpcClass,
							PFILETIME lpLastWriteTime) {
		return RegEnumKeyEx(m_hKey, 
			index,
			lpName,
			lpcName,
			NULL, // reserved
			lpClass,
			lpcClass,
			lpLastWriteTime);
	}

	inline LONG QueryInfoKey(LPTSTR lpClass = NULL, 
								LPDWORD lpcClass = NULL, 
								LPDWORD lpcSubKeys = NULL, 
								LPDWORD lpcMaxSubKeyLen = NULL, 
								LPDWORD lpcMaxClassLen = NULL, 
								LPDWORD lpcValues = NULL, 
								LPDWORD lpcMaxValueNameLen = NULL, 
								LPDWORD lpcMaxValueLen = NULL, 
								LPDWORD lpcbSecurityDescriptor = NULL, 
								PFILETIME lpftLastWriteTime = NULL) {
		return RegQueryInfoKey(m_hKey,
								lpClass, 
								lpcClass, 
								NULL, // reserved
								lpcSubKeys, 
								lpcMaxSubKeyLen, 
								lpcMaxClassLen, 
								lpcValues, 
								lpcMaxValueNameLen, 
								lpcMaxValueLen, 
								lpcbSecurityDescriptor, 
								lpftLastWriteTime);
	}

	inline HKEY GetKey() const
	{ 
		return m_hKey; 
	}

	operator HKEY () const
	{
		return m_hKey;
	}

private:
	HKEY m_hKey;
};



#endif // __REGKEY_H__
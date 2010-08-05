/* $Id: oeapi_utils.cpp,v 1.27 2008/09/07 16:55:58 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "oeapi_config.h"
#include "oeapi_utils.h"

#include <tchar.h>
#include <sstream>

#include "nkt_registry.h"
#include "se_debug.h"

#include <xmllite.h>
#include <shlwapi.h>
#include <sstream>


typedef struct FindWndRecTag{
	DWORD processID;
	HWND winResult;
	LPCTSTR wndClass;
} FindWndRec;

//---------------------------------------------------------------------------//
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	FindWndRec* findWnd = (FindWndRec*)lParam;
	DWORD wndProcId;

	if(findWnd->processID != -1) {
		GetWindowThreadProcessId(hwnd, &wndProcId);
		if(wndProcId != findWnd->processID) {
			return TRUE;
		}
	}

	TCHAR wndClass[1024];
	GetClassName(hwnd, wndClass, sizeof(wndClass)/sizeof(TCHAR)-1);

	if(!_tcscmp(wndClass, findWnd->wndClass)) {
		findWnd->winResult = hwnd;
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
HWND FindWindowRecursive(HWND parent, TCHAR *className, DWORD processId)
{
	FindWndRec findWnd;
	if(parent == NULL) {
		parent = GetDesktopWindow();
	}

	findWnd.winResult = NULL;
	findWnd.processID = processId;
	findWnd.wndClass = className;

	EnumChildWindows(parent, EnumChildProc, (LPARAM)&findWnd);

	return findWnd.winResult;
}

//---------------------------------------------------------------------------//
BOOL GetDefaultAccountSettingsOE(AccountInfoOE* info)
{
	NktRegistry reg;
	BOOL ret = FALSE;
	BOOL found = FALSE;

	if(reg.Open(HKEY_CURRENT_USER, _T("Identities"), KEY_READ) == ERROR_SUCCESS)
	{
		TCHAR userId[1024];
		DWORD dwSize = sizeof(userId);
		DWORD dwType;
		LONG res = reg.QueryValue(_T("Last User ID"), &dwType, (LPBYTE)userId, &dwSize);
		reg.Close();
		if(res == ERROR_SUCCESS)
		{
			std::basic_stringstream<TCHAR> userKey;
			userKey << _T("Identities\\") << userId << _T("\\Software\\Microsoft\\Internet Account Manager");
			res = reg.Open(HKEY_CURRENT_USER, userKey.str().c_str(), KEY_READ);
			if(res == ERROR_SUCCESS)
			{
				found = TRUE;
			}
		}
	}

	if(!found)
	{
		if(reg.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Internet Account Manager"), KEY_READ) != ERROR_SUCCESS)
		{
			debug_print(DEBUG_INFO, _T("GetDefaultAccountSettings: Can't find default id.\n"));
			return ret;
		}
	}

	DWORD dwSize = sizeof(info->accountId);
	DWORD dwType;

	if(reg.QueryValue(_T("Default Mail Account"), &dwType, (LPBYTE)info->accountId, &dwSize) != ERROR_SUCCESS)
	{
		debug_print(DEBUG_INFO, _T("GetDefaultAccountSettings: Can't read account id.\n"));
		return ret;
	}
	std::basic_stringstream<TCHAR> account;
	account << _T("Accounts\\") << info->accountId;

	NktRegistry subKey;

	if(subKey.Open(reg, account.str().c_str(), KEY_ALL_ACCESS) != ERROR_SUCCESS)
	{
		debug_print(DEBUG_INFO, _T("GetDefaultAccountSettings: Can't open account.\n"));
		return ret;
	}

	dwSize = sizeof(info->accountName);

	if(subKey.QueryValue(_T("Account Name"), &dwType, (LPBYTE)info->accountName, &dwSize) != ERROR_SUCCESS)
	{
		debug_print(DEBUG_INFO, _T("GetDefaultAccountSettings: Can't read acount name.\n"));
		return ret;
	}

	ret = TRUE;

	return ret;
}

//---------------------------------------------------------------------------//
// TODO: Cleanup WinMail profile reading 
typedef HRESULT (STDAPICALLTYPE * PCREATEXMLREADER)(REFIID riid, void ** ppvObject, IMalloc * pMalloc);
typedef HRESULT (STDAPICALLTYPE * PSHCREATESTREAMONFILE)(LPCTSTR pszFile, DWORD grfMode, struct IStream **ppstm);


class WinMailProfileReader {
public:
	WinMailProfileReader() { }

	~WinMailProfileReader() {
		if(hXmlLite_) {
			::FreeLibrary(hXmlLite_);
			hXmlLite_ = 0;
		}
		if(hShlwapi_) {
			::FreeLibrary(hShlwapi_);
			hShlwapi_ = NULL;
		}
	}

	BOOL Init() {
		if(hXmlLite_ == NULL) {
			hXmlLite_ = ::LoadLibrary(_T("xmllite.dll"));
			if(hXmlLite_) {
				// GetProcAddress 2nd parameter is LPCSTR
				pCreateXmlReader_ = (PCREATEXMLREADER)GetProcAddress(hXmlLite_, "CreateXmlReader");
			}
		}
		if(hShlwapi_ == NULL) {
			hShlwapi_ = ::LoadLibrary(_T("shlwapi.dll"));
			if(hShlwapi_) {
#ifdef UNICODE
				pSHCreateStreamOnFile_ = (PSHCREATESTREAMONFILE)GetProcAddress(hShlwapi_, "SHCreateStreamOnFileW");
#else
				pSHCreateStreamOnFile_ = (PSHCREATESTREAMONFILE)GetProcAddress(hShlwapi_, "SHCreateStreamOnFileA");
#endif
			}
		}
		return (hShlwapi_ != NULL) && (hXmlLite_ != NULL);
	}

	HRESULT CreateXmlReader(REFIID riid, void ** ppvObject, IMalloc * pMalloc) {
		if(pCreateXmlReader_) {
			return pCreateXmlReader_(riid, ppvObject, pMalloc);
		}
		return E_FAIL;
	}

	HRESULT SHCreateStreamOnFile(LPCTSTR pszFile, DWORD grfMode, struct IStream **ppstm) {
		if(pSHCreateStreamOnFile_) {
			return pSHCreateStreamOnFile_(pszFile, grfMode, ppstm);
		}
		return E_FAIL;
	}

private:
	static HMODULE hXmlLite_;
	static PCREATEXMLREADER pCreateXmlReader_;
	static HMODULE hShlwapi_;
	static PSHCREATESTREAMONFILE pSHCreateStreamOnFile_;
};

HMODULE WinMailProfileReader::hXmlLite_ = NULL;
HMODULE WinMailProfileReader::hShlwapi_ = NULL;
PCREATEXMLREADER WinMailProfileReader::pCreateXmlReader_ = NULL;
PSHCREATESTREAMONFILE WinMailProfileReader::pSHCreateStreamOnFile_ = NULL;

WinMailProfileReader profileReader;

//---------------------------------------------------------------------------//
BOOL GetDefaultAccountSettingsWM(AccountInfoWM* info)
{
	if(!profileReader.Init()) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: Load libraries failed %d.\n"), ::GetLastError());
		return FALSE;
	}

	NktRegistry reg;
	if(reg.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\IAM"), KEY_READ) != ERROR_SUCCESS) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: Can't open account registry %d.\n"), ::GetLastError());
		return FALSE;
	}

	DWORD dwType;
	DWORD dwSize = sizeof(info->accountId);

	if(reg.QueryValue(_T("Default Mail Account"), &dwType, (LPBYTE)info->accountId, &dwSize) != ERROR_SUCCESS) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: Can't read account registry %d.\n"), ::GetLastError());
		return FALSE;
	}

	if(reg.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows Mail"), KEY_READ) != ERROR_SUCCESS) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: Can't open store registry %d.\n"), ::GetLastError());
		return FALSE;
	}

	TCHAR storeRoot[1024];
	dwSize = sizeof(storeRoot);
	if(reg.QueryValue(_T("Store Root"), &dwType, (LPBYTE)storeRoot, &dwSize) != ERROR_SUCCESS) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: Can't read account registry %d.\n"), ::GetLastError());
		return FALSE;
	}

	std::basic_string<TCHAR> fullPath;
	TCHAR tmp[2048];
	if(dwType == REG_EXPAND_SZ) {
		if(::ExpandEnvironmentStrings(storeRoot, tmp, sizeof(tmp)/sizeof(TCHAR)) == 0) {
			debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: Expand string failed %d.\n"), ::GetLastError());
			return FALSE;
		}
		fullPath += tmp;
	}
	else {
		//_tcscpy_s(fullPath, sizeof(fullPath), storeRoot);
		fullPath += storeRoot;
	}

	//_tcscat_s(fullPath, _T("Local Folders\\"));
	fullPath += _T("Local Folders\\");
	//_tcscat_s(fullPath, info->accountId);
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, info->accountId, -1, tmp, sizeof(tmp)/sizeof(TCHAR));
	fullPath += tmp; // .append(info->accountId);
#else
	fullPath += info->accountId;
#endif

	IStream* pStream = NULL;
	HRESULT hr;

	hr = profileReader.SHCreateStreamOnFile(fullPath.c_str(), STGM_READ, &pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: SHCreateStreamOnFile failed %08x.\n"), hr);
		return FALSE;
	}

	IXmlReader* pReader = NULL;
	hr = profileReader.CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: CreateXmlReader failed %08x.\n"), hr);
		return FALSE;
	}

	hr = pReader->SetInput(pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: SetInput failed %08x.\n"), hr);
		return FALSE;
	}

	XmlNodeType nodeType;
    const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;

	typedef enum ElementType__ {
		ElementNone = 0,
		ElementAccountName = 1,
		ElementDisplayFrom = 2,
		ElementSender = 3,
	} ElementType;

	ElementType element = ElementNone;

	hr = pReader->Read(&nodeType);
	while (S_OK == hr) {
		switch (nodeType)
        {
			case XmlNodeType_Element:
				hr = pReader->GetLocalName(&pwszLocalName, NULL);
				if(FAILED(hr)) {
					element = ElementNone;
					debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: GetLocalName failed %08x.\n"), hr);
					break;
				}
				if(wcscmp(pwszLocalName, L"Account_Name") == 0) {
					element = ElementAccountName;
					break;
				} else if(wcscmp(pwszLocalName, L"SMTP_Display_Name") == 0) {
					element = ElementDisplayFrom;
					break;
				} else if(wcscmp(pwszLocalName, L"SMTP_Email_Address") == 0) {
					element = ElementSender;
					break;
				}
				break;
			case XmlNodeType_EndElement:
				element = ElementNone;
				break;
			case XmlNodeType_Text:
				hr = pReader->GetValue(&pwszValue, NULL);
				if(FAILED(hr)) {
					debug_print(DEBUG_ERROR, _T("GetDefaultAccountSettingsWM: GetValue failed %08x.\n"), hr);
					break;
				}
				switch(element) {
					case ElementAccountName:
						wcscpy_s(info->accountName, pwszValue);
						break;
					case ElementDisplayFrom:
						wcscpy_s(info->displayFrom, pwszValue);
						break;
					case ElementSender:
						::WideCharToMultiByte(CP_ACP, 0, pwszValue, -1, info->sender, sizeof(info->sender), NULL, NULL);
						break;
				}
				break;
		}

		hr = pReader->Read(&nodeType);
	}

	pReader->Release();
	pStream->Release();

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL IsOE()
{
	static BOOL _Initialized = 0;	// 0 unitialized
									// 1 not oe/wm
									// 2 oe/wm
	if(_Initialized == 0) {
		_Initialized = 1;
		TCHAR szBuf[2048];
		// double verification: verify that the exe is OE
		if(GetModuleFileName(NULL, szBuf, sizeof(szBuf)/sizeof(TCHAR)-sizeof(TCHAR)) == 0) {
			debug_print(DEBUG_ERROR, _T("IsOE: Error GetModuleFileName %d.\n"), ::GetLastError());
			_Initialized = 0;
			return FALSE;
		}
		else {
			TCHAR *lastPart;
			lastPart = _tcsrchr(szBuf, _T('\\'));
			if(lastPart++ != NULL) {
				if(_tcsicmp(lastPart, _T("msimn.exe")) && _tcsicmp(lastPart, _T("msimn")) &&
					_tcsicmp(lastPart, _T("winmail.exe")) && _tcsicmp(lastPart, _T("winmail"))) {
					return FALSE;
				}
			}
		}
		_Initialized = 2;	// IsOE == TRUE
	}

	return _Initialized == 2;
}

//---------------------------------------------------------------------------//
// TODO: Fix to really detect WinMail
BOOL FlagSet = FALSE;
BOOL bIsWMail = FALSE;

BOOL IsWMail()
{
	if(!FlagSet) {
		OSVERSIONINFO osvi;

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		if(GetVersionEx(&osvi)) {
			bIsWMail = (osvi.dwMajorVersion == 6);
			FlagSet = TRUE;
		}
		else {
			debug_print(DEBUG_ERROR, _T("IsWMail: GetVersionEx Error.\n"));
		}
	}

	return bIsWMail;
}

//---------------------------------------------------------------------------//
// Note: If UAC is enabled then we must load on IE else it hang
BOOL IsVistaIE()
{
	static BOOL _Initialized = 0;	// 0 unitialized
									// 1 not ie
									// 2 ie
	if(_Initialized == 0) {
		_Initialized = 1;
		TCHAR szBuf[2048];
		if(GetModuleFileName(NULL, szBuf, sizeof(szBuf)/sizeof(TCHAR)-sizeof(TCHAR)) == 0) {
			debug_print(DEBUG_ERROR, _T("IsIE: Error GetModuleFileName %d.\n"), ::GetLastError());
			_Initialized = 0;
			return FALSE;
		}
		else {
			TCHAR *lastPart;
			lastPart = _tcsrchr(szBuf, _T('\\'));
			if(lastPart++ != NULL) {
				if(_tcsicmp(lastPart, _T("iexplore.exe")) && _tcsicmp(lastPart, _T("iexplore"))) {
					return FALSE;
				}
			}
		}

		OSVERSIONINFO osvi;

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		if(!GetVersionEx(&osvi)) {
			debug_print(DEBUG_ERROR, _T("IsWMail: GetVersionEx Error.\n"));
			return FALSE;
		}
		
		if(osvi.dwMajorVersion != 6) {
			return FALSE;
		};

		_Initialized = 2;	// IsVistaIE == TRUE
	}

	return _Initialized == 2;
}


//---------------------------------------------------------------------------//
std::basic_string<TCHAR> GetResourceString(HINSTANCE hInstance, int stringId)
{
	TCHAR buffer[2048];
	int count = LoadString(hInstance, stringId, buffer, sizeof(buffer)/sizeof(buffer[0]));
	if(count == 0)
	{
		debug_print(DEBUG_INFO, _T("GetResourceString: Error loading string %d - %d\n"), stringId, GetLastError());
	}
	return buffer;
}

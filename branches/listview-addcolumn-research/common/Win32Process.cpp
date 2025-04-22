// Win32Process.cpp: implementation of the Win32Process class.
//
//////////////////////////////////////////////////////////////////////

/******************************************************************************\
		Win32Process: a Win95/NT-compatible class for acquiring the list
		of currently active processes.
		Copyright (C) 1998 Tomer Petel
		You may modify and/or integrate this code into your commercial software
		for free in exchange for a small e-mail, just so I can know
		if this code ever became useful to someone else but me.
		email to: tomerp@eng2.uconn.edu
\******************************************************************************/

#include <windows.h>
#include <tchar.h>
#include "Win32Process.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef BOOL (__stdcall *EnumProcessModules_t)(HANDLE,HMODULE *,DWORD,LPDWORD);
typedef DWORD (__stdcall *GetModuleBaseName_t)(HANDLE,HMODULE,LPTSTR,DWORD);
typedef BOOL (__stdcall *EnumProcesses_t)(DWORD *,DWORD,DWORD *);

EnumProcessModules_t pEnumProcessModules = 0;
GetModuleBaseName_t pGetModuleBaseName = 0;
EnumProcesses_t pEnumProcesses = 0;

bool GetProcessName( DWORD processID, char *procName, int maxLen )
{
    char szProcessName[MAX_PATH] = TEXT("<unknown>");
	bool ret = false;

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( pEnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            pGetModuleBaseName( hProcess, hMod, procName,
                               maxLen );

			ret = true;
        }

	    CloseHandle( hProcess );
    }

	return ret;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Win32Process::Win32Process()
{
	m_p_fnProcess32Next=NULL;
	m_p_fnProcess32First=NULL;
	m_p_fnCreateToolhelp32Snapshot=NULL;
	m_p_fnThread32First = NULL;
	m_p_fnThread32Next = NULL;
	m_hSnapshot = INVALID_HANDLE_VALUE;

	Init();
}

Win32Process::~Win32Process()
{
}

bool Win32Process::Init()
{
	DWORD dwVersion = ::GetVersion();

   // Windows NT?
   if(dwVersion < 0x80000000)
       // Yes, Windows NT
	   m_bIsNt=true;
    // Windows 95?
   else if(LOBYTE(LOWORD(dwVersion)) >= 4)
	   m_bIsNt=false;
   else
   {
	   m_strLastError="Unsupported OS version";
	   return false;  // Win16, 32s or NT3.x.
   }
   ////if its Win95, we must get the appropriate function pointers...
   ////This esnures that we dont tie up to a specific Compiler option, and that we can run on
   ////both 95 or NT (because NT does not have the following procedures in its kernel32.dll)

	if (m_bIsNt)
	{
		HMODULE pPsapi = LoadLibrary("psapi.dll");
		if( pPsapi ) {
			pEnumProcessModules = (EnumProcessModules_t) GetProcAddress(pPsapi, "EnumProcessModules");
			pGetModuleBaseName = (GetModuleBaseName_t) GetProcAddress(pPsapi, "GetModuleBaseNameA");
			pEnumProcesses = (EnumProcesses_t) GetProcAddress(pPsapi, "EnumProcesses");
		}

	}
	else
	{
		//// If here, We are running on Win95. Must use the tool help functions....

		m_hWin95Kernel=GetModuleHandle("kernel32.dll");
		if(!m_hWin95Kernel)
		{
			m_strLastError="Could not get kernel32 handle";
			return false;
		}
		m_p_fnCreateToolhelp32Snapshot=(PFN_CREATETHELP32SNPSHT)GetProcAddress(m_hWin95Kernel,"CreateToolhelp32Snapshot");
		if (!m_p_fnCreateToolhelp32Snapshot)
		{
			m_strLastError="Could not get CreateToolhelp32Snapshot pointer";
			return false;
		}

		m_p_fnProcess32First=(PFN_PROCESS32FIRST)GetProcAddress(m_hWin95Kernel,"Process32First");
		if (!m_p_fnProcess32First)
		{
			m_strLastError="Could not get Process32First pointer";
			return false;
		}

		m_p_fnProcess32Next=(PFN_PROCESS32NEXT)GetProcAddress(m_hWin95Kernel,"Process32Next");
		if (!m_p_fnProcess32Next)
		{
			m_strLastError="Could not get Process32Next pointer";
			return false;
		}

		m_p_fnThread32First=(PFN_THREAD32FIRST)GetProcAddress(m_hWin95Kernel,"Thread32First");
		if (!m_p_fnThread32First)
		{
			m_strLastError="Could not get Thread32First pointer";
			return false;
		}

		m_p_fnThread32Next=(PFN_THREAD32NEXT)GetProcAddress(m_hWin95Kernel,"Thread32Next");
		if (!m_p_fnThread32Next)
		{
			m_strLastError="Could not get Thread32Next pointer";
			return false;
		}

	}
	return true;

}

int Win32Process::FindProcess(const std::basic_string<TCHAR>& ProcessName)
{
	if( m_bIsNt ) {
		return FindProcessNT(ProcessName);
	}
	else {
		return FindProcess95(ProcessName);
	}
}

int Win32Process::FindProcessNT(const std::basic_string<TCHAR>& ProcessName)
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
	char procName[1024];

	if( pEnumProcessModules == 0 || pGetModuleBaseName == 0 || pEnumProcesses == 0 ) {
		return 0;
	}

    if ( !pEnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return 0;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

	if(cProcesses > sizeof(aProcesses)/sizeof(aProcesses[0])) {
		cProcesses = sizeof(aProcesses)/sizeof(aProcesses[0]);
	}

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ ) {
        if( GetProcessName( aProcesses[i], procName, sizeof(procName)) ) {
			if( !_tcsicmp(ExtractProcessName(procName, sizeof(procName)), ProcessName.c_str())) {
				return aProcesses[i];
			}
		}
	}

	return 0;
}


int Win32Process::FindProcess95(const std::basic_string<TCHAR>& ProcessName)
{
	///This function gets all the running process, the win95 way, using the
	///ToolHelp functions. If you simply are checking if a process is active
	///pass its name as a parameter and make the second parameter false.
	///Otherwise, the function will ignore the ProcessName param and add to the array
	///all the currently active processes...


	m_bActive=false;

	HANDLE handle;
	handle=m_p_fnCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (handle == (HANDLE)-1)
	{
		m_strLastError="Failed in creating snapshot";
		return false;
	}

	PROCESSENTRY32 process;
	process.dwSize=sizeof(PROCESSENTRY32);

	BOOL bContinue=m_p_fnProcess32First(handle,&process);

	while(bContinue)
	{
		///if here, then we are just looking to see if a certain process is alive...
		if (!_tcsicmp(ProcessName.c_str(), ExtractProcessName(process.szExeFile, sizeof(process.szExeFile))))
		{
			///found the process, it is active....
			CloseHandle(handle);
			m_bActive=true;
			return process.th32ProcessID;
		}
		bContinue=m_p_fnProcess32Next(handle,&process);
	}

	CloseHandle(handle);
	return 0;
}

bool Win32Process::IsWinNT()
{
	return m_bIsNt;
}

char* Win32Process::ExtractProcessName(char *path, int size)
{

	//gets the process name out of the whole path name and makes it lower case.
	//this is only necessary in WIN95 since the ToolHelp functions return
	//the entire path of the executable, in upper case...

	char *p, *tmp=strrchr (path, '\\');
	if (tmp)
	{
		tmp++; //get after the backslash
		p=strrchr (tmp, '.');
		*p=0;				  //get rid of the .exe extension
#if _MSC_VER >= 1400
		_tcslwr_s(tmp, size + path - tmp); //make it lower case and return it.
		return tmp;
#else
		return _tcslwr(tmp); //make it lower case and return it.
#endif
	}
	else	 //couldnt find the backslash: just deal with the .exe:
	{
		p=strrchr (path, '.');
		*p=0;				  //get rid of the .exe extension
#if _MSC_VER >= 1400
		_tcslwr_s(path, size); //make it lower case and return it.
		return path;
#else
		return _tcslwr(path); //make it lower case and return it.
#endif
	}

	return path;
}

DWORD Win32Process::GetFirstThreadId(DWORD procId)
{
	m_enumProcId = procId;

	if(m_bIsNt) {
		return -1;
	}
	else {
		return GetThreadId();
	}
}

DWORD Win32Process::GetNextThreadId()
{
	if(m_bIsNt) {
		return -1;
	}
	else {
		return GetThreadId();
	}
}

DWORD Win32Process::GetThreadId()
{
	THREADENTRY32 thEntry;
	DWORD ret = -1;

	thEntry.dwSize = sizeof(THREADENTRY32);

	if(m_hSnapshot == INVALID_HANDLE_VALUE) {
		m_hSnapshot = m_p_fnCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		if(m_hSnapshot == INVALID_HANDLE_VALUE) {
			m_strLastError="Failed in creating snapshot";
			return ret;
		}

		if(m_p_fnThread32First(m_hSnapshot, &thEntry)) {
			ret = thEntry.th32ThreadID;
		}
	}

	while(ret != -1 && m_hSnapshot != INVALID_HANDLE_VALUE) {
		if(m_p_fnThread32Next(m_hSnapshot, &thEntry)) {
			if(thEntry.th32OwnerProcessID == m_enumProcId) {
				ret = thEntry.th32ThreadID;
			}
		}
		else {
			// enumeration finished
			CloseThreadEnum();
		}
	}

	return ret;
}

void Win32Process::CloseThreadEnum()
{
	if(m_hSnapshot != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hSnapshot);
		m_hSnapshot = INVALID_HANDLE_VALUE;
	}
}

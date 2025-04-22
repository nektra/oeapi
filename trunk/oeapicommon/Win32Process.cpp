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
#include <Psapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*typedef BOOL (__stdcall *EnumProcessModules_t)(HANDLE,HMODULE *,DWORD,LPDWORD);
typedef DWORD (__stdcall *GetModuleBaseName_t)(HANDLE,HMODULE,LPCSTR,DWORD);
typedef BOOL (__stdcall *EnumProcesses_t)(DWORD *,DWORD,DWORD *);*/

/*EnumProcessModules_t pEnumProcessModules = 0;
GetModuleBaseName_t pGetModuleBaseName = 0;
EnumProcesses_t pEnumProcesses = 0;*/

bool GetProcessName( DWORD processID, LPTSTR procName, int maxLen )
{
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

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, procName,
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

}

Win32Process::~Win32Process()
{
}

int Win32Process::FindProcess(const std::basic_string<TCHAR> & ProcessName)
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
	TCHAR procName[1024];


    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return 0;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

	if(cProcesses > sizeof(aProcesses)/sizeof(aProcesses[0])) {
		cProcesses = sizeof(aProcesses)/sizeof(aProcesses[0]);
	}

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
    {
        if( GetProcessName( aProcesses[i], procName, sizeof(procName)/sizeof(procName[0])) )
        {
			if( !_tcsicmp(ExtractProcessName(procName, sizeof(procName)/sizeof(procName[0])), ProcessName.c_str()))
            {
				return aProcesses[i];
			}
		}
	}

	return 0;
}


LPTSTR Win32Process::ExtractProcessName(LPTSTR path, int size)
{

	//gets the process name out of the whole path name and makes it lower case.
	//this is only necessary in WIN95 since the ToolHelp functions return
	//the entire path of the executable, in upper case...

	LPTSTR p;
    LPTSTR tmp = _tcsrchr(path, '\\');
	if (tmp)
	{
		tmp++; //get after the backslash
		p = _tcsrchr (tmp, '.');
		*p = 0;				  //get rid of the .exe extension

		_tcslwr_s(tmp, size + path - tmp); //make it lower case and return it.
		return tmp;

	}
	else	 //couldnt find the backslash: just deal with the .exe:
	{
		p = _tcsrchr (path, '.');
		*p=0;				  //get rid of the .exe extension
		_tcslwr_s(path, size); //make it lower case and return it.
	}

	return path;
}

DWORD Win32Process::GetFirstThreadId(DWORD procId)
{
	m_enumProcId = procId;

    return GetThreadId();

}

DWORD Win32Process::GetNextThreadId()
{
	return GetThreadId();
}

DWORD Win32Process::GetThreadId()
{
	THREADENTRY32 thEntry;
	DWORD ret = (DWORD)-1;

	thEntry.dwSize = sizeof(THREADENTRY32);

	if(m_hSnapshot == INVALID_HANDLE_VALUE) {
		m_hSnapshot = m_p_fnCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		if(m_hSnapshot == INVALID_HANDLE_VALUE) {
			m_strLastError = _T("Failed in creating snapshot");
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

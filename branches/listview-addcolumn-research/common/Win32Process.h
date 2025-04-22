// Win32Process.h
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

#ifndef WIN32_PROCESS_H
#define WIN32_PROCESS_H

#pragma warning(disable:4786)

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "winperf.h"
#include <tlhelp32.h>

#include <string>
#include <list>

typedef HANDLE (WINAPI *PFN_CREATETHELP32SNPSHT)(DWORD,DWORD);
typedef BOOL (WINAPI *PFN_PROCESS32FIRST)(HANDLE,PROCESSENTRY32*);
typedef BOOL (WINAPI *PFN_PROCESS32NEXT)(HANDLE,PROCESSENTRY32*);
typedef BOOL (WINAPI *PFN_THREAD32FIRST)(HANDLE,LPTHREADENTRY32);
typedef BOOL (WINAPI *PFN_THREAD32NEXT)(HANDLE,LPTHREADENTRY32);

class Win32Process
{
public:
	bool IsWinNT();
	std::string GetLastError();
	bool EnumAllProcesses();
	bool Init();
	Win32Process();
	virtual ~Win32Process();

	int FindProcess(const std::string& ProcessName);

	/**
	These functions work in 2K / XP / 9x
	*/
	DWORD GetFirstThreadId(DWORD processId);
	DWORD GetNextThreadId();

	void CloseThreadEnum();

protected:
	int FindProcess95(const std::string& ProcessName);
	int FindProcessNT(const std::string& ProcessName);

	DWORD GetThreadId();

	char *ExtractProcessName(char *path, int size);
	std::list<std::string> m_strArray;
	bool m_bActive;
	HMODULE m_hWin95Kernel;
	PFN_PROCESS32NEXT m_p_fnProcess32Next;
	PFN_PROCESS32FIRST m_p_fnProcess32First;
	PFN_THREAD32FIRST m_p_fnThread32First;
	PFN_THREAD32NEXT m_p_fnThread32Next;
	PFN_CREATETHELP32SNPSHT m_p_fnCreateToolhelp32Snapshot;
	std::string m_strLastError;
	bool m_bIsNt;

	DWORD m_enumProcId;
	HANDLE m_hSnapshot;
};

#endif // WIN32_PROCESS_H

#include "NktMutex.h"
#include <tchar.h>
#include <Windows.h>
#include <se_debug.h>

NktMutex::NktMutex(LPCTSTR lpName)
{
    _hMutex = CreateMutex(NULL, FALSE, lpName);
    if(_hMutex == NULL)
    {
        debug_print(DEBUG_ERROR, _T("NktMutex::NktMutex: Error CreateMutex\n"));
    }
    else
    {
        WaitForSingleObject(_hMutex, INFINITE);
    }
}

NktMutex::~NktMutex()
{
    if(_hMutex)
    {
        ReleaseMutex(_hMutex);
    }
}

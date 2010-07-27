#pragma once

#include <tchar.h>
#include <Windows.h>

class NktMutex
{
    public:
        NktMutex(LPCTSTR lpName);
        ~NktMutex();
    private:
        HANDLE _hMutex;
};
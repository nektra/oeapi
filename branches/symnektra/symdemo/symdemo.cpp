// oeapi_sample.cpp : Implementation of DLL Exports.

#include "CSymDemo.h"

CSymDemo symdemo;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}

extern "C" BOOL WINAPI InitializePlugin()
{
    symdemo.InitializePlugin();
    return TRUE;
}


//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>

extern "C" HRESULT __stdcall GetCoreInstance(REFGUID objid, REFGUID key, void** ppObj);

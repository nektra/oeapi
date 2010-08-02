#pragma once

#include <Windows.h>

extern "C" HRESULT __stdcall GetCoreInstance(REFGUID clsid, REFGUID key, void** ppObj);

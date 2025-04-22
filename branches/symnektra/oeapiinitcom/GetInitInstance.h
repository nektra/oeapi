#pragma once

#include <Windows.h>

extern "C" HRESULT __stdcall GetInitInstance(REFGUID clsid, REFGUID key, void** ppObj);

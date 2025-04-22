#pragma once

#include <Windows.h>

extern "C" HRESULT __stdcall GetStoreInstance(REFGUID clsid, REFGUID key, void** ppObj);

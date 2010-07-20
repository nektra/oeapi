#pragma once

#include <Windows.h>

extern "C" HRESULT __stdcall GetStoreInstance(REFGUID objid, REFGUID key, void** ppObj);

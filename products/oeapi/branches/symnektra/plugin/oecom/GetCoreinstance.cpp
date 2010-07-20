#include "GetCoreInstance.h"
#include <Windows.h>
#include "oeapi.h"

using namespace comet;
using namespace OEAPI;

HRESULT __stdcall GetCoreInstance(REFGUID objid, REFGUID key, void** ppObj)
{
    if (ppObj == NULL) return E_POINTER;

    if (objid == comtype<IOEAPIObj>::uuid())
    {
        *ppObj = new coclass_implementation<OEAPIObj>();
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
    return E_FAIL;
}
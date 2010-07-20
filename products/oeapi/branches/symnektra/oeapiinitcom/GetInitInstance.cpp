#include "GetInitInstance.h"
#include <Windows.h>
#include "OEAPIINITCOM.h"

using namespace comet;
using namespace OEAPIINITCOM;

HRESULT __stdcall GetInitInstance(REFGUID objid, REFGUID key, void** ppObj)
{
    if (ppObj == NULL) return E_POINTER;

    if (objid == comtype<IOEAPIInit>::uuid())
    {
        *ppObj = new coclass_implementation<OEAPIInit>();
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
    return E_FAIL;
}


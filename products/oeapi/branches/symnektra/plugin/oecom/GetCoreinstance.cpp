#include "GetCoreInstance.h"
#include <Windows.h>
#include "oeapi.h"
#include "oeapiobj.h"

using namespace comet;
using namespace OEAPI;

HRESULT __stdcall GetCoreInstance(REFGUID clsid, REFGUID key, void** ppObj)
{
    if (ppObj == NULL) return E_POINTER;

    if (clsid == uuidof<OEAPIObj>())
    {
        TOEAPIObj* ret = new TOEAPIObj;
        ret->AddRef();
        *ppObj = ret;
        return (ret == NULL) ? E_FAIL : S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
 
    return E_FAIL;
}
#include "GetInitInstance.h"
#include <Windows.h>
#include "OEAPIINITCOM.h"
#include "OeApiInitState.h"
#include "OeApiInit.h"
#include "oeidentity.h"
#include <NktApiKey.h>

using namespace comet;
using namespace OEAPIINITCOM;

HRESULT __stdcall GetInitInstance(REFGUID clsid, REFGUID key, void** ppObj)
{
    if (ppObj == NULL) return E_POINTER;

    if (GetApiKey(clsid) != key) return E_NOINTERFACE;

    if (clsid == uuidof<OEAPIInit>())
    {
        TOEAPIInit* ret = new TOEAPIInit;
        ret->AddRef();
        *ppObj = ret;
        return (ret == NULL) ? E_FAIL : S_OK;
    }
    else if (clsid == uuidof<OEAPIInitState>())
    {
        TOEAPIInitState* ret =  new TOEAPIInitState;
        ret->AddRef();
        *ppObj = ret;
        return (ret == NULL) ? E_FAIL : S_OK;
    }
    else if (clsid == uuidof<OEIdentity>())
    {
        TOeIdentity* ret =  new TOeIdentity;
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


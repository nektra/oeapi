#include "GetStoreInstance.h"
#include <Windows.h>
#include "OESTORE.h"

using namespace comet;
using namespace OESTORE;

HRESULT __stdcall GetStoreInstance(REFGUID objid, REFGUID key, void** ppObj)
{
    if (ppObj == NULL) return E_POINTER;

    if (objid == comtype<IOEFolderManager>::uuid())
    {
        *ppObj = new coclass_implementation<OEFolderManager>();
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
    return E_FAIL;
}
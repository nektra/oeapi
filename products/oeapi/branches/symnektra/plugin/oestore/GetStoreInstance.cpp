#include "std.h"

#include "GetStoreInstance.h"
#include <Windows.h>
#include "OESTORE.h"
#include "folder.h"

using namespace comet;
using namespace OESTORE;

HRESULT __stdcall GetStoreInstance(REFGUID clsid, REFGUID key, void** ppObj)
{
    if (ppObj == NULL) return E_POINTER;

    if (clsid == uuidof<OEFolderManager>())
    {
        TOEFolderManager* ret = new TOEFolderManager;
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
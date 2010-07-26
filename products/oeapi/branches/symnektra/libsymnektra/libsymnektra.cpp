#include "libsymnektra.h"

#include <OESTORE.h>
#include <OEAPIINITCOM.h>
#include <oeapi.h>

#include <GetStoreInstance.h>
#include <GetInitInstance.h>
#include <GetCoreInstance.h>

#include <tchar.h>

using namespace comet;
using namespace OEAPIINITCOM;
using namespace OESTORE;

GUID GetApiKey(REFGUID clsid)
{
    return clsid;
}

HRESULT SNGetInit(void** obj)
{
    return GetInitInstance(uuidof<OEAPIInit>(),GetApiKey(uuidof<OEAPIInit>()),obj);
}

HRESULT SNGetInitState(void** obj)
{
    return GetInitInstance(uuidof<OEAPIInitState>(),GetApiKey(uuidof<OEAPIInitState>()),obj);
}

HRESULT SNGetIdentity(void** obj)
{
    return GetInitInstance(uuidof<OEIdentity>(),GetApiKey(uuidof<OEIdentity>()),obj);
}

HRESULT SNGetApiCore(void** obj)
{
    return GetCoreInstance(uuidof<OEAPIInit>(),GetApiKey(uuidof<OEAPIInit>()),obj);
}

HRESULT SNGetFolderManager(void** obj)
{
    return GetStoreInstance(comtype<IOEFolderManager>::uuid(),comtype<IOEFolderManager>::uuid(),obj);
}


#include "libsymnektra.h"

#include <OESTORE.h>
#include <OEAPIINITCOM.h>
#include <OEAPI.h>

#include <GetStoreInstance.h>
#include <GetInitInstance.h>
#include <GetCoreInstance.h>

#include <NktApiKey.h>

using namespace comet;
using namespace OEAPIINITCOM;
using namespace OESTORE;
using namespace OEAPI;


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
    return GetCoreInstance(uuidof<OEAPIObj>(),GetApiKey(uuidof<OEAPIObj>()),obj);
}

HRESULT SNGetFolderManager(void** obj)
{
    return GetStoreInstance(uuidof<OEFolderManager>(),uuidof<OEFolderManager>(),obj);
}


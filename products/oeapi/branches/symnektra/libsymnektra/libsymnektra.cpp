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

void SNGetInit(void** obj)
{
    GetInitInstance(comtype<IOEAPIInit>::uuid(),comtype<IOEAPIInit>::uuid(),obj);
}

void SNGetApiCore(void** obj)
{
    GetCoreInstance(comtype<IOEAPIInit>::uuid(),comtype<IOEAPIInit>::uuid(),obj);
}

void SNGetFolderManager(void** obj)
{
    GetStoreInstance(comtype<IOEFolderManager>::uuid(),comtype<IOEFolderManager>::uuid(),obj);
}


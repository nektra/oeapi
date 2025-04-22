#include "OeApiInitState.h"
#include <oeapi_config.h>
#include "NktMutex.h"
#include <se_debug.h>

#include "globals.h"


TOEAPIInitState::coclass_implementation()
{

}

TOEAPIInitState::~coclass_implementation()
{

}

TCHAR *TOEAPIInitState::get_progid()
{
    return OEAPI_OEAPIINITSTATE_PROGID;
}

int TOEAPIInitState::GetOEAPIInitObjectsCount()
{
    int count = 0;

    NktMutex mutex(OEAPI_INITCOM_MUTEX);

    // verify that the windows exist.
    for(int i=0; i < g_hwndCount; i++)
    {
        if(IsWindow((HWND) g_hwndServerArray[i])) {
            count++;
        }
        else {
            debug_print(DEBUG_ERROR, _T("GetActiveObjectsCount: Window Deleted.\n"));
        }
    }

    return count;
}

int TOEAPIInitState::GetOEAPIInitInitializedObjectsCount()
{
    return g_initializedObjects;
}

void TOEAPIInitState::ResetOEAPIInitInitializedObjectsCount()
{
    g_initializedObjects = 0;
}

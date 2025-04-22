#pragma once

#include "OEAPIINITCOM.h"

#define TOEAPIInitState coclass_implementation<comet::OEAPIINITCOM::OEAPIInitState>

template<> class TOEAPIInitState : public comet::coclass<comet::OEAPIINITCOM::OEAPIInitState>
{
    public:
        TOEAPIInitState();
        ~TOEAPIInitState();
        static TCHAR *get_progid();
        int GetOEAPIInitObjectsCount();
        int GetOEAPIInitInitializedObjectsCount();
        void ResetOEAPIInitInitializedObjectsCount();
};
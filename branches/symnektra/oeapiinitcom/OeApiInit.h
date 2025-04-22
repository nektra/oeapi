#pragma once

#include <tchar.h>
#include <Windows.h>
#include "OEAPIINITCOM.h"

///////////////////////////////////////////////////////////////////

/**
This object implements the OEAPIInit. It has 2 events: one when the OEAPI inits and the
other when it shutdown.
The OEAPI set the events that this object handles in the ServerProc.
*/
#define TOEAPIInit coclass_implementation<comet::OEAPIINITCOM::OEAPIInit>

template<> class TOEAPIInit : public comet::coclass<comet::OEAPIINITCOM::OEAPIInit>
{
    public:
	    TOEAPIInit();
	    ~TOEAPIInit();

	    static TCHAR *get_progid();

        BOOL IsOEAPIActive();
	    void HelloWorld();
	    void TriggerOnInitOEAPI();
	    void FireOnInitOEAPI();
	    void TriggerOnShutdownOEAPI();
	    void FireOnShutdownOEAPI();

    private:
	    HANDLE hThread_;
	    HWND hwnd_;
	    BOOL active_;
};
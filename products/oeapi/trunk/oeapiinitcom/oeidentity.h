#pragma once

#include "OEAPIINITCOM.h"

using namespace comet;
using namespace OEAPIINITCOM;

#define TOeIdentity coclass_implementation<OEIdentity>

template<> class TOeIdentity : public coclass<OEIdentity>
{
public:
	TOeIdentity() {}
	~TOeIdentity() {}

	static TCHAR *get_progid() { return _T("OEAPIINITCOM.OEIdentity"); }

	bstr_t GetCurrentIdentity();
};

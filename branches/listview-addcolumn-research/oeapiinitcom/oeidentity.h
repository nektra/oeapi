
using namespace comet;
using namespace OEAPIINITCOM;


template<> class coclass_implementation<OEIdentity> : public coclass<OEIdentity>
{
public:
	coclass_implementation<OEIdentity>() {}
	~coclass_implementation<OEIdentity>() {}

	static TCHAR *get_progid() { return _T("OEAPIINITCOM.OEIdentity"); }

	bstr_t GetCurrentIdentity();
};

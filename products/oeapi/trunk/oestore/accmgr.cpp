#include "accmgr.h"
#include "se_debug.h"

#if _MSC_VER >= 1400
#pragma warning(push)
// disable warning C4812: 
// obsolete declaration style: please use 'coclass_implementation<comet::OESTORE::OEMessage>::coclass_implementation' instead
#pragma warning(disable:4812) 
#endif


//////////////////////////////////////////////////////////////////////////
//
// TOEMailAccountManager
//
//////////////////////////////////////////////////////////////////////////
TOEMailAccountManager::TOEMailAccountManager()
{
	HRESULT hr;
	if (FAILED(hr = CoCreateInstance(CLSID_IOEAccountManager, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IOEAccountManager, 
		(void**) &_pMSOEAccMgr)))
	{
		debug_print(DEBUG_ERROR, TEXT("CoCreateInstance for CLSID_IOEAccountManager failed with HR=0x%08x"), hr);
	}
}

TOEMailAccountManager::~TOEMailAccountManager()
{


}

TOEMailAccountPtr TOEMailAccountManager::GetFirstAccount()
{
	return 0;
}

TOEMailAccountPtr TOEMailAccountManager::GetNextAccount()
{

	return 0;
}

TOEMailAccountPtr TOEMailAccountManager::GetDefaultAccount()
{
	return 0;

}

TOEMailAccountPtr TOEMailAccountManager::GetCurrentAccount()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// TOEMailAccount
//
//////////////////////////////////////////////////////////////////////////



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
	_pMSOEAccMgr = NULL;

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
	HRESULT hr;

	_pMSOEAccEnum = NULL;
	TOEMailAccountPtr pAcc;
	
	if (SUCCEEDED(hr = _pMSOEAccMgr->EnumerateAccounts(0xFFFFFFFF, &_pMSOEAccEnum)))
	{
		CComPtr<IMSOEAccount> pMSOEAcc;
		_pMSOEAccEnum->Reset();
		_pMSOEAccEnum->Next(&pMSOEAcc);

		pAcc->
	}
	else
	{
		debug_print(DEBUG_ERROR, TEXT("EnumerateAccounts failed with HR=0x%08x"), hr);
	}

	return 0;
}

TOEMailAccountPtr TOEMailAccountManager::GetNextAccount()
{
	if (!_pMSOEAccEnum)
		return 0;

	

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

unsigned __int64 TOEMailAccount::GetAccountId()
{
	return 0;

}
comet::bstr_t TOEMailAccount::GetAccountName()
{
	return "";

}
comet::bstr_t TOEMailAccount::GetAccountGuid()
{
	return "";

}
comet::bstr_t TOEMailAccount::GetMailAddress()
{
	return "";

}
comet::OESTORE::ACCOUNTTYPE TOEMailAccount::GetAccountType()
{
	return comet::OESTORE::OE_ACCTYPE_HTTP;
}


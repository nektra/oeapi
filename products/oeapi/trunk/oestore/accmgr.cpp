#include "accmgr.h"
#include "se_debug.h"

#include "acct_props.h"

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

	_dwCookie = 0;
	_pEventSink = NULL;

	HRESULT hr;
	if (FAILED(hr = CoCreateInstance(CLSID_IOEAccountManager, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IOEAccountManager, 
		(void**) &_pMSOEAccMgr)))
	{
		debug_print(DEBUG_ERROR, TEXT("CoCreateInstance for CLSID_IOEAccountManager failed with HR=0x%08x"), hr);		
	}
	else
	{
		_pEventSink = new CAccMgrEventSink(this);
		if (FAILED(hr = _pMSOEAccMgr->Advise(_pEventSink, &_dwCookie)))
		{
			debug_print(DEBUG_ERROR, TEXT("Advise failed with HR=0x%08x"), hr);
		}
	}

	
}

TOEMailAccountManager::~TOEMailAccountManager()
{
	if (_pEventSink)
	{
		_pMSOEAccMgr->Unadvise(_dwCookie);
		delete _pEventSink;
	}	
}

TOEMailAccountPtr TOEMailAccountManager::GetFirstAccount()
{
	HRESULT hr;

	_pMSOEAccEnum = NULL;
	
	if (SUCCEEDED(hr = _pMSOEAccMgr->EnumerateAccounts(0xFFFFFFFF, &_pMSOEAccEnum)))
	{
		CComPtr<IMSOEAccount> pMSOEAcc;
		_pMSOEAccEnum->Reset();
		_pMSOEAccEnum->Next(&pMSOEAcc);

		TOEMailAccountPtr pAcc = TOEMailAccountPtr(new TOEMailAccount);
		((TOEMailAccount*)pAcc.get())->setMSOEAcct(pMSOEAcc);

		_currentAccount = pAcc;

		return pAcc;
		
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

	CComPtr<IMSOEAccount> pMSOEAcc;
	HRESULT hr = _pMSOEAccEnum->Next(&pMSOEAcc);

	if (SUCCEEDED(hr) && pMSOEAcc)
	{
		TOEMailAccountPtr pAcc = TOEMailAccountPtr(new TOEMailAccount);
		((TOEMailAccount*)pAcc.get())->setMSOEAcct(pMSOEAcc);
		_currentAccount = pAcc;
		return pAcc;
	}

	return 0;
}

TOEMailAccountPtr TOEMailAccountManager::GetDefaultAccount()
{
	CComPtr<IMSOEAccount> pMSOEAcc;
	if (SUCCEEDED(_pMSOEAccMgr->GetDefaultAccount(ACCT_MAIL, &pMSOEAcc)))
	{
		TOEMailAccountPtr pAcc = TOEMailAccountPtr(new TOEMailAccount);
		((TOEMailAccount*)pAcc.get())->setMSOEAcct(pMSOEAcc);
		return pAcc;
	}

	return 0;
}

TOEMailAccountPtr TOEMailAccountManager::GetCurrentAccount()
{
	return _currentAccount;
}

//////////////////////////////////////////////////////////////////////////
//
// TOEMailAccount
//
//////////////////////////////////////////////////////////////////////////

unsigned __int64 TOEMailAccount::GetAccountId()
{
	char s[1024] = {0};
	_pMSOEAcct->GetPropSz(AP_ACCOUNT_ID, s, 1024);
	return atoi(s);
}

comet::bstr_t TOEMailAccount::GetAccountName()
{
	char s[1024] = {0};
	_pMSOEAcct->GetPropSz(AP_ACCOUNT_NAME, s, 1024);
	return comet::bstr_t(s);
}
comet::bstr_t TOEMailAccount::GetAccountGuid()
{
	const ULONG bs = 1024;
	BYTE b[bs];
	HRESULT hr = _pMSOEAcct->GetPropA(AP_UNIQUE_ID, b, (ULONG*)&bs);
	
	if (SUCCEEDED(hr))
	{
		WCHAR szGuid[40];
		GUID guid;
		memcpy(&guid, b, sizeof(GUID));
		StringFromGUID2(guid, szGuid, 40);
		return comet::bstr_t(szGuid);
	}
	else
	{
		return "";
	}
}
	
comet::bstr_t TOEMailAccount::GetMailAddress()
{
	char s[1024] = {0};
	_pMSOEAcct->GetPropSz(AP_SMTP_EMAIL_ADDRESS, s, 1024);
	return comet::bstr_t(s);
}
comet::OESTORE::ACCOUNTTYPE TOEMailAccount::GetAccountType()
{
	char tmp[CCHMAX_SERVER_NAME];
	
	if (SUCCEEDED(_pMSOEAcct->GetPropSz(AP_HTTPMAIL_SERVER, tmp, CCHMAX_SERVER_NAME)))
		return OE_ACCTYPE_HTTP;

	if (SUCCEEDED(_pMSOEAcct->GetPropSz(AP_LDAP_SERVER, tmp, CCHMAX_SERVER_NAME)))
		return OE_ACCTYPE_LDAP;

	if (SUCCEEDED(_pMSOEAcct->GetPropSz(AP_POP3_SERVER, tmp, CCHMAX_SERVER_NAME)))
		return OE_ACCTYPE_POP3;

	if (SUCCEEDED(_pMSOEAcct->GetPropSz(AP_IMAP_SERVER, tmp, CCHMAX_SERVER_NAME)))
		return OE_ACCTYPE_IMAP;

	if (SUCCEEDED(_pMSOEAcct->GetPropSz(AP_NNTP_SERVER, tmp, CCHMAX_SERVER_NAME)))
		return OE_ACCTYPE_NEWS;

	return OE_ACCTYPE_UNKNOWN;

}


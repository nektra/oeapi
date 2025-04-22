#ifndef __OEAPI_ACC_MANAGER_H
#define __OEAPI_ACC_MANAGER_H

#include "OESTORE.h"
#include "msoeapi.h"
#include "mimeole.h"
#include "oeundoc.h"
#include "std.h"
	
#include <atlbase.h>

#define TOEMailAccountPtr comet::com_ptr<comet::OESTORE::IOEMailAccount>
#define TOEMailAccount coclass_implementation<comet::OESTORE::OEMailAccount>

template<>
class TOEMailAccount : public comet::coclass<OEMailAccount>
{
	CComPtr<IMSOEAccount> _pMSOEAcct;

	
public:

	static TCHAR *get_progid() { return _T("OESTORE.OEMailAccount"); }

	unsigned __int64 GetAccountId();
	comet::bstr_t GetAccountName();
	comet::bstr_t GetAccountGuid();
	comet::bstr_t GetMailAddress();
	comet::OESTORE::ACCOUNTTYPE GetAccountType();

	void setMSOEAcct(IMSOEAccount* p ) { _pMSOEAcct = p; }

	
	TOEMailAccount()
	{
		_pMSOEAcct = NULL;
	}

	~TOEMailAccount() {};
};

#define TOEMailAccountManagerPtr comet::com_ptr<comet::OESTORE::IOEMailAccountManager>
#define TOEMailAccountManager coclass_implementation<comet::OESTORE::OEMailAccountManager>

class CAccMgrEventSink;

template<> 
class TOEMailAccountManager : public comet::coclass<comet::OESTORE::OEMailAccountManager>
{
	CComPtr<IMSOEAccountManager> _pMSOEAccMgr;
	CComPtr<IMSOEAccountEnum> _pMSOEAccEnum;

	TOEMailAccountPtr _currentAccount;

	CAccMgrEventSink* _pEventSink;
	DWORD _dwCookie;

public:
	TOEMailAccountManager();
	~TOEMailAccountManager() ;
	
	static TCHAR *get_progid() { return _T("OESTORE.OEMailAccountManager"); }

	TOEMailAccountPtr GetFirstAccount();
	TOEMailAccountPtr GetNextAccount();
	TOEMailAccountPtr GetCurrentAccount();
	TOEMailAccountPtr GetDefaultAccount();

	virtual void OnDefaultAccountChanged()
	{
		connection_point.Fire_OnDefaultAccountChanged();
	}
};


// Implementation of internal IImnAdviseAccount for catching events
//
class CAccMgrEventSink : public IMSOEAdviseAccount
{
	TOEMailAccountManagerPtr _accMgr;
public:

	CAccMgrEventSink(TOEMailAccountManagerPtr pAccMgr)
	{
		_accMgr = pAccMgr;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void **ppvObject)
	{
		*ppvObject = this;
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	HRESULT STDMETHODCALLTYPE AdviseAccount(DWORD dwAdviseType, void*)
	{
		if (dwAdviseType == AN_DEFAULT_CHANGED)
		{
			((TOEMailAccountManager*)_accMgr.get())->OnDefaultAccountChanged();
		}
		return S_OK;
	}
};


#endif //__OEAPI_ACC_MANAGER_H
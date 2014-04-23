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
	
	TOEMailAccount()
	{
		_pMSOEAcct = NULL;
	}

	TOEMailAccount(IMSOEAccount* pacct)
	{
		_pMSOEAcct = pacct;
	};

	~TOEMailAccount() {};
};

#define TOEMailAccountManagerPtr comet::com_ptr<comet::OESTORE::IOEMailAccountManager>
#define TOEMailAccountManager coclass_implementation<comet::OESTORE::OEMailAccountManager>

template<> 
class TOEMailAccountManager : public comet::coclass<comet::OESTORE::OEMailAccountManager>
{
	CComPtr<IMSOEAccountManager> _pMSOEAccMgr;
	CComPtr<IMSOEAccountEnum> _pMSOEAccEnum;

	TOEMailAccountPtr _currentAccount;	

public:
	TOEMailAccountManager();
	~TOEMailAccountManager() ;
	
	static TCHAR *get_progid() { return _T("OESTORE.OEMailAccountManager"); }

	TOEMailAccountPtr GetFirstAccount();
	TOEMailAccountPtr GetNextAccount();
	TOEMailAccountPtr GetCurrentAccount();
	TOEMailAccountPtr GetDefaultAccount();
};

#endif //__OEAPI_ACC_MANAGER_H
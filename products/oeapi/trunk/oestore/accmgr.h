#ifndef __OEAPI_ACC_MANAGER_H
#define __OEAPI_ACC_MANAGER_H

#include "OESTORE.h"
#include "msoeapi.h"
#include "mimeole.h"
#include "oeundoc.h"

#define TOEMailAccountPtr comet::com_ptr<comet::OESTORE::IOEMailAccount>
#define TOEMailAccount coclass_implementation<comet::OESTORE::OEMailAccount>
template<> class TOEMailAccount : public comet::coclass<comet::OESTORE::OEMailAccount>
{
public:

	unsigned __int64 GetAccountId();
	comet::bstr_t GetAccountName();
	comet::bstr_t GetAccountGuid();
	comet::bstr_t GetMailAddress();
	comet::OESTORE::ACCOUNTTYPE GetAccountType();
	
	TOEMailAccount();
	virtual ~TOEMailAccount();
};

#define TOEMailAccountManagerPtr comet::com_ptr<comet::OESTORE::IOEMailAccountManager>
#define TOEMailAccountManager coclass_implementation<comet::OESTORE::OEMailAccountManager>

template<> class TOEMailAccountManager : public comet::coclass<comet::OESTORE::OEMailAccountManager>
{
	comet::com_ptr<IMSOEAccountManager> _pMSOEAccMgr;
public:
	TOEMailAccountManager();
	virtual ~TOEMailAccountManager();
	
	TOEMailAccountPtr GetFirstAccount();
	TOEMailAccountPtr GetNextAccount();
	TOEMailAccountPtr GetCurrentAccount();
	TOEMailAccountPtr GetDefaultAccount();
};

#endif //__OEAPI_ACC_MANAGER_H
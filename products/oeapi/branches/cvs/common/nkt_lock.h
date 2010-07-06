/* $Id: nkt_lock.h,v 1.8 2008/09/07 16:55:57 ibejarano Exp $
 *
 * Author: Ismael Bejarano <ismael.bejarano@nektra.com>
 *
 * Copyright (c) 2006-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __NKT_LOCK_H__
#define __NKT_LOCK_H__ 1

#ifndef _WINDOWS_
#include <windows.h>
#endif

/**
Nkt locking class, support
*/
class NktLock
{
	friend class NktAutoLock;

public:
	NktLock(bool autoInit = false);
	~NktLock();

	/**
	Init lock
	*/
	void Init();

	/**
	Finish lock
	*/
	void Term();

	/**
	Acquire lock
	*/
	void Enter();

	/**
	Release lock
	*/
	void Leave();

	/**
	Try to acquire lock, return true if successful
	*/
	bool TryEnter();

private:
	CRITICAL_SECTION _criticalSection;
	//LONG _flag;
	bool _autoInit;
};

/**
Acquire a critical section and lock on construction and
unlock on destruction
*/
class NktAutoLock {
public:
	NktAutoLock(CRITICAL_SECTION* criticalSection);
	NktAutoLock(class NktLock& lock);
	~NktAutoLock();

private:
	CRITICAL_SECTION* _pCriticalSection;
};

#endif //__NKT_LOCK_H__

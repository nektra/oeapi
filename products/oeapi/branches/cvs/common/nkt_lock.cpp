/* $Id: nkt_lock.cpp,v 1.8 2008/09/07 16:55:57 ibejarano Exp $
 *
 * Author: Ismael Bejarano <ismael.bejarano@nektra.com>
 *
 * Copyright (c) 2006-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "nkt_lock.h"


///////////////////////////////////////////////////////////////
//-----------------------------------------------------------//
NktLock::NktLock(bool autoInit):_autoInit(autoInit)
{
	// Init to -1
	// InterlockIncrement & Decrement on WIN95 and NT351
	// only return positive, negative and zero.
	//
	if(_autoInit) {
		Init();
	}
}

//-----------------------------------------------------------//
NktLock::~NktLock()
{
	if(_autoInit) {
		Term();
	}
}

//-----------------------------------------------------------//
void NktLock::Init()
{
	::InitializeCriticalSection(&_criticalSection);
}

//-----------------------------------------------------------//
void NktLock::Term()
{
	::DeleteCriticalSection(&_criticalSection);
}

//-----------------------------------------------------------//
void NktLock::Enter()
{
	::EnterCriticalSection(&_criticalSection);
}

//-----------------------------------------------------------//
void NktLock::Leave()
{
	::LeaveCriticalSection(&_criticalSection);
}

//-----------------------------------------------------------//
bool NktLock::TryEnter()
{
	return ::TryEnterCriticalSection(&_criticalSection) ? true : false;
}

///////////////////////////////////////////////////////////////
//-----------------------------------------------------------//
NktAutoLock::NktAutoLock(CRITICAL_SECTION* criticalSection)
:_pCriticalSection(criticalSection)
{
	if(_pCriticalSection) {
		::EnterCriticalSection(_pCriticalSection);
	}
}

//-----------------------------------------------------------//
NktAutoLock::NktAutoLock(class NktLock& lock)
:_pCriticalSection(&lock._criticalSection)
{
	if(_pCriticalSection) {
		::EnterCriticalSection(_pCriticalSection);
	}
}


//-----------------------------------------------------------//
NktAutoLock::~NktAutoLock() 
{
	if(_pCriticalSection) {
		::LeaveCriticalSection(_pCriticalSection);
		_pCriticalSection = NULL;
	}
}

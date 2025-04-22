/* $Id: nkt_utils.h,v 1.5.6.2 2007/07/31 21:29:40 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef NKT_UTILS
#define NKT_UTILS

#include "assert.h"

template <class X> class CountedPtr
{
public:
    typedef X element_type;

    CountedPtr(X* p = 0) // allocate a new counter
        : itsCounter(0) {if (p) itsCounter = new counter(p);}
    ~CountedPtr()
        {release();}
    CountedPtr(const CountedPtr& r) throw()
        {acquire(r.itsCounter);}
    CountedPtr& operator=(const CountedPtr& r)
    {
        if (this != &r) {
            release();
            acquire(r.itsCounter);
        }
        return *this;
    }

	/**
	Pablo: I want to assign a pointer after creating a null CountedPtr or set it null.
	*/
    CountedPtr& operator=(X* p)
    {
        release();
		if(p) {
			itsCounter = new counter(p);
		}
        return *this;
    }

	operator X*() const throw() {return itsCounter ? itsCounter->ptr : 0;}
    X& operator*()  const throw()   {return *itsCounter->ptr;}
    X* operator->() const throw()   {return itsCounter->ptr;}
    X* get()        const throw()   {return itsCounter ? itsCounter->ptr : 0;}
    bool unique()   const throw()
        {return (itsCounter ? itsCounter->count == 1 : true);}
	bool is_null() { return (get() == NULL); }

	// Add support for if(ptr) and if(!ptr) syntax sugar
	bool operator!()
	{
		return (get() == NULL);
	}

	operator bool() const
	{
		return (get() != NULL);
	}


private:

    struct counter {
        counter(X* p = 0, unsigned c = 1) : ptr(p), count(c) {}
        X*          ptr;
        unsigned    count;
    }* itsCounter;

    void acquire(counter* c) throw()
    { // increment the count
        itsCounter = c;
        if (c) ++c->count;
    }

    void release()
    { // decrement the count, delete if it is 0
        if (itsCounter) {
            if (--itsCounter->count == 0) {
                delete itsCounter->ptr;
                delete itsCounter;
            }
            itsCounter = 0;
        }
    }
};

class NKTRefCounted {
public:
	NKTRefCounted() : _count(0) {}

	DWORD addref() { return ++_count; }
	DWORD release()
	{
		if(--_count == 0) {
			delete this;
			return 0;
		}

		return _count;
	}

private:
	int _count;
};

template <class T, const IID* piid> class NKTComPtr
{
public:
	NKTComPtr()
	{
		_comPtr = NULL;
	}

	NKTComPtr(T *ptr)
	{
		_comPtr = ptr;
		if (_comPtr != NULL) {
			_comPtr->AddRef();
		}
	}

	NKTComPtr(IUnknown *pUnknown)
	{
		HRESULT hr;

		_comPtr = NULL;
		if(pUnknown != NULL) {
			hr = pUnknown->QueryInterface(*piid, (void**) &_comPtr);
			assert(SUCCEEDED(hr) && (_comPtr != NULL));
		}
	}

	NKTComPtr(const NKTComPtr &ptr)
	{
		_comPtr = ptr;
		if(_comPtr != NULL) {
			_comPtr->AddRef();
		}
	}

	~NKTComPtr()
	{
		Release();
	}

	void Release()
	{
		if (_comPtr != NULL) {
			T *oldPtr = _comPtr;
			_comPtr = NULL;
			oldPtr->Release();
		}
	};

	void Attach(T* ptr) {
		Release();
		_comPtr = ptr;
	}

	T* Detach() {
		T* ptr = _comPtr;
		if (_comPtr != NULL) {
			_comPtr = NULL;
		}
		return ptr;
	}

	T* operator=(int ptr)
	{
		assert(ptr == 0);
		if(_comPtr != NULL) {
			_comPtr->Release();
			_comPtr = NULL;
		}

		return _comPtr;
	}

	T* operator=(T *ptr)
	{
		if(_comPtr != ptr) {
			IUnknown *oldPtr;

			oldPtr = _comPtr;

			_comPtr = ptr;
			if(_comPtr != NULL) {
				_comPtr->AddRef();
			}
			if(oldPtr != NULL) {
				oldPtr->Release();
			}
		}

		return _comPtr;
	}

	T* operator=(IUnknown *pUnknown)
	{
		HRESULT hr;
		IUnknown *oldPtr = _comPtr;

		_comPtr = NULL;
		if(pUnknown != NULL) {
			hr = pUnknown->QueryInterface(*piid, (void **) &_comPtr);
			assert(SUCCEEDED(hr) && (_comPtr != NULL));
		}

		if(oldPtr != NULL) {
			oldPtr->Release();
		}

		return _comPtr;
	}

	T* operator=(const NKTComPtr &ptr)
	{
		if(this == &ptr) {
			return _comPtr;
		}

		return operator=(ptr.operator T*());
	}

	T &operator*()
	{
		assert(_comPtr != NULL);
		return *_comPtr;
	}

	T **operator&()
	{
		return &_comPtr;
	}

	operator T*() const
	{
		return _comPtr;
	}

	T *operator->()
	{
		assert(_comPtr != NULL);
		return _comPtr;
	}

	// for c style pointer verifications
	BOOL operator!()
	{
		return (_comPtr == NULL);
	}

	operator BOOL() const
	{
		return (_comPtr != NULL);
	}

	const IID &iid()
	{
		return *piid;
	}

private:
	T *_comPtr;
};

template <typename T> 
class NktBuffer
{
public:
	NktBuffer() : _ptr(NULL) {}
	NktBuffer(T* ptr) : _ptr(ptr) {}
	explicit NktBuffer(size_t size) : _ptr(NULL) { alloc(size); }

	~NktBuffer() {
		release();
	}

	T* alloc(size_t size) {
		release();
		_ptr = new T[size];
		return _ptr;
	}

	void release() {
		if(_ptr) {
			delete [] _ptr;
			_ptr = NULL;
		}
	}

	operator T* () { return _ptr; }

private:
	T* _ptr;
};


#endif // NKT_UTILS
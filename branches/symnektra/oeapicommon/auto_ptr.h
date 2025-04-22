/* $Id: auto_ptr.h,v 1.4 2008/09/07 16:55:56 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __AUTO_PTR_H
#define __AUTO_PTR_H

template <class T> class AutoPtr {
public:
	AutoPtr() {	ptr_ = 0; }
	AutoPtr(T *ptr)
	{
		ptr_ = ptr;
		if(ptr_)
			ptr_->AddRef();
	}

	AutoPtr(AutoPtr<T> &autoPtr)
	{
		ptr_ = autoPtr.get();
		if(ptr_)
			ptr_->AddRef();
	}

	~AutoPtr()
	{
		if(ptr_!=NULL)
			ptr_->Release();
	}

	AutoPtr<T> &operator=(AutoPtr<T> &ptr)
	{
		if(ptr_!=NULL)
			ptr_->Release();

		ptr_ = ptr.get();
		if(ptr_)
			ptr_->AddRef();

		return *this;
	}

	AutoPtr<T> &operator=(T *ptr)
	{
		if(ptr_!=NULL)
			ptr_->Release();

		ptr_ = ptr;
		if(ptr)
			ptr_->AddRef();

		return *this;
	}

	T *get() { return ptr_;	}
	T *operator->() { return ptr_; }
	T &operator*() { return (*ptr_); }
	BOOL is_null() { return (ptr_ == NULL); }
private:
    T *ptr_;
};

#endif // __AUTO_PTR_H

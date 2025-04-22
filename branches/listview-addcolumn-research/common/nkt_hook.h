/* $Id: nkt_hook.h,v 1.1.2.2 2007/07/30 21:17:43 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/
#ifndef __NKT_HOOK__
#define __NKT_HOOK__ 1

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
interface NktIUnknown;
struct IUnknownVtbl
{
	BEGIN_INTERFACE

		HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
		NktIUnknown __RPC_FAR * This,
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

		ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
			NktIUnknown __RPC_FAR * This);

		ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
			NktIUnknown __RPC_FAR * This);

	END_INTERFACE
}; // *PUnknownVtbl;


//////////////////////////////////////////////////////////////////////////
interface NktIUnknown
{
	CONST_VTBL struct IUnknownVtbl __RPC_FAR *lpVtbl;
};


//////////////////////////////////////////////////////////////////////////
//
class NktBasicHooker
{
public:
	//typedef ULONG ( STDMETHODCALLTYPE __RPC_FAR *FunctionPointer)(NktIUnknown __RPC_FAR * This);
	//typedef T FunctionPointer;

protected:
	//template  <typename FunctionPointer>
	static void Patch(void** address, void* value, void** oldValue = NULL)
	{
		if (oldValue) {
			*oldValue = *address;
		}
		DWORD oldProtect, thunkProtect;
		::VirtualProtect(address, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
		*address = value;
		::VirtualProtect(address, 4, oldProtect, &thunkProtect);
	}
};

//////////////////////////////////////////////////////////////////////
typedef ULONG ( STDMETHODCALLTYPE __RPC_FAR *NktAddRefFunction)(NktIUnknown __RPC_FAR * This);


//////////////////////////////////////////////////////////////////////
template <typename Iface>
class NktMethodHooker : public NktBasicHooker
{
	//typedef typename Iface Iface;
	//typedef typename Method Method;
public:
	NktMethodHooker()
		:_ready(false)
	{
	}

	~NktMethodHooker()
	{
		if(_ready) {
			Uninit();
		}
	}

	template <typename Method, typename UU>
	Method Init(Iface* iface, Method newMethod, UU u) 
	{
		_oldMethodPtr = (void**)(Method*)&(iface->lpVtbl->*u);
		_oldMethod = *_oldMethodPtr;
		_newMethod = newMethod;
		Patch(_oldMethodPtr, _newMethod, &_oldMethod);
		_ready = true;
		return (Method)_oldMethod;
	}

	bool IsHooked() { return _ready; }

	void Uninit()
	{
		if(!_ready) {
			return;
		}

		if(_oldMethodPtr == NULL || _oldMethod == NULL || *_oldMethodPtr != _newMethod) {
			return;
		}

		_ready = false;

		Patch(_oldMethodPtr, _oldMethod);
	}

	void Detach()
	{
		_ready = false;
	}

	// Method GetRawMethod() {
	//	return _oldMethod;
	//}

private:
	bool _ready;
	void* _oldMethod;
	void* _newMethod;
	void** _oldMethodPtr;
};

//.
////////////////////////////////////////////////////////////////////////
//template<typename TT, typename UU = NktAddRefFunction>
//class NktHookedIUnknown : public NktBasicHooker<UU>
//{
//	typedef NktHookedIUnknown<TT, UU>* myPointer;
//public:
//	NktHookedIUnknown()
//	{
//		_this = NULL;
//		_ready = false;
//		_oldVtblAddRef = NULL;
//		_oldAddRef = NULL;
//		_sink = 0;
//		GetThis(this);
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	~NktHookedIUnknown()
//	{
//		if (_ready)
//			Uninit();
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	int Init(IUnknown *_intf)
//	{
//		NktIUnknown* target = (NktIUnknown*)_intf;
//		_oldVtblAddRef = (FunctionPointer*)&target->lpVtbl->AddRef;
//		_oldAddRef = *_oldVtblAddRef;
//		Patch(_oldVtblAddRef, _HookedAddRef, &_oldAddRef);
//		_ready = true;
//		return 1;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	int Init(const IID &rclsid, const IID &riid)
//	{
//		IUnknown *msl;
//		IUnknown *pUnk;
////		HRESULT hr = ::CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER, riid,(void**)&msl);
//		HRESULT hr = ::CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnk);
//
//		if(SUCCEEDED(hr)) {
//			hr = pUnk->QueryInterface(riid, (LPVOID *)&msl);
//			pUnk->Release();
//		}
//
//		if(FAILED(hr)) {
//			return 0;
//		}
//
//		if(!msl) {
//			return 0;
//		}
//
//		int res = Init(msl);
//		msl->Release();
//		return res;
//	}
//
//	void SetSink(const FunctionPointer _sinker, bool sinkOnce = true)
//	{
//		_sink = _sinker;
//		_sinkOnce = onlyOnce;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	void Uninit()
//	{
//		if(!_ready)
//			return;
//
//		_ready = false;
//
//		if(_oldAddRef == NULL)
//			return;
//
//		if (_oldVtblAddRef == NULL)
//			return;
//
//		if (*_oldVtblAddRef != &_HookedAddRef)
//			return;
//
//		Patch(_oldVtblAddRef, _oldAddRef);
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	inline bool IsReady() const { return _ready; }
//
//private:
//	//////////////////////////////////////////////////////////////////////////
//	inline FunctionPointer GetFunction() const { return _oldAddRef; }
//	inline void SetThis(NktIUnknown* This) { _this = This; }
//
//public:
//	inline operator TT*() { return (TT*)_this; }
//
//	//////////////////////////////////////////////////////////////////////////
////	inline static const char* _log_(const char* rr = NULL)
////	{
////		static const char* _stcch = 0;
////
////		if (!_stcch)
////			_stcch = rr?rr:"hooked_iunknown";
////
////		return _stcch;
////	}
//private:
//
//	inline static myPointer GetThis(myPointer newThis = NULL)
//	{
//		static myPointer address;
//		if(newThis) {
//			address = newThis;
//		}
//		return address;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	static ULONG STDMETHODCALLTYPE __RPC_FAR _HookedAddRef(NktIUnknown __RPC_FAR *This)
//	{
//		NktHookedIUnknown* hooked = static_cast<NktHookedIUnknown*>(GetThis());
//		FunctionPointer origAddRef = hooked->GetFunction();
//		hooked->SetThis(This);
//		if(hooked->_sink) {
//			FunctionPointer sink = hooked->_sink;
//			if(hooked->_sinkOnce) {
//				hooked->_sink = NULL;
//			}
//			sink(This);
//		}
//		return origAddRef(This);
//	}
//
//
//	bool _ready;
//	FunctionPointer *_oldVtblAddRef;
//	FunctionPointer _oldAddRef;
//	FunctionPointer _sink;
//	bool _sinkOnce;
//	NktIUnknown* _this;
//};

#endif // __NKT_HOOK__

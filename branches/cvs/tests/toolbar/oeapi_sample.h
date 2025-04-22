

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Mon Feb 18 14:03:21 2008
 */
/* Compiler settings for .\oeapi_sample.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __oeapi_sample_h__
#define __oeapi_sample_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Iatl_addin_FWD_DEFINED__
#define __Iatl_addin_FWD_DEFINED__
typedef interface Iatl_addin Iatl_addin;
#endif 	/* __Iatl_addin_FWD_DEFINED__ */


#ifndef __atl_addin_FWD_DEFINED__
#define __atl_addin_FWD_DEFINED__

#ifdef __cplusplus
typedef class atl_addin atl_addin;
#else
typedef struct atl_addin atl_addin;
#endif /* __cplusplus */

#endif 	/* __atl_addin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __Iatl_addin_INTERFACE_DEFINED__
#define __Iatl_addin_INTERFACE_DEFINED__

/* interface Iatl_addin */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_Iatl_addin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CFC1DF70-C1DC-48BF-A531-00F7081A8879")
    Iatl_addin : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct Iatl_addinVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Iatl_addin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Iatl_addin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Iatl_addin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Iatl_addin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Iatl_addin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Iatl_addin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Iatl_addin * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } Iatl_addinVtbl;

    interface Iatl_addin
    {
        CONST_VTBL struct Iatl_addinVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Iatl_addin_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Iatl_addin_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Iatl_addin_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Iatl_addin_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Iatl_addin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Iatl_addin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Iatl_addin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __Iatl_addin_INTERFACE_DEFINED__ */



#ifndef __oeapi_sampleLib_LIBRARY_DEFINED__
#define __oeapi_sampleLib_LIBRARY_DEFINED__

/* library oeapi_sampleLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_oeapi_sampleLib;

EXTERN_C const CLSID CLSID_atl_addin;

#ifdef __cplusplus

class DECLSPEC_UUID("6D4D32D9-3499-4615-A04F-CAB011A914B1")
atl_addin;
#endif
#endif /* __oeapi_sampleLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



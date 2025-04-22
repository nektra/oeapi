

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Jul 24 12:47:25 2006
 */
/* Compiler settings for msgtable.idl:
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

#ifndef __msgtable_h__
#define __msgtable_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMessageTable_FWD_DEFINED__
#define __IMessageTable_FWD_DEFINED__
typedef interface IMessageTable IMessageTable;
#endif 	/* __IMessageTable_FWD_DEFINED__ */


#ifndef __IMessageList_FWD_DEFINED__
#define __IMessageList_FWD_DEFINED__
typedef interface IMessageList IMessageList;
#endif 	/* __IMessageList_FWD_DEFINED__ */


#ifndef __IMessageListWMail_FWD_DEFINED__
#define __IMessageListWMail_FWD_DEFINED__
typedef interface IMessageListWMail IMessageListWMail;
#endif 	/* __IMessageListWMail_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_msgtable_0000 */
/* [local] */ 

//+-------------------------------------------------------------------------
// GUID Definitions
//--------------------------------------------------------------------------
#pragma comment(lib,"uuid.lib")

// {233A9692-667E-11D1-9DFB-006097D50408}
DEFINE_GUID(CLSID_IMessageList, 0x233a9692, 0x667e, 0x11d1, 0x9d, 0xfb, 0x0, 0x60, 0x97, 0xd5, 0x04, 0x08);

// {DE89B950-A597-11D1-9E46-006097D50408}
DEFINE_GUID(IID_IMessageList, 0xde89b950, 0xa597, 0x11d1, 0x9e, 0x46, 0x0, 0x60, 0x97, 0xd5, 0x04, 0x08);

// {51A0B0D3-7AFE-4081-BF82-F87AE36373DD}
DEFINE_GUID(IID_IMessageListWMail, 0x51A0B0D3, 0x7AFE, 0x4081, 0xbf, 0x82, 0xf8, 0x7a, 0xe3, 0x63, 0x73, 0xdd);

#ifndef __MESSAGETABLE_DEFINED
#define __MESSAGETABLE_DEFINED


extern RPC_IF_HANDLE __MIDL_itf_msgtable_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_msgtable_0000_v0_0_s_ifspec;

#ifndef __IMessageTable_INTERFACE_DEFINED__
#define __IMessageTable_INTERFACE_DEFINED__

/* interface IMessageTable */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMessageTable;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DE89B96f-A597-11D1-9E46-006097D50408")
    IMessageTable : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StartFind( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOwner( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Synchronize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ DWORD *__MIDL_0011) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRow( DWORD rowId, DWORD** msgInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseRow( DWORD* msgInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndentLevel( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Mark( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSortInfo( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSynchronizeComplete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSortInfo( 
            DWORD *__MIDL_0012,
            DWORD __MIDL_0013) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLanguage( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLanguage( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNextRow( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRelativeRow( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRowState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelectionState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Expand( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Collapse( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenMessage( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRowMessageId( 
            /* [in] */ DWORD __MIDL_0014,
            /* [out] */ DWORD *__MIDL_0015) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRowIndex( 
            /* [in] */ DWORD __MIDL_0016,
            /* [out] */ DWORD *__MIDL_0017) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteRows( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyRows( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterNotify( 
            /* [in] */ HWND __MIDL_0018) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterNotify( 
            /* [in] */ DWORD dwReserved,
            /* [in] */ HWND __MIDL_0019) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindNextRow( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRowFolderId( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageIdList( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConnectionAddRef( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConnectionRelease( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsChild( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAdBarUrl( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessageTableVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMessageTable * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMessageTable * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *StartFind )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetOwner )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Synchronize )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMessageTable * This,
            /* [out] */ DWORD *__MIDL_0011);
        
        HRESULT ( STDMETHODCALLTYPE *GetRow )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *ReleaseRow )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndentLevel )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Mark )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSortInfo )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnSynchronizeComplete )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetSortInfo )( 
            IMessageTable * This,
            DWORD *__MIDL_0012,
            DWORD __MIDL_0013);
        
        HRESULT ( STDMETHODCALLTYPE *GetLanguage )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetLanguage )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetNextRow )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRelativeRow )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRowState )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelectionState )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Expand )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Collapse )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *OpenMessage )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRowMessageId )( 
            IMessageTable * This,
            /* [in] */ DWORD __MIDL_0014,
            /* [out] */ DWORD *__MIDL_0015);
        
        HRESULT ( STDMETHODCALLTYPE *GetRowIndex )( 
            IMessageTable * This,
            /* [in] */ DWORD __MIDL_0016,
            /* [out] */ DWORD *__MIDL_0017);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteRows )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *CopyRows )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterNotify )( 
            IMessageTable * This,
            /* [in] */ HWND __MIDL_0018);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterNotify )( 
            IMessageTable * This,
            /* [in] */ DWORD dwReserved,
            /* [in] */ HWND __MIDL_0019);
        
        HRESULT ( STDMETHODCALLTYPE *FindNextRow )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRowFolderId )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageIdList )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *ConnectionAddRef )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *ConnectionRelease )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *IsChild )( 
            IMessageTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetAdBarUrl )( 
            IMessageTable * This);
        
        END_INTERFACE
    } IMessageTableVtbl;

    interface IMessageTable
    {
        CONST_VTBL struct IMessageTableVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessageTable_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMessageTable_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMessageTable_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMessageTable_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#define IMessageTable_StartFind(This)	\
    (This)->lpVtbl -> StartFind(This)

#define IMessageTable_SetOwner(This)	\
    (This)->lpVtbl -> SetOwner(This)

#define IMessageTable_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IMessageTable_Synchronize(This)	\
    (This)->lpVtbl -> Synchronize(This)

#define IMessageTable_GetCount(This,__MIDL_0011)	\
    (This)->lpVtbl -> GetCount(This,__MIDL_0011)

#define IMessageTable_GetRow(This)	\
    (This)->lpVtbl -> GetRow(This)

#define IMessageTable_ReleaseRow(This)	\
    (This)->lpVtbl -> ReleaseRow(This)

#define IMessageTable_GetIndentLevel(This)	\
    (This)->lpVtbl -> GetIndentLevel(This)

#define IMessageTable_Mark(This)	\
    (This)->lpVtbl -> Mark(This)

#define IMessageTable_GetSortInfo(This)	\
    (This)->lpVtbl -> GetSortInfo(This)

#define IMessageTable_OnSynchronizeComplete(This)	\
    (This)->lpVtbl -> OnSynchronizeComplete(This)

#define IMessageTable_SetSortInfo(This,__MIDL_0012,__MIDL_0013)	\
    (This)->lpVtbl -> SetSortInfo(This,__MIDL_0012,__MIDL_0013)

#define IMessageTable_GetLanguage(This)	\
    (This)->lpVtbl -> GetLanguage(This)

#define IMessageTable_SetLanguage(This)	\
    (This)->lpVtbl -> SetLanguage(This)

#define IMessageTable_GetNextRow(This)	\
    (This)->lpVtbl -> GetNextRow(This)

#define IMessageTable_GetRelativeRow(This)	\
    (This)->lpVtbl -> GetRelativeRow(This)

#define IMessageTable_GetRowState(This)	\
    (This)->lpVtbl -> GetRowState(This)

#define IMessageTable_GetSelectionState(This)	\
    (This)->lpVtbl -> GetSelectionState(This)

#define IMessageTable_Expand(This)	\
    (This)->lpVtbl -> Expand(This)

#define IMessageTable_Collapse(This)	\
    (This)->lpVtbl -> Collapse(This)

#define IMessageTable_OpenMessage(This)	\
    (This)->lpVtbl -> OpenMessage(This)

#define IMessageTable_GetRowMessageId(This,__MIDL_0014,__MIDL_0015)	\
    (This)->lpVtbl -> GetRowMessageId(This,__MIDL_0014,__MIDL_0015)

#define IMessageTable_GetRowIndex(This,__MIDL_0016,__MIDL_0017)	\
    (This)->lpVtbl -> GetRowIndex(This,__MIDL_0016,__MIDL_0017)

#define IMessageTable_DeleteRows(This)	\
    (This)->lpVtbl -> DeleteRows(This)

#define IMessageTable_CopyRows(This)	\
    (This)->lpVtbl -> CopyRows(This)

#define IMessageTable_RegisterNotify(This,__MIDL_0018)	\
    (This)->lpVtbl -> RegisterNotify(This,__MIDL_0018)

#define IMessageTable_UnregisterNotify(This,dwReserved,__MIDL_0019)	\
    (This)->lpVtbl -> UnregisterNotify(This,dwReserved,__MIDL_0019)

#define IMessageTable_FindNextRow(This)	\
    (This)->lpVtbl -> FindNextRow(This)

#define IMessageTable_GetRowFolderId(This)	\
    (This)->lpVtbl -> GetRowFolderId(This)

#define IMessageTable_GetMessageIdList(This)	\
    (This)->lpVtbl -> GetMessageIdList(This)

#define IMessageTable_ConnectionAddRef(This)	\
    (This)->lpVtbl -> ConnectionAddRef(This)

#define IMessageTable_ConnectionRelease(This)	\
    (This)->lpVtbl -> ConnectionRelease(This)

#define IMessageTable_IsChild(This)	\
    (This)->lpVtbl -> IsChild(This)

#define IMessageTable_GetAdBarUrl(This)	\
    (This)->lpVtbl -> GetAdBarUrl(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMessageTable_Initialize_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_StartFind_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_StartFind_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_SetOwner_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_SetOwner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_Close_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_Synchronize_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_Synchronize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetCount_Proxy( 
    IMessageTable * This,
    /* [out] */ DWORD *__MIDL_0011);


void __RPC_STUB IMessageTable_GetCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetRow_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_ReleaseRow_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_ReleaseRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetIndentLevel_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetIndentLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_Mark_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_Mark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetSortInfo_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetSortInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_OnSynchronizeComplete_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_OnSynchronizeComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_SetSortInfo_Proxy( 
    IMessageTable * This,
    DWORD *__MIDL_0012,
    DWORD __MIDL_0013);


void __RPC_STUB IMessageTable_SetSortInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetLanguage_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_SetLanguage_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_SetLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetNextRow_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetNextRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetRelativeRow_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetRelativeRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetRowState_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetRowState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetSelectionState_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetSelectionState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_Expand_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_Expand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_Collapse_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_Collapse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_OpenMessage_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_OpenMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetRowMessageId_Proxy( 
    IMessageTable * This,
    /* [in] */ DWORD __MIDL_0014,
    /* [out] */ DWORD *__MIDL_0015);


void __RPC_STUB IMessageTable_GetRowMessageId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetRowIndex_Proxy( 
    IMessageTable * This,
    /* [in] */ DWORD __MIDL_0016,
    /* [out] */ DWORD *__MIDL_0017);


void __RPC_STUB IMessageTable_GetRowIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_DeleteRows_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_DeleteRows_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_CopyRows_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_CopyRows_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_RegisterNotify_Proxy( 
    IMessageTable * This,
    /* [in] */ HWND __MIDL_0018);


void __RPC_STUB IMessageTable_RegisterNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_UnregisterNotify_Proxy( 
    IMessageTable * This,
    /* [in] */ DWORD dwReserved,
    /* [in] */ HWND __MIDL_0019);


void __RPC_STUB IMessageTable_UnregisterNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_FindNextRow_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_FindNextRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetRowFolderId_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetRowFolderId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetMessageIdList_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetMessageIdList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_ConnectionAddRef_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_ConnectionAddRef_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_ConnectionRelease_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_ConnectionRelease_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_IsChild_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_IsChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageTable_GetAdBarUrl_Proxy( 
    IMessageTable * This);


void __RPC_STUB IMessageTable_GetAdBarUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMessageTable_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_msgtable_0258 */
/* [local] */ 

#endif // __MESSAGETABLE_DEFINED
#ifndef __MESSAGELIST_DEFINED
#define __MESSAGELIST_DEFINED


extern RPC_IF_HANDLE __MIDL_itf_msgtable_0258_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_msgtable_0258_v0_0_s_ifspec;

#ifndef __IMessageList_INTERFACE_DEFINED__
#define __IMessageList_INTERFACE_DEFINED__

/* interface IMessageList */
/* [unique][uuid][object] */ 

typedef IMessageList *LPMESSAGELIST;


EXTERN_C const IID IID_IMessageList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DE89B950-A597-11D1-9E46-006097D50408")
    IMessageList : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateList( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFolder( 
            DWORD folderId,
            DWORD __MIDL_0020,
            DWORD __MIDL_0021,
            DWORD __MIDL_0022,
            DWORD __MIDL_0023) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetViewOptions( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetViewOptions( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClose( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HasFocus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPopupMenu( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelected( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelectedCount( 
            /* [out] */ DWORD *__MIDL_0024) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageA( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageInfo( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRowFolderId( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MarkMessage( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FreeMessageInfo( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MarkRead( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageTable( 
            /* [out] */ IMessageTable **__MIDL_0025) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetListSelector( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageCounts( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageServer( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFocusedItemState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessReceipt( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAdBarUrl( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessageListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMessageList * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMessageList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateList )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFolder )( 
            IMessageList * This,
            DWORD folderId,
            DWORD __MIDL_0020,
            DWORD __MIDL_0021,
            DWORD __MIDL_0022,
            DWORD __MIDL_0023);
        
        HRESULT ( STDMETHODCALLTYPE *SetViewOptions )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetViewOptions )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnClose )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRect )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetRect )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *HasFocus )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnPopupMenu )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelected )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelectedCount )( 
            IMessageList * This,
            /* [out] */ DWORD *__MIDL_0024);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageA )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageInfo )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRowFolderId )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *MarkMessage )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *FreeMessageInfo )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *MarkRead )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageTable )( 
            IMessageList * This,
            /* [out] */ IMessageTable **__MIDL_0025);
        
        HRESULT ( STDMETHODCALLTYPE *GetListSelector )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageCounts )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageServer )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetFocusedItemState )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessReceipt )( 
            IMessageList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetAdBarUrl )( 
            IMessageList * This);
        
        END_INTERFACE
    } IMessageListVtbl;

    interface IMessageList
    {
        CONST_VTBL struct IMessageListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessageList_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMessageList_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMessageList_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMessageList_CreateList(This)	\
    (This)->lpVtbl -> CreateList(This)

#define IMessageList_SetFolder(This,folderId,__MIDL_0020,__MIDL_0021,__MIDL_0022,__MIDL_0023)	\
    (This)->lpVtbl -> SetFolder(This,folderId,__MIDL_0020,__MIDL_0021,__MIDL_0022,__MIDL_0023)

#define IMessageList_SetViewOptions(This)	\
    (This)->lpVtbl -> SetViewOptions(This)

#define IMessageList_GetViewOptions(This)	\
    (This)->lpVtbl -> GetViewOptions(This)

#define IMessageList_OnClose(This)	\
    (This)->lpVtbl -> OnClose(This)

#define IMessageList_GetRect(This)	\
    (This)->lpVtbl -> GetRect(This)

#define IMessageList_SetRect(This)	\
    (This)->lpVtbl -> SetRect(This)

#define IMessageList_HasFocus(This)	\
    (This)->lpVtbl -> HasFocus(This)

#define IMessageList_OnPopupMenu(This)	\
    (This)->lpVtbl -> OnPopupMenu(This)

#define IMessageList_GetSelected(This)	\
    (This)->lpVtbl -> GetSelected(This)

#define IMessageList_GetSelectedCount(This,__MIDL_0024)	\
    (This)->lpVtbl -> GetSelectedCount(This,__MIDL_0024)

#define IMessageList_GetMessageA(This)	\
    (This)->lpVtbl -> GetMessageA(This)

#define IMessageList_GetMessageInfo(This)	\
    (This)->lpVtbl -> GetMessageInfo(This)

#define IMessageList_GetRowFolderId(This)	\
    (This)->lpVtbl -> GetRowFolderId(This)

#define IMessageList_MarkMessage(This)	\
    (This)->lpVtbl -> MarkMessage(This)

#define IMessageList_FreeMessageInfo(This)	\
    (This)->lpVtbl -> FreeMessageInfo(This)

#define IMessageList_MarkRead(This)	\
    (This)->lpVtbl -> MarkRead(This)

#define IMessageList_GetMessageTable(This,__MIDL_0025)	\
    (This)->lpVtbl -> GetMessageTable(This,__MIDL_0025)

#define IMessageList_GetListSelector(This)	\
    (This)->lpVtbl -> GetListSelector(This)

#define IMessageList_GetMessageCounts(This)	\
    (This)->lpVtbl -> GetMessageCounts(This)

#define IMessageList_GetMessageServer(This)	\
    (This)->lpVtbl -> GetMessageServer(This)

#define IMessageList_GetFocusedItemState(This)	\
    (This)->lpVtbl -> GetFocusedItemState(This)

#define IMessageList_ProcessReceipt(This)	\
    (This)->lpVtbl -> ProcessReceipt(This)

#define IMessageList_GetAdBarUrl(This)	\
    (This)->lpVtbl -> GetAdBarUrl(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMessageList_CreateList_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_CreateList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_SetFolder_Proxy( 
    IMessageList * This,
    DWORD folderId,
    DWORD __MIDL_0020,
    DWORD __MIDL_0021,
    DWORD __MIDL_0022,
    DWORD __MIDL_0023);


void __RPC_STUB IMessageList_SetFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_SetViewOptions_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_SetViewOptions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetViewOptions_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetViewOptions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_OnClose_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_OnClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetRect_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_SetRect_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_SetRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_HasFocus_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_HasFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_OnPopupMenu_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_OnPopupMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetSelected_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetSelected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetSelectedCount_Proxy( 
    IMessageList * This,
    /* [out] */ DWORD *__MIDL_0024);


void __RPC_STUB IMessageList_GetSelectedCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetMessageA_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetMessageA_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetMessageInfo_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetMessageInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetRowFolderId_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetRowFolderId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_MarkMessage_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_MarkMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_FreeMessageInfo_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_FreeMessageInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_MarkRead_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_MarkRead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetMessageTable_Proxy( 
    IMessageList * This,
    /* [out] */ IMessageTable **__MIDL_0025);


void __RPC_STUB IMessageList_GetMessageTable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetListSelector_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetListSelector_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetMessageCounts_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetMessageCounts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetMessageServer_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetMessageServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetFocusedItemState_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetFocusedItemState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_ProcessReceipt_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_ProcessReceipt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageList_GetAdBarUrl_Proxy( 
    IMessageList * This);


void __RPC_STUB IMessageList_GetAdBarUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMessageList_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_msgtable_0259 */
/* [local] */ 

#endif // __MESSAGELIST_DEFINED
#ifndef __MESSAGELISTWMAIL_DEFINED
#define __MESSAGELISTWMAIL_DEFINED


extern RPC_IF_HANDLE __MIDL_itf_msgtable_0259_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_msgtable_0259_v0_0_s_ifspec;

#ifndef __IMessageListWMail_INTERFACE_DEFINED__
#define __IMessageListWMail_INTERFACE_DEFINED__

/* interface IMessageListWMail */
/* [unique][uuid][object] */ 

typedef IMessageListWMail *LPMESSAGELISTWMAIL;


EXTERN_C const IID IID_IMessageListWMail;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51A0B0D3-7AFE-4081-BF82-F87AE36373DD")
    IMessageListWMail : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateList( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFolder( 
            DWORD folderId,
            DWORD __MIDL_0026,
            DWORD __MIDL_0027,
            DWORD __MIDL_0028,
            DWORD __MIDL_0029,
            DWORD __MIDL_0030) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetViewOptions( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetViewOptions( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE sub_unknown( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClose( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HasFocus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPopupMenu( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelected( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelectedCount( 
            /* [out] */ DWORD *__MIDL_0031) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageA( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageInfo( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRowFolderId( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MarkMessage( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FreeMessageInfo( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MarkRead( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageTable( 
            /* [out] */ IMessageTable **__MIDL_0032) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetListSelector( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageCounts( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMessageServer( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFocusedItemState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessReceipt( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAdBarUrl( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessageListWMailVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMessageListWMail * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMessageListWMail * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateList )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFolder )( 
            IMessageListWMail * This,
            DWORD folderId,
            DWORD __MIDL_0026,
            DWORD __MIDL_0027,
            DWORD __MIDL_0028,
            DWORD __MIDL_0029,
            DWORD __MIDL_0030);
        
        HRESULT ( STDMETHODCALLTYPE *SetViewOptions )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetViewOptions )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *sub_unknown )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnClose )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRect )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetRect )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *HasFocus )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnPopupMenu )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelected )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelectedCount )( 
            IMessageListWMail * This,
            /* [out] */ DWORD *__MIDL_0031);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageA )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageInfo )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRowFolderId )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *MarkMessage )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *FreeMessageInfo )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *MarkRead )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageTable )( 
            IMessageListWMail * This,
            /* [out] */ IMessageTable **__MIDL_0032);
        
        HRESULT ( STDMETHODCALLTYPE *GetListSelector )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageCounts )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMessageServer )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetFocusedItemState )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessReceipt )( 
            IMessageListWMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetAdBarUrl )( 
            IMessageListWMail * This);
        
        END_INTERFACE
    } IMessageListWMailVtbl;

    interface IMessageListWMail
    {
        CONST_VTBL struct IMessageListWMailVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessageListWMail_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMessageListWMail_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMessageListWMail_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMessageListWMail_CreateList(This)	\
    (This)->lpVtbl -> CreateList(This)

#define IMessageListWMail_SetFolder(This,folderId,__MIDL_0026,__MIDL_0027,__MIDL_0028,__MIDL_0029,__MIDL_0030)	\
    (This)->lpVtbl -> SetFolder(This,folderId,__MIDL_0026,__MIDL_0027,__MIDL_0028,__MIDL_0029,__MIDL_0030)

#define IMessageListWMail_SetViewOptions(This)	\
    (This)->lpVtbl -> SetViewOptions(This)

#define IMessageListWMail_GetViewOptions(This)	\
    (This)->lpVtbl -> GetViewOptions(This)

#define IMessageListWMail_sub_unknown(This)	\
    (This)->lpVtbl -> sub_unknown(This)

#define IMessageListWMail_OnClose(This)	\
    (This)->lpVtbl -> OnClose(This)

#define IMessageListWMail_GetRect(This)	\
    (This)->lpVtbl -> GetRect(This)

#define IMessageListWMail_SetRect(This)	\
    (This)->lpVtbl -> SetRect(This)

#define IMessageListWMail_HasFocus(This)	\
    (This)->lpVtbl -> HasFocus(This)

#define IMessageListWMail_OnPopupMenu(This)	\
    (This)->lpVtbl -> OnPopupMenu(This)

#define IMessageListWMail_GetSelected(This)	\
    (This)->lpVtbl -> GetSelected(This)

#define IMessageListWMail_GetSelectedCount(This,__MIDL_0031)	\
    (This)->lpVtbl -> GetSelectedCount(This,__MIDL_0031)

#define IMessageListWMail_GetMessageA(This)	\
    (This)->lpVtbl -> GetMessageA(This)

#define IMessageListWMail_GetMessageInfo(This)	\
    (This)->lpVtbl -> GetMessageInfo(This)

#define IMessageListWMail_GetRowFolderId(This)	\
    (This)->lpVtbl -> GetRowFolderId(This)

#define IMessageListWMail_MarkMessage(This)	\
    (This)->lpVtbl -> MarkMessage(This)

#define IMessageListWMail_FreeMessageInfo(This)	\
    (This)->lpVtbl -> FreeMessageInfo(This)

#define IMessageListWMail_MarkRead(This)	\
    (This)->lpVtbl -> MarkRead(This)

#define IMessageListWMail_GetMessageTable(This,__MIDL_0032)	\
    (This)->lpVtbl -> GetMessageTable(This,__MIDL_0032)

#define IMessageListWMail_GetListSelector(This)	\
    (This)->lpVtbl -> GetListSelector(This)

#define IMessageListWMail_GetMessageCounts(This)	\
    (This)->lpVtbl -> GetMessageCounts(This)

#define IMessageListWMail_GetMessageServer(This)	\
    (This)->lpVtbl -> GetMessageServer(This)

#define IMessageListWMail_GetFocusedItemState(This)	\
    (This)->lpVtbl -> GetFocusedItemState(This)

#define IMessageListWMail_ProcessReceipt(This)	\
    (This)->lpVtbl -> ProcessReceipt(This)

#define IMessageListWMail_GetAdBarUrl(This)	\
    (This)->lpVtbl -> GetAdBarUrl(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMessageListWMail_CreateList_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_CreateList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_SetFolder_Proxy( 
    IMessageListWMail * This,
    DWORD folderId,
    DWORD __MIDL_0026,
    DWORD __MIDL_0027,
    DWORD __MIDL_0028,
    DWORD __MIDL_0029,
    DWORD __MIDL_0030);


void __RPC_STUB IMessageListWMail_SetFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_SetViewOptions_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_SetViewOptions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetViewOptions_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetViewOptions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_sub_unknown_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_sub_unknown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_OnClose_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_OnClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetRect_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_SetRect_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_SetRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_HasFocus_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_HasFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_OnPopupMenu_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_OnPopupMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetSelected_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetSelected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetSelectedCount_Proxy( 
    IMessageListWMail * This,
    /* [out] */ DWORD *__MIDL_0031);


void __RPC_STUB IMessageListWMail_GetSelectedCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetMessageA_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetMessageA_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetMessageInfo_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetMessageInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetRowFolderId_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetRowFolderId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_MarkMessage_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_MarkMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_FreeMessageInfo_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_FreeMessageInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_MarkRead_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_MarkRead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetMessageTable_Proxy( 
    IMessageListWMail * This,
    /* [out] */ IMessageTable **__MIDL_0032);


void __RPC_STUB IMessageListWMail_GetMessageTable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetListSelector_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetListSelector_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetMessageCounts_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetMessageCounts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetMessageServer_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetMessageServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetFocusedItemState_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetFocusedItemState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_ProcessReceipt_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_ProcessReceipt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMessageListWMail_GetAdBarUrl_Proxy( 
    IMessageListWMail * This);


void __RPC_STUB IMessageListWMail_GetAdBarUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMessageListWMail_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_msgtable_0260 */
/* [local] */ 

#endif // __MESSAGELISTWMAIL_DEFINED


extern RPC_IF_HANDLE __MIDL_itf_msgtable_0260_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_msgtable_0260_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long *, unsigned long            , HWND * ); 
unsigned char * __RPC_USER  HWND_UserMarshal(  unsigned long *, unsigned char *, HWND * ); 
unsigned char * __RPC_USER  HWND_UserUnmarshal(unsigned long *, unsigned char *, HWND * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long *, HWND * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



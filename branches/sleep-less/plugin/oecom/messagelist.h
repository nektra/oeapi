/* $Id: messagelist.h,v 1.2.14.1 2007/08/08 22:39:36 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef OEAPI_MESSAGE_LIST_H____
#define OEAPI_MESSAGE_LIST_H____


#include "nkt_utils.h"

#include "msgtable.h"
#include "oeundoc.h"
#include "nkt_hook.h"

//----------------------------------------------------------------------------
typedef HRESULT (STDMETHODCALLTYPE *NktSetFolder)(NktIMessageList* , DWORD , DWORD , DWORD , DWORD , DWORD );
typedef HRESULT (STDMETHODCALLTYPE *NktSetFolderWMail)(NktIMessageList* , DWORD , DWORD , DWORD , DWORD , DWORD , DWORD );

class OEAPIMessageList {
public:
	OEAPIMessageList();
	~OEAPIMessageList();

	void Init();
	void Uninit();

protected:
	HRESULT GetMessageList(IUnknown** msgList);
	static HRESULT STDMETHODCALLTYPE SetFolderHook(NktIMessageList* , DWORD , DWORD , DWORD , DWORD , DWORD );
	static HRESULT STDMETHODCALLTYPE SetFolderHookWMail(NktIMessageList* , DWORD , DWORD, DWORD , DWORD , DWORD , DWORD );

private:
	NktMethodHooker<NktIMessageList> _hook;
	static NktSetFolder _oldSetFolder;
	static NktSetFolderWMail _oldSetFolderWMail;
};

typedef CountedPtr<OEAPIMessageList> OEAPIMessageListPtr;

#endif // OEAPI_MESSAGE_LIST_H____


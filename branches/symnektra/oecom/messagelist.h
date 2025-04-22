/* $Id: messagelist.h,v 1.9 2008/09/07 16:56:52 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef OEAPI_MESSAGE_LIST_H____
#define OEAPI_MESSAGE_LIST_H____


#include "nkt_utils.h"

#include "msgtable.h"
#include "oeundoc.h"
#include "nkt_hook.h"

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------

class NktMessageList
{
public:
	NktMessageList() {};
	virtual ~NktMessageList() {};

	virtual void SetHook() = 0;
	virtual void RemoveHook() = 0;

	virtual HRESULT GetMessageList(IUnknown** msgList) = 0;
	virtual void ReleaseObjects() = 0;
	virtual void SetMessageList(IUnknown* msgList) = 0;
	virtual DWORD GetMessageIndex(DWORD msgId) = 0;
	virtual DWORD GetMessageId(DWORD index) = 0;
	virtual BOOL IsNull() = 0;

protected:
	NktMethodHooker<NktIMessageList> _hook;
};


//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------

class OEAPIMessageList {
public:
	OEAPIMessageList();
	~OEAPIMessageList();

	void Init();
	void Uninit();

	void SetMessageList(IUnknown* msgList);
	void ReleaseObjects();

	BOOL IsNull();

	DWORD GetMessageId(DWORD index);
	DWORD GetMessageIndex(DWORD msgId);

//protected:
	//HRESULT GetMessageList(IUnknown** msgList);


private:
	//NktMethodHooker<NktIMessageList> _hook;
	//static NktSetFolder _oldSetFolder;
	//static NktSetFolderWMail _oldSetFolderWMail;
	NktMessageList* _msgList;
};

typedef CountedPtr<OEAPIMessageList> OEAPIMessageListPtr;

#endif // OEAPI_MESSAGE_LIST_H____


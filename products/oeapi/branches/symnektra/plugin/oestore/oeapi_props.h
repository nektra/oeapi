/* $Id: oeapi_props.h,v 1.2 2008/09/07 17:26:00 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __OEAPI_PROPS__
#define __OEAPI_PROPS__ 1

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
class NktFolderProps
{
public:
	NktFolderProps() {}
	virtual ~NktFolderProps() {};

	static NktFolderProps* Create(IStoreNamespace* pSN);
	static NktFolderProps* Create(IStoreFolder* pSF);

	virtual HRESULT GetFolderProps() = 0;
	virtual HRESULT GetFirstSubFolder(DWORD folderId, LPHENUMSTORE phEnum) = 0;
	virtual HRESULT GetNextSubFolder(HENUMSTORE hEnum) = 0;

	virtual DWORD GetID() = 0;
	virtual LPCSTR GetName() = 0;
	virtual DWORD GetSubFolders() = 0;
	virtual DWORD GetSpecialID() = 0;
	virtual DWORD GetUnreadCount() = 0;
	virtual DWORD GetMessageCount() = 0;
};

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
class NktMessageProps
{
public:
	NktMessageProps() {}
	virtual ~NktMessageProps() {};

	static NktMessageProps* Create(IStoreFolder* pSF);

	virtual HRESULT GetMessageProps(DWORD msgId, DWORD flags) = 0;
	virtual HRESULT GetFirstMessage(DWORD flags, DWORD first, HENUMSTORE* hEnum) = 0;
	virtual HRESULT GetNextMessage(HENUMSTORE hEnum, DWORD flags) = 0;

	virtual DWORD GetID() = 0;
	virtual LPCSTR GetSubject() = 0;
	virtual LPCSTR GetNormalSubject() = 0;
	virtual LPCSTR GetDisplayTo() = 0;
	virtual LPCSTR GetDisplayFrom() = 0;
	virtual DWORD GetState() = 0;
	virtual DWORD GetSourceLength() = 0;
	virtual DWORD GetPriority() = 0;
};

#endif // __OEAPI_PROPS__
/* $Id: oeapi_props.cpp,v 1.2 2008/09/07 17:26:00 ibejarano Exp $
 *
 * Author: Ismael Bejarano (ismael.bejarano@nektra.com)
 *
 * Copyright (c) 2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"

#include "se_debug.h"
#include "msoeapi.h"
#include "oeundoc.h"
#include "oeapi_utils.h"

#include "oeapi_props.h"

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
template <typename FolderProps>
class NktFolderPropsImpl : public NktFolderProps
{
public:
	NktFolderPropsImpl(IStoreFolder* pSF);
	NktFolderPropsImpl(IStoreNamespace* pSN);

	virtual ~NktFolderPropsImpl();

	void Release();

	HRESULT GetFolderProps();
	HRESULT GetFirstSubFolder(DWORD folderId, LPHENUMSTORE phEnum);
	HRESULT GetNextSubFolder(HENUMSTORE hEnum);

	DWORD GetID();
	LPCSTR GetName();
	DWORD GetSubFolders();
	DWORD GetSpecialID();
	DWORD GetUnreadCount();
	DWORD GetMessageCount();

private:
	IStoreNamespace* _pSN;
	IStoreFolder* _pSF;
	FolderProps _props;
};

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
NktFolderProps* NktFolderProps::Create(IStoreFolder* pSF)
{
	NktFolderProps* ret = NULL;
#ifdef _WIN64
	if(IsWMail()) {
		ret = new NktFolderPropsImpl<FOLDERPROPS>(pSF);
	}
	else {
		ret = new NktFolderPropsImpl<FOLDERPROPS_OE64>(pSF);
	}
#else
	ret = new NktFolderPropsImpl<FOLDERPROPS>(pSF);
#endif
	return ret;
}

//---------------------------------------------------------------------------//
NktFolderProps* NktFolderProps::Create(IStoreNamespace* pSN)
{
	NktFolderProps* ret = NULL;
#ifdef _WIN64
	if(IsWMail()) {
		ret = new NktFolderPropsImpl<FOLDERPROPS>(pSN);
	}
	else {
		ret = new NktFolderPropsImpl<FOLDERPROPS_OE64>(pSN);
	}
#else
	ret = new NktFolderPropsImpl<FOLDERPROPS>(pSN);
#endif
	return ret;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
NktFolderPropsImpl<FolderProps>::NktFolderPropsImpl(IStoreFolder* pSF)
:_pSN(NULL), _pSF(pSF)
{
	if(_pSF) {
		_pSF->AddRef();
	}
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
NktFolderPropsImpl<FolderProps>::NktFolderPropsImpl(IStoreNamespace* pSN)
:_pSN(pSN), _pSF(NULL)
{
	if(_pSN) {
		_pSN->AddRef();
	}
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
NktFolderPropsImpl<FolderProps>::~NktFolderPropsImpl()
{
	Release();
	if(_pSN) {
		_pSN->Release();
		_pSN = NULL;
	}
	if(_pSF) {
		_pSF->Release();
		_pSF = NULL;
	}
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
void NktFolderPropsImpl<FolderProps>::Release()
{
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
DWORD NktFolderPropsImpl<FolderProps>::GetID()
{
	return _props.dwFolderId;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
LPCSTR NktFolderPropsImpl<FolderProps>::GetName()
{
	return _props.szName;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
DWORD NktFolderPropsImpl<FolderProps>::GetSubFolders()
{
	return _props.cSubFolders;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
DWORD NktFolderPropsImpl<FolderProps>::GetSpecialID()
{
	return _props.sfType;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
DWORD NktFolderPropsImpl<FolderProps>::GetUnreadCount()
{
	return _props.cUnread;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
DWORD NktFolderPropsImpl<FolderProps>::GetMessageCount()
{
	return _props.cMessage;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
HRESULT NktFolderPropsImpl<FolderProps>::GetFolderProps()
{
	Release();
	HRESULT hr;
	ZeroMemory(&_props, sizeof(_props));
	_props.cbSize = sizeof(_props);
	hr = _pSF->GetFolderProps(0, (LPFOLDERPROPS)&_props);
	if(hr != S_OK) {
		if(hr != S_FALSE) {
			debug_print(DEBUG_ERROR, _T("NktFolderPropsImpl::GetFolderProps: GetFolderProps failed %08x.\n"), hr);
		}
	}
	return hr;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
HRESULT NktFolderPropsImpl<FolderProps>::GetFirstSubFolder(DWORD folderId, LPHENUMSTORE phEnum)
{
	Release();
	HRESULT hr;
	ZeroMemory(&_props, sizeof(_props));
	_props.cbSize = sizeof(_props);
	hr = _pSN->GetFirstSubFolder(folderId, (LPFOLDERPROPS)&_props, phEnum);
	if(hr != S_OK) {
		if(hr != S_FALSE) {
			debug_print(DEBUG_ERROR, _T("NktFolderPropsImpl::GetFirstSubFolder: GetFirstSubFolder failed %08x.\n"), hr);
		}
	}
	return hr;
}

//---------------------------------------------------------------------------//
template <typename FolderProps>
HRESULT NktFolderPropsImpl<FolderProps>::GetNextSubFolder(HENUMSTORE hEnum)
{
	Release();
	HRESULT hr;
	ZeroMemory(&_props, sizeof(_props));
	_props.cbSize = sizeof(_props);
	hr = _pSN->GetNextSubFolder(hEnum, (LPFOLDERPROPS)&_props);
	if(hr != S_OK) {
		if(hr != S_FALSE) {
			debug_print(DEBUG_ERROR, _T("NktFolderPropsImpl::GetNextSubFolder: GetNextSubFolder failed %08x.\n"), hr);
		}
	}
	return hr;
}


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
template <typename MessageProps>
class NktMessagePropsImpl : public NktMessageProps
{
public:
	NktMessagePropsImpl(IStoreFolder* pSF);
	virtual ~NktMessagePropsImpl();

	void Release();

	HRESULT GetMessageProps(DWORD msgId, DWORD flags);
	HRESULT GetFirstMessage(DWORD flags, DWORD first, HENUMSTORE* hEnum);
	HRESULT GetNextMessage(HENUMSTORE hEnum, DWORD flags);

	DWORD GetID();
	LPCSTR GetSubject();
	LPCSTR GetNormalSubject();
	LPCSTR GetDisplayTo();
	LPCSTR GetDisplayFrom();
	DWORD GetState();
	DWORD GetSourceLength();
	DWORD GetPriority();
	
private:
	IStoreFolder* _pSF;
	MessageProps _props;
};

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
NktMessageProps* NktMessageProps::Create(IStoreFolder* pSF)
{
	NktMessageProps* ret = NULL;
#ifdef _WIN64
	if(IsWMail()) {
		ret = new NktMessagePropsImpl<MESSAGEPROPS>(pSF);
	}
	else {
		ret = new NktMessagePropsImpl<MESSAGEPROPS_OE64>(pSF);
	}
#else
	ret = new NktMessagePropsImpl<MESSAGEPROPS>(pSF);
#endif
	return ret;
}

//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////

template <typename MessageProps>
NktMessagePropsImpl<MessageProps>::NktMessagePropsImpl(IStoreFolder* pSF)
:_pSF(pSF)
{
	if(_pSF) {
		_pSF->AddRef();
	}
	ZeroMemory(&_props, sizeof(_props));
};

//---------------------------------------------------------------------------//
template <typename MessageProps>
NktMessagePropsImpl<MessageProps>::~NktMessagePropsImpl()
{
	Release();
	if(_pSF) {
		_pSF->Release();
		_pSF = NULL;
	}
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
void NktMessagePropsImpl<MessageProps>::Release()
{
	if(_pSF) {
		_pSF->FreeMessageProps((LPMESSAGEPROPS)&_props);
	}
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
DWORD NktMessagePropsImpl<MessageProps>::GetID()
{
	return _props.dwMessageId;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
LPCSTR NktMessagePropsImpl<MessageProps>::GetSubject()
{
	return _props.pszSubject;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
LPCSTR NktMessagePropsImpl<MessageProps>::GetNormalSubject()
{
	return _props.pszNormalSubject;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
LPCSTR NktMessagePropsImpl<MessageProps>::GetDisplayTo()
{
	return _props.pszDisplayTo;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
LPCSTR NktMessagePropsImpl<MessageProps>::GetDisplayFrom()
{
	return _props.pszDisplayFrom;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
DWORD NktMessagePropsImpl<MessageProps>::GetState()
{
	return _props.dwState;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
DWORD NktMessagePropsImpl<MessageProps>::GetSourceLength()
{
	return _props.cbMessage;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
DWORD NktMessagePropsImpl<MessageProps>::GetPriority()
{
	return _props.priority;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
HRESULT NktMessagePropsImpl<MessageProps>::GetMessageProps(DWORD msgId, DWORD flags)
{
	Release();
	HRESULT hr;
	ZeroMemory(&_props, sizeof(_props));
	_props.cbSize = sizeof(_props);
	hr = _pSF->GetMessageProps(msgId, flags, (LPMESSAGEPROPS)&_props);
	if(hr != S_OK) {
		debug_print(DEBUG_ERROR, _T("NktMessagePropsImpl::GetMessageProps: GetMessageProps failed %08x.\n"), hr);
	}
	return hr;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
HRESULT NktMessagePropsImpl<MessageProps>::GetFirstMessage(DWORD flags, DWORD first, HENUMSTORE* phEnum)
{
	Release();
	HRESULT hr;
	ZeroMemory(&_props, sizeof(_props));
	_props.cbSize = sizeof(_props);
	hr = _pSF->GetFirstMessage(flags, 0, first, (LPMESSAGEPROPS)&_props, phEnum);
	if(hr != S_OK) {
		if(hr != S_FALSE) {
			debug_print(DEBUG_ERROR, _T("NktMessagePropsImpl::GetFirstMessage: GetFirstMessage failed %08x.\n"), hr);
		}
	}
	return hr;
}

//---------------------------------------------------------------------------//
template <typename MessageProps>
HRESULT NktMessagePropsImpl<MessageProps>::GetNextMessage(HENUMSTORE hEnum, DWORD flags)
{
	Release();
	HRESULT hr;
	ZeroMemory(&_props, sizeof(_props));
	_props.cbSize = sizeof(_props);
	hr = _pSF->GetNextMessage(hEnum, flags, (LPMESSAGEPROPS)&_props);
	if(hr != S_OK) {
		if(hr != S_FALSE) {
			debug_print(DEBUG_ERROR, _T("NktMessagePropsImpl::GetNextMessage: GetNextMessage failed %08x.\n"), hr);
		}
	}
	return hr;
}


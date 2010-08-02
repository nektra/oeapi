/* $Id: messagelist.cpp,v 1.20 2008/09/07 16:56:52 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"
#include "messagelist.h"

#include "oeapi_utils.h"
#include "main.h"	// for OEAPIManager


#define NKT_INVALID_FOLDERID_WM ((ULONGLONG)-1LL)

#ifdef _WIN64
#  define NKT_INVALID_FOLDERID_OE ((ULONGLONG)-1LL)
#else
#  define NKT_INVALID_FOLDERID_OE ((DWORD)-1)
#endif

//----------------------------------------------------------------------------

#ifdef _WIN64
typedef HRESULT (STDMETHODCALLTYPE *NktSetFolder)(NktIMessageList* , ULONGLONG , LPVOID , DWORD , LPVOID, LPVOID);
#else
typedef HRESULT (STDMETHODCALLTYPE *NktSetFolder)(NktIMessageList* , DWORD , DWORD , DWORD , DWORD , DWORD );
#endif

#ifdef _WIN64
typedef HRESULT (STDMETHODCALLTYPE *NktSetFolderWMail)(NktIMessageList* , ULONGLONG , LPVOID , DWORD, LPVOID, LPVOID);
#else
typedef HRESULT (STDMETHODCALLTYPE *NktSetFolderWMail)(NktIMessageList* , ULONGLONG, DWORD , DWORD , DWORD , DWORD );
#endif

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------

class NktMessageListOE : public NktMessageList
{
public:
	NktMessageListOE();
	virtual ~NktMessageListOE();

	virtual void SetHook();
	virtual void RemoveHook();

	virtual HRESULT GetMessageList(IUnknown** msgList);
	virtual void ReleaseObjects();
	virtual void SetMessageList(IUnknown* msgList);
	virtual DWORD GetMessageIndex(DWORD msgId);
	virtual DWORD GetMessageId(DWORD index);
	virtual BOOL IsNull();

#ifdef _WIN64
	static HRESULT STDMETHODCALLTYPE SetFolderHook(NktIMessageList* , ULONGLONG , LPVOID , DWORD , LPVOID, LPVOID);
#else
	static HRESULT STDMETHODCALLTYPE SetFolderHook(NktIMessageList* , DWORD , DWORD , DWORD , DWORD , DWORD );
#endif

private:
	IMessageList* _msgList;
	IMessageTable* _msgTable;
	static NktSetFolder _oldSetFolder;
};

//----------------------------------------------------------------------------
NktSetFolder NktMessageListOE::_oldSetFolder = NULL;

//----------------------------------------------------------------------------
NktMessageListOE::NktMessageListOE()
:_msgList(NULL), _msgTable(NULL)
{
}

//----------------------------------------------------------------------------
NktMessageListOE::~NktMessageListOE()
{
	ReleaseObjects();
}

//----------------------------------------------------------------------------
void NktMessageListOE::SetHook()
{
	HRESULT hr;

	IUnknown* msgList = NULL; 
	hr = GetMessageList(&msgList);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("NktMessageListOE::SetHook: GetMessageList %08x.\n"), hr);
		return;
	}

	if(msgList) {
		_oldSetFolder = _hook.Init((NktIMessageList*)msgList, SetFolderHook, &NktIMessageListVtbl::SetFolder);	
		msgList->Release();
	}
}

//----------------------------------------------------------------------------
void NktMessageListOE::RemoveHook()
{
	_hook.Uninit();
}

//----------------------------------------------------------------------------
HRESULT NktMessageListOE::GetMessageList(IUnknown** msgList)
{
	HRESULT hr;
	hr = CoCreateInstance(CLSID_IMessageList, NULL,
		CLSCTX_INPROC_SERVER, IID_IMessageList, (LPVOID*) msgList);
	return hr;
}

//----------------------------------------------------------------------------
void NktMessageListOE::ReleaseObjects()
{
	if(_msgTable) {
		_msgTable->Release();
		_msgTable = NULL;
	}
	if(_msgList) {
		_msgList->Release();
		_msgList = NULL;
	}
}

//----------------------------------------------------------------------------
void NktMessageListOE::SetMessageList(IUnknown* ml)
{
	ReleaseObjects();
	if(ml) {
		HRESULT hr;
		IMessageTable* msgTable = NULL;
		IMessageList* msgList = static_cast<IMessageList*>(ml);
		_msgList = msgList;
		_msgList->AddRef();
		hr = _msgList->GetMessageTable(&msgTable);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("NktMessageListOE::SetMessageList: GetMessageTable failed %08x.\n"), hr);
			return;
		}
		_msgTable = msgTable;
	}
}

//----------------------------------------------------------------------------
DWORD NktMessageListOE::GetMessageIndex(DWORD msgId)
{
	DWORD index = -1;
	if(_msgTable) {
		HRESULT hr;
		hr = _msgTable->GetRowIndex(msgId, &index);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("NktMessageListOE::GetMessageIndex: GetRowIndex failed %08x.\n"), hr);
			return -1;
		}
	}
	return index;
}

//----------------------------------------------------------------------------
DWORD NktMessageListOE::GetMessageId(DWORD index)
{
#ifdef _WIN64
	ULONGLONG msgId = -1;
#else
	DWORD msgId = -1;
#endif
	if(_msgTable) {
		HRESULT hr;
		hr = _msgTable->GetRowMessageId(index, (LPDWORD)&msgId);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("NktMessageListOE::GetMessageId: GetRowMessageId failed %08x.\n"), hr);
			return -1;
		}
	}
	return msgId;
}

//----------------------------------------------------------------------------
BOOL NktMessageListOE::IsNull()
{
	return _msgList == NULL || _msgTable == NULL;
}

//----------------------------------------------------------------------------
#ifndef _WIN64
HRESULT STDMETHODCALLTYPE NktMessageListOE::SetFolderHook(NktIMessageList* msgList, DWORD dwFolderId, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4)
#else
HRESULT STDMETHODCALLTYPE NktMessageListOE::SetFolderHook(NktIMessageList* msgList, ULONGLONG dwFolderId, LPVOID arg1, DWORD arg2, LPVOID arg3, LPVOID arg4)
#endif
{
	HRESULT hr = _oldSetFolder(msgList, dwFolderId, arg1, arg2, arg3, arg4);
	if(SUCCEEDED(hr) && dwFolderId != NKT_INVALID_FOLDERID_OE) {
		OEAPIManager::Get()->SetMsgList((IUnknown*)msgList, dwFolderId);
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------

class NktMessageListWM : public NktMessageList
{
public:
	NktMessageListWM();
	virtual ~NktMessageListWM();

	virtual void SetHook();
	virtual void RemoveHook();

	virtual HRESULT GetMessageList(IUnknown** msgList);
	virtual void ReleaseObjects();
	virtual void SetMessageList(IUnknown* msgList);
	virtual DWORD GetMessageIndex(DWORD msgId);
	virtual DWORD GetMessageId(DWORD index);
	virtual BOOL IsNull();

#ifdef _WIN64
	static HRESULT STDMETHODCALLTYPE SetFolderHook(NktIMessageList* , ULONGLONG , LPVOID , DWORD , LPVOID, LPVOID);
#else
	static HRESULT STDMETHODCALLTYPE SetFolderHook(NktIMessageList* , ULONGLONG , DWORD, DWORD , DWORD , DWORD);
#endif

private:
	IMessageListWMail* _msgList;
	IMessageTableWMail* _msgTable;
	static NktSetFolderWMail _oldSetFolder;
};

//----------------------------------------------------------------------------
NktSetFolderWMail NktMessageListWM::_oldSetFolder = NULL;

//----------------------------------------------------------------------------
NktMessageListWM::NktMessageListWM()
:_msgList(NULL), _msgTable(NULL)
{
}

//----------------------------------------------------------------------------
NktMessageListWM::~NktMessageListWM()
{
	ReleaseObjects();
}

//----------------------------------------------------------------------------
void NktMessageListWM::SetHook()
{
	HRESULT hr;

	IUnknown* msgList = NULL; 
	hr = GetMessageList(&msgList);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("NktMessageListOE::SetHook: GetMessageList %08x.\n"), hr);
		return;
	}

	if(msgList) {
		_oldSetFolder = _hook.Init((NktIMessageList*)msgList, SetFolderHook, &NktIMessageListVtbl::SetFolder);	
		msgList->Release();
	}
}

//----------------------------------------------------------------------------
void NktMessageListWM::RemoveHook()
{
	_hook.Uninit();
}

//----------------------------------------------------------------------------
HRESULT NktMessageListWM::GetMessageList(IUnknown** msgList)
{
	HRESULT hr = E_FAIL;

	IClassFactory* clsFactory;
	TCHAR msoepath[1024];

	// Note: Here we assume that HMODULE == 0 is winmail.exe or msimn.exe
	GetModuleFileName(0, msoepath, sizeof(msoepath)/sizeof(TCHAR));
	TCHAR* p = _tcsrchr(msoepath, '\\');
	*p = '\0';
	_tcscat_s(msoepath, sizeof(msoepath), _T("\\msoe.dll"));
	HINSTANCE hLib = LoadLibrary(msoepath);
	if(hLib == NULL) {
		debug_print(DEBUG_ERROR, _T("NktMessageListWM::GetMessageList: Failed LoadLibrary.\n"));
		return hr;
	}

	LPFNGETCLASSOBJECT pGetClassObject;
	pGetClassObject = (LPFNGETCLASSOBJECT)GetProcAddress(hLib, "DllGetClassObject");
	if(pGetClassObject == NULL) {
		debug_print(DEBUG_ERROR, _T("NktMessageListWM::GetMessageList: Failed GetProcAddress.\n"));
		FreeLibrary(hLib);		
		return hr;
	}

	debug_print(DEBUG_TRACE, _T("NktMessageListWM::GetMessageList: GetProcAddress %08p.\n"), pGetClassObject);

	hr = pGetClassObject(CLSID_IMessageList, IID_IClassFactory, (LPVOID*)&clsFactory);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("NktMessageListWM::GetMessageList: Failed DllGetClassObject.\n"));
		FreeLibrary(hLib);		
		return hr;
	}

	hr = clsFactory->CreateInstance(NULL, IID_IMessageListWMail, (LPVOID*)msgList);
	clsFactory->Release();
	FreeLibrary(hLib);		

	return hr;
}

//----------------------------------------------------------------------------
void NktMessageListWM::ReleaseObjects()
{
	if(_msgTable) {
		_msgTable->Release();
		_msgTable = NULL;
	}
	if(_msgList) {
		_msgList->Release();
		_msgList = NULL;
	}
}

//----------------------------------------------------------------------------
void NktMessageListWM::SetMessageList(IUnknown* ml)
{
	ReleaseObjects();
	if(ml) {
		HRESULT hr;
		IMessageTableWMail* msgTable = NULL;
		IMessageListWMail* msgList = static_cast<IMessageListWMail*>(ml);
		_msgList = msgList;
		_msgList->AddRef();
		hr = _msgList->GetMessageTable((IMessageTable**)&msgTable);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("NktMessageListOE::SetMessageList: GetMessageTable failed %08x.\n"), hr);
			return;
		}
		_msgTable = msgTable;
	}
}

//----------------------------------------------------------------------------
DWORD NktMessageListWM::GetMessageIndex(DWORD msgId)
{
	DWORD index = -1;
	if(_msgTable) {
		HRESULT hr;
		hr = _msgTable->GetRowIndex(msgId, &index);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("NktMessageListOE::GetMessageIndex: GetRowIndex failed %08x.\n"), hr);
			return -1;
		}
	}
	return index;
}

//----------------------------------------------------------------------------
DWORD NktMessageListWM::GetMessageId(DWORD index)
{
	ULONGLONG msgId = -1;
	if(_msgTable) {
		HRESULT hr;
		hr = _msgTable->GetRowMessageId(index, &msgId);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("NktMessageListOE::GetMessageId: GetRowMessageId failed %08x.\n"), hr);
			return -1;
		}
	}
	return msgId;
}

//----------------------------------------------------------------------------
BOOL NktMessageListWM::IsNull()
{
	return _msgList == NULL || _msgTable == NULL;
}

//----------------------------------------------------------------------------
#ifdef _WIN64
HRESULT STDMETHODCALLTYPE NktMessageListWM::SetFolderHook(NktIMessageList* msgList, ULONGLONG dwFolderId, LPVOID arg1, DWORD arg2, LPVOID arg3, LPVOID arg4)
#else
HRESULT STDMETHODCALLTYPE NktMessageListWM::SetFolderHook(NktIMessageList* msgList, ULONGLONG dwFolderId, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4)
#endif
{
	HRESULT hr = _oldSetFolder(msgList, dwFolderId, arg1, arg2, arg3, arg4);
	if(SUCCEEDED(hr) && dwFolderId != NKT_INVALID_FOLDERID_WM) {
		OEAPIManager::Get()->SetMsgList((IUnknown*)msgList, dwFolderId);
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
OEAPIMessageList::OEAPIMessageList()
{
	if(IsWMail()) {
		_msgList = new NktMessageListWM();
	}
	else {
		_msgList = new NktMessageListOE();
	}
}

//----------------------------------------------------------------------------
OEAPIMessageList::~OEAPIMessageList()
{
	Uninit();
	if(_msgList) {
		delete _msgList;
		_msgList = NULL;
	}
}


////----------------------------------------------------------------------------
//HRESULT OEAPIMessageList::GetMessageList(IUnknown** msgList)
//{
//	return _msgList->GetMessageList(msgList);
//}

//----------------------------------------------------------------------------
void OEAPIMessageList::Init()
{
	_msgList->SetHook();

	//IUnknown* msgList = NULL; 
	//HRESULT hr = GetMessageList(&msgList);
	//if(FAILED(hr)) {
	//	debug_print(DEBUG_ERROR, _T("OEAPIMessageList::Init: Failed CoCreateInstance.\n"));
	//	return;
	//}

	//if(msgList) {
	//	if(IsWMail()) {
	//		_oldSetFolderWMail = _hook.Init((NktIMessageList*)msgList, SetFolderHookWMail, &NktIMessageListVtbl::SetFolder);
	//	}
	//	else {
	//		_oldSetFolder = _hook.Init((NktIMessageList*)msgList, SetFolderHook, &NktIMessageListVtbl::SetFolder);
	//	}
	//	msgList->Release();
	//}
}

//----------------------------------------------------------------------------
void OEAPIMessageList::Uninit()
{
	_msgList->RemoveHook();
}

//----------------------------------------------------------------------------
void OEAPIMessageList::ReleaseObjects()
{
	_msgList->ReleaseObjects();
}

//----------------------------------------------------------------------------
void OEAPIMessageList::SetMessageList(IUnknown* msgList)
{
	_msgList->SetMessageList(msgList);
}

//----------------------------------------------------------------------------
DWORD OEAPIMessageList::GetMessageIndex(DWORD msgId)
{
	return _msgList->GetMessageIndex(msgId);
}

//----------------------------------------------------------------------------
DWORD OEAPIMessageList::GetMessageId(DWORD index)
{
	return _msgList->GetMessageId(index);
}

//----------------------------------------------------------------------------
BOOL OEAPIMessageList::IsNull()
{
	return _msgList->IsNull();
}

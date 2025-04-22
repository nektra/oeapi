/* $Id: messagelist.cpp,v 1.13.6.1 2007/08/08 22:39:35 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"
#include "messagelist.h"

#include "oeapi_utils.h"
#include "main.h"	// for OEAPIManager


//DWORD pIMessageList_SetFolder;
//FARPROC pSetFolder_afterPrelude;
//FARPROC pOldSetFolder;


//IMessageListVista *msgList;

//__declspec (naked) void SetFolderPatched()
//{
//	_asm {
//		mov     edi, edi
//		push    ebp
//		mov     ebp, esp
//
//		jmp pSetFolder_afterPrelude
//	}
//}

//FARPROC oldRet;
//DWORD dwFolderId;
//IUnknown *msgList;
//LPVOID vtableSetFolder;
//
//__declspec (naked) void SetFolderPrelude()
//{
//	_asm {
//		cmp [esp+8], -1
//		jne get_msg_list
//
//		jmp [pOldSetFolder]
//
//get_msg_list:
//		push eax
//		mov eax, [esp+8]
//		mov msgList, eax
//		mov eax, [esp+0ch]
//		mov dwFolderId, eax
//		pop eax
//	}
//
//	_asm {
//		push eax
//		mov eax, [esp+4]
//		mov oldRet, eax
//		mov eax, offset retCreateList
//		mov [esp+4], eax
//		pop eax
//
//		jmp [pOldSetFolder]
//
//retCreateList:
//		pushad
//	}
//
//	OEAPIManager::Get()->SetMsgList(msgList, dwFolderId);
//
//	_asm {
//		popad
//		jmp oldRet
//	}
//}

NktSetFolder OEAPIMessageList::_oldSetFolder = NULL;
NktSetFolderWMail OEAPIMessageList::_oldSetFolderWMail = NULL;

//----------------------------------------------------------------------------
OEAPIMessageList::OEAPIMessageList()
{
	//vtableSetFolder = NULL;
}

//----------------------------------------------------------------------------
OEAPIMessageList::~OEAPIMessageList()
{
	Uninit();
	//if(vtableSetFolder) {
	//	DWORD oldProtectionMask, oldProtectionMask2;

	//	if(VirtualProtect(vtableSetFolder, 4, PAGE_EXECUTE_READWRITE, &oldProtectionMask) == FALSE) {
	//		debug_print(DEBUG_ERROR, _T("OEAPIMessageList::~OEAPIMessageList: VirtualProtect\n"));
	//		return;
	//	}

	//	*reinterpret_cast<void**>(vtableSetFolder) = (void*)pOldSetFolder;

	//	//_asm {
	//	//	push eax
	//	//	push edx

	//	//	mov edx, [vtableSetFolder]
	//	//	mov eax, [pOldSetFolder]
	//	//	mov [edx], eax

	//	//	pop edx
	//	//	pop eax
	//	//}

	//	if(VirtualProtect(vtableSetFolder, 4, oldProtectionMask, &oldProtectionMask2) == FALSE) {
	//		debug_print(DEBUG_ERROR, _T("OEAPIMessageList::~OEAPIMessageList: VirtualProtect\n"));
	//		return;
	//	}
	//}
}


//----------------------------------------------------------------------------
HRESULT OEAPIMessageList::GetMessageList(IUnknown** msgList)
{
	HRESULT hr = E_FAIL;
	if(IsWMail()) {
		IClassFactory* clsFactory;
		TCHAR msoepath[1024];

		// Note: Here we assume that HMODULE == 0 is winmail.exe or msimn.exe
		GetModuleFileName(0, msoepath, sizeof(msoepath)/sizeof(TCHAR));
		TCHAR* p = _tcsrchr(msoepath, '\\');
#if _MSC_VER >= 1400
		*p = '\0';
		_tcscat_s(msoepath, sizeof(msoepath), _T("\\msoe.dll"));
#else
		_tcscat(msoepath, _T("\\msoe.dll"));
#endif
		HINSTANCE hLib = LoadLibrary(msoepath);
		if(hLib == NULL) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageList::Init: Failed LoadLibrary.\n"));
			return hr;
		}

		LPFNGETCLASSOBJECT pGetClassObject;
		pGetClassObject = (LPFNGETCLASSOBJECT)GetProcAddress(hLib, _T("DllGetClassObject"));
		if(pGetClassObject == NULL) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageList::Init: Failed GetProcAddress.\n"));
			FreeLibrary(hLib);		
			return hr;
		}

		debug_print(DEBUG_TRACE, _T("OEAPIMessageList::Init: GetProcAddress %08p.\n"), pGetClassObject);

		hr = pGetClassObject(CLSID_IMessageList, IID_IClassFactory, (LPVOID*)&clsFactory);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIMessageList::Init: Failed DllGetClassObject.\n"));
			FreeLibrary(hLib);		
			return hr;
		}

		hr = clsFactory->CreateInstance(NULL, IID_IMessageListWMail, (LPVOID*)msgList);
		clsFactory->Release();
		FreeLibrary(hLib);		
	}
	else {
		hr = CoCreateInstance(CLSID_IMessageList, NULL,
			CLSCTX_INPROC_SERVER, IID_IMessageList, (LPVOID*) msgList);
	}

	return hr;
}

//----------------------------------------------------------------------------
void OEAPIMessageList::Init()
{
	IUnknown* msgList = NULL; 
	HRESULT hr = GetMessageList(&msgList);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIMessageList::Init: Failed CoCreateInstance.\n"));
		return;
	}

	if(msgList) {
		if(IsWMail()) {
			_oldSetFolderWMail = _hook.Init((NktIMessageList*)msgList, SetFolderHookWMail, &NktIMessageListVtbl::SetFolder);
		}
		else {
			_oldSetFolder = _hook.Init((NktIMessageList*)msgList, SetFolderHook, &NktIMessageListVtbl::SetFolder);
		}
		msgList->Release();
	}
}

//----------------------------------------------------------------------------
void OEAPIMessageList::Uninit()
{
	_hook.Uninit();
}

	//	pop edx
	//	pop eax
	//}

//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE OEAPIMessageList::SetFolderHook(NktIMessageList* msgList, DWORD dwFolderId, DWORD dw2, DWORD dw3, DWORD dw4, DWORD dw5)
{
	HRESULT hr = _oldSetFolder(msgList, dwFolderId, dw2, dw3, dw4, dw5);
	if(dwFolderId != (DWORD)-1) {
		OEAPIManager::Get()->SetMsgList((IUnknown*)msgList, dwFolderId);
	}
	return hr;
}

//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE OEAPIMessageList::SetFolderHookWMail(NktIMessageList* msgList, DWORD dwFolderId, DWORD dw1, DWORD dw2, DWORD dw3, DWORD dw4, DWORD dw5)
{
	HRESULT hr = _oldSetFolderWMail(msgList, dwFolderId, dw1, dw2, dw3, dw4, dw5);
	if(dwFolderId != (DWORD)-1) {
		OEAPIManager::Get()->SetMsgList((IUnknown*)msgList, dwFolderId);
	}
	return hr;
}

//
//	// TODO: Check if this works
//	vtableSetFolder = (void*)(*reinterpret_cast<unsigned char**>(msgList) + 0x10);
//	//_asm {
//	//	push eax
//	//	push edx
//
//	//	// get SetFolder address
//	//	mov edx,dword ptr [msgList]
//	//	mov eax,dword ptr [edx]
//
//	//	add eax, 10h
//	//	mov [vtableSetFolder], eax
//
//	//	pop edx
//	//	pop eax
//	//}
//
//
//	msgList->Release();
//
//	DWORD oldProtectionMask, oldProtectionMask2;
//
//	if(VirtualProtect(vtableSetFolder, 4, PAGE_EXECUTE_READWRITE, &oldProtectionMask) == FALSE) {
//		debug_print(DEBUG_ERROR, _T("OEAPIMessageList::Init: VirtualProtect.\n"));
//		return;
//	}
//
//	pOldSetFolder = (FARPROC)*reinterpret_cast<void**>(vtableSetFolder);
//	*reinterpret_cast<void**>(vtableSetFolder) = (void*)SetFolderPrelude;
//
//	//_asm {
//	//	push eax
//	//	push edx
//
//	//	// get SetFolder address
//	//	mov edx, [vtableSetFolder]
//	//	mov eax, [edx]
//	//	mov [pOldSetFolder], eax
//	//	mov eax, offset SetFolderPrelude
//	//	mov [edx], eax
//
//	//	pop edx
//	//	pop eax
//	//}
//
//	if(VirtualProtect(vtableSetFolder, 4, oldProtectionMask, &oldProtectionMask2) == FALSE) {
//		debug_print(DEBUG_ERROR, _T("OEAPIMessageList::Init: VirtualProtect.\n"));
//		return;
//	}
//
//



//	_asm {
//		// get SetFolder address
//		mov edx,dword ptr [msgList]
//		mov eax,dword ptr [edx]
//
//		mov	edi, [eax+10h]
//		mov pIMessageList_SetFolder, edi
//	}
//
//	msgList->Release();
//
//	MEMORY_BASIC_INFORMATION mbiStart, mbiFinish;
//	DWORD oldProtectionMaskStart, oldProtectionMaskFinish;
//
//	VirtualQuery((LPCVOID) pIMessageList_SetFolder, &mbiStart, sizeof(MEMORY_BASIC_INFORMATION));
//	if(VirtualProtect(mbiStart.AllocationBase, mbiStart.RegionSize,
//					PAGE_EXECUTE_READWRITE, &oldProtectionMaskStart) == FALSE) {
//		debug_print(DEBUG_ERROR, _T("StartServer: VirtualProtect\n"));
//		return;
//	}
//
//	VirtualQuery((LPCVOID) (pIMessageList_SetFolder+4), &mbiFinish, sizeof(MEMORY_BASIC_INFORMATION));
//	if(VirtualProtect(mbiFinish.AllocationBase, mbiFinish.RegionSize,
//					PAGE_EXECUTE_READWRITE, &oldProtectionMaskFinish) == FALSE) {
//		debug_print(DEBUG_ERROR, _T("StartServer: VirtualProtect\n"));
//		return;
//	}
//
//	_asm {
//		mov edi, pIMessageList_SetFolder
//
//		mov byte PTR [edi], 0xe9
//		mov eax, edi
//		add eax, 5
//		mov ecx, offset SetFolderPrelude
//		sub ecx, eax
//		inc edi
//		mov [edi], ecx
//		add edi, 4
//		mov pSetFolder_afterPrelude, edi
//	}
//
//	if(VirtualProtect(mbiStart.AllocationBase, mbiStart.RegionSize,
//					PAGE_EXECUTE_READWRITE, &oldProtectionMaskStart) == FALSE) {
//		debug_print(DEBUG_ERROR, _T("StartServer: VirtualProtect\n"));
//		return;
//	}
//
//	if(VirtualProtect(mbiFinish.AllocationBase, mbiFinish.RegionSize,
//					PAGE_EXECUTE_READWRITE, &oldProtectionMaskFinish) == FALSE) {
//		debug_print(DEBUG_ERROR, _T("StartServer: VirtualProtect\n"));
//		return;
//	}
//}



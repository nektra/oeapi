/* $Id: main.cpp,v 1.94 2009/04/21 17:57:53 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"

#include "oeapiobj.h"
#include "user_interface.h"
#include "oemsgwnd.h"

#include "oe_toolbar.h"
#include "oe_msgwnd.h"
#include "oe_menu.h"

#include <string>
#include <sstream>

#include "oeapi_utils.h"

#include <initguid.h>
#include "mimeole.h"
#include "msgtable.h"

#include "OEAPIINITCOM.h"
#include "nkt_registry.h"

#include <GetInitInstance.h>

#ifndef STATIC_LIBRARY

#include <comet/comet.h>
#include <comet/cmd_line_parser.h>
#include <comet/exe_server.h>


//#pragma warning(disable : 4800)

//using namespace OEAPI;
//using namespace comet;

typedef comet::com_server< comet::OEAPI::type_library > SERVER;
#endif // STATIC_LIBRARY

HWND g_oehWnd = NULL;

///////////////////////////////////////////////////////////////////////////
#include <list>

#include "main.h"
#include "se_debug.h"
#include "oeapi_event.h"

#include "resource.h"

#ifdef EVALUATION_VERSION
#include "eval_utils.h"
#endif // EVALUATION_VERSION

// there is a auto-generated oeapi.h in this project, so we must specify the path
#include "../lib/oeapi.h"

static HANDLE hServer = NULL;
static DWORD serverID;


using namespace comet;
using namespace OEAPI;


// HINSTANCE of the dll
HINSTANCE hInstOecom = NULL;

// OEAPIInit object
com_ptr<OEAPIINITCOM::IOEAPIInitState> g_OEAPIInitState;
//com_ptr<OEAPIINITCOM::IOEAPIInit> g_OEAPIInit;

// saves the number of OEAPIInit objects that have already finished initialization.
long g_initializedCount;

HWND hWnd = NULL;

//HWND hWndDebug = NULL;

const UINT BUTTON_CLICKED_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnButtonClick"));
const UINT MENU_ITEM_CLICKED_MESSAGE_CODE = RegisterWindowMessage(_T("OEAPI.OnMenuItemClick"));
const UINT MESSAGE_SELECTION_CHANGED_CODE = RegisterWindowMessage(_T("OEAPI.OnMessageSelectionChanged"));
const UINT MESSAGE_CURRENT_CHANGED_CODE = RegisterWindowMessage(_T("OEAPI.OnCurrentMessageChanged"));
const UINT MESSAGE_DOWNLOAD_CODE = RegisterWindowMessage(_T("OEAPI.OnMessageDownload"));
const UINT MSG_WND_CREATION_CODE = RegisterWindowMessage(_T("OEAPI.OnMsgWndCreation"));
const UINT MSG_WND_CLOSED_CODE = RegisterWindowMessage(_T("OEAPI.OnMsgWndClosed"));
const UINT MSG_WND_SEND_BUTTON_CODE = RegisterWindowMessage(_T("OEAPI.OnMsgWndSendButton"));
const UINT DATABASE_TRANSACTION_CODE = RegisterWindowMessage(_T("OEAPI.OnDatabaseChange"));

// This code depends on WM/OE
const int DownloadFinishedCode = IsWMail() ? 14 : 13;

//#define OEAPI_AFTER_THREAD_EXIT_EVENT_NAME _T("OEAPI_AFTER_THREAD_EXIT_EVENT")
//#define OEAPI_AFTER_ISTORENAMESPACE_RELEASE_EVENT_NAME _T("OEAPI_AFTER_ISTORENAMESPACE_RELEASE_EVENT")

//#define OEAPI_PLUGINS_INITIALIZED_EVENT_NAME _T("OEAPI_PLUGINS_INITIALIZED_EVENT")


INT __cdecl ExitServerCallback(BOOL logOff);

OEAPIManager *pThis = NULL;

LRESULT CALLBACK OEAPIWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//#define DEBUG_MESSAGE_CODE WM_USER + 0x1000

//LRESULT CALLBACK DebugWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if(uMsg == DEBUG_MESSAGE_CODE) {
//		OEAPIManager::Get()->DestroyMsgTables();
//	}
//
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}


// Create a window
HWND CreateCallbackWindow();

void SetOecomLoaded(DWORD loaded);


#define SEND_MSG_CODE 0xaa4
#define SEND_MSG_LPARAM 0xc20111
#define RECV_MSG_CODE 0xaa8
#define RECV_MSG_LPARAM 0x0f9f30f
#define SNDRCV_MSG_LPARAM 0x0f5e389

// These 2 message codes are sent as a parameter of the 'Send/Recv' Command in the
// SendDlgWnd when the user presses the button.
#define OE_SHOW_SEND_RECV_WND 0x00f5e348
#define OE_SHOW_SEND_RECV_WND2 0x00f5e308
#define OE_SHOW_SEND_RECV_PROGRESS 0x060

#define OE_WM_CMD_SENDRECV 40193
#define OE_WM_CMD_RECVALL 40194
#define OE_WM_CMD_SENDALL 40195

#define OE_INVALID_FOLDER_ID -1
#define OE_LOCAL_FOLDER_STORE_ID 0
#define OE_LOCAL_FOLDER_UI_ID 1			// 'Local Folders' has id 1 at UI level but 0 at storage level
#define OE_OUTLOOK_EXPRESS_FOLDER_ID 0	// 'Outlook Express' folder doesn't exist at storage level but has id 0 at UI level


//#define MSG_COLUMN_TO 0
//#define MSG_COLUMN_FROM 1
//#define MSG_COLUMN_SUBJECT 2
//#define MSG_COLUMN_RECEIVED 3
//#define MSG_COLUMN_SENT 4
//#define MSG_COLUMN_SIZE 5
//#define MSG_COLUMN_ACCOUNT 0xd
//#define MSG_COLUMN_PRIORITY 0xf
//#define MSG_COLUMN_ATTACH 0x10
//#define MSG_COLUMN_FLAG 0x14
//#define MSG_COLUMN_WATCH 0x17

typedef struct tagButtonInfo {
	INT buttonId;
	INT toolbarId;
	HWND wndMsgId;
} ButtonInfo;

typedef struct tagMenuInfo {
	INT menuItemId;
	INT menuIndex;
	HWND wndMsgId;
} MenuItemInfo;


#ifdef ENTERPRISE_VERSION

//LRESULT CALLBACK OutboxWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//HWND CreateOutboxCallbackWindow()
//{
//	WNDCLASSEX wndclass;
//	const TCHAR szFolderSink[] = _T("OEAPI Outbox sink");
//	HWND hWnd;
//
//	wndclass.cbSize			= sizeof(wndclass);
//	wndclass.style			= 0;
//	wndclass.lpfnWndProc	= &OutboxWndProc;
//	wndclass.cbClsExtra		= 0;
//	wndclass.cbWndExtra		= 0;
//	wndclass.hInstance		= hInstOecom;
//	wndclass.hIcon			= NULL;
//	wndclass.hCursor		= NULL;
//	wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
//	wndclass.lpszMenuName	= NULL;
//	wndclass.lpszClassName	= szFolderSink;
//	wndclass.hIconSm		= NULL;
//
//	// Register it
//	RegisterClassEx(&wndclass);
//
//	// And create a window
//	hWnd = CreateWindow(szFolderSink,
//				_T("Folder"),
//				WS_OVERLAPPEDWINDOW,
//				CW_USEDEFAULT,
//				CW_USEDEFAULT,
//				400, 200,
//				NULL,
//				NULL,
//				hInstOecom,
//				NULL);
//
//	if (hWnd == NULL) {
//		debug_print(DEBUG_ERROR, _T("CreateOutboxCallbackWindow: CreateWindow() failed.\n"));
//		return NULL;
//	}
//
//	return hWnd;
//}

//DWORD GetFolderMaxMsgID(IStoreFolder *folder)
//{
//	HENUMSTORE hEnum;
//	HRESULT hr;
//	MESSAGEPROPS msgProps;
//	DWORD maxId = -1;
//
//	if(folder) {
//		msgProps.cbSize = sizeof(MESSAGEPROPS);
//
//		hr = folder->GetFirstMessage(MSGPROPS_FAST, 0, MESSAGEID_FIRST, &msgProps, &hEnum);
//
//		while(SUCCEEDED(hr) && hr != S_FALSE) {
//			maxId = msgProps.dwMessageId;
//
//			folder->FreeMessageProps(&msgProps);
//
//			hr = folder->GetNextMessage(hEnum, MSGPROPS_FAST, &msgProps);
//		}
//
//		if(SUCCEEDED(hr)) {
//			folder->GetMessageClose(hEnum);
//		}
//	}
//
//	return maxId;
//}

#endif // ENTERPRISE_VERSION


//BOOL g_sendDlgSyncblocked = FALSE;

//static INT msgList[1024];
//static INT msgCount = 0;

LRESULT CALLBACK SendDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpOldProc;
	lpOldProc = (WNDPROC) OEAPIManager::Get()->GetSendDlgWndProc();
			
	//debug_print(DEBUG_TRACE, _T("<<<hwnd:%08x msg:%08x wp:%08x lp:%08x>>\n"), hwnd, msg, wParam, lParam);

//	for(int j=0; j<msgCount; j++) {
//		if(msgList[j] == msg)
//			break;
//	}
//	if(j==msgCount) {
//		msgList[msgCount++] = msg;
//	}

	switch(msg)
	{
		case WM_WINDOWPOSCHANGING:
		{
			// Don't let window show up if the event was generated by OEAPI
			WINDOWPOS* wp = (WINDOWPOS*)lParam;
			if(wp && wp->flags & SWP_SHOWWINDOW) {
				if(OEAPIManager::Get()->GetHideSendWindow()) {
					wp->flags &= ~SWP_SHOWWINDOW;
					OEAPIManager::Get()->SetHideSendWindow(FALSE);
				}
			}
			break;
		}
		case SEND_MSG_CODE:
		{
			// Workaround: Don't block if MainWindow is closed cause the database events are disabled
			if(!::IsWindow(OEAPI_GetOEMainWindowHandle())) {
				break;
			}

			// processing outbox filter or any client of OEAPIInit didn't process
			// OnOEAPIInit event
			
			if(OEAPIManager::Get()->IsSendWindowLocked()) {
				//debug_print(DEBUG_TRACE, _T("SendDlgWndProc: blocked %08x, %08x - %d.\n"), wParam, lParam, ::GetCurrentThreadId());
				//OEAPIManager::Get()->SetSynchroPending(TRUE, lParam);
				OEAPIManager::Get()->PushPendingSendCommand(lParam);
				return TRUE;
			}

#ifdef ENTERPRISE_VERSION
			// Check if we have a new message in outbox 
			if(OEAPIManager::Get()->CheckNewMessagesInOutbox()) {
				OEAPIManager::Get()->PushPendingSendCommand(lParam);
				return TRUE;
			}

#endif // ENTERPRISE_VERSION

//			OEAPIManager::Get()->LockUIObjects();
//
			// the OE_SHOW_SEND_RECV_WND lParam is sent when the Send/Recv button is pressed
			// if we eat the message, the window doesn't appear after the user hides it.
			// I mean: if you hide the window and you press Send/Recv the window should be
			// shown again. If we eat this message too, the window is not shown
//			if(wParam != 0 || lParam != OE_SHOW_SEND_RECV_WND) {
//				if(OEAPIManager::Get()->IsSynchroInProgress()) {
////					OEAPIManager::Get()->UnlockUIObjects();
//					return TRUE;
//				}
//			}
			// these two possible parameters we have to let them go because the window
			// is not shown.
			//if(wParam != 0) { // || 
				// TODO: Ask test cases so we can workaround in other way
				//(!IsWMail() 
				//	&& lParam != OE_SHOW_SEND_RECV_WND
				//	&& lParam != OE_SHOW_SEND_RECV_WND2 
				//	&& lParam != OE_SHOW_SEND_RECV_PROGRESS) ) {
				if(OEAPIManager::Get()->IsSynchroInProgress()) {
//					OEAPIManager::Get()->UnlockUIObjects();
					OEAPIManager::Get()->PushPendingSendCommand(lParam);
					return TRUE;
				}
			//}

			OEAPIManager::Get()->SetSynchroInProgress(TRUE);
//
//			OEAPIManager::Get()->UnlockUIObjects();

			break;
		}
	}

	return CallWindowProc(lpOldProc, hwnd, msg, wParam, lParam);
}

//FARPROC pCreateList_afterPrelude;
//DWORD pIMessageList_SetFolder;
//
//FARPROC pSetFolder_afterPrelude;
//
//
//IMessageListWMail *msgList;
//
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
//
//__declspec (naked) void SetFolderPrelude()
//{
//	FARPROC oldRet;
//	DWORD dwFolderId;
//
//	_asm {
//		cmp [esp+8], -1
//		jne get_msg_list
//
//		jmp SetFolderPatched
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
//		// ret to retCreateList to get the msg list after IMessageTable object is created
//		push eax
//		mov eax, [esp+4]
//		mov oldRet, eax
//		mov eax, offset retCreateList
//		mov [esp+4], eax
//		pop eax
//
//		jmp SetFolderPatched
//
//retCreateList:
//		pusha
//	}
//
//	OEAPIManager::Get()->SetMsgList(msgList, dwFolderId);
//
//	_asm {
//		popa
//		jmp oldRet
//	}
//}


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
OEAPIManager::OEAPIManager()
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::OEAPIManager\n"));
	//pThis = NULL;
	menusIDs_ = 0;
	lastFolderProp_.idFolderNew = lastFolderProp_.idFolderOld = OE_INVALID_FOLDER_ID;
	//msgTable_ = NULL;
	//msgList_ = NULL;
	//msgListWMail_ = NULL;
	curFolderId_ = OE_INVALID_FOLDER_ID;
	curMsgId_ = -1;
	selMsgIds_ = NULL;
	//guiFlag_ = FALSE;
	hideSendWindow_ = FALSE;
	lockCountSendWindow_ = 0;

	msgWndCloseEvent_ = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	databaseEvent_ = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	folderChangeEvent_ = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	lastFolderName_[0] = 0;
	pSN_ = NULL;
	hWnd_ = NULL;
	shutdown_ = FALSE;
	eventCount_ = 0;
	bLater_ = FALSE;
	outbox_ = NULL;

//	folderSelPending_ = FALSE;

	synchroInProcess_ = FALSE;
	//synchroPendingParams_ = FALSE;
	//SendRecvLparam_ = NULL;

	InitializeCriticalSection(&msgSelectionCS_);
	InitializeCriticalSection(&uiCS_);
	InitializeCriticalSection(&eventCS_);

	InitializeMessageStore();

	// Create the window that handles the button clicks
	hWnd_ = CreateCallbackWindow();

	if(!CreateMessageStore()) {
		return;
	}


#ifdef ENTERPRISE_VERSION
	// GetFolderMaxMsgId prevent creation of storage objects
	//if(msgStore_ != NULL) {
	lastProcessedOutboxMsgId_ = GetMaxOutboxMsgId(); //  msgStore_->GetMaxMessageId(outboxFolderId_);
	//}
#endif // ENTERPRISE_VERSION

	hSendDlg_ = NULL;
	lpOldSendDlgWndProc_ = NULL;

	HookSendWnd();
}

//---------------------------------------------------------------------------//
OEAPIManager::~OEAPIManager()
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::~OEAPIManager\n"));

	RemoveProp(hWnd_, _T("NKTHANDLE"));

	if(outbox_ != NULL) {
		outbox_->Release();
		outbox_ = NULL;
	}

	if(pSN_ != NULL) {
		pSN_->Release();
		pSN_ = NULL;
	}

	EnterCriticalSection(&msgSelectionCS_);
	EnterCriticalSection(&uiCS_);

	if(hSendDlg_ && lpOldSendDlgWndProc_) {
		SetWindowLongPtr(hSendDlg_, GWLP_WNDPROC, (LONG_PTR)lpOldSendDlgWndProc_);
		lpOldSendDlgWndProc_ = NULL;
	}

	for(MenuItemReprList::iterator itItem = menuItemsRepr_.begin(); itItem!=menuItemsRepr_.end(); itItem++) {
		OEMenuItemRepr *item = *itItem;
		delete item;
	}

	// this function hangs if it is called from here (this thread) when the user is logging
	// of WM_ENDSESSION. That's why is called from ExitServerCallback in the GUI thread.
	// sometime (in some computers) releasing message tables out of GUI thread was generating
	// a DBNotify error. Now it's called from GUI thread (ExitServerCallback)
//	DestroyMsgTables();

	if(selMsgIds_) {
		delete [] selMsgIds_;
	}

	LeaveCriticalSection(&msgSelectionCS_);
	LeaveCriticalSection(&uiCS_);

	DeleteCriticalSection(&msgSelectionCS_);
	DeleteCriticalSection(&uiCS_);
	DeleteCriticalSection(&eventCS_);

//	::SendMessage(hWndDebug, DEBUG_MESSAGE_CODE, 0, 0);

	::DestroyWindow(hWnd_);

	::CloseHandle(folderChangeEvent_);
	::CloseHandle(databaseEvent_);
	::CloseHandle(msgWndCloseEvent_);

	if(!g_OEAPIInitState.is_null()) {
		g_OEAPIInitState = NULL;
	}
}

//---------------------------------------------------------------------------//
void OEAPIManager::InitializeMessageStore()
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::CreateStorageObjects: st\n"));
	EnterCriticalSection(&msgSelectionCS_);

	msgList_ = new OEAPIMessageList;
	msgList_->Init();

	msgStore_ = OEAPIMessageStore::CreateMessageStore();

	msgStore_->SetListener(this);
	msgStore_->Init();

	LeaveCriticalSection(&msgSelectionCS_);
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::CreateStorageObjects: en\n"));
	return;
}

//---------------------------------------------------------------------------//
BOOL OEAPIManager::CreateMessageStore()
{
	BOOL ret = FALSE;

	if(msgStore_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageStore: MsgStore was not initialized.\n"));
		return ret;
	}

	//IStoreNamespace* pSN = NULL;
	//IStoreFolder* outbox = NULL;

	HRESULT hr = CoCreateInstance(CLSID_StoreNamespace, NULL,
		CLSCTX_SERVER, IID_IStoreNamespace, (LPVOID*) &pSN_);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageStore: Error CoCreateInstance.\n"));
		return ret;
	}

	hr = pSN_->Initialize(NULL, 1);
	if(FAILED(hr)) {
		pSN_->Release();
		pSN_ = NULL;
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageStore: Error Initialize.\n"));
		return ret;
	}

	hr = pSN_->OpenSpecialFolder(FOLDER_OUTBOX, 0, &outbox_);
	if(FAILED(hr)) {
		pSN_->Release();
		pSN_ = NULL;
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageStore: Error OpenSpecialFolder Outbox.\n"));
		return ret;
	}

	if(IsWMail()) {
		outbox_->Release();
		outbox_ = NULL;
	}

	if(msgStore_->CheckStore() != NULL) {
		//if(!IsWMail()) {
		//	// WinMail doesn't like if we register events here
		//	msgStore_->RegisterAllFoldersNotification();
		//}
		ret = TRUE;

		SetProp(hWnd_, _T("NKTHANDLE"), (HANDLE)((OEAPIMessageStore*)msgStore_));
	}

	if(IsWMail()) {
		pSN_->Release();
		pSN_ = NULL;
	}

	if(IsWMail()) {
		rootFolderId_ = 1; // TODO: Check this
		inboxFolderId_ = 3;
		outboxFolderId_ = 4;
		sentFolderId_ =  5;
		deletedFolderId_ = 6;
		draftFolderId_ = 7;
	}
	else {
		rootFolderId_ = 1;
		inboxFolderId_ = 4;
		outboxFolderId_ = 5;
		sentFolderId_ =  6;
		deletedFolderId_ = 7;
		draftFolderId_ = 8;
	}

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::DestroyMsgTables()
{
	EnterCriticalSection(&msgSelectionCS_);

	msgList_->ReleaseObjects();

//	// if I release the msgTable the 'Compact Folders' generates a error 'busy' (???).
//	// if I don't release it, OE shutdowns perfect. I suppose that it's released by IMessageList
//	if(msgTable_) {
////		if(!IsWMail()) {
//			msgTable_->Release();
////		}
//		msgTable_ = NULL;
//	}
//	if(msgList_) {
//		msgList_->Release();
//		msgList_ = NULL;
//	}
//	if(msgListWMail_) {
////		msgListWMail_->OnClose();
//		msgListWMail_->Release();
//		msgListWMail_ = NULL;
//	}

	LeaveCriticalSection(&msgSelectionCS_);
}

#ifdef ENTERPRISE_VERSION

//---------------------------------------------------------------------------//
INT OEAPIManager::GetMaxOutboxMsgId()
{
	INT maxId = -1;

	if(msgStore_ != NULL) {
		maxId = msgStore_->GetMaxMessageId(outboxFolderId_);
	}

	//if(outbox_ == NULL) {
	//	debug_print(DEBUG_ERROR, _T("OEAPIManager::GetMaxOutboxMsgId: Outbox is null.\n"));
	//	return maxId;
	//}

	//HENUMSTORE hEnumMsg = (HENUMSTORE)-1;

	//LPMESSAGEPROPS pMsgProps = new MESSAGEPROPS;

	//pMsgProps->cbSize = sizeof(MESSAGEPROPS);
	//pMsgProps->dwFlags = 0;

	//HRESULT hr = outbox_->GetFirstMessage(MSGPROPS_FAST, 0, MESSAGEID_FIRST, pMsgProps, &hEnumMsg);
	//if(FAILED(hr) || hr == S_FALSE) {
	//	delete pMsgProps;
	//	return maxId;
	//}

	//while(1) {
	//	if(maxId > (INT)pMsgProps->dwMessageId) {
	//		maxId = pMsgProps->dwMessageId;
	//	}
	//	
	//	outbox_->FreeMessageProps(pMsgProps);

	//	HRESULT hr = outbox_->GetNextMessage(hEnumMsg, MSGPROPS_FAST, pMsgProps);
	//	if(FAILED(hr) || hr == S_FALSE) {
	//		break;
	//	}
	//}

	//delete pMsgProps;
	//outbox_->GetMessageClose(hEnumMsg);

	return maxId;
}

#endif // ENTERPRISE_VERSION


//---------------------------------------------------------------------------//
void OEAPIManager::SetMsgList(IUnknown *msgList, DWORD dwFolderId)
{
	//HRESULT hr;

	EnterCriticalSection(&msgSelectionCS_);

	// Fix bug when renaming current folder and it has messages
	DestroyMsgTables();

	// dwFolderId contains the folder id even when it's not part of 'Local Folders'
//	curFolderId_ = dwFolderId;

	msgList_->SetMessageList(msgList);

	//if(IsWMail()) {
	//	if(msgListWMail_) {
	//		msgListWMail_->Release();
	//		msgListWMail_ = NULL;
	//	}
	//	if(msgList) {
	//		msgListWMail_ = (IMessageListWMail *) msgList;
	//		msgListWMail_->AddRef();

	//		hr = msgListWMail_->GetMessageTable(&msgTable_);
	//		if(FAILED(hr)) {
	//			debug_print(DEBUG_ERROR, _T("OEAPIManager::SetMsgList: GetMessageTable\n"));
	//			msgListWMail_->Release();
	//			msgListWMail_ = NULL;
	//		}
	//	}
	//}
	//else {
	//	if(msgList_) {
	//		msgList_->Release();
	//		msgList_ = NULL;
	//	}
	//	if(msgList) {
	//		msgList_ = (IMessageList *) msgList;
	//		msgList_->AddRef();
	//	}

	//	hr = msgList_->GetMessageTable(&msgTable_);
	//	if(FAILED(hr)) {
	//		debug_print(DEBUG_ERROR, _T("OEAPIManager::SetMsgList: GetMessageTable\n"));
	//		msgList_->Release();
	//		msgList_ = NULL;
	//	}
	//}

	LeaveCriticalSection(&msgSelectionCS_);
}

//---------------------------------------------------------------------------//
BOOL OEAPIManager::EnterEvent()
{
	BOOL ret = TRUE;

	EnterCriticalSection(&eventCS_);
	if(IsAboutToShutdown()) {
		ret = FALSE;
	}
	else {
		eventCount_++;
	}
	LeaveCriticalSection(&eventCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::LeaveEvent()
{
	EnterCriticalSection(&eventCS_);
	eventCount_--;
	LeaveCriticalSection(&eventCS_);
}


//---------------------------------------------------------------------------//
void OEAPIManager::HookSendWnd()
{
	if(hSendDlg_ == NULL) {
		hSendDlg_ = FindWindowRecursive(NULL, _T("#32770"), GetCurrentProcessId());
		if(hSendDlg_) {
			lpOldSendDlgWndProc_ = (WNDPROC) GetWindowLongPtr(hSendDlg_, GWLP_WNDPROC);
			SetWindowLongPtr(hSendDlg_, GWLP_WNDPROC, (LONG_PTR)SendDlgWndProc);
		}
		else {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::HookSendWnd: Can't find send windows.\n"));
		}
	}
}

//---------------------------------------------------------------------------//
void OEAPIManager::CreateOEAPIInitObject()
{
	if(g_OEAPIInitState.is_null())
    {
        //g_OEAPIInitState = OEAPIINITCOM::OEAPIInitState::create();
        GetInitInstance(uuidof<OEAPIINITCOM::OEAPIInitState>(),uuidof<OEAPIINITCOM::OEAPIInitState>(),(void**)&g_OEAPIInitState);
		if(g_OEAPIInitState.is_null()) {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOEAPIInitObject: Error CoCreateInstance.\n"));
		}
	}
}

//---------------------------------------------------------------------------//
void OEAPIManager::MenuItemCreated(TOEMenuItem *menuItem, int parentMenuItemId)
{
	OEMenuItemRepr *itemRepr = new OEMenuItemRepr;

	itemRepr->SetID(menuItem->GetID());
	itemRepr->SetMenuID(menuItem->GetMenuID());
	if(parentMenuItemId == -1) {
		TOEMenu *menu = GetMenu(menuItem->GetMenuID());
		if(menu) {
			itemRepr->SetOEAPIID(menu->GetOEAPIID());
		}
	}
	else {
		OEMenuItemRepr *parentItem = GetMenuItemRepr(parentMenuItemId);
		itemRepr->SetOEAPIID(parentItem->GetOEAPIID());
	}

	EnterCriticalSection(&uiCS_);
	menuItemsRepr_.push_back(itemRepr);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
OEMenuItemRepr *OEAPIManager::GetMenuItemRepr(int id)
{
	EnterCriticalSection(&uiCS_);

	OEMenuItemRepr *ret = NULL;

	for(MenuItemReprList::iterator it=menuItemsRepr_.begin(); it!=menuItemsRepr_.end(); it++) {
		OEMenuItemRepr *item = *it;

		if(item->GetID() == id) {
			ret = item;
			break;
		}
	}

	LeaveCriticalSection(&uiCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::DestroyMenuItem(int itemId)
{
	EnterCriticalSection(&uiCS_);

	for(MenuItemReprList::iterator it=menuItemsRepr_.begin(); it!=menuItemsRepr_.end(); it++) {
		OEMenuItemRepr *item = *it;
		if(item->GetID() == itemId) {
			// OEAPI_RemoveMenuItem(item->GetID());
			// OEPluginMenuMgr::Get()->RemoveMenuItem(item->GetID());
			menuItemsRepr_.remove(item);
			delete item;
			break;
		}
	}

	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::DestroyAllMenuItemsByOEAPI(int oeapiId)
{
	EnterCriticalSection(&uiCS_);

	for(MenuItemReprList::iterator it=menuItemsRepr_.begin(); it!=menuItemsRepr_.end();) {
		OEMenuItemRepr *item = *it;

		// advance first
		it++;
		if(item->GetOEAPIID() == oeapiId) {
			// OEAPI_RemoveMenuItem(item->GetID());
			OEPluginMenuMgr::Get()->RemoveMenuItem(item->GetID());
			menuItemsRepr_.remove(item);
			delete item;
		}
	}

	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
TOEAPIObj *OEAPIManager::GetOEAPI(int oeapiId)
{
	TOEAPIObj *ret = NULL;

	EnterCriticalSection(&uiCS_);

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oeapi = *it;

		if(oeapi->GetID() == oeapiId) {
			ret = oeapi;
			break;
		}
	}

	LeaveCriticalSection(&uiCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::AddOEAPI(TOEAPIObj *oeapi)
{
	EnterCriticalSection(&uiCS_);
	oeapis_.push_back(oeapi);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::RemoveOEAPI(TOEAPIObj *oeapi)
{
	EnterCriticalSection(&uiCS_);
	oeapis_.remove(oeapi);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
TOEMsgWnd *OEAPIManager::GetMsgWnd(int msgWndId)
{
	TOEMsgWnd *ret = NULL;

	EnterCriticalSection(&uiCS_);
	for(MsgWndList::iterator it=msgWnds_.begin(); it!=msgWnds_.end(); it++) {
		TOEMsgWnd *msgWnd = *it;

		if(msgWnd->GetID() == msgWndId) {
			ret = msgWnd;
			break;
		}
	}
	LeaveCriticalSection(&uiCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::AddMsgWnd(TOEMsgWnd *msgWnd)
{
	EnterCriticalSection(&uiCS_);
	msgWnds_.push_back(msgWnd);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::RemoveMsgWnd(TOEMsgWnd *msgWnd)
{
	EnterCriticalSection(&uiCS_);
	msgWnds_.remove(msgWnd);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
TOEToolbar *OEAPIManager::GetToolbar(int toolbarId)
{
	TOEToolbar *ret = NULL;

	EnterCriticalSection(&uiCS_);

	for(ToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
		TOEToolbar *toolbar = *it;

		if(toolbar->GetID() == toolbarId) {
			ret = toolbar;
			break;
		}
	}

	LeaveCriticalSection(&uiCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::AddToolbar(TOEToolbar *toolbar)
{
	EnterCriticalSection(&uiCS_);
	toolbars_.push_back(toolbar);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::RemoveToolbar(TOEToolbar *toolbar)
{
	EnterCriticalSection(&uiCS_);
	toolbars_.remove(toolbar);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
TOEButton *OEAPIManager::GetButton(int buttonId)
{
	TOEButton *ret = NULL;

	EnterCriticalSection(&uiCS_);

	for(ButtonList::iterator it=buttons_.begin(); it!=buttons_.end(); it++) {
		TOEButton *button = *it;

		if(button->GetID() == buttonId) {
			ret = button;
			break;
		}
	}

	LeaveCriticalSection(&uiCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::AddButton(TOEButton *button)
{
	EnterCriticalSection(&uiCS_);
	buttons_.push_back(button);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::RemoveButton(TOEButton *button)
{
	EnterCriticalSection(&uiCS_);
	buttons_.remove(button);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
TOEMenu *OEAPIManager::GetMenu(int menuID)
{
	TOEMenu *ret = NULL;

	EnterCriticalSection(&uiCS_);

	for(MenuList::iterator it=menus_.begin(); it!=menus_.end(); it++) {
		TOEMenu *menu = *it;

		if(menu->GetID() == menuID) {
			ret = menu;
			break;
		}
	}

	LeaveCriticalSection(&uiCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::AddMenu(TOEMenu *menu)
{
	EnterCriticalSection(&uiCS_);
	menus_.push_back(menu);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::RemoveMenu(TOEMenu *menu)
{
	EnterCriticalSection(&uiCS_);
	menus_.remove(menu);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
TOEMenuItem *OEAPIManager::GetMenuItem(int menuItemId)
{
	TOEMenuItem *ret = NULL;

	EnterCriticalSection(&uiCS_);

	for(MenuItemList::iterator it=menuItems_.begin(); it!=menuItems_.end(); it++) {
		TOEMenuItem *menuItem = *it;

		if(menuItem->GetID() == menuItemId) {
			ret = menuItem;
			break;
		}
	}

	LeaveCriticalSection(&uiCS_);

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::AddMenuItem(TOEMenuItem *menuItem)
{
	EnterCriticalSection(&uiCS_);
	menuItems_.push_back(menuItem);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::RemoveMenuItem(TOEMenuItem *menuItem)
{
	EnterCriticalSection(&uiCS_);
	menuItems_.remove(menuItem);
	LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::LockUIObjects()
{
	EnterCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::UnlockUIObjects()
{
	LeaveCriticalSection(&uiCS_);
}

/**
Wait until watchFlag is true and non-input process events.
*/
//---------------------------------------------------------------------------//
void OEAPIManager::WaitGUIEvent(HANDLE hEvent)
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::WaitGUIEvent: Enter\n"));
	// wait until all the clients process the event. Otherwise, a 'Compact Folders'
	while(true) {
		DWORD ret = ::MsgWaitForMultipleObjectsEx(1, &hEvent, INFINITE, QS_ALLEVENTS, 0);
		if(ret == WAIT_OBJECT_0) {
			break;
		}
		else if(ret == WAIT_OBJECT_0 + 1) {
			MSG msg;
			if(::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
				if((msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) ||
					(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)) {
						// don't process message
						if(::PeekMessage(&msg, msg.hwnd, msg.message, msg.message, PM_REMOVE)) {
							msg.message = WM_NULL;
							::DispatchMessage(&msg);
						}
				}
				else {
					if(::PeekMessage(&msg, msg.hwnd, msg.message, msg.message, PM_REMOVE)) {
						::DispatchMessage(&msg);
					}
				}
			}
		}
	}
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::WaitGUIEvent: Leave\n"));
}

//---------------------------------------------------------------------------//
//void OEAPIManager::ResetGUIFlag()
//{
//	debug_print(DEBUG_TRACE, _T("OEAPIManager::ResetGUIFlag: Enter\n"));
//	::ResetEvent(guiEvent_);
//	debug_print(DEBUG_TRACE, _T("OEAPIManager::ResetGUIFlag: Leave\n"));
//}

//---------------------------------------------------------------------------//
//void OEAPIManager::SetGUIFlag()
//{
//	debug_print(DEBUG_TRACE, _T("OEAPIManager::SetGUIFlag: Enter\n"));
//	::SetEvent(guiEvent_);
//	debug_print(DEBUG_TRACE, _T("OEAPIManager::SetGUIFlag: Leave\n"));
//}

//---------------------------------------------------------------------------//
//void OEAPIManager::MsgWaitEvent(HANDLE hEvent)
//{
//	bool quit = false;
//	while(!quit) {
//		DWORD ret = ::MsgWaitForMultipleObjectsEx(1, &hEvent, INFINITE, QS_ALLEVENTS & ~QS_INPUT, 0);
//		if(ret == WAIT_OBJECT_0) {
//			quit = true;
//			break;
//		}
//		else {
//			MSG msg;
//			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//				::TranslateMessage(&msg);
//				::DispatchMessage(&msg);
//			}
//		}
//	}
//}

//---------------------------------------------------------------------------//
DWORD OEAPIManager::GetFirstSelectedMessage()
{
	EnterCriticalSection(&msgSelectionCS_);

	DWORD msgId = -1;

	if(selMsgIds_ != NULL && selCount_ > 0) {
		msgId = selMsgIds_[0];
	}

	LeaveCriticalSection(&msgSelectionCS_);

	return msgId;
}

//---------------------------------------------------------------------------//
DWORD OEAPIManager::GetNextSelectedMessage(DWORD lastReturnedId)
{
	if(lastReturnedId == -1) {
		return -1;
	}

	EnterCriticalSection(&msgSelectionCS_);

	DWORD msgId = -1;

	if(selMsgIds_ != NULL && selCount_) {
		for(DWORD i=0; i<selCount_-1; i++) {
			if(selMsgIds_[i] == lastReturnedId) {
				msgId = selMsgIds_[i+1];
				break;
			}
		}
	}

	LeaveCriticalSection(&msgSelectionCS_);

	return msgId;
}

//---------------------------------------------------------------------------//
int OEAPIManager::GetMessageIndex(DWORD msgId)
{
	int index;
	index = msgList_->GetMessageIndex(msgId);
	return index;

	//if(msgTable_) {
	//	HRESULT hr;
	//	// TODO: Check under 64 bits

	//	DWORD index = -1;

	//	index = msgList_->GetMessageIndex(msgId);

	//	//if(IsWMail()) {
	//	//	// TODO: 
	//	//	// To use GetRowIndex we need to update IMessageTable for WM msgId is 64 bits
	//	//	ULONGLONG id;
	//	//	for(index=0; ; index++) {
	//	//		hr = msgTable_->GetRowMessageId(index, (LPDWORD)&id);
	//	//	
	//	//		if(FAILED(hr)) {
	//	//			return -1;
	//	//		}

	//	//		if(id == msgId) {
	//	//			return index;
	//	//		}
	//	//	}
	//	//}
	//	//else {
	//	//	hr = msgTable_->GetRowIndex(msgId, &index);
	//	//}

	//	//if(FAILED(hr)) {
	//	//	debug_print(DEBUG_ERROR, _T("OEAPIManager::GetMessageIndex: GetRowIndex failed %08x.\n"), hr);
	//	//	return -1;
	//	//}

	//	return index;

	//	//for(int i=0;; i++) {
	//	//	DWORD id;

	//	//	// FIXME: WinMail uses UINT64
	//	//	if(IsWMail()) {
	//	//		DWORD idAux[2];
	//	//		hr = msgTable_->GetRowMessageId(i, idAux);
	//	//		id = idAux[0];
	//	//	}
	//	//	else {
	//	//		hr = msgTable_->GetRowMessageId(i, &id);
	//	//	}
	//	//	if(FAILED(hr)) {
	//	//		return -1;
	//	//	}

	//	//	if(id == msgId) {
	//	//		return i;
	//	//	}
	//	//}
	//}

	//return -1;
}

//---------------------------------------------------------------------------//
int OEAPIManager::GetMessageID(DWORD index)
{
	DWORD msgId;

	msgId = msgList_->GetMessageId(index);

	//if(msgTable_ == NULL) {
	//	return -1;
	//}

	//DWORD msgId;
	//HRESULT hr = msgTable_->GetRowMessageId(index, &msgId);
	//if(FAILED(hr)) {
	//	return -1;
	//}
	return msgId;
}

//---------------------------------------------------------------------------//
void OEAPIManager::SetSelectedMessages(int count, int *msgIndexs, int focusIndex, BOOL regenerateTable)
{
	if(curFolderId_ == OE_INVALID_FOLDER_ID) {
		return;
	}

	// synchronize the table before if the regenerateTable flag is set.
	// the flag is set when a delete ocurrs because the table is wrong.
	// create tables here because the IMessageList object must be created from the
	// GUI thread. I don't know why, but if I create tables from the OEAPIWndProc
	// then the OE doesn't exit properly.
//	if(regenerateTable || msgTable_ == NULL || (msgList_ == NULL && msgListWMail_ == NULL)) {
//		CreateMsgTable();
//		return;
//	}

	//if(msgTable_ == NULL || (msgList_ == NULL && msgListWMail_ == NULL)) {
	if(msgList_->IsNull()) { // msgTable_ == NULL || (msgList_ == NULL && msgListWMail_ == NULL)) {
//		debug_print(DEBUG_ERROR, _T("SetSelectedMessages: table null\n"));
		EnterCriticalSection(&msgSelectionCS_);

		if(selCount_ > 0) {
			delete [] selMsgIds_;
			selMsgIds_ = NULL;

			selCount_ = 0;
		}

		PostMessage(GetCallbackWindow(), MESSAGE_SELECTION_CHANGED_CODE, 0, 0);

		LeaveCriticalSection(&msgSelectionCS_);
		return;
	}

	//DWORD *newSel;
	NktDynamicArray<DWORD> newSel;
	//HRESULT hr;
	BOOL useFocusIndex = FALSE;
	int i;

	newSel.alloc(count);
	for(i=0; i<count; i++) {
		DWORD msgId;
		msgId = msgList_->GetMessageId(msgIndexs[i]);
		if(msgId == -1) {
			// this could happen when a user deletes a message.
			// The message index doesn't
			debug_print(DEBUG_ERROR, _T("SetSelectedMessages: Error GetRowMessageId\n"));
			if(selMsgIds_) {
				delete [] selMsgIds_;
				selMsgIds_ = NULL;
			}
			selCount_ = 0;
			return;
		}
		// only use this parameter if the focusIndex is part of the selection.
		// Otherwise, OE doesn't use it to set the message preview wnd
		if(msgIndexs[i] == focusIndex) {
			useFocusIndex = TRUE;
		}

		newSel[i] = msgId;
	}

	if(useFocusIndex || count == 0) {
		DWORD focusId = -1;
		DWORD msgId;

		if(count != 0) {
			msgId = msgList_->GetMessageId(focusIndex);
			if(msgId == -1) {
				debug_print(DEBUG_ERROR, _T("SetSelectedMessages: Error GetRowMessageId\n"));
				return;
			}
			focusId = msgId;
		}
		//else {
		//	focusId = -1;
		//}

		if(focusId != curMsgId_) {
			curMsgId_ = focusId;
			PostMessage(GetCallbackWindow(), MESSAGE_CURRENT_CHANGED_CODE, curMsgId_, 0);
		}
	}

	// FIXME: WinMail has 64 bits integers. This solution is TERRIBLE in all sense
//	if(IsWMail()) {
//		//newSel = new DWORD[count*2];
//		newSel.alloc(count);
//		for(i=0; i<count; i++) {
//			ULONGLONG sel;
//			hr = msgTable_->GetRowMessageId(msgIndexs[i], (DWORD*)&sel);
//			newSel[i] = sel;
//			if(FAILED(hr)) {
//				// this could happen when a user deletes a message.
//				// The message index doesn't
//	//			debug_print(DEBUG_ERROR, _T("SetSelectedMessages: Error GetRowMessageId\n"));
//				selMsgIds_ = NULL;
//				selCount_ = 0;
//				//delete newSel;
//				return;
//			}
//
//			// only use this parameter if the focusIndex is part of the selection.
//			// Otherwise, OE doesn't use it to set the message preview wnd
//			if(msgIndexs[i] == focusIndex) {
//				useFocusIndex = TRUE;
//			}
//		}
//
//		if(useFocusIndex || count == 0) {
//			DWORD focusId;
//			DWORD buf[2];
//
//			if(count != 0) {
//				hr = msgTable_->GetRowMessageId(focusIndex, buf);
//				if(FAILED(hr)) {
//					debug_print(DEBUG_ERROR, _T("SetSelectedMessages: Error GetRowMessageId\n"));
//					return;
//				}
//				else {
//					focusId = buf[0];
//				}
//			}
//			else {
//				focusId = -1;
//			}
//
//			if(focusId != curMsgId_) {
//				curMsgId_ = focusId;
//				PostMessage(GetCallbackWindow(), MESSAGE_CURRENT_CHANGED_CODE, curMsgId_, 0);
//			}
//		}
//	}
//	else {
//		//newSel = new DWORD[count];
//		newSel.alloc(count);
//		for(int i=0; i<count; i++) {
//#ifndef _WIN64
//			DWORD sel;
//#else
//			ULONGLONG sel;
//#endif
//			hr = msgTable_->GetRowMessageId(msgIndexs[i], (DWORD*)&sel);
//			newSel[i] = sel;
//			if(FAILED(hr)) {
//				// this could happen when a user deletes a message.
//				// The message index doesn't
//	//			debug_print(DEBUG_ERROR, _T("SetSelectedMessages: Error GetRowMessageId\n"));
//				selMsgIds_ = NULL;
//				selCount_ = 0;
//				//delete newSel;
//				return;
//			}
//
//			// only use this parameter if the focusIndex is part of the selection.
//			// Otherwise, OE doesn't use it to set the message preview wnd
//			if(msgIndexs[i] == focusIndex) {
//				useFocusIndex = TRUE;
//			}
//		}
//
//		if(useFocusIndex || count == 0) {
//#ifndef _WIN64
//			DWORD focusId;
//#else
//			ULONGLONG focusId;
//#endif
//
//			if(count != 0) {
//				hr = msgTable_->GetRowMessageId(focusIndex, (DWORD*)&focusId);
//				if(FAILED(hr)) {
//					debug_print(DEBUG_ERROR, _T("SetSelectedMessages: Error GetRowMessageId\n"));
//					return;
//				}
//			}
//			else {
//				focusId = -1;
//			}
//
//			if(focusId != curMsgId_) {
//				curMsgId_ = focusId;
//				PostMessage(GetCallbackWindow(), MESSAGE_CURRENT_CHANGED_CODE, curMsgId_, 0);
//			}
//		}
//	}

	BOOL triggerEvent = FALSE;

	// verify if the selection has changed
	// we don't verify for each id the complete array. We assume that they are in the same
	// order. This could be false if the order changes but in that situation the  event
	// is triggered the same.
	if(selMsgIds_ == NULL || count != selCount_) {
		triggerEvent = TRUE;
	}
	else {
		for(i=0; i<count; i++) {
			if(newSel[i] != selMsgIds_[i]) {
				triggerEvent = TRUE;
				break;
			}
		}
	}

	if(triggerEvent) {
		EnterCriticalSection(&msgSelectionCS_);

		if(selMsgIds_) {
			delete [] selMsgIds_;
			selMsgIds_ = NULL;
		}

		selMsgIds_ = newSel.detach();
		selCount_ = count;

		PostMessage(GetCallbackWindow(), MESSAGE_SELECTION_CHANGED_CODE, 0, 0);

		LeaveCriticalSection(&msgSelectionCS_);
	}
	//else {
	//	delete [] newSel;
	//}
}
//
////---------------------------------------------------------------------------//
//void OEAPIManager::UpdateMsgListViewOrder(int colInfo, int type)
//{
//	DWORD *sortOrder;
//	DWORD *msgListViewColOrder;
//
//	// sent and draft folders have another column configuration
//	if(curFolderId_ == sentFolderId_ || curFolderId_ == draftFolderId_) {
//		sortOrder = sortOrderOut_;
//		msgListViewColOrder = msgListViewColOrderOut_;
//	}
//	else if(curFolderId_ != -1) {
//		sortOrder = sortOrderIn_;
//		msgListViewColOrder = msgListViewColOrderIn_;
//	}
//	else {
//		return;
//	}
//
//	if(msgTable_) {
//		if(type == -1) {
//			if(sortOrder[0] == msgListViewColOrder[colInfo]) {
//				if(sortOrder[1] == 0) {
//					sortOrder[1] = 1;
//				}
//				else {
//					sortOrder[1] = 0;
//				}
//			}
//			else {
//				sortOrder[0] = msgListViewColOrder[colInfo];
//			}
//		}
//		else {
//			sortOrder[0] = msgListViewColOrder[colInfo];
//			sortOrder[1] = type;
//		}
//
//		if(IsWMail()) {
//			// ???: it doesn't work to just re-set the order
//			CreateMsgTable();
//		}
//		else {
//			msgTable_->SetSortInfo(sortOrder, NULL);
//		}
//	}
//}
//
////---------------------------------------------------------------------------//
//void OEAPIManager::UpdateMsgListViewHeaderOrderChanged(int colInfo, int newPos)
//{
//	DWORD *sortOrder;
//	DWORD *msgListViewColOrder;
//	int i;
//
//	// sent and draft folders have another column configuration
//	if(curFolderId_ == sentFolderId_ || curFolderId_ == draftFolderId_) {
//		sortOrder = sortOrderOut_;
//		msgListViewColOrder = msgListViewColOrderOut_;
//	}
//	else if(curFolderId_ != -1) {
//		sortOrder = sortOrderIn_;
//		msgListViewColOrder = msgListViewColOrderIn_;
//	}
//	else {
//		return;
//	}
//
//	if(colInfo < newPos) {
//		int aux = msgListViewColOrder[colInfo];
//		for(i=colInfo; i<newPos;i++) {
//			msgListViewColOrder[i] = msgListViewColOrder[i+1];
//		}
//		msgListViewColOrder[newPos] = aux;
//	}
//	else if(colInfo > newPos) {
//		int aux = msgListViewColOrder[colInfo];
//		for(i=colInfo-1; i>=newPos; i--) {
//			msgListViewColOrder[i+1] = msgListViewColOrder[i];
//		}
//		msgListViewColOrder[newPos] = aux;
//	}
//}

//---------------------------------------------------------------------------//
BOOL OEAPIManager::GetSendMailImmediatelyKeyValue()
{
	NktRegistry reg;
	// HKEY hKey;
	TCHAR szText[1024];
	//TCHAR colKey[1024];
	DWORD dwType;
	DWORD dwSize;
	DWORD value;
	// assume true if I cannot find it
	BOOL ret = TRUE;

	if(!IsWMail()) {
		// OE
		if(reg.Open(HKEY_CURRENT_USER, _T("Identities"), KEY_QUERY_VALUE) != ERROR_SUCCESS) {
			return ret;
		}

		dwSize = sizeof(szText);
		if(reg.QueryValue(_T("Last User ID"), &dwType, (LPBYTE)szText, &dwSize) != ERROR_SUCCESS) {
			return ret;
		}

		std::basic_stringstream<TCHAR> buffer;
		buffer << _T("Identities\\") << szText << _T("\\Software\\Microsoft\\Outlook Express\\5.0\\Mail");
		// _stprintf(colKey, _T("Identities\\%s\\Software\\Microsoft\\Outlook Express\\5.0\\Mail"), text);
		if(reg.Open(HKEY_CURRENT_USER, buffer.str().c_str(), KEY_QUERY_VALUE) != ERROR_SUCCESS) {
			return ret;
		}

		dwSize = sizeof(value);
		dwType = REG_DWORD;
		if(reg.QueryValue(_T("Send Mail Immediately"), &dwType, (LPBYTE)&value, &dwSize) == ERROR_SUCCESS) {
			if(value == 0) {
				ret = FALSE;
			}
		}
	}
	else {
		// WinMail
		if(reg.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows Mail\\Mail"), KEY_QUERY_VALUE) != ERROR_SUCCESS) {
			return ret;
		}

		dwSize = sizeof(value);
		dwType = REG_DWORD;
		if(reg.QueryValue(_T("Send Mail Immediately"), &dwType, (LPBYTE)&value, &dwSize) == ERROR_SUCCESS) {
			if(value == 0) {
				ret = FALSE;
			}
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL OEAPIManager::GetOfflineMode()
{
	NktRegistry reg;
	DWORD dwType;
	DWORD dwSize;
	DWORD value;
	// assume true if I cannot find it
	BOOL ret = FALSE;
	if(reg.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), KEY_QUERY_VALUE) != ERROR_SUCCESS) {
		return ret;
	}
	dwSize = sizeof(value);
	dwType = REG_DWORD;
	if(reg.QueryValue(_T("GlobalUserOffline"), &dwType, (LPBYTE)&value, &dwSize) == ERROR_SUCCESS) {
		if(value == 1) {
			ret = TRUE;
		}
	}
	return ret;
}

//
////---------------------------------------------------------------------------//
//void OEAPIManager::GetStartupOrderInformation()
//{
//	HKEY hKey;
//	TCHAR text[1024];
//	TCHAR colKey[1024];
//	DWORD type, size;
//	char colArray[1024];
//
//	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Identities"), 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
//		return;
//	}
//
//	size = sizeof(text);
//	if(RegQueryValueEx(hKey, _T("Last User ID"), NULL, &type, (BYTE*) text, &size) != ERROR_SUCCESS) {
//		RegCloseKey(hKey);
//		return;
//	}
//
//	RegCloseKey(hKey);
//
//	if(IsWMail()) {
//		_stprintf(colKey, _T("Software\\Microsoft\\Windows Mail\\Columns"), text);
//	}
//	else {
//		_stprintf(colKey, _T("Identities\\%s\\Software\\Microsoft\\Outlook Express\\5.0\\Columns"), text);
//	}
//
//	if(RegOpenKeyEx(HKEY_CURRENT_USER, colKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
//		return;
//	}
//
//	memset(colArray, 0, sizeof(colArray));
//
//	// all the folders have their column configuration in 'Mail Column Info (In)' but
//	// Sent and Draft folders configuration is in 'Mail Column Info (Out)'
//	size = sizeof(colArray);
//	if(RegQueryValueEx(hKey, _T("Mail Column Info (In)"), NULL, &type, (BYTE*) colArray, &size) == ERROR_SUCCESS) {
//		FillOrderArrays(colArray, sizeof(colArray), sortOrderIn_, msgListViewColOrderIn_);
//	}
//
//	memset(colArray, 0, sizeof(colArray));
//	size = sizeof(colArray);
//	if(RegQueryValueEx(hKey, _T("Mail Column Info (Out)"), NULL, &type, (BYTE*) colArray, &size) == ERROR_SUCCESS) {
//		FillOrderArrays(colArray, sizeof(colArray), sortOrderOut_, msgListViewColOrderOut_);
//	}
//
//	RegCloseKey(hKey);
//}
//
////---------------------------------------------------------------------------//
//void OEAPIManager::FillOrderArrays(char *codedColArray, int bufSize, DWORD sortOrder[7], DWORD msgListViewColOrder[32])
//{
//	memset(sortOrder, 0, sizeof(sortOrderIn_));
//	memset(msgListViewColOrder, 0, sizeof(msgListViewColOrderIn_));
//
//	// set the default column order and default message order (Received)
//	// if the registry key is not set use it.
//	msgListViewColOrder[0] = MSG_COLUMN_PRIORITY;
//	msgListViewColOrder[1] = MSG_COLUMN_ATTACH;
//	msgListViewColOrder[2] = MSG_COLUMN_FLAG;
//	msgListViewColOrder[3] = MSG_COLUMN_FROM;
//	msgListViewColOrder[4] = MSG_COLUMN_SUBJECT;
//	msgListViewColOrder[5] = MSG_COLUMN_RECEIVED;
//
//	// default order is received: last received at the bottom.
//	sortOrder[0] = MSG_COLUMN_RECEIVED;
//	sortOrder[1] = 1;
//
//	int j, i;
//	for(i=12, j=0; i<bufSize && j<sizeof(msgListViewColOrderIn_)/sizeof(msgListViewColOrder[0]); i+=12, j++) {
//		// this code is no longer needed as the order is got from the list view using
//		// IMessageList
//		int value = codedColArray[i];
//		if(value == 3 || value == 11) {
//			sortOrder[0] = codedColArray[i-4];
//			sortOrder[1] = 1;
//		}
//		if(value == 5 || value == 13) {
//			sortOrder[0] = codedColArray[i-4];
//			sortOrder[1] = 0;
//		}
//
//		msgListViewColOrder[j] = codedColArray[i-4];
//	}
//}
//
////---------------------------------------------------------------------------//
//void OEAPIManager::SetMessagesListViewOrderArray(int count, int *orderArray)
//{
//	DWORD *sortOrder;
//	DWORD *msgListViewColOrder;
//
//	// sent and draft folders have another column configuration
//	if(curFolderId_ == sentFolderId_ || curFolderId_ == draftFolderId_) {
//		sortOrder = sortOrderOut_;
//		msgListViewColOrder = msgListViewColOrderOut_;
//	}
//	else if(curFolderId_ != -1) {
//		sortOrder = sortOrderIn_;
//		msgListViewColOrder = msgListViewColOrderIn_;
//	}
//	else {
//		return;
//	}
//
//	memcpy(msgListViewColOrder, orderArray, sizeof(int)*count);
//
//	// if the current order column is not in the list view, the order is the first column
//	// that is normal (not attach, not flag, etc)
//	for(int i=0; i<count; i++) {
//		if(sortOrder[0] == msgListViewColOrder[i]) {
//			return;
//		}
//	}
//
//	// find the first column that can be a order column.
//	for(i=0; i<count; i++) {
//		if(msgListViewColOrder[i] != MSG_COLUMN_PRIORITY &&
//			msgListViewColOrder[i] != MSG_COLUMN_ATTACH &&
//			msgListViewColOrder[i] != MSG_COLUMN_FLAG &&
//			msgListViewColOrder[i] != MSG_COLUMN_WATCH) {
//			sortOrder[0] = msgListViewColOrder[i];
//			return;
//		}
//	}
//
//	// if there isn't any, the first
//	sortOrder[0] = msgListViewColOrder[0];
//}
//
////---------------------------------------------------------------------------//
//void OEAPIManager::CreateMsgTable()
//{
////	static HWND hwnd = NULL;
//	HRESULT hr;
//
//	EnterCriticalSection(&msgSelectionCS_);
//
//	DestroyMsgTables();
//
//	if(selMsgIds_) {
//		delete [] selMsgIds_;
//		selMsgIds_ = NULL;
//	}
//
//	// workaround to prevent a bug that presents sometimes at start-up.
//	// the curFolderId_ is not set and this function fails.
//	// OE starts with Inbox selected, so assume that.
//	// curFolderId_ does not come with -1 any more. It was fixed, but I keep this code as
//	// a double verification.
//	// XXX: workaround is wrong as Inbox is not always the first folder selected.
//	if(curFolderId_ == -1) {
//		LeaveCriticalSection(&msgSelectionCS_);
//		return;
//
////		IStoreFolder *pSF;
////		FOLDERPROPS props;
////
////		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: Error current folder invalid.\n"));
////
////		// assume Inbox folder
////		hr = pSN_->OpenSpecialFolder(FOLDER_INBOX, 0, &pSF);
////		if(FAILED(hr)) {
////			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: Error OpenSpecialFolder.\n"));
////			LeaveCriticalSection(&msgSelectionCS_);
////			return;
////		}
////
////		props.cbSize = sizeof(FOLDERPROPS);
////
////		hr = pSF->GetFolderProps(0, &props);
////		if(FAILED(hr)) {
////			pSF->Release();
////			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: Error GetFolderProps.\n"));
////			return;
////		}
////
////		curFolderId_ = props.dwFolderId;
////
////		pSF->Release();
//	}
//
//	if(IsWMail()) {
//		hr = CoCreateInstance(CLSID_IMessageList, NULL,
//					CLSCTX_INPROC_SERVER, IID_IMessageListWMail, (LPVOID*) &msgListWMail_);
//	}
//	else {
//		hr = CoCreateInstance(CLSID_IMessageList, NULL,
//					CLSCTX_INPROC_SERVER, IID_IMessageList, (LPVOID*) &msgList_);
//	}
//
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: CoCreateInstance\n"));
//		LeaveCriticalSection(&msgSelectionCS_);
//		return;
//	}
//
//	DWORD *sortOrder;
//
//	// sent and draft folders have another column configuration
//	if(curFolderId_ == sentFolderId_ || curFolderId_ == draftFolderId_) {
//		sortOrder = sortOrderOut_;
//	}
//	else if(curFolderId_ != -1) {
//		sortOrder = sortOrderIn_;
//	}
//	else {
//		return;
//	}
//
//	if(IsWMail()) {
//		HWND hMessageView = OEAPI_GetMessageViewHandle();
//		IMessageListWMail *pMsgList = msgListWMail_;
//		_asm {
//			push esi
//			push eax
//			mov esi, [pMsgList]
//			mov eax, [hMessageView]
//			mov [esi+70h], eax                      ; Set the hWnd
//			pop eax
//			pop esi
//		}
//
//		hr = msgListWMail_->SetFolder(curFolderId_, 0, 0, 0, 0, 0);
//
//		_asm {
//			push esi
//			mov esi, [pMsgList]
//			mov DWORD ptr [esi+70h], 0
//			pop esi
//		}
//
//		if(FAILED(hr)) {
//			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: SetFolder\n"));
//			msgListWMail_->Release();
//			msgListWMail_ = NULL;
//			LeaveCriticalSection(&msgSelectionCS_);
//			return;
//		}
//		hr = msgListWMail_->GetMessageTable(&msgTable_);
//		if(FAILED(hr)) {
//			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: GetMessageTable\n"));
//			msgListWMail_->Release();
//			msgListWMail_ = NULL;
//			LeaveCriticalSection(&msgSelectionCS_);
//			return;
//		}
//
//		hr = msgTable_->SetSortInfo(sortOrder, NULL);
//		if(FAILED(hr)) {
//			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: SetSortInfo\n"));
//			msgList_->Release();
//			msgTable_->Release();
//			msgList_ = NULL;
//			msgTable_ = NULL;
//			LeaveCriticalSection(&msgSelectionCS_);
//			return;
//		}
//
////		DestroyMsgTables();
////		curFolderId_ = -1;
//	}
//	else {
//		hr = msgList_->SetFolder(curFolderId_, 0, 0, 0, 0);
//		if(FAILED(hr)) {
//			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: SetFolder\n"));
//			msgList_->Release();
//			msgList_ = NULL;
//			LeaveCriticalSection(&msgSelectionCS_);
//			return;
//		}
//		hr = msgList_->GetMessageTable(&msgTable_);
//		if(FAILED(hr)) {
//			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: GetMessageTable\n"));
//			msgList_->Release();
//			msgList_ = NULL;
//			LeaveCriticalSection(&msgSelectionCS_);
//			return;
//		}
//		hr = msgTable_->SetSortInfo(sortOrder, NULL);
//		if(FAILED(hr)) {
//			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMsgTable: SetSortInfo\n"));
//			msgList_->Release();
//			msgTable_->Release();
//			msgList_ = NULL;
//			msgTable_ = NULL;
//			LeaveCriticalSection(&msgSelectionCS_);
//			return;
//		}
//	}
//
//	LeaveCriticalSection(&msgSelectionCS_);
//}
//

//---------------------------------------------------------------------------//
void OEAPIManager::SetSelectedFolderName(HWND hTree)
{
	TVITEM itemInfo;
	HTREEITEM hSelItem;
	FOLDERID selFolderId;
	HRESULT hr;
	LPFOLDERNOTIFYEX folderNot;


	hSelItem = TreeView_GetSelection(hTree);
	if(!hSelItem) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::SetSelectedFolderName: Can't get selection %08x.\n"), hTree);
		return;
	}

	ZeroMemory(&itemInfo, sizeof(itemInfo));
	itemInfo.hItem = hSelItem;
	itemInfo.lParam = 0;

	itemInfo.mask = TVIF_HANDLE | TVIF_PARAM;
	if(!TreeView_GetItem(hTree, &itemInfo)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::SetSelectedFolderName: Error TreeView_GetItem.\n"));
		return;
	}

	if(itemInfo.lParam == 0) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::SetSelectedFolderName: Null lParam.\n"));
		return;
	}

#ifndef _WIN64
	LONG* p = (LONG*)itemInfo.lParam;
#else
	ULONGLONG* p = (ULONGLONG*)itemInfo.lParam;
#endif

#ifndef _WIN64
	// TODO: WinMail uses 64 bits to store folderId
	if(IsWMail() && p[3] != 0) {
		selFolderId = OE_INVALID_FOLDER_ID;
	}
	else {
		selFolderId = p[2];
	}
#else
	selFolderId = p[2];
#endif

	switch(selFolderId)	{
		case OE_OUTLOOK_EXPRESS_FOLDER_ID:
			selFolderId = OE_INVALID_FOLDER_ID;
			break;
		case OE_LOCAL_FOLDER_UI_ID:
			selFolderId = OE_LOCAL_FOLDER_STORE_ID;
			break;
	}

	if(selFolderId != curFolderId_) {
		// Only destroy messages if we really change folders
		DestroyMsgTables();	
	}


	OEAPIMessageStore::FolderType type = folder_unknown;
	if(selFolderId != OE_INVALID_FOLDER_ID && msgStore_ != NULL) {
		hr =  msgStore_->GetFolderType(selFolderId, &type);
	}
	else {
		hr = E_FAIL;
	}
	if(selFolderId != OE_INVALID_FOLDER_ID && FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::SetSelectedFolderName: Can't get folder type %08x.\n"), hr);
		return;
	}

	if(selFolderId != OE_LOCAL_FOLDER_STORE_ID && type != folder_local_mail) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::SetSelectedFolderName: Not local folder selected %d:%d.\n"), selFolderId, type);
		selFolderId = OE_INVALID_FOLDER_ID;
		curFolderId_ = OE_INVALID_FOLDER_ID;
		//return;
	}
//		folderEventFinished_ = FALSE;

	folderNot = (LPFOLDERNOTIFYEX) CoTaskMemAlloc(sizeof(FOLDERNOTIFYEX));
	folderNot->type = FOLDER_PROPS_CHANGED;
	folderNot->idFolderNew = folderNot->idFolderOld = selFolderId;

	EnterCriticalSection(&msgSelectionCS_);
	memcpy(&lastFolderProp_, folderNot, sizeof(FOLDERNOTIFYEX));
	curFolderId_ = folderNot->idFolderNew;
	LeaveCriticalSection(&msgSelectionCS_);

	//ResetGUIFlag();
	::ResetEvent(folderChangeEvent_);
	::PostMessage(hWnd_, WM_FOLDERNOTIFY+OEAPI_MSG_BASE, 0, (WPARAM) folderNot);
	WaitGUIEvent(folderChangeEvent_);
}
//
////---------------------------------------------------------------------------//
//void OEAPIManager::SetSelectedFolderName(TCHAR *name, int depth, BOOL localFolder)
//{
//	return;
////	EnterCriticalSection(&msgSelectionCS_);
////
////	// folder selection starts with a call of this function with all the parameters.
////	// if the folder is local, a FOLDER_PROPS_CHANGED is triggered after and this function
////	// is called using the default values of the parameters.
////	// FOLDER_PROPS_CHANGED can be triggered in other situations but the folderSelPending_
////	// flag will not be set and will be filtered.
////	if(name) {
////		folderEventFinished_ = FALSE;
////
////		curFolderId_ = -1;
////		folderPropsCount_ = 0;
////
////		if(selMsgIds_) {
////			delete [] selMsgIds_;
////			selMsgIds_ = NULL;
////			selCount_ = 0;
////		}
////
////		curMsgId_ = -1;
////
////		DestroyMsgTables();
////
////		// if it is a local folder and it isn't the root folder, a FOLDER_PROPS_CHANGED
////		// will be received.
////		if(localFolder && depth > 1) {
////			folderSelPending_ = TRUE;
////
////			_tcsncpy(lastFolderName_, name, sizeof(lastFolderName_)/sizeof(TCHAR)-1);
////			lastFolderName_[sizeof(lastFolderName_)/sizeof(TCHAR)-1] = 0;
////		}
////
////		PostMessage(GetCallbackWindow(), MESSAGE_SELECTION_CHANGED_CODE, 0, 0);
////
////		if(!localFolder || depth <= 1) {
////			// create a new one because the event destroy it
////			LPFOLDERNOTIFYEX folderNot = (LPFOLDERNOTIFYEX) CoTaskMemAlloc(sizeof(FOLDERNOTIFYEX));
////
////			if(localFolder) {
////				folderNot->idFolderNew = folderNot->idFolderOld = FOLDERID_ROOT;
////			}
////			else {
////				folderNot->idFolderNew = folderNot->idFolderOld = -1;
////			}
////
////			folderNot->type = FOLDER_PROPS_CHANGED;
////
////			PostMessage(hWnd_, WM_FOLDERNOTIFY+OEAPI_MSG_BASE, 0, (WPARAM) folderNot);
////		}
////
////		LeaveCriticalSection(&msgSelectionCS_);
////
//////		while(!folderEventFinished_) {
//////			MSG msg;
//////			if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
//////				/*if(msg.hwnd == hWnd_) {
//////					::DispatchMessage(&msg);
//////				}
//////				else if((msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) ||
//////					(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)) {
//////					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
//////				}
//////				else */if(::GetMessage(&msg, 0, 0, 0)) {
//////					::DispatchMessage(&msg);
//////				}
//////			}
//////
//////			if(!folderEventFinished_) {
//////				Sleep(30);
//////			}
//////		}
////
////		return;
////	}
////	else if(folderSelPending_ && lastFolderProp_.idFolderNew != FOLDERID_ROOT) {
////		folderSelPending_ = FALSE;
////
////		// create a new one because the event destroy it
////		LPFOLDERNOTIFYEX folderNot = (LPFOLDERNOTIFYEX) CoTaskMemAlloc(sizeof(FOLDERNOTIFYEX));
////
////		// wait until the WM_FOLDERNOTIFY arrives
////		if(lastFolderProp_.idFolderNew == -1) {
////			LeaveCriticalSection(&msgSelectionCS_);
////			return;
////		}
////
////		DWORD id = lastFolderProp_.idFolderNew;
////		IStoreFolder *pSF;
////
////		HRESULT hr = OEAPIManager::Get()->GetSN()->OpenFolder(id, 0, &pSF);
////		if(FAILED(hr)) {
////			CoTaskMemFree(folderNot);
////			debug_print(DEBUG_ERROR, _T("OEAPIManager::SetSelectedFolderName: OpenFolder\n"));
////			LeaveCriticalSection(&msgSelectionCS_);
////			return;
////		}
////
////		FOLDERPROPS props;
////		props.cbSize = sizeof(FOLDERPROPS);
////
////		hr = pSF->GetFolderProps(0, &props);
////		if(FAILED(hr)) {
////			CoTaskMemFree(folderNot);
////			debug_print(DEBUG_ERROR, _T("OEAPIManager::SetSelectedFolderName: GetFolderProps\n"));
////			pSF->Release();
////			LeaveCriticalSection(&msgSelectionCS_);
////			return;
////		}
////
////#ifdef _UNICODE
////		std::wstring folderName;
////#else
////		std::string folderName;
////#endif // _UNICODE
////
////		folderName = props.szName;
////
////		memcpy(folderNot, &lastFolderProp_, sizeof(FOLDERNOTIFYEX));
////
////		pSF->Release();
////
////		curFolderId_ = folderNot->idFolderNew;
////
////		PostMessage(hWnd_, WM_FOLDERNOTIFY+OEAPI_MSG_BASE, 0, (WPARAM) folderNot);
////
////		while(!folderEventFinished_) {
////			MSG msg;
////			if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
////				if(msg.hwnd == hWnd_) {
////					::DispatchMessage(&msg);
////				}
////				else if((msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) ||
////					(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)) {
////					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
////				}
////				else if(::GetMessage(&msg, 0, 0, 0)) {
////					::DispatchMessage(&msg);
////				}
////			}
////
////			if(!folderEventFinished_) {
////				Sleep(30);
////			}
////		}
////	}
////
////	LeaveCriticalSection(&msgSelectionCS_);
//}
//
//---------------------------------------------------------------------------//
void OEAPIManager::SetLastSelFolderProp(LPFOLDERNOTIFYEX folNotify)
{
	memcpy(&lastFolderProp_, folNotify, sizeof(FOLDERNOTIFYEX));
	folderPropsCount_++;
}

//---------------------------------------------------------------------------//
void OEAPIManager::PushPendingSendCommand(LPARAM lParam)
{
	debug_print(DEBUG_TRACE, _T("OEAPIManager::PushPendingSendCommand: %08x.\n"), lParam);
	synchroPendingParams_.push_back(lParam);
}

//---------------------------------------------------------------------------//
//void OEAPIManager::SetSynchroPending(BOOL pending, LPARAM lParam)
//{ 
//	//debug_print(DEBUG_TRACE, _T("OEAPIManager::SetSynchroPending: %08x.\n"), lParam);
//	synchroPending_ = pending; 
//	SendRecvLparam_ = lParam; 
//}

//---------------------------------------------------------------------------//
void OEAPIManager::StartSynchroPending()
{
	debug_print(DEBUG_TRACE, _T("OEAPIManager::StartSynchroPending:.\n"));
	if(!synchroPendingParams_.empty()) {
		LPARAM lparam = synchroPendingParams_.front();
		synchroPendingParams_.pop_front();
		debug_print(DEBUG_TRACE, _T("OEAPIManager::StartSynchroPending: %08x.\n"), lparam);
		::PostMessage(GetSendDlg(), SEND_MSG_CODE, 0, lparam);
	}
	else {
		debug_print(DEBUG_TRACE, _T("OEAPIManager::StartSynchroPending: NULL.\n"));
		::PostMessage(GetSendDlg(), SEND_MSG_CODE, 0, NULL);
	}
//	LPARAM lparam = SendRecvLparam_;
//	SetSynchroPending(FALSE, NULL);
//	::PostMessage(GetSendDlg(), SEND_MSG_CODE, 0, lparam);
}

//---------------------------------------------------------------------------//
//void OEAPIManager::ForceSynchroPending(BOOL receive)
//{
//	//debug_print(DEBUG_TRACE, _T("OEAPIManager::ForceSynchroPending:.\n"));
//	if(IsSynchroPending()) {
//		SetSynchroPending(FALSE, NULL);
//	}
//	if(!IsWMail()) {
//		if(receive) {
//			::PostMessage(GetSendDlg(), SEND_MSG_CODE, 0, SNDRCV_MSG_LPARAM);
//		}
//		else {
//			::PostMessage(GetSendDlg(), SEND_MSG_CODE, 0, SEND_MSG_LPARAM);
//		}
//	}
//	else {
//		::SetProp(GetSendDlg(), _T("NKT#SNDRCV"), (HANDLE)TRUE);
//		HWND hMain = OEAPI_GetOEMainWindowHandle();
//		if(receive) {
//			::PostMessage(hMain, WM_COMMAND, OE_WM_CMD_SENDRECV, 0);
//		}
//		else {
//			::PostMessage(hMain, WM_COMMAND, OE_WM_CMD_SENDALL, 0);
//		}
//	}
//}

//---------------------------------------------------------------------------//
void OEAPIManager::SendAndReceiveMessages(BOOL bReceive)
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::SendAndReceiveMessages.\n"));

	NktAutoLock a(&uiCS_);
	//EnterCriticalSection();
	
	if(IsSynchroInProgress()) {
		if(!IsSynchroPending()) {
			synchroPendingParams_.push_back(-1);
			//SetSynchroPending(TRUE);
		}
	}
//	else {
//		if(IsSynchroPending() && SendRecvLparam_ != NULL) {
//			StartSynchroPending();
//		}
//		else {
//			// TODO: I've added this PostMessage to send message to newsgroups but with
//			// this line OE doesn't send normal messages. Fix this to work in both cases.
////			PostMessage(GetSendDlg(), SEND_MSG_CODE, 0, 0x120101);
//			ForceSynchroPending(bReceive);
//		}
//	}

	//LeaveCriticalSection(&uiCS_);
}


//---------------------------------------------------------------------------//
void OEAPIManager::SendMailMessage(int folderId, int msgId)
{
	// Will handle lock
	NktAutoLock autoLock(&uiCS_);

	// In WinMail we only can send message that are in outbox
	//if(IsWMail() && folderId != outboxFolderId_) {
	//	debug_print(DEBUG_ERROR, _T("OEAPIManager::SendMailMessage: .\n"));
	//	return;
	//}

	if(!IsSynchroInProgress()) {
		CreateMessageInOutbox(folderId, msgId);
	}
	else {
		//if(IsWMail()) {
		//	HRESULT hr;
		//	IMimeMessagePtr	pMimeMsg;
		//	LONG newMsgId = -1;

		//	hr = msgStore_->OpenMimeMessage(folderId, msgId, &pMimeMsg);
		//	if(FAILED(hr)) {
		//		debug_print(DEBUG_ERROR, _T("OEAPIManager::SendMessage: Can't read message %08x.\n"), hr);
		//		return;
		//	}

		//	hr = msgStore_->SaveMimeMessage(deletedFolderId_, 0, pMimeMsg, (LPMESSAGEID)&newMsgId);
		//	if(FAILED(hr)) {
		//		debug_print(DEBUG_ERROR, _T("OEAPIManager::SendMessage: Can't save message: %08x %d.\n"), hr, newMsgId);
		//		return;
		//	}

		//	folderId = deletedFolderId_;
		//	msgId = newMsgId;
		//}
		//else {
			HRESULT hr;
			IMimeMessagePtr	pMimeMsg;
			LONG newMsgId = -1;

			hr = msgStore_->OpenMimeMessage(folderId, msgId, &pMimeMsg);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEAPIManager::SendMessage: Can't read message %08x.\n"), hr);
				return;
			}

			hr = msgStore_->SaveMimeMessage(deletedFolderId_, 0x000010089, pMimeMsg, (LPMESSAGEID)&newMsgId);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEAPIManager::SendMessage: Can't save message: %08x %d.\n"), hr, newMsgId);
				return;
			}

			folderId = deletedFolderId_;
			msgId = newMsgId;
		//}

		MessageInfo msgInfo;

		msgInfo.folderId = folderId;
		msgInfo.msgId = msgId;

		sendMsgInfoList_.push_back(msgInfo);

		debug_print(DEBUG_INFO, _T("OEAPIManager::SendMessage: Message enqueued.\n"));
	}
	// LeaveCriticalSection(&uiCS_);
}

//---------------------------------------------------------------------------//
void OEAPIManager::CreateMessageInOutbox(int folderId, int msgId, BOOL deleteAfter)
{
	if(IsWMail()) {
		CreateOutboxMessageWM(folderId, msgId, deleteAfter);
	}
	else {
		CreateOutboxMessageOE(folderId, msgId, deleteAfter);
	}
}

//---------------------------------------------------------------------------//
void OEAPIManager::CreateOutboxMessageOE(int folderId, int msgId, BOOL deleteAfter)
{
	HRESULT hr;
	//std::basic_string<TCHAR> accId;
	//std::basic_string<TCHAR> accName;
	PROPVARIANT value;
	IStoreFolderPtr pSF; // = NULL; //, *pOutbox = NULL;
	IMimeMessagePtr pMimeMsg; // = NULL;
	DWORD newId;
	DWORD state = 0;
	AccountInfoOE accInfo;

	while(1) {
		if(pSN_ == NULL) {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: NULL pSN.\n"));
			break;
		}

		hr = pSN_->OpenFolder(folderId, 0, &pSF);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: OpenFolder\n"));
			break;
		}

		if(!GetDefaultAccountSettingsOE(&accInfo)) {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: GetDefaultAccountSettings.\n"));
			break;
		}

		// Open it as an IMimeMessage interface
		hr = pSF->OpenMessage(msgId,
							IID_IMimeMessage,
							(LPVOID*)&pMimeMsg);

		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: OpenMessage\n"));
			break;
		}

		value.vt = VT_LPSTR;

		// test if the property exists. If not set the default account.
		hr = pMimeMsg->GetBodyProp(HBODY_ROOT, PIDTOSTR(PID_ATT_ACCOUNT), NOFLAGS, &value);
		if(FAILED(hr)) {
			value.vt = VT_LPSTR;
			value.pszVal = (LPSTR) accInfo.accountId; // accId.c_str();

			hr = pMimeMsg->SetBodyProp(HBODY_ROOT, PIDTOSTR(PID_ATT_ACCOUNT), NOFLAGS, &value);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: SetBodyProp\n"));
				break;
			}

			value.vt = VT_LPSTR;
			value.pszVal = (LPSTR) accInfo.accountName; // accName.c_str();

			hr = pMimeMsg->SetBodyProp(HBODY_ROOT, "att:athena-account-name", NOFLAGS, &value);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: SetBodyProp\n"));
				break;
			}
		}
		else {
			PropVariantClear(&value);
		}

		state |= MSG_SUBMITTED | MSG_UNSENT;

		hr = outbox_->SaveMessage(IID_IMimeMessage, pMimeMsg, state, &newId);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: SaveMessage\n"));
			break;
		}

		if(deleteAfter) {
			MESSAGEIDLIST msgIdList;
			DWORD msgIds[] = {msgId};

			msgIdList.cbSize = sizeof(MESSAGEIDLIST);
			msgIdList.cMsgs = 1;
			msgIdList.prgdwMsgId = msgIds;

			hr = pSF->DeleteMessages(&msgIdList, 0, 0);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateMessageInOutbox: DeleteMessages.\n"));
				break;
			}
		}
		break;
	}

	//if(pSF) {
	//	pSF->Release();
	//}
	//if(pMimeMsg) {
	//	pMimeMsg->Release();
	//}
}

//---------------------------------------------------------------------------//
void OEAPIManager::CreateOutboxMessageWM(int folderId, int msgId, BOOL deleteAfter)
{
	AccountInfoWM info;
	HRESULT hr;

	//if(folderId != outboxFolderId_) {
	//	// Only work if the message is in outbox
	//	return;
	//}

	if(!GetDefaultAccountSettingsWM(&info)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: Can't get account info.\n"));
		return;
	}

	IMimeMessagePtr message;
	hr = msgStore_->OpenMimeMessage(folderId, msgId, &message);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: OpenMimeMessage failed %08x.\n"), hr);
		return;
	}

	MESSAGEID newMsgId = 0;
	hr = msgStore_->SaveMimeMessage(outboxFolderId_, 0, message, &newMsgId);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: SaveMimeMessage failed %08x.\n"), hr);
		return;
	}

	//IMessageFolderWMailPtr msgFolder;
	//hr = msgStore_->OpenFolder(folderId, (IMessageFolder**)&msgFolder);
	//if(FAILED(hr)) {
	//	debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: OpenFolder failed %08x.\n"), hr);
	//	return;
	//}

	//IMimeMessagePtr message;
	//hr = msgFolder->OpenMessage(msgId, 0, 0, &message, NULL);
	//if(FAILED(hr)) {
	//	debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: OpenMessage failed %08x.\n"), hr);
	//	return;
	//}

	//IMessageFolderWMailPtr outbox;
	//hr = msgStore_->OpenFolder(outboxFolderId_, (IMessageFolder**)&outbox);
	//if(FAILED(hr)) {
	//	debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: OpenFolder failed %08x.\n"), hr);
	//	return;
	//}

	//msgFolder = NULL;

	//MESSAGEID newMsgId = -1;
	//LONGLONG newMsgId;
	//hr = outbox->SaveMessage((MESSAGEID*)&newMsgId, 1, 0, NULL, message, NULL);
	//if(FAILED(hr)) {
	//	debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: SaveMessage failed %08x.\n"), hr);
	//	return;
	//}

	message = NULL;

	IMessageFolderWMailPtr outbox;
	hr = msgStore_->OpenFolder(outboxFolderId_, (IMessageFolder**)&outbox);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: OpenFolder failed %08x.\n"), hr);
		return;
	}

	MESSAGEINFOWMAIL msgInfo = {0};
	msgInfo.dwMessageId = newMsgId;
	hr = outbox->FindRecord(0, -1, &msgInfo, NULL);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: FindRecord failed %08x.\n"), hr);
		return;
	}

	LPCSTR accountId = msgInfo.szAccountGUID;
	LPCWSTR accountName = msgInfo.wzAccountName;
	LPCWSTR displayFrom = msgInfo.wzDisplayFrom;
	LPCSTR sender = msgInfo.szSender;

	msgInfo.szAccountGUID = info.accountId;
	msgInfo.wzAccountName = info.accountName;
	msgInfo.wzDisplayFrom = info.displayFrom;
	msgInfo.szSender = info.sender; 

	//msgInfo.dwARF |= 0x000010089;	// read
	msgInfo.dwARF = 0x000010009;	// unread

	hr = outbox->UpdateRecord(&msgInfo);

	msgInfo.szAccountGUID = accountId;
	msgInfo.wzAccountName = accountName;
	msgInfo.szSender = sender;
	msgInfo.wzDisplayFrom = displayFrom;

	outbox->FreeRecord(&msgInfo);

	outbox = NULL; // ->Release();

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: UpdateRecord failed %08x.\n"), hr);
		return;
	}

	if(deleteAfter) {
		hr = msgStore_->DeleteMessage(folderId, msgId, deleteAfter);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEAPIManager::CreateOutboxMessageWM: DeleteMessage failed %08x.\n"), hr);
		}
	}
}

//---------------------------------------------------------------------------//
BOOL OEAPIManager::SelectFolderID(DWORD selFolderId)
{
	HWND hTree = OEAPI_GetTreeViewHandle();
	if(hTree == NULL) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::SelectFolderID: TreeView NULL.\n"));
		return FALSE;
	}

	// Get "Local folders"
	HTREEITEM hRoot = TreeView_GetRoot(hTree);
	if(!IsWMail()) {
		hRoot = TreeView_GetChild(hTree, hRoot);
	}

	HTREEITEM hItem = FindHTreeItem(hTree, hRoot, selFolderId);

	if(hItem != NULL) {
		TreeView_EnsureVisible(hTree, hItem);
		TreeView_Select(hTree, hItem, TVGN_CARET);
	}

	return FALSE;
}

//---------------------------------------------------------------------------//
HTREEITEM OEAPIManager::FindHTreeItem(HWND hTree, HTREEITEM hRoot, LONG folderId)
{
	if(folderId == rootFolderId_)	{
		return hRoot;
	}

	FOLDERID parentId;
	HRESULT hr = msgStore_->GetParentId(folderId, &parentId);

	if(FAILED(hr) || parentId == OE_INVALID_FOLDER_ID) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::FindHTreeItem: Can't get parent id %d %08x.\n"), folderId, hr);
		return NULL;
	}

	HTREEITEM hParent = FindHTreeItem(hTree, hRoot, parentId);

	if(hParent == NULL) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::FindHTreeItem: Parent is null %d.\n"), folderId);
		return NULL;
	}

	return FindChild(hTree, hParent, folderId);
}

//---------------------------------------------------------------------------//
HTREEITEM OEAPIManager::FindChild(HWND hTree, HTREEITEM hParent, LONG folderId)
{
	HTREEITEM hItem;
	hItem = TreeView_GetChild(hTree, hParent);
	while(hItem) {
		TVITEM tv;
		ZeroMemory(&tv, sizeof(tv));
		tv.mask = TVIF_HANDLE | TVIF_PARAM;
		tv.hItem = hItem;
		tv.lParam = 0;
		TreeView_GetItem(hTree, &tv);
		if(tv.lParam != 0) {
#ifndef _WIN64
			LONG* p = (LONG*)tv.lParam;
#else
			ULONGLONG* p = (ULONGLONG*)tv.lParam;
#endif
			if(p[2] == folderId) {
				return hItem;
			}
		}
		hItem = TreeView_GetNextSibling(hTree, hItem);
	}
	return NULL;
}

//---------------------------------------------------------------------------//
void OEAPIManager::OpenCurrentMessage()
{
	PostMessage(OEAPI_GetOEMainWindowHandle(), WM_COMMAND, 0x00019ca4, 0);
}

//---------------------------------------------------------------------------//
void OEAPIManager::ComposeMessage()
{
	PostMessage(OEAPI_GetOEMainWindowHandle(), WM_COMMAND, 0x00009e11, 0);
}

//---------------------------------------------------------------------------//
void OEAPIManager::OnDatabaseTransaction(OEStoreTransactionType txType, OEStoreFolderType ft,
								DWORD folId, DWORD msgId1, DWORD arf1, DWORD msgId2, DWORD arf2)
{
	if(ft == folder_local_mail) {
		if((txType & oeapi_tx_insert) ||
			(txType & oeapi_tx_delete) ||
			(txType & oeapi_tx_unread) ||
			(txType & oeapi_tx_read) ||
			(txType == oeapi_tx_folder_move) ||
			(txType == oeapi_tx_folder_rename)) {

			DatabaseTx* tx = new DatabaseTx;				
			tx->folderId = folId;
			tx->msgId = msgId1;
			tx->txType = txType;
			tx->parentId = (txType == oeapi_tx_folder_move) ? msgId2 : OE_INVALID_FOLDER_ID;

			::ResetEvent(databaseEvent_);
			::PostMessage(hWnd_, DATABASE_TRANSACTION_CODE, (WPARAM)tx, (LPARAM)0);
			WaitGUIEvent(databaseEvent_);
		}
	}
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnDatabaseChange(OEStoreTransactionType dt, int folderId, int msgId, int parentId)
{
	if(!EnterEvent()) {
		return;
	}

	debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnDatabaseChange: %d.\n"), ::GetCurrentThreadId());

#ifdef ENTERPRISE_VERSION
	if(dt == OE_TR_INSERT_MESSAGE && folderId == outboxFolderId_) {
		if(GetSendDlg()) {
			//g_sendDlgSyncblocked = TRUE;
			if(!IsSendWindowLocked())
				LockSendWindow(TRUE);

			RaiseOnNewMessageInOutbox(msgId);

			SetLastOutboxMsgProcessed(msgId);

			//g_sendDlgSyncblocked = FALSE;
			LockSendWindow(FALSE);
		}
		else {
			debug_print(DEBUG_ERROR, _T("OutboxWndProc: Error Cannot find SendWnd (#32770).\n"));
		}
	}
#endif

	BOOL msgDeleted = FALSE;
	if(IsWMail() && dt == OE_TR_INSERT_MESSAGE && folderId == deletedFolderId_)	{
		 msgDeleted = RemoveMessageWMail(msgId);
	}

	if(!msgDeleted) {
		EnterCriticalSection(&uiCS_);

		for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
			TOEAPIObj *oe = *it;

			oe->OnDatabaseChange((comet::OEAPI::tagOE_DATABASE_TRANSACTION)dt, folderId, msgId, parentId);
		}

		LeaveCriticalSection(&uiCS_);
	}

	if(dt == OE_TR_INSERT_MESSAGE && folderId == outboxFolderId_) {
		// If mail flagged as Send Later then do not issue Send and Receive
		if(GetSendLater()) {
			// Reset flag
			SetSendLater(FALSE);
		}
		else if(GetSendMailImmediatelyKeyValue() && !GetOfflineMode()) {
			// Send but don't receive
			if(IsSynchroPending()) {
				StartSynchroPending();
				//OEAPIManager::Get()->SendAndReceiveMessages(FALSE);
			}
		}
	}

	LeaveEvent();

	::SetEvent(databaseEvent_);
}

//---------------------------------------------------------------------------//
BOOL OEAPIManager::RemoveMessageWMail(int msgId) 
{
	BOOL ret = FALSE;
	HRESULT hr = E_FAIL;
	ULONG address = -1;

	IMessageFolderWMailPtr deletedItems;
	hr = msgStore_->OpenFolder(deletedFolderId_, (IMessageFolder**)&deletedItems);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::RemoveMessageWMail: Can't open folder %08x.\n"), hr);
		return ret;
	}

	IMimeMessagePtr pMimeMsg; // = NULL;
	hr = deletedItems->OpenMessage(msgId, 0, &pMimeMsg, NULL);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::RemoveMessageWMail: Can't open message %08x.\n"), hr);
		return ret;
	}

	PROPVARIANT value;
	value.vt = VT_LPSTR;
	hr = pMimeMsg->GetBodyProp(HBODY_ROOT, DELETE_MSG_HEADER, 0, &value);
	if(SUCCEEDED(hr) && hr != S_FALSE) {
		if(!strcmp(value.pszVal, "True")) {
			//pMimeMsg->Release();
			pMimeMsg = NULL;
	
			MESSAGEIDLIST msgIdList;
			DWORD msgIds[] = {msgId, 0, 0, 0};

			msgIdList.cbSize = 1; // sizeof(MESSAGEIDLIST);
			msgIdList.cMsgs = 1;
			msgIdList.prgdwMsgId = msgIds;

			hr = deletedItems->DeleteMessages(4, &msgIdList, NULL, NULL); 
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEAPIManager::RemoveMessageWMail: DeleteMessages failed %08x.\n"), hr);
			}
			else {
				ret = TRUE;
			}
		}
		PropVariantClear(&value);
	}

	//if(pMimeMsg) {
	//	pMimeMsg->Release();
	//	pMimeMsg = NULL;
	//}

	return ret;
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnButtonClicked(int toolbarId, int buttonId, HWND wndMsgId)
{
	if(!EnterEvent()) {
		return;
	}

	OEPluginToolbarBtn *button = OEPluginToolbarMgr::Get()->GetButton(buttonId);
	if(button != NULL) {
		TOEButton *but = GetButton(buttonId);
		if(but) {
			if(wndMsgId == INVALID_MSGWND_ID) {
				but->OnClick();
			}
#ifdef ENTERPRISE_VERSION
			else {
				but->OnMsgWndClick((INT)wndMsgId);
			}
#endif // ENTERPRISE_VERSION
		}

		TOEToolbar *toolbar = GetToolbar(button->GetToolbarID());
		if(toolbar) {
			if(wndMsgId == INVALID_MSGWND_ID) {
				toolbar->OnButtonClicked(buttonId);
			}
#ifdef ENTERPRISE_VERSION
			else {
				toolbar->OnButtonMsgWndClicked(buttonId, (INT)wndMsgId);
			}
#endif // ENTERPRISE_VERSION
		}

#ifdef ENTERPRISE_VERSION
		TOEMsgWnd *msgWnd = GetMsgWnd((INT)wndMsgId);
		if(msgWnd) {
			msgWnd->OnToolbarButtonClicked(toolbarId, buttonId);
		}
#endif // ENTERPRISE_VERSION

		TOEAPIObj *oeapi = GetOEAPI(button->GetOEAPIID());
		if(oeapi) {
			if(wndMsgId == INVALID_MSGWND_ID) {
				oeapi->OnToolbarButtonClicked(toolbarId, buttonId);
			}
#ifdef ENTERPRISE_VERSION
			else {
				oeapi->OnToolbarButtonMsgWndClicked(toolbarId, buttonId, (INT)wndMsgId);
			}
#endif // ENTERPRISE_VERSION
		}

		button->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("RaiseOnButtonClicked: Cannot find object\n"));
	}

	LeaveEvent();
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnMenuItemClicked(int menuIndex, int menuItemId, HWND wndMsgId)
{
	if(!EnterEvent()) {
		return;
	}

	EnterCriticalSection(&uiCS_);

	OEMenuItemRepr *item = GetMenuItemRepr(menuItemId);
	if(item == NULL) {
		debug_print(DEBUG_ERROR, _T("RaiseOnMenuItemClicked: Cannot find class representation\n"));

		LeaveCriticalSection(&uiCS_);
		return;
	}

	TOEMenuItem *i = GetMenuItem(menuItemId);
	if(i) {
		if(wndMsgId == INVALID_MSGWND_ID) {
			i->OnClick();
		}
#ifdef ENTERPRISE_VERSION
		else {
			i->OnMsgWndClick((INT)wndMsgId);
		}
#endif
	}

	TOEMenu *menu = GetMenu(item->GetMenuID());
	if(menu) {
		if(wndMsgId == INVALID_MSGWND_ID) {
			menu->OnItemClicked(menuItemId);
		}
#ifdef ENTERPRISE_VERSION
		else {
			menu->OnMsgWndItemClicked(menuItemId, (INT)wndMsgId);
		}
#endif
	}

#ifdef ENTERPRISE_VERSION
	TOEMsgWnd *msgWnd = GetMsgWnd((INT)wndMsgId);	// Fix this for 64 bits
	if(msgWnd) {
		msgWnd->OnMenuItemClicked(menuIndex, menuItemId);
	}
#endif

	TOEAPIObj *oeapi = GetOEAPI(item->GetOEAPIID());
	if(oeapi) {
		if(wndMsgId == INVALID_MSGWND_ID) {
			oeapi->OnMenuItemClicked(menuIndex, menuItemId);
		}
#ifdef ENTERPRISE_VERSION
		else {
			oeapi->OnMsgWndMenuItemClicked(menuIndex, menuItemId, (INT)wndMsgId);
		}
#endif
	}

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnFolderSelectionChangedEvent(DWORD newSelFolderId)
{
	if(!EnterEvent()) {
		return;
	}

	EnterCriticalSection(&uiCS_);

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnFolderSelectionChangedPrivate(newSelFolderId);
	}

	LeaveCriticalSection(&uiCS_);

	//SetGUIFlag();
	//folderEventFinished_ = TRUE;
	::SetEvent(folderChangeEvent_);

	LeaveEvent();
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnMessageSelectionChangedEvent()
{
	if(!EnterEvent()) {
		return;
	}

	EnterCriticalSection(&uiCS_);

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnMessageSelectionChanged();
	}

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnCurrentMessageChangedEvent(DWORD newCurMsgId)
{
	if(!EnterEvent()) {
		return;
	}

	EnterCriticalSection(&uiCS_);

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnCurrentMessageChanged(newCurMsgId);
	}

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnMessageDownloadStartedEvent()
{
	if(!EnterEvent()) {
		return;
	}

	EnterCriticalSection(&uiCS_);

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnMessageDownloadStarted();
	}

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnMessageDownloadFinishedEvent()
{
	debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnMessageDownloadFinishedEvent.\n"));
	if(!EnterEvent()) {
		return;
	}

	SetSynchroInProgress(FALSE);

	// let finish all notifications of new messages because it's a common workaround to
	// handle OnNewMessage notification only between OnMessageDownloadStarted and
	// OnMessageDownloadFinishedEvent
	::Sleep(1500);	

	EnterCriticalSection(&uiCS_);

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnMessageDownloadFinished();
	}

	//MessageInfoList::iterator itMsgInfo = sendMsgInfoList_.begin();

	while(sendMsgInfoList_.size() != 0) {
		debug_print(DEBUG_INFO, _T("RaiseOnMessageDownloadFinishedEvent: Send message.\n"));

		MessageInfo &msgInfo = sendMsgInfoList_.front();

		CreateMessageInOutbox(msgInfo.folderId, msgInfo.msgId, TRUE);
		sendMsgInfoList_.pop_front();
	}

	if(IsSynchroPending()) {
		debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnMessageDownloadFinishedEvent: Synchro pending.\n"));
		//if(SendRecvLparam_ != NULL) {
			StartSynchroPending();
		//}
		//else {
		//	ForceSynchroPending();
		//}
	}

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnSendButtonMsgWndClicked(HWND msgWndId, BOOL bLater)
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnSendButtonMsgWndClicked: Enter.\n"));
#ifdef ENTERPRISE_VERSION
	OEPluginMsgWnd *msgWnd;

	if(!EnterEvent()) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::RaiseOnSendButtonMsgWndClicked: Can't enter event.\n"));
		return;
	}

	EnterCriticalSection(&uiCS_);

	//debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnSendButtonMsgWndClicked: Blocking SendDlg %d.\n"), ::GetCurrentThreadId());

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnSendButtonMsgWndClicked((INT)msgWndId);
	}

	LeaveCriticalSection(&uiCS_);

	msgWnd = MsgWndMgr.GetMsgWnd(msgWndId);
	if(msgWnd) {
		msgWnd->SetPendingSendEvent(FALSE);
		if(!msgWnd->SendCancelled()) {
			bLater_ = bLater;
			msgWnd->GetTopLevelWndMgr()->SendMail(bLater);
		}
		msgWnd->Unlock();
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::RaiseOnSendButtonMsgWndClicked: Can't get OEPluginMsgWnd\n"));
	}

	LeaveEvent();

#endif // ENTERPRISE_VERSION
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnSendButtonMsgWndClicked: Leave.\n"));
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnNewMsgWnd(HWND msgWndId)
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnNewMsgWnd: Enter.\n"));
	if(!EnterEvent()) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::RaiseOnNewMsgWnd: Can't enter event.\n"));
		return;
	}

	EnterCriticalSection(&uiCS_);

	HookSendWnd();

#ifdef ENTERPRISE_VERSION
	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnNewMsgWnd((INT)msgWndId);
	}
#endif // ENTERPRISE_VERSION

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnNewMsgWnd: Leave.\n"));
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnMsgWndClosed(HWND msgWndId, BOOL isMainWindow)
{
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnMsgWndClosed: Enter.\n"));
	if(!EnterEvent()) {
		debug_print(DEBUG_ERROR, _T("OEAPIManager::RaiseOnMsgWndClosed: Can't enter event.\n"));
		return;
	}

	EnterCriticalSection(&uiCS_);

#ifdef ENTERPRISE_VERSION
	TOEMsgWnd *msgWnd = GetMsgWnd((INT)msgWndId);
	if(msgWnd) {
		msgWnd->OnClose(isMainWindow);
	}
#endif // ENTERPRISE_VERSION


#ifdef ENTERPRISE_VERSION
	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnMsgWndClosed((INT)msgWndId, isMainWindow);
	}
#endif // ENTERPRISE_VERSION

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();

	::SetEvent(msgWndCloseEvent_);
	//debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnMsgWndClosed: Leave.\n"));
}

//---------------------------------------------------------------------------//
void OEAPIManager::RaiseOnNewMessageInOutbox(int msgId)
{
	debug_print(DEBUG_TRACE, _T("OEAPIManager::RaiseOnNewMessageInOutbox: %d.\n"), msgId);

#ifdef ENTERPRISE_VERSION
	if(!EnterEvent()) {
		return;
	}

	EnterCriticalSection(&uiCS_);

	for(OEAPIList::iterator it=oeapis_.begin(); it!=oeapis_.end(); it++) {
		TOEAPIObj *oe = *it;

		oe->OnNewMessageInOutbox(msgId);
	}

	LeaveCriticalSection(&uiCS_);

	LeaveEvent();

#endif // ENTERPRISE_VERSION
}

//---------------------------------------------------------------------------//
BOOL OEAPIManager::IsReadToShutdown()
{
	BOOL result;

	EnterCriticalSection(&uiCS_);

	result = oeapis_.empty() && msgWnds_.empty() && toolbars_.empty() &&
		toolbars_.empty() && buttons_.empty() && menus_.empty() && menuItems_.empty();

	LeaveCriticalSection(&uiCS_);

	return result;
}

#ifdef ENTERPRISE_VERSION
//---------------------------------------------------------------------------//
BOOL OEAPIManager::CheckNewMessagesInOutbox()
{
	BOOL ret = FALSE;
	LONG maxId = msgStore_->GetMaxMessageId(outboxFolderId_);
	if(((lastProcessedOutboxMsgId_ == -1) && (maxId != -1))
		|| ((maxId != -1) && (maxId > lastProcessedOutboxMsgId_))) {
		ret = TRUE;
	}
	return ret;
}
#endif 

//---------------------------------------------------------------------------//
void OEAPIManager::NotifyMessageWindowClosed(DWORD msgWndId, BOOL isMainWindow)
{
	if(!isMainWindow) {
		::ResetEvent(msgWndCloseEvent_);
		::PostMessage(GetCallbackWindow(), MSG_WND_CLOSED_CODE, (WPARAM)msgWndId, isMainWindow);
		WaitGUIEvent(msgWndCloseEvent_);
	}
	else {
		::PostMessage(GetCallbackWindow(), MSG_WND_CLOSED_CODE, (WPARAM)msgWndId, isMainWindow);
	}
}

//---------------------------------------------------------------------------//
void OEAPIManager::LockSendWindow(BOOL lock)
{
	if(lock) {
		::InterlockedIncrement(&lockCountSendWindow_);
	}
	else {
		::InterlockedDecrement(&lockCountSendWindow_);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////
INT __cdecl ButtonClickedCallback(INT toolbarId, INT buttonId, HWND wndMsgId)
{
	ButtonInfo *info = new ButtonInfo;

	info->buttonId = buttonId;
	info->toolbarId = toolbarId;
	info->wndMsgId = wndMsgId;

	// asynch call because this thread is marked as input_sync and cannot make a
	// inter-process call (it returns RPC_E_CANTCALLOUT_ININPUTSYNCCALL)
	PostMessage(OEAPIManager::Get()->GetCallbackWindow(), BUTTON_CLICKED_MESSAGE_CODE, (WPARAM) info, 0);

	return 0;
}

//---------------------------------------------------------------------------//
INT __cdecl MenuItemClickedCallback(HWND msgWndId, INT menuIndex, INT buttonId)
{
	MenuItemInfo* info = new MenuItemInfo;
	info->menuIndex = menuIndex;
	info->menuItemId = buttonId;
	info->wndMsgId = msgWndId;

 	// asynch call because this thread is marked as input_sync and cannot make a
 	// inter-process call (it returns RPC_E_CANTCALLOUT_ININPUTSYNCCALL)
	PostMessage(OEAPIManager::Get()->GetCallbackWindow(), MENU_ITEM_CLICKED_MESSAGE_CODE, (WPARAM) info, 0);

	return 0;
}

//---------------------------------------------------------------------------//
// Event called when the folder list view changes the selection. This value is used to skip
// some incorrect FOLDER_PROPS_CHANGED when a message is moved. This is a second verification.
// When a Hotmail folder is selected or a newsgroup folder this event is generated but not
// the IStoreNamespace event, so we can know that another folder (Unknown) is selected.
INT __cdecl FolderSelChangedCallback(HWND hTree)
{
	OEAPIManager::Get()->SetSelectedFolderName(hTree);
	return 0;
}
//INT __cdecl FolderSelChangedCallback(TCHAR *folderName, INT depth, BOOL localFolder)
//{
//	OEAPIManager::Get()->SetSelectedFolderName(folderName, depth, localFolder);
//	return 0;
//}

//---------------------------------------------------------------------------//
INT __cdecl MessageSelChangedCallback(INT count, INT *msgIndexs, INT focusIndex, BOOL regenerateTable)
{
	OEAPIManager::Get()->SetSelectedMessages(count, msgIndexs, focusIndex, regenerateTable);
	return 0;
}

//INT __cdecl MessageListViewOrderItemChangedCallback(INT colInfo, INT ascendant)
//{
//	OEAPIManager::Get()->UpdateMsgListViewOrder(colInfo, ascendant);
//	return 0;
//}
//
//INT __cdecl MessageListViewHeaderOrderChangedClickedCallback(INT column, INT newPos)
//{
//	OEAPIManager::Get()->UpdateMsgListViewHeaderOrderChanged(column, newPos);
//	return 0;
//}

//INT __cdecl MessageListViewHeaderOrderRegeneratedCallback(INT count, INT *orderArray)
//{
//	OEAPIManager::Get()->SetMessagesListViewOrderArray(count, orderArray);
//	return 0;
//}

//---------------------------------------------------------------------------//
INT __cdecl MessageDownloadCallback(INT param)
{
	OEAPIManager *manager = OEAPIManager::Get();

	PostMessage(manager->GetCallbackWindow(), MESSAGE_DOWNLOAD_CODE, param, 0);

	return 0;
}

//---------------------------------------------------------------------------//
INT __cdecl MsgWndCallback(HWND msgWndId, INT when)
{
	OEAPIManager *manager = OEAPIManager::Get();

	// creation: verify if there is a current message.
	// if not, it's a empty message window
	if(when == 0) {
		// we don't need to lock as this code is in the GUI thread.
//		OEPluginMsgWnd *msgWnd;
//		msgWnd = MsgWndMgr.GetMsgWnd((HWND) msgWndId, FALSE);
//		if(msgWnd && manager->GetCurrentMessageID() == -1) {
//			msgWnd->SetEmptyMsgWnd(TRUE);
//		}
//		// if the selection is pending, we assume that the window is a empty window
//		if(msgWnd && (OEAPIManager::Get()->IsFolderSelectionPending() ||
//			!OEAPI_IsAnyMessageSelected())) {
//			msgWnd->SetEmptyMsgWnd(TRUE);
//		}
	}
	else if(when == 1) {
		PostMessage(manager->GetCallbackWindow(), MSG_WND_CREATION_CODE, (WPARAM)msgWndId, 0);
	}
	// window closed
	else if(when == 2) {
		OEPluginMsgWnd *msgWnd = MsgWndMgr.GetMsgWnd(msgWndId, FALSE);

		//manager->ResetGUIFlag();
		if(msgWnd && !msgWnd->IsMainWndMsgWnd()) {
			manager->NotifyMessageWindowClosed((DWORD)msgWndId, 0);
			//PostMessage(manager->GetCallbackWindow(), MSG_WND_CLOSED_CODE, (WPARAM)msgWndId, 0);
			//manager->WaitGUIEvent();
		}
		else {
			manager->NotifyMessageWindowClosed((DWORD)msgWndId, 1);
			//PostMessage(manager->GetCallbackWindow(), MSG_WND_CLOSED_CODE, (WPARAM)msgWndId, 1);
		}
	}

	return 0;
}

//---------------------------------------------------------------------------//
INT __cdecl MsgWndSendButtonPressedCallback(HWND msgWndId, BOOL bLater)
{
	OEAPIManager *manager = OEAPIManager::Get();

	PostMessage(manager->GetCallbackWindow(), MSG_WND_SEND_BUTTON_CODE, (WPARAM)msgWndId, bLater);

	return 0;
}

//---------------------------------------------------------------------------//
VOID _cdecl MessageStoreRegisterEventsCallback(BOOL unregister = FALSE)
{
	OEAPIManager *manager = OEAPIManager::Get();
	if(manager != NULL) {
		manager->RegisterMessageStoreEvents(unregister);
	}
}

//---------------------------------------------------------------------------//
INT _cdecl SendWindowCallback(INT action)
{
	INT ret = FALSE;
	OEAPIManager *manager = OEAPIManager::Get();
	
	if(action == SendWindow_Lock) {
		manager->LockSendWindow(TRUE);
	}
	else if(action == SendWindow_Unlock) {
		manager->LockSendWindow(FALSE);
	}
	else if(action == SendWindow_Status) {
		ret = manager->IsSendWindowLocked();
	}
	return ret;
}

//---------------------------------------------------------------------------//
LRESULT CALLBACK OEAPIWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	OEAPIManager *mgr = NULL;

	if(uMsg > WM_USER) {
		mgr = OEAPIManager::Get();

		if(mgr == NULL) {
			debug_print(DEBUG_ERROR, _T("OEAPIWndProc: Cannot get manager\n"));
			return FALSE;
		}
	}

	if(uMsg == BUTTON_CLICKED_MESSAGE_CODE) {
		ButtonInfo *info = (ButtonInfo *) wParam;

		INT buttonId = info->buttonId;
		INT toolbarId = info->toolbarId;
		HWND wndMsgId = info->wndMsgId;

		mgr->RaiseOnButtonClicked(toolbarId, buttonId, wndMsgId);
		delete info;
	}
	else if(uMsg == MENU_ITEM_CLICKED_MESSAGE_CODE) {
		MenuItemInfo* info = (MenuItemInfo*) wParam;
		INT buttonId = info->menuItemId;
		INT menuIndex = info->menuIndex;
		HWND wndMsgId = info->wndMsgId;

		mgr->RaiseOnMenuItemClicked(menuIndex, buttonId, wndMsgId);
		delete info;
	}
	else if(uMsg == MESSAGE_SELECTION_CHANGED_CODE) {
		mgr->RaiseOnMessageSelectionChangedEvent();
	}
	else if(uMsg == MESSAGE_CURRENT_CHANGED_CODE) {
		mgr->RaiseOnCurrentMessageChangedEvent((DWORD)wParam);
	}
	else if(uMsg == MESSAGE_DOWNLOAD_CODE) {
		if(wParam == 0) {
			mgr->RaiseOnMessageDownloadStartedEvent();
		}
		else if(wParam == DownloadFinishedCode) {
			mgr->RaiseOnMessageDownloadFinishedEvent();
		}
	}
	else if(uMsg == MSG_WND_CREATION_CODE) {
		mgr->RaiseOnNewMsgWnd((HWND)wParam);
	}
	else if(uMsg == MSG_WND_CLOSED_CODE) {
		mgr->RaiseOnMsgWndClosed((HWND)wParam, (BOOL)lParam);
		//mgr->SetGUIFlag();
	}
	else if(uMsg == MSG_WND_SEND_BUTTON_CODE) {
		mgr->RaiseOnSendButtonMsgWndClicked((HWND)wParam, (BOOL)lParam);
	}
	else if(uMsg == DATABASE_TRANSACTION_CODE) {
		//debug_print(DEBUG_TRACE, _T("OEAPIWndProc: DATABASE_TRANSACTION_CODE.\n"));
		OEAPIManager::DatabaseTx *tx = (OEAPIManager::DatabaseTx *) wParam;
		mgr->RaiseOnDatabaseChange(tx->txType, tx->folderId, tx->msgId, tx->parentId);
		delete tx;
#ifdef OEAPI_DATABASECHANGE_USE_EVENT
		::SetEvent((HANDLE)lParam);
#else
		//mgr->SetGUIFlag();
#endif

	}
	// from this event cannot generate COM events because the thread is marked as
	// user-input. To finally generate the event a message must be posted.
	else if(uMsg == WM_FOLDERNOTIFY) {
// TODO: Review if this code is used
//		LPFOLDERNOTIFYEX folderNot = (LPFOLDERNOTIFYEX) lParam;
//
//		// after a message NEW_FOLDER or DELETE_FOLDER, a FOLDER_PROPS_CHANGED occurs that is not a folder
//		// selection change. There is a callback that is called when the folder Tree View
//		// changes its selection.
//		// this event must be saved to verify if it has to be skipped.
//		if(folderNot->type == FOLDER_PROPS_CHANGED) {
//			STOREFOLDERID lastId = mgr->GetLastSelectedFolder();
//
//			mgr->SetLastSelFolderProp(folderNot);
//
////			mgr->SetSelectedFolderName();
//		}
	}
	else if(uMsg == WM_FOLDERNOTIFY+OEAPI_MSG_BASE) {
		LPFOLDERNOTIFYEX folderNot = (LPFOLDERNOTIFYEX) lParam;
		mgr->RaiseOnFolderSelectionChangedEvent(folderNot->idFolderNew);
		CoTaskMemFree((LPVOID) lParam);
	}
	// another OEAPIInit client finished initialization.
	else if(!g_OEAPIInitState.is_null() && uMsg == WM_OEAPI_INIT_NOTIFICATION_MSG_CODE) {
		if(g_OEAPIInitState->GetOEAPIInitInitializedObjectsCount() >= g_OEAPIInitState->GetOEAPIInitObjectsCount()) {
//			g_sendDlgSyncblocked = FALSE;
//			if(OEAPIManager::Get()->IsSynchroPending()) {
//				OEAPIManager::Get()->SendAndReceiveMessages();
//			}
			HANDLE initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_PLUGINS_INITIALIZED_EVENT_NAME);
			SetEvent(initEventHandle);
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//---------------------------------------------------------------------------//
HWND CreateCallbackWindow()
{	
	WNDCLASSEX wndclass;
	const TCHAR szOEAPISink[] = OEAPI_OECOM_CALLBACK_CLASS;

	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= 0;
	wndclass.lpfnWndProc	= &OEAPIWndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstOecom;
	wndclass.hIcon			= NULL;
	wndclass.hCursor		= NULL;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szOEAPISink;
	wndclass.hIconSm		= NULL;

	// Register it
	RegisterClassEx(&wndclass);

	// And create a window
	HWND hWnd = CreateWindow(szOEAPISink,
				OEAPI_OECOM_CALLBACK_NAME,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				400, 200,
				NULL,
				NULL,
				hInstOecom,
				NULL);

	if (hWnd == NULL) {
		debug_print(DEBUG_ERROR, _T("CreateCallbackWindow: CreateWindow() failed.\n"));
		return NULL;
	}

	return hWnd;
}

#ifdef EVALUATION_VERSION

#include "resource.h"

extern HINSTANCE hInstOecom;
HWND hAboutDlg = NULL;

BOOL CALLBACK AboutOEAPIDlgProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch (msg)
    {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            if(LOWORD(wParam) == IDOK) {
				EndDialog(hwnd, IDOK);
				return TRUE;
            }
    }

    return FALSE;
}

INT __cdecl AboutOEAPI(HWND, INT, INT)
{
	hAboutDlg = CreateDialogParam( hInstOecom, MAKEINTRESOURCE(IDD_ABOUT),
						g_oehWnd,
					    (DLGPROC) AboutOEAPIDlgProc,
					    (LONG) 0);
	return 0;
}

#endif // EVALUATION_VERSION

//---------------------------------------------------------------------------//
void LoadPlugin(HWND hwnd)
{
//	MWShutdowned = FALSE;

	g_oehWnd = hwnd;
	AttachPlugin(hwnd, hInstOecom);
	// Set the function that will be called when the OE window is destroyed
	OEAPI_SetDetachCallback(ExitServerCallback);
	// Set the function that will be called when the OE window is destroyed
	OEAPI_SetFolderSelChangedCallback(FolderSelChangedCallback);
	// Set the function that will be called when the selected message changes
	OEAPI_SetMessageSelChangedCallback(MessageSelChangedCallback);
	// Set the function that will be called when the order item of the messages list view changes
//	OEAPI_SetMessageListViewOrderItemChangedCallback((OEAPI_MessageOrderChangedCallback) MessageListViewOrderItemChangedCallback);
	// Set the function that will be called when the any column of the header of the messages list view changes the position clicked
//	OEAPI_SetMessageListViewHeaderColumnOrderChangedCallback((OEAPI_HeaderItemsOrderChangedCallback) MessageListViewHeaderOrderChangedClickedCallback);
	// Set the function that will be called when the column dialog is displayed and the user
	// changes the order, add or remove columns of the messages list view
	// the array sent in the callback is the internal order array of OE, the same obtained
	// in GetStartupOrderInformation from the registry at startup
//	OEAPI_SetMessageListViewHeaderColumnOrderRegeneratedCallback((OEAPI_HeaderOrderRegeneratedCallback) MessageListViewHeaderOrderRegeneratedCallback);

	// Set the function that will be called when OE starts / finishes downloading messages
	// from servers.
	OEAPI_SetMessageDownloadCallback(MessageDownloadCallback);

	// Set the function that will be called when OE starts / finishes downloading messages
	// from servers.
	OEAPI_SetMessageDownloadCallback(MessageDownloadCallback);

#ifdef ENTERPRISE_VERSION
	// Set the function called when a new msg wnd is created
	OEAPI_SetMsgWndCallback(MsgWndCallback);

	// Set the function called when the 'Send' button of a msg wnd is pressed
	OEAPI_SetMsgWndSendButtonCallback(MsgWndSendButtonPressedCallback);
#endif // ENTERPRISE_VERSION

	OEAPI_SetRegisterMessageStoreEvents(MessageStoreRegisterEventsCallback);

	OEAPI_SetSendWindowCallback(SendWindowCallback);
}

#ifndef STATIC_LIBRARY
void UnloadRegisteredPlugins();
#endif // STATIC_LIBRARY

//---------------------------------------------------------------------------//
INT ExitServerCallback(BOOL logOff)
{
	if(!logOff) {
		if(MsgWndMgr.GetMsgWndCount() != 0 || OEAPI_GetOEMainWindowHandle()) {
			return 0;
		}
	}

	// delete the toolbar
	delete OEPluginToolbarMgr::Get();

	pThis->SetAboutToShutdown(TRUE);

	// while a pending event wait and process messages.
	// new events are not triggered.
	while(pThis->GetActiveEventCount() > 0) {
		MSG msg;
		if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
			if(::GetMessage(&msg, 0, 0, 0)) {
				::DispatchMessage(&msg);
			}
			else {
				::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			}
		}

		Sleep(100);
	}

	// update the events for oeapiinitcom
	HANDLE shutdownEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_SHUTDOWN_EVENT_NAME);
	HANDLE initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_INIT_EVENT_NAME);

	ResetEvent(initEventHandle);
	SetEvent(shutdownEventHandle);

	CloseHandle(shutdownEventHandle);
	CloseHandle(initEventHandle);

	if(hServer != 0) {
		// skip this staff when the user is logging off because we cannot communicate
		// with other process.
		if(!logOff) {
			// wait until there is no client object living
			// dispatch message to let the shutdown event be processed.
			do {
				MSG msg;
				if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
					if(::GetMessage(&msg, 0, 0, 0)) {
						::DispatchMessage(&msg);
					}
					else {
						::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
					}
				}

				Sleep(100);

			} while(!pThis->IsReadToShutdown());
		}

#ifndef STATIC_LIBRARY
		//UnloadRegisteredPlugins();
#endif // STATIC_LIBRARY

		// message tables are created in this thread and must be destroy here.
		// Otherwise, DBNotifyWndProc generates a error.
		// Weird: it was working for non-logoffs but sometimes it throws an error.
//		if(logOff) {
			pThis->DestroyMsgTables();
//		}

		DetachPlugin();

		// this event is used to be sure that we exit this function after the com server
		// shutdowned.
		// the previous code generated a exception when this thread terminated before the
		// hServer thread. Now, wait until hServer finishes. Process messages because some
		// OE objects need GUI message processing to be released.
//		HANDLE afterExitEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_AFTER_THREAD_EXIT_EVENT_NAME);
//		if(afterExitEventHandle != NULL) {
			PostThreadMessage(serverID, WM_QUIT, 0, 0);
//			WaitForSingleObject(afterExitEventHandle, INFINITE);
			while(WaitForSingleObject(hServer, 0) == WAIT_TIMEOUT) {
				MSG msg;
				if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
						if(::GetMessage(&msg, 0, 0, 0)) {
							::DispatchMessage(&msg);
						}
						else {
							::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
						}
					}

				Sleep(10);
				}

//			CloseHandle(afterExitEventHandle);
//		}
//		else {
//			debug_print(DEBUG_ERROR, _T("ExitServerCallback: Error CreateEvent afterExitEventHandle.\n"));
//			PostThreadMessage(serverID, WM_QUIT, 0, 0);
//		}


		// Doesn't seems to be a safe place to unregister plugins, 
		// is better to unregister in the same thread that loaded them.
		//UnloadRegisteredPlugins();
		//DetachPlugin();

		hServer = 0;
	}

	return 0;
}

#ifndef STATIC_LIBRARY

typedef enum OEAPIPluginType_ {
	oeapi_dll_plugin = 0,
	oeapi_iunknown_plugin = 1,
	oeapi_idtextensibility_plugin = 2
} OEAPIPluginType;

struct PluginInfo {
	union {
		HMODULE modHandle;
		IUnknown* pUnknown;
	};
	OEAPIPluginType type;
	// BOOL isHMODULE;
} LoadedPlugins[1024];

DWORD LoadedPluginsCount = 0;

//struct __declspec(uuid("b65ad801-abaf-11d0-bb8b-00a0c90f2744"))
//_IDTExtensibility2 : IDispatch
//{
//    //
//    // Raw methods provided by interface
//    //
//
//      virtual HRESULT __stdcall OnConnection (
//        /*[in]*/ IDispatch * Application,
//        /*[in]*/ enum ext_ConnectMode ConnectMode,
//        /*[in]*/ IDispatch * AddInInst,
//        /*[in]*/ SAFEARRAY * * custom ) = 0;
//      virtual HRESULT __stdcall OnDisconnection (
//        /*[in]*/ enum ext_DisconnectMode RemoveMode,
//        /*[in]*/ SAFEARRAY * * custom ) = 0;
//      virtual HRESULT __stdcall OnAddInsUpdate (
//        /*[in]*/ SAFEARRAY * * custom ) = 0;
//      virtual HRESULT __stdcall OnStartupComplete (
//        /*[in]*/ SAFEARRAY * * custom ) = 0;
//      virtual HRESULT __stdcall OnBeginShutdown (
//        /*[in]*/ SAFEARRAY * * custom ) = 0;
//};
//
//extern "C" const GUID __declspec(selectany) IID__IDTExtensibility2 =
//    {0xb65ad801,0xabaf,0x11d0,{0xbb,0x8b,0x00,0xa0,0xc9,0x0f,0x27,0x44}};

//---------------------------------------------------------------------------//
void LoadPluginFromString(LPCTSTR valueData) 
{
	HMODULE mod = LoadLibrary(valueData);
	if(mod) {
		LoadedPlugins[LoadedPluginsCount].modHandle = mod;
		LoadedPlugins[LoadedPluginsCount].type = oeapi_dll_plugin; // .isHMODULE = TRUE;
		LoadedPluginsCount++;
		FARPROC pInit = GetProcAddress(mod, "InitializePlugin");
		if(pInit) {
			pInit();
		}
		return;
	}

	// Get CLSID for our server...
	CLSID clsid;
	TCHAR pzValue[1024];
	HRESULT hr;

	// Convert down to ANSI
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, 0, valueData, -1, pzValue, sizeof(pzValue)/sizeof(WCHAR)-1);
#else
	_tcscpy_s(pzValue,sizeof(pzValue)/sizeof(pzValue[0]),valueData);
#endif

	hr = CLSIDFromProgID(pzValue, &clsid);
	if(FAILED(hr)) {
		//hr = CLSIDFromString(pzValue, &clsid);
		//if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("LoadPluginFromString: CLSIDFromProgID failed %s %08x.\n"), valueData, hr);
			return;
		//}
	}

	IUnknown *pUnknown = NULL;
	// Start server and get IDispatch...
	hr = CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IUnknown, (void **) &pUnknown);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("LoadPluginFromString: Error CoCreateInstance loading %s hr = %08x.\n"), valueData, hr);
	}
	else {
		LoadedPlugins[LoadedPluginsCount].pUnknown = pUnknown;
		LoadedPlugins[LoadedPluginsCount].type = oeapi_iunknown_plugin; // .isHMODULE = FALSE;
		LoadedPluginsCount++;
	}
}

//---------------------------------------------------------------------------//
//void LoadIDTExtensibility2Plugin(LPCTSTR valueData) 
//{
//	// Get CLSID for our server...
//	CLSID clsid;
//	WCHAR pzValue[1024];
//	HRESULT hr;
//
//	// Convert down to ANSI
//#ifndef UNICODE
//	MultiByteToWideChar(CP_ACP, 0, valueData, -1, pzValue, sizeof(pzValue)/sizeof(WCHAR)-1);
//#else
//	wcscpy(pzValue, valueData);
//#endif
//
//	hr = CLSIDFromString(pzValue, &clsid);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("LoadIDTExtensibility2Plugin: CLSIDFromString failed %s %08x.\n"), valueData, hr);
//		return;
//	}
//
//	IUnknown* pUnknown = NULL;
//
//	// Start server and get IDispatch...
//	hr = CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IUnknown, (void **) &pUnknown);
//	if(FAILED(hr)) {
//		debug_print(DEBUG_ERROR, _T("LoadIDTExtensibility2Plugin: Error CoCreateInstance loading %s hr = %08x.\n"), valueData, hr);
//	}
//	else {
//		_IDTExtensibility2* pExtensibility2 = NULL;
//		hr = pUnknown->QueryInterface(IID__IDTExtensibility2, (void**)&pExtensibility2);
//		pUnknown->Release();
//		if(FAILED(hr)) {
//			debug_print(DEBUG_ERROR, _T("LoadPluginFromString: Failed to load %s hr = %08x.\n"), valueData, hr);
//			return;
//		}
//		LoadedPlugins[LoadedPluginsCount].modHandle = pExtensibility2;
//		LoadedPlugins[LoadedPluginsCount].type = oeapi_idtextensibility_plugin; 
//		LoadedPluginsCount++;
//	}
//}

//---------------------------------------------------------------------------//
void LoadPluginsInKey(NktRegistry& reg)
{
	TCHAR valueName[1024];
	TCHAR valueData[1024];
	DWORD valueNameSize, valueDataSize, valueType;

	// NktRegistry reg = hKey;
	LONG ret;

	//for(int i=0;; i++) {
	//	valueNameSize = sizeof(valueName)/sizeof(valueName[0]);

	//	ret = reg.EnumKey(i, valueName, &valueNameSize, NULL, NULL, NULL);

	//	if(ret != ERROR_SUCCESS) {
	//		if(ret != ERROR_NO_MORE_ITEMS) {
	//			debug_print(DEBUG_ERROR, _T("LoadPluginsInKey: RegEnumKey error %d.\n"), ret);
	//		}
	//		break;
	//	}

	//	LoadIDTExtensibility2Plugin(valueData);
	//} //

	for(int i=0;; i++) {
		valueNameSize = sizeof(valueName)/sizeof(valueName[0]);
		valueDataSize = sizeof(valueData)/sizeof(valueData[0]);

		ret = reg.EnumValue(i, valueName, &valueNameSize, &valueType, (LPBYTE)valueData, &valueDataSize);

		if(ret != ERROR_SUCCESS) {
			if(ret != ERROR_NO_MORE_ITEMS) {
				debug_print(DEBUG_ERROR, _T("LoadPluginsInKey: RegEnumValue error %d.\n"), ret);
			}
			break;
		}

		if(valueType != REG_SZ) {
			debug_print(DEBUG_ERROR, _T("LoadPluginsInKey: Unknow value type %d.\n"), valueType);
			continue;
		}

		LoadPluginFromString(valueData);
	}
    

}

//---------------------------------------------------------------------------//
void LoadRegisteredPlugins()
{
	NktRegistry reg;

	//std::basic_string<TCHAR> pluginPath = GetResourceString(hInstOecom, IDS_PLUGIN_PATH);
	std::basic_string<TCHAR> pluginPath = OEAPI_OECOM_PLUGINS_KEY;

	if(reg.Open(HKEY_LOCAL_MACHINE, pluginPath.c_str(), KEY_QUERY_VALUE) == ERROR_SUCCESS) {
		LoadPluginsInKey(reg);
	}

	if(reg.Open(HKEY_CURRENT_USER, pluginPath.c_str(), KEY_QUERY_VALUE) == ERROR_SUCCESS) {
		LoadPluginsInKey(reg);
	}
}

//---------------------------------------------------------------------------//
void UnloadRegisteredPlugins()
{
	for(DWORD i=0; i<LoadedPluginsCount; i++) {
		if(LoadedPlugins[i].type == oeapi_dll_plugin) {
			FreeLibrary(LoadedPlugins[i].modHandle);
		}
		else if(LoadedPlugins[i].type == oeapi_iunknown_plugin) {
			IUnknown *pPlugin = LoadedPlugins[i].pUnknown;
			pPlugin->Release();
		}
		else if(LoadedPlugins[i].type == oeapi_idtextensibility_plugin) {
		}
	}

	LoadedPluginsCount = 0;
}

// the thread that is created we don't need it and it was left running when a user switched
// identities.
class oeapi_exe_server : public exe_server<type_library> {
public:
	oeapi_exe_server(HINSTANCE instance) : exe_server<type_library>(instance) {}
	DWORD thread_main()
	{
		return 0;
	}

#ifdef OEAPI_USE_CUSTOM_MESSAGE_LOOP
	HRESULT run()
	{
		//thread::start();

		HRESULT hr;
		hr = register_class_objects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE);

		if (SUCCEEDED(hr))
		{
			MSG msg;
			BOOL ret;
			while ((ret = ::GetMessage(&msg, 0, 0, 0)) != 0) {
				if (ret == -1) {
					break;
				}
				else {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
		}
		//else if (thread::running())
		//{
		//	shutdown_event_.set();
		//	thread::wait(0);
		//}

		revoke_class_objects();
		Sleep(1000); //wait for any threads to finish

		module().shutdown();

		return hr;
	}
#endif
};


//---------------------------------------------------------------------------//
DWORD _stdcall OEAPICOMServerProc(void *)
{
	HRESULT hr;
	HANDLE shutdownEventHandle;
	HANDLE initEventHandle;
//	HANDLE afterExitEventHandle;

#ifdef EVALUATION_VERSION
	if(!VerifyExpiration()) {
		return 0;
	}
#endif // EVALUATION_VERSION

	//LoadRegisteredPlugins();

	if(CoInitialize(0) == S_OK) {
		// Needs to be called AFTER CoInitialize
		
        //LoadRegisteredPlugins();
        LoadPluginFromString(_T("symdemo.dll"));

		oeapi_exe_server server(hInstOecom);
//		exe_server<type_library> server(hInstOecom);

		// register_server function uses the hInstOecom to register the typelibrary and the
		// COCLASS_TABLE. In this case, the typelibrary must be registered with the dll
		// hInstance and the COCLASS_TABLE with the exe HMODULE
//		server.register_server();

		// it isn't possible to register the server as normal users are unable to register
		// it must be registered in installation
		// if the library is registered here it needs administrator privileges.
//		DllRegisterServer();

		// TODO: this prevents the server to shutdown after an application release it.
		// The event should be set in a different way to set it when necessary.
		module_t &m = comet::module();
		m.clear_shutdown_event();

		// This code was copied from comet (exe_server<type_library>::run)
		// I need to set the init flag after registration, so I can't call run
		hr = server.register_class_objects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("ServerProc: Error registering server register_class_objects.\n"));
		}

		// force Manager creation in this thread		
		OEAPIManager::Get();

		// reset: OE shouldn't get messages from server until all OEInit clients finish
		// processing the OnOEAPIInit event
		g_initializedCount = 0;

		BOOL setEvent = FALSE;

		if(!g_OEAPIInitState.is_null()) {
			g_OEAPIInitState->ResetOEAPIInitInitializedObjectsCount();

			// let all OEAPIInit clients return from the OnOEAPIInit function before OE starts
			// message download.
			// if there are no OEAPIInit objects don't wait their initialization.
			if(g_OEAPIInitState->GetOEAPIInitObjectsCount() == 0) {
				setEvent = TRUE;
			}
		}
		else {
			setEvent = TRUE;
		}

		if(setEvent) {
			HANDLE initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_PLUGINS_INITIALIZED_EVENT_NAME);
			SetEvent(initEventHandle);
		}

		shutdownEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_SHUTDOWN_EVENT_NAME);
		initEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_INIT_EVENT_NAME);

		ResetEvent(shutdownEventHandle);
		SetEvent(initEventHandle);

//		afterExitEventHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_AFTER_THREAD_EXIT_EVENT_NAME);
//		ResetEvent(afterExitEventHandle);

		server.run();

		CloseHandle(shutdownEventHandle);
		CloseHandle(initEventHandle);

//		SetEvent(afterExitEventHandle);

//		CloseHandle(afterExitEventHandle);

		UnloadRegisteredPlugins();

		// destroy OEAPIManager from the same thread it was created.
		if(pThis != NULL) {
			delete pThis;
			pThis = NULL;
		}
		CoUninitialize();
	}

	return 0;
}


/////////////////////////////////////////////////////////
// Exported Functions
/////////////////////////////////////////////////////////
void _stdcall StartServer(HWND hwnd)
{
	if(hServer == NULL)
    {
	    if(!IsOE())
        {
			return;
		}

#ifdef EVALUATION_VERSION
		if(!VerifyExpiration(TRUE)) {
			hServer = INVALID_HANDLE_VALUE;
			return;
		}
#endif // EVALUATION_VERSION

		if(hwnd)
        {
			LoadPlugin(hwnd);

#ifdef EVALUATION_VERSION
			OEPluginMenuMgr::Get()->AddMenuItem(MsgWndStyles::OE_MSG_INVALID, 5, _T("About OEAPI"), _T("About"), TRUE, &AboutOEAPI);
			//OEAPI_AddMenuItem(5, _T("About OEAPI"), _T("About"), TRUE, &AboutOEAPI);
#endif // EVALUATION_VERSION

			// HACK: if I don't create this object before loading add-ins there could be
			// a problem sometimes: when I call FreeLibrary of the last add-in that use
			// OEAPIINITCOM library, OEAPIINITCOM library unloads before (sometimes) deleting
			// global variables, so if any global variable is a referenced counted object
			// owned by OEAPIINITCOM library it generates a exception in the Release.
			// if I create this object here I prevent the unload because this module should
			// be the owner of the library.
			
            OEAPIINITCOM::IOEAPIInitState *pInitstate = NULL;
            GetInitInstance(uuidof<OEAPIINITCOM::OEAPIInitState>(),uuidof<OEAPIINITCOM::OEAPIInitState>(),(void**)&pInitstate);
            g_OEAPIInitState = pInitstate;

            //g_OEAPIInitState = OEAPIINITCOM::OEAPIInitState::create();

			//LoadRegisteredPlugins();

			if((hServer = CreateThread(0, 0, OEAPICOMServerProc, 0, 0, &serverID)) == NULL)
            {
				debug_print(DEBUG_ERROR, _T("COM Server: CreateThread failed.\n"));
			}

			// Force OEAPIManager object creation before continue
			while(pThis == NULL)
            {
				MSG msg;
				if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
                {
					if(::GetMessage(&msg, 0, 0, 0))
                    {
						if(msg.message != 0x0413)
                        {
							::DispatchMessage(&msg);
						}
						else
                        {
							//debug_print(DEBUG_INFO, _T("COM Server: (2) msg 0x0413 skipped %08x %08x %08x.\n"), msg.hwnd, msg.wParam, msg.lParam);
							::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
						}
					}
					else
                    {
					    break;
				    }
				}

				Sleep(10);
			}

			// force to finish all initialization staff in ServerProc before continuing
			HANDLE afterInitializationFinishHandle = CreateEvent(NULL, TRUE, FALSE, OEAPI_PLUGINS_INITIALIZED_EVENT_NAME);

			// don't block because OEAPInit object needs messages to serve the events.
			MSG msg;
			while(WaitForSingleObject(afterInitializationFinishHandle, 0) == WAIT_TIMEOUT) {
				if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
					if(::GetMessage(&msg, 0, 0, 0)) {
						if(msg.message != 0x0413) {
							::DispatchMessage(&msg);
						}
						else {
							//debug_print(DEBUG_INFO, _T("COM Server: (2) msg 0x0413 skipped %08x %08x %08x.\n"), msg.hwnd, msg.wParam, msg.lParam);
							::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
						}
					}
					else {
						break;
					}
				}
			}

			CloseHandle(afterInitializationFinishHandle);

			//SetThreadPriority(hServer, THREAD_PRIORITY_HIGHEST);
		}
	}
}

//---------------------------------------------------------------------------//
void _stdcall ExitServer()
{
}

//---------------------------------------------------------------------------//
int _stdcall GetLibraryVersion()
{
	int version = OEAPI_VERSION;

#ifdef ENTERPRISE_VERSION
	version+=3;
#elif STANDARD_VERSION
	version++;
#elif EVALUATION_VERSION
#else
	version+=2;
#endif // ENTERPRISE_VERSION

	return version;
}

//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////
// Server Registration
///////////////////////////////////////////////////////////
std::basic_string<TCHAR> GetOEFullPath()
{
	NktRegistry reg;
	BOOL isWMail = FALSE;

	if(reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows Mail"), KEY_QUERY_VALUE) == ERROR_SUCCESS) {
		isWMail = TRUE;
	}
	else if(reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Outlook Express"), KEY_QUERY_VALUE) == ERROR_SUCCESS) {
	}
	else {
		return _T("");
	}

	TCHAR auxPath[2048];
	DWORD dwType = REG_SZ;
	DWORD dwSize = sizeof(auxPath);
	//TCHAR *str;
	DWORD ret;

	if((ret = reg.QueryValue(_T("InstallRoot"), &dwType, (LPBYTE)&auxPath, &dwSize) != ERROR_SUCCESS)) {
		debug_print(DEBUG_ERROR, _T("GetOEFullPath: Error RegQueryValueEx: %d.\n"), ret);
		return _T("");
	}

	TCHAR path[2048];
	::ExpandEnvironmentStrings(auxPath, path, sizeof(path)/sizeof(TCHAR)-1);

	std::basic_stringstream<TCHAR> ss;
	if(!isWMail) {
		ss << path << "\\MSIMN.EXE";
	}
	else {
		ss << path << "\\WinMail.EXE";
	}
	return ss.str();
}

//---------------------------------------------------------------------------//
//HRESULT GetOEFullPath(TCHAR *fullPath, size_t count)
//{
//	//HKEY HKeyLM;
//	NktRegistry reg;
//	HRESULT hr = E_FAIL;
//	BOOL isWMail = FALSE;
//
//	if(reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows Mail"), KEY_QUERY_VALUE) == ERROR_SUCCESS) {
//		isWMail = TRUE;
//	}
//	else if(reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Outlook Express"), KEY_QUERY_VALUE) == ERROR_SUCCESS) {
//	}
//	else {
//		return hr;
//	}
//
//	//while(1)
//	{
//		TCHAR path[2048], auxPath[2048];
//		DWORD ReturnType = REG_SZ;
//		DWORD returnsize=sizeof(path);
//		//TCHAR *str;
//		DWORD ret;
//
//		if((ret = reg.QueryValue(_T("InstallRoot"), &ReturnType, (LPBYTE) &auxPath, &returnsize) != ERROR_SUCCESS)) {
//			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
//                            NULL,
//                            ret,
//                            0,
//                            auxPath,
//                            sizeof(auxPath)-1,
//                            NULL);
//			//sprintf(path, "GetOEFullPath: Error RegQueryValueEx: %d %s.\n", ret, auxPath);
//			//dbgprint(path);
//			//break;
//			debug_print(DEBUG_ERROR, _T("GetOEFullPath: Error RegQueryValueEx: %d %s.\n"), ret, auxPath);
//			return hr;
//		}
//
//		//_tcscpy(path, auxPath);
//		::ExpandEnvironmentStrings(auxPath, path, sizeof(path)/sizeof(TCHAR)-1);
//
//		//str = _tcsstr(_tcsupr(path), _T("%PROGRAMFILES%"));
//		//if(str) {
//		//	DWORD lenStr = _tcslen(str);
//
//		//	DWORD lenVar = GetEnvironmentVariable(_T("ProgramFiles"), str, MAX_PATH);
//		//	if(lenVar == 0) {
//		//		debug_print(DEBUG_ERROR, _T("GetOEFullPath: Error GetEnvironmentVariable.\n"));
//		//		break;
//		//	}
//
//		//	// copy all the path after %PROGRAMFILES
//		//	_tcscpy(str+lenVar,
//		//		auxPath + (lenStr - _tcslen(auxPath)) + strlen("%PROGRAMFILES%"));
//		//}
//
//		if(!isWMail) {
//			_sntprintf(fullPath, count, _T("%s\\MSIMN.EXE"), path);
//		}
//		else {
//			_sntprintf(fullPath, count, _T("%s\\WinMail.EXE"), path);
//		}
//
//		hr = S_OK;
//
//		//break;
//	}
//
//	// RegCloseKey(HKeyLM);
//
//	return hr;
//}

//---------------------------------------------------------------------------//
STDAPI DllRegisterServer()
{
	TCHAR filename[2048];

	// remove garbage of old versions
	HKEY hKey;

	LPOLESTR szCLSID;
	::StringFromCLSID(__uuidof(OEAPI::type_library), &szCLSID);

	bstr_t keyName = _T("TypeLib\\");
	keyName += szCLSID;

	::CoTaskMemFree(szCLSID);

	// delete key only if SHDeleteKey exists (NT / 98 and upper)
	if(RegOpenKeyEx(HKEY_CLASSES_ROOT, keyName.t_str().c_str(),
		0, KEY_ALL_ACCESS, &hKey) == NO_ERROR) {
		HMODULE hMod = LoadLibrary(_T("Shlwapi"));
		if(hMod) {
			typedef DWORD (_stdcall *pSHDeleteKey_t)(HKEY hkey, LPSTR pszSubKey);
			pSHDeleteKey_t pSHDeleteKey;

			pSHDeleteKey = (pSHDeleteKey_t) GetProcAddress(hMod, "SHDeleteKeyA");
			if(pSHDeleteKey) {
				pSHDeleteKey(hKey, "");
			}
		}

		RegCloseKey(hKey);
	}

	if(GetModuleFileName(hInstOecom, filename, sizeof(filename)/sizeof(TCHAR)-1) == 0) {
		debug_print(DEBUG_ERROR, _T("DllRegisterServer: Error GetModuleFileName.\n"));
		return E_FAIL;
	}

	std::basic_string<TCHAR> oePath = GetOEFullPath();
	if(oePath.empty()) {
		debug_print(DEBUG_ERROR, _T("DllRegisterServer: Error GetOEFullPath.\n"));
		//return hr;
		return E_FAIL;
	}

	{
		HRESULT hr = impl::typelibrary_registration<com_server_traits<0>::embedded_tlb>::perform(filename, false);
		if(FAILED(hr)) return SELFREG_E_TYPELIB;
	}

	try {
		coclass_table<type_library::coclasses>::registration(oePath.c_str(), false, false, 0);
	}
	catch (const com_error &e)
	{
		return impl::return_com_error(e);
	}
	catch (const std::exception &e)
	{
		::OutputDebugStringA(e.what());
		return E_FAIL;
	}

	return S_OK;
}

//---------------------------------------------------------------------------//
STDAPI DllUnregisterServer()
{
	TCHAR filename[MAX_PATH*4];
	HRESULT hr;

	if(GetModuleFileName(hInstOecom, filename, sizeof(filename)/sizeof(TCHAR)-1) == 0) {
		debug_print(DEBUG_ERROR, _T("UnregisterServer: Error GetModuleFileName.\n"));
		return E_FAIL;
	}

	hr = impl::typelibrary_registration<com_server_traits<0>::embedded_tlb>::perform(filename, true);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("UnregisterServer: Error perform.\n"));
		return hr;
	}

	std::basic_string<TCHAR> oePath = GetOEFullPath(); // filename, sizeof(filename)/sizeof(TCHAR)-1);
	if(oePath.empty()) {
		debug_print(DEBUG_ERROR, _T("UnregisterServer: Can't get full path.\n"));
		return E_FAIL;
	}

	coclass_table<type_library::coclasses>::registration(oePath.c_str(), true, false, 0);
	return S_OK;
}

//---------------------------------------------------------------------------//
// Set the registry key with the oecom loaded flag set
void SetOecomLoaded(DWORD loaded)
{
	//HKEY hKey;
	NktRegistry reg;

	if(reg.Open(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY, KEY_ALL_ACCESS) != ERROR_SUCCESS) {
		if(reg.Create(HKEY_CURRENT_USER, OEAPI_NEKTRA_KEY) != ERROR_SUCCESS) {
			debug_print(DEBUG_ERROR, _T("SetOecomLoaded: Error RegCreateKey\n"));
			return;
		}
	}

	// set the flag used to load only 1 instance of oecom
	if(reg.SetValue(_T("DllLoaded"), REG_DWORD, (LPBYTE) &loaded, sizeof(loaded)) != ERROR_SUCCESS) {
		debug_print(DEBUG_ERROR, _T("SetOecomLoaded: Error RegSetValueEx\n"));
	}

	// RegCloseKey(hKey);
}

//---------------------------------------------------------------------------//
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	switch(dwReason) {
	case DLL_PROCESS_ATTACH:
		{
			// just save the hInstance of the dll
			hInstOecom = hInstance;
			break;
		}
	}

	return TRUE;
}

#endif // STATIC_LIBRARY
/* $Id: main.h,v 1.49 2009/01/28 16:13:16 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef OECOM_MAIN_H__
#define OECOM_MAIN_H__

//#include "oeapiobj.h"

#include "msoeapi.h"
#include "msgtable.h"

// there is a auto-generated oeapi.h in this project, so we must specify the path
#include "oeapi.h"

#include "messagelist.h"
#include "messagestore.h"

#include "oeundoc.h"
//#include "oestate_helpers.inl"

#define OEAPI_MSG_BASE 100
#define OEAPI_VERSION 3230

class OEMenuItemRepr;

#include "oeapiobj.h"			// for TOEAPIObj
#include "user_interface.h"		// for TOEMenu, MenuItem, Button, Toolbar
#include "oemsgwnd.h"			// for TOEMsgWnd

#include <commctrl.h>			// for HTREEITEM

typedef std::list<TOEAPIObj *> OEAPIList;
typedef std::list<TOEMsgWnd *> MsgWndList;
typedef std::list<TOEToolbar *> ToolbarList;
typedef std::list<TOEButton *> ButtonList;
typedef std::list<TOEMenu *> MenuList;
typedef std::list<TOEMenuItem *> MenuItemList;

typedef std::list<OEMenuItemRepr *> MenuItemReprList;

typedef struct _MessageInfo {
	DWORD folderId;
	DWORD msgId;
} MessageInfo;

typedef std::list<MessageInfo> MessageInfoList;

// Callback used to handle the click events on button clicks
INT __cdecl ButtonClickedCallback(INT, INT, HWND);

// Callback used to handle the click events on menu items clicks
INT __cdecl MenuItemClickedCallback(HWND, INT, INT);

/**
Initialize the static version of the plugin
hwnd must be the main window of OE and hInstance the instance of dll 
running in the address space of OE.
*/
void InitializePlugin(HINSTANCE hInstance, HWND hwnd);

class OEAPIManager;
extern OEAPIManager *pThis;


/**
Internal representation of a OEMenuItem.
*/
//---------------------------------------------------------------------------//
class OEMenuItemRepr {
public:
	OEMenuItemRepr()
	{
		id_ = oeapi_ = menu_ = -1;
	}

	void SetID(int id) { id_ = id; }
	int GetID() { return id_; }

	void SetMenuID(int id) { menu_ = id; }
	int GetMenuID() { return menu_; }

	void SetOEAPIID(int id) { oeapi_ = id; }
	int GetOEAPIID() { return oeapi_; }

private:
	int oeapi_;
	int menu_;
	int id_;
};

/**
Internal representation of a OEMenu.
*/
//---------------------------------------------------------------------------//
class OEMenuRepr {
public:
	OEMenuRepr()
	{
		id_ = oeapi_ = -1;
	}

	void SetID(int id) { id_ = id; }
	int GetID() { return id_; }

	void SetOEAPIID(int id) { oeapi_ = id; }
	int GetOEAPIID() { return oeapi_; }

private:
	int oeapi_;
	int id_;
};

///**
//Internal representation of a OEToolbar.
//*/
////---------------------------------------------------------------------------//
//class OEToolbarRepr {
//public:
//	OEToolbarRepr()
//	{
//		id_ = oeapi_ = -1;
//	}
//
//	void SetID(int id) { id_ = id; }
//	int GetID() { return id_; }
//
//	void SetOEAPIID(int id) { oeapi_ = id; }
//	int GetOEAPIID() { return oeapi_; }
//
//private:
//	int oeapi_;
//	int id_;
//};

// it is used to prevent multiple manager
//CRITICAL_SECTION MgrCriSec;

/**
This class handles all the active objects in the clients.
*/
//---------------------------------------------------------------------------//
class OEAPIManager : public OEAPIStoreListener {
public:
	typedef struct tagDatabaseTx
	{
		DWORD folderId;
		DWORD msgId;
		DWORD parentId;
		OEStoreTransactionType txType;
	} DatabaseTx;

	~OEAPIManager();

	/**
	Get the only OEAPIManager instance
	*/
	static OEAPIManager *Get(bool createIfNotExist=true)
	{
		if(pThis == NULL && createIfNotExist) {
			pThis = new OEAPIManager;
		}

		return pThis;
	}

	/**
	Get the IStoreNamespace object
	*/
	//IStoreNamespace *GetSN();

	void InitializeMessageStore();

	BOOL CreateMessageStore();

	// void GetFoldersInfo();


	/**
	Get the window used to post messages
	This window is used to generate the COM events in the same thread that is running
	*/
	HWND GetCallbackWindow() { return hWnd_; }

	/**
	This function returns TRUE while the selected folder is pending.
	This occur after the user selects another folder and the transition is not finished.
	*/
//	BOOL IsFolderSelectionPending() { return folderSelPending_; }

	/**
	Get a new menu id (unique)
	*/
	int GetNewMenuID() { return menusIDs_++; }

	/**
	Get the window that OE uses to send / recv messages and hook it.
	*/
	void HookSendWnd();

	/**
	Called when a new menu item is created.
	If parentMenuItemId is different from -1, the oeapiId is got from the parent item
	representation. Otherwise, the Menu must exist to attach the menu item to the oeapiId.
	*/
	void MenuItemCreated(TOEMenuItem *menuItem, int parentMenuItemId=-1);

	/**
	Return the menu item representation of the id.
	ALL the menu items created in the plugin have a representation.
	The instances of the COM classes can be destroyed but the representation leave while
	the object exist.
	*/
	OEMenuItemRepr *GetMenuItemRepr(int id);

	/**
	Return the oeapi object associated to the id or NULL if it there isn't any
	*/
	TOEAPIObj *GetOEAPI(int oeapiId);

	/**
	Add a oeapi object
	*/
	void AddOEAPI(TOEAPIObj *oeapi);

	/**
	Remove a oeapi object
	*/
	void RemoveOEAPI(TOEAPIObj *oeapi);

	/**
	Return the msg wnd object associated to the id or NULL if it there isn't any
	*/
	TOEMsgWnd *GetMsgWnd(int msgWndId);

	/**
	Add a msg wnd object
	*/
	void AddMsgWnd(TOEMsgWnd *msgWnd);

	/**
	Remove a msg wnd object
	*/
	void RemoveMsgWnd(TOEMsgWnd *msgWnd);

	/**
	Return the toolbar object associated to the id or NULL if it there isn't any
	*/
	TOEToolbar *GetToolbar(int toolbarId);

	/**
	Add a toolbar object
	*/
	void AddToolbar(TOEToolbar *toolbar);

	/**
	Remove a toolbar object
	*/
	void RemoveToolbar(TOEToolbar *toolbar);

	/**
	Return the button object associated to the id or NULL if it there isn't any
	*/
	TOEButton *GetButton(int buttonId);

	/**
	Add a button object
	*/
	void AddButton(TOEButton *button);

	/**
	Remove a button object
	*/
	void RemoveButton(TOEButton *button);

	/**
	Return the menu object associated to the id or NULL if it there isn't any
	*/
	TOEMenu *GetMenu(int menuID);

	/**
	Add a menu object
	*/
	void AddMenu(TOEMenu *menu);

	/**
	Remove a menu object
	*/
	void RemoveMenu(TOEMenu *menu);

	/**
	Return the menuItem object associated to the id or NULL if it there isn't any
	*/
	TOEMenuItem *GetMenuItem(int menuItemId);

	/**
	Add a menuItem object
	*/
	void AddMenuItem(TOEMenuItem *menuItem);

	/**
	Remove a menuItem object
	*/
	void RemoveMenuItem(TOEMenuItem *menuItem);

	/**
	Get the list of menu items (representation)
	*/
	MenuItemReprList &GetMenuItemsRepr() { return menuItemsRepr_; }

	/**
	Destroy the menu item associated to the itemId
	*/
	void DestroyMenuItem(int itemId);

	/**
	Destroy all the menu items associated to the oeapiId
	*/
	void DestroyAllMenuItemsByOEAPI(int oeapiId);

	/**
	Lock the UI objects (Call before calling GetMenuItemsRepr)
	*/
	void LockUIObjects();

	/**
	Unlock the UI objects
	*/
	void UnlockUIObjects();

	/**
	Wait until the pending GUI event (if any) is attended.
	This function process events and filter user input until the GUIFlag is set.
	It's used to stop the GUI until a event is processed by OEAPI clients.
	*/
	void WaitGUIEvent(HANDLE hEvent);

	/**
	Set GUI flag false. Call this function before triggering a event.
	*/
	//void ResetGUIFlag(); // { guiFlag_ = FALSE; }

	/**
	Set GUI flag true. Set this flag after the event was processed.
	*/
	//void SetGUIFlag(); // { guiFlag_ = TRUE; }

	/**
	*/
	//void MsgWaitEvent(HANDLE hEvent);

	/**
	Test if the GUI flag is set.
	*/
	//BOOL IsGUIFlagSet() { return guiFlag_; }

	/**
	Get the value of the key value where is stored OE 'Send Mail Immediately' property.
	*/
	BOOL GetSendMailImmediatelyKeyValue();

	/**
	Get the value of the key value 'Offline mode' property.
	*/
	BOOL GetOfflineMode();

	/**
	Get first selected msgId in the current folder. If the folder is not local it returns
	-1.
	*/
	DWORD GetFirstSelectedMessage();

	/**
	Return the next selected msgId after lastReturnedId.
	*/
	DWORD GetNextSelectedMessage(DWORD lastReturnedId);

	/**
	Return the current msgId.
	*/
	DWORD GetCurrentMessageID() { return curMsgId_; }

	/**
	Get the index in the list view of the msgId.
	Return -1 if not found.
	*/
	int GetMessageIndex(DWORD msgId);

	/**
	Get the id of the message specifying the index in the list view.
	Return -1 if not found.
	*/
	int GetMessageID(DWORD index);

	/**
	Get the id of the selected folder.
	If the selected folder is not local (Hotmail or a newsgroup) it returns -1;
	*/
	DWORD GetSelectedFolderID() { return curFolderId_; }

	/**
	Select a new folder in the tree view.
	*/
	BOOL SelectFolderID(DWORD selFolderId);

	/**
	Display current message in a new window: equivalent to double-click the message.<br>
	Current message is displayed in the bottom part of OE.
	*/
	void OpenCurrentMessage();

	/**
	Open a new message window to compose a message: equivalent to press 'Create Mail' button.
	*/
	void ComposeMessage();

	/**
	Find the specified folderId and return the path of folders to reach it.
	count contains the number of ids in path.
	fromId contains the id from where the search is started (use FOLDERID_ROOT for a complete
	path to the folder).
	*/
	//BOOL FindFolderID(DWORD fromId, DWORD folderId, DWORD *path, DWORD maxPath, DWORD *count);

	/**
	Set the id of the selected folder.
	*/
	void SetSelectedFolderID(LONG folderId) { curFolderId_ = folderId; }

//	/**
//	Function called when the selected folder changes to create a table of messages of the
//	selected folder.
//	*/
//	void CreateMsgTable();
//
//	/**
//	This function is called when the order item of the messages list view changed because
//	the order of the messages changed (maybe).
//	The msgTable_ must be set again.
//	if type -1, the item orderInfo was clicked and keeps the old direction if the item is
//	different from the current and changes the direction if is the same.
//	if type == 1 ascendant and 0 is descendant
//	*/
//	void UpdateMsgListViewOrder(int orderInfo, int type);
//
//	/**
//	This function is called when any column of the header changes its position. colInfo
//	is the column and newPos is the new position.
//	*/
//	void UpdateMsgListViewHeaderOrderChanged(int colInfo, int newPos);
//
//	/**
//	Set the internal OE order array. All the columns contain the absolute value that 
//	identify the data that contain in the order that they appear in the listview.
//	*/
//	void SetMessagesListViewOrderArray(int count, int *orderArray);
//
//	/**
//	Get the information about the messages order that is in the registry.
//	*/
//	void GetStartupOrderInformation();
//
//	/**
//	Get the information of the specified registry key and set the order arrays
//	*/
//	void FillOrderArrays(char *codedColArray, int bufSize, DWORD sortOrder[7], DWORD msgListViewColOrder[32]);

	/**
	This function is called from the FolderSelChangedCallback event, so we can skip incorrect
	events.
	If name == NULL name is taken from lastFolderName_.

	NOTE: Folder Selection change starts with a empty string in name (not NULL) sent by the 
	window hook of OE main window when the title of the window changes. There, message
	selection, current message and folder selection is put as invalid (-1).
	Then a FOLDER_PROPS_CHANGED is triggered if the folder is a local folder.
	At last, the message list view is created in OE main window. At that point the selection
	change is ended.
	If only the first or the first and third are triggered, the selection is kept as invalid.
	*/
	void SetSelectedFolderName(HWND hTree);
//	void SetSelectedFolderName(TCHAR *name=NULL, int depth=-1, BOOL localFolder=FALSE);

	/**
	Get the last selected folder id.
	Return -1 if the folder is not local or not setted.
	*/
	STOREFOLDERID GetLastSelectedFolder() { return lastFolderProp_.idFolderNew; }

	/**
	Return the number of FOLDER_PROPS_CHANGED since last folder change.
	*/
	int GetFolderPropsCount() { return folderPropsCount_; }

	/**
	Set the new selected messages.
	*/
	void SetSelectedMessages(int count, int *msgIndexs, int focusIndex, BOOL regenerateTable);

	/**
	Set the folder notify struct of the last FOLDER_PROPS_CHANGED event
	*/
	void SetLastSelFolderProp(LPFOLDERNOTIFYEX folNotify);

	/**
	Equivalent to press 'Send/Recv' button without displaying the dialog.
	*/
	void SendAndReceiveMessages(BOOL bReceive = TRUE);

	void StartSynchroPending();

	//void ForceSynchroPending(BOOL receive = TRUE);

	/**
	Create a message in Outbox folder to be sent.
	*/
	void SendMailMessage(int folderId, int msgId);

	/**
	Create a message in Outbox folder immediately.
	If deleteAfter is TRUE the message is deleted after making a copy in Outbox folder.
	*/
	void CreateMessageInOutbox(int folderId, int msgId, BOOL deleteAfter=FALSE);
	void CreateOutboxMessageOE(int folderId, int msgId, BOOL deleteAfter);
	void CreateOutboxMessageWM(int folderId, int msgId, BOOL deleteAfter);

	/**
	Raise the event OnClick in the button if exist a implementation and the same event
	in the toolbar and in the oeapi if exists
	*/
	void RaiseOnButtonClicked(int toolbarId, int buttonId, HWND wndMsgId);

	/**
	Raise the event OnClick in the menu item if exist a implementation and the same event
	in the oeapi if exists
	*/
	void RaiseOnMenuItemClicked(int menuId, int menuItemId, HWND wndMsgId);

	/**
	Notify that the folder selection has changed.
	Raise the event RaiseOnFolderSelectionChanged in all OEAPIObj objects
	*/
	void RaiseOnFolderSelectionChangedEvent(DWORD newSelFolderId);

	/**
	Notify that the message selection has changed.
	Raise the event RaiseOnMessageSelectionChanged in all OEAPIObj objects
	*/
	void RaiseOnMessageSelectionChangedEvent();

	/**
	Notify that the current message has changed.
	Raise the event RaiseOnCurrentMessageChanged in all OEAPIObj objects
	*/
	void RaiseOnCurrentMessageChangedEvent(DWORD newCurMsgId);

	/**
	Notify that the message download started.
	*/
	void RaiseOnMessageDownloadStartedEvent();

	/**
	Notify that the message download finished.
	*/
	void RaiseOnMessageDownloadFinishedEvent();

	/**
	Notify that the 'Send' button of a msg wnd was clicked.
	*/
	void RaiseOnSendButtonMsgWndClicked(HWND msgWndId, BOOL bLater);

	/**
	Notify that a msg wnd was created.
	*/
	void RaiseOnNewMsgWnd(HWND msgWndId);

	/**
	Notify that a msg wnd was closed.
	*/
	void RaiseOnMsgWndClosed(HWND msgWndId, BOOL isMainWindow);

	/**
	Notify that a new message is in the Outbox folder.
	This function was implemented to provide a Outbox filter and it doesn't return until
	the clients return.
	*/
	void RaiseOnNewMessageInOutbox(int msgId);

	/**
	Notify that a message or folder has changed.
	*/
	void RaiseOnDatabaseChange(OEStoreTransactionType dt, int folderId, 
								int msgId, int parentId);

	/**
	Return TRUE if there is no object living and the object can this destoyed.
	*/
	BOOL IsReadToShutdown();

	/**
	Set that the library is waiting to shutdown.
	After this flag is set no events are triggered.
	*/
	void SetAboutToShutdown(BOOL aboutToShutdown) { shutdown_ = aboutToShutdown; }

	/**
	Return TRUE if the COM server is waiting the clients to release objects to shutdown.
	*/
	BOOL IsAboutToShutdown() { return shutdown_; }

	/**
	Return the number of events that are active at this moment.
	The library cannot shutdown until this counter is 0.
	*/
	int GetActiveEventCount() { return eventCount_; }

	/**
	Return the dialog used to send the Outbox messages instantly.
	*/
	HWND GetSendDlg() { return hSendDlg_; }

	/**
	Get send dlg old window proc.
	*/
	WNDPROC GetSendDlgWndProc() { return lpOldSendDlgWndProc_; }

	/**
	Set that message download / upload started or ended.
	*/
	void SetSynchroInProgress(BOOL progress) { synchroInProcess_ = progress; }

	/**
	Get if message download / upload is in progress.
	*/
	BOOL IsSynchroInProgress() { return synchroInProcess_; }

	/**
	Set that synchronization is pending or not.
	*/
	//void SetSynchroPending(BOOL pending, LPARAM lparam = NULL);

	/**
	*/ 
	void PushPendingSendCommand(LPARAM lParam);

	/**
	Get if synchronization is pending.
	*/
	BOOL IsSynchroPending() { return !synchroPendingParams_.empty(); }

#ifdef ENTERPRISE_VERSION
	/**
	Get outbox IStoreFolder object.
	*/
	//IStoreFolder *GetOutboxFolder() { return outbox_; }

	/**
	Set the last outbox message processed by the outbox filter.
	It is used to know if the outbox messages can be sent or not.
	*/
	void SetLastOutboxMsgProcessed(DWORD msgId) { lastProcessedOutboxMsgId_ = msgId; }

	/**
	Get the last outbox message processed by the outbox filter.
	*/
	//DWORD GetLastOutboxMsgProcessed() { return lastProcessedOutboxMsgId_; }

	INT GetMaxOutboxMsgId();

#endif // ENTERPRISE_VERSION

	void SetMsgList(IUnknown *msgList, DWORD dwFolderId);

	void DestroyMsgTables();

	/**
	Called when OE database changes
	*/
	void OnDatabaseTransaction(OEAPIMessageStore::TransactionType txType, OEAPIMessageStore::FolderType ft, 
								DWORD folId, DWORD msgId1=0, DWORD arf1=0, DWORD msgId2=0, DWORD arf2=0);

	/**
	Enter a notification event. 
	Returns TRUE if the events can be triggered. When OEAPI is shutting down events are
	canceled.
	All the events must call this function in the begining and LeaveEvent when they finish.
	*/
	BOOL EnterEvent();

	/**
	This function must be called after finishing the event.
	*/
	void LeaveEvent();

	/**
	Flag next message as Sent Later
	*/
	BOOL GetSendLater() { return bLater_; }
	void SetSendLater(BOOL bLater) { bLater_ = bLater; }

	BOOL CheckNewMessagesInOutbox();

	void RegisterMessageStoreEvents(BOOL bUnregister = FALSE) {
		if(msgStore_ == NULL) 
			return;
		if(bUnregister) {
			msgStore_->UnregisterAllFoldersNotification();
		}
		else {
			msgStore_->RegisterAllFoldersNotification();
		}
	}

	void NotifyMessageWindowClosed(DWORD msgWndId, BOOL isMainWindow);

	BOOL GetHideSendWindow() { return hideSendWindow_; }

	void SetHideSendWindow(BOOL hide) { hideSendWindow_ = hide; }

	BOOL IsSendWindowLocked() { return lockCountSendWindow_ > 0; }

	void LockSendWindow(BOOL block); //  { blockSendWindow_ = block; }

protected:
	OEAPIManager();

	/**
	Auxiliary function to find recursive a folder.
	*/
	//BOOL _FindFolderID(DWORD fromId, DWORD folderId, DWORD *path, DWORD maxPath, DWORD *count);

	HTREEITEM FindHTreeItem(HWND hTree, HTREEITEM hRoot, LONG folderId);
	HTREEITEM FindChild(HWND hTree, HTREEITEM hParent, LONG folderId);
	
	/**
	*/
	BOOL RemoveMessageWMail(int msgId);

private:
	// void InitStorage();
	// void UninitStorage();

private:
	OEAPIList oeapis_;
	MsgWndList msgWnds_;
	ToolbarList toolbars_;
	ButtonList buttons_;
	MenuList menus_;
	MenuItemList menuItems_;
	int menusIDs_;
	MenuItemReprList menuItemsRepr_;
	HWND hWnd_;
	DWORD curFolderId_;
	
//	DWORD sortOrderIn_[7];		// internal array to set the correct order of incomming messages
//	DWORD msgListViewColOrderIn_[32]; // this array contains the order of the columns of the message list view of incomming messages
//	DWORD sortOrderOut_[7];		// internal array to set the correct order of out messages (Sent folder)
//	DWORD msgListViewColOrderOut_[32]; // this array contains the order of the columns of the message list view of out messages (Sent folder)
//	DWORD colOrderCount_;
	DWORD *selMsgIds_;
	DWORD selCount_;
	DWORD curMsgId_;

	// flag indicates if the COM server is waiting that the clients release objects to
	// shutdown
	BOOL shutdown_;
	// counter of events that the library is attending at this moment. It is used to
	// prevent shutdown while a event is attended.
	int eventCount_;

	CRITICAL_SECTION eventCS_;

	// flag indicates that there is a OE GUI thread pending event 
	//BOOL guiFlag_;
	//HANDLE guiEvent_;
	HANDLE msgWndCloseEvent_;
	HANDLE databaseEvent_;
	HANDLE folderChangeEvent_;

	// UI critical section
	CRITICAL_SECTION uiCS_;

	// prevent multiple access to selMsgIds_
	CRITICAL_SECTION msgSelectionCS_;

	// this var is set to store the last notify FOLDER_PROPS_CHANGED.
	// we need to know if that event was correct and skip the incorrect ones, so we 
	// synchronize that information with the Folder TreeView to see if they are the same.
	FOLDERNOTIFYEX lastFolderProp_;
	// indicates the number of FOLDER_PROPS_CHANGED events from last folder change.
	// it is used because sometimes (why?) in W98 the event is triggered only once
	int folderPropsCount_;

	// name of the selected folder. Used to store the name of the selected folder when the
	// FOLDER_PROPS_CHANGED didn't arrive.
	TCHAR lastFolderName_[256];

	LONG rootFolderId_;
	LONG inboxFolderId_;
	LONG sentFolderId_;
	LONG deletedFolderId_;
	LONG draftFolderId_;

	IStoreNamespace *pSN_;
	//IMessageList *msgList_;
	//IMessageListWMail *msgListWMail_;
	//IMessageTable *msgTable_;
	//IMessageTableWMail *msgTableWMail_;

//	BOOL folderSelPending_;

	BOOL hideSendWindow_;
	//BOOL lockSendWindow_;
	volatile LONG lockCountSendWindow_;

	// window used to send a message to send messages in Outbox inmediately.
	HWND hSendDlg_;
	//LPARAM SendRecvLparam_;

	// flag indicates that OE is synchronizing with the server.
	BOOL synchroInProcess_;
	// flag indicates that a synchronization was requested by API while OE was synchronizing.
	//BOOL synchroPending_;
	std::list<LPARAM> synchroPendingParams_;

	//BOOL folderEventFinished_;

	// flag indicates if the next message should not be sent immediately
	BOOL bLater_;

	// list of messages that should be created in Outbox folder to be sent.
	MessageInfoList sendMsgInfoList_;

	WNDPROC lpOldSendDlgWndProc_;

	LONG outboxFolderId_;
	IStoreFolder *outbox_;
#ifdef ENTERPRISE_VERSION
	//HWND hOutboxWnd_;
	LONG lastProcessedOutboxMsgId_;
#endif // ENTERPRISE_VERSION

	//HANDLE _afterIStoreNamespaceRelease;

	OEAPIMessageListPtr msgList_;
	OEAPIMessageStorePtr msgStore_;
};

#endif // OECOM_MAIN_H__

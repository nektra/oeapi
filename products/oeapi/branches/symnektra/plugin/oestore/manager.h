/* $Id: manager.h,v 1.13 2008/09/07 16:57:01 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef OESTORE_MAIN_H__
#define OESTORE_MAIN_H__

#include "msoeapi.h"

#include "oeapi_utils.h"

#include "../oecom/messagestore.h"

#define OEAPI_MSG_BASE 100


class OEMenuItemRepr;
class OEButtonRepr;
class OEStoreManager;


//typedef std::list<int> IntList;

typedef std::map<DWORD, DWORD> FolderIdMessageIdMap;

extern OEStoreManager *pThis;

#include "folder.h"

/**
This class handles all the active objects in the clients.
*/
//---------------------------------------------------------------------------//
class OEStoreManager {
public:
	~OEStoreManager();

	/**
	Get the only OEAPIManager instance
	*/
	static OEStoreManager *Get(bool createIfNotExist=true)
	{
		if(pThis == NULL && createIfNotExist) {
			pThis = new OEStoreManager;
		}

		return pThis;
	}

	/**
	Get the IStoreNamespace object
	*/
	IStoreNamespace *GetSN() { return pSN_; }

	OEAPIMessageStore *GetOEAPIStore() { return msgStore_; }

	/**
	Delete all the objects (if created) and create them again.
	This function should be used at startup and when the user switchs identity.
	*/
	void Restart();

	/**
	Call this function to release all objects and destroy all windows.
	*/
	void DestroyObjects();

	/**
	Get the window used to post messages
	This window is used to generate the COM events in the same thread that is running
	*/
	HWND GetCallbackWindow() { return hWnd_; }

	void ActivateGlobalNotification();

	/**
	Activate the global OnNewMessage event. This workaround was made to get all new
	messages. It triggers the event when a message changes its unread status to unread and
	its msg id is greater than the last msg id that is saved. This means that the message
	is new or (side effect) the message was copied before (and is not marked as unread)
	and now it changes its unread flag (false positive).
	*/
	static DWORD WINAPI ActivateGlobalNotificationThread(LPVOID lpParameter);

	/**
	Get the maximum message id of a specific folder.
	*/
	DWORD GetMaxMsgID(TOEFolderPtr folderPtr);

	/**
	Returns the msg id map.
	*/
	FolderIdMessageIdMap &GetMaxMsgIDsMap() { return maxMsgIds_; }

	/**
	Triggered when a message arrives to the hooked folder
	*/
//	void OnHookedFolderEvent(DWORD uMsg, LPARAM lParam, WPARAM wParam);

	/**
	This function is called when a UNREAD_CHANGE event is triggered. It is used to trigger
	the global OEFolderManager.OnNewMessage event.
	*/
	void OnFolderUnreadChanged(DWORD folderId);

	/**
	Add a FolderManage object
	*/
	void AddFolder(TOEFolder *folder);

	/**
	Remove a FolderManage object
	*/
	void RemoveFolder(TOEFolder *folder);

	/**
	Add a FolderManage object
	*/
	void AddFolderManager(TOEFolderManager *folderMan);

	/**
	Register notification
	*/
//	void HookFolder(DWORD folderId);

//	void UnhookFolder();

	/**
	Remove a FolderManage object
	*/
	void RemoveFolderManager(TOEFolderManager *folderMan);

	/**
	Raise the event OnFolderManagerEvent in all OEFolderManager objects
	*/
	void RaiseOnFolderManagerEvent(LPFOLDERNOTIFYEX folderNotify);

	/**
	Raise the event OnNewMessage in all OEFolderManager objects
	*/
	void RaiseOnNewMessageEvent(DWORD folderId, DWORD newMsgId);

	/**
	*/
	BOOL OpenFolder(INT folderId, INT specialfolderId, IStoreFolder* pSF, TOEFolder* folderInfo);

	/**
	*/
	BOOL OpenStorageFolder(INT folderId, IMessageFolder** folder);

protected:
	OEStoreManager();

private:
	// used to lock lists. The ActivateGlobalNotification is in another thread so we have
	// to lock.
	CRITICAL_SECTION folderCS_;

	FolderManagerList folderManagers_;
	FolderList folders_;
	HWND hWnd_;
	IStoreNamespace *pSN_;
	DWORD curFolderId_;

	IStoreFolder *pDeletedFolder_;
	HWND hDeletedWnd_;

//	IStoreFolderPtr hookedFolder_;
//	DWORD hookedFolderId_;

	// last message id of each folder. 
	// When OE creates a new message in a folder, it uses a number greater that the last
	// greatest msg id. So, if we remember the last msg id of each folder, we can know
	// when a new message arrives using the OnFolderUnreadChanged
	FolderIdMessageIdMap maxMsgIds_;
	HANDLE hThread_;
	// thread id of the ActivateGlobalNotification
	DWORD dwThreadId_;

	OEAPIMessageStore* msgStore_;
};


#endif // OESTORE_MAIN_H__

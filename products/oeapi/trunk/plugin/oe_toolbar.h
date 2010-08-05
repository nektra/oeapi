/* $Id: oe_toolbar.h,v 1.35 2008/09/07 16:56:46 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef __OE_TOOLBAR_H
#define __OE_TOOLBAR_H

#pragma warning(disable:4786)

#include <windows.h>
#include <commctrl.h>
#include <list>
#include <tchar.h>
#include <string>

#include "oeapi.h"

// 'Send' button id that can be found in the Create Mail / Reply / Reply All / Forward msg wnds 
#define OE_SEND_TOOLBAR_BTN 0x9ddb
// Send Later
#define OE_SENDLATER_MENUITEM_BTN 0x9d21


#ifndef RBBS_USECHEVRON

typedef struct tagNMREBARCHEVRON {
    NMHDR hdr;
    UINT uBand;
    UINT wID;
    LPARAM lParam;
    RECT rc;
    LPARAM lParamNM;
} NMREBARCHEVRON, *LPNMREBARCHEVRON;

#define RBBS_USECHEVRON     0x00000200
#define RBHT_CHEVRON        0x0008
#define RB_PUSHCHEVRON      ( WM_USER + 43 )
#define RBN_CHEVRONPUSHED   ( RBN_FIRST - 10 )

#endif // RBBS_USECHEVRON


class OEPluginToolbar;
class OEPluginToolbarBtn;
class OEPluginToolbarMgr;
class OEPluginTopLevelWndMgr;

typedef std::list<OEPluginTopLevelWndMgr*> OEPluginTopLevelWndMgrList;
typedef std::list<OEPluginToolbar*> OEPluginToolbarList;
typedef std::list<OEPluginToolbarBtn*> OEPluginToolbarBtnList;


extern OEPluginToolbarMgr *ToolbarMgr;


/**
This class contains all the created toolbars in all the plugins
*/
//---------------------------------------------------------------------------------//
class OEPluginToolbarMgr {
public:
	~OEPluginToolbarMgr();

	/**
	Return the OEPluginToolbarMgr singleton
	*/
	static OEPluginToolbarMgr *Get()
	{
		if(ToolbarMgr == NULL) {
			ToolbarMgr = new OEPluginToolbarMgr;
		}

		return ToolbarMgr;
	}

	/**
	Get the mgr of the OE main window
	*/
	OEPluginTopLevelWndMgr *GetMainWndMgr();

	/**
	Get the mgr of the message windows.
	This mgr shouldn't be initialized, it is only used to store toolbar info. 
	To use in a window you should create a copy of it.
	*/
	OEPluginTopLevelWndMgr *GetMsgWndMgr() { return msgWndMgr_; }

	/**
	Get a toolbar using the internal OEAPI id
	The toolbar is returned locked if lock is TRUE.
	*/
	OEPluginToolbar *GetToolbar(INT id, BOOL lock = TRUE);

	/**
	Get a toolbar using the handle of the window
	The toolbar is returned locked if lock is TRUE.
	*/
	OEPluginToolbar *GetToolbar(HWND handle, BOOL lock = TRUE);

	/**
	Get toolbar by index
	If oeapiId == -1 ignores that parameter.
	This function return first the toolbars in the main window and then the toolbars
	defined in the message windows.
	The toolbar is returned locked if lock is TRUE.
	*/
	OEPluginToolbar *GetToolbarByIndex(INT index, INT oeapiId=-1, BOOL lock = TRUE);

	/**
	Destroy all the toolbars created in oeapiId
	If oeapiId == -1 ignores that parameter
	*/
	VOID DestroyAllToolbars(INT oeapiId=-1);

	/**
	Destroy OEPluginTopLevelWndMgr of OE main window.
	This function is called when OE main window is closed.
	*/
	VOID DestroyOEMainWindowTopLevelWndMgr();

	/**
	Get the button using the id.
	The button is returned locked if lock is TRUE.
	*/
	OEPluginToolbarBtn *GetButton(INT id, BOOL lock = TRUE);

	/**
	Get the button instance using the id. It returns a instance of a button created in 
	a toolbar. The above function returns a button representation for the buttons in 
	the message windows, not the real instance created in a specific toolbar.
	The button is returned locked if lock is TRUE.
	*/
	OEPluginToolbarBtn *GetButtonInstance(HWND hToolbar, INT id, BOOL lock = TRUE);

	/**
	Lock the manager and its components.
	Do not let it die.
	*/
	static VOID Lock();

	/**
	Unlock the manager and its components.
	*/
	static VOID Unlock();

	/**
	Lock the list of top level window.
	*/
	static VOID LockTopLevelList();

	/**
	Unlock the list of top level window.
	*/
	static VOID UnlockTopLevelList();

	/**
	*/
	VOID RemoveToolbarProcs();

private:
	OEPluginToolbarMgr();

private:
	// mgr of the OE main window
	OEPluginTopLevelWndMgr *mwWndMgr_;

	// mgr of the message windows
	// this manager is used to be cloned when a new message window is created
	// it has all the toolbar info
	OEPluginTopLevelWndMgr *msgWndMgr_;

};

/**
This class contains all the toolbar information of a top level window and handle the
necessary messages of them.
*/
//---------------------------------------------------------------------------------//
class OEPluginTopLevelWndMgr {
public:
	typedef MsgWndStyles::WNDSTYLE WNDSTYLE;

	OEPluginTopLevelWndMgr();
	OEPluginTopLevelWndMgr(OEPluginTopLevelWndMgr *mgr);
	~OEPluginTopLevelWndMgr();

	/**
	Return TRUE if the function RebarCreated was called.
	*/
	BOOL Initialized() { return initialized_; }

	/**
	Called after the OE creates (idealy) the SizableRebar and the Rebar (if not it fails).
	In this function the toolbars created before this point are created in the rebar of the 
	OE.
	mainWindow is the OE main window or the message main window.
	if mainWindow is the OE main window isMainWindow must be TRUE.
	*/
	BOOL Initialize(HWND mainWindow, BOOL isMainWindow=FALSE);

	/**
	Look for rebar and OE toolbar handles and returns OE toolbar handle
	*/
	HWND FindWindows(HWND mainWindow);

	/**
	Get the sizable rebar handle
	*/
	HWND GetSizableRebarHandle() { return hSizableRebar_; }

	/**
	Get the rebar handle
	*/
	HWND GetRebarHandle() { return hRebar_; }

	/**
	Send the message.
	Only valid when the wndStyle is different from OE_MSG_DETAIL_WND or OE_MSG_INVALID.
	*/
	VOID SendMail(BOOL bLater = FALSE);

	/**
	Send a key to the top level window.
	*/
	VOID SendKey(BYTE key, BYTE controlKey, BOOL setForeground);

	/**
	Set the window that will receive the keys sent using SendKey.
	*/
	VOID SetKeyMsgWnd(HWND hwnd) { hKeyReceiver_ = hwnd; }

	/**
	Get the sizable rebar old proc
	*/
	WNDPROC GetSizableRebarOldProc() { return oldSizableRebarProc_; }

	/**
	Get the sizable rebar old proc
	*/
	WNDPROC GetRebarOldProc() { return oldRebarProc_; }

	/**
	Get the type of top level wnd.
	*/
	WNDSTYLE GetWndStyle() { return wndStyle_; }

	/**
	Create all the specified toolbars.
	Return FALSE if the rebar or sizable rebar windows doesn't exist.
	*/
	BOOL CreateToolbars();

	/**
	*/
	BOOL ResetToolbars();

	/**
	Add a toolbar reference.
	*/
	VOID AddToolbar(OEPluginToolbar *toolbar);

	/**
	Remove a toolbar reference.
	*/
	VOID RemoveToolbar(OEPluginToolbar *toolbar);

	/**
	Get a toolbar using the internal OEAPI id.
	The toolbar is returned locked if lock is TRUE.
	*/
	OEPluginToolbar *GetToolbar(INT id, BOOL lock = TRUE);

	/**
	Get a toolbar using the handle of the window.
	The toolbar is returned locked if lock is TRUE.
	*/
	OEPluginToolbar *GetToolbar(HWND handle, BOOL lock = TRUE);

	/**
	Get the toolbar that is in the specified band index.
	The toolbar is returned locked if lock is TRUE.
	*/
	OEPluginToolbar *GetToolbarByBandIndex(INT index, BOOL lock = TRUE);

	/**
	Get the number of toolbars of the specified oeapiId.
	If oeapiId == -1 return the number of toolbars.
	*/
	INT GetToolbarCount(INT oeapiId=-1);

	/**
	Get toolbar by index.
	If oeapiId == -1 ignores that parameter.
	The toolbar is returned locked if lock is TRUE.
	*/
	OEPluginToolbar *GetToolbarByIndex(INT index, INT oeapiId=-1, BOOL lock = TRUE);

	/**
	Destroy all the toolbars created in oeapiId
	If oeapiId == -1 ignores that parameter
	*/
	VOID DestroyAllToolbars(INT oeapiId=-1);

	/**
	Get the button using the id
	The button is returned locked if lock is TRUE.
	*/
	OEPluginToolbarBtn *GetButton(INT id, BOOL lock = TRUE);

	/**
	Get the OE main window or NULL if it wasn't created yet.
	*/
	HWND GetMainWindow() { return mainWindow_; }

	/**
	Called when any toolbar change its band to set the correct index.
	hWnd must have the sizable rebar.
	*/
	//VOID UpdateToolbarsBandIndex();

	/**
	Get the old proc of the OE toolbar.
	*/
	WNDPROC GetOEToolbarOldProc() { return oldToolbarProc_; }

	/**
	Get the handle of OE toolbar.
	*/
	HWND GetOEToolbarHandle() { return hOEToolbar_; }

private:
	OEPluginToolbarList toolbars_;
	HWND mainWindow_;
	HWND hSizableRebar_;
	HWND hOEToolbar_;

	// window that receives the keys sent using SendKey
	HWND hKeyReceiver_;

	WNDPROC oldSizableRebarProc_;
	BOOL initialized_;
	WNDSTYLE wndStyle_;

	WNDPROC oldToolbarProc_;

	HWND hRebar_;
	WNDPROC oldRebarProc_;
};

/**
This class implements a toolbar in the OE.
*/
//---------------------------------------------------------------------------------//
class OEPluginToolbar {
	friend class OEPluginToolbarMgr;

public:

	OEPluginToolbar(OEPluginTopLevelWndMgr *mgr);
	OEPluginToolbar(OEPluginTopLevelWndMgr *mgr, OEPluginToolbar *toolbar);
	~OEPluginToolbar();

	/**
	Create the toolbar in the OE main window
	createAlways indicates if this function creates again the toolbar (or/and the buttons)
	even if the toolbar was created before.
	*/
	VOID Create(INT oeapiId=-1, BOOL createAlways=FALSE);

	/**
	Create the toolbar in the specified (main) window
	*/
	VOID CreateInWnd(HWND mainWindow);

	/**
	*/
	VOID Reset();

	/**
	Get the internal OEAPI id of the toolbar
	*/
	INT GetID() { return id_; }

	/**
	Get the window handle
	*/
	HWND GetHandle() { return handle_; }

	/**
	Set the window handle
	*/
	VOID SetHandle(HWND handle) { handle_ = handle; }

	/**
	Get the toolbar manager where this object lives.
	*/
	OEPluginTopLevelWndMgr *GetManager() { return topLevelMgr_; }

	/**
	Get the OEAPI owner of this toolbar
	*/
	INT GetOEAPIID() { return oeapiId_; }

	/**
	Hide or Show the toolbar
	*/
	VOID SetShown(BOOL show);

	/**
	Test the hidden property
	*/
	BOOL IsHidden() { return hidden_; }

	/**
	Add the button to the toolbar
	*/
	VOID AddButton(OEPluginToolbarBtn *button);

	/**
	Remove button to the toolbar
	*/
	VOID RemoveButton(OEPluginToolbarBtn *button);

	/**
	Get the button using the id.
	The button is returned locked if lock is TRUE.
	*/
	OEPluginToolbarBtn *GetButton(INT id, BOOL lock = TRUE);

	/**
	Get the button by index.
	The button is returned locked if lock is TRUE.
	*/
	OEPluginToolbarBtn *GetButtonByIndex(INT index, BOOL lock = TRUE);

	/**
	Return the number of visible buttons.
	*/
	int GetVisibleButtonsCount();

	/**
	Return the band index where the toolbar is placed or -1 it wasn't created yet.
	*/
	INT GetBandIndex() { 
		return (INT)::SendMessage(rebar_, RB_IDTOINDEX, (WPARAM)id_, 0); 
	}

	/**
	Change the band index. This function must be called ONLY if the toolbar is changed from
	the original where it was placed.
	*/
	//VOID SetBandIndex(INT index) { bandIndex_ = index; }

	/**
	Change the style of the toolbar
	*/
	VOID SetLargeButtons(BOOL large);

	/**
	Set max rows of text
	*/
	VOID SetMaxTextRows(int rows);

	/**
	Enable tooltips (only work at creation time)
	*/
	VOID EnableTooltips(BOOL enable);

	/**
	Get tooltip handle
	*/
	HWND GetTooltipsHandle();

	/**
	Only for toolbars in msg wnds. See 	typedef WNDSTYLE.
	*/
	VOID SetWndStyle(DWORD wndStyle) { wndStyle_ = wndStyle; }

	/**
	Get the wndStyle property.
	*/
	DWORD GetWndStyle() { return wndStyle_; }

	/**
	Set transparent color for buttons bitmaps
	*/
	void SetMaskColor(COLORREF color) { maskColor_ = color; }

	/**
	Get transparent color for button bitmaps
	*/
	COLORREF GetMaskColor() { return maskColor_; }

	/**
	Set if this toolbar is the default OE toolbar.
	*/
	VOID SetOEDefaultToolbar(BOOL defToolbar) { defOEToolbar_ = defToolbar; }

	/**
	Get if this toolbar is the default OE toolbar.
	*/
	BOOL IsOEDefaultToolbar() { return defOEToolbar_; }

	/**
	Returns TRUE if the toolbar has any button with sub button.
	*/
	BOOL HasSubButtons();

	/**
	Called when the chevron button is pressed to show the popup menu that contains 
	the buttons that are not shown in the toolbar
	*/
	VOID ShowChevron();

	/**
	This function is called after a new button is added to re-calculate the size of the band.
	*/
	VOID UpdateBandSize(OEPluginToolbarBtn *btn=NULL);

	/**
	Lock the toolbar.
	Do not let it die.
	*/
	VOID Lock();

	/**
	Unlock the toolbar.
	*/
	VOID Unlock();

	//////////////////////////////////////////////////////////////////////		
	//

	/**
	Load a bitmap from file to load in this toolbar (internal use)
	*/
	HBITMAP LoadButtonImage(LPCTSTR filename);

	/**
	Add an image to normal ImageList (internal use)
	*/
	INT AddImage(HBITMAP hbmp);

	/**
	Add an image to hot ImageList (internal use)
	*/
	INT AddHotImage(HBITMAP hbmp);

	/**
	Replace normal image (internal use)
	*/
	INT ReplaceImage(INT index, HBITMAP hbmp, HBITMAP hmask);

	/**
	Replace hot image (internal use)
	*/
	INT ReplaceHotImage(INT index, HBITMAP hbmp, HBITMAP hmask);

	/**
	Create a normal image list (internal use)
	*/
	HIMAGELIST CreateImageList();

	BOOL SetBitmapSize(INT cx, INT cy);
	BOOL GetBitmapSize(INT* cx, INT* cy);

protected:
	/**
	Return the toolbar style made from the properties of the object
	*/
	DWORD GetStyle();

	/**
	Create the chevron with buttons that are not displayed.
	*/
	VOID CreateChevron();

	/**
	This function shows all the buttons.
	*/
	VOID ShowAllButtons();

	/**
	Return the band index of the toolbar.
	This function get the child of each band and compares with handle_, so it can be used
	when bandIndex_ is not set.
	*/
	INT CalculateBandIndex();

private:
	INT id_;
	INT oeapiId_;
	//INT bandIndex_; // the index in the Rebar window, the band index.
	HWND handle_;
	HMENU popupMenu_;
	HWND rebar_;
	BOOL hidden_;
	BOOL largeBtns_;
	BOOL defOEToolbar_;
	BOOL tooltips_;
	INT defOEToolbarBtnCount_;
	OEPluginToolbarBtnList buttons_;
	OEPluginTopLevelWndMgr *topLevelMgr_;
	DWORD wndStyle_;

	INT cx_; // width of the toolbar pictures
	INT cy_; // height of the toolbar pictures

	// string used to workaround the bug that occurs when a sub button is added to a button
	// and the button is already created -> size of the button is not updated so we have to
	// create a new button (dummy) that updates the size of the old button and delete it.
	//INT dummyStrIndex_;

	// used to prevent object die
	CRITICAL_SECTION critSection_;
	INT rows_;

	COLORREF maskColor_;
};

/**
This class implements a button in a toolbar in the OE.
*/
//---------------------------------------------------------------------------------//
class OEPluginToolbarBtn {
	friend class OEPluginToolbar;
public:
	OEPluginToolbarBtn();
	OEPluginToolbarBtn(OEPluginToolbarBtn *btn);
	~OEPluginToolbarBtn();

	enum ImgSourceType {
		img_invalid = -1,
		img_file = 0,			// the image is loaded from a file
		img_resource = 1,		// the image is in a resource
		img_bitmap = 2
	};

	enum BtnProperties {
		btn_image_normal = 1,
		btn_image_mouse_over = 2,
		btn_name = 4
	};

	/**
	Get the id
	*/
	INT GetID() { return id_; }

	/**
	Get the handle of the toolbar where is placed
	*/
	HWND GetToolbarHandle() { return toolbar_->GetHandle(); }

	/**
	Get the toolbar where is placed
	*/
	OEPluginToolbar *GetToolbar() { return toolbar_; }

	/**
	Set the enabled state of the button
	*/
	VOID SetEnabled(BOOL enabled);

	/**
	Return true if the button is enabled
	*/
	BOOL IsEnabled() { return enabled_; }

	/**
	Hide or Show the button
	*/
	VOID SetShown(BOOL show, BOOL update = TRUE);

	/**
	Return true if the button is hidden
	*/
	BOOL IsHidden() { return hidden_; }

	/**
	Set that the button is a sub button of id button
	*/
	VOID SetParentButtonID(INT id) { parentButId_ = id; }

	/**
	Set the handle of the toolbar where is placed
	*/
	VOID SetToolbar(OEPluginToolbar *toolbar);

	/**
	Set the toolbar id where the button is located
	*/
	VOID SetToolbarID(INT id) { toolbarId_ = id; }

	/**
	Get the toolbar id where the button is located
	*/
	INT GetToolbarID() { return toolbarId_; }

	/**
	Set the OEAPI id where the button is located
	*/
	VOID SetOEAPIID(INT id) { oeapiId_ = id; }

	/**
	Get the OEAPI id where the button is located
	*/
	INT GetOEAPIID() { return oeapiId_; }

	/**
	Set the type of image source
	*/
	VOID SetType(ImgSourceType type) { type_ = type; }

	/**
	Set the name of the button.
	*/
	VOID SetName(LPCTSTR name);

	/**
	*/
	LPCTSTR GetName();

	/**
	Set tooltip of the button
	*/
	VOID SetTooltip(LPCTSTR tooltip);

	/**
	Set the bmp used when the mouse is not over the button.
	If the mouse over bmp is not set, this bmp is used when the mouse is over too.
	*/
	VOID SetBMPNormal(LPCTSTR bmpNormal);

	/**
	Set the bmp used when the mouse is over the button.
	*/
	VOID SetBMPMouseOver(LPCTSTR bmpMouseOver);

	/**
	Set the normal bitmap 
	*/
	VOID SetPictureNormal(interface IPicture* pict);

	/**
	Set the mouse over bitmap 
	*/
	VOID SetPictureOver(interface IPicture* pict);

	/**
	Set the function called when the button is pressed.
	*/
	VOID SetCallback(OEAPI_ButtonCallBack cb) { callback_ = cb; }

	/**
	Get the function called when the button is pressed.
	*/
	OEAPI_ButtonCallBack GetCallback() { return callback_; }

	/**
	Add a sub button to this button
	*/
	VOID AddButton(OEPluginToolbarBtn *button);

	/**
	Remove a sub button to this button
	*/
	VOID RemoveButton(OEPluginToolbarBtn *button);

	/**
	Create the sub buttons.
	This function is called when the arrow is pressed in the buttons that have sub buttons.
	*/
	VOID CreateSubButtons();

	/**
	Returns the number of sub buttons of this button.
	*/
	INT GetSubButtonCount() { return (INT)subButtons_.size(); }

	/**
	Get the sub button of the button.
	The button is returned locked if lock is TRUE.
	*/
	OEPluginToolbarBtn *GetSubButton(int id, BOOL lock = TRUE);

	/**
	Get the sub button by index.
	The button is returned locked if lock is TRUE.
	*/
	OEPluginToolbarBtn *GetSubButtonByIndex(INT index, BOOL lock = TRUE);

	/**
	Get current state flags to set it in the toolbar.
	*/
	DWORD GetState();

	/**
	Get current style flags to set it in the toolbar.
	*/
	DWORD GetStyle();

	/**
	Set if the button is a separator.
	*/
	void SetSeparator(BOOL sep) { isSeparator_ = sep; }

	/**
	Returns TRUE if it is a separator.
	*/
	BOOL IsSeparator() { return isSeparator_; }

	/**
	Set if the button has the popup style.
	Only for buttons with sub buttons: if the button has popup style when is pressed
	it shows the sub buttons instead of generate a OnClick event.
	*/
	void SetPopupStyle(BOOL popup) { popupStyle_ = popup; }

	/**
	Returns TRUE if it has popup style.
	*/
	BOOL IsPopupStyle() { return popupStyle_; }

	/**
	Set button with check style
	*/
	void SetCheckStyle(BOOL check) { checkStyle_ = check; }

	/**
	Return true if button has check style
	*/
	BOOL IsCheckStyle() { return checkStyle_; }

	/**
	Set button as checked if check is true
	*/
	void SetChecked(BOOL check, BOOL update = TRUE);

	/**
	Return true if the button is checked
	*/
	BOOL IsChecked(); // { return checked_; } 
	
	/**
	Lock the button.
	Do not let it die.
	*/
	VOID Lock();

	/**
	Unlock the button.
	*/
	VOID Unlock();

	/**
	Notify menu item click, return TRUE if processed
	*/
	BOOL NotifyButtonClick(HWND msgWndId, INT id);

	/**
	Notify menu item click, return TRUE if processed
	*/
	BOOL NotifyMsgButtonClick(HWND msgWndId, INT id);

	/**
	+++
	*/
	VOID Remove();

protected:
	/**
	Create the button in the toolbar. After calling this function the Toolbar must update
	the button positions.
	*/
	VOID Create(OEPluginToolbar *toolbar);

	/**
	Create the button in a popup menu.
	*/
	VOID CreateInMenu(HMENU menu);

	/**
	*/
	VOID Reset();

	/**
	Enable or Disable the button in order to match the property value.
	Hide or Show the button in order to match the property value.
	*/
	VOID UpdateState();

	/**
	Update the properties of the button.
	This function should be called after changing any button property after the button
	was created.
	props contains the properties that must be updated.
	*/
	VOID UpdateButtonInfo(int props);

	/**
	Load a bitmaps for popup menu (internal use)
	*/
	VOID LoadMenuBitmaps();

	/**
	Set tooltip text
	*/
	//VOID SetToolTipText(HWND hToolTip, HWND toolbar, INT id, LPCTSTR text);

private:
	OEPluginToolbar *toolbar_;
	INT toolbarId_;
	ImgSourceType type_;
	WORD normalImg_;
	WORD mouseOverImg_;
	std::basic_string<TCHAR> name_;
	std::basic_string<TCHAR> tooltip_;
	std::basic_string<TCHAR> bmpMouseOver_;
	std::basic_string<TCHAR> bmpNormal_;
	OEAPI_ButtonCallBack callback_;
	IPicture* pictNormal_;
	IPicture* pictOver_;
	HBITMAP hbmpChecked_;
	HBITMAP hbmpUnchecked_;
	INT id_;
	INT oeapiId_;
	BOOL created_;
	BOOL hidden_;
	BOOL enabled_;
	INT parentButId_;
	INT bmpIndexNormal_;
	OEPluginToolbarBtnList subButtons_;
	HMENU popupMenu_;
	BOOL isSeparator_;
	BOOL popupStyle_;

	BOOL checkStyle_;
	BOOL checked_;

	// used to prevent object die
	CRITICAL_SECTION critSection_;
};

#endif // __OE_TOOLBAR_H
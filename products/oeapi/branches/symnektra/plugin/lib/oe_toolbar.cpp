/* $Id: oe_toolbar.cpp,v 1.60 2008/09/07 16:56:45 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>

#include "oe_toolbar.h"
#include "se_plugin.h"
#include "se_debug.h"
#include "oeapi_utils.h"

#if !defined(TB_SETWINDOWTHEME)
#define TB_SETWINDOWTHEME 0x200b
#endif

#define TOOLBAR_ID_START	30100

OEPluginToolbarMgr *ToolbarMgr = NULL;
INT ButtonId = 60100;
INT ToolbarId = TOOLBAR_ID_START;

// used for all the toolbar functionality
CRITICAL_SECTION critSection;
BOOL critSectionInitialized = FALSE;

CRITICAL_SECTION topLevelCritSection;

OEPluginTopLevelWndMgrList TopLevelWndMgrList;


//////////////////////////////////////////////////////////////////////
// Copied from: http://www.winprog.org/tutorial/transparency.html
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	if(hbmColour == NULL)	{
		debug_print(DEBUG_ERROR, _T("CreateBitmapMask: Null bitmap\n"));
		return NULL;
	}
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

	//DIBSECTION dib;
	//INT res = ::GetObject(hbmColour, sizeof(DIBSECTION), &dib);

    // Create monochrome (1 bit) mask bitmap.  
    ::GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = ::CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver
    hdcMem = ::CreateCompatibleDC(0);
    hdcMem2 = ::CreateCompatibleDC(0);

	::SelectObject(hdcMem, (HGDIOBJ)(HBITMAP)(hbmColour));
	::SelectObject(hdcMem2, (HGDIOBJ)(HBITMAP)(hbmMask));

    // Set the background color of the color image to the color
    // you want to be transparent.
	if(crTransparent == CLR_DEFAULT) {
		COLORREF clr = ::GetPixel(hdcMem, 0, 0);
		::SetBkColor(hdcMem, clr);
	} 
	else {
		::SetBkColor(hdcMem, crTransparent);
	}		

    // Copy the bits from the color image to the B+W mask... everything
    // with the background color ends up white while everything else ends up
    // black...Just what we wanted.
    ::BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    ::BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.
    ::DeleteDC(hdcMem);
    ::DeleteDC(hdcMem2);

    return hbmMask;
}


//////////////////////////////////////////////////////////////////////
HBITMAP CreateMenuBitmap(HBITMAP hbmIn, INT cx, INT cy) 
{
	HWND hwndDesktop = ::GetDesktopWindow(); 
	HDC hdcDesktop = ::GetDC(hwndDesktop); 
	HDC hdcMem = ::CreateCompatibleDC(hdcDesktop); 
	HDC hdcMem2 = ::CreateCompatibleDC(hdcDesktop); 
	HBRUSH hbrOld; 
	HBITMAP hbmOld; 
	HBITMAP hbmOld2; 

	COLORREF clrMenu = ::GetSysColor(COLOR_MENU); 
	hbrOld = (HBRUSH)::SelectObject(hdcMem, CreateSolidBrush(clrMenu)); 
	HBITMAP hbmOut = ::CreateCompatibleBitmap(hdcDesktop, cx, cy);
	hbmOld = (HBITMAP)::SelectObject(hdcMem, hbmOut); 
	//::PatBlt(hdcMem, 0, 0, cx, cy, PATCOPY); 
	HBITMAP hMask = ::CreateBitmapMask(hbmIn, 0xff000000);

	hbmOld2 = (HBITMAP)::SelectObject(hdcMem2, hbmIn);
	::MaskBlt(hdcMem, 0, 0, cx, cy, hdcMem2, 0, 0, hMask, 0, 0, MAKEROP4(PATCOPY, SRCCOPY));

	::DeleteObject(SelectObject(hdcMem, hbrOld));
	::SelectObject(hdcMem2, hbmOld2);

	::DeleteObject(hMask);

	::SelectObject(hdcMem, hbmOld);
	::DeleteDC(hdcMem);
	::DeleteDC(hdcMem2);
	::ReleaseDC(hwndDesktop, hdcDesktop);

	return hbmOut;
}

//////////////////////////////////////////////////////////////////////
//HBITMAP CopyBitmap(HBITMAP hbm) {
//	if(hbm == NULL)	{
//		debug_print(DEBUG_ERROR, _T("CopyBitmap: Null bitmap\n"));
//		return NULL;
//	}
//    HDC src = CreateCompatibleDC(NULL);
//    HDC dest = CreateCompatibleDC(NULL);
//    HBITMAP hbmNew;
//    BITMAP bm;
//
//    ::GetObject(hbm, sizeof(bm), &bm);
//    (HBITMAP)::SelectObject(src, hbm);
//    hbmNew = ::CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL);
//    (HBITMAP)::SelectObject(dest, hbmNew);
//    ::BitBlt(dest, 0, 0, bm.bmWidth, bm.bmHeight, src, 0, 0, SRCCOPY);
//    DeleteDC(src);
//    DeleteDC(dest);
//
//    return hbmNew;
//}

/////////////////////////////////////////////////////////////////////////////////////
HBITMAP CreateHBitmapFromIPicture(IPicture* pict, LONG xsize, LONG ysize)
{
	OLE_YSIZE_HIMETRIC h;
	OLE_XSIZE_HIMETRIC w;

	HRESULT hr;
	hr = pict->get_Height(&h);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("CreateHBitmapFromIPicture: Can't get IPicture height %08x\n"), hr);
		return NULL;
	}
	hr = pict->get_Width(&w);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("CreateHBitmapFromIPicture: Can't get IPicture width %08x\n"), hr);
		return NULL;
	}

	HDC hMemDC = ::CreateCompatibleDC(0);
	if(hMemDC == NULL) {
		debug_print(DEBUG_ERROR, _T("CreateHBitmapFromIPicture: Can't create device context\n"));
		return NULL;
	}

	if(xsize == -1 || ysize == -1) {
		// Use pict size
		xsize = MulDiv(w, GetDeviceCaps(hMemDC, LOGPIXELSX ), 2540 );
		ysize = MulDiv(h, GetDeviceCaps(hMemDC, LOGPIXELSY ), 2540 );
	}

	HWND hwndDesktop = GetDesktopWindow(); 
	HDC hdcDesktop = GetDC(hwndDesktop); 

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = xsize;
	bmi.bmiHeader.biHeight = ysize;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	LPVOID bits;
	HBITMAP hbmp = ::CreateDIBSection(hdcDesktop, &bmi, DIB_RGB_COLORS, &bits, NULL, NULL);
	::ReleaseDC(hwndDesktop, hdcDesktop);

	//HBITMAP hbmp = ::CreateCompatibleBitmap(hDesktop, xsize, ysize);

	if(hbmp == NULL) {
		debug_print(DEBUG_ERROR, _T("CreateHBitmapFromIPicture: Can't create bitmap\n"));
		::DeleteDC(hMemDC);
		return NULL;
	}
	HBITMAP hbmOld = (HBITMAP)::SelectObject(hMemDC, hbmp);
	hr = pict->Render(hMemDC, 0, 0, xsize, ysize, 0, h, w, -h, NULL);
	::SelectObject(hMemDC, hbmOld);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("CreateHBitmapFromIPicture: Can't render bitmap\n"));
		::DeleteObject(hbmp);
		::DeleteDC(hMemDC);
		return NULL;
	}
	::DeleteDC(hMemDC);

	return hbmp;
}

/////////////////////////////////////////////////////////////////////////////////////
HBITMAP LoadImageFromFile(LPCTSTR filename, UINT flags, LONG cx, LONG cy) 
{
	HBITMAP hbmp;
	hbmp = (HBITMAP)::LoadImage(NULL, filename, IMAGE_BITMAP, cx, cy, flags);
	return hbmp;
}

/////////////////////////////////////////////////////////////////////////////////////
static LRESULT CALLBACK SizableRebarWndProc(	HWND hWnd, 
									UINT msg,
									WPARAM wParam, 
									LPARAM lParam)
{
	LPNMHDR lpnm;
	WNDPROC oldRebarProc = NULL;
	OEPluginTopLevelWndMgr *tlMgr = NULL;

	lpnm = (LPNMHDR) lParam;

	OEPluginToolbarMgr::LockTopLevelList();
	for(OEPluginTopLevelWndMgrList::iterator it=TopLevelWndMgrList.begin(); it!=TopLevelWndMgrList.end(); it++) {
		OEPluginTopLevelWndMgr *mgr = *it;

		if(mgr->GetSizableRebarHandle() == hWnd) {
			tlMgr = mgr;
			oldRebarProc = mgr->GetSizableRebarOldProc();
			break;
		}
	}

	if(tlMgr == NULL) {
		debug_print(DEBUG_ERROR, _T("SizableRebarWndProc: Fatal Error: cannot get toolbar mgr\n"));
		OEPluginToolbarMgr::UnlockTopLevelList();
		return FALSE;
	}

	switch(msg)
	{
		case WM_NOTIFY:
		{
			switch(lpnm->code)
			{
				case RBN_CHEVRONPUSHED:
				{
					LPNMREBARCHEVRON lpnm;

					lpnm = (LPNMREBARCHEVRON) lParam;

					// it isn't necessary to lock the object while inside the window proc.
					OEPluginToolbar *toolbar = tlMgr->GetToolbarByBandIndex(lpnm->uBand, FALSE);
					if(toolbar) {
						toolbar->ShowChevron();
						OEPluginToolbarMgr::UnlockTopLevelList();
						return 0;
					}
					break;
				}

				case RBN_ENDDRAG:
				{
					// Not needed anymore
					//tlMgr->UpdateToolbarsBandIndex();
					break;
				}
			}

			break;
		} 
	}

	OEPluginToolbarMgr::UnlockTopLevelList();

	return CallWindowProc(oldRebarProc, hWnd, msg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
static LRESULT CALLBACK RebarWndProc(HWND hWnd, 
									UINT msg,
									WPARAM wParam, 
									LPARAM lParam)
{
	LPNMHDR lpnm;
	WNDPROC oldRebarProc = NULL;
	OEPluginTopLevelWndMgr *tlMgr = NULL;
	BOOL ret = FALSE;

	lpnm = (LPNMHDR) lParam;

	OEPluginToolbarMgr::LockTopLevelList();
	for(OEPluginTopLevelWndMgrList::iterator it=TopLevelWndMgrList.begin(); it!=TopLevelWndMgrList.end(); it++) {
		OEPluginTopLevelWndMgr *mgr = *it;

		if(mgr->GetRebarHandle() == hWnd) {
			tlMgr = mgr;
			oldRebarProc = mgr->GetRebarOldProc();
			break;
		}
	}

	if(tlMgr == NULL) {
		debug_print(DEBUG_ERROR, _T("RebarWndProc: Fatal Error: cannot get toolbar mgr\n"));
		OEPluginToolbarMgr::UnlockTopLevelList();
		return FALSE;
	}

	switch(msg)
	{
		case WM_NOTIFY:
		{
			LPNMHDR lpnm = (LPNMHDR) lParam;

			if(lpnm->code == TBN_DROPDOWN) {
				//debug_print(DEBUG_INFO, _T("RebarWndProc: TBN_DROPDOWN\n"));
				LPNMTOOLBAR pnmtb = (LPNMTOOLBAR) lParam;

				OEPluginToolbarBtn *btn = OEPluginToolbarMgr::Get()->GetButtonInstance(lpnm->hwndFrom, pnmtb->iItem, FALSE);
				if(btn) {
					btn->CreateSubButtons();
					ret = TRUE;
				}
			}
			break;
		}
	}

	OEPluginToolbarMgr::UnlockTopLevelList();

	if(ret)	{
		// Notify that the message was processed, fix drop down bug in OE default toolbar
		return FALSE;
	}

	return CallWindowProc(oldRebarProc, hWnd, msg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ToolbarWndProc(HWND hWnd, 
								 UINT msg,
								 WPARAM wParam, 
								 LPARAM lParam)
{
	WNDPROC lpOldProc;
	OEPluginTopLevelWndMgr *mgr = NULL;

	OEPluginToolbarMgr::LockTopLevelList();
	for(OEPluginTopLevelWndMgrList::iterator it=TopLevelWndMgrList.begin(); it!=TopLevelWndMgrList.end(); it++) {
		OEPluginTopLevelWndMgr *curmgr = *it;

		if(curmgr->GetOEToolbarHandle() == hWnd) {
			mgr = curmgr;
			break;
		}
	}
	OEPluginToolbarMgr::UnlockTopLevelList();

	if(mgr == NULL) {
		debug_print(DEBUG_ERROR, _T("ToolbarWndProc: Fatal Error: cannot get top level wnd manager.\n"));
		return FALSE;
	}

	lpOldProc = mgr->GetOEToolbarOldProc();

	switch(msg) 
	{
		case TB_SETIMAGELIST:
		{
			return TRUE;
		}
		
		case TB_SETHOTIMAGELIST:
		{
			return TRUE;
		}

		case TB_ENABLEBUTTON:
		{
			// it isn't necessary to lock the object while inside the window proc.
			OEPluginToolbarBtn *btn = mgr->GetButton((INT)wParam, FALSE);
			if(btn != NULL) {
				lParam = MAKELONG(btn->IsEnabled(), 0);
			}

			break;
		}
		case TB_SETSTATE:
		{
			// it isn't necessary to lock the object while inside the window proc.
			OEPluginToolbarBtn *btn = mgr->GetButton((INT)wParam, FALSE);
			if(btn != NULL) {
				lParam = MAKELONG(btn->GetState(), 0);
			}

			break;
		}
		case TB_SETBUTTONINFO:
		{
//			// it isn't necessary to lock the object while inside the window proc.
//			OEPluginToolbarBtn *btn = mgr->GetButton((INT)wParam, FALSE);
//			if(btn != NULL) {
////				lParam = MAKELONG(btn->GetStyle(), 0);
//			}
//
			break;
		}
	}

	return CallWindowProc(lpOldProc, hWnd, msg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



class OEPluginToolbarMgrCleaner {
public:
	~OEPluginToolbarMgrCleaner()
	{
		if(critSectionInitialized) {
			OEPluginToolbarMgr::Lock();
		}
		if(ToolbarMgr) {
			delete ToolbarMgr;
		}
		if(critSectionInitialized) {
			OEPluginToolbarMgr::Unlock();
		}
	}

} MgrCleaner;

/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------//
OEPluginToolbarMgr::OEPluginToolbarMgr()
{
	if(!critSectionInitialized) {
		critSectionInitialized = TRUE;
		InitializeCriticalSection(&critSection);
		InitializeCriticalSection(&topLevelCritSection);
	}

	Lock();

	mwWndMgr_ = new OEPluginTopLevelWndMgr;
	msgWndMgr_ = new OEPluginTopLevelWndMgr;

	Unlock();
}

//---------------------------------------------------------------------------------//
OEPluginToolbarMgr::~OEPluginToolbarMgr()
{
	Lock();

	if(mwWndMgr_) {
		delete mwWndMgr_;
		mwWndMgr_ = NULL;
	}
	if(msgWndMgr_) {
		delete msgWndMgr_;
		msgWndMgr_ = NULL;
	}

	if(this == ToolbarMgr) {
		ToolbarMgr = NULL;
	}

	Unlock();
}

//---------------------------------------------------------------------------------//
OEPluginTopLevelWndMgr *OEPluginToolbarMgr::GetMainWndMgr()
{
	if(mwWndMgr_ == NULL) {
		mwWndMgr_ = new OEPluginTopLevelWndMgr;
	}
	return mwWndMgr_;
}

//---------------------------------------------------------------------------------//
OEPluginToolbar *OEPluginToolbarMgr::GetToolbar(INT id, BOOL lock)
{
	OEPluginToolbar *ret = NULL;

	Lock();

	if(mwWndMgr_) {
		ret = mwWndMgr_->GetToolbar(id, lock);
	}
	if(ret == NULL && msgWndMgr_) {
		ret = msgWndMgr_->GetToolbar(id, lock);
	}

	Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
OEPluginToolbar *OEPluginToolbarMgr::GetToolbarByIndex(INT index, INT oeapiId, BOOL lock)
{
	OEPluginToolbar *ret = NULL;
	INT count = 0;

	Lock();

	if(mwWndMgr_) {
		count = mwWndMgr_->GetToolbarCount(oeapiId);
		if(count > index) {
			ret = mwWndMgr_->GetToolbarByIndex(index, oeapiId, lock);
		}
	}
	else if(msgWndMgr_) {
		ret = msgWndMgr_->GetToolbarByIndex(index-count, oeapiId, lock);
	}

	Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
OEPluginToolbar *OEPluginToolbarMgr::GetToolbar(HWND handle, BOOL lock)
{
	OEPluginToolbar *ret = NULL;

	Lock();

	if(mwWndMgr_) {
		ret = mwWndMgr_->GetToolbar(handle, lock);
	}
	if(ret == NULL && msgWndMgr_) {
		ret = msgWndMgr_->GetToolbar(handle, lock);
	}

	Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarMgr::DestroyAllToolbars(INT oeapiId)
{
	if(mwWndMgr_) {
		mwWndMgr_->DestroyAllToolbars(oeapiId);
	}
	if(msgWndMgr_) {
		msgWndMgr_->DestroyAllToolbars(oeapiId);
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarMgr::DestroyOEMainWindowTopLevelWndMgr()
{
	Lock();

	if(mwWndMgr_) {
		mwWndMgr_->ResetToolbars();
		//delete mwWndMgr_;
		//mwWndMgr_ = NULL;
	}

	Unlock();
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn *OEPluginToolbarMgr::GetButton(INT id, BOOL lock)
{
	OEPluginToolbarBtn *ret = NULL;

	Lock();

	if(mwWndMgr_ != NULL) {
		ret = mwWndMgr_->GetButton(id, lock);
	}
	if(ret == NULL && msgWndMgr_ != NULL) {
		ret = msgWndMgr_->GetButton(id, lock);
	}

	Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn *OEPluginToolbarMgr::GetButtonInstance(HWND hToolbar, INT id, BOOL lock)
{
	OEPluginToolbarBtn *btn = NULL;

	OEPluginToolbarMgr::LockTopLevelList();
	for(OEPluginTopLevelWndMgrList::iterator it=TopLevelWndMgrList.begin(); it!=TopLevelWndMgrList.end(); it++) {
		OEPluginTopLevelWndMgr *curmgr = *it;
		OEPluginToolbar *tlb;

		tlb = curmgr->GetToolbar(hToolbar, lock);
		if(tlb) {
			btn = tlb->GetButton(id, lock);
			if(lock) {
				tlb->Unlock();
			}

			if(btn) {
				break;
			}
		}
	}

	OEPluginToolbarMgr::UnlockTopLevelList();

	return btn;
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarMgr::Lock()
{
	EnterCriticalSection(&critSection);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarMgr::Unlock()
{
	LeaveCriticalSection(&critSection);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarMgr::LockTopLevelList()
{
	EnterCriticalSection(&topLevelCritSection);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarMgr::UnlockTopLevelList()
{
	LeaveCriticalSection(&topLevelCritSection);
}

/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------//
OEPluginTopLevelWndMgr::OEPluginTopLevelWndMgr()
{
	mainWindow_ = NULL;
	hKeyReceiver_ = NULL;
	oldSizableRebarProc_ = NULL;
	hSizableRebar_ = NULL;
	hOEToolbar_ = NULL;
	initialized_ = FALSE;
	wndStyle_ = MsgWndStyles::OE_MSG_INVALID;
	oldToolbarProc_ = NULL;

	OEPluginToolbarMgr::LockTopLevelList();
	TopLevelWndMgrList.push_back(this);
	OEPluginToolbarMgr::UnlockTopLevelList();

	OEPluginToolbarMgr::Lock();
	OEPluginToolbarMgr::Unlock();
}

//---------------------------------------------------------------------------------//
OEPluginTopLevelWndMgr::OEPluginTopLevelWndMgr(OEPluginTopLevelWndMgr *mgr)
{
	mainWindow_ = NULL;
	hKeyReceiver_ = NULL;
	hSizableRebar_ = NULL;
	hOEToolbar_ = NULL;
	oldSizableRebarProc_ = NULL;
	initialized_ = FALSE;
	oldToolbarProc_ = NULL;

	hRebar_ = NULL;
	oldRebarProc_ = NULL;

	OEPluginToolbarMgr::LockTopLevelList();
	TopLevelWndMgrList.push_back(this);
	OEPluginToolbarMgr::UnlockTopLevelList();

	OEPluginToolbarMgr::Lock();

	for(OEPluginToolbarList::iterator it=mgr->toolbars_.begin(); it!=mgr->toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;
		new OEPluginToolbar(this, toolbar);
	}

	OEPluginToolbarMgr::Unlock();
}

//---------------------------------------------------------------------------------//
OEPluginTopLevelWndMgr::~OEPluginTopLevelWndMgr()
{
	debug_print(DEBUG_ERROR, _T("OEPluginTopLevelWndMgr::~OEPluginTopLevelWndMgr\n"));

	OEPluginToolbarMgr::Lock();

	if(oldToolbarProc_) {
		::SetWindowLongPtr(hOEToolbar_, GWLP_WNDPROC, (LONG_PTR)oldToolbarProc_);
		hOEToolbar_ = NULL;
		oldToolbarProc_ = NULL;
	}

	if(oldSizableRebarProc_) {
		::SetWindowLongPtr(hSizableRebar_, GWLP_WNDPROC, (LONG_PTR)oldSizableRebarProc_);
		hSizableRebar_ = NULL;
		oldSizableRebarProc_ = NULL;
	}

	if(oldRebarProc_) {
		::SetWindowLongPtr(hRebar_, GWLP_WNDPROC, (LONG_PTR)oldRebarProc_);
		hRebar_ = NULL;
		oldRebarProc_ = NULL;
	}

	while(toolbars_.size() != 0) {
		OEPluginToolbar *toolbar = toolbars_.back();
		delete toolbar;
	}

	TopLevelWndMgrList.remove(this);

	OEPluginToolbarMgr::Unlock();
}


//---------------------------------------------------------------------------------//
//VOID OEPluginTopLevelWndMgr::UpdateToolbarsBandIndex()
//{
//	INT i;
//	HWND hReBar;
//
//	OEPluginToolbarMgr::Lock();
//
//	hReBar = FindWindowEx(hSizableRebar_, NULL, _T("ReBarWindow32"), NULL);
//	if(hReBar == NULL) {
//		debug_print(DEBUG_ERROR, _T("UpdateToolbarsBandIndex: hReBar = NULL\n"));
//		return;
//	}
//
//	i = (INT)SendMessage(hReBar, RB_GETBANDCOUNT, 0, 0);
//	
//	for(; i >= 0; i --) {
//		REBARBANDINFO rbBand;
//
//		memset(&rbBand, 0, sizeof(REBARBANDINFO));
//		rbBand.cbSize = sizeof(REBARBANDINFO);
//		rbBand.fMask = RBBIM_CHILD;
//		
//		SendMessage(hReBar,
//					RB_GETBANDINFO, 
//					i, 
//					(LPARAM) &rbBand);
//		
//		// the toolbar cannot be destroyed while we are attending a message.
//		OEPluginToolbar *toolbar = GetToolbar(rbBand.hwndChild, FALSE);
//		if(toolbar) {
//			toolbar->SetBandIndex(i);
//		}
//	}
//
//	OEPluginToolbarMgr::Unlock();
//}

#define OE_TOOLBAR_ID 101

//---------------------------------------------------------------------------------//
HWND OEPluginTopLevelWndMgr::FindWindows(HWND mainWindow)
{
	REBARBANDINFO rbBand;
	HWND hSizableRebar, hRebar;

	if(hOEToolbar_ != NULL && IsWindow(hOEToolbar_)) {
		return hOEToolbar_;
	}

	hOEToolbar_ = NULL;

	// Get SizableRebar and hReBar handles
	hSizableRebar = FindWindowEx(mainWindow, NULL, _T("SizableRebar"), NULL);
	hRebar = FindWindowEx(hSizableRebar, NULL, _T("ReBarWindow32"), NULL);

	if(IsWMail() && hRebar) {
		hRebar = FindWindowEx(hSizableRebar, hRebar, _T("ReBarWindow32"), NULL);
	}
	if(!hRebar) {
		debug_print(DEBUG_ERROR, _T("OEPluginTopLevelWndMgr::FindWindows: Can't find rebar %d.\n"), ::GetLastError());
		return NULL;
	}

	INT band = ::SendMessage(hRebar, RB_IDTOINDEX, (WPARAM)OE_TOOLBAR_ID, 0);

	rbBand.cbSize = REBARBANDINFOA_V3_SIZE; // sizeof(rbBand);
	rbBand.fMask  = RBBIM_CHILD | RBBIM_ID;

	if(::SendMessage(hRebar, RB_GETBANDINFO, band, (LPARAM) &rbBand)) {
		if(rbBand.wID == 101) {
			hOEToolbar_ = rbBand.hwndChild;
		}
	}
	else {
		debug_print(DEBUG_ERROR, _T("OEPluginTopLevelWndMgr::FindWindows: GetBandInfo failed %d.\n"), ::GetLastError());
	}

	if(hOEToolbar_ != NULL) {
		hRebar_ = hRebar;
		hSizableRebar_ = hSizableRebar;
	}

	return hOEToolbar_;
}

//---------------------------------------------------------------------------------//
BOOL OEPluginTopLevelWndMgr::Initialize(HWND mainWindow, BOOL isMainWindow)
{
	BOOL ret = TRUE;
	TBBUTTONINFO btnInfo;

	OEPluginToolbarMgr::Lock();

	if(hOEToolbar_ == NULL || !IsWindow(hOEToolbar_)) {
		
		if(FindWindows(mainWindow) != NULL) {
			oldToolbarProc_ = (WNDPROC) GetWindowLongPtr(hOEToolbar_, GWLP_WNDPROC);
			SetWindowLongPtr(hOEToolbar_, GWLP_WNDPROC, (LONG_PTR)ToolbarWndProc);

			// if it is a message window enter always.
			if(mainWindow_ == NULL) {
				if(!isMainWindow) {
					btnInfo.cbSize = sizeof(TBBUTTONINFO);
					btnInfo.dwMask = 0;

					// if the toolbar does not have the Send btn it is a Msg Detail wnd
					if(::SendMessage(hOEToolbar_, TB_GETBUTTONINFO, OE_SEND_TOOLBAR_BTN, (LPARAM) &btnInfo) == -1) {
						wndStyle_ = MsgWndStyles::OE_MSG_DETAIL_WND;
					}
					else {
						wndStyle_ = MsgWndStyles::OE_MSG_SEND_WND;
					}
				}
				else {
					wndStyle_ = MsgWndStyles::OE_MSG_INVALID;
				}

				mainWindow_ = mainWindow;
				
				if(CreateToolbars()) {
					initialized_ = TRUE;
					ret = TRUE;
				}
			}
		}
	}

	// if OE toolbar was re-created, create again buttons. 
	// Old hOEToolbar_ was destroyed.
	if(isMainWindow) {
		OEPluginToolbarMgr::Lock();

		for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
			OEPluginToolbar *toolbar = *it;
			if(toolbar->IsOEDefaultToolbar()) {
				if(wndStyle_ == MsgWndStyles::OE_MSG_INVALID || (toolbar->GetWndStyle() & wndStyle_)) {
					toolbar->Create(-1, TRUE);
				}
			}
		}

		OEPluginToolbarMgr::Unlock();
	}

	OEPluginToolbarMgr::Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
BOOL OEPluginTopLevelWndMgr::CreateToolbars()
{
	BOOL ret = FALSE;

	OEPluginToolbarMgr::Lock();

	oldSizableRebarProc_ = (WNDPROC) GetWindowLongPtr(hSizableRebar_, GWLP_WNDPROC);
	SetWindowLongPtr(hSizableRebar_, GWLP_WNDPROC, (LONG_PTR)SizableRebarWndProc);

	oldRebarProc_ = (WNDPROC) GetWindowLongPtr(hRebar_, GWLP_WNDPROC);
	SetWindowLongPtr(hRebar_, GWLP_WNDPROC, (LONG_PTR)RebarWndProc);

	for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;
		if(wndStyle_ == MsgWndStyles::OE_MSG_INVALID || (toolbar->GetWndStyle() & wndStyle_) != 0) {
			// Update toolbar with the correct style
			if(wndStyle_ != MsgWndStyles::OE_MSG_INVALID) {
				toolbar->SetWndStyle(wndStyle_);
			}
			toolbar->Create();
		}
	}

	ret = TRUE;

	OEPluginToolbarMgr::Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
BOOL OEPluginTopLevelWndMgr::ResetToolbars()
{
	OEPluginToolbarMgr::Lock();

	if(oldToolbarProc_) {
		::SetWindowLongPtr(hOEToolbar_, GWLP_WNDPROC, (LONG_PTR)oldToolbarProc_);
		hOEToolbar_ = NULL;
		oldToolbarProc_ = NULL;
	}

	if(oldSizableRebarProc_) {
		::SetWindowLongPtr(hSizableRebar_, GWLP_WNDPROC, (LONG_PTR)oldSizableRebarProc_);
		hSizableRebar_ = NULL;
		oldSizableRebarProc_ = NULL;
	}

	if(oldRebarProc_) {
		::SetWindowLongPtr(hRebar_, GWLP_WNDPROC, (LONG_PTR)oldRebarProc_);
		hRebar_ = NULL;
		oldRebarProc_ = NULL;
	}

	initialized_ = FALSE;
	mainWindow_ = NULL;

	OEPluginToolbarList::iterator it = toolbars_.begin();

	for(; it != toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;
		toolbar->Reset();
	}

	OEPluginToolbarMgr::Unlock();

	return TRUE;
}

//---------------------------------------------------------------------------------//
VOID OEPluginTopLevelWndMgr::AddToolbar(OEPluginToolbar *toolbar)
{
	OEPluginToolbarMgr::Lock();
	toolbars_.push_back(toolbar);
	OEPluginToolbarMgr::Unlock();
}

//---------------------------------------------------------------------------------//
VOID OEPluginTopLevelWndMgr::RemoveToolbar(OEPluginToolbar *toolbar)
{
	OEPluginToolbarMgr::Lock();
	toolbars_.remove(toolbar);
	OEPluginToolbarMgr::Unlock();
}

//---------------------------------------------------------------------------------//
OEPluginToolbar *OEPluginTopLevelWndMgr::GetToolbar(INT id, BOOL lock)
{
	OEPluginToolbarMgr::Lock();

	OEPluginToolbar *ret = NULL;
	for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;

		if(toolbar->GetID() == id) {
			if(lock) {
				toolbar->Lock();
			}
			ret = toolbar;
			break;
		}
	}

	OEPluginToolbarMgr::Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
OEPluginToolbar *OEPluginTopLevelWndMgr::GetToolbar(HWND handle, BOOL lock)
{
	OEPluginToolbarMgr::Lock();

	OEPluginToolbar *ret = NULL;
	for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;

		if(toolbar->GetHandle() == handle) {
			if(lock) {
				toolbar->Lock();
			}
			ret = toolbar;
			break;
		}
	}

	OEPluginToolbarMgr::Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
OEPluginToolbar *OEPluginTopLevelWndMgr::GetToolbarByBandIndex(INT index, BOOL lock)
{
	OEPluginToolbarMgr::Lock();

	OEPluginToolbar *ret = NULL;
	for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;
		if(toolbar->GetBandIndex() == index) {
			if(lock) {
				toolbar->Lock();
			}
			ret = toolbar;
			break;
		}
	}

	OEPluginToolbarMgr::Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
INT OEPluginTopLevelWndMgr::GetToolbarCount(INT oeapiId)
{
	INT count;

	OEPluginToolbarMgr::Lock();

	if(oeapiId == -1) {
		count = (INT)toolbars_.size();
	}
	else {
		count = 0;
		for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
			OEPluginToolbar *toolbar = *it;

			if(oeapiId == toolbar->GetOEAPIID()) {
				count++;
			}
		}
	}

	OEPluginToolbarMgr::Unlock();

	return count;
}

//---------------------------------------------------------------------------------//
OEPluginToolbar *OEPluginTopLevelWndMgr::GetToolbarByIndex(INT index, INT oeapiId, BOOL lock)
{
	OEPluginToolbarMgr::Lock();

	OEPluginToolbar *ret = NULL;
	INT i = 0;
	for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;

		if(oeapiId == -1 || oeapiId == toolbar->GetOEAPIID()) {
			if(i++ == index) {
				if(lock) {
					toolbar->Lock();
				}
				ret = toolbar;
				break;
			}
		}
	}

	OEPluginToolbarMgr::Unlock();

	return ret;
}

//---------------------------------------------------------------------------------//
VOID OEPluginTopLevelWndMgr::DestroyAllToolbars(INT oeapiId)
{
	OEPluginToolbarMgr::Lock();

	for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end();) {
		OEPluginToolbar *toolbar = *it;

		// advance before removing the object
		it++;
		if(oeapiId == -1 || oeapiId == toolbar->GetOEAPIID()) {
			toolbars_.remove(toolbar);
			delete toolbar;
		}
	}

	OEPluginToolbarMgr::Unlock();

//	while(toolbars_.size() != 0) {
//		OEPluginToolbar *toolbar = toolbars_.front();
//
//		if(oeapiId == -1 || oeapiId == toolbar->GetOEAPIID()) {
//			OEPluginToolbarMgr::Lock();
//			toolbars_.remove(toolbar);
//			OEPluginToolbarMgr::Unlock();
//			delete toolbar;
//		}
//	}
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn *OEPluginTopLevelWndMgr::GetButton(INT id, BOOL lock)
{
	OEPluginToolbarMgr::Lock();
	
	OEPluginToolbarBtn *btn = NULL;
	for(OEPluginToolbarList::iterator it=toolbars_.begin(); it!=toolbars_.end(); it++) {
		OEPluginToolbar *toolbar = *it;

		btn = toolbar->GetButton(id, lock);
		if(btn) {
			break;
		}
	}

	OEPluginToolbarMgr::Unlock();

	return btn;
}

//---------------------------------------------------------------------------------//
VOID OEPluginTopLevelWndMgr::SendMail(BOOL bLater)
{
	OEPluginToolbarMgr::Lock();

	if(hOEToolbar_) {
		// lParam -1 to notify that the message was generated from OEAPI and shouldn't be
		// reported.
		if(!bLater) {
			PostMessage(hOEToolbar_, WM_COMMAND, OE_SEND_TOOLBAR_BTN, -1);
		}
		else {
			PostMessage(hOEToolbar_, WM_COMMAND, OE_SENDLATER_MENUITEM_BTN, -1);
		}
	}

	OEPluginToolbarMgr::Unlock();
}

//---------------------------------------------------------------------------------//
VOID OEPluginTopLevelWndMgr::SendKey(BYTE key, BYTE controlKey, BOOL setForeground)
{
	HWND oldForeWnd;

	if(hKeyReceiver_) {

		if(setForeground) {
			oldForeWnd = GetForegroundWindow();
			SetForegroundWindow(mainWindow_);
		}

		if(controlKey) {
			keybd_event(controlKey, 0, 0, 0);
		}

		keybd_event(key, 0, 0, 0);
		keybd_event(key, 0, KEYEVENTF_KEYUP, 0);

		if(controlKey) {
			keybd_event(controlKey, 0, KEYEVENTF_KEYUP, 0);
		}

		if(setForeground && oldForeWnd) {
			SetForegroundWindow(oldForeWnd);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------//
OEPluginToolbar::OEPluginToolbar(OEPluginTopLevelWndMgr *mgr)
{
	handle_ = NULL;
	hidden_ = FALSE;
	tooltips_ = FALSE;
	rebar_ = NULL;
	popupMenu_ = NULL;
	id_ = ToolbarId++;
	largeBtns_ = TRUE;
	//bandIndex_ = -1;
	oeapiId_ = -1;
	wndStyle_ = MsgWndStyles::OE_MSG_INVALID;
	defOEToolbar_ = FALSE;
	defOEToolbarBtnCount_ = 0;
	maskColor_ = TRANSPARENT_COLOR;

	//dummyStrIndex_ = -1;
	rows_ = -1;

	cx_ = -1;
	cy_ = -1;

	InitializeCriticalSection(&critSection_);

	topLevelMgr_ = mgr;

	topLevelMgr_->AddToolbar(this);
}

//---------------------------------------------------------------------------------//
OEPluginToolbar::OEPluginToolbar(OEPluginTopLevelWndMgr *mgr, OEPluginToolbar *toolbar)
{
	handle_ = NULL;
	rebar_ = NULL;
	popupMenu_ = NULL;
	//bandIndex_ = -1;
	topLevelMgr_ = mgr;
	defOEToolbarBtnCount_ = 0;

	//dummyStrIndex_ = -1;
	rows_ = toolbar->rows_;

	hidden_ = toolbar->hidden_;
	tooltips_ = toolbar->tooltips_;
	id_ = toolbar->id_;
	largeBtns_ = toolbar->largeBtns_;
	oeapiId_ = toolbar->oeapiId_;
	wndStyle_ = toolbar->wndStyle_;
	defOEToolbar_ = toolbar->defOEToolbar_;
	maskColor_ = toolbar->maskColor_;

	cx_ = toolbar->cx_;
	cy_ = toolbar->cy_;

	InitializeCriticalSection(&critSection_);

	for(OEPluginToolbarBtnList::iterator it=toolbar->buttons_.begin(); it!=toolbar->buttons_.end(); it++) {
		OEPluginToolbarBtn *btn = *it;
		OEPluginToolbarBtn *newBtn = new OEPluginToolbarBtn(btn);

		AddButton(newBtn);
	}

	topLevelMgr_->AddToolbar(this);
}

//---------------------------------------------------------------------------------//
OEPluginToolbar::~OEPluginToolbar()
{
	topLevelMgr_->RemoveToolbar(this);

	Lock();

	if(rebar_ && !defOEToolbar_) {
		// get the band index again. It should be ok.
		INT bandIndex = CalculateBandIndex();

		// FIXME: sometimes it doesn't work
		if(bandIndex == -1) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbar::~OEPluginToolbar: Invalid band index.\n"));
		}
		else {
			if(::SendMessage(rebar_, RB_DELETEBAND, bandIndex, 0) == 0) {
				debug_print(DEBUG_ERROR, _T("OEPluginToolbar::~OEPluginToolbar: RB_DELETEBAND failed.\n"));
			}
			else {
				DestroyWindow(handle_);
				handle_ = NULL;
			}
		}

//		int i;
//
//		i = SendMessage(rebar_, RB_GETBANDCOUNT, 0, 0) - 1;
//		
//		for(; i>= 0; i--) {
//			REBARBANDINFO rbBand;
//
//			memset(&rbBand, 0, sizeof(REBARBANDINFO));
//			rbBand.cbSize = sizeof(REBARBANDINFO);
//			rbBand.fMask = RBBIM_CHILD;
//			
//			SendMessage(rebar_, RB_GETBANDINFO, i, (LPARAM) &rbBand);
//			
//			if(rbBand.hwndChild == handle_) {
//				debug_print(DEBUG_ERROR, _T("Deleting band...\n"));
//
//				if (SendMessage(rebar_, RB_DELETEBAND, i, 0) == 0) {
//					debug_print(DEBUG_ERROR, _T("RB_DELETEBAND failed.\n"));
//				}
//				else {
//					DestroyWindow(handle_);
//					handle_ = NULL;
//				}
//
//				break;
//			}
//		}
	}

	while(buttons_.size() != 0) {
		OEPluginToolbarBtn *button = buttons_.front();
		delete button;
	}

	if(popupMenu_) {
		DestroyMenu(popupMenu_);
	}

	Unlock();

	DeleteCriticalSection(&critSection_);
}

//---------------------------------------------------------------------------------//
DWORD OEPluginToolbar::GetStyle()
{

	DWORD style = TBSTYLE_FLAT | TBSTYLE_AUTOSIZE | TBSTYLE_TRANSPARENT;
	//style =  CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER 
	//	| WS_CHILD | TBSTYLE_FLAT;
	
//	DWORD style = CCS_NOPARENTALIGN|TBSTYLE_AUTOSIZE|CCS_NODIVIDER|WS_CHILD|TBSTYLE_FLAT;

	//if(IsWMail()) {
	//	// This doesn't seems to be necesary
	//	//style = 0x4600097C;
	//}

	if(!largeBtns_) {
		style |= TBSTYLE_LIST;
	}
	if(!hidden_) {
		style |= WS_VISIBLE;
	}
	if(tooltips_) {
		style |= TBSTYLE_TOOLTIPS;
	}
	return style;
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::Lock()
{
	EnterCriticalSection(&critSection_);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::Unlock()
{
	LeaveCriticalSection(&critSection_);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::Create(INT oeapiId, BOOL createAlways)
{
	HWND hwndParent;

	if(oeapiId != -1) {
		oeapiId_ = oeapiId;
	}

	// don't create it twice
	if(handle_ != NULL && !createAlways) {
		return;
	}

	// if the main window wasn't created, create the toolbar later
	hwndParent = topLevelMgr_->GetMainWindow();
	if(hwndParent == NULL) {
		return;
	}

	CreateInWnd(hwndParent);
}
//
//typedef struct tagREBARBANDINFOVISTA
//{
//    UINT        cbSize;
//    UINT        fMask;
//    UINT        fStyle;
//    COLORREF    clrFore;
//    COLORREF    clrBack;
//    LPSTR       lpText;
//    UINT        cch;
//    int         iImage;
//    HWND        hwndChild;
//    UINT        cxMinChild;
//    UINT        cyMinChild;
//    UINT        cx;
//    HBITMAP     hbmBack;
//    UINT        wID;
//#if (_WIN32_IE >= 0x0400)
//    UINT        cyChild;
//    UINT        cyMaxChild;
//    UINT        cyIntegral;
//    UINT        cxIdeal;
//    LPARAM      lParam;
//    UINT        cxHeader;
//#endif
//    RECT        rcChevronLocation;  
//    UINT        uChevronState; 
//} REBARBANDINFOVISTAA, *LPREBARBANDINFOVISTAA;
//
//#define REBARBANDINFOVISTA REBARBANDINFOVISTAA
//
//#include <wingdi.h>

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::CreateInWnd(HWND mainWindow)
{
	HWND hSizableRebar;
	//REBARBANDINFO rbBand;
	HIMAGELIST imageList;

	// Get SizableRebar and hReBar handles
	hSizableRebar = FindWindowEx(mainWindow, NULL, _T("SizableRebar"), NULL);
	rebar_ = FindWindowEx(hSizableRebar, NULL, _T("ReBarWindow32"), NULL);

	if(IsWMail() && rebar_) {
		rebar_ = FindWindowEx(hSizableRebar, rebar_, _T("ReBarWindow32"), NULL);
	}

	if(hSizableRebar == NULL || rebar_ == NULL) {
		return;
	}

	if(defOEToolbar_) {
		handle_ = topLevelMgr_->GetOEToolbarHandle();
		if (handle_ == NULL) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbar::CreateInWnd Find Toolbar failed.\n"));
			return;
		}
		defOEToolbarBtnCount_ = (INT)::SendMessage(handle_, TB_BUTTONCOUNT, 0, 0);
	}
	else {
		if(IsWMail()) {
			// Create a new toolbar
			DWORD dwExStyle = 0;	//WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_TOOLWINDOW; // WS_EX_TOOLWINDOW;
			DWORD dwStyle = CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER | WS_CHILD | GetStyle(); 
				//WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | 
				//CCS_NORESIZE | GetStyle();

			handle_ = CreateWindowEx( dwExStyle, 
				TOOLBARCLASSNAME, 
				NULL, 
				dwStyle, 
				0, 0, 
				CW_USEDEFAULT, CW_USEDEFAULT, 
				rebar_, 
				NULL, NULL, NULL);

			if (handle_ == NULL) {
				debug_print(DEBUG_ERROR, _T("CreateWindow Toolbar failed.\n"));
				return;
			}

			::SendMessage(handle_, TB_SETEXTENDEDSTYLE, 0, 
				TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_HIDECLIPPEDBUTTONS);

			::SendMessage(handle_, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

			::SendMessageW(handle_, TB_SETWINDOWTHEME, 0, (LPARAM) L"Communications");
		}
		else { // OE6
			// Create a new toolbar
			DWORD dwExStyle = 0;	//WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_TOOLWINDOW; // WS_EX_TOOLWINDOW;
			DWORD dwStyle = CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER | WS_CHILD | GetStyle(); 
				//WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | 
				//CCS_NORESIZE | GetStyle();

			handle_ = CreateWindowEx( dwExStyle, 
				TOOLBARCLASSNAME, 
				NULL, 
				dwStyle, 
				0, 0, 
				CW_USEDEFAULT, CW_USEDEFAULT, 
				rebar_, 
				NULL, NULL, NULL);

			if (handle_ == NULL) {
				debug_print(DEBUG_ERROR, _T("CreateWindow Toolbar failed.\n"));
				return;
			}

			::SendMessage(handle_, TB_SETEXTENDEDSTYLE, 0, 
						TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_HIDECLIPPEDBUTTONS);

			::SendMessage(handle_, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
		}

		// Set text rows
		if (rows_ >= 0)	{
			SendMessage(handle_, TB_SETMAXTEXTROWS, rows_, 0);
		}

		imageList = CreateImageList();

		if(imageList == NULL) {
			debug_print(DEBUG_ERROR, _T("ImageList_Create.\n"));
			return;
		}

		::SendMessage(handle_, TB_SETIMAGELIST, 0, (LPARAM) imageList);
	}

	// now, create the buttons already added
	for(OEPluginToolbarBtnList::iterator it=buttons_.begin(); it!=buttons_.end(); it++) {
		OEPluginToolbarBtn *button = *it;
		button->Create(this);
	}
	
	if(!defOEToolbar_) {
		DWORD dwBtnSize;

		// This is used to calculate the height of the band
		dwBtnSize = (INT)::SendMessage(handle_, TB_GETBUTTONSIZE, 0, 0);

		// Fill the band info structure
		REBARBANDINFO rbBand;
		rbBand.cbSize = REBARBANDINFOA_V3_SIZE; // sizeof(REBARBANDINFO);
		
		rbBand.fMask  = RBBIM_STYLE | RBBIM_CHILD | 
						RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;

		// Messenger threw here DirectDBNotifyWndProc
		::SendMessage(rebar_, RB_GETBANDINFO, 0, (LPARAM) &rbBand);

		rbBand.cbSize = REBARBANDINFOA_V3_SIZE; // sizeof(REBARBANDINFO);

		rbBand.fMask |=  RBBIM_ID;

		rbBand.fStyle = RBBS_CHILDEDGE | RBBS_USECHEVRON | RBBS_BREAK;
		if(hidden_)
		{
			rbBand.fStyle |= RBBS_HIDDEN;
		}

		rbBand.hwndChild = handle_;
		rbBand.cxMinChild = 0;
		rbBand.cyMinChild = HIWORD(dwBtnSize);
		rbBand.cx = 250;
		rbBand.cxIdeal = LOWORD(dwBtnSize)*GetVisibleButtonsCount();
		rbBand.wID = id_;

		// And finally, we add the band ...
		::SendMessage(rebar_, RB_INSERTBAND, -1, (LPARAM)&rbBand);

		if(buttons_.size() > 0) {
			UpdateBandSize(*buttons_.rbegin());
		}
	}
	else {
		//bandIndex_ = CalculateBandIndex();
		UpdateBandSize();
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::Reset()
{
	Lock();

	if(rebar_ && !defOEToolbar_) {
		// get the band index again. It should be ok.
		INT bandIndex = CalculateBandIndex();

		// FIXME: sometimes it doesn't work
		if(bandIndex == -1) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbar::~OEPluginToolbar: Invalid band index.\n"));
		}
		else {
			if(::SendMessage(rebar_, RB_DELETEBAND, bandIndex, 0) == 0) {
				debug_print(DEBUG_ERROR, _T("OEPluginToolbar::~OEPluginToolbar: RB_DELETEBAND failed.\n"));
			}
			else {
				DestroyWindow(handle_);
				handle_ = NULL;
			}
		}

		rebar_ = NULL;
	}

	OEPluginToolbarBtnList::iterator it = buttons_.begin();
	for(; it != buttons_.end(); it++) {
		OEPluginToolbarBtn* button = *it;
		button->Reset();
	}

	if(popupMenu_) {
		DestroyMenu(popupMenu_);
		popupMenu_ = NULL;
	}

	Unlock();
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::SetLargeButtons(BOOL large)
{
	if(!defOEToolbar_) {
		largeBtns_ = large;
		if(handle_) {
			SendMessage(handle_, TB_SETSTYLE, 0, GetStyle());
			UpdateWindow(handle_);
		}
	}
}

//-----------------------------------------------------------//
VOID OEPluginToolbar::SetMaxTextRows(INT rows)
{
	rows_ = rows;
	if(handle_) {
		SendMessage(handle_, TB_SETMAXTEXTROWS, rows_, 0);
		UpdateWindow(handle_);
	}
}

//-----------------------------------------------------------//
VOID OEPluginToolbar::EnableTooltips(BOOL enable)
{
	if(!IsOEDefaultToolbar() && (handle_ == NULL)) {
		tooltips_ = enable;
	}
}

//-----------------------------------------------------------//
HWND OEPluginToolbar::GetTooltipsHandle()
{
	if(handle_)	{
		return (HWND)SendMessage(handle_, TB_GETTOOLTIPS, 0, 0);
	}
	return NULL;
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::SetShown(BOOL show)
{
	hidden_ = !show;
	if(handle_) {
		SendMessage(rebar_, RB_SHOWBAND, (WPARAM)GetBandIndex(), show);
		UpdateWindow(handle_);
		//bandIndex_ = CalculateBandIndex();
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::AddButton(OEPluginToolbarBtn *button)
{
	buttons_.push_back(button);

	button->SetOEAPIID(oeapiId_);
	button->SetToolbarID(id_);
	button->Create(this);

	if(handle_) {
		UpdateBandSize(button);
		ShowAllButtons();
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::RemoveButton(OEPluginToolbarBtn *button)
{
	buttons_.remove(button);

	if(handle_) {
		TBBUTTONINFO tb;
		tb.cbSize = sizeof(TBBUTTONINFO);
		tb.dwMask = 0;
		INT index = (INT)::SendMessage(handle_, TB_GETBUTTONINFO, button->GetID(), (LPARAM) &tb);

		if(index != -1) {
			SendMessage(handle_, TB_DELETEBUTTON, index,0);
		}

		UpdateBandSize();
		ShowAllButtons();
	}
}

//---------------------------------------------------------------------------------//
BOOL OEPluginToolbar::HasSubButtons()
{
	for(OEPluginToolbarBtnList::iterator it=buttons_.begin(); it!=buttons_.end(); it++) {
		OEPluginToolbarBtn *button = *it;
		if(button->GetSubButtonCount() != 0) {
			return TRUE;
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::UpdateBandSize(OEPluginToolbarBtn *btn)
{
	SIZE size;

	if(btn) {
		BOOL show = !btn->IsHidden();
		btn->SetShown(!show, FALSE);
		btn->SetShown(show, FALSE);
	}

	if(SendMessage(handle_, TB_GETMAXSIZE, 0, (LPARAM) &size)) {
		if(!IsOEDefaultToolbar()) {
			REBARBANDINFO rbBand;

			// Fill the band info structure
			rbBand.cbSize = REBARBANDINFOA_V3_SIZE; //sizeof(REBARBANDINFO);
			
			rbBand.fMask  = RBBIM_CHILD | 
							RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;

			::SendMessage(rebar_, RB_GETBANDINFO, (WPARAM)GetBandIndex(), (LPARAM)&rbBand);

			rbBand.hwndChild = handle_;

			// RBBIM_CHILDSIZE 
			rbBand.cxMinChild = 0;
			rbBand.cyMinChild = size.cy;
			rbBand.cyChild = size.cy;
			rbBand.cyMaxChild = size.cy;
			rbBand.cyIntegral = 0;

			// RBBIM_IDEALSIZE
			rbBand.cxIdeal = size.cx;

			// RBBIM_SIZE
			rbBand.cx = 250;

			// And finally, set the info of the band
			SendMessage(rebar_, RB_SETBANDINFO, GetBandIndex(), (LPARAM)&rbBand);
		}
	}
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn *OEPluginToolbar::GetButton(INT id, BOOL lock)
{
	OEPluginToolbarBtn *ret = NULL;
	for(OEPluginToolbarBtnList::iterator it=buttons_.begin(); it!=buttons_.end(); it++) {
		OEPluginToolbarBtn *button = *it;
		if(button->GetID() == id) {
			if(lock) {
				button->Lock();
			}
			return button;
		}

		ret = button->GetSubButton(id, lock);
		if(ret) {
			return ret;
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn *OEPluginToolbar::GetButtonByIndex(INT index, BOOL lock)
{
	INT i = 0;
	for(OEPluginToolbarBtnList::iterator it=buttons_.begin(); it!=buttons_.end(); it++) {
		if(i++ == index) {
			OEPluginToolbarBtn *button = *it;
			if(lock) {
				button->Lock();
			}
			return button;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------------//
int OEPluginToolbar::GetVisibleButtonsCount()
{
	INT count = 0;
	for(OEPluginToolbarBtnList::iterator it=buttons_.begin(); it!=buttons_.end(); it++) {
		OEPluginToolbarBtn *button = *it;
		if(!button->IsHidden()) {
			count++;
		}
	}

	return count;
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::ShowChevron()
{
	CreateChevron();
	if(popupMenu_) {
		RECT rc;
		POINT p;

		SendMessage(rebar_, RB_GETRECT,
					(WPARAM) GetBandIndex(), (LPARAM) &rc);

		p.x = rc.right;
		p.y = rc.bottom;

		ClientToScreen(rebar_, &p);

		TrackPopupMenu(popupMenu_, TPM_RIGHTALIGN|TPM_TOPALIGN,
						p.x, p.y, 0, 
						rebar_, 0);
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::ShowAllButtons()
{
	for(OEPluginToolbarBtnList::iterator it=buttons_.begin(); it!=buttons_.end(); it++) {
		OEPluginToolbarBtn *btn = *it;
		
		if(!btn->IsHidden()) {
			SendMessage(handle_, TB_HIDEBUTTON,
				(WPARAM) btn->GetID(), (LPARAM) MAKELONG(FALSE, 0));
		}
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbar::CreateChevron()
{
	RECT BandRC;
	RECT DstRC;
	RECT ButtonRC;
	INT ButtonCount;
	INT i;
	POINT p0;
	BOOL first = TRUE;
	BOOL hideRestBtns = FALSE;

	if(handle_ == NULL) {
		return;
	}

	SendMessage(rebar_, RB_GETRECT,
			(WPARAM) GetBandIndex(), (LPARAM) &BandRC);

	// Moving it to an absolute value
	BandRC.bottom = BandRC.bottom - BandRC.top;
	BandRC.top = 0;

	BandRC.right = BandRC.right - BandRC.left;
	BandRC.left = 0;

	ButtonCount = (INT)::SendMessage(handle_, TB_BUTTONCOUNT, 0, 0);

	if(popupMenu_) {
		DestroyMenu(popupMenu_);
	}

	popupMenu_ = CreatePopupMenu();
	if(popupMenu_ == NULL)
		return;

	// TODO: Check if this work when there's no text rows
	SendMessage(handle_, TB_AUTOSIZE, 0, 0);

	// After a button cannot be shown, add the rest of the buttons to the chevron popupMenu_
	for (i = 0; i < ButtonCount; i++)
	{
		TBBUTTON tb;

		SendMessage(handle_, TB_GETBUTTON,
					(WPARAM) i, (LPARAM) &tb);

		if(!hideRestBtns) {
			POINT Point;
			DWORD dwBtnSize;

			if (SendMessage(handle_, TB_GETITEMRECT,
						(WPARAM) i, (LPARAM) &ButtonRC) == FALSE)
			{
				debug_print(DEBUG_ERROR, _T("OEPluginToolbar::UpdateBtnPositions: TB_GETITEMRECT failed\n"));
			}

			Point.x = ButtonRC.left;
			Point.y = ButtonRC.top;

			if(first) {
				p0 = Point;
				first = false;
			}

			dwBtnSize = (DWORD)::SendMessage(handle_, TB_GETBUTTONSIZE, 0, 0);

			ButtonRC.right = Point.x + LOWORD(dwBtnSize);
			ButtonRC.bottom = Point.y + HIWORD(dwBtnSize);
			ButtonRC.top = Point.y;
			ButtonRC.left = Point.x;

			// DstRC will have the visible portion of the button
			if (IntersectRect(&DstRC, &ButtonRC, &BandRC) == FALSE)
			{
				debug_print(DEBUG_ERROR, _T("OEPluginToolbar::UpdateBtnPositions: Rects do not intersect\n"));
			}

			// XXX: this is not legal, but i don't know why i get
			// the bottom for the band rc different from the bottom
			// for the button rc. It cannot be possible; but as we
			// do not change the height for the band...
			DstRC.bottom = ButtonRC.bottom;
		}

		// If FALSE, a portion of the button is not visible
		if(hideRestBtns || EqualRect(&ButtonRC, &DstRC) == FALSE)
		{
			TBBUTTON tb;

			SendMessage(handle_, TB_GETBUTTON,
						(WPARAM) i, (LPARAM) &tb);

			// we are in the GUI thread, so we don't need to lock
			OEPluginToolbarBtn *btn = GetButton(tb.idCommand, FALSE);
			if(btn) {
				btn->CreateInMenu(popupMenu_);
			}

			hideRestBtns = TRUE;
		}
	}
}

//---------------------------------------------------------------------------------//
INT OEPluginToolbar::CalculateBandIndex()
{
	INT i;

	i = (INT)::SendMessage(rebar_, RB_IDTOINDEX, id_, 0);

	if(i == -1) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::CalculateBandIndex: Cannot get band index.\n"));
	}

	return i;
}


//------------------------------------------------------------------//
HBITMAP OEPluginToolbar::LoadButtonImage(LPCTSTR filename)
{
	INT cx, cy;

	HIMAGELIST imageList = (HIMAGELIST)SendMessage(GetHandle(), TB_GETIMAGELIST, 0, 0); 
	ImageList_GetIconSize(imageList, &cx, &cy);

	UINT flags = (GetMaskColor() != TRANSPARENT_COLOR) ? 
		LR_CREATEDIBSECTION | LR_LOADFROMFILE :								// new method
		LR_LOADFROMFILE | LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS;			// old method

	return ::LoadImageFromFile(filename, flags, cx, cy);
}

//------------------------------------------------------------------//
HIMAGELIST OEPluginToolbar::CreateImageList()
{
	HIMAGELIST hImgList;
	UINT flags;
	if(GetMaskColor() != TRANSPARENT_COLOR) {
		flags = ILC_COLOR32 | ILC_MASK;
	}
	else {
		flags = TRANSPARENT_COLOR;
	}
	if(cx_ <= 0 || cy_ <= 0) {
		cx_ = 16;
		cy_ = 16;
	}
	hImgList = ImageList_Create(cx_, cy_, flags, 0, 10);
	return hImgList;
}

//------------------------------------------------------------------//
BOOL OEPluginToolbar::SetBitmapSize(INT cx, INT cy)
{
	if(IsOEDefaultToolbar()) {
		return FALSE;
	}

	if(GetHandle()) {
		HIMAGELIST imageList = (HIMAGELIST)SendMessage(GetHandle(), TB_GETIMAGELIST, 0, 0); 
		if(imageList != NULL) {
			// A button has been added can't change size
			return FALSE;
		}
	}

	// Not created yet
	cx_ = cx;
	cy_ = cy;
	return TRUE;
}


//------------------------------------------------------------------//
BOOL OEPluginToolbar::GetBitmapSize(INT* cx, INT* cy)
{
	if(cx_ > 0 && cy_ > 0) {
		*cx = cx_;
		*cy = cy_;
		return TRUE;
	}

	if(GetHandle()) {
		HIMAGELIST imageList = (HIMAGELIST)SendMessage(GetHandle(), TB_GETIMAGELIST, 0, 0); 
		if(imageList != NULL) {
			ImageList_GetIconSize(imageList, cx, cy);
			cx_ = *cx;
			cy_ = *cy;
			return TRUE;
		}
	}

	if(IsOEDefaultToolbar()) {
		// We don't know it's size until is createed
		cx_ = *cx = -1;
		cy_ = *cy = -1;
	}
	else {
		// If not set use default size;
		cx_ = *cx = 16;
		cy_ = *cy = 16;
	}

	return FALSE;
}


//------------------------------------------------------------------//
INT OEPluginToolbar::AddImage(HBITMAP hbmp)
{
	int index = -1;
	DWORD gle = 0;

	HIMAGELIST imageList = (HIMAGELIST)SendMessage(GetHandle(), TB_GETIMAGELIST, 0, 0); 

	if(!IsOEDefaultToolbar()) {
		HBITMAP hMask = ::CreateBitmapMask(hbmp, GetMaskColor());
		index = ImageList_Add(imageList, hbmp, hMask);
		gle = ::GetLastError();
		::DeleteObject(hMask);
	}
	else {
		index = ImageList_AddMasked(imageList, hbmp, GetMaskColor());
		gle = ::GetLastError();
	}
	if(index < 0) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::AddImage: ImageList_AddMasked failed %d.\n"), gle);
		return index;
	}

	// Update ImagesList
	if (SendMessage(GetHandle(), TB_SETIMAGELIST, 0, (LPARAM) imageList) == 0) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::AddImage: TB_SETIMAGELIST return 0.\n"));
		return -1;
	}

	return index;
}

//------------------------------------------------------------------//
INT OEPluginToolbar::AddHotImage(HBITMAP hbmp)
{
	int index = -1;	// error 
	DWORD gle = 0;

	HIMAGELIST hotImageList = (HIMAGELIST)SendMessage(GetHandle(), TB_GETHOTIMAGELIST, 0, 0); 
	if(hotImageList == NULL) {
		hotImageList = CreateImageList();
		if(hotImageList == NULL) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbar::AddHotImage: Can't create hot image list.\n"));
			return -1;
		}
	}

	if(!IsOEDefaultToolbar()) {
		HBITMAP hMask = ::CreateBitmapMask(hbmp, GetMaskColor());
		index = ImageList_Add(hotImageList, hbmp, hMask);
		gle = ::GetLastError();
		::DeleteObject(hMask);
	}
	else {
		index = ImageList_AddMasked(hotImageList, hbmp, GetMaskColor());
		gle = ::GetLastError();
	}

	if(index < 0) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::AddHotImage: ImageList_Add failed %d.\n"), gle);
		return -1;
	}

	// Update ImagesList
	SendMessage(GetHandle(), TB_SETHOTIMAGELIST, 0, (LPARAM) hotImageList);

	return index;
}

//------------------------------------------------------------------//
INT OEPluginToolbar::ReplaceImage(INT index, HBITMAP hbmp, HBITMAP hmask)
{
	HIMAGELIST imageList = (HIMAGELIST)SendMessage(GetHandle(), TB_GETIMAGELIST, 0, 0); 
	if(!ImageList_Replace(imageList, index, hbmp, hmask)) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::ReplaceImage: Can't replace image\n"));
		return FALSE;
	}
	else {
		// Update ImagesList
		if (SendMessage(GetHandle(), TB_SETIMAGELIST, 0, (LPARAM) imageList) == 0) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbar::ReplaceImage: TB_SETIMAGELIST return 0.\n"));
			return FALSE;
		}
	}
	return TRUE;
}

//------------------------------------------------------------------//
INT OEPluginToolbar::ReplaceHotImage(INT index, HBITMAP hbmp, HBITMAP hmask)
{
	HIMAGELIST hotImageList = (HIMAGELIST)SendMessage(GetHandle(), TB_GETHOTIMAGELIST, 0, 0); 
	if(hotImageList == NULL) {
		hotImageList = CreateImageList();
		if(hotImageList == NULL) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbar::ReplaceHotImage: Can't create hot image list.\n"));
			return FALSE;
		}
	}
	if(!ImageList_Replace(hotImageList, index, hbmp, hmask)) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbar::ReplaceHotImage: Can't replace image\n"));
		return FALSE;
	}
	else {
		// Update ImagesList
		if (SendMessage(GetHandle(), TB_SETHOTIMAGELIST, 0, (LPARAM) hotImageList) == 0) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbar::ReplaceHotImage: TB_SETIMAGELIST return 0.\n"));
			return FALSE;
		}
	}
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------//
OEPluginToolbarBtn::OEPluginToolbarBtn()
{
	name_ = _T(""); //[0] = 0;
	tooltip_ = _T(""); //[0] = '\0';
	bmpNormal_ = _T(""); //[0] = 0;
	bmpMouseOver_ = _T(""); //[0] = 0;
	//hbmpNormal_ = NULL;
	//hbmpOver_ = NULL;
	pictNormal_ = NULL;
	pictOver_ = NULL;
	hbmpChecked_ = NULL;
	hbmpUnchecked_ = NULL;
	callback_ = NULL;
	id_ = ButtonId++;
	created_ = FALSE;
	hidden_ = FALSE;
	enabled_ = TRUE;
	toolbar_ = NULL;
	isSeparator_ = FALSE;
	type_ = img_invalid;
	toolbarId_ = -1;
	oeapiId_ = -1;
	parentButId_ = -1;
	bmpIndexNormal_ = -1;
	popupMenu_ = NULL;
	popupStyle_ = FALSE;
	checkStyle_ = FALSE;
	checked_ = FALSE;
 
	InitializeCriticalSection(&critSection_);
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn::OEPluginToolbarBtn(OEPluginToolbarBtn *btn)
{
	created_ = FALSE;
	toolbar_ = NULL;
	bmpIndexNormal_ = -1;
	toolbarId_ = -1;
	oeapiId_ = -1;
	parentButId_ = -1;
	popupMenu_ = NULL;

	name_ = btn->name_;
	tooltip_ = btn->tooltip_;
	bmpNormal_ = btn->bmpNormal_;
	bmpMouseOver_ = btn->bmpMouseOver_;
	//hbmpNormal_ = (btn->hbmpNormal_ != NULL) ? CopyBitmap(btn->hbmpNormal_) : NULL;
	//hbmpOver_ = (btn->hbmpOver_ != NULL) ? CopyBitmap(btn->hbmpOver_) : NULL;
	if(btn->pictNormal_ != NULL) {
		pictNormal_ =  btn->pictNormal_;
		pictNormal_->AddRef();
	}
	else {
		pictNormal_ =  NULL;
	}
	if(btn->pictOver_ != NULL) {
		pictOver_ =  btn->pictNormal_;
		pictOver_->AddRef();
	}
	else {
		pictOver_ =  NULL;
	}
	hbmpChecked_ = NULL;
	hbmpUnchecked_ = NULL;

	callback_ = btn->callback_;
	id_ = btn->id_;
	hidden_ = btn->hidden_;
	enabled_ = btn->enabled_;
	type_ = btn->type_;
	isSeparator_ = btn->isSeparator_;
	popupStyle_ = btn->popupStyle_;

	checkStyle_ = btn->checkStyle_;
	checked_ = btn->checked_;

	InitializeCriticalSection(&critSection_);

	for(OEPluginToolbarBtnList::iterator it=btn->subButtons_.begin(); it!=btn->subButtons_.end(); it++) {
		OEPluginToolbarBtn *btn = *it;
		OEPluginToolbarBtn *newBtn = new OEPluginToolbarBtn(btn);

		AddButton(newBtn);
	}
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn::~OEPluginToolbarBtn()
{
	Lock();

	Remove();

	if(pictNormal_ != NULL)	{
		pictNormal_->Release();
		pictNormal_ = NULL;
	}
	
	if(pictOver_ != NULL) {
		pictOver_->Release();
		pictOver_ = NULL;
	}

	if(hbmpChecked_ != NULL)	{
		::DeleteObject(hbmpChecked_);
		hbmpChecked_ = NULL;
	}
	
	if(hbmpUnchecked_ != NULL) {
		::DeleteObject(hbmpUnchecked_);
		hbmpUnchecked_ = NULL;
	}

	if(popupMenu_) {
		DestroyMenu(popupMenu_);
		popupMenu_ = NULL;
	}

	while(subButtons_.size() != 0) {
		OEPluginToolbarBtn *button = subButtons_.front();
		delete button;
	}

	if(toolbar_) {
		if(parentButId_ == -1) {
			toolbar_->RemoveButton(this);
		}
		else {
			OEPluginToolbarBtn *parBtn = toolbar_->GetButton(parentButId_);
			if(parBtn) {
				parBtn->RemoveButton(this);
				parBtn->Unlock();
			}
		}
	}

	Unlock();

	DeleteCriticalSection(&critSection_);
}

//-----------------------------------------------------------//
VOID OEPluginToolbarBtn::Remove()
{
	if(popupMenu_) {
		DestroyMenu(popupMenu_);
		popupMenu_ = NULL;
	}

	while(subButtons_.size() != 0) {
		OEPluginToolbarBtn *button = subButtons_.front();
		delete button;
	}

	if(toolbar_) {
		if(parentButId_ == -1) {
			toolbar_->RemoveButton(this);
		}
		else {
			OEPluginToolbarBtn *parBtn = toolbar_->GetButton(parentButId_);
			if(parBtn) {
				parBtn->RemoveButton(this);
				parBtn->Unlock();
			}
		}
		toolbar_ = NULL;
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::Lock()
{
	EnterCriticalSection(&critSection_);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::Unlock()
{
	LeaveCriticalSection(&critSection_);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetBMPNormal(LPCTSTR bmpNormal)
{
	type_ = img_file;

	bmpNormal_ = bmpNormal;

	if(hbmpChecked_ != NULL) {
		::DeleteObject(hbmpChecked_);
		hbmpChecked_ = NULL;
	}

	if(created_) {
		UpdateButtonInfo(btn_image_normal);
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetBMPMouseOver(LPCTSTR bmpMouseOver)
{
	type_ = img_file;

	bmpMouseOver_ = bmpMouseOver;

	if(hbmpUnchecked_ != NULL) {
		::DeleteObject(hbmpUnchecked_);
		hbmpUnchecked_ = NULL;
	}

	if(created_) {
		UpdateButtonInfo(btn_image_mouse_over);
	}
}


//------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetPictureNormal(IPicture* pict)
{
	type_ = img_bitmap;

	if(pictNormal_) {
		pictNormal_->Release();
		pictNormal_ = NULL;
	}

	pictNormal_ = pict;

	if(pictNormal_ != NULL) {
		pictNormal_->AddRef();
	}

	if(hbmpChecked_ != NULL) {
		::DeleteObject(hbmpChecked_);
		hbmpChecked_ = NULL;
	}

	if(created_) {
		UpdateButtonInfo(btn_image_normal);
	}
}

//------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetPictureOver(IPicture* pict) 
{
	if(pictOver_) {
		pictOver_->Release();
		pictOver_ = NULL;
	}

	type_ = img_bitmap;
	pictOver_ = pict;

	if(pictOver_ != NULL) {
		pictOver_->AddRef();
	}

	if(hbmpUnchecked_ != NULL) {
		::DeleteObject(hbmpUnchecked_);
		hbmpUnchecked_ = NULL;
	}

	if(created_) {
		UpdateButtonInfo(btn_image_mouse_over);
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetName(LPCTSTR name)
{
	name_ = name;

	if(created_) {
		UpdateButtonInfo(btn_name);
	}
}

//------------------------------------------------------------------//
LPCTSTR OEPluginToolbarBtn::GetName()
{
	return name_.c_str();
}

//-----------------------------------------------------------//
VOID OEPluginToolbarBtn::SetTooltip(LPCTSTR tooltip)
{
	tooltip_ = tooltip;
	if(created_) {
		HWND hToolTip = toolbar_->GetTooltipsHandle(); 
		if(hToolTip)
		{
			TOOLINFO ti;
			TCHAR tooltip[1024];
			ti.cbSize = sizeof(ti);
			ti.hwnd = toolbar_->GetHandle();
			ti.uId = id_;
#if _MSC_VER >= 1400
			_tcsncpy_s(tooltip, sizeof(tooltip)/sizeof(tooltip[0]), tooltip_.c_str(), _TRUNCATE);
#else
			_tcsncpy(tooltip, tooltip_.c_str(), sizeof(tooltip)/sizeof(TCHAR)-1);
#endif
			ti.lpszText = tooltip;
			::SendMessage(hToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
		}
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetEnabled(BOOL enabled)
{
	enabled_ = enabled;
	if(parentButId_ == -1) {
		UpdateState();
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetToolbar(OEPluginToolbar *toolbar)
{
	Lock();

	toolbar_ = toolbar;

	for(OEPluginToolbarBtnList::iterator it=subButtons_.begin(); it!=subButtons_.end(); it++) {
		OEPluginToolbarBtn *button = *it;

		button->SetToolbar(toolbar);
	}

	Unlock();
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::SetShown(BOOL show, BOOL update)
{
	hidden_ = !show;
	if(parentButId_ == -1) {
		UpdateState();

		if(toolbar_ && update) {
			toolbar_->UpdateBandSize();
		}
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::UpdateState()
{
	if(toolbar_) {
		SendMessage(toolbar_->GetHandle(),
					TB_SETSTATE,
					id_,
					MAKELONG(GetState(), 0));
	}
}

//---------------------------------------------------------------------------------//
DWORD OEPluginToolbarBtn::GetState()
{
	DWORD state = 0;

	if(IsEnabled()) {
		state |= TBSTATE_ENABLED;
	}
	if(IsHidden()) {
		state |= TBSTATE_HIDDEN;
	}
	if(IsCheckStyle() && checked_) {
		state |= TBSTATE_CHECKED;
	}

	return state;
}

//---------------------------------------------------------------------------------//
DWORD OEPluginToolbarBtn::GetStyle()
{
//	DWORD style = TBSTYLE_BUTTON;
	DWORD style = 0;
	
	if(isSeparator_) {
		style = TBSTYLE_SEP;
	}
	else {
		style = BTNS_AUTOSIZE | TBSTYLE_BUTTON;
		if(IsWMail()) {
			if(!name_.empty()) {
				style |= BTNS_SHOWTEXT;
			}
		}
	}

	if(checkStyle_) {
		style |= BTNS_CHECK; // TBSTYLE_CHECK;
	}

	if(!subButtons_.empty()) {
		style |= BTNS_DROPDOWN; // TBSTYLE_DROPDOWN;
	}

	return style;
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn *OEPluginToolbarBtn::GetSubButton(int id, BOOL lock)
{
	for(OEPluginToolbarBtnList::iterator it=subButtons_.begin(); it!=subButtons_.end(); it++) {
		OEPluginToolbarBtn *button = *it;
		if(button->GetID() == id) {
			if(lock) {
				button->Lock();
			}
			return button;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------------//
OEPluginToolbarBtn *OEPluginToolbarBtn::GetSubButtonByIndex(INT index, BOOL lock)
{
	INT i = 0;
	for(OEPluginToolbarBtnList::iterator it=subButtons_.begin(); it!=subButtons_.end(); it++) {
		if(i++ == index) {
			OEPluginToolbarBtn *button = *it;
			if(lock) {
				button->Lock();
			}
			return button;
		}
	}

	return NULL;
}

//-----------------------------------------------------------//
void OEPluginToolbarBtn::SetChecked(BOOL check, BOOL update)
{
	checked_ = check;
	if(update)
		UpdateState();
}

//-----------------------------------------------------------//
BOOL OEPluginToolbarBtn::IsChecked()
{
	return checked_;
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::Create(OEPluginToolbar *toolbar)
{
	if(type_ == img_invalid && !isSeparator_) {
		return;
	}

	SetToolbar(toolbar);

	if(toolbar_->GetHandle() == NULL) {
		return;
	}

	// verify if the button exist. If not, create it again
	if(created_) {
		INT nIndex = (INT)::SendMessage(toolbar_->GetHandle(), TB_COMMANDTOINDEX, id_, 0);
		if(nIndex != -1) {
			UpdateState();
			return;
		}
	}

	created_ = TRUE;

	TBBUTTON tb;
	INT nIndex;
	INT ButtonStrIndex;
	HBITMAP hBMP;
	BOOL error = TRUE;
	HWND hToolTip = toolbar_->GetTooltipsHandle(); 

	if(!isSeparator_) {
		hBMP = NULL;

		if(type_ == img_file && !bmpNormal_.empty()) {
			hBMP = toolbar_->LoadButtonImage(bmpNormal_.c_str());
		}
		else if(type_ == img_bitmap && pictNormal_ != NULL) {
			INT cx, cy;
			toolbar_->GetBitmapSize(&cx, &cy);
			hBMP = CreateHBitmapFromIPicture(pictNormal_, cx, cy);
		}

		if (hBMP != NULL)
		{
			bmpIndexNormal_ = toolbar_->AddImage(hBMP);
			// Delete the BMP Object
			::DeleteObject(hBMP);
			if (bmpIndexNormal_ == -1) {
				debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::Create: bmpIndexNormal_ = -1.\n"));
				return;
			}
		}

		// Note: We can't have a hot image if we don't have a normal image
		hBMP = NULL;
		if(type_ == img_file && !bmpNormal_.empty()) {
			hBMP = toolbar_->LoadButtonImage((!bmpMouseOver_.empty()) ? bmpMouseOver_.c_str() : bmpNormal_.c_str());
		}
		else if(type_ == img_bitmap && pictNormal_ != NULL) {
			INT cx, cy;
			toolbar_->GetBitmapSize(&cx, &cy);
			hBMP = CreateHBitmapFromIPicture(pictOver_ != NULL ? pictOver_ : pictNormal_, cx, cy);
		}

		if (hBMP != NULL) {
			toolbar_->AddHotImage(hBMP);
			::DeleteObject(hBMP);
		}

		//
		// Add the buttons to OE Toolbar
		//
		SendMessage(toolbar_->GetHandle(), TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0);

		// Set the string name for this button
		ButtonStrIndex = (INT)::SendMessage(toolbar_->GetHandle(), 
									TB_ADDSTRING, 
									0, 
									(LPARAM)name_.c_str());

		// Fill the TBBUTTON structure and send it to the Toolbar
		tb.fsState = (BYTE)GetState();
		tb.fsStyle = (BYTE)GetStyle();
		tb.iBitmap = (type_ == img_bitmap && pictNormal_ == NULL) ? I_IMAGENONE : bmpIndexNormal_;
		tb.idCommand = id_;
		tb.dwData = 0;
		tb.iString = ButtonStrIndex;

		nIndex = (INT)::SendMessage(toolbar_->GetHandle(),
							TB_COMMANDTOINDEX,
							id_,
							0);

		if(nIndex < 0) {
			::SendMessage(toolbar_->GetHandle(), TB_ADDBUTTONS, 1, (LPARAM) &tb);
			error = FALSE;

			if(hToolTip && !tooltip_.empty())
			{
				TCHAR tooltip[1024];
				TOOLINFO ti;
				ti.cbSize = sizeof(ti);
				ti.hwnd = toolbar_->GetHandle();
				ti.uId = id_;
				::SendMessage(hToolTip, TTM_GETTOOLINFO, 0, (LPARAM)&ti);

#if _MSC_VER >= 1400
				_tcscpy_s(tooltip, sizeof(tooltip)/sizeof(tooltip[0]), tooltip_.c_str());
#else
				_tcsncpy(tooltip, tooltip_.c_str(), sizeof(tooltip)/sizeof(TCHAR)-1);
#endif
				ti.lpszText = tooltip;
				::SendMessage(hToolTip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);
			}
		}
	}
	else { // separator
		// Fill the TBBUTTON structure and send it to the Toolbar
		tb.fsState = (BYTE)GetState();
		tb.fsStyle = (BYTE)GetStyle();
		tb.iBitmap = -1;
		tb.idCommand = id_;
		tb.dwData = 0;
		tb.iString = -1;

		nIndex = (INT)::SendMessage(toolbar_->GetHandle(),
							TB_COMMANDTOINDEX,
							id_,
							0);

		if(nIndex < 0) {
			::SendMessage(toolbar_->GetHandle(), TB_ADDBUTTONS, 1, (LPARAM) &tb);
			error = FALSE;
		}
	}

	// HACK: adjust button size so they look as before. After adding a button the
	// button size changes and need some adjustment. This magic numbers are the
	// result of some tries.
	if(!error && toolbar_->IsOEDefaultToolbar() &&
		(toolbar_->GetWndStyle() & MsgWndStyles::OE_ALL_MSG_WND)) { // && !IsWMail()) {
		INT maxWidth = 0x31;

		// the idea was to detect when a button with sub buttons was present but the magic
		// number changes so it cannot be done in this way. This issue appeared when a 
		// button with sub buttons is present in OE toolbar in a msg wnd. In this situation
		// the buttons weren't shown.
		// the problem is that this magic number should be different when there is a button
		// with sub buttons but I cannot detect it easily so I did only one magic number.
//		if(LOWORD(dwSize) < 0x3a) {
//			maxWidth += 6;
//		}
//
		// Don't send TB_AUTOSIZE if there's no text rows
		LONG rows = (LONG)SendMessage(toolbar->GetHandle(), TB_GETTEXTROWS, 0,0);
		if(rows>0)
		{
			SendMessage(toolbar->GetHandle(), TB_SETBUTTONWIDTH, 0, MAKELONG(24, maxWidth));
			SendMessage(toolbar->GetHandle(), TB_AUTOSIZE, 0, 0);
		}
	}
	
	UpdateState();
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::Reset()
{
	Lock();

	if(pictNormal_ != NULL)	{
		pictNormal_->Release();
		pictNormal_ = NULL;
	}
	
	if(pictOver_ != NULL) {
		pictOver_->Release();
		pictOver_ = NULL;
	}

	if(hbmpChecked_ != NULL)	{
		::DeleteObject(hbmpChecked_);
		hbmpChecked_ = NULL;
	}
	
	if(hbmpUnchecked_ != NULL) {
		::DeleteObject(hbmpUnchecked_);
		hbmpUnchecked_ = NULL;
	}

	if(popupMenu_) {
		DestroyMenu(popupMenu_);
		popupMenu_ = NULL;
	}

	OEPluginToolbarBtnList::iterator it = subButtons_.begin();
	for(; it != subButtons_.end(); it++) {
		OEPluginToolbarBtn* button = *it;
		button->Reset();
	}

	Unlock();
}


//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::UpdateButtonInfo(int props)
{
	HBITMAP hBMP = NULL;
	HBITMAP hMask;
	TBBUTTONINFO btnInfo;

	if(!created_) {
		return;
	}

	if(props & btn_image_normal) {
		hBMP = NULL;

		if(type_ == img_file && !bmpNormal_.empty()) {
			hBMP = toolbar_->LoadButtonImage(bmpNormal_.c_str());
		}
		else if(type_ == img_bitmap && pictNormal_ != NULL) {
			hBMP = CreateHBitmapFromIPicture(pictNormal_, 16, 16);
		}

		if (hBMP != NULL) {			
			if(bmpIndexNormal_ != -1) { // replace
				hMask = CreateBitmapMask(hBMP, toolbar_->GetMaskColor());  //TRANSPARENT_COLOR);
				if(!toolbar_->ReplaceImage(bmpIndexNormal_, hBMP, hMask)) {
					debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't replace image.\n"));
					::DeleteObject(hMask);
					return;
				}
				::DeleteObject(hMask);
			}
			else { // add new
				bmpIndexNormal_ = toolbar_->AddImage(hBMP);
				if(bmpIndexNormal_ >= 0) {
					toolbar_->AddHotImage(hBMP);
					btnInfo.cbSize = sizeof(TBBUTTONINFO);
					btnInfo.dwMask = TBIF_IMAGE;
					btnInfo.iImage = bmpIndexNormal_;
					if (::SendMessage(toolbar_->GetHandle(), TB_SETBUTTONINFO, id_, (LPARAM) &btnInfo) == 0) {
						debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't update button image info.\n"));
						::DeleteObject(hBMP);
						return;
					}
				}
				else {
					debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't add new image.\n"));
				}
			}
			::DeleteObject(hBMP);
		}
		else if(type_ == img_bitmap && pictNormal_ == NULL) {
			// Remove image
			btnInfo.cbSize = sizeof(TBBUTTONINFO);
			btnInfo.dwMask = TBIF_IMAGE;
			btnInfo.iImage = I_IMAGENONE;
			if (::SendMessage(toolbar_->GetHandle(), TB_SETBUTTONINFO, id_, (LPARAM) &btnInfo) == 0) {
				debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't remove image info.\n"));
				return;
			}
		}

		// if normal and mouse over image are the same force the mouse over change to update it
		if(bmpMouseOver_.empty() || pictOver_ == NULL) {
			props |= btn_image_mouse_over;
		}
	}

	if(props & btn_image_mouse_over) {
		hBMP = NULL;
		if(type_ == img_file && !bmpNormal_.empty()) {
			hBMP = toolbar_->LoadButtonImage((!bmpMouseOver_.empty()) ? bmpMouseOver_.c_str() : bmpNormal_.c_str());
		}
		else if(type_ == img_bitmap && pictNormal_ != NULL) {
			hBMP = CreateHBitmapFromIPicture( pictOver_ != NULL ? pictOver_ : pictNormal_, 16, 16);
		}

		if (hBMP != NULL) {
			if(bmpIndexNormal_ != -1) {
				hMask = CreateBitmapMask(hBMP, toolbar_->GetMaskColor());
				if(!toolbar_->ReplaceHotImage(bmpIndexNormal_, hBMP, hMask)) {
					debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't replace hot image.\n"));
					::DeleteObject(hMask);
					return;
				}
				::DeleteObject(hMask);
			}
			else {
				bmpIndexNormal_ = toolbar_->AddHotImage(hBMP);
				if(bmpIndexNormal_ >= 0) {
					toolbar_->AddImage(hBMP);
					btnInfo.cbSize = sizeof(TBBUTTONINFO);
					btnInfo.dwMask = TBIF_IMAGE;
					btnInfo.iImage = bmpIndexNormal_;
					if (::SendMessage(toolbar_->GetHandle(), TB_SETBUTTONINFO, id_, (LPARAM) &btnInfo) == 0) {
						debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't update hot image info.\n"));
						::DeleteObject(hBMP);
						return;
					}
				}
				else {
					debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't add new image.\n"));
				}
			}
			::DeleteObject(hBMP);
		}
		else if(type_ == img_bitmap && pictNormal_ == NULL) {
			// Remove image
			btnInfo.cbSize = sizeof(TBBUTTONINFO);
			btnInfo.dwMask = TBIF_IMAGE;
			btnInfo.iImage = I_IMAGENONE;
			if (::SendMessage(toolbar_->GetHandle(), TB_SETBUTTONINFO, id_, (LPARAM) &btnInfo) == 0) {
				debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't remove hot image.\n"));
				return;
			}
		}
	}
	if(props & btn_name) {
		TCHAR name[256];
#if _MSC_VER >= 1400
		_tcscpy_s(name, sizeof(name)/sizeof(name[0]), name_.c_str());
#else
		_tcsncpy(name, name_.c_str(), sizeof(name)/sizeof(TCHAR)-1);
#endif
		btnInfo.cbSize = sizeof(TBBUTTONINFO);
		btnInfo.dwMask = TBIF_TEXT;
		btnInfo.pszText = name;

		// Update ImagesList
		if (::SendMessage(toolbar_->GetHandle(), TB_SETBUTTONINFO, id_, (LPARAM) &btnInfo) == 0) {
			debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::UpdateButtonInfo: Can't update text button info.\n"));
			return;
		}
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::CreateInMenu(HMENU menu)
{
	if(IsHidden()) {
		return;
	}

	TCHAR szText[256];

	MENUITEMINFO mInfo;
	ZeroMemory(&mInfo, sizeof(MENUITEMINFO));

	if(!IsSeparator()) {
		LoadMenuBitmaps();

		//MENUITEMINFO mInfo;
		mInfo.cbSize = sizeof(MENUITEMINFO);
		mInfo.fMask = MIIM_ID | MIIM_STATE | MIIM_TYPE;
		mInfo.wID = id_;
		mInfo.fState = (IsEnabled() ? MFS_ENABLED : MFS_DISABLED);
		if(IsCheckStyle() && IsChecked()) {
			mInfo.fState |= MFS_CHECKED;
		}
		mInfo.fType = MFT_STRING;
#if _MSC_VER >= 1400
		_tcscpy_s(szText, sizeof(szText)/sizeof(szText[0]), name_.c_str());
#else
		_tcsncpy(szText, name_.c_str(), sizeof(szText)/sizeof(TCHAR)-1);
#endif
		mInfo.dwTypeData = szText;
		mInfo.cch = (INT)_tcslen(szText);

		if(IsCheckStyle()) {
			mInfo.fMask |= MIIM_CHECKMARKS;
			mInfo.hbmpChecked = NULL;
			mInfo.hbmpUnchecked = NULL;
		}

		if((hbmpChecked_ != NULL || hbmpUnchecked_ != NULL)) {
			mInfo.fMask |= MIIM_CHECKMARKS;
			mInfo.hbmpChecked = hbmpChecked_;
			mInfo.hbmpUnchecked = hbmpUnchecked_;
		}
	}
	else {
		// Separator
		mInfo.cbSize = sizeof(MENUITEMINFO);
		mInfo.fMask = MIIM_ID | MIIM_TYPE;
		mInfo.wID = id_;
		// mInfo.fState = MFS_DEFAULT;
		mInfo.fType = MFT_SEPARATOR;
	}

	if(::InsertMenuItem(menu, id_, FALSE, &mInfo) == 0) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::CreateInMenu: Can't add menuitem %d\n"), ::GetLastError());
	};
}

//------------------------------------------------------------------//
VOID OEPluginToolbarBtn::LoadMenuBitmaps()
{
	if(hbmpChecked_ != NULL && hbmpUnchecked_ != NULL) {
		return;
	}

	if(hbmpChecked_ != NULL) {
		::DeleteObject(hbmpChecked_);
		hbmpChecked_ = NULL;
	}

	if(hbmpUnchecked_ != NULL) {
		::DeleteObject(hbmpUnchecked_);
		hbmpUnchecked_ = NULL;
	}

	SIZE sz = { ::GetSystemMetrics(SM_CXMENUCHECK), ::GetSystemMetrics(SM_CYMENUCHECK) };

	UINT flags = (toolbar_->GetMaskColor() != TRANSPARENT_COLOR) ? 
		LR_CREATEDIBSECTION | LR_LOADFROMFILE :								// new method
		LR_LOADFROMFILE | LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS;			// old method

	HBITMAP hbm = NULL;

	if(type_ == img_file && !bmpNormal_.empty()) {
		hbm = LoadImageFromFile(bmpNormal_.c_str(), flags, sz.cx, sz.cy);
	}
	if(type_ == img_bitmap && pictNormal_ != NULL) {
		hbm = CreateHBitmapFromIPicture(pictNormal_, sz.cx, sz.cy);
	}

	if(hbm == NULL) {
		return;
	}

	hbmpChecked_ = CreateMenuBitmap(hbm, sz.cx, sz.cy);
	::DeleteObject(hbm);

	hbm = NULL;

	if(type_ == img_file && !bmpNormal_.empty()) {
		hbm = LoadImageFromFile(!bmpMouseOver_.empty() ? bmpMouseOver_.c_str() : bmpNormal_.c_str(), flags, sz.cx, sz.cy);
	}
	if(type_ == img_bitmap && pictNormal_ != NULL) {
		hbm = CreateHBitmapFromIPicture((pictOver_ ? pictOver_ : pictNormal_), sz.cx, sz.cy);
	}

	hbmpUnchecked_ = CreateMenuBitmap(hbm, sz.cx, sz.cy);
	::DeleteObject(hbm);
}


//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::AddButton(OEPluginToolbarBtn *button)
{
	button->SetOEAPIID(oeapiId_);
	button->SetToolbarID(toolbarId_);
	button->SetParentButtonID(id_);
	button->SetToolbar(toolbar_);
	subButtons_.push_back(button);

	// only for the first sub button: change the style to show the arrow
	if(toolbar_ && toolbar_->GetHandle() && subButtons_.size() == 1) {
		TBBUTTONINFO tb;

		tb.cbSize = sizeof(TBBUTTONINFO);
		tb.dwMask = TBIF_STYLE;
		tb.fsStyle = (BYTE)GetStyle();

		SendMessage(toolbar_->GetHandle(), 
					TB_SETBUTTONINFO, 
					(WPARAM) id_, 
					(LPARAM) &tb);

		toolbar_->UpdateBandSize(this);
	}
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::RemoveButton(OEPluginToolbarBtn *button)
{
	subButtons_.remove(button);
}

//---------------------------------------------------------------------------------//
VOID OEPluginToolbarBtn::CreateSubButtons()
{
	RECT rc;
	POINT p;

	if(popupMenu_) {
		DestroyMenu(popupMenu_);
		popupMenu_ = NULL;
	}

	popupMenu_ = CreatePopupMenu();
	if(popupMenu_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEPluginToolbarBtn::CreateSubButtons: Can't create popup menu\n"));
		return;
	}

	for(OEPluginToolbarBtnList::iterator it=subButtons_.begin(); it!=subButtons_.end(); it++) {
		OEPluginToolbarBtn *btn = *it;
		btn->CreateInMenu(popupMenu_);
	}

	SendMessage(toolbar_->GetHandle(), TB_GETRECT,
				(WPARAM) id_, (LPARAM) &rc);

	p.x = rc.left;
	p.y = rc.bottom;

	ClientToScreen(toolbar_->GetHandle(), &p);

	TrackPopupMenu(popupMenu_, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
					p.x, p.y, 0, 
					toolbar_->GetManager()->GetRebarHandle(), &rc);
//					toolbar_->GetManager()->GetMainWindow(), &rc);

}

//-----------------------------------------------------------//
BOOL OEPluginToolbarBtn::NotifyButtonClick(HWND msgWndId, INT id)
{
	BOOL ret = FALSE;
	INT param = id;

	// Update state only for main window
	if(msgWndId == INVALID_MSGWND_ID && checkStyle_) {
		// Flip checked state
		// don't update
		SetChecked(!checked_, FALSE);
	}

	// popup style expand the sub buttons on click.
	if(msgWndId == INVALID_MSGWND_ID && GetSubButtonCount() > 0 && IsPopupStyle())  {
		CreateSubButtons();
		ret = TRUE;
	}
	else {
		OEAPI_ButtonCallBack cb = GetCallback();
		OEPluginToolbar *toolbar = GetToolbar();
		if(toolbar) {
			param = toolbar->GetID();
		}
		
		// keep the value of msgWndId if the toolbar is not in the OE main wnd
		//if((HWND)msgWndId == Plugin.OEMainWindow) {
		//	msgWndId = -1;
		//}

		if(cb) {
			(*cb)((INT) param, id, msgWndId);
			ret = TRUE;
		}
	}

	return ret;
}

//-----------------------------------------------------------//
BOOL OEPluginToolbarBtn::NotifyMsgButtonClick(HWND msgWndId, INT id)
{
	BOOL ret = FALSE;
	INT param = id;

	// ReadState
	if(msgWndId != INVALID_MSGWND_ID && checkStyle_) {
		// don't update state
		SetChecked(!checked_, FALSE);
	}

	// pop up style expand the sub buttons on click.
	if(msgWndId != INVALID_MSGWND_ID && GetSubButtonCount() > 0 && IsPopupStyle())  {
		CreateSubButtons();
		ret = TRUE;
	}
	//else {
	//	OEAPI_ButtonCallBack cb = GetCallback();
	//	OEPluginToolbar *toolbar = GetToolbar();
	//	if(toolbar) {
	//		param = toolbar->GetID();
	//	}
	//	
	//	// keep the value of msgWndId if the toolbar is not in the OE main wnd
	//	//if((HWND)msgWndId == Plugin.OEMainWindow) {
	//	//	msgWndId = -1;
	//	//}
	//
	//	if(cb) {
	//		(*cb)((INT) param, id, msgWndId);
	//		ret = TRUE;
	//	}
	//}

	return ret;
}


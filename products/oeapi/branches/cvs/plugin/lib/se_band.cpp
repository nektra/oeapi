#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>
#include <tchar.h>

#include <strstrea.h>

#include "se_band.h"
#include "se_plugin.h"
#include "se_debug.h"
#include "se_button.h"


extern OEPlugin Plugin ;

extern HINSTANCE g_hInst;


VOID Band_Refresh(VOID)
{
	UINT BandIndex ;
	REBARBANDINFO rbBand ;

	dbgprint("Band_Refresh\n") ;

	if (Band_GetBandIndex(&BandIndex) == -1)
	{
		dbgprint("Band_GetBandIndex failed.\n") ;
		return ; 
	}

	rbBand.cbSize = sizeof(REBARBANDINFO);
	
	rbBand.fMask  = RBBIM_TEXT  | RBBIM_STYLE |  
					RBBIM_SIZE ;
						
	SendMessage(Plugin.OE6ReBar, RB_GETBANDINFO, 0, (LPARAM)&rbBand);

	rbBand.fStyle = RBBS_CHILDEDGE | RBBS_USECHEVRON | RBBS_BREAK ;

	rbBand.lpText = _T("");
	rbBand.cxMinChild = 0;
	rbBand.cx = 250 ;

	SendMessage(Plugin.OE6ReBar, RB_SETBANDINFO,
				(WPARAM) BandIndex, (LPARAM) &rbBand) ;

	Band_VerifyButtonPositions(BandIndex) ;
}

VOID Band_DestroyChevronMenu(VOID)
{
	if (Plugin.TrackPopupMenu != NULL)
		DestroyMenu(Plugin.TrackPopupMenu) ;

	// Should i delete the BMP checked/unchecked objects ?

#if 0
	UINT i ;
	INT r = 1 ;
	MENUITEMINFO mInfo ;
	UINT tButtons ;

	dbgprint("Band_DestroyChevronMenu\n") ;

	if (Plugin.TrackPopupMenu == NULL)
		return ;

	tButtons = SendMessage(Plugin.OE6Toolbar, TB_BUTTONCOUNT, 0, 0) ;

	for (i = 0 ; i < tButtons ; i ++)
	{
		TBBUTTON tbi ;

		SendMessage(Plugin.OE6Toolbar, TB_GETBUTTON,
					(WPARAM) i, (LPARAM) &tbi) ;

		mInfo.cbSize = sizeof(MENUITEMINFO) ;
		mInfo.fMask = MIIM_CHECKMARKS |MIIM_ID;
		mInfo.wID = tbi.idCommand ;
		mInfo.hbmpChecked = NULL ;
		mInfo.hbmpUnchecked = NULL ;

		r = GetMenuItemInfo(Plugin.TrackPopupMenu, 
						TRUE, 
						i,
						&mInfo) ;
		if (r == 0) // no more items in the menu
		{
			dbgprint("error\n") ;
			break ;
		}

		dbgprint("item!\n") ;

		if (mInfo.hbmpChecked != NULL)
		{
			dbgprint("Deleting checked object hbmp\n") ;
			DeleteObject(mInfo.hbmpChecked) ;
		}
		
		if (mInfo.hbmpUnchecked != NULL)
		{
			dbgprint("Deleting hbmpUnchecled object\n") ;
			DeleteObject(mInfo.hbmpUnchecked) ;
		}
	}

	DestroyMenu(Plugin.TrackPopupMenu) ;
	Plugin.TrackPopupMenu = NULL ;
#endif
}

VOID Band_ShowChevronMenu(VOID)
{
	UINT BandIndex;
	RECT rc;
	POINT p;


	Band_GetBandIndex(&BandIndex) ;

	GetWindowRect(Plugin.OE6ReBar, &rc);

	SendMessage(Plugin.OE6ReBar, RB_GETRECT,
				(WPARAM) BandIndex, (LPARAM) &rc);

	p.x = rc.right;
	p.y = rc.bottom;

	ClientToScreen(Plugin.OE6ReBar, &p);

	TrackPopupMenu(Plugin.TrackPopupMenu, TPM_RIGHTALIGN|TPM_TOPALIGN,
					p.x, p.y, 0, 
					Plugin.OE6ReBar, 0);


#if 0
	char s[1024];

	memset(s, 0, 1024) ;
	sprintf(s, "left=%d top=%d right=%d bottom=%d\n", rc.left, rc.top, rc.right, rc.bottom);

	p.x = rc.right;
	p.y = rc.bottom;

	ClientToScreen(Plugin.OE6ReBar, &p);

	sprintf(s, "%sp.x=%d p.y=%d\n", s, p.x, p.y);

	MessageBoxA(NULL, s, "MSG", MB_OK);
//	OutputDebugStringA(s);

#endif


/*	TrackPopupMenu(Plugin.TrackPopupMenu, TPM_RIGHTALIGN|TPM_TOPALIGN,
					rc.right, rc.bottom+BAND_Y_POS, 0, 
					Plugin.OE6ReBar, 0);*/
}

VOID Band_AddButtonToChevron(UINT ButtonId)
{
	TCHAR szBMP[256];
	TCHAR szText[256];
	TCHAR szText2[256];
	TCHAR BMPLocation[256];
//	CHAR szBMP[256];
//	CHAR szText[256] ;
//	CHAR szText2[256] ;
	HBITMAP hBMPchecked, hBMPUnchecked ;
//	CHAR BMPLocation[256] ;
	MENUITEMINFO mInfo ;
	WORD wType;
	WORD wNormal;
	WORD wMouseOver;

	dbgprint("Band_AddButtonToChevron\n") ;

	memset(szText, 0, sizeof(szText)) ;
	Button_GetName(ButtonId, (PTCHAR) &szText);


	wType = Button_GetType(ButtonId, &wNormal, &wMouseOver);

	switch(wType)
	{
		case 0: // Load from File (Old)
		{

			// Load the corresponding Bitmaps for
			// CHECKED/UNCHECKED states

			memset(BMPLocation, 0, sizeof(BMPLocation)) ;
			Button_GetBMPLocation(ButtonId, (PTCHAR) &BMPLocation) ;

			// SW :: February 8 2004
			_tcscpy(szBMP, BMPLocation) ;
			_tcscat(szBMP, _T("_normal.bmp"));
			hBMPchecked = (HBITMAP) LoadImage(g_hInst,
		//							BMPLocation,
				  					szBMP, 
									IMAGE_BITMAP,
									15,
									15,
									LR_DEFAULTCOLOR|LR_LOADFROMFILE|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS) ;
//									LR_DEFAULTCOLOR|LR_LOADFROMFILE|LR_LOADTRANSPARENT) ;

			if (hBMPchecked == NULL)
			{
				dbgprint("track popup hBMP = NULL\n");
				return ;
			}

			// SW :: February 8 2004
			_tcscpy(szBMP, BMPLocation) ;
			_tcscat(szBMP, _T("_mouseover.bmp"));
			hBMPUnchecked = (HBITMAP) LoadImage(g_hInst, 
		//				  			BMPLocation,
									szBMP,
									IMAGE_BITMAP,
									15,
									15,
									LR_DEFAULTCOLOR|LR_LOADFROMFILE|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS) ;
//									LR_DEFAULTCOLOR|LR_LOADFROMFILE|LR_LOADTRANSPARENT) ;

			if (hBMPUnchecked == NULL)
			{
				dbgprint("track popup hBMP = NULL\n");
				return ;
			}

			mInfo.cbSize = sizeof(MENUITEMINFO) ;
			mInfo.fMask = MIIM_ID |MIIM_STATE|MIIM_TYPE;
			mInfo.wID = ButtonId ;
			mInfo.fState = MFS_ENABLED ;
			mInfo.fType = MFT_STRING/*|MFT_RADIOCHECK */;

			memset(szText2, 0, sizeof(szText2)) ;
			_stprintf(szText2, _T("   %s"), szText) ;
			_stprintf(szText, _T("%s"), szText) ;
			mInfo.dwTypeData = (LPTSTR) &szText2 ;
			mInfo.cch = _tcslen(szText2) ;

			InsertMenuItem(	Plugin.TrackPopupMenu, 
							ButtonId, 
							FALSE, 
							(MENUITEMINFO *) &mInfo) ;

			mInfo.hbmpChecked = hBMPchecked;
			mInfo.hbmpUnchecked = hBMPUnchecked;
			mInfo.fMask = MIIM_CHECKMARKS ;

			break;
		}

		case 1:
		{
			// Load the corresponding Bitmaps for
			// CHECKED/UNCHECKED states

			memset(BMPLocation, 0, sizeof(BMPLocation)) ;
			Button_GetBMPLocation(ButtonId, (PTCHAR) &BMPLocation) ;

			// SW :: February 8 2004
			_tcscpy(szBMP, BMPLocation) ;
			_tcscat(szBMP, _T("_normal.bmp"));
			hBMPchecked = (HBITMAP) LoadImage(g_hInst,
		//							BMPLocation,
		//		  					szBMP, 
									MAKEINTRESOURCE(wNormal),
									IMAGE_BITMAP,
									15,
									15,
									LR_DEFAULTCOLOR|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS) ;
//									LR_DEFAULTCOLOR|LR_LOADTRANSPARENT) ;

			if (hBMPchecked == NULL)
			{
				dbgprint("track popup hBMP = NULL\n");
				return ;
			}

			// SW :: February 8 2004
			_tcscpy(szBMP, BMPLocation) ;
			_tcscat(szBMP, _T("_mouseover.bmp"));
			hBMPUnchecked = (HBITMAP) LoadImage(g_hInst, 
		//				  			BMPLocation,
		//							szBMP,
									MAKEINTRESOURCE(wMouseOver),
									IMAGE_BITMAP,
									15,
									15,
									LR_DEFAULTCOLOR|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS) ;
//									LR_DEFAULTCOLOR|LR_LOADTRANSPARENT) ;

			if (hBMPUnchecked == NULL)
			{
				dbgprint("track popup hBMP = NULL\n");
				return ;
			}

			mInfo.cbSize = sizeof(MENUITEMINFO) ;
			mInfo.fMask = MIIM_ID |MIIM_STATE|MIIM_TYPE;
			mInfo.wID = ButtonId ;
			mInfo.fState = MFS_ENABLED ;
			mInfo.fType = MFT_STRING/*|MFT_RADIOCHECK */;

			memset(szText2, 0, sizeof(szText2)) ;
			_stprintf(szText2, _T("   %s"), szText) ;
			_stprintf(szText, _T("%s"), szText) ;
			mInfo.dwTypeData = (LPTSTR) &szText2 ;
			mInfo.cch = _tcslen(szText2) ;

			InsertMenuItem(	Plugin.TrackPopupMenu, 
							ButtonId, 
							FALSE, 
							(MENUITEMINFO *) &mInfo) ;

			mInfo.hbmpChecked = hBMPchecked;
			mInfo.hbmpUnchecked = hBMPUnchecked;
			mInfo.fMask = MIIM_CHECKMARKS ;

			break;
		}
	}

	SetMenuItemInfo(Plugin.TrackPopupMenu, ButtonId, 
					FALSE, (MENUITEMINFO *) &mInfo) ;
}

// Band Index may change when the user drags it and place it
// before an existing one

INT Band_GetBandIndex(PUINT BandIndex)
{
	UINT i ;

	if(Plugin.OE6ReBar && Plugin.OE6Toolbar)
	{
		i = SendMessage(Plugin.OE6ReBar, RB_GETBANDCOUNT, 0, 0) ;

// SW 25/3/2004
//		for ( ; i >= 0 ; i --)
		for ( ; i > 0 ; i --)
		{
			REBARBANDINFO binfo ;

			memset(&binfo, 0, sizeof(REBARBANDINFO));
			binfo.cbSize = sizeof(REBARBANDINFO);
			binfo.fMask = RBBIM_CHILD;
							
			SendMessage(Plugin.OE6ReBar, RB_GETBANDINFO, 
				(WPARAM) i, (LPARAM) &binfo);
		
			if (binfo.hwndChild == Plugin.OE6Toolbar)
			{
				*BandIndex = i ; 
				return 0 ;
			}
		}
	}

	return -1 ;
}

VOID Band_VerifyButtonPositions(UINT BandIndex)
{
	RECT BandRC ;
	RECT DstRC ;
	RECT ButtonRC ;
	INT ButtonCount ;
	INT i ;
	POINT p0;
	BOOL first = true;

//	char s[1024];
//	char s2[1024];
	strstream coord;

	dbgprint("Band_VerifyButtonPositions\n") ;

	SendMessage(Plugin.OE6ReBar, RB_GETRECT,
			(WPARAM) BandIndex, (LPARAM) &BandRC);

	// Moving it to an absolute value
	BandRC.bottom = BandRC.bottom - BandRC.top;
	BandRC.top = 0;

	BandRC.right = BandRC.right - BandRC.left;
	BandRC.left = 0;

#if 0
	memset(s, 0, 1024) ;
	sprintf(s, "bandrc %d %d %d %d\n", BandRC.left, BandRC.right, BandRC.top, BandRC.bottom) ;
	dbgprint((s)) ;
#endif

	ButtonCount = SendMessage(Plugin.OE6Toolbar, TB_BUTTONCOUNT, 0, 0) ;

	for (i = 0 ; i < ButtonCount ; i++)
	{
		TBBUTTON tb ;
				
		SendMessage(Plugin.OE6Toolbar, TB_GETBUTTON,
					(WPARAM) i, (LPARAM) &tb) ;

		SendMessage(Plugin.OE6Toolbar, TB_HIDEBUTTON,
			(WPARAM) tb.idCommand, (LPARAM) MAKELONG(FALSE, 0)) ;

		SendMessage(Plugin.OE6Toolbar, TB_AUTOSIZE, 0, 0) ;
	}

	// Refresh the menu handle
	
	Band_DestroyChevronMenu() ;
		
	Plugin.TrackPopupMenu = CreatePopupMenu() ;
	if (Plugin.TrackPopupMenu == NULL)
		return ;

	SendMessage(Plugin.OE6Toolbar, TB_AUTOSIZE, 0, 0) ;

	// Remove the buttons that are not visible

	for (i = 0 ; i < ButtonCount ; i++)
	{
		POINT Point ;
		DWORD dwBtnSize ;

		if (SendMessage(Plugin.OE6Toolbar, TB_GETITEMRECT,
					(WPARAM) i, (LPARAM) &ButtonRC) == FALSE)
		{
			dbgprint("TB_GETITEMRECT failed\n") ;
		}

		Point.x = ButtonRC.left ;
		Point.y = ButtonRC.top ;

#if 0
		strstream po;

		po << "Point.x = " << Point.x << endl;
		po << "Point.y = " << Point.y << endl;
		po << ends;

//		dbgprint(po.str());

		MessageBox(NULL, po.str(), "MSG", MB_OK);

#endif

//		ClientToScreen(Plugin.OE6Toolbar, &Point);

#if 0
		dbgprint("------");
//		strstream po2;
//		po2.clear();
		po.clear();

		po << "Point.x = " << Point.x << endl;
		po << "Point.y = " << Point.y << endl;
		po << ends;

//		dbgprint(po.str());

		MessageBox(NULL, po.str(), "MSG", MB_OK);

#endif
		if(first) {
			p0 = Point;
//			BandRC.left += p0.x;
//			BandRC.right += p0.x;
//			BandRC.top += p0.y;
//			BandRC.bottom += p0.y;
			first = false;
		}


// SW 24/3/2004
		// Don't forget...
//		ClientToScreen(
	
		dwBtnSize = SendMessage(Plugin.OE6Toolbar, TB_GETBUTTONSIZE, 0, 0) ;

		ButtonRC.right = Point.x + LOWORD(dwBtnSize) ;
		ButtonRC.bottom = Point.y + HIWORD(dwBtnSize) ;
		ButtonRC.top = Point.y ;
		ButtonRC.left = Point.x ;

#if 0
		memset(s, 0, 1024) ;
		sprintf(s, "ButtonRC %d %d %d %d\n", ButtonRC.left, ButtonRC.right, ButtonRC.top, ButtonRC.bottom) ;
		dbgprint((s)) ;
	
		memset(s, 0, 1024) ;
		sprintf(s, "Point %d %d\n", Point.x, Point.y) ;
		dbgprint((s)) ;
#endif

		// DstRC will have the visible portion of the button

		if (IntersectRect(&DstRC, &ButtonRC, &BandRC) == FALSE)
		{
			dbgprint("Rects do not intersect\n") ;
		}

#if 0
		memset(s, 0, 1024) ;
		sprintf(s, "DstRC %d %d %d %d\n", DstRC.left, DstRC.right, 
			DstRC.top, DstRC.bottom) ;
		dbgprint((s)) ;
#endif

		// XXX: this is not legal, but i dont know why i get
		// the bottom for the band rc different from the bottom
		// for the button rc. It cannot be possible; but as we
		// do not change the height for the band...

		DstRC.bottom = ButtonRC.bottom ;

#if 0
		memset(s, 0, 1024) ;
		sprintf(s, "* DstRC %d %d %d %d\n", DstRC.left, DstRC.right, 
		DstRC.top, DstRC.bottom) ;
		dbgprint((s)) ;
#endif

		// If FALSE, a portion of the button is not visible

// SWAIN Seeing at: 24/3/2004
#if 0
//			strstream coord;

			coord << "BandRC :: Left = " << BandRC.left << " Top = " << BandRC.top <<
				" Right = " << BandRC.right << " Bottom = " << BandRC.bottom << endl;

			coord << "Button :: Left = " << ButtonRC.left << " Top = " << ButtonRC.top <<
				" Right = " << ButtonRC.right << " Bottom = " << ButtonRC.bottom << endl;

			coord << "DstRC :: Left = " << DstRC.left << " Top = " << DstRC.top <<
				" Right = " << DstRC.right << " Bottom = " << DstRC.bottom << endl;

			coord << "Width dwBtnSize = " << LOWORD(dwBtnSize) << " Height dwBtnSize = " << HIWORD(dwBtnSize) << endl;

//			coord << ends;

//			dbgprint(coord.str());

#endif
		if (EqualRect(&ButtonRC, &DstRC) == FALSE)
		{
//			char s[1024] ;
			TBBUTTON tb;

#if 0
			coord << "BUTTON OUTSIDE" << endl;
#endif

#if 0
			memset(s, 0, 1024) ;
			sprintf(s,"btn num = %d %p hide\n", i, Plugin.OE6Toolbar) ;
			dbgprint((s)) ;
#endif
			
			SendMessage(Plugin.OE6Toolbar, TB_GETBUTTON,
						(WPARAM) i, (LPARAM) &tb) ;

			SendMessage(Plugin.OE6Toolbar, TB_HIDEBUTTON,
				(WPARAM) tb.idCommand, (LPARAM) MAKELONG(TRUE, 0)) ;

			Band_AddButtonToChevron(tb.idCommand) ;
		}
	}

	coord << ends;

	dbgprint(coord.str());
}

VOID Band_HideBand(VOID)
{
	UINT BandIndex ;

	Band_GetBandIndex(&BandIndex) ;

	SendMessage(Plugin.OE6ReBar, RB_SHOWBAND, 
		(WPARAM) BandIndex, (LPARAM) FALSE) ;
}

VOID Band_ShowBand(VOID)
{
	UINT BandIndex ;

	Band_GetBandIndex(&BandIndex) ;

	SendMessage(Plugin.OE6ReBar, RB_SHOWBAND, 
		(WPARAM) BandIndex, (LPARAM) TRUE) ;
}
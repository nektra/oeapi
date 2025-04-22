// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Tray.h"
#include "MainFrm.h"
#include "Splash.h"
#include "TrayOptions.h"
#include <afxdisp.h>    // COleDateTime

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
#ifndef _WIN32_WCE
	ON_WM_TIMECHANGE()
#endif
	ON_COMMAND(ID_TRAYCAL_QUIT,    Shutdown)
	ON_MESSAGE(WM_ICON_NOTIFY,     OnTrayNotification)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_DateCheckTimerID = 0;			// Timer ID for periodic date check recieved from "SetTimer"
	m_ShuffleTimerID = 0;			// Timer ID for initial reshuffle
 
	m_bInitialShuffleDone = FALSE;
    m_DateCheckTime = COleDateTime::GetCurrentTime();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	
    cs.style &= ~WS_VISIBLE;

#ifdef _WIN32_WCE
    // Create invisible window
    if (!::IsWindow(m_wndInvisible.m_hWnd))
    {
        LPCTSTR pstrOwnerClass = AfxRegisterWndClass(0);
		if (!m_wndInvisible.CreateEx(0, pstrOwnerClass, _T(""), WS_POPUP,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, 0))
			return FALSE;
    }
    cs.hwndParent = m_wndInvisible.m_hWnd;
#endif

	return CFrameWnd::PreCreateWindow(cs);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef _WIN32_WCE  // No tooltips in CE
	CTime theTime = CTime::GetCurrentTime();
	CString strTime = theTime.Format(_T("%#x"));
#else
    CString strTime = _T("");
#endif

#if defined(EVALUATION_VERSION) || !defined(NDEBUG)
	if (!m_TrayIcon.Create(this, WM_ICON_NOTIFY, strTime, theApp.m_hIcon, IDR_POPUP_MENU))
		return -1;

	StartShuffleTimer();
	StartCheckDateTimer();
#endif // EVALUATION_VERSION

	SetWindowText(theApp.m_strWindowName);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::Shuffle()
{
	m_TrayIcon.MoveToRight();
}

void CMainFrame::KillShuffleTimer()
{
	if (m_ShuffleTimerID)
        KillTimer(ID_SHUFFLE_TIMER);
	m_ShuffleTimerID = 0;
}

void CMainFrame::KillDateCheckTimer()
{
	if (m_DateCheckTimerID) 
        KillTimer(ID_CHECKDATE_TIMER);
	m_DateCheckTimerID = 0;
}

void CMainFrame::StartShuffleTimer()
{
    // Check it's not already going
	if (m_ShuffleTimerID) 
        return;

	m_ShuffleTimerID = SetTimer(ID_SHUFFLE_TIMER, theApp.m_nShuffleDelay*1000, 
							    (TIMERPROC) NULL);
    if (!m_ShuffleTimerID)
		MessageBox(_T("Unable to get timer. Will not auto-reshuffle tray icon"));
}

void CMainFrame::GetDateCheckTime()
{
    // Get the time interval until midnight
    // I just don't trust windows on timers, so I've built in some
    // iterative tolerance.

    COleDateTime time = COleDateTime::GetCurrentTime();
    COleDateTime midnight = COleDateTime(time.GetYear(), time.GetMonth(), time.GetDay(),
                                         0,0,0) + COleDateTimeSpan(1,0,0,0);

    COleDateTimeSpan interval = midnight - time;
    if (interval.GetTotalMinutes() > 15.0)
    {
        TRACE1("Total hours till midnight = %0.0f\n", interval.GetTotalMinutes()/60);
        m_DateCheckTime = midnight - COleDateTimeSpan(0,0,1,0);
    }
    else if (interval.GetTotalMinutes() > 1.0)
    {
        TRACE1("Total minutes till midnight = %0.0f\n", interval.GetTotalMinutes());
        m_DateCheckTime = midnight - COleDateTimeSpan(0,0,0,10);
    }
    else 
        m_DateCheckTime = midnight + COleDateTimeSpan(0,0,0,1);
}

COleDateTimeSpan CMainFrame::GetTimeTillDateCheck()
{
    return m_DateCheckTime - COleDateTime::GetCurrentTime();
}

void CMainFrame::StartCheckDateTimer()
{
    // Check that we should be checking for date change, and that 
    // it's not already going
	if (!theApp.m_bCheckDate || m_DateCheckTimerID) 
        return;

    GetDateCheckTime();
    COleDateTimeSpan delay = GetTimeTillDateCheck();

    DWORD dwNumSeconds = (DWORD) delay.GetTotalSeconds();
    TRACE1("Num seconds till timer tick = %d\n", dwNumSeconds);

	m_DateCheckTimerID = SetTimer(ID_CHECKDATE_TIMER, dwNumSeconds*1000, (TIMERPROC) NULL);
    if (!m_DateCheckTimerID)
		MessageBox(_T("Unable to get timer. Will not check date from now on."));
}

void CMainFrame::Shutdown()
{
	KillShuffleTimer();
	KillDateCheckTimer();

	TRACE0("Shutting down...\n");
	PostMessage( WM_CLOSE, 0, 0 );
}

afx_msg LRESULT CMainFrame::OnPowerBroadcast(UINT /*wParam*/, LONG /*lParam*/)
{
	CheckDate();
	return 0L;
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
    // Uncomment this if you want to add the time that the date will
    // next be checked to the context menu.

    /*
    // Clicking with right button brings up a context menu
    if (LOWORD(lParam) == WM_RBUTTONUP)
    {    
        // Get "Check Date" menu item
        CMenu menu;
        if (!menu.LoadMenu(IDR_POPUP_MENU))
            return 0;
        
        CMenu* pSubMenu = menu.GetSubMenu(0);
        if (!pSubMenu)
            return 0;

        CString strMenuItem;
        pSubMenu->GetMenuString(ID_CHECK_DATE, strMenuItem, MF_BYCOMMAND);

        // Get time till midnight
        COleDateTime time = COleDateTime::GetCurrentTime();
        COleDateTime midnight = COleDateTime(time.GetYear(), time.GetMonth(), time.GetDay(),
                                         0,0,0) + COleDateTimeSpan(1,0,0,0);

        COleDateTimeSpan interval = midnight - time;

        // Add a time at next check to the ID_CHECK_DATE menu item
        CString strAddition;
        int nHour = m_DateCheckTime.GetHour();
        if (nHour <= 12)
            strAddition.Format(_T(" (%d:%02d AM)"), nHour, m_DateCheckTime.GetMinute(),
                               m_DateCheckTime.GetSecond());
        else
            strAddition.Format(_T(" (%d:%02d PM)"), nHour-12, m_DateCheckTime.GetMinute(),
                               m_DateCheckTime.GetSecond());

        strMenuItem += strAddition;

        ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);
        pSubMenu->ModifyMenu(ID_CHECK_DATE, MF_BYCOMMAND|MF_STRING, ID_CHECK_DATE,
                             strMenuItem);

        // Display and track the popup menu
        CPoint pos;
        GetCursorPos(&pos);

        SetForegroundWindow();  
        ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, GetSafeHwnd(), NULL);

        // BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
        PostMessage(WM_NULL, 0, 0);

        menu.DestroyMenu();
        return 1;
    } 
    else
        */
    	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CMainFrame::SetOptions()
{
	COEAPITrayOptions dlg;

	dlg.SetValues(theApp.m_bAutoShuffle, theApp.m_nShuffleDelay, 
				  theApp.m_bCheckDate);

	if (dlg.DoModal() == IDOK) 
    {
		dlg.GetValues(&theApp.m_bAutoShuffle, &theApp.m_nShuffleDelay, 
					  &theApp.m_bCheckDate);

		KillDateCheckTimer();
        StartCheckDateTimer();			
	}
}

void CMainFrame::CheckDate()
{
	TRACE0("Checking date\n");

	HICON hIcon = theApp.CheckDate();
	if (hIcon) 						    // New icon, which means a new day
    {
#ifndef _WIN32_WCE  // No tooltips in CE
	    CTime theTime = CTime::GetCurrentTime();
	    CString strTime = theTime.Format(_T("%#x"));
#else
        CString strTime = _T("");
#endif
        
        m_TrayIcon.SetIcon(hIcon);
		m_TrayIcon.SetTooltipText(strTime);
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{	
	if (nIDEvent == ID_SHUFFLE_TIMER)				// Initial Shuffle timer
    {
		KillShuffleTimer();
		if (theApp.m_bAutoShuffle || !m_bInitialShuffleDone) 
        {
			m_TrayIcon.MoveToRight();				// Shuffle icon
			m_bInitialShuffleDone = TRUE;			// Don't do this again
		}
	} 
    else if (nIDEvent == ID_CHECKDATE_TIMER) 	    // Intermittant Check date timer
    {
		if (theApp.m_bCheckDate)
			CheckDate();							// Check if date has changed

   		KillDateCheckTimer();   // Restart the time with a new delay
        StartCheckDateTimer();
	}
	else 
    {
		CString str;
		str.Format(_T("Unknown timer call (%d)"), nIDEvent);
		MessageBox(str, _T("Error"), MB_OK|MB_ICONWARNING);
	}

	CFrameWnd::OnTimer(nIDEvent);
}

#ifndef _WIN32_WCE
void CMainFrame::OnTimeChange() 
{
	CFrameWnd::OnTimeChange();
	CheckDate();
	KillDateCheckTimer();   // Restart the time with a new delay
    StartCheckDateTimer();
}
#endif

void CMainFrame::OnClose() 
{
    m_TrayIcon.RemoveIcon();
	
	CFrameWnd::OnClose();
}

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWnd
{
// Operations
public:
	CMainFrame();

	DECLARE_DYNCREATE(CMainFrame)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Shutdown();
	void SetOptions();
	void CheckDate();
	BOOL StartIcon(HICON hIcon);

	void Shuffle();
	void KillShuffleTimer();
	void KillDateCheckTimer();
	void StartShuffleTimer();
	void StartCheckDateTimer();
    void GetDateCheckTime();
    COleDateTimeSpan GetTimeTillDateCheck();

// Attributes
protected:
	CSystemTray m_TrayIcon;
	CMenu m_Menu;					// popup menu
	UINT  m_DateCheckTimerID;		// Timer ID for periodic date check recieved from "SetTimer"
	UINT  m_ShuffleTimerID;			// Timer ID for initial reshuffle
	BOOL  m_bInitialShuffleDone;	// Shuffled icon yet?
    COleDateTime m_DateCheckTime;

#ifdef _WIN32_WCE
    CWnd  m_wndInvisible;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
#ifndef _WIN32_WCE
	afx_msg void OnTimeChange();
#endif
	afx_msg LRESULT OnPowerBroadcast(UINT wParam, LONG lParam);
	afx_msg LRESULT OnTrayNotification(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

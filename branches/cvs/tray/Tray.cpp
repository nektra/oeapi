// TrayCalender.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "Tray.h"
#include "MainFrm.h"
#include "Splash.h"

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE g_hSEdll = NULL;
#define dbgprint(a) OutputDebugString(_T(a))

// Define the Section and Keys to use for registry storage
LPCTSTR szSection       = _T("Settings");
LPCTSTR szLocation		= _T("Location");


/////////////////////////////////////////////////////////////////////////////
// COEAPITray

BEGIN_MESSAGE_MAP(COEAPITray, CWinApp)
	//{{AFX_MSG_MAP(COEAPITray)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_STARTHOOK, OnStartHook)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEAPITray construction

COEAPITray::COEAPITray()
{
#ifdef _WIN32_WCE
	m_strWindowName		 = _T("CE NektraTray");      // Window/Application name
#else
	m_strWindowName		 = _T("NektraTray");     // Window/Application name
#endif
	m_bAutoShuffle	     = TRUE;
	m_nShuffleDelay	     = 20;							// Auto-shuffle after 20 secs
	m_bCheckDate	     = TRUE;

	m_bShuffled			 = FALSE;						// Have we auto-shuffled yet?
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COEAPITray object

COEAPITray theApp;

/////////////////////////////////////////////////////////////////////////////
// COEAPITray initialization

BOOL COEAPITray::FirstInstance()
{
	// Determine if another window with our class name exists...
	CWnd *PrevCWnd = CWnd::FindWindow(NULL, m_strWindowName);
	return (PrevCWnd == NULL);
}

void COEAPITray::GetApplicationSettings()
{
}

int COEAPITray::SaveApplicationSettings()
{
	INT len;
	TCHAR fullpath[4096];

	len = GetModuleFileName(NULL, fullpath, sizeof(fullpath)-1);

	if(len == 0) {
		MessageBox(NULL, _T("I don't know my path, please report to Nektra .....@....com"), _T("OEAPI Init Message"), MB_OK);
		return -1;
	} else {
        TCHAR tmppath[_MAX_PATH];
        TCHAR drive[_MAX_DRIVE];
        TCHAR dir[_MAX_DIR];

		_tsplitpath(fullpath, drive, dir, NULL, NULL);
		_tmakepath(tmppath, drive, dir, NULL, NULL);

		WriteProfileString(szSection, szLocation, tmppath);

		return 0;
	}
}


BOOL COEAPITray::InitInstance()
{
	if (!FirstInstance())
		return FALSE;

	SetRegistryKey(_T("Nektra"));

//	if(SaveApplicationSettings() == -1) // Gives the current directory for the dlls.
//		return FALSE;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
//	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	m_StartTime = CTime::GetCurrentTime();

	m_CurrentDay = m_StartTime.GetDay(); // 1-31

#if defined(EVALUATION_VERSION) || !defined(NDEBUG)
	m_hIcon = LoadIcon(IDI_OEAPI);

	if (m_hIcon == NULL)
	{
		AfxMessageBox(_T("Unable to load resource icon."));
		return FALSE;
	}
#endif // EVALUATION_VERSION || !NDEBUG
	
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

    pMainFrame->ModifyStyle(WS_VISIBLE, 0);

	this->OnStartHook();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#ifndef _WIN32_WCE
#include "HyperLink.h"
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA
#ifndef _WIN32_WCE
	CHyperLink m_MailButton;
	CHyperLink m_MailButton2;
#endif

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
    static BOOL m_bVisible;
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BOOL CAboutDlg::m_bVisible = FALSE;

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
#ifndef _WIN32_WCE
	DDX_Control(pDX, IDC_HYPERLINK, m_MailButton);
	DDX_Control(pDX, IDC_HYPERLINK2, m_MailButton2);
#endif
	//{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifndef _WIN32_WCE
	m_MailButton.SetURL(_T("mailto:product@nektra.com?Subject=NektraTray"));
	m_MailButton.SetColours(m_MailButton.GetLinkColour(),m_MailButton.GetLinkColour());
	m_MailButton.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_MYHAND));

	m_MailButton2.SetURL(_T("mailto:oeapi-support@nektra.com?Subject=OEAPISupport"));
	m_MailButton2.SetColours(m_MailButton.GetLinkColour(),m_MailButton.GetLinkColour());
	m_MailButton2.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_MYHAND));
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void COEAPITray::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void COEAPITray::OnStartHook()
{
	if (g_hSEdll != NULL)
		return;

	g_hSEdll = LoadLibrary(_T(".\\oehook.dll"));
	if (g_hSEdll == NULL)
	{
		dbgprint("COEAPITray::OnStartHook: Error loading oehook.dll library\n");
//		MessageBox(NULL, _T("Error"), _T("NektraTray Message"), MB_OK);

		return;
	}
#if 0
//#ifndef NDEBUG
	else {
		HKEY hKey = NULL;

		if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Nektra\\OEAPI\\Settings"), 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
		{
			if(RegCreateKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Nektra\\OEAPI\\Settings"), &hKey) != ERROR_SUCCESS) {
				dbgprint("COEAPITray::OnStartHook: Error RegCreateKey\n");
			}
		}

		if(hKey != NULL)
		{
			TCHAR path[MAX_PATH];

			if(GetCurrentDirectory(sizeof(path), path)) {
				_tcsncat(path, _T("\\"), sizeof(path));

				if(RegSetValueEx(hKey, _T("Location"), NULL, REG_SZ, (CONST BYTE *) path, sizeof(path)) != ERROR_SUCCESS) {
					dbgprint("COEAPITray::OnStartHook: Error RegSetValueEx\n");
				}
			}
			else {
				dbgprint("COEAPITray::OnStartHook: Error GetCurrentDirectory\n");
			}

			RegCloseKey(hKey);
		}
	}
#endif // NDEBUG
	dbgprint("Library oehook.dll loaded.\n") ;

	return;
}

/////////////////////////////////////////////////////////////////////////////
// COEAPITray commands

// Called when the application gets its death sentence.
int COEAPITray::ExitInstance() 
{
	DestroyIcon(m_hIcon);							// ditch the icon
	return CWinApp::ExitInstance();					// get outta here
}

HICON COEAPITray::CheckDate()
{
	CTime time = CTime::GetCurrentTime();
	HICON hIcon;

	hIcon = LoadIcon(IDI_OEAPI);

	return hIcon;
}

void CAboutDlg::OnDestroy() 
{
	CDialog::OnDestroy();
    CSystemTray::MinimiseToTray(this, TRUE);
    m_bVisible = FALSE;
}

void CAboutDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
    
    if (bShow)
        CSystemTray::MaximiseFromTray(this, TRUE);
}

int CAboutDlg::DoModal() 
{
    if (m_bVisible)
        return IDCANCEL;

    m_bVisible = TRUE;
	return CDialog::DoModal();
}

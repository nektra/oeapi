/* $Id: demoDlg.cpp,v 1.11 2008/02/21 05:59:44 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

// demoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "demoDlg.h"

#include "InputDlg.h"
#include "FolderSelect.h"
#include "MessagesDlg.h"
#include "MsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent), m_fm(TRUE), m_oeapiInit(TRUE)
{
	//{{AFX_DATA_INIT(CDemoDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_OEAPI);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
	DDX_Control(pDX, IDC_ADD_BUTTON, m_addButtonBtn);
	DDX_Control(pDX, IDC_TOOLBAR, m_addToolbarBtn);
	DDX_Control(pDX, IDC_ADD_MENU_ITEMS, m_addMenuItemsBtn);
	DDX_Control(pDX, IDC_CREATE_MSG, m_createMsgBtn);
	DDX_Control(pDX, IDC_MSG_SELECTION, m_msgSelBtn);
	DDX_Control(pDX, IDC_FOLDER_LIST, m_folderList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TOOLBAR, OnToolbar)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_ADD_MENU_ITEMS, OnAddMenuItems)
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_MOVE, OnMove)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_PRINT_MSGS, OnPrintMsgs)
	ON_BN_CLICKED(IDC_CREATE_MSG, OnCreateMsg)
	ON_BN_CLICKED(IDC_MSG_SELECTION, OnMsgSelection)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTopMostWindow(m_hWnd, TRUE);

	OEIdentityWrapper identity(TRUE);

	m_previousIdentity = identity->GetCurrentIdentity().t_str().c_str();

	// don't let the user use OEAPI library if OE is not running
	// enable in OEAPI init event
	m_addMenuItemsBtn.EnableWindow(FALSE);
	m_addToolbarBtn.EnableWindow(FALSE);
	m_addButtonBtn.EnableWindow(FALSE);
	m_msgSelBtn.EnableWindow(FALSE);

	m_oeapiInit.SetDialog(this);
	m_fm.SetDialog(this);
	m_oeapi.SetDialog(this);
	m_toolbar.SetDialog(this);
	m_button.SetDialog(this);
	m_menu.SetDialog(this);
	m_menuItem.SetDialog(this);

//	m_inbox.SetDialog(this);
//	m_inbox = m_fm->GetInboxFolder();

	// activate the OnNewMessage event
	// only needed for Standard Edition
//	m_fm->ActivateGlobalNotification();

	RefreshFolders();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDemoDlg::RefreshFolders()
{
	m_folderList.ResetContent();
	OEFolderWrapper folder = m_fm->GetFirstFolder();

	while(!folder.is_null()) {
		m_folderList.AddString(folder->Name().t_str().c_str());
		folder = m_fm->GetNextFolder();
	}
}

OEFolderWrapper CDemoDlg::GetFolder(const TCHAR *name)
{
	OEFolderWrapper folder = m_fm->GetFirstFolder();

	while(!folder.is_null()) {
		if(folder->Name().t_str() == name) {
			break;
		}
		folder = m_fm->GetNextFolder();
	}

	return folder;
}

void CDemoDlg::OnInitOEAPI()
{
	SetWindowText(_T("OE Loaded"));
	if(m_oeapi.is_null()) {
		OEIdentityWrapper identity(TRUE);
		CString newIdentity;

		newIdentity = identity->GetCurrentIdentity().t_str().c_str();

		m_oeapi.Create();

		// only refresh folders if the identity had changed
		if(newIdentity != m_previousIdentity) {
			m_previousIdentity = newIdentity;
			
			// remove all OESTORE object to force library unload.
			// this is done to manage identities. The library must be unloaded and then 
			// loaded again to update current identity.
			m_fm = NULL;
//			m_inbox = NULL;

			m_fm.Create();

//			m_inbox = m_fm->GetInboxFolder();

			// activate the OnNewMessage event
			// only needed for Standard Edition
//			m_fm->ActivateGlobalNotification();

			RefreshFolders();
		}
	}

	m_addMenuItemsBtn.EnableWindow(TRUE);
	m_addToolbarBtn.EnableWindow(TRUE);
	m_addButtonBtn.EnableWindow(TRUE);

#if !defined(EVALUATION_VERSION) && !defined(STANDARD_VERSION)
	m_msgSelBtn.EnableWindow(TRUE);
#endif // EVALUATION_VERSION || STANDARD_VERSION
}

void CDemoDlg::OnShutdownOEAPI()
{
	m_addMenuItemsBtn.EnableWindow(FALSE);
	m_addToolbarBtn.EnableWindow(FALSE);
	m_addButtonBtn.EnableWindow(FALSE);
	m_msgSelBtn.EnableWindow(FALSE);

	SetWindowText(_T("OE Not Loaded"));
	m_oeapi = NULL;
	m_toolbar = NULL;
	m_menu = NULL;
	m_menuItem = NULL;
	m_button = NULL;
}

void CDemoDlg::OnToolbar() 
{
	if(m_oeapi.is_null()) {
		m_oeapi.Create();
	}

	m_toolbar = m_oeapi->CreateToolbar();
}

void CDemoDlg::OnAddButton() 
{
	if(m_toolbar.is_null()) {
		MessageBox(_T("Create a toolbar before creating a button"), _T("Button Creation Error"), MB_OK);
		return;
	}

	TCHAR filename[4*MAX_PATH];
	//GetCurrentDirectory(sizeof(filename), filename);
	GetModuleFileName(NULL, filename, sizeof(filename));
	// PathRemoveFileSpec(filename);
	TCHAR* p = strrchr(filename, '\\');
	*p = '\0';
	
	CString appPath, butPath;
	butPath = filename;

	CString normal, over;
	normal = butPath + _T("\\images\\img_normal.bmp");
	over = butPath + _T("\\images\\img_mouseover.bmp");
	m_button = m_toolbar->CreateButton(_T("Nektra"), (LPCTSTR) normal, (LPCTSTR) over);
}

void CDemoDlg::OnAddMenuItems() 
{
	if(m_oeapi.is_null()) {
		m_oeapi.Create();
	}
	m_menu = m_oeapi->GetMenu(5);
	m_menuItem = m_menu->CreateItem(_T("Test Item"));
	m_menuItem->CreateSubItem(_T("Test Sub Item 1"));
	m_menuItem->CreateSubItem(_T("Test Sub Item 2"));
	m_menuItem = m_menuItem->CreateSubItem(_T("Test Sub Item 3"));
}

void CDemoDlg::OnRename() 
{
	CString str;
	m_folderList.GetText(m_folderList.GetCurSel(), str);

	OEFolderWrapper folder;
	
	folder = GetFolder((LPCTSTR) str);
	if(folder.is_null()) {
		MessageBox(_T("Select a folder first"), _T("Error"));
		return;
	}
	CInputDlg dlg;
	if(dlg.DoModal() == IDOK) {
		folder->Rename((LPCTSTR) dlg.m_strValue);
		RefreshFolders();
	}
}

void CDemoDlg::OnDelete() 
{
	CString str;
	m_folderList.GetText(m_folderList.GetCurSel(), str);

	OEFolderWrapper folder;
	
	folder = GetFolder((LPCTSTR) str);
	if(folder.is_null()) {
		MessageBox(_T("Select a folder first"), _T("Error"));
		return;
	}
	folder->Delete();
	RefreshFolders();
}

void CDemoDlg::OnMove() 
{
	CString str;
	m_folderList.GetText(m_folderList.GetCurSel(), str);

	OEFolderWrapper folder;
	
	folder = GetFolder((LPCTSTR) str);
	if(folder.is_null()) {
		MessageBox(_T("Select a folder first"), _T("Error"));
		return;
	}
	CFolderSelect dlg;
	if(dlg.DoModal() == IDOK) {
		OEFolderWrapper parFolder = GetFolder((LPCTSTR) dlg.m_folderSelection);
		folder->Move(parFolder->GetID());
		RefreshFolders();
	}
}

void CDemoDlg::OnCreate() 
{
	CString str;
	m_folderList.GetText(m_folderList.GetCurSel(), str);

	OEFolderWrapper folder;
	
	folder = GetFolder((LPCTSTR) str);
	if(folder.is_null()) {
		MessageBox(_T("Select a folder first"), _T("Error"));
		return;
	}
	CInputDlg dlg;
	if(dlg.DoModal() == IDOK) {
		folder->CreateFolder((LPCTSTR) dlg.m_strValue);
		RefreshFolders();
	}
}

void CDemoDlg::OnPrintMsgs() 
{
	CMessagesDlg dlg;
	CString str;
	m_folderList.GetText(m_folderList.GetCurSel(), str);

	OEFolderWrapper folder = GetFolder((LPCTSTR) str);
	if(folder.is_null()) {
		MessageBox(_T("Select a folder first"), _T("Error"));
		return;
	}

	dlg.SetFolderID(folder->GetID());
	dlg.DoModal();
}

void CDemoDlg::OnCreateMsg() 
{
	CString str;
	if(m_folderList.GetCurSel() != -1) {
		m_folderList.GetText(m_folderList.GetCurSel(), str);
	}

	OEFolderWrapper folder = GetFolder((LPCTSTR) str);
	if(folder.is_null()) {
		MessageBox(_T("Select a folder first"), _T("Error"));
		return;
	}

	CMsgDlg dlg;

	if(dlg.DoModal() == IDOK) {
		CString msgSource;

		msgSource = msgSource + _T("From: ") + dlg.m_from + _T("\n");
		msgSource = msgSource + _T("To: ") + dlg.m_to + _T("\n");
		msgSource = msgSource + _T("Subject: ") + dlg.m_subject + _T("\n") + _T("\n");
		msgSource = msgSource + dlg.m_body;

        OEMessageWrapper m = folder->CreateMessage((LPCTSTR) msgSource, TRUE);
		if(!m.is_null()) {
			MessageBox(_T("Message succesfully created."), _T("Create Message"));
		}
	}
}

void CDemoDlg::OnMsgSelection()
{
#if !defined(EVALUATION_VERSION) && !defined(STANDARD_VERSION)
	CMessagesDlg dlg;
	dlg.SetHandleSelection();
	dlg.DoModal();
#endif // !EVALUATION_VERSION && !STANDARD_VERSION
}

void CDemoDlg::OnClose() 
{
	if(!m_oeapi.is_null()) {
		m_oeapi->RemoveAllMenuItems();
		m_oeapi->RemoveAllToolbars();
	}
	CDialog::OnClose();
}

void CDemoDlg::OnAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

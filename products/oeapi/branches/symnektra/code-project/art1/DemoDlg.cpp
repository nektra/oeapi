/* $Id: DemoDlg.cpp,v 1.4 2007/01/03 11:09:50 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 * You may modify and/or integrate this code into your commercial software
 * keeping this notice.
 *
 **/

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "InputDlg.h"
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
	: CDialog(CDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDemoDlg::~CDemoDlg()
{
	if(m_pStoreNamespace) {
		m_pStoreNamespace->Release();
	}
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
	DDX_Control(pDX, IDC_LIST_FOLDER, m_listFolder);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CRE, OnCreateFolder)
	ON_BN_CLICKED(IDC_BUTTON_REM, OnRemoveFolder)
	ON_BN_CLICKED(IDC_BUTTON_REN, OnRenameFolder)
	ON_BN_CLICKED(IDC_BUTTON_PRI, OnViewMessages)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pStoreNamespace = NULL;

	HRESULT hr = CoInitialize(0);
	if(FAILED(hr)) {
		MessageBox(_T("Error CoInitialize."), _T("Demo Error"));
		EndDialog(1);
		return FALSE;
	}

	// TODO: Add extra initialization here
	hr = CoCreateInstance(CLSID_StoreNamespace, NULL, CLSCTX_SERVER, 
									IID_IStoreNamespace, (LPVOID*) &m_pStoreNamespace);
	if(FAILED(hr)) {
		MessageBox(_T("Error creating IStoreNamespace."), _T("Demo Error"));
		EndDialog(1);
		return FALSE;
	}

	hr = m_pStoreNamespace->Initialize(NULL, NULL);
	if(FAILED(hr)) {
		MessageBox(_T("Error initializing IStoreNamespace."), _T("Demo Error"));
		m_pStoreNamespace->Release();
		EndDialog(1);
		return FALSE;
	}

	m_listFolder.ResetContent();
	AddFolders();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemoDlg::AddFolders(STOREFOLDERID dwFolderId)
{
	FOLDERPROPS props;
	HENUMSTORE hEnum;
	int nIndex;

	hEnum = NULL;
	
	// set the size of the structure or the function return error
	props.cbSize = sizeof(FOLDERPROPS);

	HRESULT hr = m_pStoreNamespace->GetFirstSubFolder(dwFolderId, &props, &hEnum);

	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) {
		
		nIndex = m_listFolder.AddString(props.szName);
		
		if(nIndex != LB_ERR && nIndex != LB_ERRSPACE) {
			// set the folder id as the data of the item
			m_listFolder.SetItemData(nIndex, props.dwFolderId);

			// add children of this folder too
			AddFolders(props.dwFolderId);
		}

		hr = m_pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	// close the enum
	if(hEnum) {
		m_pStoreNamespace->GetSubFolderClose(hEnum);
	}
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDemoDlg::OnCreateFolder() 
{
	int nIndex;
	STOREFOLDERID dwSelFolder, dwNewFolder;
	HRESULT hr;
	const char *szName;

	// Get selected folder id
	nIndex = m_listFolder.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a folder first."), _T("Demo Error"));
		return;
	}

	dwSelFolder = m_listFolder.GetItemData(nIndex);

	CInputDlg inputDlg;

	inputDlg.SetCaption(_T("Create Folder"));

	if(inputDlg.DoModal() != IDOK) {
		return;
	}

	szName = (const char *) inputDlg.m_edit;

	hr = m_pStoreNamespace->CreateFolder(dwSelFolder, szName, 0, &dwNewFolder);
	if(FAILED(hr)) {
		MessageBox(_T("Error creating folder."), _T("Demo Error"));
		return;
	}

	m_listFolder.ResetContent();
	AddFolders();
}

void CDemoDlg::OnRemoveFolder() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	STOREFOLDERID dwSelFolder;
	HRESULT hr;

	// Get selected folder id
	nIndex = m_listFolder.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a folder first."), _T("Demo Error"));
		return;
	}

	dwSelFolder = m_listFolder.GetItemData(nIndex);

	hr = m_pStoreNamespace->DeleteFolder(dwSelFolder, 0);
	if(FAILED(hr)) {
		MessageBox(_T("Error deleting folder."), _T("Demo Error"));
		return;
	}

	m_listFolder.ResetContent();
	AddFolders();
}

void CDemoDlg::OnRenameFolder() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	STOREFOLDERID dwSelFolder;
	HRESULT hr;
	const char *szName;

	// Get selected folder id
	nIndex = m_listFolder.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a folder first."), _T("Demo Error"));
		return;
	}

	dwSelFolder = m_listFolder.GetItemData(nIndex);

	CInputDlg inputDlg;

	inputDlg.SetCaption(_T("Rename Folder"));

	if(inputDlg.DoModal() != IDOK) {
		return;
	}

	szName = (const char *) inputDlg.m_edit;

	hr = m_pStoreNamespace->RenameFolder(dwSelFolder, 0, szName);
	if(FAILED(hr)) {
		MessageBox(_T("Error renaming folder."), _T("Demo Error"));
		return;
	}

	m_listFolder.ResetContent();
	AddFolders();
}

void CDemoDlg::OnViewMessages() 
{
	int nIndex;
	STOREFOLDERID dwSelFolder;

	// Get selected folder id
	nIndex = m_listFolder.GetCurSel();
	if(nIndex == LB_ERR) {
		MessageBox(_T("Select a folder first."), _T("Demo Error"));
		return;
	}

	dwSelFolder = m_listFolder.GetItemData(nIndex);
	
	CMsgDlg msgDlg;

	msgDlg.SetFolder(m_pStoreNamespace, dwSelFolder);
	msgDlg.DoModal();
}

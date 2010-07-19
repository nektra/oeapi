// trayView.cpp : implementation of the CTrayView class
//

#include "stdafx.h"
#include "tray.h"

#include "trayDoc.h"
#include "trayView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrayView

IMPLEMENT_DYNCREATE(CTrayView, CView)

BEGIN_MESSAGE_MAP(CTrayView, CView)
	//{{AFX_MSG_MAP(CTrayView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayView construction/destruction

CTrayView::CTrayView()
{
	// TODO: add construction code here

}

CTrayView::~CTrayView()
{
}

BOOL CTrayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTrayView drawing

void CTrayView::OnDraw(CDC* pDC)
{
	CTrayDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTrayView diagnostics

#ifdef _DEBUG
void CTrayView::AssertValid() const
{
	CView::AssertValid();
}

void CTrayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTrayDoc* CTrayView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTrayDoc)));
	return (CTrayDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrayView message handlers

// trayDoc.cpp : implementation of the CTrayDoc class
//

#include "stdafx.h"
#include "tray.h"

#include "trayDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrayDoc

IMPLEMENT_DYNCREATE(CTrayDoc, CDocument)

BEGIN_MESSAGE_MAP(CTrayDoc, CDocument)
	//{{AFX_MSG_MAP(CTrayDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayDoc construction/destruction

CTrayDoc::CTrayDoc()
{
	// TODO: add one-time construction code here

}

CTrayDoc::~CTrayDoc()
{
}

BOOL CTrayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTrayDoc serialization

void CTrayDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTrayDoc diagnostics

#ifdef _DEBUG
void CTrayDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTrayDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrayDoc commands

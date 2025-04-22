// trayView.h : interface of the CTrayView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYVIEW_H__56C9D0B7_9C19_4B59_AA47_9508DBC4C0B7__INCLUDED_)
#define AFX_TRAYVIEW_H__56C9D0B7_9C19_4B59_AA47_9508DBC4C0B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTrayView : public CView
{
protected: // create from serialization only
	CTrayView();
	DECLARE_DYNCREATE(CTrayView)

// Attributes
public:
	CTrayDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTrayView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in trayView.cpp
inline CTrayDoc* CTrayView::GetDocument()
   { return (CTrayDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYVIEW_H__56C9D0B7_9C19_4B59_AA47_9508DBC4C0B7__INCLUDED_)

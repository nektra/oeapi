// trayDoc.h : interface of the CTrayDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYDOC_H__E9F3AB2E_C273_4B70_A8BF_0D99D1EF078A__INCLUDED_)
#define AFX_TRAYDOC_H__E9F3AB2E_C273_4B70_A8BF_0D99D1EF078A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTrayDoc : public CDocument
{
protected: // create from serialization only
	CTrayDoc();
	DECLARE_DYNCREATE(CTrayDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrayDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTrayDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYDOC_H__E9F3AB2E_C273_4B70_A8BF_0D99D1EF078A__INCLUDED_)

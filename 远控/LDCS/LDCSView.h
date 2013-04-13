// LDCSView.h : interface of the CLDCSView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDCSVIEW_H__D94A0EDD_2DDC_4AB8_AED2_6603A8F9EA6E__INCLUDED_)
#define AFX_LDCSVIEW_H__D94A0EDD_2DDC_4AB8_AED2_6603A8F9EA6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLDCSView : public CView
{
protected: // create from serialization only
	CLDCSView();
	DECLARE_DYNCREATE(CLDCSView)

// Attributes
public:
	CLDCSDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLDCSView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLDCSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLDCSView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LDCSView.cpp
inline CLDCSDoc* CLDCSView::GetDocument()
   { return (CLDCSDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDCSVIEW_H__D94A0EDD_2DDC_4AB8_AED2_6603A8F9EA6E__INCLUDED_)

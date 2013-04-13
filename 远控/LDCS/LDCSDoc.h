// LDCSDoc.h : interface of the CLDCSDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDCSDOC_H__D93D3C64_2CA5_439A_B451_810046F7AE22__INCLUDED_)
#define AFX_LDCSDOC_H__D93D3C64_2CA5_439A_B451_810046F7AE22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLDCSDoc : public CDocument
{
protected: // create from serialization only
	CLDCSDoc();
	DECLARE_DYNCREATE(CLDCSDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLDCSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLDCSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLDCSDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDCSDOC_H__D93D3C64_2CA5_439A_B451_810046F7AE22__INCLUDED_)

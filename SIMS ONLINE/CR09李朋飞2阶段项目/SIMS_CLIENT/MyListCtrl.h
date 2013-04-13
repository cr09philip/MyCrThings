#if !defined(AFX_MYLISTCTRL_H__5B3794B3_64BF_4A9D_80E0_F29FF026BE4C__INCLUDED_)
#define AFX_MYLISTCTRL_H__5B3794B3_64BF_4A9D_80E0_F29FF026BE4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window

class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InsertColumns( CString csTxt1,CString csTxt2 = _T(""),CString csTxt3 = _T(""),
						CString csTxt4 = _T(""),CString csTxt5 = _T(""),CString csTxt6 = _T(""));
	void Init();
	void AutoSize();
	virtual ~CMyListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__5B3794B3_64BF_4A9D_80E0_F29FF026BE4C__INCLUDED_)

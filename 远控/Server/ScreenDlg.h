#if !defined(AFX_SCREENDLG_H__68B25106_6171_4A7B_A2D9_6804A8946DA5__INCLUDED_)
#define AFX_SCREENDLG_H__68B25106_6171_4A7B_A2D9_6804A8946DA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenDlg.h : header file
//

#include "struct.h"
/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog

class CScreenDlg : public CDialog
{
// Construction
public:
	HANDLE  m_hThread;
    DWORD   m_dwThreadId;

	CScreenDlg(CWnd* pParent = NULL);   // standard constructor
    CScreenDlg(CONNECT_INFO connInfo);

    CONNECT_INFO m_connInfo;
// Dialog Data
	//{{AFX_DATA(CScreenDlg)
	enum { IDD = IDD_SCREEN_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreenDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENDLG_H__68B25106_6171_4A7B_A2D9_6804A8946DA5__INCLUDED_)

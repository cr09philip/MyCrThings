// CbitmapTestDlg.h : header file
//

#if !defined(AFX_CBITMAPTESTDLG_H__B830BC8A_B5FB_44F3_A7B3_E02F8B75299F__INCLUDED_)
#define AFX_CBITMAPTESTDLG_H__B830BC8A_B5FB_44F3_A7B3_E02F8B75299F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCbitmapTestDlg dialog

class CCbitmapTestDlg : public CDialog
{
// Construction
public:
	void DrawVidio( CWnd* pWnd, RECT &rect ,char *pBitBuff);
	void ShowScreen();
	CCbitmapTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCbitmapTestDlg)
	enum { IDD = IDD_CBITMAPTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCbitmapTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCbitmapTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShowScreen();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBITMAPTESTDLG_H__B830BC8A_B5FB_44F3_A7B3_E02F8B75299F__INCLUDED_)

// SIMS_SERVERDlg.h : header file
//

#if !defined(AFX_SIMS_SERVERDLG_H__09E7C338_0A49_46A5_AD36_AF7D1C6FBEE1__INCLUDED_)
#define AFX_SIMS_SERVERDLG_H__09E7C338_0A49_46A5_AD36_AF7D1C6FBEE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySocketInit.h"
#include "ThreadManage.h"
#include "ConnSql.h"

/////////////////////////////////////////////////////////////////////////////
// CSIMS_SERVERDlg dialog

class CSIMS_SERVERDlg : public CDialog
{
// Construction
public:
	void BeginWork();
	CSIMS_SERVERDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSIMS_SERVERDlg)
	enum { IDD = IDD_SIMS_SERVER_DIALOG };
	CStatic	m_ctlStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIMS_SERVERDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CMySocketInit m_socketInit;
	CComInit m_ComInit;
	SOCKET m_hServer;

	// Generated message map functions
	//{{AFX_MSG(CSIMS_SERVERDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartBtn();
	afx_msg void OnEndBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMS_SERVERDLG_H__09E7C338_0A49_46A5_AD36_AF7D1C6FBEE1__INCLUDED_)

// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__80A01C3A_E1A9_46FC_8BB8_CA85CC5C5358__INCLUDED_)
#define AFX_SERVERDLG_H__80A01C3A_E1A9_46FC_8BB8_CA85CC5C5358__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InitSocket.h"
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	SOCKET m_hSocket;
	CToolBar m_wndToolBar;
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

    CInitSocket m_initSocket;
    HANDLE      m_hAcptThread; 
// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartServer();
	afx_msg void OnStopServer();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__80A01C3A_E1A9_46FC_8BB8_CA85CC5C5358__INCLUDED_)

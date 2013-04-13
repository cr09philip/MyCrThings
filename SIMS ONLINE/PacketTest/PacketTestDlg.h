// PacketTestDlg.h : header file
//

#if !defined(AFX_PACKETTESTDLG_H__3AF9A2F8_1EC0_4E38_B8A6_DB5E7205CCE6__INCLUDED_)
#define AFX_PACKETTESTDLG_H__3AF9A2F8_1EC0_4E38_B8A6_DB5E7205CCE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPacketTestDlg dialog

class CPacketTestDlg : public CDialog
{
// Construction
public:
	CPacketTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPacketTestDlg)
	enum { IDD = IDD_PACKETTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPacketTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPacketTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETTESTDLG_H__3AF9A2F8_1EC0_4E38_B8A6_DB5E7205CCE6__INCLUDED_)

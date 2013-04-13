// SIMS_CLIENTDlg.h : header file
//

#if !defined(AFX_SIMS_CLIENTDLG_H__358EB381_8758_413B_9835_A2548F9B4AFB__INCLUDED_)
#define AFX_SIMS_CLIENTDLG_H__358EB381_8758_413B_9835_A2548F9B4AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySocketInit.h"
#include "MyListCtrl.h"
#include "MyPacket.h"
#include "Cache.h"

#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSIMS_CLIENTDlg dialog

// struct tagRecordSet
// {
// 	char m_szId[12];
// 	char m_szName[12];
// 	char m_szGender[12];
// 	char m_szPhone[16];
// 	char m_szAddr[12];
// 	char m_szBirth[16];
// };

class CSIMS_CLIENTDlg : public CDialog
{
// Construction
public:
	void OnSendSql(CString& csSql,DWORD dwOper = 0);
	void UpdateUI();
//	vector<tagRecordSet> m_recvList;

	CMySocketInit m_socketInit;
	sockaddr_in m_destAddr;
	SOCKET m_hClient;
	CSIMS_CLIENTDlg(CWnd* pParent = NULL);	// standard constructor
	CCache  m_cache;
// Dialog Data
	//{{AFX_DATA(CSIMS_CLIENTDlg)
	enum { IDD = IDD_SIMS_CLIENT_DIALOG };
	CEdit	m_ctlEdit;
	CString	m_Name;
	CString	m_Id;
	CString	m_Birth;
	CString	m_Addr;
	CString	m_Gender;
	CString	m_Phone;
	int		m_nSelect;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIMS_CLIENTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSIMS_CLIENTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnectionBtn();
	afx_msg void OnQureyBtn();
	afx_msg void OnAddBtn();
	afx_msg void OnModifyBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnOperOkBtn();
	afx_msg void OnAddRadio();
	afx_msg void OnModifyRadio();
	afx_msg void OnDelRadio();
	afx_msg void OnClearBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMS_CLIENTDLG_H__358EB381_8758_413B_9835_A2548F9B4AFB__INCLUDED_)

// SIMS_CLIENTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIMS_CLIENT.h"
#include "SIMS_CLIENTDlg.h"
#include "resource.h"


#include <afxsock.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIMS_CLIENTDlg dialog

CSIMS_CLIENTDlg::CSIMS_CLIENTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIMS_CLIENTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIMS_CLIENTDlg)
	m_Name = _T("");
	m_Id = _T("");
	m_Birth = _T("");
	m_Addr = _T("");
	m_Gender = _T("");
	m_Phone = _T("");
	m_nSelect = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset( (char*)&m_destAddr, 0, sizeof(sockaddr_in) );
}

void CSIMS_CLIENTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIMS_CLIENTDlg)
	DDX_Control(pDX, IDC_EDIT1, m_ctlEdit);
	DDX_Text(pDX, IDC_NAME_EDIT, m_Name);
	DDV_MaxChars(pDX, m_Name, 10);
	DDX_Text(pDX, IDC_ID_EDIT, m_Id);
	DDV_MaxChars(pDX, m_Id, 10);
	DDX_Text(pDX, IDC_BIRTH_EDIT, m_Birth);
	DDV_MaxChars(pDX, m_Birth, 10);
	DDX_Text(pDX, IDC_ADDR_EDIT, m_Addr);
	DDV_MaxChars(pDX, m_Addr, 10);
	DDX_Text(pDX, IDC_GENDER_EDIT, m_Gender);
	DDV_MaxChars(pDX, m_Gender, 10);
	DDX_Text(pDX, IDC_PHONE_EDIT, m_Phone);
	DDV_MaxChars(pDX, m_Phone, 12);
	DDX_Radio(pDX, IDC_ADD_RADIO, m_nSelect);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSIMS_CLIENTDlg, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CSIMS_CLIENTDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECTION_BTN, OnConnectionBtn)
	ON_BN_CLICKED(IDC_QUREY_BTN, OnQureyBtn)
	ON_BN_CLICKED(IDC_ADD_BTN, OnAddBtn)
	ON_BN_CLICKED(IDC_MODIFY_BTN, OnModifyBtn)
	ON_BN_CLICKED(IDC_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(IDC_OPER_OK_BTN, OnOperOkBtn)
	ON_BN_CLICKED(IDC_ADD_RADIO, OnAddRadio)
	ON_BN_CLICKED(IDC_MODIFY_RADIO, OnModifyRadio)
	ON_BN_CLICKED(IDC_DEL_RADIO, OnDelRadio)
	ON_BN_CLICKED(IDC_CLEAR_BTN, OnClearBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIMS_CLIENTDlg message handlers

BOOL CSIMS_CLIENTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Id	= "007";
	m_Name	= "soso";
	m_Gender = "女";
	m_Phone	= "15927727200";
	m_Addr	= "洪山区";
	m_Birth = "1986-01-22";

	m_nSelect = 0;
	UpdateData(FALSE);

	m_cache.CreateObj();

	OnConnectionBtn();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSIMS_CLIENTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSIMS_CLIENTDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSIMS_CLIENTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSIMS_CLIENTDlg::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_SIMS_CLIENTDLG));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

DWORD WINAPI ThreadProc( LPVOID lpParameter )
{
	CSIMS_CLIENTDlg* dlg = (CSIMS_CLIENTDlg*) lpParameter;
//	dlg->m_recvList.empty();
	
	char szBuf[65536] = {0};
	sockaddr_in revAddr = {0};
	int nLen = sizeof(sockaddr);
	
	int nCount = 0;
	//dengdai
	while (true)
	{
		memset(szBuf,0,65536);
		int nRet = recvfrom( dlg->m_hClient, szBuf, 65536, 0, (sockaddr*)&revAddr, &nLen );
	
		if (nRet == SOCKET_ERROR)
		{
			AfxMessageBox("Recvfrom Error");
		}	
		//解包
 		CMyPacket thePacket;

 		if (!thePacket.UnPacket(szBuf))//解包出错
 			continue;


		if (thePacket.m_head.m_dwOperType == ADO_GETTIME)//获取时间操作的返回包
		{
			if (dlg->m_cache.IsNew( CString(thePacket.m_lpTagContent->m_szPacket) ))//缓冲区为最新
			{
				dlg->m_ctlEdit.Clear();
				dlg->m_ctlEdit.SetWindowText(dlg->m_cache.m_csCache);
			}
			else//缓冲区数据过期，则重新向服务端申请查询
			{
				DWORD nNeed = thePacket.Packet("select * from student",NULL,AD0_SELECT);
				
				char* szSendBuf = new char[nNeed];

				if (szSendBuf == NULL)
					continue;

				thePacket.Packet("select * from student",szSendBuf,AD0_SELECT);

				int nRet = sendto( dlg->m_hClient,szSendBuf,nNeed,0, (sockaddr*)&dlg->m_destAddr, sizeof(sockaddr) );

				if (szSendBuf != NULL)
					delete [] szSendBuf;

				if ( nRet == SOCKET_ERROR )
				{
					closesocket(dlg->m_hClient);
					AfxMessageBox("OnQureyBtn sendto Error");
					continue;
				}
			}
		}
		else if (thePacket.m_head.m_dwOperType == AD0_SELECT)//查询操作的返回包
		{		
			dlg->m_cache.SetCache(thePacket.m_lpTagContent->m_szPacket);
//			dlg->m_cache.m_lastTime = thePacket.m_head.m_time;

			dlg->m_ctlEdit.Clear();
			dlg->m_ctlEdit.SetWindowText(thePacket.m_lpTagContent->m_szPacket);

			dlg->UpdateUI();
		}

	}
	
	return 0;
}

//创建socket并给服务器地址赋值
void CSIMS_CLIENTDlg::OnConnectionBtn() 
{
	// TODO: Add your control notification handler code here
	m_hClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if ( m_hClient == INVALID_SOCKET )
	{
		AfxMessageBox("创建socket套接字失败，请确认服务端是否打开。");
		return;
	}
	m_destAddr.sin_family = AF_INET;
	m_destAddr.sin_port = htons(7777);
	m_destAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

//	OnQureyBtn();
}


//发送报文给服务器
void CSIMS_CLIENTDlg::OnQureyBtn() 
{
	// TODO: Add your control notification handler code here
	CMyPacket thePacket;

	DWORD nNeed = thePacket.Packet("",NULL,ADO_GETTIME);//ADO_GETTIME
	
	char* szSendBuf = new char[nNeed];

	if (szSendBuf == NULL)
		return;

	thePacket.Packet("",szSendBuf,ADO_GETTIME);
	
	int nRet = sendto( m_hClient,szSendBuf,nNeed, 0, (sockaddr*)&m_destAddr, sizeof(sockaddr) );
	
	if (szSendBuf != NULL)
		delete [] szSendBuf;

	if ( nRet == SOCKET_ERROR )
	{
		closesocket(m_hClient);
		AfxMessageBox("OnAddBtn sendto Error");
		return;
	}
	
	HANDLE hThread = CreateThread(NULL,0,ThreadProc,(LPVOID)this,NULL,NULL);
}

void CSIMS_CLIENTDlg::UpdateUI()
{
	//获取数据
	OnQureyBtn();
}

void CSIMS_CLIENTDlg::OnAddBtn() 
{
	// TODO: Add your control notification handler code here
	CMyPacket thePacket;
	thePacket.Packet("INSERT INTO Student VALUES ('007','Hello','男','132145787','武汉','19800901')",AD0_ADD);
	
	int nRet = sendto( m_hClient,(char*)&thePacket, sizeof(CSocketHead) +sizeof(int) + thePacket.m_lpTagContent->m_nLen,  
		0, (sockaddr*)&m_destAddr, sizeof(sockaddr) );

	if ( nRet == SOCKET_ERROR )
	{
		closesocket(m_hClient);
		AfxMessageBox("OnAddBtn sendto Error");
		return;
	}
}

void CSIMS_CLIENTDlg::OnModifyBtn() 
{
	// TODO: Add your control notification handler code here
	CMyPacket thePacket;
	
	DWORD nNeed = thePacket.Packet("update student set stu_name = '张三' where stu_id = '001'",NULL,AD0_MODIFY);
	
	char* szSendBuf = new char[nNeed];
	
	if (szSendBuf == NULL)
		return;
	
	thePacket.Packet("update student set stu_name = '张三' where stu_id = '001'",szSendBuf,AD0_MODIFY);
	
	int nRet = sendto( m_hClient,szSendBuf,nNeed, 0, (sockaddr*)&m_destAddr, sizeof(sockaddr) );
	
	if (szSendBuf != NULL)
		delete [] szSendBuf;
	
	if ( nRet == SOCKET_ERROR )
	{
		closesocket(m_hClient);
		AfxMessageBox("OnModifyBtn sendto Error");
		return;
	}
}

void CSIMS_CLIENTDlg::OnDelBtn() 
{
	// TODO: Add your control notification handler code here
	CMyPacket thePacket;
	
	DWORD nNeed = thePacket.Packet("delete from student where stu_id = '007' ",NULL,AD0_DELETE);
	
	char* szSendBuf = new char[nNeed];
	
	if (szSendBuf == NULL)
		return;
	
	thePacket.Packet("delete from student where stu_id = '007' ",szSendBuf,AD0_DELETE);
	
	int nRet = sendto( m_hClient,szSendBuf,nNeed, 0, (sockaddr*)&m_destAddr, sizeof(sockaddr) );
	
	if (szSendBuf != NULL)
		delete [] szSendBuf;
	
	if ( nRet == SOCKET_ERROR )
	{
		closesocket(m_hClient);
		AfxMessageBox("OnDelBtn sendto Error");
		return;
	}
}

void CSIMS_CLIENTDlg::OnOperOkBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString csSql;

	switch( m_nSelect)
	{
	case 0:
		csSql.Format("INSERT INTO Student VALUES ('%s','%s','%s','%s','%s','%s')",
						m_Id, m_Name, m_Gender, m_Phone, m_Addr, m_Birth);
		break;
		
	case 1:
		csSql.Format("delete from student where stu_id = '%s' ", m_Id);
		break;

	case 2:
	csSql.Format("update student set stu_name = '%s',\
									 stu_gender = '%s', \
									 stu_phone = '%s', \
									 stu_addr = '%s', \
									 stu_birth = '%s' \
									where stu_id = '%s'  ",
						m_Name, m_Gender, m_Phone, m_Addr, m_Birth, m_Id);
		break;

	default:
		break;	
	}

	OnSendSql(csSql,m_nSelect);
	
	UpdateUI();
	UpdateData(FALSE);
}

void CSIMS_CLIENTDlg::OnAddRadio() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ID_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_NAME_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_GENDER_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PHONE_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_ADDR_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BIRTH_EDIT)->EnableWindow(TRUE);
}

void CSIMS_CLIENTDlg::OnModifyRadio() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ID_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_NAME_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_GENDER_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PHONE_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_ADDR_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BIRTH_EDIT)->EnableWindow(TRUE);
}

void CSIMS_CLIENTDlg::OnDelRadio() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ID_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_NAME_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_GENDER_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PHONE_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADDR_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BIRTH_EDIT)->EnableWindow(FALSE);
}

void CSIMS_CLIENTDlg::OnSendSql(CString &csSql,DWORD dwOper)
{
	CMyPacket thePacket;
	
	DWORD nNeed = thePacket.Packet((LPTSTR)(LPCTSTR)csSql,NULL,dwOper);
	
	char* szSendBuf = new char[nNeed];
	
	if (szSendBuf == NULL)
		return;
	
	thePacket.Packet((LPTSTR)(LPCTSTR)csSql,szSendBuf,dwOper);
	
	int nRet = sendto( m_hClient,szSendBuf,nNeed, 0, (sockaddr*)&m_destAddr, sizeof(sockaddr) );
	
	if (szSendBuf != NULL)
		delete [] szSendBuf;
	
	if ( nRet == SOCKET_ERROR )
	{
		closesocket(m_hClient);
		AfxMessageBox("OnSendSql sendto Error");
		return;
	}
}

void CSIMS_CLIENTDlg::OnClearBtn() 
{
	// TODO: Add your control notification handler code here
	m_ctlEdit.Clear();
	m_ctlEdit.SetWindowText("");
}

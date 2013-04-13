// SIMS_SERVERDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIMS_SERVER.h"
#include "SIMS_SERVERDlg.h"

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
// CSIMS_SERVERDlg dialog

CSIMS_SERVERDlg::CSIMS_SERVERDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIMS_SERVERDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIMS_SERVERDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSIMS_SERVERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIMS_SERVERDlg)
	DDX_Control(pDX, IDC_STATUS_STATIC, m_ctlStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSIMS_SERVERDlg, CDialog)
	//{{AFX_MSG_MAP(CSIMS_SERVERDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BTN, OnStartBtn)
	ON_BN_CLICKED(IDC_END_BTN, OnEndBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIMS_SERVERDlg message handlers

BOOL CSIMS_SERVERDlg::OnInitDialog()
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
	GetDlgItem(IDC_START_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_END_BTN)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSIMS_SERVERDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSIMS_SERVERDlg::OnPaint() 
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
HCURSOR CSIMS_SERVERDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//服务器程序开启UDP接收数据线程
//并整理接收的数据，将合适的内容加入任务队列
//2011-6-17
DWORD WINAPI RecvThreadProc( LPVOID lpParameter )
{
	SOCKET hServer = (SOCKET)lpParameter;

	char szBuf[65536] = {0};
	sockaddr_in revAddr = {0};
	int nLen = sizeof(sockaddr);
	
	while (true)
	{
		memset(szBuf, 0, 65536);
		recvfrom( hServer, szBuf, 65536, 0, (sockaddr*)&revAddr, &nLen );
		
		//解包
 		CMyPacket thePacket;

 		if (!thePacket.UnPacket(szBuf))//解包出错
			continue;
		
		//添加入任务队列
		CTask theTask;
		theTask.m_hSocket	= hServer;
		theTask.m_destAddr	= revAddr;
		theTask.m_nStatus	= UnDone;
		memcpy(&theTask.m_head, &thePacket.m_head, sizeof(CSocketHead));
		theTask.m_csTask	= thePacket.m_lpTagContent->m_szPacket;
		
		g_TaskQueue.push(theTask);
		
		//sendto(hServer,"got it",7,0,(sockaddr*)&revAddr, sizeof(sockaddr));
	}

	return 0;

}

//开启服务，设置socket，并绑定
void CSIMS_SERVERDlg::OnStartBtn() 
{
	(CButton*)GetDlgItem(IDC_START_BTN)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_END_BTN)->EnableWindow(TRUE);

	// TODO: Add your control notification handler code here
	m_hServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if ( m_hServer == INVALID_SOCKET)
	{
		m_ctlStatus.SetWindowText("开启服务失败！\r\n");
		
		//WSAGetLastError();
		
		return;
	}

	sockaddr_in addr = {0};
	
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(7777);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ( SOCKET_ERROR == bind( m_hServer, (sockaddr*)&addr, sizeof(sockaddr)) )
	{
		m_ctlStatus.SetWindowText("绑定失败！\r\n");
		
		return;
	}

	//创建接收线程
	DWORD dwThreadId = 0;
	HANDLE hThread = NULL;
	
	hThread = CreateThread( NULL, 0, RecvThreadProc,(void*)m_hServer,NULL,&dwThreadId);

	if (hThread == NULL)
	{
		AfxMessageBox("CreateThread Error");
		return;
	}

	//初始化执行线程组
	BeginWork();
	
	m_ctlStatus.SetWindowText("开启服务成功！\r\n");
}

//开启线程接收数据
void CSIMS_SERVERDlg::BeginWork()
{
	static CThreadManage* lpTM;
	lpTM->CreateObj(m_hServer);

	lpTM->Init();
}

void CSIMS_SERVERDlg::OnEndBtn() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_START_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_END_BTN)->EnableWindow(FALSE);

	OnCancel();
}

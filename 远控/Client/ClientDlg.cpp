// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

#include "Err.h"
#include "struct.h"

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
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONN_BTN, OnConnBtn)
	ON_BN_CLICKED(IDC_DISCONN_BTN, OnDisconnBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint() 
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD WINAPI AcptThreadFunc( LPVOID lpParam ) 
{
    SOCKET hSocket = (SOCKET) lpParam;

    int nRecvLen = 0;

    PACKAGE thePack;
    memset( &thePack, 0, sizeof(PACKAGE) );

    while (true)
    {
        //先接收包首部,获得附加信息大小
        nRecvLen = recv(hSocket, (char*)&thePack, sizeof(PACKAGE), 0);
        
        if ( nRecvLen == 0 || SOCKET_ERROR == nRecvLen)
        {
            ShowError();
            
            return -1;
        }

        //为附加信息申请空间
        char* pBuffer = new char[thePack.dwLen];
        if (pBuffer == NULL)
        {
            return -1;
        }

        //接收附加信息
        nRecvLen = recv(hSocket, pBuffer, thePack.dwLen, 0);

        if ( nRecvLen == 0 || SOCKET_ERROR == nRecvLen)
        {
            ShowError();
            
            return -1;
        }

        memset( &thePack, 0, sizeof(PACKAGE) );
    }

    return 0;
}

void CClientDlg::OnConnBtn() 
{
	// TODO: Add your control notification handler code here
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (m_hSocket == INVALID_SOCKET)    
    {
        ShowError();
        return;
    }
    
    sockaddr_in addr = {0};
    
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(7777);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ( 0 != connect(m_hSocket, (sockaddr*)& addr, sizeof(addr)))
    {     
        ShowError();
        return;
    }
    
    PACKAGE thePack;
    memset(&thePack, 0, sizeof(PACKAGE));

    thePack.opType = ConnSuccess;

    if (SOCKET_ERROR  == send(m_hSocket, (char*)&thePack, sizeof(PACKAGE), 0)   )
    {
        ShowError();
        return;
    }

    DWORD dwAcptThreadId = 0;
    HANDLE hRecvThread =  CreateThread( NULL,
                                        0,
                                        AcptThreadFunc,
                                        (LPVOID)m_hSocket,
                                        0,
                                        &dwAcptThreadId);

}

void CClientDlg::OnDisconnBtn() 
{
	// TODO: Add your control notification handler code here
	if (m_hSocket != INVALID_SOCKET)
	{
        closesocket(m_hSocket);
	}
}

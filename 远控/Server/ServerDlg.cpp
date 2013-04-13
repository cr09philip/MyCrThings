// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#include "Err.h"
#include "resource.h"
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
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_START_SERVER, OnStartServer)
	ON_COMMAND(IDM_STOP_SERVER, OnStopServer)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint() 
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD WINAPI AcptThreadFunc( LPVOID lpParam ) 
{ 
    SOCKET  hServerSocket = (SOCKET)lpParam;

    sockaddr_in addr_in = {0};
    int nLen = sizeof(sockaddr_in);

    SOCKET hConnSocket = NULL;
    int     nRecvLen = 0;
    
    PACKAGE thePack;
    memset( &thePack, 0, sizeof(PACKAGE) );

    while (true)
    {
        hConnSocket = accept(hServerSocket, (sockaddr*)&addr_in, &nLen);
        
        if ( hConnSocket == INVALID_SOCKET)
        {
            ShowError();

            return -1;
        }

        //添加各连接的信息到CSocketInfo
//        theSocketInfo.AddtoMap(connInfo);

        while(true)
        {
            nRecvLen = recv(hConnSocket, (char*)&thePack, sizeof(PACKAGE), 0);

            if ( nRecvLen == 0 || SOCKET_ERROR == nRecvLen)
            {
                ShowError();
            
                return -1;
            }

            if (thePack.opType == ConnSuccess)
            {
                PPACKAGE sendPack = (PPACKAGE)new char[sizeof(PACKAGE) + 6];
                sendPack->opType = GetDisplay;
                sendPack->dwLen = 6;
                strcpy(sendPack->szContext,"Hello");
                send(hConnSocket, (char*)&sendPack, sizeof(PACKAGE),0);
            }
        }
    }
    
    return 0; 
} 

void CServerDlg::OnStartServer() 
{
	// TODO: Add your command handler code here
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
    
    if ( SOCKET_ERROR == bind( m_hSocket, (sockaddr*)&addr, sizeof(sockaddr)) )
    {
        ShowError();
        
        return;
    }

    if ( 0 != listen(m_hSocket, MAXBYTE))
    {
        ShowError();
        
        return;
    }

    DWORD dwAcptThreadId = 0;
    m_hAcptThread = CreateThread(   NULL,
                                    0,
                                    AcptThreadFunc,
                                    (LPVOID)m_hSocket,
                                    0,
                                    &dwAcptThreadId);

    if ( m_hAcptThread == NULL)
    {
        ShowError();
        
        return;
    }
}

void CServerDlg::OnStopServer() 
{
    // TODO: Add your command handler code here
    closesocket(m_hSocket);
}

void CServerDlg::OnContextMenu(CWnd*, CPoint point)
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
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_SERVER_DLG));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CServerDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//    TVITEM item = pNMTreeView->itemNew; 

	*pResult = 0;
}

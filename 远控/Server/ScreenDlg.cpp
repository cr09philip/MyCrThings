// ScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ScreenDlg.h"

#include "struct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog


CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreenDlg::CScreenDlg(CONNECT_INFO connInfo)
{
    memcpy(&m_connInfo, &connInfo, sizeof(CONNECT_INFO));
}

void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg message handlers
DWORD WINAPI RecvProc( LPVOID lpParameter)
{
    PCONNECT_INFO pConnInfo = (PCONNECT_INFO)lpParameter;
    char buf[MAXBYTE] = {0};

    while (true)
    {
        if ( 0 == recv( pConnInfo->hSocket, buf, MAXBYTE, 0))
        {
            ShowError();
            
            return -1;
        }
        
        PPACKAGE pPack = (PPACKAGE) buf;
        if ( pPack->opType == ConnSuccess)
        {
            PACKAGE thePack;
            memset(&thePack, 0, sizeof(PACKAGE) );
            
            thePack.opType = GetDisplay;
            strcpy(thePack.szContext,"Hello");
            thePack.dwLen = strlen(thePack.szContext)+1;
            
            if (SOCKET_ERROR  == send( pConnInfo->hSocket, (char*)&thePack, sizeof(PACKAGE) + thePack.dwLen, 0)   )
            {
                ShowError();
                return -1;
            }
        }
    }   
    return 0;
}

BOOL CScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_hThread = CreateThread(   NULL,                        // no security attributes 
                                0,                           // use default stack size  
                                RecvProc,                  // thread function 
                                &m_connInfo,                // argument to thread function 
                                0,                           // use default creation flags 
                                &m_dwThreadId);                // returns the thread identifier 


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


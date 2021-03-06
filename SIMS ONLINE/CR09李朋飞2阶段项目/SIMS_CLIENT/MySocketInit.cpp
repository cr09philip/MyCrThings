// MySocketInit.cpp: implementation of the CMySocketInit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySocketInit.h"
/*#include "Shlwapi.h"*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySocketInit::CMySocketInit()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}
	
	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup( );
		return; 
	}
}

CMySocketInit::~CMySocketInit()
{
	WSACleanup( );
}

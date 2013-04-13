//struct.h: useful data struct.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITSOCKET_H__5CA7B9231_17C2_403A_85DC_49E06D0F3658__INCLUDED_)
#define AFX_INITSOCKET_H__5CA7B9231_17C2_403A_85DC_49E06D0F3658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WINSOCK2.H>

typedef enum _OPERATE_TYPE
{
    ConnSuccess = 1,
    GetDisplay,
    FarCmd,
    FileManage,
    Test
}OPERATE_TYPE;

typedef struct _CONNECT_INFO
{
    BOOL    isActive;
    DWORD   dwIndex;
    SOCKET  hSocket;
    sockaddr_in addr_in;
    char    szContext[0];
}CONNECT_INFO,*PCONNECT_INFO;

typedef struct _PACKAGE
{
    OPERATE_TYPE opType;
    DWORD   dwLen;
    char    szContext[0];
}PACKAGE,*PPACKAGE;

#endif // !defined(AFX_INITSOCKET_H__5CA7B9231_17C2_403A_85DC_49E06D0F3658__INCLUDED_)

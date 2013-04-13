// MyUDPSocket.h: interface for the CMyUDPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYUDPSOCKET_H__7EFD73CC_A11B_4CD8_9273_7AF21FE6F701__INCLUDED_)
#define AFX_MYUDPSOCKET_H__7EFD73CC_A11B_4CD8_9273_7AF21FE6F701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>
#pragma comment(lib,"Ws2_32.lib")

#include "MySocketInit.h"
#include "MyPacket.h"

enum
{
	SOCKET_SYN = 0,
	SOCKET_ACK,
	SOCKET_FIN
};

#define RESEND_TIMER 100

//报文段结构体
struct  CPacket
{
	CSocketHead m_head;
	char        m_szContent[];
};

class CMyUDPSocket  
{
public:
	CMyUDPSocket();
	virtual ~CMyUDPSocket();
	static CMySocketInit* m_lpInit; 
public:
	SOCKET Socket( int af, int type, int protocol = -1);

	int Bind( const struct sockaddr FAR *name, int namelen);

	int RecvFrom( char FAR* buf, int len, struct sockaddr FAR *from, int FAR *fromlen);

	int SendTo( const char FAR *buf, int len, const struct sockaddr FAR *to, int tolen);

	//请求连接 三次握手

	//套接字
	SOCKET m_hSocket; 
	//发送缓冲区,缓冲区满则封包发送
	char* m_szSendBuf;
	//接收缓冲区,缓冲区满则交付用户
	char* m_szRecvBuf;

	sockaddr_in m_fromAddr;
	sockaddr_in m_toAddr;

//	queue<>;
};

#endif // !defined(AFX_MYUDPSOCKET_H__7EFD73CC_A11B_4CD8_9273_7AF21FE6F701__INCLUDED_)

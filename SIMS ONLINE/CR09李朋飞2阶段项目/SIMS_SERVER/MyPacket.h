// MyPacket.h: interface for the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKET_H__41CFC5FE_69D9_4337_8B92_4F6C836E6C12__INCLUDED_)
#define AFX_MYPACKET_H__41CFC5FE_69D9_4337_8B92_4F6C836E6C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//ADO OPERATE TYPE
#define AD0_ADD		0
#define AD0_DELETE	1
#define AD0_MODIFY	2
#define AD0_SELECT	3
#define ADO_GETTIME 4
#define AD0_TXT		5

struct CSocketHead
{
	DWORD m_dwOperType;
	DWORD m_dwCheckSum;
//	CTime m_time;
};

struct tagPacketContent
{	
	int		m_nLen;
	char	m_szPacket[4];
};

struct CMyPacket  
{
public:
	DWORD UnPacket(void* buf);	
	//outBuf为待发送的数据。outBuf == NULL,则返回需要的缓冲区
	DWORD Packet( void* buf, OUT void* outBuf = NULL, DWORD dwOperate = 0);
private:
	BOOL  CheckCheckSum();
	DWORD GetCheckSum();
	BOOL  SetContent(void* buf);
public:
	CMyPacket();
	~CMyPacket();
public:
	CSocketHead m_head;
	tagPacketContent* m_lpTagContent;
};

#endif // !defined(AFX_MYPACKET_H__41CFC5FE_69D9_4337_8B92_4F6C836E6C12__INCLUDED_)

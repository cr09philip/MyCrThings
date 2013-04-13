// MyPacket.h: interface for the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKET_H__41CFC5FE_69D9_4337_8B92_4F6C836E6C12__INCLUDED_)
#define AFX_MYPACKET_H__41CFC5FE_69D9_4337_8B92_4F6C836E6C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//ADO OPERATE TYPE
#define AD0_TXT		0
#define AD0_ADD		1
#define AD0_DELETE	2
#define AD0_MODIFY	3
#define AD0_SELECT	4

struct CSocketHead
{
	DWORD m_dwOperType;
	DWORD m_dwCheckSum;
	DWORD m_dwPacketLen;
	CTime m_time;
};

class CMyPacket  
{
public:
	DWORD UnPacket();
	DWORD Packet(DWORD dwOperate = 0);
private:
	BOOL  CheckCheckSum();
	DWORD GetCheckSum();
public:
	CMyPacket( BOOL isPacketed);//TRUE为封包，FALSE为解包
	virtual ~CMyPacket();
public:
	BOOL		m_isPacketed;
	CSocketHead m_head;
	CString		m_csContent;
	char		m_szPacket[MAXWORD];//65535
};

#endif // !defined(AFX_MYPACKET_H__41CFC5FE_69D9_4337_8B92_4F6C836E6C12__INCLUDED_)

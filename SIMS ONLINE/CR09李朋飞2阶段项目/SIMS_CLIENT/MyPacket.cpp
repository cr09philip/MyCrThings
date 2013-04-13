// MyPacket.cpp: implementation of the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIMS_CLIENT.h"
#include "MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPacket::CMyPacket( )
{
	memset(&m_head,0,sizeof(CSocketHead));
	m_lpTagContent = NULL;
}

CMyPacket::~CMyPacket()
{
	memset(&m_head,0,sizeof(CSocketHead));
	m_lpTagContent = NULL;
}

//head + content => packet
DWORD CMyPacket::Packet(void* buf, OUT void* outBuf, DWORD dwOperate)
{
	DWORD dwRet = SetContent(buf);

 	m_head.m_dwOperType = dwOperate;
 	m_head.m_dwCheckSum = GetCheckSum();
// 	m_head.m_time = t;
 
	if( outBuf == NULL)
	{
		dwRet = sizeof(CSocketHead) + sizeof(int) + m_lpTagContent->m_nLen;
	}
	else
	{
		memcpy(outBuf, this, sizeof(CSocketHead));
		memcpy( (char*)outBuf + sizeof(CSocketHead), m_lpTagContent, m_lpTagContent->m_nLen + sizeof(int));
	}

	return 	dwRet;
}

//packet => head + content 
DWORD CMyPacket::UnPacket(void* buf)
{
	memcpy(&m_head, buf, sizeof(CSocketHead));
	
	BOOL bRet = SetContent((char*)buf + sizeof(CSocketHead) + sizeof(int));

	if (!CheckCheckSum())
		return 0;

	return bRet;
}

DWORD CMyPacket::GetCheckSum()
{
	DWORD dwCheckSum = 0;

	for ( int i = 0; i < m_lpTagContent->m_nLen; i++)
	{
		dwCheckSum += m_lpTagContent->m_szPacket[i];
	}

	return dwCheckSum;
}

BOOL CMyPacket::CheckCheckSum()
{
	return	(GetCheckSum() == m_head.m_dwCheckSum) ;
}

BOOL  CMyPacket::SetContent(void* buf)
{
	char* szBuf = (char*) buf;

	int nLen = strlen(szBuf);
	int nNeedLen = (nLen + sizeof(char))/(sizeof(tagPacketContent)) + 1;

	m_lpTagContent = new tagPacketContent[nNeedLen];

	if (m_lpTagContent == NULL)
	{
		return FALSE;
	}
	
	m_lpTagContent->m_nLen = nLen + sizeof(char);

	memcpy(m_lpTagContent->m_szPacket, szBuf, m_lpTagContent->m_nLen);	

	return TRUE;
}
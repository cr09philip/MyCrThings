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

CMyPacket::CMyPacket( BOOL isPacketed):m_isPacketed(isPacketed)
{
	memset(&m_head,0,sizeof(CSocketHead));
	m_csContent = "";
	memset(m_szPacket,0,MAXWORD);//65535
}

CMyPacket::~CMyPacket()
{
	memset(&m_head,0,sizeof(CSocketHead));
	m_csContent = "";
	memset(m_szPacket,0,MAXWORD);//65535
}

//head + content => packet
DWORD CMyPacket::Packet(DWORD dwOperate)
{
	if (!m_isPacketed)
		return 0;

	m_head.m_dwOperType = dwOperate;
	m_head.m_dwCheckSum = GetCheckSum();
	m_head.m_dwPacketLen = m_csContent.GetLength() + 1 + sizeof(CSocketHead);

	memcpy( m_szPacket, &m_head, sizeof(CSocketHead));
	memcpy( m_szPacket+sizeof(CSocketHead), m_csContent, m_csContent.GetLength());

	return 1;
}

//packet => head + content 
DWORD CMyPacket::UnPacket()
{
	if (m_isPacketed)
		return 0;
	
	memcpy(&m_head, m_szPacket, sizeof(CSocketHead));
	m_csContent = (LPCTSTR)(m_szPacket + sizeof(CSocketHead));

	return CheckCheckSum();
}

DWORD CMyPacket::GetCheckSum()
{
	DWORD dwCheckSum = 0;
	for (int i = 0; i < m_csContent.GetLength(); i++)
	{
		 dwCheckSum += m_csContent.GetAt(i++);
	}

	return dwCheckSum;
}

BOOL CMyPacket::CheckCheckSum()
{
	if (m_isPacketed)
		return 0;

	return	(GetCheckSum() == m_head.m_dwCheckSum) ;
}

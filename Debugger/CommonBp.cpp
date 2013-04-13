// CommonBp.cpp: implementation of the CCommonBp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonBp.h"

BYTE	g_bBpCode[]	= {0xcc,0xfa,0xfb};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCommonBp* CCommonBp::m_obj	= NULL;
HANDLE CCommonBp::m_hProcess = NULL;

CCommonBp::CCommonBp()
{

}

CCommonBp::~CCommonBp()
{

}

CCommonBp* CCommonBp::CreateObj(HANDLE hProcess)
{
	if ( hProcess == NULL)
	{
		return NULL;
	}

	m_hProcess = hProcess;

	if ( m_obj == NULL)
	{
		m_obj = new CCommonBp;
	}

	return m_obj;
}

int CCommonBp::SetCommonBp(DWORD dwAddr, BYTE bIsOnce, BYTE bBpType, BYTE bIsValid)
{
	//此处需判断该地址是否已有同类断点
	if ( SearchTableForIsHit(dwAddr) != NULL)
	{
		printf("Error :Bp Existed!\r\n");
		return 0;
	}
	
	
	PCOMMON_BP_INFO pBpInfo = new COMMON_BP_INFO;
	
	//保存原数据
	if ( !ReadProcessMemory( m_hProcess, (LPVOID)dwAddr, &pBpInfo->m_bOldData, 1, NULL) )
	{
		return 0;
	}
	
	//改写为CC
	if ( !WriteProcessMemory( m_hProcess, (LPVOID)dwAddr, &g_bBpCode[bBpType], 1, NULL) )
	{
		return 0;
	}
	
	//添加到断点信息列表
	pBpInfo->m_bIsOnce = bIsOnce;
	pBpInfo->m_bBpType = bBpType;
	pBpInfo->m_bIsValid = bIsValid; 
	pBpInfo->m_dwAddr = dwAddr; 
	pBpInfo->m_bDelFlag = 1;

	m_commonBpList.insert( m_commonBpList.end(), pBpInfo);

	return 1;
}

LPVOID CCommonBp::SearchTableForIsHit(DWORD dwAddr)
{
	list<PCOMMON_BP_INFO>::iterator pCount;
	
	for (pCount = m_commonBpList.begin(); pCount != m_commonBpList.end(); ++pCount) 
	{
		if (dwAddr == (*pCount)->m_dwAddr)
		{
			return (LPVOID)(*pCount);
		}
	}
	
	return NULL;
}

BOOL CCommonBp::RemoveCommonBp(DWORD dwAddr)
{
	PCOMMON_BP_INFO pBpInfo = (PCOMMON_BP_INFO)SearchTableForIsHit(dwAddr);

	if ( pBpInfo != NULL)
	{
		//从链表中移除
		m_commonBpList.remove(pBpInfo);
		//释放堆空间
		delete pBpInfo;
		return TRUE;
	}

	return FALSE;
}

void CCommonBp::ListCommonBp()
{
	printf("Addr\t\tIsOnce\tType\tIsValid\r\n");
	list<PCOMMON_BP_INFO>::iterator pCount;
	
	for (	pCount = m_commonBpList.begin(); pCount != m_commonBpList.end();++pCount) 
	{
		printf("%p\t%d\t%d\t%d\r\n",(*pCount)->m_dwAddr,
			(*pCount)->m_bIsOnce,
			(*pCount)->m_bBpType,
			(*pCount)->m_bIsValid);	
	}
}

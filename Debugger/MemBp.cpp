// MemBp.cpp: implementation of the CMemBp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemBp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMemBp* CMemBp::m_obj	= NULL;
HANDLE CMemBp::m_hProcess = NULL;

CMemBp::CMemBp()
{
	
}

CMemBp::~CMemBp()
{

}

CMemBp* CMemBp::CreateObj(HANDLE hProcess)
{
	if ( hProcess == NULL)
	{
		return NULL;
	}
	
	m_hProcess = hProcess;
	
	if ( m_obj == NULL)
	{
		m_obj = new CMemBp;
	}
	
	return m_obj;
}

BOOL CMemBp::SetMemBp( DWORD dwAddr, DWORD dwRange, BOOL bIsWriteBp)
{
	DWORD dwOldProtect = 0;
	
	PMEM_BP_INFO memBp = new MEM_BP_INFO;
	if ( memBp == NULL)
	{
		return FALSE;
	}
	
	PBP_PAGE_INFO bpPage = new BP_PAGE_INFO;
	if ( bpPage == NULL)
	{
		if ( memBp != NULL)
			delete memBp;
		
		return FALSE;
	}
	
	//搜索PageTable	
	PMEMORY_BASIC_INFORMATION lpPageInfo = (PMEMORY_BASIC_INFORMATION)SearchPageTableByBaseAddr(dwAddr);
	
	if ( lpPageInfo == NULL)
	{
		if ( memBp != NULL) 
			delete memBp;
		if ( bpPage != NULL) 
			delete bpPage;
		
		return FALSE;
	}
	
	//添加到断点表
	memBp->m_dwIndex = m_MemBpTable.size()+1;
	memBp->m_dwBpAddr = dwAddr;
	memBp->m_dwBpSize = dwRange;
	memBp->m_dwBpType = bIsWriteBp;
	m_MemBpTable.insert( m_MemBpTable.end(), memBp);	
	
	//增加相应的BpPageTable项
	bpPage->lpMemBpInfo = memBp;
	bpPage->lpPageInfo = lpPageInfo;
	bpPage->dwOldProtect = lpPageInfo->Protect;
	m_BpPageTable.insert( m_BpPageTable.end(),bpPage);
	
	if ( !bIsWriteBp)//读断点
	{
		//更改内存属性为读断点(PAGE_NOACCESS)
		if (!VirtualProtectEx( m_hProcess, (LPVOID)dwAddr, dwRange, PAGE_NOACCESS, &dwOldProtect))
		{
			if ( memBp != NULL) 
				delete memBp;
			if ( bpPage != NULL) 
				delete bpPage;
			
			return FALSE;
		}
		
		if ( dwOldProtect != lpPageInfo->Protect)
		{
			if ( memBp != NULL) 
				delete memBp;
			if ( bpPage != NULL) 
				delete bpPage;
			
			return FALSE;
		}
	}
	else//写断点
	{
		//更改内存属性为读断点(PAGE_NOACCESS)
		if ( !VirtualProtectEx( m_hProcess, (LPVOID)dwAddr, dwRange, PAGE_READONLY, &dwOldProtect))
		{
			if ( memBp != NULL) 
				delete memBp;
			if ( bpPage != NULL) 
				delete bpPage;
			
			return FALSE;
		}
		
		if ( dwOldProtect != lpPageInfo->Protect)
		{
			if ( memBp != NULL) 
				delete memBp;
			if ( bpPage != NULL) 
				delete bpPage;
			
			return FALSE;
		}
	}

	return TRUE;	
}

DWORD CMemBp::RemoveMemBp(DWORD dwIndex)
{
	//首先删除BP_PAGE_INFO中的项
	if ( !RemoveBpPageNode(dwIndex))
	{
		return 0;
	}
	
	//删除MEM_BP_INFO
	PMEM_BP_INFO pInfo = NULL;
	
	if ( (pInfo = (PMEM_BP_INFO)SearchMemBpTableByIndex(dwIndex)) != NULL)
	{
		m_MemBpTable.remove(pInfo);
		delete pInfo;
	}
	
	return 1;
}

DWORD CMemBp::RemoveBpPageNode(DWORD dwIndex)
{
	DWORD dwTmpProtect = 0;
	list<PBP_PAGE_INFO>::iterator pCount;
	
	for (pCount = m_BpPageTable.begin(); pCount != m_BpPageTable.end(); ++pCount) 
	{
		//查找符合索引值的所有BP_PAGE_INFO项
		if ( dwIndex == (DWORD)((*pCount)->lpMemBpInfo->m_dwIndex))
		{
			//恢复oldprotect
			if ( !VirtualProtectEx( m_hProcess, (*pCount)->lpPageInfo->BaseAddress,
				(*pCount)->lpPageInfo->RegionSize,(*pCount)->dwOldProtect, &dwTmpProtect) )
			{
				return 0;
			}
			//删除BP_PAGE_INFO项
			if ( (*pCount)->lpPageInfo != NULL)
			{
				//从链表中移除
				m_PageTable.remove( (*pCount)->lpPageInfo );
				//释放掉内存
				delete (*pCount)->lpPageInfo;
			}
		}
	}
	
	return 1;
}

LPVOID CMemBp::SearchMemBpTableByIndex(DWORD dwIndex)
{
	RefreshMemBpTable();
	list<PMEM_BP_INFO>::iterator pCount;
	
	for (pCount = m_MemBpTable.begin(); pCount != m_MemBpTable.end(); ++pCount) 
	{
		if ( dwIndex == (*pCount)->m_dwIndex )
		{
			return (*pCount);
		}
	}
	
	return NULL;
}

LPVOID CMemBp::SearchPageTableByBaseAddr(DWORD dwAddr)
{
	RefreshMemBpTable();
	list<PMEMORY_BASIC_INFORMATION>::iterator pCount;
	
	for (pCount = m_PageTable.begin(); pCount != m_PageTable.end(); ++pCount) 
	{
		DWORD dwBaseAddr = (DWORD)(*pCount)->BaseAddress;
		DWORD dwEndAddr = dwBaseAddr + (*pCount)->RegionSize;
		
		//目标地址大于分页基址 小于基址+范围，则在此分页下断
		if ( dwAddr >= dwBaseAddr && dwAddr < dwEndAddr )
		{
			return (LPVOID)(*pCount);
		}
	}
	
	return NULL;
}

void CMemBp::RefreshMemBpTable()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	
	DWORD dwStartAddr = (DWORD)info.lpMinimumApplicationAddress;	
	DWORD dwEndAddr = (DWORD)info.lpMaximumApplicationAddress;
	
	DWORD dwAddr = dwStartAddr;
	DWORD dwIndex = 0;
	
	while (dwAddr <= dwEndAddr)
	{
		PMEMORY_BASIC_INFORMATION pMemInfo = new MEMORY_BASIC_INFORMATION;
		
		if (pMemInfo == NULL)
		{
			break;
		}
		
		if ( VirtualQueryEx( m_hProcess,
							(LPVOID)dwAddr,                   // address of region
							pMemInfo,  // information buffer
							sizeof(MEMORY_BASIC_INFORMATION) ) == 0)
		{
			if (pMemInfo != NULL)
			{
				delete pMemInfo;
				pMemInfo = NULL;
			}
			break;
		}
		
		DWORD dwOldPtr = dwAddr;
		dwAddr = (DWORD)pMemInfo->BaseAddress + pMemInfo->RegionSize;
		
		m_PageTable.insert(m_PageTable.end(), pMemInfo);
		
		if(dwAddr <= dwOldPtr)   // prevent unendable iteration
            break;
	}
}

BOOL CMemBp::IsHit(DWORD dwAddr, BOOL bIsWriteBp)
{
	RefreshMemBpTable();
	list<PBP_PAGE_INFO>::iterator pCount;
	
	for (pCount = m_BpPageTable.begin(); pCount != m_BpPageTable.end(); ++pCount) 
	{
		DWORD dwBaseAddr = (DWORD)(*pCount)->lpPageInfo->BaseAddress;
		DWORD dwEndAddr = dwBaseAddr + (*pCount)->lpPageInfo->RegionSize;
		
		if ( dwAddr >= dwBaseAddr && dwAddr < dwEndAddr && bIsWriteBp == (*pCount)->lpMemBpInfo->m_dwBpType)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

PBP_PAGE_INFO CMemBp::IsInMemBpPage(DWORD dwAddr)
{
	RefreshMemBpTable();
	list<PBP_PAGE_INFO>::iterator pCount;
	
	//此处判断有问题
	for (pCount = m_BpPageTable.begin(); pCount != m_BpPageTable.end(); ++pCount) 
	{
		DWORD dwBaseAddr = (DWORD)((*pCount)->lpPageInfo->BaseAddress);
		DWORD dwEndAddr  = dwBaseAddr + (*pCount)->lpPageInfo->RegionSize;
		
		if ( dwAddr >= dwBaseAddr && dwAddr < dwEndAddr)
		{
			return (*pCount);
		}
	}
	
	return NULL;
}

void CMemBp::ListMemBp()
{
	RefreshMemBpTable();
	printf("Index\tAddr\tRange\tType\r\n");
	list<PMEM_BP_INFO>::iterator pCount;
	
	//此处判断有问题
	for (pCount = m_MemBpTable.begin(); pCount != m_MemBpTable.end(); ++pCount) 
	{
		printf("%d\t%p\t%d\t%d\r\n",(*pCount)->m_dwIndex,
									(*pCount)->m_dwBpAddr,
									(*pCount)->m_dwBpSize,
									(*pCount)->m_dwBpType);	
	}
	
}

// MemBp.h: interface for the CMemBp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBP_H__07A3B8A2_D8DE_4C64_8398_12BCD8B9017E__INCLUDED_)
#define AFX_MEMBP_H__07A3B8A2_D8DE_4C64_8398_12BCD8B9017E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <algorithm>
using namespace std;

// �ڴ�ϵ���Ϣ�ṹ��
typedef struct tagMemBP
{
	DWORD	m_dwIndex;
	DWORD   m_dwBpAddr;             // �ϵ��ַ
	DWORD   m_dwBpSize;             // �ϵ㳤��
	DWORD   m_dwBpType;             // �ϵ����� 0:���쳣 1:д�쳣
	//	DWORD	m_dwOldProtect;
}MEM_BP_INFO,*PMEM_BP_INFO;

//�м��
typedef struct tagBpPage
{
	PMEM_BP_INFO lpMemBpInfo;
	PMEMORY_BASIC_INFORMATION lpPageInfo;
	DWORD	dwOldProtect;
}BP_PAGE_INFO,*PBP_PAGE_INFO;

//�ڴ��ҳ��Ϣ��
// typedef struct _MEMORY_BASIC_INFORMATION {
//     PVOID BaseAddress;
//     PVOID AllocationBase;
//     DWORD AllocationProtect;
//     SIZE_T RegionSize;
//     DWORD State;
//     DWORD Protect;
//     DWORD Type;
// } MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

class CMemBp  
{
public:
	CMemBp();
	virtual ~CMemBp();
public:
	static CMemBp* CreateObj(HANDLE hProcess);
private:
	static CMemBp* m_obj;
	static HANDLE m_hProcess;
public:
	LPVOID	SearchPageTableByBaseAddr(DWORD dwAddr);
	LPVOID	SearchMemBpTableByIndex(DWORD dwIndex);
	DWORD	RemoveBpPageNode(DWORD dwIndex);
	DWORD	RemoveMemBp(DWORD dwIndex);
	BOOL	SetMemBp(  DWORD dwAddr, DWORD dwRange, BOOL bIsWriteBp);
public:
	void ListMemBp();
	PBP_PAGE_INFO IsInMemBpPage(DWORD dwAddr);
	BOOL IsHit( DWORD dwAddr,BOOL bIsWriteBp);
	void RefreshMemBpTable();
	list<PMEMORY_BASIC_INFORMATION> m_PageTable;//�����ڴ����� ����ΪBaseAddress
	list<PMEM_BP_INFO> m_MemBpTable;//�ڴ�ϵ���Ϣ��
	list<PBP_PAGE_INFO> m_BpPageTable;//�м��
};

#endif // !defined(AFX_MEMBP_H__07A3B8A2_D8DE_4C64_8398_12BCD8B9017E__INCLUDED_)

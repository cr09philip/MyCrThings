// CommonBp.h: interface for the CCommonBp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONBP_H__8A472EA8_6B19_4004_A310_065BD13DB596__INCLUDED_)
#define AFX_COMMONBP_H__8A472EA8_6B19_4004_A310_065BD13DB596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <algorithm>
using namespace std;

extern BYTE	g_bBpCode[];

typedef struct tagCommonBpInfo
{
	BYTE m_bDelFlag; //0:del 1:un-del
	BYTE m_bIsOnce:4;//0:always 1:once
	BYTE m_bBpType:4;//0:int3 1:mem bp 2:hardware bp
	BYTE m_bIsValid;//0:Invalid 1:valid
	BYTE m_bOldData;
	DWORD m_dwAddr;
}COMMON_BP_INFO,*PCOMMON_BP_INFO;

class CCommonBp  
{
public:
	CCommonBp();
	virtual ~CCommonBp();
public:
	BOOL	RemoveCommonBp(DWORD dwAddr);
	LPVOID	SearchTableForIsHit(DWORD dwAddr);
	int		SetCommonBp(DWORD dwAddr,	BYTE bIsOnce = 1,BYTE bBpType = 0,BYTE bIsValid = 1);
	static	CCommonBp* CreateObj(HANDLE hProcess);
private:
	static CCommonBp* m_obj;
	static HANDLE m_hProcess;
public:
	void ListCommonBp();
	list<PCOMMON_BP_INFO> m_commonBpList;
};

#endif // !defined(AFX_COMMONBP_H__8A472EA8_6B19_4004_A310_065BD13DB596__INCLUDED_)

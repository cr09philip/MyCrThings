// HardBp.h: interface for the CHardBp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDBP_H__1800D830_24C0_483C_B92B_72FC14A6B034__INCLUDED_)
#define AFX_HARDBP_H__1800D830_24C0_483C_B92B_72FC14A6B034__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

typedef union tagREG_DR7
{
	DWORD dwValue;
	struct
	{
		BYTE L0			: 1;
		BYTE G0			: 1;
		BYTE L1			: 1;
		BYTE G1			: 1;
		BYTE L2			: 1;
		BYTE G2			: 1;
		BYTE L3			: 1;
		BYTE G3			: 1;
		BYTE LE			: 1;
		BYTE GE			: 1;
		BYTE Reserve1	: 3;
		BYTE GD			: 1;
		BYTE Reserve2	: 2;
		BYTE RW0		: 2;
		BYTE LEN0		: 2;
		BYTE RW1		: 2;
		BYTE LEN1		: 2;
		BYTE RW2		: 2;
		BYTE LEN2		: 2;
		BYTE RW3		: 2;
		BYTE LEN3		: 2;
	}tagDR7;
}DR7,*PDR7;

class CHardBp  
{
public:
	CHardBp();
	virtual ~CHardBp();
public:
	void ListHardBp(CONTEXT* pContext);
	BOOL RemoveHardBp(DWORD dwIndex,CONTEXT* pContext);

	//bAccess 00:只执行 01:写入断点 10:I/O端口断点 11:4读或写断点
	//bLen 00:1字节 01:2字节 10:保留 11:4字节
	BOOL SetHardBp( DWORD dwAddr, BYTE bAccess, BYTE bLen, CONTEXT* pContext);
	static CHardBp* CreateObj(HANDLE hThread);
public:
	static DWORD m_dwCurHardBpIndex;
private:
	static CHardBp* m_obj;
	static HANDLE m_hThread;
};

#endif // !defined(AFX_HARDBP_H__1800D830_24C0_483C_B92B_72FC14A6B034__INCLUDED_)

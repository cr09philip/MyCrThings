// HardBp.cpp: implementation of the CHardBp class.
//
//////////////////////////////////////////////////////////////////////

/* ==========================================================================
Class :			CHardBp

Author :		CR09-Philip

Date :			2011-08-27

Purpose :		
	
Description :	

Usage :			

   ========================================================================*/

#include "stdafx.h"
#include "HardBp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHardBp* CHardBp::m_obj = NULL;
HANDLE   CHardBp::m_hThread = NULL;
DWORD	 CHardBp::m_dwCurHardBpIndex = 0;

char g_szAccess[][16] = { "EXCUTE_ONLY_BP", "WRITE_BP", "Reserved" ,"READ_WRITE_BP"};

/* ============================================================
	Function :		CHardBp::CHardBp
	Description :	Constructor
	Access :		Public
	Return :		void
	Parameters :	none
	Usage :			Should normally be created on the stack.
   ============================================================*/
CHardBp::CHardBp()
{

}

/* ============================================================
	Function :		CHardBp::~CHardBp
	Description :	Destructor
	Access :		Public
	Return :		void
	Parameters :	none
	Usage :			Should normally be created on the stack.
   ============================================================*/
CHardBp::~CHardBp()
{

}

/* ============================================================
	Function :		CHardBp::CreateObj
	Description :	Get the single obj pointer
	Access :		Public
	Return :		CHardBp*		-	return the point to this
					"GetErrorString" will return errors.

	Parameters :	HANDLE hThread	- handle of the target thread
					if NULL, return NULL
	Usage :			
   ============================================================*/
CHardBp* CHardBp::CreateObj( HANDLE hThread)
{
	if ( hThread == NULL)
	{
		return NULL;
	}

	m_hThread = hThread;

	if ( m_obj == NULL)
	{
		m_obj = new CHardBp;
	}
	
	return m_obj;
}

/* ============================================================
	Function :		CHardBp::SetHardBp
	Description :	Set Hard Bp,and modify to thread context
	Access :		Public
	Return :		BOOL			-	"TRUE" if OK.
					"GetErrorString" will return errors.

	Parameters :	DWORD dwAddr	- the Addr of Bp,write to DR0~DR3
					BYTE bAccess	- 00: 1 only Execute 
									- 01: 2 Write Bp 
									- 10: 3 I/O port	
									- 11: 4 Write or Read Bp
					BYTE bLen		- 00: 1 byte		
									- 01: 2 bytes 
									- 10: 3	Reserved	
									- 11: 4 bytes
	Usage :			
   ============================================================*/
BOOL CHardBp::SetHardBp(DWORD dwAddr, BYTE bAccess, BYTE bLen, CONTEXT* pContext)
{
// 	if ( m_hThread == NULL)
// 	{
// 		return FALSE;
// 	}

	if ( bAccess != 0 && bAccess != 1 && bAccess != 3)
	{
		return FALSE;
	}

	if ( bLen != 0 && bLen != 1 && bLen != 3)
	{
		return FALSE;
	}

// 	CONTEXT context = {0};
// 	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
// 
// 	if ( !GetThreadContext( m_hThread, &context) )
// 	{
// 		return FALSE;
// 	}

	DR7 dr7 = {0};
	dr7.dwValue = pContext->Dr7;
	dr7.tagDR7.Reserve1 = 001;
	dr7.tagDR7.LE = 1;
	dr7.tagDR7.GE = 1;

	//如果0位置为无效硬件断点，设在0处
	if ( dr7.tagDR7.G0 != 1 && dr7.tagDR7.L0 != 1)	
	{
		pContext->Dr0 = dwAddr;
		dr7.tagDR7.L0 = 1;

		dr7.tagDR7.RW0 = bAccess;
		if ( bAccess == 00 )
		{
			dr7.tagDR7.LEN0 = 0;
		}
		else
		{
			dr7.tagDR7.LEN0 = bLen;
		}

		pContext->Dr7 = dr7.dwValue;
// 		if ( !SetThreadContext( m_hThread, &context) )
// 		{
// 			return FALSE;
// 		}
		
		return TRUE;
	}

	if ( dr7.tagDR7.G1 != 1 && dr7.tagDR7.L1 != 1)	
	{
		pContext->Dr1 = dwAddr;
		dr7.tagDR7.L1 = 1;
		
		dr7.tagDR7.RW1 = bAccess;
		if ( bAccess == 00 )
		{
			dr7.tagDR7.LEN1 = 0;
		}
		else
		{
			dr7.tagDR7.LEN1 = bLen;
		}

		pContext->Dr7 = dr7.dwValue;
// 		if ( !SetThreadContext( m_hThread, &context) )
// 		{
// 			return FALSE;
// 		}
		
		return TRUE;
	}

	if ( dr7.tagDR7.G2 != 1 && dr7.tagDR7.L2 != 1)	
	{
		pContext->Dr2 = dwAddr;
		dr7.tagDR7.L2 = 1;
		
		dr7.tagDR7.RW2 = bAccess;
		if ( bAccess == 00 )
		{
			dr7.tagDR7.LEN2 = 0;
		}
		else
		{
			dr7.tagDR7.LEN2 = bLen;
		}

		pContext->Dr7 = dr7.dwValue;
// 		if ( !SetThreadContext( m_hThread, &context) )
// 		{
// 			return FALSE;
// 		}
		
		return TRUE;
	}

	if ( dr7.tagDR7.G3 != 1 && dr7.tagDR7.L3 != 1)	
	{
		pContext->Dr3 = dwAddr;
		dr7.tagDR7.L3 = 1;
		
		dr7.tagDR7.RW3 = bAccess;
		if ( bAccess == 00 )
		{
			dr7.tagDR7.LEN3 = 0;
		}
		else
		{
			dr7.tagDR7.LEN3 = bLen;
		}

		pContext->Dr7 = dr7.dwValue;
// 		if ( !SetThreadContext( m_hThread, &context) )
// 		{
// 			return FALSE;
// 		}

		return TRUE;
	}
	
	return FALSE;
}

/* ============================================================
	Function :		CHardBp::RemoveHardBp
	Description :	Set the target DRX status to 0 in the thread context
	Access :		Public
	Return :		BOOL			-	"TRUE" if OK.
					"GetErrorString" will return errors.

	Parameters :	DWORD dwIndex	- DRX 0~3 means DR0~DR3
					if not in this range(0~3) returns FALSE
	Usage :			
   ============================================================*/
BOOL CHardBp::RemoveHardBp(DWORD dwIndex,CONTEXT* pContext)
{
	if ( dwIndex < 0 || dwIndex > 3)
	{
		return FALSE;
	}

	BOOL bRet = TRUE;
	
// 	CONTEXT context = {0};
// 	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
// 	
// 	if ( !GetThreadContext( m_hThread, &context) )
// 	{
// 		return FALSE;
// 	}
	
	DR7 dr7 = {0};
	dr7.dwValue = pContext->Dr7;
	
	switch(dwIndex)
	{
	case 0:
		dr7.tagDR7.G0 = 0;
		dr7.tagDR7.L0 = 0;
		break;
	case 1:
		dr7.tagDR7.G1 = 0;
		dr7.tagDR7.L1 = 0;
		break;
	case 2:
		dr7.tagDR7.G2 = 0;
		dr7.tagDR7.L2 = 0;
		break;
	case 3:
		dr7.tagDR7.G3 = 0;
		dr7.tagDR7.L3 = 0;
		break;
	default:
		bRet = FALSE;
		break;
	}
	
	pContext->Dr7 = dr7.dwValue;
	
// 	if ( !SetThreadContext( m_hThread, &context) )
// 	{
// 		return FALSE;
// 	}
// 	
	return bRet;
}

/* ============================================================
	Function :		CHardBp::ListHardBp
	Description :	Show the Hard BP list
	Access :		Public
	Return :		void			- no returns.
					"GetErrorString" will return errors.

	Parameters :	none.
	Usage :			
   ============================================================*/
void CHardBp::ListHardBp(CONTEXT* pContext)
{
// 	CONTEXT context = {0};
// 	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
// 	
// 	if ( !GetThreadContext( m_hThread, &context) )
// 	{
// 		return;
// 	}
	
	DR7 dr7 = {0};
	dr7.dwValue = pContext->Dr7;
	
	printf("=================HARD BP===================\r\n");
	printf("Index\tAddr\t\tAccess\t\tLen\r\n");
	
	if ( dr7.tagDR7.G0 == 1 || dr7.tagDR7.L0 == 1)	
	{
		printf("0:\t%p\t%s\t%d\r\n", pContext->Dr0,
			g_szAccess[dr7.tagDR7.RW0],
			dr7.tagDR7.LEN0 + 1);
	}
	
	if ( dr7.tagDR7.G1 == 1 || dr7.tagDR7.L1 == 1)	
	{
		printf("1:\t%p\t%s\t%d\r\n", pContext->Dr1,
			g_szAccess[dr7.tagDR7.RW1],
			dr7.tagDR7.LEN1 + 1);
	}
	
	if ( dr7.tagDR7.G2 == 1 || dr7.tagDR7.L2 == 1)	
	{
		printf("2:\t%p\t%s\t%d\r\n", pContext->Dr2,
			g_szAccess[dr7.tagDR7.RW2],
			dr7.tagDR7.LEN2 + 1);
	}
	
	if ( dr7.tagDR7.G3 == 1 || dr7.tagDR7.L3 == 1)	
	{
		printf("3:\t%p\t%s\t%d\r\n", pContext->Dr3,
			g_szAccess[dr7.tagDR7.RW3],
			dr7.tagDR7.LEN3 + 1);
	}
	
	printf("===========================================\r\n");
}

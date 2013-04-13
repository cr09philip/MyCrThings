// DebuggerUi.cpp: implementation of the CDebuggerUi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DebuggerUi.h"

#include <stdlib.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HANDLE CDebuggerUi::m_hProcess = NULL;
CDebuggerUi* CDebuggerUi::m_obj = NULL;

CDebuggerUi::CDebuggerUi()
{

}

CDebuggerUi::~CDebuggerUi()
{

}

CDebuggerUi* CDebuggerUi::CreateObj(HANDLE hProcess)
{
	if ( hProcess == NULL)
	{
		return NULL;
	}

	m_hProcess = hProcess;

	if ( m_obj == NULL)
	{
		m_obj =  new CDebuggerUi;
	}

	return m_obj;	
}

// void CDebuggerUi::WaitForUserCommand(CONTEXT* pContext)
// {		
// 	CMD cmd = {0};
// 	
// 	GetCommand(&cmd);
// }

BOOL CDebuggerUi::GetCommand(PCMD pCmd)
{
	printf("-");
	char szBuf[MAXBYTE] = {0};
	DWORD dwIndex = 0;
	fflush(stdin);
	while( (szBuf[dwIndex] = getchar()) != '\n')
	{
		//允许命令最大字节数32
		if (dwIndex >= 0x20)
		{
			break;
		}

		dwIndex++;
	}
	
	TranslateInputToCMD(szBuf, dwIndex, pCmd);

	return TRUE;
}

BOOL CDebuggerUi::TranslateInputToCMD(char *szInput, DWORD dwBufLen, PCMD pCmd)
{
	char* pStopString = 0;

	sscanf(szInput,"%s %s %s %s\n", pCmd->szCmd, pCmd->szArgu1, pCmd->szArgu2, pCmd->szArgu3);

// 	pCmd->dwArgu1 = strtol(szBuf1, &pStopString, 16); 
// 	pCmd->dwArgu2 = strtol(szBuf2, &pStopString, 16);
// 	pCmd->dwArgu3 = strtol(szBuf3, &pStopString, 16);

	return TRUE;
}

void CDebuggerUi::ShowRegEnvironment(CONTEXT *pContext, map<DWORD,char*> iatMap)
{
	printf("=============================================================================\r\n");
    printf("EAX=%p EBX=%p ECX=%p EDX=%p ESI=%p EDI=%p\r\n",pContext->Eax, pContext->Ebx, 
				pContext->Ecx, pContext->Edx, pContext->Esi, pContext->Edi);
	
    printf("ESP=%p EBP=%p\t\t\t  OF DF IF SF ZF AF PF CF\r\n", 
				pContext->Esp, pContext->Ebp);
	
    printf("CS=%0.4X\tSS=%0.4X\tDS=%0.4X\tES=%0.4X\tFS=%0.4X\tGS=%0.4X",
				pContext->SegCs, pContext->SegSs, pContext->SegDs, 
				pContext->SegEs, pContext->SegFs, pContext->SegGs );
	
    printf("\t  %d  %d  %d  %d  %d  %d  %d  %d\r\n", (pContext->EFlags & 0x0800)?1:0,
													(pContext->EFlags & 0x0400)?1:0,
													(pContext->EFlags & 0x0200)?1:0,
													(pContext->EFlags & 0x0080)?1:0,
													(pContext->EFlags & 0x0040)?1:0,
													(pContext->EFlags & 0x0010)?1:0,
													(pContext->EFlags & 0x0004)?1:0,
													(pContext->EFlags & 0x0001)?1:0 );
	
	BYTE szCode[20] = {0};
	t_disasm da = {0};
	//读目标进程的代码段
	if ( !ReadProcessMemory( m_hProcess,(LPVOID)pContext->Eip,
		szCode, 20, NULL)  )
	{
		return;
	}
	
    int nLen = Disasm(szCode, 20, 0, &da, DISASM_CODE);
	
	if ( da.cmdtype == 0x70 && da.adrconst != 0)
	{
		DWORD dwFuncPtr = 0;
		if ( !ReadProcessMemory( m_hProcess,(LPVOID)(da.adrconst),
			&dwFuncPtr, 4, NULL)  )
		{
			return;
		}
		BOOL bFound = FALSE;
		map<DWORD,char*>::iterator it;
		for( it = iatMap.begin(); it != iatMap.end(); ++it)
		{
			if ( it->first == dwFuncPtr )
			{
				bFound = TRUE;
				break;
			}
		}

		if (bFound)
		{
			printf("EIP=%p\t%s\t%s [%s]\r\n", pContext->Eip, da.dump, da.result,it->second);
		}
		else
		{
			printf("EIP=%p\t%s\t%s [<null>]\r\n", pContext->Eip, da.dump, da.result);
		}
	}
	else
	{
		printf("EIP=%p\t%s\t%s\r\n", pContext->Eip, da.dump, da.result);
	}
	
	printf("=============================================================================\r\n");	
}

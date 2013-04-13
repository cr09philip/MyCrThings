// DebugFrame.cpp: implementation of the CDebugFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DebugFrame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
pOpenThread OpenThread;

CDebugFrame::CDebugFrame()
{
	m_hProcess		= NULL;

	m_pCurCommonBp	= NULL;
	m_pCurMemBp		= NULL;

	m_bIsBeginBp	= TRUE;

	m_pObjUi		= NULL;
	m_pObjData		= NULL;
	m_pObjProc	= NULL;
	
	m_bIsStepHardBp	= FALSE;
	m_bIsStepMemBp	= FALSE;
	m_bIsStepCommonBp	= FALSE;
//	m_bIsStepTP		= FALSE;

	m_bIsRTing		= FALSE;
	
	m_hDll = LoadLibrary("kernel32.dll");
	
	if (m_hDll == NULL)
	{
		printf("LoadLibrary error:can't GetProcAddr of OpenThread");
	}

	OpenThread = (pOpenThread)GetProcAddress(m_hDll, "OpenThread");
}

CDebugFrame::~CDebugFrame()
{
	if ( m_hProcess != NULL)
	{
		CloseHandle(m_hProcess);
		m_hProcess = NULL;
	}

	if ( m_hDll != NULL)
	{
		FreeLibrary(m_hDll);
	}

	if (m_pObjProc != NULL)
	{
		delete m_pObjProc;
	}
	
}

void CDebugFrame::BeginDebug( char* szName)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;	
	DEBUG_EVENT dbg;
	
	memset( &si, 0, sizeof(si));
	memset( &pi, 0, sizeof(pi));
	memset( &dbg, 0, sizeof(dbg));
	
	BOOL bRet = CreateProcess(  NULL, 
								szName,       // command line 
								NULL,          // process security attributes 
								NULL,          // primary thread security attributes 
								TRUE,          // handles are inherited 
								DEBUG_PROCESS,             // creation flags 
								NULL,          // use parent's environment 
								NULL,          // use parent's current directory 
								&si,  // STARTUPINFO pointer 
								&pi);  // receives PROCESS_INFORMATION 
	
	if (!bRet)
	{
		return;
	}
	
	dbg.dwProcessId = pi.dwProcessId;
	dbg.dwThreadId = pi.dwThreadId;
	
	while ( WaitForDebugEvent(&dbg,INFINITE) )
	{
		DWORD dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
		
		switch(dbg.dwDebugEventCode)
		{
		case EXCEPTION_DEBUG_EVENT:
			EXCEPTION_DEBUG_INFO;
			dwContinueStatus = DbgException(&dbg);
			break;
		case CREATE_THREAD_DEBUG_EVENT:
			CREATE_THREAD_DEBUG_INFO;
			dwContinueStatus = DbgCreateThread(&dbg);
			break;
		case CREATE_PROCESS_DEBUG_EVENT:
			CREATE_PROCESS_DEBUG_INFO;
			dwContinueStatus = DbgCreateProcess(&dbg);
			break;
		case EXIT_THREAD_DEBUG_EVENT:
			EXIT_THREAD_DEBUG_INFO;
			dwContinueStatus = DbgExitThread(&dbg);
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			EXIT_PROCESS_DEBUG_INFO;
			dwContinueStatus = DbgExitProcess(&dbg);
			break;
		case LOAD_DLL_DEBUG_EVENT:
			LOAD_DLL_DEBUG_INFO;
			dwContinueStatus = DbgLoadDll(&dbg);
			break;
		case UNLOAD_DLL_DEBUG_EVENT:
			UNLOAD_DLL_DEBUG_INFO;
			dwContinueStatus = DbgUnLoadDll(&dbg);
			break;
		case OUTPUT_DEBUG_STRING_EVENT:
			OUTPUT_DEBUG_STRING_INFO;
			dwContinueStatus = DbgOutPutStrting(&dbg);
			break;
		case RIP_EVENT:
			RIP_INFO;
			dwContinueStatus = DbgRip(&dbg);
			break;
		default:
			break;
		}
		
		if ( !ContinueDebugEvent( dbg.dwProcessId, dbg.dwThreadId, dwContinueStatus) )
		{
			break;
		}
	}
}

int CDebugFrame::DbgException(DEBUG_EVENT *pDbg)
{
	EXCEPTION_DEBUG_INFO info = pDbg->u.Exception;
	
	int nRet = DBG_CONTINUE;
	
	switch(info.ExceptionRecord.ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		nRet = EcptAccessViolation(pDbg);
		break;		
		
	case EXCEPTION_BREAKPOINT: 
		nRet = EcptBreakPoint(pDbg);
		break;
		
	case STATUS_PRIVILEGED_INSTRUCTION: 
		nRet = EcptPrivilegedInstruction(pDbg);
		break;
		
	case EXCEPTION_SINGLE_STEP: 
		nRet = EcptSingleStep(pDbg);
		break;
		
	case DBG_CONTROL_C: 
		nRet = EcptControlC(pDbg);
		break;
	default:
		nRet = DBG_CONTINUE;
	}
	
	return nRet;
}

int CDebugFrame::DbgCreateThread(DEBUG_EVENT *pDbg)
{	
	CREATE_THREAD_DEBUG_INFO info = pDbg->u.CreateThread;

//	MessageBox(NULL,"DbgCreateThread","Debug",MB_OK);
	return DBG_CONTINUE;
}

int CDebugFrame::DbgCreateProcess(DEBUG_EVENT *pDbg)
{
	CREATE_PROCESS_DEBUG_INFO info = pDbg->u.CreateProcessInfo;
	
	m_hProcess = info.hProcess;
	m_pObjUi = m_pObjUi->CreateObj(m_hProcess);
	m_pObjData = m_pObjData->CreateObj(m_hProcess);
	m_pObjProc = new CDebugProc(m_hProcess);

	HANDLE hThread = OpenThread(PROCESS_ALL_ACCESS, FALSE, pDbg->dwThreadId);
	if (hThread == NULL)
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
	//初始化断点类
	m_pObjData->m_pObjCommonBp = m_pObjData->m_pObjCommonBp->CreateObj(m_hProcess);
	m_pObjData->m_pObjMemBp	= m_pObjData->m_pObjMemBp->CreateObj(m_hProcess);
	m_pObjData->m_pObjHardBp = m_pObjData->m_pObjHardBp->CreateObj(hThread);
	
	//在OEP中下一次性断点
	if ( !m_pObjData->m_pObjCommonBp->SetCommonBp	( (DWORD)info.lpStartAddress, 1, 0, 1) )
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	return DBG_CONTINUE;
}

int CDebugFrame::DbgExitThread(DEBUG_EVENT *pDbg)
{
	EXIT_THREAD_DEBUG_INFO info = pDbg->u.ExitThread;
	
//	MessageBox(NULL,"DbgExitThread","Debug",MB_OK);
	return DBG_CONTINUE;
}

int CDebugFrame::DbgExitProcess(DEBUG_EVENT *pDbg)
{
	EXIT_PROCESS_DEBUG_INFO info = pDbg->u.ExitProcess;
	
//	MessageBox(NULL,"DbgExitProcess","Debug",MB_OK);
	return DBG_CONTINUE;
}

int CDebugFrame::DbgLoadDll(DEBUG_EVENT *pDbg)
{
	LOAD_DLL_DEBUG_INFO info = pDbg->u.LoadDll;
	
//	m_pObjData->InitIatMap();
	//此处实现添加模块到模块链表or遍历PEB里面的LDR结构体链表

	//
	DWORD dwRead = 0;
	LPVOID lpAddr = NULL;
	WCHAR szWideBuf[MAX_PATH*2] = {0};
	char szMultiBuf[MAX_PATH] = {0};
	
	if ( m_hProcess == NULL)
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
	if (!ReadProcessMemory(m_hProcess, info.lpImageName, &lpAddr, 4, &dwRead))
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
	if (!ReadProcessMemory(m_hProcess, lpAddr, szWideBuf, MAX_PATH*2, &dwRead))
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
    WideCharToMultiByte( CP_ACP, 0, szWideBuf, -1, szMultiBuf, MAX_PATH, NULL, NULL );

//	MessageBox(NULL,szMultiBuf,"DbgLoadDll",MB_OK);
	return DBG_CONTINUE;	
}

int CDebugFrame::DbgUnLoadDll(DEBUG_EVENT *pDbg)
{
	UNLOAD_DLL_DEBUG_INFO info = pDbg->u.UnloadDll;
	
	//此处实现模块链表的维护

//	MessageBox(NULL,"DbgUnLoadDll","Debug",MB_OK);
	return DBG_CONTINUE;
}

int CDebugFrame::DbgOutPutStrting(DEBUG_EVENT *pDbg)
{
	OUTPUT_DEBUG_STRING_INFO info = pDbg->u.DebugString;
	
//	MessageBox(NULL,"DbgOutPutStrting","Debug",MB_OK);
	return DBG_CONTINUE;
}

int CDebugFrame::DbgRip(DEBUG_EVENT *pDbg)
{
	RIP_INFO info = pDbg->u.RipInfo;
	
//	MessageBox(NULL,"DbgRip","Debug",MB_OK);
	return DBG_EXCEPTION_NOT_HANDLED;	
}

int CDebugFrame::EcptAccessViolation(DEBUG_EVENT *pDbg)
{
	EXCEPTION_RECORD record = pDbg->u.Exception.ExceptionRecord;
//	MessageBox(NULL,"EcptAccessViolation","DbgException",MB_OK);

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pDbg->dwThreadId);
	if (hThread == NULL)
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
	CONTEXT context = {0};
	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	
	if ( !GetThreadContext( hThread, &context) )
	{
		return FALSE;
	}
	
	//EXCEPTION_ACCESS_VIOLATION要考察ExceptionInformation的0\1两项,若NumberParameters<2,则异常信息有误
	if ( record.NumberParameters < 2)
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
	//是否在断点所在分页
	if ( (m_CurBpPageInfo = m_pObjData->m_pObjMemBp->IsInMemBpPage(record.ExceptionInformation[1]))  != NULL )
	{
		DWORD dwBpProtect = 0;
		//不论是否命中，都要改属性让这句过去，设单步，再恢复内存断点
		if ( !VirtualProtectEx ( m_hProcess, (LPVOID)record.ExceptionInformation[1], 
			1, m_CurBpPageInfo->dwOldProtect, &dwBpProtect) )
		{
			return DBG_EXCEPTION_NOT_HANDLED;
		}
		
		m_CurBpPageInfo->dwOldProtect = dwBpProtect;
		context.EFlags |= 0x100;

		//是否命中
		if ( m_pObjData->m_pObjMemBp->IsHit(record.ExceptionInformation[1], record.ExceptionInformation[0]) )
		{
//			char szBuf[MAXBYTE] = {0};
//			wsprintf(szBuf,"ExceptionCode:%p,ExceptionAddress:%p",record.ExceptionCode,record.ExceptionAddress);		
//			MessageBox(NULL,szBuf,"EcptAccessViolation",MB_OK);

			//可以开始等待命令了
			ExecuteCommand(&context);
		}
		
		if ( !SetThreadContext( hThread, &context) )
		{
			return DBG_EXCEPTION_NOT_HANDLED;
		}
		
		m_bIsStepMemBp = TRUE;
		m_dwCurMemBpAddr = record.ExceptionInformation[1];
	}
	else
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	return DBG_CONTINUE;
}

int CDebugFrame::EcptBreakPoint(DEBUG_EVENT *pDbg)
{
	EXCEPTION_RECORD record = pDbg->u.Exception.ExceptionRecord;
//	MessageBox(NULL,"EcptBreakPoint","DbgException",MB_OK);
	
	//系统在OEP前下的int3
	if ( m_bIsBeginBp == TRUE)
	{
		m_pObjData->InitIatMap();
		m_bIsBeginBp = FALSE;
		return DBG_CONTINUE;
	}

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pDbg->dwThreadId);
	if (hThread == NULL)
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
	CONTEXT context = {0};
	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	
	if ( !GetThreadContext(hThread, &context) )
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	PCOMMON_BP_INFO pBpInfo = (PCOMMON_BP_INFO)( m_pObjData->m_pObjCommonBp->SearchTableForIsHit( (DWORD)record.ExceptionAddress) ) ;

	//断点命中且有效则写回数据，eip--
	if (  pBpInfo!= NULL && pBpInfo->m_bIsValid == 1)
	{
		char szBuf[MAXBYTE] = {0};
		wsprintf(szBuf,"ExceptionCode:%p,ExceptionAddress:%p",record.ExceptionCode,record.ExceptionAddress);
			
//		MessageBox(NULL,szBuf,"EcptBreakPoint",MB_OK);
			
		//恢复数据
		if ( !WriteProcessMemory(	m_hProcess, (LPVOID)pBpInfo->m_dwAddr,
				&pBpInfo->m_bOldData, 1, NULL) )
		{
			return DBG_EXCEPTION_NOT_HANDLED;
		}
		
		//
		context.Eip--;	

		//如果不是一次性断点，设置未激活，需下单步用以恢复断点
		if ( pBpInfo->m_bIsOnce == 0 )
		{
			context.EFlags |= 0x100;
			pBpInfo->m_bIsValid = 0;
			m_bIsStepCommonBp = TRUE;
			//记录当前是哪个断点设置的单步
			m_pCurCommonBp = pBpInfo;
		}
		else //如果是一次性断点，则删除断点信息
		{
			m_pObjData->m_pObjCommonBp->RemoveCommonBp(pBpInfo->m_dwAddr);
		
			if ( m_pObjData->m_dwRtBegin == (DWORD)record.ExceptionAddress)
			{
				m_bIsRTing = TRUE;
				context.EFlags |= 0x100;	
				if (!AddRecordToMap(pDbg->u.Exception.ExceptionRecord.ExceptionAddress))
				{
					return FALSE;
				}
			}
		}
		
		//可以开始等待命令了
		ExecuteCommand(&context);

		if ( !SetThreadContext( hThread, &context) )
		{
			return DBG_EXCEPTION_NOT_HANDLED;
		}

		return DBG_CONTINUE;
	}

	//无效断点或者未命中的断点则不处理，放过
	return DBG_EXCEPTION_NOT_HANDLED;
}

int CDebugFrame::EcptPrivilegedInstruction(DEBUG_EVENT *pDbg)
{
	EXCEPTION_RECORD record = pDbg->u.Exception.ExceptionRecord;

	CONTEXT context = {0};
	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pDbg->dwThreadId);
	
	if ( !GetThreadContext(hThread, &context) )
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}	
	//恢复数据
	
	//可以开始等待命令了
	ExecuteCommand(&context);

	if ( !SetThreadContext( hThread, &context) )
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	return DBG_CONTINUE;
}

int CDebugFrame::EcptSingleStep(DEBUG_EVENT *pDbg)
{
	EXCEPTION_RECORD record = pDbg->u.Exception.ExceptionRecord;
//	MessageBox(NULL,"EcptSingleStep","DbgException",MB_OK);

	CONTEXT context = {0};
	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pDbg->dwThreadId);
	
	if ( !GetThreadContext(hThread, &context) )
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}	

	//配合一般断点触发的单步 在此处理
	if ( !EcptSingleStepCommonBp(hThread, pDbg, &context))
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	//配合内存断点触发的单步
	if ( !EcptSingleStepMemBp(hThread, pDbg, &context))
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	//硬件断点或配合硬件断点的单步 在此函数内处理
	if ( !EcptSingleStepHardBp(hThread, pDbg, &context))
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}
	
//	if ( m_bIsStepTP == TRUE)
	{
//		m_bIsStepTP = FALSE;
	}

	//可以开始等待命令了
	if ( !m_bIsStepCommonBp && !m_bIsStepMemBp && !m_bIsStepHardBp)
	{
		ExecuteCommand(&context);
	}

	//处理RT命令
	if ( !EcptSingleStepForRT(hThread, pDbg, &context) )
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	if ( !SetThreadContext( hThread, &context) )
	{
		return DBG_EXCEPTION_NOT_HANDLED;
	}

	return DBG_CONTINUE;
}

int CDebugFrame::EcptControlC(DEBUG_EVENT *pDbg)
{
	EXCEPTION_RECORD record = pDbg->u.Exception.ExceptionRecord;
//	MessageBox(NULL,"EcptControlC","DbgException",MB_OK);
	return DBG_CONTINUE;
}

BOOL CDebugFrame::EcptSingleStepHardBp(HANDLE hThread, DEBUG_EVENT *pDbg, CONTEXT *pContext)
{
	m_pObjData->m_pObjHardBp = m_pObjData->m_pObjHardBp->CreateObj(hThread);

	//硬件断点所需单步用以恢复断点
	if ( m_bIsStepHardBp)
	{
// 		char szBuf[MAXBYTE] = {0};
// 		wsprintf(szBuf,"ExceptionCode:%p,ExceptionAddress:%p",pDbg->u.Exception.ExceptionRecord.ExceptionCode,
// 															pDbg->u.Exception.ExceptionRecord.ExceptionAddress);

//		MessageBox(NULL,szBuf,"EcptSingleStepHardBp",MB_OK);
//		pCcontext->Dr0 = dwAddr;

		DR7 dr7 = {0};
		dr7.dwValue = pContext->Dr7;
		

		switch(m_pObjData->m_pObjHardBp->m_dwCurHardBpIndex)
		{
		case 0:
			dr7.tagDR7.L0 = 1;
			break;
		case 1:
			dr7.tagDR7.L1 = 1;
			break;
		case 2:
			dr7.tagDR7.L2 = 1;
			break;
		case 3:
			dr7.tagDR7.L3 = 1;
			break;
		}

		m_bIsStepHardBp = FALSE;	
		
		pContext->Dr7 = dr7.dwValue;
		if ( !SetThreadContext(hThread,pContext))
		{
			return FALSE;
		}

//		m_pObjData->m_pObjHardBp->ListHardBp();

		return TRUE;
	}
	
	//硬件断点
	if (pContext->Dr6 & 0xf)
	{
		char szBuf[MAXBYTE] = {0};
		wsprintf(szBuf,"ExceptionCode:%p,ExceptionAddress:%p",pDbg->u.Exception.ExceptionRecord.ExceptionCode,
			pDbg->u.Exception.ExceptionRecord.ExceptionAddress);
		
//		MessageBox(NULL,szBuf,"EcptHardBp",MB_OK);
		//用来判断硬件断点的属性
		DR7 dr7 = {0};
		dr7.dwValue = pContext->Dr7;

		switch(pContext->Dr6 & 0xf)
		{
		case 1:
			//执行断点下单步
			if ( dr7.tagDR7.RW0 == 0)
			{
				pContext->EFlags |= 0x100;
				dr7.tagDR7.G0 = 0;
				dr7.tagDR7.L0 = 0;

				m_pObjData->m_pObjHardBp->m_dwCurHardBpIndex = 0;
				m_bIsStepHardBp = TRUE;
			}
			
			break;
		case 2:
			//执行断点下单步
			if ( dr7.tagDR7.RW1 == 0)
			{
				pContext->EFlags |= 0x100;
				dr7.tagDR7.G1 = 0;
				dr7.tagDR7.L1 = 0;
				
				m_pObjData->m_pObjHardBp->m_dwCurHardBpIndex = 1;
				m_bIsStepHardBp = TRUE;
			}

			break;
		case 4:
			//执行断点下单步
			if ( dr7.tagDR7.RW2 == 0)
			{
				pContext->EFlags |= 0x100;
				dr7.tagDR7.G2 = 0;
				dr7.tagDR7.L2 = 0;
				
				m_pObjData->m_pObjHardBp->m_dwCurHardBpIndex = 2;
				m_bIsStepHardBp = TRUE;
			}

			break;
		case 8:
			//执行断点下单步
			if ( dr7.tagDR7.RW3 == 0)
			{
				pContext->EFlags |= 0x100;
				dr7.tagDR7.G3 = 0;
				dr7.tagDR7.L3 = 0;
				
				m_pObjData->m_pObjHardBp->m_dwCurHardBpIndex = 3;
				m_bIsStepHardBp = TRUE;
			}

			break;
		default:
			break;
		}
		
		pContext->Dr7 = dr7.dwValue;
		if ( !SetThreadContext(hThread,pContext))
		{
			return FALSE;
		}

		return TRUE;
	}
	
	return TRUE;
}

BOOL CDebugFrame::EcptSingleStepCommonBp(HANDLE hThread, DEBUG_EVENT *pDbg, CONTEXT *pContext)
{
	if ( m_bIsStepCommonBp == TRUE )
	{
		if ( m_pCurCommonBp->m_bIsValid == 0)
		{
			if ( !ReadProcessMemory( m_hProcess,(LPVOID)m_pCurCommonBp->m_dwAddr,
									&m_pCurCommonBp->m_bOldData, 1, NULL)  )
			{
				return FALSE;
			}
			
			if ( !WriteProcessMemory( m_hProcess, (LPVOID)m_pCurCommonBp->m_dwAddr, &g_bBpCode[m_pCurCommonBp->m_bBpType], 1, NULL) )
			{
				return FALSE;
			}
			
			pContext->EFlags |= 0x100;
			
			if ( !SetThreadContext(hThread, pContext) )
			{
				return FALSE;
			}
			m_pCurCommonBp->m_bIsValid = 1; 

			return TRUE;
		}
		else
		{
			m_bIsStepCommonBp = TRUE;
		}
		
	}

	return TRUE;
}

BOOL CDebugFrame::EcptSingleStepMemBp(HANDLE hThread, DEBUG_EVENT *pDbg, CONTEXT *pContext)
{
	//内存断点引发的单步
	if ( m_bIsStepMemBp)
	{
		DWORD dwOldProtect = 0;
		//恢复内存断点
		if ( !VirtualProtectEx(m_hProcess, (LPVOID)m_dwCurMemBpAddr, 
				1, m_CurBpPageInfo->dwOldProtect, &dwOldProtect) )
		{
			return DBG_EXCEPTION_NOT_HANDLED;
		}

		m_CurBpPageInfo->dwOldProtect = dwOldProtect;
		
		return DBG_CONTINUE;
	}
	
	return TRUE;
}

void CDebugFrame::ExecuteCommand(CONTEXT* pContext)
{
	CMD cmd = {0};
	FuncType pFunc = NULL;
	
	m_pObjUi->ShowRegEnvironment(pContext,m_pObjData->m_mapFuncIat);

	while (true)
	{
		memset( &cmd, 0, sizeof(CMD));
		if ( m_pObjData->m_bIsScriptCmd )
		{
			//从文件获取CMD
			if (!GetCommandFromFile(&cmd))
			{
				continue;
			}
		}
		else
		{
			m_pObjUi->GetCommand(&cmd);
		}

		pFunc = m_pObjProc->GetCommandFuncPtr(cmd.szCmd);
		
		if (pFunc == NULL)
		{
			printf("Wrong Input!\r\n");
			continue;
		}
		
		if ( cmd.szCmd[0] == 'p' || cmd.szCmd[0] == 'P' 
			|| cmd.szCmd[0] == 't' || cmd.szCmd[0] == 'T' 
			|| cmd.szCmd[0] == 'g' || cmd.szCmd[0] == 'G')
		{
//			m_bIsStepTP = TRUE;
			(m_pObjProc->*pFunc)(&cmd, pContext, TRUE);
			break;
		}
		else
		{
			(m_pObjProc->*pFunc)(&cmd, pContext, FALSE);
		}

		//cmd 添加到链表
		m_pObjData->m_cmdList.insert( m_pObjData->m_cmdList.end(), cmd);
	}
}

BOOL CDebugFrame::GetCommandFromFile(PCMD pCmd)
{
	FILE* cmdFile = fopen(m_pObjData->m_szScrName, "r");

	if ( cmdFile == NULL )
	{
		return FALSE;
	}
	
	char szBuf[MAXBYTE] = {0};
	if ( fseek(cmdFile, m_pObjData->m_dwCurFileOffset, SEEK_SET))
	{
		m_pObjData->m_bIsScriptCmd = FALSE;
		return FALSE;
	}

	
	if ( !fread(szBuf, sizeof(char), MAXBYTE, cmdFile))
	{
		m_pObjData->m_bIsScriptCmd = FALSE;
		return FALSE;
	}

	if ( cmdFile != NULL )
	{
		fclose(cmdFile);
	}
	//计算脚本中下一行的指针偏移量
	DWORD dwIndex = 0;
	while( szBuf[dwIndex++] != '\n' );
	m_pObjData->m_dwCurFileOffset += dwIndex + 1;

	//将文件中读出的数据转为cmd
	sscanf(szBuf,"%s[^\n]",szBuf);
	sscanf(szBuf,"%s %s %s %s\n", pCmd->szCmd, pCmd->szArgu1, pCmd->szArgu2, pCmd->szArgu3);

	printf("-%s\r\n",pCmd->szCmd);
	return TRUE;
}

BOOL CDebugFrame::AddRecordToMap(LPVOID lpAddr)
{
	BYTE szCode[20] = {0};
	t_disasm da = {0};

	//读目标进程的代码段
	if ( !ReadProcessMemory( m_hProcess, lpAddr, szCode, 20, NULL)  )
	{
		return FALSE;
	}
	
    int nLen = Disasm(szCode, 20, 0, &da, DISASM_CODE);
	
	m_pObjData->m_record[(DWORD)lpAddr] = da;

	return TRUE;
}

BOOL CDebugFrame::EcptSingleStepForRT(HANDLE hThread, DEBUG_EVENT* pDbg, CONTEXT* pContext)
{
	if ( m_bIsRTing == TRUE )
	{
		pContext->EFlags |= 0x100;
		if (!AddRecordToMap(pDbg->u.Exception.ExceptionRecord.ExceptionAddress))
		{
			return FALSE;
		}
	}

	if (m_pObjData->m_dwRtBegin != 0 &&  m_pObjData->m_dwRtEnd != 0 
		&& m_pObjData->m_dwRtEnd == (DWORD)pDbg->u.Exception.ExceptionRecord.ExceptionAddress)
	{
		m_bIsRTing = FALSE;
		if (!SaveRecordToFile())
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDebugFrame::SaveRecordToFile()
{
	OPENFILENAME Ofn = {0};
	char szRecordName[MAXBYTE] = {0};
	
	Ofn.lStructSize = sizeof(OPENFILENAME); 
	Ofn.hwndOwner = NULL; 
	Ofn.lpstrFilter = "Txt(*.txt)\0*.txt\0"; 
	Ofn.lpstrFile= szRecordName; 
	Ofn.nMaxFile = MAXBYTE; 
	Ofn.lpstrFileTitle = NULL; 
	Ofn.nMaxFileTitle = 0; 
	Ofn.lpstrInitialDir = (LPSTR)NULL; 
	Ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT; 
	Ofn.lpstrTitle = NULL; 
	
	if ( GetSaveFileName(&Ofn) == TRUE ) 
	{	
		//将链表中的命令写入文件
		FILE* file = fopen( szRecordName, "w+" );
		if ( file == NULL)
		{
			return FALSE;
		}

		map<DWORD,t_disasm>::iterator it;
		
		for ( it = m_pObjData->m_record.begin(); it != m_pObjData->m_record.end(); ++it) 
		{
			if (fprintf(file,"%p %16s\t%s\r\n", it->first, it->second.dump, it->second.result) < 0)
			{
				return FALSE;
			}			
		}
		
		fclose(file);
	}
	else
	{
		printf("User Cancel\r\n",m_pObjData->m_szScrName);
		return FALSE;
	}

	return TRUE;
}

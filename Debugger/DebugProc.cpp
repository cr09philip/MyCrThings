// DebugProc.cpp: implementation of the CDebugProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DebugProc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebugProc::CDebugProc(HANDLE hProcess) : m_hProcess(hProcess)
{
	InitCmdMap();

	m_pObjData = m_pObjData->CreateObj(m_hProcess);	

	m_pObjUi = m_pObjUi->CreateObj(m_hProcess);
}

CDebugProc::~CDebugProc()
{

}

void CDebugProc::InitCmdMap()
{
	m_FuncPtrMap.insert(pair<char* ,FuncType>("rt",	&CDebugProc::Command_RT));	
	m_FuncPtrMap.insert(pair<char* ,FuncType>("sv",	&CDebugProc::Command_SV));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("ld",	&CDebugProc::Command_LD));

	m_FuncPtrMap.insert(pair<char* ,FuncType>("bp",	&CDebugProc::Command_BP));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bpl",	&CDebugProc::Command_BPL));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bpc",	&CDebugProc::Command_BPC));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bm",	&CDebugProc::Command_BM));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bml",	&CDebugProc::Command_BML));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bmc",	&CDebugProc::Command_BMC));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bh",	&CDebugProc::Command_BH));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bhl",	&CDebugProc::Command_BHL));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("bhc",	&CDebugProc::Command_BHC));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("d",	&CDebugProc::Command_D));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("e",	&CDebugProc::Command_E));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("g",	&CDebugProc::Command_G));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("p",	&CDebugProc::Command_P));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("r",	&CDebugProc::Command_R));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("t",	&CDebugProc::Command_T));
	m_FuncPtrMap.insert(pair<char* ,FuncType>("u",	&CDebugProc::Command_U));
    m_FuncPtrMap.insert(pair<char* ,FuncType>("q",	&CDebugProc::Command_Q));

//	m_FuncPtrMap["q"] = &CDebugProc::Command_Q;
}

void CDebugProc::Command_RT(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	m_pObjData->m_dwRtBegin	= strtol(pCmd->szArgu1,&pStopString,16);
	m_pObjData->m_dwRtEnd	= strtol(pCmd->szArgu2,&pStopString,16);


	m_pObjData->m_pObjCommonBp->SetCommonBp(m_pObjData->m_dwRtBegin,1,0,1);
}

void CDebugProc::Command_SV(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	OPENFILENAME Ofn = {0};

	memset(m_pObjData->m_szScrName,0,MAXBYTE);
 
	Ofn.lStructSize = sizeof(OPENFILENAME);
	Ofn.hwndOwner = NULL; 
	Ofn.lpstrFilter = "Txt(*.txt)\0*.txt\0"; 
	Ofn.lpstrFile= m_pObjData->m_szScrName; 
	Ofn.nMaxFile = MAXBYTE; 
	Ofn.lpstrFileTitle = NULL; 
	Ofn.nMaxFileTitle = 0; 
	Ofn.lpstrInitialDir = (LPSTR)NULL;
	Ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT; 
	Ofn.lpstrTitle = NULL; 

	if ( GetSaveFileName(&Ofn) == TRUE ) 
	{	
		//将链表中的命令写入文件
		FILE* file = fopen( m_pObjData->m_szScrName, "w+" );
		
		list<CMD>::iterator pCount;
		
		for ( pCount = m_pObjData->m_cmdList.begin(); pCount != m_pObjData->m_cmdList.end(); ++pCount) 
		{
			fprintf(file,"%s %s %s %s\r\n",(*pCount).szCmd, (*pCount).szArgu1,
				(*pCount).szArgu2,	(*pCount).szArgu3);	
			
		}
		
		if ( 0 == fclose(file))
		{
			printf("Success!\r\n");
		}
		else
		{
			printf("Save Failed!\r\n");
		}
	}
	else
	{
		printf("User Cancel\r\n",m_pObjData->m_szScrName);
	}

}

void CDebugProc::Command_LD(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	OPENFILENAME ofn;       // common dialog box structure
	memset(m_pObjData->m_szScrName, 0, MAXBYTE);

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = m_pObjData->m_szScrName;
	ofn.nMaxFile = sizeof(m_pObjData->m_szScrName);
	ofn.lpstrFilter = "Txt(*.txt)\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Display the Open dialog box. 
	
	if ( GetOpenFileName(&ofn) == TRUE ) 
	{
		m_pObjData->m_bIsScriptCmd = TRUE;
		printf("Get Cmd From %s\r\n",m_pObjData->m_szScrName);
	}
	else
	{
		printf("User Cancel\r\n",m_pObjData->m_szScrName);
	}
}

void CDebugProc::Command_BP(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwAddr	= strtol(pCmd->szArgu1,&pStopString,16);
// 	BYTE  bIsOnce	= strtol(pCmd->szArgu2,&pStopString,16);
// 	BYTE  bType		= strtol(pCmd->szArgu3,&pStopString,16);

	m_pObjData->m_pObjCommonBp->SetCommonBp(dwAddr,0,0,1);
}

void CDebugProc::Command_BPL(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	m_pObjData->m_pObjCommonBp->ListCommonBp();
}

void CDebugProc::Command_BPC(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwAddr	= strtol(pCmd->szArgu1,&pStopString,16);
	
	m_pObjData->m_pObjCommonBp->RemoveCommonBp(dwAddr);
}

void CDebugProc::Command_BM(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwAddr	= strtol(pCmd->szArgu1,&pStopString,16);
	DWORD dwRange	= strtol(pCmd->szArgu2,&pStopString,16);
	DWORD dwIsWrite	= strtol(pCmd->szArgu3,&pStopString,16);
	
	m_pObjData->m_pObjMemBp->RefreshMemBpTable();
	m_pObjData->m_pObjMemBp->SetMemBp(dwAddr, dwRange, dwIsWrite);
}

void CDebugProc::Command_BML(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	m_pObjData->m_pObjMemBp->RefreshMemBpTable();
	m_pObjData->m_pObjMemBp->ListMemBp();
}

void CDebugProc::Command_BMC(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwIndex	= strtol(pCmd->szArgu1,&pStopString,16);

	m_pObjData->m_pObjMemBp->RemoveMemBp(dwIndex);
}

void CDebugProc::Command_BH(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwAddr	= strtol(pCmd->szArgu1,&pStopString,16);
	DWORD dwAccess	= strtol(pCmd->szArgu2,&pStopString,16);
	DWORD dwLen	= strtol(pCmd->szArgu3,&pStopString,16);

	m_pObjData->m_pObjHardBp->SetHardBp(dwAddr,dwAccess,dwLen, pContext);
}

void CDebugProc::Command_BHL(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	m_pObjData->m_pObjHardBp->ListHardBp(pContext);
}

void CDebugProc::Command_BHC(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwIndex	= strtol(pCmd->szArgu1,&pStopString,16);
	m_pObjData->m_pObjHardBp->RemoveHardBp(dwIndex,pContext);
}


void CDebugProc::Command_D(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	if ( m_hProcess == INVALID_HANDLE_VALUE)
	{
		return;
	}

	BYTE szCode[MAXBYTE] = {0};
	t_disasm da = {0};
	//读目标进程的
	if ( !ReadProcessMemory( m_hProcess,(LPVOID)(pContext->Eip),
		szCode, 256, NULL)  )
	{
		return;
	}
	
	DWORD dwBase = pContext->Eip;
	for (int i = 0; i < 16; i++)
	{
		printf("%p %02X %02X %02X %02X %02X %02X %02X %02X-%02X %02X %02X %02X %02X %02X %02X %02X    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n",
								dwBase+i*16,	szCode[i*16+0],	szCode[i*16+1],	szCode[i*16+2],	szCode[i*16+3],
								szCode[i*16+4],	szCode[i*16+5],	szCode[i*16+6],	szCode[i*16+7],
								szCode[i*16+8],	szCode[i*16+9],	szCode[i*16+10],szCode[i*16+11],
								szCode[i*16+12],szCode[i*16+13],szCode[i*16+14],szCode[i*16+15],//);
								szCode[i*16+0],	szCode[i*16+1],	szCode[i*16+2],	szCode[i*16+3],
								szCode[i*16+4],	szCode[i*16+5],	szCode[i*16+6],	szCode[i*16+7],
								szCode[i*16+8],	szCode[i*16+9],	szCode[i*16+10],szCode[i*16+11],
								szCode[i*16+12],szCode[i*16+13],szCode[i*16+14],szCode[i*16+15]	);
	}
}

void CDebugProc::Command_E(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwAddr	= strtol(pCmd->szArgu1,&pStopString,16);

	if (dwAddr == 0)
	{
		dwAddr = pContext->Eip;
	}

    BYTE szCode[MAXBYTE] = {0};
	//读目标进程的
	if ( !ReadProcessMemory( m_hProcess,(LPVOID)dwAddr,	szCode, MAXBYTE, NULL)  )
	{
		return;
	}

	BYTE bNewByte = 0;
	DWORD dwIndex = 0;
	while (true)
	{
		printf("%p\t%02X:",dwAddr,szCode[dwIndex]);
		
		if ( !GetInput(&bNewByte) )
		{
			break;
		}
		
		if ( !WriteProcessMemory( m_hProcess, (LPVOID)(dwAddr+dwIndex),(LPVOID)&bNewByte,1,NULL) )
		{
			break;
		}
		
		dwIndex++;
	}
}

void CDebugProc::Command_G(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{	
	char* pStopString = 0;
	DWORD dwAddr	= strtol(pCmd->szArgu1,&pStopString,16);

	if ( dwAddr != 0 )
	{
		m_pObjData->m_pObjCommonBp->SetCommonBp(dwAddr,1);
	}
}

void CDebugProc::Command_P(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	//m_pObjUi->ShowRegEnvironment(pContext,m_pObjData->m_mapFuncIat);
	
	BYTE szCode[MAXBYTE] = {0};
	t_disasm da = {0};
	//读目标进程的
	if ( !ReadProcessMemory( m_hProcess,(LPVOID)(pContext->Eip),
		szCode, 256, NULL)  )
	{
		return;
	}

	int nLen = Disasm(szCode, 20, 0, &da,DISASM_SIZE);

    //遇到call指令，单步步过
    //将下一条指令的设一次性int3断点
    if ( da.cmdtype == 0x70)
    {
		m_pObjData->m_pObjCommonBp->SetCommonBp(pContext->Eip + nLen,1);
    }
    else
    {
        pContext->EFlags |= 0x100;   
    }
}

void CDebugProc::Command_R(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{	
	m_pObjUi->ShowRegEnvironment(pContext,m_pObjData->m_mapFuncIat);
	
	if (pCmd->szArgu1 != "")
	{
	}
}

void CDebugProc::Command_T(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
//	m_pObjUi->ShowRegEnvironment(pContext, m_pObjData->m_mapFuncIat);
	
    pContext->EFlags |= 0x100;
}

void CDebugProc::Command_Q(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	exit(0);
}

void CDebugProc::Command_U(PCMD pCmd,CONTEXT* pContext,BOOL isRunning)
{
	char* pStopString = 0;
	DWORD dwAddr	= strtol(pCmd->szArgu1,&pStopString,16);
	

	DWORD dwBase = 0;	
	
	if ( dwAddr != 0 )
	{
		dwBase = dwAddr;	
	}
	else
	{
		dwBase = pContext->Eip;
	}
	
	static DWORD dwIndex = dwBase;
	
	if ( isRunning )
	{
		dwIndex = dwBase;
	}
	//
	BYTE szCode[MAXBYTE] = {0};
	t_disasm da = {0};
	//读目标进程的
	if ( !ReadProcessMemory( m_hProcess,(LPVOID)(dwIndex),
		szCode, 256, NULL)  )
	{
		return;
	}
	
	BYTE* pCode = szCode;
	for (int i = 0; i < 16; i++)
	{
		int nLen = Disasm(pCode, 20, 0, &da, DISASM_CODE);

		if ( da.cmdtype == 0x70 && da.adrconst != 0)
		{
			DWORD dwFuncPtr = 0;
			if ( !ReadProcessMemory( m_hProcess,(LPVOID)(da.adrconst),
				&dwFuncPtr, 4, NULL)  )
			{
				return;
			}

// 			map<DWORD,char*>::iterator it;
// 			for( it = m_pObjData->m_mapFuncIat.begin(); it != m_pObjData->m_mapFuncIat.end(); ++it)
// 			{
// 				if ( it->first == dwFuncPtr )
// 				{
// 					break;
// 				}
// 			}
// 
// 			printf("%p\t%16s\t%s;[%s]\r\n", dwIndex, da.dump, da.result,it->second);
			printf("%p\t%16s\t%s;[%s]\r\n", dwIndex, da.dump, da.result, m_pObjData->m_mapFuncIat[dwFuncPtr]);
		}
		else
		{
			printf("%p\t%16s\t%s\r\n", dwIndex, da.dump, da.result);
		}
		
		pCode += nLen;
		dwIndex += nLen;
	}
}

FuncType CDebugProc::GetCommandFuncPtr(char *szCmd)
{
	//定义一个条目变量(实际是指针)
	map<char* ,FuncType>::iterator it;

	for( it = m_FuncPtrMap.begin(); it != m_FuncPtrMap.end(); ++it)
	{
		if ( 0 == strcmp( it->first, szCmd ) )
		{
			return it->second;
		}
	}

	return NULL;
}

BOOL CDebugProc::GetInput(BYTE* pByte)
{
	BOOL bRet = FALSE;
	DWORD dwIndex = 0;
	char* pStopString = 0;
	char szBuf[MAXBYTE] = {0};
	memset( szBuf, 0, MAXBYTE);

	fflush(stdin);
	while( (szBuf[dwIndex] = getchar()) != '\n')
	{
		//允许命令最大字节数
		if (dwIndex >= 0x2)
		{
			return FALSE;
		}
		
		dwIndex++;
		bRet = TRUE;
	}

	*pByte = strtol(szBuf,&pStopString,16);
	return bRet;
}

void CDebugProc::Command_Help()
{
    
}

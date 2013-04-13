// DataCls.cpp: implementation of the CDataCls class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataCls.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDataCls* CDataCls::m_obj	= NULL;
HANDLE CDataCls::m_hProcess	= NULL;

CDataCls::CDataCls()
{
	m_dwCurFileOffset = 0;
	
	m_bIsScriptCmd	= FALSE;
}

CDataCls::~CDataCls()
{
	
}

CDataCls* CDataCls::CreateObj(HANDLE hProcess)
{
	if ( hProcess == NULL)
	{
		return NULL;
	}
	
	m_hProcess = hProcess;
	
	if ( m_obj == NULL)
	{
		m_obj = new CDataCls;
	}
	
	return m_obj;
}

void CDataCls::InitIatMap()
{
	LPVOID lpBaseImage = (LPVOID)0x400000;
	//读取目标进程的DOS头
	IMAGE_DOS_HEADER dosHeader = {0};
	if ( !ReadProcessMemory( m_hProcess, lpBaseImage, &dosHeader, sizeof(IMAGE_DOS_HEADER), NULL) )
	{
		return;
	}

	////读取目标进程的nt头
	IMAGE_NT_HEADERS ntHeaders = {0};
	if ( !ReadProcessMemory( m_hProcess, (LPVOID)((DWORD)lpBaseImage + dosHeader.e_lfanew),
							&ntHeaders, sizeof(IMAGE_NT_HEADERS), NULL)	 )
	{
		return;
	}

	IMAGE_IMPORT_DESCRIPTOR curImportDesc = {0};
	LPVOID lpAddr = (LPVOID)((DWORD)lpBaseImage + 
		ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );
	IMAGE_IMPORT_DESCRIPTOR zeroImport = {0};
	char szDllName[MAXBYTE] = {0};
	char* pFuncName = NULL;
	DWORD dwIndex = 0;

	//读取目标进程的第一个导入表描述符
	if ( !ReadProcessMemory( m_hProcess,lpAddr,	&curImportDesc, sizeof(IMAGE_IMPORT_DESCRIPTOR), NULL)	 )
	{
		return;
	}

	while ( memcmp(&curImportDesc, &zeroImport, sizeof(zeroImport)) != 0 )
	{	
		//获取dll名称
		if ( !ReadProcessMemory( m_hProcess, (LPVOID)(curImportDesc.Name + (DWORD)lpBaseImage),
								&szDllName, MAXBYTE, NULL)	 )
		{
			return;
		}

		//获取INT信息
		IMAGE_THUNK_DATA ThunkData = {0} ;
		DWORD dwOriginalThunkAddr = curImportDesc.OriginalFirstThunk + (DWORD) lpBaseImage;
		if ( !ReadProcessMemory( m_hProcess, (LPVOID)dwOriginalThunkAddr,
									&ThunkData, sizeof(IMAGE_THUNK_DATA), NULL)	 )
		{
			return;
		}

		//获取IAT表的信息，即获得函数指针
		DWORD dwFirstThunk = 0;
		DWORD dwFirstThunkAddr = curImportDesc.FirstThunk + (DWORD) lpBaseImage;
		if ( !ReadProcessMemory( m_hProcess, (LPVOID)dwFirstThunkAddr,
			&dwFirstThunk, sizeof(IMAGE_THUNK_DATA), NULL)	 )
		{
			return;
		}

		while (  ThunkData.u1.AddressOfData != 0 && dwFirstThunk != 0)
		{
			//根据INT获取序号或者名称RVA
			if ( ThunkData.u1.Ordinal & 0x80000000)
			{
				pFuncName = new char [strlen(szDllName) + 1 + 8];
				if (pFuncName == NULL)
				{
					return;
				}
				sprintf(pFuncName, "%s-%p", szDllName,ThunkData.u1.Ordinal & 0x7FFFFFFF);
			}
			else
			{
				char szBuf[MAXBYTE] = {0};
				//获取Func名称
				if ( !ReadProcessMemory( m_hProcess, (LPVOID)(ThunkData.u1.Ordinal + (DWORD)lpBaseImage + 2),
					&szBuf, MAXBYTE, NULL)	 )
				{
					return;
				}

				pFuncName = new char [strlen(szDllName) + 1 + strlen(szBuf)];
				if (pFuncName == NULL)
				{
					return;
				}
				sprintf(pFuncName, "%s-%s", szDllName,szBuf);
			}

			//添加FuncPtr、FuncName到map
			m_mapFuncIat.insert(pair<DWORD,char*>(dwFirstThunk, pFuncName));
			//获取下一个IAT信息
			dwFirstThunkAddr += sizeof(DWORD);
			if ( !ReadProcessMemory( m_hProcess, (LPVOID)dwFirstThunkAddr,
				&dwFirstThunk, sizeof(IMAGE_THUNK_DATA), NULL)	 )
			{
				return;
			}

			//获取下个INT信息
			dwOriginalThunkAddr += sizeof(IMAGE_THUNK_DATA);
			if ( !ReadProcessMemory( m_hProcess, (LPVOID)dwOriginalThunkAddr,
				&ThunkData, sizeof(IMAGE_THUNK_DATA), NULL)	 )
			{
				return;
			}
		}
		
		//获取下个IMPORT_DESCRIPTOR信息，即下个dll的导入表
		lpAddr =(LPVOID)((DWORD)lpAddr + sizeof(IMAGE_IMPORT_DESCRIPTOR));
		if ( !ReadProcessMemory( m_hProcess,lpAddr,	&curImportDesc, sizeof(IMAGE_IMPORT_DESCRIPTOR), NULL)	 )
		{
			return;
		}
	}

// 	if ( pFuncName != NULL)
// 	{
// 		delete [] pFuncName;
// 	}
}

//void CDataCls::InitBpCls()
//{
//	
//}

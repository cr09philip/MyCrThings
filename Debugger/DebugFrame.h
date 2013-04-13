// DebugFrame.h: interface for the CDebugFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGFRAME_H__9BBB6212_9066_432B_A3E9_7AAB983EDE31__INCLUDED_)
#define AFX_DEBUGFRAME_H__9BBB6212_9066_432B_A3E9_7AAB983EDE31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Headers.h"
#include "DebuggerUi.h"
#include "DataCls.h"
#include "DebugProc.h"

#include <list>
#include <algorithm>
using namespace std;

typedef HANDLE (__stdcall *pOpenThread)(DWORD dwDesiredAccess,  // access right
										BOOL bInheritHandle,    // handle inheritance option
										DWORD dwThreadId        // thread identifier
										);
extern pOpenThread OpenThread;

class CDebugFrame
{
public:
	CDebugFrame();
	virtual ~CDebugFrame();
public:
	//�ϲ������ϸ��ֶϵ�Ĵ���
	BOOL EcptSingleStepMemBp(HANDLE hThread, DEBUG_EVENT *pDbg, CONTEXT *pContext);
	BOOL EcptSingleStepCommonBp(HANDLE hThread, DEBUG_EVENT *pDbg, CONTEXT *pContext);
	BOOL EcptSingleStepHardBp(HANDLE hThread, DEBUG_EVENT* pDbg, CONTEXT* pContext);
	BOOL EcptSingleStepForRT(HANDLE hThread, DEBUG_EVENT* pDbg, CONTEXT* pContext);

	int EcptControlC( DEBUG_EVENT* pDbg);
	int EcptSingleStep( DEBUG_EVENT* pDbg);
	int EcptPrivilegedInstruction( DEBUG_EVENT* pDbg);
	int EcptBreakPoint( DEBUG_EVENT* pDbg);
	int EcptAccessViolation( DEBUG_EVENT* pDbg);
	int DbgRip( DEBUG_EVENT* pDbg);
	int DbgOutPutStrting( DEBUG_EVENT* pDbg);
	int DbgUnLoadDll( DEBUG_EVENT* pDbg);
	int DbgLoadDll( DEBUG_EVENT* pDbg);
	int DbgExitProcess( DEBUG_EVENT* pDbg);
	int DbgExitThread( DEBUG_EVENT* pDbg);
	int DbgCreateProcess( DEBUG_EVENT* pDbg);
	int DbgCreateThread( DEBUG_EVENT* pDbg);
	int	DbgException( DEBUG_EVENT* pDbg);
	void BeginDebug( char* szName);
public:
	BOOL SaveRecordToFile();
	BOOL AddRecordToMap(LPVOID lpAddr);
	BOOL GetCommandFromFile(PCMD pCmd);
	void ExecuteCommand(CONTEXT* pContext);
	HANDLE m_hProcess;
public:
	CDebuggerUi* m_pObjUi;
	CDataCls* m_pObjData;
	CDebugProc* m_pObjProc;

// 	CHardBp* m_pObjHardBp;
// 	CMemBp*  m_pObjMemBp;
// 	CCommonBp* m_pObjCommonBp;
	
	//�ϲ����ʱ����¼��ǰ�ϵ���Ϣ
	PCOMMON_BP_INFO m_pCurCommonBp;
	PMEM_BP_INFO m_pCurMemBp;
	PBP_PAGE_INFO m_CurBpPageInfo;

	//����״̬
	BOOL m_bIsStepHardBp;
	BOOL m_bIsStepMemBp;
	BOOL m_bIsStepCommonBp;
//	BOOL m_bIsStepTP;

	//�ж��Ƿ���ϵͳ�µ�int3
	BOOL m_bIsBeginBp;

	//��¼��ǰ�ڴ�ϵ�ĵ�ַ���Ա��޸��ڴ����ԣ���ʵʹ�õ�ǰ�ڴ�ϵ����ֵ�޸�
	DWORD m_dwCurMemBpAddr;
	//�Ƿ��ڼ�¼ָ��
	BOOL m_bIsRTing;
private:
	//Ϊ�˶�̬��ȡOpenThread�õ���kernel32.dll�ļ��ص�ַ
	HMODULE m_hDll;	
	list<PModuleInfo> m_ModuleList;
};

#endif // !defined(AFX_DEBUGFRAME_H__9BBB6212_9066_432B_A3E9_7AAB983EDE31__INCLUDED_)

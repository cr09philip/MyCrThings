// DebugProc.h: interface for the CDebugProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGPROC_H__4417173E_898B_43D5_A12D_25F427009A8C__INCLUDED_)
#define AFX_DEBUGPROC_H__4417173E_898B_43D5_A12D_25F427009A8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Headers.h"
#include "DataCls.h"
#include "DebuggerUi.h"

class CDebugProc;

typedef		void (CDebugProc:: *FuncType)(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);

#include <map>
using namespace std;

class CDebugProc
{
public:
	CDebugProc(HANDLE hProcess);
	virtual ~CDebugProc();
public:	
	void InitCmdMap();
	BOOL GetInput(BYTE* pByte);
	FuncType GetCommandFuncPtr(char* szCmd);

	//指令记录 rt xx xx 
	void Command_RT(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	//保存脚本
	void Command_SV(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	//读入脚本
	void Command_LD(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);

	void Command_BP(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BPL(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BPC(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BM(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BML(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BMC(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BH(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BHL(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_BHC(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);

	void Command_D(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_E(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_G(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_P(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_R(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_T(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_U(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
	void Command_Q(PCMD pCmd,CONTEXT* pContext,BOOL isRunning);
public:
	void Command_Help();
	CDataCls* m_pObjData;
	CDebuggerUi* m_pObjUi;
	map<char* ,FuncType> m_FuncPtrMap;
	HANDLE m_hProcess;
};

#endif // !defined(AFX_DEBUGPROC_H__4417173E_898B_43D5_A12D_25F427009A8C__INCLUDED_)

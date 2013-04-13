// DebuggerUi.h: interface for the CDebuggerUi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGGERUI_H__773714E1_ABAE_45BB_9960_259F085E12F4__INCLUDED_)
#define AFX_DEBUGGERUI_H__773714E1_ABAE_45BB_9960_259F085E12F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Headers.h"
#include "Disasm/disasm.h"
#include "DataCls.h"

class CDebuggerUi
{
public:
	BOOL TranslateInputToCMD(char* szInput, DWORD dwBufLen, PCMD pCmd);
	BOOL GetCommand(PCMD pCmd);
//	void WaitForUserCommand(CONTEXT* pContext);
	CDebuggerUi();
	virtual ~CDebuggerUi();
public:
	void ShowRegEnvironment(CONTEXT* pContext, map<DWORD,char*> iatMap);
	static CDebuggerUi* CreateObj(HANDLE hProcess);
private:
	static HANDLE m_hProcess;
	static CDebuggerUi* m_obj;
//	CDataCls* m_pObjData;
};

#endif // !defined(AFX_DEBUGGERUI_H__773714E1_ABAE_45BB_9960_259F085E12F4__INCLUDED_)

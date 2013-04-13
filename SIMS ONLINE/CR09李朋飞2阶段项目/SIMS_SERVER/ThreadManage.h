// ThreadManage.h: interface for the CThreadManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADMANAGE_H__5B303F2C_8D94_4E62_8730_4C68F66D420F__INCLUDED_)
#define AFX_THREADMANAGE_H__5B303F2C_8D94_4E62_8730_4C68F66D420F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Task.h"
#include "ConnSql.h"

enum
{
	Ready = 0,
	Busy,
	Running
};

struct CThreadStatus
{
	DWORD m_dwThreadId;
	HANDLE m_hThread;
	DWORD m_dwCurIndex;
	DWORD m_dwStatus;//0为挂起 1为busy  2为running
};

//DWORD WINAPI ExecuteThreadProc( LPVOID lpParameter  );
class CThreadManage  ;

class CThreadManage  
{
	friend DWORD WINAPI ExecuteThreadProc( LPVOID lpParameter  );
	friend DWORD WINAPI MonitorThreadProc( LPVOID lpParameter  );
private:
	CThreadManage();
	virtual ~CThreadManage();
public:
	static CThreadManage*   CreateObj(SOCKET hSocket);

	static DWORD GetTask(CTask& taskObj);//返回0为空任务队列
	static BOOL  ExecuteTask(CTask& taskObj, int nConnIndex);
	static DWORD MainWorkFunc(DWORD dwIndex);
	
	static DWORD HasFreeThreadOrNot(DWORD dwStatus);//返回1表示还有处于dwStatus状态的线程，0则表示没有
	static DWORD AddThread();
	static void  Init();
public:
	static DWORD GetStatus(DWORD dwIndex);
	static void  SetStatus(DWORD dwIndex, DWORD dwStatus);
	static CThreadManage*	m_lpObj;

	static SOCKET m_hSocket;
	static DWORD  m_dwThreadCount;
	static CThreadStatus* m_lpStatus;

	static CString m_csTime;

	//创建连接组
	static CConnSql m_theConn[10];

	static CCriticalSection m_theSection;
//	static CTime  m_time;
};

#endif // !defined(AFX_THREADMANAGE_H__5B303F2C_8D94_4E62_8730_4C68F66D420F__INCLUDED_)

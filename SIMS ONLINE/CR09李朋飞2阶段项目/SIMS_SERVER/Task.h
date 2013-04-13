// Task.h: interface for the CTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASK_H__519A7021_3456_4689_9092_AAE5CF66C0D3__INCLUDED_)
#define AFX_TASK_H__519A7021_3456_4689_9092_AAE5CF66C0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySocketInit.h"
#include "MyPacket.h"

#include <queue>
using namespace std;

enum TaskStatus
{
	UnDone = 0,
	Finished = 1,
	Error    = 2,
	TimeOut  = 3
};

class CTask  
{
public:
	SOCKET		m_hSocket;
	int			m_nStatus;
	sockaddr_in m_destAddr;
	CSocketHead m_head;
	CString		m_csTask;
public:
	CTask()
	{
		m_hSocket = 0;
		m_nStatus = UnDone;
		memset(&m_destAddr, 0, sizeof(sockaddr_in) );
		memset(&m_head, 0, sizeof(CSocketHead));
		m_csTask = "";
	}
	~CTask()
	{
		m_hSocket = 0;
		m_nStatus = UnDone;
		memset(&m_destAddr, 0, sizeof(sockaddr_in) );
		memset(&m_head, 0, sizeof(CSocketHead));
		m_csTask = "";
	}
};

extern queue<CTask> g_TaskQueue;
#endif // !defined(AFX_TASK_H__519A7021_3456_4689_9092_AAE5CF66C0D3__INCLUDED_)






















// ThreadManage.cpp: implementation of the CThreadManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIMS_SERVER.h"
#include "ThreadManage.h"

#include "MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CThreadManage*  CThreadManage::m_lpObj			= NULL;
DWORD			CThreadManage::m_dwThreadCount  = 0;
SOCKET			CThreadManage::m_hSocket		= NULL;
CThreadStatus*	CThreadManage::m_lpStatus		= NULL;
//CTime			CThreadManage::m_time			= NULL;
CString			CThreadManage::m_csTime			= "";
CConnSql		CThreadManage::m_theConn[10];
CCriticalSection CThreadManage::m_theSection	;

CThreadManage::CThreadManage()
{
	m_lpStatus = new CThreadStatus[10];
}

CThreadManage::~CThreadManage()
{
	if ( m_lpObj != NULL)
	{
		delete m_lpObj;
	}	

	if ( m_lpStatus != NULL)
	{
		delete [] m_lpStatus;
	}
}	

DWORD CThreadManage::AddThread()
{
	m_lpStatus[m_dwThreadCount].m_hThread = 
		CreateThread( NULL, 0, ExecuteThreadProc,(void*)&(m_lpStatus[m_dwThreadCount].m_dwThreadId),NULL,&m_lpStatus[m_dwThreadCount].m_dwThreadId);

	if (m_lpStatus[m_dwThreadCount].m_hThread == NULL)
	{
		AfxMessageBox("CThreadManage::Init CreateThread Error");
		return 0;
	}
	
	m_lpStatus[m_dwThreadCount].m_dwCurIndex = m_dwThreadCount;

	m_dwThreadCount++;

	return 1;
}

void  CThreadManage::Init()
{
	if (m_dwThreadCount != 0)
		return;
		
	AddThread();
}

CThreadManage* CThreadManage::CreateObj(SOCKET hSocket)
{
	if (m_lpObj == NULL )
	{
		m_lpObj = new CThreadManage;
		m_hSocket = hSocket;
//		m_time = CTime::GetCurrentTime();
		m_csTime = CTime::GetCurrentTime().Format("%Y-%m-%d-%H-%M-%S");
		
		//初始化连接组
		for (int i = 0; i < 10; i++)
		{
			m_theConn[i].InitConn();
		}

		//每当实例存在，则初始化监视线程
		DWORD  dwMonitorThread = 0;
		HANDLE hMonitorThread = CreateThread( NULL, 0, MonitorThreadProc,NULL,NULL,&dwMonitorThread);

		if (hMonitorThread == NULL)
		{
			AfxMessageBox("CThreadManage::CreateObj CreateThread Error");
			return NULL;
		}
	}
	
	return m_lpObj;
}

DWORD CThreadManage::HasFreeThreadOrNot(DWORD dwStatus)
{
	DWORD dwRet = 0;

	for (int i = 0; i < m_dwThreadCount; i++)
	{
		if ( GetStatus(i) != dwStatus)
		{
			dwRet = 1;
			break;
		}
	}
	
	//若有线程挂起则恢复
	if ( i < m_dwThreadCount && dwStatus == Ready )
	{
		ResumeThread(m_lpStatus[i].m_hThread);
	}

	return dwRet;
}

DWORD CThreadManage::GetTask(CTask& taskObj)
{
	m_theSection.Lock();
	
	if ( g_TaskQueue.empty())
	{
		return 0;//空任务队列
	}
	
	taskObj = g_TaskQueue.front();
	
	g_TaskQueue.pop();	
	
	m_theSection.Unlock();

	return 1;
}

//执行任务
BOOL CThreadManage::ExecuteTask(CTask& taskObj,int nConnIndex )
{
	try
	{
		CMyPacket thePacket;
		
		if ( taskObj.m_head.m_dwOperType == AD0_ADD ||
			 taskObj.m_head.m_dwOperType == AD0_DELETE ||
			 taskObj.m_head.m_dwOperType == AD0_MODIFY ||
			 taskObj.m_head.m_dwOperType == AD0_SELECT
			 )
		{
			DWORD dwRet = m_theConn[nConnIndex].ExecuteSql( taskObj.m_csTask,
															taskObj.m_head.m_dwOperType,
															m_csTime);
			
			if ( dwRet != 0 )
			{	
				vector<CString> resList =  m_theConn[nConnIndex].GetRecordSet();
				
				CString csSend = "";
				int nSize = 0;
				
				if ( ( nSize = resList.size() ) != 0)
				{
					for (int i = 0; i < nSize; i++)
					{
						csSend += resList.at(i);
					}
					
					DWORD nNeed = thePacket.Packet((LPTSTR)(LPCTSTR)csSend,NULL,AD0_SELECT);
					
					char* szSendBuf = new char[nNeed];
					
					if (szSendBuf == NULL)
						return 0;
					
					thePacket.Packet((LPTSTR)(LPCTSTR)csSend,szSendBuf,AD0_SELECT);
					
					int nRet = sendto( taskObj.m_hSocket,szSendBuf,nNeed, 0, (sockaddr*)&(taskObj.m_destAddr), sizeof(sockaddr) );
					
					if (szSendBuf != NULL)
						delete [] szSendBuf;
					
					if ( nRet == SOCKET_ERROR )
					{
						closesocket(taskObj.m_hSocket);
						AfxMessageBox("ExecuteTask sendto Error");
						return FALSE;
					}	
				}
				
				m_theConn[nConnIndex].UnSetBusy();	
			}
			else
			{
				return FALSE;
			}
		}
		else if (taskObj.m_head.m_dwOperType == ADO_GETTIME)
		{
			DWORD nNeed = thePacket.Packet( (LPTSTR)(LPCTSTR)m_csTime, NULL, ADO_GETTIME);
			
			char* szSendBuf = new char[nNeed];
			
			if (szSendBuf == NULL)
				return 0;
			
			thePacket.Packet( (LPTSTR)(LPCTSTR)m_csTime, szSendBuf, ADO_GETTIME);
			
			int nRet = sendto( taskObj.m_hSocket,szSendBuf,nNeed, 0, (sockaddr*)&(taskObj.m_destAddr), sizeof(sockaddr) );
			
			if (szSendBuf != NULL)
				delete [] szSendBuf;
			
			if ( nRet == SOCKET_ERROR )
			{
				closesocket(taskObj.m_hSocket);
				AfxMessageBox("ExecuteTask sendto Error");
				return FALSE;
			}	
		}

	}
	catch (...)
	{
		
	}

	return TRUE;
	
}

//单个线程内部处理任务
DWORD CThreadManage::MainWorkFunc(DWORD dwIndex)
{
	try
	{	
		int curConnIndex = -1;
		CTask theTask;

		//寻找空闲连接
		for (int i = 0; i < 10 ; i++)
		{
			if (m_theConn[i].GetStatus() == TRUE)
			{
				continue;
			}
			else
			{
				curConnIndex = i;
				break;
			}
		}
		
		//如果有空闲连接，且有任务 则执行任务
		if ( curConnIndex >= 0 && curConnIndex < 10
			&& GetTask(theTask) != 0 )
		{
			//AfxMessageBox(theTask.m_csTask);
			ExecuteTask(theTask,curConnIndex);
		}
		else
		{
			//若无空闲，则将该线程挂起
			if ( CThreadManage::m_lpStatus[dwIndex].m_hThread != NULL)
			{
				if ( SuspendThread(CThreadManage::m_lpStatus[dwIndex].m_hThread) != -1 )
				{				
					SetStatus(dwIndex, Ready);
				}
			}	
		}

		//执行任务完毕继续循环寻找可执行任务
	}
	catch (...)
	{
		//错误处理
//		GetError();
	}
	
	return ERROR_SUCCESS;
}

DWORD WINAPI ExecuteThreadProc( LPVOID lpParameter  )
{
	DWORD nIndex = *(LPDWORD) lpParameter;

	while (true)
	{
		try
		{
			while (true)
			{
				if ( !g_TaskQueue.empty() )
				{
					CThreadManage::MainWorkFunc(nIndex);
				}

				Sleep(100);
			}
		}
		catch (...)
		{
			
		}
	}
	
	return 0;
}

DWORD WINAPI MonitorThreadProc( LPVOID lpParameter  )
{ 
	while (true)
	{
//  		if (CThreadManage::m_dwThreadCount == 0)
// 		    {
//  			CThreadManage::AddThread();
//  		}

		//有线程空闲则退出
		if (CThreadManage::HasFreeThreadOrNot(Running) == 1)
		{
			Sleep(100);
			continue;
		}
		
		//无空闲线程，若有线程挂起则恢复
		if (CThreadManage::HasFreeThreadOrNot(Ready) == 1)
		{
			Sleep(100);
			continue;
		}
		
		//若无空闲线程，无挂起线程，且未达到线程上限(10)，则新建空闲线程，
		//保证在未到上限的情况下,始终有一空闲线程等待处理任务
		//此处未做测试，因条件有限
		if (CThreadManage::m_dwThreadCount <= 10)
		{
			CThreadManage::AddThread();
		}
	}
}


void  CThreadManage::SetStatus(DWORD dwIndex, DWORD dwStatus)
{
	m_lpStatus[dwIndex].m_dwStatus = dwStatus;
}

DWORD CThreadManage::GetStatus(DWORD dwIndex)
{
	return m_lpStatus[dwIndex].m_dwStatus;
}
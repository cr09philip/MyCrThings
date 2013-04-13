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
		
		//��ʼ��������
		for (int i = 0; i < 10; i++)
		{
			m_theConn[i].InitConn();
		}

		//ÿ��ʵ�����ڣ����ʼ�������߳�
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
	
	//�����̹߳�����ָ�
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
		return 0;//���������
	}
	
	taskObj = g_TaskQueue.front();
	
	g_TaskQueue.pop();	
	
	m_theSection.Unlock();

	return 1;
}

//ִ������
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

//�����߳��ڲ���������
DWORD CThreadManage::MainWorkFunc(DWORD dwIndex)
{
	try
	{	
		int curConnIndex = -1;
		CTask theTask;

		//Ѱ�ҿ�������
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
		
		//����п������ӣ��������� ��ִ������
		if ( curConnIndex >= 0 && curConnIndex < 10
			&& GetTask(theTask) != 0 )
		{
			//AfxMessageBox(theTask.m_csTask);
			ExecuteTask(theTask,curConnIndex);
		}
		else
		{
			//���޿��У��򽫸��̹߳���
			if ( CThreadManage::m_lpStatus[dwIndex].m_hThread != NULL)
			{
				if ( SuspendThread(CThreadManage::m_lpStatus[dwIndex].m_hThread) != -1 )
				{				
					SetStatus(dwIndex, Ready);
				}
			}	
		}

		//ִ��������ϼ���ѭ��Ѱ�ҿ�ִ������
	}
	catch (...)
	{
		//������
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

		//���߳̿������˳�
		if (CThreadManage::HasFreeThreadOrNot(Running) == 1)
		{
			Sleep(100);
			continue;
		}
		
		//�޿����̣߳������̹߳�����ָ�
		if (CThreadManage::HasFreeThreadOrNot(Ready) == 1)
		{
			Sleep(100);
			continue;
		}
		
		//���޿����̣߳��޹����̣߳���δ�ﵽ�߳�����(10)�����½������̣߳�
		//��֤��δ�����޵������,ʼ����һ�����̵߳ȴ���������
		//�˴�δ�����ԣ�����������
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
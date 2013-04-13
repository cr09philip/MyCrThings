// ConnSql.cpp: implementation of the CConnSql class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIMS_SERVER.h"
#include "ConnSql.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConnSql::CConnSql()
{
	m_isBusy	= FALSE;
	m_res		= NULL;
	m_cmd		= NULL;
	m_conn		= NULL; 
}

CConnSql::~CConnSql()
{
	//关闭连接
	if ( m_conn!= NULL && m_conn->State == adStateOpen )
		m_conn->Close();

	m_isBusy	= FALSE;
	m_res		= NULL;
	m_cmd		= NULL;
	m_conn		= NULL; 
}

BOOL CConnSql::InitConn()
{
	if ( FAILED(  m_conn.CreateInstance(L"ADODB.Connection")) )
		return FALSE;
	if ( FAILED(  m_cmd.CreateInstance(L"ADODB.Command")) )
		return FALSE;
	if ( FAILED(  m_res.CreateInstance(L"ADODB.Recordset")) )
		return FALSE;

	char* lpConnectString = "Provider=SQLOLEDB.1;Integrated Security=SSPI; \
			Persist Security Info=False;User ID=sa;Initial Catalog=CR09;Data Source=.";
	
	HRESULT hRet = S_FALSE;
	
	hRet = m_conn->Open(_bstr_t(lpConnectString),
		                _bstr_t(),_bstr_t(),
		                adConnectUnspecified);
  
	return FAILED(hRet);
}

DWORD CConnSql::ExecuteSql(CString& csSql, DWORD dwOperate, CString& csTime)
{
	SetBusy();

	DWORD bRet = 1;

	if ( m_conn->State == adStateOpen )
	{
		if (dwOperate == AD0_SELECT) //ADO_SELECT
		{
			Query(csSql);
		}
		else
		{
			try
			{
				//开始事务
				m_conn->BeginTrans();
				
				try
				{
		  
					VARIANT vt;
					
					VariantInit(&vt);

					m_conn->Execute((_bstr_t)csSql, &vt, adCmdText);
					
//					t = CTime::GetCurrentTime();
					csTime = CTime::GetCurrentTime().Format("%Y-%m-%d-%H-%M-%S");
					//提交事务
					m_conn->CommitTrans();

				}
				catch ( ... )
				{
					//回滚
					m_conn->RollbackTrans();
					bRet = 0;
				}
			}
			catch ( _com_error& err )
			{
				//printf( "%s" , (char*)err.Description() );
				
				bRet = 0;
			}
		}
	}

	return bRet;
}

vector<CString> CConnSql::GetRecordSet()
{
	return m_resList;
}

void CConnSql::SetBusy()
{
	m_isBusy = TRUE;
}

void CConnSql::UnSetBusy()
{
	m_isBusy = FALSE;
	m_res = NULL;
	m_cmd = NULL;
	m_resList.clear();
}

BOOL CConnSql::GetStatus()
{
	return m_isBusy;
}

DWORD CConnSql::Query(CString &csSql)
{
	DWORD bRet = 0;
	try
	{
		//开始事务
		m_conn->BeginTrans();
		
		try
		{
  
			VARIANT vt;
			
			VariantInit(&vt);
			
			m_res = m_conn->Execute((_bstr_t)csSql, &vt, adCmdText);

			while ( !m_res->AdoEOF )
			{
				CString csTxt;
				csTxt.Format("%s %s %s %s %s %s\r\n",
					(char*)_bstr_t(m_res->GetCollect("stu_id")),
					(char*)_bstr_t(m_res->GetCollect("stu_name")), 
					(char*)_bstr_t(m_res->GetCollect("stu_gender")),
					(char*)_bstr_t(m_res->GetCollect("stu_phone")), 
					(char*)_bstr_t(m_res->GetCollect("stu_addr")),
					(char*)_bstr_t(m_res->GetCollect("stu_birth"))
					);
				
				m_resList.push_back(csTxt);
				
				m_res->MoveNext();
				
			}
			
			//提交事务
			m_conn->CommitTrans();
			
		}
		catch ( ... )
		{
			//回滚
			m_conn->RollbackTrans();
			bRet = -1;
		}
	}
	catch ( _com_error& err )
	{
		//printf( "%s" , (char*)err.Description() );
		
		bRet = -1;
	}

	return bRet;
}

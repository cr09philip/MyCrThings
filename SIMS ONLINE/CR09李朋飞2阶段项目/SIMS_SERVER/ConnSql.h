// ConnSql.h: interface for the CConnSql class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNSQL_H__9AC28000_54A5_4B86_BACA_6D5537E9AA1B__INCLUDED_)
#define AFX_CONNSQL_H__9AC28000_54A5_4B86_BACA_6D5537E9AA1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include "Init.h"
#include "MyPacket.h"

#pragma warning(disable:4146)

#import "C:\Program Files\Common Files\System\ado\msado15.dll" \
rename("EOF","AdoEOF")

using namespace ADODB;

#include <vector>
using namespace std;

class CComInit : public CInit
{
public:
	CComInit()
	{
		Init();
	}

	~CComInit()
	{
		Realease();
	}

public:
	void Init()
	{
		CoInitialize(NULL);
	}

	void Realease()
	{
		CoUninitialize();
	}
};


class CConnSql  
{
public:
	CConnSql();
	virtual ~CConnSql();
public:
	BOOL InitConn();
	DWORD ExecuteSql(CString& csSql, DWORD dwOperate, CString& csTime);
	vector<CString> GetRecordSet();

public:
	DWORD Query(CString& csSql);
	void UnSetBusy();
	void SetBusy();
	BOOL GetStatus();
public:
	BOOL		   m_isBusy;

	_ConnectionPtr m_conn; 
	_CommandPtr	   m_cmd;
	_RecordsetPtr  m_res;

	vector<CString> m_resList;
};


#endif // !defined(AFX_CONNSQL_H__9AC28000_54A5_4B86_BACA_6D5537E9AA1B__INCLUDED_)

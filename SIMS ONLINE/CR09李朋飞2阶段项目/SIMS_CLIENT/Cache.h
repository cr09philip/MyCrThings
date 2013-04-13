// Cache.h: interface for the CCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CACHE_H__2F41D68A_5302_44EE_ADBB_BCA2BE05F4FA__INCLUDED_)
#define AFX_CACHE_H__2F41D68A_5302_44EE_ADBB_BCA2BE05F4FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//±æµÿª∫¥Ê¿‡
class CCache  
{
public:
	CCache();
	virtual ~CCache();
public:
	static CString	GetCache();
	static void		SetCache(char* szBuf);

	static CCache*	CreateObj();
	static BOOL		IsNew(CString& csTime);

	static CCache*	m_theObj;
	static CString	m_csCache;
	static CString  m_csTime;
//	static CTime	m_lastTime;
};

#endif // !defined(AFX_CACHE_H__2F41D68A_5302_44EE_ADBB_BCA2BE05F4FA__INCLUDED_)

// Cache.cpp: implementation of the CCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIMS_CLIENT.h"
#include "Cache.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCache* CCache::m_theObj = NULL;
CString CCache::m_csCache = "";
//CTime	CCache::m_lastTime = NULL;
CString CCache::m_csTime = "";

CCache::CCache()
{
	
}

CCache::~CCache()
{
}

CCache* CCache::CreateObj()
{
	if (m_theObj == NULL)
	{
		m_theObj = new CCache;
	}

	return m_theObj;
}

void CCache::SetCache(char* szBuf)
{
	m_csCache.Format("%s", szBuf);
}

CString CCache::GetCache()
{
	return m_csCache;
}

BOOL CCache::IsNew(CString& csTime)
{
	if (m_csTime == csTime)
	{
		return TRUE;
	}

	m_csTime = csTime;

	return FALSE;
}
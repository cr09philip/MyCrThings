// SocketInfo.cpp: implementation of the CSocketInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "SocketInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSocketInfo* CSocketInfo::m_obj = NULL;

CSocketInfo::CSocketInfo()
{

}

CSocketInfo::~CSocketInfo()
{

}

CSocketInfo* CSocketInfo::CreateObj()
{
    if ( m_obj != NULL)
    {
        return NULL;
    }

    return new CSocketInfo;
}

CSocketInfo theSocketInfo;
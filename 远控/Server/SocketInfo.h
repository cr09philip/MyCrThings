// SocketInfo.h: interface for the CSocketInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETINFO_H__6158A5A6_6230_4BAD_8853_D0A42BFA63A8__INCLUDED_)
#define AFX_SOCKETINFO_H__6158A5A6_6230_4BAD_8853_D0A42BFA63A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "struct.h"

using namespace std;

class CSocketInfo  
{
public:
	CSocketInfo();
	virtual ~CSocketInfo();
public:
//    SOCKET m_hServerSocket;
  
    map<int, CONNECT_INFO*> m_ConnMap;

    static CSocketInfo* m_obj;

    static CSocketInfo* CreateObj();
};

#endif // !defined(AFX_SOCKETINFO_H__6158A5A6_6230_4BAD_8853_D0A42BFA63A8__INCLUDED_)

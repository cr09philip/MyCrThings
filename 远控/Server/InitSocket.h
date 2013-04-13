// InitSocket.h: interface for the CInitSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITSOCKET_H__5CA7B923_17C2_403A_85DC_49E06D0F3658__INCLUDED_)
#define AFX_INITSOCKET_H__5CA7B923_17C2_403A_85DC_49E06D0F3658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WINSOCK2.H>

#pragma comment(lib,"Ws2_32.lib") 

class CInitSocket  
{
public:
	CInitSocket();
	virtual ~CInitSocket();
};

#endif // !defined(AFX_INITSOCKET_H__5CA7B923_17C2_403A_85DC_49E06D0F3658__INCLUDED_)

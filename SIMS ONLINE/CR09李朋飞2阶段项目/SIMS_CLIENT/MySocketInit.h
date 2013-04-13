// MySocketInit.h: interface for the CMySocketInit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKETINIT_H__AF5187FB_D5D8_40C2_940E_3315997C245E__INCLUDED_)
#define AFX_MYSOCKETINIT_H__AF5187FB_D5D8_40C2_940E_3315997C245E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>
#pragma comment(lib,"Ws2_32.lib")

class CMySocketInit  
{
public:
	CMySocketInit();
	virtual ~CMySocketInit();
};


#endif // !defined(AFX_MYSOCKETINIT_H__AF5187FB_D5D8_40C2_940E_3315997C245E__INCLUDED_)

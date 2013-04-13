// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__5A87666E_889C_4FEA_B0B1_A870465FD1E5__INCLUDED_)
#define AFX_DIB_H__5A87666E_889C_4FEA_B0B1_A870465FD1E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dibapi.h"

class CDib
{
public:
    CDib();
 
    CDib( HBITMAP hBitmap );
    
    ~CDib();
    
public:
    
    HDIB m_hDIB;
    
    CPalette* m_palDIB;
    
public:
	DWORD GetDibSize();
	HANDLE DDB2DIB(HBITMAP hBitmap);
    
    BOOL Draw(CDC *pDC, int x, int y, int cx=0, int cy=0);
    
    int Width();
    
    int Height();
    
    void DeleteDIB();

//     BOOL Load(LPCTSTR lpszFileName);
// 
//     BOOL LoadFromResource(UINT nID);
};

#endif // !defined(AFX_DIB_H__5A87666E_889C_4FEA_B0B1_A870465FD1E5__INCLUDED_)

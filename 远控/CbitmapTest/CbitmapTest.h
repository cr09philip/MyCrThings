// CbitmapTest.h : main header file for the CBITMAPTEST application
//

#if !defined(AFX_CBITMAPTEST_H__D9F97715_DBAC_498A_AF26_B698DB433132__INCLUDED_)
#define AFX_CBITMAPTEST_H__D9F97715_DBAC_498A_AF26_B698DB433132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCbitmapTestApp:
// See CbitmapTest.cpp for the implementation of this class
//

class CCbitmapTestApp : public CWinApp
{
public:
	CCbitmapTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCbitmapTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCbitmapTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBITMAPTEST_H__D9F97715_DBAC_498A_AF26_B698DB433132__INCLUDED_)

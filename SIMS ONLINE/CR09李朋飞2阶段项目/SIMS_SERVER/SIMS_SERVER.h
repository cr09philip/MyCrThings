// SIMS_SERVER.h : main header file for the SIMS_SERVER application
//

#if !defined(AFX_SIMS_SERVER_H__51761336_59E7_441A_B4DE_70F30DC810EF__INCLUDED_)
#define AFX_SIMS_SERVER_H__51761336_59E7_441A_B4DE_70F30DC810EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSIMS_SERVERApp:
// See SIMS_SERVER.cpp for the implementation of this class
//

class CSIMS_SERVERApp : public CWinApp
{
public:
	CSIMS_SERVERApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIMS_SERVERApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSIMS_SERVERApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMS_SERVER_H__51761336_59E7_441A_B4DE_70F30DC810EF__INCLUDED_)

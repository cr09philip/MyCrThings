// SIMS_CLIENT.h : main header file for the SIMS_CLIENT application
//

#if !defined(AFX_SIMS_CLIENT_H__31B25327_FBB2_412B_8B9E_B5B509195F98__INCLUDED_)
#define AFX_SIMS_CLIENT_H__31B25327_FBB2_412B_8B9E_B5B509195F98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSIMS_CLIENTApp:
// See SIMS_CLIENT.cpp for the implementation of this class
//

class CSIMS_CLIENTApp : public CWinApp
{
public:
	CSIMS_CLIENTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIMS_CLIENTApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSIMS_CLIENTApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMS_CLIENT_H__31B25327_FBB2_412B_8B9E_B5B509195F98__INCLUDED_)

// LDCSDoc.cpp : implementation of the CLDCSDoc class
//

#include "stdafx.h"
#include "LDCS.h"

#include "LDCSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLDCSDoc

IMPLEMENT_DYNCREATE(CLDCSDoc, CDocument)

BEGIN_MESSAGE_MAP(CLDCSDoc, CDocument)
	//{{AFX_MSG_MAP(CLDCSDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLDCSDoc construction/destruction

CLDCSDoc::CLDCSDoc()
{
	// TODO: add one-time construction code here

}

CLDCSDoc::~CLDCSDoc()
{
}

BOOL CLDCSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLDCSDoc serialization

void CLDCSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLDCSDoc diagnostics

#ifdef _DEBUG
void CLDCSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLDCSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLDCSDoc commands

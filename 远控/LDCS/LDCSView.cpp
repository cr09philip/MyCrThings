// LDCSView.cpp : implementation of the CLDCSView class
//

#include "stdafx.h"
#include "LDCS.h"

#include "LDCSDoc.h"
#include "LDCSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLDCSView

IMPLEMENT_DYNCREATE(CLDCSView, CView)

BEGIN_MESSAGE_MAP(CLDCSView, CView)
	//{{AFX_MSG_MAP(CLDCSView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLDCSView construction/destruction

CLDCSView::CLDCSView()
{
	// TODO: add construction code here

}

CLDCSView::~CLDCSView()
{
}

BOOL CLDCSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLDCSView drawing

void CLDCSView::OnDraw(CDC* pDC)
{
	CLDCSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CLDCSView printing

BOOL CLDCSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLDCSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLDCSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CLDCSView diagnostics

#ifdef _DEBUG
void CLDCSView::AssertValid() const
{
	CView::AssertValid();
}

void CLDCSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLDCSDoc* CLDCSView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLDCSDoc)));
	return (CLDCSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLDCSView message handlers

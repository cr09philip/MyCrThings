// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	//ON_NOTIFY(HDN_ITEMCLICKW,0, OnItemclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

void CMyListCtrl::AutoSize()
{
  SetRedraw(FALSE);
  for ( int i = 0 ;i < GetHeaderCtrl()->GetItemCount() ;i++ )
  {
    SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
  }
  SetRedraw(TRUE);
}


void CMyListCtrl::Init()
{
  //修改窗口样式
	LONG nRet = ::GetWindowLong(GetSafeHwnd(),GWL_STYLE);

	::SetWindowLong(GetSafeHwnd(),GWL_STYLE, nRet | LVS_REPORT);

	SetExtendedStyle(
		  GetExtendedStyle() | 
		  LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}

void CMyListCtrl::InsertColumns( CString csTxt1, CString csTxt2, CString csTxt3,
								CString csTxt4,CString csTxt5,CString csTxt6 )
{
	if ( csTxt1 != _T(""))
		CListCtrl::InsertColumn(0,csTxt1);
	if ( csTxt2 != _T(""))
		CListCtrl::InsertColumn(1,csTxt2);
	if ( csTxt3 != _T(""))
		CListCtrl::InsertColumn(2,csTxt3);
	if ( csTxt4 != _T(""))
		CListCtrl::InsertColumn(3,csTxt4);
	if ( csTxt5 != _T(""))
		CListCtrl::InsertColumn(4,csTxt5);
	if ( csTxt6 != _T(""))
		CListCtrl::InsertColumn(5,csTxt6);
}

void CMyListCtrl::OnItemclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	AfxMessageBox(_T("CMyListCtrl::OnItemclick"));

	*pResult = 0;
}

LRESULT CMyListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if ( message == WM_NOTIFY && 
      (((NMHDR*)lParam)->code == HDN_ITEMCLICKW) )
	{
    return CListCtrl::WindowProc(message, wParam, lParam);  
	}

	return CListCtrl::WindowProc(message, wParam, lParam);
}

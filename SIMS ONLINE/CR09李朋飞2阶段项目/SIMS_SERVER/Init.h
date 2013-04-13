// Init.h: interface for the CInit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIT_H__844690FC_0A1B_4ADD_B2D5_E101680056FB__INCLUDED_)
#define AFX_INIT_H__844690FC_0A1B_4ADD_B2D5_E101680056FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInit  
{
public:
	CInit();
	virtual ~CInit();
public:
	virtual void Init() = 0;
	virtual void Realease() = 0;
};

#endif // !defined(AFX_INIT_H__844690FC_0A1B_4ADD_B2D5_E101680056FB__INCLUDED_)

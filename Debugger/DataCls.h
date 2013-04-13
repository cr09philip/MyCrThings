// DataCls.h: interface for the CDataCls class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATACLS_H__DBB8D3D4_984A_4F57_B331_50C4042438EB__INCLUDED_)
#define AFX_DATACLS_H__DBB8D3D4_984A_4F57_B331_50C4042438EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Headers.h"

#include "MyTree.h"
#include "Disasm/disasm.h"

#include "HardBp.h"
#include "CommonBp.h"
#include "MemBp.h"

#include <map>
#include <list>
using namespace std;

class CDataCls  
{
public:
	void InitIatMap();
//	void InitBpCls();
	CDataCls();
	virtual ~CDataCls();
	static CDataCls* CreateObj(HANDLE hProcess);
public:
	static CDataCls* m_obj;
	static HANDLE m_hProcess;

	map<DWORD,char*> m_mapFuncIat;

//	list<PMODULE_STRUCT> m_listModule; //ģ����Ϣ�б�

	CHardBp* m_pObjHardBp;
	CMemBp*  m_pObjMemBp;
	CCommonBp* m_pObjCommonBp;

	char m_szScrName[MAXBYTE];	
	
	//�ű�����
	list<CMD>  m_cmdList;
	//�ű��ļ��ĵ�ǰƫ��
	DWORD m_dwCurFileOffset;
	//�����Ƿ�ӽű���ȡ
	BOOL  m_bIsScriptCmd;

	//ָ���¼
//	CMyTree<t_disasm> m_tree;
	map<DWORD,t_disasm> m_record;
	
	//RT�������ʼ��ַ
	DWORD m_dwRtBegin;
	DWORD m_dwRtEnd;
};

#endif // !defined(AFX_DATACLS_H__DBB8D3D4_984A_4F57_B331_50C4042438EB__INCLUDED_)

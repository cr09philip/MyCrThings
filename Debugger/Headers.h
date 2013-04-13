// Headers.h: interface for the CHeaders class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADERS_H__80A90510_000E_4BD6_932D_D90FF2C94D22__INCLUDED_)
#define AFX_HEADERS_H__80A90510_000E_4BD6_932D_D90FF2C94D22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct command
{
	char szCmd[8];
	char szArgu1[8];
	char szArgu2[8];
	char szArgu3[8];
}CMD,*PCMD;

typedef struct tagModuleInfo {
    HANDLE hFile;
    LPVOID lpBaseOfDll;
    DWORD dwDebugInfoFileOffset;
    DWORD nDebugInfoSize;
    LPVOID lpImageName;
    WORD fUnicode;
} ModuleInfo, *PModuleInfo;

struct LdrNode
{
	LdrNode* lpNext;//0		
	LdrNode* lpPrev;//4
	DWORD    dw1;	//8
	DWORD    dw2;	//c
	DWORD	 dw3;	//10
	DWORD    dw4;	//14
	LPVOID	 lpImageBase;//18
	LPVOID	 lpOep;		//1c
	DWORD	 dw5;	//20
	DWORD	 dw6;	//24
	LPVOID	 lpImagePath;//28
	DWORD	 dw7;//2c
	LPVOID	 lpImageName; //30
};

#endif // !defined(AFX_HEADERS_H__80A90510_000E_4BD6_932D_D90FF2C94D22__INCLUDED_)

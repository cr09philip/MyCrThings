// Debugger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DebugFrame.h"

int main(int argc, char* argv[])
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[MAXBYTE] = {0};       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Exe(*.exe)\0*.exe\0All(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Display the Open dialog box. 
	if ( GetOpenFileName(&ofn) == TRUE ) 
	{
		CDebugFrame dbg;
		dbg.BeginDebug(szFile);
	}

	return 0;
}

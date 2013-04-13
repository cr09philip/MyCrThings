; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CScreenDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Server.h"

ClassCount=4
Class1=CServerApp
Class2=CServerDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SERVER_DIALOG
Resource4=IDD_SCREEN_DLG
Resource5=IDR_MENU
Class4=CScreenDlg
Resource6=CG_IDR_POPUP_SERVER_DLG

[CLS:CServerApp]
Type=0
HeaderFile=Server.h
ImplementationFile=Server.cpp
Filter=N

[CLS:CServerDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
Filter=D
LastObject=IDM_STOP_SERVER
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERVER_DIALOG]
Type=1
Class=CServerDlg
ControlCount=1
Control1=IDC_LIST,SysListView32,1350631685

[MNU:IDR_MENU]
Type=1
Class=?
Command1=IDM_START_SERVER
Command2=IDM_STOP_SERVER
Command3=IDM_QUIT
CommandCount=3

[MNU:CG_IDR_POPUP_SERVER_DLG]
Type=1
Class=?
Command1=ID_GETDISPLAY
Command2=ID_FILE_MANAGE
Command3=ID_FAR_CMD
CommandCount=3

[DLG:IDD_SCREEN_DLG]
Type=1
Class=CScreenDlg
ControlCount=0

[CLS:CScreenDlg]
Type=0
HeaderFile=ScreenDlg.h
ImplementationFile=ScreenDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC


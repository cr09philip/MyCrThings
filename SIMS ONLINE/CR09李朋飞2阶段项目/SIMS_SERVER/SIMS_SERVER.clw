; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSIMS_SERVERDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SIMS_SERVER.h"

ClassCount=3
Class1=CSIMS_SERVERApp
Class2=CSIMS_SERVERDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_SIMS_SERVER_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDD_DIALOG1

[CLS:CSIMS_SERVERApp]
Type=0
HeaderFile=SIMS_SERVER.h
ImplementationFile=SIMS_SERVER.cpp
Filter=N

[CLS:CSIMS_SERVERDlg]
Type=0
HeaderFile=SIMS_SERVERDlg.h
ImplementationFile=SIMS_SERVERDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_START_BTN

[CLS:CAboutDlg]
Type=0
HeaderFile=SIMS_SERVERDlg.h
ImplementationFile=SIMS_SERVERDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SIMS_SERVER_DIALOG]
Type=1
Class=CSIMS_SERVERDlg
ControlCount=3
Control1=IDC_STATUS_STATIC,static,1342308352
Control2=IDC_START_BTN,button,1342242816
Control3=IDC_END_BTN,button,1342242816

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816


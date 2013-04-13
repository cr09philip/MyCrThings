; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSIMS_CLIENTDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SIMS_CLIENT.h"

ClassCount=3
Class1=CSIMS_CLIENTApp
Class2=CSIMS_CLIENTDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SIMS_CLIENT_DIALOG
Resource4=IDD_DIALOG1
Resource5=CG_IDR_POPUP_SIMS_CLIENTDLG

[CLS:CSIMS_CLIENTApp]
Type=0
HeaderFile=SIMS_CLIENT.h
ImplementationFile=SIMS_CLIENT.cpp
Filter=N

[CLS:CSIMS_CLIENTDlg]
Type=0
HeaderFile=SIMS_CLIENTDlg.h
ImplementationFile=SIMS_CLIENTDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CSIMS_CLIENTDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=SIMS_CLIENTDlg.h
ImplementationFile=SIMS_CLIENTDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SIMS_CLIENT_DIALOG]
Type=1
Class=CSIMS_CLIENTDlg
ControlCount=25
Control1=IDC_CONNECTION_BTN,button,1073807361
Control2=IDC_QUREY_BTN,button,1342242816
Control3=IDC_EDIT1,edit,1353783492
Control4=IDC_ADD_BTN,button,1073807360
Control5=IDC_MODIFY_BTN,button,1073807360
Control6=IDC_DEL_BTN,button,1073807360
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308353
Control9=IDC_STATIC,static,1342308353
Control10=IDC_STATIC,static,1342308353
Control11=IDC_STATIC,static,1342308353
Control12=IDC_STATIC,static,1342308353
Control13=IDC_STATIC,static,1342308353
Control14=IDC_ID_EDIT,edit,1350631552
Control15=IDC_NAME_EDIT,edit,1350631552
Control16=IDC_GENDER_EDIT,edit,1350631552
Control17=IDC_PHONE_EDIT,edit,1350631552
Control18=IDC_ADDR_EDIT,edit,1350631552
Control19=IDC_BIRTH_EDIT,edit,1350631552
Control20=IDC_STATIC,button,1342177287
Control21=IDC_ADD_RADIO,button,1342308361
Control22=IDC_DEL_RADIO,button,1342177289
Control23=IDC_MODIFY_RADIO,button,1342177289
Control24=IDC_OPER_OK_BTN,button,1342242816
Control25=IDC_CLEAR_BTN,button,1342242816

[MNU:CG_IDR_POPUP_SIMS_CLIENTDLG]
Type=1
Class=?
Command1=ID_ADD
Command2=ID_DEL
CommandCount=2

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816


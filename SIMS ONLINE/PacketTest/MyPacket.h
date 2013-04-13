// MyPacket.h: interface for the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKET_H__42730E34_B308_4A57_9057_3B699CFE922A__INCLUDED_)
#define AFX_MYPACKET_H__42730E34_B308_4A57_9057_3B699CFE922A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>

typedef enum _OPERATE_TYPE
{
    RequestForConn = 1,
    SuccessConn,
    TransportData,
    EndData
}OPERATE_TYPE;

typedef struct _PACKAGE_HEADER
{   
    OPERATE_TYPE    m_opType;       //��������
    sockaddr_in     m_addr;         //�ͻ��˵�ַ��˿���Ϣ
    DWORD           m_dwCheckSum;   //�������ݵļ����
    DWORD           m_dwTotalSize;  //�ܰ��Ĵ�С(Bytes)
    DWORD           m_dwIndex;      //���������
    DWORD           m_ContentLen;   //�������ݵĳ���
}PACKAGE_HEADER,*PPACKAGE_HEADER;

class CMyPacket  
{
public:
    PACKAGE_HEADER  m_packHeader;
    char            m_szContent[0];
public:
    //��������ȷ��
    BOOL IsValidPackage();
};

#endif // !defined(AFX_MYPACKET_H__42730E34_B308_4A57_9057_3B699CFE922A__INCLUDED_)

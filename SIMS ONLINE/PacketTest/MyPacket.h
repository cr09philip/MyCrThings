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
    OPERATE_TYPE    m_opType;       //操作类型
    sockaddr_in     m_addr;         //客户端地址与端口信息
    DWORD           m_dwCheckSum;   //本包内容的检验和
    DWORD           m_dwTotalSize;  //总包的大小(Bytes)
    DWORD           m_dwIndex;      //本包的序号
    DWORD           m_ContentLen;   //本包内容的长度
}PACKAGE_HEADER,*PPACKAGE_HEADER;

class CMyPacket  
{
public:
    PACKAGE_HEADER  m_packHeader;
    char            m_szContent[0];
public:
    //检查包的正确性
    BOOL IsValidPackage();
};

#endif // !defined(AFX_MYPACKET_H__42730E34_B308_4A57_9057_3B699CFE922A__INCLUDED_)

// MyTree.h: interface for the CMyTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTREE_H__32B23398_02B7_4B6A_B0DA_542FFEE4D4A2__INCLUDED_)
#define AFX_MYTREE_H__32B23398_02B7_4B6A_B0DA_542FFEE4D4A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stack>
using namespace std;

template <typename T>
struct CTreeNode  
{
    CTreeNode(T nData):m_lpData(nData)
    {
        m_lpParent = m_lpLeft = m_lpRight = NULL;
        
        m_nHeight = 0;
        
        m_nSubHeight = 0;
    }
	
	
    int  m_nDelFlag;
    T  m_lpData;
    CTreeNode<T>* m_lpSelf;
    CTreeNode<T>* m_lpParent;
    CTreeNode<T>* m_lpLeft;
    CTreeNode<T>* m_lpRight;
    //�߶� //����
    int       m_nHeight;
    //���������߶Ȳ�ֵ
    int       m_nSubHeight;
};

template <typename T>
class CMyTree  
{
public:

    typedef void (CMyTree<T>::*FUNTYPE)( CTreeNode<T>* lpCurNode );

	CMyTree();

	virtual ~CMyTree();

public:

    //������
    bool InsertNode( const T& nData );

    //ɾ�����
    bool RemoveNode( const T& nData );

    bool RemoveNode( CTreeNode<T>* lpDelNode );

    //���ҽ��
    CTreeNode<T>* FindNode( const T& nData );

public:

    //�������
    void PrePrint( CTreeNode<T>* lpCurNode , FUNTYPE lpFun = NULL);
    //��
    void MidPrint( CTreeNode<T>* lpCurNode , FUNTYPE lpFun = NULL);
    //��
    void AftPrint( CTreeNode<T>* lpCurNode , FUNTYPE lpFun = NULL);

    //
    void PreOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun = NULL);
    void MidOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun = NULL);
    void AftOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun = NULL);
    void LayerOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun = NULL);
        
    CTreeNode<T>* GetRoot() const
    {
        return m_lpRoot;
    }
    
public:
    CTreeNode<T>* FindMaxNode( CTreeNode<T>* lpCurNode );
    
//     void PrintNode( CTreeNode<T>* lpCurNode )
//     {
//         cout << lpCurNode->m_nData << '\t' << flush;    
//     }
//     
//     void PrintLayerNode( CTreeNode<T>* lpCurNode )
//     {
//         static CTreeNode<T>* lpLastPrintNode = lpCurNode;
// 
//         if ( lpCurNode->m_nHeight > lpLastPrintNode->m_nHeight )
//         {
//             cout << endl;
//         }
// 
//         cout << lpCurNode->m_nData << "[" 
//              << lpCurNode->m_nHeight << "]" 
//              <<  '\t'  << flush;    
// 
//         lpLastPrintNode = lpCurNode;
//     }

    void DeleteNode( CTreeNode<T>* lpCurNode )
    {
        if ( lpCurNode != NULL )
        {
            delete lpCurNode;

            m_nCount--;
        }  
    }

private:
    
    //root node
    CTreeNode<T>* m_lpRoot;

    //count
    int       m_nCount;

    //tree hight
    int       m_nTreeHeight;
};

template < typename T >
CMyTree<T>::CMyTree()
{
    m_lpRoot = NULL;
    
    m_nCount = 0;

    m_nTreeHeight = 0;
}

template < typename T >
CMyTree<T>::~CMyTree()
{
    AftOrder( m_lpRoot, DeleteNode );

    m_nTreeHeight = 0;

    m_nCount = 0;
}

//������
template < typename T >
bool CMyTree<T>::InsertNode( const T& nData )
{
    //1 �ҵ�����ڵ㴦 
    
    //    8
    // 2     10      [11]

    CTreeNode<T>* lpFindNode = m_lpRoot;

    //�ҵ�Ҫ����ڵ㴦
    while ( lpFindNode )
    {
        if ( nData >= lpFindNode->m_lpData )
        {
            if ( lpFindNode->m_lpRight == NULL )
                break;

            lpFindNode = lpFindNode->m_lpRight;
        }
        else
        {
            if ( lpFindNode->m_lpLeft == NULL )
                break;

            lpFindNode = lpFindNode->m_lpLeft;
        }
    }

    CTreeNode<T>* lpNew = new CTreeNode<T>(nData);

    //11.parent = 10
    lpNew->m_lpParent = lpFindNode;

    //10.right/left = 11
    if ( lpFindNode != NULL )
    {
        if (  lpNew->m_lpData >= lpFindNode->m_lpData  )
            lpFindNode->m_lpRight = lpNew;
        else
            lpFindNode->m_lpLeft = lpNew;
    }

    if ( lpNew->m_lpParent != NULL )
        lpNew->m_nHeight = lpNew->m_lpParent->m_nHeight+1;
    else
    {
        lpNew->m_nHeight = 1;
        m_lpRoot = lpNew;
    }

    //���ĸ߶�
    if ( lpNew->m_nHeight > m_nTreeHeight )
    {
        m_nTreeHeight = lpNew->m_nHeight;
    }

    m_nCount++;

    return true;
}

//ɾ�����
template < typename T >
bool CMyTree<T>::RemoveNode( const T& nData )
{
    CTreeNode<T>* lpDelNode = FindNode(nData);

    if ( lpDelNode == NULL )
        return false;

    return RemoveNode(lpDelNode);
}

//ɾ�����
template < typename T >
bool CMyTree<T>::RemoveNode( CTreeNode<T>* lpDelNode )
{
    if ( lpDelNode == NULL )
        return false;


    CTreeNode<T>* lpParent = lpDelNode->m_lpParent;

    if ( lpDelNode->m_lpLeft == NULL &&
         lpDelNode->m_lpRight == NULL )
    {
        //1 ɾ��Ҷ��
        /*
                      P
                [d]      10

        */

        // P.Left/Right = NULL;
       
        if ( lpParent != NULL )
        {
            if ( lpParent->m_lpLeft == lpDelNode )
                lpParent->m_lpLeft = NULL;
            else
                lpParent->m_lpRight = NULL;
        }
        else
            m_lpRoot = NULL;
    }
    else if ( lpDelNode->m_lpLeft != NULL &&
              lpDelNode->m_lpRight != NULL )
    {
        //2 ɾ���ڵ��к��� ��Ҷ��
        //�ҵ������������ֵ(5)���滻ɾ�ڵ�(D)
        /*
                                       9
                            [7]             10
              4                    8  
           2       4.5
                         [5]    
                      4.8   

        ====>
        
                                     9
                        [5]                   10
             4                 8  
          2     4.5
                    4.8
        */

        //5
        CTreeNode<T>* lpChangeNode = FindMaxNode(lpDelNode->m_lpLeft);
        //4.8
        CTreeNode<T>* lpChangeLeftNode = lpChangeNode->m_lpLeft;
        //8
        CTreeNode<T>* lpDelRightNode = lpDelNode->m_lpRight;    
        //4
        CTreeNode<T>* lpDelLeftNode = lpDelNode->m_lpLeft;   
        //4.5
        CTreeNode<T>* lpChangeParentNode = lpChangeNode->m_lpParent;

        
        //   P.Left = 5      5.Parent = P  
        if ( lpParent != NULL )
            lpParent->m_lpLeft = lpChangeNode;
        else
            m_lpRoot = lpChangeNode; 
  
        lpChangeNode->m_lpParent = lpParent;

        //   5.Right = 8          8.Parent = 5
        lpChangeNode->m_lpRight = lpDelRightNode;
        lpDelRightNode->m_lpParent = lpChangeNode;


        //   4.8.Parent = 4.5      4.5.Right =  4.8
        if ( lpChangeLeftNode!= NULL)
            lpChangeLeftNode->m_lpParent = lpChangeParentNode;
        lpChangeParentNode->m_lpRight = lpChangeLeftNode;

        //   5.Left = 4      4.Parent = 5
        if ( lpChangeNode != lpDelLeftNode )
        {
            lpChangeNode->m_lpLeft = lpDelLeftNode;
            lpDelLeftNode->m_lpParent = lpChangeNode;
        }
    }
    else
    {
        //һ��Ϊ��
        /*
                          P
                   [D]
             NULL      R  
        */

        CTreeNode<T>* lpChildNode = lpDelNode->m_lpRight?lpDelNode->m_lpRight:lpDelNode->m_lpLeft;


        //P.Left/Right = R
        if ( lpParent != NULL )
        {
            if ( lpParent->m_lpRight == lpDelNode )
                lpParent->m_lpRight = lpChildNode;
            else
                lpParent->m_lpLeft = lpChildNode;
        }
        else
            m_lpRoot = lpChildNode;
        
        //R.Prarent = P
        lpChildNode->m_lpParent= lpParent;
    }

    DeleteNode(lpDelNode);

    m_nCount--;

    return true;   
}

//���ҽ��
template < typename T >
CTreeNode<T>* CMyTree<T>::FindNode( const T& nData )
{
    //        10
    //   1          15    
    //-1    2    11     16     [11]


    CTreeNode<T>* lpFindNode = m_lpRoot;
    
    while ( lpFindNode )
    {
        if ( nData == lpFindNode->m_nData )
        {
            break;
        }

        if ( nData > lpFindNode->m_nData )
            lpFindNode = lpFindNode->m_lpRight;
        else
            lpFindNode = lpFindNode->m_lpLeft;
    }

    return lpFindNode;    
}


//�������
template < typename T >
void CMyTree<T>::PrePrint( CTreeNode<T>* lpCurNode, FUNTYPE lpFun )
{
    //D L R

    if ( lpCurNode != NULL )
    {
        if ( lpFun != NULL )
            (this->*lpFun)(lpCurNode);

        PrePrint( lpCurNode->m_lpLeft,lpFun );
        
        PrePrint( lpCurNode->m_lpRight,lpFun );
    }
}

//�������
template < typename T >
void CMyTree<T>::MidPrint( CTreeNode<T>* lpCurNode, FUNTYPE lpFun )
{
    //L D R
    
    if ( lpCurNode != NULL )
    {
        MidPrint( lpCurNode->m_lpLeft,lpFun );

        if ( lpFun != NULL )
            (this->*lpFun)(lpCurNode);
        
        MidPrint( lpCurNode->m_lpRight,lpFun );
    }
}


//�������
template < typename T >
void CMyTree<T>::AftPrint( CTreeNode<T>* lpCurNode, FUNTYPE lpFun )
{
    //L R D
    
    if ( lpCurNode != NULL )
    {
        AftPrint( lpCurNode->m_lpLeft,lpFun );
        
        AftPrint( lpCurNode->m_lpRight,lpFun );

        if ( lpFun != NULL )
            (this->*lpFun)(lpCurNode);
    }
}

//������� 
template < typename T >
void CMyTree<T>::PreOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun )
{
    //D L R
    /*
                  8
           10           9
       1       2     7     13
         3                     14
    */

    //   8  10  1 3 2  9 7 13 14
    MyStackTable< CTreeNode<T>* > MyStack;

    while ( true )
    {
        //û�д�����ɵ����� ��ջ  �ȴ����
        while ( lpCurNode )
        {
            if ( lpFun != NULL )
                (this->*lpFun)(lpCurNode);

            //��ջ       
            MyStack.push( lpCurNode );

            lpCurNode = lpCurNode->m_lpLeft;
        }

        
        //��ջ  ��������
        if ( !MyStack.pop( lpCurNode ) )
        {
            break;
        }

        //����ǰ���
        //
        
        //���������������
        lpCurNode = lpCurNode->m_lpRight;
    }
}

//������� 
template < typename T >
void CMyTree<T>::MidOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun )
{
    // L D R
    /*
                  8
           10           9
       1       2     7     13
         3                     14
    */

    //  1 3 10 2 8 7 9 13 14
    MyStackTable< CTreeNode<T>* > MyStack;

    while ( true )
    {
        //û�д�����ɵ����� ��ջ  �ȴ����
        while ( lpCurNode )
        {
            //��ջ       
            MyStack.push( lpCurNode );

            lpCurNode = lpCurNode->m_lpLeft;
        }

        //��ջ  ��������
        if ( !MyStack.pop( lpCurNode ) )
        {
            break;
        }

        //����ǰ���
        if ( lpFun != NULL )
            (this->*lpFun)(lpCurNode);
        
        //���������������
        lpCurNode = lpCurNode->m_lpRight;
    }
}

//������� 
template < typename T >
void CMyTree<T>::AftOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun )
{
    // L R D
    /*
                  8
           10           9
       1       2     7     13
         3                     14
    */

    //  3 1 2 10 7 14 13 9 8 
    CTreeNode<T>* lpFlagNode = NULL;

    stack< CTreeNode<T>* > MyStack;

    while ( true )
    {
        //û�д�����ɵ����� ��ջ  �ȴ����
        while ( lpCurNode )
        {
            //��ջ       
            MyStack.push( lpCurNode );
            lpCurNode = lpCurNode->m_lpLeft;
        }

        //��ջ
//         if ( !MyStack.pop(lpCurNode) )  
//             break;
		lpCurNode = MyStack.top();
		MyStack.pop();
		if ( lpCurNode == NULL)
			break;

        //2.��ӡ����
        if ( lpCurNode->m_lpRight == NULL ||
             lpCurNode->m_lpRight == lpFlagNode )
        {
            //��־�ڵ� ���������� ���˵�·��
            lpFlagNode = lpCurNode;

            if ( lpFun != NULL )
                (this->*lpFun)(lpCurNode);

            lpCurNode = NULL;

            continue;
        }
        else
        {
            //1.����δ��� ��ջ
            MyStack.push( lpCurNode );
        }

        lpCurNode = lpCurNode->m_lpRight;
    }
}

//����
template < typename T >
void CMyTree<T>::LayerOrder( CTreeNode<T>* lpCurNode, FUNTYPE lpFun )
{
    /*
                  8
           10           9
       1       2     7     13
         3                     14
    */

    //  8 10  9 1 2 7 13 3 14

    
    MyQueue< CTreeNode<T>* > theQueue;

    while ( lpCurNode )
    {
        if ( lpFun != NULL )
        {
            (this->*lpFun)(lpCurNode);
        }

        if ( lpCurNode->m_lpLeft != NULL )
            theQueue.EnQueue(lpCurNode->m_lpLeft);

        if ( lpCurNode->m_lpRight != NULL )
            theQueue.EnQueue(lpCurNode->m_lpRight);

        if ( !theQueue.DeQueue(lpCurNode) )
        {
            break;
        }
    }
}

template < typename T >
CTreeNode<T>* CMyTree<T>::FindMaxNode(CTreeNode<T>* lpCurNode)
{
    while ( lpCurNode )
    {
        if ( lpCurNode->m_lpRight == NULL )
             break;

        lpCurNode = lpCurNode->m_lpRight;
    } 

    return lpCurNode;
}

#endif // !defined(AFX_MYTREE_H__32B23398_02B7_4B6A_B0DA_542FFEE4D4A2__INCLUDED_)

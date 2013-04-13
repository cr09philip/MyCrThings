// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()
{
    m_palDIB    =NULL;
    m_hDIB      = NULL;
}

CDib::CDib(HBITMAP hBitmap) 
{
    m_hDIB = (HDIB)DDB2DIB(hBitmap);

    m_palDIB = new CPalette;
    
    if (CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
    {
        // DIB有可能没有调色板
        delete m_palDIB;
        m_palDIB = NULL;
    }
}

CDib::~CDib()
{
    DeleteDIB();
}

void CDib::DeleteDIB()
{
    if (m_hDIB != NULL)
    {
        ::GlobalFree((HGLOBAL) m_hDIB);
    }

    if (m_palDIB != NULL)
    {
        delete m_palDIB;
    }
}
/*
//从文件中载入DIB
BOOL CDib::Load(LPCTSTR lpszFileName)
{
    HDIB hDIB;
    CFile file;
    CFileException fe;

    if (!file.Open(lpszFileName, CFile::modeRead|CFile::shareDenyWrite, &fe))
    {
        AfxMessageBox(fe.m_cause);
        return FALSE;
    }
    TRY
    {
 //       hDIB = ::ReadDIBFile(file);
    }
    CATCH (CFileException, eLoad)
    {
        file.Abort();
        return FALSE;
    }
    END_CATCH

    DeleteDIB(); //清除旧位图

    m_hDIB = hDIB;

    m_palDIB = new CPalette;

    if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
    {
        // DIB有可能没有调色板
        delete m_palDIB;
        m_palDIB = NULL;
    }
    return TRUE;
}

//从资源中载入DIB
BOOL CDib::LoadFromResource(UINT nID)
{
    HINSTANCE hResInst = AfxGetResourceHandle();
    HRSRC hFindRes;
    HDIB hDIB;
    LPSTR pDIB;
    LPSTR pRes;
    HGLOBAL hRes;

    //搜寻指定的资源
    hFindRes = ::FindResource(hResInst, MAKEINTRESOURCE(nID), RT_BITMAP);

    if (hFindRes == NULL) 
    {
        return FALSE;
    }

    hRes = ::LoadResource(hResInst, hFindRes); //载入位图资源

    if (hRes == NULL) 
    {
        return FALSE;
    }

    DWORD dwSize=::SizeofResource(hResInst,hFindRes);

    hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize);

    if (hDIB == NULL) 
    {
        return FALSE;
    }

    pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDIB);

    pRes = (LPSTR) ::LockResource(hRes);

    memcpy(pDIB, pRes, dwSize); //把hRes中的内容复制hDIB中

    ::GlobalUnlock((HGLOBAL) hDIB);

    DeleteDIB();

    m_hDIB=hDIB;

    m_palDIB = new CPalette;

    if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
    {
        // DIB有可能没有调色板
        delete m_palDIB;
        m_palDIB = NULL;
    }

    return TRUE;

}
*/
int CDib::Width()
{
    if(m_hDIB==NULL) 
    {
        return 0;
    }

    LPSTR lpDIB = (LPSTR)::GlobalLock((HGLOBAL) m_hDIB);

    int cxDIB = (int)::DIBWidth(lpDIB); // Size of DIB - x

    ::GlobalUnlock((HGLOBAL) m_hDIB);

    return cxDIB;
}

int CDib::Height()
{
    if(m_hDIB==NULL) 
    {
        return 0;
    }

    LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);

    int cyDIB = (int) ::DIBHeight(lpDIB); // Size of DIB - y

    ::GlobalUnlock((HGLOBAL) m_hDIB);

    return cyDIB;

}

//显示DIB，该函数具有缩放功能
//参数x和y说明了目的矩形的左上角坐标，cx和cy说明了目的矩形的尺寸
//cx和cy若有一个为0则该函数按DIB的实际大小绘制，cx和cy的缺省值是0

BOOL CDib::Draw(CDC *pDC, int x, int y, int cx, int cy)
{
    if(m_hDIB == NULL) 
    {
        return FALSE;
    }

    CRect rDIB,rDest;

    rDest.left=x;
    rDest.top=x;

    if(cx==0||cy==0)
    {
        cx=Width();
        cy=Height();
    }

    rDest.right = rDest.left+cx;

    rDest.bottom = rDest.top+cy;

    rDIB.left = rDIB.top = 0;

    rDIB.right=Width();

    rDIB.bottom=Height();

    return ::PaintDIB(pDC->GetSafeHdc(),&rDest,m_hDIB,&rDIB,m_palDIB);

}

HANDLE CDib::DDB2DIB(HBITMAP hBitmap)
{
    BITMAP              bm;
    BITMAPINFOHEADER    bi;
    BITMAPINFOHEADER    *lpbi;
    DWORD               dwLen;
    HANDLE              hDIB;
    HDC                 hDC;
    WORD                wBits;
    HPALETTE            hPal=NULL;

    if (!hBitmap)
    {
        return NULL;
    }

    ::GetObject (hBitmap, sizeof (bm), (LPSTR)&bm);

    wBits =  bm.bmBitsPixel;

    bi.biSize               = sizeof (BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = wBits;
    bi.biCompression        = BI_RGB;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    dwLen  = bi.biSize + PaletteSize((LPSTR) &bi);
    
    hDIB   = GlobalAlloc(GMEM_DDESHARE|GMEM_MOVEABLE,dwLen);
    if (!hDIB)
    {
        return NULL;
    }

    lpbi   = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    *lpbi  = bi;

    HWND hWnd = ::GetFocus();

    if(!hWnd)
    {
        //load default system palette
        hPal   = (HPALETTE)GetStockObject (DEFAULT_PALETTE);
        hDC    = ::GetDC (NULL);
        hPal   = SelectPalette (hDC, hPal, FALSE);
        RealizePalette(hDC);
    }
    else
    {
        //or get palette from focused window
        HDC hDCw    = ::GetDC (hWnd);
        hPal   = SelectPalette (hDCw, (HPALETTE)GetStockObject (DEFAULT_PALETTE), FALSE);
        hDC    = ::GetDC (NULL);
        hPal   = SelectPalette (hDC, hPal, FALSE);
        RealizePalette(hDC);
        SelectPalette (hDCw, hPal, FALSE);
        ::ReleaseDC(hWnd,hDCw);
    }

    // calculate the biSizeImage
    GetDIBits (hDC,
              hBitmap,
              0,
              (WORD) bi.biHeight,
              NULL,
              (LPBITMAPINFO) lpbi,
              DIB_RGB_COLORS);
 
    bi = *lpbi;
    GlobalUnlock(hDIB);

    // If the driver did not fill in the biSizeImage field, 
    // make one up
    if (bi.biSizeImage == 0)
    {
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * wBits) * bm.bmHeight;
        if (bi.biCompression != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    DWORD sl = dwLen;
    HGLOBAL hPtr = hDIB;
    dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage;

    // if you have plans to use DDE or clipboard, you have
    // to allocate memory with GMEM_DDESHARE flag
    if(!(hDIB = GlobalAlloc(GMEM_DDESHARE|GMEM_MOVEABLE,dwLen))) 
    {
        SelectPalette(hDC,hPal,FALSE);
        ::ReleaseDC(NULL,hDC);
        GlobalFree(hPtr);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    LPBITMAPINFOHEADER lpS=(LPBITMAPINFOHEADER)GlobalLock(hPtr);

    CopyMemory(lpbi,lpS,sl);
    GlobalUnlock(hPtr);
    GlobalFree(hPtr);

    // actually fill lpBits
    if ( GetDIBits( hDC,
                    hBitmap,
                    0,
                    (WORD) bi.biHeight,
                    (LPSTR) lpbi + (WORD) lpbi->biSize + PaletteSize((LPSTR) lpbi),
                    (LPBITMAPINFO) lpbi, DIB_RGB_COLORS) == 0)
    {
        GlobalUnlock (hDIB);
        hDIB = NULL;
        SelectPalette (hDC, hPal, FALSE);
        ::ReleaseDC (NULL, hDC);
        return NULL;
    }

    bi = *lpbi;
    GlobalUnlock (hDIB);
    SelectPalette (hDC, hPal, FALSE);
    ::ReleaseDC (NULL, hDC);
    //OK
    return hDIB; 
}

DWORD CDib::GetDibSize()
{
    if ( m_hDIB == NULL )
    {
        return 0;
    }

    return GlobalSize(m_hDIB);
}

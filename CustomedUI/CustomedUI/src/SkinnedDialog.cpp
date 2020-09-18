#include "stdafx.h"
#include "SkinnedDialog.h"
#include <olectl.h>

#define WS_EX_LAYERED 0x00080000 

/////////////////////////////////////////////////////////////////////////////
// CSkinnedDialog dialog

lpfnSetLayeredWindowAttributes CSkinnedDialog::m_pSetLayeredWindowAttributes = 0;
int CSkinnedDialog::m_nReferCount=0;
CCriticalSection CSkinnedDialog::m_csRefer;

IMPLEMENT_DYNAMIC(CSkinnedDialog, CDialog)

CSkinnedDialog::CSkinnedDialog(CWnd* pParent /*=NULL*/) :
m_hBitmap(NULL), m_bTransparentColor(false), m_bTransparent(false), m_btDensity(0)
{
	m_csRefer.Lock();
	
	m_nReferCount++;
	if(m_pSetLayeredWindowAttributes == 0)
	{
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		m_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	}
	
	m_csRefer.Unlock();

	m_colTransparent = RGB(0,255,0);

	m_bStretchX = FALSE;
	m_bStretchY = FALSE;

}

CSkinnedDialog::CSkinnedDialog(UINT uResourceID, CWnd* pParent)
: CDialog(uResourceID, pParent), m_hBitmap(NULL), m_bTransparentColor(false), m_bTransparent(false), m_btDensity(0)
{
	m_csRefer.Lock();
	
	m_nReferCount++;
	if(m_pSetLayeredWindowAttributes == 0)
	{
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		m_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	}
	
	m_csRefer.Unlock();

	m_colTransparent = RGB(0,255,0);

	m_bStretchX = FALSE;
	m_bStretchY = FALSE;
}

CSkinnedDialog::~CSkinnedDialog()
{ 
	m_csRefer.Lock();
	
	m_nReferCount--;
	if(m_nReferCount == 0)
	{
		m_pSetLayeredWindowAttributes = 0;
	}

	m_csRefer.Unlock();

    FreeResources();
}

BOOL CSkinnedDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    
	return TRUE;
}

void CSkinnedDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSkinnedDialog)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSkinnedDialog, CDialog)
//{{AFX_MSG_MAP(CSkinnedDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinnedDialog message handlers

void CSkinnedDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
    PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	

    CDialog::OnLButtonDown(nFlags, point);
}

void CSkinnedDialog::FreeResources()
{
    if (m_hBitmap)
        ::DeleteObject (m_hBitmap);

    m_hBitmap = NULL;
}

DWORD CSkinnedDialog::SetBitmap(HBITMAP hBitmap)
{
    int nRetValue;
    BITMAP  csBitmapSize;

    FreeResources();
    
    if (hBitmap)
    {
        m_hBitmap = hBitmap;

        nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
        if (nRetValue == 0)
        {
            FreeResources();
            return 0;
        }
        m_dwWidth = (DWORD)csBitmapSize.bmWidth;
        m_dwHeight = (DWORD)csBitmapSize.bmHeight;
    }
    
    if (IsWindow(this->GetSafeHwnd()))
        Invalidate();
    
    return 1;
    
}

BOOL CSkinnedDialog::setTransparentBG (BYTE bAlpha, BOOL bTrans)
{
    if(m_pSetLayeredWindowAttributes == NULL)
        return FALSE;

    if(bTrans)
	{
        SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		if(m_bTransparentColor)
		{
			m_pSetLayeredWindowAttributes(m_hWnd, m_colTransparent, bAlpha, LWA_COLORKEY | LWA_ALPHA);
		}
		else
		{
			m_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

		m_btDensity = bAlpha;
		m_bTransparent = true;
    }
    else
	{
		if(m_bTransparentColor)
		{
			SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			m_pSetLayeredWindowAttributes(m_hWnd, m_colTransparent, 0, LWA_COLORKEY);
		}
		else
		{
			SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		}

		m_bTransparent = false;

		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
    }

    return TRUE;
}


BOOL CSkinnedDialog::setTransparentColor(COLORREF col, BOOL bTrans)
{
    if (m_pSetLayeredWindowAttributes == NULL)
        return FALSE;

    if (bTrans)
	{
        SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		if(m_bTransparent)
		{
			m_pSetLayeredWindowAttributes(m_hWnd, m_colTransparent, m_btDensity, LWA_COLORKEY | LWA_ALPHA);
		}
		else
		{
			m_pSetLayeredWindowAttributes(m_hWnd, col, 0, LWA_COLORKEY);
		}   

		m_colTransparent = col;
		m_bTransparentColor = true;
    }
    else
	{
		if(m_bTransparent)
		{
			SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			m_pSetLayeredWindowAttributes(m_hWnd, 0, m_btDensity, LWA_ALPHA);
		}
		else
		{
			SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		}

        ::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);

		m_bTransparentColor = false;
    }

    return TRUE;
}

void CSkinnedDialog::skin(int nBitmap, HINSTANCE hinst)
{
 	HBITMAP hBitmap;
 
	if(hinst)
	{
		hBitmap = (HBITMAP)::LoadImage(hinst, MAKEINTRESOURCE(nBitmap), IMAGE_BITMAP, 0, 0, 0);
	}
	else
	{
		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nBitmap), IMAGE_BITMAP, 0, 0, 0);
	}

	SetBitmap(hBitmap);
 
    if(m_hBitmap)
	{
        SetWindowPos(0, 0, 0, m_dwWidth, m_dwHeight, SWP_NOMOVE | SWP_NOREPOSITION );
    }

	RedrawWindow();
}

BOOL CSkinnedDialog::OnEraseBkgnd(CDC* pDC) 
{
    BOOL bRetValue = CDialog::OnEraseBkgnd(pDC);

	if(!m_hBitmap)
	{
		return FALSE;
	}

	CRect   rect;
	GetClientRect(rect);

	CDC dc;
	dc.CreateCompatibleDC(pDC);

	HBITMAP    pbmpOldBmp = NULL;
	pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hBitmap);

	if (m_bStretchX && m_bStretchY)
		bRetValue = pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, m_dwWidth, m_dwHeight, SRCCOPY);
	else if (m_bStretchX)
		bRetValue = pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, m_dwWidth, rect.Height(), SRCCOPY);
	else if(m_bStretchY)
		bRetValue = pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, rect.Width(), m_dwHeight, SRCCOPY);
	else
		bRetValue = pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
	
	::SelectObject(dc.m_hDC, pbmpOldBmp);

    return bRetValue;
}

void CSkinnedDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	//if (m_hBitmap != NULL)
	//{
	//	Invalidate(FALSE);
	//}	
}

void CSkinnedDialog::OnPaint()
{
	if(!m_hBitmap)
	{
		return;
	}

	CPaintDC pDC(this);

	CRect   rect;
	GetClientRect(rect);

	CDC dc;
	if(dc.CreateCompatibleDC(&pDC) == FALSE) return;

	HBITMAP    pbmpOldBmp = NULL;
	pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hBitmap);

	if (m_bStretchX && m_bStretchY)
		pDC.StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, m_dwWidth, m_dwHeight, SRCCOPY);
	else if (m_bStretchX)
		pDC.StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, m_dwWidth, rect.Height(), SRCCOPY);
	else if(m_bStretchY)
		pDC.StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, rect.Width(), m_dwHeight, SRCCOPY);
	else
		pDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

	::SelectObject(dc.m_hDC, pbmpOldBmp);
}

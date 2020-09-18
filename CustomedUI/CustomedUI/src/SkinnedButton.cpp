#include "stdafx.h"
#include "SkinnedButton.h"

IMPLEMENT_DYNAMIC(CSkinnedButton, CButton)
CSkinnedButton::CSkinnedButton() : m_nTimer(0), m_bTracking(false), m_bImageLoaded(false), m_bIsHovering(false)
, m_bmpUp(0), m_bmpDown(0), m_bmpHover(0), m_bmpDisable(0)
{

}

CSkinnedButton::~CSkinnedButton()
{
	FreeResources();
}

void CSkinnedButton::FreeResources()
{
	if(m_bmpUp)
	{
        ::DeleteObject (m_bmpUp);
		m_bmpUp = NULL;
	}

	if(m_bmpDown)
	{
        ::DeleteObject (m_bmpDown);
		m_bmpDown = NULL;
	}

	if(m_bmpHover)
	{
        ::DeleteObject (m_bmpHover);
		m_bmpHover = NULL;
	}

	if(m_bmpDisable)
	{
        ::DeleteObject (m_bmpDisable);
		m_bmpDisable = NULL;
	}
}

DWORD CSkinnedButton::SetBitmap(CBitmap& aBitmap)
{  
	m_bImageLoaded = false;

    FreeResources();  

	CClientDC dc(this);

	CDC mDC;
	CDC mDC2;

	if(!mDC.CreateCompatibleDC(&dc))
	{
		return 0;
	}
	if(!mDC2.CreateCompatibleDC(&dc))
	{
		return 0;
	}

	CBitmap *pOldBitmap = mDC.SelectObject(&aBitmap);
	CBitmap *pOldBitmap2 = mDC2.SelectObject(&aBitmap);

	BITMAP bmp;
	aBitmap.GetBitmap(&bmp);

	m_dwWidth = (DWORD)(bmp.bmWidth/4);
	m_dwHeight = bmp.bmHeight;

	// Normal
	m_bmpUp = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpUp);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,0,0,SRCCOPY);

	// Key down
	m_bmpDown = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpDown);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,m_dwWidth,0,SRCCOPY);

	// Hovering
	m_bmpHover = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpHover);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,m_dwWidth*2,0,SRCCOPY);

	// Disable
	m_bmpDisable = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpDisable);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,m_dwWidth*3,0,SRCCOPY);

	SetWindowPos(0,0,0,m_dwWidth,m_dwHeight,SWP_NOREDRAW|SWP_NOMOVE);
    
    if (IsWindow(this->GetSafeHwnd()))
        Invalidate();
    
    m_bImageLoaded = true;

	mDC.SelectObject(pOldBitmap);
	mDC2.SelectObject(pOldBitmap2);

	return 1;
}

void CSkinnedButton::skin(int nBitmap, POINT aPoint)
{
    CBitmap aBitmap;

	if(aBitmap.LoadBitmap(nBitmap))
	{
        SetBitmap(aBitmap);
		m_bImageLoaded = true;
		aBitmap.DeleteObject();
	}
	else
	{
		m_bImageLoaded = false;
	}

	SetWindowPos(0,aPoint.x,aPoint.y,0,0,SWP_NOREDRAW|SWP_NOSIZE);

	RedrawWindow();
}

LRESULT CSkinnedButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bIsHovering=false;
	Invalidate(FALSE);

	return 0;
}

LRESULT CSkinnedButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bIsHovering=true;
	Invalidate(FALSE);

	return 0;
}

void CSkinnedButton::StartTrace()
{
	if ( m_nTimer == 0 )
		m_nTimer = SetTimer(1,100,0);
}

void CSkinnedButton::StopTrace()
{
	if ( m_nTimer ){
		if ( KillTimer(m_nTimer) ) 
			m_nTimer=0;
	}
}

BEGIN_MESSAGE_MAP(CSkinnedButton, CButton)
	ON_WM_SHOWWINDOW()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER() 
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CSkinnedButton::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CButton::OnShowWindow(bShow, nStatus);

	if ( !bShow && m_bTracking )
	{
		StopTrace();
		m_bTracking=false;
		OnMouseLeave(0,0);	
	}

}

void CSkinnedButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	if ( m_bTracking )
	{
		StopTrace();
		m_bTracking=false;
		OnMouseLeave(0,0);	
	}
}

void CSkinnedButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( !m_bTracking )
	{
		StartTrace();
		m_bTracking=true;
		OnMouseHover(0,0);
	}

	CButton::OnMouseMove(nFlags, point);
}
 

void CSkinnedButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if ( !m_bImageLoaded ) 
		return;

	CDC *pDC = new CDC();
	pDC->Attach(lpDrawItemStruct->hDC);

	HBITMAP hNewBitmap;

	if(lpDrawItemStruct->itemState & ODS_SELECTED) // if((lpDrawItemStruct->itemState & ODS_SELECTED) && m_bmpDown)
		hNewBitmap=m_bmpDown;
	else if (m_bIsHovering)
		hNewBitmap=m_bmpHover;
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		hNewBitmap=m_bmpDisable;
	else
		hNewBitmap=m_bmpUp;

    CDC dc;
    dc.CreateCompatibleDC(pDC);

    HBITMAP    pbmpOldBmp = NULL;
    pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, hNewBitmap);
    
    pDC->BitBlt(0, 0, m_dwWidth, m_dwHeight, &dc, 0, 0, SRCCOPY);

	delete pDC;
}

BOOL CSkinnedButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinnedButton::OnTimer(UINT_PTR nIDEvent)
{
	if ( !m_bTracking )
	{
		StopTrace();
		return;
	}
	CPoint ptMouse;
	CRect rect;
	GetWindowRect(&rect);
	GetCursorPos(&ptMouse);
	
	if ( !rect.PtInRect(ptMouse) )
	{
		StopTrace();	
		m_bTracking=false;
		OnMouseLeave(0,0);
	}
}

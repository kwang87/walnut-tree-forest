#include "stdafx.h"
#include "SkinnedStatic.h"

IMPLEMENT_DYNAMIC(CSkinnedStatic, CStatic)
CSkinnedStatic::CSkinnedStatic()
{
	m_pImage = NULL;
}

CSkinnedStatic::~CSkinnedStatic()
{
	FreeResources();
}

void CSkinnedStatic::FreeResources()
{
	if(m_bmpUp)
	{
		::DeleteObject (m_bmpUp);
		m_bmpUp = NULL;
	}
}

DWORD CSkinnedStatic::SetBitmap(CImage* pImage)
{  
	m_pImage = pImage;

	return 1;
}

DWORD CSkinnedStatic::SetBitmap(CBitmap& aBitmap)
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

	m_dwWidth = bmp.bmWidth;
	m_dwHeight = bmp.bmHeight;

	// Normal
	m_bmpUp = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpUp);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,0,0,SRCCOPY);

	SetWindowPos(0,0,0,m_dwWidth,m_dwHeight,SWP_NOREDRAW|SWP_NOMOVE);

	if (IsWindow(this->GetSafeHwnd()))
		Invalidate();

	m_bImageLoaded = true;

	mDC.SelectObject(pOldBitmap);
	mDC2.SelectObject(pOldBitmap2);

	return 1;
}

void CSkinnedStatic::skin(int nBitmap, POINT aPoint)
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

BEGIN_MESSAGE_MAP(CSkinnedStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CSkinnedStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinnedStatic::OnPaint()
{
	CPaintDC dc(this);

	if(m_pImage == NULL) 
	{
		//CStatic::OnPaint();

		if ( !m_bImageLoaded ) 
			return;
		
		HBITMAP    pbmpOldBmp = NULL;
		pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_bmpUp);

		dc.BitBlt(0, 0, m_dwWidth, m_dwHeight, &dc, 0, 0, SRCCOPY);
	}
	else
	{
		CRect rect;
		GetClientRect(rect);

		m_pImage->StretchBlt(dc.m_hDC, rect);
	}
}

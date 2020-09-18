#include "stdafx.h"
#include "SkinnedTextButton.h"

IMPLEMENT_DYNAMIC(CSkinnedTextButton, CSkinnedButton)

CSkinnedTextButton::CSkinnedTextButton() : m_nTextLocation(0), m_rcText(0,0,0,0)
{	
	
}

CSkinnedTextButton::~CSkinnedTextButton()
{
}

BEGIN_MESSAGE_MAP(CSkinnedTextButton, CSkinnedButton)
END_MESSAGE_MAP()

DWORD CSkinnedTextButton::SetBitmap(CBitmap& aBitmap)
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

	CBitmap *pOldBitmap  = mDC.SelectObject(&aBitmap);
	CBitmap *pOldBitmap2 = mDC2.SelectObject(&aBitmap);

	BITMAP bmp;
	aBitmap.GetBitmap(&bmp);

	m_dwWidth = (DWORD)(bmp.bmWidth/4);
	m_dwHeight = bmp.bmHeight;

	int      nBackMode     = mDC2.SetBkMode(TRANSPARENT);
	CFont*   pFont         = mDC2.SelectObject(&m_Font);
	COLORREF nOldTextColor = mDC2.SetTextColor(m_dwNormalColor);
	
	CRect rtRect(0, 0, m_dwWidth, m_dwHeight);
	CRect rtRectPress(0, 0, m_dwWidth, m_dwHeight);
	if (m_rcText.Width() && m_rcText.Height())
	{
		rtRect = m_rcText;
		rtRectPress = m_rcText;
	}

	UINT nFlag = DT_SINGLELINE | DT_CENTER | DT_VCENTER;

	if(m_nTextLocation == -1)
	{
		rtRect.MoveToXY(5,0);
		rtRectPress.MoveToXY(6,1);
		nFlag = DT_SINGLELINE | DT_LEFT | DT_VCENTER;
	}
	else if(m_nTextLocation == 1)
	{
		rtRect.MoveToXY(-5,0);
		rtRectPress.MoveToXY(-4,1);
		nFlag = DT_SINGLELINE | DT_RIGHT | DT_VCENTER;
	}
	else
	{
		rtRectPress.MoveToXY(1,1);
	}

	// Normal
	m_bmpUp = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpUp);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,0,0,SRCCOPY);
	mDC2.DrawText(m_strButtonText, &rtRect, nFlag);

	mDC2.SetTextColor(m_dwPressColor);

	// Key down
	m_bmpDown = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpDown);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,m_dwWidth,0,SRCCOPY);
	mDC2.DrawText(m_strButtonText, &rtRectPress, nFlag);

	mDC2.SetTextColor(m_dwHoverColor);

	// Hovering
	m_bmpHover = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpHover);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,m_dwWidth*2,0,SRCCOPY);
	mDC2.DrawText(m_strButtonText, &rtRect, nFlag);

	mDC2.SetTextColor(m_dwDisableColor);
	
	// Disable
	m_bmpDisable = ::CreateCompatibleBitmap(dc.m_hDC,m_dwWidth,m_dwHeight);
	mDC2.SelectObject(m_bmpDisable);
	mDC2.BitBlt(0,0,m_dwWidth,m_dwHeight,&mDC,m_dwWidth*3,0,SRCCOPY);
	mDC2.DrawText(m_strButtonText, &rtRect, nFlag);

	SetWindowPos(0,0,0,m_dwWidth,m_dwHeight,SWP_NOREDRAW|SWP_NOMOVE);
    
    if (IsWindow(this->GetSafeHwnd()))
        Invalidate();
    
    m_bImageLoaded = true;

	mDC.SelectObject(pOldBitmap);
	mDC2.SelectObject(pOldBitmap2);

	return 1;
}

void CSkinnedTextButton::skin(int nBitmap, POINT aPoint,
		const TCHAR* szButtonText, const LOGFONT* pFont
		, COLORREF dwNormalColor, COLORREF dwPressColor
		, COLORREF dwHoverColor, COLORREF dwDisableColor
		, int nTextLocation)
{
	m_nTextLocation = nTextLocation;
	
	if(m_Font.GetSafeHandle() != NULL)
	{
		m_Font.Detach();
		m_Font.DeleteObject();
	}
	m_Font.CreateFontIndirect(pFont);
	
	m_strButtonText  = szButtonText;

	m_dwNormalColor  = dwNormalColor;
	m_dwPressColor   = dwPressColor;
	m_dwHoverColor   = dwHoverColor;
	m_dwDisableColor = dwDisableColor;

	return CSkinnedButton::skin(nBitmap, aPoint);
}

void CSkinnedTextButton::skin(int nBitmap, POINT aPoint,
		const TCHAR* szButtonText, CFont* pFont
		, COLORREF dwNormalColor, COLORREF dwPressColor
		, COLORREF dwHoverColor, COLORREF dwDisableColor
		, int nTextLocation)
{
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	CSkinnedTextButton::skin(nBitmap, aPoint,
		szButtonText, (const LOGFONT*)&lf
		, dwNormalColor, dwPressColor
		, dwHoverColor, dwDisableColor
		, nTextLocation);
}

//void CSkinnedTextButton::SetTextRect(CRect rcText)
//{
//	m_rcText = rcText;
//}

void CSkinnedTextButton::SetTextRect(int x, int y, int w, int h)
{
	m_rcText = CRect(x,y,w,h);
}

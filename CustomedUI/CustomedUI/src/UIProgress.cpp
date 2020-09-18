#include "stdafx.h"
#include "UIProgress.h"
#include "memdc.h"

CUIProgress::SkinTable CUIProgress::m_constProSkin[] = 
{
	{0,  0,  2, 12},
	{2,  0,  1,	12},
	{3,  0,  2, 12},
	{5,  0,  2, 12},
	{7,  0,  1,	12},
	{8,  0,  2, 12},
};
 
IMPLEMENT_DYNAMIC(CUIProgress, CStatic)

CUIProgress::CUIProgress()
{
	m_ullTotal    = 1000000000;
	m_ullCurrent  = 0;
	m_prePer	  = 0;
	m_nWidth      = 0;
}

CUIProgress::~CUIProgress()
{
	m_bmBg.DeleteObject();
	m_ProSkin.DeleteObject();
}

BEGIN_MESSAGE_MAP(CUIProgress, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CUIProgress::SetTotalSize(ULONGLONG ullSize)
{
	m_ullTotal = ullSize;
	Invalidate();
}

void CUIProgress::SetCurrentSize(ULONGLONG ullSize)
{
	m_ullCurrent = ullSize;
	Invalidate();
}

void CUIProgress::AddCurrentSize(ULONGLONG ullSize)
{
	if(m_ullTotal < (m_ullCurrent + ullSize))
		return ;

	m_ullCurrent = m_ullCurrent + ullSize;
	Invalidate();
}

void CUIProgress::AddTotalSize(ULONGLONG ullAddSize)
{
	m_ullTotal += ullAddSize;
	Invalidate();
}

ULONGLONG CUIProgress::GetTotalSize()
{
	return m_ullTotal;
}

ULONGLONG CUIProgress::GetCurrentSize()
{
	return m_ullCurrent;
}

void CUIProgress::SetFullProgress() 
{
	m_ullCurrent = m_ullTotal;
	Invalidate();
}

UINT CUIProgress::ReCalWidth()
{
	UINT nWidth;
	
	if(m_ullTotal == 0)
		return 0;

	double nPer = ((double)m_ullCurrent / (double)m_ullTotal);
	nWidth = (UINT)(m_nWidth/*670*/ * nPer); 

	return nWidth;
}

BOOL CUIProgress::Create(UINT nBgID, UINT nBarID, int nX, int nY, CWnd* pParentWnd, UINT nID)
{
	m_bmBg.LoadBitmap(nBgID);
	BITMAP	bmpBG;
	m_bmBg.GetBitmap(&bmpBG);

	m_ProSkin.LoadBitmap(nBarID);
 
	m_nHeight = bmpBG.bmHeight;
	m_nWidth = bmpBG.bmWidth;

	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_EX_TRANSPARENT | SS_BITMAP | WS_CLIPCHILDREN;
	BOOL retVal = CStatic::Create(NULL, dwStyle, CRect(nX, nY, m_nWidth, m_nHeight), pParentWnd, nID); 
	this->SetBitmap(m_bmBg);

	return retVal;
}

void CUIProgress::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
    DMemDC   dc(&dcPaint);

	CRect		rcClient;
	GetClientRect(rcClient);
	
	//GDI Create
	CDC			memDC;
	HBITMAP*	hOldBitmap;
	memDC.CreateCompatibleDC(&dc);
	hOldBitmap = (HBITMAP *)memDC.SelectObject(m_ProSkin);
 
	//Progress Bkg Left
	dc.BitBlt(0,0,m_constProSkin[0].width, m_nHeight, &memDC, m_constProSkin[0].x, m_constProSkin[0].y, SRCCOPY);
	
	//Progress Bkg Span
	UINT nSpan = rcClient.Width() - m_constProSkin[0].width - m_constProSkin[2].width;
	dc.StretchBlt(m_constProSkin[0].width, 0,nSpan, m_nHeight, &memDC,
				  m_constProSkin[1].x, m_constProSkin[1].y, m_constProSkin[1].width, m_nHeight, SRCCOPY);

	//Progress Bkg right
	dc.BitBlt(rcClient.Width()-m_constProSkin[2].width, 0, m_constProSkin[2].width, m_nHeight, &memDC, m_constProSkin[2].x, m_constProSkin[2].y,SRCCOPY);

	//Progress Bar
	nSpan = ReCalWidth();
	if(nSpan == 2) 
	{
		dc.BitBlt(0,0,1,m_nHeight, &memDC, m_constProSkin[3].x, m_constProSkin[3].y, SRCCOPY);
		dc.BitBlt(1,0,1,m_nHeight, &memDC, m_constProSkin[5].x, m_constProSkin[5].y, SRCCOPY);
	}
	else if(nSpan == 3) 
	{
		dc.BitBlt(0,0,1,m_nHeight, &memDC, m_constProSkin[3].x, m_constProSkin[3].y, SRCCOPY);
		dc.BitBlt(1,0,1,m_nHeight, &memDC, m_constProSkin[4].x, m_constProSkin[4].y, SRCCOPY);
		dc.BitBlt(2,0,1,m_nHeight, &memDC, m_constProSkin[5].x, m_constProSkin[5].y, SRCCOPY);
	}
	else if(nSpan == 4) 
	{
		dc.BitBlt(0,0,m_constProSkin[3].width,m_nHeight, &memDC, m_constProSkin[3].x, m_constProSkin[3].y, SRCCOPY);
		dc.BitBlt(2,0,m_constProSkin[5].width,m_nHeight, &memDC, m_constProSkin[5].x, m_constProSkin[5].y, SRCCOPY);
	} 
	else if(nSpan > 4)
	{
		UINT proArea = nSpan - m_constProSkin[3].width - m_constProSkin[5].width;
		dc.BitBlt(0,0,m_constProSkin[3].width,m_nHeight, &memDC, m_constProSkin[3].x, m_constProSkin[3].y, SRCCOPY);
		dc.StretchBlt(m_constProSkin[3].width, 0, proArea, m_nHeight, &memDC, m_constProSkin[4].x, m_constProSkin[4].y, m_constProSkin[4].width, m_nHeight, SRCCOPY);
		dc.BitBlt(nSpan	- m_constProSkin[5].width,0,m_constProSkin[5].width,m_nHeight, &memDC, m_constProSkin[5].x, m_constProSkin[5].y, SRCCOPY);
	}
 
	//CFont *pOldFont = dc.SelectObject(&m_defFont);

	/*
	CString strMsg;
	//strMsg.Format(_T("%I64u Byte/ %I64u Byte(%2d)%%"),m_ullCurrent, m_ullTotal, nPer);
	strMsg = CreateProgressString();
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.DrawText(strMsg, CRect(0,0,rcClient.Width(),rcClient.Height()),DT_SINGLELINE | DT_VCENTER | DT_NOCLIP | DT_CENTER);
	dc.SelectObject(pOldFont);
	*/

	memDC.SelectObject(hOldBitmap);
	memDC.DeleteDC();
}

CString CUIProgress::CreateProgressString()
{
	CString strInfo;
	UINT nPer = (UINT)(((double)m_ullCurrent / (double)m_ullTotal) * 100);
	m_prePer = nPer;

	if(m_ullTotal < 1024)
		strInfo.Format(_T("%I64u Byte/ %I64u Byte (%02d%%)"),m_ullCurrent, m_ullTotal, nPer);
	else if(m_ullTotal < (1024)*(1024))
		strInfo.Format(_T("%.1lf KByte/ %.1lf KByte (%02d%%)"),(double)m_ullCurrent / (1024), (double)m_ullTotal / (1024), nPer);
	else if(m_ullTotal < (1024)*(1024)*(1024))
		strInfo.Format(_T("%.1lf MByte/ %.1lf MByte (%02d%%)"),(double)m_ullCurrent / (1024*1024), (double)m_ullTotal / (1024*1024), nPer);
	else 
		strInfo.Format(_T("%.1lf GByte/ %.1lf GByte (%02d%%)"),(double)m_ullCurrent / (1024*1024*1024), (double)m_ullTotal / (1024*1024*1024), nPer);

	return strInfo;
}

#pragma once

#include <atlimage.h>

class  AFX_CLASS_EXPORT CSkinnedStatic : public CStatic
{
	DECLARE_DYNAMIC(CSkinnedStatic)

public:
	CSkinnedStatic();
	virtual ~CSkinnedStatic();

	void FreeResources();
	DWORD SetBitmap(CImage* pImage);
	
	virtual DWORD SetBitmap(CBitmap& aBitmap);

protected:
	HBITMAP m_bmpUp;
	CImage* m_pImage;

	DWORD   m_dwWidth;
	DWORD   m_dwHeight;

	UINT_PTR m_nTimer;
	bool m_bTracking;
	bool m_bImageLoaded;
	bool m_bIsHovering;

	DECLARE_MESSAGE_MAP()

public:
	virtual void skin(int nBitmap, POINT aPoint);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	COLORREF m_rgb;
};

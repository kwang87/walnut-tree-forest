#pragma once

class AFX_CLASS_EXPORT DMemDC : public CDC
{
private:
	CBitmap		m_bitmap;		// Offscreen bitmap
	CBitmap*	m_oldBitmap;	// bitmap originally found in DMemDC
	CDC*		m_pDC;			// Saves CDC passed in constructor
	CRect		m_rect;			// Rectangle of drawing area.
	BOOL		m_bMemDC;		// TRUE if CDC really is a Memory DC.
public:

	DMemDC(CDC* pDC, const CRect* pRect = NULL);

	virtual ~DMemDC();
 
	DMemDC* operator->()
	{
		return this;
	}
 
	operator DMemDC*()
	{
		return this;
	}
};

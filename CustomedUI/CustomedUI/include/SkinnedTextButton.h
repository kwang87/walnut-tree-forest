#pragma once

#include "SkinnedButton.h"

class AFX_CLASS_EXPORT CSkinnedTextButton : public CSkinnedButton
{
	DECLARE_DYNAMIC(CSkinnedTextButton)

public:
	CSkinnedTextButton();
	virtual ~CSkinnedTextButton();

	virtual void skin(int nBitmap, POINT aPoint,
		const TCHAR* szButtonText, const LOGFONT* pFont, COLORREF dwNormalColor, COLORREF dwPressColor, COLORREF dwHoverColor, COLORREF dwDisableColor, int nTextLocation = 0);

	virtual void skin(int nBitmap, POINT aPoint,
		const TCHAR* szButtonText, CFont* pFont, COLORREF dwNormalColor, COLORREF dwPressColor, COLORREF dwHoverColor, COLORREF dwDisableColor, int nTextLocation = 0);
	
	//void	SetTextRect(CRect rcText);
	void	SetTextRect(int x, int y, int w, int h);

protected:
	DECLARE_MESSAGE_MAP()
	virtual  DWORD SetBitmap(CBitmap& aBitmap);

protected:
	int      m_nTextLocation;

	CString  m_strButtonText;
	CFont    m_Font;

	COLORREF m_dwNormalColor;
	COLORREF m_dwPressColor;
	COLORREF m_dwHoverColor;
	COLORREF m_dwDisableColor;

private:
	CRect	m_rcText;
};


#pragma once

class AFX_CLASS_EXPORT CXColorStatic : public CStatic
{
// Construction
public:
	CXColorStatic();
	virtual ~CXColorStatic();

// Attributes
public:
	void SetBackgroundColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetTextColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetBold(BOOL bFlag, BOOL bRedraw = TRUE);
	void SetFont(LPCTSTR lpszFaceName, int nPointSize, BOOL bRedraw = TRUE);
	void SetFont(LOGFONT * pLogFont, BOOL bRedraw = TRUE);
	void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
	void SetIcon(HICON hIcon, BOOL bRedraw = TRUE);
	void SetMargins(int x, int y) { m_nXMargin = x; m_nYMargin = y; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorStatic)
protected:
    virtual void PreSubclassWindow();
	int GetFontPointSize(int nHeight);
	int GetFontHeight(int nPointSize);

	CFont		m_font;
	COLORREF	m_rgbText;
	COLORREF	m_rgbBackground;
	CBrush *	m_pBrush;
	BOOL		m_bBold;
	int			m_nXMargin, m_nYMargin;
	HICON		m_hIcon;

	// Generated message map functions
protected:
	//{{AFX_MSG(CXColorStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

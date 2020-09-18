#pragma once

#include <afxtempl.h>

class  AFX_CLASS_EXPORT CTransListBox : public CListBox
{
protected:
	virtual void PreSubclassWindow();

	int            m_ItemHeight;
	COLORREF       m_Color;
	COLORREF       m_SelColor;
	COLORREF       m_ShadowColor;
	CFont          m_Font;
	CBitmap        m_Bmp;
	BOOL           m_HasBackGround;
	int            m_ShadowOffset;
	BOOL           m_Shadow;
	long           m_PointSize;
	BOOL           m_bReadOnly;

	virtual CFont *GetFont();
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void DrawItem(CDC &Dc,int Index,CRect &Rect,BOOL Selected);

	//{{AFX_MSG(CTransListBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnLbnSelchange();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CTransListBox();
	virtual ~CTransListBox();
	void SetFont(int nPointSize, CString FaceName,COLORREF Color,COLORREF SelColor,BOOL Shadow=TRUE,int SOffset = 2,COLORREF ShadowColor = RGB(0,0,0));
	void SetColor(COLORREF Color,COLORREF SelColor, BOOL Shadow=TRUE, int SOffset = 2, COLORREF ShadowColor = RGB(0,0,0));
	void SetReadOnly(BOOL bTrue = TRUE);

	BOOL SetTopIndex(int Index);
	int ScrollUp(int Lines=1);
	int ScrollDown(int Lines=1);
	int AddString(CString Text,DWORD ItemData = 0,BOOL Enabled = TRUE);
	int InsertString(int Index,CString Text, DWORD ItemData = 0,BOOL Enabled = TRUE);
	virtual void ResetContent(BOOL bInvalidate = TRUE);

	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

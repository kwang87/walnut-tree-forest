#pragma once

class  AFX_CLASS_EXPORT CSkinnedButton : public CButton
{
	DECLARE_DYNAMIC(CSkinnedButton)

public:
	CSkinnedButton();
	virtual ~CSkinnedButton();

protected:
	HBITMAP m_bmpUp;
	HBITMAP m_bmpDown;
	HBITMAP m_bmpHover;
	HBITMAP m_bmpDisable;

	DWORD   m_dwWidth;
	DWORD   m_dwHeight;

	UINT_PTR m_nTimer;
	bool m_bTracking;
	bool m_bImageLoaded;
	bool m_bIsHovering;

	void StopTrace();
	void StartTrace();

	void FreeResources();
	virtual DWORD SetBitmap(CBitmap& aBitmap);

	LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual void skin(int nBitmap, POINT aPoint);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

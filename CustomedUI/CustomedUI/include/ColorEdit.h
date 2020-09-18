#pragma once

class AFX_CLASS_EXPORT CColorEdit : public CEdit
{
	DECLARE_DYNAMIC(CColorEdit)

protected:
	CBrush     m_brushBG;
	COLORREF   m_colBG;
	COLORREF   m_colBGForReadOnly;
	COLORREF   m_colText;
	BOOL       m_bReadOnly;

public:
	CColorEdit();
	CColorEdit(COLORREF colBackGround, COLORREF colBGForReadOnly, COLORREF colText);
	virtual ~CColorEdit();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL SetReadOnly(BOOL bReadOnly);
	void SetTextColor(COLORREF colText);
	void SetBGColor(COLORREF colBackGround, COLORREF colBGForReadOnly);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};



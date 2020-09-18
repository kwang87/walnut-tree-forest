#pragma once

class AFX_CLASS_EXPORT CUIProgress : public CStatic
{
	DECLARE_DYNAMIC(CUIProgress)

public:
	typedef struct
	{
		int x, y;
		int width, height;
	} SkinTable;

protected:
	ULONGLONG		m_ullTotal;
	ULONGLONG		m_ullCurrent;
	UINT			m_prePer;

	CBitmap			m_bmBg;
	CBitmap			m_ProSkin;
	CFont			m_defFont;

	int m_nWidth;
	int m_nHeight;

public:
	static SkinTable m_constProSkin[];

	CString			CreateProgressString();

	void			SetTotalSize(ULONGLONG ullSize);
	void			SetCurrentSize(ULONGLONG ullSize);
	void			AddTotalSize(ULONGLONG ullAddSize);
	void			AddCurrentSize(ULONGLONG ullSize);

	ULONGLONG		GetTotalSize();
	ULONGLONG		GetCurrentSize();
	void			SetFullProgress();

	CUIProgress();
	virtual ~CUIProgress();

protected:
	UINT    ReCalWidth();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(UINT nBgID, UINT nBarID, int nX, int nY, CWnd* pParentWnd, UINT nID = 0xffff);
	afx_msg void OnPaint();
};



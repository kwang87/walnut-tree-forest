#pragma once
 
class AFX_CLASS_EXPORT CVEdit : public CEdit
{
public:
	CVEdit(CFont *pFont = NULL);

protected:
	CFont	*m_pFont;
	CBrush  m_Brush;
	bool    m_bOutLine;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVEdit)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVEdit)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();

public:
	BOOL SetVMargins(INT nTop = 0, INT nBottom = 0);
	void SetText(CString sText) { m_sText = sText; }

protected:
	INT m_nTopMargin;
	INT m_nBottomMargin;

private:
	CString m_sText;
};

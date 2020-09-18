#pragma once

class AFX_CLASS_EXPORT CInPlaceEdit : public CEdit
{
	DECLARE_DYNAMIC(CInPlaceEdit)

public:
	CInPlaceEdit(int iItem, int iSubItem, CString sInitText);
	virtual ~CInPlaceEdit();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CInPlaceEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

private:
	int m_iItem;
	int m_iSubItem;
	CString m_sInitText;
	BOOL    m_bESC;	 	// To indicate whether ESC key was pressed

	int m_nType;

};



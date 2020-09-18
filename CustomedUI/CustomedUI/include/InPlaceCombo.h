#pragma once

class AFX_CLASS_EXPORT CInPlaceCombo : public CComboBox
{
	DECLARE_DYNAMIC(CInPlaceCombo)

private:
	int m_iItem;
	int m_iSubItem;
	int m_iCurSel;
	BOOL m_bESC;

	CString     m_strWindowText;
	CStringList m_DropDownList;

protected:
	//{{AFX_MSG(CInPlaceCombo)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnCloseup();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CInPlaceCombo(int iItem, int iSubItem, CStringList* pDropDownList, int iCurSel = -1);
	virtual ~CInPlaceCombo();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#pragma once
 
class AFX_CLASS_EXPORT CEditListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditListCtrl)
 
protected:
	int HitTestEx(CPoint& point, int* col) const;
	CEdit* EditSubLabel(int nItem, int nCol);
	CComboBox* ComboSubLabel(int nItem, int nCol);

	virtual bool checkIfEditableColumn(int nColumn, int nRow);
	virtual bool checkIfComboColumn(int nColumn);
	virtual int  getComboValue(CString szCurrentSel, CStringList& sDropDownList);
	virtual bool checkValue(LV_ITEM *plvItem);

	DECLARE_MESSAGE_MAP()

public:
	CEditListCtrl();
	virtual ~CEditListCtrl();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
};

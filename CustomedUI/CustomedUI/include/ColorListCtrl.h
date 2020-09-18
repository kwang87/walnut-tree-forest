#pragma once

#include <vector>

using namespace std;
 
class AFX_CLASS_EXPORT CColorListCtrl : public CListCtrl
{
public:
	CColorListCtrl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CColorListCtrl)
 
	virtual ~CColorListCtrl();

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
 
	//{{AFX_MSG(CColorListCtrl)
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void CustDraw( NMHDR * pNotifyStruct, LRESULT * result );
	typedef struct {
		COLORREF cf;
		long id;
		long subId;
	}ColorStruct;
	enum RecType {ROW=0,COL,CELL};
	vector<ColorStruct> RowColors;
	vector<ColorStruct> ColColors;
	vector<ColorStruct> CellColors;
	COLORREF Gx;
	COLORREF Gy;
	COLORREF Rx;
	COLORREF Ry;

	bool GridMode;
	bool AltRowMode;
	bool MaskMode;
	long findDetails(long id,RecType rt = ROW,long subId=-1);
	COLORREF InvertColor(COLORREF cf);

public: 
	void SetRowColor(COLORREF cf, long row);
	void SetColColor(COLORREF cf, long col);
	void SetCellColor(COLORREF cf, long row, long col);
	void ClrRowColor(long row);
	void ClrColColor(long col);
	void ClrCellColor(long row,long col);
	void SetGridMode(bool TurnOn,COLORREF x=0,COLORREF y=0);
	void SetTextMask(bool TurnOn);
	void SetAltRowColors(bool TurnOn,COLORREF x=0,COLORREF y=0);
	void ResetColors(void);
};

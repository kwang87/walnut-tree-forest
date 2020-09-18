#include "stdafx.h"
#include "EditListCtrl.h"
#include "InPlaceEdit.h"
#include "InPlaceCombo.h"

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)

CEditListCtrl::CEditListCtrl()
{
}

CEditListCtrl::~CEditListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CEditListCtrl::OnLvnEndLabelEdit)
END_MESSAGE_MAP()

void CEditListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int index;
	CListCtrl::OnLButtonDown(nFlags, point);

	int column;
	int row = HitTest(point, NULL);
	if( ( index = HitTestEx( point, &column )) != -1 )
	{
		if(checkIfEditableColumn(column,row))
		{
			UINT flag = LVIS_FOCUSED;
			if( (GetItemState( index, flag ) & flag) == flag /*&& column > 0*/)//0컬럼도 가능
			{
				// Add check for LVS_EDITLABELS
				//if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
				EditSubLabel(index, column);
			}
			else
			{
				SetItemState( index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		else if(checkIfComboColumn(column))
		{
			UINT flag = LVIS_FOCUSED;
			if( (GetItemState( index, flag ) & flag) == flag /*&& column > 0*/)//0컬럼도 가능
			{
				// Add check for LVS_EDITLABELS
				//if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
				ComboSubLabel(index, column);
			}
			else
			{
				SetItemState( index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}
}

bool CEditListCtrl::checkIfEditableColumn(int nColumn, int nRow)
{
	return true;
}

bool CEditListCtrl::checkIfComboColumn(int nColumn)
{
	return true;
}

int CEditListCtrl::getComboValue(CString szCurrentSel, CStringList& sDropDownList)
{
	return -1;
}

void CEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

bool CEditListCtrl::checkValue(LV_ITEM *plvItem)
{
	// plvItem->iItem(Row), plvItem->iSubItem(Column), plvItem->pszText
	return true;
}

void CEditListCtrl::OnLvnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	//*pResult = 0;
	//LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
	LV_ITEM	*plvItem = &pDispInfo->item;

	if(checkValue(plvItem))
	{
		if(plvItem->pszText != NULL)
		{
			SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
		}
	}
 
	*pResult = FALSE;
}

// HitTestEx - Determine the row index and column index for a point
// Returns	 - the row index or -1 if point is not over a row
// point	 - point to be tested.
// col		 - to hold the column index
int CEditListCtrl::HitTestEx(CPoint &point, int *col) const
{
	int colnum = 0;
	int row = HitTest( point, NULL );

	if(col){
		*col = 0;
	}

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return row;

	// Get the top and bottom row visible
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )
		bottom = GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left
					&& point.x <= (rect.left + colwidth ) )
				{
					if( col ) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

// EditSubLabel		- Start edit of a sub item label
// Returns		- Temporary pointer to the new edit control
// nItem		- The row index of the item to edit
// nCol			- The column of the sub item.
CEdit* CEditListCtrl::EditSubLabel(int nItem, int nCol)
{
	// The returned pointer should not be saved

	// Make sure that the item is visible
	if( !EnsureVisible( nItem, TRUE ) ) return NULL;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 5 )
		return NULL;

	// Get the column offset
	int offset = 0;
	for( int i = 0; i < nCol; i++ )
		offset += GetColumnWidth( i );

	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}

	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn( nCol, &lvcol );
	DWORD dwStyle ;
	if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;

	//rect.left += offset+4;
	//rect.right = rect.left + GetColumnWidth( nCol ) - 3 ;

 	rect.left += offset;
	rect.right = rect.left + GetColumnWidth( nCol );

	if( rect.right > rcClient.right) rect.right = rcClient.right;

	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL;

	CEdit* pEdit = new CInPlaceEdit(nItem, nCol, GetItemText( nItem, nCol));
	pEdit->Create(dwStyle, rect, this, 0);

	return pEdit;
}

CComboBox* CEditListCtrl::ComboSubLabel(int nItem, int nCol)
{
	// Make sure that the item is visible
	if( !EnsureVisible( nItem, TRUE ) ) return NULL;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 5 )
		return NULL;

	// Get the column offset
	int offset = 0;
	for( int i = 0; i < nCol; i++ )
		offset += GetColumnWidth( i );

	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}
 
	DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | 
							CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
 
 	rect.left += offset;
	rect.right = rect.left + GetColumnWidth( nCol );

	if( rect.right > rcClient.right) rect.right = rcClient.right;
 
	CStringList sDropDownList;
	int nSel = getComboValue(GetItemText(nItem, nCol), sDropDownList);

	CComboBox *pCComboBox = new CInPlaceCombo(nItem, nCol, &sDropDownList, nSel);
	pCComboBox->Create(dwStyle, rect, this, 0);

	return pCComboBox;
}

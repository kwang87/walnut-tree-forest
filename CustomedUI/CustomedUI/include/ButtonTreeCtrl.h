#pragma once
#include <afxcmn.h>

#define UM_CLICKED_ICON ( WM_USER + 1001 )
#define UM_MOUSEOVER_ICON ( WM_USER + 1002 )

class AFX_CLASS_EXPORT CButtonTreeCtrl :	public CTreeCtrl
{
	DECLARE_DYNAMIC(CButtonTreeCtrl)
	
public:
	HTREEITEM GetNextItemEx(HTREEITEM hItem);
	void ResetPrevMouseoverInfo();

protected:
	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

private:
	void OnClickedIcon(HTREEITEM hItem);

private:
	bool m_IconMouseOver = false;
	HTREEITEM m_prevMouseOverTreeItem = nullptr;
};


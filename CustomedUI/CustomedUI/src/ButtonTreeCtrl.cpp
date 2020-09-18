#include "ButtonTreeCtrl.h"

IMPLEMENT_DYNAMIC(CButtonTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CButtonTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


void CButtonTreeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	//CTreeCtrl::OnKillFocus(pNewWnd);
}

void CButtonTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM	hItem = NULL;
	UINT uFlag = 0;
	hItem = HitTest(point, &uFlag);
	if (TVHT_ONITEMICON != uFlag)
	{
		if (m_IconMouseOver == true)
		{
			m_IconMouseOver = false;
			this->GetParent()->PostMessageW(UM_MOUSEOVER_ICON, 0, (LPARAM)m_prevMouseOverTreeItem);
		}
	}
	else
	{
		if (hItem != m_prevMouseOverTreeItem)
		{
			m_IconMouseOver = false;
			this->GetParent()->PostMessageW(UM_MOUSEOVER_ICON, 0, (LPARAM)m_prevMouseOverTreeItem);
		}

		if (m_IconMouseOver == false)
		{
			m_IconMouseOver = true;
			this->GetParent()->PostMessageW(UM_MOUSEOVER_ICON, 1, (LPARAM)hItem);
			m_prevMouseOverTreeItem = hItem;
		}
	}
	CTreeCtrl::OnMouseHover(nFlags, point);
}

void CButtonTreeCtrl::ResetPrevMouseoverInfo()
{
	m_prevMouseOverTreeItem = nullptr;
	m_IconMouseOver = false;
}

void CButtonTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM	hItem = NULL;
	UINT uFlag = 0;
	hItem = HitTest(point, &uFlag);
	if (TVHT_ONITEMICON != uFlag)
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
	else
	{
		ResetPrevMouseoverInfo();
		this->SelectItem(hItem);
		OnClickedIcon(hItem);
	}

	SetFocus();
}

void CButtonTreeCtrl::OnClickedIcon(HTREEITEM hItem)
{
	this->GetParent()->PostMessageW(UM_CLICKED_ICON, 0, (LPARAM)hItem);
}

HTREEITEM CButtonTreeCtrl::GetNextItemEx(HTREEITEM hItem)
{
	if (ItemHasChildren(hItem))
	{
		return GetChildItem(hItem);
	}

	HTREEITEM tmp;
	if (( tmp = GetNextSiblingItem(hItem) ) != NULL)
	{
		return tmp;
	}

	HTREEITEM p = hItem;
	while (( p = GetParentItem(p) ) != NULL)
	{
		if (( tmp = GetNextSiblingItem(p) ) != NULL)
		{
			return tmp;
		}
	}
	return NULL;
}
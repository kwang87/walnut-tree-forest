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

HTREEITEM CButtonTreeCtrl::FindTreeData(HTREEITEM hItem, DWORD dwData)
{
	HTREEITEM hitemFind, hItemChile, hItemSibling;
	hitemFind = hItemChile = hItemSibling = NULL;


	if ( GetItemData(hItem) == dwData)
	{
		hitemFind = hItem;
	}
	else
	{
		// 자식 노드를 찾는다.
		hItemChile = GetChildItem(hItem);
		if (hItemChile)
		{
			hitemFind = FindTreeData(hItemChile, dwData);

		}

		// 형제노드를 찾는다.
		hItemSibling = GetNextSiblingItem(hItem);
		if (hitemFind == NULL && hItemSibling)
		{
			hitemFind = FindTreeData(hItemSibling, dwData);
		}
	}

	return hitemFind;
}

HTREEITEM CButtonTreeCtrl::FindTreeText(HTREEITEM hItem, CString text, UINT uFlag/* = 0*/)
{
	HTREEITEM hitemFind, hItemChile, hItemSibling;
	hitemFind = hItemChile = hItemSibling = NULL;

	CString strNodeName = GetItemText(hItem);
	bool bFind = false;
	if (uFlag == 0)
	{
		if (strNodeName.Compare(text) == 0)
		{
			hitemFind = hItem;
			bFind = true;
		}
	}
	else
	{
		if (strNodeName.Find(text) != -1)
		{
			hitemFind = hItem;
			bFind = true;
		}
	}


	if (bFind == false)
	{
		// 자식 노드를 찾는다.
		hItemChile = GetChildItem(hItem);
		if (hItemChile)
		{
			hitemFind = FindTreeText(hItemChile, text);

		}

		// 형제노드를 찾는다.
		hItemSibling = GetNextSiblingItem(hItem);
		if (hitemFind == NULL && hItemSibling)
		{
			hitemFind = FindTreeText(hItemSibling, text);
		}
	}

	return hitemFind;
}

HTREEITEM CButtonTreeCtrl::MoveTreeItem(HTREEITEM hSrcItem, HTREEITEM hDestItem)
{
	// 이동할 아이템의 정보를 알아내자.
	TVITEM    TV;
	TCHAR    str[256];
	ZeroMemory(str, sizeof(str));
	TV.hItem = hSrcItem;
	TV.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TV.pszText = str;
	TV.cchTextMax = sizeof(str);
	GetItem(&TV);

	DWORD dwData = GetItemData(hSrcItem);

	// 아이템을 추가 하자.
	TVINSERTSTRUCT  TI;
	TI.hParent = hDestItem;
	TI.hInsertAfter = TVI_LAST;
	TI.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.item.iImage = TV.iImage;
	TI.item.iSelectedImage = TV.iSelectedImage;
	TI.item.pszText = TV.pszText;
	HTREEITEM hItem = InsertItem(&TI);
	SetItemData(hItem, dwData);

	// 현재 아이템에 자식 아이템이 있다면
	HTREEITEM hChildItem = GetChildItem(hSrcItem);
	if (hChildItem)
	{
		// 자식 아이템이 있다면 같이 이동하자.
		MoveChildTreeItem(hChildItem, hItem);
	}

	// 확장 여부를 알아서 똑같이 하자.
	TVITEM  item;
	item.mask = TVIF_HANDLE;
	item.hItem = hSrcItem;
	GetItem(&item);
	if (item.state & TVIS_EXPANDED)
	{
		Expand(hItem, TVE_EXPAND);
	}

	// 기존 아이템을 제거한다.
	DeleteItem(hSrcItem);

	return hItem;
}

// 현재 트리의 모든 아이템 데이터 이동
BOOL CButtonTreeCtrl::MoveChildTreeItem(HTREEITEM hChildItem, HTREEITEM hDestItem)
{
	HTREEITEM hSrcItem = hChildItem;
	while (hSrcItem)
	{
		// 이동할 아이템의 정보를 알아내자.
		TVITEM    TV;
		TCHAR    str[256];
		ZeroMemory(str, sizeof(str));
		TV.hItem = hSrcItem;
		TV.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TV.pszText = str;
		TV.cchTextMax = sizeof(str);
		GetItem(&TV);

		DWORD dwData = GetItemData(hSrcItem);
		// 아이템을 추가 하자.
		TVINSERTSTRUCT  TI;
		TI.hParent = hDestItem;
		TI.hInsertAfter = TVI_LAST;
		TI.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage = TV.iImage;
		TI.item.iSelectedImage = TV.iSelectedImage;
		TI.item.pszText = TV.pszText;
		HTREEITEM hItem = InsertItem(&TI);
		SetItemData(hItem, dwData);

		// 현재 아이템에 자식 아이템이 있다면
		HTREEITEM hChildItem = GetChildItem(hSrcItem);
		// pTree->GetNextItem(hSrcItem, TVGN_CHILD;
		if (hChildItem)
		{
			MoveChildTreeItem(hChildItem, hItem);
		}

		// 확장 여부를 알아서 똑같이 하자.
		TVITEM  item;
		item.mask = TVIF_HANDLE;
		item.hItem = hSrcItem;
		GetItem(&item);
		if (item.state & TVIS_EXPANDED)
		{
			Expand(hItem, TVE_EXPAND);
		}

		// 다음 아이템을 알아보자.
		hSrcItem = GetNextItem(hSrcItem, TVGN_NEXT);
	}

	// 기존 아이템을 제거한다.
	DeleteItem(hChildItem);
	return TRUE;
}

DWORD_PTR CButtonTreeCtrl::GetTopParentItemData(HTREEITEM hSrcItem)
{
	DWORD_PTR dwData = NULL;

	if (hSrcItem != nullptr )
	{
		dwData = GetItemData(hSrcItem);
		HTREEITEM hItem = GetNextItem(hSrcItem, TVGN_PARENT);
		while (true)
		{
			if (hItem == nullptr)
			{
				break;
			}
			dwData = GetItemData(hItem);
			hItem = GetNextItem(hItem, TVGN_PARENT);
		}
	}

	return dwData;
}
void CButtonTreeCtrl::ExpandAllGroupTree()
{
	HTREEITEM hItem;
	hItem = GetRootItem();
	while (hItem != nullptr)
	{
		Expand(hItem, TVE_EXPAND);
		hItem = GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}
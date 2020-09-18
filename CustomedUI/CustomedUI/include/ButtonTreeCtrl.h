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

	// 파라미터 dwData와 일치하는 itemdata 를 갖는 트리아이템 찾기. 트리, 기준노드, 찾는아이템 [9/15/2020 Kwang]
	HTREEITEM FindTreeData(HTREEITEM hItem, DWORD dwData);
	// 파라미터 text와 일치하는 node name을 갖는 HTREEITEM찾기. 트리, 기준노드, 찾는아이템 [9/15/2020 Kwang]
	HTREEITEM FindTreeText(HTREEITEM hItem, CString text, UINT uFlag = 0);

	// 현재 트리의 아이템 데이터 이동
	HTREEITEM MoveTreeItem(HTREEITEM hSrcItem, HTREEITEM hDestItem);
	// 현재 트리의 모든 아이템 데이터 이동
	BOOL MoveChildTreeItem(HTREEITEM hChildItem, HTREEITEM hDestItem);

	// 최상위 노드 아이템데이터 찾기 [9/16/2020 Kwang]
	DWORD_PTR GetTopParentItemData(HTREEITEM hSrcItem);

	// 모든 노드 펼침 [9/18/2020 Kwang]
	void ExpandAllGroupTree();
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


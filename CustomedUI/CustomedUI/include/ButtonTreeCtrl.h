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

	// �Ķ���� dwData�� ��ġ�ϴ� itemdata �� ���� Ʈ�������� ã��. Ʈ��, ���س��, ã�¾����� [9/15/2020 Kwang]
	HTREEITEM FindTreeData(HTREEITEM hItem, DWORD dwData);
	// �Ķ���� text�� ��ġ�ϴ� node name�� ���� HTREEITEMã��. Ʈ��, ���س��, ã�¾����� [9/15/2020 Kwang]
	HTREEITEM FindTreeText(HTREEITEM hItem, CString text, UINT uFlag = 0);

	// ���� Ʈ���� ������ ������ �̵�
	HTREEITEM MoveTreeItem(HTREEITEM hSrcItem, HTREEITEM hDestItem);
	// ���� Ʈ���� ��� ������ ������ �̵�
	BOOL MoveChildTreeItem(HTREEITEM hChildItem, HTREEITEM hDestItem);

	// �ֻ��� ��� �����۵����� ã�� [9/16/2020 Kwang]
	DWORD_PTR GetTopParentItemData(HTREEITEM hSrcItem);

	// ��� ��� ��ħ [9/18/2020 Kwang]
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


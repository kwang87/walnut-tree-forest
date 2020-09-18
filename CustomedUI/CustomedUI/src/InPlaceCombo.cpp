#include "stdafx.h"
#include "InPlaceCombo.h"

IMPLEMENT_DYNAMIC(CInPlaceCombo, CComboBox)

CInPlaceCombo::CInPlaceCombo(int iItem, int iSubItem, CStringList* pDropDownList, int iCurSel)
: m_iItem(iItem), m_iSubItem(iSubItem), m_iCurSel(iCurSel), m_bESC(FALSE), m_strWindowText(_T(""))
{
	m_DropDownList.AddTail(pDropDownList);
}

CInPlaceCombo::~CInPlaceCombo()
{
	m_DropDownList.RemoveAll(); 
}

BEGIN_MESSAGE_MAP(CInPlaceCombo, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceCombo)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()
 
/////////////////////////////////////////////////////////////////////////////
// CInPlaceCombo message handlers

BOOL CInPlaceCombo::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| GetKeyState(VK_CONTROL)
			)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}

	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CInPlaceCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// Get the current selection text
	CString str;
	GetWindowText(str);

	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iItem;
	dispinfo.item.iSubItem = m_iSubItem;
	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strWindowText) : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = m_bESC ? m_strWindowText.GetLength() : str.GetLength();
	
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);

	// Close the control
	PostMessage(WM_CLOSE);
}

void CInPlaceCombo::OnNcDestroy()
{
	CComboBox::OnNcDestroy();
	delete this;
}

void CInPlaceCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// If the key is "Esc" set focus back to the list control
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_ESCAPE)
		{
			m_bESC = TRUE;
		}

		GetParent()->SetFocus();
		return;
	}
	
	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CInPlaceCombo::OnCloseup() 
{
	// Set the focus to the parent list control
	GetParent()->SetFocus();
}

int CInPlaceCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);
	
	SetFocus();

	ResetContent(); 
	for(POSITION Pos_ = m_DropDownList.GetHeadPosition(); Pos_ != NULL;){
		AddString((LPCTSTR)(m_DropDownList.GetNext(Pos_)));
	}

	if(-1 != m_iCurSel){
		GetLBText(m_iCurSel, m_strWindowText);
	}
 
	SetCurSel(m_iCurSel);

	return 0;
}

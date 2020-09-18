#include "stdafx.h"
#include "ColorEdit.h"

IMPLEMENT_DYNAMIC(CColorEdit, CEdit)

CColorEdit::CColorEdit() : m_colText(RGB(0,0,0)), m_colBG(RGB(255,255,255)), m_colBGForReadOnly(RGB(212,208,200)), m_bReadOnly(FALSE)
{
	m_brushBG.CreateSolidBrush(m_colBG);
}

CColorEdit::CColorEdit(COLORREF colBackGround, COLORREF colBGForReadOnly, COLORREF colText) 
: m_colBG(colBackGround), m_colBGForReadOnly(colBGForReadOnly), m_colText(colText), m_bReadOnly(FALSE)
{
	m_brushBG.CreateSolidBrush(m_colBG);
}

CColorEdit::~CColorEdit()
{
	m_brushBG.DeleteObject();
}

BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
 
void CColorEdit::SetTextColor(COLORREF colText)
{
	m_colText = colText;
	RedrawWindow();
}
 
void CColorEdit::SetBGColor(COLORREF colBackGround, COLORREF colBGForReadOnly)
{
	m_colBG = colBackGround;
	m_colBGForReadOnly = colBGForReadOnly;
 
	m_brushBG.DeleteObject();

	if(m_bReadOnly){
		m_brushBG.CreateSolidBrush(m_colBGForReadOnly);
	}
	else{
		m_brushBG.CreateSolidBrush(m_colBG);
	}
 	RedrawWindow( );
}

BOOL CColorEdit::SetReadOnly(BOOL bReadOnly)
{
	if(m_bReadOnly == bReadOnly){
		return TRUE;
	}

	BOOL bResult = CEdit::SetReadOnly(bReadOnly);
	m_bReadOnly = bReadOnly;

	m_brushBG.DeleteObject();

	if(m_bReadOnly){
		m_brushBG.CreateSolidBrush(m_colBGForReadOnly);
	}
	else{
		m_brushBG.CreateSolidBrush(m_colBG);
	}
 	RedrawWindow();

	return bResult;
}

HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if(m_bReadOnly){
		pDC->SetBkColor(m_colBGForReadOnly);
	}
	else{
		pDC->SetBkColor(m_colBG);
	}

	pDC->SetTextColor(m_colText);

	nCtlColor = CTLCOLOR_EDIT;
 
	return m_brushBG;
}

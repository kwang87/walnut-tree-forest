#include "stdafx.h"
#include "VEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define VE_TOP_MARGIN 4

/////////////////////////////////////////////////////////////////////////////
// CVEdit

CVEdit::CVEdit(CFont *pFont) 
: m_pFont(pFont), m_Brush(GetSysColor(COLOR_WINDOW))
, m_nTopMargin(0), m_nBottomMargin(0)
{
}

CVEdit::~CVEdit()
{
	m_Brush.DeleteObject();
}

BEGIN_MESSAGE_MAP(CVEdit, CEdit)
	//{{AFX_MSG_MAP(CVEdit)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVEdit message handlers

void CVEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if(bCalcValidRects){
		lpncsp->rgrc[0].top +=m_nTopMargin; 
		lpncsp->rgrc[0].bottom += m_nBottomMargin;
	}
} 

BOOL CVEdit::SetVMargins(INT nTop /* = 0 */, INT nBottom /* = 0 */)
{
	m_nTopMargin = nTop;
	m_nBottomMargin = nBottom;

	CRect rc;
	GetWindowRect(&rc);
	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOMOVE|SWP_FRAMECHANGED);

	return TRUE;
}

void CVEdit::OnNcPaint() 
{
	CRect rcNCTop, rcNCBottom;

	GetClientRect(&rcNCTop);

	CWindowDC dc(this);

	rcNCBottom = rcNCTop;

	rcNCTop.bottom = rcNCTop.top + m_nTopMargin;
	rcNCBottom.top = rcNCBottom.bottom - m_nBottomMargin;
	 
	if(rcNCTop.Height() > 0)
		dc.FillRect(rcNCTop, &m_Brush);

	if(rcNCBottom.Height() > 0)
		dc.FillRect(rcNCBottom, &m_Brush);

	Default();
}

void CVEdit::OnPaint()
{
	CPaintDC dc(this);
		
	CRect rect;
	GetClientRect(&rect);
	dc.DrawText(m_sText, &rect, DT_LEFT | DT_VCENTER);
}

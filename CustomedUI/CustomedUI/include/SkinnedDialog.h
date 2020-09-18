#ifndef _SKINNEDDIALOG_H_
#define _SKINNEDDIALOG_H_

#include <afxcmn.h>
#include <afxmt.h>

typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
	(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

class AFX_CLASS_EXPORT CSkinnedDialog : public CDialog
{
	DECLARE_DYNAMIC(CSkinnedDialog)

public:
	CSkinnedDialog(CWnd* pParent = NULL);
	CSkinnedDialog(UINT uResourceID, CWnd* pParent = NULL);
	virtual  ~CSkinnedDialog();

	BOOL setTransparentBG (BYTE bAlpha, BOOL bTrans = TRUE);
	BOOL setTransparentColor (COLORREF col, BOOL bTrans = TRUE);

	virtual void skin(int nBitmap, HINSTANCE hinst = 0);

	// Dialog Data
	//{{AFX_DATA(CSkinnedDialog)
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinnedDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void FreeResources();
	DWORD SetBitmap(HBITMAP hBitmap);
	// Generated message map functions
	//{{AFX_MSG(CSkinnedDialog)

	afx_msg void OnPaint();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	static lpfnSetLayeredWindowAttributes m_pSetLayeredWindowAttributes;
	static int m_nReferCount;
	static CCriticalSection m_csRefer;

protected:
	HBITMAP     m_hBitmap;
	DWORD		m_dwWidth;			// Width of bitmap
	DWORD		m_dwHeight;			// Height of bitmap

	COLORREF    m_colTransparent;
	bool        m_bTransparentColor;
	bool        m_bTransparent;
	BYTE        m_btDensity;

	BOOL		m_bStretchX;
	BOOL		m_bStretchY;
};

#endif // _SKINNEDDIALOG_H_

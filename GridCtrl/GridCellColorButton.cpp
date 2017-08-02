///////////////////////////////////////////////////////////////////////////
//
// GridCellColorButton.cpp: implementation of the CGridCellColorButton class.
//
// Provides the implementation for a ColorButton picker cell type of the
// grid control.
//
// Written by Podsypalnikov Eugen 15 Mar 2001
// Modified:
//    31 May 2001  Fixed m_cTime bug (Chris Maunder)
//
// For use with CGridCtrl v2.22+
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"
#include "GridCell.h"
#include "GridCellColorButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CGridCellColorButton

IMPLEMENT_DYNCREATE(CGridCellColorButton, CGridCell)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridCellColorButton::CGridCellColorButton() : CGridCell()
{
	m_dwStyle = 0;
	m_cColor   = RGB(0,200,0);
}

CGridCellColorButton::CGridCellColorButton(DWORD_PTR dwStyle) : CGridCell()
{
	//Init(dwStyle);
}

CGridCellColorButton::~CGridCellColorButton()
{
}

CSize CGridCellColorButton::GetCellExtent(CDC* pDC)
{    
    CSize sizeScroll (GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));	
    CSize sizeCell (CGridCell::GetCellExtent(pDC));	
    sizeCell.cx += sizeScroll.cx;	
    sizeCell.cy = max(sizeCell.cy,sizeScroll.cy);	
    return sizeCell;
}

BOOL CGridCellColorButton::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, 
							 UINT nID, UINT nChar)
{

	m_bEditing = TRUE;
	
	// CInPlaceColorButton auto-deletes itself
	m_pEditWnd = new CInPlaceColorButton(GetGrid(), rect,
		m_dwStyle/*|DTS_UPDOWN*/, nID, nRow, nCol, 
		GetTextClr(), GetBackClr(), m_cColor, nChar);
	

	return TRUE;
}

CWnd* CGridCellColorButton::GetEditWnd() const
{
	return m_pEditWnd;
}

void CGridCellColorButton::EndEdit()
{
	if (m_pEditWnd)
	{
		((CInPlaceColorButton*)m_pEditWnd)->EndEdit();
	}
}

void CGridCellColorButton::Init(COLORREF dwColor)
{
//	SetColor(dwColor);

	SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX
#ifndef _WIN32_WCE
		|DT_END_ELLIPSIS
#endif
		);
}

// Should be changed to use locale settings
void CGridCellColorButton::SetColor(COLORREF colo)
{
	m_cColor = colo;
}

BOOL CGridCellColorButton::Draw( CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /*= TRUE*/ )
{
	SetBackClr(m_cColor);
	return CGridCell::Draw(pDC, nRow, nCol, rect, bEraseBkgnd);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceColorButton

CInPlaceColorButton::CInPlaceColorButton(CWnd* pParent, CRect& rect, DWORD_PTR dwStyle, UINT nID,
								   int nRow, int nColumn, 
								   COLORREF crFore, COLORREF crBack,
								   COLORREF& pcColor,
								   UINT nFirstChar): CMFCColorButton(),
								   m_pcColor(pcColor)
{
	m_crForeClr     = crFore;
	m_crBackClr     = crBack;
	m_nRow          = nRow;
	m_nCol          = nColumn;
	m_nLastChar     = 0; 
	m_bExitOnArrows = FALSE;

	DWORD_PTR dwStl = WS_BORDER|WS_VISIBLE|WS_CHILD|dwStyle;

	if (!Create(_T(""),dwStl, rect, pParent, nID)) {
		return;
	}

	EnableOtherButton(_T("ÆäËû"));
	SetColor(pcColor);

	SetFont(pParent->GetFont());
	SetFocus();

	switch (nFirstChar) 
	{
		case VK_LBUTTON: 
		case VK_RETURN: return;
		case VK_BACK:   break;
		case VK_DOWN: 
		case VK_UP:   
		case VK_RIGHT:
		case VK_LEFT:  
		case VK_NEXT:  
		case VK_PRIOR: 
		case VK_HOME:  
		case VK_END:    return;
		default:        break;
	}
	SendMessage(WM_CHAR, nFirstChar);
}

CInPlaceColorButton::~CInPlaceColorButton()
{
}

void CInPlaceColorButton::EndEdit()
{
	CString str;
	if (::IsWindow(m_hWnd)) 
	{
		GetWindowText(str);
		//GetTime(*m_pcTime);
	}
	m_pcColor = GetColor();
	// Send Notification to parent
	GV_DISPINFO dispinfo;

	dispinfo.hdr.hwndFrom = GetSafeHwnd();
	dispinfo.hdr.idFrom   = GetDlgCtrlID();
	dispinfo.hdr.code     = GVN_ENDLABELEDIT;

	dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
	dispinfo.item.row     = m_nRow;
	dispinfo.item.col     = m_nCol;
	dispinfo.item.strText = str;
	dispinfo.item.lParam  = (LPARAM) m_nLastChar; 

	CWnd* pOwner = GetOwner();
	if (IsWindow(pOwner->GetSafeHwnd())) {
		pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo);
	}

	// Close this window (PostNcDestroy will delete this)
	if (::IsWindow(m_hWnd)) {
		PostMessage(WM_CLOSE, 0, 0);
	}
}

void CInPlaceColorButton::PostNcDestroy() 
{
	CMFCColorButton::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(CInPlaceColorButton, CMFCColorButton)
	//{{AFX_MSG_MAP(CInPlaceColorButton)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInPlaceColorButton message handlers

void CInPlaceColorButton::OnKillFocus(CWnd* pNewWnd) 
{
	CMFCColorButton::OnKillFocus(pNewWnd);
	if (GetSafeHwnd() == pNewWnd->GetSafeHwnd()) {
		return;
	}
	if (m_pPopup == NULL)
	{
		EndEdit();
	}
}

UINT CInPlaceColorButton::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}

void CInPlaceColorButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
// 	if (( nChar == VK_PRIOR || nChar == VK_NEXT ||
// 		nChar == VK_DOWN  || nChar == VK_UP   ||
// 		nChar == VK_RIGHT || nChar == VK_LEFT) &&
// 		(m_bExitOnArrows  || GetKeyState(VK_CONTROL) < 0))
// 	{
// 		m_nLastChar = nChar;
// 		GetParent()->SetFocus();
// 		return;
// 	}

	CMFCColorButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInPlaceColorButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
// 	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
// 	{
// 		m_nLastChar = nChar;
// 		GetParent()->SetFocus();    // This will destroy this window
// 		return;
// 	}

	CMFCColorButton::OnKeyUp(nChar, nRepCnt, nFlags);
}


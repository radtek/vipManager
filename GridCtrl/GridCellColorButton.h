// GridCellColorButton.h: interface for the CGridCellColorButton class.
//
// Provides the implementation for a ColorButton picker cell type of the
// grid control.
//
// For use with CGridCtrl v2.22+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORBUTTONCELL_H__A0B7DA0A_0AFE_4D28_A00E_846C96D7507A__INCLUDED_)
#define AFX_COLORBUTTONCELL_H__A0B7DA0A_0AFE_4D28_A00E_846C96D7507A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"
#include "afxdtctl.h"	// for CColorButtonCtrl

class CGridCellColorButton : public CGridCell  
{
  friend class CGridCtrl;
  DECLARE_DYNCREATE(CGridCellColorButton)

  COLORREF m_cColor;
  DWORD_PTR m_dwStyle;

public:
	CGridCellColorButton();
	CGridCellColorButton(DWORD_PTR dwStyle);
	virtual ~CGridCellColorButton();
    virtual CSize GetCellExtent(CDC* pDC);
	virtual BOOL     Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

  // editing cells
public:
	void Init(COLORREF dwColor);
	virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	virtual CWnd* GetEditWnd() const;
	virtual void  EndEdit();


	COLORREF GetColor() {return m_cColor;};
	void   SetColor(COLORREF colo);
};

class CInPlaceColorButton : public CMFCColorButton
{
public:
	CInPlaceColorButton(CWnd* pParent,         // parent
		CRect& rect,           // dimensions & location
		DWORD_PTR dwStyle,         // window/combobox style
		UINT nID,              // control ID
		int nRow, int nColumn, // row and column
		COLORREF crFore, COLORREF crBack,  // Foreground, background colour
		COLORREF& pcColor,
		UINT nFirstChar);      // first character to pass to control

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceList)
protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CInPlaceColorButton();
	void EndEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	//afx_msg void OnSelendOK();

	DECLARE_MESSAGE_MAP()

private:
	COLORREF& m_pcColor;
	int		 m_nRow;
	int		 m_nCol;
	UINT     m_nLastChar; 
	BOOL	 m_bExitOnArrows; 
	COLORREF m_crForeClr, m_crBackClr;
};

#endif // !defined(AFX_COLORBUTTONCELL_H__A0B7DA0A_0AFE_4D28_A00E_846C96D7507A__INCLUDED_)

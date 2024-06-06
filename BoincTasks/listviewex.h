// ListVwEx.h : interface of the CListViewEx class
//
// This class provedes a full row selection mode for the report
// mode list view control.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

#include "ListViewMine.h"

// starting with 0.68 moved to the lower part.
//#define IDC_IPEDIT 4321		
//#define PERC_CHAR '\255'
//#define PASS_CHAR '\254'
//#define HIDE_CHAR '\253'
//#define HIDE_SECOND '\252'

#define IDC_IPEDIT 4321	
#define PERC_CHAR	'\001'
#define PASS_CHAR	'\002'
#define HIDE_CHAR	'\003'
#define HIDE_SECOND	'\004'
#define COLOR_CHAR	'\005'

#define COLOR_OFFSET_STRIPING	10
#define COLOR_OFFSET_SKIN		20
#define COLOR_OFFSET_TOTAL		(COLOR_OFFSET_SKIN + COLOR_OFFSET_STRIPING)
#define COLOR_OFFSET_MIN		0

// offsets for first and other columns
#define OFFSET_FIRST    2
#define OFFSET_OTHER    6

class CListData
{
public:
	CListData(void);
	~CListData(void);

	COLORREF	m_color;
	int			m_iSortPrimary;
	int			m_iSortSecondary;
	int			m_iSortTertiary;
	double		m_fSortPrimary;
	double		m_fSortSecondary;
	double		m_fSortTertiary;
	char		*m_cSortPrimary;
	char		*m_cSortSecondary;
	char		*m_cSortTertiary;
	int			m_iComputerId;
	bool		m_bSelected;
	__int64		m_i64FilterApplicationAndVersion;		// when filtered

	CString		m_sSortPrimary;
	CString		m_sSortSecondary;
	CString		m_sSortTertiary;
};

class CInPlaceEdit;
class CListViewEx : public CListViewMine
{
	DECLARE_DYNCREATE(CListViewEx)

// Construction
public:
	CListViewEx();

// Attributes
protected:
	BOOL	m_bFullRowSel;
	BOOL	m_bSelection;

	char	m_cToolTipLongerBuffer[256];
	WCHAR	m_wToolTipLongerBuffer[256];

public:
	BOOL SetFullRowSel(BOOL bFillRowSel);
	BOOL GetFullRowSel();

	void EndInPlaceEdit();

	bool RemoveSpecialText(CString *pTxt) const;

	BOOL m_bClientWidthSel;

	void RemoveInplace();
	bool m_bInPlaceEdit;

	bool	m_bUseHorizontalGrid;
	bool	m_bUseVerticalGrid;
	bool	m_bUseSkin;
	int		m_iUseSkin;

// Overrides
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
			void PreSubclassWindow() ;
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
			int CellRectFromPoint(CPoint & point, RECT * cellrect, int * col) const;


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListViewEx)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
			
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListViewEx();

	static LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void ProcessColumn(LPCTSTR pszBuffer, CDC *pDC, CRect rcLabel, COLORREF	*pColorsGeneral, LV_COLUMN *pLvc);
	void ToMemRect(CRect *pRect);
	void DrawSkin(CDC* pDC, CRect rect, COLORREF color);

	void RepaintSelectedItems();

	int		HitTestEx(CListCtrl *pListCtrl, CPoint point, int *col) const;
	CEdit*	EditSubLabel(CListCtrl *pListCtrl, int nItem, int nCol );

	bool	m_bWasEditSubLabel;

	CFont	*m_pFont;

// Implementation - client area width
	int m_cxClient;

// Implementation - state icon width
	int m_cxStateImageOffset;
	afx_msg LRESULT OnSetImageList(WPARAM wParam, LPARAM lParam);

// Implementation - list view colors
	COLORREF m_clrText;
	COLORREF m_clrTextBk;
	COLORREF m_clrBkgnd;
	afx_msg LRESULT OnSetTextColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTextBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBkColor(WPARAM wParam, LPARAM lParam);

	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );


	afx_msg BOOL OnToolNeedText(UINT,NMHDR *,LRESULT *);

//	afx_msg void OnEndLabelEdit(WPARAM wParam, LRESULT* pResult) ;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	bool bFlickerFree;
	afx_msg BOOL OnEraseBkgnd(CDC *pDc);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	BOOL afx_msg OnDividerdblclick(UINT, NMHDR *,LRESULT *);
	afx_msg LRESULT OnInPlaceReady(WPARAM wParam, LPARAM lParam);
	CInPlaceEdit	*m_pInPlaceEdit;

// Generated message map functions
protected:
	//{{AFX_MSG(CListViewEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove( int, int );
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

/////////////////////////////////////////////////////////////////////////////

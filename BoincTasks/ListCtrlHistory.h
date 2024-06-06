#pragma once

//#include "SkinHeader.h"

// CListCtrlHistory

#define OFFSET_FIRST    2
#define OFFSET_OTHER    6

class CListData;
class CSelection;
class CListCtrlHistory : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlHistory)

public:
	CListCtrlHistory();
	virtual ~CListCtrlHistory();
	void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	void Sort();

	std::deque<CListData*> m_lListData;
	CSortColumn			*m_pSortColumn;
	CSelection			*m_pSelection;

	bool	m_bUseSkin;
	int		m_iUseSkin;

	bool	m_bUseVerticalGrid;
	bool	m_bUseHorizontalGrid;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

			void DrawSkin(CDC* pDC, CRect rect, COLORREF color);
			void ProcessColumn(LPCTSTR pszBuffer,  CDC* pDC, CRect rcItem, COLORREF	*pColorsGeneral, LV_COLUMN *pLvc);
			LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
			int		CellRectFromPoint(CPoint & point, RECT * cellrect, int * col) const;
			void DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin);

	CFont	*m_pFont;

	char	m_cToolTipLongerBuffer[256];
	WCHAR	m_wToolTipLongerBuffer[256];

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontHasChanged(WPARAM wParam,LPARAM lParam);

	afx_msg BOOL OnToolNeedText(UINT,NMHDR *,LRESULT *);
//	CSkinHeaderCtrl	m_SkinHeaderCtrl;

public:
	afx_msg BOOL OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
};



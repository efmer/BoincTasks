#pragma once

#include "SkinHeader.h"

// CListCtrlFloater

#define OFFSET_FIRST    2
#define OFFSET_OTHER    6

class CListCtrlFloater : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlFloater)

public:
	CListCtrlFloater();
	virtual ~CListCtrlFloater();
	void PreSubclassWindow();

	bool	m_bUseSkin;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

			void DrawSkin(CDC* pDC, CRect rect, COLORREF color);
			void ProcessColumn(LPCTSTR pszBuffer,  CDC* pDC, CRect rcItem, COLORREF	*pColorsGeneral, LV_COLUMN *pLvc);
			LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);

	CSkinHeaderCtrl	m_SkinHeaderCtrl;

public:
	afx_msg BOOL OnNMCustomdrawListFloating(NMHDR *pNMHDR, LRESULT *pResult);
};



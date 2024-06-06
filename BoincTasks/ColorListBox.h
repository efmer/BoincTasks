#pragma once

#include "DlgColorGraphic.h"

#define COLOR_BOX_COLOR_WIDTH	10
#define COLOR_BOX_COLOR_MARGIN_UD	3
#define COLOR_BOX_COLOR_MARGIN_LR	1

#define MAX_COLOR_ARRAY_G_NUMBER_COMPUTERS 300

// CColorListBox

class CColorListBox : public CListBox
{
	DECLARE_DYNAMIC(CColorListBox)

public:
	CColorListBox(bool bColor = true);
	virtual ~CColorListBox();
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	int	AddString(int iIndex, LPCTSTR lpszItem, int iColorIndex);
	int	AddString(LPCTSTR lpszItem, int iColorIndex);
	int AddString(LPCTSTR lpszItem);
	void	MessagesTo(HWND hWnd);
	void	SetColor(COLORREF color, int iRepeat, int iIndex, int iColorPos);
	COLORREF GetColor(int iPos, int *piRepeat=NULL);
	void Reset();	

	bool	m_bColor;
protected:
	COLORREF m_color[MAX_COLOR_ARRAY_G_NUMBER_COMPUTERS+1];
	int		 m_iRepeat[MAX_COLOR_ARRAY_G_NUMBER_COMPUTERS+1];

	bool	m_bMultipleSelection;
	int		m_iLastItem;

	HWND	m_hWndMessagesTo;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg LRESULT OnSelectionMode(WPARAM parm1, LPARAM parm2);
};



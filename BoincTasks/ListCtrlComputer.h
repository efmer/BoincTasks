#pragma once

// CListCtrlComputer

class CListCtrlComputer : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlComputer)

public:
	CListCtrlComputer();
	virtual ~CListCtrlComputer();
	virtual void PreSubclassWindow();
    enum EHighlight { HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW };
	virtual INT_PTR OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	//virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );

//	BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
protected:
	DECLARE_MESSAGE_MAP()

	int	m_nHighlight;
	HWND m_hWnd;

	void RepaintSelectedItems();

	afx_msg LRESULT OnWnd ( WPARAM wParam, LPARAM lParam);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd) ;
	afx_msg void OnSetFocus(CWnd* pOldWnd) ;
	virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );
//	afx_msg BOOL OnToolNeedText(UINT, NMHDR* pNMHDR, LRESULT* pResult);

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};



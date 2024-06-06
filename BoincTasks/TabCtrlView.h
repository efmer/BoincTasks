#pragma once


// CTabCtrlView

class CTabCtrlView : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlView)

public:
	CTabCtrlView();
	virtual ~CTabCtrlView();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	BOOL	m_bEnable;
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnDrawItem(int nIDCtrl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};



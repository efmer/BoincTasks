#pragma once


// CListCtrlRules

class CListCtrlRules : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlRules)

public:
	CListCtrlRules();
	virtual ~CListCtrlRules();

	bool	m_bCheckChanged;

protected:
	void	CheckItem(int nNewCheckedItem);
	void	Reset(int nNewCheckedItem);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

};



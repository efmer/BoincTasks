#pragma once

#define INDEX_GRAYED					INDEXTOSTATEIMAGEMASK(3)
#define INDEX_CHECKED					INDEXTOSTATEIMAGEMASK(2)
#define INDEX_UNCHECKED					INDEXTOSTATEIMAGEMASK(1)

// CListCtrlFindComputers

class CListCtrlFindComputers : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlFindComputers)

public:
	CListCtrlFindComputers();
	virtual ~CListCtrlFindComputers();

protected:
	void	CheckItem(int nNewCheckedItem);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

};



#pragma once


// CListViewMine view

class CListViewMine : public CListView
{
	DECLARE_DYNCREATE(CListViewMine)

protected:
	CListViewMine();           // protected constructor used by dynamic creation
	virtual ~CListViewMine();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};



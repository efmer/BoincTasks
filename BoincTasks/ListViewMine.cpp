// ListViewMine.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewMine.h"


// CListViewMine

IMPLEMENT_DYNCREATE(CListViewMine, CListView)

CListViewMine::CListViewMine()
{

}

CListViewMine::~CListViewMine()
{
}

BOOL CListViewMine::OnEraseBkgnd(CDC* pDC) 
{

  return TRUE;
}

BEGIN_MESSAGE_MAP(CListViewMine, CListView)
END_MESSAGE_MAP()


// CListViewMine diagnostics

#ifdef _DEBUG
void CListViewMine::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CListViewMine::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CListViewMine message handlers

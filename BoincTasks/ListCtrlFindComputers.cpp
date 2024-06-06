// ListCtrlFindComputers.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListCtrlFindComputers.h"


// CListCtrlFindComputers

IMPLEMENT_DYNAMIC(CListCtrlFindComputers, CListCtrl)

CListCtrlFindComputers::CListCtrlFindComputers()
{

}

CListCtrlFindComputers::~CListCtrlFindComputers()
{
}


BEGIN_MESSAGE_MAP(CListCtrlFindComputers, CListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CListCtrlFindComputers message handlers

void CListCtrlFindComputers::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT uFlags = 0;

	int nHitItem =  HitTest(point, &uFlags);

	// we need additional checking in owner-draw mode
	// because we only get LVHT_ONITEM
	BOOL bHit = FALSE;
	if (uFlags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))
	{
		CRect rect;
		GetItemRect(nHitItem, rect, LVIR_ICON);

		// check if hit was on a state icon
		if (point.x < rect.left) bHit = TRUE;
	}
	else if (uFlags & LVHT_ONITEMSTATEICON)	bHit = TRUE;

	if (bHit) CheckItem(nHitItem);
	else
	{

		CListCtrl::OnLButtonDown(nFlags, point);
	}


//	if (bHit) m_bChanged = true;

}

void CListCtrlFindComputers::CheckItem(int nNewCheckedItem)
{

	if (GetItemState(nNewCheckedItem,LVIS_STATEIMAGEMASK) == INDEX_GRAYED) return;				// disabled

	if (GetItemState(nNewCheckedItem,LVIS_STATEIMAGEMASK) == INDEX_CHECKED)
	{
		SetItemState(nNewCheckedItem,INDEX_UNCHECKED, LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK);
	}
	else
	{
		SetItemState(nNewCheckedItem, INDEX_CHECKED,	LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK);
	}
}

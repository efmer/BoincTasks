// ListCtrlFindComputers.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListCtrlRules.h"


// CListCtrlRules

IMPLEMENT_DYNAMIC(CListCtrlRules, CListCtrl)

CListCtrlRules::CListCtrlRules()
{
	m_bCheckChanged = false;
}

CListCtrlRules::~CListCtrlRules()
{
}

void CListCtrlRules::CheckItem(int nNewCheckedItem)
{

// reset if there is checked item

	if (GetItemState(nNewCheckedItem,LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))
	{
		SetItemState(nNewCheckedItem,INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK);
	}
	else
	{
		SetItemState(nNewCheckedItem, INDEXTOSTATEIMAGEMASK(2),	LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK);
	}
}

void CListCtrlRules::Reset(int nNewCheckedItem)
{
//	int iItems;
//
//	iItems = this->GetItemCount();
//
//	for (int iCount = 0; iCount < iItems; iCount++)
//	{
//		if (iCount != nNewCheckedItem)
//		{
//			SetItemState(iCount,INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK);
//		}
//	}
}

BEGIN_MESSAGE_MAP(CListCtrlRules, CListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CListCtrlRules message handlers

void CListCtrlRules::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT uFlags = 0;

	int nHitItem =  HitTest(point, &uFlags);

	LVHITTESTINFO lvhti;

	// Clear the subitem text the user clicked on.
	lvhti.pt = point;
	SubItemHitTest(&lvhti);


	// we need additional checking in owner-draw mode
	// because we only get LVHT_ONITEM
	BOOL bHit = FALSE;
	if (uFlags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))
	{
		CRect rect;
		GetItemRect(lvhti.iItem, rect, LVIR_ICON);

		// check if hit was on a state icon
		if (point.x < rect.left)
		{
			bHit = TRUE;
		}
	}
	else
	{
		if (lvhti.iSubItem == 0)
		{
			if (uFlags & LVHT_ONITEMSTATEICON)
			{
				bHit = TRUE;
				m_bCheckChanged = true;
			}
		}
	}

	if (bHit)
	{
		CheckItem(lvhti.iItem);
		Reset(lvhti.iItem);
	}
	else
	{

		CListCtrl::OnLButtonDown(nFlags, point);
	}


//	if (bHit) m_bChanged = true;

}

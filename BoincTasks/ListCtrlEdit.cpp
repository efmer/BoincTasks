// ListCtrlEdit.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListCtrlEdit.h"


// CListCtrlEdit

IMPLEMENT_DYNAMIC(CListCtrlEdit, CListCtrl)

CListCtrlEdit::CListCtrlEdit()
{
	m_item = 0;
	m_subitem = 0;
}

CListCtrlEdit::~CListCtrlEdit()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEdit, CListCtrl)
ON_WM_LBUTTONDOWN()
ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnLvnBeginlabeledit)
ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
ON_WM_PAINT()
END_MESSAGE_MAP()



// CListCtrlEdit message handlers

void CListCtrlEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO lvhit;
	lvhit.pt = point;
	int item = SubItemHitTest(&lvhit);
	//if (over a subitem)
	if (item != -1 && lvhit.iSubItem && (lvhit.flags & LVHT_ONITEM ))
	{
		//mouse click outside the editbox in an already
		//editing cell cancels editing
		if (m_subitem == lvhit.iSubItem && item == m_item)
		{
			CListCtrl::OnLButtonDown(nFlags, point);
		}
		else
		{
			CListCtrl::OnLButtonDown(nFlags, point);
			m_subitem = lvhit.iSubItem;
			m_item = item;
			EditLabel(item);
		}
	}
	else
	CListCtrl::OnLButtonDown(nFlags, point);
}
void CListCtrlEdit::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	//if (subitem selected for editing)
	if (m_subitem)
	{
		ASSERT(m_item == pDispInfo->item.iItem);
		CRect subrect;
		GetSubItemRect( pDispInfo->item.iItem, m_subitem, LVIR_BOUNDS , subrect );
		//get edit control and subclass
		HWND hWnd=(HWND)SendMessage(LVM_GETEDITCONTROL);
		ASSERT(hWnd!=NULL);
		VERIFY(m_editWnd.SubclassWindow(hWnd));
		//move edit control text 1 pixel to the right of org label,
		//as Windows does it...
		m_editWnd.m_x=subrect.left + 6;

		m_editWnd.SetWindowText(GetItemText(pDispInfo->item.iItem,m_subitem));
		//hide subitem text so it don't show if we delete some
		//text in the edit control
		//OnPaint handles other issues also regarding this
		CRect rect;
		GetSubItemRect(pDispInfo->item.iItem,m_subitem, LVIR_LABEL ,rect);
		CDC* hDc = GetDC();

		hDc->FillRect(rect,&CBrush(::GetSysColor(COLOR_WINDOW)));

//		m_editWnd.SetWindowText(GetItemText(pDispInfo->item.iItem,m_subitem));
		ReleaseDC(hDc);
	}
	*pResult = 0;
}

//Saving the Changes
void CListCtrlEdit::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	LV_ITEM *plvItem = &pDispInfo->item;
	//if (end of sub-editing) do cleanup
	if (m_subitem)
	{
		//plvItem->pszText is NULL if editing canceled
		if (plvItem->pszText != NULL )
		SetItemText(plvItem->iItem,m_subitem, plvItem->pszText);
		VERIFY(m_editWnd.UnsubclassWindow()!=NULL);
		m_subitem = 0;
		//always revert to org label (Windows thinks we are editing
		//the leftmost item)
		*pResult = 0;
	}
	else
	//return: update label on leftmost item
	*pResult = 1;
}

// Make sure that first Item is not getting invisible.
void CListCtrlEdit::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	if (m_subitem)
	{
		CRect rect;
		CRect editrect;
		GetSubItemRect(m_item,m_subitem,LVIR_LABEL, rect);
		m_editWnd.GetWindowRect(editrect);
		ScreenToClient(editrect);
		//block text redraw of the subitems text (underneath the
		//editcontrol)
		//if we didn't do this and deleted some text in the edit
		//control, the subitems original label would show
		if (editrect.right < rect.right)
		{
			rect.left = editrect.right ;
			ValidateRect(rect);
		}	
		//block filling redraw of leftmost item (caused by FillRect)
		GetItemRect(m_item, rect,LVIR_LABEL );
		ValidateRect(rect);
	}
	// Do not call CListCtrl::OnPaint() for painting messages
	CListCtrl::OnPaint();
}


//*****************************************
BEGIN_MESSAGE_MAP(cMyEdit, CEdit)
ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

void cMyEdit::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CEdit::OnWindowPosChanging(lpwndpos);
	lpwndpos->x = m_x;
}


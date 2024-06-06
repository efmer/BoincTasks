// ListCtrlComputer.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "MainFrm.h"
#include "ListCtrlComputer.h"


// CListCtrlComputer

IMPLEMENT_DYNAMIC(CListCtrlComputer, CListCtrl)

CListCtrlComputer::CListCtrlComputer()
{
	m_hWnd = NULL;
	m_nHighlight = HIGHLIGHT_ROW;
}

CListCtrlComputer::~CListCtrlComputer()
{
}

void CListCtrlComputer::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();

	EnableToolTips(TRUE);
}


BEGIN_MESSAGE_MAP(CListCtrlComputer, CListCtrl)
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_MESSAGE(UWM_MSG_WND, OnWnd)

	ON_WM_KILLFOCUS()   
	ON_WM_SETFOCUS()

	ON_WM_LBUTTONDOWN()

	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CListCtrlComputer message handlers

LRESULT CListCtrlComputer::OnWnd ( WPARAM wParam, LPARAM lParam)
{
	m_hWnd = (HWND) wParam;

	return 0;
}

void CListCtrlComputer::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iItemIcon;
	UINT uFlags;
	iItemIcon = HitTest(point, &uFlags);
	if (uFlags == LVHT_ONITEMICON)
	{
		::PostMessage(m_hWnd, UWM_MSG_CLICKED_ON_ICON, 0, iItemIcon);
		return;
	}
	CListCtrl::OnLButtonDblClk(nFlags,point);
}

INT_PTR CListCtrlComputer::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	RECT rect;

	UINT nFlags;

	//	HTREEITEM hitem = HitTest( point, &nFlags );

	int iItem = HitTest( point, &nFlags );
	if( nFlags & TVHT_ONITEMLABEL  )
	{
		GetItemRect( iItem, &rect, TRUE );
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT) iItem;

//		pTI->lpszText = (char *)malloc(200 * sizeof(char));
//		lstrcpyn(pTI->lpszText, "test111", 198);

		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rect;
		return pTI->uId +32000;
	}
	
	return -1;
}

//here we supply the text for the item 
BOOL CListCtrlComputer::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT_PTR nID = pNMHDR->idFrom;
	// Do not process the message from built in tooltip 
	if( nID == (UINT_PTR)m_hWnd &&
		(( pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND ) ||
		( pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND ) ) )
		return FALSE;

	// Get the mouse position
	const MSG* pMessage;
	CPoint pt;
	pMessage = GetCurrentMessage(); // get mouse pos 
	ASSERT ( pMessage );
	pt = pMessage->pt;
	ScreenToClient( &pt );

	UINT nFlags;
//	HTREEITEM hitem = HitTest( pt, &nFlags ); //Get item pointed by mouse
	int iItem;
	iItem = HitTest( pt, &nFlags ); //Get item pointed by mouse
	
//	strTipText.Format( "%s", GetItemText( (HTREEITEM ) nID));  //get item text
	strTipText = "test";

//	DWORD dw = GetItemData((HTREEITEM ) nID); //get item data
//	ItemDataABSTRACT* ItemData = (ItemDataABSTRACT*)dw; //CAST item data
//	if (ItemData!=NULL)
//	{
//		CString s = ItemData->GetToolTipString(); //pure virtual function
//		strTipText=" " + s; //add node text to node data text
//	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, 80);
	else
		_mbstowcsz(pTTTW->szText, strTipText, 80);
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, 80);
	else
		lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
	*pResult = 0;

	return TRUE;    // message was handled
}



void CListCtrlComputer::OnKillFocus(CWnd* pNewWnd) 
{
//    CListCtrl::OnKillFocus(pNewWnd);
}

void CListCtrlComputer::OnSetFocus(CWnd* pOldWnd) 
{
  CListCtrl::OnSetFocus(pOldWnd);
}



void CListCtrlComputer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,1);
		break;
	}

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CListCtrlComputer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,0);
		break;
	}

	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

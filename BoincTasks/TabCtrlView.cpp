// TabCtrlView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "TabCtrlView.h"


// CTabCtrlView

IMPLEMENT_DYNAMIC(CTabCtrlView, CTabCtrl)

CTabCtrlView::CTabCtrlView()
{
}

CTabCtrlView::~CTabCtrlView()
{
}


BEGIN_MESSAGE_MAP(CTabCtrlView, CTabCtrl)
//	ON_WM_ERASEBKGND()
//	ON_WM_DRAWITEM()

	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnTcnSelchange)

END_MESSAGE_MAP()



// CTabCtrlView message handlers

BOOL CTabCtrlView::OnEraseBkgnd(CDC* pDC)
{
	CBrush bBg(RGB(255,0,0));  
	RECT rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect,&bBg);
//	(m_brBackGround));

return 0; // don't do the default erase
}

void CTabCtrlView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect = lpDrawItemStruct->rcItem;
	int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0) return;
	BOOL bSelected = (nTabIndex == GetCurSel());

	char label[64];
	TC_ITEM tci;
	tci.mask = TCIF_TEXT|TCIF_IMAGE;
	tci.pszText = label;
	tci.cchTextMax = 63;
	if (!GetItem(nTabIndex, &tci )) return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC) return;
	int nSavedDC = pDC->SaveDC();

	// For some bizarre reason the rcItem you get extends above the actual
	// drawing area. We have to workaround this "feature".
	rect.top += ::GetSystemMetrics(SM_CYEDGE);

	pDC->SetBkMode(OPAQUE);

	if (!m_bEnable)
	{
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
	}
	else
	{
		if (bSelected)	pDC->FillSolidRect(rect, RGB (198,228,253)); // ::GetSysColor(COLOR_BTNFACE));
		else			pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
	}
	// Draw image
	CImageList* pImageList = GetImageList();
	if (pImageList && tci.iImage >= 0) {

		rect.left += pDC->GetTextExtent(_T(" ")).cx;		// Margin

		// Get height of image so we 
		IMAGEINFO info;
		pImageList->GetImageInfo(tci.iImage, &info);
		CRect ImageRect(info.rcImage);
		int nYpos = rect.top;

		if (!m_bEnable)
		{
		}
		else
		{
			pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT | ILS_GLOW);
		}
		rect.left += ImageRect.Width();
	}

	if (!m_bEnable)
	{
			pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
			rect.top -= ::GetSystemMetrics(SM_CYEDGE);
			pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
	else
	{
		if (bSelected)
		{
			pDC->SetTextColor(RGB(0,0,255));
			rect.top -= ::GetSystemMetrics(SM_CYEDGE);
			pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
		}
	}
	
	pDC->RestoreDC(nSavedDC);

}

BOOL CTabCtrlView::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	int iSelected;
	CWnd *pWnd;

	pWnd =  GetParent();
	iSelected = this->GetCurSel();

	if (pWnd != NULL) pWnd->PostMessage(UWM_MSG_TAB_VIEW_SELECTED,iSelected,0);

	return TRUE;
}


// ListVwEx.cpp : implementation of the CListViewEx class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "boincTasks.h"
#include "ListViewMine.h"
#include "ListViewEx.h"
#include "InPlaceEdit.h"
#include "MainFrm.h"
#include "DlgBarViewSelect.h"
#include "DlgColorGeneral.h"
#include "DlgSettingsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CListData::CListData()
{
	m_bSelected = false;
}

CListData::~CListData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CListViewEx

IMPLEMENT_DYNCREATE(CListViewEx, CListViewMine)

BEGIN_MESSAGE_MAP(CListViewEx, CListViewMine)
	//{{AFX_MSG_MAP(CListViewEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
//	ON_WM_MOVE()

	//}}AFX_MSG_MAP
	ON_MESSAGE(LVM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(LVM_SETTEXTCOLOR, OnSetTextColor)
	ON_MESSAGE(LVM_SETTEXTBKCOLOR, OnSetTextBkColor)
	ON_MESSAGE(LVM_SETBKCOLOR, OnSetBkColor)

	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_INPLACE_READY, OnInPlaceReady)

    ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
    ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
	
	ON_WM_MEASUREITEM_REFLECT( )
	ON_NOTIFY_EX(HDN_DIVIDERDBLCLICK, 0, OnDividerdblclick)

	ON_MESSAGE(WM_SETFONT, OnSetFont)


	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListViewEx construction/destruction

CListViewEx::CListViewEx()
{
	m_bFullRowSel = FALSE;
	m_bClientWidthSel = TRUE;
	m_bSelection = TRUE;
	m_bUseSkin = true;

	m_cxClient = 0;
	m_cxStateImageOffset = 0;

	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk = ::GetSysColor(COLOR_WINDOW);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);

	m_pInPlaceEdit = NULL;
	m_bInPlaceEdit = false;
	m_bWasEditSubLabel = false;

	bFlickerFree = true;

	m_bUseHorizontalGrid = false;
	m_bUseVerticalGrid = false;

}

CListViewEx::~CListViewEx()
{
}

BOOL CListViewEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// default is report view and full row
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED;// | LVS_EX_LABELTIP;
//	cs.style |= LVS_EX_DOUBLEBUFFER;	// nothing happens
	m_bFullRowSel = TRUE;

	return(CListViewMine::PreCreateWindow(cs));
}

void CListViewEx::PreSubclassWindow()
{
	CListViewMine::PreSubclassWindow();
	EnableToolTips(TRUE);
}

void CListViewEx::MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	CDC dcMemory;
	TEXTMETRIC tm;
	LOGFONT lf;
	CFont *pFontPrevious;
	BOOL	bResult;
	CSize	sizeText;

	GetFont()->GetLogFont( &lf );

    dcMemory.CreateCompatibleDC(NULL);
	CFont * font = GetFont();
	pFontPrevious = dcMemory.SelectObject(font);
	bResult = dcMemory.GetTextMetrics(&tm);
	lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + 1;
	dcMemory.SelectObject(pFontPrevious);
}

BOOL CListViewEx::SetFullRowSel(BOOL bFullRowSel)
{
	// no painting during change
	LockWindowUpdate();

	m_bFullRowSel = bFullRowSel;

	BOOL bRet;

	if (m_bFullRowSel) bRet = ModifyStyle(0L, LVS_OWNERDRAWFIXED);
	else bRet = ModifyStyle(LVS_OWNERDRAWFIXED, 0L);

	// repaint window if we are not changing view type
	if (bRet && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)	Invalidate();

	// repaint changes
	UnlockWindowUpdate();

	return(bRet);
}

BOOL CListViewEx::GetFullRowSel()
{
	return(m_bFullRowSel);
}




/////////////////////////////////////////////////////////////////////////////
// CListViewEx drawing

void CListViewEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CListCtrl& ListCtrl=GetListCtrl();
	CDC* pDCScreen = CDC::FromHandle(lpDrawItemStruct->hDC);
	CDC dcMem;
	CDC *pDC = &dcMem;
	CBitmap bitmap;

	CRect rcItemScreen(lpDrawItemStruct->rcItem);
	CRect rcItemLabel;

	CPen penBorder;
	UINT uiFlags = ILD_TRANSPARENT;
	CImageList* pImageList;
	int nItem = lpDrawItemStruct->itemID;
	BOOL bFocus = (GetFocus() == this);
	BOOL bSelected = FALSE;
	COLORREF clrTextSave = 0;
	COLORREF clrBkSave = 0;
	COLORREF clrImage = m_clrBkgnd;

	CFont *pFontPrevious;

	static _TCHAR szBuff[MAX_PATH];
	COLORREF	*pColorsGeneral;
	
	dcMem.CreateCompatibleDC(pDCScreen);
	bitmap.CreateCompatibleBitmap(pDCScreen, rcItemScreen.Width(), rcItemScreen.Height());
	CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);

	CRect rcItem;
	rcItem.SetRect(0,0,rcItemScreen.Width(), rcItemScreen.Height());
	ToMemRect(&rcItem);

	CBrush brush;
	brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	pDC->FillRect(rcItem,&brush);

	CFont * font = GetFont();
	pFontPrevious = dcMem.SelectObject(font);

	pDC->SetBkMode(TRANSPARENT);

// get item data

	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.lParam = 0xffffff;
	lvi.pszText = szBuff;
	lvi.cchTextMax = sizeof(szBuff);
	lvi.stateMask = 0xFFFF;     // get all state flags
	ListCtrl.GetItem(&lvi);

	CListData	*pListData;
	pListData = (CListData *) lvi.lParam;

	pColorsGeneral = theApp.m_pDlgColorGeneral->GetColorArray();

	penBorder.CreatePen(PS_SOLID,1,*(pColorsGeneral + COLOR_GENERAL_ARRAY_GRID));

	if (m_bSelection)
	{
		bSelected = (bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
		bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);
	}
	else
	{
		bSelected = pListData->m_bSelected;
	}

// set colors if item is selected

	CRect rcAllLabels;
	ListCtrl.GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);
	ToMemRect(&rcAllLabels);

	CRect rcLabel;
	ListCtrl.GetItemRect(nItem, rcLabel, LVIR_LABEL);
	ToMemRect(&rcLabel);

	rcAllLabels.left = rcLabel.left;
	if (m_bClientWidthSel && rcAllLabels.right<m_cxClient)	rcAllLabels.right = m_cxClient;

	if (bSelected)
	{
		COLORREF clr;
		clrTextSave = pDC->SetTextColor(*(pColorsGeneral + COLOR_GENERAL_ARRAY_FONT_SELECTED));
		clrBkSave = pDC->SetBkColor(*(pColorsGeneral + COLOR_GENERAL_ARRAY_BACK_SELECTED));
		clr = (*(pColorsGeneral + COLOR_GENERAL_ARRAY_BACK_SELECTED));
		if (m_bUseSkin)	DrawSkin(pDC, rcAllLabels, clr);
		else pDC->FillSolidRect(rcAllLabels, clr);
	}
	else
	{
		COLORREF clr;
		clrTextSave = pDC->SetTextColor(*(pColorsGeneral + COLOR_GENERAL_ARRAY_FONT));
		clrBkSave = pDC->GetBkColor();
		if (pListData != NULL)	clr = pListData->m_color;
		else clr = RGB(255,255,255);							// NULL shouldn't happen but it can
		if (m_bUseSkin)	DrawSkin(pDC, rcAllLabels, clr);
		else pDC->FillSolidRect(rcAllLabels, clr);
	}

// set color and mask for the icon

	if (lvi.state & LVIS_CUT)
	{
		clrImage = m_clrBkgnd;
		uiFlags |= ILD_BLEND50;
	}
	else
	{
		if (bSelected)
		{
//			if (m_bSelection)
			{
				clrImage = ::GetSysColor(COLOR_HIGHLIGHT);
				uiFlags |= ILD_BLEND50;
			}
		}
	}

// draw state icon

	UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
	if (nStateImageMask)
	{
		int nImage = (nStateImageMask>>12) - 1;
		pImageList = ListCtrl.GetImageList(LVSIL_STATE);
		if (pImageList)
		{
			pImageList->Draw(pDC, nImage,
			CPoint(rcItem.left, rcItem.top), ILD_TRANSPARENT);
		}
	}

// draw normal and overlay icon

	CRect rcIcon;
	ListCtrl.GetItemRect(nItem, rcIcon, LVIR_ICON);

	pImageList = ListCtrl.GetImageList(LVSIL_SMALL);
	if (pImageList)
	{
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		if (rcItem.left<rcItem.right-1)
		{
			ImageList_DrawEx(pImageList->m_hImageList, lvi.iImage, pDC->m_hDC,rcIcon.left,rcIcon.top, 16, 16, m_clrBkgnd, clrImage, uiFlags | nOvlImageMask);
		}
	}

// draw item label

	ListCtrl.GetItemRect(nItem, rcItemLabel, LVIR_LABEL);
	ToMemRect(&rcItemLabel);

	rcItemLabel.left += m_cxStateImageOffset;
	rcItemLabel.right+= m_cxStateImageOffset;

//	rcItemLabel.right -= m_cxStateImageOffset;

//	nJustify = DT_LEFT;
	ProcessColumn(szBuff, pDC, rcItemLabel, pColorsGeneral, NULL);

// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for(int nColumn = 1; ListCtrl.GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItemLabel.left = rcItemLabel.right;
		rcItemLabel.right += lvc.cx;

		
		// draw vertical lines
		if (m_bUseVerticalGrid)
		{
			pDC->SelectObject(&penBorder);
			pDC->MoveTo(rcItemLabel.left-1, rcItemLabel.top);
			pDC->LineTo(rcItemLabel.left-1, rcItemLabel.bottom);
		}
		int nRetLen = ListCtrl.GetItemText(nItem, nColumn,	szBuff, sizeof(szBuff));
		if (nRetLen == 0) continue;
		ProcessColumn(szBuff, pDC, rcItemLabel, pColorsGeneral, &lvc);
	}

// draw focus rectangle if item has focus

	if (m_bSelection)
	{
//		if (lvi.state & LVIS_FOCUSED && bFocus) pDC->DrawFocusRect(rcAllLabels);
	}

	
	// draw horizontal lines
	if (m_bUseHorizontalGrid)
	{
		pDC->SelectObject(&penBorder);
		pDC->MoveTo(rcAllLabels.left, rcAllLabels.bottom-1);
		pDC->LineTo(rcAllLabels.right, rcAllLabels.bottom-1);
	}
	


// set original colors if item was selected

//	if (m_bSelection)
	{
//		if (bSelected)
		{
			pDC->SetTextColor(clrTextSave);
			pDC->SetBkColor(clrBkSave);
		}
	}

	penBorder.DeleteObject();

	pDCScreen->BitBlt(rcItemScreen.left, rcItemScreen.top, rcItemScreen.Width(), rcItemScreen.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pFontPrevious);
	dcMem.SelectObject(pOldBitmap);
}

void CListViewEx::ToMemRect(CRect *pRect)
{
	int iWidth, iHeight;

	iWidth = pRect->Width();
	iHeight = pRect->Height();

	pRect->top = 0;
	pRect->bottom = iHeight;

	pRect->left = 0;
	pRect->right= iWidth;
}

void CListViewEx::ProcessColumn(LPCTSTR pszBuffer,  CDC* pDC, CRect rcItem, COLORREF	*pColorsGeneral, LV_COLUMN *pLvc)
{
	int			iSpecial, iPercDraw, iPerc, iColor;
	char		*pszHide, *pszColor;
	char		cBuffer[255];
	LPCTSTR		pszCleanText, pszText;
	CRect		rcLabel;
	UINT		nJustify;
	COLORREF	crBackgroundRestore, crBackground;
	bool		bSafeBackgroundRestore;

	iSpecial = *pszBuffer;
	rcLabel = rcItem;
	rcLabel.left += OFFSET_OTHER;
	rcLabel.right -= OFFSET_OTHER;

	bSafeBackgroundRestore	= false;

	switch (iSpecial)
	{
		case PERC_CHAR:
			iPercDraw = *(pszBuffer+1);
			iPerc = *(pszBuffer+2);
			iPerc--;					// 0 is not used
			if (iPercDraw ==  2)
			{
				CRect rectPercOutline;
				rectPercOutline = rcLabel;
				CBrush brPerc(*(pColorsGeneral + COLOR_GENERAL_ARRAY_BAR_OUTLINE));
				CRect rectPerc;
				int iWidth = rcLabel.Width()-1;
				iWidth *= iPerc;
				iWidth /= 100;
				if (m_bUseSkin && (iWidth < 4)) iWidth = 4;
				iWidth += rcLabel.left;
				rectPerc.SetRect(rcLabel.left+1,rcLabel.top+1,iWidth,rcLabel.bottom-1); 
				
				strcpy_s(cBuffer,254,pszBuffer+3);

				pszColor = strchr(cBuffer, COLOR_CHAR);

				crBackgroundRestore = pDC->GetBkColor();
				bSafeBackgroundRestore = true;
				if (pszColor != NULL)
				{
					iColor = atoi(pszColor+1);
					*pszColor = 0;
					CRect rectCellColor;
					int iWidth = rcItem.Width();
					iWidth += rcItem.left;
					rectCellColor.SetRect(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom); 
					if (m_bUseSkin)	DrawSkin(pDC, rectCellColor, iColor);
					else pDC->FillSolidRect(rectCellColor, iColor);
//					pDC->FillSolidRect(rectCellColor,iColor);
				}
				
				crBackground = *(pColorsGeneral + COLOR_GENERAL_ARRAY_BAR);

				if (m_bUseSkin)	DrawSkin(pDC, rectPerc, crBackground);
				else pDC->FillSolidRect(rectPerc, crBackground);

				if (theApp.m_pDlgSettingsView->m_bPercRect)
				{
					pDC->FrameRect(rectPercOutline,&brPerc);
				}
				pszCleanText = cBuffer;
			}
			else 
			{
				pszCleanText = NULL;
			}
		break;

		case COLOR_CHAR:
			crBackgroundRestore = pDC->GetBkColor();
			bSafeBackgroundRestore = true;
			strcpy_s(cBuffer,255,pszBuffer+1);
			cBuffer[254] = 0;
			pszHide = strchr(cBuffer, HIDE_CHAR);
			if (pszHide != NULL)
			{
				iColor = atoi(pszHide+1);
				CRect rectColor;
				int iWidth = rcItem.Width();
				iWidth += rcItem.left;
				rectColor.SetRect(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom); 
				if (m_bUseSkin)	DrawSkin(pDC, rectColor, iColor);
				else pDC->FillSolidRect(rectColor, iColor);
//				pDC->FillSolidRect(rectColor,iColor);
			}
			strcpy_s(cBuffer,255,pszBuffer+1);
			cBuffer[254] = 0;
			pszHide = strchr(cBuffer, HIDE_CHAR);
			if (pszHide != NULL)
			{
				*pszHide = 0;
			}
			pszCleanText = cBuffer;
		break;

		case PASS_CHAR:
			pszCleanText = "*****";
		break;
			
		case HIDE_CHAR:
			strcpy_s(cBuffer,255,pszBuffer+1);
			cBuffer[254] = 0;
			pszHide = strchr(cBuffer, HIDE_CHAR);
			if (pszHide != NULL) *pszHide = 0;
			pszCleanText = cBuffer;
		break;

		case HIDE_SECOND:
			strcpy_s(cBuffer,255,pszBuffer+1);
			cBuffer[254] = 0;
			pszHide = strchr(cBuffer, HIDE_CHAR);
			if (pszHide != NULL) *pszHide = 0;
			pszCleanText = cBuffer;
		break;

		default:
			pszCleanText = pszBuffer;
	}

	if (pszCleanText == NULL) return;

	pszText = MakeShortString(pDC, pszCleanText, rcItem.right - rcItem.left, 2*OFFSET_OTHER);

	nJustify = DT_LEFT;

	if(pszText == pszCleanText)
	{
		if (pLvc != NULL)
		{
			switch(pLvc->fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_RIGHT:
				nJustify = DT_RIGHT;
				break;
			case LVCFMT_CENTER:
				nJustify = DT_CENTER;
				break;
			default:
				break;
			}
		}
	}
	
	pDC->DrawText(pszText, -1, rcLabel,	nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

	if (bSafeBackgroundRestore)
	{
		pDC->SetBkColor(crBackgroundRestore);
	}
}

void CListViewEx::DrawSkin(CDC* pDC, CRect rect, COLORREF color)
{
	COLORREF clrSafe;
	int iRectLine= rect.top;
	int iDecreseCount, iDecreseCountMax;
	pDC->FillSolidRect(rect.left+2,iRectLine++, rect.Width()-4, 1, color-RGB(20,20,20)); 
	pDC->FillSolidRect(rect.left+1,iRectLine++, rect.Width()-2, 1, color-RGB(10,10,10));
	clrSafe = color;
	iDecreseCount = 0;
	iDecreseCountMax = (rect.Height()/2);

	switch (m_iUseSkin)
	{
		case 2:
//			iDecreseCountMax -= 4;
			iDecreseCountMax = 10;
			for (; iRectLine < rect.bottom-2; iRectLine++)
			{
				pDC->FillSolidRect(rect.left,iRectLine, rect.Width(), 1, color);
				iDecreseCount++;
				if (iDecreseCount > iDecreseCountMax)
				{
				}
				else
				{
					color -= RGB(2,2,2);
				}
			}
			pDC->FillSolidRect(rect.left+1,iRectLine++, rect.Width()-2, 1, clrSafe-RGB(20,20,20));
			pDC->FillSolidRect(rect.left+2,iRectLine++, rect.Width()-4, 1, clrSafe-RGB(20,20,20));
		break;
		default:
			if (iDecreseCountMax > 10) iDecreseCountMax = 10;
			for (; iRectLine < rect.bottom-2; iRectLine++)
			{
				pDC->FillSolidRect(rect.left,iRectLine, rect.Width(), 1, color);
				iDecreseCount++;
			}
			pDC->FillSolidRect(rect.left+1,iRectLine++, rect.Width()-2, 1, clrSafe-RGB(10,10,10));
			pDC->FillSolidRect(rect.left+2,iRectLine++, rect.Width()-4, 1, clrSafe-RGB(20,20,20));
	}


}

LPCTSTR CListViewEx::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[] = _T("...");

	int nStringLen = lstrlen(lpszLong);

	if (nColumnLen < 1)
	{
		return "";
	}

	if(nStringLen == 0 ||
		(pDC->GetTextExtent(lpszLong, nStringLen).cx + nOffset) <= nColumnLen)
	{
		return(lpszLong);
	}

	static _TCHAR szShort[MAX_PATH];

	_tcsncpy_s(szShort,MAX_PATH,lpszLong,_TRUNCATE);
	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i = nStringLen-1; i > 0; i--)
	{
		szShort[i] = 0;
		if((pDC->GetTextExtent(szShort, i).cx + nOffset + nAddLen)
			<= nColumnLen)
		{
			break;
		}
	}

	_tcscat_s(szShort, MAX_PATH, szThreeDots);
	return(szShort);
}

void CListViewEx::RepaintSelectedItems()
{
	CListCtrl& ListCtrl = GetListCtrl();
	CRect rcItem, rcLabel;

// invalidate focused item so it can repaint properly

	int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);

	if(nItem != -1)
	{
		ListCtrl.GetItemRect(nItem, rcItem, LVIR_BOUNDS);
		ListCtrl.GetItemRect(nItem, rcLabel, LVIR_LABEL);
		rcItem.left = rcLabel.left;

		InvalidateRect(rcItem, FALSE);
	}

// if selected items should not be preserved, invalidate them

	if(!(GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
			nItem != -1; nItem = ListCtrl.GetNextItem(nItem, LVNI_SELECTED))
		{
			ListCtrl.GetItemRect(nItem, rcItem, LVIR_BOUNDS);
			ListCtrl.GetItemRect(nItem, rcLabel, LVIR_LABEL);
			rcItem.left = rcLabel.left;

			InvalidateRect(rcItem, FALSE);
		}
	}

// update changes

	UpdateWindow();
}

void CListViewEx::EndInPlaceEdit()
{
	CString sTxt;
	CListCtrl& ListCtrl=GetListCtrl();

	if (m_pInPlaceEdit != NULL)
	{
		sTxt = m_pInPlaceEdit->GetText();
		if (!m_pInPlaceEdit->m_bESC)
		{
			ListCtrl.SetItemText(m_pInPlaceEdit->m_iItem,m_pInPlaceEdit->m_iSubItem,sTxt);		// update text
			m_bWasEditSubLabel = true;
		}
		RemoveInplace();
	}
}

int CListViewEx::CellRectFromPoint(CPoint & point, RECT * cellrect, int * col) const
{
	CString sTxt;
	int colnum;

	CListCtrl& listCtrl = GetListCtrl();

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return -1;

	// Get the top and bottom row visible
	int row = listCtrl.GetTopIndex();
	int bottom = row + listCtrl.GetCountPerPage();
	if( bottom > listCtrl.GetItemCount() )
		bottom = listCtrl.GetItemCount();
	
	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		listCtrl.GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = listCtrl.GetColumnWidth(colnum);
				if( point.x >= rect.left 
					&& point.x <= (rect.left + colwidth ) )
				{
					RECT rectClient;
					GetClientRect( &rectClient );
					if( col ) *col = colnum;
					rect.right = rect.left + colwidth;

					// Make sure that the right extent does not exceed
					// the client area
					if( rect.right > rectClient.right ) 
						rect.right = rectClient.right;
					*cellrect = rect;

					// check if the text fits

					sTxt = listCtrl.GetItemText( row, colnum );

					RemoveSpecialText(&sTxt);
					int iWidthItem = listCtrl.GetStringWidth(sTxt);
					int iWidth = listCtrl.GetColumnWidth(colnum);
					iWidth -= 12;	// correct for margings

					if (iWidth < iWidthItem) return row;
					return -1;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

bool CListViewEx::RemoveSpecialText(CString *pTxt) const
{
	char	c;
	int		iPos;
	CString	sTxt;

	if (pTxt->GetLength() < 1) return false;
	c = pTxt->GetAt(0);


	switch(c)
	{
		case HIDE_SECOND:
			iPos = pTxt->Find(HIDE_CHAR, 1);
			if (iPos >=0)
			{
				sTxt = pTxt->Mid(1,iPos-1);
				sTxt+= "       ";	// correction because there is a check before it in the same column
				*pTxt = sTxt;
			}
		break;
		case COLOR_CHAR:
			iPos = pTxt->Find(HIDE_CHAR, 1);
			if (iPos >=0)
			{
				sTxt = pTxt->Mid(1,iPos-1);
				*pTxt = sTxt;
			}
		break;
		case PERC_CHAR:
			sTxt = pTxt->Mid(3);
			iPos = sTxt.Find(COLOR_CHAR, 1);
			if (iPos >=0)
			{
				*pTxt = sTxt.Mid(0,iPos-1);
			}
			else *pTxt = sTxt;
		break;
		case PASS_CHAR:
			*pTxt = "*****";			
		break;
		case HIDE_CHAR:
			iPos = pTxt->Find(HIDE_CHAR, 1);
			if (iPos >=0)
			{
				sTxt = pTxt->Mid(1,iPos-1);
				*pTxt = sTxt;
			}
		break;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// CListViewEx diagnostics

#ifdef _DEBUG

void CListViewEx::Dump(CDumpContext& dc) const
{
	CListViewMine::Dump(dc);

	dc << "m_bFullRowSel = " << (UINT)m_bFullRowSel;
	dc << "\n";
	dc << "m_cxStateImageOffset = " << m_cxStateImageOffset;
	dc << "\n";
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListViewEx message handlers


//#define TEST_BACKGROUND_ERASING 1

//#define TEST_BACKGROUND_ERASING 1

BOOL CListViewEx::OnEraseBkgnd(CDC *pDc)
{

//	CBrush  Brush;
	CRect rcWnd, rcWndCtrlFirst, rcWndCtrlLast;

#ifdef TEST_BACKGROUND_ERASING
	COLORREF clr = RGB(0,0,255);
	COLORREF clr2 = RGB(255,0,0);
	COLORREF clr3 = RGB(155,0,0);
#else
//	COLORREF clr = RGB(255,255,255);
//	COLORREF clr2 = RGB(255,0,0);
//	COLORREF clr3 = RGB(155,0,0);
	COLORREF clr = GetSysColor(COLOR_WINDOW);
	COLORREF clr2 = GetSysColor(COLOR_WINDOW);
	COLORREF clr3 = GetSysColor(COLOR_WINDOW);
#endif
	CListCtrl& listCtrl = GetListCtrl();

//	Brush.CreateSolidBrush(RGB(255,255,255));
	GetWindowRect(&rcWnd);
	int rcWndW = rcWnd.Width(), rcWndH = rcWnd.Height();

	int iItems = listCtrl.GetItemCount();

	if (!bFlickerFree)
	{
		iItems = 0;
	}

	if (iItems > 0)
	{
		listCtrl.GetItemRect(0, rcWndCtrlFirst, LVIR_BOUNDS);
		listCtrl.GetItemRect(iItems-1, rcWndCtrlLast, LVIR_BOUNDS);
		int iWndEraseW = rcWnd.Width();

		// 1e left, 2e top, 3e right, 4e bottom
		if (rcWndH > rcWndCtrlLast.bottom)
		{
			pDc->FillSolidRect(&CRect(0,rcWndCtrlLast.bottom, iWndEraseW,rcWndH), clr);	// rect below the list.
		}
		pDc->FillSolidRect(&CRect(0,0,rcWndCtrlFirst.right, rcWndCtrlFirst.top), clr2);					// rect top small between header and rows.

//		if (m_bUseSkin)
//		{
//			pDc->FillSolidRect(&CRect(0,0,4, rcWndH), clr);									// rect left of the list (small).
//			pDc->FillSolidRect(&CRect(rcWndCtrlLast.right,0, iWndEraseW,rcWndH), clr);	// rect right of the list.
//		}
//		else
//		{
//			pDc->FillSolidRect(&CRect(0,0,2, rcWndH), clr);									// rect left of the list (small).
			pDc->FillSolidRect(&CRect(rcWndCtrlLast.right,0, iWndEraseW,rcWndH), clr);		// rect right of the list.
//		}


		pDc->FillSolidRect(&CRect(rcWndCtrlLast.right,0,iWndEraseW, 32), clr3);			// Next to clr2, Top right of the listctrl, small area at the top
	}
	else
	{
		pDc->FillSolidRect(&CRect(0,0, rcWndW,rcWndH), clr);
//		pDc->FillSolidRect(&CRect(0,0,rcWndW, 22), clr2);				// rect top small between header and rows.
	}

//	Brush.DeleteObject();

	return TRUE;	// no furhter action needed xxxxxxxxxxxxxxxx
}

INT_PTR CListViewEx::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	int row, col;
	RECT cellrect;
	
	row = CellRectFromPoint(point, &cellrect, &col );

	if ( row == -1 ) 
		return -1;

	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT)((row<<10)+(col&0x3ff)+1);
	pTI->lpszText = LPSTR_TEXTCALLBACK;

	pTI->rect = cellrect;

	return pTI->uId;
}

BOOL CListViewEx::OnToolNeedText(UINT iValue,NMHDR *pNMHDR,LRESULT *pResult)	// #$# $#$
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT_PTR nID = pNMHDR->idFrom;

	if( nID == 0 )	  	// Notification in NT from automatically
		return FALSE;   	// created tooltip

	CListCtrl& listCtrl = GetListCtrl();

	int row = ((nID-1) >> 10) & 0x3fffff ;
	int col = (nID-1) & 0x3ff;
	strTipText = listCtrl.GetItemText( row, col );
	RemoveSpecialText(&strTipText);

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		lstrcpyn(m_cToolTipLongerBuffer, strTipText, 255);
		pTTTA->lpszText = &m_cToolTipLongerBuffer[0];
		pTTTA->szText[0] = 0;
	}
	else
	{
		_mbstowcsz(m_wToolTipLongerBuffer, strTipText, 255);
		pTTTW->lpszText = m_wToolTipLongerBuffer;
		pTTTW->szText[0] = 0;
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, 80);
	else
		lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
	*pResult = 0;

	return TRUE;    // message was handled

}


void CListViewEx::OnMove( int x, int y)
{
	CListViewMine::OnMove(x,y);
}



LRESULT CListViewEx::OnSetImageList(WPARAM wParam, LPARAM lParam)
{
	// if we're running Windows 4, there's no need to offset the
	// item text location

	OSVERSIONINFO info;
	info.dwOSVersionInfoSize = sizeof(info);
	VERIFY(::GetVersionEx(&info));

	if( (int) wParam == LVSIL_STATE)// && info.dwMajorVersion < 4)	// happens all of a sudden in W7 as well.
	{
		int cx, cy;

		if(::ImageList_GetIconSize((HIMAGELIST)lParam, &cx, &cy))
			m_cxStateImageOffset = cx;
		else
			m_cxStateImageOffset = 0;
	}

	return(Default());
}

LRESULT CListViewEx::OnSetTextColor(WPARAM /*wParam*/, LPARAM lParam)
{
	m_clrText = (COLORREF)lParam;
	return(Default());
}

LRESULT CListViewEx::OnSetTextBkColor(WPARAM /*wParam*/, LPARAM lParam)
{
	m_clrTextBk = (COLORREF)lParam;
	return(Default());
}

LRESULT CListViewEx::OnSetBkColor(WPARAM /*wParam*/, LPARAM lParam)
{
	m_clrBkgnd = (COLORREF)lParam;
	return(Default());
}

void CListViewEx::OnSize(UINT nType, int cx, int cy)
{
//	m_cxClient = cx;
	CListViewMine::OnSize(nType, cx, cy);

//	::PostMessage(HWND_BROADCAST,WM_MSG_NEED_ERASE_BACKGROUND,0);

//	m_bEraseBackgroundNeeded = true;
}

void CListViewEx::OnPaint()
{
//	ModifyStyle(0,LVS_EX_DOUBLEBUFFER,0);

	// in full row select mode, we need to extend the clipping region
	// so we can paint a selection all the way to the right
	if (m_bClientWidthSel && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT && GetFullRowSel())
	{
		CRect rcAllLabels;
		GetListCtrl().GetItemRect(0, rcAllLabels, LVIR_BOUNDS);

		if(rcAllLabels.right < m_cxClient)
		{
			int i = 1;
						// need to call BeginPaint (in CPaintDC c-tor)
			// to get correct clipping rect

			if (m_bSelection)
			{
				CPaintDC dc(this);

				CRect rcClip;
				dc.GetClipBox(rcClip);

				rcClip.left = std::min(rcAllLabels.right-1, rcClip.left);
				rcClip.right = m_cxClient;

				InvalidateRect(rcClip, FALSE);
			}
			// EndPaint will be called in CPaintDC d-tor
		}
	}

	CListViewMine::OnPaint();
}

void CListViewEx::OnSetFocus(CWnd* pOldWnd)
{
	CListViewMine::OnSetFocus(pOldWnd);

	// check if we are getting focus from label edit box
	if(pOldWnd!=NULL && pOldWnd->GetParent()==this)
		return;

	// repaint items that should change appearance
	if(m_bFullRowSel && (GetStyle() & LVS_TYPEMASK)==LVS_REPORT) RepaintSelectedItems();
}

void CListViewEx::OnKillFocus(CWnd* pNewWnd)
{
	CListViewMine::OnKillFocus(pNewWnd);

	// check if we are losing focus to label edit box
	if(pNewWnd != NULL && pNewWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if(m_bFullRowSel && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)	RepaintSelectedItems();
}

/////////// inplace text edit

int CListViewEx::HitTestEx(CListCtrl *pListCtrl, CPoint point, int *col) const
{
	int colnum = 0;
	HWND hWnd;
	hWnd = pListCtrl->m_hWnd;
	
	int row = pListCtrl->HitTest( point, NULL );

	if( col ) *col = 0;

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return row;

	// Get the top and bottom row visible
	row = pListCtrl->GetTopIndex();
	int bottom = row +  pListCtrl->GetCountPerPage();
	if( bottom >  pListCtrl->GetItemCount() )
		bottom =  pListCtrl->GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		 pListCtrl->GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth =  pListCtrl->GetColumnWidth(colnum);
				if( point.x >= rect.left
					&& point.x <= (rect.left + colwidth ) )
				{
					if( col ) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

// EditSubLabel		- Start edit of a sub item label
// Returns		- Temporary pointer to the new edit control
// nItem		- The row index of the item to edit
// nCol			- The column of the sub item.
CEdit* CListViewEx::EditSubLabel(CListCtrl *pListCtrl, int nItem, int nCol )
{
	// The returned pointer should not be saved

	// Make sure that the item is visible
	if( !pListCtrl->EnsureVisible( nItem, TRUE ) ) return NULL;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || pListCtrl->GetColumnWidth(nCol) < 5 )
		return NULL;

	// Get the column offset
	int offset = 0;
	for( int i = 0; i < nCol; i++ )
		offset += pListCtrl->GetColumnWidth( i );

	CRect rect;
	pListCtrl->GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		pListCtrl->Scroll( size );
		rect.left -= size.cx;
	}

	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	pListCtrl->GetColumn( nCol, &lvcol );
	DWORD dwStyle ;
	if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;

	rect.left += offset+4;
	rect.right = rect.left + pListCtrl->GetColumnWidth( nCol ) - 3 ;
	if( rect.right > rcClient.right) rect.right = rcClient.right;

	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL;

	if (m_pInPlaceEdit == NULL)
	{
		m_pInPlaceEdit = new CInPlaceEdit(this, nItem, nCol, pListCtrl->GetItemText( nItem, nCol ));
		m_pInPlaceEdit->Create(dwStyle, rect, this, IDC_IPEDIT );
		m_pInPlaceEdit->m_pView = this;
	}

	return m_pInPlaceEdit;
}

//void CListViewEx::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//}

void CListViewEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	int	colnum;
	UINT uFlags = 0;
	int index;
	CString sTxt;
	
	if (m_bInPlaceEdit)
	{
		CListCtrl& listCtrl = GetListCtrl();

		// click again....	
		if (m_pInPlaceEdit != NULL)
		{
			sTxt = m_pInPlaceEdit->GetText();
			if (!m_pInPlaceEdit->m_bESC)
			{
				listCtrl.SetItemText(m_pInPlaceEdit->m_iItem,m_pInPlaceEdit->m_iSubItem,sTxt);		// update text
				m_bWasEditSubLabel = true;
			}
			RemoveInplace();
		}

		if( ( index = 	HitTestEx(&listCtrl, point, &colnum )) != -1 )
		{
//			if (colnum != 0)
			{
				UINT flag = LVIS_SELECTED;
				if( (listCtrl.GetItemState( index, flag ) & flag) == flag && colnum >= 0)
				{
					// Add check for LVS_EDITLABELS
//					if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
					{
						EditSubLabel(&listCtrl, index, colnum );
						return;	// don't tell the rest
					}
				}
				else
				{
					listCtrl.SetItemState( index, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVS_EDITLABELS); 
				}
			}
		}
	}
	Invalidate(FALSE);		// redraw the selection again.
	CListViewMine::OnLButtonDown(nFlags, point);
}

LRESULT CListViewEx::OnInPlaceReady(WPARAM wParam, LPARAM lParam)
{
	CString *psTxt;

	psTxt = (CString *) lParam;

//	delete m_pInPlaceEdit;
//	m_pInPlaceEdit = NULL;

	return 0;
}

 void CListViewEx::RemoveInplace()
 {
	 if (m_pInPlaceEdit != NULL)
	 {
		delete m_pInPlaceEdit;
		m_pInPlaceEdit = NULL;
	 }
 }


void CListViewEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString sTxt;
	CListCtrl& ListCtrl=GetListCtrl();

	if (m_pInPlaceEdit != NULL)
	{
		if(nChar == VK_RETURN || nChar == VK_TAB)
		{
			EndInPlaceEdit();
		}
		if(nChar == VK_ESCAPE)
		{
			RemoveInplace();
		}
	}
}

LRESULT CListViewEx::OnSetFont(WPARAM wParam, LPARAM lParam)
{
	LRESULT res =  Default();

	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

	return res;
}

BOOL CListViewEx::OnDividerdblclick(UINT i, NMHDR *pNMHDR,LRESULT *pResult)
{
	CString sTxt;
	int		iLen, iLenMax;

	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

	CListCtrl& ListCtrl=GetListCtrl();
	int iItems = ListCtrl.GetItemCount();
	int iColumn = phdn->iItem;
	iLenMax = 0;

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		sTxt = ListCtrl.GetItemText(iCount, iColumn);
		RemoveSpecialText(&sTxt);
		iLen = ListCtrl.GetStringWidth(sTxt);
		if (iLen > iLenMax) iLenMax = iLen;
	}
	if (iColumn == 0) iLenMax += 14;
	else iLenMax +=12;	// margins
	ListCtrl.SetColumnWidth(iColumn,iLenMax);

	*pResult = 0;

	return TRUE;
}


void CListViewEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,1);
		break;
	}

	CListViewMine::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CListViewEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,0);
		break;
	}

	CListViewMine::OnKeyUp(nChar, nRepCnt, nFlags);
}

// ListCtrlFloater.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "listviewex.h"
#include "TemplateRemoteHistoryView.h"
#include "DlgColorGeneral.h"
#include "ListCtrlHistory.h"
#include "listviewex.h"
#include "DlgSettingsView.h"
#include "Selection.h"
#include "SortColumn.h"
#include "ThreadRpc.h"
#include "SortHistory.h"

// CListCtrlHistory

IMPLEMENT_DYNAMIC(CListCtrlHistory, CListCtrl)

CListCtrlHistory::CListCtrlHistory()
{
	m_bUseSkin = false;
	m_iUseSkin = 0;
	m_bUseHorizontalGrid = false;
	m_bUseVerticalGrid = false;

	m_pSelection = new CSelection;
	m_pSelection->Setup(this, &m_lListData, COLUMN_HISTORY_NAME, COLUMN_HISTORY_COMPUTER, NUM_REMOTE_HISTORY_COLUMNS, NULL);
	m_pSortColumn = new CSortColumn;

	m_pFont = new CFont;
}

CListCtrlHistory::~CListCtrlHistory()
{
	for (int iPos = 0; iPos < (int) m_lListData.size(); iPos++)
	{
		delete m_lListData.at(iPos);
	}
	delete m_pSortColumn;
	delete m_pSelection;
	delete m_pFont;
}

void CListCtrlHistory::PreSubclassWindow()
{
  //use our custom CHeaderCtrl
 // m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);

	CListCtrl::PreSubclassWindow();
	EnableToolTips(TRUE);
}

void CListCtrlHistory::MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
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

void CListCtrlHistory::DrawSkin(CDC* pDC, CRect rect, COLORREF color)
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
			iDecreseCountMax -= 4;
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


void CListCtrlHistory::ProcessColumn(LPCTSTR pszBuffer,  CDC* pDC, CRect rcItem, COLORREF	*pColorsGeneral, LV_COLUMN *pLvc)
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
				}
				
				crBackground = *(pColorsGeneral + COLOR_GENERAL_ARRAY_BAR);

				if (m_bUseSkin)	DrawSkin(pDC, rectPerc, crBackground);
				else pDC->FillSolidRect(rectPerc, crBackground);
				pDC->FrameRect(rectPercOutline,&brPerc);
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

LPCTSTR CListCtrlHistory::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
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

int CListCtrlHistory::CellRectFromPoint(CPoint & point, RECT * cellrect, int * col) const
{
	CString sTxt;
	int colnum;

//	CListCtrl& listCtrl = GetListCtrl();

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return -1;

	// Get the top and bottom row visible
	int row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )
		bottom = GetItemCount();
	
	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
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

					sTxt = GetItemText( row, colnum );

//					RemoveSpecialText(&sTxt);
					int iWidthItem = GetStringWidth(sTxt);
					int iWidth = GetColumnWidth(colnum);
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

void CListCtrlHistory::Sort()
{
	int iListCount;
	UINT iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;
	iListCount = GetItemCount();
	
	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);

	if (iListCount > 1)
	{
		DWORD iSorting = (iSortingColumnPrimary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionPrimary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnSecondary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionSecondary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnTertiary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionTertiary&0xf);
	
		int iTopItem = GetTopIndex();

		SortItems(HistorySorting,iSorting);
		EnsureVisible(iTopItem, FALSE);			// otherwise, there is a scroll to the botton om the right mouse button.
	}
	DisplayMakeLineColors(theApp.m_pDlgSettingsView->m_bAlternatingStriped, theApp.m_pDlgSettingsView->m_bUseSkin, theApp.m_pDlgSettingsView->m_iUseSkin);
}

void CListCtrlHistory::DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin)
{
	int			iItemCount;
	bool		bMax;
	CListData	*pData;

	m_bUseSkin = bUseSkin;
	m_iUseSkin = iUseSkin;
	if (!bUseAlternatingLines) return;

//	CListCtrl& listCtrl = GetListCtrl();
	iItemCount = GetItemCount();

	for (int iCount = 1; iCount < iItemCount; iCount+=2)
	{
		pData = (CListData*) GetItemData(iCount);

		if (bUseSkin)
		{
			bMax = false;
			if ((pData->m_color&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>8)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>16)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
	
			if (bMax) 	pData->m_color -= RGB(COLOR_OFFSET_MIN,COLOR_OFFSET_MIN,COLOR_OFFSET_MIN);
			else pData->m_color -= RGB(COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING);
		}
		else
		{
			pData->m_color -= RGB(20,20,20);
		}
	}
}

BEGIN_MESSAGE_MAP(CListCtrlHistory, CListCtrl)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, &CListCtrlHistory::OnNMCustomdrawList)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnFontHasChanged)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT( )

    ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
    ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
END_MESSAGE_MAP()



// CListCtrlHistory message handlers

BOOL CListCtrlHistory::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
	{
		int ii =1;
	}


	return CListCtrl::PreTranslateMessage(pMsg);
}


INT_PTR CListCtrlHistory::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
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

BOOL CListCtrlHistory::OnToolNeedText(UINT iValue,NMHDR *pNMHDR,LRESULT *pResult)	// #$# $#$
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT_PTR nID = pNMHDR->idFrom;

	if( nID == 0 )	  	// Notification in NT from automatically
		return FALSE;   	// created tooltip

//	CListCtrl& listCtrl = GetListCtrl();

	int row = ((nID-1) >> 10) & 0x3fffff ;
	int col = (nID-1) & 0x3ff;
	strTipText = GetItemText( row, col );
//	RemoveSpecialText(&strTipText);

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		lstrcpyn(m_cToolTipLongerBuffer, strTipText, 255);
		pTTTA->lpszText = &m_cToolTipLongerBuffer[0];
		pTTTA->szText[0] = 0;
//		lstrcpyn(pTTTA->szText, strTipText, 80);
	}
	else
	{
		_mbstowcsz(m_wToolTipLongerBuffer, strTipText, 255);
		pTTTW->lpszText = m_wToolTipLongerBuffer;
		pTTTW->szText[0] = 0;
//		_mbstowcsz(pTTTW->szText, strTipText, 80);
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



void CListCtrlHistory::OnMouseMove( UINT nFlags, CPoint point)
{
	if( nFlags != 0)
	{
		int ii =1;
	}

	if (nFlags & MK_LBUTTON  )
	{
//		this->ShowWindow(SW_HIDE);
	}
	else
	{

	}
	CListCtrl::OnMouseMove(nFlags,point);
}

void CListCtrlHistory::OnLButtonDown(UINT nFlags,CPoint point)
{
	int row = HitTest( point, NULL );
	m_pSelection->SelectionSet(row);
	Invalidate(FALSE);
	this->SetFocus();
}

void CListCtrlHistory::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (nChar == VK_END)
	{
		m_pSelection->SelectionEnd(this);		
	}
	if (nChar == VK_HOME)
	{
		m_pSelection->SelectionBegin(this);		
	}

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags );
}

BOOL CListCtrlHistory::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListData	*pData;
	COLORREF	*pColorsGeneral;
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	int iRow = (int) lplvcd->nmcd.dwItemSpec;
	int iCol = lplvcd->iSubItem;
	*pResult = 0;

	pColorsGeneral = theApp.m_pDlgColorGeneral->GetColorArray();
	pData = (CListData *) lplvcd->nmcd.lItemlParam;

	switch(lplvcd->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		break;

		case CDDS_ITEMPREPAINT:
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			break;
		case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
			{
				COLORREF clrBack = RGB(200,200,200);////0xFFFFFFFF;
				COLORREF clrText = 0xFFFFFFFF;
				
				*pResult = CDRF_DODEFAULT;

//				CellInfo *pCell = GetCellInfo(iRow, iCol);
//				if(pCell)
//				{
//					clrBack = pCell->m_clrBack;
//					clrText = pCell->m_clrText;
//				}
//				if(clrBack == 0xFFFFFFFF && clrText == 0xFFFFFFFF)
//				{
//					ItemData* pData = (ItemData*)GetItemDataInternal(iRow);
//					if(pData)
//					{
//						int ii = 1;
//						clrBack = pData->m_clrBack;
//						clrText = pData->m_clrText;
//					}
//				}
//				if(clrBack == 0xFFFFFFFF && clrText == 0xFFFFFFFF)
//				{
//					ColumnInfo *pInfo = GetColumnInfo(iCol);
//					if(pInfo)
//					{						
//						clrBack = pInfo->m_clrBack;
//						clrText = pInfo->m_clrText;
//					}
//				}
//				if(clrBack != 0xFFFFFFFF)
				{
					if (pData->m_bSelected)
					{
						lplvcd->clrTextBk = (COLORREF) *(pColorsGeneral + COLOR_GENERAL_ARRAY_BACK_SELECTED);
					}
					else
					{
						lplvcd->clrTextBk = (COLORREF) pData->m_color;
					}
					*pResult  = CDRF_NEWFONT;
				}
//				else
//				{
//					if(clrBack != m_clrDefBack)
//					{
//						lplvcd->clrTextBk = m_clrDefBack;				
//						*pResult  = CDRF_NEWFONT;
//					}
//				}
//				if(clrText != 0xFFFFFFFF)
				{			
					if (pData->m_bSelected)
					{
						*(pColorsGeneral + COLOR_GENERAL_ARRAY_FONT_SELECTED);
					}
					else
					{
						lplvcd->clrText = clrText;
					}
					*pResult  = CDRF_NEWFONT;
				}				
//				else
//				{
//					if(clrText != m_clrDefText)
//					{
//						lplvcd->clrText = m_clrDefText;		
//						*pResult  = CDRF_NEWFONT;
//					}
//				}
			}
		break;

		default:
		*pResult = CDRF_DODEFAULT;
	}
	if(*pResult == 0 || *pResult == CDRF_DODEFAULT)	return FALSE;
	

	return TRUE;
}


// offsets for first and other columns


void CListCtrlHistory::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
//	CListCtrl& ListCtrl=GetListCtrl();
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	CPen penBorder;
	UINT uiFlags = ILD_TRANSPARENT;
//	CImageList* pImageList;
	int nItem = lpDrawItemStruct->itemID;
	BOOL bFocus = (GetFocus() == this);
	BOOL bSelected = FALSE;
	COLORREF clrTextSave = 0;
	COLORREF clrBkSave = 0;
	COLORREF clrLine;
//	COLORREF clrImage = m_clrBkgnd;

	static _TCHAR szBuff[MAX_PATH];
	COLORREF	*pColorsGeneral;
	
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
	this->GetItem(&lvi);

	CListData	*pListData;
	pListData = (CListData *) lvi.lParam;

	clrLine = (COLORREF) lvi.lParam;

	pColorsGeneral = theApp.m_pDlgColorGeneral->GetColorArray();

	penBorder.CreatePen(PS_SOLID,1,*(pColorsGeneral + COLOR_GENERAL_ARRAY_GRID));

	if (pListData)
	{
		bSelected = pListData->m_bSelected;
	}
	else bSelected = false;

// set colors if item is selected

	CRect rcAllLabels;
	this->GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);

	CRect rcLabel;
	this->GetItemRect(nItem, rcLabel, LVIR_LABEL);

	rcAllLabels.left = rcLabel.left;
//	if (m_bClientWidthSel && rcAllLabels.right< this->m_cxClient)	rcAllLabels.right = m_cxClient;

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

// draw item label

	this->GetItemRect(nItem, rcItem, LVIR_LABEL);
//	rcItem.right -= m_cxStateImageOffset;

//	nJustify = DT_LEFT;
	ProcessColumn(szBuff, pDC, rcItem, pColorsGeneral, NULL);

// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for(int nColumn = 1; this->GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;

		
		// draw vertical lines
		if (m_bUseVerticalGrid)
		{
			pDC->SelectObject(&penBorder);
			pDC->MoveTo(rcItem.left-1, rcItem.top);
			pDC->LineTo(rcItem.left-1, rcItem.bottom);
		}
		int nRetLen = this->GetItemText(nItem, nColumn,	szBuff, sizeof(szBuff));
		if (nRetLen == 0) continue;
		ProcessColumn(szBuff, pDC, rcItem, pColorsGeneral, &lvc);
	}

// draw focus rectangle if item has focus

//	if (m_bSelection)
//	{
//		if (lvi.state & LVIS_FOCUSED && bFocus) pDC->DrawFocusRect(rcAllLabels);
//	}

	
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
}

LRESULT CListCtrlHistory::OnSetFont(WPARAM wParam, LPARAM lParam)
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

LRESULT CListCtrlHistory::OnFontHasChanged(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLf;
	pLf = (LOGFONT *) wParam;

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLf);
	SetFont(m_pFont,TRUE);

	return 0;
}


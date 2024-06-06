// ListCtrlFloater.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
//#include "SkinHeader.h"
#include "DlgColorGeneral.h"
#include "ListCtrlFloater.h"
#include "listviewex.h"

// CListCtrlFloater

IMPLEMENT_DYNAMIC(CListCtrlFloater, CListCtrl)

CListCtrlFloater::CListCtrlFloater()
{
	m_bUseSkin = true;
}

CListCtrlFloater::~CListCtrlFloater()
{
}

void CListCtrlFloater::PreSubclassWindow()
{
  //use our custom CHeaderCtrl
  m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);

  CListCtrl::PreSubclassWindow();
}

void CListCtrlFloater::DrawSkin(CDC* pDC, CRect rect, COLORREF color)
{
	COLORREF clrSafe;
	int iRectLine= rect.top;
	pDC->FillSolidRect(rect.left+2,iRectLine++, rect.Width()-4, 1, color-RGB(20,20,20)); 
	pDC->FillSolidRect(rect.left+1,iRectLine++, rect.Width()-2, 1, color-RGB(10,10,10));
	clrSafe = color;
	int iDecreseCount = 0;
	int	iDecreseCountMax = rect.Height()/2;
	if (iDecreseCountMax > 10) iDecreseCountMax = 10;
	for (; iRectLine < rect.bottom-2; iRectLine++)
	{
		pDC->FillSolidRect(rect.left,iRectLine, rect.Width(), 1, color);
		iDecreseCount++;
		if (iDecreseCount > iDecreseCountMax)
		{
			if (iDecreseCount < iDecreseCountMax*2) 
			{
				color += RGB(2,2,2);
			}
		}
		else
		{
			color -= RGB(2,2,2);
		}
	}
	pDC->FillSolidRect(rect.left+1,iRectLine++, rect.Width()-2, 1, clrSafe-RGB(10,10,10));
	pDC->FillSolidRect(rect.left+2,iRectLine++, rect.Width()-4, 1, clrSafe-RGB(20,20,20));
}

void CListCtrlFloater::ProcessColumn(LPCTSTR pszBuffer,  CDC* pDC, CRect rcItem, COLORREF	*pColorsGeneral, LV_COLUMN *pLvc)
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
//					pDC->FillSolidRect(rectCellColor,iColor);
				}
				
				crBackground = *(pColorsGeneral + COLOR_GENERAL_ARRAY_BAR);

				if (m_bUseSkin)	DrawSkin(pDC, rectPerc, crBackground);
				else pDC->FillSolidRect(rectPerc, crBackground);
//				pDC->FillSolidRect(rectPerc, crBackground);
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

LPCTSTR CListCtrlFloater::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
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


BEGIN_MESSAGE_MAP(CListCtrlFloater, CListCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, &CListCtrlFloater::OnNMCustomdrawListFloating)
END_MESSAGE_MAP()



// CListCtrlFloater message handlers


void CListCtrlFloater::OnMouseMove( UINT nFlags, CPoint point)
{
	if( nFlags != 0)
	{
		int ii =1;
	}

	if (nFlags & MK_LBUTTON  )
	{
		this->ShowWindow(SW_HIDE);
	}
	else
	{

	}
	CListCtrl::OnMouseMove(nFlags,point);
}

void CListCtrlFloater::OnLButtonDown(UINT nFlags,CPoint point)
{
	this->ShowWindow(SW_HIDE);
}

BOOL CListCtrlFloater::OnNMCustomdrawListFloating(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	int iRow = (int) lplvcd->nmcd.dwItemSpec;
	int iCol = lplvcd->iSubItem;
	*pResult = 0;
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
				lplvcd->clrTextBk = (COLORREF) lplvcd->nmcd.lItemlParam;					
				lplvcd->clrText = clrText;					
				*pResult  = CDRF_NEWFONT;
			}
		break;

		default:
		*pResult = CDRF_DODEFAULT;
	}
	if(*pResult == 0 || *pResult == CDRF_DODEFAULT)	return FALSE;
	

	return TRUE;
}


// offsets for first and other columns


void CListCtrlFloater::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	CPen penBorder;
	UINT uiFlags = ILD_TRANSPARENT;
	int nItem = lpDrawItemStruct->itemID;
	BOOL bFocus = (GetFocus() == this);
	COLORREF clrTextSave = 0;
	COLORREF clrBkSave = 0;
	COLORREF clrLine;

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

	clrLine = (COLORREF) lvi.lParam;

	pColorsGeneral = theApp.m_pDlgColorGeneral->GetColorArray();

	penBorder.CreatePen(PS_SOLID,1,*(pColorsGeneral + COLOR_GENERAL_ARRAY_GRID));

// set colors if item is selected

	CRect rcAllLabels;
	this->GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);

	CRect rcLabel;
	this->GetItemRect(nItem, rcLabel, LVIR_LABEL);

	rcAllLabels.left = rcLabel.left;


	clrTextSave = pDC->SetTextColor(*(pColorsGeneral + COLOR_GENERAL_ARRAY_FONT));
	clrBkSave = pDC->GetBkColor();
	if (m_bUseSkin)	DrawSkin(pDC, rcAllLabels, clrLine);
	else pDC->FillSolidRect(rcAllLabels, clrLine);

// draw item label

	this->GetItemRect(nItem, rcItem, LVIR_LABEL);

	ProcessColumn(szBuff, pDC, rcItem, pColorsGeneral, NULL);

// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for(int nColumn = 1; this->GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;

		int nRetLen = this->GetItemText(nItem, nColumn,	szBuff, sizeof(szBuff));
		if (nRetLen == 0) continue;
		ProcessColumn(szBuff, pDC, rcItem, pColorsGeneral, &lvc);
	}

	pDC->SetTextColor(clrTextSave);
	pDC->SetBkColor(clrBkSave);

	penBorder.DeleteObject();
}
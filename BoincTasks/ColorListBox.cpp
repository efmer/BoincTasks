// ColorListBox.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
#include "ColorListBox.h"
#include "DlgNotices.h"


// CColorListBox

IMPLEMENT_DYNAMIC(CColorListBox, CListBox)

CColorListBox::CColorListBox(bool bColor)
{
	m_hWndMessagesTo = NULL;
	m_bColor = bColor;
	Reset();
	m_bMultipleSelection = true;
	m_iLastItem = -1;
}

CColorListBox::~CColorListBox()
{
}

void CColorListBox::Reset()
{
	if (m_bColor)
	{
		for (int iCount = 0; iCount < MAX_COLOR_ARRAY_G_NUMBER_COMPUTERS; iCount++)
		{
			m_color[iCount] = RGB(255,255,255);
		}
	}
}

void CColorListBox::MessagesTo(HWND hWnd)
{
	m_hWndMessagesTo = hWnd;
}

BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_MESSAGE(UWM_MSG_SELECTION_MODE, OnSelectionMode)
END_MESSAGE_MAP()

// CColorListBox message handlers

void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemHeight = ::GetSystemMetrics(SM_CYMENUCHECK);
}

int CColorListBox::AddString(LPCTSTR lpszItem)
{
	return ((CListBox*)this)->AddString(lpszItem);
}

int CColorListBox::AddString(LPCTSTR lpszItem, int iColorIndex)
{
	int iItem = AddString(lpszItem);
	if (iItem >= 0) SetItemData(iItem, iColorIndex);
	return iItem;
}

int CColorListBox::AddString(int iIndex, LPCTSTR lpszItem, int iColorIndex)
{
	InsertString(iIndex, lpszItem);
	SetItemData(iIndex, iColorIndex);
	return 0;
}

void CColorListBox::SetColor(COLORREF color, int iRepeat, int iIndex, int iColorPos)
{
	if (iIndex > (MAX_COLOR_ARRAY_G_NUMBER_COMPUTERS+1)) return;
	m_color[iColorPos] = color;
	m_iRepeat[iColorPos] = iRepeat;
	SetItemData(iIndex, iColorPos);
}


COLORREF CColorListBox::GetColor(int iPos, int *piRepeat)
{
	if (piRepeat) *piRepeat = 0;

	if (m_bColor)
	{
		if (iPos > (MAX_COLOR_ARRAY_G_NUMBER_COMPUTERS+1)) return 0;
		if (iPos < 0) return 0;
		if (piRepeat) *piRepeat = m_iRepeat[iPos];
		return m_color[iPos];
	}
	return 0;
}

void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if ((int)lpDrawItemStruct->itemID < 0)	return; 

	CString sText;

	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	CRect rectColor;
	CRect rectItem = &lpDrawItemStruct->rcItem;
	CRect rectText = rectItem;
	rectText.SetRect(COLOR_BOX_COLOR_WIDTH+1,rectItem.TopLeft().y,rectItem.BottomRight().x,rectItem.BottomRight().y);	
	rectColor.SetRect(0,rectItem.TopLeft().y, COLOR_BOX_COLOR_WIDTH, rectItem.BottomRight().y);

	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		CRect rect = rectItem;
		CRect highRect;

		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		int	iIndex = (int)lpDrawItemStruct->itemData;				// Color index information is in item data.
		if (m_bColor)
		{
			COLORREF color = GetColor(iIndex);
			dc.FillSolidRect(rectColor,color);
		}

		highRect.SetRect(COLOR_BOX_COLOR_WIDTH,rect.TopLeft().y,rect.BottomRight().x,rect.BottomRight().y);
		dc.FillSolidRect(highRect, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);

	GetText(lpDrawItemStruct->itemID, sText);
   // Draw the text.

   dc.DrawText(sText, sText.GetLength(),&rectText,DT_LEFT|DT_SINGLELINE);

	if (!m_bColor)
	{
		CPoint	points[6];
//		CRect rect;
//		GetClientRect(rect);

		int	iData = (int)lpDrawItemStruct->itemData;

		if (iData != NOTICES_SORT_OPERATOR_NO)
		{
			if (iData == NOTICES_SORT_OPERATOR_UP)
			{
				points[0].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.TopLeft().y+COLOR_BOX_COLOR_MARGIN_UD);
				points[1].SetPoint(0+COLOR_BOX_COLOR_MARGIN_LR, rectColor.BottomRight().y-COLOR_BOX_COLOR_MARGIN_UD);
				points[2].SetPoint(COLOR_BOX_COLOR_WIDTH-COLOR_BOX_COLOR_MARGIN_LR, rectColor.BottomRight().y-COLOR_BOX_COLOR_MARGIN_UD);
				points[3].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.TopLeft().y+COLOR_BOX_COLOR_MARGIN_UD);

				points[4].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.BottomRight().y-COLOR_BOX_COLOR_MARGIN_UD);
				points[5].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.TopLeft().y+COLOR_BOX_COLOR_MARGIN_UD);
			}
			else
			{
				points[0].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.BottomRight().y-COLOR_BOX_COLOR_MARGIN_UD);
				points[1].SetPoint(0+COLOR_BOX_COLOR_MARGIN_LR, rectColor.TopLeft().y+COLOR_BOX_COLOR_MARGIN_UD);
				points[2].SetPoint(COLOR_BOX_COLOR_WIDTH-COLOR_BOX_COLOR_MARGIN_LR, rectColor.TopLeft().y+COLOR_BOX_COLOR_MARGIN_UD);
				points[3].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.BottomRight().y-COLOR_BOX_COLOR_MARGIN_UD);

				points[4].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.TopLeft().y+COLOR_BOX_COLOR_MARGIN_UD);
				points[5].SetPoint(COLOR_BOX_COLOR_WIDTH/2, rectColor.BottomRight().y-COLOR_BOX_COLOR_MARGIN_UD);
			}

			CRgn   rgn;
			VERIFY(rgn.CreatePolygonRgn(points, 6, POLYFILL_LAST));
	
			CRect rectRgnBox;
			int nRgnBoxResult = rgn.GetRgnBox( &rectRgnBox );
			ASSERT( nRgnBoxResult != ERROR && nRgnBoxResult != NULLREGION );
			CBrush br;
			VERIFY(br.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT)));
			VERIFY(dc.FrameRgn( &rgn, &br, 2, 2 ));
			DeleteObject(rgn);
		}

	//	dc.MoveTo(points[0]);
	//	dc.LineTo(points[1]);
	//	dc.LineTo(points[2]);
	//	dc.LineTo(points[3]);

	}

   // Reset the background color and the text color back to their
   // original values.
   dc.SetTextColor(crOldTextColor);
   dc.SetBkColor(crOldBkColor);

   dc.Detach();
}

void CColorListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iItem, iFirst, iLast;
	BOOL	bOutside;

	SHORT iControlKey, iShiftKey;
	if (m_bColor)
	{
		if (m_bMultipleSelection)
		{
			return CListBox::OnLButtonDown(nFlags, point);
		}
		else
		{
			iItem = ItemFromPoint(point, bOutside);
			if (bOutside) return;

			iControlKey = GetKeyState(VK_CONTROL);
			iControlKey &= 0x8000;

			iShiftKey = GetKeyState(VK_SHIFT);
			iShiftKey &= 0x8000;
			if (iShiftKey && m_iLastItem != -1)
			{
				if (!iControlKey)
				{
					SelItemRange(FALSE, 0, GetCount());
				}

				if (m_iLastItem > iItem)
				{
					iFirst = iItem;
					iLast = m_iLastItem;
				}
				else
				{
					iFirst = m_iLastItem;
					iLast = iItem;
				}
				SelItemRange(TRUE, iFirst, iLast);
				m_iLastItem = iItem;
				GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE),(LPARAM)CListBox::m_hWnd);	// ON_LBN_SELCHANGE
				return;
			}
			m_iLastItem = iItem;
			if (!iControlKey)
			{
				SelItemRange(FALSE, 0, GetCount());
			}
			return CListBox::OnLButtonDown(nFlags, point);
		}
	}

	iItem = ItemFromPoint(point, bOutside);

	if (bOutside) return;

	if (point.x > COLOR_BOX_COLOR_WIDTH)
	{
		SetCurSel(iItem);
	}
	else
	{
		DWORD dwData = (DWORD) GetItemData(iItem);

		switch (dwData)
		{
		case NOTICES_SORT_OPERATOR_UP: 
			dwData = NOTICES_SORT_OPERATOR_DOWN;
		break;
		case NOTICES_SORT_OPERATOR_DOWN:
			dwData = NOTICES_SORT_OPERATOR_NO;
		break;
		default:
			dwData = NOTICES_SORT_OPERATOR_UP;
		}

		SetItemData(iItem, dwData);
		this->Invalidate();
	}
	::PostMessage(m_hWndMessagesTo, UWM_MSG_SELECTION_HAS_CHANGED, 0, 0);
}

void CColorListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	SHORT iShiftKey;

	iShiftKey = GetKeyState(VK_SHIFT);
	iShiftKey &= 0x8000;

	if (m_iLastItem != -1 && iShiftKey)
	{
		if (nChar == VK_END)
		{
			SelItemRange(FALSE, 0, GetCount());
			SelItemRange(TRUE, m_iLastItem, GetCount());
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE),(LPARAM)CListBox::m_hWnd);	// ON_LBN_SELCHANGE
		}
		if (nChar == VK_HOME)
		{
			SelItemRange(FALSE, 0, GetCount());
			SelItemRange(TRUE, 0, m_iLastItem);
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE),(LPARAM)CListBox::m_hWnd);	// ON_LBN_SELCHANGE
		}
	}

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags );
}

LRESULT CColorListBox::OnSelectionMode(WPARAM parm1, LPARAM parm2)
{
	m_bMultipleSelection = (bool) (parm2!=0);
	return 0;
}
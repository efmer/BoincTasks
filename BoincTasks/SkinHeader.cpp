// SkinHeader.cpp : implementation file
//

#include "stdafx.h"
#include "boincTasks.h"
#include "SkinHeader.h"
#include "MemDC.h"
#include "DlgColorGeneral.h"

// CSkinHeader

IMPLEMENT_DYNAMIC(CSkinHeaderCtrl, CHeaderCtrl)

CSkinHeaderCtrl::CSkinHeaderCtrl()
{

}

CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
//	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSkinHeader message handlers

void CSkinHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

}

/*
void CSkinHeaderCtrl::OnPaint() 
{
	CRect rect, rectItem, clientRect;
	GetClientRect(&rect);

	int iHeight;
	iHeight = rect.Height();

	if (iHeight != 17)
	{
		return CHeaderCtrl::OnPaint();
	}

	CPaintDC dc(this); // device context for painting
	GetClientRect(&clientRect);
	CMemCDC memDC(&dc, rect);
	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);

	memDC.FillSolidRect(&rect, RGB(76,85,118));
	// fill whole header
	CBitmap bitmapSpan;
	bitmapSpan.LoadBitmap(IDB_COLUMNHEADER_SPAN_17);


	CBitmap* pOldBitmapSpan = bitmapDC.SelectObject(&bitmapSpan);
	
	for(int v=0; v<rect.Width(); v++)
	{
		memDC.BitBlt((rect.left+2)+(v*1), 0, 1, 21, &bitmapDC,0,0,SRCCOPY);
	}

	bitmapDC.SelectObject(pOldBitmapSpan);
	bitmapSpan.DeleteObject();

	int nItems = GetItemCount();

	CBitmap bitmapStart;
	CBitmap bitmap2;
	CBitmap bitmapEnd;
	
	bitmapStart.LoadBitmap(IDB_COLUMNHEADER_START_17);
	bitmap2.LoadBitmap(IDB_COLUMNHEADER_SPAN_17);
	bitmapEnd.LoadBitmap(IDB_COLUMNHEADER_END_17);

	for(int i = 0; i <nItems; i++)
	{
		
		TCHAR buf1[256];
		HD_ITEM hditem1;
		
		hditem1.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem1.pszText = buf1;
		hditem1.cchTextMax = 255;
		GetItem( i, &hditem1 );
		
		GetItemRect(i, &rect);
		
		CBitmap* pOldBitmap = NULL;
		
		//make sure we draw the start piece
		//on the first item so it has a left border

		//For the following items we will just use the
		//right border of the previous items as the left
		//border
		if(hditem1.iOrder==0)
		{
			pOldBitmap = bitmapDC.SelectObject(&bitmapStart);
			memDC.BitBlt(rect.left,rect.top,2,21,&bitmapDC,0,0,SRCCOPY);//12
		}
		else
		{
			memDC.BitBlt(rect.left-1,rect.top,2,21,&bitmapDC,0,0,SRCCOPY);//12
			pOldBitmap = bitmapDC.SelectObject(&bitmap2);
			memDC.BitBlt(rect.left+1,rect.top,1,21,&bitmapDC,0,0,SRCCOPY);//12
		}

		bitmapDC.SelectObject(pOldBitmap);
		
		//span the bitmap for the width of the column header item
		int nWidth = rect.Width() - 4;
		
		CBitmap* pOldBitmap2 = bitmapDC.SelectObject(&bitmap2);
		for(int v=0; v<nWidth; v++)
		{
			memDC.BitBlt((rect.left+2)+(v*1), 0, 1, 21, &bitmapDC,0,0,SRCCOPY);//12
		}

		bitmapDC.SelectObject(pOldBitmap2);
		
		
		//draw the end piece of the column header
		CBitmap* pOldBitmap3 = bitmapDC.SelectObject(&bitmapEnd);
		memDC.BitBlt((rect.right-2), 0, 2, 21, &bitmapDC,0,0,SRCCOPY);	//12
		bitmapDC.SelectObject(pOldBitmap3);
		
		//Get all the info for the current
		//item so we can draw the text to it
		//in the desired font and style
		DRAWITEMSTRUCT	DrawItemStruct;
		GetItemRect(i, &rectItem);
		
		
		DrawItemStruct.CtlType		= 100;
		DrawItemStruct.hDC			= dc.GetSafeHdc();
		DrawItemStruct.itemAction	= ODA_DRAWENTIRE; 
		DrawItemStruct.hwndItem 	= GetSafeHwnd(); 
		DrawItemStruct.rcItem	= rectItem;
		DrawItemStruct.itemID	= i;
		DrawItem(&DrawItemStruct);
		
		UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_TOP |DT_CENTER | DT_END_ELLIPSIS ;

		CFont *pFont;
		pFont = GetFont();
//		CString sFont;
//		sFont = theApp.m_pDlgColorGeneral->m_sFont;

//		CFont font;
//		LOGFONT lf;
//		memset(&lf, 0, sizeof(LOGFONT));
//		lf.lfHeight = 14;
//		strcpy(lf.lfFaceName, m_sFont); // "Sevenet 7");
//		font.CreateFontIndirect(&lf);
		CFont* def_font = memDC.SelectObject(pFont);
		
		memDC.SetBkMode(TRANSPARENT);
		rectItem.DeflateRect(2,2,2,2);
		
		TCHAR buf[256];
		HD_ITEM hditem;
		
		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( DrawItemStruct.itemID, &hditem );

		memDC.DrawText(buf, &rectItem, uFormat);
		memDC.SelectObject(def_font);
//		font.DeleteObject();


	}		
}

BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return false;	
}
*/

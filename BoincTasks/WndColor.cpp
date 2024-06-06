// WndFloaterSmall.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "WndColor.h"


// CWndGrapic

IMPLEMENT_DYNAMIC(CWndColor, CWnd)

CWndColor::CWndColor()
{

}

CWndColor::~CWndColor()
{
}

void CWndColor::SetColor(COLORREF color)
{
	m_color = color;
}

BEGIN_MESSAGE_MAP(CWndColor, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CWndGrapic message handlers


void CWndColor::OnPaint()
{
	CDC		dcMemory;
	HBITMAP memoryBitmap, memoryBitmapOld;
	COLORREF bgColor = RGB(30,100,30);
//	COLORREF txtColor= RGB(255,255,255);

	CBrush brushBg;

	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	dcMemory.CreateCompatibleDC(&dc);
	memoryBitmap = CreateCompatibleBitmap(dc.m_hDC, rect.Width(), rect.Height());
	memoryBitmapOld = (HBITMAP) SelectObject(dcMemory, memoryBitmap);

	brushBg.CreateSolidBrush(m_color);		// background
	dcMemory.FillRect(rect,&brushBg);

	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);

	brushBg.DeleteObject();			// new 1.64
	SelectObject(dcMemory, memoryBitmapOld);
	DeleteObject(memoryBitmap);
	DeleteDC(dcMemory);

	CWnd::OnPaint();
}


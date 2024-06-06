// WndThreadTiming.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadRpc.h"
#include "NumberFormat.h"
#include "DlgColorGraphic.h"
#include "Draw.h"
#include "WndThreadGraph.h"
#include "Translation.h"

// CWndThreadGraph

IMPLEMENT_DYNAMIC(CWndThreadGraph, CWnd)

CWndThreadGraph::CWndThreadGraph()
{
	m_bEnable = false;
	m_bPainting = false;
	m_iLineThickness = 2;
	m_iComputers = 0;
	m_iComputerCount = 0;
	m_pDraw	= new CDraw;
}

CWndThreadGraph::~CWndThreadGraph()
{
	Reset();
	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		delete m_lComputer.at(iPos);
	}
	delete m_pDraw;
}

void CWndThreadGraph::SetConnected(bool bConnected)
{
	m_bConnected = bConnected;
//	this->Invalidate(TRUE);
}

void CWndThreadGraph::GetMaxMinI(std::deque<float>  *plData, int *iMinReturn, int *iMaxReturn, int iSamplesPerWindow)
{
	float	fMin, fMax;
	int		iMin, iMax;

	fMin = 200;
	fMax = 0;

	m_pDraw->GetMaxMinF(plData,&fMin, &fMax, iSamplesPerWindow/THREAD_TIME_RATE, false);

	iMin = (int) fMin;
	if (iMin < 0) iMin = 0;

	iMax = (int) fMax;
	iMax+= 1;

	if (iMax > *iMaxReturn)
	{
		*iMaxReturn = iMax;
	}
	if (iMin < *iMinReturn)
	{
		*iMinReturn = iMin;
	}
}

void CWndThreadGraph::Average(CComputerWndThreadListItem *plTime)
{
	float dAverage;

	plTime->m_lThreadTimeAverage.clear();
	dAverage = 0;

	for (UINT iPos = 0; iPos < plTime->m_lThreadTime.size(); iPos++)
	{
		dAverage = (dAverage*59 + plTime->m_lThreadTime.at(iPos))/60;
		plTime->m_lThreadTimeAverage.push_back(dAverage);
	}
}

void CWndThreadGraph::SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid, COLORREF colorGrid10)
{
	m_colorBackGround	= colorBackGround;
//	m_colorProjectText	= colorProjectText;
	m_colorScaleText	= colorScaleText;
	m_colorGrid			= colorGrid;
	m_colorGrid10		= colorGrid10;
}

void CWndThreadGraph::ShowComputers(int iComputers)
{
	m_iComputers = iComputers;

	this->Invalidate(TRUE);
}

void CWndThreadGraph::Enable(bool bEnable)
{
	m_bEnable = bEnable;
}

bool CWndThreadGraph::IsPainting()
{
	return m_bPainting;
}

BEGIN_MESSAGE_MAP(CWndThreadGraph, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(UWM_MSG_GRAPHIC_AVERAGE, OnAverage)
END_MESSAGE_MAP()


// CWndGrapic message handlers


void CWndThreadGraph::OnPaint()
{
	if (m_bEnable)
	{
		m_bPainting = true;

		CDC		dcMemory;
		HBITMAP memoryBitmap, memoryBitmapOld;

		COLORREF bgColor = m_colorBackGround;

		CBrush brushBg;

		CPaintDC dc(this); // device context for painting
		CRect rect;
		GetClientRect(&rect);

		dcMemory.CreateCompatibleDC(&dc);
		memoryBitmap = CreateCompatibleBitmap(dc.m_hDC, rect.Width(), rect.Height());
		memoryBitmapOld = (HBITMAP) SelectObject(dcMemory, memoryBitmap);

		brushBg.CreateSolidBrush(RGB(255,255,255));		// background
		dcMemory.FillRect(rect,&brushBg);

		CFont font;
		VERIFY(font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		gCodePage,					//ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));             // lpszFacename

		if (m_iComputers == 0)
		{
			dcMemory.SelectObject(&font);
			dcMemory.SetBkColor(RGB(255,255,255));
			dcMemory.SetTextColor(RGB(255,0,0));
			dcMemory.TextOut(10,10, gszTranslation[PosDialogGraphicEmptyComputer]);
			dcMemory.TextOut(10,40, gszTranslation[PosDialogGraphicDoubleClick]);
			font.DeleteObject();
		}

		Draw(&bgColor,(CPaintDC*) &dcMemory,&rect);

		dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);

		brushBg.DeleteObject();
		SelectObject(dcMemory, memoryBitmapOld);
		DeleteObject(memoryBitmap);
		DeleteDC(dcMemory);
	}
	CWnd::OnPaint();
	m_bPainting = false;
}


void CWndThreadGraph::Draw(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	int			iCount, iRepeat;
	int			iHeight, iWidth, iXTop, iYTop, iXBottom, iYBottom;
	CString		sComputer, sProject, sNumber;
	COLORREF	color;
	CNumberFormat numberFormat;
	CSize		sizeNumber;
	bool		bHorzAll;
	int			iMin, iMax;
	time_t		tLastAddTime;
	std::deque<float>	m_lAverage;

	tLastAddTime = -1;

	CPoint pointTopLeft, pointBottomRight;

	pointTopLeft =	pRect->TopLeft();
	pointBottomRight = pRect->BottomRight();
	iHeight = pRect->Height();
	iWidth = pRect->Width();
	iXTop = pointTopLeft.x;
	iYTop = pointTopLeft.y;
	iXBottom = pointBottomRight.x;
	iYBottom = pointBottomRight.y;

	iMin = 1000;
	iMax = 0;

	for (iCount = 0; iCount < (int) m_lComputer.size();iCount++)
	{
		Average(m_lComputer.at(iCount));
		if (m_bAverage)
		{
			GetMaxMinI(&m_lComputer.at(iCount)->m_lThreadTimeAverage, &iMin, &iMax, m_iSamplesPerWindow);
		}
		else
		{
			GetMaxMinI(&m_lComputer.at(iCount)->m_lThreadTime, &iMin, &iMax, m_iSamplesPerWindow);
		}
	}

	int iMin10;
	iMin10 = iMin/10;
	iMin10*=10;
	iMin10 -= 1;

	if ((iMax - iMin) <= 10)
	{
		iMin = iMin10;
	}
	else
	{
		int iDegreesPixel = (pRect->Height() / (iMax - iMin10))+1;	// +1 otherwise 0 is possible
		int iDegreesFree = 60  / iDegreesPixel;
		if (iDegreesFree < 1) iDegreesFree = 1;
		iMin -= iDegreesFree;
	}

	if (iMin < -5)	iMin = -5;

	if (m_iComputers > 0)
	{
		if (iMin < 1000)
		{
			sNumber = numberFormat.FormatNumber(iMax,0);
			sizeNumber = pDc->GetTextExtent(sNumber);
			sizeNumber.cx -= 50;

			DrawRect(pBackgroundColor, pDc, pRect);
			m_pDraw->DrawGridVertical(&m_colorGrid, &m_colorGrid10, pBackgroundColor,(CPaintDC*)pDc,pRect, m_iGridHorzTotal,m_iGridHorzStep,m_iGridHorzThickEvery,m_iScaleHorzValue,m_iScaleHorzValueStep);
			int iDelta = iMax - iMin;
			if (iDelta*6 > pRect->Height()) bHorzAll = false;
			else bHorzAll = true;
	
			m_pDraw->DrawGridHorizontal(&m_colorGrid, &m_colorGrid10, (CPaintDC*)pDc,pRect, iMax, iMin,1,bHorzAll,"%");	

			for (iCount = 0; iCount < m_iComputerCount;iCount++)
			{		
				color = m_lComputer.at(iCount)->m_color;
				iRepeat = m_lComputer.at(iCount)->m_iRepeat;

				if (m_bAverage)
				{
					m_pDraw->DrawTemperature(&m_lComputer.at(iCount)->m_lThreadTimeAverage, color, pDc, pRect, iMax, iMin, m_iSamplesPerWindow/THREAD_TIME_RATE, 0, false, m_iLineThickness);
				}
				else
				{
					m_pDraw->DrawTemperature(&m_lComputer.at(iCount)->m_lThreadTime, color, pDc, pRect, iMax, iMin, m_iSamplesPerWindow/THREAD_TIME_RATE, 0, false, m_iLineThickness);
					m_pDraw->DrawTemperature(&m_lComputer.at(iCount)->m_lThreadTimeAverage, color, pDc, pRect, iMax, iMin, m_iSamplesPerWindow/THREAD_TIME_RATE, 0, false, 1);
				}
				tLastAddTime = m_lComputer.at(iCount)->m_tBegin;
			}
			m_pDraw->DrawGridVerticalNumbers(&m_colorScaleText, pBackgroundColor,(CPaintDC*)pDc,pRect, m_iGridHorzTotal,m_iGridHorzStep,m_iGridHorzThickEvery,m_iScaleHorzValue,m_iScaleHorzValueStep, m_iSamplesPerWindow, tLastAddTime);

			if (iMax > 10)	m_pDraw->DrawGridHorizontalNumbers(&m_colorScaleText, (CPaintDC*)pDc,pRect, iMax, iMin,1,bHorzAll, 10, "%");	
			else 			m_pDraw->DrawGridHorizontalNumbers(&m_colorScaleText, (CPaintDC*)pDc,pRect, iMax, iMin,1,bHorzAll, 1, "%");	
		}
		else
		{
			CDC		dcMemory;
			HBITMAP memoryBitmap, memoryBitmapOld;
	
			COLORREF bgColor = m_colorBackGround;

			CBrush brushBg;

			CPaintDC dc(this); // device context for painting
			CRect rect;
			GetClientRect(&rect);

			dcMemory.CreateCompatibleDC(&dc);
			memoryBitmap = CreateCompatibleBitmap(dc.m_hDC, rect.Width(), rect.Height());
			memoryBitmapOld = (HBITMAP) SelectObject(dcMemory, memoryBitmap);

			brushBg.CreateSolidBrush(RGB(255,255,255));		// background
			dcMemory.FillRect(rect,&brushBg);

			CFont font;
			VERIFY(font.CreateFont(
			24,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			gCodePage,					//ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("Arial")));             // lpszFacename

			dcMemory.SelectObject(&font);
			dcMemory.SetBkColor(RGB(255,255,255));
			dcMemory.SetTextColor(RGB(255,0,0));
			dcMemory.TextOut(10,10, gszTranslation[PosDialogGraphicNoData]);
			pDc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);
			brushBg.DeleteObject();
			SelectObject(dcMemory, memoryBitmapOld);
			DeleteObject(memoryBitmap);
			DeleteDC(dcMemory);
			font.DeleteObject();
		}
	}
}

void CWndThreadGraph::DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CRect rect;
	CBrush brushBg;

	rect = pRect;
	rect.InflateRect(48,28);

	CPen pen;
	pen.CreatePen(PS_SOLID,1, *pBackgroundColor);
	pDc->SelectObject(&pen);

	brushBg.CreateSolidBrush(*pBackgroundColor);		// background
	pDc->SelectObject(&brushBg);

	pDc->RoundRect(rect, CPoint(17, 17));

	brushBg.DeleteObject();
	pen.DeleteObject();

}

void CWndThreadGraph::SetScale(bool bReset,int iGridHorzTotal,int iGridHorzStep,int iGridHorzThickEvery,int iScaleHorzValue,int iScaleHorzValueStep, int iSamplesPerWindow)
{
	m_iGridHorzTotal		= iGridHorzTotal;
	m_iGridHorzStep			= iGridHorzStep;
	m_iGridHorzThickEvery	= iGridHorzThickEvery;
	m_iScaleHorzValue		= iScaleHorzValue;
	m_iScaleHorzValueStep	= iScaleHorzValueStep;
	m_iSamplesPerWindow		= iSamplesPerWindow;
}

void CWndThreadGraph::Reset()
{
	m_iComputerCount = 0;

	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		m_lComputer.at(iPos)->m_lThreadTime.clear();
	}
}

void CWndThreadGraph::AddComputer(std::deque<float> *plThreadTime, COLORREF color, int iRepeat, time_t tBegin)
{
	if (m_iComputerCount >= (int) m_lComputer.size())
	{
		CComputerWndThreadListItem *pComputerWndThreadListItem;
		pComputerWndThreadListItem = new CComputerWndThreadListItem;
		m_lComputer.push_back(pComputerWndThreadListItem);
	}

	m_lComputer.at(m_iComputerCount)->m_color = color;
	m_lComputer.at(m_iComputerCount)->m_iRepeat = iRepeat;
	m_lComputer.at(m_iComputerCount)->m_tBegin = tBegin;


	for (UINT iPos = 0; iPos < plThreadTime->size(); iPos++)
	{
		m_lComputer.at(m_iComputerCount)->m_lThreadTime.push_back(plThreadTime->at(iPos));
	}

	m_iComputerCount++;
}

LRESULT CWndThreadGraph::OnAverage(WPARAM parm1, LPARAM parm2)
{
	m_bAverage = (parm1 != 0);
	return 0;
}
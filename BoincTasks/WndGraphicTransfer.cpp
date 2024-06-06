// WndGraphicTransfer.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "NumberFormat.h"
#include "Translation.h"
#include "DlgGraphicTransfer.h"
#include "WndGraphicTransfer.h"


// CWndGraphicTransfer

IMPLEMENT_DYNAMIC(CWndGraphicTransfer, CWnd)

CWndGraphicTransfer::CWndGraphicTransfer()
{
	m_bEnable = false;
	m_bPainting = false;
	m_bDebugMode = false;
	m_iLineThickness = 2;
	m_iComputers = 0;
	m_iComputerCount = 0;
	m_bShowDown = false;
	m_bShowUp = false; 
	m_bBlockMode = false;

	m_iTooltipDown = -1;
	m_iTooltipUp = -1;
}

CWndGraphicTransfer::~CWndGraphicTransfer()
{
	Reset();
	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		delete m_lComputer.at(iPos);
	}
}

void CWndGraphicTransfer::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
//	EnableToolTips(TRUE);
}

void CWndGraphicTransfer::Reset()
{
	m_iComputerCount = 0;

	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		for (int iPosXferClear = 0; iPosXferClear < (int) m_lComputer.at(iPos)->m_lXfer.size(); iPosXferClear++)
		{
			delete m_lComputer.at(iPos)->m_lXfer.at(iPosXferClear);
		}
		m_lComputer.at(iPos)->m_lXfer.clear();
	}
	m_lComputer.clear();
}

void CWndGraphicTransfer::SetDownUp(bool bShowDown, bool bShowUp)
{
	m_bShowDown = bShowDown;
	m_bShowUp = bShowUp;
}

void CWndGraphicTransfer::SetBlockMode(bool bBlockMode)
{
	m_bBlockMode = bBlockMode;
}

void CWndGraphicTransfer::AddComputer(DAILY_XFER_HISTORY *plXfer, COLORREF color, int iRepeat)
{
	if (m_iComputerCount >= (int) m_lComputer.size())
	{
		CComputerWndTransferListItem *pComputerWndTransferListItem;
		pComputerWndTransferListItem = new CComputerWndTransferListItem;
		m_lComputer.push_back(pComputerWndTransferListItem);
	}

	CreateTimeLine(m_iComputerCount, plXfer);
	m_lComputer.at(m_iComputerCount)->m_color = color;
	m_lComputer.at(m_iComputerCount)->m_iRepeat = iRepeat;
	m_iComputerCount++;
}

void CWndGraphicTransfer::CreateTimeLine(int iPos, DAILY_XFER_HISTORY *plXfer)
{
	int iTimeMax, iTimeMin, iWhen, iWhenOffset;
	CGraphicXfer *pGraphicXfer;

//	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		MinMaxTime(iPos, plXfer, &iTimeMin, &iTimeMax);

		if (iTimeMin >= 0)
		{
			iWhenOffset = iTimeMax - iTimeMin;

			for (int iPosMake = 0; iPosMake <= iWhenOffset; iPosMake++)
			{
				pGraphicXfer = new CGraphicXfer;
				pGraphicXfer->m_dTime = (iTimeMin + iPosMake) * 86400;
				pGraphicXfer->m_dDownload = 0;
				pGraphicXfer->m_dUpload = 0;
				pGraphicXfer->m_rectDown.SetRect(-1,-1,-1,-1);
				pGraphicXfer->m_rectUp.SetRect(-1,-1,-1,-1);
				m_lComputer.at(iPos)->m_lXfer.push_back(pGraphicXfer);
			}

			for (int iPosXfer = 0; iPosXfer < (int) plXfer->daily_xfers.size(); iPosXfer++)
			{
				iWhen = plXfer->daily_xfers.at(iPosXfer).when;
				iWhen-= iTimeMin;

				m_lComputer.at(iPos)->m_lXfer.at(iWhen)->m_dDownload = plXfer->daily_xfers.at(iPosXfer).down;
				m_lComputer.at(iPos)->m_lXfer.at(iWhen)->m_dUpload = plXfer->daily_xfers.at(iPosXfer).up;
			}
		}
	}
}

void CWndGraphicTransfer::MinMaxTime(int iPos, DAILY_XFER_HISTORY *plXfer, int *piTimeMin, int *piTimeMax)
{
	*piTimeMin = -1;
	*piTimeMax = 0;

	for (int iPosXfer = 0; iPosXfer < (int) plXfer->daily_xfers.size(); iPosXfer++)
	{
		if (*piTimeMin == -1) *piTimeMin = plXfer->daily_xfers.at(iPosXfer).when;
		if (*piTimeMin > plXfer->daily_xfers.at(iPosXfer).when) *piTimeMin = plXfer->daily_xfers.at(iPosXfer).when;
		if (*piTimeMax < plXfer->daily_xfers.at(iPosXfer).when) *piTimeMax = plXfer->daily_xfers.at(iPosXfer).when;
	}
}

void CWndGraphicTransfer::SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid)
{
	m_colorBackGround	= colorBackGround;
	m_colorProjectText	= colorProjectText;
	m_colorScaleText	= colorScaleText;
	m_colorGrid			= colorGrid;
}

void CWndGraphicTransfer::ShowComputers(int iComputers)
{
	m_iComputers = iComputers;

	this->Invalidate(TRUE);
}

void CWndGraphicTransfer::Enable(bool bEnable, bool bDebug)
{
	m_bEnable = bEnable;
}

bool CWndGraphicTransfer::IsPainting()
{
	return m_bPainting;
}

bool CWndGraphicTransfer::InRect(CRect *pRect, CPoint *pPoint)
{
	if ((pRect->left >= 0) && (pPoint->x >= pRect->left))
	{
		if (pPoint->x <= pRect->right)
		{
			if (pPoint->y <= pRect->top)
			{
				if (pPoint->y >= pRect->bottom)	// in rect xxxx
				{
					return true;
				}
			}
		}
	}
	return false;
}

BEGIN_MESSAGE_MAP(CWndGraphicTransfer, CWnd)
	ON_WM_MOUSEMOVE( )
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(UWM_MSG_GRAPHIC_WINDOW_LINE, OnLineThickness)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CWndGraphicTransfer message handlers

void CWndGraphicTransfer::OnMouseMove(UINT nFlags, CPoint point)
{
	bool	bTip;

	bTip = false;

	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		for (int iPosXfer = 0; iPosXfer < (int) m_lComputer.at(iPos)->m_lXfer.size(); iPosXfer++)
		{
			if (InRect(&m_lComputer.at(iPos)->m_lXfer.at(iPosXfer)->m_rectDown, &point))
			{
				bTip = true;
				m_iTooltipDown = iPosXfer;
				m_iTooltipUp = -1;
				Invalidate(FALSE);
			}
			else
			{
				if (InRect(&m_lComputer.at(iPos)->m_lXfer.at(iPosXfer)->m_rectUp, &point))
				{
					bTip = true;
					m_iTooltipUp = iPosXfer;
					m_iTooltipDown = -1;
					Invalidate(FALSE);	
				}
			}
		}
	}

	if (!bTip)
	{
		if (m_iTooltipDown != -1)
		{
			m_iTooltipDown = -1;
			Invalidate(FALSE);
		}
		if (m_iTooltipUp != -1)
		{
			m_iTooltipUp = -1;
			Invalidate(FALSE);
		}
	}

	CWnd::OnMouseMove(nFlags,point);
}

void CWndGraphicTransfer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd *pWnd;

	pWnd = this->GetParent();
	pWnd->PostMessage(UWM_MSG_DOUBLE_CLICK,0,0);
}

void CWndGraphicTransfer::OnPaint()
{
	char	cBufferLogging[128];

	m_sLog.m_pFrom = "CWndGraphicTransfer::OnPaint";

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

//		brushBg.CreateSolidBrush(RGB(255,255,255));		// background
		brushBg.CreateSolidBrush(GetSysColor(COLOR_WINDOW));	// background
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

		if (m_bDebugMode)
		{
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"Computers: %d", m_iComputers);m_sLog.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
		}

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

void CWndGraphicTransfer::Draw(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
//	PROJECT		*pProject;
	double		dMinTime, dMaxTime, dMinXfer, dMaxXfer;
	CRect		rect;
//	char		cProjectName[256];		
	int			iCount, iRepeat;
	int			iHeight, iWidth, iXTop, iYTop, iXBottom, iYBottom;
	CString		sComputer, sProject, sNumber;
	COLORREF	color;
	CNumberFormat numberFormat;
	CSize		sizeNumber;

	// squares 1,2,4,8,16  16 = max;

	CPoint pointTopLeft, pointBottomRight;

	pointTopLeft =	pRect->TopLeft();
	pointBottomRight = pRect->BottomRight();
	iHeight = pRect->Height();
	iWidth = pRect->Width();
	iXTop = pointTopLeft.x;
	iYTop = pointTopLeft.y;
	iXBottom = pointBottomRight.x;
	iYBottom = pointBottomRight.y;

	dMinXfer = -1;
	dMaxXfer = 0;
	dMinTime = -1;
	dMaxTime = 0;

	for (iCount = 0; iCount < (int) m_lComputer.size();iCount++)
	{
		MinMax(&m_lComputer.at(iCount)->m_lXfer,&dMinTime, &dMaxTime, &dMinXfer, &dMaxXfer);
	}

	if (m_iComputers > 0)
	{
		if (dMinXfer >= 0)
		{
			rect = *pRect;
			rect.DeflateRect(50,30);
			sNumber = numberFormat.FormatNumber(dMaxXfer,0);
			sizeNumber = pDc->GetTextExtent(sNumber);
			sizeNumber.cx -= 50;

			DrawRect(pBackgroundColor, pDc, &rect);
			DrawXferText(&dMinXfer, &dMaxXfer, pBackgroundColor, pDc, &rect);
			// now move the whole to the right to make place for the credit number
			if (sizeNumber.cx > 0) rect.left += sizeNumber.cx;
			DrawTimeText(&dMinTime, &dMaxTime, pBackgroundColor, pDc, &rect);

			for (iCount = 0; iCount < m_iComputerCount;iCount++)
			{		
				color = m_lComputer.at(iCount)->m_color;
				iRepeat = m_lComputer.at(iCount)->m_iRepeat;

				if (m_bShowDown)
				{
					DrawXfer(&m_lComputer.at(iCount)->m_lXfer, &dMinTime, &dMaxTime,&dMinXfer, &dMaxXfer, &color, true, &iRepeat, pBackgroundColor, pDc, &rect);
				}
				if (m_bShowUp)
				{
					DrawXfer(&m_lComputer.at(iCount)->m_lXfer, &dMinTime, &dMaxTime,&dMinXfer, &dMaxXfer, &color, false, &iRepeat, pBackgroundColor, pDc, &rect);
//					break;
				}
			}
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

void CWndGraphicTransfer::DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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

void CWndGraphicTransfer::DrawXfer(std::deque<CGraphicXfer *> *pXfer, double *pdMinTime,double *pdMaxTime,double *pdMinCredits,double *pdMaxCredits,COLORREF *pColor, bool bDown, int *piRepeat, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint		top, bottom;
	CString		sTxt;
	int			iLineThickness;

	int	iXferSize;
	double 	dLow, dHigh, dStepPixelsXfer, dPixelPosXfer, dStepPixelsTime, dPixelPosTime;
	double dXfer, dXferOriginal, dXferOriginalTip, dXferTPixel, dDeltaXfer, dTime, dPrevTime, dTimeMiddle, dDeltaTime, dHeight, dWidth;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*pdMaxCredits == 0) return;

	iXferSize = (int) pXfer->size();
	if (iXferSize <=0) return;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	dLow =  floor(*pdMinCredits);
	dHigh =  ceil(*pdMaxCredits);

	dDeltaXfer = dHigh - dLow;
	dDeltaTime = *pdMaxTime - *pdMinTime;
	
	dHeight = pRect->Height();
	dWidth	= pRect->Width();
	
	dStepPixelsXfer = dHeight / dDeltaXfer;

	dStepPixelsTime = dWidth / dDeltaTime;

	COLORREF lineColor = *pColor;

	CPen penLine,penLine10;
	iLineThickness = m_iLineThickness;

	int iPenStyle;

	if (bDown)
	{
		iPenStyle = PS_SOLID;
		iLineThickness = m_iLineThickness;
	}
	else
	{
		iPenStyle = PS_DASH;
		iLineThickness = 1;
	}

	penLine.CreatePen(iPenStyle,iLineThickness,lineColor);

	pDc->SelectObject(&penLine);
	pDc->SetBkColor(*pBackgroundColor);

	dPixelPosXfer = bottom.y;
	dPixelPosTime	 = top.x;

//	int	iHorzOffset = 0;

	CFont font;
	VERIFY(font.CreateFont(
	14,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	gCodePage,				     // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial")));             // lpszFacename

	pDc->SelectObject(&font);
	pDc->SetBkColor(*pBackgroundColor);
	pDc->SetTextColor(m_colorScaleText);

	bool bToolTip = false;
	int iToolTipPos = -1, iToolTipLeft = -1, iToolTipTop = -1;

//	pDc->MoveTo((int) (0), (int) (0));	

	for (int iCount = 0; iCount < iXferSize; iCount++)
	{
		if (bDown)	dXferOriginal = pXfer->at(iCount)->m_dDownload;
		else dXferOriginal = pXfer->at(iCount)->m_dUpload;

		dXfer = dXferOriginal - dLow;
		dXferTPixel = dXfer;
		dXferTPixel*= dStepPixelsXfer;

		dTime = pXfer->at(iCount)->m_dTime;
		dTime-= *pdMinTime;
		dTime*= dStepPixelsTime;

		if (m_bBlockMode)
		{
			if (iCount == 0)
			{
				pDc->MoveTo((int) (dPixelPosTime + dTime), (int) (dPixelPosXfer - dXferTPixel));		// the first one
				dPrevTime = dTime;
				if (iXferSize > 1)
				{
					double dNextTime;
					dNextTime = pXfer->at(iCount)->m_dTime;
					dNextTime-= *pdMinTime;
					dNextTime*= dStepPixelsTime;
		
					dTimeMiddle = dTime + (dNextTime - dTime)/2;
					dTimeMiddle+= dPixelPosTime;
				}
				else
				{
					dTimeMiddle = dPixelPosTime + 20;
				}
			}
			else
			{
				pDc->LineTo((int) (dPixelPosTime + dPrevTime), (int) (dPixelPosXfer - dXferTPixel));
				pDc->LineTo((int) (dPixelPosTime + dTime), (int) (dPixelPosXfer - dXferTPixel));
				dTimeMiddle = dPrevTime + (dTime - dPrevTime)/2;
				dTimeMiddle+= dPixelPosTime;
				dPrevTime = dTime;
			}
		}
		else
		{
			if (iCount == 0)
			{
				pDc->MoveTo((int) (dPixelPosTime + dTime), (int) (dPixelPosXfer - dXferTPixel));
			}
			else
			{
				pDc->LineTo((int) (dPixelPosTime + dTime), (int) (dPixelPosXfer - dXferTPixel));
			}
			dTimeMiddle = dPixelPosTime + dTime;
		}

		// draw rect for tooltip

		if (dXferOriginal > 0)
		{
			int iTop = (int) (dPixelPosXfer - dXferTPixel);
			iTop += SELECTION_RECT/2;
			int iBottom = iTop - SELECTION_RECT;
			int iLeft = (int) dTimeMiddle;
			iLeft -= SELECTION_RECT/2;
			int iRight = iLeft + SELECTION_RECT;

			if (bDown)
			{
				CBrush brushLine(lineColor);
				pDc->SelectObject(&brushLine);

				pXfer->at(iCount)->m_rectDown.SetRect(iLeft,iTop,iRight,iBottom);
				pDc->Ellipse(&pXfer->at(iCount)->m_rectDown);

				pXfer->at(iCount)->m_rectDown.SetRect(iLeft-SELECTION_RECT_INFLATE,iTop+SELECTION_RECT_INFLATE,iRight+SELECTION_RECT_INFLATE,iBottom-SELECTION_RECT_INFLATE);
			}
			else
			{
				CBrush brushBg(*pBackgroundColor);
				pDc->SelectObject(&brushBg);
				pXfer->at(iCount)->m_rectUp.SetRect(iLeft,iTop,iRight,iBottom);
				pDc->Ellipse(&pXfer->at(iCount)->m_rectUp);
				pXfer->at(iCount)->m_rectUp.SetRect(iLeft-SELECTION_RECT_INFLATE,iTop+SELECTION_RECT_INFLATE,iRight+SELECTION_RECT_INFLATE,iBottom-SELECTION_RECT_INFLATE);
			}

			if (m_iTooltipDown == iCount && bDown)
			{
				bToolTip = true;
				iToolTipPos = iCount;
				iToolTipLeft = iLeft;
				iToolTipTop = iTop;
				dXferOriginalTip = dXferOriginal;
			}
			if (m_iTooltipUp == iCount && !bDown)
			{
				bToolTip = true;
				iToolTipPos = iCount;
				iToolTipLeft = iLeft;
				iToolTipTop = iTop;
				dXferOriginalTip = dXferOriginal;
			}
		}
	}
	if (bToolTip)
	{
		CString sTooltip;
		CNumberFormat numFormat;
		CTime ctime((time_t) pXfer->at(iToolTipPos)->m_dTime);
		sTooltip = ctime.Format("%d %b %y - ");
		if (dXferOriginalTip > KILO_BYTE)
		{
			if (dXferOriginalTip > MEGA_BYTE)
			{
				if (dXferOriginalTip > GIGA_BYTE)
				{
					sTooltip+= numFormat.FormatNumber(dXferOriginalTip/GIGA_BYTE,1);
					sTooltip+= " GB";
				}
				else
				{
					sTooltip+= numFormat.FormatNumber(dXferOriginalTip/MEGA_BYTE,1);
					sTooltip+= " MB";
				}
			}
			else
			{
				sTooltip+= numFormat.FormatNumber(dXferOriginalTip/KILO_BYTE,1);
				sTooltip+= " kB";
			}
		}
		else
		{
			sTooltip+= numFormat.FormatNumber(dXferOriginalTip,0);
			sTooltip+= " B";
		}
		CSize size = pDc->GetTextExtent(sTooltip);
		iToolTipLeft += size.cx/2;

		pDc->TextOut(iToolTipLeft - size.cx -4, iToolTipTop - size.cy - 4 ,sTooltip);
	}

}


void CWndGraphicTransfer::DrawTimeText(double *pdMinTime, double *pdMaxTime, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint top, bottom;
	CString sTime;
	int iNrTextInterval;
	int iSeconds, iDays, iDaysUsed, iTimeStep;
	double dPosHorz, dStepHorz;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	iSeconds = (int) (*pdMaxTime - *pdMinTime);
	iDays = iSeconds/3600;

	iNrTextInterval = iDays * 120;
	if (pRect->Width() > 0) iNrTextInterval /= pRect->Width();
	if (iNrTextInterval < 1) iNrTextInterval = 1;		// 1 day minimum

	bool bOK = false;
//	while (!bOK)
//	{
		iDaysUsed = iDays / iNrTextInterval;
//		if (iDaysUsed * iNrTextInterval == iDays)
//		{
//			bOK = true;
//			break;
//		}
//		iNrTextInterval++;
//		if (iNrTextInterval > iDays) break;
//	}

	if (iDaysUsed > 0) iTimeStep = iSeconds/iDaysUsed;
	else iTimeStep = iSeconds;

	dPosHorz = 0;
	if (iDaysUsed > 0)
	{
		dStepHorz = pRect->Width();
		dStepHorz /= iDaysUsed;
	}
	else dStepHorz = pRect->Width();

	COLORREF lineColor = m_colorGrid;
	COLORREF textColor = m_colorScaleText;

	CPen penLine,penLine10;
	penLine.CreatePen(PS_SOLID,1,lineColor);

	CFont font;
	VERIFY(font.CreateFont(
	14,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	gCodePage,				  // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial")));             // lpszFacename


	pDc->SelectObject(&font);
	pDc->SelectObject(&penLine);


	pDc->SetBkColor(*pBackgroundColor);
	pDc->SetTextColor(m_colorScaleText);

	if (iTimeStep > 0)
	{
		for (int iTime  = (int) *pdMinTime; iTime <= (int) *pdMaxTime; iTime += iTimeStep)
		{
			pDc->MoveTo((int) dPosHorz +top.x, top.y);
			pDc->LineTo((int) dPosHorz +top.x, bottom.y);

			CTime ctime((time_t) iTime);
			sTime = ctime.Format("%d %b %y");
			pDc->TextOut((int) dPosHorz + top.x - 20, bottom.y ,sTime);
			dPosHorz += dStepHorz;
		}
	}
	penLine.DeleteObject();
	font.DeleteObject();

}

void CWndGraphicTransfer::DrawXferText(double *pdMinXfer, double *pdMaxXfer, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint top, bottom;
	CString sTxt, sExt;
	int iNrTextInterval, iHeight;
	int	iExp;
	double dDelta, dLow, dHigh,dInterval, dPower, dPixelPosVert, dDeltaXfer, dStepPixelsXfer, dPixelPosXfer, dValue, dXfer, dHeight;
	CNumberFormat numFormat;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*pdMaxXfer == 0) return;

	if ((*pdMaxXfer) > GIGA_BYTE)
	{
		dLow = (int) floor(*pdMinXfer/ GIGA_BYTE);
		dHigh = (int) ceil(*pdMaxXfer/ GIGA_BYTE);
		sExt = " GB";
	}
	else
	{
		if ((*pdMaxXfer) > MEGA_BYTE)
		{
			dLow = (int) floor(*pdMinXfer/ MEGA_BYTE);
			dHigh = (int) ceil(*pdMaxXfer/ MEGA_BYTE);
			sExt = " MB";
		}
		else
		{
			if ((*pdMaxXfer) > KILO_BYTE)
			{
				dLow = (int) floor(*pdMinXfer/ KILO_BYTE);
				dHigh = (int) ceil(*pdMaxXfer/KILO_BYTE);
				sExt = " kB";
			}
			else
			{
				dLow = (int) floor(*pdMinXfer);
				dHigh = (int) ceil(*pdMaxXfer);
			}
		}
	}

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	dDelta = dHigh - dLow;
	dDeltaXfer = dHigh - dLow;	

	iHeight = pRect->Height();
	dHeight = iHeight;
	iNrTextInterval = iHeight / 40;
	if (iNrTextInterval < 1) iNrTextInterval = 1;		// 1 minimum

	dInterval = dDelta / iNrTextInterval;

	if ((dHigh - dLow) <= 10)
	{
		if (dInterval < 1) dInterval = 1;
	}
	else
	{
		if (dInterval < 10) dInterval = 10;
	}
	iExp = (int)(floor(log10(dInterval)));
	dPower = pow((double) 10,iExp);

	dInterval = dInterval / dPower;
	dInterval = ceil(dInterval);
	dInterval *= dPower*1;

	dStepPixelsXfer = dHeight / dDeltaXfer;


	COLORREF lineColor = m_colorGrid;
	COLORREF textColor = m_colorScaleText;

	CPen penLine,penLine10;
	penLine.CreatePen(PS_SOLID,1,lineColor);

	CFont font;
	VERIFY(font.CreateFont(
	14,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	gCodePage,			        // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial")));             // lpszFacename


	pDc->SelectObject(&font);
	pDc->SelectObject(&penLine);
	pDc->SetBkColor(*pBackgroundColor);
	pDc->SetTextColor(m_colorScaleText);

	dPixelPosVert = bottom.y;
	dPixelPosVert-= dStepPixelsXfer;

	dValue = dLow;
	dValue  += dInterval;		// 1e not shown

	dPixelPosXfer = bottom.y;

	for (int iCount = 1; iCount < iNrTextInterval; iCount++)
	{
		dXfer = dValue;
		dXfer -=  dLow;
		dXfer*= dStepPixelsXfer;

		if (dXfer > dHeight)
		{
			break;
		}

		pDc->MoveTo(top.x -4, (int) (dPixelPosXfer - dXfer));
		pDc->LineTo(bottom.x, (int) (dPixelPosXfer - dXfer));

		sTxt = numFormat.FormatNumber(dValue,0);
		sTxt+= sExt;

//		if (dValue < 1)	sTxt.Format("%.2f",dValue);

		pDc->TextOut(top.x -40, (int) (dPixelPosXfer - dXfer) - 8 ,sTxt);

		dValue  += dInterval;
	}

	penLine.DeleteObject();
	font.DeleteObject();

}

void CWndGraphicTransfer::MinMax(std::deque<CGraphicXfer *> *pXfer, double *pdMinTime, double *pdMaxTime, double *pdMinXfer, double *pdMaxXfer)
{
//	bool	bFirst = true;
	int iSize;
	double	dXferDown, dXferUp;
	iSize = (int) pXfer->size();
	int iExp;
	double dPower;
	char cBufferLogging[2048];	//, cBufferNumber[30];
	m_sLog.m_pFrom = "CWndGraphicTransfer::MinMax";

	strcpy_s(cBufferLogging,255,"Xfer: ");
	for (int iCount = 0; iCount < iSize; iCount++)
	{
		dXferDown = pXfer->at(iCount)->m_dDownload;
		dXferUp = pXfer->at(iCount)->m_dUpload;
//		if (m_bDebugMode)
//		{
//			_snprintf_s(cBufferNumber,29,_TRUNCATE,"%.2f ",dCredits);
//			strcat_s(cBufferLogging,2047,cBufferNumber);
//		}	

		if (m_bShowDown)
		{
			if (*pdMinXfer == -1) *pdMinXfer = dXferDown;
			if (*pdMinXfer > dXferDown) *pdMinXfer = dXferDown;
			if (*pdMaxXfer < dXferDown) *pdMaxXfer = dXferDown;
		}
		if (m_bShowUp)
		{
			if (*pdMinXfer == -1) *pdMinXfer = dXferUp;
			if (*pdMinXfer > dXferUp) *pdMinXfer = dXferUp;
			if (*pdMaxXfer < dXferUp) *pdMaxXfer = dXferUp;
		}

		if (*pdMinTime == -1) *pdMinTime = (pXfer->at(iCount)->m_dTime);
		else if (*pdMinTime > (pXfer->at(iCount)->m_dTime)) *pdMinTime = pXfer->at(iCount)->m_dTime;
		if (*pdMaxTime < (pXfer->at(iCount)->m_dTime)) *pdMaxTime = pXfer->at(iCount)->m_dTime;
	}

//	if (m_bDebugMode)
//	{
//		m_sLog.m_pText = cBufferLogging;
//		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
//	}

	if (*pdMinXfer > 0)
	{
		iExp = (int)(floor(log10(*pdMinXfer/100)));	
		if (iExp < 1) iExp = 1;
		dPower = pow((double) 10,iExp);
		if (dPower < 10) dPower = 10;
		double dMin = *pdMinXfer / dPower;
		*pdMinXfer = floor(dMin)*dPower;
	}

	if (*pdMaxXfer > 0)
	{
		iExp = (int)(floor(log10(*pdMaxXfer/100)));	// V 0.94 / 10 -> 100
		if (iExp < 1) iExp = 1;
		dPower = pow((double) 10,iExp);
		if (dPower < 10) dPower = 10;
		double dMax = *pdMaxXfer / dPower;
		*pdMaxXfer = ceil(dMax)*dPower;

	}

//	if (m_bDebugMode)
//	{
//		_snprintf_s(cBufferLogging,1024,_TRUNCATE,"Min: %.2f, Max: %.2f", *pdMinCredits, *pdMaxCredits);m_sLog.m_pText = cBufferLogging;
//		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
//	}
}

LRESULT CWndGraphicTransfer::OnLineThickness(WPARAM parm1, LPARAM parm2)
{
	m_iLineThickness = (int) parm1;
	return 0;
}
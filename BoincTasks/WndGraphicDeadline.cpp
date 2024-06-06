// WndGraphic.cpp :9 implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "WndGraphicDeadline.h"
#include "DlgGraphicDeadline.h"
#include "DlgLogging.h"
#include "NumberFormat.h"
#include "Translation.h"
#include "History.h"

/*

	time frame: (first) 0 - MAX_HISTORY_STATISTICS_LEN (now)

	Work is sampled in an one hour time slice.			 [hour][hour][hour][hour]
	m_iReceived = counter in an one hour time slice.

	The time is relative to m_tTimeBaseline that is equal to the current time, rounded down to an hour -= MAX_HISTORY_STATISTICS_LEN * HOUR_SECONDS

*/


// m_tTimeBaseline  = current time rounded down to an hour.


// CWndGraphicDeadline

IMPLEMENT_DYNAMIC(CWndGraphicDeadline, CWnd)

CWndGraphicDeadline::CWndGraphicDeadline()
{
	m_iProjectCount = 0;

	m_bAverage	= true;
	m_bPainting = false;
	m_bEnable = false;
	m_bDebugMode = false;
	m_bBlockMode = false;

	m_bCpu = false;
	m_bGpu = false;

	m_iLineThickness = 2;
}

CWndGraphicDeadline::~CWndGraphicDeadline()
{
}

void CWndGraphicDeadline::ResetProjects()
{
	m_iProjectCount = 0;

	m_iComputers = 0;
	m_iProjects = 0;
}

void CWndGraphicDeadline::AddProject(CString sComputer, CHistoryProjectDeadlineItem *pProjectDeadlineItem, COLORREF color)
{
	if (m_iProjectCount < MAX_PROJECTS)
	{
		m_pProjectDeadlineItems[m_iProjectCount] = pProjectDeadlineItem;
		m_sProjectComputer[m_iProjectCount] = sComputer;
		m_cProjectColor[m_iProjectCount] = color;
		m_iProjectCount++;
	}
}

void CWndGraphicDeadline::SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid, COLORREF colorAverage)
{
	m_colorBackGround	= colorBackGround;
	m_colorProjectText	= colorProjectText;
	m_colorScaleText	= colorScaleText;
	m_colorGrid			= colorGrid;
	m_colorAverage		= colorAverage;
}

void CWndGraphicDeadline::ShowProjects(int iComputers, int iProjects)
{
	m_iComputers = iComputers;
	m_iProjects = iProjects;

	this->Invalidate(TRUE);
}

void CWndGraphicDeadline::SetType(int iUserHost, int iTotalAverage)
{
	m_iUserHost = iUserHost;
	m_iTotalAverage = iTotalAverage;
}

void CWndGraphicDeadline::Enable(bool bEnable, bool bDebug)
{
	m_bEnable = bEnable;
	m_bDebugMode = bDebug;
}

bool CWndGraphicDeadline::IsPainting()
{
	return m_bPainting;
}

void CWndGraphicDeadline::AverageAdd(CHistoryProjectDeadlineItem *pHistoryProjectDeadlineItem)
{
//	int iCpu, iGpu;
//	iCpu = pHistoryProjectDeadlineItem->m_iCpu;
//	iGpu = pHistoryProjectDeadlineItem->m_iGpu;

	for (int iCount = 0; iCount <= MAX_HISTORY_STATISTICS_LEN+1; iCount++)
	{
		if(m_bCpu) m_ProjectAverageItem.m_dReceivedCpu[iCount]+= pHistoryProjectDeadlineItem->m_dReceivedCpu[iCount];///iCpu);
		if(m_bGpu) m_ProjectAverageItem.m_dReceivedGpu[iCount]+= pHistoryProjectDeadlineItem->m_dReceivedGpu[iCount];///iGpu);
	}
}

BEGIN_MESSAGE_MAP(CWndGraphicDeadline, CWnd)
	ON_MESSAGE(UWM_MSG_GRAPHIC_WINDOW_LINE, OnLineThickness)
	ON_MESSAGE(UWM_MSG_GRAPHIC_PROJECTS_ONE, OnProjectsOne)
	ON_MESSAGE(UWM_MSG_GRAPHIC_AVERAGE, OnAverage)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CWndGraphicDeadline message handlers

void CWndGraphicDeadline::OnPaint()
{
	char	cBufferLogging[128];


	m_sLog.m_pFrom = "CWndGraphicDeadline::OnPaint";

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
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"Computers: %d, Projects: %d", m_iComputers, m_iProjects);m_sLog.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
		}

		if (m_iComputers == 0 || m_iProjects == 0)
		{
			dcMemory.SelectObject(&font);
			dcMemory.SetBkColor(RGB(255,255,255));
			dcMemory.SetTextColor(RGB(255,0,0));
			if (m_iComputers == 0)	dcMemory.TextOut(10,10, gszTranslation[PosDialogGraphicEmptyComputer]);
			else dcMemory.TextOut(10,10, gszTranslation[PosDialogGraphicEmptyProject]);
			dcMemory.TextOut(10,40, gszTranslation[PosDialogGraphicDoubleClick]);
			font.DeleteObject();
		}
		else DrawProjects(&bgColor,(CPaintDC*) &dcMemory,&rect);

		dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);

		brushBg.DeleteObject();
		SelectObject(dcMemory, memoryBitmapOld);
		DeleteObject(memoryBitmap);
		DeleteDC(dcMemory);
	}
	CWnd::OnPaint();
	m_bPainting = false;
}

void CWndGraphicDeadline::DrawProjects(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	if (m_bProjectsOneGraph)
	{
		DrawProjectsOneGraph(pBackgroundColor, pDc, pRect);
	}
	else
	{
		DrawProjectsSeperateGraph(pBackgroundColor, pDc, pRect);
	}
}

// all project in the same graph

void CWndGraphicDeadline::DrawProjectsOneGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CHistoryProjectDeadlineItem	*pProjectDeadlineItem;
	int			iMinTime, iMaxTime, iMinDeadline, iMaxDeadline;
	CRect		rect;
	char		cProjectName[256];		
	int			iCount;
	int			iHeight, iWidth, iXTop, iYTop, iXBottom, iYBottom;
	CString		sComputer, sNumber;
	COLORREF	color;
	CNumberFormat numberFormat;
	CSize		sizeNumber;
	struct tm   tmtime;

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

	if (m_iProjects > 8) m_iProjects = 8;

	iMinDeadline = -1;
	iMaxDeadline = 0;
	iMinTime = -1;
	iMaxTime = 0;

	if (m_iProjects <= 0) return;
	if (m_pProjectDeadlineItems[0] == NULL) return;
	m_tTimeBaseline = m_pProjectDeadlineItems[0]->m_tTimeBaseline;

//	_time64(&m_timeBaseline);
	localtime_s(&tmtime, &m_tTimeBaseline);
	int iMinutes = tmtime.tm_min;
//	m_timeBaseline += iMinutes * 60;	// round to the hour
//	m_timeBaseline += MAX_DEADLINE_LEN * HOUR_SECONDS;	// hour to seconds

	for (int iCount = 0; iCount <= MAX_HISTORY_STATISTICS_LEN+1; iCount++)
	{
		m_ProjectAverageItem.m_dReceivedCpu[iCount] = 0;
		m_ProjectAverageItem.m_dReceivedGpu[iCount] = 0;
	}

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		pProjectDeadlineItem = m_pProjectDeadlineItems[iCount];
		AverageAdd(pProjectDeadlineItem);
	}

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		pProjectDeadlineItem = m_pProjectDeadlineItems[iCount];
		strcpy_s(cProjectName,255, pProjectDeadlineItem->m_pcProject);//.c_str());
		MinMax(pProjectDeadlineItem,&iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline);
	}
	if (m_bAverage)	MinMaxAverage(&iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline);
	MinMaxTimeAdjust(m_tTimeBaseline, &iMinTime, &iMaxTime);

	rect = *pRect;
	rect.DeflateRect(50,30);
	sNumber = numberFormat.FormatNumber(iMaxDeadline,0);
	sizeNumber = pDc->GetTextExtent(sNumber);
	sizeNumber.cx -= 50;

	DrawRect(pBackgroundColor, pDc, &rect);
	DrawDeadlineText(&iMinDeadline, &iMaxDeadline, pBackgroundColor, pDc, &rect);
	// now move the whole to the right to make place for the credit number
	if (sizeNumber.cx > 0) rect.left += sizeNumber.cx;
	DrawTime(m_tTimeBaseline, &iMinTime, &iMaxTime, pBackgroundColor, pDc, &rect); 	// any item will do, so take the first

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{		
		pProjectDeadlineItem = m_pProjectDeadlineItems[iCount];
		sComputer= m_sProjectComputer[iCount];
		color = m_cProjectColor[iCount];// m_lineColors[m_iProjectComputerNr[iCount]];

//		if (m_bAverage)
//		{
////			DrawProjectAverage(iCount, &iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline, &color, pBackgroundColor, pDc, &rect);
//		}
//		else
		{
			DrawProject(iCount, pProjectDeadlineItem, &iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline, &color, pBackgroundColor, pDc, &rect);
		}
	}

//	if ((m_iProjects == 1) && (m_iComputers == 1)) 
	if (m_bAverage)
	{
		DrawProjectAverage(iCount, &iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline, &m_colorAverage, pDc, &rect);
	}


	if (m_iProjectCount == 1)
	{
		DrawProjectName(pProjectDeadlineItem->m_pcProject, pBackgroundColor, pDc, &rect);
	}
}

// project in seperare graphs

void CWndGraphicDeadline::DrawProjectsSeperateGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CHistoryProjectDeadlineItem	*pProjectDeadlineItem;
	int			iMinTime, iMaxTime, iMinTimeAdjusted, iMaxTimeAdjusted, iMinDeadline, iMaxDeadline;
	CRect		rect;
	char		cProjectName[256];		
	int			iCount;
	bool		bDraw;
	int			iHeight, iWidth, iDrawPos, iSquares, iXTop, iYTop, iXBottom, iYBottom;
	CString		sComputer;
	COLORREF	color;
	CNumberFormat numberFormat;
	CSize		sizeNumber;
	CString		sNumber;
	struct tm   tmtime;

	if (m_pProjectDeadlineItems[0] == NULL) return;
	m_tTimeBaseline = m_pProjectDeadlineItems[0]->m_tTimeBaseline;

//	_time64(&m_timeBaseline);
	localtime_s(&tmtime, &m_tTimeBaseline);
	int iMinutes = tmtime.tm_min;
//	m_timeBaseline += iMinutes * 60;	// round to the hour
//	m_timeBaseline += MAX_DEADLINE_LEN * HOUR_SECONDS;	// hour to seconds

	iMinDeadline = -1;
	iMaxDeadline = 0;
	iMinTime = -1;
	iMaxTime = 0;

	for (int iCount = 0; iCount <= MAX_HISTORY_STATISTICS_LEN+1; iCount++)
	{
		m_ProjectAverageItem.m_dReceivedCpu[iCount] = 0;
		m_ProjectAverageItem.m_dReceivedGpu[iCount] = 0;
	}

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		pProjectDeadlineItem = m_pProjectDeadlineItems[iCount];
		AverageAdd(pProjectDeadlineItem);
	}
	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		pProjectDeadlineItem = m_pProjectDeadlineItems[iCount];
		MinMax(pProjectDeadlineItem,&iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline);
		m_bProjectDrawn[iCount] = false;
	}
	if (m_bAverage)	MinMaxAverage(&iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline);
	MinMaxTimeAdjust(m_tTimeBaseline, &iMinTime, &iMaxTime);

	// squares 1,2,4,8,16  16 = max;

	iSquares = 1;
	iDrawPos = 0;

	CPoint pointTopLeft, pointBottomRight;

	pointTopLeft =	pRect->TopLeft();
	pointBottomRight = pRect->BottomRight();
	iHeight = pRect->Height();
	iWidth = pRect->Width();
	iXTop = pointTopLeft.x;
	iYTop = pointTopLeft.y;
	iXBottom = pointBottomRight.x;
	iYBottom = pointBottomRight.y;

	if (m_iProjects > 8) m_iProjects = 8;

	if (m_iProjects > 1)
	{
		if (m_iProjects > 2)
		{
			if (m_iProjects > 4)
			{
//				if (m_iProjects > 8)
//				{
//				//	iSquares = 16;
//				}
//				else
				{
					int iWidth4 = iWidth /4;
					m_iSquares[0].SetRect(iXTop						, iYTop				, iXTop + iWidth4	, iYTop + iHeight/2);
					m_iSquares[1].SetRect(iXTop + iWidth4			, iYTop 			, iXTop + iWidth4*2	, iYTop + iHeight/2);
					m_iSquares[2].SetRect(iXTop + iWidth4*2			, iYTop				, iXTop	+ iWidth4*3	, iYTop + iHeight/2);
					m_iSquares[3].SetRect(iXTop + iWidth4*3			, iYTop				, iXTop + iWidth	, iYTop + iHeight/2);	
					m_iSquares[4].SetRect(iXTop						, iYTop + iHeight/2	, iXTop + iWidth4	, iYTop + iHeight);
					m_iSquares[5].SetRect(iXTop + iWidth4			, iYTop + iHeight/2	, iXTop + iWidth4*2	, iYTop + iHeight);
					m_iSquares[6].SetRect(iXTop + iWidth4*2			, iYTop	+ iHeight/2	, iXTop	+ iWidth4*3	, iYTop + iHeight);
					m_iSquares[7].SetRect(iXTop + iWidth4*3			, iYTop	+ iHeight/2	, iXTop + iWidth	, iYTop + iHeight);	
					iSquares = 8;
				}
			}
			else
			{
				m_iSquares[0].SetRect(iXTop						, iYTop				, iXTop + iWidth/2	, iYTop + iHeight/2);
				m_iSquares[1].SetRect(iXTop + iWidth/2			, iYTop 			, iXBottom			, iYTop + iHeight/2);
				m_iSquares[2].SetRect(iXTop 					, iYTop + iHeight/2	, iXTop	+ iWidth/2	, iYTop + iHeight);
				m_iSquares[3].SetRect(iXTop + iWidth/2			, iYTop + iHeight/2	, iXBottom			, iYTop + iHeight);				
				iSquares = 4;
			}
		}
		else
		{
			iSquares = 2;
			m_iSquares[0].SetRect(iXTop, iYTop, iXBottom, iHeight/2);
			m_iSquares[1].SetRect(iXTop, iHeight/2, iXBottom, iHeight);
		}
	}
	else
	{
		iSquares = 1;
		m_iSquares[0] = *pRect;
	}

	bDraw = true;
	while (bDraw)
	{
		bDraw = false;
		cProjectName[0] = 0;

		for (iCount = 0; iCount < m_iProjectCount;iCount++)
		{
			m_bProjectToDraw[iCount] = false;
			if (m_bProjectDrawn[iCount] == false)
			{

				bDraw = true;
				if (m_pProjectDeadlineItems[iCount] == NULL) return;

				pProjectDeadlineItem = m_pProjectDeadlineItems[iCount];

				if (cProjectName[0] == 0)
				{
//					iMinDeadline = -1;
//					iMaxDeadline = 0;
//					iMinTime = -1;
//					iMaxTime = 0;
					iMinTimeAdjusted = iMinTime;
					iMaxTimeAdjusted = iMaxTime;
					strcpy_s(cProjectName,255, pProjectDeadlineItem->m_pcProject);;
				}

				if (pProjectDeadlineItem->m_pcProject == NULL)
				{
					return;				// oeps
				}

				if (strcmp(cProjectName, pProjectDeadlineItem->m_pcProject) == 0)
				{
//					MinMax(pProjectDeadlineItem, &iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline);
					iMinTimeAdjusted = iMinTime;
					iMaxTimeAdjusted = iMaxTime;
					m_bProjectToDraw[iCount] = true;
					m_bProjectDrawn[iCount] = true;
				}
			}
		}
//		if (m_iProjectCount > 0) MinMaxTimeAdjust(m_timeBaseline, &iMinTimeAdjusted, &iMaxTimeAdjusted);

		if (bDraw)
		{
			rect = m_iSquares[iDrawPos];
			rect.DeflateRect(50,30);
			sNumber = numberFormat.FormatNumber(iMaxDeadline,0);	//
			sizeNumber = pDc->GetTextExtent(sNumber);			//
			sizeNumber.cx -= 40;								//

			DrawRect(pBackgroundColor, pDc, &rect);
			DrawDeadlineText(&iMinDeadline, &iMaxDeadline, pBackgroundColor, pDc, &rect);
			// now move the whole to the right to make place for the credit number
			if (sizeNumber.cx > 0) rect.left += sizeNumber.cx;

			DrawTime(m_tTimeBaseline, &iMinTimeAdjusted, &iMaxTimeAdjusted, pBackgroundColor, pDc, &rect);

		
			for (iCount = 0; iCount < m_iProjectCount;iCount++)
			{		
				if 	(m_bProjectToDraw[iCount])
				{
					pProjectDeadlineItem = m_pProjectDeadlineItems[iCount];
					sComputer= m_sProjectComputer[iCount];
					DrawProjectName(pProjectDeadlineItem->m_pcProject, pBackgroundColor, pDc, &rect);
					color = m_cProjectColor[iCount];

//					if (m_bAverage)
//					{
////						DrawProjectAverage(iCount, &iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline, &color, pBackgroundColor, pDc, &rect);
//					}
//					else
					{
						DrawProject(iCount, pProjectDeadlineItem, &iMinTime, &iMaxTime,&iMinDeadline, &iMaxDeadline, &color, pBackgroundColor, pDc, &rect);
					}
				}
			}
//			if (m_iComputers == 1) 
			if (m_bAverage)
			{
				DrawProjectAverage(iCount, &iMinTime, &iMaxTime, &iMinDeadline, &iMaxDeadline, &m_colorAverage, pDc, &rect);
			}
		}
		else
		{
/*
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
*/
		}
		iDrawPos ++;
		if (iDrawPos >= iSquares) return;
		if (iDrawPos >= m_iProjects) return;
	}
}

void CWndGraphicDeadline::MinMax(CHistoryProjectDeadlineItem *pHistoryProjectDeadlineItem, int *piMinTime, int *piMaxTime, int *piMinDeadline, int *piMaxDeadline)
{
//	int		iDeadline;
	int		iExp;
	double	dPower;
	double	dAverage, dDeadline;
	
//	iDeadline = 0;
	dAverage = 0;

	for (int iCount = 0; iCount <= MAX_DEADLINE_LEN; iCount++)		// from begin to end
	{
		dDeadline = 0;
		if (m_bCpu) dDeadline+= (pHistoryProjectDeadlineItem->m_dReceivedCpu[iCount]/HOUR_SECONDS);
		if (m_bGpu) dDeadline+= (pHistoryProjectDeadlineItem->m_dReceivedGpu[iCount]/HOUR_SECONDS);
		if (dDeadline != 0)														// the first one with WU's
		{
			if (*piMinTime == -1) *piMinTime = iCount;
			else
			{
				if (*piMinTime > iCount) *piMinTime = iCount;
			}
			if (*piMaxTime < iCount) *piMaxTime = iCount;						// the last one with WU's
		}

//		if (m_bAverage)
//		{
//			dAverage = (dAverage*23 + dDeadline)/24;
//			if (*piMaxDeadline < ceil(dAverage)) *piMaxDeadline = (int) ceil(dAverage);
//		}
//		else
//		{
			if (*piMaxDeadline < dDeadline) *piMaxDeadline = (int) ceil(dDeadline);
//		}
	}

	*piMinDeadline = 0;
	if (*piMinDeadline > 0)
	{
		iExp = (int)(floor(log10((double) (*piMinDeadline/100))));
		if (iExp < 1) iExp = 1;
		dPower = pow((double) 10,iExp);
		if (dPower < 10) dPower = 10;
		double dMin = *piMinDeadline / dPower;
		*piMinDeadline = (int) (floor(dMin)*dPower);
	}

	if (*piMaxDeadline > 0)
	{
		if (*piMaxDeadline >= 10)
		{
			iExp = (int)(floor(log10((double) (*piMaxDeadline/100))));
			if (iExp < 1) iExp = 1;
			dPower = pow((double) 10,iExp);
			if (dPower < 10) dPower = 10;
			double dMax = *piMaxDeadline / dPower;
			*piMaxDeadline = (int) (ceil(dMax)*dPower);
		}
	}
}

void CWndGraphicDeadline::MinMaxAverage(int *piMinTime, int *piMaxTime, int *piMinDeadline, int *piMaxDeadline)
{
	CPoint top, bottom;
	CString sTxt;

	int		iLow, iHigh, iDeltaDeadline;
	double	dAverage, dAverageMax;

	if (*piMinTime < 0) return;

	iLow = *piMinDeadline;
	iHigh = *piMaxDeadline;

	iDeltaDeadline = iHigh - iLow;
	
	int iMaxTime;
	iMaxTime = *piMaxTime;
	if (iMaxTime > MAX_DEADLINE_LEN) iMaxTime = MAX_DEADLINE_LEN;
	
	dAverageMax = 0;
	dAverage = 0;
	for (int iCount = iMaxTime; iCount >= *piMinTime; iCount--)
	{
		dAverage-= 1;	if (dAverage < 0) dAverage = 0;								//1 already adjusted to 1 core
		dAverage+= (m_ProjectAverageItem.m_dReceivedCpu[iCount]/HOUR_SECONDS);
		if (dAverage > dAverageMax) dAverageMax = dAverage;
	}

	dAverage = 0;
	for (int iCount = iMaxTime; iCount >= *piMinTime; iCount--)
	{
		dAverage-= 1;	if (dAverage < 0) dAverage = 0;								//1 already adjusted to 1 gpu core
		dAverage+= (m_ProjectAverageItem.m_dReceivedGpu[iCount]/HOUR_SECONDS);
		if (dAverage > dAverageMax) dAverageMax = dAverage;
	}

	if (*piMaxDeadline < dAverageMax) *piMaxDeadline = (int) ceil(dAverageMax);
}

// adjust min/max to the localtime, max to nearest day.
void CWndGraphicDeadline::MinMaxTimeAdjust(time_t	timeBaseline, int *piMinTime, int *piMaxTime)
{
	struct tm   tmtime;
	int			iHour, iMinute, iDayMonth;
	time_t		tMin, tMax;

	// round time to the nearest hour
//	localtime_s(&tmtime, &timeBaseline);
	tMin = timeBaseline + (*piMinTime * HOUR_SECONDS);
	tMax = timeBaseline + MAX_DEADLINE_LEN * HOUR_SECONDS;	//(*piMaxTime * HOUR_SECONDS);
//	*piMaxTime = MAX_DEADLINE_LEN;

//	localtime_s(&tmtime, &tMin);
//	iHour	= tmtime.tm_hour;
//	iMinute	= tmtime.tm_min;
//	iDayMonth = tmtime.tm_mday;
//	*piMinTime -= iHour;						// adjust to nearest day.

	*piMinTime  = 0;							// 0 = now


	localtime_s(&tmtime, &tMax);
	iHour	= tmtime.tm_hour;
	iMinute	= tmtime.tm_min;
	iDayMonth = tmtime.tm_mday;
	*piMaxTime += (24 - iHour);					// adjust to nearest day.

//	if ((*piMaxTime - *piMinTime) < 24)
//	{
//		*piMinTime -=  25;
//	}
}



void CWndGraphicDeadline::DrawText(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, CString sTxt)
{
	CPoint top;

	CFont font;
	VERIFY(font.CreateFont(
	20,                        // nHeight
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


	pDc->SetBkColor(*pBackgroundColor);
	pDc->SetTextColor(m_colorScaleText);

	top = pRect->TopLeft();

	pDc->TextOut(top.x +2, top.y +2 ,sTxt);

	font.DeleteObject();

}

void CWndGraphicDeadline::DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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

//	pDc->FillRect(rect,&brushBg);
	pDc->RoundRect(rect, CPoint(17, 17));

	brushBg.DeleteObject();
	pen.DeleteObject();

}

void CWndGraphicDeadline::DrawTime(time_t	tTimeBaseline, int *piMin, int *piMax, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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

	// convert min & max to real time;
	double dMinRealTime, dMaxRealTime;

	dMinRealTime = (double) (tTimeBaseline + (*piMin * HOUR_SECONDS));
	dMaxRealTime = (double) (tTimeBaseline + (*piMax * HOUR_SECONDS));

	iSeconds = (int) (dMaxRealTime - dMinRealTime);
	iDays = iSeconds / 86400;

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

	if (iDaysUsed > 0)
	{
		iTimeStep = iSeconds / iDaysUsed;
	}
	else iTimeStep = iSeconds;

	dPosHorz = 0;
	if (iDaysUsed > 0)
	{
		dStepHorz = pRect->Width();
		dStepHorz/= iDaysUsed;
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
	gCodePage,					// nCharSet
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
		for (int iTime  = (int) dMinRealTime; iTime <= (int) dMaxRealTime; iTime += iTimeStep)
		{
			pDc->MoveTo((int) dPosHorz +top.x, top.y);
			pDc->LineTo((int) dPosHorz +top.x, bottom.y);

			CTime ctime((time_t) iTime);
			sTime = ctime.Format("%d %b %y");
			pDc->TextOut((int) dPosHorz + top.x - 20, bottom.y ,sTime);
			dPosHorz += dStepHorz;
		}
	}

/*
// test calibration grid
	dPosHorz = 0;
	iSeconds = (int) (dMaxRealTime - dMinRealTime);
	iDays = iSeconds / 86400;
	dStepHorz = pRect->Width();
	dStepHorz = dStepHorz / (iDays * 24);
	CPen penLineTest;
	penLineTest.CreatePen(PS_SOLID, 3, RGB(255,0,0));
	pDc->SelectObject(&penLineTest);
	int i24 =0;
	for (double iTime  = (int) dMinRealTime; iTime <= (int) dMaxRealTime; iTime+= HOUR_SECONDS)
	{
		if (i24 == 0)
		{
			pDc->MoveTo((int) (dPosHorz + top.x), (int) (bottom.y - 0));
			pDc->LineTo((int) (dPosHorz + top.x), (int) (bottom.y - 40));
		}
		i24++;
		if (i24 == 24)
		{
			i24 = 0;
		}
		dPosHorz += dStepHorz;
	}
	penLineTest.DeleteObject();
// end test calibration grid
*/

	penLine.DeleteObject();
	font.DeleteObject();

}

void CWndGraphicDeadline::DrawDeadlineText(int *piMinDeadline, int *piMaxDeadline, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{

	CPoint top, bottom;
	CString sTxt;
	int iNrTextInterval, iHeight;
	int	iExp;
	double	dInterval, dIntervalNew, dPower, dPixelPosVert, dStepPixelsCredit, dPixelPosCredits, dValue, dDeadline, dHeight;
	int		iDelta, iLow, iHigh, iDeltaDeadline;
	CNumberFormat numFormat;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*piMaxDeadline <= 0) return;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	iLow = *piMinDeadline;
	iHigh = *piMaxDeadline;

	iDelta = iHigh - iLow;
	iDeltaDeadline = iHigh - iLow;	

	iHeight = pRect->Height();
	dHeight = iHeight;
	iNrTextInterval = iHeight / 40;
	if (iNrTextInterval < 1) iNrTextInterval = 1;		// 1 minimum

	dInterval = iDelta / iNrTextInterval;
	if (dInterval < 1) dInterval = 1;
	iExp = (int)(floor(log10(dInterval)));
	dPower = pow((double) 10,iExp);

	dInterval = dInterval / dPower;
	dInterval = ceil(dInterval);
	dInterval *= dPower*1;
	
	dIntervalNew = dInterval;
//	if (dInterval < 1000) dIntervalNew = 1000;
//	if (dInterval < 500) dIntervalNew = 500;
//	if (dInterval < 200) dIntervalNew = 200;
//	if (dInterval < 100) dIntervalNew = 100;
//	if (dInterval < 50) dIntervalNew = 50;
//	if (dInterval < 25) dIntervalNew = 25;
//	if (dInterval < 20) dIntervalNew = 20;
	if (dInterval < 10) dIntervalNew = 10;
	if (dInterval < 5) dIntervalNew = 5;
	if (dInterval < 2) dIntervalNew = 1;
	dInterval = dIntervalNew;


	dStepPixelsCredit = dHeight / iDeltaDeadline;


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
	gCodePage,					// nCharSet
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
	dPixelPosVert-= dStepPixelsCredit;

	dValue = iLow;
	dValue  += dInterval;		// 1e not shown

	dPixelPosCredits = bottom.y;

	while (1)
	{
		dDeadline =	dValue;
		dDeadline -=  iLow;
		dDeadline*= dStepPixelsCredit;

		if (dDeadline > dHeight)
		{
			break;
		}

		pDc->MoveTo(top.x -4, (int) (dPixelPosCredits - dDeadline));
		pDc->LineTo(bottom.x, (int) (dPixelPosCredits - dDeadline));

		if (dValue < 1)	sTxt.Format("%.2f",dValue);
		else
		{
			sTxt = numFormat.FormatNumber(dValue,0);
		}
		pDc->TextOut(top.x -40, (int) (dPixelPosCredits - dDeadline) - 8 ,sTxt);

		dValue  += dInterval;
	}

	penLine.DeleteObject();
	font.DeleteObject();

}

void CWndGraphicDeadline::DrawProjectName(CString sProject, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint top, bottom;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

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
	gCodePage,					// nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial")));             // lpszFacename

	pDc->SelectObject(&font);
	pDc->SetBkColor(*pBackgroundColor);
	pDc->SetTextColor(m_colorProjectText);

	pDc->TextOut(top.x + pRect->Width()/2 -30, top.y-20, sProject);

	font.DeleteObject();
}

void CWndGraphicDeadline::DrawProject(int iHorzOffset, CHistoryProjectDeadlineItem *pProjectDeadlineItem, int *piMinTime, int *piMaxTime, int *piMinDeadline, int *piMaxDeadline,COLORREF *pColor, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint top, bottom;
	CString sTxt;

	double 	dStepPixels, dPixelPos;//, dStepPixelsTime, dPixelPosTime;
	int		iLow, iHigh, iDeltaDeadline;
	double  dTPixel, dPrevTime, dHeight, dWidth;
	double	dDeadline;
	double	dPosHorz, dStepHorz;
	int		iHourOffset, iSeconds, iDays;
	struct tm   tmtime;
	CPen	penLine,penLineNow;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*piMaxDeadline <= 0) return;

	if (*piMinTime < 0) return;

	while (iHorzOffset > 10)
	{
		iHorzOffset -= 10;
	}

	// convert min & max to real time;
	double dMinRealTime, dMaxRealTime;

	dMinRealTime = (double) (m_tTimeBaseline + (*piMinTime * HOUR_SECONDS));
	dMaxRealTime = (double) (m_tTimeBaseline + (*piMaxTime * HOUR_SECONDS));

	iSeconds = (int) (dMaxRealTime - dMinRealTime);
	iDays = iSeconds / 86400;

	// calculate hour offset

	localtime_s(&tmtime, &m_tTimeBaseline);
	iHourOffset	= tmtime.tm_hour;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	iLow = *piMinDeadline;
	iHigh = *piMaxDeadline;

	iDeltaDeadline = iHigh - iLow;

	dHeight = pRect->Height();
	dWidth	= pRect->Width();
	
	dStepPixels = dHeight;
	dStepPixels/= iDeltaDeadline;

	COLORREF lineColor = *pColor;
	penLine.CreatePen(PS_SOLID, m_iLineThickness, lineColor);
	penLineNow.CreatePen(PS_SOLID, 1, m_colorGrid);

//	pDc->SelectObject(&penLine);
	pDc->SetBkColor(*pBackgroundColor);

	dPixelPos = bottom.y;
//	dStepHorz = pRect->Width();
//	dStepHorz = dStepHorz / ((dMaxRealTime - dMinRealTime)/HOUR_SECONDS);
//	dPosHorz = 0;//dStepHorz * iHourOffset;

/*
// test calibration grid
	dPosHorz = 0;
	iSeconds = (int) (dMaxRealTime - dMinRealTime);
	iDays = iSeconds / 86400;
	dStepHorz = pRect->Width();
	dStepHorz = dStepHorz / (iDays * 24);
	CPen penLineTest;
	penLineTest.CreatePen(PS_SOLID, 3, RGB(255,0,0));
	pDc->SelectObject(&penLineTest);
	int i24 =0;
	for (double iTime  = (int) dMinRealTime; iTime <= (int) dMaxRealTime; iTime+= HOUR_SECONDS)
	{
		if (i24 == 0)
		{
			pDc->MoveTo((int) (dPosHorz + top.x), (int) (bottom.y - 0));
			pDc->LineTo((int) (dPosHorz + top.x), (int) (bottom.y - 40));
		}
		i24++;
		if (i24 == 24)
		{
			i24 = 0;
		}
		dPosHorz += dStepHorz;
	}
	penLineTest.DeleteObject();
// end test calibration grid
*/

	iSeconds = (int) (dMaxRealTime - dMinRealTime);
	iDays = iSeconds / 86400;
	dStepHorz = pRect->Width();
	dStepHorz = dStepHorz / (iDays * 24);
	dPosHorz = dStepHorz * 1; // why
	dPosHorz = dPosHorz + (dStepHorz * iHourOffset);

	pDc->SelectObject(&penLineNow);
	pDc->MoveTo((int) (dPosHorz + top.x), (int) (dPixelPos - 0));
	pDc->LineTo((int) (dPosHorz + top.x), (int) (dPixelPos - 80));

	pDc->SelectObject(&penLine);

	int iCount = 0;
	for (double iTime  = (int) dMinRealTime; iTime <= (int) dMaxRealTime; iTime+= HOUR_SECONDS)	// interval 1 hour
	{
		dDeadline = 0;
		if (m_bCpu) dDeadline+= (pProjectDeadlineItem->m_dReceivedCpu[iCount]/HOUR_SECONDS);
		if (m_bGpu) dDeadline+= (pProjectDeadlineItem->m_dReceivedGpu[iCount]/HOUR_SECONDS);
		dDeadline -=  iLow;
		dTPixel = dDeadline;
		dTPixel*= dStepPixels;

		if (!m_bBlockMode)
		{
			if (iTime == dMinRealTime)	pDc->MoveTo((int) dPosHorz + top.x, (int) (dPixelPos - dTPixel));								// the first one
			else
			{
				pDc->LineTo((int) dPosHorz + top.x, (int) (dPixelPos - dTPixel));
			}
		}
		else
		{
			if (iTime == dMinRealTime)
			{
				pDc->MoveTo((int) dPosHorz + top.x, (int) (dPixelPos - dTPixel));								// the first one
				dPrevTime = dPosHorz;
			}
			else
			{
				pDc->LineTo((int) (dPrevTime + top.x), (int) (dPixelPos - dTPixel));
				pDc->LineTo((int) (dPosHorz + top.x), (int) (dPixelPos - dTPixel));
				dPrevTime = dPosHorz;
			}
		}
		iCount++;
		if (iCount > MAX_DEADLINE_LEN) break;
		dPosHorz += dStepHorz;
	}

	penLineNow.DeleteObject();
	penLine.DeleteObject();
}

void CWndGraphicDeadline::DrawProjectAverage(int iHorzOffset, int *piMinTime, int *piMaxTime, int *piMinDeadline, int *piMaxDeadline,COLORREF *pColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint top, bottom;
	CString sTxt;

	double 	dStepPixels, dPixelPos;//, dStepPixelsTime, dPixelPosTime;
	int		iLow, iHigh, iDeltaDeadline;
	double  dTPixel, dHeight, dWidth;
	double	dPosHorz, dStepHorz;
	int		iHourOffset, iSeconds, iDays;
	int		iAverageSamples, iCount;
	double	dAverage;
	struct tm   tmtime;
	CPen	penLineCpu, penLineGpu;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*piMaxDeadline <= 0) return;

	if (*piMinTime < 0) return;

	while (iHorzOffset > 10)
	{
		iHorzOffset -= 10;
	}

	// convert min & max to real time;
	double dMinRealTime, dMaxRealTime;

	dMinRealTime = (double) (m_tTimeBaseline + (*piMinTime * HOUR_SECONDS));
	dMaxRealTime = (double) (m_tTimeBaseline + (*piMaxTime * HOUR_SECONDS));

	iSeconds = (int) (dMaxRealTime - dMinRealTime);
	iDays = iSeconds / 86400;

	// calculate hour offset

	localtime_s(&tmtime, &m_tTimeBaseline);
	iHourOffset	= tmtime.tm_hour;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	iLow = *piMinDeadline;
	iHigh = *piMaxDeadline;

	iDeltaDeadline = iHigh - iLow;

	dHeight = pRect->Height();
	dWidth	= pRect->Width();
	
	dStepPixels = dHeight;
	dStepPixels/= iDeltaDeadline;
	dPixelPos = bottom.y;

	COLORREF lineColor = *pColor;
	if (m_bCpu & m_bGpu)
	{
		penLineCpu.CreatePen(PS_DASH, 1, lineColor);
		penLineGpu.CreatePen(PS_DOT, 1, lineColor);
	}
	else
	{
		penLineCpu.CreatePen(PS_SOLID, m_iLineThickness, lineColor);
		penLineGpu.CreatePen(PS_SOLID, m_iLineThickness, lineColor);
	}

	iSeconds = (int) (dMaxRealTime - dMinRealTime);
	iDays = iSeconds / 86400;
	dStepHorz = pRect->Width();
	dStepHorz = dStepHorz / (iDays * 24);
	dPosHorz = dStepHorz * 1; // why
	dPosHorz = dPosHorz + (dStepHorz * iHourOffset);

	iAverageSamples = 0;

	if(m_bCpu)
	{
		pDc->SelectObject(&penLineCpu);
		iCount = iSeconds / HOUR_SECONDS;
		if (iCount > MAX_DEADLINE_LEN) iCount = MAX_DEADLINE_LEN;
		dAverage = 0;
		for (double iTime  = (int) dMaxRealTime; iTime >= (int) dMinRealTime; iTime-= HOUR_SECONDS)	// interval 1 hour
		{
			dAverage-= 1;	if (dAverage < 0) dAverage = 0;
			dAverage+= (m_ProjectAverageItem.m_dReceivedCpu[iCount]/HOUR_SECONDS);
			dTPixel = dAverage;
			dTPixel*= dStepPixels;
			dPosHorz = dStepHorz * iHourOffset;
			dPosHorz = dPosHorz + (iCount * dStepHorz);

			if (iCount == iSeconds / HOUR_SECONDS)
			{
				pDc->MoveTo((int) (dPosHorz + top.x), (int) (dPixelPos - dTPixel));		// the first one
			}
			else
			{
				pDc->LineTo((int) (dPosHorz + top.x), (int) (dPixelPos - dTPixel));
			}
			iCount--;
			if (iCount < 0) break;
		}
		penLineCpu.DeleteObject();
	}

	iCount = iSeconds / HOUR_SECONDS;
	if (iCount > MAX_DEADLINE_LEN) iCount = MAX_DEADLINE_LEN;
	if(m_bGpu)
	{
		pDc->SelectObject(&penLineGpu);
		iCount = iSeconds / HOUR_SECONDS;
		if (iCount > MAX_DEADLINE_LEN) iCount = MAX_DEADLINE_LEN;
		dAverage = 0;
		for (double iTime  = (int) dMaxRealTime; iTime >= (int) dMinRealTime; iTime-= HOUR_SECONDS)	// interval 1 hour
		{
			dAverage-= 1;	if (dAverage < 0) dAverage = 0;
			dAverage+= (m_ProjectAverageItem.m_dReceivedGpu[iCount]/HOUR_SECONDS);
			dTPixel = dAverage;
			dTPixel*= dStepPixels;
			dPosHorz = dStepHorz * iHourOffset;
			dPosHorz = dPosHorz + (iCount * dStepHorz);

			if (iCount == iSeconds / HOUR_SECONDS)
			{
				pDc->MoveTo((int) (dPosHorz + top.x), (int) (dPixelPos - dTPixel));		// the first one
			}
			else
			{
				pDc->LineTo((int) (dPosHorz + top.x), (int) (dPixelPos - dTPixel));
			}
			iCount--;
			if (iCount < 0) break;
		}
		penLineGpu.DeleteObject();
	}
}

LRESULT CWndGraphicDeadline::OnLineThickness(WPARAM parm1, LPARAM parm2)
{
	m_iLineThickness = (int) parm1;
	m_bBlockMode = (bool) (parm2!=0);
	return 0;
}

LRESULT CWndGraphicDeadline::OnProjectsOne(WPARAM parm1, LPARAM parm2)
{
	m_bProjectsOneGraph = (parm1 != 0);

	return 0;
}

LRESULT CWndGraphicDeadline::OnAverage(WPARAM parm1, LPARAM parm2)
{
	int iCpuGpu;
	m_bAverage = (parm1 != 0);

	iCpuGpu = (int) parm2;

	if (iCpuGpu > 1) m_bGpu = true;
	else m_bGpu = false;
	iCpuGpu = iCpuGpu & 1;
	m_bCpu = (iCpuGpu != 0);

	return 0;
}





// WndGraphic.cpp :9 implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "WndGraphicTasks.h"
#include "DlgGraphicTasks.h"
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


// CWndGraphicTasks

IMPLEMENT_DYNAMIC(CWndGraphicTasks, CWnd)

CWndGraphicTasks::CWndGraphicTasks()
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

CWndGraphicTasks::~CWndGraphicTasks()
{
}

void CWndGraphicTasks::ResetProjects()
{
	m_iProjectCount = 0;

	m_iComputers = 0;
	m_iProjects = 0;
}

void CWndGraphicTasks::AddProject(CString sComputer, CHistoryProjectTasksItem *pProjectTasksItem, COLORREF color)
{
	if (m_iProjectCount < MAX_PROJECTS)
	{
		m_pProjectTasksItems[m_iProjectCount] = pProjectTasksItem;
		m_sProjectComputer[m_iProjectCount] = sComputer;
		m_cProjectColor[m_iProjectCount] = color;
		m_iProjectCount++;
	}
}

void CWndGraphicTasks::SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid, COLORREF colorAverage)
{
	m_colorBackGround	= colorBackGround;
	m_colorProjectText	= colorProjectText;
	m_colorScaleText	= colorScaleText;
	m_colorGrid			= colorGrid;
	m_colorAverage		= colorAverage;
}

void CWndGraphicTasks::ShowProjects(int iComputers, int iProjects)
{
	m_iComputers = iComputers;
	m_iProjects = iProjects;

	this->Invalidate(TRUE);
}

void CWndGraphicTasks::SetType(int iUserHost, int iTotalAverage)
{
	m_iUserHost = iUserHost;
	m_iTotalAverage = iTotalAverage;
}

void CWndGraphicTasks::Enable(bool bEnable, bool bDebug)
{
	m_bEnable = bEnable;
	m_bDebugMode = bDebug;
}

bool CWndGraphicTasks::IsPainting()
{
	return m_bPainting;
}

BEGIN_MESSAGE_MAP(CWndGraphicTasks, CWnd)
	ON_MESSAGE(UWM_MSG_GRAPHIC_WINDOW_LINE, OnLineThickness)
	ON_MESSAGE(UWM_MSG_GRAPHIC_PROJECTS_ONE, OnProjectsOne)
	ON_MESSAGE(UWM_MSG_GRAPHIC_AVERAGE, OnAverage)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CWndGraphicTasks message handlers

void CWndGraphicTasks::OnPaint()
{
	char	cBufferLogging[128];


	m_sLog.m_pFrom = "CWndGraphicTasks::OnPaint";

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
		brushBg.CreateSolidBrush(GetSysColor(COLOR_WINDOW));		// background

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

void CWndGraphicTasks::DrawProjects(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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

void CWndGraphicTasks::DrawProjectsOneGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CHistoryProjectTasksItem	*pProjectTasksItem;
	int			iMinTime, iMaxTime, iMinTasks, iMaxTasks;
	CRect		rect;
	char		cProjectName[256];		
	int			iCount;
	int			iHeight, iWidth, iXTop, iYTop, iXBottom, iYBottom;
	CString		sComputer, sNumber;
	COLORREF	color;
	CNumberFormat numberFormat;
	CSize		sizeNumber;
//	struct tm   tmtime;

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

	iMinTasks = -1;
	iMaxTasks = 0;
	iMinTime = -1;
	iMaxTime = 0;

	if (m_iProjects <= 0) return;
	if (m_pProjectTasksItems[0] == NULL) return;

	m_tTimeBaseline = m_pProjectTasksItems[0]->m_tTimeBaseline;

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		pProjectTasksItem = m_pProjectTasksItems[iCount];
		strcpy_s(cProjectName,255, pProjectTasksItem->m_pcProject);//.c_str());
		MinMax(pProjectTasksItem,&iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks);
	}

	MinMaxTimeAdjust(m_tTimeBaseline, &iMinTime, &iMaxTime);

	rect = *pRect;
	rect.DeflateRect(50,30);
	sNumber = numberFormat.FormatNumber(iMaxTasks,0);
	sizeNumber = pDc->GetTextExtent(sNumber);
	sizeNumber.cx -= 50;

	DrawRect(pBackgroundColor, pDc, &rect);
	DrawTasksText(&iMinTasks, &iMaxTasks, pBackgroundColor, pDc, &rect);
	// now move the whole to the right to make place for the credit number
	if (sizeNumber.cx > 0) rect.left += sizeNumber.cx;
	DrawTime(m_tTimeBaseline, &iMinTime, &iMaxTime, pBackgroundColor, pDc, &rect); 	// any item will do, so take the first

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{		
		pProjectTasksItem = m_pProjectTasksItems[iCount];
		sComputer= m_sProjectComputer[iCount];
		color = m_cProjectColor[iCount];// m_lineColors[m_iProjectComputerNr[iCount]];

		if (m_bAverage)
		{
			DrawProject(iCount, pProjectTasksItem, &iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks, &color, pBackgroundColor, pDc, &rect, true);
		}
		else
		{
			if ((m_iProjects == 1) && (m_iComputers == 1)) 
			{
				DrawProject(iCount, pProjectTasksItem, &iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks, &m_colorAverage, pBackgroundColor, pDc, &rect, true);
			}
			DrawProject(iCount, pProjectTasksItem, &iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks, &color, pBackgroundColor, pDc, &rect, false);
		}
	}

	if (m_iProjectCount == 1)
	{
		DrawProjectName(pProjectTasksItem->m_pcProject, pBackgroundColor, pDc, &rect);
	}
}

// project in seperare graphs

void CWndGraphicTasks::DrawProjectsSeperateGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CHistoryProjectTasksItem	*pProjectTasksItem;
	int			iMinTime, iMaxTime, iMinTimeAdjusted, iMaxTimeAdjusted, iMinTasks, iMaxTasks;
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

	if (m_pProjectTasksItems[0] == NULL) return;
	m_tTimeBaseline = m_pProjectTasksItems[0]->m_tTimeBaseline;

	iMinTasks = -1;
	iMaxTasks = 0;
	iMinTime = -1;
	iMaxTime = 0;

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		pProjectTasksItem = m_pProjectTasksItems[iCount];
		MinMax(pProjectTasksItem,&iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks);
		m_bProjectDrawn[iCount] = false;
	}
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
				if (m_pProjectTasksItems[iCount] == NULL) return;

				pProjectTasksItem = m_pProjectTasksItems[iCount];

				if (cProjectName[0] == 0)
				{
//					iMinTasks = -1;
//					iMaxTasks = 0;
//					iMinTime = -1;
//					iMaxTime = 0;
					iMinTimeAdjusted = iMinTime;
					iMaxTimeAdjusted = iMaxTime;
					strcpy_s(cProjectName,255, pProjectTasksItem->m_pcProject);;
				}

				if (pProjectTasksItem->m_pcProject == NULL)
				{
					return;				// oeps
				}

				if (strcmp(cProjectName, pProjectTasksItem->m_pcProject) == 0)
				{
//					MinMax(pProjectTasksItem, &iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks);
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
			sNumber = numberFormat.FormatNumber(iMaxTasks,0);	//
			sizeNumber = pDc->GetTextExtent(sNumber);			//
			sizeNumber.cx -= 40;								//

			DrawRect(pBackgroundColor, pDc, &rect);
			DrawTasksText(&iMinTasks, &iMaxTasks, pBackgroundColor, pDc, &rect);
			// now move the whole to the right to make place for the credit number
			if (sizeNumber.cx > 0) rect.left += sizeNumber.cx;

			DrawTime(m_tTimeBaseline, &iMinTimeAdjusted, &iMaxTimeAdjusted, pBackgroundColor, pDc, &rect);

		
			for (iCount = 0; iCount < m_iProjectCount;iCount++)
			{		
				if 	(m_bProjectToDraw[iCount])
				{
					pProjectTasksItem = m_pProjectTasksItems[iCount];
					sComputer= m_sProjectComputer[iCount];
					DrawProjectName(pProjectTasksItem->m_pcProject, pBackgroundColor, pDc, &rect);
					color = m_cProjectColor[iCount];

					if (m_bAverage)
					{
						DrawProject(iCount, pProjectTasksItem, &iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks, &color, pBackgroundColor, pDc, &rect, true);
					}
					else
					{
						if (m_iComputers == 1) 
						{
							DrawProject(iCount, pProjectTasksItem, &iMinTime, &iMaxTime, &iMinTasks, &iMaxTasks, &m_colorAverage, pBackgroundColor, pDc, &rect, true);
						}
						DrawProject(iCount, pProjectTasksItem, &iMinTime, &iMaxTime,&iMinTasks, &iMaxTasks, &color, pBackgroundColor, pDc, &rect, false);
					}
				}
			}
		}
		iDrawPos ++;
		if (iDrawPos >= iSquares) return;
		if (iDrawPos >= m_iProjects) return;
	}
}

void CWndGraphicTasks::MinMax(CHistoryProjectTasksItem *pHistoryProjectTasksItem, int *piMinTime, int *piMaxTime, int *piMinTasks, int *piMaxTasks)
{
	int		iTasks;
	int		iExp;
	double	dPower;
	double	dAverage, dTasks;

//	iTasks = 0;
	dAverage = 0;

	for (int iCount = 0; iCount <= MAX_HISTORY_STATISTICS_LEN; iCount++)		// from begin to end
	{
		iTasks = 0;
		if (m_bCpu) iTasks+= pHistoryProjectTasksItem->m_iReceivedCpu[iCount];
		if (m_bGpu) iTasks+= pHistoryProjectTasksItem->m_iReceivedGpu[iCount];
		if (iTasks != 0)														// the first one with WU's
		{
			if (*piMinTime == -1) *piMinTime = iCount;
			else
			{
				if (*piMinTime > iCount) *piMinTime = iCount;
			}
			if (*piMaxTime < iCount) *piMaxTime = iCount;						// the last one with WU's
		}

		if (m_bAverage)
		{
			dTasks = iTasks;
			dAverage = (dAverage*23 + dTasks)/24;
			if (*piMaxTasks < ceil(dAverage)) *piMaxTasks = (int) ceil(dAverage);
		}
		else
		{
			if (*piMaxTasks < iTasks) *piMaxTasks = iTasks;
		}
	}

	*piMinTasks = 0;
	if (*piMinTasks > 0)
	{
		iExp = (int)(floor(log10((double) (*piMinTasks/100))));	// V 0.94 / 10 -> 100
		if (iExp < 1) iExp = 1;
		dPower = pow((double) 10,iExp);
		if (dPower < 10) dPower = 10;
		double dMin = *piMinTasks / dPower;
		*piMinTasks = (int) (floor(dMin)*dPower);
	}

	if (*piMaxTasks > 0)
	{
		if (*piMaxTasks >= 10)
		{
			iExp = (int)(floor(log10((double) (*piMaxTasks/100))));	// V 0.94 / 10 -> 100
			if (iExp < 1) iExp = 1;
			dPower = pow((double) 10,iExp);
			if (dPower < 10) dPower = 10;
			double dMax = *piMaxTasks / dPower;
			*piMaxTasks = (int) (ceil(dMax)*dPower);
		}
	}
}

// adjust min/max to the localtime, max to nearest day.
void CWndGraphicTasks::MinMaxTimeAdjust(time_t	timeBaseline, int *piMinTime, int *piMaxTime)
{
	struct tm   tmtime;
	int	iHour, iMinute, iDayMonth;
	time_t		tMin,tMax;

	// round time to the nearest hour
	localtime_s(&tmtime, &timeBaseline);
	tMin = timeBaseline + (*piMinTime * HOUR_SECONDS);
	tMax = timeBaseline + MAX_HISTORY_STATISTICS_LEN * HOUR_SECONDS;	//(*piMaxTime * HOUR_SECONDS);	tmax alway the now
	*piMaxTime = MAX_HISTORY_STATISTICS_LEN;

	localtime_s(&tmtime, &tMin);
	iHour	= tmtime.tm_hour;
	iMinute	= tmtime.tm_min;
	iDayMonth = tmtime.tm_mday;
	*piMinTime -= iHour;

	localtime_s(&tmtime, &tMax);
	iHour	= tmtime.tm_hour;
	iMinute	= tmtime.tm_min;
	iDayMonth = tmtime.tm_mday;
	*piMaxTime += (24 - iHour);					// adjust to nearest day.

	if ((*piMaxTime - *piMinTime) < 24)
	{
		*piMinTime -=  25;
	}
}



void CWndGraphicTasks::DrawText(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, CString sTxt)
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
	gCodePage,			      // nCharSet
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

void CWndGraphicTasks::DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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

void CWndGraphicTasks::DrawTime(time_t	tTimeBaseline, int *piMin, int *piMax, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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
	{
		iDaysUsed = iDays / iNrTextInterval;
//		if (iDaysUsed * iNrTextInterval == iDays)
//		{
//			bOK = true;
//			break;
//		}
//		iNrTextInterval++;
//		if (iNrTextInterval > iDays) break;
	}

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
	gCodePage,				     // nCharSet
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
	penLine.DeleteObject();
	font.DeleteObject();

}

void CWndGraphicTasks::DrawTasksText(int *piMinTasks, int *piMaxTasks, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{

	CPoint top, bottom;
	CString sTxt;
	int iNrTextInterval, iHeight;
	int	iExp;
	double	dInterval, dIntervalNew, dPower, dPixelPosVert, dStepPixelsCredit, dPixelPosCredits, dValue, dTasks, dHeight;
	int		iDelta, iLow, iHigh, iDeltaTasks;
	CNumberFormat numFormat;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*piMaxTasks <= 0) return;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	iLow = *piMinTasks;
	iHigh = *piMaxTasks;

	iDelta = iHigh - iLow;
	iDeltaTasks = iHigh - iLow;	

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


	dStepPixelsCredit = dHeight / iDeltaTasks;


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
	gCodePage,				   // nCharSet
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
		dTasks =	dValue;
		dTasks -=  iLow;
		dTasks*= dStepPixelsCredit;

		if (dTasks > dHeight)
		{
			break;
		}

		pDc->MoveTo(top.x -4, (int) (dPixelPosCredits - dTasks));
		pDc->LineTo(bottom.x, (int) (dPixelPosCredits - dTasks));

		if (dValue < 1)	sTxt.Format("%.2f",dValue);
		else
		{
			sTxt = numFormat.FormatNumber(dValue,0);
	//		sTxt.Format("%.0f",dValue);
		}
		pDc->TextOut(top.x -40, (int) (dPixelPosCredits - dTasks) - 8 ,sTxt);

		dValue  += dInterval;
	}

	penLine.DeleteObject();
	font.DeleteObject();

}

void CWndGraphicTasks::DrawProjectName(CString sProject, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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
	gCodePage,				  // nCharSet
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

void CWndGraphicTasks::DrawProject(int iHorzOffset, CHistoryProjectTasksItem *pProjectTasksItem, int *piMinTime, int *piMaxTime, int *piMinTasks, int *piMaxTasks,COLORREF *pColor, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, bool bAverage)
{
	CPoint top, bottom;
	CString sTxt;

	double 		dStepPixelsTasks, dPixelPosTasks, dStepPixelsTime, dPixelPosTime;
	int			iLow, iHigh, iDeltaTasks, iTasks;
	double		dTasksTPixel, dTime, dPrevTime, dDeltaTime, dHeight, dWidth;
	double		dAverage;
	int			iAverageSamples, iHourOffset;
	struct tm   tmtime;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*piMaxTasks <= 0) return;

	if (*piMinTime < 0) return;

	while (iHorzOffset > 10)
	{
		iHorzOffset -= 10;
	}

	// calculate hour offset
	localtime_s(&tmtime, &m_tTimeBaseline);
	iHourOffset	= tmtime.tm_hour;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

	iLow = *piMinTasks;
	iHigh = *piMaxTasks;

	iDeltaTasks = iHigh - iLow;
	dDeltaTime = *piMaxTime - *piMinTime;
	
	dHeight = pRect->Height();
	dWidth	= pRect->Width();
	
	dStepPixelsTasks = dHeight;
	dStepPixelsTasks/= iDeltaTasks;

	dStepPixelsTime = dWidth;
	dStepPixelsTime/= dDeltaTime;

	CPen penLine,penLine10;

	COLORREF lineColor = *pColor;
	penLine.CreatePen(PS_SOLID, m_iLineThickness, lineColor);

	pDc->SelectObject(&penLine);
	pDc->SetBkColor(*pBackgroundColor);

	dPixelPosTasks = bottom.y;
	dPixelPosTime	 = top.x;


	int iMaxTime;
	iMaxTime = *piMaxTime;
	if (iMaxTime > MAX_HISTORY_STATISTICS_LEN) iMaxTime = MAX_HISTORY_STATISTICS_LEN;

	iAverageSamples = 0;
	dAverage = 0;

	for (int iCount = *piMinTime; iCount <= iMaxTime; iCount++)
	{
		iTasks = 0;
		if (m_bCpu) iTasks+= pProjectTasksItem->m_iReceivedCpu[iCount];
		if (m_bGpu) iTasks+= pProjectTasksItem->m_iReceivedGpu[iCount];
		iTasks -=  iLow;
		dTasksTPixel = iTasks;
		dTasksTPixel*= dStepPixelsTasks;

		dTime = iCount;
		dTime-= *piMinTime;
		dTime+= 1;				// 1 hour correction
		dTime*= dStepPixelsTime;

		if (bAverage)
		{
			iAverageSamples ++;
			dAverage = (dAverage*23 + dTasksTPixel)/24;
			if (iCount == *piMinTime)	pDc->MoveTo((int) (dPixelPosTime + dTime), (int) (dPixelPosTasks - dAverage));		// the first one
			else
			{
				pDc->LineTo((int) (dPixelPosTime + dTime), (int) (dPixelPosTasks - dAverage));
			}
		}
		else
		{
			if (!m_bBlockMode)
			{
				if (iCount == *piMinTime)	pDc->MoveTo((int) (dPixelPosTime + dTime), (int) (dPixelPosTasks - dTasksTPixel));		// the first one
				else
				{
					pDc->LineTo((int) (dPixelPosTime + dTime), (int) (dPixelPosTasks - dTasksTPixel));
				}
			}
			else
			{
				if (iCount == *piMinTime)
				{
					pDc->MoveTo((int) (dPixelPosTime + dTime + iHorzOffset), (int) (dPixelPosTasks - dTasksTPixel));		// the first one
					dPrevTime = dTime;
				}
				else
				{
					pDc->LineTo((int) (dPixelPosTime + dPrevTime + iHorzOffset), (int) (dPixelPosTasks - dTasksTPixel));
					pDc->LineTo((int) (dPixelPosTime + dTime + iHorzOffset), (int) (dPixelPosTasks - dTasksTPixel));
					dPrevTime = dTime;
				}
			}
		}
	}
	penLine.DeleteObject();
}

LRESULT CWndGraphicTasks::OnLineThickness(WPARAM parm1, LPARAM parm2)
{
	m_iLineThickness = (int) parm1;
	m_bBlockMode = (bool) (parm2!=0);
	return 0;
}

LRESULT CWndGraphicTasks::OnProjectsOne(WPARAM parm1, LPARAM parm2)
{
	m_bProjectsOneGraph = (parm1 != 0);

	return 0;
}

LRESULT CWndGraphicTasks::OnAverage(WPARAM parm1, LPARAM parm2)
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





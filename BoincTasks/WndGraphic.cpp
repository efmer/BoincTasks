// WndGraphic.cpp :9 implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "WndGraphic.h"
#include "DlgGraphic.h"
#include "DlgLogging.h"
#include "NumberFormat.h"
#include "Translation.h"

// CWndGraphic

IMPLEMENT_DYNAMIC(CWndGraphic, CWnd)

CWndGraphic::CWndGraphic()
{
	m_iProjectCount = 0;

/*
	m_lineColors[0]= RGB(255,0,0);
	m_lineColors[1]= RGB(0,255,0);
	m_lineColors[2]= RGB(0,0,255);
	m_lineColors[3]= RGB(255,255,255);
	m_lineColors[4]= RGB(255,255,255);
	m_lineColors[5]= RGB(255,255,255);
	m_lineColors[6]= RGB(255,255,255);
	m_lineColors[7]= RGB(255,255,255);
	m_lineColors[8]= RGB(255,255,255);
	m_lineColors[9]= RGB(255,255,255);
	m_lineColors[10]= RGB(255,255,255);
	m_lineColors[11]= RGB(255,255,255);
	m_lineColors[12]= RGB(255,255,255);
	m_lineColors[13]= RGB(255,255,255);
	m_lineColors[14]= RGB(255,255,255);
	m_lineColors[15]= RGB(255,255,255);
	m_lineColors[16]= RGB(255,255,255);
	m_lineColors[17]= RGB(255,255,255);
	m_lineColors[18]= RGB(255,255,255);
	m_lineColors[19]= RGB(255,255,255);
*/

	m_bPainting = false;
	m_bEnable = false;
	m_bDebugMode = false;

	m_iLineThickness = 2;
	m_iTimeToolTip = 0;
	m_btimerActive = false;
}

CWndGraphic::~CWndGraphic()
{
}

void CWndGraphic::ResetProjects()
{
	m_iProjectCount = 0;

	m_iComputers = 0;
	m_iProjects = 0;
}

void CWndGraphic::AddProject(CString sComputer, CString sProject, PROJECT *pProject, COLORREF color, int iRepeat)
{
	if (m_iProjectCount < MAX_PROJECTS)
	{
		m_pProjects[m_iProjectCount] = pProject;
		m_sProjectComputer[m_iProjectCount] = sComputer;
		m_sProjectProject[m_iProjectCount] = sProject;
		m_cProjectColor[m_iProjectCount] = color;
		m_iProjectRepeat[m_iProjectCount] = iRepeat;
		m_iProjectCount++;
	}
}

void CWndGraphic::SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid)
{
	m_colorBackGround	= colorBackGround;
	m_colorProjectText	= colorProjectText;
	m_colorScaleText	= colorScaleText;
	m_colorGrid			= colorGrid;
}

void CWndGraphic::ShowProjects(int iComputers, int iProjects)
{
	m_iComputers = iComputers;
	m_iProjects = iProjects;

	this->Invalidate(TRUE);
}

void CWndGraphic::SetType(int iUserHost, int iTotalAverage)
{
	m_iUserHost = iUserHost;
	m_iTotalAverage = iTotalAverage;
}

void CWndGraphic::Enable(bool bEnable, bool bDebug)
{
	m_bEnable = bEnable;
	m_bDebugMode = bDebug;
}

bool CWndGraphic::IsPainting()
{
	return m_bPainting;
}

bool CWndGraphic::InRect(DAILY_STATS *pDs, CPoint *pPoint)
{
	int iLeft, iRight, iTop, iBottom;

	iLeft = pDs->m_iXPos - m_iLineThickness - 2;
	iRight = pDs->m_iXPos + m_iLineThickness + 2;
	iTop = pDs->m_iYPos  + m_iLineThickness + 2;
	iBottom = pDs->m_iYPos - m_iLineThickness - 2;

	if ((iLeft >= 0) && (pPoint->x >= iLeft))
	{
		if (pPoint->x <= iRight)
		{
			if (pPoint->y <= iTop)
			{
				if (pPoint->y >= iBottom)
				{
					return true;
				}
			}
		}
	}
	return false;
}


BEGIN_MESSAGE_MAP(CWndGraphic, CWnd)
	ON_WM_MOUSEMOVE( )
	ON_WM_TIMER()
	ON_MESSAGE(UWM_MSG_GRAPHIC_WINDOW_LINE, OnLineThickness)
	ON_MESSAGE(UWM_MSG_GRAPHIC_PROJECTS_ONE, OnProjectsOne)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CWndGraphic message handlers

void CWndGraphic::OnTimer(UINT_PTR nIDEvent)
{
	if (m_iTimeToolTip > 0)
	{
		m_iTimeToolTip--;
		if (m_iTimeToolTip == 0)
		{
			this->Invalidate();	// remove tooltip
		}
	}
}


void CWndGraphic::OnMouseMove(UINT nFlags, CPoint point)
{
	PROJECT	*pProject;

	if (m_bEnable)
	{
		for (int iProjectPos = 0; iProjectPos < m_iProjectCount; iProjectPos++)
		{
			if (m_iProjects > 0)
			{
				pProject = m_pProjects[iProjectPos];
				for (int iPosStat = 0; iPosStat < (int) pProject->statistics.size(); iPosStat++)
				{
					if (InRect(&pProject->statistics.at(iPosStat), &point))
					{
						pProject->statistics.at(iPosStat).m_bTip = true;
						m_iTimeToolTip = 5;
						Invalidate(FALSE);
						if (!m_btimerActive)
						{
							SetTimer(UTIMER_DLG_GRAPHIC_WND, 1000, 0);
							m_btimerActive = true;
						}
					}
					else
					{
						pProject->statistics.at(iPosStat).m_bTip = false;
					}
				}
			}
		}
	}

	CWnd::OnMouseMove(nFlags,point);
}

void CWndGraphic::OnPaint()
{
	char	cBufferLogging[128];


	m_sLog.m_pFrom = "CWndGraphic::OnPaint";

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

void CWndGraphic::DrawProjects(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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

void CWndGraphic::DrawProjectsOneGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	PROJECT		*pProject;
	double		dMinTime, dMaxTime, dMinCredits, dMaxCredits;
	CRect		rect;
	char		cProjectName[256];		
	int			iCount, iRepeat;
	int			iHeight, iWidth, iXTop, iYTop, iXBottom, iYBottom;
	CString		sComputer, sProject, sNumber;
	COLORREF	color;
	CNumberFormat numberFormat;
	CSize		sizeNumber;

	// squares 1,2,4,8,16  16 = max;

//	iSquares = 1;
//	iDrawPos = 0;

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

	dMinCredits = -1;
	dMaxCredits = 0;
	dMinTime = -1;
	dMaxTime = 0;

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		if (m_iProjects > 0)
		{
			pProject = m_pProjects[iCount];
			strcpy_s(cProjectName,255, pProject->master_url.c_str());//.c_str());
			MinMax(pProject,&dMinTime, &dMaxTime, &dMinCredits, &dMaxCredits);
		}
	}

	if (m_iProjects > 0)
	{
		rect = *pRect;
		rect.DeflateRect(50,30);
		sNumber = numberFormat.FormatNumber(dMaxCredits,0);
		sizeNumber = pDc->GetTextExtent(sNumber);
		sizeNumber.cx -= 50;

		DrawRect(pBackgroundColor, pDc, &rect);
		DrawCredits(&dMinCredits, &dMaxCredits, pBackgroundColor, pDc, &rect);
		// now move the whole to the right to make place for the credit number
		if (sizeNumber.cx > 0) rect.left += sizeNumber.cx;
		DrawTime(&dMinTime, &dMaxTime, pBackgroundColor, pDc, &rect);

		for (iCount = 0; iCount < m_iProjectCount;iCount++)
		{		
			pProject = m_pProjects[iCount];
			sComputer= m_sProjectComputer[iCount];
			sProject = m_sProjectProject[iCount];
//			DrawProjectName(sProject, pBackgroundColor, pDc, &rect);
//			DrawProjectText(sComputer, pBackgroundColor, pDc, &rect);
			color = m_cProjectColor[iCount];// m_lineColors[m_iProjectComputerNr[iCount]];
			iRepeat = m_iProjectRepeat[iCount];
			DrawProject(pProject, &dMinTime, &dMaxTime,&dMinCredits, &dMaxCredits, &color, &iRepeat, pBackgroundColor, pDc, &rect);
		}

		if (m_iProjectCount == 1)
		{
			DrawProjectName(sProject, pBackgroundColor, pDc, &rect);
		}

	}
}

// project in seperare graphs

void CWndGraphic::DrawProjectsSeperateGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	PROJECT *pProject;
	double		dMinTime, dMaxTime, dMinCredits, dMaxCredits;
	CRect		rect;
	char		cProjectName[256];		
	int			iCount, iRepeat;
	bool		bDraw;
	int			iHeight, iWidth, iDrawPos, iSquares, iXTop, iYTop, iXBottom, iYBottom;
	CString		sComputer, sProject;
	COLORREF	color;
	CNumberFormat numberFormat;
	CSize		sizeNumber;
	CString		sNumber;

	for (iCount = 0; iCount < m_iProjectCount;iCount++)
	{
		m_bProjectDrawn[iCount] = false;
	}

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
				pProject = m_pProjects[iCount];

				if (cProjectName[0] == 0)
				{
					dMinCredits = -1;
					dMaxCredits = 0;
					dMinTime = -1;
					dMaxTime = 0;
					strcpy_s(cProjectName,255, pProject->master_url.c_str());//.c_str());
				}

				if (strcmp(cProjectName,pProject->master_url.c_str()) == 0)//.c_str()) == 0)
				{
					MinMax(pProject,&dMinTime, &dMaxTime, &dMinCredits, &dMaxCredits);
					m_bProjectToDraw[iCount] = true;
					m_bProjectDrawn[iCount] = true;
				}
			}
		}

		if (bDraw)
		{
			rect = m_iSquares[iDrawPos];
			rect.DeflateRect(50,30);
			sNumber = numberFormat.FormatNumber(dMaxCredits,0);	//
			sizeNumber = pDc->GetTextExtent(sNumber);			//
			sizeNumber.cx -= 40;								//

			DrawRect(pBackgroundColor, pDc, &rect);
			DrawCredits(&dMinCredits, &dMaxCredits, pBackgroundColor, pDc, &rect);
			// now move the whole to the right to make place for the credit number
			if (sizeNumber.cx > 0) rect.left += sizeNumber.cx;

			DrawTime(&dMinTime, &dMaxTime, pBackgroundColor, pDc, &rect);

		
			for (iCount = 0; iCount < m_iProjectCount;iCount++)
			{		
				if 	(m_bProjectToDraw[iCount])
				{
					pProject = m_pProjects[iCount];
					sComputer= m_sProjectComputer[iCount];
					sProject = m_sProjectProject[iCount];
					DrawProjectName(sProject, pBackgroundColor, pDc, &rect);

//					DrawProjectText(sComputer, pBackgroundColor, pDc, &rect);

					color = m_cProjectColor[iCount];// m_lineColors[m_iProjectComputerNr[iCount]];
					iRepeat = m_iProjectRepeat[iCount];
					DrawProject(pProject, &dMinTime, &dMaxTime,&dMinCredits, &dMaxCredits, &color, &iRepeat, pBackgroundColor, pDc, &rect);
				}
			}
		}
		iDrawPos ++;
		if (iDrawPos >= iSquares) return;
		if (iDrawPos >= m_iProjects) return;
	}
}

void CWndGraphic::MinMax(PROJECT *pProject, double *pdMinTime, double *pdMaxTime, double *pdMinCredits, double *pdMaxCredits)
{
//	bool	bFirst = true;
	int iSize;
	double	dCredits;
	iSize = (int) pProject->statistics.size();
	int iExp;
	double dPower;
	char cBufferLogging[2048], cBufferNumber[30];
	m_sLog.m_pFrom = "CWndGraphic::MinMax";

	strcpy_s(cBufferLogging,255,"Credits: ");
	for (int iCount = 0; iCount < iSize; iCount++)
	{
		dCredits = GetCredits(pProject, iCount);

		if (m_bDebugMode)
		{
			_snprintf_s(cBufferNumber,29,_TRUNCATE,"%.2f ",dCredits);
			strcat_s(cBufferLogging,2047,cBufferNumber);
		}	

		if (*pdMinCredits == -1) *pdMinCredits = dCredits;
		else if (*pdMinCredits > dCredits) *pdMinCredits = dCredits;
		if (*pdMaxCredits < dCredits) *pdMaxCredits = dCredits;

		if (*pdMinTime == -1) *pdMinTime = pProject->statistics[iCount].day;
		else if (*pdMinTime > pProject->statistics[iCount].day)	*pdMinTime = pProject->statistics[iCount].day;
		if (*pdMaxTime < pProject->statistics[iCount].day)	*pdMaxTime = pProject->statistics[iCount].day;
	}

	if (m_bDebugMode)
	{
		m_sLog.m_pText = cBufferLogging;
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
	}

	if (*pdMinCredits > 0)
	{
		iExp = (int)(floor(log10(*pdMinCredits/100)));	// V 0.94 / 10 -> 100
		if (iExp < 1) iExp = 1;
		dPower = pow((double) 10,iExp);
		if (dPower < 10) dPower = 10;
		double dMin = *pdMinCredits / dPower;
		*pdMinCredits = floor(dMin)*dPower;
	}

	if (*pdMaxCredits > 0)
	{
		iExp = (int)(floor(log10(*pdMaxCredits/100)));	// V 0.94 / 10 -> 100
		if (iExp < 1) iExp = 1;
		dPower = pow((double) 10,iExp);
		if (dPower < 10) dPower = 10;
		double dMax = *pdMaxCredits / dPower;
		*pdMaxCredits = ceil(dMax)*dPower;

	}

	if (m_bDebugMode)
	{
		_snprintf_s(cBufferLogging,1024,_TRUNCATE,"Min: %.2f, Max: %.2f", *pdMinCredits, *pdMaxCredits);m_sLog.m_pText = cBufferLogging;
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
	}
}

void CWndGraphic::DrawText(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, CString sTxt)
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
	gCodePage,			       // nCharSet
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

void CWndGraphic::DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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

void CWndGraphic::DrawTime(double *pdMinTime, double *pdMaxTime, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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
	iDays = iSeconds / 86400;

	iNrTextInterval = iDays * 120;
	if (pRect->Width() > 0) iNrTextInterval /= pRect->Width();
	if (iNrTextInterval < 1) iNrTextInterval = 1;		// 1 day minimum

//	bool bOK = false;
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
		iTimeStep = iSeconds;
		iTimeStep/= iDaysUsed;
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
	gCodePage,				    // nCharSet
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

void CWndGraphic::DrawCredits(double *pdMinCredits, double *pdMaxCredits, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint top, bottom;
	CString sTxt;
	int iNrTextInterval, iHeight;
	int	iExp;
	double dDelta, dLow, dHigh,dInterval, dPower, dPixelPosVert, dDeltaCredits, dStepPixelsCredit, dPixelPosCredits, dValue, dCredits, dHeight;
	CNumberFormat numFormat;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*pdMaxCredits == 0) return;

//	if (*pdMinCredits > dScale)
//	{
//		*pdMinCredits /= dScale;
//		*pdMaxCredits /= dScale;
//	}

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

//	dLow = (int) floor(*pdMinCredits); number get too high for int
//	dHigh = (int) ceil(*pdMaxCredits);

	dLow = floor(*pdMinCredits);
	dHigh = ceil(*pdMaxCredits);

	dDelta = dHigh - dLow;
	dDeltaCredits = dHigh - dLow;	

	iHeight = pRect->Height();
	dHeight = iHeight;
	iNrTextInterval = iHeight / 40;
	if (iNrTextInterval < 1) iNrTextInterval = 1;		// 1 minimum

	dInterval = dDelta / iNrTextInterval;
	if (dInterval < 1) dInterval = 1;
	iExp = (int)(floor(log10(dInterval)));
	dPower = pow((double) 10,iExp);

	dInterval = dInterval / dPower;
	dInterval = ceil(dInterval);
	dInterval *= dPower*1;

//	dStepPixels = pRect->Height() / iNrTextInterval;

	dStepPixelsCredit = dHeight / dDeltaCredits;


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

	dValue = dLow;
	dValue  += dInterval;		// 1e not shown

	dPixelPosCredits = bottom.y;

	for (int iCount = 1; iCount < iNrTextInterval; iCount++)
	{
		dCredits = dValue;
		dCredits -=  dLow;
		dCredits*= dStepPixelsCredit;

		if (dCredits > dHeight)
		{
			break;
		}

		pDc->MoveTo(top.x -4, (int) (dPixelPosCredits - dCredits));
		pDc->LineTo(bottom.x, (int) (dPixelPosCredits - dCredits));

		if (dValue > 1000000000000)
		{
			sTxt = numFormat.FormatNumber(dValue / 1000000, 0) + " M";
		}
		else
		{
			if (dValue > 1000000000)
			{
				sTxt = numFormat.FormatNumber(dValue / 1000, 0) + " K";
			}
			else
			{
				if (dValue < 100)	sTxt = numFormat.FormatNumber(dValue, 2);
				else				sTxt = numFormat.FormatNumber(dValue, 0);
			}
		}
		pDc->TextOut(top.x -40, (int) (dPixelPosCredits - dCredits) - 8 ,sTxt);

		dValue  += dInterval;
	}

	penLine.DeleteObject();
	font.DeleteObject();

}

void CWndGraphic::DrawProjectName(CString sProject, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
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
	gCodePage,			      // nCharSet
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

void CWndGraphic::DrawProject(PROJECT *pProject, double *pdMinTime,double *pdMaxTime,double *pdMinCredits,double *pdMaxCredits,COLORREF *pColor, int *piRepeat, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	CPoint		top, bottom;
	CString		sTxt, sToolTip;
//	char		cBuffer[64];
	int			iLineThickness, iTooltipX, iTooltipY;
	bool		bTooltip;

	int	iProjectSize;
	int		iSeconds, iDays, iDaysUsed;
	double	dStepHorz, dPosHorz;
	double 	dLow, dHigh, dStepPixelsCredit, dPixelPosCredits, dPixelPosTime;
	double dCredits, dCreditsTPixel, dDeltaCredits, dTime, dHeight, dWidth;

	if (pRect->Width() <= 0) return;
	if (pRect->Height()<= 0) return;
	if (*pdMaxCredits == 0) return;

	iProjectSize = (int) pProject->statistics.size();
	if (iProjectSize <=0) return;

	top		= pRect->TopLeft();
	bottom  = pRect->BottomRight();

//	dLow = (int) floor(*pdMinCredits); // some project number are higher than int
//	dHigh = (int) ceil(*pdMaxCredits);

	dLow = floor(*pdMinCredits);
	dHigh = ceil(*pdMaxCredits);

	dDeltaCredits = dHigh - dLow;
		
	dHeight = pRect->Height();
	dWidth	= pRect->Width();
	
	dStepPixelsCredit = dHeight / dDeltaCredits;

	iSeconds = (int) (*pdMaxTime - *pdMinTime);
	iDays = iSeconds / 86400;

	iDaysUsed = iDays;

	dPosHorz = 0;
	if (iDaysUsed > 0)
	{
		dStepHorz = pRect->Width();
		dStepHorz/= iDaysUsed;
	}
	else dStepHorz = pRect->Width();

	COLORREF lineColor = *pColor;

	CPen penLine,penLine10;
	iLineThickness = m_iLineThickness;

	int iPenStyle;
	switch(*piRepeat)
	{
		case 1:
			iPenStyle = PS_DASH;
			iLineThickness =1;			// must be 1
		break;
		case 2:
			iPenStyle = PS_DOT;
			iLineThickness =1;			// must be 1
		break;
		case 3:
			iPenStyle = PS_DASHDOT;
			iLineThickness =1;			// must be 1
		break;
		case 4:
			iPenStyle = PS_DASHDOTDOT;
			iLineThickness =1;			// must be 1
		break;

		default:
			iPenStyle = PS_SOLID;
	}

	penLine.CreatePen(iPenStyle,iLineThickness,lineColor);

	pDc->SelectObject(&penLine);
	pDc->SetBkColor(*pBackgroundColor);

	dPixelPosCredits = bottom.y;
	dPixelPosTime	 = top.x;

	bTooltip = false;

	for (int iCount = 0; iCount < iProjectSize; iCount++)
	{
		dCredits = GetCredits(pProject, iCount);
		dCredits -=  dLow;
		dCreditsTPixel = dCredits;
		dCreditsTPixel*= dStepPixelsCredit;

		dTime = pProject->statistics[iCount].day;
		dTime-= *pdMinTime;
		int iTimePos, iCreditsPos;

		iTimePos = (int) (dPixelPosTime + dTime);

		iTimePos = (int) (dStepHorz * (dTime / 86400));
		iTimePos+= top.x;

		iCreditsPos = (int) (dPixelPosCredits - dCreditsTPixel);

		if (iCount == 0)	pDc->MoveTo(iTimePos, iCreditsPos);
		else				pDc->LineTo(iTimePos, iCreditsPos);

		// draw rect for tooltip

		CBrush brushLine(lineColor);
		pDc->SelectObject(&brushLine);
		pProject->statistics[iCount].m_iXPos = iTimePos;
		pProject->statistics[iCount].m_iYPos = iCreditsPos;
		pDc->Ellipse(iTimePos+iLineThickness+1,iCreditsPos+iLineThickness+1,iTimePos-iLineThickness-1,iCreditsPos-iLineThickness-1);

		if (pProject->statistics[iCount].m_bTip && (m_iTimeToolTip > 0))
		{
			CNumberFormat numFormat;
			
//		_strftime_l(cBuffer, 64, "%d %b %y - ", (time_t) pProject->statistics[iCount].day,  _locale_t locale);

			CTime ctime((time_t) pProject->statistics[iCount].day);
			sToolTip = ctime.Format("%d %b %y - ");	
			dCredits = GetCredits(pProject, iCount);

			if (dCredits < 100)	sToolTip+= numFormat.FormatNumber(dCredits,2);
			else				sToolTip+= numFormat.FormatNumber(dCredits,0);

			CSize size = pDc->GetTextExtent(sToolTip);
			iTimePos += size.cx/2;

			bTooltip = true;
			iTooltipX = iTimePos - size.cx -4;
			iTooltipY = iCreditsPos - size.cy - 4;
		}
	}

	// the actual tooltip text

	if (bTooltip)
	{
		pDc->TextOut(iTooltipX, iTooltipY ,sToolTip);
	}

	penLine.DeleteObject();
}

double CWndGraphic::GetCredits(PROJECT *pProject, int iPosition )
{
	double dCredits;

	if (m_iUserHost == GRAPHIC_USER)
	{
		if (m_iTotalAverage == GRAPHIC_TOTAL)	dCredits = pProject->statistics[iPosition].user_total_credit;
		else									dCredits = pProject->statistics[iPosition].user_expavg_credit;
	}
	else
	{
		if (m_iTotalAverage == GRAPHIC_TOTAL)	dCredits = pProject->statistics[iPosition].host_total_credit;
		else									dCredits = pProject->statistics[iPosition].host_expavg_credit;
	}
	return dCredits;
}

LRESULT CWndGraphic::OnLineThickness(WPARAM parm1, LPARAM parm2)
{
	m_iLineThickness = (int) parm1;
	return 0;
}

LRESULT CWndGraphic::OnProjectsOne(WPARAM parm1, LPARAM parm2)
{
	m_bProjectsOneGraph = (parm1 != 0);

	return 0;
}




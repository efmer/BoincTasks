// WndScheduler.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "TimeString.h"
#include "WndScheduler.h"
#include "Rules.h"
#include "Translation.h"


// CWndScheduler

IMPLEMENT_DYNAMIC(CWndScheduler, CWnd)

CWndScheduler::CWndScheduler()
{
	m_bShiftKey = false;
	m_bCtrlKey = false;
	m_bMouseTracking = false;
	m_bSelected = false;
	m_bDisabled = true;
	m_bForceActive = true;
	m_bUpdateSchedule = false;
	m_bTimerActivated = false;
}

CWndScheduler::~CWndScheduler()
{
	int iPos = 0;

	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			delete m_lSchedulerItems.at(iPos);
			iPos++;
		}
	}
}

void CWndScheduler::Setup(int iSetHorzTime, int iSetHorzTimeHour, int iSetVertTime, bool bVertGrid, bool bDisable, bool bInvers)
{
	CRect rect(-1,-1,-1,-1);

	m_iSetHorzTime = iSetHorzTime;
	m_iSetHorzTimeHour = iSetHorzTimeHour;
	m_iSetVertTime = iSetVertTime;
	m_bDisabled = bDisable;
	m_bVertGrid = bVertGrid;
	m_bInvers = bInvers;

	CSchedulerListItem *pSchedulerListItem;

	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			pSchedulerListItem = new CSchedulerListItem;
			pSchedulerListItem->m_rBox = rect;
			pSchedulerListItem->m_bHoover = false;
			pSchedulerListItem->m_bActive = false;
			pSchedulerListItem->m_bForcedActive = false;
			m_lSchedulerItems.push_back(pSchedulerListItem);
		}
	}
}

void CWndScheduler::ClearInterval()
{
	int iPos;

	iPos = 0;
	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			m_lSchedulerItems.at(iPos)->m_bActive = false;
			m_lSchedulerItems.at(iPos)->m_bForcedActive = false;
			iPos++;
		}
	}
}

void CWndScheduler::AddInterval(int iStart, int iStop)
{
	int iDay, iHour, iMinute, iSecond;
	int	iPos;
	CRules rules;
	
	rules.TimeExtract(iStart, &iDay, &iHour, &iMinute, &iSecond);	
	iStart /= 60;				// minutes
	iStart -= iDay * 24 * 60;	// minutes / day
	iStart /= m_iSetHorzTimeHour;
	rules.TimeExtract(iStop, &iDay, &iHour, &iMinute, &iSecond);	

	iStop /= 60;				// minutes
	iStop -= iDay * 24 * 60;	// minutes / day
	iStop /= m_iSetHorzTimeHour;

	iPos = iDay * m_iSetHorzTime;

	for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
	{
		if (iHorz >= iStart && iHorz < iStop)
		{
			m_lSchedulerItems.at(iPos)->m_bActive = true;
		}
		iPos++;
	}
}

void CWndScheduler::Draw(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect)
{
	int iWidthBox, iHeightBox;
	int	iPos;

	iHeightBox = (pRect->Height()-BOTTOM_TEXT) / m_iSetVertTime;
	m_iWidthText = DrawDay(pBackgroundColor, pDc, iHeightBox);
	iWidthBox = (pRect->Width()- m_iWidthText) / m_iSetHorzTime;

	iPos = 0;

	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			DrawRect(iPos, pBackgroundColor, pDc, iHorz, iWidthBox, iVert, iHeightBox);
			iPos++;
		}
	}

}

void CWndScheduler::DrawRect(int iPos, COLORREF *pBackgroundColor, CPaintDC *pDc, int iHorz, int iWidthBox, int iVert, int iHeightBox)
{
	CFont	*pOldFont;
	CBrush	*pOldBrush;
	CPen	*pOldPen;
	double	dTime, dTimeStep;
	CString sTxt;
	CTimeString	timeString;

	int iHorzPos, iVertPos;

	iHorzPos = iHorz * iWidthBox;
	iVertPos = iVert * iHeightBox;

	iHorzPos += m_iWidthText;

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
	gCodePage,					//ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial")));             // lpszFacename

	pOldFont = pDc->SelectObject(&font);
	pDc->SetTextColor(RGB(0,0,0));

	CRect rect(iHorzPos, iVertPos, iHorzPos + iWidthBox, iVertPos + iHeightBox);
	CBrush brushBg;

	CBrush brushNot;
	CBrush brushForcedActive(RGB(0,255,0));
	CBrush brushHoover;
	CBrush brushActive;
	CBrush brushHooverActive;

	if (m_bInvers)
	{
		brushActive.CreateSolidBrush(RGB(150,0,0));
		brushHooverActive.CreateSolidBrush(RGB(255,0,0));
		brushNot.CreateSolidBrush(RGB(0,150,0));
		brushHoover.CreateSolidBrush(RGB(0,255,0));
	}
	else
	{
		brushActive.CreateSolidBrush(RGB(0,150,0));
		brushHooverActive.CreateSolidBrush(RGB(0,255,0));
		brushNot.CreateSolidBrush(RGB(150,0,0));
		brushHoover.CreateSolidBrush(RGB(255,0,0));
	}

	CPen pen, penSelect;

	brushBg.CreateSolidBrush(*pBackgroundColor);		// background
	pOldBrush = pDc->SelectObject(&brushBg);

	pen.CreatePen(PS_SOLID,1, RGB(200,200,200));
	penSelect.CreatePen(PS_SOLID,1, RGB(0,0,0));
	pOldPen = pDc->SelectObject(&pen);

	CRect	rectSolid = rect;

	pDc->Rectangle(rect);

	if (m_bVertGrid)
	{
		rectSolid.InflateRect(-1,-1);
	}
	else
	{
		rectSolid.InflateRect(0, -1);
	}

	dTimeStep = 60/m_iSetHorzTimeHour;

	if (m_lSchedulerItems.at(iPos)->m_bHoover && !m_bShiftKey)
	{
		if (m_lSchedulerItems.at(iPos)->m_bActive)
		{
			pDc->FillRect(rectSolid, &brushHooverActive);
		}
		else
		{
			pDc->FillRect(rectSolid, &brushHoover);
		}
		sTxt = gszTranslation[PosDialogBoincSettingsGeneralSunday + iVert];
		sTxt+= ", ";
		dTime = iHorz;
		dTime/= dTimeStep;
		sTxt+= timeString.DoubleToTimeString(dTime);
		dTime+= 1/dTimeStep;
		sTxt+= " - ";
		sTxt+= timeString.DoubleToTimeString(dTime);

		pDc->TextOut(m_iWidthText, iHeightBox * m_iSetVertTime, sTxt);
	}
	else
	{
		if (m_lSchedulerItems.at(iPos)->m_bActive)
		{
			pDc->FillRect(rectSolid, &brushActive);
		}
		else
		{
			if (m_lSchedulerItems.at(iPos)->m_bForcedActive)
			{
				pDc->FillRect(rectSolid, &brushForcedActive);
			}
			else
			{
				pDc->FillRect(rectSolid, &brushNot);
			}
		}
	}

	if (m_bShiftKey)
	{
		pDc->SelectObject(&penSelect);
		m_rSelection.right = m_pointSelection.x;
		m_rSelection.bottom = m_pointSelection.y;
		pDc->SetBkMode(TRANSPARENT);
		pDc->Rectangle(m_rSelection);
	}


	pDc->SelectObject(pOldBrush);
	pDc->SelectObject(pOldPen);
	pDc->SelectObject(pOldFont);

	m_lSchedulerItems.at((iVert * m_iSetHorzTime) + iHorz)->m_rBox = rect;
}

int CWndScheduler::DrawDay(COLORREF *pBackgroundColor, CPaintDC *pDc, int iHeight)
{
	CFont *pOldFont;
	int iVertPos;
	int iWidth, iWidthMax;

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
	gCodePage,					//ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial")));             // lpszFacename

	pOldFont = pDc->SelectObject(&font);
	pDc->SetTextColor(RGB(0,0,0));

	iWidthMax = 0;
	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		iVertPos = (iVert * iHeight) + iHeight / 3;
		pDc->TextOut(2, iVertPos, gszTranslation[PosDialogBoincSettingsGeneralSunday + iVert]);
		iWidth = pDc->GetTextExtent(gszTranslation[PosDialogBoincSettingsGeneralSunday + iVert], sizeof(gszTranslation[PosDialogBoincSettingsGeneralSunday + iVert])).cx;
		if (iWidthMax < iWidth) iWidthMax = iWidth;
	}

	pDc->SelectObject(pOldFont);

	return iWidthMax+18;
}

bool CWndScheduler::IsInBox(CPoint point, bool bButton)
{
	bool	bHoover, bHooverFound;
	int		iPos = 0;
	int		iTop, iLeft;
	bool	bMadeActive;

	iTop = point.y;
	iLeft= point.x;

	bMadeActive = false;
	bHooverFound = false;

	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			bHoover = false;
			if (iLeft > m_lSchedulerItems.at(iPos)->m_rBox.left && iLeft < m_lSchedulerItems.at(iPos)->m_rBox.right)
			{
				if (iTop > m_lSchedulerItems.at(iPos)->m_rBox.top && iTop < m_lSchedulerItems.at(iPos)->m_rBox.bottom)
				{
					bHoover = true;
					bHooverFound = true;
					if (bButton)
					{
						m_lSchedulerItems.at(iPos)->m_bActive = ! m_lSchedulerItems.at(iPos)->m_bActive;
						bMadeActive = true;
					}
				}
			}

			if (bHoover != m_lSchedulerItems.at(iPos)->m_bHoover)
			{
				m_lSchedulerItems.at(iPos)->m_bHoover = bHoover;
				this->Invalidate(FALSE);
			}

			iPos++;
		}
	}

	if (m_bShiftKey)
	{
		CRect rect;
		GetClientRect(&rect);

		m_pointSelection.x = iLeft;
		m_pointSelection.y = iTop;

		if (m_pointSelection.x > rect.right)
		{
			m_pointSelection.x = rect.right;
		}
		if (m_pointSelection.y > rect.bottom)
		{
			m_pointSelection.y = rect.bottom;
		}

		this->Invalidate(FALSE);
	}
	else
	{
		if (bMadeActive)
		{
			if (m_bForceActive)
			{
				CheckBeginEnd();
			}
		}
	}

	return bHooverFound;
}

void CWndScheduler::CheckBeginEnd()
{
	int iPos, iPosCheck;
	int iStart, iStop, iPosEnd;
	bool bWrap24;
	

	iPos = 0;
	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			m_lSchedulerItems.at(iPos)->m_bForcedActive = false;
			iPos++;
		}
	}

	iPos = 0;
	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		iStart = -1; 
		iStop = -1;
		iPosEnd = iPos + m_iSetHorzTime-1;
		bWrap24 = false;

		if (m_lSchedulerItems.at(iPosEnd)->m_bActive)
		{
			if (m_lSchedulerItems.at(iPos)->m_bActive)
			{
				bWrap24 = true;
			}
		}

		if (bWrap24)
		{
			iPosCheck = iPosEnd;
			for (int iHorz = m_iSetHorzTime -1; iHorz >= 0; iHorz--)
			{
				if (m_lSchedulerItems.at(iPosEnd)->m_bActive)
				{
					if (iStart != -1)
					{
						if (iStop == -1)
						{
							iStop = iPosEnd;
						}
					}
				}
				else
				{
					if (iStart == -1)
					{
						iStart = iPosEnd;
					}
				}
				iPosEnd--;
			}
			if ((iStart >=0) && (iStop >= 0))
			{
				for (int iHorz = m_iSetHorzTime -1; iHorz >= 0; iHorz--)
				{
					if ((iPosCheck < iStop))
					{
						if (!m_lSchedulerItems.at(iPosCheck)->m_bActive)
						{
							m_lSchedulerItems.at(iPosCheck)->m_bForcedActive = true;
						}
					}
					iPosCheck--;
				}
			}
			iPos += m_iSetHorzTime;
		}
		else
		{
			iPosCheck = iPos;
			for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
			{
				if (m_lSchedulerItems.at(iPos)->m_bActive)
				{
					if (iStart == -1)
					{
						iStart = iPos;
					}
				
					if (iPos > iStop)
					{
						iStop = iPos;
					}
				}
				iPos++;
			}
			
			if ((iStart >=0) && (iStop >= 0))
			{
				for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
				{
					if ((iPosCheck > iStart) && (iPosCheck < iStop))
					{
						if (!m_lSchedulerItems.at(iPosCheck)->m_bActive)
						{
							m_lSchedulerItems.at(iPosCheck)->m_bForcedActive = true;
						}
					}
					iPosCheck++;
				}
			}
		}
	}
}

void CWndScheduler::LeftWindow()
{
	int iPos = 0;

	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			m_lSchedulerItems.at(iPos)->m_bHoover = false;
			iPos++;
		}
	}
	m_bShiftKey = false;
	this->Invalidate(FALSE);
}

void CWndScheduler::Keys(CPoint point, bool bShift, bool bCtrl)
{
	int iTop, iLeft;

	iTop = point.y;
	iLeft= point.x;

	if (bShift)
	{
		m_bShiftKey = true;
		m_bCtrlKey = bCtrl;
		m_rSelection.left = iLeft;
		m_rSelection.top = iTop;
	}
	else
	{
		if (m_bShiftKey)
		{
			SelectRect();
			if (m_bForceActive)
			{
				CheckBeginEnd();
			}			
		}

		m_bShiftKey = false;
		m_bCtrlKey = false;
	}
}

void CWndScheduler::SelectRect()
{
	int iLeft, iTop, iRight, iBottom;
	int iWidthBox, iHeightBox;
	int iPos;

	CRect rect;
	GetClientRect(&rect);

	iWidthBox = (rect.Width()- m_iWidthText) / m_iSetHorzTime;
	iHeightBox = (rect.Height()-BOTTOM_TEXT) / m_iSetVertTime;

	if (m_rSelection.left < m_rSelection.right)
	{
		iLeft	= m_rSelection.left;
		iRight	= m_rSelection.right;
	}
	else
	{
		iRight	= m_rSelection.left;
		iLeft	= m_rSelection.right;
	}

	if (m_rSelection.top < m_rSelection.bottom)
	{
		iTop	= m_rSelection.top;
		iBottom	= m_rSelection.bottom;
	}
	else
	{
		iBottom	= m_rSelection.top;
		iTop	= m_rSelection.bottom;
	}

//	iPos = 0;
//	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
//	{
//		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
//		{
//			m_lSchedulerItems.at(iPos)->m_bActive = false;
//			iPos++;
//		}
//	}

	iPos = 0;

	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			if (m_lSchedulerItems.at(iPos)->m_rBox.left > iLeft && m_lSchedulerItems.at(iPos)->m_rBox.right < iRight )
			{
				if (m_lSchedulerItems.at(iPos)->m_rBox.top > iTop && m_lSchedulerItems.at(iPos)->m_rBox.bottom < iBottom)
				{
					if (m_bCtrlKey)	m_lSchedulerItems.at(iPos)->m_bActive = false;
					else			m_lSchedulerItems.at(iPos)->m_bActive = true;
				}
			}
			if (m_lSchedulerItems.at(iPos)->m_rBox.left+iWidthBox > iLeft && m_lSchedulerItems.at(iPos)->m_rBox.right < iRight+iWidthBox )
			{
				if (m_lSchedulerItems.at(iPos)->m_rBox.top+iHeightBox > iTop && m_lSchedulerItems.at(iPos)->m_rBox.bottom < iBottom+iHeightBox)
				{
					if (m_bCtrlKey)	m_lSchedulerItems.at(iPos)->m_bActive = false;
					else			m_lSchedulerItems.at(iPos)->m_bActive = true;
				}
			}
			iPos++;
		}
	}
	this->Invalidate();	
}

void CWndScheduler::TimeSpanToWindow(int iDay, TIME_SPAN cpu)
{
	int		iPos;
	double	dTime;

	iPos = iDay * m_iSetHorzTime;
	if (!cpu.present)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			m_lSchedulerItems.at(iPos)->m_bActive = true;
			iPos++;
		}
		return;
	}

	for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
	{
		m_lSchedulerItems.at(iPos)->m_bActive = false;
		m_lSchedulerItems.at(iPos)->m_bForcedActive = false;
		iPos++;
	}

	iPos = iDay * m_iSetHorzTime;
	for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
	{
		dTime = iHorz * m_iSetHorzTimeHour;
		dTime /= 60;
		if (cpu.start_hour > cpu.end_hour)
		{
			if (dTime >= cpu.start_hour || dTime < cpu.end_hour)
			{
				m_lSchedulerItems.at(iPos)->m_bActive = true;
			}
		}
		else
		{
			if (dTime >= cpu.start_hour && dTime < cpu.end_hour)
			{
				m_lSchedulerItems.at(iPos)->m_bActive = true;
			}
		}
		iPos++;
	}
}

void CWndScheduler::GetTime(int iVert, CSchedulerTime *pSchedulerTime)
{
	int		iPos, iPosEnd;
	int		iStart, iStop, iActiveCount;
	bool	bWrap24;

	iPos = iVert * m_iSetHorzTime;
	iPosEnd = iPos + m_iSetHorzTime-1;

	pSchedulerTime->m_dStart= -1;
	pSchedulerTime->m_bStop = -1;
	iStart = -1;
	iStop = -1;
	iActiveCount = 0;
	bWrap24 = false;

	if (m_lSchedulerItems.at(iPosEnd)->m_bActive)
	{
		if (m_lSchedulerItems.at(iPos)->m_bActive)
		{
			bWrap24 = true;
		}
	}

	if (bWrap24)
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			if (m_lSchedulerItems.at(iPos)->m_bActive || m_lSchedulerItems.at(iPos)->m_bForcedActive)
			{
				iActiveCount++;
				if (iStop != -1)
				{
					if (iStart == -1)
					{
						iStart = iHorz;
					}
				}
			}
			else
			{
				if (iStop == -1)
				{
					iStop = iHorz;
				}
			}
			iPos++;
		}
	}
	else
	{
		for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
		{
			if (m_lSchedulerItems.at(iPos)->m_bActive || m_lSchedulerItems.at(iPos)->m_bForcedActive)
			{
				iActiveCount++;
				if (iStart == -1)
				{
					iStart = iHorz;
				}
			}
			else
			{
				if (iStart != -1)
				{
					if (iStop == -1)
					{
						iStop = iHorz;
					}
				}
			}
			iPos++;
		}
		if (iStart != -1)
		{
			if (iStop == -1)
			{
				iStop = m_iSetHorzTime;
			}
		}
	}
	if ((iStart != -1) && (iStop != -1))
	{
		pSchedulerTime->m_dStart = iStart;
		pSchedulerTime->m_dStart*= 0.5;
		pSchedulerTime->m_bStop = iStop;
		pSchedulerTime->m_bStop*= 0.5;
	}
	else
	{
		if (iActiveCount = 0)
		{
			pSchedulerTime->m_dStart = 24;
			pSchedulerTime->m_bStop = 0;
		}
		else;
		{
			pSchedulerTime->m_dStart = 0;
			pSchedulerTime->m_bStop = 0;
		}
	}
}

void CWndScheduler::GetTimeInterval(std::deque<CRuleIntervalItem *>	**plInterval)
{
	InitInterval(plInterval);
	for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
	{
		GetTimeIntervalRow(iVert, plInterval);
	}
}

void CWndScheduler::GetTimeIntervalRow(int iVert, std::deque<CRuleIntervalItem *>	**plInterval)
{
	int		iPos, iStartTime, iStopTime, iTimeStep;
	bool	bNewItem = false;
//	std::deque<CRuleIntervalItem *> *plInterval;
	CRuleIntervalItem *pRuleIntervalItem;

//	plInterval = *plIntervalOrg;

	iTimeStep = m_iSetHorzTimeHour * 60;

	iPos = iVert * m_iSetHorzTime;

	iStartTime = -1;
	iStopTime = -1;
	for (int iHorz = 0; iHorz < m_iSetHorzTime; iHorz++)
	{
		if (m_lSchedulerItems.at(iPos)->m_bActive)
		{
			if (iStartTime == -1)
			{
				iStartTime = iPos * iTimeStep;
			}
		}
		else
		{
			if (iStartTime != -1)
			{
				bNewItem = true;
				iStopTime = iPos * iTimeStep;
			}
		}

		if (iHorz == m_iSetHorzTime-1)
		{
			if (iStartTime != -1)
			{
				bNewItem = true;
				iStopTime = iPos * iTimeStep;
			}
		}

		if (bNewItem)
		{
			CRules rules;
//			int iDay, iHour, iMinute, iSecond;
//			rules.TimeExtract(iStartTime, &iDay, &iHour, &iMinute, &iSecond);
//			rules.TimeExtract(iStopTime, &iDay, &iHour, &iMinute, &iSecond);
			bNewItem = false;
			pRuleIntervalItem = new CRuleIntervalItem;
			pRuleIntervalItem->m_iStartTime = iStartTime;
			pRuleIntervalItem->m_iStopTime = iStopTime;
			(*plInterval)->push_back(pRuleIntervalItem);
			iStartTime = -1;
			iStopTime = -1;
		}
		iPos++;
	}
}

void CWndScheduler::InitInterval(std::deque<CRuleIntervalItem *> **plInterval)
{
//	std::deque<CRuleIntervalItem *> *plInterval;
//	plInterval = *plIntervalOrg;

	if ((*plInterval) == NULL)
	{
		(*plInterval) = new (std::deque<CRuleIntervalItem *>);
		return;
	}

	for (int iInterval = 0; iInterval < (int) (*plInterval)->size(); iInterval++)
	{
		delete (*plInterval)->at(iInterval);
	}
	(*plInterval)->clear();
}


BEGIN_MESSAGE_MAP(CWndScheduler, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE( )
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CWndScheduler message handlers

void CWndScheduler::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bUpdateSchedule)
	{
		CWnd *pWnd;
		pWnd = CWnd::GetParent();

		m_bUpdateSchedule = false;
		CSchedulerTime schedulerTime;
		for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
		{
			GetTime(iVert, &schedulerTime);
			pWnd->SendMessage(UWM_MSG_DLG_SETTINGS_BOINC_GRAPHIC,iVert, (LPARAM) &schedulerTime);
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CWndScheduler::OnPaint()
{
	CDC		dcMemory;
	HBITMAP memoryBitmap, memoryBitmapOld;

	CBrush brushBg;
	COLORREF bgColor =  RGB(255,255,255);

	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	dcMemory.CreateCompatibleDC(&dc);
	memoryBitmap = CreateCompatibleBitmap(dc.m_hDC, rect.Width(), rect.Height());
	memoryBitmapOld = (HBITMAP) SelectObject(dcMemory, memoryBitmap);

	brushBg.CreateSolidBrush(bgColor);			// background
	dcMemory.FillRect(rect,&brushBg);

	Draw(&bgColor,(CPaintDC*) &dcMemory,&rect);

	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);

	brushBg.DeleteObject();
	SelectObject(dcMemory, memoryBitmapOld);
	DeleteObject(memoryBitmap);
	DeleteDC(dcMemory);

	CWnd::OnPaint();
}


void CWndScheduler::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;

	if (m_bDisabled)
	{
		return;
	}

	GetClientRect(&rect);

	if (IsInBox(point, false))
	{
		if (!m_bMouseTracking)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = this->m_hWnd;
			if (::_TrackMouseEvent(&tme))
			{
				m_bMouseTracking = true;
			}
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CWndScheduler::OnMouseLeave()
{
	m_bMouseTracking = false;
	LeftWindow();

	CWnd::OnMouseLeave();

}

void CWndScheduler::OnLButtonDown(UINT nFlags, CPoint point)
{
	bool	bShift, bCtrl;

	if (m_bDisabled)
	{
		CWnd *pWnd;
		pWnd = CWnd::GetParent();
		pWnd->SendMessage(UWM_MSG_BUTTON, 0, 0);
		return;
	}

	if (nFlags & MK_SHIFT)
	{
		bShift = true;
	}
	else bShift = false;

	if (nFlags & MK_CONTROL)
	{
		bCtrl = true;
	}
	else bCtrl = false;

	Keys(point, bShift, bCtrl);
	if (IsInBox(point, true))
	{
		m_bSelected = true;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CWndScheduler::OnLButtonUp(UINT nFlags, CPoint point)
{
//	CWnd *pWnd;
//	CSchedulerTime schedulerTime;

	if (m_bSelected)
	{
		this->Invalidate();
		m_bSelected = true;
//		pWnd = CWnd::GetParent();

		m_bUpdateSchedule = true;

		if (!m_bTimerActivated)
		{
			m_bTimerActivated = true;
			SetTimer(UTIMER_WND_SCHEDULER, 2000, 0);
		}

//		for (int iVert = 0; iVert < m_iSetVertTime; iVert++)
//		{
//			GetTime(iVert, &schedulerTime);
//			pWnd->SendMessage(UWM_MSG_DLG_SETTINGS_BOINC_GRAPHIC,iVert, (LPARAM) &schedulerTime);
//		}
	}

	Keys(point, false, false);
	CWnd::OnLButtonUp(nFlags, point);
}





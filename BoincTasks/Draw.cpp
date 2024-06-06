// Draw.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorTemperature.h"
#include "DlgGraphicTemperature.h"
#include "Translation.h"
#include "Draw.h"


// CDraw

CDraw::CDraw()
{
}

CDraw::~CDraw()
{
}

// CDraw member functions


void CDraw::DrawGridHorizontal(COLORREF *plineColor, COLORREF *plineColor10, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom, int iStep, bool bDrawAll, CString sAfter)
{
	int iWidth,iHeight,iHeightGrid;
	int iCount;
	float fStep, fPos;

	CString	 sNr;

	CPen penLine,penLine10;
	penLine.CreatePen(PS_SOLID,1,*plineColor);
	penLine10.CreatePen(PS_SOLID,1,*plineColor10);

	iWidth = pRect->Width();
	iHeight= pRect->Height();

	iHeightGrid = iTop - iBottom;

	if (iTop <= iBottom)
	{
		return;			// error
	}

	fStep = (float) ((float) iHeight / (float) iHeightGrid);


//	pDc->SelectObject(&font);

	fPos = fStep;
	for (iCount = iTop-iStep; iCount > iBottom; iCount-=iStep)
	{
		int i10;
		i10 = iCount / 10;
		if (i10*10  == iCount)
		{
			// 10,20,30...
			pDc->SelectObject(&penLine10);
			pDc->MoveTo(0,(int) fPos);
			pDc->LineTo(iWidth,(int) fPos);
		}
		else
		{
			if (bDrawAll)
			{
				pDc->SelectObject(&penLine);
				pDc->MoveTo(0,(int) fPos);
				pDc->LineTo(iWidth,(int) fPos);
			}
		}

		fPos += fStep;
	}
	penLine.DeleteObject();	
	penLine10.DeleteObject();
}

void CDraw::DrawGridHorizontalNumbers(COLORREF *ptextColor, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom, int iStep, bool bDrawAll, int iLowest, CString sAfter)
{
	int iWidth,iHeight,iHeightGrid;
	int iCount;
	float fStep, fPos;

	CString	 sNr;
	
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

	iWidth = pRect->Width();
	iHeight= pRect->Height();

	iHeightGrid = iTop - iBottom;

	if (iTop <= iBottom)
	{
		return;			// error
	}

	fStep = (float) ((float) iHeight / (float) iHeightGrid);


	pDc->SelectObject(&font);

	fPos = fStep;
	for (iCount = iTop-iStep; iCount > iBottom; iCount-=iStep)
	{
		int i10;
		i10 = iCount / iLowest;

		if (i10*iLowest  == iCount)
		{
			// 10,20,30...
			sNr.Format(_T("%d"),iCount);
			sNr += sAfter; //°
			pDc->TextOut(0,(int) fPos-6,sNr);
		}

		fPos += fStep;
	}
	font.DeleteObject();
}

//
// Draw timebase grid
//

void CDraw::DrawGridVertical(COLORREF *plineColor, COLORREF *plineColor10, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, int iGridHorzTotal, int iGridHorzStep, int iGridHorzThickEvery, int iScaleHorzValue, int iScaleHorzValueStepping)
{
	int iWidthWindow,iHeightWindow,iGridThickEveryCount;
	int iCount, iScale;
	float fStep, fPos;
	CString	 sNr;

	CPen penLine,penLine10;
	penLine.CreatePen(PS_SOLID,1,*plineColor);
	penLine10.CreatePen(PS_SOLID,1,*plineColor10);

	iWidthWindow = pRect->Width();
	iHeightWindow= pRect->Height();

	fStep = (float) ((float) iWidthWindow / (float) iGridHorzTotal);

	iGridThickEveryCount = 0;
	iScale = 0;
	fPos = iWidthWindow-fStep;

	for (iCount = iGridHorzTotal-iGridHorzStep; iCount > 0; iCount-=iGridHorzStep)
	{
		iGridThickEveryCount++;
		if (iGridThickEveryCount == iGridHorzThickEvery)
		{
			iScale+=iScaleHorzValueStepping;
			// iGridThickEvery e.g. 10,20,30...
			pDc->SelectObject(&penLine10);

		}
		else
		{
			pDc->SelectObject(&penLine);
		}

		pDc->MoveTo((int) fPos,0);
		pDc->LineTo((int) fPos,iHeightWindow );

		if (iGridThickEveryCount == iGridHorzThickEvery)
		{
			// iGridThickEvery e.g. 10,20,30...
			iGridThickEveryCount = 0;
		}

		fPos -= fStep;
	}
	penLine.DeleteObject();
	penLine10.DeleteObject();
}

void CDraw::DrawGridVerticalNumbers(COLORREF *pTextColor, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, int iGridHorzTotal, int iGridHorzStep, int iGridHorzThickEvery, int iScaleHorzValue, int iScaleHorzValueStepping, int iSamplesPerWindow, time_t tReadTime)
{
	int iWidthWindow,iHeightWindow,iGridThickEveryCount;
	int iCount, iScale, iSkipEvery, iSkipEveryCount, iSkipBase, iSkip;
	float fStep, fPos, fTextSpace;

	CString	 sNr;
	time_t	 tStep;
	char	cBuffer[21];

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


	iWidthWindow = pRect->Width();
	iHeightWindow= pRect->Height();

	fStep = (float) ((float) iWidthWindow / (float) iGridHorzTotal);
	fTextSpace = fStep*iGridHorzThickEvery;

	iSkipBase = 0;
	if (fTextSpace < 50) iSkipBase++;
	if (fTextSpace < 25) iSkipBase++;
	if (fTextSpace < 16) iSkipBase++;
	iSkip = iSkipBase;

	pDc->SelectObject(&font);

	iGridThickEveryCount = 0;
	iScale = 0;
	fPos = iWidthWindow-fStep;


	iSkipEvery = (int) (10/fStep);
	iSkipEveryCount = 0;

	for (iCount = iGridHorzTotal-iGridHorzStep; iCount > 0; iCount-=iGridHorzStep)
	{
		iGridThickEveryCount++;
		if (iGridThickEveryCount == iGridHorzThickEvery)
		{
			iScale+=iScaleHorzValueStepping;
			// iGridThickEvery e.g. 10,20,30...
		}

		pDc->SetBkColor(*pBackgroundColor);
		pDc->SetTextColor(*pTextColor);

		if (iGridThickEveryCount == iGridHorzThickEvery)
		{
			// iGridThickEvery e.g. 10,20,30...
			iGridThickEveryCount = 0;
			sNr.Format(_T("%d"),iScale);
			if (sNr.GetLength() > 1)
			{
				pDc->TextOut((int) fPos-6,iHeightWindow-14,sNr);
			}
			else
			{
				pDc->TextOut((int) fPos-3,iHeightWindow-14,sNr);
			}

			if (iSkipEveryCount == 0)
			{
				if (tReadTime > 0 && iSkip == 0)
				{
					int		iStep;
					iStep = iSamplesPerWindow / iScaleHorzValue;
					tStep = tReadTime - (iStep*iScale);
					if (tStep >= 0)
					{
						struct tm   tmtime;
						localtime_s(&tmtime, &tStep);
						if (iSamplesPerWindow >= 3600)
						{
							strftime(cBuffer, 20, "%H:%M", &tmtime);
							pDc->TextOut((int) fPos-13, iHeightWindow-26, cBuffer);
						}
						else
						{
							strftime(cBuffer, 20, "%H:%M:%S", &tmtime);
							pDc->TextOut((int) fPos-20, iHeightWindow-26, cBuffer);
						}
					}
					iSkip = iSkipBase+1;
				}
				if (iSkip > 0) iSkip--;
			}
			iSkipEveryCount++;
			if (iSkipEveryCount > iSkipEvery) iSkipEveryCount = 0;
		}

		fPos -= fStep;
	}
	font.DeleteObject();
}

//
// Temperature of 1 Core
//


void CDraw::DrawTemperature(std::deque<float> *plTemperature,COLORREF CoreColor, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom, int iSamplesPerWindow, int iOffset, bool bTemperature, int iLineThickness )
{
	COLORREF txtColor = RGB(255,0,0);
	COLORREF bgColor = RGB(255,255,255);

//	POSITION pos;
	int	iPos;
	int iNr, iHeightTemperature, iWidth, iHeight, iCount, iScaleBack, iScaleBackCount;
	int iFont;
	int iSkip;
	float fTemperature, fHeightFactor, fStep, fPos, fHorzPos = 0;

	CPen penLine;
	penLine.CreatePen(PS_SOLID,iLineThickness,CoreColor);

	iWidth = pRect->Width();
	iHeight= pRect->Height();


	if (iWidth > iHeight) iFont = iHeight;
	else iFont = iWidth;
	iFont /= 14;
	if (iFont < 10) iFont = 14;

	CFont font;
	VERIFY(font.CreateFont(
	iFont,                     // nHeight
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


	// check if the screen width is enough, if not scale back

	iNr = (int) plTemperature->size();
	iNr -= iOffset;
	if (iNr <= 0) return;				// no data at all return

	iPos = 0;

	// skip to window
	iSkip = iNr - iSamplesPerWindow;
	iSkip--;
	if (iSkip < 0) iSkip = 0;
	while (iSkip && (iNr > 0))
	{
		iNr--;
		iSkip--;
		iPos++;
	}

	iScaleBack = iSamplesPerWindow / iWidth;
	iScaleBack++;
	iSamplesPerWindow /= iScaleBack;

	iHeightTemperature = iTop - iBottom;
	fHeightFactor = (float) iHeight / (float) iHeightTemperature;

	if (iTop <= iBottom)
	{
		return;			// error
	}

	iNr/= iScaleBack;

	if (iNr < 2)			// no sense to draw if less then 2 points
	{
		pDc->SelectObject(&font);
		pDc->SetBkColor(bgColor);
		pDc->SetTextColor(txtColor);
		pDc->TextOut((int) 10,iHeight/2,_T(" Please wait, not enough data yet "));
		return;
	}

	iScaleBackCount = iScaleBack-1;

	if (bTemperature) fTemperature = Convert(plTemperature->at(iPos));			// only if temperature
	else fTemperature = plTemperature->at(iPos);
	iPos++;

	while (iScaleBackCount > 0)	// scale back by combining samples
	{
		if (bTemperature) fTemperature += Convert(plTemperature->at(iPos));		// only if temperature
		else fTemperature += plTemperature->at(iPos);
		iPos++;
		iScaleBackCount--;
	}
	if (iScaleBack > 0) fTemperature /= iScaleBack;

	fTemperature-= iBottom;
	fPos =  fTemperature * fHeightFactor;

	fStep = (float) ((float) iWidth / (float) iSamplesPerWindow);

	fHorzPos = fStep * (float) (iSamplesPerWindow - (iNr-1));
	pDc->MoveTo((int) fHorzPos,iHeight - (int) fPos);
	pDc->SelectObject(&penLine);
	fHorzPos += fStep;

	for (iCount = 0; iCount < iNr-1; iCount++)
	{
		if (bTemperature) fTemperature = Convert(plTemperature->at(iPos));			// only if temperature
		else fTemperature = plTemperature->at(iPos);
		iPos++;
		iScaleBackCount = iScaleBack-1;
		while (iScaleBackCount > 0)	// scale back by combining samples
		{
			if (bTemperature) fTemperature += Convert(plTemperature->at(iPos));		// only if temperature
			else fTemperature += plTemperature->at(iPos);
			iPos++;
			iScaleBackCount--;
		}
		if (iScaleBack > 0) fTemperature /= iScaleBack;

		fTemperature-= iBottom;
		fPos = fTemperature * fHeightFactor;
		pDc->LineTo((int) fHorzPos,iHeight - (int) fPos);
		fHorzPos += fStep;
	}
	penLine.DeleteObject();
	font.DeleteObject();
}

void CDraw::MaxTemperatureLines(int iCpuMaxTemperature, int iGpuMaxTemperature, COLORREF *pCpuMaxColor, COLORREF *pGpuMaxColor, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom)
{
	int iHeightTemperature, iWidth, iHeight;
	float fHeightFactor, fPos, fHorzPos = 0;

	iWidth = pRect->Width();
	iHeight= pRect->Height();

	CPen penLineCpu, penLineGpu;
	penLineCpu.CreatePen(PS_SOLID,2,*pCpuMaxColor);
	penLineGpu.CreatePen(PS_SOLID,2,*pGpuMaxColor);

	iHeightTemperature = iTop - iBottom;
	fHeightFactor = (float) iHeight / (float) iHeightTemperature;

	iCpuMaxTemperature-= iBottom;
	fPos = iCpuMaxTemperature * fHeightFactor;

	pDc->MoveTo((int) 0,iHeight -1 - (int) fPos);
	pDc->SelectObject(&penLineCpu);
	pDc->LineTo((int) iWidth,iHeight - 1 - (int) fPos);

	iGpuMaxTemperature-= iBottom;
	fPos = iGpuMaxTemperature * fHeightFactor;

	pDc->MoveTo((int) 0,iHeight - 1 - (int) fPos);
	pDc->SelectObject(&penLineGpu);
	pDc->LineTo((int) iWidth,iHeight - 1 - (int) fPos);

	penLineCpu.DeleteObject();
	penLineGpu.DeleteObject();
}

//void CDraw::GetMaxMinI(CList<float> *pList,int *iMinReturn, int *iMaxReturn, int iSamplesPerWindow, int iPosition)
void CDraw::GetMaxMinI(std::deque<CTemperatureOneCore *> *plTemperatureCores, int iNrTemperatures, int *iMinReturn, int *iMaxReturn, int iSamplesPerWindow, int iPosition, bool bTemperature)
{
	float	fMin, fMax;
	int		iMin, iMax;
//	int		i10;

	fMin = 200;
	fMax = 0;
//	bTemperature = true; parameter

	if (iPosition >= TEMPERATURE_ARRAY) bTemperature = false;

	GetMaxMinF(&plTemperatureCores->at(iPosition)->m_lTemperatureCore,&fMin, &fMax, iSamplesPerWindow, bTemperature);

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


void CDraw::GetMaxMinF(std::deque<float> *plTemperature,float *piMin, float *piMax, int iSamplesPerWindow, bool bTemperature)
{
	float	fTemperature;
	int		iPos, iNr, iSkip;

	if (plTemperature->size() == 0)
	{
		return;
	}

	iNr = (int) plTemperature->size();
	iPos = 0;

	// skip to window
	iSkip = iNr - iSamplesPerWindow;
	iSkip--;
	if (iSkip < 0) iSkip = 0;
	while (iSkip)
	{
		iNr--;
		iSkip--;
		iPos++;
	}

	for (int iCount = 0; iCount < iNr; iCount++)
	{
		fTemperature = plTemperature->at(iPos);
		iPos++;
		if (bTemperature)	fTemperature = Convert(fTemperature);
	
		if (fTemperature > *piMax)
		{
			*piMax = fTemperature;
		}
		if (fTemperature < *piMin)
		{
			*piMin = fTemperature;
		}
	}
}


float CDraw::Convert(float fTemp)
{
//	if (m_bFahrenheit)
//	{
//		fTemp = Fahrenheit(fTemp);
//	}
	return fTemp;
}
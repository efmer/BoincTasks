// WndGrapic.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgGraphicTemperature.h"
#include "WndGraphicTemperature.h"
#include "draw.h"
#include "DlgColorTemperature.h"


// CWndGrapic

IMPLEMENT_DYNAMIC(CWndGraphicTemperature, CWnd)

CWndGraphicTemperature::CWndGraphicTemperature()
{
	m_iGridHorzTotal = 60;
	m_iGridHorzStep = 1;
	m_iGridHorzThickEvery = 10;
	m_iScaleHorzValue = 60;
	m_iScaleHorzValueStep = 10;
	m_iSamplesPerWindow = 60;

//	m_iShowCore	= SHOW_ALL_CORES;
//	m_iShowMode = SHOW_TEMP;

	m_pDraw	= new CDraw;

	for (int iCount=0; iCount < TEMPERATURE_ARRAY; iCount++)
	{
		m_bActive[iCount] = false;
	}

	m_iCpuMaxTemperature = -200;
	m_iGpuMaxTemperature = -200;

	m_tLastAddTime = 0;
	m_iOffset = 0;
	m_bConnected = false;
}

CWndGraphicTemperature::~CWndGraphicTemperature()
{
	delete	m_pDraw;

	for (int iCount=0; iCount < (int) m_temperatureGraphItem.m_lTemperatureCores.size(); iCount++)
	{
		delete m_temperatureGraphItem.m_lTemperatureCores.at(iCount);
	}
}

void CWndGraphicTemperature::SetConnected(bool bConnected)
{
	m_bConnected = bConnected;
//	this->Invalidate(TRUE);
}


BEGIN_MESSAGE_MAP(CWndGraphicTemperature, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWndGrapic message handlers


void CWndGraphicTemperature::OnPaint()
{
	bool	bHorzAll, bData;
	int		iMax,iMin;
	CDC		dcMemory;
	HBITMAP memoryBitmap, memoryBitmapOld;
	int		iArrayBeginPos;

	CDlgColorTemperature *pColorDlg;
	pColorDlg = theApp.m_pDlgColorTemperature;

	COLORREF coreColor[TEMPERATURE_ARRAY];
	COLORREF *pColorArray = pColorDlg->GetColorArray();

	COLORREF txtColor;
	COLORREF bgColor;
	COLORREF lineColor;
	COLORREF lineColor10;

	for (int iCount = 0; iCount < TEMPERATURE_ARRAY; iCount++)
	{
		coreColor[iCount] = *(pColorArray + iCount);
	}

	txtColor = *(pColorArray + COLOR_TEMPERATURE_ARRAY_TEXT);
	bgColor = *(pColorArray + COLOR_TEMPERATURE_ARRAY_BACK);

	lineColor =  *(pColorArray + COLOR_TEMPERATURE_ARRAY_GRID_FINE);
	lineColor10 =  *(pColorArray + COLOR_TEMPERATURE_ARRAY_GRID);

	CBrush brushBg;


	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	dcMemory.CreateCompatibleDC(&dc);
	memoryBitmap = CreateCompatibleBitmap(dc.m_hDC, rect.Width(), rect.Height());
	memoryBitmapOld = (HBITMAP) SelectObject(dcMemory, memoryBitmap);

	brushBg.CreateSolidBrush(bgColor);		// background
	dcMemory.FillRect(rect,&brushBg);

	int iSizeCores = (int) m_temperatureGraphItem.m_lTemperatureCores.size();

	if (iSizeCores == 0) bData = false;
	else bData = true;

	if (!m_bConnected) bData = false;



	if (bData)
	{
		iMin = 200;
		iMax = 0;

		iArrayBeginPos = 0;

		for (int iCount = 0; iCount < iSizeCores; iCount++)
		{
			if (m_bActive[iCount])
			{
				if (iCount == LIMIT_CPU) continue;
				if (iCount == LIMIT_GPU) continue;

				if (m_temperatureGraphItem.m_lTemperatureCores.at(iCount)->m_lTemperatureCore.size() > 2)	
				{
					m_pDraw->GetMaxMinI(&m_temperatureGraphItem.m_lTemperatureCores,0, &iMin, &iMax, m_iSamplesPerWindow, iCount);
				}
			}
		}

		// min not too close to the bottom depending on the scale and height of the graphic.
	
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
			int iDegreesPixel = (rect.Height() / (iMax - iMin10))+1;	// +1 otherwise 0 is possible
			int iDegreesFree = 60  / iDegreesPixel;
			if (iDegreesFree < 1) iDegreesFree = 1;
			iMin -= iDegreesFree;
		}

		if (iMax > 200) iMax = 200;
		if (iMin < 0)	iMin = 0;

		m_pDraw->DrawGridVertical(&lineColor, &lineColor10, &bgColor,(CPaintDC*)&dcMemory,&rect, m_iGridHorzTotal,m_iGridHorzStep,m_iGridHorzThickEvery,m_iScaleHorzValue,m_iScaleHorzValueStep);

		int iDelta = iMax - iMin;
		if (iDelta*6 > rect.Height()) bHorzAll = false;
		else bHorzAll = true;
	
		m_pDraw->DrawGridHorizontal(&lineColor, &lineColor10, (CPaintDC*)&dcMemory,&rect, iMax, iMin,1,bHorzAll,"°");	

		for (int iCount = 0; iCount < iSizeCores; iCount++)
		{
			if (m_bActive[iCount])
			{
				if (m_temperatureGraphItem.m_lTemperatureCores.at(iCount)->m_lTemperatureCore.size() > 2)	
				{
					m_pDraw->DrawTemperature(&m_temperatureGraphItem.m_lTemperatureCores.at(iCount)->m_lTemperatureCore,coreColor[iCount],(CPaintDC*) &dcMemory, &rect, iMax, iMin, m_iSamplesPerWindow, m_iOffset);
				}
			}
		}
		m_pDraw->DrawGridVerticalNumbers(&txtColor, &bgColor,(CPaintDC*)&dcMemory,&rect, m_iGridHorzTotal,m_iGridHorzStep,m_iGridHorzThickEvery,m_iScaleHorzValue,m_iScaleHorzValueStep, m_iSamplesPerWindow, m_tLastAddTime-m_iOffset);
		m_pDraw->DrawGridHorizontalNumbers(&txtColor, (CPaintDC*)&dcMemory,&rect, iMax, iMin,1,bHorzAll, 10, "°");	
	}
	else
	{
	}



	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);

	brushBg.DeleteObject();	
	SelectObject(dcMemory, memoryBitmapOld);
	DeleteObject(memoryBitmap);
	DeleteDC(dcMemory);

	CWnd::OnPaint();
}

void CWndGraphicTemperature::SetScale(bool bReset,int iGridHorzTotal,int iGridHorzStep,int iGridHorzThickEvery,int iScaleHorzValue,int iScaleHorzValueStep, int iSamplesPerWindow)
{
	m_iGridHorzTotal		= iGridHorzTotal;
	m_iGridHorzStep			= iGridHorzStep;
	m_iGridHorzThickEvery	= iGridHorzThickEvery;
	m_iScaleHorzValue		= iScaleHorzValue;
	m_iScaleHorzValueStep	= iScaleHorzValueStep;
	m_iSamplesPerWindow		= iSamplesPerWindow;
}

void CWndGraphicTemperature::Reset()
{
	for (int iCount=0; iCount < (int) m_temperatureGraphItem.m_lTemperatureCores.size(); iCount++)
	{
		m_temperatureGraphItem.m_lTemperatureCores.at(iCount)->m_lTemperatureCore.clear();
	}
	Invalidate(TRUE);
}

void CWndGraphicTemperature::SetCores(bool *pbActive)
{
	for (int iCount = 0; iCount < TEMPERATURE_ARRAY; iCount++)
	{
		m_bActive[iCount] = *pbActive;
		pbActive++;
	}
//	this->Invalidate(TRUE);
}

void CWndGraphicTemperature::AddTemperature(int iCore, float fTemperature, int iPos, time_t tLastTimeAdd)
{
	CTemperatureOneCore	*pListOneCore;

	while ((int) m_temperatureGraphItem.m_lTemperatureCores.size() <= iCore)
	{
		pListOneCore = new CTemperatureOneCore;
		m_temperatureGraphItem.m_lTemperatureCores.push_back(pListOneCore);
	}

	m_temperatureGraphItem.m_lTemperatureCores.at(iCore)->m_lTemperatureCore.push_back(fTemperature);

	while (m_temperatureGraphItem.m_lTemperatureCores.at(iCore)->m_lTemperatureCore.size() > MAX_SAMPLE_BUFFER_GRAPHIC_SCALABLE+1)
	{
		m_temperatureGraphItem.m_lTemperatureCores.at(iCore)->m_lTemperatureCore.erase(m_temperatureGraphItem.m_lTemperatureCores.at(iCore)->m_lTemperatureCore.begin());
	}
	m_temperatureGraphItem.m_tLastAddTime = tLastTimeAdd;
	m_iOffset = iPos*10;

	m_tLastAddTime = tLastTimeAdd;
	m_iOffset = iPos*10;
}

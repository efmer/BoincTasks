#pragma once



// CDraw command target

class CWndThreadGraph;
class CTemperatureOneCore;
class CWndGraphicTemperature;
class CWndGraphic;

typedef CList<float> temperatureList;
//typedef	CTypedPtrArray<CPtrArray,temperatureList*>	ListTypeArray;  

class CDraw : public CObject
{
friend CWndGraphicTemperature;
friend CWndGraphic;
friend CWndThreadGraph;

public: 
	CDraw();
	virtual ~CDraw();

protected:
	void	DrawGridHorizontal(COLORREF *plineColor, COLORREF *plineColor10, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom, int iStep, bool bDrawAll, CString sAfter);
	void	DrawGridHorizontalNumbers(COLORREF *ptextColor, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom, int iStep, bool bDrawAll, int iLowest, CString sAfter);
	void	DrawGridVertical(COLORREF *plineColor, COLORREF *plineColor10, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, int iGridTotal, int iGridStep, int iGridThickEvery, int iScaleValue, int iScaleValueStepping);
	void	DrawGridVerticalNumbers(COLORREF *pText, COLORREF *pBackground, CPaintDC *pDc,CRect *pRect, int iGridTotal, int iGridStep, int iGridThickEvery, int iScaleValue, int iScaleValueStepping, int iSamplesPerWindow, time_t tReadTime);
	void	DrawTemperature(std::deque<float> *plTemperature,COLORREF coreColor, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom, int iHorzStep, int iOffset, bool bTemperature = true, int iLineThickness = 1);	
	void	CoreColors(COLORREF *pTextColor, COLORREF *pCoreColor, COLORREF *pBackground, CPaintDC *pDc, CString sCPUGPU, CString sCore, CString sGpu, CString sCpuPerc, CString sGpuPerc);
	void	MaxTemperatureLines(int iCpuMaxTemperature, int iGpuMaxTemperature, COLORREF *pCpuMaxColor, COLORREF *pGpuMaxColor, CPaintDC *pDc,CRect *pRect, int iTop, int iBottom);
	void	GetMaxMinF(std::deque<float> *plTemperature,float *piMin, float *piMax, int iSamplesPerWindow, bool bTemperature = true);
	void	GetMaxMinI(std::deque<CTemperatureOneCore *> *plTemperatureCores,int iNrTemperatures, int *iMinReturn, int *iMaxReturn, int iSamplesPerWindow, int iPosition = -1, bool bTemperature = true );


	float	Convert(float fTemp);
};



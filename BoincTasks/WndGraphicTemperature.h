#pragma once

#include "ThreadRpcClient.h"

#define MAX_SAMPLE_BUFFER_GRAPHIC_SCALABLE	86400

#ifdef _NVAPI
// +2 GPU
#define MAX_GRAPHIC_CORES 8+2
#else
#define MAX_GRAPHIC_CORES 8
#endif

class CDraw;

// CWndGraphicTemperature

class CWndGraphicTemperature : public CWnd
{
	DECLARE_DYNAMIC(CWndGraphicTemperature)

public:
	CWndGraphicTemperature();
	virtual ~CWndGraphicTemperature();
	void	SetConnected(bool bConnected);

	void	SetScale(bool bReset,int iGridHorzTotal,int iGridHorzStep,int iGridHorzThickEvery,int iScaleHorzValue,int iScaleHorzValueStep,int iSamplesPerWindow);
	void	AddTemperature(int iCore, float fTemperature, int iPos, time_t tLastTimeAdd);
	void	SetCores(bool *pActive);
	void	Reset();

	int		m_iCpuMaxTemperature;
	int		m_iGpuMaxTemperature;

	bool	m_bActive[TEMPERATURE_ARRAY];

protected:
	CDraw	*m_pDraw;	

	afx_msg void OnPaint();

	CTemperatureGraphItem m_temperatureGraphItem;

	bool	m_bConnected;

	int		m_iGridHorzTotal;
	int		m_iGridHorzStep;
	int		m_iGridHorzThickEvery;
	int		m_iScaleHorzValue;
	int		m_iScaleHorzValueStep;
	int		m_iSamplesPerWindow;

	time_t	m_tLastAddTime;
	int		m_iOffset;

	DECLARE_MESSAGE_MAP()
};



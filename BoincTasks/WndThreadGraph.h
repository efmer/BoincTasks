#pragma once

class CComputerWndThreadListItem
{
public:
	COLORREF			m_color;
	int					m_iRepeat;
	time_t				m_tBegin;
	std::deque<float>	m_lThreadTime;
	std::deque<float>	m_lThreadTimeAverage;
};

// CWndThreadGraph

class CDraw;
class CWndThreadGraph : public CWnd
{
	DECLARE_DYNAMIC(CWndThreadGraph)

public:
	CWndThreadGraph();
	virtual ~CWndThreadGraph();
	void	SetConnected(bool bConnected);

	void	SetScale(bool bReset,int iGridHorzTotal,int iGridHorzStep,int iGridHorzThickEvery,int iScaleHorzValue,int iScaleHorzValueStep,int iSamplesPerWindow);
	void	AddComputer(std::deque<float> *plThreadTime, COLORREF color, int iRepeat, time_t tBegin);
	void	SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid, COLORREF colorGrid10);
	void	ShowComputers(int iComputers);
	void	Enable(bool bEnable);
	bool	IsPainting();

	void	Reset();

	int		m_iCpuMaxTemperature;
	int		m_iGpuMaxTemperature;

	bool	m_bActive[TEMPERATURE_ARRAY];

	afx_msg void OnPaint();


	bool	m_bConnected;

	int		m_iGridHorzTotal;
	int		m_iGridHorzStep;
	int		m_iGridHorzThickEvery;
	int		m_iScaleHorzValue;
	int		m_iScaleHorzValueStep;
	int		m_iSamplesPerWindow;

protected:
//	void	MinMax(CComputerWndThreadListItem *pComputerWndThreadListItem, double *pdMinTime, double *pdMaxTime, double *pdMinXfer, double *pdMaxXfer);
	void	Draw(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);

	void	GetMaxMinI(std::deque<float>  *plData, int *iMinReturn, int *iMaxReturn, int iSamplesPerWindow);
	void	Average(CComputerWndThreadListItem *plTime);

	bool	m_bEnable;
	bool	m_bPainting;

	int		m_iComputerCount;
	int		m_iComputers;

	int		m_iLineThickness;
	COLORREF m_colorBackGround;
	COLORREF m_colorScaleText;
	COLORREF m_colorGrid;
	COLORREF m_colorGrid10;

	bool	m_bAverage;

	std::deque<CComputerWndThreadListItem*> m_lComputer;

	CDraw	*m_pDraw;

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAverage(WPARAM parm1, LPARAM parm2);
};



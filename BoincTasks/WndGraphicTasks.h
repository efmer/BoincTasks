#pragma once

#include "ThreadRpc.h"
//#include "ComputerDoc.h"
#include "DlgLogging.h"

// CWndGraphicTasks

class CHistoryProjectTasksItem;
class CWndGraphicTasks : public CWnd
{
	DECLARE_DYNAMIC(CWndGraphicTasks)

public:
	CWndGraphicTasks();
	virtual ~CWndGraphicTasks();

	void	ResetProjects(void);
	void	AddProject(CString sComputer, CHistoryProjectTasksItem *pHistoryProjectTasksItem, COLORREF color);
	void	ShowProjects(int iComputers, int iProjects);
	void	SetType(int iUserHost, int iTotalAverage);
	void	Enable(bool bEnable, bool bDebug);
	bool	IsPainting();

	void	SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid, COLORREF colorAverage);

	bool	m_bAverage;

protected:
	void	DrawProjects(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawProjectsOneGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawProjectsSeperateGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);

	void	DrawText(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, CString sTxt);
	void	DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawTime(time_t	tTimeBaseline, int *piMin, int *piMax, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawTasksText(int *piMinTasks, int *piMaxTasks, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	MinMax(CHistoryProjectTasksItem *pHistoryProjectTasksItem, int *piMinTime, int *piMaxTime, int *piMinTasks, int *piMaxTasks);
	void	MinMaxTimeAdjust(time_t	tTimeBaseline, int *piMinTime, int *piMaxTime);
	void	DrawProjectName(CString sProject, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
//	void	DrawProjectText(CString sComputer, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawProject(int iPos, CHistoryProjectTasksItem *pProjectTasksItem, int *piMinVertical, int *piMaxVertical, int *piMinHorizontal, int *piMaxHorizontal,COLORREF *pColor, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, bool bAverage);

	int		m_iProjectCount;
	CHistoryProjectTasksItem *m_pProjectTasksItems[MAX_PROJECTS+1];
	CString m_sProjectComputer[MAX_PROJECTS+1];
	int		m_iProjectComputerNr[MAX_PROJECTS+1];
	COLORREF m_cProjectColor[MAX_PROJECTS+1];
	bool	m_bProjectDrawn[MAX_PROJECTS+1];
	int		m_bProjectToDraw[MAX_PROJECTS+1];

	bool	m_bProjectDrawnOne;
	int		m_bProjectToDrawOne;
	bool	m_bBlockMode;

	time_t	m_tTimeBaseline;

	COLORREF	m_colorBackGround;
	COLORREF	m_colorProjectText;
	COLORREF	m_colorScaleText;
	COLORREF	m_colorGrid;
	COLORREF	m_colorAverage;

//	int		m_iTimeAdjusted;			// adjusted to the right, to a whole day.

	int		m_iUserHost;
	int		m_iTotalAverage;

	int		m_iComputers;
	int		m_iProjects;

	int		m_iLineThickness;
//	int		m_iPeriod;

	CRect	m_iSquares[16];

	bool	m_bEnable;
	bool	m_bPainting;

	bool	m_bProjectsOneGraph;

	bool	m_bCpu;
	bool	m_bGpu;

	bool	m_bDebugMode;
	CLoggingMessage m_sLog;	

	afx_msg void OnPaint();
	afx_msg LRESULT OnLineThickness(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProjectsOne(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAverage(WPARAM parm1, LPARAM parm2);


	DECLARE_MESSAGE_MAP()
};



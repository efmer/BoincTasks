#pragma once

#include "ThreadRpc.h"
//#include "ComputerDoc.h"
#include "DlgLogging.h"

// CWndGraphic

class PROJECT;
class CWndGraphic : public CWnd
{
	DECLARE_DYNAMIC(CWndGraphic)

public:
	CWndGraphic();
	virtual ~CWndGraphic();

	void	ResetProjects(void);
	void	AddProject(CString sComputer, CString sProject, PROJECT *pProject, COLORREF color, int iRepeat);
	void	ShowProjects(int iComputers, int iProjects);
	void	SetType(int iUserHost, int iTotalAverage);
	void	Enable(bool bEnable, bool bDebug);
	bool	IsPainting();

	void	SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid);

	afx_msg void  OnMouseMove(UINT, CPoint);

protected:
	void	DrawProjects(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawProjectsOneGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawProjectsSeperateGraph(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);

	void	DrawText(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect, CString sTxt);
	void	DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawTime(double *pdMin, double *pdMax, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawCredits(double *pdMin, double *pdMax, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	MinMax(PROJECT *pProject, double *pdMinVertical, double *pdMaxVertical, double *pdMinHorizontal, double *pdMaxHorizontal);
	void	DrawProjectName(CString sProject, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
//	void	DrawProjectText(CString sComputer, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawProject(PROJECT *pProject, double *pdMinVertical,double *pdMaxVertical,double *pdMinHorizontal,double *pdMaxHorizontal,COLORREF *pColor, int *piRepeat, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	bool	InRect(DAILY_STATS	*pDs, CPoint *pPoint);

	double	GetCredits(PROJECT *pProject, int iPosition);

	int		m_iProjectCount;
	PROJECT *m_pProjects[MAX_PROJECTS+1];
	CString m_sProjectComputer[MAX_PROJECTS+1];
	CString m_sProjectProject[MAX_PROJECTS+1];
	int		m_iProjectComputerNr[MAX_PROJECTS+1];
	COLORREF m_cProjectColor[MAX_PROJECTS+1];
	int		m_iProjectRepeat[MAX_PROJECTS+1];
	bool	m_bProjectDrawn[MAX_PROJECTS+1];
	int		m_bProjectToDraw[MAX_PROJECTS+1];

	bool	m_bProjectDrawnOne;
	int		m_bProjectToDrawOne;

//	COLORREF	m_lineColors[MAX_COMPUTERS_REMOTE+1];

	COLORREF	m_colorBackGround;
	COLORREF	m_colorProjectText;
	COLORREF	m_colorScaleText;
	COLORREF	m_colorGrid;

	int		m_iUserHost;
	int		m_iTotalAverage;

	int		m_iComputers;
	int		m_iProjects;

	int		m_iLineThickness;

	int		m_iTimeToolTip;
	bool	m_btimerActive;

	CRect	m_iSquares[16];

	bool	m_bEnable;
	bool	m_bPainting;

	bool	m_bProjectsOneGraph;

	bool	m_bDebugMode;
	CLoggingMessage m_sLog;	

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnLineThickness(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProjectsOne(WPARAM parm1, LPARAM parm2);


	DECLARE_MESSAGE_MAP()
};



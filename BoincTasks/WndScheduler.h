#pragma once

#include "ThreadRpc.h"

//#define HORZ_TIME 48
//#define HORZ_TIME_HOUR 30
//#define VERT_TIME 7

//#define LEFT_TEXT 100
#define BOTTOM_TEXT 12

class CRuleIntervalItem;

class CSchedulerListItem
{
public:
	CRect	m_rBox;
	bool	m_bHoover;
	bool	m_bActive;
	bool	m_bForcedActive;
};

class CSchedulerTime
{
public:
	double	m_dStart;
	double	m_bStop;
};

// CWndScheduler

class CWndScheduler : public CWnd
{
	DECLARE_DYNAMIC(CWndScheduler)

public:
	CWndScheduler();
	virtual ~CWndScheduler();

	bool	IsInBox(CPoint point, bool bButton);
	void	LeftWindow();
	void	Keys(CPoint point, bool bShift, bool bCtrl);
	void	TimeSpanToWindow(int iDay, TIME_SPAN cpu);
	void	GetTime(int iVert, CSchedulerTime *pSchedulerTime);
	void	GetTimeInterval(std::deque<CRuleIntervalItem *>	**plInterval);
	void	Setup(int iSetHorzTime, int iSetHorzTimeHour, int iSetVertTime, bool bVertGrid, bool bDisable, bool bInvers=false);
	void	ClearInterval();
	void	AddInterval(int iStart, int iStop);

	bool	m_bForceActive;
protected:
	void	InitInterval(std::deque<CRuleIntervalItem *> **plInterval);
	void	GetTimeIntervalRow(int iVert, std::deque<CRuleIntervalItem *> **plInterval);

	DECLARE_MESSAGE_MAP()

	int		m_iSetHorzTime;
	int		m_iSetHorzTimeHour;
	int		m_iSetVertTime;
	bool	m_bDisabled;
	bool	m_bVertGrid;

	CRect	m_rSelection;
	CPoint	m_pointSelection;
	bool	m_bShiftKey;
	bool	m_bCtrlKey;

	bool	m_bMouseTracking;
	bool	m_bLeftButton;
	bool	m_bSelected;
	bool	m_bInvers;

	int		m_iWidthText;

	bool	m_bTimerActivated;
	bool	m_bUpdateSchedule;

	void Draw(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void DrawRect(int iPos, COLORREF *pBackgroundColor, CPaintDC *pDc, int iHorz, int iWidth, int iVert, int iHeight);
	int  DrawDay(COLORREF *pBackgroundColor, CPaintDC *pDc, int iHeight);
	void SelectRect();
	void CheckBeginEnd();

	std::deque<CSchedulerListItem *>	m_lSchedulerItems;
//	std::deque<C *>		*m_plInterval;

	afx_msg void OnPaint();
	afx_msg void  OnMouseMove(UINT, CPoint);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:


	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



#pragma once
#include <deque>
#include "ThreadRpc.h"
#include "DlgLogging.h"

#define SELECTION_RECT	6
#define SELECTION_RECT_INFLATE	2

class CGraphicXfer
{
public:
	double m_dTime;
	double m_dUpload;
	double m_dDownload;
	CRect  m_rectDown;
	CRect  m_rectUp;
};

class CComputerWndTransferListItem
{
public:
	COLORREF			m_color;
	int					m_iRepeat;
	std::deque<CGraphicXfer *> m_lXfer;
};

// CWndGraphicTransfer

class CWndGraphicTransfer : public CWnd
{
	DECLARE_DYNAMIC(CWndGraphicTransfer)

public:
	CWndGraphicTransfer();
	virtual ~CWndGraphicTransfer();

	void	Reset();
	void	SetDownUp(bool bShowDown, bool bShowUp);
	void	SetBlockMode(bool bBlockMode);
	void	AddComputer(DAILY_XFER_HISTORY *plXfer, COLORREF color, int iRepeat);
	void	SetColors(COLORREF colorBackGround, COLORREF colorProjectText, COLORREF colorScaleText, COLORREF colorGrid);
	void	ShowComputers(int iComputers);
	void	Enable(bool bEnable, bool bDebug);
	bool	IsPainting();
	bool	InRect(CRect *pRect, CPoint *pPoint);
protected:
	void	PreSubclassWindow() ;
	void	CreateTimeLine(int iPos, DAILY_XFER_HISTORY *plXfer);
	void	MinMaxTime(int iPos, DAILY_XFER_HISTORY *plXfer, int *piTimeMin, int *piTimeMax);
	void	Draw(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawRect(COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawXfer(std::deque<CGraphicXfer *> *pXfer, double *pdMinTime,double *pdMaxTime,double *pdMinCredits,double *pdMaxCredits,COLORREF *pColor, bool bDown, int *piRepeat, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawTimeText(double *pdMinTime, double *pdMaxTime, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawXferText(double *pdMinXfer, double *pdMaxXfer, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	DrawToolTip(std::deque<CGraphicXfer *> *pXfer, double *pdMinTime,double *pdMaxTime,double *pdMinCredits,double *pdMaxCredits,COLORREF *pColor, bool bDown, int *piRepeat, COLORREF *pBackgroundColor, CPaintDC *pDc,CRect *pRect);
	void	MinMax(std::deque<CGraphicXfer *> *pXfer, double *pdMinTime, double *pdMaxTime, double *pdMinXfer, double *pdMaxXfer);
	bool	m_bEnable;
	bool	m_bPainting;

	int		m_iComputerCount;
	int		m_iComputers;

	bool	m_bShowDown;
	bool	m_bShowUp;
	bool	m_bBlockMode;

	int		m_iLineThickness;
	COLORREF m_colorBackGround;
	COLORREF m_colorProjectText;
	COLORREF m_colorScaleText;
	COLORREF m_colorGrid;

	std::deque<CComputerWndTransferListItem*> m_lComputer;

	bool	m_bDebugMode;
	CLoggingMessage m_sLog;	

	int		m_iTooltipDown;
	int		m_iTooltipUp;

	DECLARE_MESSAGE_MAP()

	afx_msg void  OnMouseMove(UINT, CPoint);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnLineThickness(WPARAM parm1, LPARAM parm2);
};



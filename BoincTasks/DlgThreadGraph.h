#pragma once

#include "ColorListBox.h"
#include "WndThreadGraph.h"

#define BOINCTASKS_PROCESS_TIMING_NAME	"_BoincTasks"

class CThreadRpc;

class CComputerThreadGraphicListItem
{
public:
	CThreadRpc	*m_pThreadRpc;
	int			m_ThreadRpcId;
	char		*m_pcThreadRpcId;

	std::deque<float> m_lThreadTime;
};


// CDlgThreadGraph dialog
class CThreadProcessTime;
class CRemoteCombinedDoc;
class CDlgThreadGraph : public CDialog
{
	DECLARE_DYNAMIC(CDlgThreadGraph)

public:
	CDlgThreadGraph(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgThreadGraph();
	void	SafeWindowPlacement();
	void	InitialUpdate(CRemoteCombinedDoc *pDoc);

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPHIC_THREAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	void		ShowControls(bool bShow);
	void		DisplayStatus(int iStatus);
	void		UpdateGraphic();
	void		AddComputer(CString sComputer, COLORREF color, int iRepeat, time_t tBegin);
	void		Reset();

	HICON		m_hIcon;
	CWndThreadGraph m_window;
	int			m_iWindowInitialLeftMargin;
	int			m_iWindowInitialTopMargin;

	int			m_iAddComputer;
	int			m_iNrThreadReturn;

	CRemoteCombinedDoc *m_pDoc;

	bool		m_bStartUpdate;
	bool		m_bShowWindow;
	int			m_iShowWindowDelay;

	bool		m_bInvalidate;
	int			m_iTimer;
	int			m_iTimerEnd;
	bool		m_bThreadBusy;

	bool		m_bUpdate;

	int			m_iLineThickness;
//	bool		m_bShowUp;
//	bool		m_bShowDown;
//	bool		m_bBlockMode;
	bool		m_bCheckAverage;

	bool		m_bTimerActive;

	bool		m_bFullScreenGraph;
	LONG		m_lLastWidth;
	LONG		m_lLastHeight;

	std::deque<CComputerThreadGraphicListItem*>	m_lComputer;
	
	time_t		m_tTime;

	CThreadProcessTime *m_pThreadProcessTime;

	CColorListBox m_listComputers;
	CComboBox m_setTime;
	CButton		m_checkAverage;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void  OnMouseMove(UINT, CPoint);
	afx_msg void  OnShowWindow(BOOL bShow, UINT nStatus);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
	afx_msg LRESULT OnInitialGraphic(WPARAM parm1, LPARAM parm2);
	afx_msg	LRESULT OnCloseGraphic(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnGraphicsReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColorChanged(WPARAM parm1, LPARAM parm2);

	afx_msg void OnLbnSelchangeListGraphicComputers();
	afx_msg void OnCbnSelchangeGraphicComboTime();
	afx_msg void OnBnClickedCheckGraphicAverage();
};

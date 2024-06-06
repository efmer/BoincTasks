#pragma once

#include "WndGraphicDeadline.h"
#include "ColorListBox.h"
#include "afxwin.h"


class CComputerGraphicDeadlineListItem
{
public:
	CThreadRpc	*m_pThreadRpc;
	int			m_ThreadRpcId;
	char		*m_pcThreadRpcId;
	CString		m_sThreadRpcUrl[MAX_PROJECTS+1];
	CString		m_sThreadRpcName[MAX_PROJECTS+1];
	std::deque<CHistoryProjectDeadlineItem*> *m_plHistoryProjectDeadline;
	bool		m_bAdjustTime;
};

// CDlgGraphicDeadline dialog

class CRemoteCombinedDoc;
class CHistoryProjectDeadlineItem;
class CThreadRpc;
class CDlgGraphicDeadline : public CDialog
{
	DECLARE_DYNAMIC(CDlgGraphicDeadline)

public:
	CDlgGraphicDeadline(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraphicDeadline();
	void	InitialUpdate(CRemoteCombinedDoc *pDo, bool bShowWindow = false);
	void	SafeWindowPlacement();
	void	ResetProjects();
	void	UpdateGraphic();
	void	CheckOneGraph();
	void	AddProject(CString sComputer, CString sProject, COLORREF color);

	int			m_iExpand;

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPHIC_DEADLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
			void ShowControls(bool bShow);
			void PostCommandView(int iCommand);
			void GetAverage(std::deque<CHistoryProjectDeadlineItem*> *plHistoryProjectDeadlineItem, int *piDay, int *piWeek);

	CBitmap m_bmp;

	bool		m_bStartUpdateStatistics;	

	bool		m_bClosing;
	bool		m_bClosingReady;
	bool		m_bShowWindow;
	int			m_iShowWindowDelay;

	std::deque<CComputerGraphicDeadlineListItem*> m_lComputer;

	CRemoteCombinedDoc *m_pDoc;

	int			m_iNrThreadRpc;
	int			m_iNrThreadReturn;

	bool		m_bInvalidate;
	int			m_iTimer;
	int			m_iTimerEnd;
	bool		m_bThreadBusy;

	bool		m_bProjectsOneGraph;
	bool		m_bCheckCombineProjects;
	bool		m_bCheckPeriod;
	bool		m_bCheckAverage;

	int			m_iPeriod;

	bool		m_bBlockMode;

	bool		m_bCpu;
	bool		m_bGpu;

	bool		m_bAdjustTime;

	HICON		m_hIcon;
//	CBitmap		m_bmp;
	CWndGraphicDeadline m_window;
	int			m_iWindowInitialLeftMargin;
	int			m_iWindowInitialTopMargin;

	CButton		m_buttonUpdate;
	CButton		m_checkCombineProject;
	CButton		m_checkPeriod;
	CButton		m_checkModeBlock;
	CButton		m_checkSingleMultiple;
	CButton		m_checkAverage;
	CButton		m_checkExpanded;

	CEdit		m_editLineThickness;
	CEdit		m_editPeriod;

	CStatic		m_textLineThickness;
//	CStatic		m_textDebugMode;
	CStatic		m_textComputers;
	CStatic		m_textProjects;
	CStatic		m_textPeriod;

	CColorListBox m_listProjects;
	CColorListBox m_listComputers;

	CButton		m_checkCpu;
	CButton		m_checkGpu;

	int			m_iLineThickness;

	bool		m_bFullScreenGraph;
	LONG		m_lLastWidth;
	LONG		m_lLastHeight;

//	CString		m_sMagic;
//	bool		m_bDebugMode;

	bool		m_bTimerActive;

//	CLoggingMessage m_sLog;	

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
	afx_msg LRESULT OnGraphicsDeadlineReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnInitialGraphic(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColorChanged(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnCloseGraphic(WPARAM parm1, LPARAM parm2);

	afx_msg void OnLbnSelchangeListGraphicComputers();
	afx_msg void OnLbnSelchangeListGraphicProjects();
	afx_msg void OnBnClickedButtonUpdate();

	afx_msg void OnEnChangeEditGraphicLine();
	afx_msg void OnBnClickedCheckGraphicProjectsSame();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnBnClickedCheckGraphPeriod();

	afx_msg void OnEnChangeEditGraphicPeriod();

	afx_msg void OnBnClickedCheckGraphicBlock();

	afx_msg void OnBnClickedCheckGraphicSingleMultiple();
	afx_msg void OnBnClickedCheckGraphicAverage();

	afx_msg void OnBnClickedCheckGraphicExtended2();

	afx_msg void OnBnClickedCheckGraphicCpu();
	afx_msg void OnBnClickedCheckGraphicGpu();
	CButton m_buttonHelp;
	afx_msg void OnBnClickedHelp();
};

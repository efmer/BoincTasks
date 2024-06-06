// MainFrm.h : interface of the CMainFrame class
//

#define TASKBAR_ICON_SUSPEND_GPU 2
#define TASKBAR_ICON_SUSPEND	1
#define TASKBAR_ICON_NORMAL		0

#define MSGFLT_ADD 1
#define MSGFLT_REMOVE 2

#pragma once

#include "DlgBarViewSelect.h"
#include "DlgBarComputer.h"
#include "DlgBarProject.h"
#include "TToolbar.h"
#include "ThreadRpc.h"

//#define ID_INDICATOR_VERSION  1

#define	USER_SWITCHING "User switching"
typedef int (__cdecl *PROC_SHUTDOWN)(HWND, LPCWSTR); 
typedef BOOL (WINAPI *LPFN_CHANGEWINDOWMESSAGEFILTER) (UINT, DWORD);
static UINT_PTR	WM_TASKBARCREATED;

class CFloaterItem
{
public:
	CString m_sComputer;
	CString m_sProjectName;
	CString m_sUserFriendly;
	CString m_sPlannClass;
	CString m_sUse;

	CString m_sCpu;
	CString m_sGpu;

	double	m_dPerc;
	double	m_dRemainingTime;
	double	m_dReportDeadline;
	int		m_iVersion;
	int		m_iState;	

	bool	m_bWarning;
};


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual	BOOL PreTranslateMessage(MSG *pMsg);

//	BOOL	AddToComputersToolbar(char *szStrings, int buttons);
	void	DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
	void	DockControlBarLeftOf(CDlgBarViewSelect* Bar, CToolBar* LeftOf);
	void	ReplaceBackgroundColor (CBitmap& ioBM);
	void	ChangeMessageFilter();

// Attributes
public:

	bool	m_bIsClosing;
	int		m_iClosingCount;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void GetMessageString( UINT nID,  CString& rMessage) const ;

	CTToolBar			m_wndToolBarGraphic;
	CTToolBar			m_wndToolBarOperations;
	CTToolBar			m_wndToolBarAllow;
//	CToolBar			m_wndToolBarComputers;
	CDlgBarComputer		m_dlgBarComputer;
	CDlgBarProject		m_dlgBarProject;
	CDlgBarViewSelect	m_dlgBarViewSelect;

	CImageList			m_imagelistToolbar;
	CImageList			m_imagelistToolbarDisabled;
	CImageList			m_imagelistToolbarOperations;
	CImageList			m_imagelistToolbarOperationsDisabled;
	CImageList			m_imagelistToolbarAllow;
	CImageList			m_imagelistToolbarAllowDisabled;

	//	CToolBarCtrl m_wndToolBarComputers;

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void	SetBoincTasksStatus(char *pszStatus);
	CStatusBar* GetBoincTasksStatus();

	int			m_iLocalHostThreadId;
	std::string m_sLocalHostThreadId;

//	CString m_sToolTipInfo;

	int		m_iSecondTimer;

	bool	m_bFloaterOverIcon;
	bool	m_bFloaterShown;
	int		m_iFloaterTime;
	int		m_iFloaterHideTime;
	int		m_iFloaterHideOverride;
	time_t	m_tFloaterFirst;

	CRect	m_rectOverIcon;
	bool	m_bCursorIsOverIcon;

	time_t	m_tNextSnoozeUpdate;

	int		m_iHistoryStateReadyCount;

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CLoggingMessage *m_pLogging;
	char			*m_pcBufferLogFrom;
	bool	SetPriority(DWORD dwOwnerPID, DWORD dwPriority) ;
	void	Logging(char *pcFrom, char *pcTxt, int iLoggingType);
	void	SendWindowShow();
	void	SafeToolBar();
	void	CloseAllRemote();

	void	RefreshTaskbarNotificationArea();
	void	WriteToolbar(void);
	void	RemoveToolbar(void);

	BOOL	VerifyBarState(LPCTSTR lpszProfileName, int iVersion);

	time_t	m_tGetToolTipData;
	int		m_iGetTooltipData;
	void	SetTaskbarIcon(int iIcon);
	void	TaskbarMenu(void);
	BOOL	ToolbarShowBalloon(char *szBalloonTitle, PTSTR szBalloonMsg, DWORD dwIcon, UINT nTimeOut);
	afx_msg	LRESULT  OnTaskbar(WPARAM wParam,LPARAM lParam);
//	afx_msg	LRESULT  OnViewTile(WPARAM wParam,LPARAM lParam);
				
	HICON	m_hIconNormal;
	HICON	m_hIconRed;
	HICON	m_hIconOrange;
	
	BOOL	m_bIconStatus;
	int		m_iIconCounter;

//	CString	m_statusForWebServer;

	bool	m_bForceRestart;

	NOTIFYICONDATA	m_tnid;
	int		m_iCurrentIcon;
	int		m_iBlinkIcon;

	int		m_iRedrawIconCounter;

	bool	m_bDoOnce;
	bool	m_bDoOncePosition;

	bool	m_bSnoozeCpu;
	bool	m_bSnoozeCpuEnable;
	bool	m_bSnoozeGpu;
	bool	m_bSnoozeGpuEnable;

	int		m_iBalloonTimeout;

	int		m_iResized;

	bool	m_bQueryClosingDown;

	CC_STATUS	m_cCStatusCpu;
	CC_STATUS	m_cCStatusGpu;


// Generated message map functions
protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg LRESULT OnForceRestart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onGarbageCollector(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onGarbageCollectorReady(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnThreadId(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDocumentIsClosing(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDocumentHasClosed(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnThreadPriority(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnWUsThatAreReady(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetFloater(WPARAM wParam,LPARAM lParam);
	afx_msg	LRESULT OnGetFloaterDataReady(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeCpuReady(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeGpuReady(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnNoticesAlert(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnNewTaskbarCreated(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnActivateTaskbar(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnThreadThreadConnectReady(WPARAM wParam, LPARAM lParam); 

	afx_msg LRESULT OnWtsSessionChange(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnExit(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTaskbarOpen();
	afx_msg void OnTaskbarUpdate();
	afx_msg void OnTaskbarAbout();
	afx_msg void OnTaskbarSnoozeCpu();
	afx_msg void OnTaskbarSnoozeGpu();
	afx_msg void OnTaskbarExit();

	afx_msg void OnComputersShow();
	afx_msg void OnUpdateComputers(CCmdUI *pCmdUI);
	afx_msg void OnShowCombinedtasks();
	afx_msg void OnUpdateShowCombinedtasks(CCmdUI *pCmdUI);
	afx_msg void OnViewViewselectionbar();
	afx_msg void OnUpdateViewViewselectionbar(CCmdUI *pCmdUI);
	afx_msg void OnViewOperationToolbar();
	afx_msg void OnUpdateViewOperationToolbar(CCmdUI *pCmdUI);
	afx_msg void OnViewAllowToolbar();
	afx_msg void OnUpdateViewAllowToolbar(CCmdUI *pCmdUI);
	afx_msg void OnViewGraphicToolbar();
	afx_msg void OnUpdateViewGraphicToolbar(CCmdUI *pCmdUI);


	afx_msg void OnUpdateTaskbarSnoozeCpu(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTaskbarSnoozeGpu(CCmdUI *pCmdUI);


	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg BOOL OnQueryEndSession();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



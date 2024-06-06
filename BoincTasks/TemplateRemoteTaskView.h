

#pragma once
#include <deque>
#include "ThreadRpc.h"
#include "TasksProperties.h"
#include "ThreadRpcClient.h"
#include "listviewex.h"
#include "Selection.h"
#include "DlgLogging.h"
#include "Rules.h"

//#include "computerDoc.h"

//static	int CALLBACK TaskSorting(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

//#define MAX_TASK_DATA				(MAX_COMPUTERS_REMOTE*(MAX_WU+10))
#define NUM_REMOTE_TASK_COLUMNS 20


/*
class CTaskPropertiesItem
{
public:
	CString				m_sComputerId;
	CString				m_sProject;
	int					m_iThreadId;
	int					m_iFoundCount;
};
*/



class CRpcInfoTasksItem
{
public:
	int		m_iNrWu;
	int		m_iThreadId;
	char	*m_pcComputerId;
	CThreadRpc *m_pThread;
	int		m_iStatusRpc;
	CFilterData	*m_pFilter;
	int		m_iFilterTotalVersion[MAX_FILTER_TYPE+1];
	int		m_iFilterTotalWu[MAX_FILTER_TYPE+1];
	int		m_iSortIndex;
	double	m_dDiskInterval;
};

class CSelectedTaskItem
{
public:
	CString m_sSelectedTaskItemsComputer;
	CString m_sSelectedTaskItemsName;
	CString m_sSelectedTaskItemsPerc;
	CString m_sSelectedTaskItemsState;
	CString m_sSelectedTaskItemsDeadline;
	CString m_sSelectedTaskItemsProject;
	CString m_sSelectedProjectItemsComputer;
	CString m_sSelectedProjectItemsName;
	CString m_sSelectedProjectItemsPerc;
	CString m_sSelectedProjectItemsState;
	__int64 m_i64FilterApplicationAndVersion;
};

class CCheckpointSuspend
{
public:
	CString m_sComputer;
	CString m_sName;
	CString m_sProject;
	__int64 m_i64FilterApplicationAndVersion;
	int		m_iCheckpoints;
	bool	m_bUsed;
	bool	m_bDelete;
};

class CRatioItem
{
public:
	CString m_sProject;
	CString m_sWu;
	double	m_dRatio;
	double	m_dCurrentCpuTime;
	double	m_dCurrentCpuTime5;
	double	m_dCurrentCpuTime10;
	double	m_dElapsedTime;
	double	m_dElapsedTime5;
	double	m_dElapsedTime10;
	int		m_iSampleCount;
	int		m_iFallBackMode;
	time_t	m_tLastSystemTime;
	time_t	m_tLastSystemTime5;
	time_t	m_tLastSystemTime10;
	bool	bWuFound;
};

class CCheckpointItem
{
public:
	CString		m_sProject;
	CString		m_sApplication;
	COLORREF	m_crColor;
	int			m_iTime;
};

class CCheckpointHistoryItem
{
public:
	CString		m_sProject;
	CString		m_sApplication;
	CString		m_sName;
	int			m_iCheckpoint;
	int			m_iCheckpoints;
};

//#define NUM_REMOTE_TASK_COLUMNS 8

// CTemplateRemoteTaskView view

class CTasksProperties;
class CThreadRpc;
class CRpcTaskInfo;
class CFilterData;
class CComputerRemoteDoc;
class CRemoteCombinedDoc;
class CDlgTimer;
class CSortColumn;
class CSelection;
class CCreateHtmlWebServerProperties;
class CTemplateRemoteTaskView : public CListViewEx
{
	friend class CRemoteCombinedDoc;
	DECLARE_DYNCREATE(CTemplateRemoteTaskView)

public:
	CTemplateRemoteTaskView();           // protected constructor used by dynamic creation
	virtual ~CTemplateRemoteTaskView();
			void DeleteTaskProperties();
	virtual void OnInitialUpdate(); // called first time after construct

	void	DeleteSelectedTaskItems();
	void	DeleteSelectedProjectItems();
//	void	DeleteSelectedGraphicItems;

//	CImageList m_StateImageList;
public:
//	void	RpcReady(int iNrWu);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	void	ColumnChanged();
	void	ReadXml();
	void	CheckPointSuspend(int iPosHistory, int iPosSuspend);

	int		m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1]; 
	int		m_iColumnWidth[NUM_REMOTE_TASK_COLUMNS+1];		// for xchange only
	bool	m_bApplyChangesOnColumn;
	int		m_iNrColomns;

	bool	m_bCombinedMode;
	bool	m_bRatioLongTerm;

	UINT	m_iMenuResource;

//	HWND	m_hWndPreference;
	HWND	m_hWndProxy;

	CSelection *m_pSelection;
	CSortColumn	*m_pSortColumn;

// computer
	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;
	int		m_iWuTotal;
	int		m_iWuTotalInList;

	std::deque<CRpcInfoTasksItem*>	m_lRpcInfo;
	std::deque<CRpcInfoClient*>		m_lRpcInfoClient;

	std::deque<CTasksProperties*>	m_lListTasksProperties;
// computer

	std::deque<int>	m_lListStatusSorting;
	int				m_iListStatusSorting;

	time_t	m_tLastSwitchedTime;
	time_t	m_tLastTasksShown;
	time_t	m_tLastTasksWWW;
	time_t	m_tLastHostInfo;
	time_t	m_tLastAccountManagerInfo;
	time_t	m_tLastSnoozeInfo;
	time_t	m_tLastMessageUpdate;
	time_t	m_tNextRefreshAuto;
	time_t	m_tNextRefreshDownloads;
	time_t	m_tNextRefreshUploads;
	time_t	m_tNextRefresh;
	time_t	m_tNextASAP;
	bool	m_bTasksWWW;

	CRemoteCombinedDoc	*m_pDoc;

	void	AddAllColumns(bool bAdd = true);
	void	ReadAllColumnsWidth();
	int		ReadColumnWidth(int iColumn, int iDefault);
	void	CheckAndSaveColumnsWidth();

//	bool	m_bTaskPropertiesBusy;

	int		m_iTaskPropertiesBusy;


	bool	DoubleClick(int iRow, int iControlKey);

	void				TaskPropertiesGet(int iBusy);
	void				DeletePropertiesList();
	void				TaskProperties(CTasksProperties *pTasksProperties, CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties = NULL);
	afx_msg LRESULT		TaskPropertiesGetReady(WPARAM parm1, LPARAM parm2);

//	std::deque<CTaskPropertiesItem*>		m_lTaskProperties;
//	CString				m_sTaskPropertiesComputerId[MAX_SELECTED_PROPERTIES+1];
//	CString				m_sTaskPropertiesProject[MAX_SELECTED_PROPERTIES+1];
//	CString				m_sTaskPropertiesName[MAX_SELECTED_PROPERTIES+1];
//	int					m_iTaskPropertiesThreadId[MAX_SELECTED_PROPERTIES+1];
//	int					m_iTaskPropertiesFoundCount;
	int					m_iTaskPropertiesUniqueThreadsFoundCount;
	int					m_iTaskPropertiesUniqueThreadId;
	int					m_iTaskPropertiesThreadsPostedCount;
	int					m_iTaskPropertiesThreadsReturnCount;



//Warning these items are members because they have to stay valid for some time
	std::deque<CSelectedTaskItem*> m_lSelectedTaskItems;
	std::deque<CSelectedTaskItem*> m_lSelectedProjectItems;
//	std::deque<CSelectedTaskItem*> m_lSelectedGraphic;
	
//	CString m_sSelectedTaskItemsComputer[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedTaskItemsName[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedTaskItemsPerc[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedTaskItemsState[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedTaskItemsDeadline[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedTaskItemsProject[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedProjectItemsComputer[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedProjectItemsName[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedProjectItemsPerc[MAX_SELECTED_TASKS+1];
//	CString m_sSelectedProjectItemsState[MAX_SELECTED_TASKS+1];
//	long long	m_llFilterApplicationAndVersion[MAX_SELECTED_TASKS+1];
// ----------------------------------------------------------------

	void	GetRules();
//	XList <CActiveRuleItem*,CActiveRuleItem*> *m_plRulesActive;
	std::deque<CActiveRuleItem*> *m_plRulesActive;
	
	void	DisplaySingleItem(CListCtrl *pListCtrl, int iRow, CRpcTaskInfo *pInfo, double dDays, int iComputerClient, char *pcComputerName, int iComputerId, bool bFilter = false);
	void	DisplayFiltered(CListCtrl *pListCtrl, int *piRow, CFilterData *pFilterData, int iComputerClient, char *pcComputerName, int iComputerId);
	void	DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin);

	void	CheckWarning(COLORREF *pColorSet, COLORREF *pColorCpu, COLORREF *pColorGpu,
						double fRatioPerc, double fCpuUse, double fGpuUse,
						double fWarningCpuLow1, double fWarningCpuHigh1,
						double fWarningGpuLow1, double fWarningGpuHigh1,
						double fWarningPercLow1,double fWarningPercHigh1);

	bool	CheckCheckpointWarning(double dCheckpointRelative, char *pszProject, char *pszApplication, int *piColor);
	void	CheckPointAddCounter(CString *psTxt, int iCheckPoint, char *pszProject, char *pszApp, char *pszName);
//	void	TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore=NULL, char *pAfter=NULL, bool bShowZero=false);

//	XList	<CRatioItem*,CRatioItem*> m_lRatio;
	std::deque<CRatioItem*> m_lRatio;
	void	StartRatio();
	double	GetRatio(CRpcTaskInfo *pInfo);
	void	StopRatio();

//	CList	<CCheckpointItem*,CCheckpointItem*> m_lCheckpoint;
	std::deque<CCheckpointItem*> m_lCheckpoint;

//	int		m_iFilterEnabled;

	int		m_iFilterEnabledWaitingToRun;
	int		m_iFilterEnabledReadyToReport;
	int		m_iFilterEnabledReadyToStart;
	int		m_iFilterEnabledUpload;
	int		m_iFilterEnabledDownload;
	int		m_iFilterEnabledSuspend;
	int		m_iFilterEnabledAborted;
	int		m_iFilterEnabledRunning;
	int		m_iFilterEnabledComputation;

	int		m_iShowActiveTasks;
	int		m_iShowCpu;
	int		m_iShowGpu;
	int		m_iShowNonCpuIntensive;

	CString	m_sCloudColumn;

	std::deque<CListData*> m_lListData;
//	CListData *m_pData[MAX_TASK_DATA+1];
	CListData m_dataDummy;

	CLoggingMessage *m_pLog;	
	CWnd		*m_pLoggingDialog;

	int			m_iDragIndex;
	CImageList	*m_pDragImage;

	CRuleItem	m_ruleItemAdd;

	bool		m_bDraged;

	std::deque<CCheckpointHistoryItem*> m_lCheckpointHistory;
	std::deque<CCheckpointSuspend*> m_lCheckpointSuspend;


	void				ClipBoard(bool bAll);

	afx_msg LRESULT OnHistoryTasks(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColumnChanged(WPARAM parm1, LPARAM parm2);
	afx_msg void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg BOOL OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHeaderChanged(UINT, NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnSettingsApplyChangesTasks(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnTaskPropertiesReady(WPARAM parm1, LPARAM parm2);
	void PropertiesAddRow(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties, CString *psColumn0, CString *psColumn1);
	void TaskPropertiesReady(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties);

	afx_msg LRESULT OnCloudColumn(WPARAM parm1, LPARAM parm2);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};








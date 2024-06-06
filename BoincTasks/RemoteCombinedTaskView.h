#pragma once

#include "ThreadRpc.h"
#include "ContextMenu.h"
#include "Selection.h"

#define FIRST_TIME	1
#define SECOND_TIME	2
#define THIRD_TIME	3

#define HOST_INFO_INTERVAL 60
#define ACCOUNTMANAGER_INFO_INTERVAL 360
#define SNOOZE_INFO_INTERVAL 8
#define SNOOZE_INFO_INTERVAL_NOT_VISIBLE 60
#define MESSAGE_UPDATE_INTERVAL	3600

// CRemoteCombinedTaskView view

class CThreadRpc;
class CRpcTaskInfo;
class CFilterData;
class CRemoteCombinedDoc;
class CDlgTimer;
class CRemoteCombinedTaskView : public CTemplateRemoteTaskView
{
	DECLARE_DYNCREATE(CRemoteCombinedTaskView)

public:
	CRemoteCombinedTaskView();           // protected constructor used by dynamic creation
	virtual ~CRemoteCombinedTaskView();
	virtual void OnInitialUpdate(); // called first time after construct

	CImageList m_StateImageList;
public:
	void	RpcReady(int iNrWu);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
//	void	GetSelectedComputers(std::deque<CString> *plSelectedComputers);

	void	SendWuRequest(int iNrRpc, bool bExcludeFromList = true);
	void	SendTThrottleRequest(int iNrTThrottleRpc);

	void	Sort(CListCtrl *plistCtrl);

	void	TaskOperation(int iOperation, bool bQuestion = true);

	void	PostProjectOperation(int iOperation);

	void	ToolbarUpdate(CCmdUI* pCmdUI);
	void	ToolbarUpdateProject(CCmdUI* pCmdUI);

	int		m_iForceClose;

	int		m_iThreadLocked;
	int		m_iThreadLockedStatusCount;
	bool	m_bThreadBusy;
//	bool	m_bThreadHistoryBusy;

	int		m_iStartupAliveTimer;
	int		m_iUpdateWuTimer;
	int		m_iTrickleMessageTimer;

	bool	m_bRpcRead;
	int		m_iNrWu;

	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;

	CDlgTimer *m_pDlgTimer;

	void	RpcRead(int iNrWu);
	void	DisplayRpc(CThreadRpc *pThreadRpc, int iRpcClientNr, int iNrWu, int *piRow, CFilterData *pFilterData, double dSetDays, char *m_pcComputerName, int iComputer);

	int		m_iStatusTextSet;

//	CContextMenuWww	m_contextMenu;
	CContextMenu m_contextMenu;
	
	bool	m_bDoOncePosition;

	bool	m_bDocumentIsClosing;
	int		m_iThreadsClosingCount;
	int		m_iNowClosingDelayCount;

	std::deque<CRpcGraphic*>  m_lRunningGraphic;


//Warning these items are members because they have to stay valid for some time
//	CString m_sSelectedTasksItemsComputer[MAX_SELECTED+1];
//	CString m_sSelectedTasksItemsName[MAX_SELECTED+1];
//	CString m_sSelectedTasksItemsPerc[MAX_SELECTED+1];
//	CString m_sSelectedTasksItemsState[MAX_SELECTED+1];
//	CString m_sSelectedProjectItemsComputer[MAX_SELECTED+1];
//	CString m_sSelectedProjectItemsName[MAX_SELECTED+1];
//	CString m_sSelectedProjectItemsPerc[MAX_SELECTED+1];
//	CString m_sSelectedProjectItemsState[MAX_SELECTED+1];
// ----------------------------------------------------------------------------

	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg LRESULT OnReadyRpc(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnReadyRpcClient(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnTimerDlg(WPARAM parm1, LPARAM parm2);
//	afx_msg LRESULT OnHistoryUnlock(WPARAM parm1, LPARAM parm2);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnSettingsTasks(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSwitch(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnPreferenceDialog(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProxyDialog(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnReadyProject(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnHostInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAccountManagerInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAccountManagerChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelTabView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnSettingsApplyChanges(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnDocumentIsClosing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadQuit(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnFloaterData(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnFloaterDataThreadReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadSnoozeCpuReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadSnoozeGpuReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadNetworkModeReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsTasksSortingStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnBoincTasksVersion(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBoincTasksRule(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchComputerReady(WPARAM wParam, LPARAM lParam);

	BOOL TasksProperties(int iBusy);
	afx_msg void OnTasksProperties();

	afx_msg void OnShowGraphic();
	afx_msg LRESULT OnShowGraphicReady(WPARAM wParam, LPARAM lParam);

	afx_msg void OnToolbarSuspend();
	afx_msg void OnUpdateToolbarSuspend(CCmdUI* pCmdUI); 
	afx_msg void OnToolbarResume();
	afx_msg void OnUpdateToolbarResume(CCmdUI* pCmdUI);
	afx_msg void OnToolbarAbort();
	afx_msg void OnUpdateToolbarAbort(CCmdUI* pCmdUI);

	afx_msg void OnToolbarProjectUpdate();
	afx_msg void OnUpdateToolbarProjectUpdate(CCmdUI* pCmdUI);
	afx_msg void OnToolbarProjectSuspend();
	afx_msg void OnUpdateToolbarProjectSuspend(CCmdUI* pCmdUI);
	afx_msg void OnToolbarProjectResume();
	afx_msg void OnUpdateToolbarProjectResume(CCmdUI* pCmdUI);
	afx_msg void OnToolbarProjectNoNew();
	afx_msg void OnUpdateToolbarProjectNoNew(CCmdUI* pCmdUI);
	afx_msg void OnToolbarProjectAllowNew();
	afx_msg void OnUpdateToolbarProjectAllowNew(CCmdUI* pCmdUI);

/*
	afx_msg void OnToolbarAllowNetworkPref();
	afx_msg void OnUpdateToolbarAllowNetworkPref(CCmdUI* pCmdUI);
	afx_msg void OnToolbarAllowNetworkNever();
	afx_msg void OnUpdateToolbarAllowNetworkNever(CCmdUI* pCmdUI);
			void ToolbarAllowNetwork(int iMode);
	afx_msg void OnToolbarAllowCpuPref();
	afx_msg void OnUpdateToolbarAllowCpuPref(CCmdUI* pCmdUI);
	afx_msg void OnToolbarAllowCpuSnooze();
	afx_msg void OnUpdateToolbarAllowCpuSnooze(CCmdUI* pCmdUI);
	afx_msg void OnToolbarAllowCpuNever();
	afx_msg void OnUpdateToolbarAllowCpuNever(CCmdUI* pCmdUI);
			void ToolbarAllowCpu(int iMode);
	afx_msg void OnToolbarAllowGpuPref();
	afx_msg void OnUpdateToolbarAllowGpuPref(CCmdUI* pCmdUI);
	afx_msg void OnToolbarAllowGpuSnooze();
	afx_msg void OnUpdateToolbarAllowGpuSnooze(CCmdUI* pCmdUI);
	afx_msg void OnToolbarAllowGpuNever();
	afx_msg void OnUpdateToolbarAllowGpuNever(CCmdUI* pCmdUI);
			void ToolbarAllowGpu(int iMode);
*/

	afx_msg LRESULT OnFontChangeAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontHasChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnNoticesNew(WPARAM wParam, LPARAM lParam);

	afx_msg void OnHeaderSetItems();

	afx_msg void OnCopyalltoclipboard();
	afx_msg void OnCopyselectedtoclipboard();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFilterReadytostart();
	afx_msg void OnUpdateFilterReadytostart(CCmdUI* pCmdUI);
	afx_msg void OnFilterReadytoreport();
	afx_msg void OnUpdateFilterReadytoreport(CCmdUI* pCmdUI);
	afx_msg void OnFilterWaitingtorun();
	afx_msg void OnUpdateFilterWaitingtorun(CCmdUI* pCmdUI);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTaskSuspend();
	afx_msg void OnTaskSuspendCheckpoint();
	afx_msg void OnTaskResume();
	afx_msg void OnTaskAbort();

	afx_msg void OnWww1();
	afx_msg void OnWww2();
	afx_msg void OnWww3();
	afx_msg void OnWww4();
	afx_msg void OnWww5();
	afx_msg void OnWww6();
	afx_msg void OnWww7();
	afx_msg void OnWww8();
	afx_msg void OnWww9();
	afx_msg void OnWww10();
	afx_msg void OnWww11();
	afx_msg void OnWww12();
	afx_msg void OnWww13();
	afx_msg void OnWww14();
	afx_msg void OnWww15();
	afx_msg void OnWww16();
	afx_msg void OnWww17();
	afx_msg void OnWww18();
	afx_msg void OnWww19();
	afx_msg void OnWww20();

	afx_msg void OnTaskAddRule();

	afx_msg void OnFilteronUpload();
	afx_msg void OnUpdateFilteronDownload(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilteronUpload(CCmdUI *pCmdUI);
	afx_msg void OnFilteronDownload();
	afx_msg void OnFilteronSuspended();
	afx_msg void OnUpdateFilteronSuspended(CCmdUI *pCmdUI);
	afx_msg void OnFilteronAborted();
	afx_msg void OnUpdateFilteronAborted(CCmdUI *pCmdUI);
	afx_msg void OnFilteronRunning();
	afx_msg void OnUpdateFilteronRunning(CCmdUI *pCmdUI);
	afx_msg void OnFilteronComputation();

	afx_msg void OnUpdateShowActiveTasks(CCmdUI *pCmdUI);
	afx_msg void OnShowActiveTasks();

	afx_msg void OnUpdateFilteronComputation(CCmdUI *pCmdUI);
	afx_msg void OnTaskProjectupdate();
	afx_msg void OnTaskProjectsuspend();
	afx_msg void OnTaskProjectresume();

	afx_msg LRESULT OnWebOperation(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebFilter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebSort(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebProperties(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWebItemSelected(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTaskShowcpu();
	afx_msg void OnTaskShowNonCpuIntensive();
	afx_msg void OnTaskShowgpu();
	afx_msg void OnUpdateTaskShowcpu(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTaskShowNonCpuIntensive(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTaskShowgpu(CCmdUI *pCmdUI);

	afx_msg void OnComputersDetect();
};



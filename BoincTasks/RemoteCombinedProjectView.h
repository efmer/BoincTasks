#pragma once

#include "ContextMenu.h"
#include "ThreadRpc.h"

// CRemoteCombinedProjectView view

class CThreadRpc;
class CRemoteCombinedDoc;
class CRemoteCombinedProjectView : public CTemplateRemoteProjectView
{
	DECLARE_DYNCREATE(CRemoteCombinedProjectView)

public:
	CRemoteCombinedProjectView();           // protected constructor used by dynamic creation
	virtual ~CRemoteCombinedProjectView();
	virtual void OnInitialUpdate();				// called first time after construct
	void	InitialUpdate(CRemoteCombinedDoc* pDoc);

	bool	m_bDocumentIsClosing;

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	time_t	m_LastSwitchedTime;

	void	GetSelectedComputers(std::deque<CString> *plSelectedComputers);
	bool	DisplayRpc(CThreadRpc *pThreadRpc, int iNrProjects, int *piRow, char *pcComputerId, int iComputerId);

	void	Sort(CListCtrl *plistCtrl);
	void	AddCPID(char *pCPID);

	void	ToolbarUpdateProject(CCmdUI* pCmdUI);


	std::deque<std::string>	m_lCPID;
	bool	m_bCpidChanged;

//	CThreadRpc			*m_pThreadRpc;


//	CString				m_sId;

	CMenu			m_menu;
	void			PostProjectOperation(int iOperation, CString sWarning = "");

	CString			m_sSelectedProjectItemsComputer[MAX_PROJECTS+1];
	CString			m_sSelectedProjectItemsName[MAX_PROJECTS+1];
	CString			m_sAddProjectSelectedComputer;
	int				m_iSelectedProjectItems;

	int				m_iNrColomns;

	HWND			m_hWndProxy;

	CContextMenu	m_contextMenu;

	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg	LRESULT OnCpidChanged(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnStartStats(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnReadyRpc(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnGraphicsTasksReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnTimerProject(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSettingsTasks(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSwitch(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColumnChanged(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAddNewProject(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAddNewAccountManager(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAccountManagerSynchronizeReady(WPARAM parm1, LPARAM parm2);

	afx_msg	LRESULT OnAccountManagerPollReady(WPARAM wParam, LPARAM lParam);

	afx_msg void OnProjectsSetDebt();
	afx_msg	void OnUpdateSetDebt(CCmdUI* pCmdUI);

	afx_msg LRESULT OnSettingsApplyChangesTasks(WPARAM parm1, LPARAM parm2);

//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
 	afx_msg LRESULT OnReadyProject(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnPreferenceDialog(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProxyDialog(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelTabView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnFloaterData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnBoincTasksVersion(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBoincTasksRule(WPARAM wParam, LPARAM lParam);	

	afx_msg LRESULT OnReadyStats(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchComputerReady(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnFontChangeAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontHasChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHeaderSetItems();

	afx_msg void OnCopyalltoclipboard();
	afx_msg void OnCopyselectedtoclipboard();

	afx_msg LRESULT OnWebOperation(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebItemSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebSort(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebProperties(WPARAM wParam, LPARAM lParam);
	BOOL ProjectProperties(int iBusy);

	afx_msg void OnComputersDetect();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnProjectsResume();
	afx_msg void OnProjectsSuspend();
	afx_msg void OnProjectsUpdate();
	afx_msg void OnProjectsAllowWork();
	afx_msg void OnProjectsNomorework();
	afx_msg void OnProjectsProperties();

	afx_msg void OnProjectsAddNewProject();
	afx_msg void OnAddNewAccountManager();

	afx_msg void OnProjectsReset();
	afx_msg void OnProjectsDetach();

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
};



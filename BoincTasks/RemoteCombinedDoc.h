#pragma once

#include "ThreadRpc.h"

#define REFRESH_VIEW_NOW 1

class CHtmlExchange
{
public:
	int		m_iRow;
	int		m_iOption;
	char	*m_pcHtml;
	time_t	tTime;
	bool	m_bSendData;
};

class CDlgGraphic;
class CDlgGraphicTransfer;
class CDlgGraphicTasks;
class CDlgGraphicDeadline;
class CDlgGraphicTemperature;
class CDlgTThrottle;
class CDlgThreadGraph;
class CDlgEditConfig;
class CDlgEditAppInfoConfig;
class CDlgHistory;
class CDlgSelectComputer;
class CThreadRpcClient;
class CThreadRpc;

class CRpcConnectionItem
{
public:
	char				*m_pcGroup;
	CThreadRpc			*m_pThreadRpc;
	int					m_iThreadRpcId;
	char				*m_pcThreadRpcId;
	CThreadRpcClient	*m_pThreadTThrottleRpc;
	int					m_iThreadTThrottleRpcId;
	char				*m_pcThreadTThrottleRpcId;
//	int					m_iNetworkMode;
	int					m_iConnectionStatus;
	HOST_INFO			m_hostInfo;
	double				m_dBenchmarktime;
	ACCT_MGR_INFO		m_accountManagerInfo;
	bool				m_bActive;

//	HMENU				m_hMenuNetwork;		// 0 = all
//	HMENU				m_hMenuAllow;		// 0 = all
//	HMENU				m_hMenuAllowGpu;	// 0 = all

	CC_STATUS			m_CcStatus;
};


// CRemoteCombinedDoc document

class CDlgPrefMain;
class CDlgPrefDisk;
class CDlgPrefNetwork;
class CDlgPrefProcessor;
//class CDgPrefExclusive;

class CDlgNotices;
class CRemoteDocs;
class CRemoteCombinedTaskView;
class CRemoteCombinedMessageView;
class CRemoteCombinedProjectView;
class CRemoteCombinedTransferView;
class CRemoteCombinedHistoryView;
class CRemoteCombinedComputerView;
class CRemoteCombinedNoticesView;
class CThreadRpc;
class CComputer;
class CComputerDoc;
class CRemoteCombinedDoc : public CDocument
{
	DECLARE_DYNCREATE(CRemoteCombinedDoc)

public:
	CRemoteCombinedDoc();
	virtual ~CRemoteCombinedDoc();
	virtual void OnCloseDocument();
			void OnCloseDocumentMine();
			void NowCloseDocument();
	void		Begin();

//	void		SetStatus(CThreadRpc *pThread, int iConnectionstatus, int iNetworkMode);

	void		DisplayStatus(int iStatusCount, int *piStatus,  bool bRefresh=false);

	bool		GetRpcThreadConnected(CString *pComputerId);
	HOST_INFO	*GetRpcThreadHostInfo(CString *pComputerId);
	int			GetRpcSelectedComputers(void);
	void		GetRpcThreadId(int *pThreadId, int iComputer);
	void		GetTThrottleRpcThreadId(int *pThreadId, int iComputer);
	bool		GetRpcThreadId(CString *pComputerId, int *piThreadId);
	bool		GetRpcThreadId(char *pcComputerId, int *piThreadId);
	bool		GetRpcThreadPointer(CString *pComputerId, CThreadRpc **piThreadId);
	int			GetRpcArrayPos(CString *pComputerId);
	void		GetComputerIds(char **pComputerId, int iComputer);
	bool		GetComputerIdStringWithIdNumber( int iThreadId, char **pcComputerId);
	int			GetComputerNumber(CString *pComputerId);
	bool		GetTThrottleComputerIdStringWithIdNumber(int iThreadId, char **pComputerId);
//	void		SetHostInfoBusy(int iNr);
	CC_STATUS	*GetHostStatus(int iNr);
	HOST_INFO	*GetHostInfo(int iNr);
	ACCT_MGR_INFO *GetAccountManagerInfo(int iNr);
	void		SettingsTasks(bool bUserFriendly);
	bool		SetSnooze(CString *psComputerId, int iTime);

	void		CleanupThreads();
	void		RemoveThreadFromList(int iThreadId);
	void		DeleteThreadFromList();
//	void		UpdateAllowToRunCpu(char *psHostName,  CC_STATUS *pCcStatus);
//	void		UpdateAllowToRunGpu(char *psHostName,  CC_STATUS *pCcStatus);

	void		NoticesNew(CString *psHtml);

	void		LogActiveComputers();

	int			StringCopyNew(char **pcDest, char *pcSource, bool bDelete = true);

	bool		m_bDocumentIsClosing;

	CDlgNotices			*m_pDlgNotices;

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	int			m_iViewActive;
//	CView		*m_pView;
//	int			m_iTypeDoc;
	
	CRemoteCombinedTaskView		*m_pTaskView;
	CRemoteCombinedMessageView	*m_pMessageView;
	CRemoteCombinedProjectView	*m_pProjectView;
	CRemoteCombinedTransferView *m_pTransferView;
	CRemoteCombinedHistoryView	*m_pHistoryView;
	CRemoteCombinedComputerView	*m_pComputerView;
	CRemoteCombinedNoticesView	*m_pNoticesView;

	CDlgPrefMain				*m_pDlgPrefMain;
	CDlgPrefDisk				*m_pDlgPrefDisk;
	CDlgPrefNetwork				*m_pDlgPrefNetwork;
	CDlgPrefProcessor			*m_pDlgPrefProcessor;

	CDlgHistory					*m_pDlgHistory;

	CDlgGraphic					*m_pDlgGraphic;

	CDlgSelectComputer			*m_pDlgSelectComputer;
protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument( LPCTSTR lpszPathName);

	void ClearMenu(CMenu *pMenu);
	void CopyComputerList();
	bool GetSelectedComputers(std::deque<CString> *plSelectedComputers);

// computer
	int								m_iSelectedComputers;
	std::deque<CRpcConnectionItem*>	m_lListConnectionsItems;
// computer


	int					m_iStatusTextSet;
	int					m_iRefresh;

//	CList		<CComputer*,CComputer*> m_lComputers;




	CDlgGraphicTransfer	*m_pDlgGraphicTransfer;
	CDlgGraphicTasks	*m_pDlgGraphicTasks;
	CDlgGraphicDeadline *m_pDlgGraphicDeadline;
	CDlgGraphicTemperature *m_pDlgTemperatureGraphic;
	CDlgTThrottle		*m_pDlgTThrottle;
	CDlgEditConfig		*m_pDlgEditConfig;
	CDlgEditAppInfoConfig	*m_pDlgEditAppInfo;
	CDlgThreadGraph		*m_pDlgThreadGraph;

	CString				m_sLastStatusTxt;

	CView*				SwitchToView ( CView* pNewView );



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDocumentIsClosing();
			 void OnHostInfo(HOST_INFO *pHi);
			 void OnAccountManagerInfo(ACCT_MGR_INFO *pAccountManagerInfo);

	afx_msg void OnHelpShowlog();
	afx_msg void OnHelpShowlogRules();
			void OnShowView(int iViewNr, int iRefresh);
	afx_msg void OnShowProjects();
	afx_msg void OnShowTasks();
	afx_msg void OnShowTransfers();
	afx_msg void OnShowMessages();
	afx_msg void OnShowHistory();
	afx_msg void OnShowHistoryLong();
	afx_msg void OnUpdateShowHistoryLong(CCmdUI *pCmdUI);

	afx_msg void OnShowNotices();
	afx_msg void OnShowComputers();
	afx_msg void OnShowStatistics();
	afx_msg void OnShowStatisticsTasks();
	afx_msg void OnShowStatisticsDeadline();
	afx_msg void OnShowTemperature();
	afx_msg void OnShowGraphicTransfer();

	afx_msg void OnShowTThrottle();
	afx_msg void OnUpdateTThrottle(CCmdUI *pCmdUI);

	afx_msg void OnShowThreadGraph();

	afx_msg void OnExtraPreferences();
	afx_msg void OnUpdateExtraPreferences(CCmdUI *pCmdUI);
	afx_msg void OnExtraProxy();
	afx_msg void OnUpdateExtraProxy(CCmdUI *pCmdUI);

	afx_msg void OnExtraDonetworkcommunication();
	afx_msg void OnUpdateExtraDonetworkcommunication(CCmdUI *pCmdUI);

	afx_msg void OnExtraSettings();
	afx_msg void OnViewComputerBar();
	afx_msg void OnViewProjectBar();
	afx_msg void OnColorsDialog();
	afx_msg void OnUpdateViewComputerBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewProjectBar(CCmdUI *pCmdUI);
	afx_msg void OnSettinsApplyChangesTasks();
	afx_msg void OnSettinsApplyChangesProjects();
	afx_msg void OnSettinsApplyChangesHistory();

	afx_msg void OnComputerEditConfig();
	afx_msg void OnUpdateComputerEditConfig(CCmdUI *pCmdUI);

	afx_msg void OnComputerEditAppInfo();
	afx_msg void OnUpdateComputerEditAppInfo(CCmdUI *pCmdUI);

	

	afx_msg void OnComputerNetDialog();
	afx_msg void OnUpdateComputerNetDialog(CCmdUI *pCmdUI);

	afx_msg void OnComputerAllowDialog();
	afx_msg void OnUpdateComputerAllowDialog(CCmdUI *pCmdUI);

	afx_msg void OnComputerAllowGpuDialog();
	afx_msg void OnUpdateComputerAllowGpuDialog(CCmdUI *pCmdUI);

	afx_msg void CRemoteCombinedDoc::OnComputerSynchronizeDialog();
	
	afx_msg void OnUpdateOnComputerSynchronizeDialog(CCmdUI *pCmdUI);
	
	//void	OnComputerSynchronize(int iNr);
	//void	OnUpdateOnComputerSynchronize(int iNr, CCmdUI *pCmdUI);


	afx_msg void OnComputerBenchmarkDialog();
	afx_msg void OnUpdateOnComputerBenchmarkDialog(CCmdUI *pCmdUI);

	afx_msg void OnComputerReadConfigDialog();
	afx_msg void OnUpdateOnComputerReadConfigDialog(CCmdUI *pCmdUI);
	
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
			void ToolbarEnable(CCmdUI* pCmdUI);

	void	OnComputerReadConfig(int iNr);
	void	OnUpdateOnComputerReadConfig(int iNr, CCmdUI *pCmdUI);

	afx_msg void OnHelpUpdate();
	afx_msg void OnAddNewProject();
	afx_msg void OnAddNewProjectManager();
	afx_msg void OnUpdateAddNewProjectManager(CCmdUI *pCmdUI);
	afx_msg void OnComputersLocation();
	afx_msg void OnRestartGraphics();
	afx_msg void OnRestartGraphicsTasks();
	afx_msg void OnRestartGraphicsTasksReady();
	afx_msg void OnRestartGraphicsDeadline();
	afx_msg void OnRestartGraphicsDeadlineReady();
};

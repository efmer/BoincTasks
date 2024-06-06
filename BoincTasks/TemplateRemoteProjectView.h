#pragma once
#include "ThreadRpc.h"
#include "selection.h"

#define NUM_REMOTE_PROJECT_COLUMNS 17

//#define MAX_PROJECT_DATA				(MAX_COMPUTERS_REMOTE*(MAX_PROJECTS+10))

class CListStatsItem
{
public:
	CString	m_sProjectName;
	CString m_sUrl;
//	CString m_sUrlAlias;
	UINT	m_iProjectRankTotalCredit;
	UINT	m_iProjectRankExpavgCredit;
	UINT	m_iTeamRankTotalCredit;
	UINT	m_iTeamRankExpavgCredit;
};

class CListAliasItem
{
public:
	CString m_sUrl;
	CString m_sUrlAlias;
};

class CProjectPropertiesItem
{
public:
	CString				m_sComputerId;
	CString				m_sProject;
	int					m_iThreadId;
	int					m_iFoundCount;
};


class CRpcInfoProjectItem
{
public:
	int			m_iNrProjects;
	int			m_iThreadId;
	char		*m_pcComputerId;
	CThreadRpc	*m_pThread;
	int			m_iStatusRpc;
	int			m_iSortIndex;
};

// for every computer
class CComputerProjectListItem
{
public:
	PROJECTS						m_projects;
	DISK_USAGE						m_diskUsage;
	CRpcProjectsDiskUsage			m_projectsDiskUsage;
	int								m_iProjectPropertiesUniqueThreadId;
	ACCT_MGR_RPC_REPLY				m_AccountManagerRpcReply;
	char							*m_pcAccountManagerRpcReplyComputer;
};

class CComputerGraphicTasksListItem;
class CHistoryCountItem;
class PROJECT;
class CThreadRpc;
class CComputerRemoteDoc;
class CSortColumn;
class CSelection;
class CThreadStats;
class CCreateHtmlWebServerProperties;
class CTemplateRemoteProjectView : public CListViewEx 
{
	friend class CRemoteCombinedDoc;
	friend class CRemoteCombinedTaskView;

	DECLARE_DYNCREATE(CTemplateRemoteProjectView)

public:
	CTemplateRemoteProjectView();           // protected constructor used by dynamic creation
	virtual ~CTemplateRemoteProjectView();
			void DeleteProjectProperties();
			void DeleteProjectTasks();
			void DeleteStatsList();
	virtual void OnInitialUpdate();				// called first time after construct
			void InitialUpdate(CComputerRemoteDoc* pDoc);

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	void	AddAllColumns(bool bAdd = true);
	void	ReadAllColumnsWidth();
	int		ReadColumnWidth(int iColumn, int iDefault);
	void	CheckAndSaveColumnsWidth();
	void	GetTasksDayWeek(char *pcComputerName, char *pcProject, int *piDayCpu, double *pdWeekCpu, int *piDayGpu, double *pdWeekGpu);
	void	ProcessStatsXml(CString *psStat);
	CString Alias(CString *psUrl);
	int		GetRank(CString *psProject, CString *psUrl, CString *psRank);
	int		GetTeamRank(CString *psProject, CString *psUrl, CString *psRank);

	void	ProjectProperties1(CComputerProjectListItem *pComputerProjectListItem, int iProjects, CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties);
	void	ProjectProperties2(DISK_USAGE *pInfo, int iPos, CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties);
//	void	TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore, char *pAfter);
	CString FormatNumber(double dNr, int iNrComma=1);
	bool	GetTaskCount(char *pcComputer, char  *pszProject, int *piTasksCpu, int *piTasksGpu, __int64 *piTimeLeftCpu, __int64 *piTimeLeftGpu, int *piCoresCpu,  int *piCoresGpu, int *piCoresLow, bool *pbWarning );
	void	GetTaskCount(char *pcComputer, int *piTasksCpu, int *piTasksGpu, __int64 *piTimeLeftCpu, __int64 *piTimeLeftGpu);
	void	CheckWarning(CHistoryCountItem *pHistoryCountItem, CString *psComputerWarning, CString *psProjectWarning, int iWarningCpuTasks, int iWarningGpuTasks, bool *pbWarning);
	void	DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin);
	CString YesNo(bool bYesNo);
	CString Time(double dTime);
	CString DiskSpace(double dSpace);


	std::deque<CListData*> m_lListData;
	CListData m_dataDummy;

	CRemoteCombinedDoc	*m_pDoc;

// computer
	int		m_iThreadLocked;
	bool	m_bThreadBusy;
	bool	m_bThreadTasksBusy;

	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;
	int		m_iProjectTotal;

	time_t	m_tIntervalTasksCount;
	time_t	m_tLastTasksCount;
	int		m_iSendTasksCount;
	int		m_iReadyTasksCount;

	std::deque<CRpcInfoProjectItem*>		m_lRpcInfo;
	std::deque<CComputerProjectListItem*>	m_lProjectItem;
	std::deque<CHistoryCountItem*>			m_lHistoryCount;

	std::deque<CComputerGraphicTasksListItem*> m_lComputer;

// computer

	void	DisplaySingleItem(CListCtrl *pListCtrl, int iRow, PROJECT *pInfo, char *pcComputerName, int iComputerId);

	void				ClipBoard(bool bAll);

	int					m_iCountReadShareTotal;
	double				m_dShareTotal;

	CSortColumn			*m_pSortColumn;

	CSelection			*m_pSelection;
	int					m_iProjectPropertiesBusy;

	bool				m_bFirstTime;

	int					m_iColumnOrder[NUM_REMOTE_PROJECT_COLUMNS+1]; 
	int					m_iColumnWidth[NUM_REMOTE_PROJECT_COLUMNS+1];		// for xchange only
	bool				m_bApplyChangesOnColumn;
	int					m_iNrColomns;

	void				ProjectPropertiesGet(int iBusy);
	afx_msg LRESULT		OnProjectPropertiesReady(WPARAM parm1, LPARAM parm2);
	void				PropertiesAddRow(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties, CString *psColumn0, CString *psColumn1);
	void				ProjectPropertiesReady(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties);

	std::deque<CProjectPropertiesItem*>		m_lProjectProperties;

	int					m_iProjectPropertiesUniqueThreadsFoundCount;

	int					m_iProjectPropertiesThreadsPostedCount;
	int					m_iProjectPropertiesThreadsReturnCount;

	bool				m_bDraged;
			
	int					m_iTimerWu;

	time_t				m_tNextStatsReadWeb;
	int					m_iNextStatsReadInterval;
	CThreadStats		*m_pThreadStats;
	int					m_iThreadStatRequest;
	bool				m_bThreadStatRequestError;
	bool				m_bUseStats;
	std::deque<CListStatsItem*> m_lListStats;
	std::deque<CListAliasItem*> m_lListUrlAlias;


	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHeaderChanged(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSettingsApplyChangesProjects(WPARAM parm1, LPARAM parm2);

	afx_msg void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg	LRESULT OnProjectHistoryCount(WPARAM parm1, LPARAM parm2);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};



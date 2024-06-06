#pragma once
#include <deque>

//#define DEBUG_TIMING					1

#define LOGGING_TEXT_CONNECTION			"Connect"
#define TEXT_STATE_UPDATING				"Updating..."

#define REFRESH_PROJECS_REGULATOR		0
#define REFRESH_PROJECTS				1

#define THREAD_TIME_RATE	10

#define RPC_FIRST_MESSAGE				0

#define RPC_STATUS_NONE					0
#define RPC_STATUS_ERROR_CANNOT_CONNECT	1
#define RPC_STATUS_ERROR_READ			2
#define RPC_STATUS_ERROR_AUTHORIZATION	3
#define RPC_STATUS_ERROR_READING_STATE	4
#define RPC_STATUS_ERROR_READING_VERSION 5
#define RPC_STATUS_ERROR_LOST_CONNECTION 6
#define RPC_STATUS_CONNECTED			10

#define SORTING_NONE	0
#define SORTING_AZ		1
#define SORTING_ZA		2

#define SORTING_COLUMN_END		0xff

#define STATE_NEW				0x00000
#define STATE_NEW_HP			0x00001
#define STATE_DOWNLOADING		0x10000
#define STATE_DOWNLOADING_HP	0x10001
#define STATE_READY_TO_START	0x20000
#define STATE_READY_TO_START_HP	0x20001
#define STATE_WAITING_TO_RUN	0x20100
#define STATE_WAITING_TO_RUN_HP	0x20101
#define STATE_RUNNING			0x20200
#define STATE_RUNNING_HP		0x20201
#define STATE_COMPUTATON_ERROR	0x30000
#define STATE_COMPUTATON_ERROR_HP	0x30001
#define STATE_COMPUTATON_ERROR_WR	0x30100
#define STATE_UPLOADING			0x40000
#define STATE_READY_TO_REPORT	0x50000
#define STATE_ABORTED			0x60000
#define STATE_ABORTED_HP		0x60001
#define STATE_ABORTED1			0x60100
#define STATE_ABORTED2			0x60200
#define STATE_ABORTED2_HP		0x60201

//#define STATE_POSTPHONED		0x20102		//= postphoned

// 2 missed status missed 2 is what missed ends up...... as the last 4 are zero after reading back from the file
#define STATE_MISSED2			0xf0000		
#define STATE_MISSED			0xfff00
// Warning the history only does >> 16 before writing the state. Last 4 digits are 0 
// Recovered from oldResults
#define STATE_MISSED_RECOVERED	0x1f0000	
#define STATE_MISSED_OLD		0x2f0000

#define STATE_DUMMY_SUSPENDED	0xfffff

#define STATUS_TRANSFER_DOWNLOAD		0
#define STATUS_TRANSFER_UPLOAD			1

#define COLUMN_COMPUTER_GROUP			0
#define COLUMN_COMPUTER_ID				1
#define COLUMN_COMPUTER_CON_ENCRYPTION	2
#define COLUMN_COMPUTER_IP				3
#define COLUMN_COMPUTER_MAC				4
#define COLUMN_COMPUTER_PORT			5
#define COLUMN_COMPUTER_PASS			6
#define COLUMN_COMPUTER_BOINC			7
#define COLUMN_COMPUTER_BOINC_PLATFORM	8
#define COLUMN_COMPUTER_TTHROTTLE		9
#define COLUMN_COMPUTER_STATUS			10

#define COLUMN_PROJECT					0
#define COLUMN_APPLICATION				1
#define COLUMN_NAME						2
#define COLUMN_ELAPSED					3
#define COLUMN_CPU_PERCENTAGE			4
#define COLUMN_PERCENTAGE				5
#define COLUMN_TIME_LEFT				6
#define COLUMN_DEADLINE					7
#define COLUMN_USE						8
#define COLUMN_STATE					9
#define COLUMN_TTHROTTLE				10
#define COLUMN_TEMPERATURE				11
#define COLUMN_COMPUTER					12
#define COLUMN_CHECKPOINT				13
#define COLUMN_DEBT						14
#define COLUMN_RECEIVED					15
#define COLUMN_ACCOUNT					16
#define COLUMN_VIRTUAL_MEMORY			17
#define COLUMN_MEMORY					18
#define COLUMN_SHARE					19

#define COLUMN_MESSAGE_NR				0
#define COLUMN_MESSAGE_PROJECT			1
#define COLUMN_MESSAGE_TIME				2
#define COLUMN_MESSAGE_MESSAGE			3
#define COLUMN_MESSAGE_COMPUTER			4

#define COLUMN_PROJECT_PROJECT			0
#define COLUMN_PROJECT_ACCOUNT			1
#define COLUMN_PROJECT_TEAM				2
#define COLUMN_PROJECT_USER_CREDITS		3
#define COLUMN_PROJECT_USER_A_CREDITS	4
#define COLUMN_PROJECT_HOST_CREDITS		5
#define COLUMN_PROJECT_HOST_A_CREDITS	6
#define COLUMN_PROJECT_SHARE			7
#define COLUMN_PROJECT_TASKS			8
#define COLUMN_PROJECT_TIME_LEFT		9
#define COLUMN_PROJECT_STATUS			10
#define COLUMN_PROJECT_COMPUTER			11
#define COLUMN_PROJECT_VENUE			12
#define COLUMN_PROJECT_TASKS_DAY		13
#define COLUMN_PROJECT_TASKS_WEEK		14
#define COLUMN_PROJECT_RANK				15
#define COLUMN_PROJECT_TEAM_RANK		16

#define COLUMN_TRANSFER_PROJECT		0
#define COLUMN_TRANSFER_FILE		1
#define COLUMN_TRANSFER_PROGRESS	2
#define COLUMN_TRANSFER_SIZE		3
#define COLUMN_TRANSFER_ELAPSED		4
#define COLUMN_TRANSFER_SPEED		5
#define COLUMN_TRANSFER_STATUS		6
#define COLUMN_TRANSFER_COMPUTER	7

#define COLUMN_HISTORY_PROJECT		0
#define COLUMN_HISTORY_APPLICATION	1
#define COLUMN_HISTORY_NAME			2
#define COLUMN_HISTORY_ELAPSED		3
#define COLUMN_HISTORY_COMPLETED	4
#define COLUMN_HISTORY_REPORTED		5
#define COLUMN_HISTORY_USE			6
#define COLUMN_HISTORY_RATIO		7
#define COLUMN_HISTORY_STATUS		8
#define COLUMN_HISTORY_COMPUTER		9
#define COLUMN_HISTORY_VIRTUAL		10
#define COLUMN_HISTORY_MEMORY		11

#define	PROJECT_OP_SUSPEND			1
#define PROJECT_OP_RESUME			2
#define PROJECT_OP_UPDATE			3
#define PROJECT_OP_NOMOREWORK		4
#define PROJECT_OP_ALLOWMOREWORK	5
#define PROJECT_OP_RESET			6
#define PROJECT_OP_DETACH			7

#define TRANSFER_OP_RETRY			1
#define TRANSFER_OP_RETRY_ALL		2
#define TRANSFER_OP_ABORT			3

#define TASKS_OP_SUSPEND			1
#define TASKS_OP_RESUME				2
#define TASKS_OP_ABORT				3

#define OPERATION_TASK				0
#define OPERATION_PROJECT			1
#define OPERATION_COMPUTER			2

#define COMPUTER_SNOOZE				0
#define COMPUTER_SNOOZE_GPU			1
#define COMPUTER_SNOOZE_CANCEL		2
#define COMPUTER_SNOOZE_CANCEL_GPU	3
#define COMPUTER_SUSPEND_NETWORK	4
#define COMPUTER_RESUME_NETWORK		5


#define SUSPEND_STATUS_WAITING_FOR_MEMORY	1

#include <vector>
#include <string>
#include "gui_rpc_client.h"
#include "error_numbers.h"
#include "util.h"
#include "str_util.h"
#include "version.h"
#include "common_defs.h"
#include "string.h"
#include "proxy_info.h"
#include "notice.h"

#define MAX_PROJECTS	1000	
#define MAX_TRANSFER	10000
#define MAX_WU			60000
#define MAX_VERSION_NR	200


class CThreadRpc;

class CRpcConfig
{
public:
	CC_CONFIG*	m_pConfig;
	LOG_FLAGS*	m_pLogFlags;
	std::string	m_sRawData;
	bool		m_bValid;
};

class CRpcAppInfo
{
public:
	CString			m_sProject;
	CString			m_sProjectUrl;
	std::string		m_sRawData;
	APP_CONFIGS*	m_pAppInfo;
	LOG_FLAGS*		m_pLogFlags;
	bool			m_bValid;
};

class CRpcFilterInfo
{
public:
	int		m_iFilter;
	int		m_iFilterApplicationAndVersion;
	CString	m_sApplication;
};

class CRpcGraphic
{
public:
	CString		m_sProject;
	CString		m_sName;
	CString		m_sGraphicsExecPath;
	CString		m_sSlotPath;
	int			m_iSlot;
	DWORD		m_dwPID;
};

class CRpcProjectsDiskUsage
{
public:
	PROJECTS* pProjects;
	DISK_USAGE* pDiskUsage;
	bool		m_bBoincClientV6;
	bool		bHave_cuda;
	bool		bHave_ati;
};

class CRpcReturnStringWithNr
{
public:
	char	m_sStr[255];
	int		m_iNr;
	int		m_iValid;
};

class CRpcProjectAttach
{
public:
	char* m_sUrl;
	char* m_sAuth;
	//	char* m_sName;
};

class CRpcManagerAttach
{
public:
	char* m_sUrl;
	char* m_sAuth;
	char* m_sName;
};

class CRpcThreadReturn
{
public:
	int m_iNumber;
	int m_iThreadId;
	int m_iNrLastRead;
	int	m_iInit;
	int m_iStatus;
	//	int m_iNetworkMode;
	int	m_iTimeLeftLowest;
	char* m_pcComputerId;
	char* m_pcHostName;
	VERSION_INFO* m_pBoincVersion;
	int	m_iBoincVersion;
	CThreadRpc* m_pThread;

	double	m_dDiskInterval;

	bool	m_bBoincClientV6;
};

class CRpcThreadInfoMessage
{
public:
	int		m_iNrRead;
	int		m_iNrLastRead;
};

class CRpcThreadSendMessage
{
public:
	int		m_iInit;
	int		m_iKeepMessages;
};

class CRpcThreadInfoNotices
{
public:
	int		m_iNrRead;
	NOTICES* m_pNotices;
	int		m_iRpcPosition;
};


class CRpcTaskInfo
{
public:
	RESULT* m_pResult;

	char* m_pszProject;
	char* m_pszAccount;
	char* m_pszPlanClass;
	char* m_pszResources;
	char* m_pszName;
	char* m_pszApp;
	int		m_iVersionNumber;

	int		m_iFinalElapsedTime;
	int		m_iRemainingTime;
	int		m_iDeadlineTime;
	int		m_iState;
	bool	m_bSuspendedByGui;
	bool	m_bSuppendedProjectByGui;
	bool	m_bNonCpuIntensive;
	int		m_iFiltered;

	double	m_dCurrentCpuTime;
	double	m_dFinalCpuTime;
	double	m_dElapsedTime;
	double	m_dFraction_done;
	CC_STATUS* m_pCc;
	int		m_iSuspendStatus;
	bool	m_bDeadline;

	double	m_dCheckpointCpuTime;

	double	m_dCpuShortTermDebt;
	double	m_dCudaShortTermDebt;
	double	m_dAtiShortTermDebt;

	double	m_dReceivedTime;

	double	m_dShare;

	int		m_iExitStatus;
};

class CRpcMessageInfo
{
public:
	int		m_iSegNo;
	int		m_iPriority;

	char* m_pszProject;
	char* m_pszBody;
	int		m_iTimeStamp;
};

class CRpcTransferInfo
{
public:
	FILE_TRANSFER* m_pFileTransfer;
	bool	m_bBoincClientV6;
	//	char	*m_pszName;
	//	char	*m_pszProjectUrl;
	//	char	*m_pszProjectName;
	//	int		m_iBytes;
	//	int		m_iTimeSoFar;
	//	int		m_iBytesXferred;
	//	int		m_iRetries;
	//	double	m_dXferSpeed;
	//	bool	m_bXferActive;
	//	bool	m_bPersXferActive;
	//	bool	m_bUploaded;
	//	bool	m_bIsUpload;
	//	int		m_iStatus;
	//	int		m_iNextRequestTime;
	//	int		m_iFirstRequestTime;
	//	int		m_iProjectBackoff;
	//	bool	m_bBoincClientV6;
};

class CRpcPreferenceInfo
{
public:
	GLOBAL_PREFS		prefs;
	GLOBAL_PREFS_MASK	prefsMask;
	int					m_iWantWorkUnits;
	int					iReturn;

	char* m_pcHostName;
	char* m_pcComputerId;

	bool				m_bBoincClientV6;

};

class CRpcProxyInfo
{
public:

	GR_PROXY_INFO		m_grProxyInfo;
	int					iReturn;

	char* m_pcHostName;
	char* m_pcComputerId;
};

class CRpcThreadConnectData
{
public:
	int					m_iThreadId;
	int					m_iPort;
	int					m_retval;
	int					m_iRpcStatus;
	int					m_iEncryptedConnection;
	char* m_pcHostName;
	char* m_pcMAC;
	char* m_pcPassword;
	char* m_pcErrorMessage;
	int					m_iValidMac;
	std::string			m_sFoundIpWithMac;
	RPC_CLIENT* m_pRpc;
};


#define NOT_FILTERED				0
#define FILTERED					1
//#define FILTERED_CUDA				2
// shift the version to make room for the filter. (only the filter states 2 bit NOT_FILTERED - FILTERED_CUDA)
#define FILTER_SHIFT_VERSION	1

#define MAX_FILTER_TYPE			10
// filter nr is also array pointer so it has to start with zero and must be sequencial

#define FILTER_NONE				-1
#define FILTER_READY_TO_REPORT	0
#define FILTER_WAITING_TO_RUN	1
#define FILTER_READY_TO_START	2
#define FILTER_DOWNLOAD			3
#define FILTER_UPLOAD			4
#define FILTER_SUSPENDED		5
#define FILTER_COMPUTATION		6
#define FILTER_ABORTED			7
#define FILTER_RUNNING			8
#define FILTER_RUNNING_HP		9


#define FILTER_BIT_POS_READY_TO_REPORT	0
#define FILTER_BIT_POS_WAITING_TO_RUN	1
#define FILTER_BIT_POS_READY_TO_START	2
#define FILTER_BIT_POS_DOWNLOAD			3
#define FILTER_BIT_POS_UPLOAD			4
#define FILTER_BIT_POS_SUSPEND			5
#define FILTER_BIT_POS_COMPUTATION		6
#define FILTER_BIT_POS_ABORTED			7
#define FILTER_BIT_POS_RUNNING			8
#define FILTER_BIT_POS_SHOW_RUNNING_TASKS 9
#define FILTER_BIT_POS_SHOW_NON_CPU_INTENSIVE 10
#define FILTER_BIT_POS_SHOW_CPU			11
#define FILTER_BIT_POS_SHOW_GPU			12
#define FILTER_POS_FULL_READ_INTERVAL	13

class CFilterData
{
public:
	bool	m_bValid;

	int		m_iCount;
	int		m_iTimeRemaining[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	int		m_iFinalElapsedTime[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dElapsedTime[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dTimeDeadline[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dTimeRemainingShortestDeadline[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dFraction_done[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	int		m_iVersion[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	int		m_iApplicationAndVersion[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	int		m_iVersionNrCount[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	char* m_pszAccountName[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	char* m_pszProjectName[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	char* m_pszUserFriendly[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	char	m_szDescription[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1][255];
	char* m_pszFilteredPlanClass[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	char* m_pszFilteredResources[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	bool	m_bProjectSuspendedViaGui[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	bool	m_bNonCpuIntensive[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];

	double	m_dCpuDebt[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dCudaDebt[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dAtiDebt[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];

	double	m_dCurrentCpuTime[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dFinalCpuTime[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];

	double	m_dCheckpointCpuTime[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	double	m_dProjectShare[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];

	double	m_dReceivedTime[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];
	int		m_iExitStatus[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];

	char* m_pszFilteredLastWU[MAX_FILTER_TYPE + 1][MAX_VERSION_NR + 1];

	int		m_iFilteredWu[MAX_WU + 1];
	int		m_iTotalVersion[MAX_FILTER_TYPE + 1];
	int		m_iTotalWu[MAX_FILTER_TYPE + 1];

};

class CVersionApplicationInfo
{
public:
	int		m_iSize;
	int		m_iVersionNr[MAX_VERSION_NR + 1];

	//	char	m_szVersionAppUserFriendly[MAX_VERSION_NR+1][255];
	//	char	m_szProjectName[MAX_VERSION_NR+1][255];;
	//	char	m_szProjectUrl[MAX_VERSION_NR+1][255];
	//	char	m_szTeamName[MAX_VERSION_NR+1][255];

	CString m_sVersionApp[MAX_VERSION_NR + 1];
	CString m_sPlanClass[MAX_VERSION_NR + 1];
	//	CString m_sVersionAppUserFriendly[MAX_VERSION_NR];

	CString m_sUnknown;
};

// CThreadRpc
class CTasksProperties;
class CHistoryRpc;
class CThreadConnect;
class CRpcThreadConnectData;
class CThreadRemote;
class CComputer;
class CComputerRemoteView;
class RPC_CLIENT;
class CC_STATE;
class RESULT;
class RESULTS;
class MESSAGES;
class FILE_TRANSFERS;
class PROJECTS;
class CLoggingMessage;
class CHistoryItem;
class CHistoryCountItem;
class CHistoryProjectTasksItem;
class CComputerGraphicDeadlineListItem;
class CThreadRpc : public CWinThread
{
	DECLARE_DYNCREATE(CThreadRpc)

protected:
	CThreadRpc();           // protected constructor used by dynamic creation

	__int64		m_iTimeLeftCpu, m_iTimeLeftGpu;
	int			m_iLeftCpu, m_iLeftGpu;

public:
	virtual ~CThreadRpc();
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	//	void		ExitThread(void);

	bool		Connect();
	bool		ConnectReady(CRpcThreadConnectData* pThreadConnectData);
	void		ShowGraphic(CRpcGraphic* pRpcGraphic);
	void		GetRpcWu(int iWu, CRpcTaskInfo* pInfo);
	CFilterData* GetFilterInfo(int iFilter, int* pTotalVersion, int* pTotalWu);

	void		GetRpcMessage(int iWu, CRpcMessageInfo* pInfo);
	bool		GetRpcProjectName(CString* psProjectName, PROJECT** pInfo);
	bool		GetRpcProject(int iProjectNr, PROJECT** pInfo);
	double		GetProjectShare(std::string* pProjectUrl);
	void		GetRpcTransfer(int iTransfer, CRpcTransferInfo* pInfo);
	void		GetRpcStatistics(int iStatistics, PROJECT** pProjectStatistics, PROJECT** pProjectProject);
	int			GetGraphicTransfer(DAILY_XFER_HISTORY* pDailyXferHistory);

	void		TaskRead(CTasksProperties* pTasksProperties);

	//	void		StartRpc(CComputerRemoteView *pComputerRemoteView);

	char* m_pcComputerId;
	char* m_pcHostName;
	std::string	m_sFoundIpWithMac;
	int			m_iEncryptedConnection;
	char* m_pcMAC;
	int			m_iValidMac;
	char* m_pcPassword;
	char* m_pcBoincPlatform;
	int			m_iPort;

	int			m_iTThrottleThread;

	void		SetNumberOfDaysRegulator(int iWuTotal);
	void		SetNumberOfDays(double dDays);
	void		UpdateDaysRegulator(int iParam);
	bool		m_bRegulatorActive;
	int			m_iRegulatorMaxTasks;
	double		m_dRegulatorDaysWorkRequested;
	double		m_dRegulatorMinDaysGpu;
	double		m_dRegulatorMinDaysCpu;
	double		m_dRegulatorDaysWorkRequestedMin;

	bool		m_bLocalhostEqual;

	double		m_dCurrentWorkForDays;

	//	int			m_iSetDays;

	bool		m_bLogDebugHistoryFetching;
	bool		m_bLogDebugConnecting;
	bool		m_bLogDebugState;
	bool		m_bLogDebugHeapCheck;
	bool		m_bLogDebugTasksShow;
	time_t		m_tNextDebugTasksShow;

	int			m_iGraphicTasksPeriod;
	int			m_iGraphicDeadlinePeriod;

	bool		m_bStateFloaterFirstTime;

	RPC_CLIENT* m_pRpc;
	bool		m_bReady;
	bool		m_bReadyMessages;

	bool		m_bWSAStartup;

	VERSION_INFO* m_pVersion;
	VERSION_INFO m_BoincVersion;
	int			 m_iBoincVersion;

	int			m_iConnectionStatus;
	int			m_iAutoReconnectTime;
	time_t		m_tLastConnectionTime;
	time_t		m_tStartupConnectionTime;

	int			m_iAutoRefreshInterval;
	time_t		m_tLastAutoRefreshTransfers;

	bool		m_bConnectStartup;

	CRpcThreadConnectData* m_pThreadConnectData;
	bool		m_bThreadConnectBusy;

	// new 
	CC_STATE* m_pState;
	//	CC_STATE	*m_pStateFloater;
	MESSAGES* m_pMessages;
	PROJECTS* m_pProjects;
	PROJECTS* m_pStatisticsProjects;
	CC_STATUS* m_pCcStatus;
	RESULTS* m_pResultsTasks;
	RESULTS* m_pResultsHistory;
	FILE_TRANSFERS* m_pTransfers;
	PROJECTS* m_pStatisticsProjectsProject;

protected:
	int			GetError(int retval, int iFallback, char* pcFallback);
	int			ReadState(CC_STATE* pState);
	//	void		BuildApplicationVersionList(void);
	//	void		GetApplication(CC_STATE, *pState, RESULTS *pResults, int iIndex, char **pszUserFriendly, char **pszProjectName, int *piVersion);
	void		GetApplication(CC_STATE* pState, RESULTS* pResults, int iIndex, char** pszUserFriendly, char** pszProjectName, char** pszAccount, int* piVersion, bool* pbCpuIntensive, double* pdCpuDept, double* pdCudaDebt, double* pdAtiDebt, double* pdShare);

	CVersionApplicationInfo m_versionInfo;

	void		LogErrorNumber(char* psTxt, int iNr);
	void		Logging(char* pcFrom, char* pcTxt, int iLoggingType);
	CWnd* m_pWndLogging;
	char* m_pcBufferLog;
	char* m_pcBufferLogFrom;
	CLoggingMessage* m_pLogging;

	bool		m_bAfxErrorOnce;
	void		HeapCheck(char* pcFrom);

	void		UpdateAllProjectsWithWork();

	bool		m_bBoincClientV6;
	bool		m_bBoincOldResults;

	bool		m_bExit;
	bool		m_bUpdateState;
	bool		m_bUpdateStateFloater;
	bool		m_bUserFriendlyName;

	//	int			m_iHistoryTimeDelete;
	int			m_iHistoryCountUpdate;

	bool		m_bSmartMode;
	bool		m_bHistoryLessAccurate;
	bool		m_bHistoryChangedRunning;
	bool		m_bHistoryReconnectFetch;
	bool		m_bAdjustTime;

	int			m_iHistoryRunningTasks;
	int			m_iHistoryTimeLeftLowest;
	int			m_iHistoryFetchOverride;
	time_t		m_tHistoryNextUpdateTime;
	time_t		m_tHistoryLastTime;
	time_t		m_tHistoryNextTimeFull;
	time_t		m_tHistoryNextTimeRemove;
	time_t		m_tHistoryNextCheckMissed;
	time_t		m_tHistoryLongNextTimeRemove;

	time_t		m_tVlarNextCheck;
	CString		m_sVlar;

	int			m_iTasksFullReadInterval;
	int			m_iTasksFullReadIntervalMax;
	time_t		m_tTasksFullReadLastTime;
	bool		m_bTasksFullRead;

	//	int			m_iTasksFullReadEvery;
	//	int			m_iTasksFullReadCount;
	int			m_iTasksFullReadSize;

	int			m_iReadyCount;

	//	CList <CHistoryItem*,CHistoryItem*> *m_plHistoryStatisticsTasks;
	std::deque<CHistoryItem*>* m_plHistoryStatisticsTasks;
	std::deque<CHistoryItem*>* m_plHistoryLongStatisticsTasks;

	time_t		m_tStateLastTime;

	time_t		m_tNextThreadTime;
	__int64		m_i64LastThreadTime;
	std::deque<float> m_lThreadTime;

	//	CRpcReturn	*m_pRpcReturn;

	char* FindProject(int iPos, RESULTS* pResults);
	void		LoadAllWu();

	void		LostConnection(bool bForcedReconnect = false);

	double		m_dDiskInterval;

	//	int			m_iResultsLastRead;
	//	bool		m_iResults2Read;

	//	bool		m_bQuickAndDirty;
	int			m_iNrWu;

	void		HideResults(RESULTS* pResults);
	void		FilterResults();
	bool		TaskOperation(char* pcWuName, int iOperation);
	void		TaskOperationApplicationAndVersion(__int64* pi64FilterApplicationAndVersion, int iOperation);

	CFilterData m_filter;

	int			m_iFilterOption;
	int			m_iFilterOptionExclude;
	int			m_iFilterOptionApplicationAndVersion;
	bool		m_bFilterOptionAll;
	CString		m_sFilterOptionApplication;

	int			m_iFilterEnabledReadyToReport;
	int			m_iFilterEnabledWaitingToRun;
	int			m_iFilterEnabledReadyToStart;
	int			m_iFilterEnabledDownload;
	int			m_iFilterEnabledUpload;
	int			m_iFilterEnabledSuspend;
	int			m_iFilterEnabledComputation;
	int			m_iFilterEnabledAborted;
	int			m_iFilterEnabledRunning;

	bool		m_bShowActiveTasks;
	bool		m_bShowNonCpuIntensive;
	bool		m_bShowCpu;
	bool		m_bShowGpu;

	int			GetNewWu();
	time_t		m_tLastWuRead;

	bool		m_bAllProjectsSelected;
	std::string m_sProjectsSelected;

	int			GetNewMessages(int* piInit, int iKeep);
	int			GetNewNotices(int iNrRead, NOTICES* pNotices);

	int			m_iLowestFoundSeqNo;
	int			m_iLastMessageSeqNoRead;				// 1 = first
	int			m_iNextMessageSeqNoRead;				// 1 = first
	int			m_iNrMessages;

	CRpcThreadInfoMessage m_InfoThread;


	int			GetNewProjects();
	int			GetProject(PROJECTS* pProjects, DISK_USAGE* pDiskUsage);
	void		GetProjectList(std::deque<std::string>* plProjects);

	int			SetProjectDebt(PROJECT* pProject);

	bool		m_bReadyProjects;
	int			m_iNrProjects;

	void		ProjectOperationAllWithWork(int iOperation);
	void		ProjectOperationNotBusy(int iOperation);
	int			GetWuInAllProjects();
	int			GetWuInProject(char* pName, RESULTS* pResults, bool bReadyOnly = true);
	bool		ProjectOperationName(PROJECTS* pProject, char* pcProjectName, int iOperation, CString* pLog = NULL);
	void		ProjectOperation(PROJECTS* pProject, int iProjectNr, int iOperation);

	int			GetNewTransfers();
	int			m_bReadyTransfers;
	int			m_iNrTransfers;

	void		TransferOperationName(char* pcTransferName, int iOperation);
	void		TransferOperation(int iProjectNr, int iOperation);
	void		RefreshAllTransfers(int iType);
	int			RefreshTransfers(FILE_TRANSFERS* pTransfer, int iType, bool bShow = true);

	int			GetNewStatistics();


	int			m_iNrStatistics;
	bool		m_bReadyStatistics;

	bool		bStartRpc;
	CComputerRemoteView* m_pComputerRemoteView;

	//	int			GetFloaterData(CRpcFloaterData *pFloaterData);

	int			SetRunModeCpu(CC_STATUS* pCcStatus);
	int			SetRunModeGpu(CC_STATUS* pCcStatus);
	int			SetNetworkMode(int iNetworkMode, int iTimeMode);

	int			ReadPreference(GLOBAL_PREFS* pPrefs, GLOBAL_PREFS_MASK* pPrefsMask);
	int			WritePreference(GLOBAL_PREFS* pPrefs, GLOBAL_PREFS_MASK* pPrefsMask);
	void		ClearPreference();

	int			ReadProxy(GR_PROXY_INFO* pProxy);
	int			WriteProxy(GR_PROXY_INFO* pProxy);

	int			GetPlatform(char* pPlatform, int* piNr, int* piFound);
	int			GetHostInfo(HOST_INFO* pHi);
	void		GetTimeStats(TIME_STATS* pTimeStats);

	int			GetAllProjectsList(ALL_PROJECTS_LIST* pPl);
	int			GetProjectConfig(char* pUrl);
	int			GetProjectConfigPoll(PROJECT_CONFIG* pPc);

	int			LookupAccount(ACCOUNT_IN* pAi);
	int			LookUpAccountPoll(ACCOUNT_OUT* pAo);
	int			ProjectAttach(CRpcProjectAttach* pPa);

	int			AccountManagerRpc(CRpcManagerAttach* pManagerAttach);
	int			AccountManagerRpcPoll(ACCT_MGR_RPC_REPLY* pReply);

	int			CreateAccount(ACCOUNT_IN* pAi);
	int			CreateAccountPoll(ACCOUNT_OUT* pAo);


	int			ClientGetCCStatus(CC_STATUS* pCc);

	int			HistorySmartMode(CHistoryRpc* plHistory, int iMaxUpdateTime, bool bHistoryForceUpdate);
	int			HistoryNormalMode(CHistoryRpc* plHistory, int iMaxUpdateTime);
	//	int			History(std::deque<CHistoryItem*> *plHistory, std::deque<CHistoryItem*> *plHistoryLong, std::deque<CHistoryCountItem*> *plHistoryCount, int *piTimeLeft,  bool bActiveOnly);
	int			History(CHistoryRpc* plHistory, int* piTimeLeft, bool bActiveOnly);


	int			HistoryAdd(RESULTS* pResults, std::deque<CHistoryItem*>* plHistory, int* piTimeLeft, bool bActiveOnly);

	int			HistoryRemove(CHistoryRpc* plHistory);
	int			HistoryLongRemove(CHistoryRpc* plHistory);
	void		MoveToLongTermHistory(CHistoryRpc* plHistory, CHistoryItem* pHistoryItem);
	void		HistoryRemoveRunning(std::deque<CHistoryItem*>* plHistory);
	void		HistoryCountAdd(RESULTS* pResults, std::deque<CHistoryCountItem*>* plHistoryCount);
	void		HistoryCountDelete(std::deque<CHistoryCountItem*>* plHistoryCount);
	int			HistoryStatistics(std::deque<CHistoryProjectTasksItem*>* plHistoryProjectTasks);
	int			DeadlineStatistics(CComputerGraphicDeadlineListItem* pComputerGraphicDeadlineListItem);

	CThreadConnect* m_pThreadConnect;


	DECLARE_MESSAGE_MAP()
};



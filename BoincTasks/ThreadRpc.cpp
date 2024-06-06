// ThreadRpc.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadRpc.h"
#include "Translation.h"
#include "DlgLogging.h"
#include "MainFrm.h"
#include "DlgSettingsExpert.h"
#include "DlgGraphicDeadline.h"
#include "UseCpuGpu.h"
#include "History.h"
#include "ListViewEx.h"					// base class for BoinCComputerView
#include "TemplateRemoteTaskView.h"		// base class for BoinCComputerView
#include "ThreadConnect.h"
#include "OperationItem.h"
#include "TasksProperties.h"
#include "StringCopyNew.h"

// CThreadRpc

IMPLEMENT_DYNCREATE(CThreadRpc, CWinThread)

CThreadRpc::CThreadRpc()
{
	m_filter.m_bValid = false;
	m_bExit = false;
	m_bReady = false;
	m_bBoincClientV6 = true;
	m_bBoincOldResults = false;

	m_pState = new CC_STATE;

	m_pRpc	 = new RPC_CLIENT;

	m_pVersion = new VERSION_INFO;
	m_pResultsHistory = new RESULTS;
	m_pResultsTasks = new RESULTS;
	m_iNrWu = 0;
	

	m_pMessages = new MESSAGES;
	m_iNrMessages = 0;
	m_bReadyMessages = false;

	m_iLowestFoundSeqNo		= -1;
	m_iLastMessageSeqNoRead = RPC_FIRST_MESSAGE;
	m_iNextMessageSeqNoRead = RPC_FIRST_MESSAGE;

	m_pProjects = new PROJECTS;
	m_iNrProjects = 0;
	m_bReadyProjects	= false;

	m_bConnectStartup = true;

	bStartRpc = false;
	m_pComputerRemoteView = NULL;

	m_pTransfers = new FILE_TRANSFERS;
	m_bReadyTransfers = false;
	m_iNrTransfers = 0;

	m_pStatisticsProjects	 = new PROJECTS;
	m_pStatisticsProjectsProject = new PROJECTS;
	m_bReadyStatistics = false;
	m_iNrStatistics = 0;

	m_iFilterOption = 0;
	m_iFilterOptionExclude = FILTER_NONE;
	m_iFilterOptionApplicationAndVersion = 0;
	m_bFilterOptionAll = false;

	m_bAllProjectsSelected = true;

	m_iConnectionStatus = RPC_STATUS_NONE;

	m_bUpdateState = false;

//	m_iHistoryTimeDelete	= 0;
	m_iHistoryCountUpdate	= 1000;
	m_tHistoryNextTimeRemove = 0;
	m_tHistoryNextCheckMissed = 0;
	m_tHistoryLongNextTimeRemove = 0;

	m_iHistoryTimeLeftLowest = -1;
	m_iHistoryFetchOverride = -1;
	m_tHistoryNextUpdateTime = 0;
	m_tHistoryLastTime = 0;
	m_tHistoryNextTimeFull = 0;

	m_dCurrentWorkForDays = 20;							// high value needed not -1

	m_iGraphicTasksPeriod = -1;
	m_iGraphicDeadlinePeriod = -1;

	m_pCcStatus = new CC_STATUS;
	
	m_bWSAStartup = false;

	m_bUserFriendlyName = true;

	m_pWndLogging = theApp.m_pDlgLogging;
	m_pcBufferLog = new char [256];
	m_pcBufferLogFrom = new char [64];

	m_bLogDebugHistoryFetching = false;
	m_bLogDebugConnecting = false;
	m_bLogDebugState = false;
	m_bLogDebugHeapCheck = false;
	m_bLogDebugTasksShow = false;
	m_tNextDebugTasksShow = 0;

	m_iAutoReconnectTime = 0;

	m_iAutoRefreshInterval	= 0;						// 0 is ok
	m_tLastAutoRefreshTransfers = 0;

	m_tLastWuRead = 0;

	m_dDiskInterval = 0;
	m_bSmartMode = false;
	m_bAdjustTime = false;
	m_bHistoryLessAccurate = false;
	m_bHistoryChangedRunning = false;
	m_bHistoryReconnectFetch = true;
	m_iHistoryRunningTasks = 0;
	m_iReadyCount = 0;

	m_pThreadConnect = NULL;

	m_pcComputerId = NULL;
	m_pcHostName = NULL;
	m_pcMAC = NULL;
	m_iValidMac = 0;
	m_pcPassword = NULL;
	m_pcBoincPlatform = NULL;

	m_pThreadConnectData = new CRpcThreadConnectData;
	m_bThreadConnectBusy = false;
	m_pLogging = new CLoggingMessage;
	
	m_bAfxErrorOnce = false;
	m_plHistoryStatisticsTasks = NULL;
	m_plHistoryLongStatisticsTasks = NULL;

	m_tTasksFullReadLastTime = 0;
	m_iTasksFullReadInterval = 10;	// every 102 seconds, will be set later to the number of results.
	m_iTasksFullReadIntervalMax = 10;
	m_iTasksFullReadSize = -1;
	m_bTasksFullRead = false;

	m_iLeftCpu = -1;
	m_iLeftGpu = -1;
	m_iTimeLeftCpu = -1;
	m_iTimeLeftGpu = -1;

	m_iTThrottleThread = 0;

	m_bShowCpu = true;
	m_bShowGpu = true;
	m_bShowNonCpuIntensive = true;
	m_bShowActiveTasks = false;
}

CThreadRpc::~CThreadRpc()
{
	if (m_pState != NULL) delete m_pState;		m_pState = NULL;
	if (m_pMessages != NULL) delete m_pMessages;m_pMessages = NULL;
	if (m_pResultsTasks != NULL) delete m_pResultsTasks;	m_pResultsTasks = NULL;
	if (m_pResultsHistory != NULL) delete m_pResultsHistory;	m_pResultsHistory = NULL;
	if (m_pProjects != NULL) delete m_pProjects;m_pProjects = NULL;
	if (m_pTransfers != NULL) delete m_pTransfers;m_pTransfers = NULL;
	if (m_pStatisticsProjects != NULL) delete m_pStatisticsProjects;m_pStatisticsProjects = NULL;
	if (m_pStatisticsProjectsProject != NULL) delete m_pStatisticsProjectsProject;m_pStatisticsProjectsProject = NULL;
	if (m_pRpc != NULL) delete m_pRpc;m_pRpc = NULL;
	if (m_pCcStatus != NULL) delete m_pCcStatus;m_pCcStatus = NULL;
	if (m_pVersion != NULL) delete m_pVersion;m_pVersion = NULL;
	if (m_pThreadConnectData != NULL) delete m_pThreadConnectData;m_pThreadConnectData = NULL;

//  Even as they are allocated in RemoteCombinedDoc, they must be deleted here.
	if (m_pcComputerId != NULL)	delete	m_pcComputerId; m_pcComputerId = NULL;
	if (m_pcHostName != NULL)	delete	m_pcHostName;m_pcHostName = NULL; 
	if (m_pcMAC != NULL)		delete	m_pcMAC;m_pcHostName = NULL; 
	if (m_pcPassword != NULL)	delete	m_pcPassword; m_pcPassword = NULL;
// this one is created here....
	if (m_pcBoincPlatform != NULL) delete m_pcBoincPlatform; m_pcBoincPlatform = NULL;

	delete m_pLogging;
	delete m_pcBufferLog;
	delete m_pcBufferLogFrom;
}

BOOL CThreadRpc::InitInstance()
{
	HWND	hWnd;
	MSG		msg;
	time_t	tSystemTime;
	int		iThreadTimes;

	bool	bThreadTimes;

	bThreadTimes = theApp.m_pDlgSettingsExpert->m_bEnableThreadGraph;


	hWnd = NULL;
	m_tNextThreadTime = 0;
	m_i64LastThreadTime = 0;

	_time64(&tSystemTime);
	m_tNextThreadTime = tSystemTime + THREAD_TIME_RATE;

	SetThreadPriority(theApp.m_dwThreadPriority);

	iThreadTimes = 0;

	while(!m_bExit)
	{
		// calculate thread times
		if (bThreadTimes && (iThreadTimes++ > 10))
		{
			iThreadTimes = 0;
			_time64(&tSystemTime);
			if (tSystemTime > m_tNextThreadTime)
			{
				FILETIME	ftKernelTime;
				FILETIME	ftUserTime;
				FILETIME	ftExit;
				FILETIME	ftCreation;
				GetThreadTimes(this->m_hThread,  &ftCreation,  &ftExit,  &ftKernelTime,  &ftUserTime);	//100 nSec = 0.1 uSec per count
	
				ULARGE_INTEGER uli;
				ULONGLONG	llTemp;
				__int64		i64TimeKernel, i64TimeUser, i64Time;
				DWORD		dwTime;
				float		fPerc;

				uli.HighPart = ftKernelTime.dwHighDateTime;
				uli.LowPart = ftKernelTime.dwLowDateTime;
				llTemp = uli.QuadPart;
				i64TimeKernel = (__int64) (llTemp / 10000);		// in mSec

				uli.HighPart = ftUserTime.dwHighDateTime;
				uli.LowPart = ftUserTime.dwLowDateTime;
				llTemp = uli.QuadPart;
				i64TimeUser = (__int64) (llTemp / 10000);		// in mSec
				i64Time = i64TimeKernel + i64TimeUser;

				dwTime = (DWORD) (i64Time - m_i64LastThreadTime);
				m_i64LastThreadTime = i64Time;

				fPerc = (float) dwTime;
				fPerc*= 100;
				fPerc/= THREAD_TIME_RATE*1000;
				m_lThreadTime.push_back(fPerc);

				m_tNextThreadTime += THREAD_TIME_RATE;
			}
		}
		// calculate thread times

		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				hWnd = (HWND) msg.wParam;
				m_bExit = true;
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_THREAD_TIMING)
			{
				hWnd = (HWND) msg.wParam;

				std::deque<float> *plThreadTime;
				plThreadTime = (std::deque<float> *) msg.lParam;

				for (UINT iPos = 0; iPos < m_lThreadTime.size(); iPos++)
				{
					plThreadTime->push_back(m_lThreadTime.at(iPos));
				}
				m_lThreadTime.clear();
				
				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iThreadId = this->m_nThreadID;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_THREAD_TIMING, (WPARAM) pThreadReturn, (LPARAM) 0);
			}

			if (msg.message == UWM_MSG_THREAD_QUIT_CLIENT)
			{
				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					m_tStartupConnectionTime = 0;
					m_tLastConnectionTime = _time64(&tSystemTime);			// reset reconnect time		
					m_pRpc->quit();		//close down the client
				}
			}

			if (msg.message == UWM_MSG_THREAD_CONNECT_LOST)
			{
				// no connection. e.g. the client has stopped
				m_tStartupConnectionTime = 0;								// disable startup retries
				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					strcpy_s(m_pcBufferLog,127,"The connection was lost, because the client stopped");
					Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_NORMAL);
					m_iConnectionStatus = RPC_STATUS_ERROR_LOST_CONNECTION;				

					m_tLastConnectionTime = _time64(&tSystemTime);			// reset reconnect time					
				}
			}

			if (msg.message == UWM_MSG_THREAD_CONNECT_RPC)
			{
				int iTThrottleThread;

				if (m_bLogDebugHeapCheck)
				{
					strcpy_s(m_pcBufferLog,127,"WARNING: is enabled");
					Logging("Heap checking ", m_pcBufferLog, LOGGING_NORMAL);
				}

				iTThrottleThread = (int) msg.lParam;
				if (iTThrottleThread > 0)
				{
					m_iTThrottleThread = iTThrottleThread;
				}
				Connect();
			}

			if (msg.message == UWM_MSG_THREAD_THREAD_CONNECT_READY)
			{
				CRpcThreadConnectData *pThreadConnectData;

				pThreadConnectData = (CRpcThreadConnectData*) msg.lParam;

				ConnectReady(pThreadConnectData);
			}

			if (msg.message == UWM_MSG_THREAD_RECONNECT_RPC)
			{
				m_bConnectStartup = true;								//repeated connect
				m_tLastConnectionTime = 0;								//force reconnect at once
				if (m_iConnectionStatus != RPC_STATUS_CONNECTED)
				{
					LostConnection(true);	// true = forced reconnect
				}
			}

			if (msg.message == UWM_MSG_THREAD_RECONNECT_TIME_RPC)
			{
				m_iAutoReconnectTime = (int) msg.lParam;
			}

			if (msg.message == UWM_MSG_THREAD_READ_STATUS)
			{
				HWND	hWnd;
				hWnd = (HWND)msg.wParam;

				ClientGetCCStatus(m_pCcStatus);
				::PostMessage(hWnd, UWM_MSG_THREAD_READ_STATUS_READY, (WPARAM) this, (LPARAM)m_pCcStatus);
			}

			if (msg.message == UWM_MSG_THREAD_GET_SNOOZE_CPU_GPU)
			{
				BOOL	bLocalHostOnly;
				bool	bCheck;
				HWND	hWnd;
				hWnd = (HWND) msg.wParam;
				bLocalHostOnly = (BOOL) msg.lParam;

				bCheck = true;

				if (bLocalHostOnly)
				{
					bCheck = false;
					if (strstr(m_pcHostName, LOCALHOST_NAME))
					{
						bCheck = true;
					}
				}

				if (bCheck)
				{
					ClientGetCCStatus(m_pCcStatus);								// read the client status

					CRpcThreadReturn *pThreadReturn1  = new CRpcThreadReturn;
					pThreadReturn1->m_pcHostName = m_pcHostName; 
					pThreadReturn1->m_pcComputerId=m_pcComputerId;
					::PostMessage(hWnd,UWM_MSG_THREAD_SET_SNOOZE_GPU_READY, (WPARAM) pThreadReturn1, (LPARAM) m_pCcStatus);	// report snooze status

					CRpcThreadReturn *pThreadReturn2  = new CRpcThreadReturn;
					pThreadReturn2->m_pcHostName = m_pcHostName; 
					pThreadReturn2->m_pcComputerId=m_pcComputerId;
					pThreadReturn2->m_pThread = this;
					pThreadReturn2->m_iStatus = m_iConnectionStatus;
					::PostMessage(hWnd,UWM_MSG_THREAD_SET_SNOOZE_CPU_READY, (WPARAM) pThreadReturn2, (LPARAM) m_pCcStatus);	// report snooze status
				}
			}

			if (msg.message == UWM_MSG_THREAD_TASK_GRAPHIC)
			{
				CRpcGraphic		*pRpcGraphic;
				HWND	hWnd;
				DISPLAY_INFO	di;
				pRpcGraphic = (CRpcGraphic *) msg.lParam;
				hWnd = (HWND) msg.wParam;

				ShowGraphic(pRpcGraphic);
//				delete pTask;
//				delete pProject;
				::PostMessage(hWnd,UWM_MSG_THREAD_TASK_GRAPHIC_READY, (WPARAM) hWnd, (LPARAM) pRpcGraphic);	
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_TASK)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_TASK b");};
				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				UINT iSorting;

				iSorting = (UINT) msg.lParam;

				m_iTasksFullReadIntervalMax = iSorting >> FILTER_POS_FULL_READ_INTERVAL;

				m_iFilterOption	 =  iSorting & 0x1fff;
				m_iFilterEnabledReadyToReport	= (m_iFilterOption & (0x01<<FILTER_BIT_POS_READY_TO_REPORT))!=0;
				m_iFilterEnabledWaitingToRun	= (m_iFilterOption & (0x01<<FILTER_BIT_POS_WAITING_TO_RUN))!=0;
				m_iFilterEnabledReadyToStart	= (m_iFilterOption & (0x01<<FILTER_BIT_POS_READY_TO_START))!=0;
				m_iFilterEnabledDownload		= (m_iFilterOption & (0x01<<FILTER_BIT_POS_DOWNLOAD))!=0;
				m_iFilterEnabledUpload			= (m_iFilterOption & (0x01<<FILTER_BIT_POS_UPLOAD))!=0;
				m_iFilterEnabledSuspend			= (m_iFilterOption & (0x01<<FILTER_BIT_POS_SUSPEND))!=0;
				m_iFilterEnabledComputation		= (m_iFilterOption & (0x01<<FILTER_BIT_POS_COMPUTATION))!=0;
				m_iFilterEnabledAborted			= (m_iFilterOption & (0x01<<FILTER_BIT_POS_ABORTED))!=0;
				m_iFilterEnabledRunning			= (m_iFilterOption & (0x01<<FILTER_BIT_POS_RUNNING))!=0;
				m_bShowActiveTasks				= (m_iFilterOption & (0x01<<FILTER_BIT_POS_SHOW_RUNNING_TASKS))!=0;
				m_bShowNonCpuIntensive			= (m_iFilterOption & (0x01<<FILTER_BIT_POS_SHOW_NON_CPU_INTENSIVE))!=0;
				m_bShowCpu						= (m_iFilterOption & (0x01<<FILTER_BIT_POS_SHOW_CPU))!=0;
				m_bShowGpu						= (m_iFilterOption & (0x01<<FILTER_BIT_POS_SHOW_GPU))!=0;

				m_filter.m_bValid = false;
				int iCount;
				iCount = 0;
				iCount = GetNewWu();

				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;

				pThreadReturn->m_iNumber = iCount;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				pThreadReturn->m_dDiskInterval = m_dDiskInterval;


				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_TASK e");};

				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_TASK, (WPARAM) pThreadReturn, 0);
			}

			if (msg.message == UWM_MSG_THREAD_FILTER_TASK)
			{
				CRpcFilterInfo	*pRpcFilterInfo;
				int				iControlKey;
				pRpcFilterInfo = (CRpcFilterInfo *) msg.lParam;
				iControlKey = (int) msg.wParam;

				if (iControlKey)
				{
					if (m_iFilterOptionExclude == pRpcFilterInfo->m_iFilter)
					{
						m_iFilterOptionExclude = FILTER_NONE;
						m_iFilterOptionApplicationAndVersion = 0;
						m_sFilterOptionApplication = "";
					}
					else
					{
						m_iFilterOptionExclude = pRpcFilterInfo->m_iFilter;
						m_iFilterOptionApplicationAndVersion = pRpcFilterInfo->m_iFilterApplicationAndVersion;
						m_sFilterOptionApplication = pRpcFilterInfo->m_sApplication;
						m_bFilterOptionAll = true;
					}
				}
				else
				{
					m_bFilterOptionAll = false;
					if (m_iFilterOptionExclude == pRpcFilterInfo->m_iFilter)
					{
						if (pRpcFilterInfo->m_sApplication == m_sFilterOptionApplication)
						{
							m_iFilterOptionExclude = FILTER_NONE;
							m_iFilterOptionApplicationAndVersion = 0;
							m_sFilterOptionApplication = "";
						}
						else
						{
							m_iFilterOptionApplicationAndVersion = pRpcFilterInfo->m_iFilterApplicationAndVersion;
							m_sFilterOptionApplication = pRpcFilterInfo->m_sApplication;
						}
					}
					else 
					{
						m_iFilterOptionExclude = pRpcFilterInfo->m_iFilter;
						m_iFilterOptionApplicationAndVersion = pRpcFilterInfo->m_iFilterApplicationAndVersion;
						m_sFilterOptionApplication = pRpcFilterInfo->m_sApplication;
					}
				}
				delete pRpcFilterInfo;
			}

			if (msg.message == UWM_MSG_THREAD_TASK_OPERATION)
			{
				char *pcWuName;
				int iOperation;
				iOperation	= (int) msg.wParam;
				pcWuName	= (char *) msg.lParam;
				TaskOperation(pcWuName, iOperation);
				delete pcWuName;
			}

			if (msg.message == UWM_MSG_THREAD_TASK_OPERATION_SEND_READY)
			{
				int		iOperation;
				char	*pcWuName;
				bool	bFound;

				HWND hWnd;
				COperationItem *pOperationItem;

				hWnd = (HWND) msg.wParam;
				pOperationItem = (COperationItem *) msg.lParam;
				iOperation = pOperationItem->m_iOperation;
				pcWuName = pOperationItem->m_pcTask;

				bFound = TaskOperation(pcWuName, iOperation);
				::PostMessage(hWnd,UWM_MSG_THREAD_TASK_OPERATION_READY, 0, bFound);
			}

			if (msg.message == UWM_MSG_THREAD_TASK_OPERATION_AAV)	// by filter, version & applicationname
			{
				int			iOperation;
				__int64		*pi64ApplicationAndVersion;
				iOperation				= (int) msg.wParam;
				pi64ApplicationAndVersion	= (__int64 *) msg.lParam;
				TaskOperationApplicationAndVersion(pi64ApplicationAndVersion, iOperation);
			}

			if (msg.message == UWM_MSG_THREAD_TASK_READ)	// by filter, version & applicationname
			{
				HWND hWnd;
				CTasksProperties *pTasksProperties;
				hWnd = (HWND) msg.wParam;
				pTasksProperties = (CTasksProperties *) msg.lParam;

				TaskRead(pTasksProperties);

				::PostMessage(hWnd,UWM_MSG_THREAD_TASK_READ_READY,m_iConnectionStatus,(LPARAM) 0);
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_MESSAGE)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_MESSAGE b");};
				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				CRpcThreadSendMessage *pRpcThreadSendMessage;

				int iInit, iKeep;
				pRpcThreadSendMessage = (CRpcThreadSendMessage *) msg.lParam;
				iInit = pRpcThreadSendMessage->m_iInit;
				iKeep = pRpcThreadSendMessage->m_iKeepMessages;

				delete pRpcThreadSendMessage;

				int iCount;
				iCount = 0;
				iCount = GetNewMessages(&iInit, iKeep);

				if (hWnd != NULL)
				{
					CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
					pThreadReturn->m_iNumber = iCount;
					pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
					pThreadReturn->m_pcHostName = m_pcHostName; 
					pThreadReturn->m_pcComputerId=m_pcComputerId;
					pThreadReturn->m_pThread = this;
					pThreadReturn->m_iNrLastRead = (m_iLastMessageSeqNoRead+1) - m_iLowestFoundSeqNo;		// adjusted to first = 1; 
					pThreadReturn->m_iInit = iInit;
					pThreadReturn->m_iStatus = m_iConnectionStatus;
					::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_MESSAGE,m_iConnectionStatus,(LPARAM) pThreadReturn);
				}
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_MESSAGE e");};
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_NOTICES)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_NOTICES b");};

				CRpcThreadInfoNotices	*pThreadNotices;
				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				pThreadNotices = (CRpcThreadInfoNotices *) msg.lParam;

				int iCount;
				iCount = 0;
				iCount = GetNewNotices(pThreadNotices->m_iNrRead, pThreadNotices->m_pNotices);

				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
				pThreadReturn->m_iNumber = iCount;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iNrLastRead = pThreadNotices->m_iNrRead;
				pThreadReturn->m_iInit = 0;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
					
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_NOTICES e");};
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_NOTICES,m_iConnectionStatus,(LPARAM) pThreadReturn);
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_COMPUTER_VIEW)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_COMPUTER_VIEW b");};

				HWND hWnd;
				hWnd = (HWND) msg.wParam;
//				HOST_INFO *pHi;
//				pHi = (HOST_INFO *) msg.lParam;
//				m_iConnectionStatus = GetHostInfo(pHi);

				if (m_iConnectionStatus != RPC_STATUS_CONNECTED)
				{
					LostConnection();
				}

				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				pThreadReturn->m_pBoincVersion = &m_BoincVersion;
				pThreadReturn->m_iBoincVersion = m_iBoincVersion;
				pThreadReturn->m_pcHostName= m_pcBoincPlatform;		// Host name used for the platform name

				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_COMPUTER_VIEW e");};
				if (hWnd != NULL)
				{
					::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_COMPUTER_VIEW,m_iConnectionStatus,(LPARAM) pThreadReturn);
				}
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_PROJECT)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_PROJECT b");};
				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				int iCount;
				iCount = 0;
				iCount = GetNewProjects();

				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
				pThreadReturn->m_iNumber = iCount;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;

				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_PROJECT e");};
				if (hWnd != NULL)
				{
					::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_PROJECT,m_iConnectionStatus,(LPARAM) pThreadReturn);
				}
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_READ_PROJECT)
			{
				HWND hWnd;

				CRpcProjectsDiskUsage *pProjectsDiskUsage;

//				PROJECTS *pProjects;
//				DISK_USAGE *pDiskUsage;
				hWnd = (HWND) msg.wParam;
				pProjectsDiskUsage = (CRpcProjectsDiskUsage *) msg.lParam;

				int iStatus;
				iStatus = GetProject(pProjectsDiskUsage->pProjects,pProjectsDiskUsage->pDiskUsage);

				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
				pThreadReturn->m_pBoincVersion = &m_BoincVersion;
				pThreadReturn->m_iBoincVersion = m_iBoincVersion;
				pProjectsDiskUsage->m_bBoincClientV6 = m_bBoincClientV6;
				pProjectsDiskUsage->bHave_cuda = m_pState->have_nvidia;
				pProjectsDiskUsage->bHave_ati = m_pState->have_ati;
				if (pProjectsDiskUsage->pDiskUsage == NULL)	::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_READ_PROJECT,iStatus,(LPARAM) pThreadReturn);
				else ::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_READ_PROJECT_DISK,iStatus,(LPARAM) pThreadReturn);

			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_PROJECT_LIST)
			{
				HWND hWnd;
				std::deque<std::string> *plProjects;
				plProjects = new std::deque<std::string>;

				hWnd = (HWND) msg.wParam;

				GetProjectList(plProjects);

				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_PROJECT_LIST,0,(LPARAM) plProjects);
			}

			if (msg.message == UWM_MSG_THREAD_SET_RPC_PROJECT_LIST)
			{
				m_sProjectsSelected = *(std::string*) msg.lParam;
				if (m_sProjectsSelected.length() == 0)	m_bAllProjectsSelected = true;
				else m_bAllProjectsSelected = false;
				m_iTasksFullReadInterval = 0;	// make sure the tasks are read back completely next time
			}

			if (msg.message == UWM_MSG_THREAD_RPC_SET_PROJECT_DEBT)
			{
				PROJECT *pProject;
				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				pProject = (PROJECT *) msg.lParam;
				int iStatus;
				iStatus = SetProjectDebt(pProject);
				::PostMessage(hWnd, UWM_MSG_THREAD_RPC_SET_PROJECT_DEBT_READY, (WPARAM) iStatus, (LPARAM) 0);
			}

			if (msg.message == UWM_MSG_THREAD_PROJECT_OPERATION)
			{
				int iProjectNr, iOperation;
				iProjectNr = (int) msg.wParam;
				iOperation = (int) msg.lParam;
				ProjectOperation(m_pProjects, iProjectNr, iOperation);
			}
			if (msg.message == UWM_MSG_THREAD_PROJECT_OPERATION_BYNAME)
			{
				char *pcProjectName;
				int iOperation;

				pcProjectName = (char *) msg.lParam;
				iOperation = (int) msg.wParam;
				ProjectOperationName(m_pProjects, pcProjectName, iOperation);
				delete pcProjectName;
			}

			if (msg.message == UWM_MSG_THREAD_PROJECT_OPERATION_SEND_READY)
			{
				int		iOperation, retval;
				char	*pcProjectName;
				bool	bFound;
				HWND hWnd;
				COperationItem *pOperationItem;

				hWnd = (HWND) msg.wParam;
				pOperationItem = (COperationItem *) msg.lParam;
				iOperation = pOperationItem->m_iOperation;
				pcProjectName = pOperationItem->m_pcProject;

				CString *pLog = new CString;

				PROJECTS projects;
				retval = m_pRpc->get_project_status(projects);
				bFound = false;
				if (!retval)
				{
					pOperationItem->m_bResult = TRUE;
					if (ProjectOperationName(&projects, pcProjectName, iOperation, pLog))
					{
						pOperationItem->m_bResult = TRUE;
					}
					else
					{
						pOperationItem->m_bResult = FALSE;
					}
				}
				else
				{
					pOperationItem->m_bResult = FALSE;
				}
				projects.clear();
				::PostMessage(hWnd,UWM_MSG_THREAD_PROJECT_OPERATION_READY, (WPARAM) pLog, (LPARAM) bFound);
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_TRANSFER)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_TRANSFER b");};

				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				int iCount;
				iCount = 0;
				iCount = GetNewTransfers();

				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
				pThreadReturn->m_iNumber = iCount;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;

				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_TRANSFER e");};
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_TRANSFER,m_iConnectionStatus,(LPARAM) pThreadReturn);

			}
			if (msg.message == UWM_MSG_THREAD_TRANSFER_OPERATION_BYNAME)
			{
				char *pcTransferName;
				int iOperation;
				pcTransferName = (char *) msg.lParam;
				iOperation = (int) msg.wParam;
				TransferOperationName(pcTransferName, iOperation);
				delete pcTransferName;
			}
			if (msg.message == UWM_MSG_THREAD_TRANSFER_OPERATION)
			{
				int iTransferNr, iOperation;
				iTransferNr = (int) msg.wParam;
				iOperation = (int) msg.lParam;
				TransferOperation(iTransferNr, iOperation);
			}
			if (msg.message == UWM_MSG_THREAD_REFRESH_TRANSFERS)
			{
				int iType;

				iType = (int) msg.lParam;
				RefreshAllTransfers(iType);
			}
			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC b");};

				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				int iCount;
				iCount = GetNewStatistics();
				iCount <<=4;
				iCount += m_iConnectionStatus;
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC e");};
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_GRAPHIC,iCount,this->m_nThreadID);
			}
			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC_TRANSFER)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC_TRANSFER b");};

				HWND hWnd;
				hWnd = (HWND) msg.wParam;
				DAILY_XFER_HISTORY *pDailyXferHistory;
				pDailyXferHistory = (DAILY_XFER_HISTORY *) msg.lParam;
				int iCount;
				iCount = GetGraphicTransfer(pDailyXferHistory);
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC_TRANSFER e");};
				iCount <<=4;
				iCount += m_iConnectionStatus;
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_GRAPHIC_TRANSFER,iCount,this->m_nThreadID);
			}
			if (msg.message == UWM_MSG_THREAD_PREFERENCE_READ)
			{
				HWND hWnd;
				hWnd = (HWND) msg.wParam;

				CRpcPreferenceInfo *pPreferenceInfo;
				pPreferenceInfo = (CRpcPreferenceInfo *) msg.lParam;

				pPreferenceInfo->iReturn = ReadPreference(&pPreferenceInfo->prefs, &pPreferenceInfo->prefsMask);

				pPreferenceInfo->m_pcHostName = m_pcHostName; 
				pPreferenceInfo->m_pcComputerId=m_pcComputerId;

				pPreferenceInfo->m_iWantWorkUnits = m_iRegulatorMaxTasks;
				pPreferenceInfo->m_bBoincClientV6 = m_bBoincClientV6;

				::PostMessage(hWnd,UWM_MSG_THREAD_PREFERENCE_READ_READY, 0,m_bBoincClientV6);
			}

			if (msg.message == UWM_MSG_THREAD_PREFERENCE_WRITE)
			{
				CRpcPreferenceInfo *pPreferenceInfo;
				pPreferenceInfo = (CRpcPreferenceInfo *) msg.lParam;

				pPreferenceInfo->iReturn = WritePreference(&pPreferenceInfo->prefs, &pPreferenceInfo->prefsMask);
			}

			if (msg.message == UWM_MSG_THREAD_PROXY_READ)
			{
				HWND hWnd;
				hWnd = (HWND) msg.wParam;

				CRpcProxyInfo *pProxyInfo;
				pProxyInfo = (CRpcProxyInfo *) msg.lParam;

				pProxyInfo->iReturn = ReadProxy(&pProxyInfo->m_grProxyInfo);

				pProxyInfo->m_pcHostName = m_pcHostName; 
				pProxyInfo->m_pcComputerId=m_pcComputerId;

				::PostMessage(hWnd,UWM_MSG_THREAD_PROXY_READ_READY, 0,0);
			}
			if (msg.message == UWM_MSG_THREAD_PROXY_WRITE)
			{
				CRpcProxyInfo *pProxyInfo;
				pProxyInfo = (CRpcProxyInfo *) msg.lParam;

				pProxyInfo->iReturn = WriteProxy(&pProxyInfo->m_grProxyInfo);
			}

			if (msg.message == UWM_MSG_THREAD_NETWORK_AVAILABLE)
			{
				UpdateAllProjectsWithWork();
			}

			if (msg.message == UWM_MSG_THREAD_WU_LEFT)
			{
				m_iLeftCpu = (int) msg.wParam;
				m_iLeftGpu = (int) msg.lParam;
			}

			if (msg.message == UWM_MSG_THREAD_DAYS_LEFT)
			{
				m_iTimeLeftCpu = (int) msg.wParam;
				m_iTimeLeftGpu = (int) msg.lParam;
			}

			if (msg.message == UWM_MSG_THREAD_UPDATE_DAYS)
			{
				int	iParam;
				iParam = (int) msg.wParam;
				UpdateDaysRegulator(iParam);
			}

			if (msg.message == UWM_MSG_THREAD_UPDATE_DAYS_ASAP)
			{
				int	iParam;
				iParam = (int) msg.wParam;
				ProjectOperationNotBusy(PROJECT_OP_UPDATE);
			}

			if (msg.message == UWM_MSG_THREAD_GET_HOST_INFO)
			{
				HWND hWnd;
				HOST_INFO *pHi;
				int	 iResult;
				hWnd = (HWND) msg.wParam;
				pHi = (HOST_INFO *) msg.lParam;
				iResult = GetHostInfo(pHi);
				::PostMessage(hWnd,UWM_MSG_THREAD_GET_HOST_INFO_READY, iResult, (LPARAM) pHi);
			}

			if (msg.message == UWM_MSG_THREAD_GET_TIME_STATS)
			{
				HWND hWnd;
				TIME_STATS *pTimeStats;
				hWnd = (HWND) msg.wParam;
				pTimeStats = (TIME_STATS *) msg.lParam;
				GetTimeStats(pTimeStats);
//				::PostMessage(hWnd,UWM_MSG_THREAD_GET_TIME_STATS_READY, 0, (LPARAM) pTimeStats);	// no need last parameter is -1 if not filled.
			}

			if (msg.message == UWM_MSG_THREAD_GETPLATFORM)
			{
				CRpcReturnStringWithNr *prStringNr;
				HWND hWnd;
//				char *pStr;
				int	 iResult, iPlatformFound;
				hWnd = (HWND) msg.wParam;
				prStringNr = (CRpcReturnStringWithNr *) msg.lParam;
				iResult = GetPlatform(prStringNr->m_sStr, &prStringNr->m_iNr, &iPlatformFound);
				::PostMessage(hWnd,UWM_MSG_THREAD_GETPLATFORM_READY, iResult, iPlatformFound);
			}

			if (msg.message == UWM_MSG_THREAD_GET_ALL_PROJECTS_LIST)
			{
				HWND hWnd;
				ALL_PROJECTS_LIST *pPl;
				int	 iResult;
				hWnd = (HWND) msg.wParam;
				pPl = (ALL_PROJECTS_LIST *) msg.lParam;
				iResult = GetAllProjectsList(pPl);
				::PostMessage(hWnd,UWM_MSG_THREAD_GET_ALL_PROJECTS_LIST_READY, iResult, 0);
			}

			if (msg.message == UWM_MSG_THREAD_PROJECT_CONFIG)
			{
				HWND hWnd;
				char *pUrl;
				int	 iResult;
				hWnd = (HWND) msg.wParam;
				pUrl = (char *) msg.lParam;
				iResult = GetProjectConfig(pUrl);
				::PostMessage(hWnd,UWM_MSG_THREAD_PROJECT_CONFIG_READY, iResult, 0);
			}
			if (msg.message == UWM_MSG_THREAD_PROJECT_CONFIG_POLL)
			{
				HWND hWnd;
				int	 iResult;
				PROJECT_CONFIG *pPc;
				hWnd = (HWND) msg.wParam;
				pPc = (PROJECT_CONFIG *) msg.lParam;
				iResult = GetProjectConfigPoll(pPc);
				::PostMessage(hWnd,UWM_MSG_THREAD_PROJECT_CONFIG_POLL_READY, iResult, 0);
			}

			if (msg.message == UWM_MSG_THREAD_ACCOUNT_LOOKUP)
			{
				HWND hWnd;
				int	 iResult;
				ACCOUNT_IN *pAi;
				hWnd = (HWND) msg.wParam;
				pAi = (ACCOUNT_IN *) msg.lParam;
				iResult = LookupAccount(pAi);
				::PostMessage(hWnd,UWM_MSG_THREAD_ACCOUNT_LOOKUP_READY, iResult, 0);
			}

			if (msg.message == UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL)
			{
				HWND hWnd;
				int	 iResult;
				ACCOUNT_OUT *pAo;
				hWnd = (HWND) msg.wParam;
				pAo = (ACCOUNT_OUT *) msg.lParam;
				iResult = LookUpAccountPoll(pAo);
				::PostMessage(hWnd,UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL_READY, iResult, 0);
			}

			if (msg.message == UWM_MSG_THREAD_PROJECT_ATTACH)
			{
				HWND hWnd;
				int	 iResult;
				CRpcProjectAttach *pPa;
				hWnd = (HWND) msg.wParam;
				pPa = (CRpcProjectAttach *) msg.lParam;
				iResult = ProjectAttach(pPa);
				::PostMessage(hWnd,UWM_MSG_THREAD_PROJECT_ATTACH_READY, iResult, (LPARAM) m_pcComputerId);
			}

			if (msg.message == UWM_MSG_THREAD_CREATE_ACCOUNT)
			{
				HWND hWnd;
				int	 iResult;
				ACCOUNT_IN *pAi;
				hWnd = (HWND) msg.wParam;
				pAi = (ACCOUNT_IN *) msg.lParam;
				iResult = CreateAccount(pAi);
				::PostMessage(hWnd,UWM_MSG_THREAD_CREATE_ACCOUNT_READY, iResult, 0);
			}

			if (msg.message == UWM_MSG_THREAD_CREATE_ACCOUNT_POLL)
			{
				HWND hWnd;
				int	 iResult;
				ACCOUNT_OUT *pAo;
				hWnd = (HWND) msg.wParam;
				pAo = (ACCOUNT_OUT *) msg.lParam;
				iResult = CreateAccountPoll(pAo);
				::PostMessage(hWnd,UWM_MSG_THREAD_CREATE_ACCOUNT_POLL_READY, iResult, 0);
			}

			if (msg.message == UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO)
			{
				HWND hWnd;
				int	 iResult;
				ACCT_MGR_INFO *pAccountManagerInfo;
				hWnd = (HWND) msg.wParam;
				pAccountManagerInfo = (ACCT_MGR_INFO *) msg.lParam;
				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					iResult = m_pRpc->acct_mgr_info(*pAccountManagerInfo);
				}
				else iResult = ERR_TIMEOUT;
				::PostMessage(hWnd,UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO_READY, iResult, (LPARAM) pAccountManagerInfo);
			}

			if (msg.message == UWM_MSG_THREAD_ACCOUNTMANAGER_ATTACH)
			{
				HWND hWnd;
				int	 iResult;
				CRpcManagerAttach *pManagerAttach;
				hWnd = (HWND) msg.wParam;
				pManagerAttach = (CRpcManagerAttach *) msg.lParam;
				iResult = AccountManagerRpc(pManagerAttach);
				::PostMessage(hWnd,UWM_MSG_THREAD_ACCOUNTMANAGER_ATTACH_READY, iResult, 0);
			}

			if (msg.message == UWM_MSG_THREAD_ACCOUNTMANAGER_POLL)
			{
				HWND hWnd;
				int	 iResult;
				ACCT_MGR_RPC_REPLY *pReply;
				hWnd = (HWND) msg.wParam;
				pReply = (ACCT_MGR_RPC_REPLY *) msg.lParam;
				iResult = AccountManagerRpcPoll(pReply);
				::PostMessage(hWnd,UWM_MSG_THREAD_ACCOUNTMANAGER_POLL_READY, iResult, (LPARAM) pReply);
			}

			if (msg.message == UWM_MSG_THREAD_ACCOUNTMANAGER_SYNCHRONIZE)
			{
				HWND hWnd;
				int	 iResult;
	
				hWnd = (HWND) msg.wParam;
				CRpcManagerAttach managerAttach;
				managerAttach.m_sAuth = "";
				managerAttach.m_sName = "";
				managerAttach.m_sUrl = "";

				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					iResult = m_pRpc->acct_mgr_rpc("" , "", "",true);
				}
				else
				{
					iResult = 0;
				}

				CRpcThreadReturn * pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				pThreadReturn->m_iThreadId = this->m_nThreadID;
				::PostMessage(hWnd,UWM_MSG_THREAD_ACCOUNTMANAGER_SYNCHRONIZE_READY, iResult, (LPARAM) pThreadReturn);
			}

			if (msg.message == UWM_MSG_THREAD_RPC_HISTORY)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_RPC_HISTORY b");};

				int		iResult;
				HWND	hWnd;
				CHistoryRpc *plHistory;

				hWnd = (HWND) msg.wParam;
				plHistory = (CHistoryRpc *) msg.lParam;

				if (plHistory->m_sComputerId.length() == 0)	plHistory->m_sComputerId = m_pcComputerId;

				m_bSmartMode = plHistory->m_bSmartMode;
				m_bAdjustTime = plHistory->m_bAdjustTime;
				m_bHistoryLessAccurate = plHistory->m_bHistoryLessAccurate;
				 
				if (m_bSmartMode)
				{
					iResult = HistorySmartMode(plHistory, plHistory->m_iUpdateMaxTime, plHistory->m_bHistoryForceUpdate);
					if (m_bHistoryChangedRunning)
					{
						if (!plHistory->m_bHistoryForceUpdate)
						{
							iResult = HistorySmartMode(plHistory, plHistory->m_iUpdateMaxTime, true);	// if a tasks is gone 
						}
					}
				}
				else iResult = HistoryNormalMode(plHistory, plHistory->m_iUpdateMaxTime);

				CRpcThreadReturn * pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_iTimeLeftLowest = m_iHistoryTimeLeftLowest;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				pThreadReturn->m_iNumber = m_iReadyCount;

				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_RPC_HISTORY e");};
				::PostMessage(hWnd,UWM_MSG_THREAD_RPC_HISTORY_READY, iResult, (LPARAM) pThreadReturn);
			}

			// GRAPHIC TASKS
			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS_P)
			{
				m_iGraphicTasksPeriod = (int) msg.lParam;
				if (m_iGraphicTasksPeriod > MAX_HISTORY_STATISTICS_LEN)
				{
					m_iGraphicTasksPeriod = MAX_HISTORY_STATISTICS_LEN;
				}
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS_L)
			{
				CHistoryRpc *plHistory;
				plHistory = (CHistoryRpc *) msg.lParam;
				m_plHistoryStatisticsTasks = &plHistory->m_lHistory;	
				m_plHistoryLongStatisticsTasks = &plHistory->m_lHistoryLong;					
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS b");};

				int		iResult;
				HWND	hWnd;
				std::deque<CHistoryProjectTasksItem*> *pHistoryProjectTasks;

				hWnd = (HWND) msg.wParam;
				pHistoryProjectTasks = (std::deque<CHistoryProjectTasksItem*> *) msg.lParam;

				iResult = HistoryStatistics(pHistoryProjectTasks);

				CRpcThreadReturn * pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_iTimeLeftLowest = m_iHistoryTimeLeftLowest;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_iStatus = m_iConnectionStatus;

				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS e");};
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_GRAPHIC_TASKS, iResult, (LPARAM) pThreadReturn);
			}

			// GRAPHIC DEADLINE
			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE_P)
			{
				m_iGraphicDeadlinePeriod = (int) msg.lParam;
				if (m_iGraphicDeadlinePeriod > MAX_DEADLINE_LEN)
				{
					m_iGraphicDeadlinePeriod = MAX_DEADLINE_LEN;
				}
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE_L)
			{
	//			CHistoryRpc *plHistory;
	//			plHistory = (CHistoryRpc *) msg.lParam;
	//			m_plHistoryStatisticsTasks = &plHistory->m_lHistory;	
	//			m_plHistoryLongStatisticsTasks = &plHistory->m_lHistoryLong;					
			}

			if (msg.message == UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE)
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE b");};

				int		iResult;
				HWND	hWnd;
				CComputerGraphicDeadlineListItem *pComputerGraphicDeadlineListItem;
				hWnd = (HWND) msg.wParam;
				pComputerGraphicDeadlineListItem =  (CComputerGraphicDeadlineListItem*) msg.lParam;
				m_bAdjustTime = pComputerGraphicDeadlineListItem->m_bAdjustTime;
				iResult = DeadlineStatistics(pComputerGraphicDeadlineListItem);

				CRpcThreadReturn * pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_iThreadId = this->m_nThreadID;		// who am I
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_iStatus = m_iConnectionStatus;

				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE e");};
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_GRAPHIC_DEADLINE, iResult, (LPARAM) pThreadReturn);
			}

			// SNOOZE
			if (msg.message == UWM_MSG_THREAD_SET_SNOOZE_CPU)
			{
				HWND hWnd;
				CC_STATUS *pCcStatus;
				pCcStatus = (CC_STATUS *) msg.lParam;

				hWnd = (HWND) msg.wParam;
				SetRunModeCpu(pCcStatus);

				CRpcThreadReturn * pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				::PostMessage(hWnd,UWM_MSG_THREAD_SET_SNOOZE_CPU_READY, (WPARAM) pThreadReturn, msg.lParam);
			}

			if (msg.message == UWM_MSG_THREAD_SET_SNOOZE_GPU)
			{
				HWND hWnd;
				CC_STATUS *pCcStatus;
				pCcStatus = (CC_STATUS *) msg.lParam;

				hWnd = (HWND) msg.wParam;

				SetRunModeGpu(pCcStatus);
				CRpcThreadReturn * pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_pcHostName = m_pcHostName; 
				pThreadReturn->m_pcComputerId=m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				::PostMessage(hWnd,UWM_MSG_THREAD_SET_SNOOZE_GPU_READY, (WPARAM) pThreadReturn, msg.lParam);
			}

			if (msg.message == UWM_MSG_THREAD_COMPUTER_OPERATION_SEND_READY)
			{
				HWND hWnd;
				COperationItem *pOperationItem;

				hWnd = (HWND) msg.wParam;
				pOperationItem = (COperationItem *) msg.lParam;		

				int iMode;

				pOperationItem->m_bResult = FALSE;
				switch (pOperationItem->m_iOperation)
				{
					case COMPUTER_SNOOZE_GPU:
					case COMPUTER_SNOOZE_CANCEL_GPU:
						pOperationItem->m_bResult = SetRunModeGpu(pOperationItem->pC_STATUS);	
					break;
					case COMPUTER_SUSPEND_NETWORK:
					case COMPUTER_RESUME_NETWORK:
						iMode = SetNetworkMode(pOperationItem->pC_STATUS->network_mode, 0);
						if (iMode == -1) pOperationItem->m_bResult = false;
						else pOperationItem->m_bResult = true;
					break;
					default:
						pOperationItem->m_bResult = SetRunModeCpu(pOperationItem->pC_STATUS);	
				}
				::PostMessage(hWnd,UWM_MSG_THREAD_COMPUTER_OPERATION_READY, 0 , 0);
			}

			if (msg.message == UWM_MSG_THREAD_SET_NETWORK_MODE)
			{
				int iNetworkMode;
				HWND hWnd;
				iNetworkMode = (int) msg.lParam;	

				hWnd = (HWND) msg.wParam;
				iNetworkMode = SetNetworkMode(iNetworkMode,0);
				::PostMessage(hWnd,UWM_MSG_THREAD_SET_NETWORK_MODE_READY, (WPARAM) this, iNetworkMode);
			}

			if (msg.message == UWM_MSG_THREAD_SETTING_TASKS)
			{
				m_bUserFriendlyName = msg.wParam != 0;	
			}

			if (msg.message == UWM_MSG_THREAD_BENCHMARK)
			{
				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					m_pRpc->run_benchmarks();
				}
			}

			if (msg.message == UWM_MSG_THREAD_READ_CONFIG)
			{
				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					m_pRpc->read_cc_config();
				}
			}

			// cc_config.xml
			// WARNING !!!!!!!! We can't write the data raw
			if (msg.message == UWM_MSG_THREAD_GET_CONFIG)
			{
				HWND hWnd;
				CRpcConfig* pRpcConfig;
				int			retval;
				pRpcConfig = (CRpcConfig*)msg.lParam;

				hWnd = (HWND)msg.wParam;

				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					delete pRpcConfig->m_pConfig;
					delete pRpcConfig->m_pLogFlags;

					pRpcConfig->m_pConfig = NULL;
					pRpcConfig->m_pLogFlags = NULL;

					std::string raw_data;
					pRpcConfig->m_pConfig = new CC_CONFIG;
					pRpcConfig->m_pLogFlags = new LOG_FLAGS;
					pRpcConfig->m_pLogFlags->init();
					pRpcConfig->m_pConfig->defaults();
//					retval = m_pRpc->get_cc_config(*pRpcConfig->m_pConfig, *pRpcConfig->m_pLogFlags);
					retval = m_pRpc->get_cc_config_raw(*pRpcConfig->m_pConfig, *pRpcConfig->m_pLogFlags, raw_data);
					pRpcConfig->m_sRawData = raw_data;

					if (!retval)
					{
						pRpcConfig->m_bValid = true;
					}
				}
				CRpcThreadReturn* pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_pcHostName = m_pcHostName;
				pThreadReturn->m_pcComputerId = m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				pThreadReturn->m_bBoincClientV6 = m_bBoincClientV6;
				pThreadReturn->m_pBoincVersion = &m_BoincVersion;
				pThreadReturn->m_iBoincVersion = m_iBoincVersion;
				::PostMessage(hWnd, UWM_MSG_THREAD_GET_CONFIG_READY, (WPARAM)pThreadReturn, msg.lParam);
			}

			if (msg.message == UWM_MSG_THREAD_SET_CONFIG)
			{
				HWND hWnd;
				CRpcConfig* pRpcConfig;
				int			retval;
				pRpcConfig = (CRpcConfig*)msg.lParam;
				BOOL		bOk;

				bOk = FALSE;

				hWnd = (HWND)msg.wParam;

				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					retval = m_pRpc->set_cc_config_raw(&pRpcConfig->m_sRawData);
					if (!retval)
					{
						m_pRpc->read_cc_config();
						bOk = TRUE;
					}
				}

				::PostMessage(hWnd, UWM_MSG_THREAD_SET_CONFIG_READY, (WPARAM)0, bOk);
			}

			// app_info.xml

			if (msg.message == UWM_MSG_THREAD_GET_APP_CONFIG)
			{
				HWND hWnd;
				CRpcAppInfo	*pAppConfig;
//				int			retval;
				pAppConfig = (CRpcAppInfo *)msg.lParam;

				hWnd = (HWND)msg.wParam;

				pAppConfig->m_bValid = false;

				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					int iNrProjects = (int)m_pProjects->projects.size();
					for (int i = 0; i < iNrProjects; i++)
					{
						if (strcmp(m_pProjects->projects.at(i)->project_name.c_str(), pAppConfig->m_sProject) == 0)
						{
							pAppConfig->m_sProjectUrl = m_pProjects->projects.at(i)->master_url.c_str();
							std::string raw_data;
							int status = m_pRpc->get_app_config_raw(m_pProjects->projects.at(i)->master_url.c_str(), raw_data);
							pAppConfig->m_sRawData = raw_data;
//							pAppConfig->m_sRawData.Replace("\r","");
//							pAppConfig->m_sRawData.Replace(" ", "");
							if (status == BOINC_SUCCESS)
							{
								pAppConfig->m_bValid = true;
							}
							break;
						}
					}
				}
				CRpcThreadReturn * pThreadReturn = new CRpcThreadReturn;
				pThreadReturn->m_pcHostName = m_pcHostName;
				pThreadReturn->m_pcComputerId = m_pcComputerId;
				pThreadReturn->m_pThread = this;
				pThreadReturn->m_iStatus = m_iConnectionStatus;
				pThreadReturn->m_bBoincClientV6 = m_bBoincClientV6;
				pThreadReturn->m_pBoincVersion = &m_BoincVersion;
				pThreadReturn->m_iBoincVersion = m_iBoincVersion;
				::PostMessage(hWnd, UWM_MSG_THREAD_GET_APP_CONFIG_READY, (WPARAM)pThreadReturn, msg.lParam);
			}

			if (msg.message == UWM_MSG_THREAD_SET_APP_CONFIG)
			{
				HWND hWnd;
				CRpcAppInfo	*pAppConfig;
				int			retval;
				pAppConfig = (CRpcAppInfo *)msg.lParam;
				BOOL		bOk;

				bOk = FALSE;

				hWnd = (HWND)msg.wParam;

				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					retval = m_pRpc->set_app_config_raw(pAppConfig->m_sProjectUrl, &pAppConfig->m_sRawData);
					if (!retval)
					{
						m_pRpc->read_cc_config();
						bOk = TRUE;
					}
				}

				::PostMessage(hWnd, UWM_MSG_THREAD_SET_APP_CONFIG_READY, (WPARAM)0, bOk);
			}


 			if (msg.message == UWM_MSG_THREAD_QUIT_READY)		// just in case this happens here.
			{
				if (m_bLogDebugHeapCheck) {HeapCheck("UWM_MSG_THREAD_QUIT_READY b");};
				m_pThreadConnect = NULL;
			}

		}
		Sleep(4);
	}

	// wait for connection thread to close down

	if (m_pThreadConnect != NULL)
	{
		m_pThreadConnect->PostThreadMessage(UWM_MSG_THREAD_QUIT,(WPARAM) this->m_nThreadID,0);
	}

	int iTimeOut = 0;
	while (m_pThreadConnect != NULL)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT_READY)
			{
				m_pThreadConnect = NULL;
			}
		}
		Sleep(100);
		if (iTimeOut++ > 300) break;	// 30 second timeout	// changed 1.83 60 -> 30
	}

	Sleep(500);		// time needed for the threads to handle the post messages. Otherwise we get memory leaks


	if (strcmp(m_pcHostName, LOCALHOST_NAME) == 0)
	{
		PostMessage(theApp.m_pMainFrame->m_hWnd,UWM_MSG_THREAD_ID,-1, NULL);	// localhost no longer valid
	}

	CString *psId;
	psId = new CString;
	*psId = "BOINC, ";
	*psId+= m_pcComputerId;
	::PostMessage(hWnd,UWM_MSG_THREAD_QUIT_READY,this->m_nThreadID,(LPARAM) psId);

	if (m_bLogDebugHeapCheck) {HeapCheck("CLOSE THREAD");};

//	if (m_bWSAStartup) 	WSACleanup();

	return FALSE;
}

int CThreadRpc::ExitInstance()
{


	return CWinThread::ExitInstance();
}


void CThreadRpc::LogErrorNumber(char *pcTxt, int iNr)
{
	char cError[128];

	switch (iNr)
	{
	case ERR_GETHOSTBYNAME:
		strcpy_s(cError, 127,  "Couldn't resolve hostname");
	break;
	case ERR_AUTHENTICATOR:
		strcpy_s(cError, 127,  "scheduler request host ID doesn't match authenticator (Password)");
	break;
	case ERR_CONNECT:
		strcpy_s(cError, 127,  "connection error");
	break;
	case ERR_READ:
		strcpy_s(cError, 127,  "read error -102");
	break;
	case ERR_TIMEOUT:
		strcpy_s(cError, 127,  "init, timeout error");
	break;
	case ERR_SELECT:
		strcpy_s(cError, 127,  "socket not ready");
	break;

	default:
		_snprintf_s(cError,127,_TRUNCATE,"Error Number: %d",iNr);
	}

	if (m_sFoundIpWithMac.length() == 0)
	{
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, IP: %s, Port: %d, %s" ,m_pcComputerId, m_pcHostName,m_iPort,cError);
	}
	else
	{
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, IP: %s, MAC: %s, Port: %d, %s" ,m_pcComputerId, m_sFoundIpWithMac.c_str(), m_pcMAC, m_iPort, cError);
	}
	Logging(pcTxt,m_pcBufferLog, LOGGING_DEBUG);
}

bool CThreadRpc::Connect()
{

	if (m_bThreadConnectBusy) return false;

	if (m_pThreadConnect == NULL)
	{
		m_pThreadConnect = (CThreadConnect *) AfxBeginThread(RUNTIME_CLASS(CThreadConnect));	// start new connection thread
	}

	time_t tSystemTime;
	m_tLastConnectionTime = _time64(&tSystemTime);

	if (m_bConnectStartup)
	{
		if (strcmp(m_pcHostName,LOCALHOST_NAME) == 0)
		{
			PostMessage(theApp.m_pMainFrame->m_hWnd,UWM_MSG_THREAD_ID,this->m_nThreadID, (LPARAM) m_pcComputerId);
			_time64(&m_tStartupConnectionTime);
			m_tStartupConnectionTime += 60*2;														// The first 2 minutes retry at once at localhost.
		}
		else
		{
			m_tStartupConnectionTime = 0;															// Try to connect at once.
		}
		m_bConnectStartup = false;
	}

	if (m_iPort <= 0) m_iPort = 31416;

	m_pThreadConnectData->m_iPort = m_iPort;
	m_pThreadConnectData->m_pcHostName = m_pcHostName;
	m_pThreadConnectData->m_iEncryptedConnection = m_iEncryptedConnection;
	m_pThreadConnectData->m_pcMAC = m_pcMAC;
	m_pThreadConnectData->m_iValidMac =	m_iValidMac;
	m_pThreadConnectData->m_pcPassword = m_pcPassword;
	m_pThreadConnectData->m_iThreadId = this->m_nThreadID;
	m_pThreadConnectData->m_iRpcStatus = RPC_STATUS_NONE;
	m_pThreadConnectData->m_retval = -1;
	m_pThreadConnectData->m_pRpc = m_pRpc;

	if (!m_bThreadConnectBusy)
	{
		if (m_bLogDebugConnecting)
		{
			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Try to connect to: Host: %s, IP: %s, MAC: %s, Port: %d, Thread ID: %d" ,m_pcComputerId, m_pcHostName, m_pcMAC, m_iPort, this->m_nThreadID);
			Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_DEBUG);
		}

		m_pThreadConnect->PostThreadMessage(UWM_MSG_THREAD_THREAD_CONNECT,(WPARAM) theApp.m_pMainFrame->m_hWnd, (LPARAM) m_pThreadConnectData);
		m_bThreadConnectBusy = true;
	}

	return false;
}

bool CThreadRpc::ConnectReady(CRpcThreadConnectData *pThreadConnectData)
{
    int		retval, iNewConnectionStatus;

	m_sFoundIpWithMac = m_pThreadConnectData->m_sFoundIpWithMac;

	if (m_pcBoincPlatform != NULL)
	{
		delete m_pcBoincPlatform;
		m_pcBoincPlatform = NULL;
	}

	m_bThreadConnectBusy = false;
	if (m_iConnectionStatus != RPC_STATUS_ERROR_LOST_CONNECTION)	// keep the lost connection status
	{
		m_iConnectionStatus = RPC_STATUS_NONE;
	}

	if (pThreadConnectData != m_pThreadConnectData) return false;	// impossible check

	if (m_pThreadConnectData->m_retval)
	{
		iNewConnectionStatus = GetError(m_pThreadConnectData->m_retval, m_pThreadConnectData->m_iRpcStatus, m_pThreadConnectData->m_pcErrorMessage);
		if (m_iConnectionStatus != RPC_STATUS_ERROR_LOST_CONNECTION)	// keep the lost connection status
		{
			m_iConnectionStatus = iNewConnectionStatus;
		}
		return false;
	}

	retval = ReadState(m_pState);
	if (retval)
	{
		if (retval == ERR_READ)m_pRpc->close();	
		m_iConnectionStatus = GetError(retval, RPC_STATUS_ERROR_READING_STATE, "Connect, ReadState");
		return false;
	}

	// get the platform name

	if (m_pState->platforms.size() > 0)
	{
		char *pcPlatform;
		pcPlatform = (char *) m_pState->platforms[0].c_str();
		CStringCopyNew stringCopyNew(&m_pcBoincPlatform, pcPlatform, false);
	}

	retval = m_pRpc->exchange_versions(*m_pVersion);
	if (retval)
	{
		if (retval == ERR_READ)m_pRpc->close();
		m_iConnectionStatus = GetError(retval, RPC_STATUS_ERROR_READING_VERSION, "Connect, exchange_versions");
		return false;
	}

	m_iConnectionStatus = RPC_STATUS_CONNECTED;

	if (m_sFoundIpWithMac.length() == 0)
	{
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Connected to: Host: %s, IP: %s, Port: %d, BOINC Version: %d.%d.%d" ,m_pcComputerId, m_pcHostName, m_iPort, m_pVersion->major, m_pVersion->minor, m_pVersion->release);
	}
	else
	{
		::PostThreadMessage(m_iTThrottleThread, UWM_MSG_THREAD_IP_WITH_MAC, 0, (LPARAM) &m_sFoundIpWithMac);
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Connected to: Host: %s, IP: %s, MAC: %s, Port: %d, BOINC Version: %d.%d.%d" , m_pcComputerId, m_sFoundIpWithMac.c_str(), m_pcMAC, m_iPort, m_pVersion->major, m_pVersion->minor, m_pVersion->release);
	}
	Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_NORMAL);

	m_BoincVersion.major = m_pVersion->major;
	m_BoincVersion.minor = m_pVersion->minor;
	m_BoincVersion.release =  m_pVersion->release;

	m_iBoincVersion = m_BoincVersion.major * 10000;
	m_iBoincVersion+= m_BoincVersion.minor * 100;
	m_iBoincVersion+= m_BoincVersion.release;

	if (m_pVersion->major > 6)
	{
		m_bBoincClientV6 = false;
	}

	if (m_pVersion->major > 6)

	if (m_iBoincVersion >= 70226)
	{
		m_bBoincOldResults = true;
	}

	GLOBAL_PREFS prefs;
	GLOBAL_PREFS_MASK prefsMask;
	ReadPreference(&prefs, &prefsMask);		// this will set the checkpoint setting

	ClientGetCCStatus(m_pCcStatus);

	m_bReady = true;
	return true;
}

int CThreadRpc::GetError(int retval, int iFallback, char *pcFallback)
{
		if (retval == ERR_READ)
		{
			LogErrorNumber((char *) &"Connect, read error", retval);	
			return RPC_STATUS_ERROR_READ;
		}
		if (retval == ERR_AUTHENTICATOR)
		{
			LogErrorNumber((char *) &"Connect, password",retval);
			return RPC_STATUS_ERROR_AUTHORIZATION;
		}
		LogErrorNumber( pcFallback,retval);
		return iFallback;
}

int CThreadRpc::ReadState(CC_STATE	*pState)
{
	int retval;
	double dTime;
	LARGE_INTEGER ilStart, ilStop, ilInterval, ilFrequency;

	if (m_bLogDebugState)
	{
		QueryPerformanceFrequency(&ilFrequency ) ; // counts / second
		QueryPerformanceCounter(&ilStart) ;
	}

	retval = m_pRpc->get_state(*pState);
	if (retval)
	{
		LogErrorNumber("ReadState, get_state",retval);
		return retval;
	}

	if (m_bLogDebugState)
	{
		QueryPerformanceCounter(&ilStop) ;
		ilInterval.QuadPart = ilStop.QuadPart - ilStart.QuadPart;
		dTime = (double) ilInterval.QuadPart;
		dTime/= ilFrequency.QuadPart;
		dTime/= 10;

		int iSize = (int) pState->results.size();
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, IP: %s, Rpc Thread ID: %d, Size: %d, Time: %0.4f Sec", m_pcComputerId, m_pcHostName, this->m_nThreadID, iSize, dTime);
		Logging("Update State", m_pcBufferLog, LOGGING_DEBUG);
	}
	_time64(&m_tStateLastTime);	

	return retval;
}

void CThreadRpc::GetApplication(CC_STATE *pState, RESULTS *pResults, int iIndex, char **pszUserFriendly, char **pszProjectName, char **pszAccountName, int *piVersion, bool *pbNonCpuIntensive, double *pdCpuDept, double *pdCudaDebt, double *pdAtiDebt, double *pdShare)
{
	char	*pName, *pUrl;
	int		iNewVersionNr;
	int		iNr;

	*pbNonCpuIntensive = false;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED)
	{
		*pszAccountName = "";
		*pszProjectName = "Unconnected...";
		*pszUserFriendly = "Unconnected, please wait";
		return;
	}

	iNr = (int) pResults->results.size();
	if (iIndex < iNr)		// check if not out of range
	{
		pName = (char *) pResults->results[iIndex]->name.c_str();//.c_str();
		pUrl  = (char *) pResults->results[iIndex]->project_url.c_str();//.c_str();

		for (int iCountOld = 0; iCountOld < (int) pState->results.size(); iCountOld++)
		{
//			if (strcmp(pState->results[iCountOld]->project_url.c_str(), pUrl) != 0) continue;
//			if (strcmp(pState->results[iCountOld]->name.c_str(), pName) == 0)
			if (strcmp(pState->results[iCountOld]->name.c_str(), pName) != 0) continue;
			if (strcmp(pState->results[iCountOld]->project_url.c_str(), pUrl) != 0) continue;

			if (m_bUserFriendlyName)
			{
				*pszUserFriendly	= (char *)		pState->results[iCountOld]->app->user_friendly_name.c_str();
			}
			else
			{
				*pszUserFriendly	= (char *)		pState->results[iCountOld]->app->name.c_str();
			}
			*pszProjectName		= (char *)		pState->results[iCountOld]->app->project->project_name.c_str();
			*pszAccountName		= (char *)		pState->results[iCountOld]->app->project->user_name.c_str();
			*pbNonCpuIntensive	=				pState->results[iCountOld]->app->project->non_cpu_intensive;
			*pdCpuDept = pState->results[iCountOld]->app->project->cpu_short_term_debt;
			*pdCudaDebt = pState->results[iCountOld]->app->project->cuda_short_term_debt;
			*pdAtiDebt = pState->results[iCountOld]->app->project->ati_short_term_debt;
			*pdShare = pState->results[iCountOld]->app->project->resource_share;
			
			iNewVersionNr		=					pState->results[iCountOld]->version_num;
			if (iNewVersionNr == 0) iNewVersionNr =	pState->results[iCountOld]->app_version_num;
			if (iNewVersionNr == 0) iNewVersionNr = pState->wus[iCountOld]->version_num;
			if (iNewVersionNr == 0) return;			// nothing found take the one we already have.

			*piVersion	=	iNewVersionNr;
			return;
		}
	}
	else
	{
		*pszAccountName	= "";
		*pszProjectName = "Updating.(o)..";
		*pszUserFriendly = "Updating (o), please wait";
		return;
	}

	// not found update State the next time.
	*pszProjectName = TEXT_STATE_UPDATING;
	*pszAccountName	= "";
	*pszUserFriendly = "Updating, please wait";

	if (pState == m_pState)
	{
		if (!m_bUpdateState)
		{
//			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, Rpc Thread ID: %d, %s",m_pcHostName, this->m_nThreadID,pName);
//			Logging("Update State", m_pcBufferLog, LOGGING_DEBUG);
			m_bUpdateState = true;
		}

	}
}

void CThreadRpc::LostConnection(bool bForcedReconnect)
{
	time_t tReconnectTime;

	_time64(&tReconnectTime);	

	m_iNextMessageSeqNoRead = RPC_FIRST_MESSAGE;				// start reading at the first message
	m_iLastMessageSeqNoRead = RPC_FIRST_MESSAGE;
	m_iLowestFoundSeqNo = -1;
	m_iNrMessages = 0;

	if (!bForcedReconnect)	// if reconnect flag is set or otherwise check autoconnect
	{
		if (tReconnectTime > m_tStartupConnectionTime)				// used by localhost
		{
			// check if autoconnect is set
	
			if (m_iAutoReconnectTime == 0) return;					// 0 = disabled

			tReconnectTime -= m_iAutoReconnectTime;

			if (tReconnectTime < m_tLastConnectionTime)
			{
				return;												// not yet
			}
		}
	}
	else
	{
		if (m_bLogDebugConnecting)
		{
			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Forced reconnect request to: Host: %s, IP: %s, Port: %d" ,m_pcComputerId, m_pcHostName, m_iPort);
			Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_DEBUG);
		}
	}

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		m_iConnectionStatus = RPC_STATUS_ERROR_LOST_CONNECTION;
		
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Lost connection to: Host: %s, %s, Port: %d" ,m_pcComputerId, m_pcHostName, m_iPort);
		Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_NORMAL);

		if (!m_bThreadConnectBusy)				// never when the thread is busy.
		{
			if (m_pRpc)
			{
				delete m_pRpc;					// new 0.56
			}

			m_pRpc	 = new RPC_CLIENT;			// new 0.56

			delete m_pState;					// new 0.56
			m_pState = new CC_STATE;			// new 0.56
		}
	}

	m_bHistoryReconnectFetch = true;	//do a full fetch after a reconnect
	Connect();
}

int CThreadRpc::GetNewWu()
{
	int retval;
	int iNrWu = 0;
	RESULTS		resultsRunning;

	m_bReady = false;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		time_t tSystemTime;
		_time64(&tSystemTime);	

		if (m_bUpdateState)
		{
			// oeps rebuilding 
			ReadState(m_pState);
			m_bUpdateState = false;
		}

		if (m_bShowActiveTasks)
		{
			retval = m_pRpc->get_results_CG(*m_pResultsTasks, m_bShowCpu, m_bShowGpu, true);		// only the active tasks
		}
		else
		{
			if (tSystemTime > m_tTasksFullReadLastTime+m_iTasksFullReadInterval)			// do a full read every...
			{
				retval = m_pRpc->get_results_CG(*m_pResultsTasks,  m_bShowCpu, m_bShowGpu, false); 
				m_tTasksFullReadLastTime = tSystemTime;
				m_bTasksFullRead = true;
			}
			else
			{
				retval = m_pRpc->get_results_CG(resultsRunning, m_bShowCpu, m_bShowGpu, true);		// only the active tasks
				int iFoundRunning = 0;
				for (int iCount = 0; iCount < (int) m_pResultsTasks->results.size(); iCount++)
				{
					for (int iCountRunning = 0; iCountRunning < (int) resultsRunning.results.size(); iCountRunning++)
					{
						if (m_pResultsTasks->results[iCount]->active_task)
						{
							if (strcmp(m_pResultsTasks->results[iCount]->wu_name.c_str(), resultsRunning.results[iCountRunning]->wu_name.c_str()) == 0)
							{
								if (m_pResultsTasks->results[iCount]->state != resultsRunning.results[iCountRunning]->state)
								{
									iFoundRunning = -1;			// set to -1 to force a full read
									m_bTasksFullRead = false;	// always do a full read
									break;						// mismatch, close
								}
								if (m_pResultsTasks->results[iCount]->active_task != resultsRunning.results[iCountRunning]->active_task)
								{
									iFoundRunning = -1;			// set to -1 to force a full read
									m_bTasksFullRead = false;	// always do a full read
									break;						// mismatch, close
								}
								if (m_pResultsTasks->results[iCount]->project_suspended_via_gui != resultsRunning.results[iCountRunning]->project_suspended_via_gui)
								{
									iFoundRunning = -1;			// set to -1 to force a full read
									m_bTasksFullRead = false;	// always do a full read
									break;						// mismatch, close
								}
								if (m_pResultsTasks->results[iCount]->suspended_via_gui != resultsRunning.results[iCountRunning]->suspended_via_gui)
								{
									iFoundRunning = -1;			// set to -1 to force a full read
									m_bTasksFullRead = false;	// always do a full read
									break;						// mismatch, close
								}
	
								iFoundRunning++;
								m_pResultsTasks->results[iCount]->current_cpu_time = resultsRunning.results[iCountRunning]->current_cpu_time;
								m_pResultsTasks->results[iCount]->checkpoint_cpu_time = resultsRunning.results[iCountRunning]->checkpoint_cpu_time;
								m_pResultsTasks->results[iCount]->elapsed_time = resultsRunning.results[iCountRunning]->elapsed_time;
								m_pResultsTasks->results[iCount]->fraction_done = resultsRunning.results[iCountRunning]->fraction_done;
								m_pResultsTasks->results[iCount]->estimated_cpu_time_remaining = resultsRunning.results[iCountRunning]->estimated_cpu_time_remaining;
							}
						}
					}
				}
				if (iFoundRunning != m_iTasksFullReadSize && !m_bTasksFullRead)		// missing so do a full read anyway
				{
					m_iTasksFullReadSize = iFoundRunning;
					m_tTasksFullReadLastTime = tSystemTime;
					retval = m_pRpc->get_results_CG(*m_pResultsTasks,  m_bShowCpu, m_bShowGpu, false); 
					m_bTasksFullRead = true;
				}
				else
				{
					m_iTasksFullReadSize = iFoundRunning;
					m_bTasksFullRead = false;
				}
			}
			m_iTasksFullReadInterval = (int) (m_pResultsTasks->results.size()) / 30;

			if (m_iTasksFullReadInterval > m_iTasksFullReadIntervalMax) m_iTasksFullReadInterval = m_iTasksFullReadIntervalMax;
		}

		if (!retval)
		{
			iNrWu = (int) m_pResultsTasks->results.size();
			m_iNrWu = iNrWu;
			if (iNrWu > MAX_WU)
			{
				m_iNrWu = MAX_WU;
				AfxMessageBox("Maximum number of WU");
				return MAX_WU;
			}

			HideResults(m_pResultsTasks);
			iNrWu = (int) m_pResultsTasks->results.size();

			if (m_iFilterOption != FALSE) FilterResults();
			else
			{
				m_filter.m_bValid = false;
			}
//			Sort(iNrWu, m_pResults);
//			SetNumberOfDays();				// set number of day according to WU or number of days.		removed to updatedays

			m_bReady = true;
		}
		else
		{
			LostConnection();
		}
	}
	else
	{
		LostConnection();
	}
	
	return iNrWu;
}

void CThreadRpc::HideResults(RESULTS *pResults)
{
	if (m_bShowNonCpuIntensive && m_bAllProjectsSelected) return;
	
	bool	bRemove;
	char	*pszUserFriendly, *pszProjectName, *pszAccountName;
	int		iVersionNew, iSize, iDetectedGpu;
	bool	bNonCpuIntensive, bDebugTasksShow;
	double	dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;

	iSize = (int) pResults->results.size();

	bDebugTasksShow = false;
	if (m_bLogDebugTasksShow)
	{
		time_t tSystemTime;
		_time64( &tSystemTime);
		if (tSystemTime > m_tNextDebugTasksShow)
		{
			m_tNextDebugTasksShow = tSystemTime + 60;
			bDebugTasksShow = true;
			_snprintf_s(m_pcBufferLog,255,_TRUNCATE,"Show CPU/GPU - Begin - ShowCpu %d, ShowGpu %d ============================================================", m_bShowCpu, m_bShowGpu);
			Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_DEBUG);
		}
	}

	for (int iPos = 0; iPos < iSize; iPos++)
	{
		bRemove = false;
		iDetectedGpu = -1;
//		if (!m_bShowNonCpuIntensive)
//		{
			GetApplication(m_pState, pResults, iPos, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersionNew, &bNonCpuIntensive, &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare); 
			if (bNonCpuIntensive)
			{
				bRemove = true;
			}
//		}
/*
		if (!m_iShowCpu || !m_iShowGpu)
		{
			CUseCpuGpu CpuGpu;
			double	fCpuUse, fGpuUse;
			int		iGpu;
			bool	bCuda;
			iDetectedGpu = DETECTED_NONE;
//			char	*pTest = "0.5 CPUs + 0.5 ATI GPUs";
//			CpuGpu.GetCpuGpu((char *) pTest, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
			CpuGpu.GetCpuGpu((char *) pResults->results[iPos]->resources.c_str(), &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
			if ((!m_iShowCpu) && (iDetectedGpu == DETECTED_NONE))
			{
				bRemove = true;
			}
			if ((!m_iShowGpu) && (iDetectedGpu != DETECTED_NONE))
			{
				bRemove = true;
			}
		}
*/

		if (!m_bAllProjectsSelected)
		{
			if (StrStrI(m_sProjectsSelected.c_str(), pResults->results[iPos]->project_url.c_str()) == NULL)
			{
				bRemove = true;
			}
		}

		if (bRemove)
		{
			if (bDebugTasksShow)
			{
				char *pResources;
				pResources = (char *) pResults->results[iPos]->resources.c_str();
				_snprintf_s(m_pcBufferLog,255,_TRUNCATE,"Remove task:  Detected %d, <-> %s" , iDetectedGpu, pResources);
				Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_DEBUG);
			}

			delete pResults->results[iPos];
			pResults->results.erase(pResults->results.begin()+iPos);
			iSize = (int) pResults->results.size();
			iPos--;
		}
	}
	if (bDebugTasksShow)
	{
		_snprintf_s(m_pcBufferLog,255,_TRUNCATE,"Show CPU/GPU - End === Next in 60 seconds =======");
		Logging(LOGGING_TEXT_CONNECTION, m_pcBufferLog, LOGGING_DEBUG);
	}
}

void CThreadRpc::FilterResults()
{
	char	*pszUserFriendly, *pszProjectName, *pszAccountName;
	int		iCountWu, iCountVersion, iCountType, iState, iFilter, iFilterType, iVersionNr, iVersionNew, iApplicationAndVersion, iNrWu;
	bool	bFilterFound, bFilterSuspended;
	bool	bNonCpuIntensive;
	double	dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;

	for (iCountType = 0; iCountType < MAX_FILTER_TYPE; iCountType++)
	{
		for (iCountVersion = 0; iCountVersion < MAX_VERSION_NR; iCountVersion++)
		{
			m_filter.m_iApplicationAndVersion[iCountType][iCountVersion] = -1;
			m_filter.m_iVersion[iCountType][iCountVersion] = 0;
			m_filter.m_iVersionNrCount[iCountType][iCountVersion] = 0;
			m_filter.m_iTimeRemaining[iCountType][iCountVersion] = 0;
			m_filter.m_dTimeDeadline[iCountType][iCountVersion] = 0;
			m_filter.m_dTimeRemainingShortestDeadline[iCountType][iCountVersion] = 0;
			m_filter.m_dElapsedTime[iCountType][iCountVersion] = 0;
			m_filter.m_dCurrentCpuTime[iCountType][iCountVersion] = 0;
			m_filter.m_dFinalCpuTime[iCountType][iCountVersion] = 0;
			m_filter.m_dReceivedTime[iCountType][iCountVersion] = 0;
			m_filter.m_iExitStatus[iCountType][iCountVersion] = 0;

			m_filter.m_iFinalElapsedTime[iCountType][iCountVersion] = 0;
			m_filter.m_dFraction_done[iCountType][iCountVersion] = 0;
			m_filter.m_pszFilteredLastWU[iCountType][iCountVersion] = "";
			m_filter.m_pszAccountName[iCountType][iCountVersion] = "";
			m_filter.m_pszProjectName[iCountType][iCountVersion] = "";
			m_filter.m_pszUserFriendly[iCountType][iCountVersion] = "";
			m_filter.m_bProjectSuspendedViaGui[iCountType][iCountVersion] = false;
			strcpy_s(m_filter.m_szDescription[iCountType][iCountVersion],10,"");
		}
	}

	iNrWu = (int) m_pResultsTasks->results.size();

	for (iCountWu = 0; iCountWu < iNrWu; iCountWu++)
	{
		m_filter.m_iFilteredWu[iCountWu] = 0;

		iState = (int) m_pResultsTasks->results[iCountWu]->state << 16;
		iState+= (int) m_pResultsTasks->results[iCountWu]->scheduler_state << 8;

		bFilterSuspended = m_pResultsTasks->results[iCountWu]->suspended_via_gui;

		iVersionNew = 0;
		iFilter = FILTERED;
		GetApplication(m_pState, m_pResultsTasks, iCountWu, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersionNew, &bNonCpuIntensive, &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare); 
		iApplicationAndVersion = (iVersionNew << FILTER_SHIFT_VERSION) + iFilter;
		iVersionNr = iVersionNew;

		// use application name added to the version number.
		int iLenUserFriendly = (int) strlen(pszUserFriendly);
		if (iLenUserFriendly > 255) iLenUserFriendly = 255;
		int iSumUserFriendly = 0;
		for (int iCountUserFriendly = 0; iCountUserFriendly < iLenUserFriendly; iCountUserFriendly++)
		{
			iSumUserFriendly += pszUserFriendly[iCountUserFriendly];
		}
		// and the project name
		int iLenProjectName = (int) strlen(pszProjectName);
		if (iLenProjectName > 255) iLenProjectName = 255;
		for (int iCountProjectName = 0; iCountProjectName < iLenProjectName; iCountProjectName++)
		{
			iSumUserFriendly += pszProjectName[iCountProjectName];
		}
		// and the plann class
		char *pszPlanClass;
		pszPlanClass = (char *) m_pResultsTasks->results[iCountWu]->plan_class.c_str();
		int iLenPlanClass = (int) strlen(pszPlanClass);
		if (iLenPlanClass > 255) iLenPlanClass = 255;
		for (int iCountPlanClass = 0; iCountPlanClass < iLenPlanClass; iCountPlanClass++)
		{
			iSumUserFriendly += pszPlanClass[iCountPlanClass];
		}
		iSumUserFriendly <<= 16;
		iApplicationAndVersion += iSumUserFriendly;

		bool	biApplicationAndVersionEqual = (m_iFilterOptionApplicationAndVersion == iApplicationAndVersion);
		if (m_bFilterOptionAll) biApplicationAndVersionEqual = true;

		bFilterFound = false;
		iFilterType = FILTER_WAITING_TO_RUN;
		if (iState == STATE_WAITING_TO_RUN)
		{
			if ((m_iFilterOptionExclude == FILTER_WAITING_TO_RUN) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_pResultsTasks->results[iCountWu]->suspended_via_gui | m_pResultsTasks->results[iCountWu]->too_large | m_pResultsTasks->results[iCountWu]->needs_shmem) {}
				else
				{
					if (m_iFilterEnabledWaitingToRun) bFilterFound = true;
					iFilterType = FILTER_WAITING_TO_RUN;
				}
			}
		}
		if (iState == STATE_READY_TO_REPORT)
		{
			if ((m_iFilterOptionExclude == FILTER_READY_TO_REPORT) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_iFilterEnabledReadyToReport) bFilterFound = true;
				iFilterType = FILTER_READY_TO_REPORT;
			}
		}
		if (iState == STATE_READY_TO_START)
		{
			if ((m_iFilterOptionExclude == FILTER_READY_TO_START) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_pResultsTasks->results[iCountWu]->suspended_via_gui) {}
				else
				{
					if (m_iFilterEnabledReadyToStart) bFilterFound = true;
					iFilterType = FILTER_READY_TO_START;
				}
			}
		}
		if (iState == STATE_DOWNLOADING)
		{
			if ((m_iFilterOptionExclude == FILTER_DOWNLOAD) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_iFilterEnabledDownload) bFilterFound = true;
				iFilterType = FILTER_DOWNLOAD;
			}
		}
		if (iState == STATE_UPLOADING)
		{
			if ((m_iFilterOptionExclude == FILTER_UPLOAD) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_iFilterEnabledUpload) bFilterFound = true;
				iFilterType = FILTER_UPLOAD;
			}
		}
		if (iState == STATE_COMPUTATON_ERROR)
		{
			if ((m_iFilterOptionExclude == FILTER_COMPUTATION) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_iFilterEnabledComputation) bFilterFound = true;
				iFilterType = FILTER_COMPUTATION;
			}
		}
		if ((iState&0xff000) == STATE_ABORTED)
		{
			if ((m_iFilterOptionExclude == FILTER_ABORTED) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_iFilterEnabledAborted) bFilterFound = true;
				iFilterType = FILTER_ABORTED;
			}
		}
		if (iState == STATE_RUNNING)
		{
			if ((m_iFilterOptionExclude == FILTER_RUNNING) && biApplicationAndVersionEqual) {}
			else
			{
				if (m_pResultsTasks->results[iCountWu]->edf_scheduled)	// High P
				{
					if (m_iFilterEnabledRunning) bFilterFound = true;
					iFilterType = FILTER_RUNNING_HP;
				}
				else
				{
					if (m_iFilterEnabledRunning) bFilterFound = true;
					iFilterType = FILTER_RUNNING;
				}
			}
		}

		// this MUST be the last filter to check, because there may already be another filter active.
		if (bFilterSuspended)
		{
			if (m_iFilterEnabledSuspend)
			{
				if ((m_iFilterOptionExclude == FILTER_SUSPENDED)	&& biApplicationAndVersionEqual) {}
				else
				{
					bFilterFound = true;
					iFilterType = FILTER_SUSPENDED;
				}
			}
		}

		if (bFilterFound)
		{
			iFilter = FILTERED;
			for (iCountVersion = 0; iCountVersion < MAX_VERSION_NR; iCountVersion++)			// max version the max number of filtered on version.
			{
				if (m_filter.m_iApplicationAndVersion[iFilterType][iCountVersion] == -1)					// first empty found
				{
					m_filter.m_dTimeDeadline[iFilterType][iCountVersion]  = m_pResultsTasks->results[iCountWu]->report_deadline;
					m_filter.m_dTimeRemainingShortestDeadline[iFilterType][iCountVersion] = m_pResultsTasks->results[iCountWu]->estimated_cpu_time_remaining;
					GetApplication(m_pState, m_pResultsTasks, iCountWu, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersionNr, &bNonCpuIntensive, &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare);
					m_filter.m_iApplicationAndVersion[iFilterType][iCountVersion] = iApplicationAndVersion;				
					m_filter.m_iVersion[iFilterType][iCountVersion] = iVersionNr;		
					m_filter.m_pszAccountName[iFilterType][iCountVersion] = pszAccountName;
					m_filter.m_pszProjectName[iFilterType][iCountVersion] = pszProjectName;
					m_filter.m_pszUserFriendly[iFilterType][iCountVersion] = pszUserFriendly;
					m_filter.m_pszFilteredPlanClass[iFilterType][iCountVersion] = (char *) m_pResultsTasks->results[iCountWu]->plan_class.c_str();
					m_filter.m_pszFilteredResources[iFilterType][iCountVersion] = (char *) m_pResultsTasks->results[iCountWu]->resources.c_str();	
					m_filter.m_bProjectSuspendedViaGui[iFilterType][iCountVersion] = m_pResultsTasks->results[iCountWu]->project_suspended_via_gui;
					m_filter.m_bNonCpuIntensive[iFilterType][iCountVersion] = bNonCpuIntensive;

					m_filter.m_dCpuDebt[iFilterType][iCountVersion] = dCpuDept;
					m_filter.m_dCudaDebt[iFilterType][iCountVersion] = dCudaDebt;
					m_filter.m_dAtiDebt[iFilterType][iCountVersion] = dAtiDebt;
					m_filter.m_dCheckpointCpuTime[iFilterType][iCountVersion] = m_pResultsTasks->results[iCountWu]->checkpoint_cpu_time;

					
					m_filter.m_dProjectShare[iFilterType][iCountVersion] = dProjectShare;													// new 1.84

					m_filter.m_dReceivedTime[iFilterType][iCountVersion]  = m_pResultsTasks->results[iCountWu]->received_time;				// only one
					m_filter.m_iExitStatus[iFilterType][iCountVersion]  = m_pResultsTasks->results[iCountWu]->exit_status;					// only one
				}

				if (m_filter.m_iApplicationAndVersion[iFilterType][iCountVersion] == iApplicationAndVersion)
				{
					m_filter.m_pszFilteredLastWU[iFilterType][iCountVersion] = (char *) m_pResultsTasks->results[iCountWu]->name.c_str();		// last WU name in case of 1, 
					m_filter.m_iVersionNrCount[iFilterType][iCountVersion] += 1;		// version found
					m_filter.m_iTimeRemaining[iFilterType][iCountVersion] += (int) m_pResultsTasks->results[iCountWu]->estimated_cpu_time_remaining;
					if (m_filter.m_dTimeDeadline[iFilterType][iCountVersion]  > m_pResultsTasks->results[iCountWu]->report_deadline)
					{
						m_filter.m_dTimeDeadline[iFilterType][iCountVersion]  = m_pResultsTasks->results[iCountWu]->report_deadline;			// take shortest
						m_filter.m_dTimeRemainingShortestDeadline[iFilterType][iCountVersion] = m_pResultsTasks->results[iCountWu]->estimated_cpu_time_remaining;
					}

					m_filter.m_iFinalElapsedTime[iFilterType][iCountVersion]+= (int) m_pResultsTasks->results[iCountWu]->final_elapsed_time;
					m_filter.m_dElapsedTime[iFilterType][iCountVersion]+= m_pResultsTasks->results[iCountWu]->elapsed_time;
					m_filter.m_dCurrentCpuTime[iFilterType][iCountVersion] += m_pResultsTasks->results[iCountWu]->current_cpu_time;				// new 0.70
					m_filter.m_dFinalCpuTime[iFilterType][iCountVersion] +=	m_pResultsTasks->results[iCountWu]->final_cpu_time;					// new 0.85
					m_filter.m_dFraction_done[iFilterType][iCountVersion]+= m_pResultsTasks->results[iCountWu]->fraction_done;
					m_filter.m_iFilteredWu[iCountWu] = iFilterType+1;
					break;
				}
			}
		}
	}

	// move 



	m_filter.m_bValid = true;
}

CFilterData *CThreadRpc::GetFilterInfo(int iFilter, int *pTotalVersion, int *pTotalWu)
{
	char cBuffer[255];
	char *pszFilteredLastWU;
	int  iLenLastWU, iSumLastWU;

	m_filter.m_iTotalVersion[iFilter] = 0;
	m_filter.m_iTotalWu[iFilter] = 0;

	if (m_filter.m_bValid)
	{
		for (int iCountVersion = 0; iCountVersion < MAX_VERSION_NR; iCountVersion++)
		{
			if (m_filter.m_iVersionNrCount[iFilter][iCountVersion] == 0) break;
			m_filter.m_iTotalVersion[iFilter]++;
			m_filter.m_iTotalWu[iFilter] += m_filter.m_iVersionNrCount[iFilter][iCountVersion];

			pszFilteredLastWU = m_filter.m_pszFilteredLastWU[iFilter][iCountVersion];
			iLenLastWU = (int) strlen(pszFilteredLastWU);
			if (iLenLastWU > 255) iLenLastWU = 255;
			iSumLastWU = 0;
			for (int iCountLastWU = 0; iCountLastWU < iLenLastWU; iCountLastWU++)
			{
				iSumLastWU += pszFilteredLastWU[iCountLastWU];
			}
			_snprintf_s(cBuffer,127,_TRUNCATE,"%c%d %s %c%d %c%d",HIDE_CHAR, m_filter.m_iVersionNrCount[iFilter][iCountVersion],gszTranslation[PosViewTasksFilterTasks],HIDE_CHAR, m_filter.m_iApplicationAndVersion[iFilter][iCountVersion],HIDE_CHAR, iSumLastWU);
			strcpy_s(m_filter.m_szDescription[iFilter][iCountVersion],254,cBuffer);
		}
	}
	else
	{
		*pTotalVersion = 0;
		*pTotalWu = 0;
		return &m_filter;
	}

	*pTotalVersion = m_filter.m_iTotalVersion[iFilter];
	*pTotalWu = m_filter.m_iTotalWu[iFilter];

	return &m_filter;
}

void CThreadRpc::ShowGraphic(CRpcGraphic *pRpcGraphic)
{
	RESULT	*pResult;
	bool	bFound;
	CString sFullProgramDir, sDataDir;
	int		iSize;

	pRpcGraphic->m_iSlot = -1;

	if (strcmp(m_pcHostName, LOCALHOST_NAME) == 0)
	{
		bFound = false;
		if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
		{
			iSize = (int) m_pResultsTasks->results.size();
			for (int iCount = 0; iCount < iSize; iCount++)
			{
				if (strcmp(pRpcGraphic->m_sName, m_pResultsTasks->results[iCount]->name.c_str()) == 0)
				{
					pResult = m_pResultsTasks->results[iCount];
					bFound = true;
					break;
				}
			}
		}
	
		if (bFound)
		{
			if (strlen(pResult->graphics_exec_path.c_str()) > 0)
			{
				char *p;
				p = strrchr((char*)pResult->slot_path.c_str(), '/');
				if (!p) return;
				pRpcGraphic->m_iSlot = atoi(p+1);
				pRpcGraphic->m_sGraphicsExecPath = pResult->graphics_exec_path.c_str();
				pRpcGraphic->m_sSlotPath = pResult->slot_path.c_str();
				return;
			}
		}
	}

/*
				PROCESS_INFORMATION pi;
				STARTUPINFO         si;
				BOOL                bProcessStarted;

				memset(&pi, 0, sizeof(pi));
				memset(&si, 0, sizeof(si));

				si.cb = sizeof(si);
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_HIDE;

//				sFullProgramDir = pResult->graphics_exec_path.c_str();
				sFullProgramDir+= "switcher";
//				sFullProgramDir+= "app_graphics";// --fullscreen";
				sDataDir		= pResult->slot_path.c_str();

				bProcessStarted = CreateProcess(
					pResult->graphics_exec_path.c_str(),
					sFullProgramDir.GetBuffer(),
					NULL,
					NULL,
					FALSE,
					0,	//CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW,
					NULL,
					sDataDir,
					&si,
					&pi
				);
				if (bProcessStarted)
				{
					int ii = 1;
//					psLog->m_pFrom = ""; psLog->m_pText = "The BOINC client started OK";
//					pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
				}
			}
		}
	}
	*/

}

void CThreadRpc::GetRpcWu(int iWu, CRpcTaskInfo *pInfo)
{
	bool bNonCpuIntensive;
	RESULTS	*pResults;
	int	iNrWu;
	double dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;

	if (m_bReady)
	{
		bNonCpuIntensive = false; 

		pResults = m_pResultsTasks;
		iNrWu = (int) pResults->results.size();
//		iNrWu = m_iNrWu;

		if (iWu < iNrWu)
		{
			pInfo->m_pResult = pResults->results[iWu];

			pInfo->m_iState = (int) pInfo->m_pResult->state << 16;
			pInfo->m_iState+= (int) pInfo->m_pResult->scheduler_state << 8;
			pInfo->m_iState+= (int) pInfo->m_pResult->edf_scheduled;

			pInfo->m_pszPlanClass	= (char*) pInfo->m_pResult->plan_class.c_str();
			pInfo->m_pszResources	= (char*) pInfo->m_pResult->resources.c_str();

			pInfo->m_iVersionNumber = pResults->results[iWu]->version_num;
			GetApplication(m_pState, pResults, iWu, &pInfo->m_pszApp, &pInfo->m_pszProject, &pInfo->m_pszAccount, &pInfo->m_iVersionNumber, &bNonCpuIntensive, &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare); // warning: iVersionNr may change in GetApplication and read from the state
 		
			pInfo->m_bNonCpuIntensive = bNonCpuIntensive;

			pInfo->m_pszName = (char*) pInfo->m_pResult->name.c_str();				

			pInfo->m_iFinalElapsedTime = (int)pInfo->m_pResult->final_elapsed_time;
			pInfo->m_iRemainingTime = (int) pInfo->m_pResult->estimated_cpu_time_remaining;


			if ( pResults->results[iWu]->active_task)
			{
				pInfo->m_dElapsedTime = pInfo->m_pResult->elapsed_time;
				pInfo->m_dCurrentCpuTime = pInfo->m_pResult->current_cpu_time;
			}
			else
			{
				if ((pInfo->m_iState&0xffffe) > STATE_RUNNING)	
				{
					pInfo->m_dElapsedTime = pInfo->m_pResult->final_elapsed_time;
					pInfo->m_dCurrentCpuTime = pInfo->m_pResult->final_cpu_time;
				}
				else
				{
					pInfo->m_dElapsedTime = pInfo->m_pResult->elapsed_time;
					pInfo->m_dCurrentCpuTime = pInfo->m_pResult->current_cpu_time;
				}
			}

			pInfo->m_dFinalCpuTime = pInfo->m_pResult->final_cpu_time;


			pInfo->m_iDeadlineTime = (int) pInfo->m_pResult->report_deadline;


			pInfo->m_dFraction_done = pInfo->m_pResult->fraction_done;

			pInfo->m_pCc = m_pCcStatus;

			pInfo->m_iSuspendStatus = 0;
			if (pInfo->m_pResult->too_large)		pInfo->m_iSuspendStatus = SUSPEND_STATUS_WAITING_FOR_MEMORY;
			if (pInfo->m_pResult->needs_shmem)		pInfo->m_iSuspendStatus = SUSPEND_STATUS_WAITING_FOR_MEMORY;
			if (pInfo->m_pResult->gpu_mem_wait)		pInfo->m_iSuspendStatus = SUSPEND_STATUS_WAITING_FOR_MEMORY;

			pInfo->m_bSuspendedByGui = pInfo->m_pResult->suspended_via_gui;

			pInfo->m_iFiltered = m_filter.m_iFilteredWu[iWu];
			pInfo->m_iExitStatus = pInfo->m_pResult->exit_status;
			pInfo->m_bDeadline = false;

			pInfo->m_bSuppendedProjectByGui = pInfo->m_pResult->project_suspended_via_gui;

			pInfo->m_dCheckpointCpuTime = pInfo->m_pResult->checkpoint_cpu_time;

			pInfo->m_dCpuShortTermDebt = dCpuDept;
			pInfo->m_dCudaShortTermDebt = dCudaDebt;
			pInfo->m_dAtiShortTermDebt = dAtiDebt;

			pInfo->m_dReceivedTime = pInfo->m_pResult->received_time;

			pInfo->m_dShare = dProjectShare; 
			return;
		}
	}
	pInfo->m_pszAccount = "";
	pInfo->m_pszProject = "";
	pInfo->m_pszName = "";
	pInfo->m_dElapsedTime = 0;
	pInfo->m_iSuspendStatus = 0;
}


// operation on filtered data by filter, application name and version
void CThreadRpc::TaskOperationApplicationAndVersion(__int64 *pi64FilterApplicationAndVersion, int iOperation)
{
	char	*pszUserFriendly, *pszProjectName, *pszAccountName;
	int		iLenUserFriendly, iSumUserFriendly, iFilter, iVersionFound, iItems;
	__int64	i64State, i64FilterApplicationAndVersionFound, i64FilterApplicationAndVersion;
	bool	bNonCpuIntensive;
	double  dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return;
	if (!m_bReady)	return;

	m_tTasksFullReadLastTime = 0;

	i64FilterApplicationAndVersion = *pi64FilterApplicationAndVersion;

	iItems = (int) m_pResultsTasks->results.size();

	// find WU's
	for (int iCountWu = 0; iCountWu < iItems; iCountWu++)
	{
		iVersionFound = 0;
		iFilter = FILTERED;

//		if (!m_bQuickAndDirty)
//		{
			GetApplication(m_pState, m_pResultsTasks, iCountWu, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersionFound, &bNonCpuIntensive, &dCpuDept, &dCudaDebt, &dAtiDebt, & dProjectShare); 
			iVersionFound = (iVersionFound << FILTER_SHIFT_VERSION) + iFilter;
//		}
//		else iVersionFound = (m_pResultsTasks->results[iCountWu]->version_num << FILTER_SHIFT_VERSION) + iFilter;
		
		iLenUserFriendly = (int) strlen(pszUserFriendly);
		if (iLenUserFriendly > 255) iLenUserFriendly = 255;
		iSumUserFriendly = 0;
		for (int iCountUserFriendly = 0; iCountUserFriendly < iLenUserFriendly; iCountUserFriendly++)
		{
			iSumUserFriendly += pszUserFriendly[iCountUserFriendly];
		}
		// and the project name
		int iLenProjectName = (int) strlen(pszProjectName);
		if (iLenProjectName > 255) iLenProjectName = 255;

		for (int iCountProjectName = 0; iCountProjectName < iLenProjectName; iCountProjectName++)
		{
			iSumUserFriendly += pszProjectName[iCountProjectName];
		}
		char *pszPlanClass;
		pszPlanClass = (char *) m_pResultsTasks->results[iCountWu]->plan_class.c_str();
		int iLenPlanClass = (int) strlen(pszPlanClass);
		if (iLenPlanClass > 255) iLenPlanClass = 255;

		for (int iCountPlanClass = 0; iCountPlanClass < iLenPlanClass; iCountPlanClass++)
		{
			iSumUserFriendly += pszPlanClass[iCountPlanClass];
		}
		
		iSumUserFriendly <<= 16;
		iVersionFound += iSumUserFriendly;

		if (m_pResultsTasks->results[iCountWu]->suspended_via_gui) 
		{
			i64State = STATE_DUMMY_SUSPENDED;
		}
		else
		{
			i64State = (__int64) m_pResultsTasks->results[iCountWu]->state << 16;
			i64State+= (__int64) m_pResultsTasks->results[iCountWu]->scheduler_state << 8;
		}
		i64FilterApplicationAndVersionFound = i64State << 32;
		i64FilterApplicationAndVersionFound += iVersionFound;

		if (i64FilterApplicationAndVersion == i64FilterApplicationAndVersionFound)
		{	// filter, version and application name match
			TaskOperation((char *) m_pResultsTasks->results[iCountWu]->name.c_str(),iOperation);
		}
	}
}

bool CThreadRpc::TaskOperation(char  *pcWuName, int iOperation)
{
//result_op(RESULT&, const char* op) ¶
//Perform a control operation on an active result. op is one of "suspend", "resume", or "abort". 

	RESULT *pResult;
	char *pcOp;
	int retval, iSize;
	bool	bFound = false;

	if (!m_bReady) return bFound;
	if (m_iNrWu < 0) return bFound; 

	m_tTasksFullReadLastTime = 0;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		iSize = (int) m_pResultsTasks->results.size();

		for (int iCount = 0; iCount < iSize; iCount++)
		{
			if (strcmp(pcWuName, m_pResultsTasks->results[iCount]->name.c_str()) == 0)
			{
				pResult = m_pResultsTasks->results[iCount];
				bFound = true;
				break;
			}
		}

		if (bFound)
		{
			pcOp = "";
			switch(iOperation)
			{
				case TASKS_OP_SUSPEND:
					pcOp = "suspend";
				break;
				case TASKS_OP_RESUME:
					pcOp = "resume";
				break;
				case TASKS_OP_ABORT:
					pcOp = "abort";
				break;
			}

			retval = m_pRpc->result_op(*pResult,pcOp);

			if (!retval)
			{
			}
			else
			{
				if (retval == ERR_AUTHENTICATOR) return false;
				LostConnection();
			}
		}
	}
	else
	{
		LostConnection();
	}
	return bFound;
}

void CThreadRpc::TaskRead(CTasksProperties *pTasksProperties)
{
	RESULT *pResult;
	int iSize, iVersion;
	char *pcName;
	char *pszUserFriendly, *pszProjectName, *pszAccountName;
	bool bNonCpuIntensive;
	double  dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;

	pcName		= pTasksProperties->m_pcName;

	pTasksProperties->m_bValid = false;

	if (!m_bReady) return;
	if (m_iNrWu < 0) return; 

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		iSize = (int) m_pResultsTasks->results.size();

		for (int iCount = 0; iCount < iSize; iCount++)
		{
			if (strcmp(pcName, m_pResultsTasks->results[iCount]->name.c_str()) == 0)
			{
				pResult = m_pResultsTasks->results[iCount];
				iVersion = pResult->version_num;
				GetApplication(m_pState, m_pResultsTasks, iCount, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersion, &bNonCpuIntensive,  &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare);
				pTasksProperties->m_bValid = true;
				break;
			}
		}

		if (pTasksProperties->m_bValid)
		{
			pTasksProperties->m_iStateDebug = pResult->state;
			pTasksProperties->m_iSchedulerDebug = pResult->scheduler_state;
			pTasksProperties->m_iState = pResult->state << 16;
			pTasksProperties->m_iState+= pResult->scheduler_state << 8;			
			pTasksProperties->m_iState+= (int) pResult->edf_scheduled;

			pTasksProperties->m_bActive = pResult->active_task;
			pTasksProperties->AddApplicationName(pszUserFriendly);
			pTasksProperties->AddWuName((char *) pResult->wu_name.c_str());
			pTasksProperties->m_dCheckpointCpuTime = pResult->checkpoint_cpu_time;

			pTasksProperties->m_dDeadline = pResult->report_deadline;
			pTasksProperties->AddResources((char *) pResult->resources.c_str());

//			if (pResult->estimated_cpu_time_remaining == 0) //= completed
//			{
//				pTasksProperties->m_dCurrentCpuTime = pResult->final_cpu_time;
//				pTasksProperties->m_dElapsedTime = pResult->final_elapsed_time;
//			}
//			else
//			{
//				pTasksProperties->m_dElapsedTime = pResult->elapsed_time;
//				pTasksProperties->m_dCurrentCpuTime = pResult->current_cpu_time;
//			}

			double dElapsed, dCurrentCpuTime;
			dElapsed = 0;
			dCurrentCpuTime = 0;

			if ((pTasksProperties->m_iState&0xffffe) == STATE_RUNNING) 
			{
				dElapsed = pResult->elapsed_time;
				dCurrentCpuTime = pResult->current_cpu_time;
			}
			else
			{
				if (pResult->active_task)
				{
					dElapsed = pResult->elapsed_time;
					dCurrentCpuTime = pResult->current_cpu_time;
				}
				else
				{
					if ((pTasksProperties->m_iState&0xffffe) > STATE_RUNNING)	
					{
						dElapsed = pResult->final_elapsed_time;
						dCurrentCpuTime = pResult->final_cpu_time;
					}
					else
					{
						dElapsed = pResult->elapsed_time;
						dCurrentCpuTime = pResult->current_cpu_time;
					}
				}
			}
			pTasksProperties->m_dElapsedTime = dElapsed;
			pTasksProperties->m_dCurrentCpuTime = dCurrentCpuTime;

			pTasksProperties->m_dEstimateCpuTimeRemaining = pResult->estimated_cpu_time_remaining;
			pTasksProperties->m_dFractionDone = pResult->fraction_done;
			pTasksProperties->m_dRecieved = pResult->received_time;
			pTasksProperties->m_dSwapSize = pResult->swap_size;
			pTasksProperties->m_dWorkingSetSize= pResult->working_set_size_smoothed;
			pTasksProperties->m_iPid = pResult->pid;
			pTasksProperties->m_iSlot = pResult->slot;
			pTasksProperties->AddPlanClass((char *) pResult->plan_class.c_str());
			pTasksProperties->AddProjectUrl((char *) pResult->project_url.c_str());
			pTasksProperties->m_iVersion = iVersion;

			APP_VERSION*	pAppVersion = NULL;
			WORKUNIT*		pWorkunit	= NULL;
			RESULT* pResultState = m_pState->lookup_result(pResult->project_url, pResult->name);
			if (pResultState)
			{
				pAppVersion = pResultState->avp;
				pWorkunit = pResultState->wup;
			}

			if (pAppVersion) pTasksProperties->m_dFlops = pAppVersion->flops;
			else pTasksProperties->m_dFlops = 0;
			if (pWorkunit) pTasksProperties->m_dRscFpopsEst = pWorkunit->rsc_fpops_est;
			else pTasksProperties->m_dRscFpopsEst = 0;

			if (m_pCcStatus != NULL) pTasksProperties->m_iSuspendReason = m_pCcStatus->task_suspend_reason;
			else pTasksProperties->m_iSuspendReason = 0;

			pTasksProperties->m_iSuspendStatus = 0;
			if (pResult->too_large)			pTasksProperties->m_iSuspendStatus = SUSPEND_STATUS_WAITING_FOR_MEMORY;
			if (pResult->needs_shmem)		pTasksProperties->m_iSuspendStatus = SUSPEND_STATUS_WAITING_FOR_MEMORY;
			if (pResult->gpu_mem_wait)		pTasksProperties->m_iSuspendStatus = SUSPEND_STATUS_WAITING_FOR_MEMORY;

			pTasksProperties->m_bSuspendedViaGui = pResult->suspended_via_gui;
			pTasksProperties->m_bProjectSuspendedViaGui = pResult->project_suspended_via_gui;
		}
	}
}

int CThreadRpc::GetNewMessages(int *piInit, int iKeep)
{
	int retval;
	int iNrMessages = 0, iSeqno;
	bool bValid = true;

	m_bReadyMessages = false;

	if (*piInit == 1)
	{
		delete m_pMessages;	
		m_pMessages = new MESSAGES;		
		m_iLowestFoundSeqNo = -1;
		m_iLastMessageSeqNoRead = RPC_FIRST_MESSAGE;
		m_iNextMessageSeqNoRead = RPC_FIRST_MESSAGE;
	}

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		// remove unwanted messages
		int	iMaxRemove;
		iMaxRemove = 0;

		while (m_pMessages->messages.size() > (UINT) iKeep)
		{
			delete m_pMessages->messages.at(0);
			m_pMessages->messages.erase(m_pMessages->messages.begin());
			if (iMaxRemove++ > 2000) // 1.88 100->2000
			{
				break;			// delete only xx at a time.
			}
		}

		// check if everything is ok, messages should be sequencial if not remove and init
		iNrMessages = (int) m_pMessages->messages.size();
		bValid = true;

		if (iNrMessages > 0)
		{
			iSeqno = m_pMessages->messages[0]->seqno;
		}
		for (int iCount = 0; iCount < iNrMessages; iCount++)
		{
			if (iSeqno != m_pMessages->messages[iCount]->seqno)
			{
				bValid = false;		// oeps remove all and start all over
				delete m_pMessages;	
				m_pMessages = new MESSAGES;		
				m_iLowestFoundSeqNo = -1;
				m_iLastMessageSeqNoRead = RPC_FIRST_MESSAGE;
				m_iNextMessageSeqNoRead = RPC_FIRST_MESSAGE;
				*piInit = true;
				break;
			}
			iSeqno++;
		}

		if (iNrMessages == 0)
		{
			retval = m_pRpc->get_message_count(iSeqno);
			if (!retval)
			{
				m_iNextMessageSeqNoRead = iSeqno;
				m_iNextMessageSeqNoRead-= iKeep;
				if (m_iNextMessageSeqNoRead < 0) m_iNextMessageSeqNoRead = 0;
			}
		}

		retval = m_pRpc->get_messages(m_iNextMessageSeqNoRead,*m_pMessages,true);
		if (!retval)
		{
			m_iLastMessageSeqNoRead = m_iNextMessageSeqNoRead;
			iNrMessages = (int) m_pMessages->messages.size();
			
			if (iNrMessages > 1)
			{
				if (m_iLowestFoundSeqNo != m_pMessages->messages[0]->seqno)
				{
					m_iLowestFoundSeqNo = m_pMessages->messages[0]->seqno;		// is the first = 1
					m_iLastMessageSeqNoRead = m_pMessages->messages[0]->seqno;	// real last read
				}
				m_iNextMessageSeqNoRead = m_pMessages->messages[iNrMessages-1]->seqno;			// m_iLastMessageSeqNoRead + iNrMessages;
			}
			m_iNrMessages = iNrMessages;
			m_bReadyMessages = true;
		}
		else
		{
			LostConnection();
		}
	}
	else
	{
		iNrMessages = 0;
		LostConnection();
	}
	
	return iNrMessages;
}

void CThreadRpc::GetRpcMessage(int iWu, CRpcMessageInfo *pInfo)
{
	MESSAGES	*pMessages;
	int			iNrMessages;

	if (m_bReadyMessages)
	{
		pMessages = m_pMessages;
		iNrMessages = m_iNrMessages;

		if (iWu < iNrMessages)
		{
			if (iWu >= 0)
			{
				pInfo->m_iPriority	= pMessages->messages[iWu]->priority;
				pInfo->m_iSegNo		= pMessages->messages[iWu]->seqno;
				pInfo->m_iTimeStamp = pMessages->messages[iWu]->timestamp;
				pInfo->m_pszProject = (char *) pMessages->messages[iWu]->project.c_str();
				pInfo->m_pszBody	= (char *) pMessages->messages[iWu]->body.c_str();
				return;
			}
		}
	}
	pInfo->m_iPriority	= 0;
	pInfo->m_iSegNo		= 0;
	pInfo->m_iTimeStamp = 0;
	pInfo->m_pszProject = "";
	pInfo->m_pszBody	= "";
}

int	CThreadRpc::GetNewNotices(int iNrRead, NOTICES *pNotices)
{
	int retval, iItems;

	if (pNotices == NULL) return 0;

	pNotices->clear();

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		retval = m_pRpc->get_notices(iNrRead, *pNotices);
		if (!retval)
		{
			iItems = (int) pNotices->notices.size();
			return iItems;
		}
	}
	else
	{
		return 0;
	}
	return 0;
}

int CThreadRpc::GetNewProjects()
{
	int retval;
	int iNrProjects = 0;

	m_bReadyProjects = false;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		retval = m_pRpc->get_project_status(*m_pProjects);
		if (!retval)
		{
			iNrProjects = (int) m_pProjects->projects.size();
			m_iNrProjects = iNrProjects;
			m_bReadyProjects = true;
		}
		else
		{
			LostConnection();
		}
	}
	else
	{
		LostConnection();
	}
	
	return iNrProjects;
}

int CThreadRpc::GetProject(PROJECTS * pProjects, DISK_USAGE *pDiskUsage)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected
	retval = m_pRpc->get_project_status(*pProjects);
	
	if (pDiskUsage != NULL)
	{
		if (!retval) retval = m_pRpc->get_disk_usage(*pDiskUsage);				// and the disk usage
	}

	return retval;
}

void CThreadRpc::GetProjectList(std::deque<std::string> *plProjects)
{
	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return;	// only when connected

	GetNewProjects();	// update project status

	for (int iPos = 0; iPos < (int) m_pProjects->projects.size(); iPos++)
	{
		plProjects->push_back(m_pProjects->projects.at(iPos)->project_name);
		plProjects->push_back(m_pProjects->projects.at(iPos)->master_url);
	}
}

/*
double CThreadRpc::GetProjectShare(std::string* pProjectUrl)
{
	for (int iPos = 0; iPos < (int)m_pProjects->projects.size(); iPos++)
	{
		if (*pProjectUrl ==  m_pProjects->projects.at(iPos)->master_url)
		{
			return m_pProjects->projects.at(iPos)->resource_share;
		}
	}
	GetNewProjects();
	for (int iPos = 0; iPos < (int)m_pProjects->projects.size(); iPos++)
	{
		if (*pProjectUrl == m_pProjects->projects.at(iPos)->master_url)
		{
			return m_pProjects->projects.at(iPos)->resource_share;
		}
	}
	return 0;
}
*/

int CThreadRpc::SetProjectDebt(PROJECT *pProject)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

	std::vector<PROJECT>projects;
    projects.push_back(*pProject);

	retval = m_pRpc->set_debts(projects);
	return retval;
}

bool CThreadRpc::GetRpcProject(int iProjectNr, PROJECT **pInfo)
{
	if (m_bReadyProjects)
	{

		if (iProjectNr < m_iNrProjects)
		{

			*pInfo = m_pProjects->projects[iProjectNr];
			return true;
		}
	}
	*pInfo = NULL;
	return false;
}

void CThreadRpc::ProjectOperationAllWithWork(int iOperation)
{
	int iSize, retval;
	char *pUrl;
	RESULTS results;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return;	// only when connected

	retval = m_pRpc->get_results(results);
	if (!retval)
	{
		iSize = (int) m_pProjects->projects.size();

		for (int iCount = 0; iCount < iSize; iCount++)
		{
			pUrl = (char*) m_pProjects->projects[iCount]->master_url.c_str();
			
			int iWu = GetWuInProject(pUrl, &results); 
			if (iWu > 0)
			{
/*
				if (bWait)
				{
					// not when busy.
					if (m_pProjects->projects[iCount]->scheduler_rpc_in_progress)
					{
						continue;
					}
					// not in the timeout zone.
					if (m_pProjects->projects[iCount]->min_rpc_time != 0)
					{
						CTime ctNext((time_t) m_pProjects->projects[iCount]->min_rpc_time);
						CTime ctNow(CTime::GetCurrentTime());
						CTimeSpan toGo = ctNext - ctNow;
						if (toGo.GetTotalSeconds() >= 0)
						{
							continue;
						}
					}
				}
*/
 				ProjectOperation(m_pProjects, iCount, iOperation);
			}
		}
	}
}

void CThreadRpc::ProjectOperationNotBusy(int iOperation)
{
	int iSize, retval;
	char *pUrl;
	RESULTS results;
	bool	bLocalhost;

	std::string sProject;
	bLocalhost = m_bLocalhostEqual;
	if (strcmp(m_pcHostName, LOCALHOST_NAME) == 0)
	{
		bLocalhost = true;
	}
	if (!bLocalhost) return;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return;	// only when connected

	GetNewProjects();	// update project status

	retval = m_pRpc->get_results(results);
	if (!retval)
	{
		iSize = (int) m_pProjects->projects.size();

		for (int iCount = 0; iCount < iSize; iCount++)
		{
			pUrl = (char*) m_pProjects->projects[iCount]->master_url.c_str();
			
			int iWu = GetWuInProject(pUrl, &results, false); 
			if (iWu > 0)
			{
				sProject = m_pProjects->projects[iCount]->project_name;
				_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"%d", iOperation);
				std::string sOperation = m_pcBufferLog;
				if (iOperation = PROJECT_OP_UPDATE)
				{
					sOperation = "Update";
				}

				// not when busy.
				if (m_pProjects->projects[iCount]->scheduler_rpc_in_progress)
				{
					_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"%s, %s, %s, busy rpc in progress",sOperation.c_str(), m_pcComputerId, sProject.c_str() );
					Logging("Project operation", m_pcBufferLog, LOGGING_DEBUG);
					continue;
				}
				if (m_pProjects->projects[iCount]->sched_rpc_pending != 0)
				{
					_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"%s, %s, %s, busy rpc pending",sOperation.c_str(), m_pcComputerId, sProject.c_str()  );
					Logging("Project operation", m_pcBufferLog, LOGGING_DEBUG);
					continue;
				}
				// not in the timeout zone.
				if (m_pProjects->projects[iCount]->min_rpc_time != 0)
				{
					CTime ctNext((time_t) m_pProjects->projects[iCount]->min_rpc_time);
					CTime ctNow(CTime::GetCurrentTime());
					CTimeSpan toGo = ctNext - ctNow;
					if (toGo.GetTotalSeconds() >= 0)
					{
						_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"%s, %s, %s, busy timeout: %d seconds",sOperation.c_str(), m_pcComputerId, sProject.c_str(), toGo.GetTotalSeconds());
						Logging("Project operation", m_pcBufferLog, LOGGING_DEBUG);
						continue;
					}
				}
 				ProjectOperation(m_pProjects, iCount, iOperation);

				_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"Project %s, %s, %s",sOperation.c_str(), m_pcComputerId, sProject.c_str());
				Logging("Project operation", m_pcBufferLog, LOGGING_DEBUG);

			}
		}
	}
}

int CThreadRpc::GetWuInAllProjects()
{
	int retval, iTotalWu;
	RESULTS results;
	PROJECTS projects;

	iTotalWu = 0;
	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return 0;	// only when connected

	retval = m_pRpc->get_results(results);
	if (!retval)
	{
		iTotalWu = (int) results.results.size();
	}
	return iTotalWu;
}

// count the number of WU / project
int CThreadRpc::GetWuInProject(char *pcUrl, RESULTS *pResults, bool bReadyOnly)
{
	int iSize, iWu;

	iWu = 0;

	iSize = (int) pResults->results.size();

	for (int iCount = 0; iCount < iSize; iCount++)
	{
		if (strcmp(pcUrl, pResults->results[iCount]->project_url.c_str()) == 0)
		{
			if (bReadyOnly)
			{
				if (pResults->results[iCount]->ready_to_report)
				{
					iWu++;
				}
			}
			else
			{
				iWu++;
			}
		}
	}
	return iWu;
}


bool CThreadRpc::ProjectOperationName(PROJECTS *pProject, char *pcProjectName, int iOperation, CString *pLog)
{
	char	*pcName;
	int		iSize;
	bool	bFound;

	bFound = false;
	iSize = (int) pProject->projects.size();

	if (pLog) *pLog = "Projects found: ";

	for (int iCount = 0; iCount < iSize; iCount++)
	{
		pcName = (char*) pProject->projects[iCount]->project_name.c_str();
		if (pLog)
		{
			*pLog += pcName;
			*pLog += " ";
		}
		if (strcmp(pcProjectName, pcName) == 0)
		{
			ProjectOperation(pProject, iCount, iOperation);
			bFound = true;
		}
	}

	if (bFound) return bFound;
	// now compare on url.
	for (int iCount = 0; iCount < iSize; iCount++)
	{
		pcName = (char*) pProject->projects[iCount]->master_url.c_str();
		if (strcmp(pcProjectName, pcName) == 0)
		{
			ProjectOperation(pProject, iCount, iOperation);
			bFound = true;
		}
	}
	return bFound;
}

void CThreadRpc::ProjectOperation(PROJECTS *pProject, int iProjectNr, int iOperation)
{
	PROJECT project;
	char *pName, *pcOp;
	int retval;
	int	iNrProjects;

	iNrProjects = (int) pProject->projects.size();
	if (iProjectNr < 0) return; 
	if (iProjectNr > iNrProjects) return;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		pName = (char*) pProject->projects[iProjectNr]->project_name.c_str();

		switch(iOperation)
		{
			case PROJECT_OP_SUSPEND:
				pcOp = "suspend";
			break;
			case PROJECT_OP_RESUME:
				pcOp = "resume";
			break;
			case PROJECT_OP_UPDATE:
				pcOp = "update";
			break;
			case PROJECT_OP_NOMOREWORK:
				pcOp = "nomorework";
			break;
			case PROJECT_OP_ALLOWMOREWORK:
				pcOp = "allowmorework";
			break;
			case PROJECT_OP_RESET:
				pcOp = "reset";
			break;
			case PROJECT_OP_DETACH:
				pcOp = "detach";
			break;	
		}

		retval = m_pRpc->project_op(*pProject->projects[iProjectNr],pcOp);

		if (!retval)
		{
//			iNrProjects = (int) m_pProjects->projects.size();
//			m_iNrProjects = iNrProjects;
//			m_bReadyProjects = true;
		}
		else
		{
			if (retval == ERR_AUTHENTICATOR) return;
			LostConnection();
		}
	}
	else
	{
		LostConnection();
	}
}

int CThreadRpc::GetNewTransfers()
{
	int retval;
	int iNrTransfers = 0;

	m_bReadyTransfers = false;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		retval = m_pRpc->get_file_transfers(*m_pTransfers);
		if (!retval)
		{
			iNrTransfers = (int) m_pTransfers->file_transfers.size();
			m_iNrTransfers = iNrTransfers;
			m_bReadyTransfers = true;
		}
		else
		{
			LostConnection();
		}
	}
	else
	{
		LostConnection();
	}

// remove projects

	for (int iPos = 0; iPos < iNrTransfers; iPos++)
	{
		if (!m_bAllProjectsSelected)
		{
			if (StrStrI(m_sProjectsSelected.c_str(), m_pTransfers->file_transfers[iPos]->project_url.c_str()) == NULL)
			{
				delete m_pTransfers->file_transfers[iPos];
				m_pTransfers->file_transfers.erase(m_pTransfers->file_transfers.begin()+iPos);
				iNrTransfers = (int) m_pTransfers->file_transfers.size();
				iPos--;
			}
		}
	}

	return iNrTransfers;
}

void CThreadRpc::GetRpcTransfer(int iTransfer, CRpcTransferInfo *pInfo)
{
	FILE_TRANSFERS	*pTransfers;
	int				iNrTransfers;

	if (m_bReadyTransfers)
	{
		pTransfers = m_pTransfers;
		iNrTransfers = m_iNrTransfers;

		if (iTransfer < iNrTransfers)
		{
			pInfo->m_pFileTransfer		= pTransfers->file_transfers[iTransfer];
			pInfo->m_bBoincClientV6		= m_bBoincClientV6;
//			pInfo->m_pszName			= (char *) pTransfers->file_transfers[iTransfer]->name.c_str();
//			pInfo->m_pszProjectUrl		= (char *)pTransfers->file_transfers[iTransfer]->project_url.c_str();
//			pInfo->m_pszProjectName		= (char *)pTransfers->file_transfers[iTransfer]->project_name.c_str();
//			pInfo->m_iBytes				= (int) pTransfers->file_transfers[iTransfer]->nbytes;
//			pInfo->m_iBytesXferred		= (int) pTransfers->file_transfers[iTransfer]->bytes_xferred;
//			pInfo->m_iTimeSoFar			= (int) pTransfers->file_transfers[iTransfer]->time_so_far;
//			pInfo->m_iRetries			=	pTransfers->file_transfers[iTransfer]->num_retries;
//			pInfo->m_dXferSpeed			=	pTransfers->file_transfers[iTransfer]->xfer_speed;
//			pInfo->m_bXferActive		=	pTransfers->file_transfers[iTransfer]->xfer_active;
//			pInfo->m_bPersXferActive	=	pTransfers->file_transfers[iTransfer]->pers_xfer_active;
//			pInfo->m_iFirstRequestTime	=	pTransfers->file_transfers[iTransfer]->first_request_time;
//			pInfo->m_iNextRequestTime	=	pTransfers->file_transfers[iTransfer]->next_request_time;
//			pInfo->m_iStatus			=	pTransfers->file_transfers[iTransfer]->status;
//			pInfo->m_bUploaded			=	pTransfers->file_transfers[iTransfer]->uploaded;
//			pInfo->m_bIsUpload			=	pTransfers->file_transfers[iTransfer
//			pInfo->m_iProjectBackoff	=	(int) pTransfers->file_transfers[iTransfer]->project_backoff;

			return;
		}
	}
//	pInfo->m_pszName			= "";
//	pInfo->m_pszProjectUrl		= "";
//	pInfo->m_pszProjectName		= "";
//	pInfo->m_iBytes				= 0;
//	pInfo->m_iBytesXferred		= 0;
//	pInfo->m_iTimeSoFar			= 0;
//	pInfo->m_dXferSpeed			= 0;
//	pInfo->m_bXferActive		= false;
//	pInfo->m_bPersXferActive	= false;
//	pInfo->m_iFirstRequestTime	= 0;
//	pInfo->m_iNextRequestTime	= 0;
//	pInfo->m_iStatus			= 0;
//	pInfo->m_bUploaded			= false;
//	pInfo->m_iProjectBackoff	= 0;
}

void CThreadRpc::TransferOperationName(char *pcTransferName, int iOperation)
{
	char *pcName;
	int iSize;

	iSize = (int) m_pTransfers->file_transfers.size();

	for (int iCount = 0; iCount < iSize; iCount++)
	{
		pcName = (char*) m_pTransfers->file_transfers[iCount]->name.c_str();
		if (strcmp(pcTransferName, pcName) == 0)
		{
			TransferOperation(iCount, iOperation);
		}
	}
}

void CThreadRpc::TransferOperation(int iTransferNr, int iOperation)
{
	FILE_TRANSFERS transfer;
	char *pcOp;
	int retval;

	if (!m_bReadyTransfers) return;
	if (iTransferNr < 0) return; 
	if (iTransferNr > m_iNrTransfers) return;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
//		pName = (char*) m_pTransfers->file_transfers[iProjectNr]->project_name.c_str();
		switch(iOperation)
		{
			case TRANSFER_OP_RETRY:
				pcOp = "retry";
			break;
			case TRANSFER_OP_ABORT:
				pcOp = "abort";
			break;
		}

		retval = m_pRpc->file_transfer_op(*m_pTransfers->file_transfers[iTransferNr],pcOp);

		if (!retval)
		{
//			iNrProjects = (int) m_pProjects->projects.size();
//			m_iNrProjects = iNrProjects;
//			m_bReadyProjects = true;
		}
		else
		{
			if (retval == ERR_AUTHENTICATOR) return;
			LostConnection();
		}
	}
	else
	{
		LostConnection();
	}
}

void CThreadRpc::RefreshAllTransfers(int iType)
{
	FILE_TRANSFERS	*pTransfer;
	int				iRetval;
	int				iTransfers, iUpload, iDownload;
	bool			bLocalhost;
#define MAX_DOWNLOAD_REFRESH		100

	iUpload = 0;
	iDownload = 0;
	iTransfers = 0;
	pTransfer = NULL;

	bLocalhost = m_bLocalhostEqual;
	if (strcmp(m_pcHostName, LOCALHOST_NAME) == 0)
	{
		bLocalhost = true;
	}

	if (bLocalhost)																	// double check
	{
		if (m_iConnectionStatus == RPC_STATUS_CONNECTED)							// localhost should always be connected
		{
			if (iType < 10)
			{
				pTransfer = new FILE_TRANSFERS;
				iRetval = m_pRpc->get_file_transfers(*pTransfer);
				if (!iRetval)
				{
					RefreshTransfers(pTransfer, iType);
				}
			}
			else
			{
				time_t tSystemTime;
				_time64( &tSystemTime);
				if (tSystemTime > (m_tLastAutoRefreshTransfers +  m_iAutoRefreshInterval))
				{
					pTransfer = new FILE_TRANSFERS;
					iRetval = m_pRpc->get_file_transfers(*pTransfer);
					if (!iRetval)
					{
						iTransfers = (int) pTransfer->file_transfers.size();
						if (iTransfers > 0)
						{
							iDownload = RefreshTransfers(pTransfer, STATUS_TRANSFER_DOWNLOAD, false);
							iUpload = RefreshTransfers(pTransfer, STATUS_TRANSFER_UPLOAD, false);
						}
						else
						{
							iDownload = 0;
							iUpload = 0;

						}

						if (iTransfers > 500)																	// too many means no connection?
						{
							m_iAutoRefreshInterval += 60;
						}
						else
						{
							if (iDownload > 0 || iUpload > 0)
							{
								if (iUpload > 200 || iDownload > 200) m_iAutoRefreshInterval += 120;			// too many slow down
								else m_iAutoRefreshInterval -= 480;												// still retries
							}
							else
							{
								m_iAutoRefreshInterval += 120;													// no retries so next time longer
							}
						}
					}
					if (m_iAutoRefreshInterval < 0) m_iAutoRefreshInterval = 0;
					if (m_iAutoRefreshInterval > iType) m_iAutoRefreshInterval = iType;
					_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"%s - Transfers: %d. Download retry: %d, Upload retry: %d - Interval %d Seconds",m_pcComputerId, iTransfers, iDownload, iUpload, m_iAutoRefreshInterval);
					Logging("Refresh", m_pcBufferLog, LOGGING_NORMAL);
					m_tLastAutoRefreshTransfers = tSystemTime;
				}
			}
		}
	}
	if (pTransfer != NULL) delete pTransfer;
}



int CThreadRpc::RefreshTransfers(FILE_TRANSFERS *pTransfer, int iType, bool bShow)
{
	int		iMax, iDone, iNrTransfers;
	bool	bRetry;
	time_t	tSystemTime, tTimeOffset;
	_time64(&tSystemTime);

	// localhost only

	iDone = 0;
	iNrTransfers = 0;

	iMax = MAX_DOWNLOAD_REFRESH;									// no more than 100 at a time

	iNrTransfers = (int) pTransfer->file_transfers.size();

	for (int iCount = 0; iCount < iNrTransfers; iCount++)
	{
		if (!pTransfer->file_transfers[iCount]->xfer_active)		// only not active
		{
			bRetry = false;
			if (pTransfer->file_transfers[iCount]->project_backoff > 0)
			{
				bRetry = true;
			}

			tTimeOffset = pTransfer->file_transfers[iCount]->next_request_time - tSystemTime;
			if (tTimeOffset > 60)									// allow 1 minute backoff
			{
				bRetry = true;
			}

			if (bRetry)
			{
				if (pTransfer->file_transfers[iCount]->status == iType)
				{
					m_pRpc->file_transfer_op(*pTransfer->file_transfers[iCount], "retry");
					iMax--;
					iDone++;
					if (iMax < 0)
					{
						break;
					}
				}
			}
		}
	}
	if (bShow)
	{
		if (iType == STATUS_TRANSFER_DOWNLOAD)	_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"%s - Transfers: %d, Download retry: %d",m_pcComputerId, iNrTransfers, iDone);
		else									_snprintf_s(m_pcBufferLog,254,_TRUNCATE,"%s - Transfers: %d, Upload retry: %d",m_pcComputerId, iNrTransfers, iDone);
		Logging("Refresh", m_pcBufferLog, LOGGING_NORMAL);
	}
	return iDone;
}

int CThreadRpc::GetNewStatistics()
{
	int retval;
	int iNrStatistics = 0;

	m_bReadyStatistics = false;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{			
		retval = m_pRpc->get_statistics(*m_pStatisticsProjectsProject);		// project info
		if (!retval)
		{
			retval = m_pRpc->get_project_status(*m_pStatisticsProjects);		// statistics
			if (!retval)
			{
				iNrStatistics = (int) m_pStatisticsProjects->projects.size();
				m_iNrStatistics = iNrStatistics;
				m_bReadyStatistics = true;
			}
			else
			{
				LostConnection();
			}
		}
		else
		{
			LostConnection();
		}
	}
	else
	{
		LostConnection();
	}
	
	return iNrStatistics;
}

void CThreadRpc::GetRpcStatistics(int iStatistics, PROJECT **pProject,  PROJECT **pProjectProject)
{

	if (m_bReadyStatistics)
	{
		if (iStatistics < m_iNrStatistics)
		{
			*pProject = m_pStatisticsProjects->projects[iStatistics];
			*pProjectProject = m_pStatisticsProjectsProject->projects[iStatistics];
			return;
		}
	}
	pProject = NULL;
	return;
}

int CThreadRpc::GetGraphicTransfer(DAILY_XFER_HISTORY *pDailyXferHistory)	// V7 only
{
	int retval;
	if (m_iConnectionStatus != RPC_STATUS_CONNECTED)
	{
		return 0;
	}
	retval = m_pRpc->get_daily_xfer_history(*pDailyXferHistory);
	if (retval)
	{
		int ii = 1;	// xxxxx
//		LostConnection();
	}

	return retval;

}

int CThreadRpc::HistorySmartMode(CHistoryRpc *plHistory, int iMaxUpdateTime, bool m_bHistoryForceUpdate)
{
	bool	bFetch, bActiveOnly;
	int		iResult, iUpdateTime;

	time_t tSystemTime;
	_time64( &tSystemTime);


	if (m_tHistoryNextUpdateTime < tSystemTime) bFetch = true;
	else bFetch = false;
	if (m_iHistoryFetchOverride > 0)
	{
		m_iHistoryFetchOverride--;
		bFetch = true;
	}

	bActiveOnly = true;
	if (m_bHistoryReconnectFetch)	// a full fetch after a reconnect
	{
		bActiveOnly = false;
		bFetch = true;
	}

	if (m_bHistoryForceUpdate)	// a full fetch on a user action
	{
		bActiveOnly = false;
		bFetch = true;
	}

	if (m_bHistoryChangedRunning)	// a full fetch after new/removed running
	{
		bActiveOnly = false;
		bFetch = true;
		m_bHistoryChangedRunning = false;
	}

	if (m_iHistoryTimeLeftLowest < 30)										// 10 -> 60 1.51
	{
		bActiveOnly = false;
		bFetch = true;
	}

	if (bFetch)
	{
		if (tSystemTime > m_tHistoryNextTimeFull)
		{
			bActiveOnly = false;											// a full fetch at least every 2 minutes
		}

		m_iHistoryTimeLeftLowest = -1;
		iResult = History(plHistory, &m_iHistoryTimeLeftLowest, bActiveOnly);

		if (m_bBoincOldResults)
		{
			m_iHistoryTimeLeftLowest = 60*2;									// a full fetch every 2 minutes
			m_tHistoryNextUpdateTime = tSystemTime + 60*2;
		}
		else
		{
			if (m_iHistoryTimeLeftLowest != -1)
			{
				if (m_bHistoryLessAccurate)
				{
					if (m_iHistoryTimeLeftLowest != -1) 	m_iHistoryTimeLeftLowest /= 2;
					iUpdateTime = m_iHistoryTimeLeftLowest;
				}
				else
				{
					iUpdateTime = m_iHistoryTimeLeftLowest;	// the time is already / 2 in the history view
				}
			}
			else
			{
				iUpdateTime = 8;	// not connected every 4 second
			}
			if (iUpdateTime < UPDATE_HISTORY_TRIGGER_MIN) iUpdateTime = UPDATE_HISTORY_TRIGGER_MIN;
			if (iUpdateTime > iMaxUpdateTime) iUpdateTime = iMaxUpdateTime;
			m_tHistoryNextUpdateTime = tSystemTime + iUpdateTime;
			m_tHistoryNextUpdateTime--; // -1 to ensure triggering.
		}

		m_tHistoryLastTime = tSystemTime;
		if (!bActiveOnly)
		{
			m_tHistoryNextTimeFull = tSystemTime + 60*2;					// a full fetch at least every 2 minutes
		}

		if (m_bHistoryChangedRunning)
		{
			m_iHistoryTimeLeftLowest = 0;
		}

		if (m_iHistoryTimeLeftLowest != -1)
		{
			if (m_iHistoryTimeLeftLowest < 30) m_iHistoryFetchOverride = 2;
		}

		if (m_bLogDebugHistoryFetching && (m_iConnectionStatus == RPC_STATUS_CONNECTED))
		{
			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"%s - Next update in: %d, Time left lowest: %d, Override: %d, Active only: %d, Changed running %d",m_pcComputerId, (int) (m_tHistoryNextUpdateTime-tSystemTime),  m_iHistoryTimeLeftLowest, m_iHistoryFetchOverride, bActiveOnly, m_bHistoryChangedRunning);
			Logging("Thread history-FETCH", m_pcBufferLog, LOGGING_DEBUG);
		}
	}
	else
	{
		if (m_bBoincOldResults)
		{

		}
		else
		{
			if (m_bHistoryLessAccurate)
			{
				m_iHistoryTimeLeftLowest -= (int) ((tSystemTime - m_tHistoryLastTime)*4);
			}
			else
			{
				m_iHistoryTimeLeftLowest -= (int) ((tSystemTime - m_tHistoryLastTime)*2);
			}
		}
		m_tHistoryLastTime = tSystemTime;
		if (m_bLogDebugHistoryFetching && (m_iConnectionStatus == RPC_STATUS_CONNECTED))
		{
			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"%s - Next update in: %d, Time left lowest: %d, Override: %d, Changed running %d",m_pcComputerId, (int) (m_tHistoryNextUpdateTime-tSystemTime),  m_iHistoryTimeLeftLowest, m_iHistoryFetchOverride, m_bHistoryChangedRunning);
			Logging("Thread history-SKIP", m_pcBufferLog, LOGGING_DEBUG);
		}

		iResult = 0;
	}
	return iResult;
}
int CThreadRpc::HistoryNormalMode(CHistoryRpc *plHistory,  int iMaxUpdateTime)
{
	bool	bFetch;
	int		iResult, iUpdateTime;

	time_t tSystemTime;
	_time64( &tSystemTime);

	if (m_tHistoryNextUpdateTime < tSystemTime) bFetch = true;
	else bFetch = false;
	if (m_iHistoryFetchOverride > 0)
	{
		m_iHistoryFetchOverride--;
		bFetch = true;
	}

	if (bFetch)
	{
		m_iHistoryTimeLeftLowest = -1;
		iResult = History(plHistory, &m_iHistoryTimeLeftLowest, false);

		if (m_bHistoryLessAccurate)
		{
			if (m_iHistoryTimeLeftLowest != -1) 	m_iHistoryTimeLeftLowest /= 2;
			iUpdateTime = m_iHistoryTimeLeftLowest;
		}
		else
		{
			iUpdateTime = m_iHistoryTimeLeftLowest;	// the time is already / 2 in the history view
		}

		if (iUpdateTime < UPDATE_HISTORY_TRIGGER_MIN) iUpdateTime = UPDATE_HISTORY_TRIGGER_MIN;
		if (iUpdateTime > iMaxUpdateTime) iUpdateTime = iMaxUpdateTime;
		m_tHistoryNextUpdateTime = tSystemTime + iUpdateTime;
		m_tHistoryNextUpdateTime--; // -1 to ensure triggering.
		m_tHistoryLastTime = tSystemTime;

		if (m_iHistoryTimeLeftLowest < 30) m_iHistoryFetchOverride = 2;

		if (m_bLogDebugHistoryFetching && (m_iConnectionStatus == RPC_STATUS_CONNECTED))
		{
			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"%s - Next update in: %d, Time left lowest: %d, Override: %d, Active only: %d, Changed running %d",m_pcComputerId, (int) (m_tHistoryNextUpdateTime-tSystemTime),  m_iHistoryTimeLeftLowest, m_iHistoryFetchOverride, 0, m_bHistoryChangedRunning);
			Logging("Thread history-FETCH", m_pcBufferLog, LOGGING_DEBUG);
		}
	}
	else
	{
		m_iHistoryTimeLeftLowest -= (int) (tSystemTime - m_tHistoryLastTime);
		m_tHistoryLastTime = tSystemTime;
		if (m_bLogDebugHistoryFetching && (m_iConnectionStatus == RPC_STATUS_CONNECTED))
		{
			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"%s - Next update in: %d, Time left lowest: %d, Override: %d, Changed running %d",m_pcComputerId, (int) (m_tHistoryNextUpdateTime-tSystemTime),  m_iHistoryTimeLeftLowest, m_iHistoryFetchOverride, m_bHistoryChangedRunning);
			Logging("Thread history-SKIP", m_pcBufferLog, LOGGING_DEBUG);
		}

		iResult = 0;
	}
	return iResult;
}

int	CThreadRpc::History(CHistoryRpc *plHistory, int *piTimeLeft, bool bActiveOnly)
{
	int			retval, iCountHistory, iCountHistoryRemove;

	iCountHistory = 0;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		if (m_bUpdateState)
		{
			// oeps rebuilding 
			ReadState(m_pState);
			m_bUpdateState = false;
		}
//		m_pResultsHistory->clear();		// no need for this
		retval = m_pRpc->get_results(*m_pResultsHistory,bActiveOnly);
		if (!retval)
		{
			m_bHistoryReconnectFetch = false;		// a valid fetch
			if (!bActiveOnly)
			{
				_time64(&m_tLastWuRead);			// only update time on a full fetch
			}
			if (m_iHistoryCountUpdate++ >= 1)		// only update the history count once every cycle
			{
				if (!bActiveOnly)
				{
					m_iHistoryCountUpdate = 0;
					HistoryCountAdd(m_pResultsHistory, &plHistory->m_lHistoryCount);
				}
			}
			iCountHistory = HistoryAdd(m_pResultsHistory, &plHistory->m_lHistory, piTimeLeft, bActiveOnly);
		}
		else
		{
			HistoryCountDelete(&plHistory->m_lHistoryCount);
			HistoryRemoveRunning(&plHistory->m_lHistory);
			LostConnection();
		}
	}
	else
	{
		HistoryCountDelete(&plHistory->m_lHistoryCount);
		HistoryRemoveRunning(&plHistory->m_lHistory);
		LostConnection();
	}

	iCountHistoryRemove = HistoryRemove(plHistory);
	HistoryLongRemove(plHistory);
	if (iCountHistory == 0)
	{
		iCountHistory = iCountHistoryRemove;
	}
	return iCountHistory;
}

int CThreadRpc::HistoryAdd(RESULTS *pResults,std::deque<CHistoryItem*> *plHistory, int *piTimeLeft, bool bActiveOnly)
{
	int				iPos, iPosOld;
	char			*pszUserFriendly, *pszProjectName, *pszAccountName;
	int				iVersion;
	int				iCountResults, iCountHistory, iState;
	CHistoryItem	*pHistoryItem;
	CHistoryRunItem	*pHistoryRunItem;
	bool			bAdd, bFound, bUpdateStateOnce, bMissed;
	bool			bNonCpuIntensive;
	double			dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;
	double			dFractionLeft, dTimeLeftAlt; 
	int				iRunningTasks;
	CString			sFormat;

	iRunningTasks = 0;
	bUpdateStateOnce = false;
	bMissed = false;

	bool bFetchedOldResults = false;
	std::vector <OLD_RESULT> oldResults;

	time_t tSystemTime;
	_time64(&tSystemTime);

	for (iPos = 0; iPos < (int) plHistory->size(); iPos++)
	{
		plHistory->at(iPos)->m_bWuGone = true;
	}

	iCountHistory = 0;												// nr changed NOT the total
	iCountResults = (int) pResults->results.size();


	for (int iCount = 0; iCount < iCountResults; iCount++)
	{
		bFound = false;
		for (iPos = 0; iPos < (int) plHistory->size(); iPos++)
		{
			pHistoryItem = plHistory->at(iPos);
			if (strcmp(pHistoryItem->m_pcName, pResults->results[iCount]->name.c_str()) == 0)
			{
				iState = (int) pResults->results[iCount]->state << 16;			// get latest state
				iState+= (int) pResults->results[iCount]->scheduler_state << 8;
				iState+= (int) pResults->results[iCount]->edf_scheduled;

				if (iState != pHistoryItem->m_iState)
				{
					pHistoryItem->m_iState = iState;
					iCountHistory++;											// changed
				}
				int iStatus;
				iStatus = pResults->results[iCount]->exit_status << 16;
				iStatus+= pResults->results[iCount]->signal & 0xffff;

				if (pHistoryItem->m_iExitStatus != iStatus)
				{
					pHistoryItem->m_iExitStatus = iStatus;
					iCountHistory++;											// changed
				}

				if (pResults->results[iCount]->swap_size > pHistoryItem->m_dVirtualMemory)
				{
					pHistoryItem->m_dVirtualMemory = pResults->results[iCount]->swap_size;
					iCountHistory++;											// changed
				}
				if (pResults->results[iCount]->working_set_size_smoothed > pHistoryItem->m_dMemory)
				{
					pHistoryItem->m_dMemory = pResults->results[iCount]->working_set_size_smoothed;
					iCountHistory++;											// changed
				}

				pHistoryItem->m_dReceivedTime = pResults->results[iCount]->received_time;

				int iElapsedTimeCpu, iElapsedTimeGpu;
				if ((iState&0xfffffe) == STATE_RUNNING)	
				{
					iRunningTasks++;
					iElapsedTimeCpu = (int) pResults->results[iCount]->elapsed_time;
					iElapsedTimeGpu = (int) pResults->results[iCount]->current_cpu_time;
					pHistoryItem->m_dFraction_done = (double) (floor( pResults->results[iCount]->fraction_done * 100000) / 1000);
					pHistoryItem->m_iTimeLeft = (int) pResults->results[iCount]->estimated_cpu_time_remaining;

					// Some projects don't do the timeleft right, check another way.
					dFractionLeft= 1 - pResults->results[iCount]->fraction_done;
					if (dFractionLeft < 0.1)
					{
						dTimeLeftAlt = pResults->results[iCount]->elapsed_time * dFractionLeft;
						if (dTimeLeftAlt < 30)
						{
							int iTest;
							iTest = 1;
						}
						if (dTimeLeftAlt < pHistoryItem->m_iTimeLeft)
						{
							pHistoryItem->m_iTimeLeft = (int) dTimeLeftAlt;
						}
					}
					
					if (*piTimeLeft != -1)
					{
						if (pHistoryItem->m_iTimeLeft < *piTimeLeft)
						{
							*piTimeLeft = pHistoryItem->m_iTimeLeft;
						}
					}
					else
					{
						*piTimeLeft = pHistoryItem->m_iTimeLeft;
					}
					int iCheckpoint;
					iCheckpoint = (int) pResults->results[iCount]->checkpoint_cpu_time;
					if (iCheckpoint != pHistoryItem->m_iCheckpoint)
					{
						pHistoryItem->m_iCheckpoint = iCheckpoint;
						pHistoryItem->m_iCheckpoints++;
					}
					pHistoryItem->m_iDeadline = (int) pResults->results[iCount]->report_deadline;

					pHistoryRunItem = pHistoryItem->m_pHistoryRunItem;
					if (pHistoryRunItem != NULL)
					{
						pHistoryRunItem->m_iPrev4ElapsedTimeCpu = pHistoryRunItem->m_iPrev3ElapsedTimeCpu;
						pHistoryRunItem->m_iPrev4ElapsedTimeGpu = pHistoryRunItem->m_iPrev3ElapsedTimeGpu;
						pHistoryRunItem->m_iPrev3ElapsedTimeCpu = pHistoryRunItem->m_iPrev2ElapsedTimeCpu;
						pHistoryRunItem->m_iPrev3ElapsedTimeGpu = pHistoryRunItem->m_iPrev2ElapsedTimeGpu;
						pHistoryRunItem->m_iPrev2ElapsedTimeCpu = pHistoryRunItem->m_iPrev1ElapsedTimeCpu;
						pHistoryRunItem->m_iPrev2ElapsedTimeGpu = pHistoryRunItem->m_iPrev1ElapsedTimeGpu;
						pHistoryRunItem->m_iPrev1ElapsedTimeCpu = pHistoryItem->m_iElapsedTimeCpu;
						pHistoryRunItem->m_iPrev1ElapsedTimeGpu = pHistoryItem->m_iElapsedTimeGpu;

						pHistoryRunItem->m_dPrev4Fraction_done = pHistoryRunItem->m_dPrev3Fraction_done;
						pHistoryRunItem->m_dPrev3Fraction_done = pHistoryRunItem->m_dPrev2Fraction_done;
						pHistoryRunItem->m_dPrev2Fraction_done = pHistoryRunItem->m_dPrev1Fraction_done;
						pHistoryRunItem->m_dPrev1Fraction_done = pHistoryItem->m_dFraction_done;
					}
				}
				else
				{
					iElapsedTimeCpu = (int) pResults->results[iCount]->final_elapsed_time;
					iElapsedTimeGpu = (int) pResults->results[iCount]->final_cpu_time;
				}

				if (pHistoryItem->m_iElapsedTimeCpu != iElapsedTimeCpu)
				{
					pHistoryItem->m_iElapsedTimeCpu = iElapsedTimeCpu;
					iCountHistory++;											// changed
				}
				if (pHistoryItem->m_iElapsedTimeGpu  != iElapsedTimeGpu)
				{
					pHistoryItem->m_iElapsedTimeGpu = iElapsedTimeGpu;
					iCountHistory++;											// changed
				}
				
				bFound = true;
				pHistoryItem->m_bWuGone = false;
				pHistoryItem->m_i64ReportedTime = 0;
				break;	// new out of the for instead of if.
			}
		}
		if (bFound) continue;
		// only results that are uploading or ready;

		bAdd = false;

		iState = pResults->results[iCount]->state << 16;

		if ((iState&0xfffffe) > STATE_RUNNING)
		{
			bAdd = true;
		}
//		if (iState == STATE_COMPUTATON_ERROR) bAdd = true;
//		if (iState == STATE_ABORTED) bAdd = true;
//		if (iState == STATE_UPLOADING) bAdd = true;
//		if (iState == STATE_READY_TO_REPORT) bAdd = true;

		iState+= (int) pResults->results[iCount]->scheduler_state << 8;

		if ((iState&0xfffffe) == STATE_RUNNING)
		{
			bAdd = true;
			iRunningTasks++;
			m_bHistoryChangedRunning = true;
		}

		if (bAdd)
		{
			pHistoryItem = new CHistoryItem;
			pHistoryItem->m_bWuGone = false;
			pHistoryItem->AddName((char *) pResults->results[iCount]->name.c_str());
			iState = (int) pResults->results[iCount]->state << 16;			// latest state
			iState+= (int) pResults->results[iCount]->scheduler_state << 8;
			iState+= (int) pResults->results[iCount]->edf_scheduled;
			pHistoryItem->m_iState = iState;
			iVersion = 0;
			GetApplication(m_pState, pResults, iCount, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersion, &bNonCpuIntensive,  &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare);
			if (m_bUpdateState)
			{
				if (!bUpdateStateOnce)
				{
					bUpdateStateOnce = true;
					ReadState(m_pState);
					m_bUpdateState = false;
					GetApplication(m_pState, pResults, iCount, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersion, &bNonCpuIntensive,  &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare);
				}
			}

			pHistoryItem->AddProject(pszProjectName);
			pHistoryItem->AddApplication(pszUserFriendly);
			pHistoryItem->AddPlanClass((char *) pResults->results[iCount]->plan_class.c_str());
			pHistoryItem->m_iVersion = iVersion;
			pHistoryItem->m_iCheckpoint = (int) pResults->results[iCount]->checkpoint_cpu_time;
			pHistoryItem->m_iCheckpoints = 0;
			pHistoryItem->m_dReceivedTime = pResults->results[iCount]->received_time;
			pHistoryItem->m_dVirtualMemory = pResults->results[iCount]->swap_size;
			pHistoryItem->m_dMemory = pResults->results[iCount]->working_set_size_smoothed;

			if ((iState&0xfffffe) == STATE_RUNNING)
			{
				if (pHistoryItem->m_pHistoryRunItem == NULL)
				{
					pHistoryRunItem = new CHistoryRunItem;
					pHistoryItem->m_pHistoryRunItem = pHistoryRunItem;
					pHistoryItem->m_iElapsedTimeCpu = (int) pResults->results[iCount]->elapsed_time;
					pHistoryItem->m_iElapsedTimeGpu = (int) pResults->results[iCount]->current_cpu_time;
					pHistoryItem->m_dFraction_done = (double) (floor( pResults->results[iCount]->fraction_done * 100000) / 1000);
					pHistoryItem->m_iTimeLeft = (int) pResults->results[iCount]->estimated_cpu_time_remaining;

					// Some projects don't do the timeleft right, check another way.
					dFractionLeft= 1 - pResults->results[iCount]->fraction_done;
					if (dFractionLeft < 0.1)
					{
						dTimeLeftAlt = pResults->results[iCount]->elapsed_time * dFractionLeft;
//						if (dTimeLeftAlt < 30)
//						{
//							int iTest;
//							iTest = 1;
//						}
						if (dTimeLeftAlt < pHistoryItem->m_iTimeLeft)
						{
							pHistoryItem->m_iTimeLeft = (int) dTimeLeftAlt;
						}
					}

					pHistoryItem->AddUse((char *) pResults->results[iCount]->resources.c_str());

					pHistoryRunItem->m_iPrev1ElapsedTimeCpu = 0;
					pHistoryRunItem->m_iPrev1ElapsedTimeGpu = 0;
					pHistoryRunItem->m_iPrev2ElapsedTimeCpu = 0;
					pHistoryRunItem->m_iPrev2ElapsedTimeGpu = 0;
					pHistoryRunItem->m_iPrev3ElapsedTimeCpu = 0;
					pHistoryRunItem->m_iPrev3ElapsedTimeGpu = 0;
					pHistoryRunItem->m_iPrev4ElapsedTimeCpu = 0;
					pHistoryRunItem->m_iPrev4ElapsedTimeGpu = 0;

					pHistoryRunItem->m_dPrev1Fraction_done = -1;
					pHistoryRunItem->m_dPrev2Fraction_done = -1;
					pHistoryRunItem->m_dPrev3Fraction_done = -1;
					pHistoryRunItem->m_dPrev4Fraction_done = -1;

				}
			}
			else
			{
				pHistoryItem->m_iElapsedTimeCpu = (int) pResults->results[iCount]->final_elapsed_time;
				pHistoryItem->m_iElapsedTimeGpu = (int) pResults->results[iCount]->final_cpu_time;
				if (pHistoryItem->m_pcUse == NULL)
				{
					pHistoryItem->AddUse((char *) pResults->results[iCount]->resources.c_str());
				}
			}

			if (pHistoryItem->m_iElapsedTimeCpu == 0)	// no elapsed time, probably still running.
			{
				pHistoryItem->m_iElapsedTimeCpu = (int) pResults->results[iCount]->elapsed_time;
			}

			pHistoryItem->m_iDeadline = (int) pResults->results[iCount]->report_deadline;

			int iStatus;
			iStatus = pResults->results[iCount]->exit_status << 16;
			iStatus+= pResults->results[iCount]->signal & 0xffff;
			pHistoryItem->m_iExitStatus = iStatus;
			pHistoryItem->AddComputer(m_pcComputerId);

			pHistoryItem->m_i64ReportedTime = 0;
			pHistoryItem->m_i64CompletedTime= 0;
			plHistory->push_back(pHistoryItem);
			iCountHistory++;
		}
	}

	if (!bActiveOnly)			// only on a full fetch
	{
		// if gone add the system time 

		for (iPos = 0; iPos < (int) plHistory->size(); iPos++)
		{
			pHistoryItem = plHistory->at(iPos);
			if (pHistoryItem->m_i64ReportedTime == 0)
			{
				if (pHistoryItem->m_bWuGone)
				{
					if ((pHistoryItem->m_iState&0xfffffe) == STATE_RUNNING)
					{
						bMissed = true;
						pHistoryItem->m_iState = STATE_MISSED;

						if (m_bLogDebugHistoryFetching && (m_iConnectionStatus == RPC_STATUS_CONNECTED))
						{
							CTime time = CTime::GetCurrentTime();  
							CString sTime = time.Format(_T("%H:%M:%S "));
							_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"%s, %s, %s, Task: %s, F: %.2f, Left: %d" ,sTime, m_pcHostName,m_pcComputerId, pHistoryItem->m_pcName, pHistoryItem->m_dFraction_done, pHistoryItem->m_iTimeLeft);
							Logging("Missed task:", m_pcBufferLog, LOGGING_DEBUG);
						}
					}
					pHistoryItem->m_i64ReportedTime = tSystemTime;
					iCountHistory++;
				}
			}
			if (pHistoryItem->m_i64CompletedTime == 0)
			{
//				if ((pHistoryItem->m_iState&0xfffffe) != STATE_RUNNING)
				if ((pHistoryItem->m_iState&0xfffffe) >= STATE_COMPUTATON_ERROR)
				{
					pHistoryItem->m_i64CompletedTime = tSystemTime;
					iCountHistory++;
				}
			}
		}
	}

	// check for missed

	if (m_bBoincOldResults)
	{
		if (tSystemTime > m_tHistoryNextCheckMissed)
		{
			bMissed = true;
		}
		
		if (bMissed)
		{
			m_tHistoryNextCheckMissed = tSystemTime + 60*30;	// check at least every 30 minutes
			for (iPos = 0; iPos < (int) plHistory->size(); iPos++)
			{
				pHistoryItem = plHistory->at(iPos);

				if (pHistoryItem->m_iState == STATE_MISSED || pHistoryItem->m_iState == STATE_MISSED2)
				{
					bool bFoundMissed = false;
					if (!bFetchedOldResults)
					{
						bFetchedOldResults = true;
						m_pRpc->get_old_results(oldResults);
					}
					for (iPosOld = 0; iPosOld < (int) oldResults.size(); iPosOld++)
					{
						if (strcmp(pHistoryItem->m_pcName, oldResults.at(iPosOld).result_name) == 0)
						{
							iCountHistory++;								// changed
							bFoundMissed = true;
							pHistoryItem->m_iState = STATE_MISSED_RECOVERED;

							pHistoryItem->m_i64CompletedTime = (__int64) oldResults.at(iPosOld).completed_time;
							pHistoryItem->m_iElapsedTimeCpu = (int) oldResults.at(iPosOld).elapsed_time;
							pHistoryItem->m_iElapsedTimeGpu = (int) oldResults.at(iPosOld).cpu_time;
							pHistoryItem->m_iExitStatus = oldResults.at(iPosOld).exit_status << 16;
						}
					}
					if (!bFoundMissed)
					{
						iCountHistory++;								// changed
						pHistoryItem->m_iState = STATE_MISSED_OLD;

					}
				}
			}
		}
	}

	if (m_iHistoryRunningTasks != iRunningTasks)
	{
		m_iHistoryRunningTasks = iRunningTasks;
		m_bHistoryChangedRunning = true;
	}

	return iCountHistory;
}

int CThreadRpc::HistoryRemove(CHistoryRpc *plHistory)
{
	int				iPos;
	CHistoryItem	*pHistoryItem;
	int				iCountHistory;
	int				iHistoryTimeDelete;

	iCountHistory = 0;

	bool bToRemove, bMoveToLongTerm;
	bool bRemoved = true;

	time_t tSystemTime;
	_time64(&tSystemTime);

	if (tSystemTime < m_tHistoryNextTimeRemove) 
	{
		return 0;	// only once every 1 minutes
	}

	if (plHistory->m_bMoveToLongHistory)
	{
		iHistoryTimeDelete = plHistory->m_iMovetoLongHistoryTime * 60;	// minutes -> seconds = move to long term instead of deleting
	}
	else
	{
		iHistoryTimeDelete = plHistory->m_iDeleteTime * 60;				// minutes -> seconds
	}

	m_tHistoryNextTimeRemove = tSystemTime + (1*60);
	tSystemTime -= iHistoryTimeDelete;
	for (iPos = 0; iPos < (int) plHistory->m_lHistory.size(); iPos++)
	{
		pHistoryItem = plHistory->m_lHistory.at(iPos);
		bToRemove = false;
		if ((pHistoryItem->m_iState&0xffffe) == STATE_READY_TO_START)
		{
			bToRemove = true;						// doesn't belong here
		}
		else
		{
			if ((pHistoryItem->m_iState&0xffffe) == STATE_WAITING_TO_RUN)
			{
				bToRemove = true;						// doesn't belong here
			}
			else
			{
				if ((pHistoryItem->m_iState&0xffffe) == STATE_NEW)
				{
					bToRemove = true;						// doesn't belong here
				}
			}
		}
		bMoveToLongTerm = false;
		if (pHistoryItem->m_i64ReportedTime > 0)
		{
			__int64 i64TimeDiff;
			i64TimeDiff = tSystemTime - pHistoryItem->m_i64ReportedTime;
			if (i64TimeDiff > 0)
			{
				if (plHistory->m_bMoveToLongHistory)
				{
					if (pHistoryItem->m_iState >= STATE_UPLOADING) bMoveToLongTerm = true;
				}
				bToRemove = true;
			}
		}
		if (bMoveToLongTerm)
		{
			MoveToLongTermHistory(plHistory, pHistoryItem);
			plHistory->m_lHistory.erase(plHistory->m_lHistory.begin()+iPos);	// don't delete it as it's move to the long history
			iCountHistory++;
		}
		else
		{
			if (bToRemove)
			{
				plHistory->m_lHistory.erase(plHistory->m_lHistory.begin()+iPos);
				delete pHistoryItem;	// only delete it if it's not moved to the long history.
				iCountHistory++;
			}
		}
	}
	return iCountHistory;
}

int CThreadRpc::HistoryLongRemove(CHistoryRpc *plHistory)
{
	UINT			iPos;
	CHistoryItem	*pHistoryItem;
	int				iCountHistory;
	int				iHistoryTimeDelete;

	iCountHistory = 0;

	bool bToRemove;

	time_t tSystemTime;
	_time64(&tSystemTime);

	if (tSystemTime < m_tHistoryLongNextTimeRemove) 
	{
		return 0;	// only once every 30 minutes
	}

	iHistoryTimeDelete = plHistory->m_iDeleteTime * 60;				// minutes -> seconds

	m_tHistoryLongNextTimeRemove = tSystemTime + (30*60);
	tSystemTime -= iHistoryTimeDelete;
	for (iPos = 0; iPos < plHistory->m_lHistoryLong.size(); iPos++)
	{
		pHistoryItem = plHistory->m_lHistoryLong.at(iPos);
		bToRemove = false;
		if ((pHistoryItem->m_iState&0xffffe) == STATE_READY_TO_START)
		{
			bToRemove = true;						// doesn't belong here
		}
		else
		{
			if ((pHistoryItem->m_iState&0xffffe) == STATE_WAITING_TO_RUN)
			{
				bToRemove = true;						// doesn't belong here
			}
			else
			{
				if ((pHistoryItem->m_iState&0xffffe) == STATE_NEW)
				{
					bToRemove = true;						// doesn't belong here
				}
			}
		}
		if (pHistoryItem->m_i64ReportedTime > 0)
		{
			__int64 i64TimeDiff;
			i64TimeDiff = tSystemTime - pHistoryItem->m_i64ReportedTime;
			if (i64TimeDiff > 0)
			{
				bToRemove = true;
			}
		}
		if (bToRemove)
		{
			plHistory->m_lHistoryLong.erase(plHistory->m_lHistoryLong.begin()+iPos);
			delete pHistoryItem;
			iCountHistory++;
		}
	}
	return iCountHistory;
}

void CThreadRpc::MoveToLongTermHistory(CHistoryRpc *plHistory,  CHistoryItem *pHistoryItem)
{
	int iCount = 0;

	plHistory->m_bHistoryLongNeedsWrite = true;
	plHistory->m_lHistoryLong.push_back(pHistoryItem);
}

void CThreadRpc::HistoryRemoveRunning(std::deque<CHistoryItem*> *plHistory)
{
	int				iPos;
	CHistoryItem	*pHistoryItem;

	for (iPos = 0; iPos < (int) plHistory->size(); iPos++)
	{
		pHistoryItem = plHistory->at(iPos);
		if ((pHistoryItem->m_iState&0xffffe) == STATE_RUNNING)
		{
			delete pHistoryItem;
			plHistory->erase(plHistory->begin()+iPos);
			iPos--;
		}
	}
}


/* xxx */
void CThreadRpc::HistoryCountAdd(RESULTS *pResults,std::deque<CHistoryCountItem*> *plHistoryCount) 
{
	int					iPos;
	char				*pszUserFriendly, *pszProjectName, *pszAccountName;
	int					iVersion;
	int					iCountResults;
//	XOSITION			pos;
	CHistoryCountItem	*pHistoryCountItem;
	bool				bAdd, bNonCpuIntensive;
	CUseCpuGpu			cpugpu;
	double				dCpu, dGpu;
	int					iGpu, iGpuType;
	bool				bCuda;
	double				dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;
	int					iStateReadyCount;
	time_t				tOneDay, tOneWeek;

	_time64(&tOneDay);
	tOneWeek = tOneDay - 604800;
	tOneDay -= 86400;

	iStateReadyCount = 0;
	HistoryCountDelete(plHistoryCount);

	iCountResults = (int) pResults->results.size();
	int iCoresCpu = 0;
	int iCoresGpu = 0;

	for (int iCount = 0; iCount < iCountResults; iCount++)
	{
		iVersion = 0;
		GetApplication(m_pState, pResults, iCount, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersion, &bNonCpuIntensive, &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare);
		if (iVersion == 0) continue;

		if (pResults->results[iCount]->state == 2) 
		{
			if (pResults->results[iCount]->scheduler_state == 2)	// STATE_RUNNING
			{
				if (!bNonCpuIntensive)	// this is not a core.
				{
					cpugpu.GetCpuGpu((char *) pResults->results[iCount]->resources.c_str(), &dCpu, &dGpu, &iGpu, &bCuda, &iGpuType);
					if (bCuda)
					{
						iCoresGpu += (int) ceil(dGpu);
					}
					else
					{
						iCoresCpu += (int) floor(dCpu);
					}
				}
			}
		}

		bAdd = true;
		for (iPos = 0; iPos < (int) plHistoryCount->size(); iPos++)
		{
			pHistoryCountItem = plHistoryCount->at(iPos);
			if (iVersion != 0)
			{
				if (strcmp(pHistoryCountItem->m_pcProject, pszProjectName) == 0)
				{
					if (!bNonCpuIntensive)	// this is not a core.
					{
						if (pResults->results[iCount]->estimated_cpu_time_remaining > 0)
						{
							// only tasks that haven't finished
							cpugpu.GetCpuGpu((char *) pResults->results[iCount]->resources.c_str(), &dCpu, &dGpu, &iGpu, &bCuda, &iGpuType);
							if (bCuda)
							{
								pHistoryCountItem->m_iTaskCountGpu++;
								pHistoryCountItem->m_iTimeLeftGpu += (__int64) pResults->results[iCount]->estimated_cpu_time_remaining;
							}
							else
							{
								pHistoryCountItem->m_iTaskCountCpu++;
								pHistoryCountItem->m_iTimeLeftCpu += (__int64) pResults->results[iCount]->estimated_cpu_time_remaining;
							}
						}
					}
					else
					{
						if (pResults->results[iCount]->estimated_cpu_time_remaining > 0)
						{
							pHistoryCountItem->m_iTaskCountLow++;
						}
					}
					bAdd = false;
				}
			}
		}
		if (bAdd)
		{
			pHistoryCountItem = new CHistoryCountItem;

			if (!bNonCpuIntensive)	// this is not a core.
			{
				pHistoryCountItem->m_iTaskCountLow = 0;
				cpugpu.GetCpuGpu((char *) pResults->results[iCount]->resources.c_str(), &dCpu, &dGpu, &iGpu, &bCuda, &iGpuType);
				if (bCuda)
				{
					if (pResults->results[iCount]->estimated_cpu_time_remaining > 0) pHistoryCountItem->m_iTaskCountGpu = 1;
					else pHistoryCountItem->m_iTaskCountGpu = 0;
					pHistoryCountItem->m_iTaskCountCpu = 0;
					pHistoryCountItem->m_iTimeLeftGpu = (__int64) pResults->results[iCount]->estimated_cpu_time_remaining;
					pHistoryCountItem->m_iTimeLeftCpu = 0;
				}
				else
				{
					pHistoryCountItem->m_iTaskCountGpu = 0;
					if (pResults->results[iCount]->estimated_cpu_time_remaining > 0) pHistoryCountItem->m_iTaskCountCpu = 1;
					else pHistoryCountItem->m_iTaskCountCpu = 0;
					pHistoryCountItem->m_iTimeLeftGpu = 0;
					pHistoryCountItem->m_iTimeLeftCpu = (__int64) pResults->results[iCount]->estimated_cpu_time_remaining;
				}
			}
			else
			{
				pHistoryCountItem->m_iTaskCountCpu = 0;	// non CPU intensive
				pHistoryCountItem->m_iTaskCountGpu = 0;
				pHistoryCountItem->m_iTimeLeftCpu = 0;
				pHistoryCountItem->m_iTimeLeftGpu = 0;
				if (pResults->results[iCount]->estimated_cpu_time_remaining > 0) pHistoryCountItem->m_iTaskCountLow = 1;
				else pHistoryCountItem->m_iTaskCountLow = 0;
			}

			pHistoryCountItem->AddProject(pszProjectName);
			pHistoryCountItem->AddComputer(m_pcComputerId);
			plHistoryCount->push_back(pHistoryCountItem);
		}
		if (pResults->results[iCount]->state >= 3)		// error and up
		{
			if (pResults->results[iCount]->state != 4)	// and not uploading
			{
				iStateReadyCount++;
			}
		}
	}

	// Add projects with 0 count as well
	PROJECTS projects;
	int		iNrProjects, retval;
	bool	bFound;
	retval = m_pRpc->get_project_status(projects);
	if (!retval)
	{
		iNrProjects = (int) projects.projects.size();
		for (int iCount = 0; iCount < iNrProjects; iCount++)
		{
			bFound = false;
			for (iPos = 0; iPos < (int) plHistoryCount->size(); iPos++)
			{
				pHistoryCountItem = plHistoryCount->at(iPos);
				if (strcmp(pHistoryCountItem->m_pcProject, (char *) projects.projects[iCount]->project_name.c_str()) == 0)
				{
					bFound = true;
					break;
				}
			}
			if (!bFound)
			{
				pHistoryCountItem = new CHistoryCountItem;
				pHistoryCountItem->m_iTaskCountCpu = 0;
				pHistoryCountItem->m_iTaskCountGpu = 0;
				pHistoryCountItem->m_iTimeLeftCpu = 0;
				pHistoryCountItem->m_iTimeLeftGpu = 0;
				pHistoryCountItem->m_iTimeLeftCpu = 0;
				pHistoryCountItem->m_iTimeLeftGpu = 0;
				pHistoryCountItem->m_iTaskCountLow = 0;
				pHistoryCountItem->AddComputer(m_pcComputerId);
				pHistoryCountItem->AddProject((char *) projects.projects[iCount]->project_name.c_str());
				plHistoryCount->push_back(pHistoryCountItem);
			}
		}
	}

	double dTimeUp, dTimeUpCpu, dTimeUpGpu;
	for (iPos = 0; iPos < (int) plHistoryCount->size(); iPos++)
	{
		pHistoryCountItem = plHistoryCount->at(iPos);
		pHistoryCountItem->m_iCoresCpu = iCoresCpu;
		pHistoryCountItem->m_iCoresGpu = iCoresGpu;

		if (m_bAdjustTime)
		{
			dTimeUp = m_pState->time_stats.on_frac;
			if (dTimeUp > 0)
			{
				dTimeUpCpu = m_pState->time_stats.active_frac;
				if (dTimeUpCpu > 0)
				{
					pHistoryCountItem->m_iTimeLeftCpu = (int) (pHistoryCountItem->m_iTimeLeftCpu / (dTimeUpCpu * dTimeUp));
				}
				dTimeUpGpu = m_pState->time_stats.gpu_active_frac;
				if (dTimeUpGpu > 0)
				{
					pHistoryCountItem->m_iTimeLeftGpu = (int) (pHistoryCountItem->m_iTimeLeftGpu / (dTimeUpGpu * dTimeUp));
				}
			}
		}
	}
	m_iReadyCount = iStateReadyCount;
}

void CThreadRpc::HistoryCountDelete(std::deque<CHistoryCountItem*> *plHistoryCount)
{
	int		iPos;

	if (plHistoryCount != NULL)
	{
		for (iPos = 0; iPos < (int) plHistoryCount->size(); iPos++)
		{
			delete plHistoryCount->at(iPos);
		}
		plHistoryCount->clear();
	}
}

// Tasks graphic

int CThreadRpc::HistoryStatistics(std::deque<CHistoryProjectTasksItem*> *plHistoryProjectTasks)
 {
	int		iPos, iPosHistory, iPosStatistics;
	RESULTS results;
	int		iState, retval;
	std::deque<CHistoryItem*> historyStatistics;
	CHistoryItem *pHistoryItem, *pHistoryStatisticsItem;
	CHistoryProjectTasksItem	*pHistoryProjectTasksItem;
	char *pszUserFriendly, *pszProjectName, *pszAccountName;
	int	iVersion;
	bool	bNonCpuIntensive;
	double	dCpuDept, dCudaDebt, dAtiDebt, dProjectShare;
	time_t	timeget, timeBaseline;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		retval = m_pRpc->get_results(results,false);
	}
	else retval = -1;

	_time64(&timeget);
	if (m_iGraphicTasksPeriod > 0)	timeget -= m_iGraphicTasksPeriod*HOUR_SECONDS;
	else timeget -= MAX_HISTORY_STATISTICS_LEN*HOUR_SECONDS;

	if (!retval)
	{
		// add all the results
		 for (int iCount = 0; iCount < (int) results.results.size(); iCount++)
		 {
			if (results.results[iCount]->received_time > timeget)
			{
				pHistoryStatisticsItem = new CHistoryItem;
				pHistoryStatisticsItem->AddName((char *) results.results[iCount]->name.c_str());

				iVersion = results.results[iCount]->version_num;
				GetApplication(m_pState, &results, iCount, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersion, &bNonCpuIntensive,  &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare);

				pHistoryStatisticsItem->AddProject(pszProjectName);
				pHistoryStatisticsItem->m_iVersion =  iVersion;

				iState = (int) results.results[iCount]->state << 16;
				iState+= (int) results.results[iCount]->scheduler_state << 8;
				iState+= (int) results.results[iCount]->edf_scheduled;
				pHistoryStatisticsItem->AddUse((char*) results.results[iCount]->resources.c_str());
				pHistoryStatisticsItem->m_iState = iState;
				pHistoryStatisticsItem->m_dReceivedTime = results.results[iCount]->received_time;
				historyStatistics.push_back(pHistoryStatisticsItem);
			}
		}
	}

	// now add the item from the history
	if (m_plHistoryStatisticsTasks)
	{
		for (iPosHistory = 0; iPosHistory < (int) m_plHistoryStatisticsTasks->size(); iPosHistory++)
		{
			pHistoryItem = m_plHistoryStatisticsTasks->at(iPosHistory);

			bool bFoundInList = false;
			if (pHistoryItem->m_dReceivedTime > timeget)
			{
				for (iPosStatistics = 0; iPosStatistics < (int) historyStatistics.size(); iPosStatistics++)
				{
					pHistoryStatisticsItem = historyStatistics.at(iPosStatistics);
					{
						if (strcmp(pHistoryItem->m_pcName, pHistoryStatisticsItem->m_pcName) == 0)
						{
							if (strcmp(pHistoryItem->m_pcProject, pHistoryStatisticsItem->m_pcProject) == 0)
							{
								bFoundInList = true;
							}
						}
					}
				}
				if (!bFoundInList)
				{
					// not found, so add it
					pHistoryStatisticsItem = new CHistoryItem;
					pHistoryStatisticsItem->AddName(pHistoryItem->m_pcName);
					pHistoryStatisticsItem->AddProject(pHistoryItem->m_pcProject);
					pHistoryStatisticsItem->m_iVersion = pHistoryItem->m_iVersion;
					pHistoryStatisticsItem->AddUse((char*) pHistoryItem->m_pcUse);
					pHistoryStatisticsItem->m_iState = pHistoryItem->m_iState;
					pHistoryStatisticsItem->m_dReceivedTime = pHistoryItem->m_dReceivedTime;
					historyStatistics.push_back(pHistoryStatisticsItem);
				}
			}
		}
	}

	// now add the item from the history long
	if (m_plHistoryLongStatisticsTasks)
	{
		for (iPosHistory = 0; iPosHistory < (int) m_plHistoryLongStatisticsTasks->size(); iPosHistory++)
		{
			pHistoryItem = m_plHistoryLongStatisticsTasks->at(iPosHistory);
			bool bFoundInList = false;
			if (pHistoryItem->m_dReceivedTime > timeget)
			{
			// history long tasks are never in the list
			// not found, so add it
				pHistoryStatisticsItem = new CHistoryItem;
				pHistoryStatisticsItem->AddName(pHistoryItem->m_pcName);
				pHistoryStatisticsItem->AddProject(pHistoryItem->m_pcProject);
				pHistoryStatisticsItem->m_iVersion = pHistoryItem->m_iVersion;
				pHistoryStatisticsItem->AddUse((char*) pHistoryItem->m_pcUse);
				pHistoryStatisticsItem->m_iState = pHistoryItem->m_iState;
				pHistoryStatisticsItem->m_dReceivedTime = pHistoryItem->m_dReceivedTime;
				historyStatistics.push_back(pHistoryStatisticsItem);
			}
		}
	}

	// now we have all items from results and history.

	CUseCpuGpu CpuGpu;
	int iDetectedGpu, iGpu;
	bool	bCuda;
	double fCpuUse, fGpuUse;
	double dTimeBaseline;
	struct tm   tmtime;

	// round time to the nearest hour
	_time64(&timeBaseline);
	localtime_s(&tmtime, &timeBaseline);
	int iMinutes = tmtime.tm_min;
	timeBaseline -= iMinutes * 60;	// round to the hour
	timeBaseline -= MAX_HISTORY_STATISTICS_LEN * HOUR_SECONDS;	// hour to seconds
	dTimeBaseline = (double) timeBaseline;
	int	iTimeIndex;

	// first remove all the old items

	for (iPos = 0; iPos < (int) plHistoryProjectTasks->size(); iPos++)
	{
		delete plHistoryProjectTasks->at(iPos);
	}
	plHistoryProjectTasks->clear();

// define for testing ONLY
//#define INJECT_TIME 1

#ifdef INJECT_TIME
	CTime testInject1(2013, 4, 29, 0, 1, 0, -1); // year,month, day ,hour, min, sec, -1
	CTime testInject2(2013, 4, 25, 0, 1, 0, -1); // year,month, day ,hour, min, sec, -1
	pHistoryProjectTasksItem = new CHistoryProjectTasksItem;
	pHistoryProjectTasksItem->AddProject("Test_Project");
	pHistoryProjectTasksItem->m_tTimeBaseline = timeBaseline;
	plHistoryProjectTasks->push_back(pHistoryProjectTasksItem);
#endif

	for (iPosStatistics = 0; iPosStatistics < (int) historyStatistics.size(); iPosStatistics++)
	{
		bool bFoundProject = false;

		// inject 2 known dates for calibration
#ifdef INJECT_TIME
		iTimeIndex = (int) (testInject1.GetTime() - dTimeBaseline)/HOUR_SECONDS;
		if (iTimeIndex >=0)
		{
			if (iTimeIndex <= MAX_HISTORY_STATISTICS_LEN)
			{
				pHistoryProjectTasksItem = plHistoryProjectTasks->at(0);	// first project
				pHistoryProjectTasksItem->m_iReceivedGpu[iTimeIndex] = 9600;
			}
		}
		iTimeIndex = (int) (testInject2.GetTime() - dTimeBaseline)/HOUR_SECONDS;
		if (iTimeIndex >=0)
		{
			if (iTimeIndex <= MAX_HISTORY_STATISTICS_LEN)
			{
				pHistoryProjectTasksItem = plHistoryProjectTasks->at(0);	// first project
				pHistoryProjectTasksItem->m_iReceivedGpu[iTimeIndex] = 9600;
			}
		}
#endif
		pHistoryStatisticsItem = historyStatistics.at(iPosStatistics);
		iTimeIndex = (int) (pHistoryStatisticsItem->m_dReceivedTime - dTimeBaseline)/HOUR_SECONDS;
		if (iTimeIndex >=0)
		{
			if (iTimeIndex <= MAX_HISTORY_STATISTICS_LEN)
			{
				for (iPosHistory = 0; iPosHistory < (int) plHistoryProjectTasks->size(); iPosHistory++)
				{
					pHistoryProjectTasksItem = plHistoryProjectTasks->at(iPosHistory);
					if (strcmp (pHistoryStatisticsItem->m_pcProject, pHistoryProjectTasksItem->m_pcProject) == 0)
					{
						CpuGpu.GetCpuGpu(pHistoryStatisticsItem->m_pcUse, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
						if (bCuda)	pHistoryProjectTasksItem->m_iReceivedGpu[iTimeIndex] += 1;		// upload in timewindow found (any state will do)
						else		pHistoryProjectTasksItem->m_iReceivedCpu[iTimeIndex] += 1;		// upload in timewindow found (any state will do)
						bFoundProject = true;
						break;
					}
				}
				if (!bFoundProject)
				{
					pHistoryProjectTasksItem = new CHistoryProjectTasksItem;
					pHistoryProjectTasksItem->AddProject(pHistoryStatisticsItem->m_pcProject);
					CpuGpu.GetCpuGpu(pHistoryStatisticsItem->m_pcUse, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
					if (bCuda)	pHistoryProjectTasksItem->m_iReceivedGpu[iTimeIndex] += 1;		// upload in timewindow found (any state will do)
					else		pHistoryProjectTasksItem->m_iReceivedCpu[iTimeIndex] += 1;		// upload in timewindow found (any state will do)
					pHistoryProjectTasksItem->m_tTimeBaseline = timeBaseline;
					plHistoryProjectTasks->push_back(pHistoryProjectTasksItem);
				}
			}
		}
	}
	for (iPosStatistics = 0; iPosStatistics < (int) historyStatistics.size(); iPosStatistics++)
	{
		delete historyStatistics.at(iPosStatistics);
	}
	historyStatistics.clear();				// everyting is delected so remove the list

	return retval;
 }

 // 0 = now -> max

 int CThreadRpc::DeadlineStatistics(CComputerGraphicDeadlineListItem *pComputerGraphicDeadlineListItem)
 {
	RESULTS results;
	int			retval, iCpuCore, iGpuCore;
	CHistoryProjectDeadlineItem *pHistoryProjectDeadlineItem;
	char		*pszUserFriendly, *pszProjectName, *pszAccountName;
	int			iVersion, iPosItem;
	double		dTimeDeadline;
	bool		bNonCpuIntensive;
	double		dCpuDept, dCudaDebt, dAtiDebt, dRemainingTime, dProjectShare;
	time_t		timeBaseline;
	CUseCpuGpu	CpuGpu;
	int			iDetectedGpu, iGpu;
	bool		bCuda;
	double		fCpuUse, fGpuUse;
	struct		tm   tmtime;
	CString		sTime;

	// round time to the nearest hour
	_time64(&timeBaseline);
	localtime_s(&tmtime, &timeBaseline);
	int iMinutes = tmtime.tm_min;
	timeBaseline -= iMinutes * 60;	// round to the hour

	int	iTimeIndex;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		retval = m_pRpc->get_results(results,false);
	}
	else retval = -1;

	std::deque<CHistoryProjectDeadlineItem*> *plHistoryProjectDeadline;
	plHistoryProjectDeadline = pComputerGraphicDeadlineListItem->m_plHistoryProjectDeadline;

	// first remove all the old items

	for (iPosItem = 0; iPosItem < (int) plHistoryProjectDeadline->size(); iPosItem++)
	{
		delete plHistoryProjectDeadline->at(iPosItem);
	}
	plHistoryProjectDeadline->clear();

	int iTimeIndexMax;
	if (m_iGraphicDeadlinePeriod > 0) iTimeIndexMax = m_iGraphicDeadlinePeriod;
	else iTimeIndexMax = MAX_DEADLINE_LEN;

	iCpuCore = 0;
	iGpuCore = 0;

	double dTimeUp, dTimeUpCpu, dTimeUpGpu;
	dTimeUpCpu = 1;
	dTimeUpGpu = 1;
	if (m_bAdjustTime)
	{
		dTimeUp = m_pState->time_stats.on_frac;
		if (dTimeUp > 0)
		{
			dTimeUpCpu = m_pState->time_stats.active_frac;
			dTimeUpGpu = m_pState->time_stats.gpu_active_frac;
		}
	}
	if (dTimeUpCpu <= 0) dTimeUpCpu = 1;
	if (dTimeUpGpu <= 0) dTimeUpGpu = 1;

	if (!retval)
	{
// define for testing ONLY
//#define INJECT_TIME 1

#ifdef INJECT_TIME
		CTime testInject1(2013, 5, 4, 1, 0, 0, -1); // year,month, day ,hour, min, sec, -1
		CTime testInject2(2013, 5, 9, 1, 0, 0, -1);
		CTime testInject3(2013, 5, 10, 1, 0, 0, -1);
		pHistoryProjectDeadlineItem = new CHistoryProjectDeadlineItem;
		pHistoryProjectDeadlineItem->AddProject("Test_Project");
		pHistoryProjectDeadlineItem->m_tTimeBaseline =  timeBaseline;
		plHistoryProjectDeadline->push_back(pHistoryProjectDeadlineItem);
#endif	
		// add all the results
		for (int iCount = 0; iCount < (int) results.results.size(); iCount++)
		{
			GetApplication(m_pState, &results, iCount, &pszUserFriendly, &pszProjectName, &pszAccountName, &iVersion, &bNonCpuIntensive,  &dCpuDept, &dCudaDebt, &dAtiDebt, &dProjectShare);
			CpuGpu.GetCpuGpu((char *) results.results[iCount]->resources.c_str(), &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
			if (results.results[iCount]->state == 2) 
			{
				if (results.results[iCount]->scheduler_state == 2)	// STATE_RUNNING
				{
					if (!bNonCpuIntensive)	// this is not a core.
					{

						if (bCuda)
						{
							iGpuCore += (int) ceil(fGpuUse);
						}
						else
						{
							iCpuCore += (int) floor(fCpuUse);
						}
					}
				}
			}

#ifdef INJECT_TIME
			iTimeIndex = (int) (testInject1.GetTime() - timeBaseline)/HOUR_SECONDS;
			if (iTimeIndex >=0)
			{
				if (iTimeIndex <= iTimeIndexMax)
				{
					pHistoryProjectDeadlineItem = plHistoryProjectDeadline->at(0);	// first project
					pHistoryProjectDeadlineItem->m_dReceivedCpu[iTimeIndex] = 9600;
				}
			}
			iTimeIndex = (int) (testInject2.GetTime() - timeBaseline)/HOUR_SECONDS;
			if (iTimeIndex >=0)
			{
				if (iTimeIndex <= iTimeIndexMax)
				{
					pHistoryProjectDeadlineItem = plHistoryProjectDeadline->at(0);	// first project
					pHistoryProjectDeadlineItem->m_dReceivedCpu[iTimeIndex] = 9600;
				}
			}
			iTimeIndex = (int) (testInject3.GetTime() - timeBaseline)/HOUR_SECONDS;
			if (iTimeIndex >=0)
			{
				if (iTimeIndex <= iTimeIndexMax)
				{
					pHistoryProjectDeadlineItem = plHistoryProjectDeadline->at(0);	// first project
					pHistoryProjectDeadlineItem->m_dReceivedCpu[iTimeIndex] = 9600;
				}
			}
#endif

			dRemainingTime = results.results[iCount]->estimated_cpu_time_remaining;
			dTimeDeadline = results.results[iCount]->report_deadline - dRemainingTime;

			iTimeIndex = (int) (dTimeDeadline - timeBaseline)/HOUR_SECONDS;
			if (iTimeIndex >=0)
			{
				if (iTimeIndex <= iTimeIndexMax)
				{
					iVersion = results.results[iCount]->version_num;
					bool bFoundProject = false;
					for (iPosItem = 0; iPosItem < (int) plHistoryProjectDeadline->size(); iPosItem++)
					{
						pHistoryProjectDeadlineItem = plHistoryProjectDeadline->at(iPosItem);
						if (strcmp (pHistoryProjectDeadlineItem->m_pcProject, pszProjectName) == 0)
						{
							if (bCuda)
							{
								pHistoryProjectDeadlineItem->m_iGpuItems+= 1;
								pHistoryProjectDeadlineItem->m_dReceivedGpu[iTimeIndex] += dRemainingTime;
							}
							else
							{
								pHistoryProjectDeadlineItem->m_iCpuItems+= 1;
								pHistoryProjectDeadlineItem->m_dReceivedCpu[iTimeIndex] += dRemainingTime;
							}

							bFoundProject = true;
							break;
						}
					}
					if (!bFoundProject)
					{
						pHistoryProjectDeadlineItem = new CHistoryProjectDeadlineItem;
						iVersion = results.results[iCount]->version_num;
						pHistoryProjectDeadlineItem->AddProject(pszProjectName);
						pHistoryProjectDeadlineItem->m_tTimeBaseline =  timeBaseline;
						if (bCuda)
						{
							pHistoryProjectDeadlineItem->m_iGpuItems = 1;
							pHistoryProjectDeadlineItem->m_dReceivedGpu[iTimeIndex] += dRemainingTime;
						}
						else
						{
							pHistoryProjectDeadlineItem->m_iCpuItems = 1;
							pHistoryProjectDeadlineItem->m_dReceivedCpu[iTimeIndex] += dRemainingTime;
						}
						plHistoryProjectDeadline->push_back(pHistoryProjectDeadlineItem);
					}
				}
			}
		}
	}

	if (iCpuCore == 0) iCpuCore = 1;
	if (iGpuCore == 0) iGpuCore = 1;

	int iCpuCoreUsed, iGpuCoreUsed;

	for (iPosItem = 0; iPosItem < (int) plHistoryProjectDeadline->size(); iPosItem++)
	{
		pHistoryProjectDeadlineItem = plHistoryProjectDeadline->at(iPosItem);

		iCpuCoreUsed = iCpuCore;
		iGpuCoreUsed = iGpuCore;
		if (pHistoryProjectDeadlineItem->m_iCpuItems > iCpuCore) iCpuCoreUsed = iCpuCore;
		else iCpuCoreUsed = pHistoryProjectDeadlineItem->m_iCpuItems;
		if (pHistoryProjectDeadlineItem->m_iGpuItems > iGpuCore) iGpuCoreUsed = iGpuCore;
		else iGpuCoreUsed = pHistoryProjectDeadlineItem->m_iGpuItems;	
		if (iCpuCoreUsed == 0) iCpuCoreUsed = 1;
		if (iGpuCoreUsed == 0) iGpuCoreUsed = 1;

		for (int iCount = 0; iCount <= MAX_HISTORY_STATISTICS_LEN+1; iCount++)
		{
			pHistoryProjectDeadlineItem->m_dReceivedCpu[iCount] = (pHistoryProjectDeadlineItem->m_dReceivedCpu[iCount] / dTimeUpCpu) / iCpuCoreUsed;
			pHistoryProjectDeadlineItem->m_dReceivedGpu[iCount] = (pHistoryProjectDeadlineItem->m_dReceivedGpu[iCount] / dTimeUpGpu) / iGpuCoreUsed;
		}

		pHistoryProjectDeadlineItem->m_iCpu = iCpuCoreUsed;
		pHistoryProjectDeadlineItem->m_iGpu = iGpuCoreUsed;
	}
	return 0;
 }

 int CThreadRpc::SetRunModeCpu(CC_STATUS *pCcStatus)
 {
	int retval, iRunMode ;
	double dTimeMode;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return FALSE;

	iRunMode = pCcStatus->task_mode;
	dTimeMode = pCcStatus->task_mode_delay;

	retval = m_pRpc->set_run_mode(iRunMode,dTimeMode);
	if (retval)
	{
		pCcStatus->task_mode = -1;
		return FALSE;
	}
	retval = m_pRpc->get_cc_status(*pCcStatus);
	if (retval)
	{
		pCcStatus->task_mode = -1;
		return FALSE;
	}

	return TRUE;
 }

int CThreadRpc::SetRunModeGpu(CC_STATUS *pCcStatus)
 {
	int retval, iRunMode;
	double dTimeMode;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return FALSE;
	
	iRunMode = pCcStatus->gpu_mode;
	dTimeMode = pCcStatus->gpu_mode_delay;

	retval = m_pRpc->set_gpu_mode(iRunMode,dTimeMode);
	if (retval)
	{
		pCcStatus->gpu_mode = -1;
		return FALSE;
	}
	retval = m_pRpc->get_cc_status(*pCcStatus);
	if (retval)
	{
		pCcStatus->gpu_mode = -1;
		return FALSE;
	}

	return TRUE;
 }

 int CThreadRpc::SetNetworkMode(int iNetworkMode, int iTimeMode)
 {
	CC_STATUS ccStatus;

	int retval;
	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return -1;

	retval = m_pRpc->set_network_mode(iNetworkMode, iTimeMode);
	if (retval) return -1;

	retval = m_pRpc->get_cc_status(ccStatus);
	if (retval) return -1;

	return ccStatus.network_mode;
 }

int CThreadRpc::ReadPreference(GLOBAL_PREFS *pPrefs, GLOBAL_PREFS_MASK *pPrefsMask)
{
	int iRet;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return -1;

	// Get current working preferences (including any overrides) from client
	iRet = m_pRpc->get_global_prefs_working_struct(*pPrefs, *pPrefsMask);
	if (iRet == ERR_NOT_FOUND) 
	{
		// Older clients don't support get_global_prefs_working_struct RPC
		m_pRpc->get_global_prefs_override_struct(*pPrefs, *pPrefsMask);
	}
	m_dDiskInterval = pPrefs->disk_interval;

	return iRet;
}


int CThreadRpc::WritePreference(GLOBAL_PREFS *pPrefs, GLOBAL_PREFS_MASK *pPrefsMask)
{
	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		m_dDiskInterval = pPrefs->disk_interval;

		/*
		if (m_iRegulatorMaxTasks > 0)
		{
			if (m_bBoincClientV6)
			{
				pPrefs->work_buf_additional_days = m_dCurrentWorkForDays;
			}
			else
			{
				pPrefs->work_buf_min_days = m_dCurrentWorkForDays;
			}
		}
		*/

		m_pRpc->set_global_prefs_override_struct(*pPrefs, *pPrefsMask);
		m_pRpc->read_global_prefs_override();
//		m_iSetDays = -1;						// set days no longer valid, may be changed in between
		return TRUE;
	}
	return FALSE;
}

int CThreadRpc::ReadProxy(GR_PROXY_INFO *pProxy)
{
	int iRet;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		iRet = m_pRpc->get_proxy_settings(*pProxy);
	}
	else
	{
		iRet = ERR_CONNECT;
	}
	return iRet;
}


int CThreadRpc::WriteProxy(GR_PROXY_INFO *pProxy)
{
	int iRet;

	if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		if (m_bBoincClientV6) iRet = m_pRpc->set_proxy_settingsV6(*pProxy);
		else iRet = m_pRpc->set_proxy_settings(*pProxy);
	}
	else
	{
		iRet = ERR_CONNECT;
	}
	return iRet;
}

int CThreadRpc::GetHostInfo(HOST_INFO *pHi)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected
	
//	if (*pHi->host_cpid == 0)
//	{
	if (pHi->_coprocs.ndevs() == 0)
	{
		retval = m_pRpc->get_host_info(*pHi);			// only once please
		return retval;
	}
//	}
	// already there update the benchmark only
	HOST_INFO hostInfo;
	retval = m_pRpc->get_host_info(hostInfo);

	pHi->p_calculated = hostInfo.p_calculated;				// update benchmark
//	hostInfo._coprocs..delete_coprocs();	xxxxxxxxxxxx

	return retval;
}

void CThreadRpc::GetTimeStats(TIME_STATS *pTimeStats)
{
	pTimeStats->active_frac = m_pState->time_stats.active_frac;
	pTimeStats->client_start_time = m_pState->time_stats.client_start_time;
	pTimeStats->connected_frac = m_pState->time_stats.connected_frac;
	pTimeStats->cpu_and_network_available_frac = m_pState->time_stats.cpu_and_network_available_frac;
	pTimeStats->gpu_active_frac = m_pState->time_stats.gpu_active_frac;
	pTimeStats->now = m_pState->time_stats.now;
	pTimeStats->on_frac  = m_pState->time_stats.on_frac;
	pTimeStats->previous_uptime = m_pState->time_stats.previous_uptime;
}

int CThreadRpc::GetPlatform(char *psPlatform, int *piPosition, int *piPlatformFound)
{
	int iSize;

	*piPlatformFound = FALSE;
	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected
	iSize = (int) m_pState->platforms.size();
	if (*piPosition >= iSize) return BOINC_SUCCESS;							// success and no string added = end
	strcpy_s (psPlatform,254, m_pState->platforms[*piPosition].c_str());
	*piPlatformFound = TRUE;
	return BOINC_SUCCESS;
}

int CThreadRpc::GetAllProjectsList(ALL_PROJECTS_LIST *pPl)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

	retval = m_pRpc->get_all_projects_list(*pPl);
	return retval;
}


int CThreadRpc::LookupAccount(ACCOUNT_IN *pAi)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected
	retval =  m_pRpc->lookup_account(*pAi);
	return retval;
}

int CThreadRpc::LookUpAccountPoll(ACCOUNT_OUT *pAo)
{
	int retval, iTimeout;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

    pAo->clear();
    pAo->error_num = ERR_RETRY;
	pAo->error_num = ERR_IN_PROGRESS;
	iTimeout = 0;
	while (pAo->error_num == ERR_IN_PROGRESS)
	{
		retval = m_pRpc->lookup_account_poll(*pAo);
		if (retval)	return retval;
		Sleep(10);
		if (iTimeout++ > 6000) return ERR_TIMEOUT;
	}
	return retval;
}

int CThreadRpc::ProjectAttach(CRpcProjectAttach *pProjectAttach)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

	retval = m_pRpc->project_attach(pProjectAttach->m_sUrl, pProjectAttach->m_sAuth, "");

	return retval;
}

int CThreadRpc::AccountManagerRpc(CRpcManagerAttach *pManagerAttach)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

	retval = m_pRpc->acct_mgr_rpc((const char *) pManagerAttach->m_sUrl, pManagerAttach->m_sName, pManagerAttach->m_sAuth);

	return retval;
}

int CThreadRpc::AccountManagerRpcPoll(ACCT_MGR_RPC_REPLY *pReply)
{
	int retval, iTimeout;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

    pReply->clear();
    pReply->error_num = ERR_RETRY;
	pReply->error_num = ERR_IN_PROGRESS;
	iTimeout = 0;
	while (pReply->error_num == ERR_IN_PROGRESS)
	{
		retval = m_pRpc->acct_mgr_rpc_poll(*pReply);
		if (retval)	return retval;
		Sleep(10);
		if (iTimeout++ > 6000) return ERR_TIMEOUT;
	}
	return retval;
}

int CThreadRpc::GetProjectConfig(char *pUrl)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected
	retval = m_pRpc->get_project_config(pUrl);
    return retval;
}

int CThreadRpc::GetProjectConfigPoll(PROJECT_CONFIG *pPc)
{
	int retval, iTimeout;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

    pPc->clear();
    pPc->error_num = ERR_RETRY;

	pPc->error_num = ERR_IN_PROGRESS;
	iTimeout = 0;
	while (pPc->error_num == ERR_IN_PROGRESS)
	{
		retval = m_pRpc->get_project_config_poll(*pPc);
		if (retval)	return retval;
		Sleep(10);
		if (iTimeout++ > 6000) return ERR_TIMEOUT;
	}
	return retval;
}

int CThreadRpc::CreateAccount(ACCOUNT_IN *pAi)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected
	retval =  m_pRpc->create_account(*pAi);
	return retval;
}

int CThreadRpc::CreateAccountPoll(ACCOUNT_OUT *pAo)
{
	int retval, iTimeout;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected

    pAo->clear();
    pAo->error_num = ERR_RETRY;
	pAo->error_num = ERR_IN_PROGRESS;
	iTimeout = 0;
	while (pAo->error_num == ERR_IN_PROGRESS)
	{
		retval = m_pRpc->create_account_poll(*pAo);
		if (retval)	return retval;
		Sleep(10);
		if (iTimeout++ > 6000) return ERR_TIMEOUT;
	}
	return retval;
}

int CThreadRpc::ClientGetCCStatus(CC_STATUS *pCc)
{
	int retval;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return ERR_TIMEOUT;	// only when connected
	pCc->gpu_mode = -1;
	retval = m_pRpc->get_cc_status(*pCc);
	return retval;
}

//void CThreadRpc::GetCCStatusPointer(CC_STATUS **pCc)
//{
//	*pCc = m_pCcStatus;
//}

void CThreadRpc::SetNumberOfDaysRegulator(int iWuTotal)
{
	GLOBAL_PREFS prefs;
	GLOBAL_PREFS prefsTemp;
	GLOBAL_PREFS_MASK prefs_mask;
	int	retval;

	double	dDays, dPreviousCurrentWorkForDays;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return;	// only when connected

	dPreviousCurrentWorkForDays = m_dCurrentWorkForDays;

	if (m_iRegulatorMaxTasks > 0)
	{
		// WU regulation
		double	dPercentage;
		dPercentage = m_iRegulatorMaxTasks/20;						// 20%

		int iDelta;
		iDelta = m_iRegulatorMaxTasks - iWuTotal;
		if (iDelta < 0)
		{
			// too many
			iDelta = abs(iDelta);
			dDays = iDelta;
			dDays /= dPercentage;
			m_dCurrentWorkForDays = m_dRegulatorDaysWorkRequested / dDays;
		}
		else
		{
			// short
			if (iDelta != 0)
			{
				m_dCurrentWorkForDays = m_dRegulatorDaysWorkRequested;
			}
		}
		if (m_dCurrentWorkForDays < 1) m_dCurrentWorkForDays = 1;
		if (m_dCurrentWorkForDays > m_dRegulatorDaysWorkRequested) m_dCurrentWorkForDays = m_dRegulatorDaysWorkRequested;

		if (dPreviousCurrentWorkForDays != m_dCurrentWorkForDays)		// only when changed
		{
			retval = m_pRpc->get_global_prefs_working_struct(prefs, prefs_mask);
			if (retval == ERR_NOT_FOUND) 
			{
				// Older clients don't support get_global_prefs_working_struct RPC
				retval = m_pRpc->get_global_prefs_override_struct(prefs, prefs_mask);
			}

			if (!retval)
			{
				if (m_bBoincClientV6)
				{
					prefs.work_buf_additional_days = m_dCurrentWorkForDays;
				}
				else
				{
					prefs.work_buf_min_days = m_dCurrentWorkForDays;
				}
				prefs_mask.clear();
				prefs_mask.set_all();
				m_pRpc->set_global_prefs_override_struct(prefs, prefs_mask);
				m_pRpc->read_global_prefs_override();
			}
			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, %s, WU now: %d, WU needed: %d, Set days: %.2f" ,m_pcHostName,m_pcComputerId, iWuTotal, m_iRegulatorMaxTasks, m_dCurrentWorkForDays);
			Logging("Regulator, set work buffer", m_pcBufferLog, LOGGING_NORMAL);
		}
	}
}

void CThreadRpc::SetNumberOfDays(double dDays)
{
	GLOBAL_PREFS prefs;
	GLOBAL_PREFS prefsTemp;
	GLOBAL_PREFS_MASK prefs_mask;
	int	retval;
	double	dPreviousCurrentWorkForDays;

	if (m_iConnectionStatus != RPC_STATUS_CONNECTED) return;	// only when connected

	dPreviousCurrentWorkForDays = m_dCurrentWorkForDays;
	m_dCurrentWorkForDays = dDays;

	if (dPreviousCurrentWorkForDays != m_dCurrentWorkForDays)		// only when changed
	{
		retval = m_pRpc->get_global_prefs_working_struct(prefs, prefs_mask);
		if (retval == ERR_NOT_FOUND) 
		{
			// Older clients don't support get_global_prefs_working_struct RPC
			retval = m_pRpc->get_global_prefs_override_struct(prefs, prefs_mask);
		}
		if (!retval)
		{
			if (m_bBoincClientV6)
			{
				prefs.work_buf_additional_days = m_dCurrentWorkForDays;
			}
			else
			{
				prefs.work_buf_min_days = m_dCurrentWorkForDays;
			}
			prefs_mask.clear();
			prefs_mask.set_all();
			m_pRpc->set_global_prefs_override_struct(prefs, prefs_mask);
			m_pRpc->read_global_prefs_override();
//			_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, %s, Set days: %.2f" ,m_pcHostName,m_pcComputerId, dDays);
//			Logging("Regulator, set work buffer", m_pcBufferLog, LOGGING_NORMAL);
		}
	}
}

// update alle projects to flush all wu, when WU regulater is set to low
// localhost only
void CThreadRpc::UpdateDaysRegulator(int iParam)
{
	int		iWuTotal;
	double	dTimeLeftCpu, dTimeLeftGpu;
	bool	bLocalhost;

	if (!m_bRegulatorActive) return;

	bLocalhost = m_bLocalhostEqual;
	if (strcmp(m_pcHostName, LOCALHOST_NAME) == 0)
	{
		bLocalhost = true;
	}

	if (!bLocalhost) return;


	if (m_iConnectionStatus != RPC_STATUS_CONNECTED)
	{
		UpdateAllProjectsWithWork(); // this will try to reconnect......				
		return;	// only when connected
	}

	UpdateAllProjectsWithWork();

	dTimeLeftCpu = 0;
	dTimeLeftGpu = 0;
	if (m_iTimeLeftCpu >= 0)
	{
		dTimeLeftCpu = (double) m_iTimeLeftCpu;
		dTimeLeftCpu /= 86400;
	}
	if (m_iTimeLeftGpu >= 0)
	{
		dTimeLeftGpu = (double) m_iTimeLeftGpu;
		dTimeLeftGpu /= 86400;
	}

	if (m_iTimeLeftCpu >= 0)
	{
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, %s, Left: CPU %.2f, GPU %.2f days, CPU min: %.2f, GPU %.2f days, Parm: %d" ,m_pcHostName, m_pcComputerId, dTimeLeftCpu, dTimeLeftGpu, m_dRegulatorMinDaysCpu, m_dRegulatorMinDaysGpu, iParam);
		Logging("Regulator, Check if Cpu low", m_pcBufferLog, LOGGING_DEBUG);

		if (dTimeLeftGpu > m_dRegulatorMinDaysGpu)
		{
			if (dTimeLeftCpu < m_dRegulatorMinDaysCpu)
			{
				_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, %s, Left: CPU %.2f, GPU %.2f days, set work to %.2f days" ,m_pcHostName, m_pcComputerId, dTimeLeftCpu, dTimeLeftGpu, m_dRegulatorDaysWorkRequestedMin);
				Logging("Regulator, Cpu low", m_pcBufferLog, LOGGING_NORMAL);
				SetNumberOfDays(m_dRegulatorDaysWorkRequestedMin);
				return;
			}
		}
	}

	if (m_iLeftCpu < 0)
	{
		iWuTotal = GetWuInAllProjects();
	}
	else 
	{
		iWuTotal = 	m_iLeftCpu + m_iLeftGpu;
	}

	if (iParam == REFRESH_PROJECTS)
	{
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Report all completed tasks on: Host: %s, %s, WU now: %d (CPU %.2f, GPU %.2f days)" ,m_pcHostName, m_pcComputerId, iWuTotal, dTimeLeftCpu, dTimeLeftGpu);
		Logging("Refresh", m_pcBufferLog, LOGGING_NORMAL);
		SetNumberOfDaysRegulator(iWuTotal);
	}
	else
	{
		if (m_iRegulatorMaxTasks > 0)
		{
			// update when there are too many wu in order to flush them
			if (iWuTotal > m_iRegulatorMaxTasks)
			{
				_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"Host: %s, %s, WU now: %d, (CPU %.2f, GPU %.2f days) WU needed: %d" ,m_pcHostName, m_pcComputerId, iWuTotal, dTimeLeftCpu, dTimeLeftGpu, m_iRegulatorMaxTasks);
				Logging("Regulator, flushing", m_pcBufferLog, LOGGING_NORMAL);
			}
			SetNumberOfDaysRegulator(iWuTotal);
		}
	}
}

//rpc.get_global_prefs_override_struct(prefs, prefs_mask);
//prefs.cpu_usage_limit = 0.5;
//prefs_mask.cpu_usage_limit = true;
//rpc.set_global_prefs_override_struct(prefs, prefs_mask);
//rpc.read_global_prefs_override();

void CThreadRpc::UpdateAllProjectsWithWork() 
{
	GetNewProjects();						// update projects
	ProjectOperationAllWithWork(PROJECT_OP_UPDATE);
//	m_pRpc->network_available();			// doet niets
}

void CThreadRpc::HeapCheck(char *pcFrom)
{
	int  heapstatus;
	
	strcpy_s(m_pcBufferLog,127,pcFrom);
	strcat_s(m_pcBufferLog,127," Computer: ");
	strcat_s(m_pcBufferLog,127,m_pcComputerId);
	strcat_s(m_pcBufferLog,127,"\r\n");
	strcat_s(m_pcBufferLog,127," ****************\r\nERROR: ");

	heapstatus = _heapchk();
	switch( heapstatus )
	{
		case _HEAPOK:
			return;
//		 strcat_s(m_pcBufferLog, 127, "heap is fine");
		break;
		case _HEAPEMPTY:
//		    strcpy_s(cStatus, 127, "OK - heap is empty" );
			return;
		break;
		case _HEAPBADBEGIN:
			strcat_s(m_pcBufferLog, 127, "bad start of heap" );
		break;
		case _HEAPBADNODE:
			strcat_s(m_pcBufferLog, 127, "bad node in heap" );
		break;
		case _HEAPEND:
			strcat_s(m_pcBufferLog, 127, "bad end of heap" );
		break;
		case _HEAPBADPTR:
			strcat_s(m_pcBufferLog, 127, "bad pointer in heap" );
		break;

		default:
			strcpy_s(m_pcBufferLog, 127, "Unknown heap problem" );
	}
	if (!m_bAfxErrorOnce)
	{
		AfxMessageBox(m_pcBufferLog);
		m_bAfxErrorOnce = true;
	}
	Logging(pcFrom, m_pcBufferLog, LOGGING_NORMAL);			// this may still work.
}

void CThreadRpc::Logging(char *pcFrom, char *pcTxt, int iLoggingType)
{
	strcpy_s(m_pcBufferLogFrom, 63, pcFrom);
	m_pLogging->m_pFrom = m_pcBufferLogFrom;
	m_pLogging->m_pText = pcTxt;
	m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLogging, iLoggingType);
}


BEGIN_MESSAGE_MAP(CThreadRpc, CWinThread)
//	ON_THREAD_MESSAGE (WM_MSG_THREAD_START_RPC_TASK, OnStartRpcTask) 
END_MESSAGE_MAP()


// CThreadRpc message handlers


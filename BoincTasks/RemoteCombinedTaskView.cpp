// RemoteView.cpp : implementation file
//



#include "stdafx.h"
#include "BoincTasks.h"
//#include "ComputerDoc.h"
#include "ListViewEx.h"					// base class for BoinCComputerView
#include "TemplateRemoteTaskView.h"		// base class for BoinCComputerView
#include "TemplateRemoteProjectView.h" 
#include "RemoteCombinedDoc.h"
#include "RemoteCombinedTaskView.h"
#include "RemoteCombinedProjectView.h"
#include "TemplateRemoteMessageView.h"
#include "RemoteCombinedMessageView.h"
#include "TemplateRemoteTransferView.h" 
#include "RemoteCombinedTransferView.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "TemplateRemoteComputerView.h"
#include "RemoteCombinedComputerView.h"
#include "RemoteCombinedNoticesView.h"
#include "threadrpc.h"
#include "dlgtimer.h"
#include "DlgColorMain.h"
#include "DlgPrefMain.h"
#include "DlgProxyMain.h"
#include "DlgYesNo.h"
#include "DlgNotices.h"
#include "DlgBarComputer.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsView.h"
#include "DlgSettingsGeneral.h"
#include "DlgSettingsTasks.h"
#include "DlgSettingsMessage.h"
#include "DlgSettingsView.h"
#include "DlgSettingsExpert.h"
#include "DlgSettingsHistory.h"
#include "DlgSettingsRules.h"
#include "DlgSettingsWWW.h"
#include "DlgFloating.h"
#include "DlgFont.h"
#include "DlgLogging.h"
#include "Translation.h"
#include "SortColumn.h"
#include "Selection.h"
#include "Rules.h"
#include "MainFrm.h"
#include "SortTask.h"
#include "CreateHtml.h"
#include "CreateHtmlWebServer.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"
#include "BoincClientManager.h"
#include "StringCopyNew.h"
#include "WebServerProcess.h"


//#include "RpcClient.h"

// CRemoteCombinedTaskView


IMPLEMENT_DYNCREATE(CRemoteCombinedTaskView, CTemplateRemoteTaskView)

CRemoteCombinedTaskView::CRemoteCombinedTaskView()
{
	m_iForceClose = 0;

	m_iThreadLocked = 0;
	m_iThreadLockedStatusCount = 0;
	m_bThreadBusy = false;
//	m_bThreadHistoryBusy = false;
	m_iStartupAliveTimer = 40;
	m_iUpdateWuTimer = UPDATE_TIMER_WU_TRIGGER-120;
	m_iTrickleMessageTimer = 0;

	m_pDoc = NULL;

	m_iWuTotal = 0;
	m_iWuTotalInList = 0;

//	m_hWndPreference = theApp.m_pDlgPrefMain->m_hWnd;
	m_hWndProxy = theApp.m_pDlgProxyMain->m_hWnd;

	m_bDocumentIsClosing = false;
	m_bDoOncePosition = true;
	m_iNowClosingDelayCount = 0;

	m_pFont = NULL;

	time_t tSystemTime;
	_time64(&tSystemTime);
	m_tNextRefreshDownloads = tSystemTime + theApp.m_iDownloadRefreshTime;
	m_tNextRefreshUploads = tSystemTime + theApp.m_iUploadRefreshTime;
	m_tNextRefresh = tSystemTime + theApp.m_iRefreshRefreshTime;
	m_tNextASAP = tSystemTime + theApp.m_iRefreshRefreshASAP;

	if (theApp.m_bAutoRefreshCheat) m_tNextRefreshAuto = tSystemTime + 60;
	else  m_tNextRefreshAuto = tSystemTime + 300;
	theApp.m_pDlgSettingsTasks->PostMessage(UWM_DLG_SETTINGS_TASKS_SORTING_STATUS, 0,0);

}

CRemoteCombinedTaskView::~CRemoteCombinedTaskView()
{
	for (int iPos = 0; iPos < (int) m_lRunningGraphic.size(); iPos++)
	{
		delete m_lRunningGraphic.at(iPos);
	}

	delete m_pFont;
	delete m_pDlgTimer;
}

void CRemoteCombinedTaskView::OnInitialUpdate()
{
//	int			i;
//	LV_COLUMN	lvc;

	CTemplateRemoteTaskView::OnInitialUpdate();

	m_pDlgTimer = new CDlgTimer;
	m_pDlgTimer->Create(IDD_DIALOG_TIMER);
	m_pDlgTimer->AttachWnd(this);

	m_pDoc = (CRemoteCombinedDoc*) GetDocument();

	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL,(WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);

	OnTimerDlg(0,0);		// fire up the timer to start the RPC A.S.A.P.
}

BEGIN_MESSAGE_MAP(CRemoteCombinedTaskView, CTemplateRemoteTaskView)
	ON_WM_SIZE()
//	ON_WM_KEYDOWN()
	ON_MESSAGE(UWM_MSG_NOTICES_NEW,OnNoticesNew)

	ON_MESSAGE(UWM_MSG_THREAD_GET_HOST_INFO_READY,OnHostInfo)
	ON_MESSAGE(UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO_READY,OnAccountManagerInfo)
	ON_MESSAGE(UWM_MSG_ADD_ACCOUNTMANAGER_CHANGED,OnAccountManagerChanged)

	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelTabView)
	ON_MESSAGE(UWM_MSG_VIEW_REFRESH,OnRefresh)

	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, OnComputerBarSelectionChanged)
	
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_TASK, OnReadyRpc) 
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_CLIENT, OnReadyRpcClient)
	ON_MESSAGE(UWM_MSG_TIMER_DLG, OnTimerDlg) 

	ON_MESSAGE (UWM_MSG_SETTING_TASKS, OnSettingsTasks)

	ON_MESSAGE (UWM_MSG_REMOTE_TASKS_SWITCH, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_WINDOW_SHOWN, OnSwitch)

	ON_MESSAGE (UWM_MSG_GET_FLOATER_DATA, OnFloaterData)
//	ON_MESSAGE (UWM_MSG_THREAD_RPC_FLOATER_DATA_READY,OnFloaterDataThreadReady)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_CPU, OnSetSnoozeCpu)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_GPU, OnSetSnoozeGpu)
	ON_MESSAGE (UWM_MSG_THREAD_SET_SNOOZE_CPU_READY, OnThreadSnoozeCpuReady)
	ON_MESSAGE (UWM_MSG_THREAD_SET_SNOOZE_GPU_READY, OnThreadSnoozeGpuReady)

	//ON_MESSAGE (UWM_MSG_THREAD_SET_NETWORK_MODE_READY, OnThreadNetworkModeReady)

	ON_MESSAGE (UWM_DLG_SETTINGS_TASKS_SORTING_STATUS, OnSettingsTasksSortingStatus)

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistorySetWidth)

	ON_COMMAND(ID_TASK_PROPERTIES, &CRemoteCombinedTaskView::OnTasksProperties)

	ON_COMMAND(ID_TASK_COPYALLTOCLIPBOARD, &CRemoteCombinedTaskView::OnCopyalltoclipboard)
	ON_COMMAND(ID_TASK_COPYSELECTEDTOCLIPBOARD, &CRemoteCombinedTaskView::OnCopyselectedtoclipboard)

	ON_COMMAND(ID_FILTER_READYTOSTART, &CRemoteCombinedTaskView::OnFilterReadytostart)
	ON_UPDATE_COMMAND_UI(ID_FILTER_READYTOSTART, OnUpdateFilterReadytostart)
	ON_COMMAND(ID_FILTERON_READYTOREPORT, &CRemoteCombinedTaskView::OnFilterReadytoreport)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_READYTOREPORT, OnUpdateFilterReadytoreport)
	ON_COMMAND(ID_FILTERON_WAITINGTORUN, &CRemoteCombinedTaskView::OnFilterWaitingtorun)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_WAITINGTORUN, OnUpdateFilterWaitingtorun)
	ON_COMMAND(ID_FILTERON_UPLOAD, &CRemoteCombinedTaskView::OnFilteronUpload)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_DOWNLOAD, &CRemoteCombinedTaskView::OnUpdateFilteronDownload)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_UPLOAD, &CRemoteCombinedTaskView::OnUpdateFilteronUpload)
	ON_COMMAND(ID_FILTERON_DOWNLOAD, &CRemoteCombinedTaskView::OnFilteronDownload)
	ON_COMMAND(ID_FILTERON_SUSPENDED, &CRemoteCombinedTaskView::OnFilteronSuspended)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_SUSPENDED, &CRemoteCombinedTaskView::OnUpdateFilteronSuspended)
	ON_COMMAND(ID_FILTERON_ABORTED, &CRemoteCombinedTaskView::OnFilteronAborted)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_ABORTED, &CRemoteCombinedTaskView::OnUpdateFilteronAborted)
	ON_COMMAND(ID_FILTERON_RUNNING, &CRemoteCombinedTaskView::OnFilteronRunning)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_RUNNING, &CRemoteCombinedTaskView::OnUpdateFilteronRunning)

	ON_COMMAND(ID_FILTERON_COMPUTATIONERROR, &CRemoteCombinedTaskView::OnFilteronComputation)
	ON_UPDATE_COMMAND_UI(ID_FILTERON_COMPUTATIONERROR, &CRemoteCombinedTaskView::OnUpdateFilteronComputation)

	ON_COMMAND(ID_TASK_SHOWACTIVETASKS, &CRemoteCombinedTaskView::OnShowActiveTasks)
	ON_UPDATE_COMMAND_UI(ID_TASK_SHOWACTIVETASKS, &CRemoteCombinedTaskView::OnUpdateShowActiveTasks)
	ON_COMMAND(ID_TASK_SHOWCPU, &CRemoteCombinedTaskView::OnTaskShowcpu)
	ON_COMMAND(ID_TASK_SHOW_NON_CPU_INTENSIVE, &CRemoteCombinedTaskView::OnTaskShowNonCpuIntensive)
	ON_COMMAND(ID_TASK_SHOWGPU, &CRemoteCombinedTaskView::OnTaskShowgpu)
	ON_UPDATE_COMMAND_UI(ID_TASK_SHOWCPU, &CRemoteCombinedTaskView::OnUpdateTaskShowcpu)
	ON_UPDATE_COMMAND_UI(ID_TASK_SHOW_NON_CPU_INTENSIVE, &CRemoteCombinedTaskView::OnUpdateTaskShowNonCpuIntensive)
	ON_UPDATE_COMMAND_UI(ID_TASK_SHOWGPU, &CRemoteCombinedTaskView::OnUpdateTaskShowgpu)

	ON_COMMAND(ID_TASK_SHOWGRAPHIC,&CRemoteCombinedTaskView::OnShowGraphic)
	ON_MESSAGE (UWM_MSG_THREAD_TASK_GRAPHIC_READY, OnShowGraphicReady)
	ON_COMMAND(ID_TOOLBAR_SUSPEND,&CRemoteCombinedTaskView::OnToolbarSuspend)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_SUSPEND,&CRemoteCombinedTaskView::OnUpdateToolbarSuspend)
	ON_COMMAND(ID_TOOLBAR_RESUME,&CRemoteCombinedTaskView::OnToolbarResume)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_RESUME,&CRemoteCombinedTaskView::OnUpdateToolbarResume)
	ON_COMMAND(ID_TOOLBAR_ABORT,&CRemoteCombinedTaskView::OnToolbarAbort)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ABORT,&CRemoteCombinedTaskView::OnUpdateToolbarAbort)
	ON_COMMAND(ID_TOOLBAR_PROJECT_UPDATE,&CRemoteCombinedTaskView::OnToolbarProjectUpdate)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_UPDATE,&CRemoteCombinedTaskView::OnUpdateToolbarProjectUpdate)
	ON_COMMAND(ID_TOOLBAR_PROJECT_SUSPEND,&CRemoteCombinedTaskView::OnToolbarProjectSuspend)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_SUSPEND,&CRemoteCombinedTaskView::OnUpdateToolbarProjectSuspend)
	ON_COMMAND(ID_TOOLBAR_PROJECT_RESUME,&CRemoteCombinedTaskView::OnToolbarProjectResume)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_RESUME,&CRemoteCombinedTaskView::OnUpdateToolbarProjectResume)
	ON_COMMAND(ID_TOOLBAR_PROJECT_NONEW,&CRemoteCombinedTaskView::OnToolbarProjectNoNew)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_NONEW,&CRemoteCombinedTaskView::OnUpdateToolbarProjectNoNew)
	ON_COMMAND(ID_TOOLBAR_PROJECT_ALLOWNEW,&CRemoteCombinedTaskView::OnToolbarProjectAllowNew)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_ALLOWNEW,&CRemoteCombinedTaskView::OnUpdateToolbarProjectAllowNew)

	ON_MESSAGE (UWM_MSG_PREFERENCE_DIALOG, OnPreferenceDialog)
	ON_MESSAGE (UWM_MSG_PROXY_DIALOG, OnProxyDialog)
	ON_COMMAND(ID_TASK_SUSPEND, &CRemoteCombinedTaskView::OnTaskSuspend)
	ON_COMMAND(ID_TASK_SUSPEND_CP, &CRemoteCombinedTaskView::OnTaskSuspendCheckpoint)
	ON_COMMAND(ID_TASK_RESUME, &CRemoteCombinedTaskView::OnTaskResume)
	ON_COMMAND(ID_TASK_ABORT, &CRemoteCombinedTaskView::OnTaskAbort)

	ON_COMMAND(ID_TASK_ADDRULE, &CRemoteCombinedTaskView::OnTaskAddRule)

	ON_MESSAGE(UWM_MSG_DOCUMENT_IS_CLOSING, OnDocumentIsClosing)
	ON_MESSAGE(UWM_MSG_THREAD_QUIT_READY, OnThreadQuit)

	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnFontChangeAll)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnFontHasChanged)

	ON_COMMAND(ID_HEADER_SETHEADERITEMS, &CRemoteCombinedTaskView::OnHeaderSetItems)

	ON_WM_CONTEXTMENU()
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_READ_PROJECT, OnReadyProject)
	ON_COMMAND(ID_WWW_1, &CRemoteCombinedTaskView::OnWww1)
	ON_COMMAND(ID_WWW_2, &CRemoteCombinedTaskView::OnWww2)
	ON_COMMAND(ID_WWW_3, &CRemoteCombinedTaskView::OnWww3)
	ON_COMMAND(ID_WWW_4, &CRemoteCombinedTaskView::OnWww4)
	ON_COMMAND(ID_WWW_5, &CRemoteCombinedTaskView::OnWww5)
	ON_COMMAND(ID_WWW_6, &CRemoteCombinedTaskView::OnWww6)
	ON_COMMAND(ID_WWW_7, &CRemoteCombinedTaskView::OnWww7)
	ON_COMMAND(ID_WWW_8, &CRemoteCombinedTaskView::OnWww8)
	ON_COMMAND(ID_WWW_9, &CRemoteCombinedTaskView::OnWww9)
	ON_COMMAND(ID_WWW_10, &CRemoteCombinedTaskView::OnWww10)
	ON_COMMAND(ID_WWW_11, &CRemoteCombinedTaskView::OnWww11)
	ON_COMMAND(ID_WWW_12, &CRemoteCombinedTaskView::OnWww12)
	ON_COMMAND(ID_WWW_13, &CRemoteCombinedTaskView::OnWww13)
	ON_COMMAND(ID_WWW_14, &CRemoteCombinedTaskView::OnWww14)
	ON_COMMAND(ID_WWW_15, &CRemoteCombinedTaskView::OnWww15)
	ON_COMMAND(ID_WWW_16, &CRemoteCombinedTaskView::OnWww16)
	ON_COMMAND(ID_WWW_17, &CRemoteCombinedTaskView::OnWww17)
	ON_COMMAND(ID_WWW_18, &CRemoteCombinedTaskView::OnWww18)
	ON_COMMAND(ID_WWW_19, &CRemoteCombinedTaskView::OnWww19)
	ON_COMMAND(ID_WWW_20, &CRemoteCombinedTaskView::OnWww20)

	ON_COMMAND(ID_TASK_PROJECTUPDATE, &CRemoteCombinedTaskView::OnTaskProjectupdate)
	ON_COMMAND(ID_TASK_PROJECTSUSPEND, &CRemoteCombinedTaskView::OnTaskProjectsuspend)
	ON_COMMAND(ID_TASK_PROJECTRESUME, &CRemoteCombinedTaskView::OnTaskProjectresume)

//	ON_MESSAGE (UWM_MSG_NOTICES_BT_UPDATE, OnBoincTasksVersion)	
//	ON_MESSAGE (UWM_MSG_NOTICES_BT_RULE, OnBoincTasksRule)	
	ON_MESSAGE (UWM_DLG_SEARCH_COMPUTER_READY, OnSearchComputerReady)

	ON_MESSAGE (UWM_MSG_WEB_ITEM_SELECTED, OnWebItemSelected)
	ON_MESSAGE(UWM_MSG_WEB_OPERATION,OnWebOperation)

	ON_MESSAGE(UWM_MSG_WEB_FILTER,OnWebFilter)
	ON_MESSAGE(UWM_MSG_WEB_SORT,OnWebSort)
	ON_MESSAGE(UWM_MSG_WEB_PROPERTIES,OnWebProperties)

	ON_COMMAND(ID_COMPUTERS_DETECT, &CRemoteCombinedTaskView::OnComputersDetect)

	END_MESSAGE_MAP()


// CRemoteCombinedTaskView diagnostics

#ifdef _DEBUG
void CRemoteCombinedTaskView::AssertValid() const
{
	CTemplateRemoteTaskView::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteCombinedTaskView::Dump(CDumpContext& dc) const
{
	CTemplateRemoteTaskView::Dump(dc);
}
#endif
#endif //_DEBUG

LRESULT CRemoteCombinedTaskView::OnTimerDlg(WPARAM parm1, LPARAM parm2)
{
	char	cBufferLogging[255];
	int		iTimerWu, iViewActive, iTimerFactorReadManual, iTimerFactor, iMinTime;
	HWND	hWnd;
	int		iNrRpc;
	CRemoteCombinedMessageView *pMessageView;
	CRemoteCombinedProjectView *pProjectView;
	CRemoteCombinedTransferView *pTransferView;
	CRemoteCombinedHistoryView *pHistoryView;
	CRemoteCombinedComputerView *pComputerView;
	CRemoteCombinedNoticesView *pNoticesView;
	CString	sFormat;
	bool	bSend, bSendWWW;
	bool	bVisible;

	if (m_pDoc == NULL) return 0;

	if (m_bDocumentIsClosing)
	{
		// force closing timer
		m_iForceClose++;

		if (m_iForceClose > theApp.m_iThreadTimeout)
		{
			m_pDoc->LogActiveComputers();
			m_iThreadsClosingCount = 0;							// ready to close
			this->PostMessage(UWM_MSG_THREAD_QUIT_READY,0,0);	// report start closing
		}
		sFormat.Format(" %d", m_iForceClose);
		sFormat = gszTranslation[PosWindowStatusMessageStatusClosing] + sFormat;
		theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
		int iForce = m_iForceClose&0x0f;
		if (iForce == 0)
		{
			sFormat.Format("%d Seconds to go", theApp.m_iThreadTimeout-m_iForceClose);
			m_pLog->m_pFrom = "Waiting to close down"; m_pLog->m_pText = sFormat.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_NORMAL);
			sFormat.ReleaseBuffer();
		}

		return 0;			// everything is closing down.
	}

	time_t tSystemTime;
	_time64(&tSystemTime);

	if (theApp.m_pDlgLogging->m_bLogDebugTasks)
	{
		m_pLog->m_pFrom = "Task: OnTimerDlg";
		_snprintf_s(cBufferLogging,254,_TRUNCATE,"m_bTreadBusy: %d, m_iThreadLocked: %d, m_iTrickleMessageTimer: %d, m_iUpdateWuTimer: %d", m_bThreadBusy, m_iThreadLocked, m_iTrickleMessageTimer,m_iUpdateWuTimer);m_pLog->m_pText = cBufferLogging;
		m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_DEBUG);
	}

//	if (theApp.m_pMainWnd->IsIconic() == TRUE) return 0;		// not visable = do nothing.

	iNrRpc = m_pDoc->GetRpcSelectedComputers();
	CheckAndSaveColumnsWidth();

	if (m_iStartupAliveTimer > 0)
	{
		m_iStartupAliveTimer--;						// keep everything alive at startup
	}
	else
	{
		m_iStartupAliveTimer = -1;
	}	

	bVisible = theApp.m_pMainWnd->IsWindowVisible() == TRUE;

	if (bVisible)
	{
		if (tSystemTime > (m_tLastHostInfo + HOST_INFO_INTERVAL))
		{
			m_tLastHostInfo = tSystemTime;
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_GET_HOST_INFO,(WPARAM) this->m_hWnd,(LPARAM) m_pDoc->GetHostInfo(iCount));
			}
		}
	}

	if (bVisible)
	{
		if (tSystemTime > (m_tLastAccountManagerInfo + ACCOUNTMANAGER_INFO_INTERVAL))
		{
			m_tLastAccountManagerInfo = tSystemTime;
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO,(WPARAM) this->m_hWnd,(LPARAM) m_pDoc->GetAccountManagerInfo(iCount));
			}
		}
	}

	if (bVisible || (m_iStartupAliveTimer > 0))	// when any window is visible to ensure menu update
	{
		if (tSystemTime > (m_tLastSnoozeInfo + SNOOZE_INFO_INTERVAL))
		{
			m_tLastSnoozeInfo = tSystemTime;
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_GET_SNOOZE_CPU_GPU,(WPARAM) this->m_hWnd, (LPARAM) FALSE);	// snooze and networkstatus
			}
		}
	}
	else
	{
		if (tSystemTime > (m_tLastSnoozeInfo + SNOOZE_INFO_INTERVAL_NOT_VISIBLE))
		{
			m_tLastSnoozeInfo = tSystemTime;
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				// only to the localhost please.
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_GET_SNOOZE_CPU_GPU,(WPARAM) this->m_hWnd, (LPARAM) TRUE);
			}
		}
	}

	if (theApp.m_iAutoRefreshTime >= 10)
	{
		if (tSystemTime > m_tNextRefreshAuto)													// every 5 minutes	
		{
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_REFRESH_TRANSFERS,(WPARAM) this->m_hWnd, theApp.m_iAutoRefreshTime);
			}
			if (theApp.m_bAutoRefreshCheat)
			{
				m_tNextRefreshAuto = tSystemTime + 60;
			}
			else
			{
				m_tNextRefreshAuto = tSystemTime + 300;
			}
		}
	}
	else
	{
		if (theApp.m_iDownloadRefreshTime > 0)
		{
			if (tSystemTime > m_tNextRefreshDownloads)
			{
				m_tNextRefreshDownloads = tSystemTime + theApp.m_iDownloadRefreshTime;
				int iThreadIdUpdate;
				for (int iCount = 0; iCount < iNrRpc; iCount++)
				{
					m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
					::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_REFRESH_TRANSFERS,(WPARAM) this->m_hWnd, STATUS_TRANSFER_DOWNLOAD);
				}
			}
		}
		if (theApp.m_iUploadRefreshTime > 0)
		{
			if (tSystemTime > m_tNextRefreshUploads)
			{
				m_tNextRefreshUploads = tSystemTime + theApp.m_iUploadRefreshTime;
				int iThreadIdUpdate;
				for (int iCount = 0; iCount < iNrRpc; iCount++)
				{
					m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
					::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_REFRESH_TRANSFERS,(WPARAM) this->m_hWnd, STATUS_TRANSFER_UPLOAD);
				}
			}
		}
	}
	if (theApp.m_iRefreshRefreshTime > 0)
	{
		if (tSystemTime > m_tNextRefresh)
		{
			m_tNextRefresh = tSystemTime + theApp.m_iRefreshRefreshTime;
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_UPDATE_DAYS,REFRESH_PROJECTS,0);
			}
			if (m_pDoc->m_pHistoryView != NULL)
			{
				m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_HISTORY_SOMETHING_HAPPENED,0,0);	// refresh history
			}
			m_iUpdateWuTimer = 0;															// not need for this
		}
	}
	if (theApp.m_iRefreshRefreshASAP > 0)
	{
		if (tSystemTime > m_tNextASAP)
		{
			m_tNextASAP = tSystemTime + theApp.m_iRefreshRefreshASAP;
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_UPDATE_DAYS_ASAP,0,0);
			}
			m_iUpdateWuTimer = 0;															// no need for this
		}
	}


// trickle update Messages to keep the list complete
	if (theApp.m_pDlgSettingsMessage->m_bMessagesKeep)
	{
		if (tSystemTime > (m_tLastMessageUpdate + MESSAGE_UPDATE_INTERVAL))
		{
			m_tLastMessageUpdate = tSystemTime;
			int iThreadIdUpdate;
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
				CRpcThreadSendMessage *pRpcThreadSendMessage;
				pRpcThreadSendMessage = new CRpcThreadSendMessage;
				pRpcThreadSendMessage->m_iInit = 0;
				pRpcThreadSendMessage->m_iKeepMessages = theApp.m_pDlgSettingsMessage->m_iKeepMessages;

				::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_START_RPC_MESSAGE,(WPARAM) NULL, (LPARAM) pRpcThreadSendMessage); 	// just update no responce back
			}
		}
	}

/*
	if (bVisible)
	{
		if (m_pDoc->m_pProjectView != NULL)
		{
			if (m_pDoc->m_pProjectView->m_bUseStats)
			{
				m_pDoc->m_pProjectView->PostMessage(UWM_MSG_THREAD_START_STATS, 1, 0);	// 1 = full read from wwww
			}
		}
	}
*/

	if (!m_bThreadBusy)
	{
		m_iThreadLocked = 0;
//		if (!m_bThreadHistoryBusy)
		{
			if (m_bApplyChangesOnColumn) ColumnChanged();					// rebuild everything
		}
	}
	else
	{
		if (m_iThreadLocked++ > theApp.m_iThreadTimeout)	// unlock threads
		{
//			theApp.m_bRebootRemoteCombined = true;			// force a reload.
			m_iThreadLocked = 0;
			m_pLog->m_pFrom = "OnTimerDlg"; m_pLog->m_pText = "Thread timeout, will restart.";m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, true);
			theApp.m_pMainFrame->PostMessage(UWM_MSG_FORCE_A_RESTART,0,0);
		}
	}
	
	// Always active even if view is not visible or the application not shown

	if (theApp.m_pDlgSettingsHistory->m_bHistoryEnabled)
	{
		if (!m_bThreadBusy)		// not when the task is still active
		{
			pHistoryView = m_pDoc->m_pHistoryView;
			if (pHistoryView != NULL)
			{
				hWnd = pHistoryView->m_hWnd;
				pHistoryView->SendMessage(UWM_MSG_TIMER_HYSTORY,0,0);
			}
		}
	}
	
	// send update request to all projects when needed, to report in wu and to send new wu

	if (m_iUpdateWuTimer++ > UPDATE_TIMER_WU_TRIGGER)
	{
		int iThreadIdUpdate;
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadIdUpdate, iCount);
			::PostThreadMessage(iThreadIdUpdate,UWM_MSG_THREAD_UPDATE_DAYS,REFRESH_PROJECS_REGULATOR,0);
		}
		if (m_pDoc->m_pHistoryView != NULL)
		{
			m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_HISTORY_SOMETHING_HAPPENED,0,0);	// refresh history
		}
		m_iUpdateWuTimer = 0;
	}

	iViewActive = m_pDoc->m_iViewActive;

	// update the tasks for WWW update

	bSendWWW = false;
	if (!bVisible) bSendWWW = true;
	if (iViewActive != TASK_VIEW_ACTIVE) bSendWWW = true;

	if (theApp.m_pDlgSettingsWWW->m_bWWWEnable)
	{
		int iRefreshTime;
		iRefreshTime = theApp.m_pDlgSettingsWWW->m_iRefreshTime;
		if (tSystemTime > (m_tLastTasksWWW + iRefreshTime))
		{
			m_bTasksWWW = true;
		}
	}

	if (bSendWWW)
	{
		if (m_bTasksWWW)
		{
			if (!m_bThreadBusy) // | !m_bThreadHistoryBusy)
			{
				if (theApp.m_pDlgSettingsGeneral->m_bConnecTthrottle)	// check if TThrottle is enabled
				{
					int iNrTThrottleRpc;
					iNrTThrottleRpc = m_pDoc->GetRpcSelectedComputers();
					SendTThrottleRequest(iNrTThrottleRpc);
				}
				else
				{
					for (int iCount = 0; iCount < (int) m_lRpcInfoClient.size(); iCount++)
					{
						m_lRpcInfoClient.at(iCount)->m_bReady = false;
						m_lRpcInfoClient.at(iCount)->m_bReadyCopy = false;
						m_lRpcInfoClient.at(iCount)->m_bConnectionLogged = false;
						// Never delete it here as it is made in RemoteCombined delete m_pRpcInfoClient[iCount]->m_pcComputerId;
						m_lRpcInfoClient.at(iCount)->m_sComputerId = "";
					}
				}
				SendWuRequest(iNrRpc, false);
			}
		}
	}

	pNoticesView = m_pDoc->m_pNoticesView;
	if (pNoticesView != NULL)
	{
		if (m_pDoc->m_pDlgNotices != NULL)	m_pDoc->m_pDlgNotices->PostMessage(UWM_MSG_TIMER_NOTICES,0,0);						// send timer to notices
	}

	switch (iViewActive)
	{
		case MESSAGE_VIEW_ACTIVE:
			pMessageView = m_pDoc->m_pMessageView;
			hWnd = pMessageView->m_hWnd;
			bSend = (pMessageView->IsWindowVisible() == TRUE);
			if (g_bWebServerActive) bSend = true;
			if (g_bCloudServerRequestData) bSend = true;
			if (m_iStartupAliveTimer > 0) bSend = true;
			if (bSend)
			{
				::PostMessage(hWnd,UWM_MSG_TIMER_MESSAGE,PARAM_MESSAGE,0);	// send timer to active view
				m_iTrickleMessageTimer = 0;
			}
		break;
		case PROJECT_VIEW_ACTIVE:
			pProjectView = m_pDoc->m_pProjectView;
			hWnd = pProjectView->m_hWnd;
			bSend = (pProjectView->IsWindowVisible() == TRUE);
			if (g_bWebServerActive) bSend = true;
			if (g_bCloudServerRequestData) bSend = true;
			if (m_iStartupAliveTimer > 0) bSend = true;
			if (bSend) ::PostMessage(hWnd,UWM_MSG_TIMER_PROJECT,0,0);				// send timer to active view
		break;
		case TRANSFER_VIEW_ACTIVE:
			if (!m_bThreadBusy)
			{
//				if (!m_bThreadHistoryBusy)
				{
					pTransferView = m_pDoc->m_pTransferView;
					hWnd = pTransferView->m_hWnd;
					bSend = (pTransferView->IsWindowVisible() == TRUE);
					if (g_bWebServerActive) bSend = true;
					if (g_bCloudServerRequestData) bSend = true;
					if (m_iStartupAliveTimer > 0) bSend = true;
					if (bSend) ::PostMessage(hWnd,UWM_MSG_TIMER_TRANSFER,0,0);		// send timer to active view
				}
			}
		break;
		case COMPUTER_VIEW_ACTIVE:
			pComputerView = m_pDoc->m_pComputerView;
			hWnd = pComputerView->m_hWnd;
			bSend = (pComputerView->IsWindowVisible() == TRUE);
			if (g_bWebServerActive) bSend = true;
			if (g_bCloudServerRequestData) bSend = true;
			if (bSend) ::PostMessage(hWnd,UWM_MSG_TIMER_COMPUTER,0,0);				// send timer to active view
		break;
		case NOTICES_VIEW_ACTIVE:
//			pNoticesView = m_pDoc->m_pNoticesView;
//			hWnd = pNoticesView->m_hWnd;
//			bSend = (pNoticesView->IsWindowVisible() == TRUE);
//			if (bSend) ::PostMessage(hWnd,UWM_MSG_TIMER_NOTICES,0,0);				// send timer to active view
		break;

		case HISTORY_VIEW_ACTIVE:
			// done someplace else
		break;
		default:
			if (m_bThreadBusy)	// | m_bThreadHistoryBusy)
			{
				if (bVisible)
				{
					if (m_pDoc->m_iViewActive == TASK_VIEW_ACTIVE)
					{
						m_iThreadLockedStatusCount++;
						if (m_iThreadLockedStatusCount > DELAY_SHOW_UPDATE_TIMER)
						{
							sFormat.Format(" %d", m_iThreadLockedStatusCount);
							sFormat = TEXT_STATUS_UPDATING + sFormat;
							theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
							sFormat.ReleaseBuffer();
						}
						else
						{
							sFormat = TEXT_STATUS_UPDATING + sFormat;
							theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
							sFormat.ReleaseBuffer();
						}
					}
				}
				return 0;
			}
			else
			{
				m_iThreadLockedStatusCount = 0;
			}

			bSend = (this->IsWindowVisible() == TRUE);
			if (m_iStartupAliveTimer > 0) bSend = true;
			if (g_bWebServerActive) bSend = true;
			if (g_bCloudServerRequestData) bSend = true;

			if (!bSend) return 0;

			int iWuTotal = m_iWuTotal;

			if (iWuTotal < 1) iWuTotal = 1;

			iTimerFactorReadManual = theApp.m_pDlgSettingsView->m_iRefreshManual;
			switch (theApp.m_pDlgSettingsView->m_iRefresh)
			{
				case REFRESH_SLOW:
					iMinTime = 7;
					iTimerFactor = 250;
					if (iWuTotal < iTimerFactor) iWuTotal = iTimerFactor;
					iTimerWu = iWuTotal / iTimerFactor;
				break;
				case REFRESH_NORMAL:
					iMinTime = 3;
					iTimerFactor = 500;
					if (iWuTotal < iTimerFactor) iWuTotal = iTimerFactor;
					iTimerWu = iWuTotal / iTimerFactor;
				break;
				case REFRESH_QUICK:
					iMinTime = 2;
					iTimerFactor = 1000;
					if (iWuTotal < iTimerFactor) iWuTotal = iTimerFactor;
					iTimerWu = iWuTotal / iTimerFactor;
				break;
				default: // manual
					iMinTime = 1;
					iTimerFactor = 1500;
					if (iWuTotal < iTimerFactor) iWuTotal = iTimerFactor;
					iTimerWu = iWuTotal / iTimerFactor;
					if (iTimerWu < iTimerFactorReadManual) iTimerWu = iTimerFactorReadManual;
			}

			if (iTimerWu < iMinTime) iTimerWu = iMinTime;

			int iTimeLeft;
			iTimeLeft = (int) (m_tLastTasksShown - tSystemTime) + iTimerWu;
			if (iTimeLeft < 0) iTimeLeft = 0;

			if (iTimerWu > 2)	sFormat.Format(gszTranslation[PosWindowStatusMessageTasksUpdateInSeconds], iTimeLeft, m_iWuTotal);	//"Update in %d seconds, %d Tasks"
			else				sFormat.Format(gszTranslation[PosWindowStatusMessageTasksWaiting], m_iWuTotal);						//"Waiting, %d Tasks"
			theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
			sFormat.ReleaseBuffer();

			if (iTimeLeft <= 0 || g_bCloudServerRequestData)
			{
				m_tLastTasksShown = tSystemTime;
				if (theApp.m_pDlgSettingsGeneral->m_bConnecTthrottle)	// check if TThrottle is enabled
				{
					int iNrTThrottleRpc;
					iNrTThrottleRpc = m_pDoc->GetRpcSelectedComputers();
					SendTThrottleRequest(iNrTThrottleRpc);
				}
				else
				{
					for (int iCount = 0; iCount < (int) m_lRpcInfoClient.size(); iCount++)
					{
						m_lRpcInfoClient.at(iCount)->m_bReady = false;
						m_lRpcInfoClient.at(iCount)->m_bReadyCopy = false;
						m_lRpcInfoClient.at(iCount)->m_bConnectionLogged = false;
						// Never delete it here as it is made in RemoteCombined delete m_pRpcInfoClient[iCount]->m_pcComputerId;
						m_lRpcInfoClient.at(iCount)->m_sComputerId = "";
					}
				}
				SendWuRequest(iNrRpc);
			}
	} // switch
	return 0;
}

void CRemoteCombinedTaskView::SendWuRequest(int iNrRpc, bool bExcludeFromList)
{
	char	*pcComputerId;
	HWND	hWnd;
	char	cBufferLogging[255];
	int		iThreadId;

	hWnd = this->m_hWnd;
	UINT iSorting;
	//       1111	1111
	//column option	filter
	iSorting = theApp.m_pDlgSettingsTasks->m_iMaxUpdateTime;
	iSorting <<= FILTER_POS_FULL_READ_INTERVAL;
	iSorting +=	m_iFilterEnabledReadyToReport	<< FILTER_BIT_POS_READY_TO_REPORT;
	iSorting +=	m_iFilterEnabledWaitingToRun	<< FILTER_BIT_POS_WAITING_TO_RUN;
	iSorting +=	m_iFilterEnabledReadyToStart	<< FILTER_BIT_POS_READY_TO_START;
	iSorting += m_iFilterEnabledDownload		<< FILTER_BIT_POS_DOWNLOAD;
	iSorting += m_iFilterEnabledUpload			<< FILTER_BIT_POS_UPLOAD;
	iSorting += m_iFilterEnabledSuspend			<< FILTER_BIT_POS_SUSPEND;
	iSorting += m_iFilterEnabledComputation		<< FILTER_BIT_POS_COMPUTATION;		
	iSorting += m_iFilterEnabledAborted			<< FILTER_BIT_POS_ABORTED;
	iSorting += m_iFilterEnabledRunning			<< FILTER_BIT_POS_RUNNING;

	iSorting += m_iShowActiveTasks				<< FILTER_BIT_POS_SHOW_RUNNING_TASKS;
	iSorting += m_iShowNonCpuIntensive			<< FILTER_BIT_POS_SHOW_NON_CPU_INTENSIVE;
	iSorting += m_iShowCpu						<< FILTER_BIT_POS_SHOW_CPU;
	iSorting += m_iShowGpu						<< FILTER_BIT_POS_SHOW_GPU;

	m_iSendRpcCount = 0;	//iNrRpc;
	m_iReadyRpcCount = 0;
	m_iWuTotal = 0;
	m_iWuTotalInList = 0;
	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		m_pDoc->GetRpcThreadId(&iThreadId, iCount);
		if (bExcludeFromList)
		{
			if (m_pDoc->GetComputerIdStringWithIdNumber(iThreadId, &pcComputerId))
			{// get the computer id string for the thread
				int iResult = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, (WPARAM) pcComputerId, 0);
				if (iResult == TRUE)
				{
					if (theApp.m_pDlgLogging->m_bLogDebugTasks)
					{
						m_pLog->m_pFrom = "Tasks: SendWuRequest_exclude";
						_snprintf_s(cBufferLogging,254,_TRUNCATE,"piThreadId: %d", iThreadId);m_pLog->m_pText = cBufferLogging;
						m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_DEBUG);
					}	

					::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_TASK,(WPARAM) hWnd,iSorting);
					m_iSendRpcCount++;
					m_bThreadBusy = true;	
				}
			}
		}
		else
		{
			if (theApp.m_pDlgLogging->m_bLogDebugTasks)
			{
				m_pLog->m_pFrom = "Tasks: SendWuRequest";
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"piThreadId: %d", iThreadId);m_pLog->m_pText = cBufferLogging;
				m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_DEBUG);
			}
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_TASK,(WPARAM) hWnd,iSorting);
			m_iSendRpcCount++;
			m_bThreadBusy = true;	
		}
	}
	if (m_bThreadBusy) theApp.m_pMainFrame->SetBoincTasksStatus(TEXT_STATUS_UPDATING);

	if (m_iSendRpcCount == 0)
	{
		CListCtrl& listCtrl = GetListCtrl();
		listCtrl.DeleteAllItems();
	}
}

void CRemoteCombinedTaskView::SendTThrottleRequest(int iNrTThrottleRpc)
{
	char	*pcComputerId;
	HWND	hWnd;
	int		iStatus, iTThrottleThreadId;

	hWnd = this->m_hWnd;

	for (int iCount = 0; iCount < iNrTThrottleRpc; iCount++)
	{
		m_pDoc->GetTThrottleRpcThreadId(&iTThrottleThreadId, iCount);
		if (m_pDoc->GetTThrottleComputerIdStringWithIdNumber(iTThrottleThreadId, &pcComputerId))
		{// get the computer id string for the thread
			int iResult = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, (WPARAM) pcComputerId, 0);
			if (iResult == TRUE)
			{
				// only computers that are connected to BOINC
				iStatus = RPC_STATUS_NONE;
				for (int iCountMatch = 0; iCountMatch < iNrTThrottleRpc; iCountMatch++)
				{
					if (m_lRpcInfo.at(iCountMatch)->m_pcComputerId != NULL)
					{
						if (strcmp(pcComputerId, m_lRpcInfo.at(iCountMatch)->m_pcComputerId) == 0)
						{
							iStatus =  m_lRpcInfo.at(iCountMatch)->m_iStatusRpc;
						}
					}
				}

				if (iStatus == RPC_STATUS_CONNECTED)
				{
					m_lRpcInfoClient.at(iCount)->m_bReady = false;
					m_lRpcInfoClient.at(iCount)->m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
					::PostThreadMessage(iTThrottleThreadId,UWM_MSG_THREAD_START_RPC_CLIENT,(WPARAM) hWnd, (LPARAM) m_lRpcInfoClient.at(iCount));
				}
			}
		}
	}
}

void CRemoteCombinedTaskView::Sort(CListCtrl *plistCtrl)
{
	UINT	iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;

	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);

	if (plistCtrl->GetItemCount() > 1)
	{
		DWORD iSorting = (iSortingColumnPrimary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionPrimary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnSecondary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionSecondary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnTertiary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionTertiary&0xf);
		plistCtrl->SortItems(TaskSorting,iSorting);
	}
	DisplayMakeLineColors(theApp.m_pDlgSettingsView->m_bAlternatingStriped, theApp.m_pDlgSettingsView->m_bUseSkin, theApp.m_pDlgSettingsView->m_iUseSkin);
}

LRESULT CRemoteCombinedTaskView::OnHostInfo(WPARAM parm1, LPARAM parm2)
{
	int iStatus;
	HOST_INFO *pHostInfo;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	iStatus = (int) parm1;

	if (iStatus == 0)
	{
		pHostInfo = (HOST_INFO *) parm2;
		m_pDoc->OnHostInfo(pHostInfo);
	}
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnAccountManagerInfo(WPARAM parm1, LPARAM parm2)
{
	// no need to do anyting hiere, the values are already updated. The pointer was provided!
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnAccountManagerChanged(WPARAM parm1, LPARAM parm2)
{
	m_tLastAccountManagerInfo = 0;	// update the menu items ASAP
	return 0;
}




//LRESULT CRemoteCombinedTaskView::OnHistoryUnlock(WPARAM parm1, LPARAM parm2)
//{
//	m_bThreadHistoryBusy = false;
//	return 0;
//}


LRESULT CRemoteCombinedTaskView::OnSwitch(WPARAM parm1, LPARAM parm2)
{
	int iTimeToUpdate;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (parm2 == REFRESH_VIEW_NOW) 
	{
		m_tLastTasksShown = 0;	// direct action
	}

	time_t tSystemTime;
	_time64(&tSystemTime);
	iTimeToUpdate = (int) (m_tLastSwitchedTime - tSystemTime);
	
	m_tLastSwitchedTime = tSystemTime + 10;	// don't update within 10 seconds.

	if (iTimeToUpdate <= 0) 
	{
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
		m_tLastTasksShown = 0;	// direct action
	}
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnSettingsTasks(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc != NULL) m_pDoc->SettingsTasks(parm1 != 0);
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnReadyRpcClient(WPARAM parm1, LPARAM parm2)
{
	CRpcInfoClient *pInfoClient;
//	CString sFrom, sText;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	pInfoClient = (CRpcInfoClient *) parm1;
	if (pInfoClient->m_iStatus == 0)		// valid
	{
		pInfoClient->m_bReady = true;
		pInfoClient->m_bReadyCopy = true;
		pInfoClient->m_fCpuThrottleCopy = pInfoClient->m_fCpuThrottle;
		pInfoClient->m_fGpuThrottleCopy = pInfoClient->m_fGpuThrottle;
		pInfoClient->m_fTopCpuTemperatureCopy = pInfoClient->m_fTopCpuTemperature;
		pInfoClient->m_fTopGpuTemperatureCopy = pInfoClient->m_fTopGpuTemperature;
		pInfoClient->m_iLostCount = 0;

		for (int iCount = 0; iCount < MAX_GPU; iCount++)
		{
			pInfoClient->m_fGpuTemperatureCopy[iCount] = pInfoClient->m_fGpuTemperature[iCount];
		}
	}
	else
	{
		if (pInfoClient->m_iLostCount > 10)
		{
			pInfoClient->m_bReadyCopy = false;				// after 3 times lost invalidate
		}
		else
		{
			pInfoClient->m_iLostCount += 1;
		}
	}

	if (pInfoClient->m_bReadyCopy)
	{	
		if (!pInfoClient->m_bConnectionLogged)	// log connection
		{
			char *cBufferLogging;
			cBufferLogging = new char [256];

			pInfoClient->m_bConnectionLogged = true;
			m_pLog->m_pFrom = LOGGING_TEXT_CONNECTION;
			_snprintf_s(cBufferLogging,128,_TRUNCATE,"Connected to: Host: %s, %s,Port: %s, TThrottle Version: %.2f", pInfoClient->m_sHostName.c_str(), pInfoClient->m_sComputerId.c_str(), pInfoClient->m_sPort.c_str(), pInfoClient->m_fVersion);
			m_pLog->m_pText = cBufferLogging;
			m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_NORMAL);
			delete cBufferLogging;
		}
	}
	else pInfoClient->m_bConnectionLogged = false;
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnReadyRpc(WPARAM parm1, LPARAM parm2)
{
	LV_ITEM				lvi;
	int					iNrWu, iItemsAdd, iNrItems, iAddPos, iItemsAddTemp, iFromWho, iRow, iCount;
	int					iFilterTotalVersion, iFilterTotalWu, iTemp;
	CFilterData			*pFilterData;
	bool				bLineColor = false, bOverride = false, bSelectedFound = false, bNumberChanged = false, bSorting;
	CRpcThreadReturn	*pRpcThreadReturn;
	CThreadRpc			*pThreadRpc;
	CString				sTest;
	char				cBufferLogging[255];
	char				*pcComputerId;
	double				m_dDiskInterval;
	UINT				iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;

	pRpcThreadReturn = (CRpcThreadReturn *)	parm1;
	iFromWho   = (int) parm2;	

	if (m_bDocumentIsClosing)
	{
		delete pRpcThreadReturn;
		return 0;			// everything is closing down.
	}


	if (m_iReadyRpcCount > (int) m_lRpcInfo.size())	// something very wrong
	{
		delete pRpcThreadReturn;
//		theApp.m_bRebootRemoteCombined = true;			
		theApp.m_pMainFrame->PostMessage(UWM_MSG_FORCE_A_RESTART,0,0); // force a reload.
		return 0;
	}

	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);

	iNrWu = pRpcThreadReturn->m_iNumber;
	m_iWuTotal+= iNrWu;
	m_iWuTotalInList += iNrWu;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_iNrWu = iNrWu;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_pcComputerId = pRpcThreadReturn->m_pcComputerId;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_pThread = pRpcThreadReturn->m_pThread;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_iNrWu = pRpcThreadReturn->m_iNumber;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_iStatusRpc = pRpcThreadReturn->m_iStatus;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_dDiskInterval = pRpcThreadReturn->m_dDiskInterval;

	if (theApp.m_pDlgLogging->m_bLogDebugTasks)
	{
		m_pLog->m_pFrom = "Task: OnReadyRpc";
		_snprintf_s(cBufferLogging,254,_TRUNCATE,"piThreadId: %d, Computer: %s, Status: %d, Nr: %d", pRpcThreadReturn->m_pThread->m_nThreadID, pRpcThreadReturn->m_pcComputerId, pRpcThreadReturn->m_iStatus, iNrWu);m_pLog->m_pText = cBufferLogging;
		m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_DEBUG);
	}	

	theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_CONNECT, (WPARAM) pRpcThreadReturn->m_pcComputerId, pRpcThreadReturn->m_iStatus);

	delete pRpcThreadReturn;
	pRpcThreadReturn = NULL;

	// filter info
	for (int iFilter=0; iFilter < MAX_FILTER_TYPE; iFilter++)
	{
		m_lRpcInfo.at(m_iReadyRpcCount)->m_pFilter = m_lRpcInfo.at(m_iReadyRpcCount)->m_pThread->GetFilterInfo(iFilter,&iFilterTotalVersion, &iFilterTotalWu);
		m_lRpcInfo.at(m_iReadyRpcCount)->m_iFilterTotalVersion[iFilter] = iFilterTotalVersion;
		m_lRpcInfo.at(m_iReadyRpcCount)->m_iFilterTotalWu[iFilter] = iFilterTotalWu;
		m_iWuTotalInList+= iFilterTotalVersion; // plus the extra version totals
		m_iWuTotalInList-= iFilterTotalWu;		// minus the filtered wu
	}

	m_iReadyRpcCount++;

	if (m_iReadyRpcCount != m_iSendRpcCount) return 0;	// wait for all results to come in

	CDlgSettingsView *pSettingsView;
	pSettingsView = theApp.m_pDlgSettingsView;
	m_bUseHorizontalGrid = pSettingsView->m_bUseHorizontalGrid;
	m_bUseVerticalGrid = pSettingsView ->m_bUseVerticalGrid;

	if (m_iWuTotalInList < 0) m_iWuTotalInList = 0;

	// setup sorting
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		m_lRpcInfo.at(iCount)->m_iSortIndex = iCount;
	}
	
	bool bSortingAZ = true;
	// just as well do the correct sorting
	if (iSortingColumnPrimary   == COLUMN_COMPUTER)	if (iSortingOptionPrimary == SORTING_ZA) bSortingAZ = false;
	if (iSortingColumnSecondary == COLUMN_COMPUTER)	if (iSortingOptionSecondary == SORTING_ZA) bSortingAZ = false;
	if (iSortingColumnTertiary  == COLUMN_COMPUTER)	if (iSortingOptionTertiary == SORTING_ZA) bSortingAZ = false;

	// no sorting if there is less than 2
	if (m_iReadyRpcCount > 1)
	{
		bSorting = true;
		while (bSorting)
		{
			bSorting = false;
			for (iCount = 0; iCount < m_iReadyRpcCount-1; iCount++)
			{
				if (bSortingAZ)
				{
					if (strcmp(m_lRpcInfo.at(m_lRpcInfo.at(iCount)->m_iSortIndex)->m_pcComputerId, m_lRpcInfo.at(m_lRpcInfo.at(iCount+1)->m_iSortIndex)->m_pcComputerId) > 0)
					{
						iTemp = m_lRpcInfo.at(iCount)->m_iSortIndex;
						m_lRpcInfo.at(iCount)->m_iSortIndex = m_lRpcInfo.at(iCount+1)->m_iSortIndex;
						m_lRpcInfo.at(iCount+1)->m_iSortIndex = iTemp;
						bSorting = true;
					}
				}
				else
				{
					if (strcmp(m_lRpcInfo.at(m_lRpcInfo.at(iCount)->m_iSortIndex)->m_pcComputerId, m_lRpcInfo.at(m_lRpcInfo.at(iCount+1)->m_iSortIndex)->m_pcComputerId) < 0)
					{
						iTemp = m_lRpcInfo.at(iCount)->m_iSortIndex;
						m_lRpcInfo.at(iCount)->m_iSortIndex = m_lRpcInfo.at(iCount+1)->m_iSortIndex;
						m_lRpcInfo.at(iCount+1)->m_iSortIndex = iTemp;
						bSorting = true;
					}
				}
			}
		}
	}

	StartRatio();
	m_pDoc->DisplayStatus(m_iReadyRpcCount, &m_lRpcInfo.at(0)->m_iStatusRpc);//, m_rpcInfo.m_pcComputerId[0]);

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();
	iItemsAdd = iNrItems - m_iWuTotalInList;

	if (iItemsAdd > 0)
	{
		bNumberChanged = true;
		iItemsAddTemp = iItemsAdd;
		// delete list items
		bOverride = true;
		while (iItemsAddTemp > 0)
		{
			listCtrl.DeleteItem(iNrItems-1);
			iNrItems--;
			iItemsAddTemp--;
		}
	}
	else
	{
		if (iItemsAdd < 0)
		{
			bNumberChanged = true;
			// add list items
			bOverride = true;
			while (iItemsAdd < 0)
			{
				lvi.mask = LVIF_TEXT | LVIF_PARAM;
				iAddPos =  iNrItems-1;
				if (iAddPos < 0) iAddPos = 0;
				lvi.iItem = iAddPos;								// add at the end

				lvi.iSubItem = 0;
				lvi.pszText = gszTranslation[PosViewTasksInitializing];		//"Initialising";// info.m_pszProject;	
				lvi.lParam = (LPARAM) &m_dataDummy;
				listCtrl.InsertItem(&lvi);
				iNrItems++;
				iItemsAdd++;
			}
		}
	}

	// for every computer

	iRow = 0;
	for (int iComputer = 0; iComputer < m_iReadyRpcCount; iComputer++)
	{
		pThreadRpc = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_pThread;
		iNrWu = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_iNrWu;
		pFilterData = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_pFilter;
		pcComputerId = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_pcComputerId;
		m_dDiskInterval = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_dDiskInterval;

// find the right RpcClient computer
		int iRpcClientFound = -1;
		for (int iComputerClient = 0; iComputerClient < (int) m_lRpcInfoClient.size(); iComputerClient++)
		{
			if (m_lRpcInfoClient.at(iComputerClient)->m_sComputerId.length() > 0)
			{
				if (strcmp(m_lRpcInfoClient.at(iComputerClient)->m_sComputerId.c_str(), (const char *) pcComputerId) == 0)
				{
					iRpcClientFound = iComputerClient;
					break;
				}
			}
		}

		DisplayRpc(pThreadRpc, iRpcClientFound, iNrWu, &iRow, pFilterData, m_dDiskInterval, pcComputerId, iComputer);
	}

	while (iRow <= iNrItems+1)
	{
		listCtrl.DeleteItem(iNrItems+1);
		iNrItems--;
	}
//	int iNRLIST = listCtrl.GetItemCount();
//	if (iRow == 0) listCtrl.DeleteAllItems();

	if (bNumberChanged)	Invalidate();

	Sort(&listCtrl);

	time_t tSystemTime;
	_time64(&tSystemTime);
	if (m_bTasksWWW)
	{
		CCreateHtml createHtml;
		createHtml.CreateMultiple(this, &listCtrl, &m_iColumnOrder[0]);
		createHtml.StartFtp();
		m_bTasksWWW = false;
		m_tLastTasksWWW = tSystemTime;
	}

	if (g_bWebServerActive)
	{
		if (!m_bDocumentIsClosing && (g_pcWebServerHtml == NULL))
		{
			CCreateHtmlWebServer createHtml;
			char *pcHtml;
			if (createHtml.Create(PosBarViewSelectTasks, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TASK_COLUMNS, &m_sCloudColumn, PosGroupViewTasks, &pcHtml))
			{
				g_pcWebServerHtml = pcHtml;
				g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_TASKS);
				g_tWebServerHtml = GetTickCount()/100;
			}
		}
	}
	if (g_pThreadCloudServer != NULL)
	{
		if (g_bCloudServerRequestData)
		{
			if (!m_bDocumentIsClosing && (g_pcCloudServerHtml == NULL))
			{
				if (g_iCloudServerHtmlTabProcessed[TAB_TASKS] == TRUE)
				{
					g_iCloudServerHtmlTabProcessed[TAB_TASKS] = FALSE;
					CCreateHtmlWebServer createHtml;
					char *pcHtml;
					if (createHtml.Create(PosBarViewSelectTasks, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TASK_COLUMNS, &m_sCloudColumn, PosGroupViewTasks, &pcHtml))
					{
						g_pcCloudServerHtml = pcHtml;
						g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML, m_iWuTotal,TAB_TASKS);
						g_tCloudServerHtml = GetTickCount()/100;
					}
				}
			}
		}
	}

//	CreateHtml(listCtrl);

	StopRatio();

	m_tLastTasksShown = tSystemTime;

	m_bThreadBusy = false;
	return 0;
}


void CRemoteCombinedTaskView::DisplayRpc(CThreadRpc *pThreadRpc,int iComputerClient, int iNrWu, int *piRow, CFilterData *pFilterData, double m_dDiskInterval, char *pcComputerName, int iComputerId)
{
	CRpcTaskInfo		info;
	bool				bLineColor = false, bOverride = false, bCuda = false;
	int					i;//, iElapsed;
	bool				bFilterValid;


	GetRules();

	bFilterValid = pFilterData->m_bValid;

	CListCtrl& listCtrl = GetListCtrl();
	for(i = 0; i < iNrWu; i++)
	{
		pThreadRpc->GetRpcWu(i,&info);

		if (bFilterValid)
		{
			if (info.m_iFiltered > 0) continue;
		}

		if (*info.m_pszProject != 0)
		{
			DisplaySingleItem(&listCtrl, *piRow, &info, m_dDiskInterval, iComputerClient,pcComputerName, iComputerId);
		}
		else
		{
			int ii = 1; // oeps
		}

		*piRow += 1;
	}

	DisplayFiltered(&listCtrl, piRow, pFilterData, iComputerClient, pcComputerName, iComputerId);
}

void CRemoteCombinedTaskView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTemplateRemoteTaskView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRemoteCombinedTaskView::OnSize(UINT nType, int cx, int cy )
{
	theApp.m_pMainFrame->m_dlgBarComputer.PostMessage(UWM_MSG_NEW_SIZE, cx, cy);
	CTemplateRemoteTaskView::OnSize(nType, cx, cy );
}

void CRemoteCombinedTaskView::OnFilterReadytostart()
{
	// can do on close document

	if (m_iFilterEnabledReadyToStart == FILTER_ACTIVE) m_iFilterEnabledReadyToStart = FILTER_INACTIVE;
	else m_iFilterEnabledReadyToStart = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterReadyToStart, m_iFilterEnabledReadyToStart);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilterReadytostart(CCmdUI* pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledReadyToStart);
}

void CRemoteCombinedTaskView::OnFilterReadytoreport()
{
	// can do on close document
	if (m_iFilterEnabledReadyToReport == FILTER_ACTIVE) m_iFilterEnabledReadyToReport = FILTER_INACTIVE;
	else m_iFilterEnabledReadyToReport = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterReadyToReport, m_iFilterEnabledReadyToReport);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilterReadytoreport(CCmdUI* pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledReadyToReport);
}
void CRemoteCombinedTaskView::OnFilterWaitingtorun()
{
	// can do on close document
	if (m_iFilterEnabledWaitingToRun == FILTER_ACTIVE) m_iFilterEnabledWaitingToRun = FILTER_INACTIVE;
	else m_iFilterEnabledWaitingToRun = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterWaitingToRun, m_iFilterEnabledWaitingToRun);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilterWaitingtorun(CCmdUI* pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledWaitingToRun);
}

void CRemoteCombinedTaskView::OnFilteronUpload()
{
	// can do on close document
	if (m_iFilterEnabledUpload == FILTER_ACTIVE) m_iFilterEnabledUpload = FILTER_INACTIVE;
	else m_iFilterEnabledUpload = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterUpload, m_iFilterEnabledUpload);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilteronUpload(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledUpload);
}

void CRemoteCombinedTaskView::OnFilteronDownload()
{
	// can do on close document
	if (m_iFilterEnabledDownload == FILTER_ACTIVE) m_iFilterEnabledDownload = FILTER_INACTIVE;
	else m_iFilterEnabledDownload = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterDownload, m_iFilterEnabledDownload);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilteronDownload(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledDownload);
}

void CRemoteCombinedTaskView::OnFilteronSuspended()
{
	// can do on close document
	if (m_iFilterEnabledSuspend == FILTER_ACTIVE) m_iFilterEnabledSuspend = FILTER_INACTIVE;
	else m_iFilterEnabledSuspend = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterSuspend, m_iFilterEnabledSuspend);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilteronSuspended(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledSuspend);
}

void CRemoteCombinedTaskView::OnFilteronAborted()
{
	// can do on close document
	if (m_iFilterEnabledAborted == FILTER_ACTIVE) m_iFilterEnabledAborted = FILTER_INACTIVE;
	else m_iFilterEnabledAborted = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterAborted, m_iFilterEnabledAborted);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilteronAborted(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledAborted);
}

void CRemoteCombinedTaskView::OnFilteronRunning()
{
	// can do on close document
	if (m_iFilterEnabledRunning == FILTER_ACTIVE) m_iFilterEnabledRunning = FILTER_INACTIVE;
	else m_iFilterEnabledRunning = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterRunning, m_iFilterEnabledRunning);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilteronRunning(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledRunning);
}

void CRemoteCombinedTaskView::OnFilteronComputation()
{
	// can do on close document
	if (m_iFilterEnabledComputation == FILTER_ACTIVE) m_iFilterEnabledComputation = FILTER_INACTIVE;
	else m_iFilterEnabledComputation = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryFilterComputation, m_iFilterEnabledComputation);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateFilteronComputation(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iFilterEnabledComputation);
}

void CRemoteCombinedTaskView::OnShowActiveTasks()
{
	// can do on close document
	if (m_iShowActiveTasks == FILTER_ACTIVE) m_iShowActiveTasks = FILTER_INACTIVE;
	else m_iShowActiveTasks = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryShowActiceTasks, m_iShowActiveTasks);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateShowActiveTasks(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iShowActiveTasks);
}

void CRemoteCombinedTaskView::OnTaskShowcpu()
{
	// can do on close document
	if (m_iShowCpu == FILTER_ACTIVE) m_iShowCpu = FILTER_INACTIVE;
	else m_iShowCpu = FILTER_ACTIVE;

	if ((m_iShowCpu == FILTER_INACTIVE) && (m_iShowGpu == FILTER_INACTIVE))
	{
		m_iShowGpu = FILTER_ACTIVE;
		theApp.WriteProfileInt(registrySectionFilter, registryShowGpu, m_iShowGpu);
	}

	theApp.WriteProfileInt(registrySectionFilter, registryShowCpu, m_iShowCpu);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateTaskShowcpu(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iShowCpu);
}

void CRemoteCombinedTaskView::OnTaskShowgpu()
{
	// can do on close document
	if (m_iShowGpu == FILTER_ACTIVE) m_iShowGpu = FILTER_INACTIVE;
	else m_iShowGpu = FILTER_ACTIVE;

	if ((m_iShowCpu == FILTER_INACTIVE) && (m_iShowGpu == FILTER_INACTIVE))
	{
		m_iShowCpu = FILTER_ACTIVE;
		theApp.WriteProfileInt(registrySectionFilter, registryShowCpu, m_iShowCpu);
	}

	theApp.WriteProfileInt(registrySectionFilter, registryShowGpu, m_iShowGpu);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateTaskShowgpu(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iShowGpu);
}

void CRemoteCombinedTaskView::OnTaskShowNonCpuIntensive()
{
	// can do on close document
	if (m_iShowNonCpuIntensive == FILTER_ACTIVE) m_iShowNonCpuIntensive = FILTER_INACTIVE;
	else m_iShowNonCpuIntensive = FILTER_ACTIVE;

	theApp.WriteProfileInt(registrySectionFilter, registryShowNonCpuIntensive, m_iShowNonCpuIntensive);
	m_tLastTasksShown = 0;	// direct action
}

void CRemoteCombinedTaskView::OnUpdateTaskShowNonCpuIntensive(CCmdUI *pCmdUI)
{
	// can do on close document
	pCmdUI->SetCheck(m_iShowNonCpuIntensive);
}

void CRemoteCombinedTaskView::OnContextMenu(CWnd* pWnd, CPoint point) 
{ 
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	CRect	rect;
	int		iRetVal;
	CPoint	clientPoint, startPoint;
	bool	bHeader;

	bHeader = false;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	clientPoint = point;
	ScreenToClient(&clientPoint);
	HDHITTESTINFO hdHitIfo;
	memset(&hdHitIfo, 0, sizeof(HDHITTESTINFO));
	for(int i = 0; i < pHeader->GetItemCount(); i++)
	{
		hdHitIfo.pt = clientPoint;
		// The hit test depends on whether the header item is visible.
		iRetVal = pHeader->HitTest(&hdHitIfo);
		if (hdHitIfo.iItem >= 0)
		{
			bHeader = true;
		}
	}

	startPoint = point;
	startPoint.y += 10;

	//our menu
	CMenu contextMenu; 
	//the actual popup menu which is the first
	//sub menu item of IDR_CONTEXT
	CMenu* tracker; 

	if (bHeader)
	{
		contextMenu.LoadMenu(IDR_MENU_CONTEXT_HEADER); 	
		tracker = contextMenu.GetSubMenu(0); 
		// Translate
		tracker->ModifyMenu(ID_HEADER_SETHEADERITEMS,MF_STRING,ID_HEADER_SETHEADERITEMS,gszTranslation[PosPopUpHeaderEnable]);
		tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y , AfxGetMainWnd()); 
		return;
	}

	CListCtrl& listCtrl = GetListCtrl();

	m_contextMenu.SetColumnOrder(NUM_REMOTE_TASK_COLUMNS, &m_iColumnOrder[0]);
	m_contextMenu.ContextMenuCombined(CONTEXT_MENU_TASKS, m_pDoc, &listCtrl, m_pSelection, point, this, IDR_MENU_TASK);
} 

LRESULT CRemoteCombinedTaskView::OnReadyProject(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_contextMenu.Ready((int) parm1);

	return 0;

}

void CRemoteCombinedTaskView::OnTaskSuspend()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	TaskOperation(TASKS_OP_SUSPEND);
}

void CRemoteCombinedTaskView::OnTaskSuspendCheckpoint()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	CListData	*pData;
	int			iPos, iPosHistory, iPosCheck, iNrItems;
	CString		sComputer, sName, sProject;
	bool		bDouble;

	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	if (iNrItems > MAX_WU) iNrItems = MAX_WU;

//	DeleteSelectedTaskItemsCheckpoint();

	for (iPos = 0; iPos < iNrItems; iPos++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl, iPos))
		{
			pData = (CListData *) listCtrl.GetItemData(iPos);

			sComputer = listCtrl.GetItemText(iPos, m_iColumnOrder[COLUMN_COMPUTER]);
			sName = listCtrl.GetItemText(iPos, m_iColumnOrder[COLUMN_NAME]);
			sProject = listCtrl.GetItemText(iPos, m_iColumnOrder[COLUMN_PROJECT]);

			for (iPosHistory = 0; iPosHistory < (int) m_lCheckpointHistory.size(); iPosHistory++)
			{
				if (sName == m_lCheckpointHistory.at(iPosHistory)->m_sName)
				{
					if (sProject == m_lCheckpointHistory.at(iPosHistory)->m_sProject)
					{
						// double??
						bDouble = false; 
						for (iPosCheck = 0; iPosCheck < (int) m_lCheckpointSuspend.size(); iPosCheck++)
						{
							if (m_lCheckpointSuspend.at(iPosCheck)->m_sName == sName)
							{
								if (m_lCheckpointSuspend.at(iPosCheck)->m_sProject == sProject)
								{
									bDouble = true;
									break;
								}
							}
						}

						if (!bDouble)
						{
							CCheckpointSuspend *pCheckpointSuspend;
							pCheckpointSuspend = new CCheckpointSuspend;
							pCheckpointSuspend->m_sComputer = sComputer;
							pCheckpointSuspend->m_sName = sName;
							pCheckpointSuspend->m_sProject = sProject;
							pCheckpointSuspend->m_iCheckpoints = m_lCheckpointHistory.at(iPosHistory)->m_iCheckpoints;
							pCheckpointSuspend->m_i64FilterApplicationAndVersion = pData->m_i64FilterApplicationAndVersion;
							pCheckpointSuspend->m_bUsed = true;
							pCheckpointSuspend->m_bDelete = false;
							m_lCheckpointSuspend.push_back(pCheckpointSuspend);
							break;
						}
					}
				}
			}
		}
	}
}

void CRemoteCombinedTaskView::OnTaskResume()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	TaskOperation(TASKS_OP_RESUME);
}

void CRemoteCombinedTaskView::OnTaskAbort()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	TaskOperation(TASKS_OP_ABORT);
}

void CRemoteCombinedTaskView::TaskOperation(int iOperation, bool bQuestion)
{
	CListData	*pData;
	int			iCount, iNrItems, iThreadId;

	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	// now copy all the wu names to a seperate array, just in case something changes

	if (iNrItems > MAX_WU) iNrItems = MAX_WU;

//	iSelectedTaskItems = 0;

	DeleteSelectedTaskItems();

	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl, iCount))
		{
			pData = (CListData *) listCtrl.GetItemData(iCount);

			CSelectedTaskItem *pSelectedTaskItem;
			pSelectedTaskItem = new CSelectedTaskItem;

			pSelectedTaskItem->m_i64FilterApplicationAndVersion = pData->m_i64FilterApplicationAndVersion;
			pSelectedTaskItem->m_sSelectedTaskItemsComputer = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_COMPUTER]);
			pSelectedTaskItem->m_sSelectedTaskItemsProject = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT]);
			pSelectedTaskItem->m_sSelectedTaskItemsName = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_NAME]);
			pSelectedTaskItem->m_sSelectedTaskItemsPerc = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PERCENTAGE]);
			pSelectedTaskItem->m_sSelectedTaskItemsDeadline = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_DEADLINE]);
			pSelectedTaskItem->m_sSelectedTaskItemsState = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_STATE]);
			m_lSelectedTaskItems.push_back(pSelectedTaskItem);

//			iSelectedTaskItems++;
		}
	}

	if (m_lSelectedTaskItems.size() == 0) return;

	if (iOperation == TASKS_OP_RESUME)
	{
		int iPos, iPosSelected; 
		for (iPos = 0; iPos < (int) m_lCheckpointSuspend.size(); iPos++)
		{
			for (iPosSelected = 0; iPosSelected < (int) m_lSelectedTaskItems.size(); iPosSelected++)
			{
				if (m_lCheckpointSuspend.at(iPos)->m_sName == m_lSelectedTaskItems.at(iPosSelected)->m_sSelectedTaskItemsName)
				{
					if (m_lCheckpointSuspend.at(iPos)->m_sProject == m_lSelectedTaskItems.at(iPosSelected)->m_sSelectedTaskItemsProject)
					{
						delete m_lCheckpointSuspend.at(iPos);
						m_lCheckpointSuspend.erase(m_lCheckpointSuspend.begin()+iPos);
					}
				}
			}
		}
	}


	if (iOperation == TASKS_OP_ABORT)
	{
		if (bQuestion)
		{
			CDlgYesNo dlgYesNo;
			dlgYesNo.bColumnMode = true;
			dlgYesNo.m_sRegistryId = "_abort";
			dlgYesNo.m_sColumn[0]		= gszTranslation[PosDialogTaskAbortColumnTask];			//"Task";
			dlgYesNo.m_iColumnWidth[0]	= 250;
			dlgYesNo.m_sColumn[1]		= gszTranslation[PosDialogTaskAbortColumnProject];		//"Project";
			dlgYesNo.m_iColumnWidth[1]	= 120;
			dlgYesNo.m_sColumn[2]		= "%";
			dlgYesNo.m_iColumnWidth[2]	= 90;
			dlgYesNo.m_sColumn[3]		= gszTranslation[PosDialogTaskAbortColumnDeadline];		//"Deadline";
			dlgYesNo.m_iColumnWidth[3]	= 140;
			dlgYesNo.m_sColumn[4]		= gszTranslation[PosDialogTaskAbortColumnState];		//"State";
			dlgYesNo.m_iColumnWidth[4]	= 120;
			dlgYesNo.m_sColumn[5]		= gszTranslation[PosDialogTaskAbortColumnComputer];		//"Computer";
			dlgYesNo.m_iColumnWidth[5]	= 120;

			for (iCount = 0; iCount < (int) m_lSelectedTaskItems.size(); iCount++)
			{
				CString sTxt1,sTxt2,sTxt3,sTxt4,sTxt5,sTxt6;
			
				sTxt1 =  m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsName;
				if (sTxt1.Find(gszTranslation[PosViewTasksFilterTasks],0) >=0)	//TASKS_STRING
				{
					// first get rid of the hide characters
					RemoveSpecialText(&sTxt1);
				}

				sTxt2 =	 m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsProject;
				sTxt3 =  m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsPerc;
				sTxt3 =  sTxt3.GetBuffer()+3;
				sTxt3.ReleaseBuffer();
				sTxt3+= " %";
				sTxt4 =  m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsDeadline;
				RemoveSpecialText(&sTxt4);
				sTxt5 =  m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsState;
				sTxt6 =  m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsComputer;

				dlgYesNo.AddColumn(sTxt1, sTxt2, sTxt3, sTxt4, sTxt5, sTxt6);
			}

			dlgYesNo.AddColumn("");
			dlgYesNo.AddColumn(gszTranslation[PosDialogTaskAbortQuestionAbort]);			//"Do you want to abort the tasks above?","");

			int iResult = (int) dlgYesNo.DoModal();
			if (iResult != IDYES) return;
		}
	}

	// The thread start timer is in the same thread so no problems there
	CWaitCursor waitCursor;

	for (iCount = 0; iCount < (int) m_lSelectedTaskItems.size(); iCount++)
	{
		bool bFound;
		bFound = m_pDoc->GetRpcThreadId(&m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsComputer, &iThreadId);
		if (bFound)
		{
			if (m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsName.Find(gszTranslation[PosViewTasksFilterTasks],0) < 0)
			{
				char *pcSelectedTasksItemsName;
				CStringCopyNew(&pcSelectedTasksItemsName, m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsName.GetBuffer(), false);
				m_lSelectedTaskItems.at(iCount)->m_sSelectedTaskItemsName.ReleaseBuffer();
				::PostThreadMessage(iThreadId,UWM_MSG_THREAD_TASK_OPERATION,iOperation, (LPARAM) pcSelectedTasksItemsName); //TASKS_STRING
			}
			else
			{	// tasks
				::PostThreadMessage(iThreadId,UWM_MSG_THREAD_TASK_OPERATION_AAV,iOperation, (LPARAM) &m_lSelectedTaskItems.at(iCount)->m_i64FilterApplicationAndVersion);	// moet een pointer zijn, waarde kan groter zijn dan LPARAM.
			}
			Sleep(10);
		}
	}

	m_tLastTasksShown = 0;	// direct action on refresh
}


void CRemoteCombinedTaskView::OnTaskAddRule()
{
	int iItems, iItemsFound;

	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	CListCtrl& listCtrl = GetListCtrl();

	iItems = listCtrl.GetItemCount();
	iItemsFound = 0;

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl,iCount))
		{
			iItemsFound = iCount;
			break;
		}
	}

	m_ruleItemAdd.m_sComputer = listCtrl.GetItemText(iItemsFound,m_iColumnOrder[COLUMN_COMPUTER]);
	m_ruleItemAdd.m_sProject = listCtrl.GetItemText(iItemsFound,m_iColumnOrder[COLUMN_PROJECT]);
	m_ruleItemAdd.m_sApplication = listCtrl.GetItemText(iItemsFound,m_iColumnOrder[COLUMN_APPLICATION]);

	theApp.m_pDlgSettingsRules->PostMessageA(UWM_MSG_RULES_ADD,(WPARAM) &m_ruleItemAdd,0);
}

LRESULT CRemoteCombinedTaskView::OnPreferenceDialog(WPARAM parm1, LPARAM parm2)
{
	CString sComputer;
	int	iThreadId;
	int iItems;

	if (m_pDoc == NULL) return 0;
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();
	sComputer = listCtrl.GetItemText(0,m_iColumnOrder[COLUMN_COMPUTER]);	// nothing get first one
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl,iCount))
		{
			sComputer = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_COMPUTER]);
			break;
		}
	}

	bool bFound = m_pDoc->GetRpcThreadId(&sComputer, &iThreadId);
	if (!bFound)
	{
		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
		if (iThreadId > 0) bFound = true;
	}
	if (bFound)
	{
		// found computer, now tell the Preference Dialog
		m_pDoc->m_pDlgPrefMain->PostMessage(UWM_MSG_PREFERENCE_OPEN_DIALOG, iThreadId, 0);
		return 0;
	}
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnProxyDialog(WPARAM parm1, LPARAM parm2)
{
	CString sComputer;
	int	iThreadId;
	int iItems;

	if (m_pDoc == NULL) return 0;
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();
	sComputer = listCtrl.GetItemText(0,m_iColumnOrder[COLUMN_COMPUTER]);	// nothing get first one
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl,iCount))
		{
			sComputer = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_COMPUTER]);
			break;
		}
	}

	bool bFound = m_pDoc->GetRpcThreadId(&sComputer, &iThreadId);
	if (!bFound)
	{
		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
		if (iThreadId > 0) bFound = true;
	}
	if (bFound)
	{
		::PostMessage(m_hWndProxy,UWM_MSG_PROXY_OPEN_DIALOG, iThreadId, 0);
		return 0;
	}
	return 0;
}

void CRemoteCombinedTaskView::OnWww1() {m_contextMenu.WWW(0);}
void CRemoteCombinedTaskView::OnWww2() {m_contextMenu.WWW(1);}
void CRemoteCombinedTaskView::OnWww3() {m_contextMenu.WWW(2);}
void CRemoteCombinedTaskView::OnWww4() {m_contextMenu.WWW(3);}
void CRemoteCombinedTaskView::OnWww5() {m_contextMenu.WWW(4);}
void CRemoteCombinedTaskView::OnWww6() {m_contextMenu.WWW(5);}
void CRemoteCombinedTaskView::OnWww7() {m_contextMenu.WWW(6);}
void CRemoteCombinedTaskView::OnWww8() {m_contextMenu.WWW(7);}
void CRemoteCombinedTaskView::OnWww9() {m_contextMenu.WWW(8);}
void CRemoteCombinedTaskView::OnWww10() {m_contextMenu.WWW(9);}
void CRemoteCombinedTaskView::OnWww11() {m_contextMenu.WWW(10);}
void CRemoteCombinedTaskView::OnWww12() {m_contextMenu.WWW(11);}
void CRemoteCombinedTaskView::OnWww13() {m_contextMenu.WWW(12);}
void CRemoteCombinedTaskView::OnWww14() {m_contextMenu.WWW(13);}
void CRemoteCombinedTaskView::OnWww15() {m_contextMenu.WWW(14);}
void CRemoteCombinedTaskView::OnWww16() {m_contextMenu.WWW(15);}
void CRemoteCombinedTaskView::OnWww17() {m_contextMenu.WWW(16);}
void CRemoteCombinedTaskView::OnWww18() {m_contextMenu.WWW(17);}
void CRemoteCombinedTaskView::OnWww19() {m_contextMenu.WWW(18);}
void CRemoteCombinedTaskView::OnWww20() {m_contextMenu.WWW(19);}

LRESULT CRemoteCombinedTaskView::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	m_tLastTasksShown = 0;				// 1000 to ensure direct timer message
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnSelTabView(WPARAM wParam, LPARAM lParam)
{
	if (m_pDoc == NULL) return 0;
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->OnShowView((int) wParam, (int) lParam);
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam)
{
	CString *psComputer;
	int		iThreadId, iReconnect;

	psComputer = (CString *) wParam;
	iReconnect = (int) lParam;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (psComputer->GetLength() > 0)
	{
		m_pDoc->GetRpcThreadId(psComputer, &iThreadId);
		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,0);
	}
	else
	{
		int iNrRpc = m_pDoc->GetRpcSelectedComputers();
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);	
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,0);
		}
	}
	if (*psComputer == COMPUTER_DUMMY_SET_TIME)
	{
		int iNrRpc = m_pDoc->GetRpcSelectedComputers();
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);	
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_TIME_RPC,0,iReconnect);
		}
	}

	m_tLastTasksShown = 0;				// 1000 to ensure direct timer message
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnDocumentIsClosing(WPARAM wParam, LPARAM lParam)
{
	int			iThreadId;
	int			iNrThreads;
	HWND		hWnd;

	if (m_pDoc == NULL) return 0;

	m_pDoc->m_pMessageView->m_bDocumentIsClosing = true;
	m_pDoc->m_pProjectView->m_bDocumentIsClosing = true;
	m_pDoc->m_pTransferView->m_bDocumentIsClosing = true;
	m_pDoc->m_pHistoryView->m_bDocumentIsClosing = true;
	m_pDoc->m_pComputerView->m_bDocumentIsClosing = true;
	m_pDoc->m_pNoticesView->m_bDocumentIsClosing = true;

	if (m_pDoc->m_pDlgNotices != NULL) m_pDoc->m_pDlgNotices->m_bDocumentIsClosing = true;

	m_bDocumentIsClosing = true;
	m_iThreadsClosingCount = 0;
	m_iNowClosingDelayCount = 0;

	hWnd = this->m_hWnd;
	iNrThreads = m_pDoc->GetRpcSelectedComputers();
	if (iNrThreads == 0)
	{
		PostMessage(UWM_MSG_THREAD_QUIT_READY,0,0);		// no thread so send the message here
	}

	for (int iCount = 0; iCount < iNrThreads; iCount++)
	{
		m_pDoc->GetRpcThreadId(&iThreadId, iCount);
		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_QUIT, (WPARAM) this->m_hWnd,0);		// doc and view closing, close thread as well
		m_iThreadsClosingCount++;
	}

	if (iNrThreads == 0)
	{
		PostMessage(UWM_MSG_THREAD_QUIT_READY,0,0);		// no thread so send the message here
	}

	for (int iCount = 0; iCount < iNrThreads; iCount++)
	{
		m_pDoc->GetTThrottleRpcThreadId(&iThreadId, iCount);
		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_QUIT, (WPARAM) this->m_hWnd,0);		// doc and view closing, close thread as well
		m_iThreadsClosingCount++;
	}

	m_pLog->m_pFrom = "";
	m_pLog->m_pText = "BoincTasks is closing down all threads, this may take a couple of minutes.";
	m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_NORMAL);

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnThreadQuit(WPARAM wParam, LPARAM lParam)
{
	int iThreadId;
	CString *psId, sFormat;

	iThreadId = (int) wParam;
	psId = (CString *) lParam;

	if (m_pDoc == NULL)
	{
		delete psId;
		return 0;
	}

	m_pDoc->RemoveThreadFromList(iThreadId);

	if (theApp.m_pDlgLogging->m_bLogDebugClosing)
	{
		m_pLog->m_pFrom = "Closing";
		if (psId == NULL) m_pLog->m_pText = "NULL";
		else m_pLog->m_pText = psId->GetBuffer();
		m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_NORMAL);
	}
	delete psId;

	m_iThreadsClosingCount--;
	if (m_iThreadsClosingCount <= 0 || m_iThreadsClosingCount == -2)	// -2 when there no threads at all
	{
		m_pLog->m_pFrom = "";
		m_pLog->m_pText = "BoincTasks has closed down all threads";
		m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_NORMAL);
		theApp.m_pMainFrame->PostMessage(UWM_MSG_DOCUMENT_IS_CLOSING,0,0);// tell the mainfrm that we are closing	
		m_pDoc->NowCloseDocument();	//The frame is handling it now
		return 0;
	}

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnFloaterData(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_pDoc->m_pHistoryView != NULL)
	{
		m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_GET_FLOATER_DATA, wParam, lParam);
	}

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam)
{
	int ThreadRpcIdLocalHost;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	ThreadRpcIdLocalHost = (int) wParam;
	if (ThreadRpcIdLocalHost > 0)
	{
		::PostThreadMessage(ThreadRpcIdLocalHost,UWM_MSG_THREAD_SET_SNOOZE_CPU, (WPARAM) this->m_hWnd, lParam);
	}

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnThreadSnoozeCpuReady(WPARAM wParam, LPARAM lParam)
{
	CRpcThreadReturn	*pRpcThreadReturn;
	CC_STATUS			*pCcStatus;

	pRpcThreadReturn = (CRpcThreadReturn *) wParam;
	pCcStatus = (CC_STATUS *) lParam;

	if (m_bDocumentIsClosing)
	{
		delete pRpcThreadReturn;
		return 0;			// everything is closing down.
	}

	if (strcmp(pRpcThreadReturn->m_pcHostName, LOCALHOST_NAME) == 0)
	{
		theApp.m_pMainFrame->SendMessage(UWM_MSG_SET_SNOOZE_CPU_READY, wParam, lParam);
	}

	if (m_pDoc != NULL)
	{
//		m_pDoc->UpdateAllowToRunCpu(pRpcThreadReturn->m_pcComputerId, pCcStatus);
//		m_pDoc->SetStatus(pRpcThreadReturn->m_pThread, pRpcThreadReturn->m_iStatus, pCcStatus->network_mode);
	}

	delete pRpcThreadReturn;

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam)
{
	int ThreadRpcIdLocalHost;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	ThreadRpcIdLocalHost = (int) wParam;
	if (ThreadRpcIdLocalHost > 0)
	{
		::PostThreadMessage(ThreadRpcIdLocalHost,UWM_MSG_THREAD_SET_SNOOZE_GPU, (WPARAM) this->m_hWnd, lParam);
	}

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnThreadSnoozeGpuReady(WPARAM wParam, LPARAM lParam)
{
	CRpcThreadReturn *pRpcThreadReturn;
	pRpcThreadReturn = (CRpcThreadReturn *) wParam;

	if (m_bDocumentIsClosing)
	{
		delete pRpcThreadReturn;
		return 0;			// everything is closing down.
	}


	if (strcmp(pRpcThreadReturn->m_pcHostName, LOCALHOST_NAME) == 0)
	{
		theApp.m_pMainFrame->SendMessage(UWM_MSG_SET_SNOOZE_GPU_READY, wParam, lParam);
	}

	if (m_pDoc != NULL)
	{
//		m_pDoc->UpdateAllowToRunGpu(pRpcThreadReturn->m_pcComputerId, (CC_STATUS *) lParam);
	}

	delete pRpcThreadReturn;
	return 0;
}



LRESULT CRemoteCombinedTaskView::OnSettingsTasksSortingStatus(WPARAM wParam, LPARAM lParam)
{
	int	iMin;
	std::deque<int> *plToSend;

	iMin = (int) wParam;
	plToSend = (std::deque<int> *) lParam;

	m_lListStatusSorting.clear();
	m_iListStatusSorting = iMin;
	for (int iCount = 0; iCount < (int) plToSend->size(); iCount++)
	{
		m_lListStatusSorting.push_back(plToSend->at(iCount));
	}
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam)
{
	int		iColumnSet, iCheck, iLen, iLenMax;
	CString	sTxt;

	iColumnSet = (int) wParam;
	iCheck = (int) lParam;

	CListCtrl& listCtrl = GetListCtrl();

	if (iCheck)
	{
		int iItems = listCtrl.GetItemCount();
		iLenMax = 0;
		for (int iCount = 0; iCount < iItems; iCount++)
		{
			sTxt = listCtrl.GetItemText(iCount, m_iColumnOrder[iColumnSet]);
			RemoveSpecialText(&sTxt);
			iLen = listCtrl.GetStringWidth(sTxt);
			if (iLen > iLenMax) iLenMax = iLen;
		}
		if (iLenMax == 0) iLenMax = 90;

		if (m_iColumnOrder[iColumnSet] == 0) iLenMax += 14;
		else iLenMax +=12;	// margins

		listCtrl.SetColumnWidth(m_iColumnOrder[iColumnSet],iLenMax);
	}
	else
	{
		listCtrl.SetColumnWidth(m_iColumnOrder[iColumnSet],0);
	}

	Invalidate(TRUE);
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	return m_pDoc->m_pProjectView->PostMessage(UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, wParam, lParam);
}

LRESULT CRemoteCombinedTaskView::OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, wParam, lParam);
	return 0;
}

void CRemoteCombinedTaskView::OnTaskProjectupdate()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_UPDATE);
}

void CRemoteCombinedTaskView::OnTaskProjectsuspend()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_SUSPEND);
}

void CRemoteCombinedTaskView::OnTaskProjectresume()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_RESUME);
}

void CRemoteCombinedTaskView::PostProjectOperation(int iOperation)
{
	CString sProjects, sProjectSpace, sProjectSelected, sComputerSelected;
	bool	bSelected;
	int iCount, iCountDouble, iNrItems;
	

	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	// now copy all the project names to a seperate array, just in case something changes

	if (iNrItems > MAX_PROJECTS) iNrItems = MAX_PROJECTS;

	DeleteSelectedProjectItems();

	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		bSelected =m_pSelection->SelectionSelected(&listCtrl, iCount);
		if (bSelected)
		{
			sProjectSelected = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT]);
			sComputerSelected = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_COMPUTER]);

			// check if the entry is double

			bool	bDouble = false;
			for (iCountDouble = 0; iCountDouble < (int) m_lSelectedProjectItems.size(); iCountDouble++)
			{
				if (m_lSelectedProjectItems.at(iCountDouble)->m_sSelectedProjectItemsComputer == sComputerSelected)
				{
					if (m_lSelectedProjectItems.at(iCountDouble)->m_sSelectedProjectItemsName == sProjectSelected)
					{
						bDouble = true;
					}
				}
			}
			if (!bDouble)
			{
				CSelectedTaskItem *pSelectedTaskItem;
				pSelectedTaskItem = new CSelectedTaskItem;

				pSelectedTaskItem->m_sSelectedProjectItemsComputer = sComputerSelected;
				pSelectedTaskItem->m_sSelectedProjectItemsName = sProjectSelected;
				m_lSelectedProjectItems.push_back(pSelectedTaskItem);
			}
		}
	}

	if (m_lSelectedProjectItems.size() == 0)	return;

	for (iCount = 0; iCount < (int) m_lSelectedProjectItems.size(); iCount++)
	{
		bool bFound;
		int  iThreadId;
		bFound = m_pDoc->GetRpcThreadId(&m_lSelectedProjectItems.at(iCount)->m_sSelectedProjectItemsComputer, &iThreadId);
		if (bFound)
		{
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_PROJECT,(WPARAM) NULL,0); // Only send an update projects, no reply
			char *pSelectedProjectItemsName;
			CStringCopyNew(&pSelectedProjectItemsName, m_lSelectedProjectItems.at(iCount)->m_sSelectedProjectItemsName.GetBuffer(), false);
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_PROJECT_OPERATION_BYNAME, iOperation, (LPARAM) pSelectedProjectItemsName);
			m_lSelectedProjectItems.at(iCount)->m_sSelectedProjectItemsName.ReleaseBuffer();
		}
		Sleep(10);
	}	

	DeleteSelectedProjectItems();

	if (m_pDoc->m_pHistoryView != NULL)
	{
		m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_HISTORY_SOMETHING_HAPPENED,0,0);
	}

	m_tLastTasksShown = 0;	// direct action on refresh
}

BOOL CRemoteCombinedTaskView::TasksProperties(int iBusy)
{
	CThreadRpc  *pThreadRpc;
	CString sTxt, sProjectProperties, sComputerId, sProjectName, sName;
	CTasksProperties *pTasksProperties;
	CListCtrl& listCtrl = GetListCtrl();
	int			nItems;//, iTimeOut;
	bool		bFirstTime = true; //, bOk;

	if (m_pDoc == NULL) return FALSE;
	if (m_bDocumentIsClosing) 	return FALSE;			// everything is closing down.

	if (m_iTaskPropertiesBusy != PROPERTIES_READY) return FALSE;

	nItems = m_pSelection->SelectionNumber();
	if (nItems == 0) return FALSE;

	DeletePropertiesList();

	nItems = listCtrl.GetItemCount();
	for (int iCount = 0; iCount < nItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl, iCount))
		{
			sComputerId = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_COMPUTER]);
			bool bFound = m_pDoc->GetRpcThreadPointer(&sComputerId, &pThreadRpc);		
			if (bFound)
			{
				pTasksProperties = new CTasksProperties;
				pTasksProperties->m_sComputer = sComputerId;
				pTasksProperties->m_sProject = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_PROJECT]);
				sName = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_NAME]);
				pTasksProperties->AddName(sName.GetBuffer());sName.ReleaseBuffer();
				pTasksProperties->m_iThreadId = pThreadRpc->m_nThreadID;
				m_lListTasksProperties.push_back(pTasksProperties);
			}
		}
	}

	TaskPropertiesGet(iBusy);

	return TRUE;
}

void CRemoteCombinedTaskView::OnTasksProperties()
{
	TasksProperties(PROPERTIES_DIALOG);
}

void CRemoteCombinedTaskView::OnShowGraphic()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

//	CListData	*pData;
	int			iCount, iNrItems, iThreadId;

	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	if (iNrItems > MAX_WU) iNrItems = MAX_WU;

	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl, iCount))
		{
//			pData = (CListData *) listCtrl.GetItemData(iCount);
//			char *pcTask, *pcProject;
			CString sComputer, sProject, sName;
			sComputer = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_COMPUTER]);
			if (m_pDoc->GetRpcThreadId(&sComputer, &iThreadId))
			{
				sProject = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT]);
				sName = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_NAME]);

//				CStringCopyNew(&pcProject, sProject.GetBuffer(), false); sProject.ReleaseBuffer();
//				CStringCopyNew(&pcTask, sName.GetBuffer() , false); sName.ReleaseBuffer();

				CRpcGraphic *pRpcGraphic = new CRpcGraphic;
				pRpcGraphic->m_iSlot = -1;
				pRpcGraphic->m_sProject = sProject;
				pRpcGraphic->m_sName = sName;

				::PostThreadMessage(iThreadId,UWM_MSG_THREAD_TASK_GRAPHIC, (WPARAM) this->m_hWnd, (LPARAM) pRpcGraphic); 
			}
		}
	}
}

LRESULT CRemoteCombinedTaskView::OnShowGraphicReady(WPARAM wParam,LPARAM lParam)
{
	CRpcGraphic		*pRpcGraphic;
	CString			sCmd, sDataDir;
	pRpcGraphic = (CRpcGraphic *) lParam;

	if (m_bDocumentIsClosing)
	{
		delete	pRpcGraphic;
		return 0;			// everything is closing down.
	}

	if (pRpcGraphic->m_iSlot < 0)
	{
		delete	pRpcGraphic;
		return 0;
	}

	for (int iPos = 0; iPos < (int) m_lRunningGraphic.size(); iPos++)
	{
		if (pRpcGraphic->m_sProject == m_lRunningGraphic.at(iPos)->m_sProject)
		{
			if (pRpcGraphic->m_sName == m_lRunningGraphic.at(iPos)->m_sName)
			{
				// check if still running;

				CBoincClientManager boincClientManager;
				if (boincClientManager.CheckProcess(m_lRunningGraphic.at(iPos)->m_dwPID))
				{
					// already running
					delete	pRpcGraphic;
					return 0;
				}
				else
				{
					m_lRunningGraphic.erase(m_lRunningGraphic.begin() + iPos);	// no longer running remove it.
				}
			}
		}
	}

	PROCESS_INFORMATION pi;
	STARTUPINFO         si;
	BOOL                bProcessStarted;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	sDataDir		= pRpcGraphic->m_sSlotPath;

	bProcessStarted = CreateProcess(
		pRpcGraphic->m_sGraphicsExecPath,
		sCmd.GetBuffer(),
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
		pRpcGraphic->m_dwPID = pi.dwProcessId;
		m_lRunningGraphic.push_back(pRpcGraphic);
	}
	return 0;
}

void CRemoteCombinedTaskView::OnToolbarSuspend()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	TaskOperation(TASKS_OP_SUSPEND);
}
void CRemoteCombinedTaskView::OnUpdateToolbarSuspend(CCmdUI* pCmdUI)
{
	ToolbarUpdate(pCmdUI);
}

void CRemoteCombinedTaskView::OnToolbarResume()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	TaskOperation(TASKS_OP_RESUME);
}

void CRemoteCombinedTaskView::OnUpdateToolbarResume(CCmdUI* pCmdUI)
{
	ToolbarUpdate(pCmdUI);
}

void CRemoteCombinedTaskView::OnToolbarAbort()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	TaskOperation(TASKS_OP_ABORT);
}

void CRemoteCombinedTaskView::OnUpdateToolbarAbort(CCmdUI* pCmdUI)
{
	ToolbarUpdate(pCmdUI);
}


void CRemoteCombinedTaskView::ToolbarUpdate(CCmdUI* pCmdUI)
{
	BOOL bSelect = FALSE;
	int	 iNrItems;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();
	if (iNrItems > 0)
	{
		if (m_pSelection->SelectionNumber() > 0) bSelect = TRUE;
	}
	pCmdUI->Enable(bSelect);
}

void CRemoteCombinedTaskView::OnToolbarProjectUpdate()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_UPDATE);
}

void CRemoteCombinedTaskView::OnUpdateToolbarProjectUpdate(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedTaskView::OnToolbarProjectSuspend()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_SUSPEND);
}

void CRemoteCombinedTaskView::OnUpdateToolbarProjectSuspend(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedTaskView::OnToolbarProjectResume()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_RESUME);
}

void CRemoteCombinedTaskView::OnUpdateToolbarProjectResume(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedTaskView::OnToolbarProjectNoNew()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_NOMOREWORK);
}

void CRemoteCombinedTaskView::OnUpdateToolbarProjectNoNew(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedTaskView::OnToolbarProjectAllowNew()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_ALLOWMOREWORK);
}

void CRemoteCombinedTaskView::OnUpdateToolbarProjectAllowNew(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}


void CRemoteCombinedTaskView::ToolbarUpdateProject(CCmdUI* pCmdUI)
{
	BOOL bSelect = FALSE;
	int	 iNrItems;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();
	if (iNrItems > 0)
	{	
		if (m_pSelection->SelectionNumber() > 0) bSelect = TRUE;
	}
	pCmdUI->Enable(bSelect);
}

LRESULT CRemoteCombinedTaskView::OnSearchComputerReady(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pComputerView->SendMessage(UWM_DLG_SEARCH_COMPUTER_READY,parm1,parm2);
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnFontChangeAll(WPARAM wParam,LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->m_pComputerView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pProjectView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTransferView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pMessageView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	if (m_pDoc->m_pHistoryView != NULL)
	{
		m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	}

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnFontHasChanged(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLf;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	pLf = (LOGFONT *) wParam;

	delete m_pFont;

	m_pFont = NULL;

	m_pFont = new CFont;

	m_pFont->CreateFontIndirect(pLf);

	SetFont(m_pFont,TRUE);
	return 0;
}

void CRemoteCombinedTaskView::OnHeaderSetItems()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	theApp.m_pDlgSettingsMain->SetActivePage(theApp.m_pDlgSettingsTasks);
	theApp.m_pDlgSettingsMain->ShowWindow(SW_SHOW);
}

void CRemoteCombinedTaskView::OnCopyalltoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	ClipBoard(true);
}

void CRemoteCombinedTaskView::OnCopyselectedtoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	ClipBoard(false);
}

LRESULT CRemoteCombinedTaskView::OnNoticesNew(WPARAM wParam,LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->NoticesNew((CString *) lParam);

	return 0;
}

LRESULT CRemoteCombinedTaskView::OnWebItemSelected(WPARAM wParam,LPARAM lParam)
{
	int	iRow, iOption;
	CHtmlExchange *pHtmlExchange;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	pHtmlExchange = (CHtmlExchange *) lParam;
	iRow = pHtmlExchange->m_iRow;
	iOption = pHtmlExchange->m_iOption;

	if (iOption == WEBSERVER_SELECT_FILTER)
	{
		if (m_pSelection->SelectionNumber() > 0)
		{
			CListCtrl& listCtrl = GetListCtrl();
			iRow = m_pSelection->SelectionGetLast(&listCtrl);
			DoubleClick(iRow, 0);
		}
	}
	else
	{
		m_pSelection->SelectionSet(iRow, iOption);
	}

	if (pHtmlExchange->m_bSendData)
	{
		if (!m_bThreadBusy)
		{
			CListCtrl& listCtrl = GetListCtrl();
			CCreateHtmlWebServer createHtml;
			createHtml.Create(PosBarViewSelectTasks, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TASK_COLUMNS, &m_sCloudColumn, PosGroupViewTasks, &pHtmlExchange->m_pcHtml);
			pHtmlExchange->tTime = GetTickCount()/100;
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CRemoteCombinedTaskView::OnWebOperation(WPARAM parm1, LPARAM parm2)
{
	int iOperation;

	iOperation = (int) parm1;
	TaskOperation(iOperation, false);
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnWebFilter(WPARAM parm1, LPARAM parm2)
{
	int iSet;
	int iFilterId, iFilter;
	char *pcFilter, *pcFound;
	iSet	= (int) parm1;

	if (iSet)
	{
		pcFilter = (char *) parm2;
		iFilterId = atoi(pcFilter);
		pcFound = (char *) strstr(pcFilter, ",");
		if (!pcFound) return 0;
		pcFound++;
		iFilter = atoi(pcFound);
		if (iFilter == FALSE) iFilter = TRUE;
		else iFilter = FALSE;

		switch(iFilterId)
		{
			case FILTER_BIT_POS_READY_TO_REPORT:
				m_iFilterEnabledReadyToReport = iFilter;
			break;
			case FILTER_BIT_POS_WAITING_TO_RUN:
				m_iFilterEnabledWaitingToRun = iFilter;
			break;
			case FILTER_BIT_POS_READY_TO_START:
				m_iFilterEnabledReadyToStart = iFilter;
			break;
			case FILTER_BIT_POS_DOWNLOAD:
				m_iFilterEnabledDownload = iFilter;
			break;
			case FILTER_BIT_POS_UPLOAD:
				m_iFilterEnabledUpload = iFilter;
			break;
			case FILTER_BIT_POS_SUSPEND:
				m_iFilterEnabledSuspend = iFilter;
			break;
			case FILTER_BIT_POS_COMPUTATION:
				m_iFilterEnabledComputation = iFilter;
			break;
			case FILTER_BIT_POS_ABORTED:
				m_iFilterEnabledAborted = iFilter;
			break;
			case FILTER_BIT_POS_RUNNING:
				m_iFilterEnabledRunning = iFilter;
			break;
			case FILTER_BIT_POS_SHOW_RUNNING_TASKS:
				m_iShowActiveTasks = iFilter;
			break;
		}
	}
	else
	{
		int iFilterId = (int) parm2;
		switch(iFilterId)
		{
			case FILTER_BIT_POS_READY_TO_REPORT:
				return m_iFilterEnabledReadyToReport;
			break;
			case FILTER_BIT_POS_WAITING_TO_RUN:
				return m_iFilterEnabledWaitingToRun;
			break;
			case FILTER_BIT_POS_READY_TO_START:
				return m_iFilterEnabledReadyToStart;
			break;
			case FILTER_BIT_POS_DOWNLOAD:
				return m_iFilterEnabledDownload;
			break;
			case FILTER_BIT_POS_UPLOAD:
				return m_iFilterEnabledUpload;
			break;
			case FILTER_BIT_POS_SUSPEND:
				return m_iFilterEnabledSuspend;
			break;
			case FILTER_BIT_POS_COMPUTATION:
				return m_iFilterEnabledComputation;
			break;
			case FILTER_BIT_POS_ABORTED:
				return m_iFilterEnabledAborted;
			break;
			case FILTER_BIT_POS_RUNNING:
				return m_iFilterEnabledRunning;
			break;
			case FILTER_BIT_POS_SHOW_RUNNING_TASKS:
				return m_iShowActiveTasks;
			break;
		}
	}
	return 0;
}

LRESULT CRemoteCombinedTaskView::OnWebSort(WPARAM parm1, LPARAM parm2)
{
	char	*pcSort;
	BOOL	bHtml;
	char	*pcHtml = NULL;

	pcSort = (char *) parm2;
	bHtml = (BOOL) parm1;

	m_pSortColumn->WebSorting(pcSort);

	if (bHtml == FALSE) return NULL;

	if (!m_bThreadBusy)
	{
		CListCtrl& listCtrl = GetListCtrl();
		Sort(&listCtrl);
		CCreateHtmlWebServer createHtml;
		createHtml.Create(PosBarViewSelectTasks, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TASK_COLUMNS, &m_sCloudColumn, PosGroupViewTasks, &pcHtml);
		m_tLastTasksShown = 0;				// 1000 to ensure direct timer message
	}
	return (LRESULT) pcHtml;
}

LRESULT CRemoteCombinedTaskView::OnWebProperties(WPARAM parm1, LPARAM parm2)
{
	BOOL bResult; 
	bResult = TasksProperties(PROPERTIES_MOBILE);
	return bResult;
}

void CRemoteCombinedTaskView::OnComputersDetect()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
//	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_OPEN,0,0);	
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SELECTED,BAR_SELECTED_COMPUTERS,0);

	m_pDoc->m_pComputerView->PostMessage(WM_COMMAND, ID_COMPUTERS_DETECT,0);
}
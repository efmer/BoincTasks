// ComputerRemoteView.cpppColorsTasks : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"   // base class for BoinCComputerView
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "TemplateRemoteTaskView.h" 
#include "RemoteCombinedDoc.h"
#include "threadrpc.h"
#include "DlgColorTasks.h"
#include "DlgColorWarnings.h"
#include "DlgLogging.h"
#include "DlgSettingsView.h"
#include "DlgSettingsWarning.h"
#include "DlgSettingsTasks.h"
#include "DlgProperties.h"
#include "SortColumn.h"
#include "Selection.h"
#include "Translation.h"
#include "DateConvert.h"
#include "TimeString.h"
#include "CondenseResource.h"
#include "UseCpuGpu.h"
#include "Xml.h"
#include "NumberFormat.h"
#include "StringCopyNew.h"
#include "CreateHtmlWebServerProperties.h"

static int _gnRemoteTaskColumnWidth[NUM_REMOTE_TASK_COLUMNS] =
{
	120,			200,			200,			90				,80				,80				,90				,120			,80			,150		,80				,80				,100		, 80,	80, 80, 80, 80, 80, 80
};


IMPLEMENT_DYNCREATE(CTemplateRemoteTaskView, CListViewEx)

CTemplateRemoteTaskView::CTemplateRemoteTaskView()
{
	m_pLog = new CLoggingMessage;

//	for (int iCount = 0; iCount < MAX_TASK_DATA; iCount++)
//	{
//		m_pData[iCount] = NULL;				// clear data pointer table
//	}

	this->m_bSelection = FALSE;	// disable default selection

	m_bDraged = false;

	m_bCombinedMode	= true;
	m_bApplyChangesOnColumn = false;
	m_bRatioLongTerm = false;

	m_iTaskPropertiesBusy = PROPERTIES_READY;

	m_dataDummy.m_color = RGB(255,255,255);
	m_dataDummy.m_cSortPrimary = "";
	m_dataDummy.m_cSortSecondary = "";
	m_dataDummy.m_cSortTertiary = "";

	m_pSortColumn = new CSortColumn;
	m_pSelection = new CSelection;

	m_pLoggingDialog = theApp.m_pDlgLogging;

	m_tLastSwitchedTime = 0;
	m_tLastTasksShown = 0;
	m_tLastTasksWWW = 0;
	m_tLastHostInfo = 0;
	m_tLastAccountManagerInfo = 0;
	m_tLastSnoozeInfo = 0;
	m_tLastMessageUpdate = 0;

	m_bTasksWWW = false;

	m_iFilterEnabledWaitingToRun = FALSE;
	m_iFilterEnabledReadyToReport = FALSE;
	m_iFilterEnabledReadyToStart = FALSE;
	m_iFilterEnabledUpload = FALSE;
	m_iFilterEnabledDownload = FALSE;
	m_iFilterEnabledSuspend = FALSE;
	m_iFilterEnabledAborted = FALSE;
	m_iFilterEnabledRunning = FALSE;
	m_iFilterEnabledComputation = FALSE;

	m_iFilterEnabledWaitingToRun = theApp.GetProfileInt(registrySectionFilter, registryFilterWaitingToRun, 1);
	if (m_iFilterEnabledWaitingToRun == FILTER_INACTIVE) m_iFilterEnabledWaitingToRun = FILTER_INACTIVE;
	else m_iFilterEnabledWaitingToRun = FILTER_ACTIVE;

	m_iFilterEnabledReadyToReport = theApp.GetProfileInt(registrySectionFilter, registryFilterReadyToReport, 1);
	if (m_iFilterEnabledReadyToReport == FILTER_INACTIVE) m_iFilterEnabledReadyToReport = FILTER_INACTIVE;
	else m_iFilterEnabledReadyToReport = FILTER_ACTIVE;

	m_iFilterEnabledReadyToStart = theApp.GetProfileInt(registrySectionFilter, registryFilterReadyToStart, 1);
	if (m_iFilterEnabledReadyToStart == FILTER_INACTIVE) m_iFilterEnabledReadyToStart = FILTER_INACTIVE;
	else m_iFilterEnabledReadyToStart = FILTER_ACTIVE;

	m_iFilterEnabledDownload = theApp.GetProfileInt(registrySectionFilter, registryFilterDownload, 1);
	if (m_iFilterEnabledDownload == FILTER_INACTIVE) m_iFilterEnabledDownload = FILTER_INACTIVE;
	else m_iFilterEnabledDownload = FILTER_ACTIVE;

	m_iFilterEnabledUpload = theApp.GetProfileInt(registrySectionFilter, registryFilterUpload, 1);
	if (m_iFilterEnabledUpload == FILTER_INACTIVE) m_iFilterEnabledUpload = FILTER_INACTIVE;
	else m_iFilterEnabledUpload = FILTER_ACTIVE;

	m_iFilterEnabledSuspend = theApp.GetProfileInt(registrySectionFilter, registryFilterSuspend, 1);
	if (m_iFilterEnabledSuspend == FILTER_INACTIVE) m_iFilterEnabledSuspend = FILTER_INACTIVE;
	else m_iFilterEnabledSuspend = FILTER_ACTIVE;

	m_iFilterEnabledAborted = theApp.GetProfileInt(registrySectionFilter, registryFilterAborted, 1);
	if (m_iFilterEnabledAborted == FILTER_INACTIVE) m_iFilterEnabledAborted = FILTER_INACTIVE;
	else m_iFilterEnabledAborted = FILTER_ACTIVE;

	m_iFilterEnabledRunning = theApp.GetProfileInt(registrySectionFilter, registryFilterRunning, 0);
	if (m_iFilterEnabledRunning == FILTER_INACTIVE) m_iFilterEnabledRunning = FILTER_INACTIVE;
	else m_iFilterEnabledRunning = FILTER_ACTIVE;

	m_iFilterEnabledComputation = theApp.GetProfileInt(registrySectionFilter, registryFilterComputation, 1);
	if (m_iFilterEnabledComputation == FILTER_INACTIVE) m_iFilterEnabledComputation = FILTER_INACTIVE;
	else m_iFilterEnabledComputation = FILTER_ACTIVE;

	m_iShowActiveTasks = theApp.GetProfileInt(registrySectionFilter, registryShowActiceTasks, 0);
	if (m_iShowActiveTasks == FILTER_INACTIVE) m_iShowActiveTasks = FILTER_INACTIVE;
	else m_iShowActiveTasks = FILTER_ACTIVE;

	m_iShowNonCpuIntensive = theApp.GetProfileInt(registrySectionFilter, registryShowNonCpuIntensive, FILTER_ACTIVE);
	if (m_iShowNonCpuIntensive == FILTER_INACTIVE) m_iShowNonCpuIntensive = FILTER_INACTIVE;
	else m_iShowNonCpuIntensive = FILTER_ACTIVE;

	m_iShowCpu = theApp.GetProfileInt(registrySectionFilter, registryShowCpu, FILTER_ACTIVE);
	if (m_iShowCpu == FILTER_INACTIVE) m_iShowCpu = FILTER_INACTIVE;
	else m_iShowCpu = FILTER_ACTIVE;

	m_iShowGpu = theApp.GetProfileInt(registrySectionFilter, registryShowGpu, FILTER_ACTIVE);
	if (m_iShowGpu == FILTER_INACTIVE) m_iShowGpu = FILTER_INACTIVE;
	else m_iShowGpu = FILTER_ACTIVE;

	if ((m_iShowCpu == FILTER_INACTIVE) && (m_iShowGpu == FILTER_INACTIVE))
	{
		m_iShowCpu = FILTER_ACTIVE;
		m_iShowGpu = FILTER_ACTIVE;
	}

	m_iNrColomns = 0;

	CRpcInfoTasksItem *pRpcInfoTasksItem;
	CRpcInfoClient *pRpcInfoClient;

	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pRpcInfoTasksItem = new CRpcInfoTasksItem;
		pRpcInfoClient = new CRpcInfoClient;

		pRpcInfoClient->m_bReady = false;
		pRpcInfoClient->m_bReadyCopy = false;
		pRpcInfoClient->m_iLostCount = 0;
		pRpcInfoClient->m_bConnectionLogged = false;
		pRpcInfoClient->m_sComputerId = "";

		pRpcInfoTasksItem->m_iStatusRpc = RPC_STATUS_NONE;
		pRpcInfoTasksItem->m_pcComputerId = NULL;

		m_lRpcInfo.push_back(pRpcInfoTasksItem);
		m_lRpcInfoClient.push_back(pRpcInfoClient);
	}

	ReadXml();
	m_iDragIndex = -1;
}

CTemplateRemoteTaskView::~CTemplateRemoteTaskView()
{
	int			iPos;
	CRatioItem *pRatioItem;

	delete m_pLog;

	for (int iCount = 0; iCount < (int) m_lListData.size(); iCount++)
	{
		delete m_lListData.at(iCount);
	}
//	m_lListData.clear();

	for (iPos = 0; iPos < (int) m_lRatio.size(); iPos++)
	{
		pRatioItem = m_lRatio.at(iPos);
		delete pRatioItem;
	}
//	m_lRatio.clear();

	for (iPos = 0; iPos < (int) m_lCheckpoint.size(); iPos++)
	{
		delete m_lCheckpoint.at(iPos);
	}
//	m_lCheckpoint.clear();

	for (iPos = 0; iPos < (int) m_lCheckpointHistory.size(); iPos++)
	{
		delete m_lCheckpointHistory.at(iPos);
	}
//	m_lCheckpointHistory.clear();

	for (iPos = 0; iPos < (int) m_lCheckpointSuspend.size(); iPos++)
	{
		delete m_lCheckpointSuspend.at(iPos);
	}

	for (int iCount = 0; iCount < (int) m_lRpcInfo.size(); iCount++)
	{
		// NEVER do this here, as it is made in RemoteCombinedDoc delete m_pRpcInfoClient[iCount]->m_pcComputerId; this is a copy only

		delete m_lRpcInfo.at(iCount);
	}
//	m_lRpcInfo.clear();

	for (int iCount = 0; iCount < (int) m_lRpcInfoClient.size(); iCount++)
	{
		// NEVER do this here, as it is made in RemoteCombinedDoc delete m_pRpcInfoClient[iCount]->m_pcComputerId; this is a copy only
		delete m_lRpcInfoClient.at(iCount);
	}
//	m_lRpcInfoClient.clear();


	delete m_pSortColumn;
	delete m_pSelection;

	DeleteSelectedTaskItems();
	DeletePropertiesList();
//	DeleteSelectedGraphicItems();
}

void CTemplateRemoteTaskView::OnInitialUpdate()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetExtendedStyle (listCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP);//|LVS_EX_DOUBLEBUFFER);
	AddAllColumns();

	CListViewEx::OnInitialUpdate();
}

void CTemplateRemoteTaskView::DeleteSelectedTaskItems()
{
	int iSize, iPos;

	iSize = (int) m_lSelectedTaskItems.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		delete m_lSelectedTaskItems.at(iPos);
	}
	m_lSelectedTaskItems.clear();
}

void CTemplateRemoteTaskView::DeleteSelectedProjectItems()
{
	int iSize, iPos;

	iSize = (int) m_lSelectedProjectItems.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		delete m_lSelectedProjectItems.at(iPos);
	}
	m_lSelectedProjectItems.clear();
}

void CTemplateRemoteTaskView::AddAllColumns(bool bAdd)
{
	int			iCount, iColumn, iWidth, iFormat, iNr;
	int			iColumnHeader[NUM_REMOTE_TASK_COLUMNS+1];
	LV_COLUMN	lvc;
//	CListData	**pData;

//	m_pListCtrl->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// read back the column sequence

	if (bAdd)
	{
		m_iNrColomns = (int) ::SendMessage(theApp.m_pDlgSettingsTasks->m_hWnd, UWM_MSG_DLG_SETTINGS_TASKS_GET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}
	else
	{	
		for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
		{
			iColumnHeader[iCount] = -1;
		}

		for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
		{
			iNr = m_iColumnOrder[iCount];
			if (iNr > NUM_REMOTE_TASK_COLUMNS) break;
			if (iNr >=0) iColumnHeader[iNr] = iCount;
		}
		::SendMessage(theApp.m_pDlgSettingsTasks->m_hWnd, UWM_MSG_DLG_SETTINGS_TASKS_SET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}

	CListCtrl& listCtrl = GetListCtrl();
	// build columns
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(iCount = 0; iCount<m_iNrColomns; iCount++)
	{
		iColumn = iColumnHeader[iCount];
		if (iColumn >= 0)
		{
			lvc.iSubItem = iCount;
			lvc.pszText = gszTranslation[PosViewTasksRowProject+iColumn];
			iWidth = ReadColumnWidth(iColumn, _gnRemoteTaskColumnWidth[iColumn]);
			m_iColumnWidth[iColumn] = iWidth;
			lvc.cx = m_iColumnWidth[iColumn];
			::SendMessage(theApp.m_pDlgSettingsTasks->m_hWnd, UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, (WPARAM) iCount,(LPARAM) iWidth);

			switch(iColumn)
			{
				case COLUMN_ELAPSED:
				case COLUMN_TIME_LEFT:
				case COLUMN_DEADLINE:
				case COLUMN_CHECKPOINT:
				case COLUMN_DEBT:
				case COLUMN_RECEIVED:
				case COLUMN_VIRTUAL_MEMORY:
				case COLUMN_MEMORY:
					iFormat = LVCFMT_RIGHT;
				break;
				case COLUMN_CPU_PERCENTAGE:
				case COLUMN_PERCENTAGE:
				case COLUMN_TTHROTTLE:
				case COLUMN_TEMPERATURE:
				case COLUMN_SHARE:
					iFormat = LVCFMT_CENTER;
				break;

				default:
					iFormat = LVCFMT_LEFT;	// COLUMN_PROJECT COLUMN_APPLICATION COLUMN_NAME COLUMN_USE COLUMN_STATE COLUMN_COMPUTER

			}

			lvc.fmt = iFormat;
			if (bAdd)
			{
				listCtrl.InsertColumn(iCount,&lvc);
			}
			else
			{
				listCtrl.SetColumn(iCount, &lvc);
			}

		}
	}
	LPINT pnOrder = (LPINT) malloc(m_iNrColomns*sizeof(int));
	if (pnOrder != NULL)
	{
		for (iCount = 0; iCount < m_iNrColomns; iCount++)
		{
			pnOrder[iCount] = iCount; 
		}
		listCtrl.SetColumnOrderArray(m_iNrColomns,pnOrder);
		free(pnOrder);
	}

	m_pSortColumn->SetColumnOrder(m_iNrColomns, &iColumnHeader[0]);
	m_pSortColumn->Setup(&listCtrl,this->m_hWnd, m_iNrColomns, registrySortTasksPrimary, registrySortTasksSecondary, registrySortTasksTertiary, 517, 265, 265); // perc - computer - computer

//	pData = &m_pData[0];

	m_pSelection->Setup(&listCtrl, &m_lListData, COLUMN_NAME, COLUMN_COMPUTER, NUM_REMOTE_TASK_COLUMNS, &m_iColumnOrder[0]);
	m_pSelection->SelectionRemoveAll();
}

void CTemplateRemoteTaskView::ReadAllColumnsWidth()
{
	CListCtrl& listCtrl = GetListCtrl();
	int iWidth;

	for(int iCount = 0; iCount< NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		iWidth = ReadColumnWidth(iCount, _gnRemoteTaskColumnWidth[iCount]);
		listCtrl.SetColumnWidth(m_iColumnOrder[iCount], iWidth);
		m_iColumnWidth[m_iColumnOrder[iCount]] = iWidth;
	}
}
int CTemplateRemoteTaskView::ReadColumnWidth(int iColumn, int iDefault)
{
	CString	sRegistry;
	char	cChar;
	int		iWidth;

	sRegistry = registryColumnTasks;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
//	if (iWidth < 20) return iDefault;
	return iWidth;
}

void CTemplateRemoteTaskView::CheckAndSaveColumnsWidth()
{
	int		iWidth, iColumn;
	bool	bChanged = false;
	char	cChar;
	CString sTxt;

	CListCtrl& listCtrl = GetListCtrl();

	for (int iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		iColumn = m_iColumnOrder[iCount];
		if (iColumn >= 0)
		{
			iWidth = listCtrl.GetColumnWidth(iColumn);
			if (m_iColumnWidth[iColumn] != listCtrl.GetColumnWidth(iColumn))
			{
				m_iColumnWidth[iColumn] = iWidth;
				bChanged = true;
				sTxt = registryColumnTasks;
				cChar = iCount + 'a';
				sTxt = sTxt + "_" + cChar; 
				// update new width in registry
				theApp.WriteProfileInt(registrySectionColumn, sTxt, iWidth);
				::SendMessage(theApp.m_pDlgSettingsTasks->m_hWnd, UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, (WPARAM) iColumn,(LPARAM) iWidth);
			}
		}
	}
}

void CTemplateRemoteTaskView::ColumnChanged()
{
	m_bApplyChangesOnColumn = false;
}


void CTemplateRemoteTaskView::ReadXml()
{
	int			iSize, iPos;
	CString		sNr, sFile, sApplication, sProject;
	char		cBufferLogging[255];
	int			iSeconds, iRed, iGreen, iBlue;
	COLORREF	crColor;
	CCheckpointItem *pCheckpointItem;

	iSize = (int) m_lCheckpoint.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		delete m_lCheckpoint.at(iPos);
	}
	m_lCheckpoint.clear();

	iSize = (int) m_lCheckpointSuspend.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		delete m_lCheckpointSuspend.at(iPos);
	}
	m_lCheckpointSuspend.clear();


	m_pLog->m_pFrom = "config.xml";
	sFile = "config.xml";
	CXml xml(sFile,false,true);
	if (xml.ReadStart("config"))
	{
		while (xml.ReadGroup("checkpoint",true))
		{
			xml.ReadItem("project",&sProject);
			xml.ReadItem("application",&sApplication);
			xml.ReadItem("seconds",&sNr);
			iSeconds = atoi(sNr);
			xml.ReadItem("red",&sNr);
			iRed = atoi(sNr);
			xml.ReadItem("green",&sNr);
			iGreen = atoi(sNr);
			xml.ReadItem("blue",&sNr);
			iBlue = atoi(sNr);
			crColor = RGB(iRed, iGreen, iBlue);

			pCheckpointItem = new CCheckpointItem;
			pCheckpointItem->m_sProject = sProject.MakeLower();

			pCheckpointItem->m_sApplication = sApplication.MakeLower();
			pCheckpointItem->m_iTime = iSeconds;
			pCheckpointItem->m_crColor = crColor;
			m_lCheckpoint.push_back(pCheckpointItem);

			_snprintf_s(cBufferLogging,254,_TRUNCATE,"project: %s, application: %s, seconds: %d, Red:%d, Green: %d, Blue: %d", sProject, sApplication, iSeconds, iRed, iGreen, iBlue);m_pLog->m_pText = cBufferLogging;
			m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_NORMAL);

		}
	}

	// any config read errors.
	if (xml.m_sLog.GetLength() > 0)
	{
		m_pLog->m_pText = xml.m_sLog.GetBuffer();
		m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_NORMAL);
	}

}

void CTemplateRemoteTaskView::CheckPointSuspend(int iPosHistory, int iPosSuspend)
{
	if (m_lCheckpointHistory.at(iPosHistory)->m_iCheckpoints == m_lCheckpointSuspend.at(iPosSuspend)->m_iCheckpoints)
	{
		return;
	}

	int		iThreadId;
	bool	bFound;
	bFound = m_pDoc->GetRpcThreadId(&m_lCheckpointSuspend.at(iPosSuspend)->m_sComputer, &iThreadId);
	if (bFound)
	{
		char *pcSelectedTasksItemsName;
		CStringCopyNew(&pcSelectedTasksItemsName, m_lCheckpointSuspend.at(iPosSuspend)->m_sName.GetBuffer(), false);
		m_lCheckpointSuspend.at(iPosSuspend)->m_sName.ReleaseBuffer();
		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_TASK_OPERATION,TASKS_OP_SUSPEND, (LPARAM) pcSelectedTasksItemsName);
		m_lCheckpointSuspend.at(iPosSuspend)->m_bDelete = true;
	}
}

BEGIN_MESSAGE_MAP(CTemplateRemoteTaskView, CListViewEx)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	ON_WM_RBUTTONDOWN()

	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickList)
    ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	ON_NOTIFY_EX(HDN_ITEMCHANGED, 0, OnHeaderChanged) 


//	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnDrag)

	ON_MESSAGE(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_TASKS,OnSettingsApplyChangesTasks)
	ON_MESSAGE(UWM_MSG_THREAD_TASK_READ_READY,OnTaskPropertiesReady)
	ON_MESSAGE(UWM_MSG_HISTORY_TASKS,OnHistoryTasks)

	ON_MESSAGE(UWM_MSG_SET_CLOUD_COLUMN,OnCloudColumn)
END_MESSAGE_MAP()


// CTemplateRemoteTaskView diagnostics

#ifdef _DEBUG
void CTemplateRemoteTaskView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CTemplateRemoteTaskView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


void CTemplateRemoteTaskView::GetRules()
{
	m_plRulesActive = &m_pDoc->m_pHistoryView->m_lRulesActive;
}


void CTemplateRemoteTaskView::DisplaySingleItem(CListCtrl *pListCtrl, int iRow, CRpcTaskInfo *pInfo, double m_dDiskInterval, int iClientNumber, char *pcComputerName,int iComputerId, bool bFilter)
{
	int			iSize, iPos;
	char		cBuffer[256];
	char		cBuffer2[256];
	char		cBufferLogging[129];
	char		*pszResources;
	double		dRatioPerc, dRatioPercLong;
	double		dPerc;
	bool		bCuda, bRunning, bSuspended, bSuspendedProject, bSuspendReason;
	int			iColumn, iGpu, iDetectedGpu;
	double		dElapsed, dCurrentCpuTime;
	UINT		iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;
	int			iSuspendReason;
	LV_ITEM		lvi;
	COLORREF	*pColorsTasks, *pColorsWarnings;
	bool		bCondenseUse;
	int			iGpuDevice;
	char		*pszFound;
	double		fCpuUse,fCpuUseCeil, fGpuUse;
	CListData	*pData;
	CString		sSuspendReason;
	char		cComma;

	cComma		= theApp.m_pDlgSettingsView->m_cComma;

	CCondenseResource condenseResource;
	CTimeString	timeString;

	char cPercString[5];
	cPercString[0] = '%';//		"%.3f";
	cPercString[1] = '.';
	cPercString[2] = '2';
	cPercString[3] = 'f';
	cPercString[4] = 0;

	while (iRow >= (int) m_lListData.size())
	{
		pData = new CListData;
		m_lListData.push_back(pData);
	}
	pData = m_lListData.at(iRow);

	if (pInfo->m_pCc != NULL)
	{
		iSuspendReason = pInfo->m_pCc->task_suspend_reason;
	}
	else
	{
		iSuspendReason = 0;
	}

	bSuspendReason = false;
	sSuspendReason = "sp?"; 

	if (iSuspendReason)
	{
		bSuspendReason = true;

		if (iSuspendReason & SUSPEND_REASON_BATTERIES)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendOnBatt];}			//"on batteries";}
		if (iSuspendReason & SUSPEND_REASON_USER_ACTIVE)			{ sSuspendReason = gszTranslation[PosViewTasksSuspendUserAct];}			//"user active";}
		if (iSuspendReason & SUSPEND_REASON_USER_REQ)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendUserRq];}			//"user request";}
		if (iSuspendReason & SUSPEND_REASON_TIME_OF_DAY)			{ sSuspendReason = gszTranslation[PosViewTasksSuspendTimeOD];}			//"time of day";}
		if (iSuspendReason & SUSPEND_REASON_BENCHMARKS)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendBench];}			//"benchmarks";}
		if (iSuspendReason & SUSPEND_REASON_DISK_SIZE)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendDisk];}			//"disk size";}
		if (iSuspendReason & SUSPEND_REASON_CPU_THROTTLE)			{ sSuspendReason = "-";}												// treat as not suspended
		if (iSuspendReason & SUSPEND_REASON_NO_RECENT_INPUT)		{ sSuspendReason = gszTranslation[PosViewTasksSuspendNoRI];}			//"no recent input";}
		if (iSuspendReason & SUSPEND_REASON_INITIAL_DELAY)			{ sSuspendReason = gszTranslation[PosViewTasksSuspendInitial];}			//"initial";}
		if (iSuspendReason & SUSPEND_REASON_EXCLUSIVE_APP_RUNNING)	{ sSuspendReason = gszTranslation[PosViewTasksSuspendEx];}				//"exclusive app running";}
		if (iSuspendReason & SUSPEND_REASON_CPU_USAGE)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendCpuUsage];}		//"cpu usage";}

		if (sSuspendReason == "")
		{
			if (bSuspendReason)
			{
				sSuspendReason.Format("Unknown suspend reason %d", iSuspendReason);
			}
		}
	}
	if (pInfo->m_iSuspendStatus == SUSPEND_STATUS_WAITING_FOR_MEMORY)
	{
		sSuspendReason = gszTranslation[PosViewTasksSuspendMemory];
		bSuspendReason = true;
	}
	else
	{
		if (pInfo->m_iSuspendStatus != 0)
		{
			sSuspendReason.Format("Unknown suspend status %d", pInfo->m_iSuspendStatus);
			bSuspendReason = true;
		}
//			bSuspendReason = false;
		if (pInfo->m_pResult != NULL)
		{
			if (pInfo->m_pResult->scheduler_wait)
			{
				sSuspendReason = pInfo->m_pResult->scheduler_wait_reason.c_str();
				bSuspendReason = true;
			}
		}
	}
	
	pszResources = pInfo->m_pszResources;

//	pszResources = "0.000152C + 1 Apple M1";

	CUseCpuGpu CpuGpu;
	iDetectedGpu = DETECTED_NONE;
	if (!CpuGpu.GetCpuGpu(pszResources, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu))
	{
		if (pInfo->m_bNonCpuIntensive)
		{
			fCpuUse = 0.025;
			pszResources = "0.025 CPUs";
		}
	}

	fCpuUseCeil = ceil(fCpuUse);

	// resolve Cpu and Gpu use
	// search for gpu device number 
	pszFound = strstr(pszResources, "device");
	if (pszFound != NULL)
	{
		pszFound += 6;
		iGpuDevice = (int)atof(pszFound);
	}
	else iGpuDevice = -1;
	// search for gpu device number 

	pColorsTasks = theApp.m_pDlgColorTasks->GetColorArray();
	pColorsWarnings = theApp.m_pDlgColorWarnings->GetColorArray();
	bCondenseUse = theApp.m_pDlgSettingsTasks->m_bUseCondense;

	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);
	pData->m_cSortPrimary = "";		// init strings, because is one of them is invalid the sorting crashes.
	pData->m_cSortSecondary = "";
	pData->m_cSortTertiary = "";

	// colors

	bSuspended = pInfo->m_bSuspendedByGui;
//	pData->m_bSuspendedByGui = bSuspended;
	bSuspendedProject = pInfo->m_bSuppendedProjectByGui;

// -------------------------------- project
	iColumn = m_iColumnOrder[COLUMN_PROJECT];
	if (iColumn >= 0) pListCtrl->SetItemText(iRow,iColumn,pInfo->m_pszProject);
	if (iSortingColumnPrimary == COLUMN_PROJECT) pData->m_cSortPrimary = pInfo->m_pszProject;
	if (iSortingColumnSecondary == COLUMN_PROJECT) pData->m_cSortSecondary = pInfo->m_pszProject;
	if (iSortingColumnTertiary == COLUMN_PROJECT) pData->m_cSortTertiary = pInfo->m_pszProject;
// -------------------------------- project

// -------------------------------- account
	iColumn = m_iColumnOrder[COLUMN_ACCOUNT];
	if (iColumn >= 0) pListCtrl->SetItemText(iRow,iColumn,pInfo->m_pszAccount);
	if (iSortingColumnPrimary == COLUMN_ACCOUNT) pData->m_cSortPrimary = pInfo->m_pszAccount;
	if (iSortingColumnSecondary == COLUMN_ACCOUNT) pData->m_cSortSecondary = pInfo->m_pszAccount;
	if (iSortingColumnTertiary == COLUMN_ACCOUNT) pData->m_cSortTertiary = pInfo->m_pszAccount;
// -------------------------------- account

// -------------------------------- application
	float fVersion;
	fVersion = (float) pInfo->m_iVersionNumber;
	fVersion/= 100;

	if (strcmp(pInfo->m_pszPlanClass,"") == 0)
	{
		_snprintf_s(cBuffer,200,_TRUNCATE,"%.2f %s", fVersion, pInfo->m_pszApp);
	}
	else
	{
		_snprintf_s(cBuffer,200,_TRUNCATE,"%.2f %s (%s)", fVersion, pInfo->m_pszApp, pInfo->m_pszPlanClass);
	}

	iColumn = m_iColumnOrder[COLUMN_APPLICATION];
	if (iColumn >= 0)
	{
		pListCtrl->SetItemText(iRow, iColumn, cBuffer);


		if (theApp.m_pDlgSettingsTasks->m_bSortOnApplicationNr)
		{
			if (iSortingColumnPrimary == COLUMN_APPLICATION) pData->m_iSortPrimary = (pInfo->m_iVersionNumber *100)+iGpu;
			if (iSortingColumnSecondary == COLUMN_APPLICATION) pData->m_iSortSecondary = (pInfo->m_iVersionNumber *100)+iGpu;
			if (iSortingColumnTertiary == COLUMN_APPLICATION) pData->m_iSortTertiary = (pInfo->m_iVersionNumber *100)+iGpu;
		}
		else
		{
			if (iSortingColumnPrimary == COLUMN_APPLICATION) pData->m_sSortPrimary = cBuffer;
			if (iSortingColumnSecondary == COLUMN_APPLICATION) pData->m_sSortSecondary = cBuffer;
			if (iSortingColumnTertiary == COLUMN_APPLICATION) pData->m_sSortTertiary = cBuffer;
		}
	}
// -------------------------------- application

// -------------------------------- name
	iColumn = m_iColumnOrder[COLUMN_NAME];
	if (iColumn >= 0) pListCtrl->SetItemText(iRow,iColumn,pInfo->m_pszName);
	if (iSortingColumnPrimary == COLUMN_NAME)
	{
		pData->m_cSortPrimary = pInfo->m_pszName;
		if (strstr(pInfo->m_pszName,gszTranslation[PosViewTasksFilterTasks]) != NULL)
		{
			pData->m_iSortPrimary = atoi(pInfo->m_pszName+1); //TASKS_STRING
		}
		else pData->m_iSortPrimary = -1;
	}
	if 	(iSortingColumnSecondary == COLUMN_NAME) 
	{
		pData->m_cSortSecondary = pInfo->m_pszName;
		if (strstr(pInfo->m_pszName,gszTranslation[PosViewTasksFilterTasks]) != NULL)
		{
			pData->m_iSortSecondary = atoi(pInfo->m_pszName);	//TASKS_STRING
		}
		else pData->m_iSortSecondary = -1;
	}
	if 	(iSortingColumnTertiary == COLUMN_NAME) 
	{
		pData->m_cSortTertiary = pInfo->m_pszName;
		if (strstr(pInfo->m_pszName,gszTranslation[PosViewTasksFilterTasks]) != NULL)
		{
			pData->m_iSortTertiary = atoi(pInfo->m_pszName);	//TASKS_STRING
		}
		else pData->m_iSortTertiary = -1;
	}

// -------------------------------- name
	cBuffer[0] = 0;

// -------------------------------- elapsed
	iColumn = m_iColumnOrder[COLUMN_ELAPSED];

	if ((pInfo->m_iState&0xffffe) == STATE_RUNNING) 
	{
		bRunning = true;
		if (bSuspendReason) bRunning = false;
		if (bSuspendedProject) bRunning = false;
		dElapsed = pInfo->m_dElapsedTime;
	}
	else
	{
		bRunning = false;
		dElapsed = pInfo->m_dElapsedTime;
	}

//	if (pInfo->)

	if (iSortingColumnPrimary == COLUMN_ELAPSED) pData->m_iSortPrimary = (int) dElapsed;
	if (iSortingColumnSecondary == COLUMN_ELAPSED) pData->m_iSortSecondary = (int) dElapsed;
	if (iSortingColumnTertiary == COLUMN_ELAPSED) pData->m_iSortTertiary = (int) dElapsed;

	dCurrentCpuTime = pInfo->m_dCurrentCpuTime;

	timeString.TimeString((int) dElapsed,cBuffer, 50, "", "");

	timeString.TimeString((int)dCurrentCpuTime,cBuffer2, 50, " (", ")");
	strcat_s(cBuffer, 50,cBuffer2);

	if (iColumn >= 0) pListCtrl->SetItemText(iRow,m_iColumnOrder[COLUMN_ELAPSED],cBuffer);
// -------------------------------- elapsed

// -------------------------------- ratio CPU %

	CString sCpuPercText;

	iColumn = m_iColumnOrder[COLUMN_CPU_PERCENTAGE];
	cPercString[2] = theApp.m_pDlgSettingsTasks->m_iCpuDigits + '0';

	bool bRatioLongTerm = m_bRatioLongTerm;
	if (!bRunning) bRatioLongTerm = true;

	dRatioPerc = 0;
	if (!bRatioLongTerm)
	{
		dRatioPerc = GetRatio(pInfo);						// ratio in 3 digits
	}

	if (fCpuUseCeil > 0)
	{
		dRatioPerc = (float) (dRatioPerc / fCpuUseCeil);
	}
	if (dRatioPerc <= 0) dRatioPerc = 0;

	if (dCurrentCpuTime == 0) dRatioPercLong = 0;
	else
	{
		if (dCurrentCpuTime > 0)
		{
			if (dElapsed > 0)
			{
				dRatioPercLong = dCurrentCpuTime;
				dRatioPercLong /= dElapsed;
				dRatioPercLong = dRatioPercLong *100;
				if (fCpuUseCeil > 0)
				{
					dRatioPercLong = (double) (dRatioPercLong / fCpuUseCeil);
				}
			}
			else dRatioPercLong = 0;
		}
		else dRatioPercLong = 0;
	}
	if (dRatioPercLong <= 0) dRatioPercLong = 0;

	if (bRatioLongTerm)
	{
		dRatioPerc = dRatioPercLong;
	}

	cBuffer[1] = 2;
	if (dRatioPerc == 0) cBuffer[1] = 1;
	_snprintf_s(&cBuffer[3],50,_TRUNCATE,cPercString,dRatioPerc);

	if (cComma != '.')
	{
		char *cFind = strchr(cBuffer, '.'); 
		if (cFind) *cFind = cComma;
	}

	cBuffer[0] = PERC_CHAR; 
	cBuffer[2] = (char) dRatioPerc;
	cBuffer[2]+= 1;

	sCpuPercText = cBuffer;

	if (iSortingColumnPrimary == COLUMN_CPU_PERCENTAGE) pData->m_fSortPrimary = dRatioPerc;
	if (iSortingColumnSecondary == COLUMN_CPU_PERCENTAGE) pData->m_fSortSecondary = dRatioPerc;
	if (iSortingColumnTertiary == COLUMN_CPU_PERCENTAGE) pData->m_fSortTertiary = dRatioPerc;
// -------------------------------- ratio CPU %

// -------------------------------- remaining time
	iColumn = m_iColumnOrder[COLUMN_TIME_LEFT];
	if (pInfo->m_iRemainingTime > 0) timeString.TimeString(pInfo->m_iRemainingTime,cBuffer, 50, "", "");
	else
	{
		if (theApp.m_bNoCleanup) timeString.TimeString(pInfo->m_iRemainingTime,cBuffer, 50, "", "",false,true);
		else strcpy_s(cBuffer,50,"-");
	}
	if (iColumn >= 0) pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	if (iSortingColumnPrimary == COLUMN_TIME_LEFT) pData->m_iSortPrimary = pInfo->m_iRemainingTime;
	if (iSortingColumnSecondary == COLUMN_TIME_LEFT) pData->m_iSortSecondary = pInfo->m_iRemainingTime;
	if (iSortingColumnTertiary == COLUMN_TIME_LEFT) pData->m_iSortTertiary = pInfo->m_iRemainingTime;
// -------------------------------- remaining time

//	strcpy_s(cBuffer,24,"-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");		// error

// -------------------------------- percentage
	iColumn = m_iColumnOrder[COLUMN_PERCENTAGE];

	cPercString[2] = theApp.m_pDlgSettingsTasks->m_iProgressDigits + '0';

	dPerc = (double) pInfo->m_dFraction_done *100;

	if (dPerc < 0.01)	// Do it the BoincTasks way
	{
		if (pInfo->m_iRemainingTime == 0)
		{
			dPerc = 100;
			cBuffer[1] = 2;
		}
		else
		{
			if (dElapsed > 0)
			{
				dPerc = dElapsed * 100;
				dPerc /= dElapsed + pInfo->m_iRemainingTime;
				cBuffer[1] = 2;
			}
			else
			{
				dPerc = 0;
				cBuffer[1] = 1;
			}
		}
	}
	else
	{
		if (dPerc == 0) cBuffer[1] = 1;
		else cBuffer[1] = 2;
	}

	
	if (dPerc > 100)
	{
		if (!bRunning)
		{
			dPerc = 100;
		}
	}

	if (theApp.m_bNoCleanup)
	{
		_snprintf_s(&cBuffer[3],50,_TRUNCATE,cPercString,dPerc);
		if (dPerc > 100) dPerc = 100;							// here !!! after the value but before the bar value is set.
		if (dPerc < 0) dPerc = 0;
	}
	else
	{
		if (dPerc > 100) dPerc = 100;							// here !!! after the value but before the bar value is set.
		if (dPerc < 0) dPerc = 0;
		_snprintf_s(&cBuffer[3],50,_TRUNCATE,cPercString,dPerc);
	}

	if (cComma != '.')
	{
		char *cFind = strchr(cBuffer, '.'); 
		if (cFind) *cFind = cComma;
	}
	cBuffer[0] = PERC_CHAR; 
	cBuffer[2] = (char) dPerc;
	cBuffer[2]+= 1;
	if (iColumn >= 0) pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	
	if (iSortingColumnPrimary == COLUMN_PERCENTAGE) pData->m_fSortPrimary = dPerc;
	if (iSortingColumnSecondary == COLUMN_PERCENTAGE) pData->m_fSortSecondary = dPerc;
	if (iSortingColumnTertiary == COLUMN_PERCENTAGE) pData->m_fSortTertiary = dPerc;
// -------------------------------- percentage

// -------------------------------- deadline
	iColumn = m_iColumnOrder[COLUMN_DEADLINE];
	if (iColumn >= 0)
	{
		time_t tSystemTime;
		_time64( &tSystemTime);

		time_t tTimeDeadline = pInfo->m_iDeadlineTime;
		cBuffer[0] = 0;

		if (theApp.m_pDlgSettingsTasks->m_bDeadlineShowRemaining)
		{
			int iTimeLeft;
			iTimeLeft = (int) (pInfo->m_iDeadlineTime - tSystemTime);
			timeString.TimeString(iTimeLeft,cBuffer, 50, "", "",false,true);	// allow negative numbers, because you be over the deadline
		}
		else
		{
			CDateConvert dateConvert;
			dateConvert.Convert(tTimeDeadline, &cBuffer[0]);	
		}

		int iDeadline;
		iDeadline = theApp.m_pDlgSettingsWarning->m_iDeadlineTime;
		if (iDeadline > 0)			// deadline - dlgDeadlin - remaining time
		{
			if (!bFilter)			// is done in the filter itself
			{
				__int64 i64TimeDiff;
				time_t  tDeadline;
				tDeadline = tTimeDeadline - iDeadline*60;
				tDeadline-= pInfo->m_iRemainingTime;
				i64TimeDiff = tSystemTime - tDeadline;
				if (i64TimeDiff > 0) pInfo->m_bDeadline = true;
				else pInfo->m_bDeadline = false;
			}
		}
		else pInfo->m_bDeadline = false;

		if (pInfo->m_bDeadline)
		{
			int iColor;
			CString sColor, sText;
			if (bRunning)	iColor  = *(pColorsWarnings + COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING);
			else			iColor  = *(pColorsWarnings + COLOR_WARNINGS_ARRAY_DEADLINE);
	
			sColor.Format("%d", iColor);
			sColor = HIDE_CHAR + sColor;
			sText = COLOR_CHAR;
			sText = sText + cBuffer + sColor;
			pListCtrl->SetItemText(iRow,iColumn,sText);
		}
		else
		{
			pListCtrl->SetItemText(iRow,iColumn,cBuffer);
		}
	}

	if (iSortingColumnPrimary == COLUMN_DEADLINE) pData->m_iSortPrimary =  pInfo->m_iDeadlineTime;
	if (iSortingColumnSecondary == COLUMN_DEADLINE) pData->m_iSortSecondary =  pInfo->m_iDeadlineTime;
	if (iSortingColumnTertiary == COLUMN_DEADLINE) pData->m_iSortTertiary =  pInfo->m_iDeadlineTime;
	


// -------------------------------- deadline

	int		iStatusSorting = 0;
	bool	bReportSuspend = true;
//	int iStateSort;
	switch (pInfo->m_iState)
	{
		case STATE_COMPUTATON_ERROR:
		case STATE_COMPUTATON_ERROR_HP:
		case STATE_COMPUTATON_ERROR_WR:
			bReportSuspend= false;

			if (pInfo->m_iExitStatus == ERR_ABORTED_BY_PROJECT) 
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAbortedProject]);		//"Aborted by project");
				iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusAbortedProject - m_iListStatusSorting);
			}
			else
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusComputation]);			//"Computation error");
				iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusComputation - m_iListStatusSorting);
			}
			if (bCuda)	pData->m_color = *(pColorsTasks + COLOR_TASKS_ARRAY_ERROR_GPU);
			else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_ERROR);
		break;
		case STATE_DOWNLOADING:
		case STATE_DOWNLOADING_HP:
			/*if (dPerc >= 100)	// 100% download is an error
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusDownloadError]);	//"Download error");
				iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusDownloadError - m_iListStatusSorting);
				if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_ERROR_GPU);
				else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_ERROR);
			}
			else
			*/
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusDownloading]);	//"Downloading");
				iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusDownloading - m_iListStatusSorting);
				if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU);
				else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD);
			}
		break;
		case STATE_UPLOADING:
			strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusUploading]);	//"Uploading");
			iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusUploading - m_iListStatusSorting);
			if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU);
			else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD);
		break;
		case STATE_READY_TO_REPORT:
			bReportSuspend= false;
			bSuspendedProject = false;
			strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusReadyTR]);	//"Ready to report");
			iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusReadyTR - m_iListStatusSorting);
			if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU);
			else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_READY_TO_REPORT);
		break;
		case STATE_READY_TO_START:
		case STATE_READY_TO_START_HP:
			strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusReadyTS]);	//"Ready to start");
			iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusReadyTS - m_iListStatusSorting);
			if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_READY_TO_START_GPU);
			else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_READY_TO_START);
		break;
		case STATE_WAITING_TO_RUN:
		case STATE_WAITING_TO_RUN_HP:
			strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusWaitingTR]);
			iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusWaitingTR - m_iListStatusSorting);
			if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU);
			else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_WAITING_TO_RUN);

			if (!bSuspendReason)
			{
				// may not be needed, but still there are reports
				if (pInfo->m_pResult != NULL)
				{
					if (pInfo->m_pResult->scheduler_wait)
					{
						sSuspendReason = pInfo->m_pResult->scheduler_wait_reason.c_str();
						bSuspendReason = true;
					}
				}
			}
		break;
		case STATE_RUNNING:
			if (!bSuspendReason)	// don't show running when suspended
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusRunning]);	//"Running");
				iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusRunning - m_iListStatusSorting);
				if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_RUNNING_GPU);
				else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_RUNNING);
				if (pInfo->m_bNonCpuIntensive)
				{
					strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusRunningLP]);	//"Running <");
					iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusRunningLP - m_iListStatusSorting);
				}
			}
			else cBuffer[0] = 0;
		break;
		case STATE_RUNNING_HP:
			if (!bSuspendReason)	// don't show running when suspended
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusRunningHP]);	//"Running High P.");
				iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusRunningHP - m_iListStatusSorting);
				if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU);
				else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_HIGH_PRIORITY);
			}
			else cBuffer[0] = 0;
		break;
		case STATE_ABORTED:
		case STATE_ABORTED_HP:
		case STATE_ABORTED1:
		case STATE_ABORTED2:
		case STATE_ABORTED2_HP:
			bReportSuspend = false;	
			switch (pInfo->m_iExitStatus)
			{
				case ERR_ABORTED_BY_PROJECT:
					strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAbortedProject]);		//"Aborted by project");
					iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusAbortedProject - m_iListStatusSorting);
				break;
				case ERR_ABORTED_VIA_GUI:
					strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAbortedUser]);			//"Aborted by user");
					iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusAbortedUser - m_iListStatusSorting);
				break;
				default:
					if (pInfo->m_iExitStatus != 0)
					{
						strcpy_s(cBuffer2,50,gszTranslation[PosViewTasksStatusAborted]);		//;"Aborted");
						strcat_s(cBuffer2,60," (%d)");
						_snprintf_s(cBuffer,60,_TRUNCATE,cBuffer2, pInfo->m_iExitStatus);
						iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusAborted - m_iListStatusSorting);
					}
					else
					{
						strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAborted]);			//;"Aborted");
						iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusAborted - m_iListStatusSorting);
					}
			}
			pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_ABORT);
		break;
		case STATE_NEW:
		case STATE_NEW_HP:
			strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusNew]);	//"New");
			iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusNew - m_iListStatusSorting);
			pData->m_color  = RGB(255,255,255);
		break;

		default:
			_snprintf_s(cBuffer,50,_TRUNCATE,"%x-state: ",pInfo->m_iState);
			pData->m_color  = RGB(255,255,255);
	}

	// suspende override!!!
	if (bSuspended)
	{
		if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_SUSPENDED_GPU);
		else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_SUSPENDED);
		strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusSuspended]);		//"Suspended");
		iStatusSorting = m_lListStatusSorting.at(PosViewTasksStatusSuspended - m_iListStatusSorting);
	}

	if (bSuspendedProject)
	{
		if (cBuffer[0] != NULL)	strcat_s(cBuffer,120,", ");

		if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_SUSPENDED_GPU);
		else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_SUSPENDED);
		strcat_s(cBuffer,140,gszTranslation[PosViewTasksSuspendProject]);	//"Project suspended");
		int iSortingPos = PosViewTasksSuspendProject - m_iListStatusSorting;
		iStatusSorting = m_lListStatusSorting.at(iSortingPos);
	}

	if (bSuspendReason)
	{
		if (cBuffer[0] != NULL)	strcat_s(cBuffer,120,",");
		else
		{
			iStatusSorting = m_lListStatusSorting.at(PosViewTasksSuspendPrefix - m_iListStatusSorting);
		}
		strcat_s(cBuffer,120,gszTranslation[PosViewTasksSuspendPrefix]);	//"suspended: ");
		strcat_s(cBuffer,140,sSuspendReason.GetBuffer()); sSuspendReason.ReleaseBuffer();

		if (bCuda)	pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_SUSPENDED_GPU);
		else		pData->m_color  = *(pColorsTasks + COLOR_TASKS_ARRAY_SUSPENDED);
	}

	if (pInfo->m_bDeadline)
	{
		strcat_s(cBuffer, 120, ", ");
		strcat_s(cBuffer, 120, gszTranslation[PosViewTasksWarningDeadline]);	//"deadline warning");
	}
	
	if (iSortingColumnPrimary == COLUMN_STATE) pData->m_iSortPrimary =  iStatusSorting;	
	if (iSortingColumnSecondary == COLUMN_STATE) pData->m_iSortSecondary =  iStatusSorting;
	if (iSortingColumnTertiary == COLUMN_STATE) pData->m_iSortTertiary =  iStatusSorting;

	// follow the name
	pData->m_bSelected = m_pSelection->Follow(pInfo->m_pszName, pcComputerName);

	lvi.mask = LVIF_PARAM;  //LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = iRow;
	lvi.iSubItem = 0;

	COLORREF cpuPercWarningColor;
	cpuPercWarningColor = 0;
	if (bRunning)
	{
		// warning rules

		CheckWarning(&cpuPercWarningColor, (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO), (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU),
					 dRatioPerc, fCpuUse, fGpuUse,
					 theApp.m_pDlgSettingsWarning->m_fWarningCpuLow1, theApp.m_pDlgSettingsWarning->m_fWarningCpuHigh1,
					 theApp.m_pDlgSettingsWarning->m_fWarningGpuLow1, theApp.m_pDlgSettingsWarning->m_fWarningGpuHigh1,
					 theApp.m_pDlgSettingsWarning->m_fWarningPercLow1,theApp.m_pDlgSettingsWarning->m_fWarningPercHigh1);
		CheckWarning(&cpuPercWarningColor, (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO), (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU),
					 dRatioPerc, fCpuUse, fGpuUse,
					 theApp.m_pDlgSettingsWarning->m_fWarningCpuLow2, theApp.m_pDlgSettingsWarning->m_fWarningCpuHigh2,
					 theApp.m_pDlgSettingsWarning->m_fWarningGpuLow2, theApp.m_pDlgSettingsWarning->m_fWarningGpuHigh2,
					 theApp.m_pDlgSettingsWarning->m_fWarningPercLow2,theApp.m_pDlgSettingsWarning->m_fWarningPercHigh2);
		CheckWarning(&cpuPercWarningColor, (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO), (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU),
					 dRatioPerc, fCpuUse, fGpuUse,
					 theApp.m_pDlgSettingsWarning->m_fWarningCpuLow3, theApp.m_pDlgSettingsWarning->m_fWarningCpuHigh3,
					 theApp.m_pDlgSettingsWarning->m_fWarningGpuLow3, theApp.m_pDlgSettingsWarning->m_fWarningGpuHigh3,
					 theApp.m_pDlgSettingsWarning->m_fWarningPercLow3,theApp.m_pDlgSettingsWarning->m_fWarningPercHigh3);
		CheckWarning(&cpuPercWarningColor, (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO), (pColorsWarnings + COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU),
					 dRatioPerc, fCpuUse, fGpuUse,
					 theApp.m_pDlgSettingsWarning->m_fWarningCpuLow4, theApp.m_pDlgSettingsWarning->m_fWarningCpuHigh4,
					 theApp.m_pDlgSettingsWarning->m_fWarningGpuLow4, theApp.m_pDlgSettingsWarning->m_fWarningGpuHigh4,
					 theApp.m_pDlgSettingsWarning->m_fWarningPercLow4,theApp.m_pDlgSettingsWarning->m_fWarningPercHigh4);

		// warning rules
		// rules

		CActiveRuleItem* pActiveRuleItem;
		iSize = (int) m_plRulesActive->size();
		for (iPos = 0; iPos < iSize; iPos++)
		{
			pActiveRuleItem =  m_plRulesActive->at(iPos);
			if (pActiveRuleItem->m_sTaskName ==  pInfo->m_pszName )
			{
				if (pActiveRuleItem->m_bEventTriggered)
				{
					pData->m_color = pActiveRuleItem->m_color;
				}
			}
		}
		

		// rules

	}

// ratio CPU% color

	// PERC_CHAR, hide, perc+1 or PERC_CHAR, hide, perc+1, COLOR_CHAR, int color

	iColumn = m_iColumnOrder[COLUMN_CPU_PERCENTAGE];

	if (cpuPercWarningColor != 0)
	{
		CString sColor, sText;
	
		sColor.Format("%d", cpuPercWarningColor);
		sText = COLOR_CHAR;
		sText = sText + sColor;
		sCpuPercText = sCpuPercText + sText;
	}

	if (iColumn >= 0) pListCtrl->SetItemText(iRow,iColumn,sCpuPercText);

	lvi.lParam = (LPARAM) pData; // &m_data[iRow];
	if (m_iColumnOrder[COLUMN_STATE] >= 0) pListCtrl->SetItemText(iRow,m_iColumnOrder[COLUMN_STATE],cBuffer);

	iColumn = m_iColumnOrder[COLUMN_COMPUTER];
	if (iColumn >=0)
	{
		if (pcComputerName != NULL)	pListCtrl->SetItemText(iRow,iColumn, pcComputerName);
	}

// -------------------------------- Resources
	iColumn = m_iColumnOrder[COLUMN_USE];
	if (iColumn >=0)
	{
		CString sResources;
		sResources = pszResources;
		if (bCondenseUse)
		{
			condenseResource.Condense(&sResources);
		}
		if (cComma != '.')
		{
			sResources.Replace('.', cComma);
		}
		pListCtrl->SetItemText(iRow,iColumn,sResources);
	}
	int iUse = (int) fCpuUse * 1000;
	iUse = iUse << 16;
	iUse+= (int) fGpuUse * 1000;
		
	if (iSortingColumnPrimary == COLUMN_USE) pData->m_iSortPrimary = iUse;
	if (iSortingColumnSecondary == COLUMN_USE) pData->m_iSortSecondary = iUse;
	if (iSortingColumnTertiary == COLUMN_USE) pData->m_iSortTertiary = iUse;
// -------------------------------- Resources


// -------------------------------- TThrottle
	iColumn = m_iColumnOrder[COLUMN_TTHROTTLE];	
	float fThrottle = -1;
	if (iColumn >= 0)
	{
		cBuffer[0] = 0;
		if (bRunning)
		{
			if (iClientNumber >= 0)
			{
				if (m_lRpcInfoClient.at(iClientNumber)->m_bReadyCopy)	// thread reports ready
				{

					if (bCuda)	fThrottle = (float) m_lRpcInfoClient.at(iClientNumber)->m_fGpuThrottleCopy;
					else		fThrottle = (float) m_lRpcInfoClient.at(iClientNumber)->m_fCpuThrottleCopy;
					if (fThrottle < 0) fThrottle = 0;		// -1 if not in use

					fThrottle = 100 - fThrottle;
					if (fThrottle >=0)
					{
						if (fThrottle == 0) cBuffer[1] = 1;
						else cBuffer[1] = 2;

						_snprintf_s(&cBuffer[3],50,_TRUNCATE,"%.0f",fThrottle);
						cBuffer[0] = PERC_CHAR; 
						cBuffer[2] = (char) fThrottle;
						cBuffer[2]+= 1;
					}
				}
			}
		}
		pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	}
	if (iSortingColumnPrimary == COLUMN_TTHROTTLE) pData->m_iSortPrimary = (int) fThrottle;
	if (iSortingColumnSecondary == COLUMN_TTHROTTLE) pData->m_iSortSecondary = (int) fThrottle;
	if (iSortingColumnTertiary == COLUMN_TTHROTTLE) pData->m_iSortTertiary = (int) fThrottle;
// -------------------------------- TThrottle

// -------------------------------- Checkpoint


	double dCheckpoint, dCheckpointRelative, dCheckpointRatio, dElapsedCheck, dCheckpointCpuFactor;
	if (fCpuUseCeil > 0)
	{
		dCheckpointCpuFactor = fCpuUseCeil;
	}
	else
	{
		dCheckpointCpuFactor = 1;
	}

	dCheckpoint = pInfo->m_dCheckpointCpuTime;
	dCheckpointRelative = 0;
	iColumn = m_iColumnOrder[COLUMN_CHECKPOINT];	
	int iState;
	iState = pInfo->m_iState&0xffffe;
	if (iColumn >= 0)
	{
		cBuffer[0] = 0;
		if (iState == STATE_WAITING_TO_RUN || iState == (STATE_DUMMY_SUSPENDED) || iState == STATE_RUNNING)
		{
			if (dCheckpoint >= 0)
			{
				if (bCuda)
				{
					dCheckpointRatio = dRatioPercLong / 100;
					if (dCheckpointRatio <= 0) dCheckpointRatio = 0.01;
					dElapsedCheck = dCurrentCpuTime;
				}
				else
				{
					dCheckpointRatio = 1;
					dElapsedCheck = dCurrentCpuTime;
				}
		
				if (dElapsedCheck >= dCheckpoint)
				{
					dCheckpointRelative = dElapsedCheck/dCheckpointCpuFactor  - dCheckpoint/dCheckpointCpuFactor;
					dCheckpointRelative /= dCheckpointRatio;
				}
				else
				{
					dCheckpointRelative = dCheckpoint / dCheckpointCpuFactor;
					dCheckpointRelative = dCheckpoint;
					dCheckpointRelative /= dCheckpointRatio;
				}
				cBuffer2[0] = 0;
				CString sColor;
				int iColor;
				if (theApp.m_pDlgSettingsWarning->m_bCheckpoint)
				{
					if (dCheckpointRelative > m_dDiskInterval*2)
					{
						cBuffer2[0] = COLOR_CHAR;
						cBuffer2[1] = 0;
		
						iColor = *(theApp.m_pDlgColorWarnings->GetColorArray() + COLOR_WARNINGS_ARRAY_CHECKPOINT);
						sColor.Format("%d", iColor);
						sColor = HIDE_CHAR + sColor;
					}
				}
				if (CheckCheckpointWarning(dCheckpointRelative, pInfo->m_pszProject, pInfo->m_pszApp, &iColor))
				{
					cBuffer2[0] = COLOR_CHAR;
					cBuffer2[1] = 0;
					sColor.Format("%d", iColor);
					sColor = HIDE_CHAR + sColor;
				}
				CString sBefore;
				sBefore = cBuffer2;
				CheckPointAddCounter(&sBefore, (int) dCheckpoint, pInfo->m_pszProject, pInfo->m_pszApp, pInfo->m_pszName);
				timeString.TimeString((int) dCheckpointRelative, cBuffer, 60, sBefore.GetBuffer(), sColor.GetBuffer(), true);
			}
		}
		
		pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	}

	if (iSortingColumnPrimary == COLUMN_CHECKPOINT) pData->m_fSortPrimary = dCheckpointRelative;
	if (iSortingColumnSecondary == COLUMN_CHECKPOINT) pData->m_fSortSecondary = dCheckpointRelative;
	if (iSortingColumnTertiary == COLUMN_CHECKPOINT) pData->m_fSortTertiary = dCheckpointRelative;

// -------------------------------- Checkpoint

// -------------------------------- Recieved

	iColumn = m_iColumnOrder[COLUMN_RECEIVED];	
	if (iColumn > 0)
	{
		time_t tTimeReceived = (time_t) pInfo->m_dReceivedTime;
		cBuffer[0] = 0;

		CDateConvert dateConvert;
		dateConvert.Convert(tTimeReceived, &cBuffer[0]);
		pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	}

	if (iSortingColumnPrimary == COLUMN_RECEIVED) pData->m_fSortPrimary = pInfo->m_dReceivedTime;
	if (iSortingColumnSecondary == COLUMN_RECEIVED) pData->m_fSortSecondary = pInfo->m_dReceivedTime;
	if (iSortingColumnTertiary == COLUMN_RECEIVED) pData->m_fSortTertiary = pInfo->m_dReceivedTime;


// -------------------------------- Recieved

	double dMemory;
// -------------------------------- Virtual

	iColumn = m_iColumnOrder[COLUMN_VIRTUAL_MEMORY];
	dMemory = 0;
	if (iColumn > 0)
	{
		if (pInfo->m_pResult != NULL)
		{
			dMemory = pInfo->m_pResult->swap_size/MEGA_BYTE;
			_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.2f MB",dMemory);
			pListCtrl->SetItemText(iRow,iColumn,cBuffer);
		}
		else
		{
			pListCtrl->SetItemText(iRow,iColumn,"");
		}
	}
	if (iSortingColumnPrimary == COLUMN_VIRTUAL_MEMORY) pData->m_fSortPrimary = dMemory;
	if (iSortingColumnSecondary == COLUMN_VIRTUAL_MEMORY) pData->m_fSortSecondary = dMemory;
	if (iSortingColumnTertiary == COLUMN_VIRTUAL_MEMORY) pData->m_fSortTertiary = dMemory;


// -------------------------------- Virtual

// -------------------------------- Memory

	iColumn = m_iColumnOrder[COLUMN_MEMORY];	
	dMemory = 0;
	if (iColumn > 0)
	{
		if (pInfo->m_pResult != NULL)
		{
			dMemory = pInfo->m_pResult->working_set_size_smoothed/MEGA_BYTE;
			_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.2f MB",dMemory);
			pListCtrl->SetItemText(iRow,iColumn,cBuffer);
		}
		else
		{
			pListCtrl->SetItemText(iRow,iColumn,"");

		}
	}
	if (iSortingColumnPrimary == COLUMN_MEMORY) pData->m_fSortPrimary = dMemory;
	if (iSortingColumnSecondary == COLUMN_MEMORY) pData->m_fSortSecondary = dMemory;
	if (iSortingColumnTertiary == COLUMN_MEMORY) pData->m_fSortTertiary = dMemory;

// -------------------------------- Memory

// -------------------------------- Debt
	double dDebt;
	dDebt = 0;
	iColumn = m_iColumnOrder[COLUMN_DEBT];
	if (iColumn >= 0)
	{
		if (bCuda)
		{
			if (iDetectedGpu == DETECTED_NV)
			{
				dDebt = pInfo->m_dCudaShortTermDebt;
			}
			else
			{
				dDebt = pInfo->m_dAtiShortTermDebt;
			}
		}
		else
		{
			dDebt = pInfo->m_dCpuShortTermDebt;
		}

		_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.2f",dDebt);
		pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	}
	if (iSortingColumnPrimary == COLUMN_DEBT) pData->m_fSortPrimary = dDebt;
	if (iSortingColumnSecondary == COLUMN_DEBT) pData->m_fSortSecondary = dDebt;
	if (iSortingColumnTertiary == COLUMN_DEBT) pData->m_fSortTertiary = dDebt;
// -------------------------------- Debt

// -------------------------------- Temperature
	iColumn = m_iColumnOrder[COLUMN_TEMPERATURE];	
	int	iRealDevice;
	float fTemperature = -1;
	if (iColumn >= 0)
	{
		cBuffer[0] = 0;
		if (bRunning)
		{
			iRealDevice = -1;
			if (iClientNumber >= 0)
			{
				if (m_lRpcInfoClient.at(iClientNumber)->m_bReadyCopy)	// thread reports ready
				{
					if (bCuda)
					{
						if (iDetectedGpu == DETECTED_NV)
						{
							iRealDevice = 0;
						}
						if (iDetectedGpu == DETECTED_ATI)
						{
							iRealDevice = m_lRpcInfoClient.at(iClientNumber)->m_iNrNVIDIA;
						}
						if (iGpuDevice >= 0) 
						{
							iRealDevice += iGpuDevice;
						}

						if (iRealDevice >=0 && iRealDevice < MAX_GPU)		// check if we can find the GPU 
						{
							fTemperature = (float) m_lRpcInfoClient.at(iClientNumber)->m_fGpuTemperatureCopy[iRealDevice];
						}
						else
						{
							fTemperature = (float) m_lRpcInfoClient.at(iClientNumber)->m_fTopGpuTemperatureCopy; // else take the Max of all of them
						}
						if (theApp.m_pDlgLogging->m_bLogDebugTemperatureGpu)
						{
							m_pLog->m_pFrom = "TemperatureGpu";
							_snprintf_s(cBufferLogging,128,_TRUNCATE,"Computer: %s, ClientNr: %d, GpuType: %d, NV:%d, ATI: %d, Device: %d RealDevice: %d, Temperature: %.1f", pcComputerName, iClientNumber, iDetectedGpu, m_lRpcInfoClient.at(iClientNumber)->m_iNrNVIDIA, m_lRpcInfoClient.at(iClientNumber)->m_iNrATI, iGpuDevice, iRealDevice, fTemperature);m_pLog->m_pText = cBufferLogging;
							m_pLoggingDialog->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLog, LOGGING_DEBUG);
						}
					}
					else		fTemperature = (float) m_lRpcInfoClient.at(iClientNumber)->m_fTopCpuTemperatureCopy;

					if (fTemperature >=0)
					{
						_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.1f °C",fTemperature);
						if (cComma != '.')
						{
							char *cFind = strchr(cBuffer, '.'); 
							if (cFind) *cFind = cComma;
						}
					}
					else
					{
						_snprintf_s(&cBuffer[0],50,_TRUNCATE,"??");
					}

				}
			}

		}
		pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	}
	if (iSortingColumnPrimary == COLUMN_TEMPERATURE) pData->m_iSortPrimary = (int) (fTemperature*10);
	if (iSortingColumnSecondary == COLUMN_TEMPERATURE) pData->m_iSortSecondary = (int) (fTemperature*10);
	if (iSortingColumnTertiary == COLUMN_TEMPERATURE) pData->m_iSortTertiary = (int) (fTemperature*10);
// -------------------------------- Temperature

// -------------------------------- Share

	
	iColumn = m_iColumnOrder[COLUMN_SHARE];
	if (iColumn >= 0)
	{
		double dShare = pInfo->m_dShare;
		//	dPerc = (dShare * 100) / m_dShareTotal;
		//	cBuffer[0] = PERC_CHAR;
		//	cBuffer[1] = 2;
		//	cBuffer[2] = (char)dPerc;
		//	cBuffer[2] += 1;

		CNumberFormat fNumber;
		CString sNumber = fNumber.FormatNumber(dShare, 0);
		_snprintf_s(&cBuffer[0], 50, _TRUNCATE, "%s", sNumber);
		pListCtrl->SetItemText(iRow, iColumn, cBuffer);

		if (iSortingColumnPrimary == COLUMN_SHARE) pData->m_fSortPrimary = (float)pInfo->m_dShare;
		if (iSortingColumnSecondary == COLUMN_SHARE) pData->m_fSortSecondary = (float)pInfo->m_dShare;
		if (iSortingColumnTertiary == COLUMN_SHARE) pData->m_fSortTertiary = (float)pInfo->m_dShare;
	}
	// -------------------------------- Share

	pData->m_iComputerId = iComputerId;

	pListCtrl->SetItem(&lvi);
}

void CTemplateRemoteTaskView::CheckWarning(COLORREF *pColorSet, COLORREF *pColorCpu, COLORREF *pColorGpu,
											double fRatioPerc, double fCpuUse, double fGpuUse,
											double fWarningCpuLow1, double fWarningCpuHigh1,
											double fWarningGpuLow1, double fWarningGpuHigh1,
											double fWarningPercLow1,double fWarningPercHigh1)
{
	if (fGpuUse >= fWarningGpuLow1)
	{
		if (fGpuUse <= fWarningGpuHigh1)
		{
			if (fCpuUse >= fWarningCpuLow1)
			{
				if (fCpuUse <= fWarningCpuHigh1)
				{
					if (fRatioPerc >= fWarningPercLow1)
					{
						if (fRatioPerc <= fWarningPercHigh1)
						{
							// all rules check = warning color
							if (fGpuUse > 0)
							{
								*pColorSet = *pColorGpu;
							}
							else
							{
								*pColorSet = *pColorCpu;
							}
						}
					}
				}
			}
		}
	}
}

bool CTemplateRemoteTaskView::CheckCheckpointWarning(double dCheckpointRelative, char *pszProject, char *pszApplication, int *piColor)
{
	int	iSize, iPos;
	CCheckpointItem *pCheckpointItem;
	CString	sApplication, sProject;
	bool	bFound, bMatchProject, bMatchApplication;

	bFound = false;

	iSize = (int) m_lCheckpoint.size();

	if (iSize > 0)
	{
		sProject = pszProject;
		sProject.MakeLower();
		sApplication = pszApplication;
		sApplication.MakeLower();
	}
	for (iPos = 0; iPos < iSize; iPos++)	
	{
		pCheckpointItem = m_lCheckpoint.at(iPos);
		bMatchProject = false;
		bMatchApplication = false;
		if (sProject.Find(pCheckpointItem->m_sProject) >= 0)
		{
			bMatchProject = true;
		}
		if (sApplication.Find(pCheckpointItem->m_sApplication) >= 0)
		{
			bMatchApplication = true;
		}

		if (bMatchProject && bMatchApplication)
		{
			if (dCheckpointRelative >= pCheckpointItem->m_iTime)
			{
				*piColor = pCheckpointItem->m_crColor;
				bFound = true;
			}
		}
	}
	return bFound;
}

void CTemplateRemoteTaskView::CheckPointAddCounter(CString *psTxt, int iCheckPoint, char *pszProject, char *pszApp, char *pszName)
{
	int		iPosHistory, iPosCheck;
	CCheckpointHistoryItem *pCheckpointHistoryItem;
	CCheckpointSuspend *pCheckpointSuspend;
	CString  sFormat;
	int		iCheckPoints;

	iCheckPoints = 0;

	for (iPosHistory = 0; iPosHistory < (int) m_lCheckpointHistory.size(); iPosHistory++)
	{
		pCheckpointHistoryItem = m_lCheckpointHistory.at(iPosHistory);
		if (pCheckpointHistoryItem->m_sName == pszName)
		{
			if (pCheckpointHistoryItem->m_sProject == pszProject)
			{
				if (pCheckpointHistoryItem->m_sApplication == pszApp)
				{
					iCheckPoints =  pCheckpointHistoryItem->m_iCheckpoints;
					if (pCheckpointHistoryItem->m_iCheckpoint < iCheckPoint)
					{
						iCheckPoints++;
//						pCheckpointHistoryItem->m_iCheckpoints = iCheckPoints;

						// check in the suspend on checkpoint list.
						// this is a duplicate from the history check, but this one is quicker if the tasks tab is visible.
						for (iPosCheck = 0; iPosCheck < (int) m_lCheckpointSuspend.size(); iPosCheck++)
						{
							if (m_lCheckpointSuspend.at(iPosCheck)->m_sName == pCheckpointHistoryItem->m_sName)
							{
								if (m_lCheckpointSuspend.at(iPosCheck)->m_sProject == pCheckpointHistoryItem->m_sProject)
								{
									CheckPointSuspend(iPosHistory, iPosCheck);
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	for (iPosCheck = 0; iPosCheck < (int) m_lCheckpointSuspend.size(); iPosCheck++)
	{
		pCheckpointSuspend = m_lCheckpointSuspend.at(iPosCheck);
		if (pCheckpointSuspend->m_sName == pszName)
		{
			if (pCheckpointSuspend->m_sProject == pszProject)
			{
				*psTxt += "=";
			}
		}
	}


	sFormat.Format("[%d] ", iCheckPoints);
	*psTxt = *psTxt + sFormat;
}

void CTemplateRemoteTaskView::DisplayFiltered(CListCtrl *pListCtrl, int *piRow, CFilterData *pFilterData, int iComputerClient, char *pcComputerName, int iComputerId)
{
	CRpcTaskInfo		info;
	UINT				iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;
	bool				bSuspended;
	CListData			*pData;

	if (pFilterData->m_bValid)
	{
		m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);
		// now add the addional filtered combined results
		
		for (int iFilter=0; iFilter < MAX_FILTER_TYPE; iFilter++)
		{
			int iTotalVersion = pFilterData->m_iTotalVersion[iFilter];

			int iStatus = STATE_WAITING_TO_RUN;
			bSuspended = false;
			switch (iFilter)
			{
				case FILTER_READY_TO_REPORT:
					iStatus = STATE_READY_TO_REPORT;
				break;
				case FILTER_READY_TO_START:
					iStatus = STATE_READY_TO_START;
				break;
				case FILTER_DOWNLOAD:
					iStatus = STATE_DOWNLOADING;
				break;
				case FILTER_UPLOAD:
					iStatus = STATE_UPLOADING;
				break;
				case FILTER_COMPUTATION:
					iStatus = STATE_COMPUTATON_ERROR;
				break;
				case FILTER_ABORTED:
					iStatus = STATE_ABORTED;
				break;
				case FILTER_RUNNING:
					iStatus = STATE_RUNNING;
				break;
				case FILTER_RUNNING_HP:
					iStatus = STATE_RUNNING_HP;
				break;
				case FILTER_SUSPENDED:
					iStatus = STATE_DUMMY_SUSPENDED;		// dummy
					bSuspended = true;
				break;
			}

			for (int iCountVersion = 0; iCountVersion < iTotalVersion; iCountVersion++)
			{
				if (pFilterData->m_iVersionNrCount[iFilter][iCountVersion] == 0) continue;

				info.m_pszProject = pFilterData->m_pszProjectName[iFilter][iCountVersion];
	
				info.m_iDeadlineTime = (int) pFilterData->m_dTimeDeadline[iFilter][iCountVersion];
				info.m_iRemainingTime = pFilterData->m_iTimeRemaining[iFilter][iCountVersion];

				if (bSuspended)	
				{
					info.m_dElapsedTime = pFilterData->m_dElapsedTime[iFilter][iCountVersion];
					info.m_dCurrentCpuTime = pFilterData->m_dCurrentCpuTime[iFilter][iCountVersion];
				}
				else
				{
					if ((iStatus&0xffffe) > STATE_RUNNING)	
					{
						info.m_dElapsedTime = pFilterData->m_iFinalElapsedTime[iFilter][iCountVersion];
						info.m_dCurrentCpuTime = pFilterData->m_dFinalCpuTime[iFilter][iCountVersion];
					}
					else
					{
						info.m_dElapsedTime = pFilterData->m_dElapsedTime[iFilter][iCountVersion];
						info.m_dCurrentCpuTime = pFilterData->m_dCurrentCpuTime[iFilter][iCountVersion];
					}
				}

				info.m_dFinalCpuTime = pFilterData->m_dFinalCpuTime[iFilter][iCountVersion];
				info.m_iFinalElapsedTime = pFilterData->m_iFinalElapsedTime[iFilter][iCountVersion];
				info.m_dReceivedTime = pFilterData->m_dReceivedTime[iFilter][iCountVersion];
				info.m_iExitStatus = pFilterData->m_iExitStatus[iFilter][iCountVersion];

				info.m_iState = iStatus;
				info.m_bSuspendedByGui = bSuspended;
				
				while (*piRow >= (int) m_lListData.size())
				{
					pData = new CListData;
					m_lListData.push_back(pData);
				}

				m_lListData.at(*piRow)->m_i64FilterApplicationAndVersion = pFilterData->m_iApplicationAndVersion[iFilter][iCountVersion];	// unique application id
				__int64 i64Status = iStatus;
				m_lListData.at(*piRow)->m_i64FilterApplicationAndVersion+= i64Status << 32;										// add the filter as well
				info.m_iVersionNumber = pFilterData->m_iVersion[iFilter][iCountVersion];
//				int iPlanClass = info.m_iVersionNumber & 0x3;
//				info.m_iVersionNumber = info.m_iVersionNumber >> FILTER_SHIFT_VERSION;

				info.m_dFraction_done = pFilterData->m_dFraction_done[iFilter][iCountVersion] / pFilterData->m_iVersionNrCount[iFilter][iCountVersion];


				char *pszDescription;
				if (pFilterData->m_iVersionNrCount[iFilter][iCountVersion] == 1)										// only one disable the filtering
				{
					pszDescription = pFilterData->m_pszFilteredLastWU[iFilter][iCountVersion];
					if (pszDescription == NULL)
					{
						pszDescription = pFilterData->m_szDescription[iFilter][iCountVersion];
					}
					info.m_pszPlanClass =  pFilterData->m_pszFilteredPlanClass[iFilter][iCountVersion];
					info.m_dCheckpointCpuTime = pFilterData->m_dCheckpointCpuTime[iFilter][iCountVersion];
//					info.m_dShare = pFilterData->m_dProjectShare[iFilter][iCountVersion];
				}
				else
				{
					info.m_pszPlanClass =  pFilterData->m_pszFilteredPlanClass[iFilter][iCountVersion];
					pszDescription = pFilterData->m_szDescription[iFilter][iCountVersion];
					info.m_dCheckpointCpuTime = -1;																		// invalid checkpoint time
					info.m_iExitStatus = 0;
				}

				info.m_bSuppendedProjectByGui = pFilterData->m_bProjectSuspendedViaGui[iFilter][iCountVersion];

				info.m_pszResources = pFilterData->m_pszFilteredResources[iFilter][iCountVersion];
				info.m_bNonCpuIntensive = pFilterData->m_bNonCpuIntensive[iFilter][iCountVersion];

				info.m_dCpuShortTermDebt = pFilterData->m_dCpuDebt[iFilter][iCountVersion];
				info.m_dCudaShortTermDebt = pFilterData->m_dCudaDebt[iFilter][iCountVersion];
				info.m_dAtiShortTermDebt = pFilterData->m_dAtiDebt[iFilter][iCountVersion];
				info.m_dShare = pFilterData->m_dProjectShare[iFilter][iCountVersion];

				// check if deadline reached

				bool bDeadline;
				int iDeadline;
				iDeadline = theApp.m_pDlgSettingsWarning->m_iDeadlineTime;
				if (iDeadline > 0) 	// deadline - dlgDeadlin - remaining time
				{
					__int64 i64TimeDiff;
					time_t tSystemTime, tDeadline;
					_time64( &tSystemTime);
					tDeadline = info.m_iDeadlineTime - iDeadline*60;
					tDeadline-=  (time_t) pFilterData->m_dTimeRemainingShortestDeadline[iFilter][iCountVersion];;
					i64TimeDiff = tSystemTime - tDeadline;
					if (i64TimeDiff > 0)
					{
						bDeadline = true;
					}
					else bDeadline = false;
				}
				else bDeadline = false;
				info.m_bDeadline = bDeadline;

				info.m_iSuspendStatus = 0;
				info.m_pCc = NULL;
				info.m_pszName = pszDescription;

				info.m_pResult = NULL;

				info.m_pszAccount = pFilterData->m_pszAccountName[iFilter][iCountVersion];
//				info.m_pszProject = pFilterData->m_pszProjectName[iFilter][iCountVersion];
				info.m_pszApp = pFilterData->m_pszUserFriendly[iFilter][iCountVersion];

//				DisplaySingleItem(pListCtrl,*piRow, &info, 0, -1, pcComputerName, iComputerId, true);
				DisplaySingleItem(pListCtrl, *piRow, &info, 0, iComputerClient, pcComputerName, iComputerId, true);
				
				*piRow += 1;
			}
		}
	}
}

void CTemplateRemoteTaskView::DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin)
{
	int			iItemCount;
	bool		bMax;
	CListData	*pData;

	m_bUseSkin = bUseSkin;
	m_iUseSkin = iUseSkin;
	if (!bUseAlternatingLines) return;

	CListCtrl& listCtrl = GetListCtrl();
	iItemCount = listCtrl.GetItemCount();


	for (int iCount = 1; iCount < iItemCount; iCount+=2)
	{
		pData = (CListData*) listCtrl.GetItemData(iCount);

		if (bUseSkin)
		{
			bMax = false;
			if ((pData->m_color&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>8)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>16)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
	
			if (bMax) 	pData->m_color -= RGB(COLOR_OFFSET_MIN,COLOR_OFFSET_MIN,COLOR_OFFSET_MIN);
			else pData->m_color -= RGB(COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING);
		}
		else
		{
			pData->m_color -= RGB(20,20,20);
		}
	}
}

/*
void CTemplateRemoteTaskView::TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore, char *pAfter, bool bShowZero)
{
	int iDay, iHour, iMinute, iSecond;

	if (iTime > 0)
	{
		iMinute = iTime / 60;
		iSecond = iTime - iMinute*60 ;
		iHour	= iMinute / 60;
		iMinute = iMinute - iHour *60;
		iDay	= iHour / 24;
		iHour	= iHour - iDay * 24;

		if (iDay > 0) _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02dd,%02d:%02d:%02d%s",pBefore,iDay,iHour,iMinute,iSecond,pAfter);
		else _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02d:%02d:%02d%s",pBefore, iHour,iMinute,iSecond, pAfter);
	}
 	else
	{
		if (bShowZero) _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02d:%02d:%02d%s",pBefore, 0,0,0, pAfter);
		else *pBuffer = 0;
	}
}
*/

void CTemplateRemoteTaskView::StartRatio()
{
	int		iSize, iPos;
	CRatioItem *pRatioItem;

	m_bRatioLongTerm = theApp.m_pDlgSettingsTasks->m_bRatioLongTerm;

	iSize = (int) m_lRatio.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRatioItem = m_lRatio.at(iPos);
		pRatioItem->bWuFound = false;
	}
}

double CTemplateRemoteTaskView::GetRatio(CRpcTaskInfo *pInfo)
{
	int			iSize, iPos;
	CRatioItem	*pRatioItem;
	bool		bAdd;	//, bFallBackMode;
	double		dRatio, dRatioNew;
	double		dDeltaElapsedTime, dDeltaCurrentCpuTime;
	time_t		tSystemTime;

	_time64( &tSystemTime);
	dRatio = 0;
	bAdd = true;
	iSize = (int) m_lRatio.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRatioItem = m_lRatio.at(iPos);
		if (pRatioItem->m_sProject == pInfo->m_pszProject)
		{
			if (pRatioItem->m_sWu == pInfo->m_pszName)
			{
				// already there
				bAdd = false;
				pRatioItem->bWuFound = true;
				dRatio = pRatioItem->m_dRatio;
				if (dRatio >10000) dRatio = 100;
				if (dRatio < 0) dRatio = 0;

				dDeltaCurrentCpuTime = pInfo->m_dCurrentCpuTime - pRatioItem->m_dCurrentCpuTime10;

				dDeltaElapsedTime = pInfo->m_dElapsedTime - pRatioItem->m_dElapsedTime10;

				if (pInfo->m_dCurrentCpuTime == pInfo->m_dElapsedTime)
				{
					pRatioItem->m_iFallBackMode++;
					if (pRatioItem->m_iFallBackMode > 1)
					{
						pRatioItem->m_iFallBackMode = 3;
						// do our own elapsed time measurement
						int iDeltaTime = (int) (tSystemTime - pRatioItem->m_tLastSystemTime10);
						dDeltaElapsedTime = iDeltaTime;
					}
				}
				else pRatioItem->m_iFallBackMode = 0;


				if (dDeltaElapsedTime > 0)
				{
					dRatioNew = dDeltaCurrentCpuTime;
					dRatioNew /= dDeltaElapsedTime;
				}
				else dRatioNew = 1;
				dRatioNew = (floor(dRatioNew *100000) / 1000);
				dRatio = dRatioNew;


				pRatioItem->m_dRatio = dRatio;

				pRatioItem->m_iSampleCount++;

				int iMaxSampleCount = 5;
				
				if (pRatioItem->m_iSampleCount > iMaxSampleCount)
				{
					pRatioItem->m_dCurrentCpuTime10 = pRatioItem->m_dCurrentCpuTime5;
					pRatioItem->m_dElapsedTime10 = pRatioItem->m_dElapsedTime5;

					pRatioItem->m_dCurrentCpuTime5 = pRatioItem->m_dCurrentCpuTime;
					pRatioItem->m_dElapsedTime5 = pRatioItem->m_dElapsedTime;

					pRatioItem->m_dCurrentCpuTime = pInfo->m_dCurrentCpuTime;
					pRatioItem->m_dElapsedTime = pInfo->m_dElapsedTime;

					pRatioItem->m_tLastSystemTime10 = pRatioItem->m_tLastSystemTime5;
					pRatioItem->m_tLastSystemTime5 = pRatioItem->m_tLastSystemTime;
					pRatioItem->m_tLastSystemTime = tSystemTime;

					pRatioItem->m_iSampleCount = 0;//iMaxSampleCount;					
				}
			}
		}
	}
	if (bAdd)
	{
		// not found add it
		pRatioItem = new CRatioItem;
		pRatioItem->bWuFound = true;
		pRatioItem->m_sProject = pInfo->m_pszProject;
		pRatioItem->m_sWu = pInfo->m_pszName;
		if (pInfo->m_dElapsedTime > 0)
		{
			dRatio = pInfo->m_dCurrentCpuTime;
			dRatio /= pInfo->m_dElapsedTime;
		}
		else dRatio = 1;
		dRatio *= 100;	
		pRatioItem->m_dRatio = dRatio;
		pRatioItem->m_iSampleCount = 1;
		pRatioItem->m_dCurrentCpuTime = pInfo->m_dCurrentCpuTime;
		pRatioItem->m_dElapsedTime = pInfo->m_dElapsedTime;
		pRatioItem->m_dCurrentCpuTime5 = pInfo->m_dCurrentCpuTime;
		pRatioItem->m_dCurrentCpuTime10 = pInfo->m_dCurrentCpuTime;
		pRatioItem->m_dElapsedTime5 = pInfo->m_dElapsedTime;
		pRatioItem->m_dElapsedTime10 = pInfo->m_dElapsedTime;

		pRatioItem->m_iFallBackMode = 0;
		pRatioItem->m_tLastSystemTime = tSystemTime;
		pRatioItem->m_tLastSystemTime5 = tSystemTime;
		pRatioItem->m_tLastSystemTime10 = tSystemTime;

		m_lRatio.push_back(pRatioItem);
	}

	return dRatio;
}

void CTemplateRemoteTaskView::StopRatio()
{
	int			iPos;
	CRatioItem *pRatioItem;

	// remove all unused (not running) wu's
	bool bRemoved = true;
	while (bRemoved)
	{
		bRemoved = false;
		for (iPos = 0; iPos < (int) m_lRatio.size(); iPos++)
		{
			pRatioItem = m_lRatio.at(iPos);
			if (!pRatioItem->bWuFound)
			{
				delete pRatioItem;
				m_lRatio.erase(m_lRatio.begin() + iPos);
				bRemoved = true;
				break;
			}
		}
	}
}

void CTemplateRemoteTaskView::TaskPropertiesGet(int iBusy)
{
//	bool bFound;
	int iCount;

	// first get the thread list and process all threads

	m_iTaskPropertiesThreadsPostedCount = 0;
	m_iTaskPropertiesThreadsReturnCount = 0;

	for (iCount = 0; iCount < (int) m_lListTasksProperties.size(); iCount++)
	{
		::PostThreadMessage(m_lListTasksProperties.at(iCount)->m_iThreadId,UWM_MSG_THREAD_TASK_READ,(WPARAM) this->m_hWnd, (LPARAM) m_lListTasksProperties.at(iCount));
		m_iTaskPropertiesThreadsPostedCount ++;
		m_iTaskPropertiesBusy = iBusy;
	}
}

void CTemplateRemoteTaskView::DeletePropertiesList()
{
	for (int iCount = 0; iCount < (int) m_lListTasksProperties.size(); iCount++)
	{
		delete m_lListTasksProperties.at(iCount);
	}
	m_lListTasksProperties.clear();
}

bool CTemplateRemoteTaskView::DoubleClick(int iRow, int iControlKey)
{
	CString		sComputer, sStatus,	sApplication;
	CListData	*pData;
	int			iFilter;
	int			iThreadId;
	int			iFilterApplicationAndVersion;

	CListCtrl& listCtrl = GetListCtrl();

	m_pSelection->SelectionRemoveAll();

	sComputer = listCtrl.GetItemText(iRow,m_iColumnOrder[COLUMN_COMPUTER]);
	sStatus = listCtrl.GetItemText(iRow,m_iColumnOrder[COLUMN_STATE]);
	sApplication = listCtrl.GetItemText(iRow,m_iColumnOrder[COLUMN_APPLICATION]);

	iFilter = FILTER_NONE;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusReadyTR],0) >= 0)		iFilter = FILTER_READY_TO_REPORT;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusWaitingTR],0) >= 0)	iFilter = FILTER_WAITING_TO_RUN;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusReadyTS],0) >= 0)		iFilter = FILTER_READY_TO_START;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusDownloading],0) >= 0)	iFilter = FILTER_DOWNLOAD;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusUploading],0) >= 0)	iFilter = FILTER_UPLOAD;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusComputation],0) >= 0)	iFilter = FILTER_COMPUTATION;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusAborted],0) >= 0)		iFilter = FILTER_ABORTED;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusSuspended],0) >= 0)	iFilter = FILTER_SUSPENDED;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusRunning],0) >= 0)		iFilter = FILTER_RUNNING;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusRunningHP],0) >= 0)	iFilter = FILTER_RUNNING;
	if (sStatus.Find(gszTranslation[PosViewTasksStatusRunningLP],0) >= 0)	iFilter = FILTER_RUNNING;

	iFilterApplicationAndVersion = 0;
	if (iFilter != FILTER_NONE)
	{
		m_pDoc->GetRpcThreadId(&sComputer, &iThreadId);
		{
			pData = (CListData *) listCtrl.GetItemData(iRow);
			iFilterApplicationAndVersion = (int) pData->m_i64FilterApplicationAndVersion;
		}
		CRpcFilterInfo *pFilterInfo = new CRpcFilterInfo;
		pFilterInfo->m_iFilter = iFilter;
		pFilterInfo->m_iFilterApplicationAndVersion = iFilterApplicationAndVersion;
		pFilterInfo->m_sApplication = sApplication;

		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_FILTER_TASK, iControlKey,(LPARAM) pFilterInfo);
		return true;
//		m_tLastTasksShown = 0;				// 1000 to ensure direct timer message
	}
	return false;
}

// messages

LRESULT CTemplateRemoteTaskView::OnHistoryTasks(WPARAM parm1, LPARAM parm2)
{
//	bool			bFound;
	UINT			iPos, iColumn;
	CHistoryItem	*pHistoryItemFrom;
	CCheckpointHistoryItem *pCheckpointHistoryItem;

	std::deque<CHistoryRpc*>		*plHistory;

	for (iPos = 0; iPos < (int) m_lCheckpointHistory.size(); iPos++)
	{
		delete m_lCheckpointHistory.at(iPos);
	}
	m_lCheckpointHistory.clear();

	iColumn = m_iColumnOrder[COLUMN_CHECKPOINT];	
	if (iColumn < 0) return 0; // if there is no checkpoint colomn shown no need to do this

	plHistory = (std::deque<CHistoryRpc*> *) parm2;

	for (int iComputer = 0; iComputer < (int) plHistory->size(); iComputer++)
	{
		for (iPos = 0; iPos < plHistory->at(iComputer)->m_lHistory.size(); iPos++)
		{
			pHistoryItemFrom = plHistory->at(iComputer)->m_lHistory.at(iPos);
			if ((pHistoryItemFrom->m_iState&0xfffffe) == STATE_RUNNING)						// only fill the running
			{
				pCheckpointHistoryItem = new CCheckpointHistoryItem;
				pCheckpointHistoryItem->m_sProject = pHistoryItemFrom->m_pcProject;
				pCheckpointHistoryItem->m_sApplication = pHistoryItemFrom->m_pcApplication;
				pCheckpointHistoryItem->m_sName = pHistoryItemFrom->m_pcName;
				pCheckpointHistoryItem->m_iCheckpoint = pHistoryItemFrom->m_iCheckpoint;
				pCheckpointHistoryItem->m_iCheckpoints = pHistoryItemFrom->m_iCheckpoints;
				m_lCheckpointHistory.push_back(pCheckpointHistoryItem);
			}
		}
	}

	// checkpoint suspend valid

	for (iPos = 0; iPos < (int) m_lCheckpointSuspend.size(); iPos++)
	{
		m_lCheckpointSuspend.at(iPos)->m_bUsed = false;
		for (int iPosHistory = 0; iPosHistory < (int) m_lCheckpointHistory.size(); iPosHistory++)
		{
			if (m_lCheckpointSuspend.at(iPos)->m_sName == m_lCheckpointHistory.at(iPosHistory)->m_sName)
			{
				if (m_lCheckpointSuspend.at(iPos)->m_sProject == m_lCheckpointHistory.at(iPosHistory)->m_sProject)
				{
					CheckPointSuspend(iPosHistory, iPos);
					m_lCheckpointSuspend.at(iPos)->m_bUsed = true;
					break;
				}
			}
		}
	}

	for (iPos = 0; iPos < (int) m_lCheckpointSuspend.size(); iPos++)
	{
		if ((m_lCheckpointSuspend.at(iPos)->m_bUsed == false) || (m_lCheckpointSuspend.at(iPos)->m_bDelete))
		{
			delete m_lCheckpointSuspend.at(iPos);
			m_lCheckpointSuspend.erase(m_lCheckpointSuspend.begin()+iPos);
		}
	}

	return 0;
}

void CTemplateRemoteTaskView::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pSortColumn->ClickItem(pNMHDR, pResult);
	m_tLastTasksShown = 0;				// 1000 to ensure direct timer message
}


void CTemplateRemoteTaskView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListCtrl& listCtrl = GetListCtrl();

	switch (nChar)
	{
		case VK_END:
			m_pSelection->SelectionEnd(&listCtrl);	
		break;
		case VK_HOME:
			m_pSelection->SelectionBegin(&listCtrl);
		break;
	}

//	if (nChar == VK_END)
//	{
//		m_pSelection->SelectionEnd(&listCtrl);		
//	}
//	if (nChar == VK_HOME)
//	{
//		m_pSelection->SelectionBegin(&listCtrl);		
//	}

	if (m_pSelection->SelectionNumber() != 0)
	{
		if (nChar == VK_DOWN)
		{
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_NAME],m_iColumnOrder[COLUMN_COMPUTER],1);
			return;
		}
		if (nChar == VK_UP)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_NAME],m_iColumnOrder[COLUMN_COMPUTER],1);
			return;
		}
		if (nChar == VK_NEXT)
		{
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_NAME],m_iColumnOrder[COLUMN_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
		if (nChar == VK_PRIOR)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_NAME],m_iColumnOrder[COLUMN_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
	}

	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteTaskView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListViewEx::OnKeyUp(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteTaskView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl& listCtrl = GetListCtrl();
	int row = listCtrl.HitTest( point, NULL );

	m_pSelection->SelectionSet(row);

	CListViewEx::OnLButtonDown(nFlags, point);
}

void CTemplateRemoteTaskView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListCtrl& listCtrl = GetListCtrl();

	int iRow = listCtrl.HitTest( point, NULL );
	int iControlKey = GetKeyState(VK_LCONTROL) &0x80;

	DoubleClick(iRow, iControlKey);
	m_tLastTasksShown = 0;				// 1000 to ensure direct timer message
}

void CTemplateRemoteTaskView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl& listCtrl = GetListCtrl();
	int row = listCtrl.HitTest( point, NULL );
	m_pSelection->SelectionSetRight(row);
	
	CListViewEx::OnRButtonDown(nFlags, point);
}

BOOL CTemplateRemoteTaskView::OnHeaderEndDrag(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER	*pNMH ;

	pNMH  = (NMHEADER *) pNMHDR;

	if (pNMH->pitem->mask & HDI_ORDER)
	{
		CListCtrl& listCtrl = GetListCtrl();
		int iw = listCtrl.GetColumnWidth(0); 
		listCtrl.PostMessage(LVM_SETCOLUMNWIDTH, 0, (LPARAM) iw); 
		m_bDraged = true;
	}

	return FALSE;
}

BOOL CTemplateRemoteTaskView::OnHeaderChanged(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	int  iCountHeader;
	int  iCount, iDelta, iMove1;
	int  iOrder[50];
	int	 iNumber, iColumnHeader[NUM_REMOTE_TASK_COLUMNS+1], iColumnHeaderAfter[NUM_REMOTE_TASK_COLUMNS+1];;

	if (!m_bDraged) return 0;
	m_bDraged = false;
	iDelta = -1;

	CListCtrl& listCtrl = GetListCtrl();	
	CHeaderCtrl* pHeaderCtrl = listCtrl.GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		iCountHeader = pHeaderCtrl->GetItemCount();

		LPINT pnOrder = (LPINT) malloc(iCountHeader*sizeof(int));
		ASSERT(pnOrder != NULL);
		if (NULL != pnOrder)
		{
			pHeaderCtrl->GetOrderArray(pnOrder, iCountHeader);
			for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iOrder[iCount] = pnOrder[iCount]; 
			}

			m_pSortColumn->OrderChanged(iCountHeader, pnOrder);

			for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
			{
				iNumber = m_iColumnOrder[iCount];
				if (iNumber > NUM_REMOTE_TASK_COLUMNS) break;
				if (iNumber >=0) iColumnHeader[iNumber] = iCount;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iMove1 = pnOrder[iCount];
				iColumnHeaderAfter[iCount] = iColumnHeader[iMove1];
			}

			for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
			{
				m_iColumnOrder[iCount] = -1;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iNumber = iColumnHeaderAfter[iCount];
				if (iNumber > NUM_REMOTE_TASK_COLUMNS) break;
				if (iNumber >=0) m_iColumnOrder[iNumber] = iCount;
			}


	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				pnOrder[iCount] = iCount; 
			}
			pHeaderCtrl->SetOrderArray(iCountHeader, pnOrder);	// set to 0,1,2, again
			free(pnOrder);
         }
	}

	AddAllColumns(false);
	listCtrl.Invalidate(TRUE);
	m_tLastTasksShown = 0;				// 1000 to ensure direct timer message
	return FALSE;
}


LRESULT CTemplateRemoteTaskView::OnSettingsApplyChangesTasks(WPARAM parm1, LPARAM parm2)
{
	m_bApplyChangesOnColumn = true;

	CListCtrl& listCtrl = GetListCtrl();

	// save all column width

	CheckAndSaveColumnsWidth();

	// clear list
	listCtrl.DeleteAllItems();

	// delete all columns
	for (int iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		if (listCtrl.DeleteColumn(0) != TRUE) break;
	}

	// add all columns

	AddAllColumns();


	// read back the column width
//	ReadAllColumnsWidth();			no no add is now modify

	listCtrl.Invalidate(TRUE);

	return 0;
}


LRESULT CTemplateRemoteTaskView::OnTaskPropertiesReady(WPARAM parm1, LPARAM parm2)
{
	int iCount;
	CCreateHtmlWebServerProperties createHtmlWebServerProperties;

	m_iTaskPropertiesThreadsReturnCount++;
	if (m_iTaskPropertiesThreadsReturnCount != m_iTaskPropertiesThreadsPostedCount)
	{
		return 0; // wait for the rest
	}

	// got them all.
	if (m_iTaskPropertiesBusy == PROPERTIES_DIALOG)	TaskPropertiesReady(NULL);
	else											TaskPropertiesReady(&createHtmlWebServerProperties);

	for (iCount = 0; iCount < (int) m_lListTasksProperties.size() ; iCount++)
	{
		delete m_lListTasksProperties.at(iCount);
	}
	m_lListTasksProperties.clear();

	m_iTaskPropertiesBusy = PROPERTIES_READY;
	return 0;
}

void CTemplateRemoteTaskView::PropertiesAddRow(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties, CString *psColumn0, CString *psColumn1)
{
	if (pCreateHtmlWebServerProperties)
	{
		pCreateHtmlWebServerProperties->Add(psColumn0, psColumn1);
	}
	else theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) psColumn0, (LPARAM) psColumn1);
}

void CTemplateRemoteTaskView::TaskPropertiesReady(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties)
{
	bool		bFirstTime = true;
	CString		sComputer;
	CString		sColumn0, sColumn1;
	
	if (pCreateHtmlWebServerProperties == NULL)
	{
		theApp.m_pDlgProperties->SetWindowText(gszTranslation[PosDialogTaskPropTitleProjectP]);
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_SETUP,2,(LPARAM) registryPropertiesTask);	// 2 columns
	}

	for (int iSelected = 0; iSelected < (int) m_lListTasksProperties.size(); iSelected++)
	{
		if (m_lListTasksProperties.at(iSelected)->m_bValid)
		{
			if (pCreateHtmlWebServerProperties == NULL)
			{
				if (!bFirstTime) {sColumn0 = "=================================================";sColumn1 = sColumn0; theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);}
			}
			else
			{
				if (!bFirstTime) {sColumn0 = "============";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);}
			}
			bFirstTime = false;
			if (sComputer != m_lListTasksProperties.at(iSelected)->m_sComputer)
			{
				sComputer = m_lListTasksProperties.at(iSelected)->m_sComputer;
				sColumn0 = gszTranslation[PosDialogTaskPropComputer];
				sColumn1 = sComputer;
				PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
			}
			sColumn0 = gszTranslation[PosDialogTaskPropProject]; sColumn1 = m_lListTasksProperties.at(iSelected)->m_sProject; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
			sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

			TaskProperties(m_lListTasksProperties.at(iSelected), pCreateHtmlWebServerProperties);
		}
	}
	if (pCreateHtmlWebServerProperties == NULL)
	{
		sColumn0 = "";sColumn1 = sColumn0; theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);	// extra empty line is needed otherwise the last item has the wrong length.
	}
	if (pCreateHtmlWebServerProperties) pCreateHtmlWebServerProperties->Finish();
}

void CTemplateRemoteTaskView::TaskProperties(CTasksProperties *pTasksProperties, CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties)
{
	CString			sFormat, sColumn0, sColumn1;
	char			buffer[256];
	CDateConvert	dateConvert;
	CTimeString		timeString;
	bool			bCompleted;
	float			fVersion;
	double			dPerc;
	CNumberFormat	numberFormat;

	bCompleted = false;
	sColumn0 = gszTranslation[PosDialogTaskPropName]; sColumn1 = pTasksProperties->m_pcName; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = ""; sColumn1 = ""; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	fVersion = (float) pTasksProperties->m_iVersion;
	fVersion/= 100;
	sFormat.Format(" %.2f", fVersion);
	sFormat = pTasksProperties->m_pcApplicationName + sFormat;
	if (strlen(pTasksProperties->m_pcPlanClass) > 0)
	{
		sFormat += " (";
		sFormat += pTasksProperties->m_pcPlanClass;
		sFormat += ")";
	}
	sColumn0 = gszTranslation[PosDialogTaskPropApplication]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogTaskPropWorkunitName]; sColumn1 = pTasksProperties->m_pcWuName; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	dPerc = (double) pTasksProperties->m_dFractionDone *100;
	if (dPerc < 0.01)
	{
		if (pTasksProperties->m_dEstimateCpuTimeRemaining == 0)
		{
			dPerc = 100;
		}
	}

	switch (pTasksProperties->m_iState)
	{
		case	STATE_NEW:
		case	STATE_NEW_HP:
			sFormat = gszTranslation[PosViewTasksStatusNew];
		break;
		case	STATE_DOWNLOADING:
		case	STATE_DOWNLOADING_HP:
			if (dPerc >= 100)	// 100% download is an error
			{
				sFormat = gszTranslation[PosViewTasksStatusDownloadError];
			}
			else
			{
				sFormat = gszTranslation[PosViewTasksStatusDownloading];
			}
		break;	
		case	STATE_READY_TO_START:
		case	STATE_READY_TO_START_HP:
			sFormat = gszTranslation[PosViewTasksStatusReadyTS];
		break;	
		case	STATE_WAITING_TO_RUN:
		case	STATE_WAITING_TO_RUN_HP:
			sFormat = gszTranslation[PosViewTasksStatusWaitingTR];
		break;
		case	STATE_RUNNING:
			sFormat = gszTranslation[PosViewTasksStatusRunning];
		break;	
		case	STATE_RUNNING_HP:
			sFormat = gszTranslation[PosViewTasksStatusRunningHP];
		break;
		case	STATE_COMPUTATON_ERROR:
			bCompleted = true;
			sFormat = gszTranslation[PosViewTasksStatusComputation];
		break;	
		case	STATE_UPLOADING:
			bCompleted = true;
			sFormat = gszTranslation[PosViewTasksStatusUploading];
		break;	
		case	STATE_READY_TO_REPORT:
			bCompleted = true;
			sFormat = gszTranslation[PosViewTasksStatusReadyTR];
		break;	
		case	STATE_ABORTED:
		case	STATE_ABORTED_HP:
		case	STATE_ABORTED1:
		case	STATE_ABORTED2:
		case	STATE_ABORTED2_HP:
			bCompleted = true;
			sFormat = gszTranslation[PosViewTasksStatusAborted];
		break;	
		default:
			sFormat.Format("%x ??", pTasksProperties->m_iState);
	}
	// suspend

	bool bSuspendReason;
	int iSuspendReason;
	CString sSuspendReason;

	iSuspendReason = pTasksProperties->m_iSuspendReason;

	if (iSuspendReason)
	{
		bSuspendReason = true;

		if (iSuspendReason & SUSPEND_REASON_BATTERIES)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendOnBatt];}			//"on batteries";}
		if (iSuspendReason & SUSPEND_REASON_USER_ACTIVE)			{ sSuspendReason = gszTranslation[PosViewTasksSuspendUserAct];}			//"user active";}
		if (iSuspendReason & SUSPEND_REASON_USER_REQ)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendUserRq];}			//"user request";}
		if (iSuspendReason & SUSPEND_REASON_TIME_OF_DAY)			{ sSuspendReason = gszTranslation[PosViewTasksSuspendTimeOD];}			//"time of day";}
		if (iSuspendReason & SUSPEND_REASON_BENCHMARKS)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendBench];}			//"benchmarks";}
		if (iSuspendReason & SUSPEND_REASON_DISK_SIZE)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendDisk];}			//"disk size";}
		if (iSuspendReason & SUSPEND_REASON_CPU_THROTTLE)			{ sSuspendReason = "";bSuspendReason = false;}							// treat as not suspended
		if (iSuspendReason & SUSPEND_REASON_NO_RECENT_INPUT)		{ sSuspendReason = gszTranslation[PosViewTasksSuspendNoRI];}			//"no recent input";}
		if (iSuspendReason & SUSPEND_REASON_INITIAL_DELAY)			{ sSuspendReason = gszTranslation[PosViewTasksSuspendInitial];}			//"initial";}
		if (iSuspendReason & SUSPEND_REASON_EXCLUSIVE_APP_RUNNING)	{ sSuspendReason = gszTranslation[PosViewTasksSuspendEx];}				//"exclusive app running";}
		if (iSuspendReason & SUSPEND_REASON_CPU_USAGE)				{ sSuspendReason = gszTranslation[PosViewTasksSuspendCpuUsage];}		//"cpu usage";}

		if (sSuspendReason == "")
		{
			if (bSuspendReason)
			{
				sSuspendReason.Format("Unknown suspend reason %d", iSuspendReason);
			}
		}
	}
	else
	{
		if (pTasksProperties->m_iSuspendStatus == SUSPEND_STATUS_WAITING_FOR_MEMORY)
		{
			sSuspendReason = gszTranslation[PosViewTasksSuspendMemory];
			bSuspendReason = true;
		}
		else
		{
			if (pTasksProperties->m_iSuspendStatus != 0)
			{
				sSuspendReason.Format("Unknown suspend status %d", pTasksProperties->m_iSuspendStatus);
			}
			bSuspendReason = false;
		}
	}

	if (pTasksProperties->m_bSuspendedViaGui)
	{
		sFormat += ", ";
		bSuspendReason = false;
		sFormat += gszTranslation[PosViewTasksStatusSuspended];
	}
	if (pTasksProperties->m_bProjectSuspendedViaGui)
	{
		sFormat += ", ";
		bSuspendReason = false;
		sFormat += gszTranslation[PosViewTasksSuspendProject];
	}

	if (bSuspendReason)
	{
		sFormat += ", ";
		sFormat += gszTranslation[PosViewTasksSuspendPrefix];
		sFormat += sSuspendReason;
	}

	// suspend


	sColumn0 = gszTranslation[PosDialogTaskPropState]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	dateConvert.Convert((time_t)pTasksProperties->m_dRecieved, buffer);
	sFormat = buffer;
	sColumn0 = gszTranslation[PosDialogTaskPropRecieved]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	dateConvert.Convert((time_t) pTasksProperties->m_dDeadline, buffer);
	sFormat = buffer;
	sColumn0 = gszTranslation[PosDialogTaskPropDeadline]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	if ( pTasksProperties->m_dFlops > 0)
	{
		sFormat = numberFormat.FormatNumber(pTasksProperties->m_dFlops/1e9,2);
//		sFormat.Format("%.2f ", );
		sFormat += " GFLOPs/sec";
		sColumn0 = gszTranslation[PosDialogTaskPropEstAppSpeed];sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}

	if (pTasksProperties->m_dRscFpopsEst > 0)
	{
		sFormat = numberFormat.FormatNumber(pTasksProperties->m_dRscFpopsEst/1e9,0);
//		sFormat.Format("%.0f ", );
		sFormat += " GFLOPs";
		sColumn0 = gszTranslation[PosDialogTaskPropEstTaskSize];sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}

	char cComma = theApp.m_pDlgSettingsView->m_cComma;
	sColumn1 = pTasksProperties->m_pcResources;
	if (cComma != '.') sColumn1.Replace('.',cComma);
	if (sColumn1.GetLength() > 0)
	{
		sColumn0 = gszTranslation[PosDialogTaskPropResources] ; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}

	timeString.TimeString((int)pTasksProperties->m_dCheckpointCpuTime, buffer, 100, "", "",true,theApp.m_bNoCleanup);
	sFormat = buffer;
	sColumn0 = gszTranslation[PosDialogTaskPropCheckpointTime]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	timeString.TimeString((int)pTasksProperties->m_dCurrentCpuTime, buffer, 100, "", "",true);
	sFormat = buffer;
	sColumn0 = gszTranslation[PosDialogTaskPropCpuTime]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	timeString.TimeString((int)pTasksProperties->m_dElapsedTime, buffer, 100, "", "",true,theApp.m_bNoCleanup);
	sFormat = buffer;
	sColumn0 = gszTranslation[PosDialogTaskPropElapsedTime]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	timeString.TimeString((int)pTasksProperties->m_dEstimateCpuTimeRemaining, buffer, 100, "", "",true,theApp.m_bNoCleanup);
	sFormat = buffer;
	sColumn0 = gszTranslation[PosDialogTaskPropEstTimeRemaining]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	if (bCompleted) sFormat = "100";
	else sFormat = numberFormat.FormatNumber(pTasksProperties->m_dFractionDone*100, 3);
//	else sFormat.Format("%0.3f %%", pTasksProperties->m_dFractionDone*100);
	sFormat += "%";
	sColumn0 = gszTranslation[PosDialogTaskPropFractionDone]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

//	sFormat.Format("%0.2f MB",pTasksProperties->m_dSwapSize/MEGA_BYTE);
	sFormat = numberFormat.FormatNumber(pTasksProperties->m_dSwapSize/MEGA_BYTE, 2);
	sFormat+= " MB";

	sColumn0 = gszTranslation[PosDialogTaskPropVirtualMemory]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

//	sFormat.Format("%0.2f MB",pTasksProperties->m_dWorkingSetSize/MEGA_BYTE);
	sFormat = numberFormat.FormatNumber(pTasksProperties->m_dWorkingSetSize/MEGA_BYTE, 2);
	sFormat+= " MB";

	sColumn0 = gszTranslation[PosDialogTaskPropWorkingSetSize]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	if (pTasksProperties->m_iSlot >= 0)
	{
		sFormat.Format("slots/%d",pTasksProperties->m_iSlot);
		sColumn0 = gszTranslation[PosDialogTaskPropDirectoy]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		sFormat.Format("%d",pTasksProperties->m_iPid);
		sColumn0 = gszTranslation[PosDialogTaskPropPid]; sColumn1 = sFormat; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}

	// empty
	sColumn0 = L""; sColumn1 = L""; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	CString sDebug;
	sDebug.Format("State: %x - Scheduler: %x", pTasksProperties->m_iStateDebug, pTasksProperties->m_iSchedulerDebug);
	sColumn0 = L"Debug"; sColumn1 = sDebug; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

}

void CTemplateRemoteTaskView::ClipBoard(bool bAll)
{
    HGLOBAL		hglbCopy; 
    LPTSTR		lptstrCopy; 
	CString		sTxt,sClipBoard;
	int			iItems, iNr;
	bool		bSelected;
	
	if ( !OpenClipboard() ) return;
	if( !EmptyClipboard() ) return;
 
	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();

	for (int iCountItems = 0; iCountItems < iItems; iCountItems++)
	{
		bSelected = m_pSelection->SelectionSelected(&listCtrl, iCountItems);

		if (bAll) bSelected = true;
		if (bSelected)
		{
			for (int iCountSearch = 0; iCountSearch < NUM_REMOTE_TASK_COLUMNS; iCountSearch++)
			{
				for (int iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
				{
					iNr = m_iColumnOrder[iCount];
					if (iNr > NUM_REMOTE_TASK_COLUMNS) break;
					if (iNr >=0)
					{
						if (iCountSearch == iNr)
						{
							sTxt = listCtrl.GetItemText(iCountItems,iNr);
							RemoveSpecialText(&sTxt);
							sClipBoard += sTxt;
							sClipBoard += "\t";
							break;
						}
					}
				}
			}
			sClipBoard += "\r\n";
		}
	}

	// Allocate a global memory object for the text. 
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (sClipBoard.GetLength() + 1) * sizeof(char)); 
	if (hglbCopy == NULL) 
    { 
		CloseClipboard(); 
        return; 
	} 
 
	// Lock the handle and copy the text to the buffer. 
    lptstrCopy = (LPTSTR) GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, sClipBoard.GetBuffer(), sClipBoard.GetLength() * sizeof(char)); 
	lptstrCopy[sClipBoard.GetLength()] = (char) 0;    // null character 
	GlobalUnlock(hglbCopy); 
 
	// set the locale
	HANDLE hLC=GlobalAlloc(GMEM_MOVEABLE,sizeof(LCID));
	if (hLC)
	{
		PLCID pLc=(PLCID)GlobalLock(hLC);
		if (pLc)
		{
			*pLc = theApp.m_pTranslation->m_lcid;
			GlobalUnlock(hLC);
			if (!SetClipboardData(CF_LOCALE, pLc))	GlobalFree(hLC);
		}
		else
		{
			GlobalFree(hLC);
		}
	}

	// Place the handle on the clipboard. 
	SetClipboardData(CF_TEXT, hglbCopy); 

	CloseClipboard();
}

LRESULT CTemplateRemoteTaskView::OnCloudColumn(WPARAM parm1, LPARAM parm2)
{
	CString *pColumn;

	pColumn = (CString *) parm2;
	m_sCloudColumn = *pColumn;
	delete pColumn;
	return 0;
}




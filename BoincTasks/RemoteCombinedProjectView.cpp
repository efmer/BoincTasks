// RemoteCombinedProjectsView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
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
#include "RemoteCombinedDoc.h"

#include "DlgSettingsMain.h"
#include "DlgSettingsView.h"
#include "DlgSettingsProject.h"
#include "DlgSettingsExpert.h"
#include "DlgProperties.h"
#include "DlgPrefMain.h"
#include "DlgProxyMain.h"
#include "DlgColorMain.h"
#include "DlgSetDebt.h"
#include "DlgAddProject.h"
#include "DlgAddAccountManager.h"
#include "DlgYesNo.h"
#include "DlgFont.h"
#include "DlgGraphicTasks.h"
#include "SortColumn.h"
#include "selection.h"
#include "Translation.h"
#include "MainFrm.h"
#include "StringCopyNew.h"
#include "SortProject.h"

#include "Xml.h"
#include "ThreadStats.h"

#include "CreateHtmlWebServer.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"
#include "WebServerProcess.h"

// CRemoteProjectsView

IMPLEMENT_DYNCREATE(CRemoteCombinedProjectView, CTemplateRemoteProjectView)

CRemoteCombinedProjectView::CRemoteCombinedProjectView()
{
	m_bCpidChanged = false;

	m_iThreadLocked = 0;
	m_bThreadBusy	= false;
	m_bThreadTasksBusy = false;

	m_LastSwitchedTime = 0;

	m_bDocumentIsClosing = false;

	m_pFont = NULL;

	m_pThreadStats = NULL;

	m_pDoc			= NULL;

	m_dShareTotal	= 1;
	m_iCountReadShareTotal = 1000;
	m_iNrColomns = NUM_REMOTE_PROJECT_COLUMNS;

//	m_hWndPreference = theApp.m_pDlgPrefMain->m_hWnd;
	m_hWndProxy = theApp.m_pDlgProxyMain->m_hWnd;

	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		m_lProjectItem.at(iCount)->m_pcAccountManagerRpcReplyComputer = 0;
	}
}

CRemoteCombinedProjectView::~CRemoteCombinedProjectView()
{
	theApp.m_pDlgAddAccountManager->ShowWindow(SW_HIDE);
	theApp.m_pDlgAddProject->ShowWindow(SW_HIDE);

	for (int iCount = 0; iCount < (int) m_lProjectItem.size(); iCount++)
	{
		delete m_lProjectItem.at(iCount)->m_pcAccountManagerRpcReplyComputer;
	}
//	m_lProjectItem.clear();

	delete m_pFont;
	m_lCPID.clear();
}

void CRemoteCombinedProjectView::OnInitialUpdate()
{
	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL,(WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);
	CTemplateRemoteProjectView::OnInitialUpdate();
}

void CRemoteCombinedProjectView::InitialUpdate(CRemoteCombinedDoc* pDoc)
{
	m_pDoc = pDoc;
}

void CRemoteCombinedProjectView::GetSelectedComputers(std::deque<CString> *plSelectedComputers)
{
	int iNrItems, iCount, iCountDouble;
	CString sComputerSelected;
	
	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();
	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl, iCount))
		{
			sComputerSelected = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
			// check if the entry is double
			bool	bDouble = false;
			for (iCountDouble = 0; iCountDouble < (int) plSelectedComputers->size(); iCountDouble++)
			{
				if (plSelectedComputers->at(iCountDouble) == sComputerSelected)
				{
					bDouble = true;
				}
			}
			if (!bDouble)
			{
				plSelectedComputers->push_back(sComputerSelected);
			}
		}
	}
}

void CRemoteCombinedProjectView::Sort(CListCtrl *plistCtrl)
{
	UINT		iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;

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
		plistCtrl->SortItems(ProjectSorting,iSorting);
	}

	DisplayMakeLineColors(theApp.m_pDlgSettingsView->m_bAlternatingStriped, theApp.m_pDlgSettingsView->m_bUseSkin, theApp.m_pDlgSettingsView->m_iUseSkin);
}

void CRemoteCombinedProjectView::AddCPID(char *pCPID)
{
	std::string sCPID;
	sCPID = pCPID;

	if (sCPID.length() < 2)
	{
		return;
	}

	for (int iCount = 0; iCount < (int) m_lCPID.size(); iCount++)
	{
		if (m_lCPID.at(iCount) == sCPID)
		{
			return;
		}
	}

	m_bCpidChanged = true;
	m_lCPID.push_back(sCPID);
}


BEGIN_MESSAGE_MAP(CRemoteCombinedProjectView, CTemplateRemoteProjectView)
	ON_WM_SIZE()

	ON_MESSAGE (UWM_MSG_TIMER_PROJECT, OnTimerProject) 
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_PROJECT, OnReadyRpc)

	ON_MESSAGE (UWM_MSG_SETTING_TASKS, OnSettingsTasks)

	ON_MESSAGE (UWM_MSG_REMOTE_PROJECT_SWITCH, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_WINDOW_SHOWN, OnSwitch)

	ON_COMMAND(ID_PROJECTS_RESUME, &CRemoteCombinedProjectView::OnProjectsResume)
	ON_COMMAND(ID_PROJECTS_SUSPEND, &CRemoteCombinedProjectView::OnProjectsSuspend)
	ON_COMMAND(ID_PROJECTS_UPDATE, &CRemoteCombinedProjectView::OnProjectsUpdate)
	ON_COMMAND(ID_PROJECTS_ALLOWWORK, &CRemoteCombinedProjectView::OnProjectsAllowWork)
	ON_COMMAND(ID_PROJECTS_NOMOREWORK, &CRemoteCombinedProjectView::OnProjectsNomorework)
	ON_COMMAND(ID_PROJECTS_PROPERTIES, &CRemoteCombinedProjectView::OnProjectsProperties)
	ON_MESSAGE (UWM_MSG_PREFERENCE_DIALOG, OnPreferenceDialog)
	ON_MESSAGE (UWM_MSG_PROXY_DIALOG, OnProxyDialog)
	ON_MESSAGE (UWM_MSG_REMOTE_COLUMN_PROJECTS, OnColumnChanged)
	ON_MESSAGE (UWM_MSG_ADD_NEW_PROJECT, OnAddNewProject)
	ON_MESSAGE (UWM_MSG_ADD_NEW_ACCOUNT_MANAGER, OnAddNewAccountManager)
	ON_MESSAGE (UWM_MSG_THREAD_ACCOUNTMANAGER_SYNCHRONIZE_READY, OnAccountManagerSynchronizeReady)
	ON_MESSAGE (UWM_MSG_THREAD_ACCOUNTMANAGER_POLL_READY, OnAccountManagerPollReady)

	ON_COMMAND(ID_PROJECTS_SETDEBT, OnProjectsSetDebt)
//	ON_MESSAGE (ID_PROJECTS_SETDEBT, OnSetDebt)
	ON_UPDATE_COMMAND_UI(ID_PROJECTS_SETDEBT,OnUpdateSetDebt)

	ON_COMMAND(ID_PROJECTS_RESET, &CRemoteCombinedProjectView::OnProjectsReset)
	ON_COMMAND(ID_PROJECTS_DETACH, &CRemoteCombinedProjectView::OnProjectsDetach)

	ON_COMMAND(ID_HEADER_SETHEADERITEMS, &CRemoteCombinedProjectView::OnHeaderSetItems)

	ON_COMMAND(ID_PROJECTS_COPYALLTOCLIPBOARD, &CRemoteCombinedProjectView::OnCopyalltoclipboard)
	ON_COMMAND(ID_PROJECTS_COPYSELECTEDTOCLIPBOARD, &CRemoteCombinedProjectView::OnCopyselectedtoclipboard)

	ON_COMMAND(ID_TOOLBAR_PROJECT_UPDATE,&CRemoteCombinedProjectView::OnToolbarProjectUpdate)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_UPDATE,&CRemoteCombinedProjectView::OnUpdateToolbarProjectUpdate)
	ON_COMMAND(ID_TOOLBAR_PROJECT_SUSPEND,&CRemoteCombinedProjectView::OnToolbarProjectSuspend)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_SUSPEND,&CRemoteCombinedProjectView::OnUpdateToolbarProjectSuspend)
	ON_COMMAND(ID_TOOLBAR_PROJECT_RESUME,&CRemoteCombinedProjectView::OnToolbarProjectResume)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_RESUME,&CRemoteCombinedProjectView::OnUpdateToolbarProjectResume)
	ON_COMMAND(ID_TOOLBAR_PROJECT_NONEW,&CRemoteCombinedProjectView::OnToolbarProjectNoNew)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_NONEW,&CRemoteCombinedProjectView::OnUpdateToolbarProjectNoNew)
	ON_COMMAND(ID_TOOLBAR_PROJECT_ALLOWNEW,&CRemoteCombinedProjectView::OnToolbarProjectAllowNew)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PROJECT_ALLOWNEW,&CRemoteCombinedProjectView::OnUpdateToolbarProjectAllowNew)

	ON_COMMAND(ID_TOOLBAR_ALLOW_NETWORK_PREF,&CRemoteCombinedProjectView::OnToolbarAllowNetworkPref)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_NETWORK_PREF,&CRemoteCombinedProjectView::OnUpdateToolbarAllowNetworkPref)
	ON_COMMAND(ID_TOOLBAR_ALLOW_NETWORK_NEVER,&CRemoteCombinedProjectView::OnToolbarAllowNetworkNever)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_NETWORK_NEVER,&CRemoteCombinedProjectView::OnUpdateToolbarAllowNetworkNever)
	ON_COMMAND(ID_TOOLBAR_ALLOW_CPU_PREF,&CRemoteCombinedProjectView::OnToolbarAllowCpuPref)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_CPU_PREF,&CRemoteCombinedProjectView::OnUpdateToolbarAllowCpuPref)
	ON_COMMAND(ID_TOOLBAR_ALLOW_CPU_SNOOZE,&CRemoteCombinedProjectView::OnToolbarAllowCpuSnooze)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_CPU_SNOOZE,&CRemoteCombinedProjectView::OnUpdateToolbarAllowCpuSnooze)
	ON_COMMAND(ID_TOOLBAR_ALLOW_CPU_NEVER,&CRemoteCombinedProjectView::OnToolbarAllowCpuNever)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_CPU_NEVER,&CRemoteCombinedProjectView::OnUpdateToolbarAllowCpuNever)
	ON_COMMAND(ID_TOOLBAR_ALLOW_GPU_PREF,&CRemoteCombinedProjectView::OnToolbarAllowGpuPref)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_GPU_PREF,&CRemoteCombinedProjectView::OnUpdateToolbarAllowGpuPref)
	ON_COMMAND(ID_TOOLBAR_ALLOW_GPU_SNOOZE,&CRemoteCombinedProjectView::OnToolbarAllowGpuSnooze)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_GPU_SNOOZE,&CRemoteCombinedProjectView::OnUpdateToolbarAllowGpuSnooze)
	ON_COMMAND(ID_TOOLBAR_ALLOW_GPU_NEVER,&CRemoteCombinedProjectView::OnToolbarAllowGpuNever)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_GPU_NEVER,&CRemoteCombinedProjectView::OnUpdateToolbarAllowGpuNever)

	ON_MESSAGE (UWM_MSG_GET_FLOATER_DATA, OnFloaterData)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_CPU, OnSetSnoozeCpu)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_GPU, OnSetSnoozeGpu)

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistorySetWidth)

	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelTabView)	
	ON_MESSAGE(UWM_MSG_VIEW_REFRESH,OnRefresh)	

	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, OnComputerBarSelectionChanged)

	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_GRAPHIC_TASKS, OnGraphicsTasksReady)

	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnFontChangeAll)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnFontHasChanged)

	ON_WM_CONTEXTMENU()
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_READ_PROJECT, OnReadyProject)
	ON_COMMAND(ID_WWW_1, &CRemoteCombinedProjectView::OnWww1)
	ON_COMMAND(ID_WWW_2, &CRemoteCombinedProjectView::OnWww2)
	ON_COMMAND(ID_WWW_3, &CRemoteCombinedProjectView::OnWww3)
	ON_COMMAND(ID_WWW_4, &CRemoteCombinedProjectView::OnWww4)
	ON_COMMAND(ID_WWW_5, &CRemoteCombinedProjectView::OnWww5)
	ON_COMMAND(ID_WWW_6, &CRemoteCombinedProjectView::OnWww6)
	ON_COMMAND(ID_WWW_7, &CRemoteCombinedProjectView::OnWww7)
	ON_COMMAND(ID_WWW_8, &CRemoteCombinedProjectView::OnWww8)
	ON_COMMAND(ID_WWW_9, &CRemoteCombinedProjectView::OnWww9)
	ON_COMMAND(ID_WWW_10, &CRemoteCombinedProjectView::OnWww10)
	ON_COMMAND(ID_WWW_11, &CRemoteCombinedProjectView::OnWww11)
	ON_COMMAND(ID_WWW_12, &CRemoteCombinedProjectView::OnWww12)
	ON_COMMAND(ID_WWW_13, &CRemoteCombinedProjectView::OnWww13)
	ON_COMMAND(ID_WWW_14, &CRemoteCombinedProjectView::OnWww14)
	ON_COMMAND(ID_WWW_15, &CRemoteCombinedProjectView::OnWww15)
	ON_COMMAND(ID_WWW_16, &CRemoteCombinedProjectView::OnWww16)
	ON_COMMAND(ID_WWW_17, &CRemoteCombinedProjectView::OnWww17)
	ON_COMMAND(ID_WWW_18, &CRemoteCombinedProjectView::OnWww18)
	ON_COMMAND(ID_WWW_19, &CRemoteCombinedProjectView::OnWww19)
	ON_COMMAND(ID_WWW_20, &CRemoteCombinedProjectView::OnWww20)


	ON_MESSAGE (UWM_MSG_THREAD_CPID_CHANGED, OnCpidChanged)	

	ON_MESSAGE (UWM_MSG_THREAD_START_STATS, OnStartStats)	
	ON_MESSAGE (UWM_MSG_THREAD_READY_STATS, OnReadyStats)	
	ON_MESSAGE (UWM_DLG_SEARCH_COMPUTER_READY, OnSearchComputerReady)

	ON_MESSAGE (UWM_MSG_WEB_ITEM_SELECTED, OnWebItemSelected)
	ON_MESSAGE(UWM_MSG_WEB_OPERATION,OnWebOperation)
	ON_MESSAGE(UWM_MSG_WEB_SORT,OnWebSort)
	ON_MESSAGE(UWM_MSG_WEB_PROPERTIES,OnWebProperties)

	ON_COMMAND(ID_COMPUTERS_DETECT, &CRemoteCombinedProjectView::OnComputersDetect)
END_MESSAGE_MAP()

LRESULT CRemoteCombinedProjectView::OnCpidChanged(WPARAM parm1, LPARAM parm2)
{
	std::string *psCPID, *psCPID2;

	m_lCPID.clear();

	if (!m_bUseStats) return 0;

	psCPID = (std::string *) parm1;
	psCPID2 = (std::string *) parm2;

	if (psCPID == NULL || psCPID2 == NULL) return 0;

	AddCPID((char *) psCPID->c_str());
	AddCPID((char *) psCPID2->c_str());

//	PostMessage(UWM_MSG_THREAD_START_STATS, 1, 1);

	return 0;
}

LRESULT CRemoteCombinedProjectView::OnStartStats(WPARAM parm1, LPARAM parm2)
{
//	bool	bFull, bForce;

//	bFull = (bool) (parm1 != 0);
//	bForce= (bool) (parm2 != 0);

	if (!m_bUseStats) return 0;

/*
	if (theApp.m_pDlgSettingsExpert->m_bCpidManual)
	{
		if (theApp.m_pDlgSettingsExpert->m_sCpidManual.length() != 0)
		{
			if (m_pThreadStats == NULL)
			{
				m_iThreadStatRequest = 1;
				m_bThreadStatRequestError = false;
				m_pThreadStats = (CThreadStats *) AfxBeginThread(RUNTIME_CLASS(CThreadStats));
				m_pThreadStats->PostThreadMessage(iCommand, (WPARAM) this->m_hWnd, (LPARAM) &theApp.m_pDlgSettingsExpert->m_sCpidManual);
				if (theApp.m_pDlgSettingsExpert->m_sCpidManual2.length() != 0)
				{
					m_pThreadStats->PostThreadMessage(iCommand, (WPARAM) this->m_hWnd, (LPARAM) &theApp.m_pDlgSettingsExpert->m_sCpidManual2);
					m_iThreadStatRequest++;
				}
			}
			m_pThreadStats->PostThreadMessage(UWM_MSG_THREAD_QUIT, (WPARAM) this->m_hWnd, (LPARAM) 0);
		}
	}
	else
*/

	if (m_lCPID.size() > 0)
	{
		if (m_pThreadStats == NULL)
		{
			m_iThreadStatRequest = 1;
			m_bThreadStatRequestError = false;
			m_pThreadStats = (CThreadStats *) AfxBeginThread(RUNTIME_CLASS(CThreadStats));
			m_pThreadStats->PostThreadMessage(UWM_MSG_THREAD_START_STATS, (WPARAM) this->m_hWnd, (LPARAM) &m_lCPID.at(0));
			if (m_lCPID.size() > 1)
			{
				m_pThreadStats->PostThreadMessage(UWM_MSG_THREAD_START_STATS, (WPARAM) this->m_hWnd, (LPARAM) &m_lCPID.at(1));
				m_iThreadStatRequest++;
			}
		}
		m_pThreadStats->PostThreadMessage(UWM_MSG_THREAD_QUIT, (WPARAM) this->m_hWnd, (LPARAM) 0);
	}
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnReadyRpc(WPARAM parm1, LPARAM parm2)
{
	LV_ITEM		lvi;

	int			iCount, iNrProjects, iItemsAdd, iItemsAddTemp, iNrItems, iAddPos, iNrItemsInitial, iStatus, iTemp, iRow;
	CRpcThreadReturn	*pThreadReturn;
	PROJECT		info;
	bool		bOverride = false, bSelectedFound = false, bSorting;
	CThreadRpc	*pThreadRpc;
	char		*pcComputerId;

	iStatus = (int) parm1;
	pThreadReturn = (CRpcThreadReturn *)	parm2;

	if (m_bDocumentIsClosing)
	{
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusClosing]);
		delete pThreadReturn;
		return 0;			// everything is closing down.
	}
	
	iNrProjects = pThreadReturn->m_iNumber;
	m_iProjectTotal+= iNrProjects;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_iNrProjects = iNrProjects;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_pcComputerId = pThreadReturn->m_pcComputerId;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_pThread = pThreadReturn->m_pThread;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_iStatusRpc = iStatus;
	
	m_iReadyRpcCount++;

	theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_CONNECT, (WPARAM) pThreadReturn->m_pcComputerId, pThreadReturn->m_iStatus);

	delete pThreadReturn;
	pThreadReturn = NULL;

	if (m_iReadyRpcCount != m_iSendRpcCount) return 0;	// wait for all results to come in

	CDlgSettingsView *pSettingsView;
	pSettingsView = theApp.m_pDlgSettingsView;
	m_bUseHorizontalGrid = pSettingsView->m_bUseHorizontalGrid;
	m_bUseVerticalGrid = pSettingsView ->m_bUseVerticalGrid;
	
	// setup sorting
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		m_lRpcInfo.at(iCount)->m_iSortIndex = iCount;
	}
	
	bool bSortingAZ = true;
	UINT		iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;
	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);
	if (iSortingColumnSecondary == COLUMN_PROJECT_COMPUTER)	if (iSortingOptionSecondary == SORTING_ZA) bSortingAZ = false;

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

	m_pDoc->DisplayStatus(m_iReadyRpcCount, &m_lRpcInfo.at(0)->m_iStatusRpc); //, m_rpcInfo.m_pcComputerId[0]);

	CListCtrl& listCtrl = GetListCtrl();

	if (m_iProjectTotal == 0)
	{
		m_bThreadBusy = false;
		theApp.m_pMainFrame->SetBoincTasksStatus(TEXT_STATUS_WAITING);
		listCtrl.DeleteAllItems();
		Invalidate();
		return FALSE;
	}



//	if (iNrMessages == 0) return FALSE;

	iNrItems = listCtrl.GetItemCount();
	iNrItemsInitial = iNrItems;
	iItemsAdd =  iNrItems - m_iProjectTotal;

//	m_iSelected = iNrItems -1;

//	if (!m_bFirstTime)
	{
		if (iItemsAdd > 0)
		{
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
				iItemsAddTemp = iItemsAdd;
				// add list items
				bOverride = true;
				while (iItemsAddTemp < 0)
				{
					lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
					iAddPos =  iNrItems-1;
					if (iAddPos < 0) iAddPos = 0;
					lvi.iItem = iAddPos;								// add at the end
					lvi.iSubItem = 0;
					lvi.pszText = "";// info.m_pszProject;	

					lvi.iImage = iAddPos;								// add at the end
					lvi.stateMask = LVIS_STATEIMAGEMASK;
					lvi.state = INDEXTOSTATEIMAGEMASK(1);

					lvi.lParam = (LPARAM) &m_dataDummy;

					listCtrl.InsertItem(&lvi);
					iNrItems++;
					iItemsAddTemp++;
				}
			}
		}
	}

	// for every computer

	iRow = 0;
	for (int iComputer = 0; iComputer < m_iReadyRpcCount; iComputer++)
	{
		pThreadRpc = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_pThread;
		iNrProjects = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_iNrProjects;
		pcComputerId = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_pcComputerId;
		bSelectedFound = DisplayRpc(pThreadRpc, iNrProjects, &iRow, pcComputerId, iComputer);
	}

	m_bFirstTime = false;

	if (iItemsAdd !=0)	Invalidate();

	Sort(&listCtrl);

	if (g_bWebServerActive)
	{
		if (!m_bDocumentIsClosing && (g_pcWebServerHtml == NULL))
		{
			CCreateHtmlWebServer createHtml;
			char *pcHtml;
			if (createHtml.Create(PosBarViewSelectProjects, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_PROJECT_COLUMNS, NULL, PosViewProjecsRowProject, &pcHtml))
			{
				g_pcWebServerHtml = pcHtml;
				g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_PROJECTS);
				g_tWebServerHtml = GetTickCount()/100;
			}
		}
	}
	if (g_bCloudServerRequestData)
	{
		if (!m_bDocumentIsClosing && (g_pcCloudServerHtml == NULL))
		{
			if (g_iCloudServerHtmlTabProcessed[TAB_PROJECTS] == TRUE)
			{
				g_iCloudServerHtmlTabProcessed[TAB_PROJECTS] = FALSE;
				CCreateHtmlWebServer createHtml;
				char *pcHtml;
				if (createHtml.Create(PosBarViewSelectProjects, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_PROJECT_COLUMNS, NULL, PosViewProjecsRowProject, &pcHtml))
				{
					g_pcCloudServerHtml = pcHtml;
					if (g_pThreadCloudServer != NULL) g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_PROJECTS);
					g_tCloudServerHtml = GetTickCount()/100;
				}
			}
		}
	}

	m_bThreadBusy = false;	
	theApp.m_pMainFrame->SetBoincTasksStatus(TEXT_STATUS_WAITING);

	return 0;
}

LRESULT CRemoteCombinedProjectView::OnGraphicsTasksReady(WPARAM parm1, LPARAM parm2)
{
	CRpcThreadReturn * pThreadReturn;
	pThreadReturn = (CRpcThreadReturn *) parm2;
	delete pThreadReturn;

	m_iReadyTasksCount++;

	if (m_iReadyTasksCount == m_iSendTasksCount)
	{
		m_bThreadTasksBusy = false;
	}
	return 0;
}

bool CRemoteCombinedProjectView::DisplayRpc(CThreadRpc *pThreadRpc, int iNrProjects, int *piRow, char *pcComputerId, int iComputerId)
{
	CString			sStatus;
	PROJECT			*pInfo;
	bool			bOverride = false, bSelectedFound = false;
	int				i;

	CListCtrl& listCtrl = GetListCtrl();

	// get share total

	m_dShareTotal = 0;
	for(i = 0; i < iNrProjects; i++)
	{
		if (!pThreadRpc->GetRpcProject(i,&pInfo)) return false; 		// error project not ready
		m_dShareTotal += pInfo->resource_share;

		AddCPID(pInfo->external_cpid);	
	}

	for(i = 0; i < iNrProjects; i++)
	{
		if (!pThreadRpc->GetRpcProject(i,&pInfo)) return false;			// error project not ready
		DisplaySingleItem(&listCtrl, *piRow, pInfo, pcComputerId, iComputerId);
		(*piRow)++;
	}
	return bSelectedFound;
}

// CRemoteProjectsView diagnostics

#ifdef _DEBUG
void CRemoteCombinedProjectView::AssertValid() const
{
	CTemplateRemoteProjectView::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteCombinedProjectView::Dump(CDumpContext& dc) const
{
	CTemplateRemoteProjectView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRemoteCombinedProjectView message handlers


LRESULT CRemoteCombinedProjectView::OnTimerProject(WPARAM parm1, LPARAM parm2)
{
	CString sFormat;
	char	*pcComputerId;
	int		iViewActive;
	int		iTimerWu;
	int		iNrRpc, iThreadId;
	HWND	hWnd;
	CRemoteCombinedProjectView *pView;
	bool	bVisible;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	bVisible = theApp.m_pMainWnd->IsWindowVisible() == TRUE;

	CheckAndSaveColumnsWidth();

	if (m_bThreadBusy)		// check for lockup
	{
		if (bVisible && m_pDoc->m_iViewActive == PROJECT_VIEW_ACTIVE)
		{
			if (m_iThreadLocked > DELAY_SHOW_UPDATE_TIMER) sFormat.Format(" %d", m_iThreadLocked);
			sFormat = gszTranslation[PosWindowStatusMessageStatusUpdating] + sFormat;
			theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
			sFormat.ReleaseBuffer();
		}

		if (m_iThreadLocked++ > theApp.m_iThreadTimeout) {m_iThreadLocked = 0; m_bThreadBusy = false;}
		return 0;
	}

	time_t tSystemTime;
	_time64(&tSystemTime);

	if (m_bCpidChanged)
	{
		PostMessage(UWM_MSG_THREAD_START_STATS, 1, 1);
		m_bCpidChanged = false;
	}
	else
	{
		if (tSystemTime > m_tNextStatsReadWeb)
		{
			PostMessage(UWM_MSG_THREAD_START_STATS, 1, 1);
		}
	}

	m_iThreadLocked = 0;

	if (m_bThreadBusy) return 0;
	if (m_pDoc == NULL) return 0;

	iNrRpc = m_pDoc->GetRpcSelectedComputers();
	iViewActive = m_pDoc->m_iViewActive;

	m_iSendRpcCount = 0;//iNrRpc;
	m_iReadyRpcCount = 0;
	m_iProjectTotal = 0;

	m_iSendTasksCount = 0;
	m_iReadyTasksCount = 0;

	pView = m_pDoc->m_pProjectView;
	hWnd = pView->m_hWnd;

	switch (theApp.m_pDlgSettingsView->m_iRefresh)
	{
		case REFRESH_SLOW:
			iTimerWu = 6;
		break;
		case REFRESH_NORMAL: 
			iTimerWu = 2;
		break;
		case REFRESH_QUICK:
			iTimerWu = 1;
		break;
		default: // manual
			iTimerWu = theApp.m_pDlgSettingsView->m_iRefreshManual;
	}

	int iTimeLeft;
	iTimeLeft = iTimerWu-m_iTimerWu;
	if (iTimeLeft < 0) iTimeLeft = 0;

	if (iTimeLeft > 1)	sFormat.Format(gszTranslation[PosWindowStatusMessageStatusUpdateInSec], iTimeLeft);					//"Update in %d seconds"
	else				sFormat.Format(gszTranslation[PosWindowStatusMessageStatusWaiting]);								//"Waiting"
	theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
	sFormat.ReleaseBuffer();

	if (m_iTimerWu++ >= iTimerWu)
	{
		int iColumn;
		bool bFetchTasksDayWeek = false;
		if (!m_bThreadTasksBusy)
		{
			if (tSystemTime > (m_tLastTasksCount + m_tIntervalTasksCount))
			{
				iColumn = m_iColumnOrder[COLUMN_PROJECT_TASKS_DAY];
				if (iColumn >=0) bFetchTasksDayWeek = true;
				iColumn = m_iColumnOrder[COLUMN_PROJECT_TASKS_WEEK];
				if (iColumn >=0) bFetchTasksDayWeek = true;

				if (bFetchTasksDayWeek)
				{
//					DeleteProjectTasks();
					m_tIntervalTasksCount += 10;
					if (m_tIntervalTasksCount > 300)
					{
						m_tIntervalTasksCount = 300;
					}
					m_tLastTasksCount = tSystemTime;
				}
			}
		}

		m_iTimerWu = 0;
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);
			if (m_pDoc->GetComputerIdStringWithIdNumber(iThreadId, &pcComputerId))
			{// get the computer id string for the thread
				int iResult = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, (WPARAM) pcComputerId, 0);
				if (iResult == TRUE)
//				if (theApp.m_pMainFrame->m_dlgBarComputer.FindSelected(&sComputerId))	// check if the computer is selected
				{
					if (bFetchTasksDayWeek)
					{
						m_lComputer.at(iCount)->m_ThreadRpcId = iThreadId;
						m_lComputer.at(iCount)->m_pcThreadRpcId = pcComputerId;
						::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS_P, (WPARAM) hWnd,(LPARAM) 7*24);		// week
						std::deque<CHistoryRpc*> *pHistoryRpc;
						pHistoryRpc = m_pDoc->m_pHistoryView->GetComputerList();
						::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS_L, (WPARAM) hWnd,(LPARAM) pHistoryRpc->at(iCount));			// history pointers
						::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS, (WPARAM) hWnd,(LPARAM) m_lComputer.at(iCount)->m_plHistoryProjectTasks);
						m_iSendTasksCount++;
						m_bThreadTasksBusy = true;
					}
					::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_PROJECT,(WPARAM) hWnd,0); // return naar project view!
					m_iSendRpcCount++;
					m_bThreadBusy = true;	
				}
			}
		}
		if (m_iSendRpcCount == 0)
		{
			CListCtrl& listCtrl = GetListCtrl();
			listCtrl.DeleteAllItems();
		}
	}

	return 0;
}


LRESULT CRemoteCombinedProjectView::OnSwitch(WPARAM parm1, LPARAM parm2)
{
	int iTimeToUpdate;

	time_t tSystemTime;
	_time64(&tSystemTime);

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (parm2 == REFRESH_VIEW_NOW) 
	{
		m_iTimerWu = 1000;	// direct action
	}

	iTimeToUpdate = (int) (m_LastSwitchedTime - tSystemTime);
	m_LastSwitchedTime = tSystemTime + 10;	// don't update within 10 seconds.

	if (iTimeToUpdate <= 0) 
	{
		m_iTimerWu = 1000;	// direct action
		m_tIntervalTasksCount = 0;
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	}
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnSettingsTasks(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc != NULL) m_pDoc->SettingsTasks(parm1 != 0);
	return 0;
}

void CRemoteCombinedProjectView::OnContextMenu(CWnd* pWnd, CPoint point) 
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
		// translate
		tracker->ModifyMenu(ID_HEADER_SETHEADERITEMS,MF_STRING,ID_HEADER_SETHEADERITEMS,gszTranslation[PosPopUpHeaderEnable]);
		tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y , AfxGetMainWnd()); 
		return;
	}

	CListCtrl& listCtrl = GetListCtrl();
	m_contextMenu.SetColumnOrder(NUM_REMOTE_PROJECT_COLUMNS, &m_iColumnOrder[0]);
	m_contextMenu.ContextMenuCombined(CONTEXT_MENU_PROJECTS,m_pDoc, &listCtrl, m_pSelection, point, this, IDR_MENU_PROJECT);
} 

LRESULT CRemoteCombinedProjectView::OnReadyProject(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_contextMenu.Ready((int) parm1);
	return 0;
}

void CRemoteCombinedProjectView::OnProjectsSuspend()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_SUSPEND);
}

void CRemoteCombinedProjectView::OnProjectsResume()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_RESUME);
}

void CRemoteCombinedProjectView::OnProjectsUpdate()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_UPDATE);
}

void CRemoteCombinedProjectView::OnProjectsAllowWork()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_ALLOWMOREWORK);
}

void CRemoteCombinedProjectView::OnProjectsNomorework()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_NOMOREWORK);
}

void CRemoteCombinedProjectView::OnProjectsReset()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_RESET,gszTranslation[PosDialogProjectOQuestionReset]);		//"Do you want to reset these projects");
}

void CRemoteCombinedProjectView::OnProjectsDetach()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_DETACH,gszTranslation[PosDialogProjectOQuestionDetach]);	//"Do you want to detach from these projects");
}

void CRemoteCombinedProjectView::PostProjectOperation(int iOperation, CString sWarning)
{
	CString sProjects, sProjectSpace;
	bool	bSelected, bEmpty;
	int iCount, iNrItems; //, iThreadId;
//	char cBuffer[256];

	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	// now copy all the project names to a seperate array, just in case something changes

	if (iNrItems > MAX_PROJECTS) iNrItems = MAX_PROJECTS;

	bEmpty =false;

	m_iSelectedProjectItems = 0;
	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		bSelected =m_pSelection->SelectionSelected(&listCtrl, iCount);
		if (bSelected)
		{
			m_sSelectedProjectItemsComputer[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
			if (m_sSelectedProjectItemsComputer[m_iSelectedProjectItems].GetLength() == 0) bEmpty = true;
			m_sSelectedProjectItemsName[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_PROJECT]);
			if (m_sSelectedProjectItemsName[m_iSelectedProjectItems].GetLength() == 0) bEmpty = true;

//			if (m_sSelectedProjectItemsName[m_iSelectedProjectItems].Find("Tasks",0) < 0)
			{
				m_iSelectedProjectItems++;
			}
		}
	}
	
	if (bEmpty)
	{
		AfxMessageBox(gszTranslation[PosGroupDialogSettingsTasksMsgBoxExclude],MB_ICONEXCLAMATION);
		return;
	}

	if (m_iSelectedProjectItems == 0) return;

	if (sWarning.GetLength() > 0)
	{
		CDlgYesNo dlgYesNo;
		dlgYesNo.bColumnMode = true;
		dlgYesNo.m_sRegistryId = "_abort_project";
		dlgYesNo.m_sColumn[0]		= gszTranslation[PosDialogTaskAbortColumnProject];	
		dlgYesNo.m_iColumnWidth[0]	= 250;
		dlgYesNo.m_sColumn[1]		= gszTranslation[PosDialogTaskAbortColumnComputer];
		dlgYesNo.m_iColumnWidth[1]	= 120;
		dlgYesNo.m_sColumn[2]		= "";
		dlgYesNo.m_iColumnWidth[2]	= 0;
		dlgYesNo.m_sColumn[3]		= "";
		dlgYesNo.m_iColumnWidth[3]	= 0;
		dlgYesNo.m_sColumn[4]		= "";
		dlgYesNo.m_iColumnWidth[4]	= 0;
		dlgYesNo.m_sColumn[5]		= "";
		dlgYesNo.m_iColumnWidth[5]	= 0;

		for (iCount = 0; iCount < m_iSelectedProjectItems; iCount++)
		{
			CString sTxt1,sTxt2,sTxt3,sTxt4,sTxt5,sTxt6;
			
			sTxt1 =  m_sSelectedProjectItemsName[iCount];
			sTxt2 =	 m_sSelectedProjectItemsComputer[iCount];
			dlgYesNo.AddColumn(sTxt1, sTxt2, sTxt3, sTxt4, sTxt5, sTxt6);
		}

		dlgYesNo.AddColumn("");
		dlgYesNo.AddColumn(sWarning);			

		int iResult = (int) dlgYesNo.DoModal();
		if (iResult != IDYES) return;
	}

	for (iCount = 0; iCount < m_iSelectedProjectItems; iCount++)
	{
		bool bFound;
		int  iThreadId;
		bFound = m_pDoc->GetRpcThreadId(&m_sSelectedProjectItemsComputer[iCount], &iThreadId);
		char *pSelectedProjectItemsName;
		CStringCopyNew(&pSelectedProjectItemsName, m_sSelectedProjectItemsName[iCount].GetBuffer(), false);
		m_sSelectedProjectItemsName[iCount].ReleaseBuffer();
		if (bFound)	::PostThreadMessage(iThreadId,UWM_MSG_THREAD_PROJECT_OPERATION_BYNAME, iOperation, (LPARAM) pSelectedProjectItemsName);
		Sleep(50);
	}	
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_HISTORY_SOMETHING_HAPPENED,0,0);

	m_iTimerWu = 1000;	// direct action on refresh
}

BOOL CRemoteCombinedProjectView::ProjectProperties(int iBusy)
{
	CThreadRpc  *pThreadRpc;
	CString sTxt, sProjectProperties, sComputerId, sProjectName;
	CProjectPropertiesItem *pProjectPropertiesItem;
	CListCtrl& listCtrl = GetListCtrl();
	int			nItems;//, iTimeOut;
	bool		bFirstTime = true; //, bOk;

	if (m_bDocumentIsClosing) 	return FALSE;			// everything is closing down.
	if (m_pDoc == NULL) return FALSE;

	if (m_iProjectPropertiesBusy != PROPERTIES_READY) return FALSE;

	nItems = m_pSelection->SelectionNumber();
	if (nItems == 0) return FALSE;

	nItems = listCtrl.GetItemCount();

	for (int iCount = 0; iCount < nItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl, iCount))
		{
			sComputerId = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
			bool bFound = m_pDoc->GetRpcThreadPointer(&sComputerId, &pThreadRpc);		
			if (bFound)
			{
				pProjectPropertiesItem = new CProjectPropertiesItem;
				pProjectPropertiesItem->m_sComputerId = sComputerId;
				pProjectPropertiesItem->m_sProject = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_PROJECT_PROJECT]);
				pProjectPropertiesItem->m_iThreadId = pThreadRpc->m_nThreadID;
				m_lProjectProperties.push_back(pProjectPropertiesItem);
			}
		}
	}

	ProjectPropertiesGet(iBusy);

	return TRUE;

}

void CRemoteCombinedProjectView::OnProjectsProperties()
{
	ProjectProperties(PROPERTIES_DIALOG);
}

LRESULT CRemoteCombinedProjectView::OnColumnChanged(WPARAM parm1, LPARAM parm2)
{
	HWND hHnd;
	int	 *pColumn;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_bFirstTime) return 0;		// wait for the 1e init

	hHnd = this->m_hWnd;

	if (hHnd != (HWND) parm2)
	{
		CListCtrl& listCtrl = GetListCtrl();
		pColumn = (int *) parm1;		
		for (int iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
		{
			listCtrl.SetColumnWidth(iCount, *pColumn);
			pColumn++;
		}
	}

	return 0;
}

LRESULT CRemoteCombinedProjectView::OnSettingsApplyChangesTasks(WPARAM parm1, LPARAM parm2)
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
	ReadAllColumnsWidth();

	listCtrl.Invalidate(TRUE);

	return 0;
}

LRESULT CRemoteCombinedProjectView::OnPreferenceDialog(WPARAM parm1, LPARAM parm2)
{
	CString sComputer;
	int	iThreadId, iItems;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();
	sComputer = listCtrl.GetItemText(0,m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);  	// nothing get first one
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl,iCount))
		{
			sComputer = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
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

LRESULT CRemoteCombinedProjectView::OnProxyDialog(WPARAM parm1, LPARAM parm2)
{
	CString sComputer;
	int	iThreadId, iItems;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();
	sComputer = listCtrl.GetItemText(0,m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);	 // nothing get first one
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (m_pSelection->SelectionSelected(&listCtrl,iCount))
		{
			sComputer = listCtrl.GetItemText(iCount,m_iColumnOrder[COLUMN_PROJECT_COMPUTER]); 
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

LRESULT CRemoteCombinedProjectView::OnAddNewProject(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	OnProjectsAddNewProject();
	return 0;
}

void CRemoteCombinedProjectView::OnProjectsAddNewProject()
{
	bool	bSelected;
	int		iCount, iNrItems, iThreadId;
	char	*pcComputerId;
	int		iThreads;

	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	if (iNrItems > MAX_PROJECTS) iNrItems = MAX_PROJECTS;

	m_iSelectedProjectItems = 0;
	/*
	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		bSelected =m_pSelection->SelectionSelected(&listCtrl, iCount);
		if (bSelected)
		{
			m_sSelectedProjectItemsComputer[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
			m_sSelectedProjectItemsName[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_PROJECT]);

			if (m_sSelectedProjectItemsName[m_iSelectedProjectItems].Find(gszTranslation[PosViewTasksFilterTasks],0) < 0) //"Tasks"
			{
				m_iSelectedProjectItems++;
				break;
			}
		}
	}
	*/
//	if (m_iSelectedProjectItems == 0)
	{


//		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
//		if (iThreadId > 0)
		{
			m_sSelectedProjectItemsComputer[0] = theApp.m_pMainFrame->m_dlgBarComputer.GetComputerSelected(); //LOCALHOST_NAME;
			bool bFound = m_pDoc->GetRpcThreadId(&m_sSelectedProjectItemsComputer[0], &iThreadId);
			if (!bFound)
			{
				int ii = 0;
				return;
			}
			theApp.m_pDlgAddProject->PostMessage(UWM_MSG_ADDPROJECT_OPEN_DIALOG, iThreadId, (LPARAM)&m_sSelectedProjectItemsComputer[0]);
			theApp.m_pDlgAddProject->PostMessage(UWM_MSG_ADDPROJECT_SET_THREAD_ID, 0, (LPARAM)m_pDoc);
			return;
		}
	}
		// ok still nothing, fall back on the first connected computer.

		/*
		HOST_INFO	*pHostInfo;
		char		*pcComputerFound;
		CString		sComputerFound;

		iThreads = m_pDoc->GetRpcSelectedComputers();
		for (int iCount = 0; iCount < iThreads; iCount++)
		{
			m_pDoc->GetComputerIds(&pcComputerFound, iCount);
			m_sSelectedProjectItemsComputer[0] = pcComputerFound;
			pHostInfo = m_pDoc->GetRpcThreadHostInfo(&m_sSelectedProjectItemsComputer[0]);
			if (pHostInfo != NULL)
			{
				if (strlen(pHostInfo->domain_name) != 0)
				{
					// found a connected computer
					m_pDoc->GetRpcThreadId(&iThreadId, iCount);
					theApp.m_pDlgAddProject->PostMessage(UWM_MSG_ADDPROJECT_OPEN_DIALOG,iThreadId,(LPARAM) &m_sSelectedProjectItemsComputer[0]);
					theApp.m_pDlgAddProject->PostMessage(UWM_MSG_ADDPROJECT_SET_THREAD_ID, (WPARAM) 0, (LPARAM) m_pDoc);
//					theApp.m_pDlgAddProject->PostMessage(UWM_MSG_ADDPROJECT_SET_STRING_ID, (WPARAM) iThreads, (LPARAM) pcComputerId);
					return;
				}
			}
		}
		

		return;
	}

	for (iCount = 0; iCount < m_iSelectedProjectItems; iCount++)
	{
		bool bFound;

		bFound = m_pDoc->GetRpcThreadId(&m_sSelectedProjectItemsComputer[iCount], &iThreadId);
		if (bFound)
		{
			theApp.m_pDlgAddProject->PostMessage(UWM_MSG_ADDPROJECT_OPEN_DIALOG,iThreadId,(LPARAM) &m_sSelectedProjectItemsComputer[0]);
			m_pDoc->GetComputerIds(&pcComputerId, iCount);
			theApp.m_pDlgAddProject->PostMessage(UWM_MSG_ADDPROJECT_SET_THREAD_ID, (WPARAM) 0, (LPARAM) m_pDoc);
		}
		break; // only one
	}	
	*/
}

LRESULT CRemoteCombinedProjectView::OnAddNewAccountManager(WPARAM wParam, LPARAM lParam)
{
	bool	bSelected;
	int		iCount, iNrItems, iThreadId;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	if (iNrItems > MAX_PROJECTS) iNrItems = MAX_PROJECTS;

	m_iSelectedProjectItems = 0;
	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		bSelected =m_pSelection->SelectionSelected(&listCtrl, iCount);
		if (bSelected)
		{
			m_sSelectedProjectItemsComputer[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
			m_sSelectedProjectItemsName[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_PROJECT]);

			if (m_sSelectedProjectItemsName[m_iSelectedProjectItems].Find(gszTranslation[PosViewTasksFilterTasks],0) < 0) //"Tasks"
			{
				m_iSelectedProjectItems++;
				break;
			}
		}
	}

	if (m_iSelectedProjectItems == 0)
	{
		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
		if (iThreadId > 0)
		{
			m_sSelectedProjectItemsComputer[0] = LOCALHOST_NAME;

			CAccountManagerData accountManagerData;
			accountManagerData.m_sComputer = m_sSelectedProjectItemsComputer[0];
			accountManagerData.m_iThreadId = iThreadId;
			accountManagerData.m_hWndTasks = m_pDoc->m_pTaskView->m_hWnd;
			accountManagerData.m_pDoc = m_pDoc;
//			m_pDoc->GetComputerIds(&accountManagerData.m_pcComputerId);

			theApp.m_pDlgAddAccountManager->SendMessage(UWM_MSG_ADD_ACCOUNTMANAGER_OPEN_DIALOG, (WPARAM) &accountManagerData,0);
			return 0;
		}

		// ok still nothing, fall back on the first connected computer.

		HOST_INFO	*pHostInfo;
		char		*pcComputerFound;
		CString		sComputerFound;
		int			iThreads;

		iThreads = m_pDoc->GetRpcSelectedComputers();
		for (int iCount = 0; iCount < iThreads; iCount++)
		{
			m_pDoc->GetComputerIds(&pcComputerFound, iCount);
			m_sSelectedProjectItemsComputer[0] = pcComputerFound;
			pHostInfo = m_pDoc->GetRpcThreadHostInfo(&m_sSelectedProjectItemsComputer[0]);
			if (pHostInfo != NULL)
			{
				if (strlen(pHostInfo->domain_name) != 0)
				{
					// found a connected computer
					m_pDoc->GetRpcThreadId(&iThreadId, iCount);

					CAccountManagerData accountManagerData;
					accountManagerData.m_sComputer = m_sSelectedProjectItemsComputer[0];
					accountManagerData.m_iThreadId = iThreadId;
					accountManagerData.m_hWndTasks = m_pDoc->m_pTaskView->m_hWnd;
					accountManagerData.m_pDoc = m_pDoc;

					theApp.m_pDlgAddAccountManager->SendMessage(UWM_MSG_ADD_ACCOUNTMANAGER_OPEN_DIALOG, (WPARAM) &accountManagerData,0);
					return 0;
				}
			}
		}
	}

	for (iCount = 0; iCount < m_iSelectedProjectItems; iCount++)
	{
		bool bFound;

		bFound = m_pDoc->GetRpcThreadId(&m_sSelectedProjectItemsComputer[iCount], &iThreadId);
		if (bFound)
		{
			CAccountManagerData accountManagerData;
			accountManagerData.m_sComputer = m_sSelectedProjectItemsComputer[0];
			accountManagerData.m_iThreadId = iThreadId;
			accountManagerData.m_hWndTasks = m_pDoc->m_pTaskView->m_hWnd;
			accountManagerData.m_pDoc = m_pDoc;

			theApp.m_pDlgAddAccountManager->SendMessage(UWM_MSG_ADD_ACCOUNTMANAGER_OPEN_DIALOG, (WPARAM) &accountManagerData,0);
		}
		break; // only one
	}	

	return 0;
}

LRESULT CRemoteCombinedProjectView::OnAccountManagerSynchronizeReady(WPARAM wParam, LPARAM lParam)
{
	CRpcThreadReturn	*pThreadReturn;
	int			iResult, iThreadId;
	char		*pcComputerId;

	iResult	= (int) wParam;
	pThreadReturn = (CRpcThreadReturn *) lParam;

	if (m_bDocumentIsClosing)
	{
		delete pThreadReturn;
		return 0;			// everything is closing down.
	}

	if (pThreadReturn->m_iStatus == RPC_STATUS_CONNECTED)
	{
		if (iResult)
		{
			CLoggingMessage log;
			CString sFormat;
			sFormat = "ERROR: failed on computer: ";
			sFormat+= pThreadReturn->m_pcHostName;
			log.m_pFrom = "Synchronize manager"; log.m_pText = sFormat.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, true);
			sFormat.ReleaseBuffer();
			delete pThreadReturn;
			return 0;
		}
	}

	pcComputerId = pThreadReturn->m_pcComputerId;
	iThreadId = pThreadReturn->m_iThreadId;

	delete pThreadReturn;
	pThreadReturn = NULL;

	CString sComputerId;
	sComputerId = pcComputerId;
	int iNr = m_pDoc->CRemoteCombinedDoc::GetRpcArrayPos(&sComputerId);
	if (iNr >= 0)
	{
		m_lProjectItem.at(iNr)->m_pcAccountManagerRpcReplyComputer = pcComputerId;
		::PostThreadMessage(iThreadId, UWM_MSG_THREAD_ACCOUNTMANAGER_POLL,(WPARAM) this->m_hWnd, (LPARAM) &m_lProjectItem.at(iNr)->m_AccountManagerRpcReply);
	}

	return 0;
}

LRESULT CRemoteCombinedProjectView::OnAccountManagerPollReady(WPARAM wParam, LPARAM lParam)
{
	int iResult;//, sFormat;
	CString sHostName;
	CString sTxt, sFormat;
	ACCT_MGR_RPC_REPLY *pAccountManagerReply;
	CLoggingMessage log;
	log.m_pFrom = "Synchronize with manager";

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	iResult	= (int) wParam;
	pAccountManagerReply = (ACCT_MGR_RPC_REPLY *) lParam;

	sHostName = "??";
	for (int iCount = 0; iCount < (int) m_lProjectItem.size(); iCount++)
	{
		if (&m_lProjectItem.at(iCount)->m_AccountManagerRpcReply == pAccountManagerReply)
		{
			if (m_lProjectItem.at(iCount)->m_pcAccountManagerRpcReplyComputer != NULL)
			{
				sHostName = m_lProjectItem.at(iCount)->m_pcAccountManagerRpcReplyComputer;
			}
		}
	}

	if (iResult)
	{
		sFormat = "ERROR: failed on computer: ";
		sFormat+= sHostName;
		log.m_pText = sFormat.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, true);
		sFormat.ReleaseBuffer();
		return 0;
	}
	else
	{
		if (pAccountManagerReply->error_num)
		{
			sTxt = "ERROR: failed on computer: ";
			sTxt+= sHostName;
			sFormat.Format(" - Error code: %d", pAccountManagerReply->error_num);
			sTxt+= sFormat;
			log.m_pText = sTxt.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, true);
			sTxt.ReleaseBuffer();
			return 0;
		}
		else
		{
			sFormat = "Completed on computer: ";
			sFormat+= sHostName;
			log.m_pText = sFormat.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, true);
			sFormat.ReleaseBuffer();
		}
	}

	return 0;
}


void CRemoteCombinedProjectView::OnUpdateSetDebt(CCmdUI* pCmdUI)
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	if (m_pSelection->SelectionNumber() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}


void CRemoteCombinedProjectView::OnProjectsSetDebt()
{
	bool	bSelected;
	int		iCount, iNrItems, iThreadId;

	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	if (iNrItems > MAX_PROJECTS) iNrItems = MAX_PROJECTS;

	m_iSelectedProjectItems = 0;
	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		bSelected =m_pSelection->SelectionSelected(&listCtrl, iCount);
		if (bSelected)
		{
			m_sSelectedProjectItemsComputer[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
			m_sSelectedProjectItemsName[m_iSelectedProjectItems] = listCtrl.GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_PROJECT]);
			m_iSelectedProjectItems++;
			break;	// only one

		}
	}

	for (iCount = 0; iCount < m_iSelectedProjectItems; iCount++)
	{
		bool bFound;

		bFound = m_pDoc->GetRpcThreadId(&m_sSelectedProjectItemsComputer[iCount], &iThreadId);
		if (bFound)
		{
			theApp.m_pDlgSetDebt->PostMessage(UWM_MSG_SET_DEBT_OPEN_DIALOG, (WPARAM) &m_sSelectedProjectItemsName[0],(LPARAM) &m_sSelectedProjectItemsComputer[0]);
//			m_pDoc->GetComputerIds(&pcComputerId, iCount);
//			m_pDoc->GetRpcThreadId(&iThreadId, iCount);			// no no this way we get the wrong one, we already got the right one.
			theApp.m_pDlgSetDebt->PostMessage(UWM_MSG_SET_DEBT_SET_THREAD_ID, (WPARAM) iThreadId, 0);
		}
		break; // only one
	}	
}

void CRemoteCombinedProjectView::OnWww1() {m_contextMenu.WWW(0);}
void CRemoteCombinedProjectView::OnWww2() {m_contextMenu.WWW(1);}
void CRemoteCombinedProjectView::OnWww3() {m_contextMenu.WWW(2);}
void CRemoteCombinedProjectView::OnWww4() {m_contextMenu.WWW(3);}
void CRemoteCombinedProjectView::OnWww5() {m_contextMenu.WWW(4);}
void CRemoteCombinedProjectView::OnWww6() {m_contextMenu.WWW(5);}
void CRemoteCombinedProjectView::OnWww7() {m_contextMenu.WWW(6);}
void CRemoteCombinedProjectView::OnWww8() {m_contextMenu.WWW(7);}
void CRemoteCombinedProjectView::OnWww9() {m_contextMenu.WWW(8);}
void CRemoteCombinedProjectView::OnWww10() {m_contextMenu.WWW(9);}
void CRemoteCombinedProjectView::OnWww11() {m_contextMenu.WWW(10);}
void CRemoteCombinedProjectView::OnWww12() {m_contextMenu.WWW(11);}
void CRemoteCombinedProjectView::OnWww13() {m_contextMenu.WWW(12);}
void CRemoteCombinedProjectView::OnWww14() {m_contextMenu.WWW(13);}
void CRemoteCombinedProjectView::OnWww15() {m_contextMenu.WWW(14);}
void CRemoteCombinedProjectView::OnWww16() {m_contextMenu.WWW(15);}
void CRemoteCombinedProjectView::OnWww17() {m_contextMenu.WWW(16);}
void CRemoteCombinedProjectView::OnWww18() {m_contextMenu.WWW(17);}
void CRemoteCombinedProjectView::OnWww19() {m_contextMenu.WWW(18);}
void CRemoteCombinedProjectView::OnWww20() {m_contextMenu.WWW(19);}

LRESULT CRemoteCombinedProjectView::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	m_iTimerWu = 1000;				// 1000 to ensure direct timer message
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnSelTabView(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	m_pDoc->OnShowView((int) wParam, (int) lParam);
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam)
{
	CString *psComputer;
	int		iThreadId, iReconnect;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	psComputer = (CString *) wParam;
	iReconnect = (int) lParam;

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
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,iReconnect);
		}
	}
	
	m_iTimerWu = 1000;				// 1000 to ensure direct timer message
	m_tIntervalTasksCount = 0;
	theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	return 0;
}

void CRemoteCombinedProjectView::OnToolbarProjectUpdate()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_UPDATE);
}

void CRemoteCombinedProjectView::OnUpdateToolbarProjectUpdate(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedProjectView::OnToolbarProjectSuspend()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_SUSPEND);
}

void CRemoteCombinedProjectView::OnUpdateToolbarProjectSuspend(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedProjectView::OnToolbarProjectResume()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_RESUME);
}

void CRemoteCombinedProjectView::OnUpdateToolbarProjectResume(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedProjectView::OnToolbarProjectNoNew()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_NOMOREWORK);
}

void CRemoteCombinedProjectView::OnUpdateToolbarProjectNoNew(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedProjectView::OnToolbarProjectAllowNew()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostProjectOperation(PROJECT_OP_ALLOWMOREWORK);
}

void CRemoteCombinedProjectView::OnUpdateToolbarProjectAllowNew(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}


void CRemoteCombinedProjectView::ToolbarUpdateProject(CCmdUI* pCmdUI)
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

void CRemoteCombinedProjectView::OnToolbarAllowNetworkPref()
{
	ToolbarAllowNetwork(1);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowNetworkPref(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}
void CRemoteCombinedProjectView::OnToolbarAllowNetworkNever()
{
	ToolbarAllowNetwork(2);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowNetworkNever(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedProjectView::ToolbarAllowNetwork(int iMode)
{
	int	iNr;

	std::deque<CString> lSelectedComputers;

	GetSelectedComputers(&lSelectedComputers);

	for (int iPos = 0; iPos < (int) lSelectedComputers.size(); iPos++)
	{
		iNr = m_pDoc->GetComputerNumber(&lSelectedComputers.at(iPos));
		if (iNr >= 0)
		{
//			m_pDoc->OnComputerNet(iNr, iMode);
		}
	}
}

void CRemoteCombinedProjectView::OnToolbarAllowCpuPref()
{
	ToolbarAllowCpu(1);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowCpuPref(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}
void CRemoteCombinedProjectView::OnToolbarAllowCpuSnooze()
{
	ToolbarAllowCpu(2);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowCpuSnooze(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}
void CRemoteCombinedProjectView::OnToolbarAllowCpuNever()
{
	ToolbarAllowCpu(3);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowCpuNever(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedProjectView::ToolbarAllowCpu(int iMode)
{
	int	iNr;

	std::deque<CString> lSelectedComputers;

	GetSelectedComputers(&lSelectedComputers);

	for (int iPos = 0; iPos < (int) lSelectedComputers.size(); iPos++)
	{
		iNr = m_pDoc->GetComputerNumber(&lSelectedComputers.at(iPos));
		if (iNr >= 0)
		{
//			m_pDoc->OnComputerAllow(iNr, iMode);
		}
	}
}

void CRemoteCombinedProjectView::OnToolbarAllowGpuPref()
{
	ToolbarAllowGpu(1);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowGpuPref(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}
void CRemoteCombinedProjectView::OnToolbarAllowGpuSnooze()
{
	ToolbarAllowGpu(2);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowGpuSnooze(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}
void CRemoteCombinedProjectView::OnToolbarAllowGpuNever()
{
	ToolbarAllowGpu(3);
}
void CRemoteCombinedProjectView::OnUpdateToolbarAllowGpuNever(CCmdUI* pCmdUI)
{
	ToolbarUpdateProject(pCmdUI);
}

void CRemoteCombinedProjectView::ToolbarAllowGpu(int iMode)
{
	int	iNr;

	std::deque<CString> lSelectedComputers;

	GetSelectedComputers(&lSelectedComputers);

	for (int iPos = 0; iPos < (int) lSelectedComputers.size(); iPos++)
	{
		iNr = m_pDoc->GetComputerNumber(&lSelectedComputers.at(iPos));
		if (iNr >= 0)
		{
//			m_pDoc->OnComputerAllowGpu(iNr, iMode);
		}
	}
}

LRESULT CRemoteCombinedProjectView::OnFloaterData(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_GET_FLOATER_DATA, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_CPU, wParam, lParam);
	return 0;
}
LRESULT CRemoteCombinedProjectView::OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam)
{
	int		iColumnSet, iCheck, iLen, iLenMax;
	CString	sTxt;

	iColumnSet = (int) wParam;
	iCheck = (int) lParam;

	CListCtrl& listCtrl = GetListCtrl();

	if (iCheck > 0)
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
		if (iCheck == -1)
		{
			return listCtrl.GetColumnWidth(m_iColumnOrder[iColumnSet]);
		}
		else
		{
			listCtrl.SetColumnWidth(m_iColumnOrder[iColumnSet],0);
		}
	}

	Invalidate(TRUE);
	return -1;
}

LRESULT CRemoteCombinedProjectView::OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, wParam, lParam);
	return 0;
}

//LRESULT CRemoteCombinedProjectView::OnBoincTasksVersion(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_UPDATE,parm1,parm2);
//	return 0;
//}

//LRESULT CRemoteCombinedProjectView::OnBoincTasksRule(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_RULE,parm1,parm2);
//	return 0;
//}

LRESULT CRemoteCombinedProjectView::OnReadyStats(WPARAM parm1, LPARAM parm2)
{
	int iStatus;
	CString *psStat;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	time_t tSystemTime;
	_time64(&tSystemTime);

	psStat = (CString *) parm1;
	iStatus = (int) parm2;
	if (iStatus != STATS_ERROR)
	{
		ProcessStatsXml(psStat);
	}
	else
	{
		m_bThreadStatRequestError = true;
	}

	m_iThreadStatRequest--;
	if (m_iThreadStatRequest == 0)
	{
		if (m_bThreadStatRequestError)
		{
			m_tNextStatsReadWeb = tSystemTime + m_iNextStatsReadInterval;
			m_iNextStatsReadInterval += 60;			// add a minute
			if (m_iNextStatsReadInterval > 60*60)
			{
				m_iNextStatsReadInterval = 60*60;	// max 1 hour interval.
			}
			theApp.WriteProfileInt(registrySectionGeneral, registryReadStatsTime, (int) (m_tNextStatsReadWeb/3600));
		}
		else
		{
			if (iStatus == STATS_OK_WEB)
			{
				m_tNextStatsReadWeb = tSystemTime + 43200;	// 12 hour interval
				theApp.WriteProfileInt(registrySectionGeneral, registryReadStatsTime, (int) (m_tNextStatsReadWeb/3600));
			}
			m_iNextStatsReadInterval = 0;
		}

		m_pThreadStats = NULL;
	}
	delete psStat;
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnSearchComputerReady(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pComputerView->SendMessage(UWM_DLG_SEARCH_COMPUTER_READY,parm1,parm2);
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnFontChangeAll(WPARAM wParam,LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->m_pComputerView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pProjectView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTransferView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pMessageView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);

	return 0;
}

LRESULT CRemoteCombinedProjectView::OnFontHasChanged(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLf;
	pLf = (LOGFONT *) wParam;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLf);
	SetFont(m_pFont,TRUE);

	return 0;
}

void CRemoteCombinedProjectView::OnHeaderSetItems()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	theApp.m_pDlgSettingsMain->SetActivePage(theApp.m_pDlgSettingsProjects);
	theApp.m_pDlgSettingsMain->ShowWindow(SW_SHOW);
}

void CRemoteCombinedProjectView::OnSize(UINT nType, int cx, int cy )
{
	theApp.m_pMainFrame->m_dlgBarComputer.PostMessage(UWM_MSG_NEW_SIZE, cx, cy);
	CTemplateRemoteProjectView::OnSize(nType, cx, cy );
}

void CRemoteCombinedProjectView::OnCopyalltoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	ClipBoard(true);
}

void CRemoteCombinedProjectView::OnCopyselectedtoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	ClipBoard(false);
}

LRESULT CRemoteCombinedProjectView::OnWebItemSelected(WPARAM wParam,LPARAM lParam)
{
	int	iRow, iOption;
	CHtmlExchange *pHtmlExchange;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	pHtmlExchange = (CHtmlExchange *) lParam;
	iRow = pHtmlExchange->m_iRow;
	iOption = pHtmlExchange->m_iOption;

	m_pSelection->SelectionSet(iRow, iOption);
	if (pHtmlExchange->m_bSendData)
	{
		if (!m_bThreadBusy)
		{
			CListCtrl& listCtrl = GetListCtrl();
			CCreateHtmlWebServer createHtml;
			createHtml.Create(PosBarViewSelectProjects, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_PROJECT_COLUMNS, NULL, PosViewProjecsRowProject, &pHtmlExchange->m_pcHtml);
			pHtmlExchange->tTime = GetTickCount()/100;
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CRemoteCombinedProjectView::OnWebOperation(WPARAM parm1, LPARAM parm2)
{
	int iOperation;

	iOperation = (int) parm1;

	if (iOperation < PROJECT_OP_RESET)
	{
		PostProjectOperation(iOperation);
	}
	return 0;
}

LRESULT CRemoteCombinedProjectView::OnWebSort(WPARAM parm1, LPARAM parm2)
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
		createHtml.Create(PosBarViewSelectProjects, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_PROJECT_COLUMNS, NULL, PosViewProjecsRowProject, &pcHtml);
		m_iTimerWu = 1000;				// 1000 to ensure direct timer message
	}
	return (LRESULT) pcHtml;
}

LRESULT CRemoteCombinedProjectView::OnWebProperties(WPARAM parm1, LPARAM parm2)
{
	BOOL bResult; 
		
	bResult =	ProjectProperties(PROPERTIES_MOBILE);
	return bResult;
}

void CRemoteCombinedProjectView::OnComputersDetect()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SELECTED,BAR_SELECTED_COMPUTERS,0);
	m_pDoc->m_pComputerView->PostMessage(WM_COMMAND, ID_COMPUTERS_DETECT,0);
}
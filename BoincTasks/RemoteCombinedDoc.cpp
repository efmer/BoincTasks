// RemoteDoc.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
//#include "ComputerDoc.h"
#include "RemoteCombinedDoc.h"
#include "ListViewEx.h"					// base class 
#include "TemplateRemoteTaskView.h"		// base class 
#include "TemplateRemoteTransferView.h" // base class

#include "RemoteCombinedTaskView.h"
#include "TemplateRemoteMessageView.h"
#include "RemoteCombinedMessageView.h"
#include "TemplateRemoteProjectView.h"	// base class
#include "RemoteCombinedProjectView.h"
#include "RemoteCombinedTransferView.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "TemplateRemoteComputerView.h"
#include "RemoteCombinedComputerView.h"
#include "RemoteCombinedNoticesView.h"
#include "DlgUpdate.h"
#include "DlgSettingsTasks.h"
#include "computer.h"
#include "ThreadRpc.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"
#include "DlgComputerLocation.h"
#include "DlgNotices.h"
#include "DlgGraphic.h"
#include "DlgGraphicTransfer.h"
#include "DlgGraphicTasks.h"
#include "DlgGraphicDeadline.h"
#include "DlgGraphicTemperature.h"
#include "DlgTThrottle.h"
#include "DlgThreadGraph.h"
#include "DlgHistory.h"
#include "DlgEditConfig.h"
#include "DlgEditAppInfoConfig.h"
#include "DlgSelectComputer.h"

#include "dlgprefdisk.h"
#include "dlgprefprocessor.h"
#include "dlgprefnetwork.h"
//#include "DlgPrefExclusive.h"
#include "dlgprefmain.h"

#include "DlgLogging.h"
#include "DlgSettingsGeneral.h"
#include "DlgSettingsWWW.h"
#include "DlgSettingsMobile.h"
#include "DlgRulesLogging.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsExpert.h"
#include "DlgSettingsHistory.h"
#include "DlgBarComputer.h"
#include "DlgBarViewSelect.h"
#include "DlgColorMain.h"
#include "mainfrm.h"
#include <AFXPRIV.H>
#include "Translation.h"
#include "ThreadRpcClient.h"
#include "DateConvert.h"
#include "TimeString.h"
#include "Mac.h"

//#include "Xml.h"

// CRemoteCombinedDoc

IMPLEMENT_DYNCREATE(CRemoteCombinedDoc, CDocument)

CRemoteCombinedDoc::CRemoteCombinedDoc()
{
	// log version number
	int	iCheck;
	CString sFormat, sVersion;

//	CMenu menuSynchronize;
//	menuSynchronize.Attach(theApp.m_hMenuSynchronize);
//	ClearMenu(&menuSynchronize);

//	CMenu menuBenchmark;
//	menuBenchmark.Attach(theApp.m_hMenuBenchmark);
//	ClearMenu(&menuBenchmark);

//	CMenu menuReadConfig;
//	menuReadConfig.Attach(theApp.m_hMenuReadConfig);
//	ClearMenu(&menuReadConfig);

//	CMenu menuNetwork;
//	menuNetwork.Attach(theApp.m_hMenuNetwork);
//	ClearMenu(&menuNetwork);

//	CMenu menuAllow;
//	menuAllow.Attach(theApp.m_hMenuAllow);
//	ClearMenu(&menuAllow);

//	CMenu menuAllowGpu;
//	menuAllowGpu.Attach(theApp.m_hMenuAllowGpu);
//	ClearMenu(&menuAllowGpu);

	m_pTaskView		= NULL;
	m_pMessageView	= NULL;
	m_pProjectView	= NULL;
	m_pTransferView = NULL;
	m_pHistoryView	= NULL;
	m_pComputerView	= NULL;
	m_pNoticesView	= NULL;
	m_pDlgNotices = NULL;
	m_pDlgGraphic = NULL;
	m_pDlgGraphicTransfer = NULL;
	m_pDlgGraphicTasks = NULL;
	m_pDlgTemperatureGraphic = NULL;
	m_pDlgTThrottle = NULL;
	m_pDlgThreadGraph = NULL;
	m_pDlgSelectComputer = NULL;

	CopyComputerList();
	m_iStatusTextSet = RPC_STATUS_NONE;

	m_iViewActive = TASK_VIEW_ACTIVE;

	m_pDlgNotices = new CDlgNotices;
	m_pDlgNotices->Create(IDD_DIALOG_NOTICES_DUMMY);
	m_pDlgNotices->InitialUpdate(this);

	m_pDlgGraphic = new CDlgGraphic;
	if (m_iSelectedComputers > 10)	iCheck = theApp.GetProfileIntA(registrySectionGraphic, registryExpanded, 1);
	else iCheck = theApp.GetProfileIntA(registrySectionGraphic, registryExpanded, 0);
	m_pDlgGraphic->m_iExpand = iCheck;
	if (iCheck)	m_pDlgGraphic->Create(IDD_DIALOG_GRAPHIC_LARGE);
	else m_pDlgGraphic->Create(IDD_DIALOG_GRAPHIC);
	m_pDlgGraphic->InitialUpdate(this);

	m_pDlgGraphicTransfer = new CDlgGraphicTransfer;
	m_pDlgGraphicTransfer->Create(IDD_DIALOG_GRAPHIC_TRANSFER);
	m_pDlgGraphicTransfer->InitialUpdate(this);

	m_pDlgThreadGraph = new CDlgThreadGraph;
	m_pDlgThreadGraph->Create(IDD_DIALOG_GRAPHIC_THREAD);
	m_pDlgThreadGraph->InitialUpdate(this);	

	m_pDlgGraphicTasks = new CDlgGraphicTasks;
	if (m_iSelectedComputers > 10)	iCheck = theApp.GetProfileIntA(registrySectionGraphicTasks, registryExpanded, 1);
	else iCheck = theApp.GetProfileIntA(registrySectionGraphicTasks, registryExpanded, 0);
	m_pDlgGraphicTasks->m_iExpand = iCheck;
	if (iCheck)	m_pDlgGraphicTasks->Create(IDD_DIALOG_GRAPHICWU_LARGE);
	else m_pDlgGraphicTasks->Create(IDD_DIALOG_GRAPHICWU);
	m_pDlgGraphicTasks->InitialUpdate(this);

	if (m_pDlgGraphicTasks != NULL) theApp.m_pDlgColorGraphic->PostMessage(UWM_GRAPHIC_COLOR_INIT, (WPARAM) m_pDlgGraphic->m_hWnd, (LPARAM) m_pDlgGraphicTasks->m_hWnd);	// tell the color dialog who we are.

	m_pDlgGraphicDeadline = new CDlgGraphicDeadline;
	if (m_iSelectedComputers > 10)	iCheck = theApp.GetProfileIntA(registrySectionGraphicDeadline, registryExpanded, 1);
	else iCheck = theApp.GetProfileIntA(registrySectionGraphicDeadline, registryExpanded, 0);
	m_pDlgGraphicDeadline->m_iExpand = iCheck;
	if (iCheck)	m_pDlgGraphicDeadline->Create(IDD_DIALOG_GRAPHIC_DEADLINE_LARGE);
	else m_pDlgGraphicDeadline->Create(IDD_DIALOG_GRAPHIC_DEADLINE);
	m_pDlgGraphicDeadline->InitialUpdate(this);
	if (m_pDlgGraphicDeadline != NULL) theApp.m_pDlgColorGraphic->PostMessage(UWM_GRAPHIC_COLOR_INIT2, (WPARAM) m_pDlgGraphicDeadline->m_hWnd, 0);	// tell the color dialog who we are.

	m_pDlgTemperatureGraphic = new CDlgGraphicTemperature;
	m_pDlgTemperatureGraphic->Create(IDD_DIALOG_GRAPHIC_TEMPERATURE);
	m_pDlgTemperatureGraphic->InitialUpdate(this);

	m_pDlgTThrottle = new CDlgTThrottle;
	m_pDlgTThrottle->Create(IDD_DIALOG_TTHROTTLE);
	m_pDlgTThrottle->InitialUpdate(this);

	m_pDlgEditConfig = NULL;
	m_pDlgEditAppInfo = NULL;

	m_pDlgHistory = new CDlgHistory;
	m_pDlgHistory->Create(IDD_DIALOG_HISTORY);
	m_pDlgHistory->InitialUpdate(this);

	m_pDlgPrefMain		= new CDlgPrefMain("P"); // translation someplace else 
	m_pDlgPrefDisk		= new CDlgPrefDisk(); m_pDlgPrefMain->m_pDlgPrefDisk = m_pDlgPrefDisk;
	m_pDlgPrefDisk->m_pDlgPrefMain = m_pDlgPrefMain;
	m_pDlgPrefNetwork	= new CDlgPrefNetwork(); m_pDlgPrefMain->m_pDlgPrefNetwork = m_pDlgPrefNetwork;
	m_pDlgPrefNetwork->m_pDlgPrefMain = m_pDlgPrefMain;
	m_pDlgPrefProcessor	= new CDlgPrefProcessor(); m_pDlgPrefMain->m_pDlgPrefProcessor = m_pDlgPrefProcessor;
	m_pDlgPrefProcessor->m_pDlgPrefMain = m_pDlgPrefMain;

//	m_pDlgPrefDisk->m_pDlgPrefMain = m_pDlgPrefMain;
//	m_pDlgPrefNetwork->m_pDlgPrefMain = m_pDlgPrefMain;
//	m_pDlgPrefProcessor->m_pDlgPrefMain = m_pDlgPrefMain;
//	m_pDlgPrefExclusive->m_pDlgPrefMain = m_pDlgPrefMain;

	m_pDlgPrefMain->AddPage(m_pDlgPrefProcessor);
	m_pDlgPrefMain->AddPage(m_pDlgPrefNetwork);
	m_pDlgPrefMain->AddPage(m_pDlgPrefDisk);
//	m_pDlgPrefMain->AddPage(m_pDlgPrefExclusive);
	m_pDlgPrefMain->Create(theApp.m_pMainWnd,WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX ,0);	// system anders geen icoon
//	m_pDlgPrefMain->SetActivePage(m_pDlgPrefExclusive);
	m_pDlgPrefMain->SetActivePage(m_pDlgPrefNetwork);							// init dialog
	m_pDlgPrefMain->SetActivePage(m_pDlgPrefDisk);								// init dialog
	m_pDlgPrefMain->SetActivePage(m_pDlgPrefProcessor);							// init dialog

	if (theApp.m_pDlgSettingsMobile->m_bMobileActive)
	{
		if (g_pThreadWebServer == NULL)
		{
			g_pThreadWebServer = (CThreadWebServer *) AfxBeginThread(RUNTIME_CLASS(CThreadWebServer));
			g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_START,0,theApp.m_pDlgSettingsMobile->m_iWebServerPort);
		}
	}

	theApp.m_pDlgSettingsWWW->PostMessage(UWM_MSG_START_CLOUD,0,0);
/*
	if (theApp.m_pDlgSettingsWWW->m_bCloudEnable)
	{
		if (g_pThreadCloudServer == NULL)
		{
			g_pThreadCloudServer = (CThreadCloudServer *) AfxBeginThread(RUNTIME_CLASS(CThreadCloudServer));
			g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_START,(WPARAM) theApp.m_pDlgSettingsWWW->m_hWnd, 0);
		}
	}
*/

	int iComputers;
	iComputers = m_iSelectedComputers;

/*
	// all computers
	if (iComputers < 3) {
		if (iComputers > 1)
		{
			menuSynchronize.AppendMenu(MF_STRING, ID_COMPUTER_SYNCHRONIZE_ALL, gszTranslation[PosGroupMenuCommonAllComputers]);
			menuSynchronize.AppendMenu(MF_SEPARATOR);
			menuBenchmark.AppendMenu(MF_STRING, ID_COMPUTER_BENCHMARK_ALL, gszTranslation[PosGroupMenuCommonAllComputers]);
			menuBenchmark.AppendMenu(MF_SEPARATOR);
			menuReadConfig.AppendMenu(MF_STRING, ID_COMPUTER_READCONFIG_ALL, gszTranslation[PosGroupMenuCommonAllComputers]);
			menuReadConfig.AppendMenu(MF_SEPARATOR);
		}
	}
*/
	theApp.m_pMainFrame->m_dlgBarComputer.RemoveTree();
	for (int iCount = 0; iCount < iComputers; iCount++)
	{
		theApp.m_pMainFrame->m_dlgBarComputer.AddComputer(m_lListConnectionsItems.at(iCount)->m_pcGroup, m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId);
		::PostMessage(m_pDlgGraphic->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
		::PostMessage(m_pDlgGraphicTransfer->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
		::PostMessage(m_pDlgGraphicTasks->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
		::PostMessage(m_pDlgGraphicDeadline->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
		::PostMessage(m_pDlgThreadGraph->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);

		// benchmark
//		menuBenchmark.AppendMenu(MF_STRING, ID_COMPUTER_BENCHMARK_0+iCount, m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId);

		// read config
//		menuReadConfig.AppendMenu(MF_STRING, ID_COMPUTER_READCONFIG_0+iCount, m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId);
		// read account manager synchronize
//		menuSynchronize.AppendMenu(MF_STRING, ID_COMPUTER_SYNCHRONIZE_0+iCount, m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId);

		m_lListConnectionsItems.at(iCount)->m_dBenchmarktime = 0;
		m_lListConnectionsItems.at(iCount)->m_CcStatus.task_mode = -1;
		m_lListConnectionsItems.at(iCount)->m_CcStatus.task_mode_delay = 0;
		m_lListConnectionsItems.at(iCount)->m_CcStatus.gpu_mode = -1;
		m_lListConnectionsItems.at(iCount)->m_CcStatus.gpu_mode_delay = -1;
	}
	theApp.m_pMainFrame->m_dlgBarComputer.AddComputerComplete();

//	if (iComputers < MAX_COMPUTERS_MENU)
//	{
//	}
//	else
//	{
//		// network connection
//		AppendMenu(theApp.m_hMenuNetwork, MF_STRING, ID_COMPUTER_NET_DIALOG, gszTranslation[PosGroupMenuExtraSelComputer]);
//	}

	if (m_pDlgTemperatureGraphic != NULL) m_pDlgTemperatureGraphic->PostMessage(UWM_MSG_THREAD_ID, iComputers, (LPARAM) &m_lListConnectionsItems);
	if (m_pDlgTThrottle != NULL) m_pDlgTThrottle->PostMessage(UWM_MSG_THREAD_ID, iComputers, (LPARAM) &m_lListConnectionsItems);

	theApp.m_pMainFrame->m_dlgBarComputer.SelectAllComputer();

	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_OPEN,0,0);	
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_TASKS,0);	

	m_bDocumentIsClosing = false;
//	menuAllow.Detach();
//	menuAllowGpu.Detach();
//	menuNetwork.Detach();
//	menuSynchronize.Detach();
//	menuBenchmark.Detach();
//	menuReadConfig.Detach();

	m_pDlgSelectComputer = new CDlgSelectComputer;
	m_pDlgSelectComputer->Create(IDD_DIALOG_SELECT_COMPUTER);
	m_pDlgSelectComputer->InitialUpdate(&m_lListConnectionsItems);
	SettingsTasks(theApp.m_pDlgSettingsTasks->m_bUserFriendly);

}
CRemoteCombinedDoc::~CRemoteCombinedDoc()
{
	theApp.m_bDocumentOpen = false;


	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_CLOSE,0,0);	

	int iComputers;
	iComputers = m_iSelectedComputers;
	
	for (int iCount = 0; iCount < (int) m_lListConnectionsItems.size(); iCount++)
	{
//		m_lListConnectionsItems.at(iCount)->m_hostInfo.coprocs.delete_coprocs(); xxxxxxxxxxxxxxxxxxxxxxxx

//		Should be deleted in the thread! delete m_pThreadRpc[iCount]->m_pcComputerId;delete m_pThreadRpc[iCount]->m_pcHostName;delete m_pThreadRpc[iCount]->m_pcPassword;
		if (m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId != NULL) delete m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId;
		if (m_lListConnectionsItems.at(iCount)->m_pcGroup != NULL) delete m_lListConnectionsItems.at(iCount)->m_pcGroup;


//		Should be deleted in the thread! delete m_pThreadTThrottleRpc[iCount]->m_pcComputerId; delete m_pThreadTThrottleRpc[iCount]->m_pcHostName; delete m_pThreadTThrottleRpc[iCount]->m_pcPassword;
		if (m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId != NULL) delete m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId;
	}


	for (int iCount = 0; iCount < (int) m_lListConnectionsItems.size(); iCount++)
	{
		delete m_lListConnectionsItems.at(iCount);
	}
//	m_lListConnectionsItems.clear();

	delete m_pDlgPrefMain; m_pDlgPrefMain = NULL;
	delete m_pDlgPrefDisk; m_pDlgPrefDisk = NULL;
	delete m_pDlgPrefNetwork; m_pDlgPrefNetwork = NULL;
	delete m_pDlgPrefProcessor; m_pDlgPrefProcessor = NULL;

	if (m_pDlgSelectComputer != NULL)
	{
		m_pDlgSelectComputer->SafeWindowPlacement();
		delete m_pDlgSelectComputer;
	}
	m_pDlgSelectComputer = NULL;

	if (m_pDlgGraphic != NULL)
	{
		m_pDlgGraphic->SafeWindowPlacement();
		delete m_pDlgGraphic;
	}
	if (m_pDlgGraphicTransfer != NULL)
	{
		m_pDlgGraphicTransfer->SafeWindowPlacement();
		delete m_pDlgGraphicTransfer;
	}
	if (m_pDlgGraphicTasks != NULL)
	{
		m_pDlgGraphicTasks->SafeWindowPlacement();
		delete m_pDlgGraphicTasks;
	}
	if (m_pDlgGraphicDeadline != NULL)
	{
		m_pDlgGraphicDeadline->SafeWindowPlacement();
		delete m_pDlgGraphicDeadline;
	}
	if (m_pDlgTemperatureGraphic != NULL)
	{
		m_pDlgTemperatureGraphic->SafeWindowPlacement();
		delete m_pDlgTemperatureGraphic;
	}
	if (m_pDlgTThrottle != NULL)
	{
		m_pDlgTThrottle->SafeWindowPlacement();
		delete m_pDlgTThrottle;
	}
	if (m_pDlgThreadGraph != NULL)
	{
		m_pDlgThreadGraph->SafeWindowPlacement();
		delete m_pDlgThreadGraph;
	}
	if (m_pDlgHistory != NULL)
	{
		m_pDlgHistory->SafeWindowPlacement();
		delete m_pDlgHistory;
	}

	if (m_pDlgEditConfig != NULL)
	{
		m_pDlgEditConfig->SafeWindowPlacement();
		delete m_pDlgEditConfig;
	}

	if (m_pDlgEditAppInfo != NULL)
	{
		m_pDlgEditAppInfo->SafeWindowPlacement();
		delete m_pDlgEditAppInfo;
	}

	delete m_pDlgNotices;

	// quiting is done in mainfrm
	CString sStatus;
	for (int iCount = 0; iCount < WEBSERVER_TIMEOUT; iCount++)
	{
		Sleep(1000);
		if (!g_bThreadWebServerIsRunning)
		{
			if (!g_bThreadCloudServerIsRunning)
			{
				break;
			}
		}
		sStatus.Format( "%d (M)",WEBSERVER_TIMEOUT - iCount);
		sStatus = gszTranslation[PosWindowStatusMessageStatusClosing] + sStatus;
		theApp.m_pMainFrame->SetBoincTasksStatus(sStatus.GetBuffer());sStatus.ReleaseBuffer();
	}

	theApp.m_pMainFrame->PostMessage(UWM_MSG_DOCUMENT_HAS_CLOSED,0,0);
}

BOOL CRemoteCombinedDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;
	return TRUE;
}

BOOL CRemoteCombinedDoc::OnOpenDocument( LPCTSTR lpszPathName)
{
	BOOL bResult;
	
	bResult = CDocument::OnNewDocument();
	if (!bResult ) return FALSE;

	// now we show the tasksbar

	theApp.m_pMainFrame->PostMessage(UWM_TASKBAR_ACTIVATE, 0,0);

	return TRUE;
}

void CRemoteCombinedDoc::OnCloseDocument()
{
}

void CRemoteCombinedDoc::OnCloseDocumentMine()
{
	// tell graphic dlg .. and wait for it to finish
//	::SendMessage(theApp.m_pDlgGraphic->m_hWnd,WM_CLOSE_GRAPHIC,(WPARAM) &m_sId,(LPARAM) m_pThreadRpc); 

	if (!m_bDocumentIsClosing) 
	{
		m_pTaskView->PostMessage(UWM_MSG_DOCUMENT_IS_CLOSING,0,0);
		m_bDocumentIsClosing = true;
		if (g_pThreadWebServer)
		{
			g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_QUIT,0,0);
		}
		if (g_pThreadCloudServer)
		{
			g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_QUIT,0,0);
		}
	}

	this->m_pComputerView->SaveComputerList();

//	CDocument::OnCloseDocument();		// never here
}

void CRemoteCombinedDoc::NowCloseDocument()
{
	CDocument::OnCloseDocument();
}

void CRemoteCombinedDoc::Begin()
{
	CMDIChildWnd *pChild; 
	CMDIFrameWnd *pMainWnd;

	// get the 1e view = Task view.
	POSITION pos = GetFirstViewPosition();
	m_pTaskView = (CRemoteCombinedTaskView*) GetNextView(pos);

	pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();
	pChild = (CMDIChildWnd*)pMainWnd->MDIGetActive();

	// create message view
	m_pMessageView = new CRemoteCombinedMessageView;
	m_pMessageView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0),  pChild, AFX_IDW_PANE_FIRST+1, NULL);
	::PostMessage(m_pMessageView->m_hWnd,WM_INITIALUPDATE,0,0);
	m_pMessageView->InitialUpdate(this);

	// create projects view
	m_pProjectView = new CRemoteCombinedProjectView;
	m_pProjectView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0),  pChild, AFX_IDW_PANE_FIRST+2, NULL);
	::PostMessage(m_pProjectView->m_hWnd,WM_INITIALUPDATE,0,0);
	m_pProjectView->InitialUpdate(this);

	// create transfers view
	m_pTransferView = new CRemoteCombinedTransferView;
	m_pTransferView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0),  pChild, AFX_IDW_PANE_FIRST+3, NULL);
	::PostMessage(m_pTransferView->m_hWnd,WM_INITIALUPDATE,0,0);
	m_pTransferView->InitialUpdate(this);

	// create history view
	m_pHistoryView = new CRemoteCombinedHistoryView;
	m_pHistoryView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0),  pChild, AFX_IDW_PANE_FIRST+4, NULL);
	::PostMessage(m_pHistoryView->m_hWnd,WM_INITIALUPDATE,0,0);
	m_pHistoryView->InitialUpdate(this);

	// create computer view
	m_pComputerView = new CRemoteCombinedComputerView;
	m_pComputerView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0),  pChild, AFX_IDW_PANE_FIRST+5, NULL);
	::PostMessage(m_pComputerView->m_hWnd,WM_INITIALUPDATE,0,0);
	m_pComputerView->InitialUpdate(this);

	// create notices view
	m_pNoticesView = new CRemoteCombinedNoticesView;
	m_pNoticesView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0),  pChild, AFX_IDW_PANE_FIRST+6, NULL);
	::PostMessage(m_pNoticesView->m_hWnd,WM_INITIALUPDATE,0,0);
	m_pNoticesView->InitialUpdate(this);

	theApp.m_bDocumentOpen = true;

	bool	bShow;

	bShow = !theApp.m_pDlgSettingsGeneral->m_bHideAtStartup;

	if (theApp.m_bShowAtStartup) bShow = true;

#ifdef _DEBUG
	bShow = true;
#endif

	if (bShow)
	{
		if (theApp.m_bDocumentFirstTimeOpen)
		{
			theApp.m_bDocumentFirstTimeOpen = false;			// only once at startup.
			theApp.m_pMainWnd->ShowWindow(SW_SHOW);
//			if (theApp.m_pMainWnd->IsIconic())
//			{
//				theApp.m_pMainWnd->ShowWindow(SW_NORMAL);
//			}
			theApp.m_pMainWnd->SetForegroundWindow();
		}
	}
}

void CRemoteCombinedDoc::SettingsTasks(bool bUserFriendly)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		m_lListConnectionsItems.at(iCount)->m_pThreadRpc->PostThreadMessage(UWM_MSG_THREAD_SETTING_TASKS,bUserFriendly,0);
	}
}

bool CRemoteCombinedDoc::SetSnooze(CString *psComputerId, int iTime)
{
	int iNr;

	if (GetRpcThreadConnected(psComputerId))
	{
		iNr = GetComputerNumber(psComputerId);
		if (iNr >=0)
		{
			m_lListConnectionsItems.at(iNr)->m_CcStatus.task_mode = RUN_MODE_RESTORE;
			m_lListConnectionsItems.at(iNr)->m_CcStatus.task_mode_delay = 0;
			::PostThreadMessage(m_lListConnectionsItems.at(iNr)->m_iThreadRpcId,UWM_MSG_THREAD_SET_SNOOZE_CPU,(WPARAM) m_pTaskView->m_hWnd, (LPARAM) &m_lListConnectionsItems.at(iNr)->m_CcStatus);
			if (iTime > 0)
			{
				m_lListConnectionsItems.at(iNr)->m_CcStatus.task_mode = RUN_MODE_NEVER;
				m_lListConnectionsItems.at(iNr)->m_CcStatus.task_mode_delay = iTime;
				::PostThreadMessage(m_lListConnectionsItems.at(iNr)->m_iThreadRpcId,UWM_MSG_THREAD_SET_SNOOZE_CPU,(WPARAM) m_pTaskView->m_hWnd, (LPARAM) &m_lListConnectionsItems.at(iNr)->m_CcStatus);
			}
			return true;
		}
	}
	return false;
}


void CRemoteCombinedDoc::ClearMenu(CMenu *pMenu)
{
	BOOL	bFound;

	bFound = TRUE;
	while (bFound)
	{
		bFound = pMenu->RemoveMenu(0,MF_BYPOSITION);
	}
}

void CRemoteCombinedDoc::CopyComputerList()
{
	int			iPos;
	int			iNrComputers, iLocalhostCount;
	CComputer	*pComputerFrom;
	CString		sHostName, sMAC, sComputerId, sPassword, sTxt, sFormat;

	CMac		mac;

	// copy computer list

	iPos = 0;
	iNrComputers = (int) theApp.m_lComputers.size();

	sFormat = "Maximum number of computers: Unlimited";
	CLoggingMessage sLog;
	sLog.m_pFrom = ""; sLog.m_pText = sFormat.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);

	m_iSelectedComputers = 0;
	iLocalhostCount = 0;

	for (int iCount = 0; iCount < iNrComputers; iCount++)
	{
		pComputerFrom = theApp.m_lComputers.at(iPos);
		iPos++;

		if (pComputerFrom->m_iComputerChecked >0)
		{
			CRpcConnectionItem *pRpcConnectionItem;
			pRpcConnectionItem = new CRpcConnectionItem;

			pRpcConnectionItem->m_pThreadRpc = (CThreadRpc *) AfxBeginThread(RUNTIME_CLASS(CThreadRpc));
			pRpcConnectionItem->m_bActive = true;
			pRpcConnectionItem->m_iThreadRpcId = pRpcConnectionItem->m_pThreadRpc->m_nThreadID;

			pRpcConnectionItem->m_pThreadTThrottleRpc = (CThreadRpcClient *) AfxBeginThread(RUNTIME_CLASS(CThreadRpcClient));
			pRpcConnectionItem->m_iThreadTThrottleRpcId = pRpcConnectionItem->m_pThreadTThrottleRpc->m_nThreadID;

			if (pComputerFrom->m_sComputerId.GetLength() > 0) sComputerId = pComputerFrom->m_sComputerId;
			else sComputerId = pComputerFrom->m_sComputerIp;

			char *pBufferString;
			pBufferString = sComputerId.GetBuffer();
			StringCopyNew(&pRpcConnectionItem->m_pThreadRpc->m_pcComputerId,  pBufferString, false);
//			StringCopyNew(&pRpcConnectionItem->m_pThreadTThrottleRpc->m_pcComputerId, pBufferString, false);
			pRpcConnectionItem->m_pThreadTThrottleRpc->m_sComputerId = pBufferString;

			StringCopyNew(&pRpcConnectionItem->m_pcThreadRpcId, pBufferString, false);
			StringCopyNew(&pRpcConnectionItem->m_pcGroup, pComputerFrom->m_sComputerGroup.GetBuffer(), false); pComputerFrom->m_sComputerGroup.ReleaseBuffer();

			StringCopyNew(&pRpcConnectionItem->m_pcThreadTThrottleRpcId, pBufferString, false);
			sComputerId.ReleaseBuffer();

			if (pComputerFrom->m_sComputerIp.GetLength() > 0) sHostName = pComputerFrom->m_sComputerIp;			// use ip address if available
			else sHostName = pComputerFrom->m_sComputerId;
			sMAC = pComputerFrom->m_sComputerMAC;
			sPassword =  pComputerFrom->m_sComputerPassword;
			if (sPassword.GetAt(0) == PASS_CHAR) sPassword = sPassword.Mid(1);	// remove Password char if present.

			// check if localhost

			sHostName.MakeLower();

			if (sHostName == LOCALHOST_NAME)
			{
				iLocalhostCount++;
				if (iLocalhostCount > 1)
				{
					AfxMessageBox("Only one localhost is allowed, please rename the IP Address");
					sTxt.Format("%d",iLocalhostCount);
					sHostName += sTxt;	// rename the > 1 localhost
				}
				else
				{
					// only use the local passwrd file if the passwrd is blank
					if (sPassword.GetLength() == 0)
					{
						CString sDir;
						sPassword = theApp.GetPassword(&sDir);
						sTxt = "localhost get local password: (";
						sTxt+= sDir;
						sTxt+= ") ";
						sTxt+= sPassword;
						CLoggingMessage sLog;
						sLog.m_pFrom = "Computers"; sLog.m_pText = sTxt.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_DEBUG);
					}
				}
			}

			pComputerFrom->m_sComputerMAC.Replace(":","-");
			pComputerFrom->m_sComputerMAC.Replace(" ","");
			pComputerFrom->m_sComputerMAC.Replace(",",";");

			pComputerFrom->m_iValidMac =  mac.Extract(pComputerFrom->m_sComputerMAC.GetBuffer()); pComputerFrom->m_sComputerMAC.ReleaseBuffer();

			if (pComputerFrom->m_iValidMac > 0)
			{
				sTxt.Format("%s , MAC: %s", sComputerId, sMAC);	
			}
			else
			{
				sTxt.Format("%s , IP: %s", sComputerId, sHostName);	
			}
			CLoggingMessage sLog;
			sLog.m_pFrom = "Computers"; sLog.m_pText = sTxt.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);

			pBufferString = sHostName.GetBuffer();
			StringCopyNew(&pRpcConnectionItem->m_pThreadRpc->m_pcHostName, pBufferString, false);
//			StringCopyNew(&pRpcConnectionItem->m_pThreadTThrottleRpc->m_pcHostName, pBufferString, false);
			pRpcConnectionItem->m_pThreadTThrottleRpc->m_sHostName = sHostName;
			sHostName.ReleaseBuffer();

			pBufferString = sMAC.GetBuffer();
			StringCopyNew(&pRpcConnectionItem->m_pThreadRpc->m_pcMAC, pBufferString, false);
			//TThrottle gets the valid IP later on.
//			StringCopyNew(&pRpcConnectionItem->m_pThreadTThrottleRpc->m_pcHostName, pBufferString, false);
			sHostName.ReleaseBuffer();
			pRpcConnectionItem->m_pThreadRpc->m_iValidMac = pComputerFrom->m_iValidMac;

			pBufferString = sPassword.GetBuffer();
			StringCopyNew(&pRpcConnectionItem->m_pThreadRpc->m_pcPassword, pBufferString, false);
//			StringCopyNew(&pRpcConnectionItem->m_pThreadTThrottleRpc->m_pcPassword, pBufferString, false);
			pRpcConnectionItem->m_pThreadTThrottleRpc->m_sPassword = pBufferString;
			sPassword.ReleaseBuffer();

			bool bLocalhostEqual = false;

			for (int iCount=0; iCount < (int) theApp.m_lLocalhost.size(); iCount++)
			{
				if (sHostName == theApp.m_lLocalhost.at(iCount).c_str())
				{
					bLocalhostEqual = true;
					break;
				}
			}
			pRpcConnectionItem->m_pThreadRpc->m_bLocalhostEqual = bLocalhostEqual;
			pRpcConnectionItem->m_pThreadRpc->m_iPort = pComputerFrom->m_iPort;
			pRpcConnectionItem->m_pThreadRpc->m_iEncryptedConnection = pComputerFrom->m_iEncryptedConnection;
			pRpcConnectionItem->m_pThreadRpc->m_bRegulatorActive = theApp.m_bRegulatorActive;
			pRpcConnectionItem->m_pThreadRpc->m_dRegulatorDaysWorkRequested = theApp.m_dRegulatorDaysWorkRequested;	// pComputerFrom->m_iDays;
			pRpcConnectionItem->m_pThreadRpc->m_iRegulatorMaxTasks = theApp.m_iRegulatorMaxTasks;					// pComputerFrom->m_iWu;
			pRpcConnectionItem->m_pThreadRpc->m_dRegulatorMinDaysGpu = theApp.m_dRegulatorMinDaysGpu;
			pRpcConnectionItem->m_pThreadRpc->m_dRegulatorMinDaysCpu = theApp.m_dRegulatorMinDaysCpu;
			pRpcConnectionItem->m_pThreadRpc->m_dRegulatorDaysWorkRequestedMin = theApp.m_dRegulatorDaysWorkRequestedMin;

			pRpcConnectionItem->m_pThreadRpc->m_bLogDebugHistoryFetching = theApp.m_pDlgLogging->m_bLogDebugHistoryFetching;
			pRpcConnectionItem->m_pThreadRpc->m_bLogDebugState = theApp.m_pDlgLogging->m_bLogDebugState;
			pRpcConnectionItem->m_pThreadRpc->m_bLogDebugConnecting = theApp.m_pDlgLogging->m_bLogDebugConnecting;
			pRpcConnectionItem->m_pThreadRpc->m_bLogDebugHeapCheck = theApp.m_pDlgLogging->m_bLogDebugHeapCheck;
			pRpcConnectionItem->m_pThreadRpc->m_bLogDebugTasksShow = theApp.m_pDlgLogging->m_bLogDebugTasksShow;

			

			pRpcConnectionItem->m_pThreadTThrottleRpc->m_iPort = pComputerFrom->m_iPort+1;	// port one up from BOINC

			// connect to host
			pRpcConnectionItem->m_pThreadRpc->PostThreadMessage(UWM_MSG_THREAD_CONNECT_RPC, 0 ,pRpcConnectionItem->m_pThreadTThrottleRpc->m_nThreadID);				// tthrottle thread in case we use MAC

			m_iSelectedComputers++;
			m_lListConnectionsItems.push_back(pRpcConnectionItem);
		}
	}

	// the menu's need one more as 0 = all


	CRpcConnectionItem *pRpcConnectionItem;
	pRpcConnectionItem = new CRpcConnectionItem;
	pRpcConnectionItem->m_pcThreadRpcId = NULL;				// never used
	pRpcConnectionItem->m_pcGroup = NULL;					// never used
	pRpcConnectionItem->m_pcThreadTThrottleRpcId = NULL;	// never used
	pRpcConnectionItem->m_pThreadRpc = NULL;				// never used
	pRpcConnectionItem->m_pThreadTThrottleRpc = NULL;		// never used
	m_lListConnectionsItems.push_back(pRpcConnectionItem);

	theApp.m_pDlgSettingsExpert->PostMessage(UWM_MSG_THREAD_RECONNECT_TIME_RPC,0,0);
}

void CRemoteCombinedDoc::RemoveThreadFromList(int iThreadId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (iThreadId == m_lListConnectionsItems.at(iCount)->m_iThreadRpcId)
		{
			m_lListConnectionsItems.at(iCount)->m_pThreadRpc = NULL;				// the whole pointer should be be invalid by now so set it to NULL
			m_lListConnectionsItems.at(iCount)->m_bActive = false;
		}
	}
}

void CRemoteCombinedDoc::DeleteThreadFromList()	// not in use?
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (m_lListConnectionsItems.at(iCount)->m_pThreadRpc != NULL)
		{
			delete (m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
			m_lListConnectionsItems.at(iCount)->m_pThreadRpc = NULL;
		}
	}
	m_lListConnectionsItems.clear();

}

// check if connected

bool CRemoteCombinedDoc::GetRpcThreadConnected(CString *pComputerId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (*pComputerId == m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId)
		{
			if (m_lListConnectionsItems.at(iCount)->m_iConnectionStatus == RPC_STATUS_CONNECTED) return true;
			return false;
		}
	}
	return false;
}

// return the hostinfo information

HOST_INFO *CRemoteCombinedDoc::GetRpcThreadHostInfo(CString *pComputerId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (*pComputerId == m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId)
		{
			return &m_lListConnectionsItems.at(iCount)->m_hostInfo;
		}
	}
	return NULL;
}

int CRemoteCombinedDoc::GetRpcSelectedComputers()
{
	return m_iSelectedComputers;
}

void CRemoteCombinedDoc::GetRpcThreadId(int *pThreadId, int iComputer)
{
	*pThreadId = m_lListConnectionsItems.at(iComputer)->m_iThreadRpcId;	
}

void CRemoteCombinedDoc::GetTThrottleRpcThreadId(int *pThreadId, int iComputer)
{
	*pThreadId = m_lListConnectionsItems.at(iComputer)->m_iThreadTThrottleRpcId;
}

// Get the Thread ID matching the computer name
// Return true if found

bool CRemoteCombinedDoc::GetRpcThreadId(CString *pComputerId, int *piThreadId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (*pComputerId == m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId)
		{
			*piThreadId = m_lListConnectionsItems.at(iCount)->m_iThreadRpcId;
			return true;
		}
	}
	*piThreadId = 0;
	return false;
}

bool CRemoteCombinedDoc::GetRpcThreadId(char *pcComputerId, int *piThreadId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (strcmp(pcComputerId, m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId) == 0)
		{
			*piThreadId = m_lListConnectionsItems.at(iCount)->m_iThreadRpcId;
			return true;
		}
	}
	*piThreadId = 0;
	return false;
}

// Get the Thread pointer matching the computer name
// Return true if found

bool CRemoteCombinedDoc::GetRpcThreadPointer(CString *pComputerId, CThreadRpc **piThreadId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
//		if (pComputerId->Find(m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,0) >= 0)
		if (*pComputerId == m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId)
		{
			*piThreadId = m_lListConnectionsItems.at(iCount)->m_pThreadRpc;
			return true;
		}
	}
	*piThreadId = 0;
	return false;
}

int CRemoteCombinedDoc::GetRpcArrayPos(CString *pComputerId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (*pComputerId == m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId)
		{
			return iCount;
		}
	}
	return -1;
}

void CRemoteCombinedDoc::GetComputerIds(char **pComputerId, int iComputer)
{
	*pComputerId = m_lListConnectionsItems.at(iComputer)->m_pcThreadRpcId;	
}

// get the computer id matching the thread id
bool	CRemoteCombinedDoc::GetComputerIdStringWithIdNumber(int iThreadId, char **pcComputerId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (iThreadId == m_lListConnectionsItems.at(iCount)->m_iThreadRpcId)
		{
			*pcComputerId = m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId;
			return true;
		}
	}
	return  false;
}

int	CRemoteCombinedDoc::GetComputerNumber(CString *pComputerId)
{
	for (int iPos = 0; iPos < (int) m_lListConnectionsItems.size(); iPos++)
	{
		if (m_lListConnectionsItems.at(iPos)->m_pcThreadRpcId != NULL)
		{
			if (m_lListConnectionsItems.at(iPos)->m_pcThreadRpcId == *pComputerId)
			{
				return iPos;
			}
		}
	}
	return -1;
}

// get the computer id matching the thread id
bool	CRemoteCombinedDoc::GetTThrottleComputerIdStringWithIdNumber(int iThreadId, char **pcComputerId)
{
	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (iThreadId == m_lListConnectionsItems.at(iCount)->m_iThreadTThrottleRpcId)
		{
			*pcComputerId = m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId;
			return true;
		}
	}
	return  false;
}

//void CRemoteCombinedDoc::SetHostInfoBusy(int iNr)
//{
//	m_bHostInfoBusy[iNr];
//}

CC_STATUS *CRemoteCombinedDoc::GetHostStatus(int iNr)
{
	return &m_lListConnectionsItems.at(iNr)->m_CcStatus;
}

HOST_INFO *CRemoteCombinedDoc::GetHostInfo(int iNr)
{
	return &m_lListConnectionsItems.at(iNr)->m_hostInfo;
}

ACCT_MGR_INFO *CRemoteCombinedDoc::GetAccountManagerInfo(int iNr)
{
	return &m_lListConnectionsItems.at(iNr)->m_accountManagerInfo;
}


void CRemoteCombinedDoc::DisplayStatus(int iStatusCount, int *piStatus, bool bRefresh)
{
	CString sTxt[5], sHost[5], sTxtWindow, sFormat, sViewTxt, sVersion;
//	int		siNr[5], iCount;
	bool	bStatusChanged = false;

//	sViewTxt = gszTranslation[PosWindowStatusMessageCombined];
	if (m_iViewActive == MESSAGE_VIEW_ACTIVE)	sViewTxt += gszTranslation[PosWindowStatusMessageMessages];
	if (m_iViewActive == TASK_VIEW_ACTIVE)		sViewTxt += gszTranslation[PosWindowStatusMessageTasks];	
	if (m_iViewActive == PROJECT_VIEW_ACTIVE)	sViewTxt += gszTranslation[PosWindowStatusMessageProjects];	
	if (m_iViewActive == TRANSFER_VIEW_ACTIVE)	sViewTxt += gszTranslation[PosWindowStatusMessageTransfer];	
	if (m_iViewActive == HISTORY_VIEW_ACTIVE)	sViewTxt += gszTranslation[PosWindowStatusMessageHistory];	
	if (m_iViewActive == COMPUTER_VIEW_ACTIVE)	sViewTxt += gszTranslation[PosWindowStatusMessageComputer];	
	if (m_iViewActive == NOTICES_VIEW_ACTIVE)	sViewTxt += gszTranslation[PosWindowStatusMessageNotices];	

//	sVersion.LoadString(IDS_PROGRAM_VERSION);
//	sVersion += ", ";

	if (sViewTxt.GetAt(0) == ' ')	// remove leading space
	{
		sViewTxt = sViewTxt.Mid(1);
	}

	sViewTxt.Remove('[');
//	sViewTxt.Remove(' ');
	sViewTxt.Remove(']');

	SetTitle(sVersion + sViewTxt + sTxtWindow);
}


CView* CRemoteCombinedDoc::SwitchToView ( CView* pNewView )
{
   CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();

   // Get the active MDI child window.

   CMDIChildWnd* pChild = (CMDIChildWnd*)pMainWnd->MDIGetActive();

   // Get the active view attached to the active MDI child window.

   CView* pOldActiveView = pChild->GetActiveView();

  // Exchange control ID of old view

	pNewView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	pOldActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST+1);

   // Set flag so that document will not be deleted when view is dettached.

   BOOL bAutoDelete = m_bAutoDelete;
   m_bAutoDelete = FALSE;

   // Dettach existing view

   RemoveView(pOldActiveView);

   // restore flag

   m_bAutoDelete = bAutoDelete;

   // Show the newly active view and hide the inactive view.

   pNewView->ShowWindow(SW_SHOW);
   pOldActiveView->ShowWindow(SW_HIDE);

   // Attach new view

   AddView(pNewView);

   pChild->RecalcLayout();
   pNewView->UpdateWindow();
   pChild->SetActiveView(pNewView);
   return pOldActiveView;
}


int CRemoteCombinedDoc::StringCopyNew(char **pcDest, char *pcSource, bool bDelete)
{
	int iLen;

	if (bDelete)
	{
		if (*pcDest != NULL)
		{
			delete *pcDest;
			*pcDest = NULL;
		}
	}

	if (pcSource == NULL) return 0;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
	return iLen;
}


void CRemoteCombinedDoc::LogActiveComputers()
{
	CLoggingMessage log;
	CString sTxt;
	int		iActiveComputers;
	DWORD	dwStatus;
	BOOL	bStatus;

	iActiveComputers = 0;

	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		if (m_lListConnectionsItems.at(iCount)->m_bActive)
		{
			iActiveComputers++;
			sTxt+= m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId;
			sTxt+= "  ";
			dwStatus = m_lListConnectionsItems.at(iCount)->m_pThreadRpc->SuspendThread();
			if (dwStatus == -1)
			{
				bStatus = ::TerminateThread(m_lListConnectionsItems.at(iCount)->m_pThreadRpc->m_hThread, 0);
				if (bStatus)
				{
					sTxt += "Killed ";
				}
				else
				{
					sTxt += "Failed ";
				}
			}
			else
			{
				sTxt += "Suspended ";
			}
		}
	}
	if (iActiveComputers > 0)
	{
		log.m_pFrom = "ERROR: Not closed down properly"; log.m_pText = sTxt.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
		sTxt.ReleaseBuffer();
	}
}

void CRemoteCombinedDoc::NoticesNew(CString *psHtml)
{
	CRemoteCombinedNoticesView *pNewNoticesView;

	CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();
	pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd *pChild = (CMDIChildWnd*)pMainWnd->MDIGetActive();

	pNewNoticesView = new CRemoteCombinedNoticesView;
	pNewNoticesView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0),  pChild, AFX_IDW_PANE_FIRST+6, NULL);
	::SendMessage(pNewNoticesView->m_hWnd,WM_INITIALUPDATE,0,0);
	pNewNoticesView->InitialUpdate(this);
	SwitchToView(pNewNoticesView);
	pNewNoticesView->NavigateHtml(psHtml);
	delete m_pNoticesView;
	m_pNoticesView = pNewNoticesView;
	m_iViewActive = NOTICES_VIEW_ACTIVE;
}

bool CRemoteCombinedDoc::GetSelectedComputers(std::deque<CString> *plSelectedComputers)
{
	int iNrItems, iItem, iCount, iCountDouble;
	bool	bSelected;
	POSITION pos;
	CString sComputerSelected, sHidden;

	// check current view

	CListCtrl	*plistCtrl;
	CSelection	*pSelection;
	int			iColumnComputer;

	switch (m_iViewActive)
	{
		case COMPUTER_VIEW_ACTIVE:
			plistCtrl = &m_pComputerView->GetListCtrl();
			iColumnComputer = COLUMN_COMPUTER_ID;
			pos = plistCtrl->GetFirstSelectedItemPosition();
			if (pos == NULL) return false;
			iItem = plistCtrl->GetNextSelectedItem(pos);
			sComputerSelected = plistCtrl->GetItemText(iItem, iColumnComputer);
			sComputerSelected = m_pComputerView->ExtractHidden(&sComputerSelected, &sHidden);
			pSelection = NULL;
		break;
		case PROJECT_VIEW_ACTIVE:
			plistCtrl = &m_pProjectView->GetListCtrl();
			iColumnComputer = m_pProjectView->m_iColumnOrder[COLUMN_PROJECT_COMPUTER];
			pSelection = m_pProjectView->m_pSelection;
		break;
		case TASK_VIEW_ACTIVE:
			plistCtrl = &m_pTaskView->GetListCtrl();
			iColumnComputer = m_pTaskView->m_iColumnOrder[COLUMN_COMPUTER];
			pSelection = m_pTaskView->m_pSelection;
		break;
		case TRANSFER_VIEW_ACTIVE:
			plistCtrl = &m_pTransferView->GetListCtrl();
			iColumnComputer = m_pTransferView->m_iColumnOrder[COLUMN_TRANSFER_COMPUTER];
			pSelection = m_pTransferView->m_pSelection;
		break;
		case MESSAGE_VIEW_ACTIVE:
			plistCtrl = &m_pMessageView->GetListCtrl();
			iColumnComputer = m_pMessageView->m_iColumnOrder[COLUMN_MESSAGE_COMPUTER];
			sComputerSelected = plistCtrl->GetItemText(0, iColumnComputer);
			pSelection = NULL;
		break;
		case HISTORY_VIEW_ACTIVE:
			plistCtrl = &m_pHistoryView->GetListCtrl();
			iColumnComputer = m_pHistoryView->m_iColumnOrder[COLUMN_HISTORY_COMPUTER];
			pSelection = m_pHistoryView->m_pSelection;
		break;
		default:
			plistCtrl = NULL;
	}

	if (plistCtrl == NULL) return false;

	bSelected = false;

	iNrItems = plistCtrl->GetItemCount();
	int iNrComputers = 0;
	CString sComputerFound1, sComputerFound2;

	if (pSelection)
	{
		for (iCount = 0; iCount < iNrItems; iCount++)
		{
			if (pSelection->SelectionSelected(plistCtrl, iCount))
			{
				bSelected = true;
				sComputerSelected = plistCtrl->GetItemText(iCount, iColumnComputer);
				// check if the entry is double
				bool	bDouble = false;
				if (plSelectedComputers)
				{
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
				else
				{
					return bSelected;
				}
			}

			sComputerFound2 = plistCtrl->GetItemText(iCount, iColumnComputer);
			if (sComputerFound1 != sComputerFound2)
			{
				sComputerFound1 = sComputerFound2;
				iNrComputers++;
			}
		}

		if (bSelected) return bSelected;
		// nothing selected, now find out if only one computer is visible.

		if (plSelectedComputers)
		{
			if (iNrComputers == 1)
			{
				// one found, set this one as selected
				plSelectedComputers->push_back(sComputerFound1);
				return bSelected;
			}
		}

		return bSelected;
	}

	if (sComputerSelected.GetLength() > 0)
	{
		bSelected = true;
		if (plSelectedComputers)		
		{
			plSelectedComputers->push_back(sComputerSelected);
		}
		return bSelected;
	}

	return bSelected;
}

BEGIN_MESSAGE_MAP(CRemoteCombinedDoc, CDocument)
ON_COMMAND(ID_COLORS_TASK, &CRemoteCombinedDoc::OnColorsDialog)

//ON_COMMAND(UWM_MSG_THREAD_GET_HOST_INFO_READY, &CRemoteCombinedDoc::OnHostInfo)	// not here is done directly

ON_COMMAND(ID_HELP_SHOWLOG_REMOTE, &CRemoteCombinedDoc::OnHelpShowlog)
ON_COMMAND(ID_HELP_SHOWLOG_RULES, &CRemoteCombinedDoc::OnHelpShowlogRules)

ON_COMMAND(ID_SHOW_PROJECTS, &CRemoteCombinedDoc::OnShowProjects)
ON_COMMAND(ID_SHOW_TASKS, &CRemoteCombinedDoc::OnShowTasks)
ON_COMMAND(ID_SHOW_TRANSFERS, &CRemoteCombinedDoc::OnShowTransfers)
ON_COMMAND(ID_SHOW_MESSAGES, &CRemoteCombinedDoc::OnShowMessages)
ON_COMMAND(ID_SHOW_HISTORY, &CRemoteCombinedDoc::OnShowHistory)
ON_COMMAND(ID_SHOW_HISTORYLONG, &CRemoteCombinedDoc::OnShowHistoryLong)
ON_UPDATE_COMMAND_UI(ID_SHOW_HISTORYLONG, &CRemoteCombinedDoc::OnUpdateShowHistoryLong)
ON_COMMAND(ID_SHOW_NOTICES, &CRemoteCombinedDoc::OnShowNotices)
ON_COMMAND(ID_SHOW_COMPUTERS, &CRemoteCombinedDoc::OnShowComputers)
ON_COMMAND(ID_SHOW_STATISTICS, &CRemoteCombinedDoc::OnShowStatistics)
ON_COMMAND(ID_SHOW_STATISTICS_TASKS, &CRemoteCombinedDoc::OnShowStatisticsTasks)
ON_COMMAND(ID_SHOW_GRAPH_DEADLINE, &CRemoteCombinedDoc::OnShowStatisticsDeadline)
ON_COMMAND(ID_SHOW_TEMPERATURE, &CRemoteCombinedDoc::OnShowTemperature)
ON_COMMAND(ID_SHOW_GRAPH_TRANSFER, &CRemoteCombinedDoc::OnShowGraphicTransfer)
ON_COMMAND(ID_SHOW_TTHROTTLE, &CRemoteCombinedDoc::OnShowTThrottle)
ON_UPDATE_COMMAND_UI(ID_SHOW_TTHROTTLE, &CRemoteCombinedDoc::OnUpdateTThrottle)

ON_COMMAND(ID_EXTRA_PREFERENCES, &CRemoteCombinedDoc::OnExtraPreferences)
ON_UPDATE_COMMAND_UI(ID_EXTRA_PREFERENCES, &CRemoteCombinedDoc::OnUpdateExtraPreferences)
ON_COMMAND(ID_EXTRA_PROXY, &CRemoteCombinedDoc::OnExtraProxy)
ON_UPDATE_COMMAND_UI(ID_EXTRA_PROXY, &CRemoteCombinedDoc::OnUpdateExtraProxy)

ON_COMMAND(ID_EXTRA_DONETWORKCOMMUNICATION, &CRemoteCombinedDoc::OnExtraDonetworkcommunication)
ON_UPDATE_COMMAND_UI(ID_EXTRA_DONETWORKCOMMUNICATION, &CRemoteCombinedDoc::OnUpdateExtraDonetworkcommunication)

ON_COMMAND(ID_EXTRA_SETTINGS, &CRemoteCombinedDoc::OnExtraSettings)
ON_COMMAND(ID_VIEW_COMPUTER_BAR, &CRemoteCombinedDoc::OnViewComputerBar)
ON_UPDATE_COMMAND_UI(ID_VIEW_COMPUTER_BAR, &CRemoteCombinedDoc::OnUpdateViewComputerBar)
ON_COMMAND(ID_VIEW_PROJECT_BAR, &CRemoteCombinedDoc::OnViewProjectBar)
ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECT_BAR, &CRemoteCombinedDoc::OnUpdateViewProjectBar)
ON_COMMAND(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_TASKS, &CRemoteCombinedDoc::OnSettinsApplyChangesTasks)
ON_COMMAND(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_PROJECTS, &CRemoteCombinedDoc::OnSettinsApplyChangesProjects)
ON_COMMAND(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_HISTORY, &CRemoteCombinedDoc::OnSettinsApplyChangesHistory)

ON_COMMAND(ID_SHOW_PROJECTS, &CRemoteCombinedDoc::OnShowProjects)

ON_COMMAND(UWM_RESTART_GRAPHIC, &CRemoteCombinedDoc::OnRestartGraphics)
ON_COMMAND(UWM_RESTART_GRAPHIC_TASKS, &CRemoteCombinedDoc::OnRestartGraphicsTasks)
ON_COMMAND(UWM_RESTART_GRAPHIC_TASKS_READY, &CRemoteCombinedDoc::OnRestartGraphicsTasksReady)
ON_COMMAND(UWM_RESTART_GRAPHIC_DEADLINE, &CRemoteCombinedDoc::OnRestartGraphicsDeadline)
ON_COMMAND(UWM_RESTART_GRAPHIC_DEADLINE_READY, &CRemoteCombinedDoc::OnRestartGraphicsDeadlineReady)

ON_COMMAND(ID_PROJECTS_ADDNEWPROJECT, &CRemoteCombinedDoc::OnAddNewProject)
ON_COMMAND(ID_PROJECTS_ADD_PROJECT_MANAGER, &CRemoteCombinedDoc::OnAddNewProjectManager)
ON_UPDATE_COMMAND_UI(ID_PROJECTS_ADD_PROJECT_MANAGER, &CRemoteCombinedDoc::OnUpdateAddNewProjectManager)

ON_COMMAND(ID_TOOLBAR_ALLOW_NETWORK_PREF,&CRemoteCombinedDoc::OnToolbarAllowNetworkPref)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_NETWORK_PREF,&CRemoteCombinedDoc::OnUpdateToolbarAllowNetworkPref)
ON_COMMAND(ID_TOOLBAR_ALLOW_NETWORK_NEVER,&CRemoteCombinedDoc::OnToolbarAllowNetworkNever)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_NETWORK_NEVER,&CRemoteCombinedDoc::OnUpdateToolbarAllowNetworkNever)
ON_COMMAND(ID_TOOLBAR_ALLOW_CPU_PREF,&CRemoteCombinedDoc::OnToolbarAllowCpuPref)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_CPU_PREF,&CRemoteCombinedDoc::OnUpdateToolbarAllowCpuPref)
ON_COMMAND(ID_TOOLBAR_ALLOW_CPU_SNOOZE,&CRemoteCombinedDoc::OnToolbarAllowCpuSnooze)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_CPU_SNOOZE,&CRemoteCombinedDoc::OnUpdateToolbarAllowCpuSnooze)
ON_COMMAND(ID_TOOLBAR_ALLOW_CPU_NEVER,&CRemoteCombinedDoc::OnToolbarAllowCpuNever)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_CPU_NEVER,&CRemoteCombinedDoc::OnUpdateToolbarAllowCpuNever)
ON_COMMAND(ID_TOOLBAR_ALLOW_GPU_PREF,&CRemoteCombinedDoc::OnToolbarAllowGpuPref)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_GPU_PREF,&CRemoteCombinedDoc::OnUpdateToolbarAllowGpuPref)
ON_COMMAND(ID_TOOLBAR_ALLOW_GPU_SNOOZE,&CRemoteCombinedDoc::OnToolbarAllowGpuSnooze)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_GPU_SNOOZE,&CRemoteCombinedDoc::OnUpdateToolbarAllowGpuSnooze)
ON_COMMAND(ID_TOOLBAR_ALLOW_GPU_NEVER,&CRemoteCombinedDoc::OnToolbarAllowGpuNever)
ON_UPDATE_COMMAND_UI(ID_TOOLBAR_ALLOW_GPU_NEVER,&CRemoteCombinedDoc::OnUpdateToolbarAllowGpuNever)

ON_COMMAND(ID_COMPUTER_EDIT_CONFIG, &CRemoteCombinedDoc::OnComputerEditConfig)
ON_UPDATE_COMMAND_UI(ID_COMPUTER_EDIT_CONFIG, &CRemoteCombinedDoc::OnUpdateComputerEditConfig)
ON_COMMAND(ID_COMPUTER_EDIT_APP_INFO, &CRemoteCombinedDoc::OnComputerEditAppInfo)
ON_UPDATE_COMMAND_UI(ID_COMPUTER_EDIT_APP_INFO, &CRemoteCombinedDoc::OnUpdateComputerEditAppInfo)

ON_COMMAND(UWM_MSG_SHOW_THREAD_GRAPH, &CRemoteCombinedDoc::OnShowThreadGraph)

ON_COMMAND(ID_HELP_UPDATE, &CRemoteCombinedDoc::OnHelpUpdate)

ON_COMMAND(				ID_COMPUTER_NET_DIALOG, &CRemoteCombinedDoc::OnComputerNetDialog)
ON_UPDATE_COMMAND_UI(	ID_COMPUTER_NET_DIALOG, &CRemoteCombinedDoc::OnUpdateComputerNetDialog)

ON_COMMAND(				ID_COMPUTER_ALLOW_DIALOG, &CRemoteCombinedDoc::OnComputerAllowDialog)
ON_UPDATE_COMMAND_UI(	ID_COMPUTER_ALLOW_DIALOG, &CRemoteCombinedDoc::OnUpdateComputerAllowDialog)

ON_COMMAND(				ID_COMPUTER_ALLOW_GPU_DIALOG, &CRemoteCombinedDoc::OnComputerAllowGpuDialog)
ON_UPDATE_COMMAND_UI(	ID_COMPUTER_ALLOW_GPU_DIALOG, &CRemoteCombinedDoc::OnUpdateComputerAllowGpuDialog)

ON_COMMAND(				ID_PROJECTS_SYNCHRONIZE_DIALOG, &CRemoteCombinedDoc::OnComputerSynchronizeDialog)
ON_UPDATE_COMMAND_UI(	ID_PROJECTS_SYNCHRONIZE_DIALOG, &CRemoteCombinedDoc::OnUpdateOnComputerSynchronizeDialog)

ON_COMMAND(				ID_COMPUTER_BENCHMARK_DIALOG, &CRemoteCombinedDoc::OnComputerBenchmarkDialog)
ON_UPDATE_COMMAND_UI(	ID_COMPUTER_BENCHMARK_DIALOG, &CRemoteCombinedDoc::OnUpdateOnComputerBenchmarkDialog)

ON_COMMAND(				ID_COMPUTER_READ_CONFIG_DIALOG, &CRemoteCombinedDoc::OnComputerReadConfigDialog)
ON_UPDATE_COMMAND_UI(	ID_COMPUTER_READ_CONFIG_DIALOG, &CRemoteCombinedDoc::OnUpdateOnComputerReadConfigDialog)

//ON_COMMAND(ID_COMPUTERS_LOCATION, &CRemoteCombinedDoc::OnComputersLocation)
END_MESSAGE_MAP()


// CRemoteCombinedDoc diagnostics

#ifdef _DEBUG
void CRemoteCombinedDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteCombinedDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CRemoteCombinedDoc serialization

void CRemoteCombinedDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CRemoteCombinedDoc commands

void CRemoteCombinedDoc::OnDocumentIsClosing()
{
	OnCloseDocumentMine();
}

/* is called in RemoteCombinedTaskView */
void CRemoteCombinedDoc::OnHostInfo(HOST_INFO *pHostInfo)
{
	int iNr;

	iNr = m_iSelectedComputers;

	for (int iCount = 0; iCount < iNr; iCount++)
	{
		if (&m_lListConnectionsItems.at(iCount)->m_hostInfo == pHostInfo)
		{
			// the right host update it.
			if (m_lListConnectionsItems.at(iCount)->m_iThreadRpcId == theApp.m_pMainFrame->m_iLocalHostThreadId)	// get the cpid from the localhost
			{
				if (theApp.m_sCPID != pHostInfo->host_cpid)
				{
					theApp.m_sCPID = pHostInfo->host_cpid;
					theApp.WriteProfileString(registrySectionGeneral,registryCPID, theApp.m_sCPID);
				}
			}

			m_lListConnectionsItems.at(iCount)->m_dBenchmarktime = pHostInfo->p_calculated;
		}
	}
}


void CRemoteCombinedDoc::OnAccountManagerInfo(ACCT_MGR_INFO *pAccountManagerInfo)
{
	// no need to do anyting hiere, the values are already updated. The pointer was provided!

}

void CRemoteCombinedDoc::OnHelpShowlog()
{
	theApp.m_pDlgLogging->ShowWindow(SW_SHOW);
	theApp.m_pDlgLogging->SetForegroundWindow();
}

void CRemoteCombinedDoc::OnHelpShowlogRules()
{
	theApp.m_pDlgRulesLogging->ShowWindow(SW_SHOW);
	theApp.m_pDlgRulesLogging->SetForegroundWindow();
}

void CRemoteCombinedDoc::OnShowView(int iViewNr, int iRefresh)
{
	bool	bActive;
//	CWnd	*pWnd;

	bActive = false;

	CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd*)pMainWnd->MDIGetActive();
	CView* pActiveView = pChild->GetActiveView();

	if (pActiveView == m_pProjectView) bActive = true;
	if (pActiveView == m_pTaskView) bActive = true;
	if (pActiveView == m_pTransferView) bActive = true;
	if (pActiveView == m_pMessageView) bActive = true;
	if (pActiveView == m_pHistoryView) bActive = true;
	if (pActiveView == m_pComputerView) bActive = true;
	if (pActiveView == m_pNoticesView) bActive = true;

	if (!bActive) return; // one window must have the focus !!!

	m_iRefresh = iRefresh;

	switch (iViewNr)
	{
	case BAR_SELECTED_PROJECTS:
		OnShowProjects();
	break;
	case BAR_SELECTED_TASKS:
		OnShowTasks();
	break;
	case BAR_SELECTED_TRANSFERS:
		OnShowTransfers();
	break;
	case BAR_SELECTED_MESSAGES:
		OnShowMessages();
	break;
	case BAR_SELECTED_HISTORY:
		OnShowHistory();
	break;
	case BAR_SELECTED_COMPUTERS:
		OnShowComputers();
	break;
	case BAR_SELECTED_NOTICES:
		OnShowNotices();
	break;
	}
}

void CRemoteCombinedDoc::OnShowProjects()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_PROJECTS,0);

	if (m_iViewActive == PROJECT_VIEW_ACTIVE)
	{
		if (m_iRefresh == REFRESH_VIEW_NOW)
		{
			::PostMessage(m_pProjectView->m_hWnd,UWM_MSG_REMOTE_PROJECT_SWITCH,0,m_iRefresh);
		}
		return;
	}

	SwitchToView(m_pProjectView);

	::PostMessage(m_pProjectView->m_hWnd,UWM_MSG_REMOTE_PROJECT_SWITCH,0,m_iRefresh);

   	m_iViewActive = PROJECT_VIEW_ACTIVE;

	DisplayStatus(0, NULL, true);
}

void CRemoteCombinedDoc::OnShowTasks()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_TASKS,0);
	if (m_iViewActive == TASK_VIEW_ACTIVE)
	{
		if (m_iRefresh == REFRESH_VIEW_NOW)
		{
			::PostMessage(m_pTaskView->m_hWnd,UWM_MSG_REMOTE_TASKS_SWITCH,0,m_iRefresh);
		}		
		return;
	}
	SwitchToView(m_pTaskView);

	::PostMessage(m_pTaskView->m_hWnd,UWM_MSG_REMOTE_TASKS_SWITCH,0,m_iRefresh);

   	m_iViewActive = TASK_VIEW_ACTIVE;

	DisplayStatus(0, NULL, true);
}

void CRemoteCombinedDoc::OnShowTransfers()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_TRANSFERS,0);
	if (m_iViewActive == TRANSFER_VIEW_ACTIVE)
	{
		if (m_iRefresh == REFRESH_VIEW_NOW)
		{
			::PostMessage(m_pTransferView->m_hWnd,UWM_MSG_REMOTE_TRANSFER_SWITCH,0,m_iRefresh);
		}		
		return;
	}
	SwitchToView(m_pTransferView);

	::PostMessage(m_pTransferView->m_hWnd,UWM_MSG_REMOTE_TRANSFER_SWITCH,0,m_iRefresh);

   	m_iViewActive = TRANSFER_VIEW_ACTIVE;

	DisplayStatus(0, NULL, true);
}

void CRemoteCombinedDoc::OnShowMessages()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_MESSAGES,0);
	if (m_iViewActive == MESSAGE_VIEW_ACTIVE)
	{
		if (m_iRefresh == REFRESH_VIEW_NOW)
		{
			::PostMessage(m_pMessageView->m_hWnd,UWM_MSG_REMOTE_MESSAGE_SWITCH,0,m_iRefresh);
		}		
		return;
	}
	SwitchToView(m_pMessageView);

	::PostMessage(m_pMessageView->m_hWnd,UWM_MSG_REMOTE_MESSAGE_SWITCH,0,m_iRefresh);

	m_iViewActive = MESSAGE_VIEW_ACTIVE;
	DisplayStatus(0, NULL, true);
}

void CRemoteCombinedDoc::OnShowHistory()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_HISTORY,0);
	if (m_iViewActive == HISTORY_VIEW_ACTIVE)
	{
		if (m_iRefresh == REFRESH_VIEW_NOW)
		{
			::PostMessage(m_pHistoryView->m_hWnd,UWM_MSG_REMOTE_HISTORY_SWITCH,0,m_iRefresh);
		}		
		return;
	}
	SwitchToView(m_pHistoryView);

	::PostMessage(m_pHistoryView->m_hWnd,UWM_MSG_REMOTE_HISTORY_SWITCH,0,m_iRefresh);

	m_iViewActive = HISTORY_VIEW_ACTIVE;
	DisplayStatus(0, NULL, true);
}

void CRemoteCombinedDoc::OnShowHistoryLong()
{
	this->m_pDlgHistory->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnUpdateShowHistoryLong(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(theApp.m_pDlgSettingsHistory->m_bMoveToLongHistory);

//	ToolbarEnable(pCmdUI);
}

void CRemoteCombinedDoc::OnShowNotices()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_NOTICES,0);
	if (m_iViewActive == NOTICES_VIEW_ACTIVE) return;

	SwitchToView(m_pNoticesView);

	::PostMessage(m_pNoticesView->m_hWnd,UWM_MSG_REMOTE_NOTICES_SWITCH,0,0);

	m_iViewActive = NOTICES_VIEW_ACTIVE;
	DisplayStatus(0, NULL, true);
}

void CRemoteCombinedDoc::OnShowComputers()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SET_TAB,BAR_SELECTED_COMPUTERS,0);
	if (m_iViewActive == COMPUTER_VIEW_ACTIVE)
	{
		if (m_iRefresh == REFRESH_VIEW_NOW)
		{
			::PostMessage(m_pComputerView->m_hWnd,UWM_MSG_REMOTE_COMPUTER_SWITCH,0,m_iRefresh);
		}		
		return;
	}
	SwitchToView(m_pComputerView);

	::PostMessage(m_pComputerView->m_hWnd,UWM_MSG_REMOTE_COMPUTER_SWITCH,0,m_iRefresh);

	m_iViewActive = COMPUTER_VIEW_ACTIVE;
	DisplayStatus(0, NULL, true);

}

void CRemoteCombinedDoc::OnShowStatistics()
{
	if (m_iSelectedComputers > 0) m_pDlgGraphic->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnShowStatisticsTasks()
{
	if (m_iSelectedComputers > 0) m_pDlgGraphicTasks->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnShowStatisticsDeadline()
{
	if (m_iSelectedComputers > 0) m_pDlgGraphicDeadline->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnShowTemperature()
{
	if (m_iSelectedComputers > 0) m_pDlgTemperatureGraphic->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnShowGraphicTransfer()
{
	if (m_iSelectedComputers > 0) m_pDlgGraphicTransfer->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnShowTThrottle()
{
	if (m_iSelectedComputers > 0) m_pDlgTThrottle->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnUpdateTThrottle(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_pDlgSettingsGeneral->m_bConnecTthrottle);
}

void CRemoteCombinedDoc::OnShowThreadGraph()
{
	m_pDlgThreadGraph->ShowWindow(SW_SHOW);
}

// Allow Toolbar

void CRemoteCombinedDoc::OnToolbarAllowNetworkPref()
{
	ToolbarAllowNetwork(1);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowNetworkPref(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}
void CRemoteCombinedDoc::OnToolbarAllowNetworkNever()
{
	ToolbarAllowNetwork(2);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowNetworkNever(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}

void CRemoteCombinedDoc::ToolbarAllowNetwork(int iMode)
{
	int	iNr;
	CString sComputer;
	std::deque<CString> lSelectedComputers;

	GetSelectedComputers(&lSelectedComputers);

	for (int iPos = 0; iPos < (int) lSelectedComputers.size(); iPos++)
	{
		sComputer = lSelectedComputers.at(iPos);
		iNr = GetComputerNumber(&sComputer);
		if (iNr >= 0)
		{
			m_pDlgSelectComputer->PostMessage(ID_TOOLBAR_ALLOW_NETWORK, iNr, iMode);
		}
	}
}

void CRemoteCombinedDoc::OnToolbarAllowCpuPref()
{
	ToolbarAllowCpu(1);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowCpuPref(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}
void CRemoteCombinedDoc::OnToolbarAllowCpuSnooze()
{
	ToolbarAllowCpu(2);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowCpuSnooze(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}
void CRemoteCombinedDoc::OnToolbarAllowCpuNever()
{
	ToolbarAllowCpu(3);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowCpuNever(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}

void CRemoteCombinedDoc::ToolbarAllowCpu(int iMode)
{
	int	iNr;

	std::deque<CString> lSelectedComputers;

	GetSelectedComputers(&lSelectedComputers);

	for (int iPos = 0; iPos < (int) lSelectedComputers.size(); iPos++)
	{
		iNr = GetComputerNumber(&lSelectedComputers.at(iPos));
		if (iNr >= 0)
		{
			m_pDlgSelectComputer->PostMessage(ID_TOOLBAR_ALLOW_CPU, iNr, iMode);
		}
	}
}

void CRemoteCombinedDoc::OnToolbarAllowGpuPref()
{
	ToolbarAllowGpu(1);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowGpuPref(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}
void CRemoteCombinedDoc::OnToolbarAllowGpuSnooze()
{
	ToolbarAllowGpu(2);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowGpuSnooze(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}
void CRemoteCombinedDoc::OnToolbarAllowGpuNever()
{
	ToolbarAllowGpu(3);
}
void CRemoteCombinedDoc::OnUpdateToolbarAllowGpuNever(CCmdUI* pCmdUI)
{
	ToolbarEnable(pCmdUI);
}

void CRemoteCombinedDoc::ToolbarAllowGpu(int iMode)
{
	int	iNr;

	std::deque<CString> lSelectedComputers;

	GetSelectedComputers(&lSelectedComputers);

	for (int iPos = 0; iPos < (int) lSelectedComputers.size(); iPos++)
	{
		iNr = GetComputerNumber(&lSelectedComputers.at(iPos));
		if (iNr >= 0)
		{
			m_pDlgSelectComputer->PostMessage(ID_TOOLBAR_ALLOW_GPU, iNr, iMode);
		}
	}
}

void CRemoteCombinedDoc::ToolbarEnable(CCmdUI* pCmdUI)
{
	BOOL bSelect = FALSE;

	if (GetSelectedComputers(NULL))
	{
		bSelect = TRUE;
	}

	pCmdUI->Enable(bSelect);
}

// Allow Toolbar


// Preference dialog on selected computer
void CRemoteCombinedDoc::OnExtraPreferences()
{
	if (m_iViewActive == COMPUTER_VIEW_ACTIVE) ::PostMessage(m_pComputerView->m_hWnd,UWM_MSG_PREFERENCE_DIALOG,0,0);
	if (m_iViewActive == PROJECT_VIEW_ACTIVE) ::PostMessage(m_pProjectView->m_hWnd,UWM_MSG_PREFERENCE_DIALOG,0,0);
	if (m_iViewActive == TASK_VIEW_ACTIVE) ::PostMessage(m_pTaskView->m_hWnd,UWM_MSG_PREFERENCE_DIALOG,0,0);
	if (m_iViewActive == TRANSFER_VIEW_ACTIVE) ::PostMessage(m_pTransferView->m_hWnd,UWM_MSG_PREFERENCE_DIALOG,0,0);
	if (m_iViewActive == MESSAGE_VIEW_ACTIVE) ::PostMessage(m_pMessageView->m_hWnd,UWM_MSG_PREFERENCE_DIALOG,0,0);
	if (m_iViewActive == HISTORY_VIEW_ACTIVE) ::PostMessage(m_pHistoryView->m_hWnd,UWM_MSG_PREFERENCE_DIALOG,0,0);
}

void CRemoteCombinedDoc::OnUpdateExtraPreferences(CCmdUI *pCmdUI)
{

//	BOOL bVisible = !theApp.m_pMainFrame->m_dlgBarComputer.IsVisible();
//	pCmdUI->SetCheck(!bVisible);
}

// Preference dialog on selected computer
void CRemoteCombinedDoc::OnExtraProxy()
{
	if (m_iViewActive == COMPUTER_VIEW_ACTIVE) ::PostMessage(m_pComputerView->m_hWnd,UWM_MSG_PROXY_DIALOG,0,0);
	if (m_iViewActive == PROJECT_VIEW_ACTIVE) ::PostMessage(m_pProjectView->m_hWnd,UWM_MSG_PROXY_DIALOG,0,0);
	if (m_iViewActive == TASK_VIEW_ACTIVE) ::PostMessage(m_pTaskView->m_hWnd,UWM_MSG_PROXY_DIALOG,0,0);
	if (m_iViewActive == TRANSFER_VIEW_ACTIVE) ::PostMessage(m_pTransferView->m_hWnd,UWM_MSG_PROXY_DIALOG,0,0);
	if (m_iViewActive == MESSAGE_VIEW_ACTIVE) ::PostMessage(m_pMessageView->m_hWnd,UWM_MSG_PROXY_DIALOG,0,0);
	if (m_iViewActive == HISTORY_VIEW_ACTIVE) ::PostMessage(m_pHistoryView->m_hWnd,UWM_MSG_PROXY_DIALOG,0,0);
}

void CRemoteCombinedDoc::OnUpdateExtraProxy(CCmdUI *pCmdUI)
{

//	BOOL bVisible = !theApp.m_pMainFrame->m_dlgBarComputer.IsVisible();
//	pCmdUI->SetCheck(!bVisible);
}

void CRemoteCombinedDoc::OnExtraDonetworkcommunication()
{
	int iNrRpc;
	iNrRpc =  m_iSelectedComputers;

	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		::PostThreadMessage(m_lListConnectionsItems.at(iCount)->m_iThreadRpcId, UWM_MSG_THREAD_NETWORK_AVAILABLE, 0, 0);
	}
	m_pHistoryView->PostMessage(UWM_MSG_HISTORY_SOMETHING_HAPPENED,0,0);
}

void CRemoteCombinedDoc::OnUpdateExtraDonetworkcommunication(CCmdUI *pCmdUI)
{
	CString sIDS;

	if ( theApp.m_pMainFrame->m_iHistoryStateReadyCount > 0)
	{
		sIDS.Format(" %d", theApp.m_pMainFrame->m_iHistoryStateReadyCount);
	}
	sIDS = gszTranslation[PosGroupMenuExtraUpdateAll] + sIDS;
	pCmdUI->SetText(sIDS);
}

void CRemoteCombinedDoc::OnExtraSettings()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnViewComputerBar()
{
	BOOL bVisible = !theApp.m_pMainFrame->m_dlgBarComputer.IsVisible();

	if (!bVisible)
	{
		theApp.m_pMainFrame->m_dlgBarComputer.ShowWindow(SW_HIDE);
		theApp.m_pMainFrame->ShowControlBar((CControlBar *) &theApp.m_pMainFrame->m_dlgBarComputer,FALSE,FALSE);
	}
	else
	{
		theApp.m_pMainFrame->m_dlgBarComputer.ShowWindow(SW_SHOW);
		theApp.m_pMainFrame->ShowControlBar((CControlBar *) &theApp.m_pMainFrame->m_dlgBarComputer,TRUE,FALSE);
	}

	theApp.m_pMainFrame->m_dlgBarComputer.Invalidate();

	theApp.m_pMainFrame->RecalcLayout(TRUE);
	theApp.m_pMainWnd->Invalidate(TRUE);

}

void CRemoteCombinedDoc::OnUpdateViewComputerBar(CCmdUI *pCmdUI)
{
	BOOL bVisible = !theApp.m_pMainFrame->m_dlgBarComputer.IsVisible();
	pCmdUI->SetCheck(!bVisible);
}

void CRemoteCombinedDoc::OnViewProjectBar()
{
	BOOL bVisible = !theApp.m_pMainFrame->m_dlgBarProject.IsVisible();

	if (!bVisible)
	{
		theApp.m_pMainFrame->m_dlgBarProject.ShowWindow(SW_HIDE);
		theApp.m_pMainFrame->ShowControlBar((CControlBar *) &theApp.m_pMainFrame->m_dlgBarProject,FALSE,FALSE);
	}
	else
	{
		theApp.m_pMainFrame->m_dlgBarProject.ShowWindow(SW_SHOW);
		theApp.m_pMainFrame->ShowControlBar((CControlBar *) &theApp.m_pMainFrame->m_dlgBarProject,TRUE,FALSE);
	}

	theApp.m_pMainFrame->m_dlgBarProject.Invalidate();

	theApp.m_pMainFrame->RecalcLayout(TRUE);
	theApp.m_pMainWnd->Invalidate(TRUE);

}

void CRemoteCombinedDoc::OnUpdateViewProjectBar(CCmdUI *pCmdUI)
{
	BOOL bVisible = !theApp.m_pMainFrame->m_dlgBarProject.IsVisible();
	pCmdUI->SetCheck(!bVisible);
}

void CRemoteCombinedDoc::OnColorsDialog()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnSettinsApplyChangesTasks()
{
	m_pTaskView->SendMessage(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_TASKS,0,0);
}

void CRemoteCombinedDoc::OnSettinsApplyChangesProjects()
{
	m_pProjectView->SendMessage(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_PROJECTS,0,0);
}

void CRemoteCombinedDoc::OnSettinsApplyChangesHistory()
{
	m_pHistoryView->SendMessage(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_HISTORY,0,0);
}

void CRemoteCombinedDoc::OnComputerEditConfig()
{
	int	iThreadId;
	bool bFound = false;

	if (m_iSelectedComputers > 0)
	{
		if (m_pDlgEditConfig == NULL)
		{
			m_pDlgEditConfig = new CDlgEditConfig;
			m_pDlgEditConfig->Create(IDD_DIALOG_EDIT_CONFIG);
			m_pDlgEditConfig->InitialUpdate(this, "config_editor.xml");
//			m_pDlgEditConfig->OnInitDialog();
		}
		std::deque<CString> lSelectedComputers;
		GetSelectedComputers(&lSelectedComputers);

		if (lSelectedComputers.size() > 0)
		{
			bFound = GetRpcThreadId(&lSelectedComputers.at(0), &iThreadId);
		}
		if (!bFound)
		{
			iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
			if (iThreadId > 0) bFound = true;
		}

		if (bFound)
		{
			// found computer, now tell the Dialog
			m_pDlgEditConfig->PostMessage(UWM_MSG_EDIT_CONFIG_OPEN_DIALOG, iThreadId, 0);
		}
	}
}

void CRemoteCombinedDoc::OnUpdateComputerEditConfig(CCmdUI *pCmdUI)
{
	if (m_iSelectedComputers > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CRemoteCombinedDoc::OnComputerEditAppInfo()
{
	int	iThreadId;
	bool bFound = false;

	if (m_iSelectedComputers > 0)
	{
		if (m_iViewActive != PROJECT_VIEW_ACTIVE) return;
	
		CSelection	*pSelection;
		pSelection = m_pProjectView->m_pSelection;
		int nItems = pSelection->SelectionNumber();
		if (nItems <= 0) return;

		CString sProject = pSelection->GetLastSelected();
		if (sProject.GetLength() <= 0) return;

		if (m_pDlgEditAppInfo == NULL)
		{
			m_pDlgEditAppInfo = new CDlgEditAppInfoConfig;
			m_pDlgEditAppInfo->Create(IDD_DIALOG_EDIT_CONFIG);
			m_pDlgEditAppInfo->InitialUpdate(this, "app_config_editor.xml");
		}
		std::deque<CString> lSelectedComputers;
		GetSelectedComputers(&lSelectedComputers);

		if (lSelectedComputers.size() > 0)
		{
			bFound = GetRpcThreadId(&lSelectedComputers.at(0), &iThreadId);
		}
		if (!bFound)
		{
			iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
			if (iThreadId > 0) bFound = true;
		}

		if (bFound)
		{
			// found computer, now tell the Dialog
			m_pDlgEditAppInfo->SendMessage(UWM_MSG_EDIT_APPP_INFO_OPEN_DIALOG, iThreadId, (LPARAM) &sProject);
		}
	}
}

void CRemoteCombinedDoc::OnUpdateComputerEditAppInfo(CCmdUI *pCmdUI)
{
	if (m_iSelectedComputers > 0)
	{
		if (m_iViewActive == PROJECT_VIEW_ACTIVE)
		{
			CSelection	*pSelection;
			pSelection = m_pProjectView->m_pSelection;
			if (pSelection->SelectionNumber() > 0)
			{
				pCmdUI->Enable(TRUE);
				return;
			}
		}
	}
	pCmdUI->Enable(FALSE);
}


void CRemoteCombinedDoc::OnComputerNetDialog()
{
	m_pDlgSelectComputer->PostMessage(ID_COMPUTER_NET_DIALOG_OPEN, 0, -1);
}

void CRemoteCombinedDoc::OnUpdateComputerNetDialog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CRemoteCombinedDoc::OnComputerAllowDialog()
{
	m_pDlgSelectComputer->PostMessage(ID_COMPUTER_ALLOW_DIALOG_OPEN, 0, -1);
}

void CRemoteCombinedDoc::OnUpdateComputerAllowDialog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CRemoteCombinedDoc::OnComputerAllowGpuDialog()
{
	m_pDlgSelectComputer->PostMessage(ID_COMPUTER_ALLOW_GPU_DIALOG_OPEN, 0, -1);
}

void CRemoteCombinedDoc::OnUpdateComputerAllowGpuDialog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CRemoteCombinedDoc::OnComputerBenchmarkDialog()
{
	m_pDlgSelectComputer->PostMessage(ID_COMPUTER_BENCHMARK_DIALOG_OPEN, 0, -1);
}

void CRemoteCombinedDoc::OnUpdateOnComputerBenchmarkDialog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CRemoteCombinedDoc::OnComputerSynchronizeDialog()
{
	m_pDlgSelectComputer->PostMessage(ID_PROJECTS_SYNCHRONIZE_DIALOG_OPEN, 0, -1);
}

void CRemoteCombinedDoc::OnUpdateOnComputerSynchronizeDialog(CCmdUI *pCmdUI)
{
	int		iConnectionStatus;
	CString sTxt, sManager;
	BOOL	bFound;

	bFound = FALSE;

	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		iConnectionStatus = m_lListConnectionsItems.at(iCount)->m_iConnectionStatus;

		if (iConnectionStatus == RPC_STATUS_CONNECTED)
		{
			if (m_lListConnectionsItems.at(iCount)->m_accountManagerInfo.acct_mgr_name.size())
			{
				bFound = TRUE;
				break;
			}
		}
	}

	pCmdUI->Enable(bFound);
}

void CRemoteCombinedDoc::OnComputerReadConfigDialog()
{
	m_pDlgSelectComputer->PostMessage(ID_COMPUTER_READ_CONFIG_DIALOG_OPEN, 0, -1);
}

void CRemoteCombinedDoc::OnUpdateOnComputerReadConfigDialog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CRemoteCombinedDoc::OnHelpUpdate()
{
	theApp.m_pDlgUpdate->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnAddNewProject()
{
	OnShowProjects();		// activate the project view.

	this->m_pProjectView->PostMessage(UWM_MSG_ADD_NEW_PROJECT,0,0);
}

void CRemoteCombinedDoc::OnAddNewProjectManager()
{
	OnShowProjects();		// activate the project view.

	this->m_pProjectView->PostMessage(UWM_MSG_ADD_NEW_ACCOUNT_MANAGER,0,0);
}

void CRemoteCombinedDoc::OnUpdateAddNewProjectManager(CCmdUI *pCmdUI)
{
	int		iConnectionStatus;
	CString sTxt, sManager;
	BOOL	bFound;

	bFound = FALSE;

	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		iConnectionStatus = m_lListConnectionsItems.at(iCount)->m_iConnectionStatus;

		if (iConnectionStatus == RPC_STATUS_CONNECTED)
		{
			if (m_lListConnectionsItems.at(iCount)->m_accountManagerInfo.acct_mgr_name.size())
			{
				sManager = m_lListConnectionsItems.at(iCount)->m_accountManagerInfo.acct_mgr_name.c_str();
				bFound = TRUE;
				break;
			}
		}
	}

	sTxt = gszTranslation[PosGroupMenuProjectsAddAccountM];

	if (sManager.GetLength() > 0)
	{
		sTxt+= " - ";
		sTxt+= sManager; 
	}
	pCmdUI->SetText(sTxt);
}



/*
void CRemoteCombinedDoc::OnSynchronizeProjectManager()
{
	OnShowProjects();		// activate the project view.

	this->m_pProjectView->PostMessage(UWM_MSG_ADD_NEW_ACCOUNT_MANAGER,0,0);
}
*/

void CRemoteCombinedDoc::OnComputersLocation()
{

	theApp.m_pDlgComputerLocation->SendMessage(UWM_MSG_DLG_COMPUTER_LOCATION_SETUP,0, 0);
//	theApp.m_pDlgComputerLocation->ShowWindow(SW_SHOW);
	// TODO: Add your command handler code here
}

void CRemoteCombinedDoc::OnRestartGraphics()
{
	int iCheck;

	if (m_pDlgGraphic != NULL)
	{
		m_pDlgGraphic->SafeWindowPlacement();
		delete m_pDlgGraphic;
	}

	m_pDlgGraphic = new CDlgGraphic;
	if (m_iSelectedComputers > 10)	iCheck = theApp.GetProfileIntA(registrySectionGraphic, registryExpanded, 1);
	else iCheck = theApp.GetProfileIntA(registrySectionGraphic, registryExpanded, 0);
	m_pDlgGraphic->m_iExpand = iCheck;
	if (iCheck)	m_pDlgGraphic->Create(IDD_DIALOG_GRAPHIC_LARGE);
	else m_pDlgGraphic->Create(IDD_DIALOG_GRAPHIC);

	m_pDlgGraphic->InitialUpdate(this, true);

	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		::PostMessage(m_pDlgGraphic->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
	}

	if (theApp.m_pDlgColorGraphic != NULL) theApp.m_pDlgColorGraphic->PostMessage(UWM_GRAPHIC_COLOR_INIT, (WPARAM) m_pDlgGraphic->m_hWnd, (LPARAM) m_pDlgGraphicTasks->m_hWnd);	// tell the color dialog who we are.

//	m_pDlgGraphic->ShowWindow(SW_SHOW);
}

void CRemoteCombinedDoc::OnRestartGraphicsTasks()
{
	if (m_pDlgGraphicTasks != NULL)
	{
		m_pDlgGraphicTasks->SafeWindowPlacement();
		m_pDlgGraphicTasks->PostMessage(UWM_CLOSE_GRAPHIC,0,0);
	}
	return;
}

void CRemoteCombinedDoc::OnRestartGraphicsTasksReady()
{
	int iCheck;

	delete m_pDlgGraphicTasks;
	m_pDlgGraphicTasks = new CDlgGraphicTasks;
	if (m_iSelectedComputers > 10)	iCheck = theApp.GetProfileIntA(registrySectionGraphicTasks, registryExpanded, 1);
	else iCheck = theApp.GetProfileIntA(registrySectionGraphicTasks, registryExpanded, 0);
	m_pDlgGraphicTasks->m_iExpand = iCheck;
	if (iCheck)	m_pDlgGraphicTasks->Create(IDD_DIALOG_GRAPHICWU_LARGE);
	else m_pDlgGraphicTasks->Create(IDD_DIALOG_GRAPHICWU);

	m_pDlgGraphicTasks->InitialUpdate(this, true);

	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		::PostMessage(m_pDlgGraphicTasks->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
	}

	if (theApp.m_pDlgColorGraphic != NULL) theApp.m_pDlgColorGraphic->PostMessage(UWM_GRAPHIC_COLOR_INIT, (WPARAM) m_pDlgGraphic->m_hWnd, (LPARAM) m_pDlgGraphicTasks->m_hWnd);	// tell the color dialog who we are.

//	m_pDlgGraphicTasks->ShowWindow(SW_SHOW);
}


void CRemoteCombinedDoc::OnRestartGraphicsDeadline()
{
	if (m_pDlgGraphicDeadline != NULL)
	{
		m_pDlgGraphicDeadline->SafeWindowPlacement();
		m_pDlgGraphicDeadline->PostMessage(UWM_CLOSE_GRAPHIC,0,0);
	}
	return;
}

void CRemoteCombinedDoc::OnRestartGraphicsDeadlineReady()
{
	int iCheck;

	delete m_pDlgGraphicDeadline;
	m_pDlgGraphicDeadline = new CDlgGraphicDeadline;
	if (m_iSelectedComputers > 10)	iCheck = theApp.GetProfileIntA(registrySectionGraphicDeadline, registryExpanded, 1);
	else iCheck = theApp.GetProfileIntA(registrySectionGraphicDeadline, registryExpanded, 0);
	m_pDlgGraphicDeadline->m_iExpand = iCheck;
	if (iCheck)	m_pDlgGraphicDeadline->Create(IDD_DIALOG_GRAPHIC_DEADLINE_LARGE);
	else m_pDlgGraphicDeadline->Create(IDD_DIALOG_GRAPHIC_DEADLINE);

	m_pDlgGraphicDeadline->InitialUpdate(this, true);

	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		::PostMessage(m_pDlgGraphicDeadline->m_hWnd,UWM_INITIAL_GRAPHIC,(WPARAM) m_lListConnectionsItems.at(iCount)->m_pcThreadRpcId,(LPARAM) m_lListConnectionsItems.at(iCount)->m_pThreadRpc);
	}

	if (theApp.m_pDlgColorGraphic != NULL) theApp.m_pDlgColorGraphic->PostMessage(UWM_GRAPHIC_COLOR_INIT2, (WPARAM) m_pDlgGraphicDeadline->m_hWnd, 0);	// tell the color dialog who we are.

//	m_pDlgGraphicTasks->ShowWindow(SW_SHOW);
}

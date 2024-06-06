// DlgDebug.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgDebug.h"
#include "afxdialogex.h"
#include "DlgLogging.h"


// CDlgDebug dialog

IMPLEMENT_DYNAMIC(CDlgDebug, CDialogEx)

CDlgDebug::CDlgDebug(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDebug::IDD, pParent)
{

}

CDlgDebug::~CDlgDebug()
{
}

void CDlgDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DEBUG_NOTICES, m_checkNotices);
	DDX_Control(pDX, IDC_CHECK_DEBUG_NOTICES_TIMER, m_checkNoticesTimer);
	DDX_Control(pDX, IDC_CHECK_DEBUG_CONNECTING, m_checkConnecting);
	DDX_Control(pDX, IDC_CHECK_DEBUG_CLOSING, m_checkClosing);
	DDX_Control(pDX, IDC_CHECK_DEBUG_COMPUTER_SEARCH, m_checkSearchComputers);
	DDX_Control(pDX, IDC_CHECK_DEBUG_TEMP_GPU, m_checkTemperatureGpu);
	DDX_Control(pDX, IDC_CHECK_DEBUG_TTHROTTLE, m_checkTThrottle);
	DDX_Control(pDX, IDC_CHECK_DEBUG_TTHROTTLE_DATA, m_checkTThrottleData);
	DDX_Control(pDX, IDC_CHECK_DEBUG_TTHROTTLE_READ, m_checkTThrottleRead);
	DDX_Control(pDX, IDC_CHECK_DEBUG_TTHROTTLE_GRAPH, m_checkTThrottleGraph);
	DDX_Control(pDX, IDC_CHECK_DEBUG_SNOOZE, m_checkSnooze);
	DDX_Control(pDX, IDC_CHECK_DEBUG_HISTORY_FETCH, m_checkHistoryFetching);
	DDX_Control(pDX, IDC_CHECK_DEBUG_TASKS, m_checkTasks);
	DDX_Control(pDX, IDC_CHECK_DEBUG_LOCAL_DIRECTORY, m_checkDirectoryLocal);
	DDX_Control(pDX, IDC_CHECK_DEBUG_DIRECTORY, m_checkDirectory);
	DDX_Control(pDX, IDC_CHECK_DEBUG_STATE, m_checkState);
	DDX_Control(pDX, IDC_CHECK_DEBUG_MOBILE, m_checkMobile);
	DDX_Control(pDX, IDC_CHECK_DEBUG_MOBILE_SCRIPT, m_checkMobileScript);
	DDX_Control(pDX, IDC_CHECK_DEBUG_MOBILE_CONNECT, m_checkMobileConnect);
	DDX_Control(pDX, IDC_CHECK_DEBUG_MOBILE_CONNECT_DUMP, m_checkMobileConnectDump);
	DDX_Control(pDX, IDC_CHECK_DEBUG_CLOUD, m_checkCloud);
	DDX_Control(pDX, IDC_CHECK_DEBUG_HEAP, m_checkHeap);
	DDX_Control(pDX, IDC_CHECK_DEBUG_FILE_ONLY, m_checkFileOnly);
	DDX_Control(pDX, IDC_CHECK_DEBUG_TASKS_SHOW, m_checkTasksShow);
}


BEGIN_MESSAGE_MAP(CDlgDebug, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_NOTICES, &CDlgDebug::OnBnClickedCheckDebugNotices)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_NOTICES_TIMER, &CDlgDebug::OnBnClickedCheckDebugNoticesTimer)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_CONNECTING, &CDlgDebug::OnBnClickedCheckDebugConnecting)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_COMPUTER_SEARCH, &CDlgDebug::OnBnClickedCheckDebugComputerSearch)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_CLOSING, &CDlgDebug::OnBnClickedCheckDebugClosing)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_TEMP_GPU, &CDlgDebug::OnBnClickedCheckDebugTempGpu)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_TTHROTTLE, &CDlgDebug::OnBnClickedCheckDebugTthrottle)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_TTHROTTLE_DATA, &CDlgDebug::OnBnClickedCheckDebugTthrottleData)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_TTHROTTLE_READ, &CDlgDebug::OnBnClickedCheckDebugTthrottleRead)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_TTHROTTLE_GRAPH, &CDlgDebug::OnBnClickedCheckDebugTthrottleGraph)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_SNOOZE, &CDlgDebug::OnBnClickedCheckDebugSnooze)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_HISTORY_FETCH, &CDlgDebug::OnBnClickedCheckDebugHistoryFetch)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_TASKS, &CDlgDebug::OnBnClickedCheckDebugTasks)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_LOCAL_DIRECTORY, &CDlgDebug::OnBnClickedCheckDebugLocalDirectory)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_DIRECTORY, &CDlgDebug::OnBnClickedCheckDebugDirectory)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_STATE, &CDlgDebug::OnBnClickedCheckDebugState)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_MOBILE, &CDlgDebug::OnBnClickedCheckDebugMobile)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_MOBILE_SCRIPT, &CDlgDebug::OnBnClickedCheckDebugMobileScript)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_MOBILE_CONNECT, &CDlgDebug::OnBnClickedCheckDebugMobileConnect)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_MOBILE_CONNECT_DUMP, &CDlgDebug::OnBnClickedCheckDebugMobileConnectDump)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_CLOUD, &CDlgDebug::OnBnClickedCheckDebugCloud)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_HEAP, &CDlgDebug::OnBnClickedCheckDebugHeap)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_FILE_ONLY, &CDlgDebug::OnBnClickedCheckDebugFileOnly)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_TASKS_SHOW, &CDlgDebug::OnBnClickedCheckDebugTasksShow)
END_MESSAGE_MAP()


// CDlgDebug message handlers


BOOL CDlgDebug::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pLogging->m_bLogDebugNotices = theApp.GetProfileInt(registrySectionDebugLog, registryDebugNotices, 0) != 0;
	if (m_pLogging->m_bLogDebugNotices)	m_checkNotices.SetCheck(1);

	m_pLogging->m_bLogDebugNoticesTimer = theApp.GetProfileInt(registrySectionDebugLog, registryDebugNoticesTimer, 0) != 0;
	if (m_pLogging->m_bLogDebugNoticesTimer)	m_checkNoticesTimer.SetCheck(1);

	m_pLogging->m_bLogDebugConnecting = theApp.GetProfileInt(registrySectionDebugLog, registryDebugConnection, 0) != 0;
	if (m_pLogging->m_bLogDebugConnecting)	m_checkConnecting.SetCheck(1);

	m_pLogging->m_bLogDebugClosing = theApp.GetProfileInt(registrySectionDebugLog, registryDebugClosing, 0) != 0;
	if (m_pLogging->m_bLogDebugClosing)	m_checkClosing.SetCheck(1);

	m_pLogging->m_bLogDebugSearchComputers = theApp.GetProfileInt(registrySectionDebugLog, registryDebugSearchComputers, 0) != 0;
	if (m_pLogging->m_bLogDebugSearchComputers)	m_checkSearchComputers.SetCheck(1);

	m_pLogging->m_bLogDebugTemperatureGpu = theApp.GetProfileInt(registrySectionDebugLog, registryDebugTemperatureGpu, 0) != 0;
	if (m_pLogging->m_bLogDebugTemperatureGpu)	m_checkTemperatureGpu.SetCheck(1);

	m_pLogging->m_bLogDebugTThrottle = theApp.GetProfileInt(registrySectionDebugLog, registryDebugTThrottle, 0) != 0;
	if (m_pLogging->m_bLogDebugTThrottle)	m_checkTThrottle.SetCheck(1);

	m_pLogging->m_bLogDebugTThrottleData = theApp.GetProfileInt(registrySectionDebugLog, registryDebugTThrottleData, 0) != 0;
	if (m_pLogging->m_bLogDebugTThrottleData)	m_checkTThrottleData.SetCheck(1);

	m_pLogging->m_bLogDebugTThrottleRead = theApp.GetProfileInt(registrySectionDebugLog, registryDebugTThrottleRead, 0) != 0;
	if (m_pLogging->m_bLogDebugTThrottleRead)	m_checkTThrottleRead.SetCheck(1);

	m_pLogging->m_bLogDebugTThrottleGraphic = theApp.GetProfileInt(registrySectionDebugLog, registryDebugTThrottleGraph, 0) != 0;
	if (m_pLogging->m_bLogDebugTThrottleGraphic)	m_checkTThrottleGraph.SetCheck(1);

	m_pLogging->m_bLogDebugSnooze = theApp.GetProfileInt(registrySectionDebugLog, registryDebugSnooze, 0) != 0;
	if (m_pLogging->m_bLogDebugSnooze)	m_checkSnooze.SetCheck(1);

	m_pLogging->m_bLogDebugHistoryFetching = theApp.GetProfileInt(registrySectionDebugLog, registryDebugHistoryFetching, 0) != 0;
	if (m_pLogging->m_bLogDebugHistoryFetching)	m_checkHistoryFetching.SetCheck(1);

	m_pLogging->m_bLogDebugTasks = theApp.GetProfileInt(registrySectionDebugLog, registryDebugTasks, 0) != 0;
	if (m_pLogging->m_bLogDebugTasks)	m_checkTasks.SetCheck(1);

	m_pLogging->m_bLogDebugTasksShow = theApp.GetProfileInt(registrySectionDebugLog, registryDebugTasksShow, 0) != 0;
	if (m_pLogging->m_bLogDebugTasksShow)	m_checkTasksShow.SetCheck(1);

	m_pLogging->m_bLogDebugLocalDirectory = theApp.GetProfileInt(registrySectionDebugLog, registryDebugLocalDirectory, 0) != 0;
	if (m_pLogging->m_bLogDebugLocalDirectory)	m_checkDirectoryLocal.SetCheck(1);

	m_pLogging->m_bLogDebugDirectory = theApp.GetProfileInt(registrySectionDebugLog, registryDebugDirectory, 0) != 0;
	if (m_pLogging->m_bLogDebugDirectory)	m_checkDirectory.SetCheck(1);

	m_pLogging->m_bLogDebugState = theApp.GetProfileInt(registrySectionDebugLog, registryDebugState, 0) != 0;
	if (m_pLogging->m_bLogDebugState)	m_checkState.SetCheck(1);

	m_pLogging->m_bLogDebugMobile = theApp.GetProfileInt(registrySectionDebugLog, registryDebugMobile, 0) != 0;
	if (m_pLogging->m_bLogDebugMobile)	 m_checkMobile.SetCheck(1);

	m_pLogging->m_bLogDebugMobileScript = theApp.GetProfileInt(registrySectionDebugLog, registryDebugMobileScript, 0) != 0;
	if (m_pLogging->m_bLogDebugMobileScript)	m_checkMobileScript.SetCheck(1);

	m_pLogging->m_bLogDebugMobileConnect = theApp.GetProfileInt(registrySectionDebugLog, registryDebugMobileConnect, 0) != 0;
	if (m_pLogging->m_bLogDebugMobileConnect)	m_checkMobileConnect.SetCheck(1);

	m_pLogging->m_bLogDebugMobileConnectDump = theApp.GetProfileInt(registrySectionDebugLog, registryDebugMobileConnectDump, 0) != 0;
	if (m_pLogging->m_bLogDebugMobileConnectDump)	m_checkMobileConnectDump.SetCheck(1);

	m_pLogging->m_bLogDebugCloud = theApp.GetProfileInt(registrySectionDebugLog, registryDebugCloud, 0) != 0;
	if (m_pLogging->m_bLogDebugCloud)	 m_checkCloud.SetCheck(1);

	m_pLogging->m_bLogDebugHeapCheck = theApp.GetProfileInt(registrySectionDebugLog, registryDebugHeapCheck, 0) != 0;
	if (m_pLogging->m_bLogDebugHeapCheck)	m_checkHeap.SetCheck(1);

	m_pLogging->m_bLogDebugFileOnly = theApp.GetProfileInt(registrySectionDebugLog, registryDebugFileOnly, 0) != 0;
	if (m_pLogging->m_bLogDebugFileOnly)	m_checkFileOnly.SetCheck(1);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgDebug::OnBnClickedCheckDebugNotices()
{
	m_pLogging->m_bLogDebugNotices = m_checkNotices.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugNotices, m_pLogging->m_bLogDebugNotices);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugNoticesTimer()
{
	m_pLogging->m_bLogDebugNoticesTimer = m_checkNoticesTimer.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugNoticesTimer, m_pLogging->m_bLogDebugNoticesTimer);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugConnecting()
{
	m_pLogging->m_bLogDebugConnecting = m_checkConnecting.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugConnection, m_pLogging->m_bLogDebugConnecting);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugComputerSearch()
{
	m_pLogging->m_bLogDebugSearchComputers = m_checkSearchComputers.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugSearchComputers, m_pLogging->m_bLogDebugSearchComputers);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugClosing()
{
	m_pLogging->m_bLogDebugClosing = m_checkClosing.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugClosing, m_pLogging->m_bLogDebugClosing);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugTempGpu()
{
	m_pLogging->m_bLogDebugTemperatureGpu = m_checkTemperatureGpu.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugTemperatureGpu, m_pLogging->m_bLogDebugTemperatureGpu);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugTthrottle()
{
	m_pLogging->m_bLogDebugTThrottle = m_checkTThrottle.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugTThrottle, m_pLogging->m_bLogDebugTThrottle);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugTthrottleData()
{
	m_pLogging->m_bLogDebugTThrottleData = m_checkTThrottleData.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugTThrottleData, m_pLogging->m_bLogDebugTThrottleData);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugTthrottleRead()
{
	m_pLogging->m_bLogDebugTThrottleRead = m_checkTThrottleRead.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugTThrottleRead, m_pLogging->m_bLogDebugTThrottleRead);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugTthrottleGraph()
{
	m_pLogging->m_bLogDebugTThrottleGraphic = m_checkTThrottleGraph.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugTThrottleGraph, m_pLogging->m_bLogDebugTThrottleGraphic);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugSnooze()
{
	m_pLogging->m_bLogDebugSnooze = m_checkSnooze.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugSnooze, m_pLogging->m_bLogDebugSnooze);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugHistoryFetch()
{
	m_pLogging->m_bLogDebugHistoryFetching = m_checkHistoryFetching.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugHistoryFetching, m_pLogging->m_bLogDebugHistoryFetching);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugTasks()
{
	m_pLogging->m_bLogDebugTasks = m_checkTasks.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugTasks, m_pLogging->m_bLogDebugTasks);
	m_pLogging->AddLoggingInfo();
}

void CDlgDebug::OnBnClickedCheckDebugTasksShow()
{
	m_pLogging->m_bLogDebugTasksShow = m_checkTasksShow.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugTasksShow, m_pLogging->m_bLogDebugTasksShow);
	m_pLogging->AddLoggingInfo();
}

void CDlgDebug::OnBnClickedCheckDebugLocalDirectory()
{
	m_pLogging->m_bLogDebugLocalDirectory = m_checkDirectoryLocal.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugLocalDirectory, m_pLogging->m_bLogDebugLocalDirectory);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugDirectory()
{
	m_pLogging->m_bLogDebugDirectory = m_checkDirectory.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugDirectory, m_pLogging->m_bLogDebugDirectory);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugState()
{
	m_pLogging->m_bLogDebugState = m_checkState.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugState, m_pLogging->m_bLogDebugState);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugMobile()
{
	m_pLogging->m_bLogDebugMobile = m_checkMobile.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugMobile, m_pLogging->m_bLogDebugMobile);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugMobileScript()
{
	m_pLogging->m_bLogDebugMobileScript = m_checkMobileScript.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugMobileScript, m_pLogging->m_bLogDebugMobileScript);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugMobileConnect()
{
	m_pLogging->m_bLogDebugMobileConnect = m_checkMobileConnect.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugMobileConnect, m_pLogging->m_bLogDebugMobileConnect);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugMobileConnectDump()
{
	m_pLogging->m_bLogDebugMobileConnectDump = m_checkMobileConnectDump.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugMobileConnectDump, m_pLogging->m_bLogDebugMobileConnectDump);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugCloud()
{
	m_pLogging->m_bLogDebugCloud = m_checkCloud.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugCloud, m_pLogging->m_bLogDebugCloud);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugHeap()
{
	m_pLogging->m_bLogDebugHeapCheck = m_checkHeap.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugHeapCheck, m_pLogging->m_bLogDebugHeapCheck);
	m_pLogging->AddLoggingInfo();
}


void CDlgDebug::OnBnClickedCheckDebugFileOnly()
{
	m_pLogging->m_bLogDebugFileOnly = m_checkFileOnly.GetCheck() != 0;
	theApp.WriteProfileInt(registrySectionDebugLog, registryDebugFileOnly, m_pLogging->m_bLogDebugFileOnly);
	m_pLogging->AddLoggingInfo();
}

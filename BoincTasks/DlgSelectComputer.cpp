// DlgSelectComputer.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "MainFrm.h"
#include "DlgSelectComputer.h"
#include "RemoteCombinedDoc.h"
#include "ThreadRpc.h"
#include "TimeString.h"
#include "DateConvert.h"
#include "Translation.h"
#include "afxdialogex.h"

static int _gnSearchComputersFmt[NUM_SELECT_COMPUTERS_COLUMNS] =
{
	LVCFMT_LEFT, LVCFMT_LEFT
};

// password is 0 = hidden
static int _gnSearchComputersWidth[NUM_SELECT_COMPUTERS_COLUMNS] =
{
	200, 240
};


// CDlgSelectComputer dialog

IMPLEMENT_DYNAMIC(CDlgSelectComputer, CDialogEx)

CDlgSelectComputer::CDlgSelectComputer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSelectComputer::IDD, pParent)
{
	m_pList = NULL;
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iType = -1;

}

CDlgSelectComputer::~CDlgSelectComputer()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
}

//	::PostThreadMessage(m_lListConnectionsItems.at(iNr)->m_iThreadRpcId,UWM_MSG_THREAD_SET_NETWORK_MODE,(WPARAM) m_pTaskView->m_hWnd, iNetworkMode);

void CDlgSelectComputer::InitialUpdate(std::deque<CRpcConnectionItem*> *pm_lListConnectionsItems)
{
	m_pList = pm_lListConnectionsItems;
	AddComputers();
//	UpdateStatusAll();
}

void CDlgSelectComputer::AddColumns()
{
	LV_COLUMN	lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for (int iCount = 0; iCount < NUM_SELECT_COMPUTERS_COLUMNS; iCount++)
	{
		lvc.iSubItem = iCount;
		switch (iCount)
		{
		case SELECT_COLUMN_COMPUTER:
			lvc.pszText = gszTranslation[PosViewProjecsRowComputer];
			break;
		case SELECT_COLUMN_STATUS:
			lvc.pszText = "";
			break;
		}
		lvc.fmt = _gnSearchComputersFmt[iCount];
		lvc.cx = _gnSearchComputersWidth[iCount];
		m_lComputer.InsertColumn(iCount, &lvc);
	}
}

void CDlgSelectComputer::AddComputers()
{
	CRpcConnectionItem *pItem;
	if (m_pList == NULL) return;

	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		pItem = m_pList->at(iCount);
		pItem->m_iConnectionStatus = -1;
		if (pItem->m_pcThreadRpcId == NULL)
		{
			continue;
		}
		m_lComputer.InsertItem(SELECT_COLUMN_COMPUTER, "");
	}
	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		pItem = m_pList->at(iCount);
		if (pItem->m_pcThreadRpcId == NULL)
		{
			continue;
		}
		m_lComputer.SetItemText(iCount, SELECT_COLUMN_COMPUTER, pItem->m_pcThreadRpcId);
	}
}

void CDlgSelectComputer::UpdateStatusAll()
{
	CRpcConnectionItem *pItem;
	if (m_pList == NULL) return;

	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		pItem = m_pList->at(iCount);
		if (pItem->m_pcThreadRpcId == NULL)
		{
			continue;
		}
		switch (m_iType)
		{
		case SELECT_COMPUTER_NETWORK:
			UpdateStatusNetworkItem(iCount);
			break;
		case SELECT_COMPUTER_ALLOW:
			UpdateStatusAllowItem(iCount, pItem, false);
			break;
		case SELECT_COMPUTER_ALLOW_GPU:
			UpdateStatusAllowItem(iCount, pItem, true);
			break;
		case SELECT_COMPUTER_READCONFIG:
			UpdateStatusConnected(iCount, pItem);
			break;
		case SELECT_COMPUTER_BENCHMARK:
			UpdateStatusBenchmark(iCount, pItem);
		case SELECT_COMPUTER_SYNCHRONIZE:
			UpdateStatusSynchronize(iCount, pItem, -1);
		}
		
	}
}

void CDlgSelectComputer::UpdateStatusNetworkItem(int iPos)
{
	CRpcConnectionItem *pItem;

	if (m_pList == NULL) return;
	pItem = m_pList->at(iPos);
	if (pItem->m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		int iMode = pItem->m_CcStatus.network_mode;
		switch (iMode)
		{
		case RUN_MODE_ALWAYS:
			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosGroupMenuCommonAlways]);
			break;
		case RUN_MODE_AUTO:
			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosGroupMenuCommonAuto]);
			break;
		case RUN_MODE_NEVER:
			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosGroupMenuCommonNever]);
			break;
		}
	}
	else
	{
		m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosWindowStatusMessageCombinedNotConnected]);
	}
}

void CDlgSelectComputer::UpdateStatusAllowItem(int iPos, CRpcConnectionItem *pItem, bool bGpu)
{
	char	cBuffer[64];
	int		iTaskMode, iConnectionStatus;
	double	dTaskModeTime;
	CString sTxt;

	if (pItem->m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{

		iConnectionStatus = pItem->m_iConnectionStatus;
		if (bGpu)
		{
			iTaskMode = pItem->m_CcStatus.gpu_mode;
			dTaskModeTime = pItem->m_CcStatus.gpu_mode_delay;
		}
		else
		{
			iTaskMode = pItem->m_CcStatus.task_mode;
			dTaskModeTime = pItem->m_CcStatus.task_mode_delay;
		}

		switch (iTaskMode)
		{
		case RUN_MODE_ALWAYS:
			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosGroupMenuCommonAlways]);
			break;
		case RUN_MODE_AUTO:
			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosGroupMenuCommonAuto]);
			break;
		case RUN_MODE_NEVER:
			if (dTaskModeTime != 0)
			{
				CTimeString timeString;
				timeString.TimeString((int)dTaskModeTime, cBuffer, 63, "", "");

				sTxt += gszTranslation[PosGroupMenuCommonSnooze];
				sTxt += " ";
				sTxt += cBuffer;
			}
			else
			{
				sTxt = gszTranslation[PosGroupMenuCommonNever];
			}

			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, sTxt);
			break;
		}
	}
	else
	{
		m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosWindowStatusMessageCombinedNotConnected]);
	}
}

void CDlgSelectComputer::UpdateStatusConnected(int iPos, CRpcConnectionItem *pItem)
{
	if (pItem->m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosWindowStatusMessageCombinedConnected]);
	}
	else
	{
		m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosWindowStatusMessageCombinedNotConnected]);
	}
}

void CDlgSelectComputer::UpdateStatusBenchmark(int iPos, CRpcConnectionItem *pItem)
{
	time_t tLast;
	CString sTxt;
	char	cBuffer[50];

	if (pItem->m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		tLast = (time_t)pItem->m_dBenchmarktime;

		if (tLast != 0)
		{
			CDateConvert dateConvert;
			dateConvert.Convert(tLast, &cBuffer[0]);

			sTxt += gszTranslation[PosGroupMenuExtraBenchmarkLast];
			sTxt += " ";
			sTxt += cBuffer;
			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, sTxt);
		}
	}
	else
	{
		m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosWindowStatusMessageCombinedNotConnected]);
	}
}

void CDlgSelectComputer::UpdateStatusSynchronize(int iPos, CRpcConnectionItem *pItem, int iFound)
{
	CString sTxt;
	if (pItem->m_iConnectionStatus == RPC_STATUS_CONNECTED)
	{
		if (pItem->m_accountManagerInfo.have_credentials)
		{
			sTxt = pItem->m_accountManagerInfo.acct_mgr_name.c_str();
			if (iFound != -1)
			{
				if (iFound)
				{
					sTxt += " ERROR";
				}
				else
				{
					sTxt += " OK";
				}
			}

			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, sTxt);
		}
		else
		{
			m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, "-");
		}
	}
	else
	{
		m_lComputer.SetItemText(iPos, SELECT_COLUMN_STATUS, gszTranslation[PosWindowStatusMessageCombinedNotConnected]);
	}
}

void CDlgSelectComputer::ShowType(int iType)
{
	m_iType = iType;
	CString sWindowTitle;

	// do not show if main window isn't visible
	if (!theApp.m_pMainWnd->IsWindowVisible())
	{
		return;
	}


	switch (iType)
	{
		case SELECT_COMPUTER_NETWORK:
			sWindowTitle = gszTranslation[PosGroupMenuExtraAllowNetwork];
			m_button1.SetWindowText(gszTranslation[PosGroupMenuCommonAlways]);
			m_button2.SetWindowText(gszTranslation[PosGroupMenuCommonAuto]);
			m_button3.SetWindowText(gszTranslation[PosGroupMenuCommonNever]);
			m_button2.ShowWindow(SW_SHOW);
			m_button3.ShowWindow(SW_SHOW);
			m_button4.ShowWindow(SW_HIDE);
			break;
		case SELECT_COMPUTER_ALLOW:
			sWindowTitle = gszTranslation[PosGroupMenuExtraAllowToRun];
			m_button1.SetWindowText(gszTranslation[PosGroupMenuCommonAlways]);
			m_button2.SetWindowText(gszTranslation[PosGroupMenuCommonAuto]);
			m_button3.SetWindowText(gszTranslation[PosGroupMenuCommonSnooze]);
			m_button4.SetWindowText(gszTranslation[PosGroupMenuCommonNever]);
			m_button2.ShowWindow(SW_SHOW);
			m_button3.ShowWindow(SW_SHOW);
			m_button4.ShowWindow(SW_SHOW);
			break;
		case SELECT_COMPUTER_ALLOW_GPU:
			sWindowTitle = gszTranslation[PosGroupMenuExtraAllowToRunGpu];
			m_button1.SetWindowText(gszTranslation[PosGroupMenuCommonAlways]);
			m_button2.SetWindowText(gszTranslation[PosGroupMenuCommonAuto]);
			m_button3.SetWindowText(gszTranslation[PosGroupMenuCommonSnooze]);
			m_button4.SetWindowText(gszTranslation[PosGroupMenuCommonNever]);
			m_button2.ShowWindow(SW_SHOW);
			m_button3.ShowWindow(SW_SHOW);
			m_button4.ShowWindow(SW_SHOW);
			break;
		case SELECT_COMPUTER_BENCHMARK:
			sWindowTitle = gszTranslation[PosGroupMenuExtraBenchmark];
			m_button1.SetWindowText(gszTranslation[PosGroupMenuExtraBenchmark]);
			m_button2.ShowWindow(SW_HIDE);
			m_button3.ShowWindow(SW_HIDE);
			m_button4.ShowWindow(SW_HIDE);
			m_button4.ShowWindow(SW_HIDE);
			break;
		case SELECT_COMPUTER_READCONFIG:
			sWindowTitle = gszTranslation[PosGroupMenuExtraReadConfig];
			m_button1.SetWindowText(gszTranslation[PosGroupMenuExtraReadConfig]);
			m_button2.ShowWindow(SW_HIDE);
			m_button3.ShowWindow(SW_HIDE);
			m_button4.ShowWindow(SW_HIDE);
			m_button4.ShowWindow(SW_HIDE);
			break;
		case SELECT_COMPUTER_SYNCHRONIZE:
			sWindowTitle = gszTranslation[PosGroupMenuProjectsSynchronize];
			m_button1.SetWindowText(gszTranslation[PosGroupMenuProjectsSynchronize]);
			m_button2.ShowWindow(SW_HIDE);
			m_button3.ShowWindow(SW_HIDE);
			m_button4.ShowWindow(SW_HIDE);
			m_button4.ShowWindow(SW_HIDE);
			break;
	}
	this->SetWindowText(sWindowTitle);
	
	m_buttonOK.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	UpdateStatusAll();
	ShowWindow(SW_SHOW);
}

void CDlgSelectComputer::ActivateSelected(int iAction)
{
	int i, iSelectedCount = m_lComputer.GetSelectedCount();
	int  iItem = -1;

	// Update all of the selected items.
	if (iSelectedCount > 0)
	{
		for (i = 0; i < iSelectedCount; i++)
		{
			iItem = m_lComputer.GetNextItem(iItem, LVNI_SELECTED);
			if (iItem == -1) return;
			SendCommand(m_iType, iItem, iAction);
//			m_lComputer.Update(nItem);
		}
	}
}


void CDlgSelectComputer::SendCommand(int iType, int iItem, int iCommand)
{
//	CRpcConnectionItem *pItem;
//	pItem = m_pList->at(iItem);

	switch (iType)
	{
	case SELECT_COMPUTER_NETWORK:
		SendCommandNetwork(iItem, iCommand);
		break;
	case SELECT_COMPUTER_ALLOW:
		SendCommandAllow(iItem, iCommand);
		break;
	case SELECT_COMPUTER_ALLOW_GPU:
		SendCommandAllowGpu(iItem, iCommand);
	break;
	case SELECT_COMPUTER_BENCHMARK:
		SendCommandBenchmark(iItem, iCommand);
		break;
	case SELECT_COMPUTER_READCONFIG:
		SendCommandReadConfig(iItem, iCommand);
		break;

	case SELECT_COMPUTER_SYNCHRONIZE:
		SendCommandSynchronize(iItem, iCommand);
		break;

	}
	
}

void CDlgSelectComputer::SendCommandNetwork(int iItem, int iCommand)
{
	int		iNetworkMode;

	switch (iCommand)
	{
	case 0:
		iNetworkMode = RUN_MODE_ALWAYS;
		break;

	case 1:
		iNetworkMode = RUN_MODE_AUTO;

		break;
	default:
		iNetworkMode = RUN_MODE_NEVER;
	}
	::PostThreadMessage(m_pList->at(iItem)->m_iThreadRpcId, UWM_MSG_THREAD_SET_NETWORK_MODE, (WPARAM)this->m_hWnd, iNetworkMode);
}

void CDlgSelectComputer::SendCommandAllow(int iItem, int iCommand)
{
	CRpcConnectionItem *pItem = m_pList->at(iItem);
	CC_STATUS *pStatus;

	pStatus = &pItem->m_CcStatus;

	switch (iCommand)
	{
	case 0:
		pStatus->task_mode = RUN_MODE_ALWAYS;
		pStatus->task_mode_delay = 0;
		break;

	case 1:
		pStatus->task_mode = RUN_MODE_AUTO;
		pStatus->task_mode_delay = 0;
		break;
	case 2:
		if (pStatus->task_mode == RUN_MODE_NEVER)
		{
			if (pStatus->task_mode_delay != 0)
			{
				// already snoozing, restore.
				pStatus->task_mode = RUN_MODE_RESTORE;
				pStatus->task_mode_delay = 0;
				break;
			}
		}
		pStatus->task_mode = RUN_MODE_NEVER;
		pStatus->task_mode_delay = 60 * 60;
		break;
	default:
		pStatus->task_mode = RUN_MODE_NEVER;
		pStatus->task_mode_delay = 0;
	}
	::PostThreadMessage(m_pList->at(iItem)->m_iThreadRpcId, UWM_MSG_THREAD_SET_SNOOZE_CPU, (WPARAM)this->m_hWnd, (LPARAM)&pItem->m_CcStatus);
}

void CDlgSelectComputer::SendCommandAllowGpu(int iItem, int iCommand)
{
	CRpcConnectionItem *pItem = m_pList->at(iItem);
	CC_STATUS	*pStatus;
	int			iThreadCommand;

	pStatus = &pItem->m_CcStatus;

	switch (iCommand)
	{
	case 0:
		pStatus->gpu_mode = RUN_MODE_ALWAYS;
		pStatus->gpu_mode_delay = 0;
		break;

	case 1:
		pStatus->gpu_mode = RUN_MODE_AUTO;
		pStatus->gpu_mode_delay = 0;
		break;
	case 2:
		if (pStatus->gpu_mode == RUN_MODE_NEVER)
		{
			if (pStatus->gpu_mode_delay != 0)
			{
				// already snoozing, restore.
				pStatus->gpu_mode = RUN_MODE_RESTORE;
				pStatus->gpu_mode_delay = 0;
				break;
			}
		}
		pStatus->gpu_mode = RUN_MODE_NEVER;
		pStatus->gpu_mode_delay = 60 * 60;
		break;
	default:
		pStatus->gpu_mode = RUN_MODE_NEVER;
		pStatus->gpu_mode_delay = 0;
	}

	if (m_iType == SELECT_COMPUTER_ALLOW_GPU)
	{
		iThreadCommand = UWM_MSG_THREAD_SET_SNOOZE_GPU;
	}
	else 
	{
		iThreadCommand = UWM_MSG_THREAD_SET_SNOOZE_CPU;
	}

	::PostThreadMessage(m_pList->at(iItem)->m_iThreadRpcId, iThreadCommand, (WPARAM)this->m_hWnd, (LPARAM)&pItem->m_CcStatus);
	/*
	switch (iCommand)
	{
	case 0:
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode = RUN_MODE_ALWAYS;
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode_delay = 0;
		break;

	case 1:
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode = RUN_MODE_AUTO;
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode_delay = 0;
		break;
	case 2:
		if (m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode == RUN_MODE_NEVER)
		{
			if (m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode_delay != 0)
			{
				// already snoozing, restore.
				m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode = RUN_MODE_RESTORE;
				m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode_delay = 0;
				break;
			}
		}
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode = RUN_MODE_NEVER;
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode_delay = 60 * 60;
		break;
	default:
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode = RUN_MODE_NEVER;
		m_lListConnectionsItems.at(iNr)->m_CcStatus.gpu_mode_delay = 0;
	}
	::PostThreadMessa
	*/
}

void CDlgSelectComputer::SendCommandReadConfig(int iItem, int iCommand)
{
	::PostThreadMessage(m_pList->at(iItem)->m_iThreadRpcId, UWM_MSG_THREAD_READ_CONFIG, 0, 0);
}

void CDlgSelectComputer::SendCommandBenchmark(int iItem, int iCommand)
{
	::PostThreadMessage(m_pList->at(iItem)->m_iThreadRpcId, UWM_MSG_THREAD_BENCHMARK, 0, 0);
}

void CDlgSelectComputer::SendCommandSynchronize(int iItem, int iCommand)
{
	if (m_pList->at(iItem)->m_accountManagerInfo.have_credentials)
	{
		::PostThreadMessage(m_pList->at(iItem)->m_iThreadRpcId, UWM_MSG_THREAD_ACCOUNTMANAGER_SYNCHRONIZE, (WPARAM)this->m_hWnd, (LPARAM)&m_pList->at(iItem)->m_accountManagerInfo);
	}
}

void CDlgSelectComputer::InvalidateAllow()
{
//	CRpcConnectionItem *pItem;
	CC_STATUS *pStatus;
	if (m_pList == NULL) return;

	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		pStatus = &m_pList->at(iCount)->m_CcStatus;
		pStatus->task_mode = -1;
		pStatus->task_mode_delay = -1;
		pStatus->gpu_mode = -1;
		pStatus->gpu_mode_delay = -1;
	}
}

void CDlgSelectComputer::RemoveStatusList()
{
		for (int iCount = 0; iCount < m_lComputer.GetItemCount(); iCount++)
	{
		m_lComputer.SetItemText(iCount, SELECT_COLUMN_STATUS, "");
	}
}

/*
void CDlgSelectComputer::BenchMarkReady()
{

}
*/
//void RequestStatus()
//{//

//}

void CDlgSelectComputer::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registrySelectComputerPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registrySelectComputerPosTop, wp.rcNormalPosition.top);
}

void CDlgSelectComputer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMPUTER, m_lComputer);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
	DDX_Control(pDX, IDC_BUTTON4, m_button4);
	DDX_Control(pDX, IDOK, m_buttonOK);
}


BEGIN_MESSAGE_MAP(CDlgSelectComputer, CDialogEx)
	ON_WM_TIMER()

	ON_MESSAGE(ID_COMPUTER_NET_DIALOG_OPEN, OnNetworkOpen)
	ON_MESSAGE(ID_COMPUTER_ALLOW_DIALOG_OPEN, OnAllowOpen)
	ON_MESSAGE(ID_COMPUTER_ALLOW_GPU_DIALOG_OPEN, OnAllowGpuOpen)
	ON_MESSAGE(ID_COMPUTER_BENCHMARK_DIALOG_OPEN, OnBenchmarkOpen)
	ON_MESSAGE(ID_COMPUTER_READ_CONFIG_DIALOG_OPEN, OnReadConfigOpen)
	ON_MESSAGE(ID_PROJECTS_SYNCHRONIZE_DIALOG_OPEN, OnSynchronizeOpen)

	ON_MESSAGE(ID_TOOLBAR_ALLOW_NETWORK, OnToolbarAllowNetwork)
	ON_MESSAGE(ID_TOOLBAR_ALLOW_CPU, OnToolbarAllowCpu)
	ON_MESSAGE(ID_TOOLBAR_ALLOW_GPU, OnToolbarAllowGpu)
	
	ON_MESSAGE(UWM_MSG_THREAD_SET_NETWORK_MODE_READY, OnThreadNetworkModeReady)
	ON_MESSAGE(UWM_MSG_THREAD_READ_STATUS_READY, OnThreadReadStatusReady)
	ON_MESSAGE(UWM_MSG_THREAD_SET_SNOOZE_CPU_READY, OnThreadReadSnoozeCpuGpuReady)
	ON_MESSAGE(UWM_MSG_THREAD_SET_SNOOZE_GPU_READY, OnThreadReadSnoozeCpuGpuReady)
	ON_MESSAGE(UWM_MSG_THREAD_GET_HOST_INFO_READY, OnThreadReadHostInfoReady)
	ON_MESSAGE(UWM_MSG_THREAD_ACCOUNTMANAGER_SYNCHRONIZE_READY, OnThreadSynchronizeReady)
	
	ON_BN_CLICKED(IDOK, &CDlgSelectComputer::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSelectComputer::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgSelectComputer::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgSelectComputer::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgSelectComputer::OnBnClickedButton4)
END_MESSAGE_MAP()


BOOL CDlgSelectComputer::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	AddColumns();

	m_lComputer.SetExtendedStyle(m_lComputer.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registrySelectComputerPosLeft, 0);
	int top = theApp.GetProfileInt(registrySectionWindowPosition, registrySelectComputerPosTop, 0);
	SetWindowPos(&CWnd::wndBottom, left, top, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE); //| SWP_NOSIZE

	m_iTimerInterval = 0;
	m_iTimerIntervalNext = 4;
	SetTimer(UTIMER_DLG_SELECT_COMPUTER, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSelectComputer::OnTimer(UINT_PTR nIDEvent)
{
	m_iTimerIntervalNext++;
	if (m_iTimerIntervalNext > 20)
	{
		m_iTimerIntervalNext = 20;
	}

	if (m_iTimerInterval-- < 0)
	{
		m_iTimerInterval = m_iTimerIntervalNext;
		CRpcConnectionItem *pItem;
		if (m_pList == NULL) return;

		for (int iCount = 0; iCount < m_pList->size(); iCount++)
		{
			pItem = m_pList->at(iCount);
			if (pItem->m_pcThreadRpcId == NULL)
			{
				continue;
			}
			::PostThreadMessage(pItem->m_iThreadRpcId, UWM_MSG_THREAD_READ_STATUS, (WPARAM)this->m_hWnd, 0);
			if (m_iType == SELECT_COMPUTER_BENCHMARK)
			{
				::PostThreadMessage(pItem->m_iThreadRpcId, UWM_MSG_THREAD_GET_HOST_INFO, (WPARAM) this->m_hWnd, (LPARAM) &pItem->m_hostInfo);
			}
		}
	}

	if (this->IsWindowVisible())
	{
		if (!theApp.m_pMainWnd->IsWindowVisible())
		{
			// main windows is not visible, hide this one as well
			ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_iType = -1;
		m_iTimerIntervalNext = 20;
	}

}

LRESULT CDlgSelectComputer::OnNetworkOpen(WPARAM parm1, LPARAM parm2)
{
	RemoveStatusList();
	m_iTimerInterval = 0;
	m_iTimerIntervalNext = 0;
	ShowType(SELECT_COMPUTER_NETWORK);
	return TRUE;
}

LRESULT CDlgSelectComputer::OnAllowOpen(WPARAM parm1, LPARAM parm2)
{
	InvalidateAllow();
	RemoveStatusList();
	m_iTimerInterval = 0;
	m_iTimerIntervalNext = 0;
	ShowType(SELECT_COMPUTER_ALLOW);
	return TRUE;
}

LRESULT CDlgSelectComputer::OnAllowGpuOpen(WPARAM parm1, LPARAM parm2)
{
	InvalidateAllow();
	RemoveStatusList();
	m_iTimerInterval = 0;
	m_iTimerIntervalNext = 0;
	ShowType(SELECT_COMPUTER_ALLOW_GPU);
	return TRUE;
}

LRESULT CDlgSelectComputer::OnBenchmarkOpen(WPARAM parm1, LPARAM parm2)
{
	RemoveStatusList();
	m_iTimerInterval = 0;
	m_iTimerIntervalNext = 0;
	ShowType(SELECT_COMPUTER_BENCHMARK);
	return TRUE;
}

LRESULT CDlgSelectComputer::OnReadConfigOpen(WPARAM parm1, LPARAM parm2)
{
	RemoveStatusList();
	m_iTimerInterval = 0;
	m_iTimerIntervalNext = 0;
	ShowType(SELECT_COMPUTER_READCONFIG);
	return TRUE;
}

LRESULT CDlgSelectComputer::OnSynchronizeOpen(WPARAM parm1, LPARAM parm2)
{
	RemoveStatusList();
	m_iTimerInterval = 0;
	m_iTimerIntervalNext = 0;
	ShowType(SELECT_COMPUTER_SYNCHRONIZE);
	return TRUE;
}

LRESULT CDlgSelectComputer::OnToolbarAllowNetwork(WPARAM parm1, LPARAM parm2)
{
	int iNr = (int)parm1;
	int iMode = (int)parm2;	
	SendCommand(SELECT_COMPUTER_NETWORK, iNr, iMode);
	UpdateStatusNetworkItem(iNr);
	return TRUE;
}
LRESULT CDlgSelectComputer::OnToolbarAllowCpu(WPARAM parm1, LPARAM parm2)
{
	int iNr = (int)parm1;
	int iMode = (int)parm2;
	SendCommand(SELECT_COMPUTER_ALLOW, iNr, iMode);
	return TRUE;
}
LRESULT CDlgSelectComputer::OnToolbarAllowGpu(WPARAM parm1, LPARAM parm2)
{
	int iNr = (int)parm1;
	int iMode = (int)parm2;
	SendCommand(SELECT_COMPUTER_ALLOW_GPU, iNr, iMode);
	return TRUE;
}

LRESULT CDlgSelectComputer::OnThreadReadStatusReady(WPARAM wParam, LPARAM lParam)
{
	CC_STATUS *pStatus;
	CThreadRpc *pThread;
	CRpcConnectionItem	*pItem;
	int iNetworkMode, iConnectionstatus, iTaskMode, iTaskModeDelay, iGpuMode, iGpuModeDelay;
	bool bChange;

	pStatus = (CC_STATUS*)lParam;
	iNetworkMode = pStatus->network_mode;
	iTaskMode = pStatus->task_mode;
	iTaskModeDelay = (int) pStatus->task_mode_delay;
	iGpuMode = pStatus->gpu_mode;
	iGpuModeDelay = (int)pStatus->gpu_mode_delay;
	pThread = (CThreadRpc *)wParam;

	if (iNetworkMode == -1) iConnectionstatus = RPC_STATUS_NONE;
	else iConnectionstatus = RPC_STATUS_CONNECTED;

	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		if (m_pList->at(iCount)->m_pThreadRpc == pThread)
		{
			pItem = m_pList->at(iCount);
			// here we set the connection status used in RemoteCombinedDoc as well
			pItem->m_iConnectionStatus = iConnectionstatus;
			if (pItem->m_CcStatus.network_mode != iNetworkMode)
			{
				pItem->m_CcStatus.network_mode = iNetworkMode;
				if (m_iType == SELECT_COMPUTER_NETWORK)
				{
					UpdateStatusNetworkItem(iCount);
				}
			}
			// CPU
			bChange = false;
			if (pItem->m_CcStatus.task_mode != iTaskMode)
			{
				bChange = true;
			}
			if (pItem->m_CcStatus.task_mode_delay != iTaskModeDelay)
			{
				bChange = true;
			}
			if (bChange)
			{
				pItem->m_CcStatus.task_mode = iTaskMode;
				pItem->m_CcStatus.task_mode_delay = iTaskModeDelay;
				if (m_iType == SELECT_COMPUTER_ALLOW)
				{
					UpdateStatusAllowItem(iCount, pItem, false);
				}
			}
			//GPU
			bChange = false;
			if (pItem->m_CcStatus.gpu_mode != iGpuMode)
			{
				bChange = true;
			}
			if (pItem->m_CcStatus.gpu_mode_delay != iGpuModeDelay)
			{
				bChange = true;
			}
			if (bChange)
			{
				pItem->m_CcStatus.gpu_mode = iGpuMode;
				pItem->m_CcStatus.gpu_mode_delay = iGpuModeDelay;
				if (m_iType == SELECT_COMPUTER_ALLOW_GPU)
				{
					UpdateStatusAllowItem(iCount, pItem, true);
				}
			}
			if (m_iType == SELECT_COMPUTER_READCONFIG)
			{
				UpdateStatusConnected(iCount, pItem);
			}
			if (m_iType == SELECT_COMPUTER_BENCHMARK)
			{
				UpdateStatusBenchmark(iCount, pItem);
			}
			if (m_iType == SELECT_COMPUTER_SYNCHRONIZE)
			{
				UpdateStatusSynchronize(iCount, pItem, -1);
			}
		}
	}


	return TRUE;
}


LRESULT CDlgSelectComputer::OnThreadNetworkModeReady(WPARAM wParam, LPARAM lParam)
{
	int			iNetworkMode;
	int			iConnectionstatus;
	CThreadRpc	*pThread;
	CRpcConnectionItem	*pItem;

	pThread = (CThreadRpc *)wParam;
	iNetworkMode = (int)lParam;

	if (iNetworkMode == -1) iConnectionstatus = RPC_STATUS_NONE;
	else iConnectionstatus = RPC_STATUS_CONNECTED;

	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		pItem = m_pList->at(iCount);
		if (pItem->m_pThreadRpc == pThread)
		{
			pItem->m_CcStatus.network_mode = iNetworkMode;
			pItem->m_iConnectionStatus = iConnectionstatus;
			UpdateStatusNetworkItem(iCount);
		}
	}

	return 0;
}

LRESULT CDlgSelectComputer::OnThreadReadSnoozeCpuGpuReady(WPARAM wParam, LPARAM lParam)
{
	CRpcThreadReturn	*pRpcThreadReturn;
	CC_STATUS			*pCcStatus;
	CThreadRpc			*pThread;
	CRpcConnectionItem	*pItem;
	bool				bGpu;

	pRpcThreadReturn = (CRpcThreadReturn *)wParam;
	pCcStatus = (CC_STATUS *)lParam;

	if (strcmp(pRpcThreadReturn->m_pcHostName, LOCALHOST_NAME) == 0)
	{
		theApp.m_pMainFrame->SendMessage(UWM_MSG_SET_SNOOZE_CPU_READY, wParam, lParam);
	}
	pThread = pRpcThreadReturn->m_pThread;
	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		if (m_pList->at(iCount)->m_pThreadRpc == pThread)
		{
			pItem = m_pList->at(iCount);
			pItem->m_CcStatus.task_mode = pCcStatus->task_mode;
			pItem->m_CcStatus.task_mode_delay = pCcStatus->task_mode_delay;
			pItem->m_CcStatus.gpu_mode = pCcStatus->gpu_mode;
			pItem->m_CcStatus.gpu_mode_delay = pCcStatus->gpu_mode_delay;
			if (m_iType == SELECT_COMPUTER_ALLOW_GPU)
			{
				bGpu = true;
			}
			else
			{
				bGpu = false;
			}
			UpdateStatusAllowItem(iCount, pItem, bGpu);
		}
	}

	delete pRpcThreadReturn;

	return 0;
}

LRESULT CDlgSelectComputer::OnThreadReadHostInfoReady(WPARAM parm1, LPARAM parm2)
{
	CRpcConnectionItem *pItem;
	int iStatus;
	HOST_INFO *pHostInfo;

	iStatus = (int)parm1;

	if (iStatus == 0)
	{
		pHostInfo = (HOST_INFO *)parm2;
		for (int iCount = 0; iCount < m_pList->size(); iCount++)
		{
			pItem = m_pList->at(iCount);
			if (&pItem->m_hostInfo == pHostInfo)
			{
				if (pItem->m_dBenchmarktime != pHostInfo->p_calculated)
				{
					pItem->m_dBenchmarktime = pHostInfo->p_calculated;
					UpdateStatusBenchmark(iCount, pItem);
				}
			}
		}
	}
	return true;
}

LRESULT CDlgSelectComputer::OnThreadSynchronizeReady(WPARAM wParam, LPARAM lParam)
{
	CRpcConnectionItem	*pItem;
	CRpcThreadReturn	*pThreadReturn;
	int					iResult, iThreadId;
	//char				*pcComputerId;

	iResult = (int)wParam;
	pThreadReturn = (CRpcThreadReturn *)lParam;
	iThreadId = pThreadReturn->m_iThreadId;
	delete pThreadReturn;

	for (int iCount = 0; iCount < m_pList->size(); iCount++)
	{
		pItem = m_pList->at(iCount);
		if (pItem->m_iThreadRpcId == iThreadId)
		{
			UpdateStatusSynchronize(iCount, pItem, iResult);
		}
	}

	return true;
}



void CDlgSelectComputer::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CDlgSelectComputer::OnBnClickedButton1()
{
	ActivateSelected(0);
}


void CDlgSelectComputer::OnBnClickedButton2()
{
	ActivateSelected(1);
}


void CDlgSelectComputer::OnBnClickedButton3()
{
	ActivateSelected(2);
}


void CDlgSelectComputer::OnBnClickedButton4()
{
	ActivateSelected(3);
}

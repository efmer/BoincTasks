// DlgTThrottle.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "RemoteCombinedDoc.h"
#include "DlgTThrottle.h"
#include "DlgSettingsGeneral.h"
#include "DlgGraphic.h"
#include "ThreadRpc.h"
#include "afxdialogex.h"
#include "Translation.h"
#include "ThreadRpcClient.h"

// CDlgTThrottle dialog

IMPLEMENT_DYNAMIC(CDlgTThrottle, CDialogEx)

CDlgTThrottle::CDlgTThrottle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTThrottle::IDD, pParent)
{
	m_pDoc = NULL;

	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iSelectedComputers = 0;
	m_bComputerSelectionChanged = false;
	m_iUpdateNumbers = 0;
	m_bUpdateApply = false;
	m_bAutoActiveEnabled = true;
	m_bTimerActive = false;

	CRpcConnectionItem *pRpcConnectionItem;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pRpcConnectionItem = new CRpcConnectionItem;
		m_lListConnectionsItems.push_back(pRpcConnectionItem);
	}

	// only one entry....
	CRpcInfoClient *pRpcInfoClient;
	pRpcInfoClient = new CRpcInfoClient;
	pRpcInfoClient->m_bReady = false;
	pRpcInfoClient->m_bReadyCopy = false;
	pRpcInfoClient->m_iLostCount = 0;
	pRpcInfoClient->m_bConnectionLogged = false;
	pRpcInfoClient->m_sComputerId = "";
	m_lRpcInfoClient.push_back(pRpcInfoClient);
}

CDlgTThrottle::~CDlgTThrottle()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
	for (int iCount = 0; iCount < (int) m_lListConnectionsItems.size(); iCount++)
	{
		delete m_lListConnectionsItems.at(iCount);
	}
	for (int iCount = 0; iCount < (int) m_lRpcInfoClient.size(); iCount++)
	{
//		delete m_pRpcInfoClient[iCount]->m_pcComputerId;		// never here it's only a copy
		delete m_lRpcInfoClient.at(iCount);
	}
}

void CDlgTThrottle::InitialUpdate(CRemoteCombinedDoc *pDoc)
{
	m_pDoc = pDoc;
}

void CDlgTThrottle::Init()
{
	int iComputers;

	iComputers = m_iSelectedComputers;

	m_listComputers.ResetContent();
	for (int iCount = 0; iCount < iComputers; iCount++)
	{
		m_listComputers.AddString(m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId);
	}

	m_bInit = true;
	m_sSelectedComputer = "";
	m_iSelectedThreadId = -1;
	m_bThreadLocked = false;
	m_iThreadLockedTimeout = 0;
	m_listComputers.SetCurSel(0);	// select the first one

//	m_editInfo.SetTabStops(8);

	OnLbnSelchangeListTthrottleComputers();
}

void CDlgTThrottle::ComputerSelectionChanged()
{
	int iSelected, iComputers;
	CString sComputer;
	if (m_pDoc == NULL) return;
	if (m_pDoc->m_bDocumentIsClosing) return;			// everything is closing down.

	iSelected = m_listComputers.GetCurSel();
	m_listComputers.GetText(iSelected,sComputer);
	m_iUpdateNumbers = TTHROTTLE_UPDATE_NUMBERS;

	if (sComputer != m_sSelectedComputer)
	{
		m_iSelectedThreadId = -1;
		InvalidReading();

		m_sSelectedComputer = sComputer;
		iComputers = m_iSelectedComputers;

		for (int iCount = 0; iCount < iComputers; iCount++)
		{
			if (sComputer == m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId)
			{
				if (m_pDoc->GetRpcThreadConnected(&sComputer))
				{
					m_iSelectedThreadId = m_lListConnectionsItems.at(iCount)->m_iThreadTThrottleRpcId;
//					::PostThreadMessage(m_iSelectedThreadId, UWM_MSG_THREAD_START_RPC_CLIENT_TEMPERATURE, (WPARAM) this->m_hWnd, (LPARAM) &m_temperatureGraphBuffer);
					break;
				}
				else
				{
//					m_window.Reset();
					m_sSelectedComputer = "";
				}
			}
		}
	}
}

void CDlgTThrottle::InvalidReading()
{
	CString sFormat;

	sFormat = "?";
	m_editCore.SetWindowText(sFormat);
	m_editSetCore.SetWindowText(sFormat);
	m_editGpu.SetWindowText(sFormat);
	m_editSetGpu.SetWindowText(sFormat);
	m_editRunningCpu.SetWindowText(sFormat);
	m_editRunningGpu.SetWindowText(sFormat);
	m_editMaxCpu.SetWindowText(sFormat);
	m_editMinCpu.SetWindowText(sFormat);
	m_editInfo.SetWindowText(sFormat);
	m_checkAutoActive.EnableWindow(FALSE);
	m_bAutoActiveEnabled = false;
	m_iUpdateNumbers = TTHROTTLE_UPDATE_NUMBERS;
	SetWindowText("TThrottle");
}

void CDlgTThrottle::ButtonsActive(BOOL bActive)
{
	m_buttonApply.EnableWindow(bActive);
	m_listComputers.EnableWindow(bActive);

	m_editSetCore.EnableWindow(bActive);
	m_editSetGpu.EnableWindow(bActive);
	m_editMaxCpu.EnableWindow(bActive);
	m_editMinCpu.EnableWindow(bActive);

	if (bActive)
	{
		if (m_bAutoActiveEnabled)
		{
			m_checkAutoActive.EnableWindow(bActive);
		}
		else m_checkAutoActive.EnableWindow(FALSE);
	}
	else m_checkAutoActive.EnableWindow(FALSE);
}

BOOL CDlgTThrottle::PreTranslateMessage(MSG* pMsg)	// catch enter and esc
{
	if (pMsg->wParam == VK_ESCAPE)
	{
		CDialog::OnCancel();
	}

	if (pMsg->wParam == VK_RETURN)
	{
		CDialog::OnOK();
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CDlgTThrottle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TTHROTTLE_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_CORE, m_editCore);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_CORE, m_textCore);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_CORE_SET, m_editSetCore);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_CORE_SET, m_textSetCore);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_GPU, m_editGpu);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_GPU, m_textGpu);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_GPU_SET, m_editSetGpu);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_GPU_SET, m_textSetGpu);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_CPU_RUN, m_editRunningCpu);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_CPU_RUN, m_textRunningCpu);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_GPU_RUN, m_editRunningGpu);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_GPU_RUN, m_textRunningGpu);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_CPU_MAX, m_editMaxCpu);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_CPU_MAX, m_textMaxCpu);
	DDX_Control(pDX, IDC_EDIT_TTHROTTLE_CPU_MIN, m_editMinCpu);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_CPU_MIN, m_textMinCpu);
	DDX_Control(pDX, IDC_CHECK_TTHROTTLE_AUTO, m_checkAutoActive);
	DDX_Control(pDX, IDC_EDIT_INFO, m_editInfo);
	DDX_Control(pDX, IDAPPLY, m_buttonApply);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_GROUP_COMPUTERS, m_textComputers);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_GROUP_TEMPERATURE, m_textTemperature);
	DDX_Control(pDX, IDC_TEXT_TTHROTTLE_GROUP_RUN, m_textRunPercentage);
}


BEGIN_MESSAGE_MAP(CDlgTThrottle, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
//	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()

	ON_MESSAGE(UWM_MSG_THREAD_ID,OnRpcConnection)
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_CLIENT, OnReadyRpcClient)

	ON_BN_CLICKED(IDC_CHECK_TTHROTTLE_AUTO, &CDlgTThrottle::OnBnClickedCheckTthrottleAuto)
	ON_EN_CHANGE(IDC_EDIT_TTHROTTLE_CORE_SET, &CDlgTThrottle::OnEnChangeEditTthrottleCoreSet)
	ON_EN_CHANGE(IDC_EDIT_TTHROTTLE_GPU_SET, &CDlgTThrottle::OnEnChangeEditTthrottleGpuSet)
	ON_EN_CHANGE(IDC_EDIT_TTHROTTLE_CPU_MAX, &CDlgTThrottle::OnEnChangeEditTthrottleCpuMax)
	ON_EN_CHANGE(IDC_EDIT_TTHROTTLE_CPU_MIN, &CDlgTThrottle::OnEnChangeEditTthrottleCpuMin)
	ON_LBN_SELCHANGE(IDC_LIST_TTHROTTLE_COMPUTERS, &CDlgTThrottle::OnLbnSelchangeListTthrottleComputers)
	ON_BN_CLICKED(IDAPPLY, &CDlgTThrottle::OnBnClickedApply)
	ON_BN_CLICKED(IDOK, &CDlgTThrottle::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgTThrottle message handlers

BOOL CDlgTThrottle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translation

	SetWindowText("TThrottle");

	m_textComputers.SetWindowText(gszTranslation[PosDialogTThrottleTitleComputer]);
	m_textTemperature.SetWindowText(gszTranslation[PosDialogTThrottleTitleTemperature]);
	m_textRunPercentage.SetWindowText(gszTranslation[PosDialogTThrottleTitleRunPercentage]);
	m_textCore.SetWindowText(gszTranslation[PosDialogTThrottleCore]);
	m_textSetCore.SetWindowText(gszTranslation[PosDialogTThrottleSetCore]);
	m_textGpu.SetWindowText(gszTranslation[PosDialogTThrottleGpu]);
	m_textSetGpu.SetWindowText(gszTranslation[PosDialogTThrottleSetGpu]);
	m_textRunningCpu.SetWindowText(gszTranslation[PosDialogTThrottleRunningCpu]);
	m_textRunningGpu.SetWindowText(gszTranslation[PosDialogTThrottleRunningGpu]);
	m_textMaxCpu.SetWindowText(gszTranslation[PosDialogTThrottleMaxCpu]);
	m_textMinCpu.SetWindowText(gszTranslation[PosDialogTThrottleMinCpu]);
	m_checkAutoActive.SetWindowText(gszTranslation[PosDialogTThrottleAutoActive]);
	m_buttonApply.SetWindowText(gszTranslation[PosDialogCommonButtonsApply]);
// Translation

//	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryTThrottlePosLeft, 0);
//	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryTThrottlePosTop, 0);
//	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryTThrottlePosRight, 800) - left;
//	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryTThrottlePosBottom, 540) - top;
// 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER ); //| SWP_NOSIZE

//	SetTimer(UTIMER_DLG_TTHROTTLE, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgTThrottle::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryTThrottlePosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryTThrottlePosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryTThrottlePosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryTThrottlePosBottom, wp.rcNormalPosition.bottom);
}

void CDlgTThrottle::OnTimer(UINT_PTR nIDEvent)
{
	if (!this->IsWindowVisible())
	{
		if (m_bTimerActive)
		{
			KillTimer(UTIMER_DLG_TTHROTTLE);
			m_bTimerActive = false;
		}
		return;
	}
	if (m_pDoc == NULL) return;
	if (m_pDoc->m_bDocumentIsClosing) return;			// everything is closing down.

	if (!m_bInit)
	{
		m_bInit = true;
		Init();
	}

	if (!m_bThreadLocked)
	{
		if (m_bUpdateApply)
		{
			if (m_iSelectedThreadId > 0)
			{
				m_lRpcInfoClient.at(0)->m_bReady = false;

				CString sEdit;
				m_editSetCore.GetWindowText(sEdit);
				m_lRpcInfoClient.at(0)->m_iSetCore = atoi(sEdit);
				m_editSetGpu.GetWindowText(sEdit);
				m_lRpcInfoClient.at(0)->m_iSetGpu = atoi(sEdit);
				m_editMaxCpu.GetWindowText(sEdit);
				m_lRpcInfoClient.at(0)->m_iMaxCpu = atoi(sEdit);
				m_editMinCpu.GetWindowText(sEdit);	
				m_lRpcInfoClient.at(0)->m_iMinCpu = atoi(sEdit);
				m_lRpcInfoClient.at(0)->m_iActivive = m_checkAutoActive.GetCheck();

				::PostThreadMessage(m_iSelectedThreadId,UWM_MSG_THREAD_START_RPC_CLIENT_SET,(WPARAM) m_hWnd, (LPARAM) m_lRpcInfoClient.at(0));
				m_bThreadLocked = true;
				m_iThreadLockedTimeout = 0;
			}

			m_iUpdateNumbers = TTHROTTLE_UPDATE_NUMBERS;
			m_bUpdateApply = false;
//			m_buttonApply.EnableWindow(FALSE);
		}
		else
		{
			if (m_bComputerSelectionChanged)
			{
				ComputerSelectionChanged();
				m_bComputerSelectionChanged = false;
				return;
			}
			if (m_iSelectedThreadId > 0)
			{
				m_lRpcInfoClient.at(0)->m_bReady = false;
				m_lRpcInfoClient.at(0)->m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
				::PostThreadMessage(m_iSelectedThreadId,UWM_MSG_THREAD_START_RPC_CLIENT,(WPARAM) m_hWnd, (LPARAM) m_lRpcInfoClient.at(0));
				m_bThreadLocked = true;
				m_iThreadLockedTimeout = 0;
			}
			else
			{
				ButtonsActive(TRUE);
			}
		}
	}
	else
	{
		if (m_iThreadLockedTimeout++ > 30)
		{
			m_bThreadLocked = false;
			m_iThreadLockedTimeout = 0;
		}
		if (m_iThreadLockedTimeout++ > 5)
		{
			int ii = 1;
		}

	}
//	if (m_bThreadLocked) m_buttonApply.EnableWindow(FALSE);
}

LRESULT CDlgTThrottle::OnReadyRpcClient(WPARAM parm1, LPARAM parm2)
{
	CRpcInfoClient *pInfoClient;
	CString			sFormat;
	bool			bInvalid;

	if (m_pDoc->m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	pInfoClient = (CRpcInfoClient *) parm1;
	if (pInfoClient->m_iStatus == 0)		// valid
	{
		if (!pInfoClient->m_bReady)
		{
			m_bThreadLocked = false;
			m_iThreadLockedTimeout = 0;
			return 0;			// TThrottle set gives not back valid data after a set
		}

		sFormat.Format("TThrottle  V %.2f", pInfoClient->m_fVersion);
		SetWindowText(sFormat);

		sFormat.Format("%.0f °C", pInfoClient->m_fTopCpuTemperature);
		m_editCore.SetWindowText(sFormat);
		sFormat.Format("%.0f °C", pInfoClient->m_fTopGpuTemperature);
		m_editGpu.SetWindowText(sFormat);
		sFormat.Format("%.0f%%", pInfoClient->m_fCpuThrottle);
		m_editRunningCpu.SetWindowText(sFormat);
		sFormat.Format("%.0f%%", pInfoClient->m_fGpuThrottle);
		m_editRunningGpu.SetWindowText(sFormat);
		m_editInfo.SetWindowText(pInfoClient->m_sTxt);
	
		if (m_iUpdateNumbers > 0)
		{
			m_iUpdateNumbers--;
			bInvalid = false;
			if (pInfoClient->m_iSetCore < 0) {sFormat = "??"; bInvalid = true;}
			else sFormat.Format("%d", pInfoClient->m_iSetCore);
			m_editSetCore.SetWindowText(sFormat);
			if (pInfoClient->m_iSetGpu < 0) {sFormat = "??"; bInvalid = true;}
			else sFormat.Format("%d", pInfoClient->m_iSetGpu);
			m_editSetGpu.SetWindowText(sFormat);
			if (pInfoClient->m_iMaxCpu < 0) {sFormat = "??"; bInvalid = true;}
			else sFormat.Format("%d", pInfoClient->m_iMaxCpu);
			m_editMaxCpu.SetWindowText(sFormat);
			if (pInfoClient->m_iMinCpu < 0) {sFormat = "??"; bInvalid = true;}
			else sFormat.Format("%d", pInfoClient->m_iMinCpu);
			m_editMinCpu.SetWindowText(sFormat);	
			if (pInfoClient->m_iActivive >=0)
			{
				m_checkAutoActive.SetCheck(pInfoClient->m_iActivive);
//				m_checkAutoActive.EnableWindow(TRUE);
				m_bAutoActiveEnabled = true;
			}
			else
			{
				m_checkAutoActive.EnableWindow(FALSE);
				m_bAutoActiveEnabled = false;
			}
		}
		else
		{
			if (m_iUpdateNumbers == 0)
			{
				ButtonsActive(TRUE);
				m_iUpdateNumbers--;
			}
		}

		m_bThreadLocked = false;
		m_iThreadLockedTimeout = 0;
//		m_buttonApply.EnableWindow(TRUE);
		return 0;
	}

	InvalidReading();
	ButtonsActive(TRUE);
	m_bThreadLocked = false;
	m_iThreadLockedTimeout = 0;
//	m_buttonApply.EnableWindow(TRUE);
	return 0;
}

void CDlgTThrottle::OnSize(UINT nType, int cx, int cy )
{
//	SetWindowPosition(cx, cy, nType);
}

void CDlgTThrottle::OnSizing(UINT fwSide, LPRECT pRect)
{
	LONG lTop, lBottom,lLeft, lRight, lWidth, lHeight;
	bool bSetWindow = false;
	bool bSetWindowWidth = false;
	bool bSetWindowHeight = false;

// limit the minimum size of the dialog

	lTop	= pRect->top;
	lBottom = pRect->bottom;
	lLeft	= pRect->left;
	lRight	= pRect->right;
	lWidth	= lRight - lLeft;
	lHeight	= lBottom - lTop;

	if (lWidth < 400)
	{
		bSetWindow = true;
		bSetWindowWidth = true;
	}
	if (lHeight < 200)
	{
		bSetWindow = true;
		bSetWindowHeight = true;
	}

	if (bSetWindowWidth)
	{
		pRect->right  = pRect->left+ 401;
	}
	if (bSetWindowHeight)
	{
		pRect->bottom = pRect->top + 201;
	}

	CDialog::OnSizing(fwSide, pRect);
}

void CDlgTThrottle::OnShowWindow(BOOL bShow, UINT nStatus)
{
//	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if (!m_bTimerActive)
		{
			SetTimer(UTIMER_DLG_TTHROTTLE, 1500, 0); // was 1000 < 1.74
			m_bTimerActive = true;
		}
	}
}

LRESULT CDlgTThrottle::OnRpcConnection(WPARAM parm1, LPARAM parm2)
{
	std::deque<CRpcConnectionItem*>	*plListConnectionsItems;
	plListConnectionsItems = (std::deque<CRpcConnectionItem*> *) parm2;

	m_iSelectedComputers = (int) parm1;

	for (int iCount = 0; iCount < m_iSelectedComputers; iCount++)
	{
		m_lListConnectionsItems.at(iCount)->m_iThreadTThrottleRpcId = plListConnectionsItems->at(iCount)->m_iThreadTThrottleRpcId;
		m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId = plListConnectionsItems->at(iCount)->m_pcThreadTThrottleRpcId;
	}
	m_bInit = false;
	return 0;
}

void CDlgTThrottle::OnLbnSelchangeListTthrottleComputers()
{
	m_bComputerSelectionChanged = true;
	ButtonsActive(FALSE);
}

void CDlgTThrottle::OnBnClickedCheckTthrottleAuto()
{
//	m_buttonApply.EnableWindow(TRUE);
//	m_bUpdateNeeded = true;
}


void CDlgTThrottle::OnEnChangeEditTthrottleCoreSet()
{
//	m_buttonApply.EnableWindow(TRUE);
//	m_bUpdateNeeded = true;
}


void CDlgTThrottle::OnEnChangeEditTthrottleGpuSet()
{
//	m_buttonApply.EnableWindow(TRUE);
//	m_bUpdateNeeded = true;
}


void CDlgTThrottle::OnEnChangeEditTthrottleCpuMax()
{
//	m_buttonApply.EnableWindow(TRUE);
//	m_bUpdateNeeded = true;
}


void CDlgTThrottle::OnEnChangeEditTthrottleCpuMin()
{
//	m_buttonApply.EnableWindow(TRUE);
//	m_bUpdateNeeded = true;
}


void CDlgTThrottle::OnBnClickedApply()
{
//	if (m_bThreadLocked) return;

	m_bUpdateApply = true;
	ButtonsActive(FALSE);
//	m_bUpdateNeeded = false;
}


void CDlgTThrottle::OnBnClickedOk()
{
	OnBnClickedApply();
//	m_bHideWhenReady = true;
}



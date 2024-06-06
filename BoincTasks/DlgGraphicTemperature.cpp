// DlgGraphicScalable.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgGraphicTemperature.h"
#include "DlgColorTemperature.h"
#include "Translation.h"

// CDlgGraphicTemperature dialog

IMPLEMENT_DYNAMIC(CDlgGraphicTemperature, CDialog)

CDlgGraphicTemperature::CDlgGraphicTemperature(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphicTemperature::IDD, pParent)
{
//	UINT	*piTemperature;
	m_pDoc = NULL;

	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int iCount = 0; iCount < V3_TEMPERATURE_NR_OF_TOTAL;iCount++)
	{
		m_bAddCoresList[iCount] = false;
	}
	m_bAddStringDone = false;
	m_bInvalidate = false;
	m_iInvalideWait = 0;
	m_iInvalidateEvery = 100;
	m_bThreadLocked = false;
	m_iThreadLocked = 0;
	m_iWindowInitialLeftMargin = -1;
	m_bTimerActive = false;

	m_temperatureGraphBuffer.m_piTemperatureBuffer = new UINT[(V2_TEMPERATURE_BUFFER_LEN * (V3_TEMPERATURE_NR_OF_TOTAL)) + 6];	// 2 percentage, Time, max cpu, max gpu
	m_temperatureGraphBuffer.m_tLastAddTime = 0;

	m_iSelectedComputers = 0;
	m_iSelectedTemperature = -1;	// nothing selected yet
	m_bInit = true;
	m_iSelectedThreadId = -1;
	m_bColorWindowUpdate = true;
	m_bColorTemperatureDialogKnowsUs = false;
	m_bComputerSelectionChanged = false;
	m_bFullScreenGraph = false;
	m_lLastWidth = 0;
	m_lLastHeight = 0;

	CRpcConnectionItem *pRpcConnectionItem;
	CTemperatureGraphItem *pTemperatureGraphItem;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pRpcConnectionItem = new CRpcConnectionItem;
		m_lListConnectionsItems.push_back(pRpcConnectionItem);

		pTemperatureGraphItem = new CTemperatureGraphItem;
		pTemperatureGraphItem->m_tLastAddTime = 0;
		m_lListTemperature.push_back(pTemperatureGraphItem);
	}
}

CDlgGraphicTemperature::~CDlgGraphicTemperature()
{
	delete m_temperatureGraphBuffer.m_piTemperatureBuffer;

	for (int iCount = 0; iCount < (int) m_lListConnectionsItems.size(); iCount++)
	{
		delete m_lListConnectionsItems.at(iCount);
	}
	for (int iComputer = 0; iComputer < (int) m_lListTemperature.size(); iComputer++)
	{
		for (int iCore = 0; iCore < (int) m_lListTemperature.at(iComputer)->m_lTemperatureCores.size(); iCore++)
		{
			delete m_lListTemperature.at(iComputer)->m_lTemperatureCores.at(iCore);
		}
		delete m_lListTemperature.at(iComputer);
	}
}

void CDlgGraphicTemperature::InitialUpdate(CRemoteCombinedDoc *pDoc)
{
	m_pDoc = pDoc;
}

void CDlgGraphicTemperature::Init()
{
	CString sFormat;
	int iComputers, iCheck;

	iComputers = m_iSelectedComputers;

	m_listComputers.ResetContent();
	for (int iCount = 0; iCount < iComputers; iCount++)
	{
		m_listComputers.AddString(m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId);
	}

	for (int iCount = 0; iCount < GRAPHIC_ARRAY_NUMBER_TEMPERATURE; iCount++)
	{
		m_bActive[iCount] = false;
		m_check[iCount].EnableWindow(FALSE);
		m_wndColor[iCount].ShowWindow(SW_HIDE);

		sFormat.Format("item_%d",iCount);
		iCheck = theApp.GetProfileInt(registrySectionGraphicTemperature,sFormat, 1);
		m_check[iCount].SetCheck(iCheck);
		m_bActive[iCount] = (iCheck != 0);
	}
	m_window.SetCores(&m_bActive[0]);

	m_sSelectedComputer = "";
	m_iSelectedThreadId = -1;

	m_listComputers.SetCurSel(0);	// select the first one
	OnLbnSelchangeListGraphicTemperatureComputers();
}

void CDlgGraphicTemperature::ComputerSelectionChanged()
{
	int iSelected, iComputers;
	CString sComputer;
	if (m_pDoc == NULL) return;
	if (m_pDoc->m_bDocumentIsClosing) return;			// everything is closing down.

//	m_iSelectedTemperature = -1;
//	m_iSelectedThreadId = -1;

	iSelected = m_listComputers.GetCurSel();
	m_listComputers.GetText(iSelected,sComputer);

	if (sComputer != m_sSelectedComputer)
	{
		m_sSelectedComputer = sComputer;
		iComputers = m_iSelectedComputers;

		for (int iCount = 0; iCount < iComputers; iCount++)
		{
			if (sComputer == m_lListConnectionsItems.at(iCount)->m_pcThreadTThrottleRpcId)
			{
				if (m_pDoc->GetRpcThreadConnected(&sComputer))
				{
					m_iSelectedThreadId = m_lListConnectionsItems.at(iCount)->m_iThreadTThrottleRpcId;
					m_window.Reset();
					m_iSelectedTemperature = iCount;

					std::deque<CTemperatureOneCore *>	*plTemperatureCores;
					plTemperatureCores = &m_lListTemperature.at(m_iSelectedTemperature)->m_lTemperatureCores;
					for (int iAddCore = 0; iAddCore < (int)  plTemperatureCores->size(); iAddCore++)
					{
						for (int iAddTemperature = 0; iAddTemperature < (int) plTemperatureCores->at(iAddCore)->m_lTemperatureCore.size(); iAddTemperature++)
						{
							m_window.AddTemperature(iAddCore,plTemperatureCores->at(iAddCore)->m_lTemperatureCore.at(iAddTemperature), 0, m_lListTemperature.at(m_iSelectedTemperature)->m_tLastAddTime);
						}
					}

					for (int iCountCores = 0; iCountCores < V3_TEMPERATURE_NR_OF_TOTAL;iCountCores++)
					{
						m_bAddCoresList[iCountCores] = false;
						m_wndColor[iCountCores].ShowWindow(SW_HIDE);	
						m_check[iCountCores].EnableWindow(FALSE);
						//m_check[iCountCores].SetCheck(FALSE);
					}

					m_temperatureGraphBuffer.m_tLastAddTime = m_lListTemperature.at(iCount)->m_tLastAddTime;
					::PostThreadMessage(m_iSelectedThreadId, UWM_MSG_THREAD_START_RPC_CLIENT_TEMPERATURE, (WPARAM) this->m_hWnd, (LPARAM) &m_temperatureGraphBuffer);
					break;
				}
				else
				{
					m_window.Reset();
					m_sSelectedComputer = "";
				}
			}
		}
	}
}

void CDlgGraphicTemperature::ShowControls(bool bShow)
{
	int iShow;

	if (bShow) iShow = SW_SHOW;
	else iShow = SW_HIDE;

	for (int iCount = 0; iCount < GRAPHIC_ARRAY_NUMBER_TEMPERATURE;iCount++)
	{
		m_bAddCoresList[iCount] = false;
		m_wndColor[iCount].ShowWindow(iShow);
		m_check[iCount].ShowWindow(iShow);
	}

	m_slider.ShowWindow(iShow);
	m_sliderFine.ShowWindow(iShow);
	m_listComputers.ShowWindow(iShow);
	m_setTime.ShowWindow(iShow);

	m_buttonHelp.ShowWindow(iShow);
}

void CDlgGraphicTemperature::AddTemperatureToList(int iComputer, int iCore, float fTemperature)
{
	CTemperatureOneCore	*pListOneCore;

	while ((int) m_lListTemperature.at(iComputer)->m_lTemperatureCores.size() <= iCore)
	{
		pListOneCore = new CTemperatureOneCore;
		m_lListTemperature.at(iComputer)->m_lTemperatureCores.push_back(pListOneCore);
	}

	pListOneCore = m_lListTemperature.at(iComputer)->m_lTemperatureCores.at(iCore);

	pListOneCore->m_lTemperatureCore.push_back(fTemperature);

	while (pListOneCore->m_lTemperatureCore.size() > MAX_SAMPLE_BUFFER_GRAPHIC_SCALABLE+1)
	{
		pListOneCore->m_lTemperatureCore.erase(pListOneCore->m_lTemperatureCore.begin());
	}
}

void CDlgGraphicTemperature::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WND_GRAPHIC_TEMPERATURE_DRAWING, m_window);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_COMBO_TIME, m_setTime);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_SLIDER_OFFSET, m_slider);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_SLIDER_OFFSET_FINE, m_sliderFine);
	DDX_Control(pDX, IDC_LIST_GRAPHIC_TEMPERATURE_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU0, m_check[CORE00]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU1, m_check[CORE01]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU2, m_check[CORE02]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU3, m_check[CORE03]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU4, m_check[CORE04]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU5, m_check[CORE05]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU6, m_check[CORE06]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU7, m_check[CORE07]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU8, m_check[CORE08]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU9, m_check[CORE09]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU10, m_check[CORE10]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU11, m_check[CORE11]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU12, m_check[CORE12]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU13, m_check[CORE13]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU14, m_check[CORE14]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU15, m_check[CORE15]);

	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU0, m_check[GPU0]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU1, m_check[GPU1]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU2, m_check[GPU2]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU3, m_check[GPU3]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU4, m_check[GPU4]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU5, m_check[GPU5]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU6, m_check[GPU6]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU7, m_check[GPU7]);

	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU_P, m_check[THROTTLE_CPU]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU_P, m_check[THROTTLE_GPU]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_CPU_MAX, m_check[LIMIT_CPU]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_CHECK_GPU_MAX, m_check[LIMIT_GPU]);

	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU_P, m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_THROTTLE]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU_P, m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_THROTTLE]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU_MAX, m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_LIMIT]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU_MAX, m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_LIMIT]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU0 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_1]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU1 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_2]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU2 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_3]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU3 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_4]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU4 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_5]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU5 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_6]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU6 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_7]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU7 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_8]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU8 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_9]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU9 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_10]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU10 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_11]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU11 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_12]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU12 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_13]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU13 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_14]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU14 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_15]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_CPU15 , m_wndColor[GRAPHIC_TEMPERATURE_CPU_ARRAY_16]);

	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU0 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_1]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU1 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_2]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU2 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_3]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU3 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_4]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU4 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_5]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU5 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_6]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU6 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_7]);
	DDX_Control(pDX, IDC_GRAPHIC_TEMPERATURE_WND_GPU7 , m_wndColor[GRAPHIC_TEMPERATURE_GPU_ARRAY_8]);

	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}

BEGIN_MESSAGE_MAP(CDlgGraphicTemperature, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_HELPINFO()

	ON_CBN_SELCHANGE(IDC_GRAPHIC_TEMPERATURE_COMBO_TIME, &CDlgGraphicTemperature::OnCbnSelchangeGraphicTemperatureComboTime)
	
	ON_MESSAGE(UWM_MSG_THREAD_ID,OnRpcConnection)
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_CLIENT_TEMPERATURE, OnThreadTemperature)
	ON_MESSAGE(UWM_GRAPHIC_COLOR_CHANGED, OnColorChanged)

	ON_LBN_SELCHANGE(IDC_LIST_GRAPHIC_TEMPERATURE_COMPUTERS, &CDlgGraphicTemperature::OnLbnSelchangeListGraphicTemperatureComputers)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU0, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu0)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU1, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu1)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU2, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu2)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU3, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu3)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU4, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu4)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU5, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu5)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU6, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu6)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU7, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu7)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU8, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu8)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU9, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu9)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU10, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu10)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU11, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu11)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU12, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu12)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU13, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu13)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU14, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu14)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU15, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu15)

	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU_P, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpuP)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU0, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu0)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU1, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu1)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU2, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu2)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU3, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu3)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU4, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu4)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU5, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu5)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU6, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu6)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU7, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu7)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU_P, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpuP2)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_CPU_MAX, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpuMax)
	ON_BN_CLICKED(IDC_GRAPHIC_TEMPERATURE_CHECK_GPU_MAX, &CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpuMax)

	ON_BN_CLICKED(ID_HELP, &CDlgGraphicTemperature::OnBnClickedHelp)
END_MESSAGE_MAP()

// CDlgGraphicTemperature message handlers

BOOL CDlgGraphicTemperature::OnInitDialog()
{
	CString sTxtId, sTxt;
	int	iSelection;

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// translation
	this->SetWindowText(gszTranslation[PosDialogTemperatureGraphicTitle]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC1Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC5Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC10Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC30Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC60Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC3Hour]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC6Hour]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC12Hour]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC24Hour]);

	for (int iCount = 0; iCount < V3_TEMPERATURE_NR_OF_CPU; iCount++)
	{
		sTxtId = gszTranslation[PosDialogTemperatureGraphicCore];
		sTxtId+= " %d";
		sTxt.Format(sTxtId,iCount);
		m_check[CORE00+iCount].SetWindowText(sTxt);
	}
	for (int iCount = 0; iCount < V3_TEMPERATURE_NR_OF_GPU; iCount++)
	{
		sTxtId = gszTranslation[PosDialogTemperatureGraphicGpu];
		sTxtId+= " %d";
		sTxt.Format(sTxtId,iCount);
		m_check[GPU0+iCount].SetWindowText(sTxt);
	}

	m_check[THROTTLE_CPU].SetWindowText(gszTranslation[PosDialogTemperatureGraphicCpuPerc]);
	m_check[THROTTLE_GPU].SetWindowText(gszTranslation[PosDialogTemperatureGraphicGpuPerc]);

	m_check[LIMIT_CPU].SetWindowText(gszTranslation[PosDialogTemperatureGraphicCpuMax]);
	m_check[LIMIT_GPU].SetWindowText(gszTranslation[PosDialogTemperatureGraphicGpuMax]);

	// translation

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	iSelection = theApp.GetProfileInt(registrySectionGraphicTemperature,registryTime, 2);
	m_setTime.SetCurSel(iSelection);
	OnCbnSelchangeGraphicTemperatureComboTime();

	m_bTimerFastMode = true;

	m_slider.SetRange(0,SLIDER_VALUE);
	m_sliderFine.SetRange(0,SLIDER_VALUE_FINE);

	m_slider.SetPos(SLIDER_VALUE);
	m_sliderFine.SetPos(SLIDER_VALUE_FINE);

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosLeft, 0);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosTop, 0);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosRight, 800) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosBottom, 540) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER ); //| SWP_NOSIZE

//	SetTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE, 1, 0);	// interval 1 mSec

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgGraphicTemperature::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTemperaturePosBottom, wp.rcNormalPosition.bottom);
}

void CDlgGraphicTemperature::OnTimer(UINT_PTR nIDEvent)
{
	if (!this->IsWindowVisible())
	{
		if (m_bTimerActive)
		{
			KillTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE);
			m_bTimerActive = false;
		}
		return;
	}
	if (m_pDoc == NULL) return;
	if (m_pDoc->m_bDocumentIsClosing) return;			// everything is closing down.

	if (m_bColorWindowUpdate)
	{
		COLORREF *pColor;
		pColor = theApp.m_pDlgColorTemperature->GetColorArray();
		m_bColorWindowUpdate = false;
		for (int iCount = 0; iCount < GRAPHIC_ARRAY_NUMBER_TEMPERATURE; iCount++)
		{
			m_wndColor[iCount].SetColor(*pColor);
			m_wndColor[iCount].Invalidate(TRUE);
			pColor++;
		}
	}

	if (!m_bColorTemperatureDialogKnowsUs)
	{
		theApp.m_pDlgColorTemperature->PostMessage(UWM_GRAPHIC_COLOR_CHANGED, (WPARAM) this->m_hWnd,0);		// request a check for a color update
	}

	if (m_bInvalidate)
	{
		if (m_iInvalideWait++ > 1)
		{
			Invalidate(TRUE);
			m_bInvalidate = false;
			m_iInvalideWait = 0;
		}
	}

	if (!m_bInit)
	{
		m_bInit = true;
		Init();
	}

	if (!m_bThreadLocked)
	{
		if (m_bComputerSelectionChanged)
		{
			ComputerSelectionChanged();
			m_bComputerSelectionChanged = false;
			return;
		}
		if (m_iSelectedThreadId > 0)
		{
			if (m_iSelectedTemperature >=0)
			{
				m_temperatureGraphBuffer.m_tLastAddTime = m_lListTemperature.at(m_iSelectedTemperature)->m_tLastAddTime;
				::PostThreadMessage(m_iSelectedThreadId, UWM_MSG_THREAD_START_RPC_CLIENT_TEMPERATURE, (WPARAM) this->m_hWnd, (LPARAM) &m_temperatureGraphBuffer);
				m_bThreadLocked = true;
				m_iThreadLocked = 0;
				//m_iDisableNotUsedCheck = 0;
			}
		}
	}
	else
	{
		if (m_iThreadLocked++ > 60)
		{
			m_bThreadLocked = false;
			m_iThreadLocked = 0;
		}
	}

	if (m_iSelectedTemperature< 0)
	{
		int iSelected;
		CString sComputer;
		iSelected = m_listComputers.GetCurSel();
		m_listComputers.GetText(iSelected,sComputer);
		if (m_pDoc->GetRpcThreadConnected(&sComputer))
		{
			ComputerSelectionChanged();		// no valid selection try again now we are connected.
		}
	}


}

void CDlgGraphicTemperature::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}


void CDlgGraphicTemperature::OnSizing(UINT fwSide, LPRECT pRect)
{
	LONG lTop, lBottom,lLeft, lRight, lWidth, lHeight;
	int	iMinHeight;
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

	if (m_bFullScreenGraph) iMinHeight = 80;
	else iMinHeight = 120;

	if (lWidth < 200)
	{
		bSetWindow = true;
		bSetWindowWidth = true;
	}
	if (lHeight < iMinHeight)
	{
		bSetWindow = true;
		bSetWindowHeight = true;
	}

	if (bSetWindowWidth)
	{
		pRect->right  = pRect->left+ 201;
	}
	if (bSetWindowHeight)
	{
		pRect->bottom = pRect->top + iMinHeight + 1;
	}

	CDialog::OnSizing(fwSide, pRect);
}

void CDlgGraphicTemperature::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
{
	LONG lTopMargin, lBottomMargin, lLeftMargin, lRightMargin, lWidthGraphic, lHeightGraphic;

	m_lLastWidth = lWidth;
	m_lLastHeight = lHeight;

	// get initial position of the graph window.
	if (m_iWindowInitialLeftMargin == -1)
	{
		if (m_window.m_hWnd != NULL)
		{
			WINDOWPLACEMENT wp;
			m_window.GetWindowPlacement(&wp);
			m_iWindowInitialLeftMargin = wp.rcNormalPosition.left;
			m_iWindowInitialTopMargin = wp.rcNormalPosition.top;
		}
	}

	lTopMargin		= 5;
	lBottomMargin	= 5;
	lLeftMargin		= 5;
	lRightMargin	= 5;

	if (!m_bFullScreenGraph)
	{
		lLeftMargin= m_iWindowInitialLeftMargin;
		lTopMargin = m_iWindowInitialTopMargin;
	}

	lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
	lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));

// position graphic relative to window NOT the screen

	if (m_window.m_hWnd != NULL)
	{
		m_window.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
	}

	m_bInvalidate = true;
}

void CDlgGraphicTemperature::OnShowWindow(BOOL bShow, UINT nStatus)
{
//	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CWnd::OnShowWindow(bShow, nStatus);
//	m_bStartUpdateStatistics = true;			// make sure the RPC is called again

	if (bShow)
	{
		if (!m_bTimerActive)
		{
			SetTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE, 1, 0);	// interval 1 mSec
			m_bTimerActive = true;
			m_bTimerFastMode = true;
		}
	}
}

void CDlgGraphicTemperature::OnCbnSelchangeGraphicTemperatureComboTime()
{
	int iSelection;
	int iGridHorzTotal, iGridHorzStep, iGridHorzThickEvery, iScaleHorzValue, iScaleHorzValueStep, iSamplesPerWindow;
	bool bReset = false;

	iSelection = m_setTime.GetCurSel();

	switch (iSelection)
	{
		case 1:	iGridHorzTotal			= 30;	// 5 minutes 
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 6;
				iScaleHorzValue			= 5;
				iScaleHorzValueStep		= 1;
				iSamplesPerWindow		= 300;
		break;
		case 2:	iGridHorzTotal			= 60;	// 10 minute
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 6;
				iScaleHorzValue			= 10;
				iScaleHorzValueStep		= 1;
				iSamplesPerWindow		= 600;
		break;
		case 3:	iGridHorzTotal			= 30;	// 30 minute
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 10;
				iScaleHorzValue			= 30;
				iScaleHorzValueStep		= 10;
				iSamplesPerWindow		= 1800;
		break;
		case 4:	iGridHorzTotal			= 60;	// 60 minutes
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 10;
				iScaleHorzValue			= 60;
				iScaleHorzValueStep		= 10;
				iSamplesPerWindow		= 3600;
		break;
		case 5:	iGridHorzTotal			= 18;	// 3 Hour
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 6;
				iScaleHorzValue			= 3;
				iScaleHorzValueStep		= 1;
				iSamplesPerWindow		= 10800;
		break;
		case 6:	iGridHorzTotal			= 36;	// 6 Hour
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 6;
				iScaleHorzValue			= 6;
				iScaleHorzValueStep		= 1;
				iSamplesPerWindow		= 21600;
		break;
		case 7:	iGridHorzTotal			= 72;	// 12 Hour
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 6;
				iScaleHorzValue			= 12;
				iScaleHorzValueStep		= 1;
				iSamplesPerWindow		= 43200;
		break;	
		case 8:	iGridHorzTotal			= 144;	// 24 Hour
				iGridHorzStep			= 1;
				iGridHorzThickEvery		= 6;
				iScaleHorzValue			= 24;
				iScaleHorzValueStep		= 1;
				iSamplesPerWindow		= 86400;
		break;	

		default: iGridHorzTotal			= 60;	// 0 = 1 minute
				 iGridHorzStep			= 1;
				 iGridHorzThickEvery	= 10;
				 iScaleHorzValue		= 60;				
				 iScaleHorzValueStep	= 10;
				iSamplesPerWindow		= 60;
	}

	m_window.SetScale(false, iGridHorzTotal, iGridHorzStep, iGridHorzThickEvery, iScaleHorzValue, iScaleHorzValueStep, iSamplesPerWindow);
	m_window.Invalidate(TRUE);

	theApp.WriteProfileInt(registrySectionGraphicTemperature,registryTime,iSelection);
}

void CDlgGraphicTemperature::AddTemperature(int iComputer, int iCore, float fTemperature)
{
	CString sTxtId;
	int		iPos,iPosFine;

	iPos = m_slider.GetPos();
	iPosFine = m_sliderFine.GetPos();

	AddTemperatureToList(iComputer, iCore, fTemperature);
	m_window.AddTemperature(iCore,fTemperature, (SLIDER_VALUE-iPos) + (SLIDER_VALUE_FINE-iPosFine) , m_lListTemperature.at(m_iSelectedTemperature)->m_tLastAddTime);

	if (iCore < TEMPERATURE_ARRAY)
	{
		if (!m_bAddCoresList[iCore])						// not added yet?
		{
			for (int iCount = 0; iCount < TEMPERATURE_ARRAY; iCount++)
			{
				if (!m_bAddCoresList[iCount])				// free
				{
					if (iCount == iCore)					// only use the 1e one free to keep the numbering ok!!!
					{
						m_bAddCoresList[iCount] = true;		// now used
						CString sTxt;	

						if (!m_bFullScreenGraph)
						{
							m_wndColor[iCount].ShowWindow(SW_SHOW);
						}
						m_check[iCount].EnableWindow(TRUE);//.ShowWindow(SW_SHOW);
					}
				}
			}
		}
	}

	if (!m_bAddStringDone)
	{
		m_bAddStringDone = true;
	}
}


void CDlgGraphicTemperature::OnBnClickedHide()
{
	this->ShowWindow(SW_HIDE);
}

LRESULT CDlgGraphicTemperature::OnRpcConnection(WPARAM parm1, LPARAM parm2)
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

LRESULT CDlgGraphicTemperature::OnThreadTemperature(WPARAM parm1, LPARAM parm2)
{
	int		iItems, iStatus;
	int		iVersion, iCpuMaxTemperature, iGpuMaxTemperature, iVersionTotal, iVersionBufferLen;
	UINT	*piTemperature, *piTemperatureTemp;
	float	fTemperature;
//	CTemperatureGraphItem *pTemperatureGraphItem;

	iItems = (int) parm1;
	iStatus = (int) parm2;

	if (m_iSelectedTemperature < 0) return 0;

	if (!iStatus)
	{
		m_window.SetConnected(true);
		piTemperature = m_temperatureGraphBuffer.m_piTemperatureBuffer;
		m_lListTemperature.at(m_iSelectedTemperature)->m_tLastAddTime = m_temperatureGraphBuffer.m_tLastAddTime;	//+= iItems;

		iVersion = *piTemperature;
		if (iVersion == 3)
		{
			iVersionTotal = V3_TEMPERATURE_NR_OF_TOTAL;			// V3 is correct
			iVersionBufferLen = V3_TEMPERATURE_BUFFER_LEN;
		}
		else
		{
			iVersionTotal = V3_TEMPERATURE_NR_OF_TOTAL;
			iVersionBufferLen = V2_TEMPERATURE_BUFFER_LEN;
		}

		piTemperature++;			// time not used
		piTemperature++;
		iCpuMaxTemperature = *piTemperature;
		m_window.m_iCpuMaxTemperature = iCpuMaxTemperature;
		piTemperature++;
		iGpuMaxTemperature = *piTemperature;
		m_window.m_iGpuMaxTemperature = iGpuMaxTemperature;
		piTemperature++;

		for (int iCore = 0; iCore < iVersionTotal; iCore++)
		{
			piTemperatureTemp = piTemperature;
			for (int iCountItems = 0; iCountItems < iItems; iCountItems++)
			{
				if (*piTemperatureTemp == STOP_TEMPERATURE)
				{
					break;
				}
				fTemperature = (float) *piTemperatureTemp;
				fTemperature /= 10;
				AddTemperature(m_iSelectedTemperature, iCore, fTemperature);
				piTemperatureTemp++;
			}
			piTemperature += iVersionBufferLen;
		}
	}
	else
	{
		m_bTimerFastMode = false;
		KillTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE);
		SetTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE, 1000, 0);	// interval 1 Sec
		m_bTimerActive = true;
		m_window.SetConnected(false);
		m_bThreadLocked = false;
		return 0;
	}

	// adjust timer according to the number of items still to fetch
	if (iItems > 10)
	{
		if (m_iInvalidateEvery++ > 20)
		{
			m_window.Invalidate(FALSE);
			m_iInvalidateEvery = 0;
		}
		if (!m_bTimerFastMode)
		{
			m_bTimerFastMode = true;
			KillTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE);
			SetTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE, 1, 0);	// interval 1 mSec
			m_bTimerActive = true;
		}
	}
	else
	{
		m_window.Invalidate(FALSE);
		if (m_bTimerFastMode)
		{
			m_bTimerFastMode = false;
			KillTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE);
			SetTimer(UTIMER_DLG_GRAPHIC_TEMPERATURE, 1000, 0);	// interval 1 Sec
			m_bTimerActive = true;
		}
	}

	m_bThreadLocked = false;

	return 0;

}

LRESULT CDlgGraphicTemperature::OnColorChanged(WPARAM parm1, LPARAM parm2)
{
	m_bColorWindowUpdate = true;
	m_bColorTemperatureDialogKnowsUs = true;
	return 0;
}

void CDlgGraphicTemperature::OnLbnSelchangeListGraphicTemperatureComputers()
{
	m_bComputerSelectionChanged = true;
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu0()
{
	Check(CORE00);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu1()
{
	Check(CORE01);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu2()
{
	Check(CORE02);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu3()
{
	Check(CORE03);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu4()
{
	Check(CORE04);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu5()
{
	Check(CORE05);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu6()
{
	Check(CORE06);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu7()
{
	Check(CORE07);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu8()
{
	Check(CORE08);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu9()
{
	Check(CORE09);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu10()
{
	Check(CORE10);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu11()
{
	Check(CORE11);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu12()
{
	Check(CORE12);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu13()
{
	Check(CORE13);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu14()
{
	Check(CORE14);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpu15()
{
	Check(CORE15);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpuP()
{
	Check(THROTTLE_CPU);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckCpuMax()
{
	Check(LIMIT_CPU);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu0()
{
	Check(GPU0);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu1()
{
	Check(GPU1);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu2()
{
	Check(GPU2);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu3()
{
	Check(GPU3);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu4()
{
	Check(GPU4);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu5()
{
	Check(GPU5);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu6()
{
	Check(GPU6);
}


void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpu7()
{
	Check(GPU7);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpuP2()
{
	Check(THROTTLE_GPU);
}

void CDlgGraphicTemperature::OnBnClickedGraphicTemperatureCheckGpuMax()
{
	Check(LIMIT_GPU);
}

void CDlgGraphicTemperature::Check(int iCheckItem)
{
	CString sFormat;
	int iCheck;

	iCheck = m_check[iCheckItem].GetCheck();

	m_bActive[iCheckItem] = (iCheck != 0);

	sFormat.Format("item_%d",iCheckItem);
	theApp.WriteProfileInt(registrySectionGraphicTemperature,sFormat,iCheck);

	m_window.SetCores(&m_bActive[0]);
}


void CDlgGraphicTemperature::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bFullScreenGraph = !m_bFullScreenGraph;

	SetWindowPosition(m_lLastWidth, m_lLastHeight, 0);
	Invalidate(TRUE);
	ShowControls(!m_bFullScreenGraph);

}

BOOL CDlgGraphicTemperature::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgGraphicTemperature::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks-graphs#temperature";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}
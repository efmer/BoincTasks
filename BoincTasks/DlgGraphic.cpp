
#include "stdafx.h"
#include "BoincTasks.h"
#include "MainFrm.h" 
#include "RemoteCombinedDoc.h"
#include "DlgGraphic.h"
#include "ThreadRpc.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"
#include "DlgColorGraphic.h"
#include "DlgLogging.h"
#include "Translation.h"

#include "Xml.h"

// CDlgGraphic dialog

IMPLEMENT_DYNAMIC(CDlgGraphic, CDialog)

CDlgGraphic::CDlgGraphic(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphic::IDD, pParent)
{
	m_pDoc = NULL;

	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iNrThreadRpc = 0;
	m_bStartUpdateStatistics = false;
	m_iNrThreadReturn = 0;
	m_bThreadBusy = false;
	m_iLineThickness = 2;
	m_iWindowInitialLeftMargin = -1;
	m_bProjectsOneGraph = false;
	m_bFullScreenGraph = false;
	m_lLastWidth = 0;
	m_lLastHeight = 0;
	m_bInvalidate = false;
	m_bShowWindow = false;
	m_iShowWindowDelay = 0;

	m_webType.m_iTotalOrAverage = GRAPHIC_TOTAL;
	m_webType.m_iHostOrUser = GRAPHIC_HOST;

	m_bWebProjectsSingle = false;
}

CDlgGraphic::~CDlgGraphic()
{
	int iCount;

	if (!m_hIcon)
	{
		delete m_hIcon;
	}
	for (iCount = 0; iCount < (int) m_lComputer.size(); iCount++)
	{
		delete m_lComputer.at(iCount);
	}
	for (iCount = 0; iCount < (int) m_lWebComputers.size(); iCount++)
	{
		delete m_lWebComputers.at(iCount);
	}
	for (iCount = 0; iCount < (int) m_lWebProjects.size(); iCount++)
	{
		delete m_lWebProjects.at(iCount);
	}
}

void CDlgGraphic::InitialUpdate(CRemoteCombinedDoc *pDoc, bool bShowWindow)
{
	m_bShowWindow = bShowWindow;
	m_iShowWindowDelay = 0;
	m_pDoc = pDoc;
}

void CDlgGraphic::ShowControls(bool bShow)
{
	int iShow;

	if (bShow) iShow = SW_SHOW;
	else iShow = SW_HIDE;

	m_radioUser.ShowWindow(iShow);
	m_radioTotal.ShowWindow(iShow);
	m_radioHost.ShowWindow(iShow);
	m_radioAverage.ShowWindow(iShow);
	m_buttonUpdate.ShowWindow(iShow);
	m_checkCombineProject.ShowWindow(iShow);
	m_checkExpanded.ShowWindow(iShow);

	m_editLineThickness.ShowWindow(iShow);

	m_textLineThickness.ShowWindow(iShow);
	m_textComputers.ShowWindow(iShow);
	m_textProjects.ShowWindow(iShow);

	m_listProjects.ShowWindow(iShow);
	m_listComputers.ShowWindow(iShow);

	m_checkSingleMultiple.ShowWindow(iShow);

	m_buttonHelp.ShowWindow(iShow);

	if (m_bDebugMode)
	{
		m_textDebugMode.ShowWindow(iShow);
	}
	else
	{
		m_textDebugMode.ShowWindow(SW_HIDE);
	}
}

#ifdef GRAPHIC_TEST_XML
// testing only !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CDlgGraphic::AddTest(PROJECT *pProjectStatistics)
{
	CString		sFile, sTemp;
	DAILY_STATS	stats;

	sFile = "statistics_test.xml"; // E:\Programma\BoincTasks\BoincTasks_1_8_0\x64\debug   remove master url tag

	CXml xml(sFile,false,true);

	if (xml.ReadStart("project_statistics"))
	{
		pProjectStatistics->master_url = "http://setiathome.berkeley.edu/";
		while (xml.ReadGroup("daily_statistics",true))
		{
			xml.ReadItem("day",&sTemp);
			stats.day = atoi(sTemp);
			xml.ReadItem("user_total_credit",&sTemp);
			stats.user_total_credit = atoi(sTemp);
			xml.ReadItem("user_expavg_credit",&sTemp);
			stats.user_expavg_credit = atoi(sTemp);
			xml.ReadItem("host_total_credit",&sTemp);
			stats.host_total_credit = atoi(sTemp);
			xml.ReadItem("host_expavg_credit",&sTemp);
			stats.host_expavg_credit = atoi(sTemp);
			pProjectStatistics->statistics.push_back(stats);
		}
	}
}
#endif

void CDlgGraphic::PostCommandView(int iCommand)
{
	POSITION posDoc, posView ;
	CRemoteCombinedDoc *pDocCombined;
	CView *pView;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc)
	{
		pDocCombined = (CRemoteCombinedDoc*) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		posView = pDocCombined->GetFirstViewPosition();
		while (posView)
		{
			pView = pDocCombined->GetNextView(posView);
			pView->SendMessage(WM_COMMAND, iCommand,0);	// fist send to the document that sends it to the view
		}
	}
}

void CDlgGraphic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_RADIO_GRAPHIC_USER, m_radioUser);
	DDX_Control(pDX, IDC_RADIO_GRAPHIC_TOTAL, m_radioTotal);
	DDX_Control(pDX, IDC_RADIO_GRAPHIC_HOST, m_radioHost);
	DDX_Control(pDX, IDC_RADIO_GRAPHIC_AVERAGE, m_radioAverage);

	DDX_Control(pDX, IDC_WND_GRAPHIC, m_window);
	//	DDX_Control(pDX, IDC_BUTTON_COLORS, m_colors);
	DDX_Control(pDX, IDC_LIST_GRAPHIC_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_LIST_GRAPHIC_PROJECTS, m_listProjects);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_GROUP_COMPUTERS, m_textComputers);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_GROUP_PROJECTS, m_textProjects);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_buttonUpdate);
	DDX_Control(pDX, IDC_FULL_DEBUG_MODE, m_textDebugMode);
	DDX_Control(pDX, IDC_EDIT_GRAPHIC_LINE, m_editLineThickness);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_LINE_THICKNESS, m_textLineThickness);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_PROJECTS_SAME, m_checkCombineProject);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, m_checkSingleMultiple);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_EXTENDED, m_checkExpanded);

	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(CDlgGraphic, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_MOUSEMOVE( )
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_HELPINFO()

	ON_LBN_SELCHANGE(IDC_LIST_GRAPHIC_COMPUTERS, &CDlgGraphic::OnLbnSelchangeListGraphicComputers)
	ON_LBN_SELCHANGE(IDC_LIST_GRAPHIC_PROJECTS, &CDlgGraphic::OnLbnSelchangeListGraphicProjects)

	ON_MESSAGE(UWM_INITIAL_GRAPHIC,OnInitialGraphic)
//	ON_MESSAGE(UWM_CLOSE_GRAPHIC,OnCloseGraphic)

	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_GRAPHIC, OnGraphicsReady)
	ON_MESSAGE(UWM_GRAPHIC_COLOR_CHANGED, OnColorChanged)
	ON_MESSAGE(UWM_MSG_GRAPHIC_GET, GetGraphic)

	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgGraphic::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_RADIO_GRAPHIC_USER, &CDlgGraphic::OnBnClickedRadioGraphicUser)
	ON_BN_CLICKED(IDC_RADIO_GRAPHIC_TOTAL, &CDlgGraphic::OnBnClickedRadioGraphicTotal)
	ON_BN_CLICKED(IDC_RADIO_GRAPHIC_HOST, &CDlgGraphic::OnBnClickedRadioGraphicHost)
	ON_BN_CLICKED(IDC_RADIO_GRAPHIC_AVERAGE, &CDlgGraphic::OnBnClickedRadioGraphicAverage)
	ON_BN_CLICKED(IDC_BUTTON_COLORS, &CDlgGraphic::OnBnClickedButtonColors)
	ON_EN_CHANGE(IDC_EDIT_GRAPHIC_LINE, &CDlgGraphic::OnEnChangeEditGraphicLine)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_PROJECTS_SAME, &CDlgGraphic::OnBnClickedCheckGraphicProjectsSame)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, &CDlgGraphic::OnBnClickedCheckGraphicSingleMultiple)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_EXTENDED, &CDlgGraphic::OnBnClickedCheckGraphicExtended)

	ON_BN_CLICKED(ID_HELP, &CDlgGraphic::OnBnClickedHelp)
END_MESSAGE_MAP()


// CDlgGraphic message handlers

BOOL CDlgGraphic::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bDebugMode = false;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translation

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	this->SetWindowText(gszTranslation[PosDialogGraphicTitle]);

	m_textComputers.SetWindowText(gszTranslation[PosDialogGraphicTitleComputers]);
	m_textProjects.SetWindowText(gszTranslation[PosDialogGraphicTitleProjects]);
	m_checkSingleMultiple.SetWindowText(gszTranslation[PosDialogGraphicMultiple]);

	m_radioUser.SetWindowText(gszTranslation[PosDialogGraphicRadioUser]);
	m_radioTotal.SetWindowText(gszTranslation[PosDialogGraphicRadioTotal]);
	m_radioHost.SetWindowText(gszTranslation[PosDialogGraphicRadioComputer]);
	m_radioAverage.SetWindowText(gszTranslation[PosDialogGraphicRadioAverage]);

	m_buttonUpdate.SetWindowText(gszTranslation[PosDialogGraphicButtonUpdate]);

	m_textLineThickness.SetWindowText(gszTranslation[PosDialogGraphicLineThickness]);
	m_checkCombineProject.SetWindowText(gszTranslation[PosDialogGraphicCombineProject]);
	m_checkExpanded.SetWindowText(gszTranslation[PosDialogGraphicExpanded]);
// Translation

	m_iLineThickness = theApp.GetProfileInt(registrySectionGraphic, registryGraphicLineThickness, 2);
	if (m_iLineThickness < 1) m_iLineThickness = 1;
	CString sTxt;
	sTxt.Format("%d",m_iLineThickness);
	m_editLineThickness.SetWindowText(sTxt);

	int iUser = theApp.GetProfileInt(registrySectionGraphic, registryGraphicUserHost, GRAPHIC_USER);
	if (iUser == GRAPHIC_USER)	m_radioUser.SetCheck(TRUE);
	else						m_radioHost.SetCheck(TRUE);

	int iTotal = theApp.GetProfileInt(registrySectionGraphic, registryGraphicTotalAverage, GRAPHIC_TOTAL);
	if (iTotal == GRAPHIC_TOTAL)m_radioTotal.SetCheck(TRUE);
	else						m_radioAverage.SetCheck(TRUE);

	m_window.SetType(iUser, iTotal);

	m_iUserHost = iUser;
	m_iTotalAverage = iTotal;

	int iCheck = theApp.GetProfileInt(registrySectionGraphic, registryGraphicCombineProject, 0);
	m_bCheckCombineProjects = (iCheck != 0);
	m_checkCombineProject.SetCheck(m_bCheckCombineProjects);

	iCheck = theApp.GetProfileInt(registrySectionGraphic, registrySingleMultiple, 1);
	m_checkSingleMultiple.SetCheck(iCheck);
	m_listProjects.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);

	m_checkExpanded.SetCheck(m_iExpand);

//	m_bmp.LoadBitmap(IDB_BITMAP_COLORS);
//	m_colors.SetBitmap(m_bmp);

	m_iTimer = 0;			// Don't start
	m_iTimerEnd = 120;		// 2 minutes

	ResetProjects();

	m_textDebugMode.ShowWindow(SW_HIDE);

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicPosLeft, 0);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicPosTop, 0);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicPosRight, 800) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicPosBottom, 540) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER ); //| SWP_NOSIZE

	SetTimer(UTIMER_DLG_GRAPHIC, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgGraphic::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicPosBottom, wp.rcNormalPosition.bottom);
}

BOOL CDlgGraphic::PreTranslateMessage(MSG* pMsg)
{
	CString sMagic = "MAGIC";

	if(pMsg->message==WM_KEYDOWN)
	{
		m_sMagic += (char) pMsg->wParam;
		sMagic = sMagic.Left(m_sMagic.GetLength());
		if (m_sMagic == sMagic)
		{
			if (m_sMagic.GetLength() == 5)
			{
				if (m_bDebugMode)
				{
					m_textDebugMode.ShowWindow(SW_HIDE);
					m_bDebugMode = false;
				}
				else
				{
					m_textDebugMode.ShowWindow(SW_SHOW);
					m_bDebugMode = true;
				}
				m_sMagic = "";
			}
		}
		else
		{
			m_sMagic = "";
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgGraphic::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bFullScreenGraph)
	{
		CPoint pointWindow;
		pointWindow.x = point.x - 5;
		pointWindow.y = point.y - 5;
		m_window.OnMouseMove(nFlags, pointWindow);
	}
	else
	{
		if (m_iWindowInitialLeftMargin > 0)
		{
			if (point.x > m_iWindowInitialLeftMargin)
			{
				CPoint pointWindow;
				pointWindow.x = point.x - m_iWindowInitialLeftMargin;
				pointWindow.y = point.y - m_iWindowInitialTopMargin;
				m_window.OnMouseMove(nFlags, pointWindow);
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgGraphic::ResetProjects()
{
	CComputerGraphicListItem *pComputerGraphicListItem;

	while (m_lComputer.size() < theApp.m_lComputers.size())
	{
		pComputerGraphicListItem = new CComputerGraphicListItem;
		pComputerGraphicListItem->m_pcThreadRpcId = NULL;
		m_lComputer.push_back(pComputerGraphicListItem);
	}

	for (int iComputers = 0; iComputers < (int) theApp.m_lComputers.size(); iComputers++)
	{
		for (int iProjects = 0; iProjects < MAX_PROJECTS; iProjects++)
		{
			m_lComputer.at(iComputers)->m_pProjects[iProjects] = NULL;
			m_lComputer.at(iComputers)->m_sThreadRpcUrl[iProjects] = "";
			m_lComputer.at(iComputers)->m_sThreadRpcName[iProjects] = "";
		}
	}
}

void CDlgGraphic::CheckOneGraph()
{
	int iSelectionCount;

	m_bProjectsOneGraph = false;

	iSelectionCount = m_listComputers.GetSelCount();
	if (iSelectionCount == 1) 
	{
		if (m_bCheckCombineProjects)
		{
			m_bProjectsOneGraph = true;
		}
	}
}

void CDlgGraphic::SortWebItem(std::deque<CSelectedItem*> *plWebItem)
{
	CSelectedItem *pTemp;
	bool	bSorted = true;
	while (bSorted)
	{
		bSorted = false;
		for (int iCount=0; iCount < (int) plWebItem->size()-1; iCount++)
		{
			plWebItem->at(iCount)->m_sName.CompareNoCase(plWebItem->at(iCount+1)->m_sName);
			if (plWebItem->at(iCount)->m_sName.CompareNoCase(plWebItem->at(iCount+1)->m_sName) > 0)
			{
				pTemp = plWebItem->at(iCount);
				plWebItem->at(iCount) = plWebItem->at(iCount+1);
				plWebItem->at(iCount+1) = pTemp;
				bSorted = true;
			}
		}
	}
}



void CDlgGraphic::OnTimer(UINT_PTR nIDEvent)
{
	char cBufferLogging[128];

	if (m_bShowWindow)
	{
		if (m_iShowWindowDelay++ > 3)
		{
			ShowWindow(SW_SHOW);
			m_bShowWindow = false;
		}
	}

	if (!g_bWebServerActive && !g_bCloudServerAwake)
	{
		if (!this->IsWindowVisible()) return;
	}

	if (m_pDoc == NULL) return;
	if (m_pDoc->m_bDocumentIsClosing) return;			// everything is closing down.

	m_sLog.m_pFrom = "CDlgGraphic::OnTimerDlg";

//	if (m_listComputers.GetCount() == 0) m_iTimerEnd = 21;

	if (m_bInvalidate)
	{
		if (!m_bThreadBusy)			// when busy wait for the update, otherwise it may not be painted.
		{
			this->Invalidate(TRUE);
			m_bInvalidate = false;
		}
	}

	if (m_bThreadBusy)
	{
		if (m_iNrThreadReturn >= m_iNrThreadRpc)
		{
			if (m_bDebugMode)
			{
				_snprintf_s(cBufferLogging,127,_TRUNCATE,"m_window.Enable(true)");m_sLog.m_pText = cBufferLogging;
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
			}

			m_window.Enable(true, m_bDebugMode);
			UpdateGraphic();		// updates the window as well
//			m_bUpdateWindow = true;	// nono

			m_bThreadBusy = false;
		}
	}

	if (m_iTimer == 0)
	{
		if (!m_bThreadBusy)
		{
			if (m_bDebugMode)
			{
				_snprintf_s(cBufferLogging,127,_TRUNCATE,"m_window.Enable(false)");m_sLog.m_pText = cBufferLogging;
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
			}
			m_window.Enable(false, m_bDebugMode);
			m_bStartUpdateStatistics = true;
		}
	}

	if (!m_window.IsPainting())
	{
		if (m_bStartUpdateStatistics)
		{
			m_window.Enable(false, m_bDebugMode);
			ResetProjects();
			m_bStartUpdateStatistics = false;
			m_iTimer = 0;
			HWND hWnd = this->m_hWnd;
			m_iNrThreadReturn = 0;
			for (int iCount = 0; iCount < m_iNrThreadRpc ;iCount ++)
			{
				::PostThreadMessage(m_lComputer.at(iCount)->m_ThreadRpcId,UWM_MSG_THREAD_START_RPC_GRAPHIC, (WPARAM) this->m_hWnd,(LPARAM) 0);
				if (m_bDebugMode)
				{
					_snprintf_s(cBufferLogging,127,_TRUNCATE,"Request data from: %s", m_lComputer.at(iCount)->m_pcThreadRpcId);m_sLog.m_pText = cBufferLogging;
					theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
				}

				m_bThreadBusy = true;
//				m_bRunUpdateStatistics = true;
			}
		}
	}

	m_iTimer++;

	if (m_iTimer > m_iTimerEnd)
	{
//		m_iTimerEnd += 30;
//		if (m_iTimerEnd > 60) m_iTimerEnd = 60;

//		this->Invalidate(TRUE);
		m_iTimer = 0;
		m_bThreadBusy = false;			// has to be ready by now otherwise it's stuck

		if (m_bDebugMode)
		{
			m_sLog.m_pFrom = "CDlgGraphic::OnTimerDlg";
			m_sLog.m_pText = "Refresh";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
		}

	}
}

void CDlgGraphic::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

void CDlgGraphic::OnSizing(UINT fwSide, LPRECT pRect)
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

#define TOP_MARGIN	10
#define LEFT_MARGIN 220

void CDlgGraphic::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
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

	if (m_bFullScreenGraph)
	{
		lLeftMargin = 5;
		lTopMargin	= 5;
	}
	else
	{
		lLeftMargin= m_iWindowInitialLeftMargin;
		lTopMargin = m_iWindowInitialTopMargin;
	}

	lRightMargin	= 5;
	lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
	lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));

// position graphic relative to window NOT the screen

	if (m_window.m_hWnd != NULL)
	{
		m_window.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
	}

	m_bInvalidate = true;
}


LRESULT CDlgGraphic::OnInitialGraphic(WPARAM parm1, LPARAM parm2)
{
	char *pc;

	if (m_iNrThreadRpc > (int) theApp.m_lComputers.size()) return 0;

	pc = (char *) parm1;
	m_lComputer.at(m_iNrThreadRpc)->m_pcThreadRpcId =  pc;
	m_lComputer.at(m_iNrThreadRpc)->m_pThreadRpc  = (CThreadRpc *) parm2;
	m_lComputer.at(m_iNrThreadRpc)->m_ThreadRpcId = m_lComputer.at(m_iNrThreadRpc)->m_pThreadRpc->m_nThreadID;

	m_iNrThreadRpc++;

	// make sure the RPC is called again
	m_bStartUpdateStatistics = true;

	return 0;
}

// from every remote/local computer there will be one tp report here
LRESULT CDlgGraphic::OnGraphicsReady(WPARAM parm1, LPARAM parm2)
{
	char			cBufferLogging[255];
	int				iNrProjects, iStatus, iThreadId, iCount, iCountThread;
	CThreadRpc		*pRpc = NULL;
	PROJECT			*pProjectStatistics, *pProjectProject;
	char			*pProjectUrl, *pProjectName;
	CSelectedItem	*pSelectedItem;

	iStatus = (int) parm1&0xf ;
	iNrProjects	= (int) parm1;
	iNrProjects >>= 4;

	iThreadId = (int) parm2;

	if (iNrProjects == 0)
	{
		m_iNrThreadReturn++;
		return 0;
	}

	m_sLog.m_pFrom = "CDlgGraphic::OnGraphicsReady";

	// first check the thread

	for (iCountThread = 0; iCountThread < m_iNrThreadRpc; iCountThread++)
	{
		if (m_lComputer.at(iCountThread)->m_ThreadRpcId == iThreadId)
		{
			pRpc = m_lComputer.at(iCountThread)->m_pThreadRpc;

			if (m_listComputers.FindStringExact(0, m_lComputer.at(iCountThread)->m_pcThreadRpcId) ==  LB_ERR)	// check if already in the list
			{
				m_listComputers.AddString(m_lComputer.at(iCountThread)->m_pcThreadRpcId,iCountThread);			//  index start a 0 = computer 1;
		
				pSelectedItem = new CSelectedItem;
				pSelectedItem->m_sName = m_lComputer.at(iCountThread)->m_pcThreadRpcId;

				if (theApp.m_pMainFrame->m_iLocalHostThreadId == m_lComputer.at(iCountThread)->m_ThreadRpcId) pSelectedItem->m_bSelected = true;
				else pSelectedItem->m_bSelected = false;
				m_lWebComputers.push_back(pSelectedItem);
				SortWebItem(&m_lWebComputers);

				if (m_bDebugMode)
				{
					_snprintf_s(cBufferLogging,254,_TRUNCATE,"Computer: %s", m_lComputer.at(iCountThread)->m_pcThreadRpcId);m_sLog.m_pText = cBufferLogging;
					theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
				}
			}
			break;
		}
	}

	if (pRpc == NULL)
	{
		m_iNrThreadReturn++;
		return 0;
	}

	if (iNrProjects > MAX_PROJECTS) iNrProjects = MAX_PROJECTS;


	if (m_bDebugMode)
	{
		_snprintf_s(cBufferLogging,254,_TRUNCATE,"Computer: %s, Projects: %d",pRpc->m_pcComputerId, iNrProjects);m_sLog.m_pText = cBufferLogging;
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
	}

	for (iCount = 0; iCount < iNrProjects; iCount++)
	{
		pProjectStatistics = NULL;
		pProjectProject = NULL;
		pRpc->GetRpcStatistics(iCount,&pProjectProject, &pProjectStatistics );
		m_lComputer.at(iCountThread)->m_pProjects[iCount] = pProjectStatistics;

		if (pProjectStatistics != NULL) 
		{
			pProjectUrl = (char *) pProjectStatistics->master_url.c_str();
			m_lComputer.at(iCountThread)->m_sThreadRpcUrl[iCount] = pProjectUrl;
			pProjectName = (char *) pProjectProject->project_name.c_str();
			m_lComputer.at(iCountThread)->m_sThreadRpcName[iCount] = pProjectName;		
			if (pProjectName != NULL)
			{
				if (strlen(pProjectName) > 3)
				{
					bool bFound = false;
					for (int iWebProjects = 0; iWebProjects < (int) m_lWebProjects.size(); iWebProjects++)
					{
						if (m_lWebProjects.at(iWebProjects)->m_sName == pProjectName)
						{
							bFound = true;
						}

					}
					if (!bFound)
					{
						pSelectedItem = new CSelectedItem;
						pSelectedItem->m_sName = pProjectName;
						pSelectedItem->m_bSelected = true;
						m_lWebProjects.push_back(pSelectedItem);
						SortWebItem(&m_lWebProjects);
					}

					if (m_listProjects.FindStringExact(0,pProjectName) ==  LB_ERR)
					{
						m_listProjects.AddString(pProjectName);
						if (m_bDebugMode)
						{
							_snprintf_s(cBufferLogging,254,_TRUNCATE,"Project: %s", pProjectName);m_sLog.m_pText = cBufferLogging;
							theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
						}
					}
				}
			}
		}
	}

#ifdef GRAPHIC_TEST_XML
	// test
	pProjectStatistics = new PROJECT;
	AddTest(pProjectStatistics);
	m_lComputer.at(iCountThread)->m_pProjects[iCount] = pProjectStatistics;
	m_lComputer.at(iCountThread)->m_sThreadRpcUrl[iCount] = "http://setiathome.berkeley.edu/";
	m_lComputer.at(iCountThread)->m_sThreadRpcName[iCount] = "Test";
	m_listProjects.AddString("Test");
//	AddProject(sComputer, sProject, color, iRepeat);
#endif

	m_iNrThreadReturn++;
	return 0;
}

LRESULT CDlgGraphic::OnColorChanged(WPARAM parm1, LPARAM parm2)
{
	UpdateGraphic();

	return 0;
}

void CDlgGraphic::DisplayStatus(int iStatus)
{
}
void CDlgGraphic::OnBnClickedButtonUpdate()
{
	m_listProjects.ResetContent();
	m_listComputers.ResetContent();

	UpdateGraphic();

	// make sure the RPC is called again
	m_bStartUpdateStatistics = true;
//	m_bRunUpdateStatistics = false;	
}

void CDlgGraphic::UpdateGraphic()
{
	int iItemsComputers, iItemsProjects;
	int iRepeat;
	CString		sComputer, sProject;
	int			iComputers, iProjects;
	COLORREF	color;
	int			iColorSelected, iColorIndex;
	bool		bFixedColors;

	iColorIndex = 0;
	iColorSelected = 0;

	iItemsComputers = m_listComputers.GetCount();
	iItemsProjects  = m_listProjects.GetCount();

	bFixedColors = theApp.m_pDlgColorGraphic->GetFixedColors();

	if (m_bProjectsOneGraph)
	{
		m_listComputers.Reset();
		m_listComputers.Invalidate();

		// set colors the 1e selected = computer 1;
		for (int iCount = 0; iCount < iItemsProjects; iCount++)
		{
			if (m_listProjects.GetSel(iCount) > 0)
			{
				color = theApp.m_pDlgColorGraphic->GetColorComputer(iColorIndex, &iRepeat);
				m_listProjects.SetColor(color, iRepeat, iCount, iColorSelected);	// only set computer color 0 = computer 1
				iColorSelected++;
				iColorIndex++;
			}
			else
			{
				if (bFixedColors) iColorIndex++;
			}
		}


	}
	else
	{
		m_listProjects.Reset();
		m_listProjects.Invalidate();

		// set colors the 1e selected = computer 1;
		for (int iCount = 0; iCount < iItemsComputers; iCount++)
		{
			if (m_listComputers.GetSel(iCount) > 0)
			{
				color = theApp.m_pDlgColorGraphic->GetColorComputer(iColorIndex, &iRepeat);
				m_listComputers.SetColor(color, iRepeat, iCount, iColorIndex);	// only set computer color 0 = computer 1
				iColorSelected++;
				iColorIndex++;
			}
			else
			{
				if (bFixedColors) iColorIndex++;
			}
		}

	}

	m_listProjects.Invalidate();	// colors changed
	m_listComputers.Invalidate();	// colors changed

	m_window.ResetProjects();

	iComputers = 0;
	iProjects = 0;

	for (int iCountComputers = 0; iCountComputers < iItemsComputers; iCountComputers++)
	{
		if (m_listComputers.GetSel(iCountComputers) > 0) 
		{
			sComputer = "";
			m_listComputers.GetText(iCountComputers,sComputer);
			if (bFixedColors)
			{
				color = m_listComputers.GetColor(iCountComputers, &iRepeat);
			}
			else
			{
				color = m_listComputers.GetColor(iComputers, &iRepeat);
			}
			iComputers++;
			iProjects = 0;
			for (int iCountProjects = 0; iCountProjects < iItemsProjects; iCountProjects++)
			{
				if (m_listProjects.GetSel(iCountProjects) > 0)
				{
					sProject = "";
					m_listProjects.GetText(iCountProjects,sProject);

					if (m_bProjectsOneGraph)
					{
						color = m_listProjects.GetColor(iProjects, &iRepeat);
					}

					AddProject(sComputer, sProject,color, iRepeat);
//					pProject = m_pProjects[iCountComputers][iCountProjects];
//					m_window.AddProject(sComputer,sProject,iCountComputers,pProject,color);
					iProjects++;
				}
			}
		}
	}
	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_WINDOW_LINE, m_iLineThickness, 0);
	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_PROJECTS_ONE, m_bProjectsOneGraph,0);
	m_window.SetColors(theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_BACKGROUND), theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_PROJECT_TEXT), theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_SCALE_TEXT), theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_GRID));
	m_window.ShowProjects(iComputers, iProjects);
}

void CDlgGraphic::AddProject(CString sComputer, CString sProject, COLORREF color, int iRepeat)
{
	PROJECT		*pProject;
	CString		sProjectUrl;

	for (int iCountComputer = 0; iCountComputer < m_iNrThreadRpc; iCountComputer++)
	{
		if (sComputer ==  m_lComputer.at(iCountComputer)->m_pcThreadRpcId)
		{
			// computer found
			for (int iCountProject = 0; iCountProject < MAX_PROJECTS; iCountProject++)
			{
				if (sProject == m_lComputer.at(iCountComputer)->m_sThreadRpcName[iCountProject])
				{
					pProject = m_lComputer.at(iCountComputer)->m_pProjects[iCountProject];
					sProjectUrl = m_lComputer.at(iCountComputer)->m_sThreadRpcUrl[iCountProject];
					if (pProject != NULL)
					{
						if (sProjectUrl == pProject->master_url.c_str())
						{
							m_window.AddProject(sComputer,sProject, pProject,color, iRepeat);
						}
					}
				}
			}
		}
	}
}

void CDlgGraphic::OnLbnSelchangeListGraphicComputers()
{
	CheckOneGraph();
	UpdateGraphic();
}

void CDlgGraphic::OnLbnSelchangeListGraphicProjects()
{
	UpdateGraphic();
}

void CDlgGraphic::OnBnClickedRadioGraphicUser()
{
	m_radioUser.SetCheck(TRUE);
	m_radioHost.SetCheck(FALSE);

	theApp.WriteProfileInt(registrySectionGraphic, registryGraphicUserHost, GRAPHIC_USER);

	m_iUserHost = GRAPHIC_USER;
	m_window.SetType(m_iUserHost, m_iTotalAverage);
	UpdateGraphic();
}

void CDlgGraphic::OnBnClickedRadioGraphicHost()
{
	m_radioHost.SetCheck(TRUE);
	m_radioUser.SetCheck(FALSE);

	theApp.WriteProfileInt(registrySectionGraphic, registryGraphicUserHost, GRAPHIC_HOST);

	m_iUserHost = GRAPHIC_HOST;
	m_window.SetType(m_iUserHost, m_iTotalAverage);
	UpdateGraphic();
}

void CDlgGraphic::OnBnClickedRadioGraphicTotal()
{
	m_radioTotal.SetCheck(TRUE);
	m_radioAverage.SetCheck(FALSE);

	theApp.WriteProfileInt(registrySectionGraphic, registryGraphicTotalAverage, GRAPHIC_TOTAL);

	m_iTotalAverage = GRAPHIC_TOTAL;
	m_window.SetType(m_iUserHost, m_iTotalAverage);
	UpdateGraphic();
}

void CDlgGraphic::OnBnClickedRadioGraphicAverage()
{
	m_radioTotal.SetCheck(FALSE);
	m_radioAverage.SetCheck(TRUE);

	theApp.WriteProfileInt(registrySectionGraphic, registryGraphicTotalAverage, GRAPHIC_AVERAGE);

	m_iTotalAverage = GRAPHIC_AVERAGE;
	m_window.SetType(m_iUserHost, m_iTotalAverage);
	UpdateGraphic();
}

void CDlgGraphic::OnBnClickedButtonColors()
{
	theApp.m_pDlgColorGraphic->ShowWindow(SW_SHOW);
}

void CDlgGraphic::OnShowWindow(BOOL bShow, UINT nStatus)
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CWnd::OnShowWindow(bShow, nStatus);
	m_bStartUpdateStatistics = true;			// make sure the RPC is called again
}
void CDlgGraphic::OnEnChangeEditGraphicLine()
{
	CString sTxt;

	m_editLineThickness.GetWindowText(sTxt);
	m_iLineThickness = atoi(sTxt);
	if (m_iLineThickness < 1)
	{
		m_iLineThickness = 1;
	}
	if (m_iLineThickness > 50)
	{
		m_iLineThickness = 50;
	}
	theApp.WriteProfileInt(registrySectionGraphic, registryGraphicLineThickness, m_iLineThickness);

	UpdateGraphic();
}


void CDlgGraphic::OnBnClickedCheckGraphicProjectsSame()
{
	int iCheck;

	iCheck = m_checkCombineProject.GetCheck();

	m_bCheckCombineProjects = (iCheck != 0);

	theApp.WriteProfileInt(registrySectionGraphic, registryGraphicCombineProject, iCheck);

	CheckOneGraph();
	UpdateGraphic();
}

void CDlgGraphic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bFullScreenGraph = !m_bFullScreenGraph;

	SetWindowPosition(m_lLastWidth, m_lLastHeight, 0);
	Invalidate(TRUE);
	ShowControls(!m_bFullScreenGraph);

}

void CDlgGraphic::OnBnClickedCheckGraphicSingleMultiple()
{
	int		iCheck;

	iCheck = m_checkSingleMultiple.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphic, registrySingleMultiple, iCheck);

	m_listProjects.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
}

void CDlgGraphic::OnBnClickedCheckGraphicExtended()
{
	int		iCheck;

	iCheck = m_checkExpanded.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphic, registryExpanded, iCheck);

	PostCommandView(UWM_RESTART_GRAPHIC);
//	m_checkExpanded.EnableWindow(FALSE);
}

LRESULT CDlgGraphic::GetGraphic(WPARAM parm1, LPARAM parm2)
{
	int iOperation;
	char *pcName;

	iOperation = (int) parm1;
	pcName = (char *) parm2;

	switch(iOperation)
	{
		case GRAPHIC_GET_LIST:
			return (LRESULT) &m_lComputer;
		break;
		case GRAPHIC_GET_COMPUTER:
			return (LRESULT) &m_lWebComputers;
		break;
		case GRAPHIC_GET_PROJECT:
			return (LRESULT) &m_lWebProjects;
		break;
		case GRAPHIC_GET_PROJECT_MODE:
			if (m_bWebProjectsSingle) return TRUE;
			else return FALSE;
		break;
		case GRAPHIC_SET_PROJECT_MODE:
			m_bWebProjectsSingle = !m_bWebProjectsSingle;
		break;
		case GRAPHIC_GET_TYPE:
			return (LRESULT) &m_webType;
		break;
		case GRAPHIC_SELECT_COMPUTER:
			for (int iCount = 0; iCount < (int) m_lWebComputers.size(); iCount++)
			{
				if (m_lWebComputers.at(iCount)->m_sName == pcName)
				{
					m_lWebComputers.at(iCount)->m_bSelected = ! m_lWebComputers.at(iCount)->m_bSelected;
					return NULL;
				}
			}
		break;
		case GRAPHIC_SELECT_PROJECT:
			if (m_bWebProjectsSingle) 
			{
				for (int iCount = 0; iCount < (int) m_lWebProjects.size(); iCount++)
				{
					m_lWebProjects.at(iCount)->m_bSelected = false;
				}
			}

			for (int iCount = 0; iCount < (int) m_lWebProjects.size(); iCount++)
			{
				if (m_lWebProjects.at(iCount)->m_sName == pcName)
				{
					m_lWebProjects.at(iCount)->m_bSelected = ! m_lWebProjects.at(iCount)->m_bSelected;
					return NULL;
				}
			}
		break;
		case GRAPHIC_SET_TYPE:
			if (strcmp(pcName,GRAPHIC_TYPE_USER) == 0) m_webType.m_iHostOrUser = GRAPHIC_USER;
			if (strcmp(pcName,GRAPHIC_TYPE_COMPUTER) == 0) m_webType.m_iHostOrUser = GRAPHIC_HOST;
			if (strcmp(pcName,GRAPHIC_TYPE_TOTAL) == 0) m_webType.m_iTotalOrAverage = GRAPHIC_TOTAL;
			if (strcmp(pcName,GRAPHIC_TYPE_AVERATE) == 0) m_webType.m_iTotalOrAverage = GRAPHIC_AVERAGE;
		break;
	}

	return NULL;
}


BOOL CDlgGraphic::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgGraphic::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks-graphs/#statistics";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}
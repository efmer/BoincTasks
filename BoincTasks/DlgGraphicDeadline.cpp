// DlgGraphicDeadline.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"
#include "DlgColorMain.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "RemoteCombinedDoc.h"
#include "DlgGraphicDeadline.h"
#include "afxdialogex.h"
#include "DlgProperties.h"
#include "DlgSettingsView.h"
#include "History.h"
#include "Translation.h"


// CDlgGraphicDeadline dialog

IMPLEMENT_DYNAMIC(CDlgGraphicDeadline, CDialog)

CDlgGraphicDeadline::CDlgGraphicDeadline(CWnd* pParent /*=NULL*/)	: CDialog(CDlgGraphicDeadline::IDD, pParent)
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
	m_bClosing = false;
	m_bClosingReady = false;
	m_bShowWindow = false;
	m_iShowWindowDelay = 0;
	m_bTimerActive = false;

	CComputerGraphicDeadlineListItem *pComputerGraphicDeadlineListItem;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pComputerGraphicDeadlineListItem = new CComputerGraphicDeadlineListItem;
		pComputerGraphicDeadlineListItem->m_pcThreadRpcId = NULL;
		pComputerGraphicDeadlineListItem->m_pThreadRpc = NULL;
		pComputerGraphicDeadlineListItem->m_plHistoryProjectDeadline = new std::deque<CHistoryProjectDeadlineItem*>;
		m_lComputer.push_back(pComputerGraphicDeadlineListItem);
	}
}

CDlgGraphicDeadline::~CDlgGraphicDeadline()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
	for (int iCount = 0; iCount < (int) m_lComputer.size(); iCount++)
	{
		for (int iCount2 = 0; iCount2 < (int) m_lComputer.at(iCount)->m_plHistoryProjectDeadline->size(); iCount2++)
		{
			delete m_lComputer.at(iCount)->m_plHistoryProjectDeadline->at(iCount2);
		}
		delete m_lComputer.at(iCount)->m_plHistoryProjectDeadline;
		delete m_lComputer.at(iCount);
	}

}

void CDlgGraphicDeadline::InitialUpdate(CRemoteCombinedDoc *pDoc, bool bShowWindow)
{
	m_bShowWindow = bShowWindow;
	m_iShowWindowDelay = 0;
	m_pDoc = pDoc;
	if (bShowWindow)
	{
		if (!m_bTimerActive)
		{
			SetTimer(UTIMER_DLG_GRAPHIC_DEADLINE, 1000, 0);
			m_bTimerActive = true;
		}
	}
}

void CDlgGraphicDeadline::ShowControls(bool bShow)
{
	int iShow;

	if (bShow) iShow = SW_SHOW;
	else iShow = SW_HIDE;

	m_buttonUpdate.ShowWindow(iShow);
	m_checkCombineProject.ShowWindow(iShow);

	m_checkPeriod.ShowWindow(iShow);
	m_editPeriod.ShowWindow(iShow);
	m_textPeriod.ShowWindow(iShow);

	m_editLineThickness.ShowWindow(iShow);

	m_textLineThickness.ShowWindow(iShow);
	m_textComputers.ShowWindow(iShow);
	m_textProjects.ShowWindow(iShow);

	m_listProjects.ShowWindow(iShow);
	m_listComputers.ShowWindow(iShow);

	m_checkSingleMultiple.ShowWindow(iShow);
	m_checkModeBlock.ShowWindow(iShow);
	m_checkAverage.ShowWindow(iShow);
	m_checkExpanded.ShowWindow(iShow);
	m_checkCpu.ShowWindow(iShow);
	m_checkGpu.ShowWindow(iShow);

	m_buttonHelp.ShowWindow(iShow);
}

void CDlgGraphicDeadline::PostCommandView(int iCommand)
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

void CDlgGraphicDeadline::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_GRAPHIC, m_window);
	DDX_Control(pDX, IDC_LIST_GRAPHIC_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_LIST_GRAPHIC_PROJECTS, m_listProjects);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_GROUP_COMPUTERS, m_textComputers);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_GROUP_PROJECTS, m_textProjects);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_buttonUpdate);
	DDX_Control(pDX, IDC_EDIT_GRAPHIC_LINE, m_editLineThickness);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_LINE_THICKNESS, m_textLineThickness);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_PROJECTS_SAME, m_checkCombineProject);
	DDX_Control(pDX, IDC_CHECK_GRAPH_PERIOD, m_checkPeriod);
	DDX_Control(pDX, IDC_EDIT_GRAPHIC_PERIOD, m_editPeriod);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_PERIOD, m_textPeriod);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_BLOCK, m_checkModeBlock);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, m_checkSingleMultiple);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_AVERAGE, m_checkAverage);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_EXTENDED2, m_checkExpanded);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_CPU, m_checkCpu);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_GPU, m_checkGpu);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(CDlgGraphicDeadline, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_HELPINFO()

	ON_LBN_SELCHANGE(IDC_LIST_GRAPHIC_COMPUTERS, &CDlgGraphicDeadline::OnLbnSelchangeListGraphicComputers)
	ON_LBN_SELCHANGE(IDC_LIST_GRAPHIC_PROJECTS, &CDlgGraphicDeadline::OnLbnSelchangeListGraphicProjects)

	ON_MESSAGE(UWM_INITIAL_GRAPHIC,OnInitialGraphic)

	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_GRAPHIC_DEADLINE, OnGraphicsDeadlineReady)

	ON_MESSAGE(UWM_GRAPHIC_COLOR_CHANGED, OnColorChanged)

	ON_MESSAGE(UWM_CLOSE_GRAPHIC, OnCloseGraphic)

	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgGraphicDeadline::OnBnClickedButtonUpdate)
	ON_EN_CHANGE(IDC_EDIT_GRAPHIC_LINE, &CDlgGraphicDeadline::OnEnChangeEditGraphicLine)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_PROJECTS_SAME, &CDlgGraphicDeadline::OnBnClickedCheckGraphicProjectsSame)
	ON_BN_CLICKED(IDC_CHECK_GRAPH_PERIOD, &CDlgGraphicDeadline::OnBnClickedCheckGraphPeriod)
	ON_EN_CHANGE(IDC_EDIT_GRAPHIC_PERIOD, &CDlgGraphicDeadline::OnEnChangeEditGraphicPeriod)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_BLOCK, &CDlgGraphicDeadline::OnBnClickedCheckGraphicBlock)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, &CDlgGraphicDeadline::OnBnClickedCheckGraphicSingleMultiple)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_AVERAGE, &CDlgGraphicDeadline::OnBnClickedCheckGraphicAverage)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_EXTENDED2, &CDlgGraphicDeadline::OnBnClickedCheckGraphicExtended2)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_CPU, &CDlgGraphicDeadline::OnBnClickedCheckGraphicCpu)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_GPU, &CDlgGraphicDeadline::OnBnClickedCheckGraphicGpu)
	ON_BN_CLICKED(ID_HELP, &CDlgGraphicDeadline::OnBnClickedHelp)
END_MESSAGE_MAP()


// CDlgGraphicDeadline message handlers

BOOL CDlgGraphicDeadline::OnInitDialog()
{
	int iCheck;

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translation

	this->SetWindowText(gszTranslation[PosDialogGraphicDeadlineTitle]);

	m_textComputers.SetWindowText(gszTranslation[PosDialogGraphicTitleComputers]);
	m_textProjects.SetWindowText(gszTranslation[PosDialogGraphicTitleProjects]);
	m_checkSingleMultiple.SetWindowText(gszTranslation[PosDialogGraphicMultiple]);
	m_checkAverage.SetWindowText(gszTranslation[PosDialogGraphicAverage]);

	m_buttonUpdate.SetWindowText(gszTranslation[PosDialogGraphicButtonUpdate]);

	m_textLineThickness.SetWindowText(gszTranslation[PosDialogGraphicLineThickness]);
	m_checkCombineProject.SetWindowText(gszTranslation[PosDialogGraphicCombineProject]);

	m_checkPeriod.SetWindowText(gszTranslation[PosDialogGraphicPeriod]);
	m_textPeriod.SetWindowText(gszTranslation[PosDialogGraphicPeriodDays]);
	m_checkExpanded.SetWindowText(gszTranslation[PosDialogGraphicExpanded]);

// Translation

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	m_iLineThickness = theApp.GetProfileInt(registrySectionGraphicDeadline, registryGraphicLineThickness, 2);
	if (m_iLineThickness < 1) m_iLineThickness = 1;
	CString sTxt;
	sTxt.Format("%d",m_iLineThickness);
	m_editLineThickness.SetWindowText(sTxt);

	iCheck = theApp.GetProfileInt(registrySectionGraphicDeadline, registryGraphicCombineProject, 0);
	m_bCheckCombineProjects = (iCheck != 0);
	m_checkCombineProject.SetCheck(m_bCheckCombineProjects);

	iCheck = theApp.GetProfileInt(registrySectionGraphicDeadline, registryBlockMode, 1);
	m_bBlockMode = (iCheck != 0);
	m_checkModeBlock.SetCheck(m_bBlockMode);

	iCheck = theApp.GetProfileInt(registrySectionGraphicDeadline, registrySingleMultiple, 1);
	m_checkSingleMultiple.SetCheck(iCheck);
	m_listProjects.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);

	iCheck = theApp.GetProfileInt(registrySectionGraphicDeadline, registryAverage, 0);
	m_bCheckAverage = (iCheck != 0);
	m_checkAverage.SetCheck(m_bCheckAverage);

	iCheck = theApp.GetProfileInt(registrySectionGraphicDeadline, registryGraphicPeriod, 0);
	m_bCheckPeriod = (iCheck != 0);
	m_checkPeriod.SetCheck(m_bCheckPeriod);
	if (m_bCheckPeriod)
	{
		m_editPeriod.ShowWindow(SW_SHOW);
		m_textPeriod.ShowWindow(SW_SHOW);
	}
	else
	{
		m_editPeriod.ShowWindow(SW_HIDE);
		m_textPeriod.ShowWindow(SW_HIDE);
	}

	m_iPeriod = theApp.GetProfileInt(registrySectionGraphicDeadline, registryGraphicPeriodValue, 1);
	if (m_iPeriod < 1) m_iPeriod = 1;
	sTxt.Format("%d",m_iPeriod);
	m_editPeriod.SetWindowText(sTxt);

	iCheck = theApp.GetProfileInt(registrySectionGraphicDeadline, registryCpu, 1);
	m_checkCpu.SetCheck(iCheck);
	m_bCpu = (iCheck != 0);
	iCheck = theApp.GetProfileInt(registrySectionGraphicDeadline, registryGpu, 1);
	m_checkGpu.SetCheck(iCheck);
	m_bGpu = (iCheck != 0);

	m_bAdjustTime = theApp.m_pDlgSettingsView->m_bAdjustTime;

	m_iTimerEnd = 120;			// 2 minutes
	m_iTimer = m_iTimerEnd-10;	// wait for 10 seconds

	ResetProjects();

	m_checkExpanded.SetCheck(m_iExpand);

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosLeft, 0);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosTop, 0);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosRight, 800) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosBottom, 540) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER ); //| SWP_NOSIZE

//	SetTimer(UTIMER_DLG_GRAPHIC_DEADLINE, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgGraphicDeadline::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicDeadlinePosBottom, wp.rcNormalPosition.bottom);
}

LRESULT CDlgGraphicDeadline::OnCloseGraphic(WPARAM parm1, LPARAM parm2)
{
	m_bClosing = true;
	m_bClosingReady = false;
	return 0;
}

BOOL CDlgGraphicDeadline::PreTranslateMessage(MSG* pMsg)
{
	CString sMagic = "MAGIC";

	if(pMsg->message==WM_KEYDOWN)
	{
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgGraphicDeadline::ResetProjects()
{
//	for (int iComputers = 0; iComputers < MAX_COMPUTERS_REMOTE; iComputers++)
//	{
//		for (int iProjects = 0; iProjects < MAX_PROJECTS; iProjects++)
//		{
//			m_pProjects[iComputers][iProjects] = NULL;
//			m_sThreadRpcUrl[iComputers][iProjects] = "";
//			m_sThreadRpcName[iComputers][iProjects] = "";
//		}
//	}
}

void CDlgGraphicDeadline::CheckOneGraph()
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


void CDlgGraphicDeadline::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bShowWindow)
	{
		if (m_iShowWindowDelay++ > 3)
		{
			ShowWindow(SW_SHOW);
			m_bShowWindow = false;
		}
		return;
	}

	if (!this->IsWindowVisible())
	{
		if (m_bTimerActive)
		{
			KillTimer(UTIMER_DLG_GRAPHIC_DEADLINE);
			m_bTimerActive = false;
		}
		return;
	}
	else
	{
		if (theApp.m_pDlgSettingsView->m_bAdjustTime != m_bAdjustTime)\
		{
			m_bAdjustTime = theApp.m_pDlgSettingsView->m_bAdjustTime;
			m_bStartUpdateStatistics = true;	// action when the check changes.
		}
	}

	if (m_pDoc == NULL) return;
	if (m_pDoc->m_bDocumentIsClosing) return;			// everything is closing down.

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
			m_window.Enable(true, false);
			UpdateGraphic();		// updates the window as well
			m_bThreadBusy = false;
		}
	}
	else
	{
		if (m_bClosing)
		{
			if (!m_bClosingReady)
			{
				m_bClosingReady = true;
				PostCommandView(UWM_RESTART_GRAPHIC_DEADLINE_READY);
				return;
			}
		}
	}

	if (m_iTimer == 0)
	{
		if (!m_bThreadBusy)
		{
			m_window.Enable(false, false);
			m_bStartUpdateStatistics = true;	// make sure the RPC is called again
		}
	}

	if (!m_window.IsPainting())
	{
		if (m_bStartUpdateStatistics && !m_bClosing && !m_bClosingReady)
		{
			m_window.Enable(false, false);
			Sleep(100);
			if (!m_window.IsPainting())
			{
				int iPeriod;
				if (m_bCheckPeriod) iPeriod = m_iPeriod*24;
				else iPeriod = -1;

				ResetProjects();
				m_bStartUpdateStatistics = false;
				m_iTimer = 0;
				HWND hWnd = this->m_hWnd;
				m_iNrThreadReturn = 0;
				for (int iCount = 0; iCount < m_iNrThreadRpc ;iCount ++)
				{
					::PostThreadMessage(m_lComputer.at(iCount)->m_ThreadRpcId,UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE_P, (WPARAM) this->m_hWnd,(LPARAM) iPeriod);
					m_lComputer.at(iCount)->m_bAdjustTime = theApp.m_pDlgSettingsView->m_bAdjustTime;
					::PostThreadMessage(m_lComputer.at(iCount)->m_ThreadRpcId,UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE, (WPARAM) this->m_hWnd,(LPARAM) m_lComputer.at(iCount));
					m_bThreadBusy = true;
				}
			}
		}
	}

	m_iTimer++;

	if (m_iTimer > m_iTimerEnd)
	{
//		m_iTimerEnd += 30;							// add 30 seconds a time
		if (m_iTimerEnd > 120) m_iTimerEnd = 120;

		m_iTimer = 0;
		m_bThreadBusy = false;			// has to be ready by now otherwise it's stuck

	}
}

void CDlgGraphicDeadline::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
//	this->Invalidate(TRUE);
}

void CDlgGraphicDeadline::OnSizing(UINT fwSide, LPRECT pRect)
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

void CDlgGraphicDeadline::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
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


LRESULT CDlgGraphicDeadline::OnInitialGraphic(WPARAM parm1, LPARAM parm2)
{
	char *pc;

	if (m_iNrThreadRpc > (int) theApp.m_lComputers.size()) return 0;

	pc = (char *) parm1;
	m_lComputer.at(m_iNrThreadRpc)->m_pcThreadRpcId =  pc;
	m_lComputer.at(m_iNrThreadRpc)->m_pThreadRpc  = (CThreadRpc *) parm2;
	m_lComputer.at(m_iNrThreadRpc)->m_ThreadRpcId = m_lComputer.at(m_iNrThreadRpc)->m_pThreadRpc->m_nThreadID;

	m_iNrThreadRpc++;

	m_bStartUpdateStatistics = true;	// make sure the RPC is called again

	return 0;
}

// from every remote/local computer there will be one tp report here
LRESULT CDlgGraphicDeadline::OnGraphicsDeadlineReady(WPARAM parm1, LPARAM parm2)
{
	int		iPos;
	int iResult;
	CRpcThreadReturn * pThreadReturn;
	std::deque<CHistoryProjectDeadlineItem*> *plHistoryProjectDeadline;

	CHistoryProjectDeadlineItem	*pHistoryProjectDeadlineItem;
	char *pcProjectName;

	iResult = (int) parm1;
	pThreadReturn = (CRpcThreadReturn *) parm2;

	m_iNrThreadReturn++;

	if (pThreadReturn->m_iStatus != RPC_STATUS_CONNECTED)
	{
		delete pThreadReturn;
		return 0;
	}

	// first check the thread

	if (m_listComputers.FindStringExact(0,pThreadReturn->m_pcComputerId) ==  LB_ERR)	// check if already in the list
	{
		m_listComputers.AddString(pThreadReturn->m_pcComputerId,0);	//  index start a 0 = computer 1;
	}

	plHistoryProjectDeadline = NULL;

	for (int iCountThread = 0; iCountThread < m_iNrThreadRpc; iCountThread++)
	{
		if (m_lComputer.at(iCountThread)->m_ThreadRpcId == pThreadReturn->m_iThreadId)
		{
			plHistoryProjectDeadline = m_lComputer.at(iCountThread)->m_plHistoryProjectDeadline;
			break;
		}
	}

	if (plHistoryProjectDeadline)
	{
		for (iPos = 0; iPos < (int) plHistoryProjectDeadline->size(); iPos++)
		{
			pHistoryProjectDeadlineItem = plHistoryProjectDeadline->at(iPos);
			pcProjectName = pHistoryProjectDeadlineItem->m_pcProject;
			if (m_listProjects.FindStringExact(0,pcProjectName) ==  LB_ERR)
			{
				if (strcmp(TEXT_STATE_UPDATING, pcProjectName) != 0)			// not the updating... text
				{
					m_listProjects.AddString(pcProjectName);
				}
			}
		}
	}

	delete pThreadReturn;

	return 0;
}

LRESULT CDlgGraphicDeadline::OnColorChanged(WPARAM parm1, LPARAM parm2)
{
	UpdateGraphic();

	return 0;
}

void CDlgGraphicDeadline::OnShowWindow(BOOL bShow, UINT nStatus)
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CWnd::OnShowWindow(bShow, nStatus);
	m_bStartUpdateStatistics = true;			// make sure the RPC is called again

	if (bShow)
	{
		if (!m_bTimerActive)
		{
			SetTimer(UTIMER_DLG_GRAPHIC_DEADLINE, 1000, 0);
			m_bTimerActive = true;
		}
	}
}

void CDlgGraphicDeadline::OnEnChangeEditGraphicLine()
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
	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryGraphicLineThickness, m_iLineThickness);

	UpdateGraphic();
}

void CDlgGraphicDeadline::OnBnClickedButtonUpdate()
{
	m_listProjects.ResetContent();
	m_listComputers.ResetContent();

	UpdateGraphic();


	m_bStartUpdateStatistics = true;	// make sure the RPC is called again
//	m_bRunUpdateStatistics = false;	
}

void CDlgGraphicDeadline::UpdateGraphic()
{
	int iItemsComputers, iItemsProjects;
//	PROJECT		*pProject;
	CString		sComputer, sProject;
	int			iComputers, iProjects;
	COLORREF	color;
	int			iColorIndex,iColorSelected;
	bool		bFixedColors;

	CDlgColorGraphic *pDlgColorGraphic;
	pDlgColorGraphic = theApp.m_pDlgColorGraphic;

	iColorIndex = 0;
	iColorSelected = 0;

	bFixedColors = theApp.m_pDlgColorGraphic->GetFixedColors();

	if (m_bProjectsOneGraph)
	{
		m_listComputers.Reset();
		m_listComputers.Invalidate();

		// set colors the 1e selected = computer 1;
		for (int iCount = 0; iCount < COLOR_ARRAY_G_NUMBER_COMPUTERS; iCount++)
		{
			if (m_listProjects.GetSel(iCount) > 0)
			{
				m_listProjects.SetColor(pDlgColorGraphic->GetColor(COLOR_ARRAY_COMPUTER1+ iColorIndex), 0, iCount, iColorSelected);	// only set computer color 0 = computer 1
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
		for (int iCount = 0; iCount < COLOR_ARRAY_G_NUMBER_COMPUTERS; iCount++)
		{
			if (m_listComputers.GetSel(iCount) > 0)
			{
				m_listComputers.SetColor(pDlgColorGraphic->GetColor(COLOR_ARRAY_COMPUTER1+ iColorIndex), 0, iCount, iColorSelected);	// only set computer color 0 = computer 1
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

	iItemsComputers = m_listComputers.GetCount();
	iItemsProjects  = m_listProjects.GetCount();

	m_window.ResetProjects();

	iComputers = 0;
	iProjects = 0;

	for (int iCountComputers = 0; iCountComputers < iItemsComputers; iCountComputers++)
	{
		if (m_listComputers.GetSel(iCountComputers) > 0) 
		{
			sComputer = "";
			m_listComputers.GetText(iCountComputers,sComputer);
			color = m_listComputers.GetColor(iComputers);
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
						color = m_listProjects.GetColor(iProjects);
					}

					AddProject(sComputer, sProject,color);
					iProjects++;
				}
			}
		}
	}

	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_WINDOW_LINE, m_iLineThickness, m_bBlockMode);
	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_PROJECTS_ONE, m_bProjectsOneGraph,0);

	int iCpuGpu = 0;
	if (m_bCpu) iCpuGpu += 1;
	if (m_bGpu) iCpuGpu += 2;
	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_AVERAGE, m_bCheckAverage,iCpuGpu);

	m_window.SetColors(pDlgColorGraphic->GetColor(COLOR_ARRAY_BACKGROUND), pDlgColorGraphic->GetColor(COLOR_ARRAY_PROJECT_TEXT), pDlgColorGraphic->GetColor(COLOR_ARRAY_SCALE_TEXT), pDlgColorGraphic->GetColor(COLOR_ARRAY_GRID), pDlgColorGraphic->GetColor(COLOR_ARRAY_AVERAGE));
	m_window.ShowProjects(iComputers, iProjects);
}

void CDlgGraphicDeadline::AddProject(CString sComputer, CString sProject, COLORREF color)
{
	int		iPos;
	CHistoryProjectDeadlineItem	*pHistoryProjectDeadlineItem;
	CString		sProjectUrl;

	for (int iCountComputer = 0; iCountComputer < m_iNrThreadRpc; iCountComputer++)
	{
		if (sComputer ==  m_lComputer.at(iCountComputer)->m_pcThreadRpcId)
		{
			// computer found
			for (iPos = 0; iPos < (int) m_lComputer.at(iCountComputer)->m_plHistoryProjectDeadline->size(); iPos++)
			{
				pHistoryProjectDeadlineItem = m_lComputer.at(iCountComputer)->m_plHistoryProjectDeadline->at(iPos);
				if (sProject == pHistoryProjectDeadlineItem->m_pcProject)
				{
					m_window.AddProject(sComputer,pHistoryProjectDeadlineItem,color);
				}
			}
		}
	}
}

void CDlgGraphicDeadline::OnLbnSelchangeListGraphicComputers()
{
	CheckOneGraph();
	UpdateGraphic();
}

void CDlgGraphicDeadline::OnLbnSelchangeListGraphicProjects()
{
	UpdateGraphic();
}

void CDlgGraphicDeadline::OnBnClickedCheckGraphicProjectsSame()
{
	int iCheck;

	iCheck = m_checkCombineProject.GetCheck();

	m_bCheckCombineProjects = (iCheck != 0);

	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryGraphicCombineProject, iCheck);

	CheckOneGraph();
	UpdateGraphic();
}

void CDlgGraphicDeadline::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bFullScreenGraph = !m_bFullScreenGraph;

	SetWindowPosition(m_lLastWidth, m_lLastHeight, 0);
	Invalidate(TRUE);
	ShowControls(!m_bFullScreenGraph);

}

void CDlgGraphicDeadline::OnBnClickedCheckGraphPeriod()
{
	int		iCheck;

	iCheck = m_checkPeriod.GetCheck();
	m_bCheckPeriod = (iCheck != 0);

	if (m_bCheckPeriod)
	{
		m_editPeriod.ShowWindow(SW_SHOW);
		m_textPeriod.ShowWindow(SW_SHOW);
	}
	else
	{
		m_editPeriod.ShowWindow(SW_HIDE);
		m_textPeriod.ShowWindow(SW_HIDE);
	}

	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryGraphicPeriod, iCheck);
	m_bStartUpdateStatistics = true;	// make sure the RPC is called again
}


void CDlgGraphicDeadline::OnEnChangeEditGraphicPeriod()
{
	CString sTxt;

	m_editPeriod.GetWindowText(sTxt);
	m_iPeriod = atoi(sTxt);
	if (m_iPeriod < 1)
	{
		m_iPeriod = 1;
	}
	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryGraphicPeriodValue, m_iPeriod);
	m_bStartUpdateStatistics = true;	// make sure the RPC is called again
}


void CDlgGraphicDeadline::OnBnClickedCheckGraphicBlock()
{
	int		iCheck;

	iCheck = m_checkModeBlock.GetCheck();
	m_bBlockMode = (iCheck != 0);

	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryBlockMode, iCheck);
	UpdateGraphic();
//	m_bStartUpdateStatistics = true;	// make sure the RPC is called again
}


void CDlgGraphicDeadline::OnBnClickedCheckGraphicSingleMultiple()
{
	int		iCheck;

	iCheck = m_checkSingleMultiple.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphicDeadline, registrySingleMultiple, iCheck);

	m_listProjects.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
}


void CDlgGraphicDeadline::OnBnClickedCheckGraphicAverage()
{
	int		iCheck;

	iCheck = m_checkAverage.GetCheck();
	m_bCheckAverage = (iCheck != 0);
	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryAverage, iCheck);
	UpdateGraphic();
}


void CDlgGraphicDeadline::OnBnClickedCheckGraphicExtended2()
{
	int		iCheck;

	iCheck = m_checkExpanded.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryExpanded, iCheck);

	PostCommandView(UWM_RESTART_GRAPHIC_DEADLINE);

	m_checkExpanded.EnableWindow(FALSE);
}


void CDlgGraphicDeadline::OnBnClickedCheckGraphicCpu()
{
	int iCheck;
	
	iCheck = m_checkCpu.GetCheck();
	m_bCpu = (iCheck != 0);

	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryCpu, iCheck);
	UpdateGraphic();
}


void CDlgGraphicDeadline::OnBnClickedCheckGraphicGpu()
{
	int iCheck;
	
	iCheck = m_checkGpu.GetCheck();
	m_bGpu = (iCheck != 0);

	theApp.WriteProfileInt(registrySectionGraphicDeadline, registryGpu, iCheck);
	UpdateGraphic();
}

BOOL CDlgGraphicDeadline::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgGraphicDeadline::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks-graphs#deadline";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

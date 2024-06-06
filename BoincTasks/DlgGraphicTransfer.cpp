
#include "stdafx.h"
#include "BoincTasks.h"
#include "RemoteCombinedDoc.h"
#include "DlgGraphicTransfer.h"
#include "ThreadRpc.h"
#include "DlgColorGraphic.h"
#include "DlgLogging.h"
#include "Translation.h"

// CDlgGraphicTransfer dialog

IMPLEMENT_DYNAMIC(CDlgGraphicTransfer, CDialog)

CDlgGraphicTransfer::CDlgGraphicTransfer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphicTransfer::IDD, pParent)
{
	m_pDoc = NULL;

	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iAddComputer = 0;
	m_bStartUpdate = false;
	m_iNrThreadReturn = 0;
	m_bThreadBusy = false;
	m_iLineThickness = 2;
	m_iWindowInitialLeftMargin = -1;
	m_bUpdate = false;
	m_bFullScreenGraph = false;
	m_lLastWidth = 0;
	m_lLastHeight = 0;
	m_bInvalidate = false;
	m_bShowWindow = false;
	m_iShowWindowDelay = 0;
	m_bTimerActive = false;
}

CDlgGraphicTransfer::~CDlgGraphicTransfer()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		if (m_lComputer.at(iPos)->pDailyXferHistory)
		{
			delete m_lComputer.at(iPos)->pDailyXferHistory;
		}
		delete m_lComputer.at(iPos);
	}
}


void CDlgGraphicTransfer::InitialUpdate(CRemoteCombinedDoc *pDoc, bool bShowWindow)
{
	m_bShowWindow = bShowWindow;
	m_iShowWindowDelay = 0;
	m_pDoc = pDoc;

	if (bShowWindow)
	{
		if (!m_bTimerActive)
		{
			SetTimer(UTIMER_DLG_GRAPHIC_TRANSFER, 1000, 0);
			m_bTimerActive = true;
		}
	}

}

void CDlgGraphicTransfer::ShowControls(bool bShow)
{
	int iShow;

	if (bShow) iShow = SW_SHOW;
	else iShow = SW_HIDE;

	m_buttonUpdate.ShowWindow(iShow);
	m_editLineThickness.ShowWindow(iShow);
	m_textLineThickness.ShowWindow(iShow);
	m_textComputers.ShowWindow(iShow);
	m_listComputers.ShowWindow(iShow);
	m_checkBar.ShowWindow(iShow);
	m_checkUp.ShowWindow(iShow);
	m_checkDown.ShowWindow(iShow);

	m_checkSingleMultiple.ShowWindow(iShow);

	m_buttonHelp.ShowWindow(iShow);
}

void CDlgGraphicTransfer::AddComputer(CString sComputer, COLORREF color, int iRepeat)
{
	for (int iCountComputer = 0; iCountComputer < (int) m_lComputer.size(); iCountComputer++)
	{
		if (m_lComputer.at(iCountComputer)->m_pcThreadRpcId != NULL)
		{
			if (sComputer ==  m_lComputer.at(iCountComputer)->m_pcThreadRpcId)
			{
				// computer found
				m_window.AddComputer(m_lComputer.at(iCountComputer)->pDailyXferHistory, color, iRepeat);
			}
		}
	}
}


void CDlgGraphicTransfer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_GRAPHIC_TRANSFER, m_window);
	//	DDX_Control(pDX, IDC_BUTTON_COLORS, m_colors);
	DDX_Control(pDX, IDC_LIST_GRAPHIC_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_GROUP_COMPUTERS, m_textComputers);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_buttonUpdate);
	//	DDX_Control(pDX, IDC_FULL_DEBUG_MODE, m_textDebugMode);
	DDX_Control(pDX, IDC_EDIT_GRAPHIC_LINE, m_editLineThickness);
	DDX_Control(pDX, IDC_TEXT_GRAPHIC_LINE_THICKNESS, m_textLineThickness);
	//	DDX_Control(pDX, IDC_CHECK_GRAPHIC_PROJECTS_SAME, m_checkCombineProject);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, m_checkSingleMultiple);
//	DDX_Control(pDX, IDC_CHECK_GRAPHIC_EXTENDED, m_checkExpanded);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_TRANSFER_DOWN, m_checkDown);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_TRANSFER_UP, m_checkUp);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_BLOCK, m_checkBar);

	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(CDlgGraphicTransfer, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_MOUSEMOVE( )
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_HELPINFO()

	ON_LBN_SELCHANGE(IDC_LIST_GRAPHIC_COMPUTERS, &CDlgGraphicTransfer::OnLbnSelchangeListGraphicComputers)

	ON_MESSAGE(UWM_INITIAL_GRAPHIC,OnInitialGraphic)

	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_GRAPHIC_TRANSFER, OnGraphicsReady)
	ON_MESSAGE(UWM_GRAPHIC_COLOR_CHANGED, OnColorChanged)
	ON_MESSAGE(UWM_MSG_DOUBLE_CLICK, OnDoubleClick)

	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgGraphicTransfer::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_COLORS, &CDlgGraphicTransfer::OnBnClickedButtonColors)
	ON_EN_CHANGE(IDC_EDIT_GRAPHIC_LINE, &CDlgGraphicTransfer::OnEnChangeEditGraphicLine)
//	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_PROJECTS_SAME, &CDlgGraphicTransfer::OnBnClickedCheckGraphicProjectsSame)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, &CDlgGraphicTransfer::OnBnClickedCheckGraphicSingleMultiple)
//	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_EXTENDED, &CDlgGraphicTransfer::OnBnClickedCheckGraphicExtended)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_TRANSFER_DOWN, &CDlgGraphicTransfer::OnBnClickedCheckGraphicTransferDown)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_TRANSFER_UP, &CDlgGraphicTransfer::OnBnClickedCheckGraphicTransferUp)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_BLOCK, &CDlgGraphicTransfer::OnBnClickedCheckGraphicBlock)

	ON_BN_CLICKED(ID_HELP, &CDlgGraphicTransfer::OnBnClickedHelp)
END_MESSAGE_MAP()


// CDlgGraphicTransfer message handlers

BOOL CDlgGraphicTransfer::OnInitDialog()
{
	int		iCheck;

	CDialog::OnInitDialog();

	m_bDebugMode = false;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translation

	this->SetWindowText(gszTranslation[PosDialogGraphicTransferTitle]);

	m_textComputers.SetWindowText(gszTranslation[PosDialogGraphicTitleComputers]);
	m_checkSingleMultiple.SetWindowText(gszTranslation[PosDialogGraphicMultiple]);
	m_buttonUpdate.SetWindowText(gszTranslation[PosDialogGraphicButtonUpdate]);
	m_textLineThickness.SetWindowText(gszTranslation[PosDialogGraphicLineThickness]);
	m_checkDown.SetWindowText(gszTranslation[PosDialogGraphicTransferDown]);
	m_checkUp.SetWindowText(gszTranslation[PosDialogGraphicTransferUp]);
// Translation

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	m_iLineThickness = theApp.GetProfileInt(registrySectionGraphicTransfer, registryGraphicLineThickness, 1);
	if (m_iLineThickness < 1) m_iLineThickness = 1;
	CString sTxt;
	sTxt.Format("%d",m_iLineThickness);
	m_editLineThickness.SetWindowText(sTxt);


	iCheck = theApp.GetProfileIntA(registrySectionGraphicTransfer, registryGraphicTransferDown, 1);
	m_checkDown.SetCheck(iCheck);
	m_bShowDown = (iCheck != 0);
	iCheck = theApp.GetProfileIntA(registrySectionGraphicTransfer, registryGraphicTransferUp, 0);
	m_checkUp.SetCheck(iCheck);
	m_bShowUp = (iCheck != 0);

	iCheck = theApp.GetProfileInt(registrySectionGraphicTransfer, registrySingleMultiple, 1);
	m_checkSingleMultiple.SetCheck(iCheck);
	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);

	iCheck = theApp.GetProfileIntA(registrySectionGraphicTransfer, registryBlockMode, 0);
	m_checkBar.SetCheck(iCheck);
	m_bBlockMode = (iCheck != 0);

//	m_checkExpanded.SetCheck(m_iExpand);

//	m_bmp.LoadBitmap(IDB_BITMAP_COLORS);
//	m_colors.SetBitmap(m_bmp);

	m_iTimer = 0;			// Don't start
	m_iTimerEnd = 120;		// 2 minutes

	ResetTransfers();

//	m_textDebugMode.ShowWindow(SW_HIDE);

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTransferPosLeft, 0);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTransferPosTop, 0);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTransferPosRight, 800) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicTransferPosBottom, 540) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER ); //| SWP_NOSIZE

//	SetTimer(UTIMER_DLG_GRAPHIC, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgGraphicTransfer::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTransferPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTransferPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTransferPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicTransferPosBottom, wp.rcNormalPosition.bottom);
}

BOOL CDlgGraphicTransfer::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgGraphicTransfer::ResetTransfers()
{
	for (int iComputers = 0; iComputers < (int) m_lComputer.size(); iComputers++)
	{
		if (m_lComputer.at(iComputers)->pDailyXferHistory != NULL)
		{
			m_lComputer.at(iComputers)->pDailyXferHistory->daily_xfers.clear();
		}
	}
}

void CDlgGraphicTransfer::OnTimer(UINT_PTR nIDEvent)
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

	if (!this->IsWindowVisible())
	{
		if (m_bTimerActive)
		{
			KillTimer(UTIMER_DLG_GRAPHIC_TRANSFER);
			m_bTimerActive = false;
		}
		return;
	}

	if (m_pDoc == NULL) return;
	if (m_pDoc->m_bDocumentIsClosing) return;			// everything is closing down.

	m_sLog.m_pFrom = "CDlgGraphicTransfer::OnTimerDlg";

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
		if (m_iNrThreadReturn >= (int) m_lComputer.size())		//m_iNrThreadRpc)
		{
			if (m_bDebugMode)
			{
				_snprintf_s(cBufferLogging,127,_TRUNCATE,"m_window.Enable(true)");m_sLog.m_pText = cBufferLogging;
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
			}

			m_window.Enable(true, m_bDebugMode);
			UpdateGraphic();
			m_bThreadBusy = false;
		}
	}
	else
	{
		if (m_bUpdate)
		{
			UpdateGraphic();
			m_bUpdate = false;
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
			m_bStartUpdate = true;
		}
	}

	if (!m_window.IsPainting())
	{
		if (m_bStartUpdate)
		{
			m_window.Enable(false, m_bDebugMode);
			ResetTransfers();
			m_bStartUpdate = false;
			m_iTimer = 0;
			HWND hWnd = this->m_hWnd;
			m_iNrThreadReturn = 0;
			for (int iCount = 0; iCount < (int) m_lComputer.size() ;iCount ++)
			{
				if (m_lComputer.at(iCount)->pDailyXferHistory == NULL)
				{
					m_lComputer.at(iCount)->pDailyXferHistory = new DAILY_XFER_HISTORY;
				}

				::PostThreadMessage(m_lComputer.at(iCount)->m_ThreadRpcId,UWM_MSG_THREAD_START_RPC_GRAPHIC_TRANSFER, (WPARAM) this->m_hWnd,(LPARAM) m_lComputer.at(iCount)->pDailyXferHistory);
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
			m_sLog.m_pFrom = "CDlgGraphicTransfer::OnTimerDlg";
			m_sLog.m_pText = "Refresh";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
		}

	}
}

void CDlgGraphicTransfer::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

void CDlgGraphicTransfer::OnSizing(UINT fwSide, LPRECT pRect)
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

void CDlgGraphicTransfer::OnMouseMove(UINT nFlags, CPoint point)
{
//	if (m_iWindowInitialLeftMargin > 0)
//	{
//		if (point.x > m_iWindowInitialLeftMargin)
//		{
//			int ii = 1;
//		//m_window.OnMouseMove(nFlags, point);
//	}

	CDialog::OnMouseMove(nFlags, point);
}


#define TOP_MARGIN	10
#define LEFT_MARGIN 220

void CDlgGraphicTransfer::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
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


LRESULT CDlgGraphicTransfer::OnInitialGraphic(WPARAM parm1, LPARAM parm2)
{
	CComputerGraphicTransferListItem *pComputerGraphicListItem;
	char *pc;

//	if (m_iNrThreadRpc > (int) theApp.m_lComputers.size()) return 0;

	pc = (char *) parm1;

	while (m_lComputer.size() <= (UINT) m_iAddComputer)
	{
		pComputerGraphicListItem = new CComputerGraphicTransferListItem;
		m_lComputer.push_back(pComputerGraphicListItem);
	}

	m_lComputer.at(m_iAddComputer)->m_pcThreadRpcId =  pc;
	m_lComputer.at(m_iAddComputer)->m_pThreadRpc  = (CThreadRpc *) parm2;
	m_lComputer.at(m_iAddComputer)->m_ThreadRpcId = m_lComputer.at(m_iAddComputer)->m_pThreadRpc->m_nThreadID;
	m_lComputer.at(m_iAddComputer)->pDailyXferHistory = NULL;

	m_iAddComputer++;

	// make sure the RPC is called again
	m_bStartUpdate = true;

	return 0;
}

// from every remote/local computer there will be one tp report here
LRESULT CDlgGraphicTransfer::OnGraphicsReady(WPARAM parm1, LPARAM parm2)
{
	char cBufferLogging[255];

	int retval, iStatus, iThreadId, iCountThread;

	iStatus = (int) parm1&0xf ;
	retval	= (int) parm1;
	retval >>= 4;

	iThreadId = (int) parm2;

	if (iStatus != RPC_STATUS_CONNECTED)
	{
		m_iNrThreadReturn++;
		return 0;
	}

	m_sLog.m_pFrom = "CDlgGraphicTransfer::OnGraphicsReady";

	// first check the thread

	for (iCountThread = 0; iCountThread < (int) m_lComputer.size(); iCountThread++)
	{
		if (m_lComputer.at(iCountThread)->m_ThreadRpcId == iThreadId)
		{
			if (m_listComputers.FindStringExact(0, m_lComputer.at(iCountThread)->m_pcThreadRpcId) ==  LB_ERR)	// check if already in the list
			{
				m_listComputers.AddString(m_lComputer.at(iCountThread)->m_pcThreadRpcId,iCountThread);			//  index start a 0 = computer 1;
				if (m_bDebugMode)
				{
					_snprintf_s(cBufferLogging,254,_TRUNCATE,"Computer: %s", m_lComputer.at(iCountThread)->m_pcThreadRpcId);m_sLog.m_pText = cBufferLogging;
					theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &m_sLog, LOGGING_NORMAL);
				}
			}
			break;
		}
	}

	m_iNrThreadReturn++;

	return 0;
}

LRESULT CDlgGraphicTransfer::OnColorChanged(WPARAM parm1, LPARAM parm2)
{
	m_bUpdate = true;

	return 0;
}

void CDlgGraphicTransfer::DisplayStatus(int iStatus)
{
}
void CDlgGraphicTransfer::OnBnClickedButtonUpdate()
{
	m_listComputers.ResetContent();

	m_bUpdate = true;

	// make sure the RPC is called again
	m_bStartUpdate = true;
}

void CDlgGraphicTransfer::UpdateGraphic()
{
	int iItemsComputers;
	int iRepeat;
	CString		sComputer;
	int			iComputers;
	COLORREF	color;
	int			iColorIndex, iColorSelected;
	bool		bFixedColors;

	iColorIndex = 0;
	iColorSelected = 0;

	iItemsComputers = m_listComputers.GetCount();

	bFixedColors = theApp.m_pDlgColorGraphic->GetFixedColors();

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

	m_listComputers.Invalidate();	// colors changed

	m_window.Reset();
	iComputers = 0;

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
			AddComputer(sComputer, color, iRepeat);
			iComputers++;
		}
	}
	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_WINDOW_LINE, m_iLineThickness, 0);
	m_window.SetDownUp(m_bShowDown, m_bShowUp);
	m_window.SetBlockMode(m_bBlockMode);
	m_window.SetColors(theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_BACKGROUND), theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_PROJECT_TEXT), theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_SCALE_TEXT), theApp.m_pDlgColorGraphic->GetColor(COLOR_ARRAY_GRID));
	m_window.ShowComputers(iColorSelected);
}

void CDlgGraphicTransfer::OnLbnSelchangeListGraphicComputers()
{
	m_bUpdate = true;
}

void CDlgGraphicTransfer::OnBnClickedButtonColors()
{
	theApp.m_pDlgColorGraphic->ShowWindow(SW_SHOW);
}

void CDlgGraphicTransfer::OnShowWindow(BOOL bShow, UINT nStatus)
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CWnd::OnShowWindow(bShow, nStatus);
	m_bStartUpdate = true;			// make sure the RPC is called again

	if (bShow)
	{
		if (!m_bTimerActive)
		{
			SetTimer(UTIMER_DLG_GRAPHIC_TRANSFER, 1000, 0);
			m_bTimerActive = true;
		}
	}

}
void CDlgGraphicTransfer::OnEnChangeEditGraphicLine()
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
	theApp.WriteProfileInt(registrySectionGraphicTransfer, registryGraphicLineThickness, m_iLineThickness);

	m_bUpdate = true;
}

LRESULT CDlgGraphicTransfer::OnDoubleClick(WPARAM parm1, LPARAM parm2)
{
	m_bFullScreenGraph = !m_bFullScreenGraph;
	SetWindowPosition(m_lLastWidth, m_lLastHeight, 0);
	Invalidate(TRUE);
	ShowControls(!m_bFullScreenGraph);
	return 0;
}

void CDlgGraphicTransfer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	PostMessage(UWM_MSG_DOUBLE_CLICK,0,0);
}

void CDlgGraphicTransfer::OnBnClickedCheckGraphicSingleMultiple()
{
	int		iCheck;

	iCheck = m_checkSingleMultiple.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphicTransfer, registrySingleMultiple, iCheck);

	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
}

void CDlgGraphicTransfer::OnBnClickedCheckGraphicTransferDown()
{
	int		iCheck;

	iCheck = m_checkDown.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphicTransfer, registryGraphicTransferDown, iCheck);

	m_bShowDown = (iCheck != 0);

	if (!m_bShowDown  && !m_bShowUp)
	{
		m_checkUp.SetCheck(TRUE);
		OnBnClickedCheckGraphicTransferUp();
		return;
	}

	m_bUpdate = true;
}


void CDlgGraphicTransfer::OnBnClickedCheckGraphicTransferUp()
{
	int		iCheck;

	iCheck = m_checkUp.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphicTransfer, registryGraphicTransferUp, iCheck);

	m_bShowUp = (iCheck != 0);

	if (!m_bShowDown  && !m_bShowUp)
	{
		m_checkDown.SetCheck(TRUE);
		OnBnClickedCheckGraphicTransferDown();
		return;
	}

	m_bUpdate = true;
}

/*
void CDlgGraphicTransfer::OnBnClickedCheckGraphicExtended()
{
	int		iCheck;

	iCheck = m_checkExpanded.GetCheck();
	theApp.WriteProfileInt(registrySectionGraphicTransfer, registryExpanded, iCheck);

	PostCommandView(UWM_RESTART_GRAPHIC);
}
*/




void CDlgGraphicTransfer::OnBnClickedCheckGraphicBlock()
{
	int iCheck;

	iCheck = m_checkBar.GetCheck();
	m_bBlockMode = (iCheck != 0);

	theApp.WriteProfileInt(registrySectionGraphicTransfer, registryBlockMode, iCheck);

	m_bUpdate = true;
}

BOOL CDlgGraphicTransfer::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgGraphicTransfer::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks-graphs#transfer";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}
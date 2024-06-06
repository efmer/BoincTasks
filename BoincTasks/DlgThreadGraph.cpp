// DlgThreadTiming.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "RemoteCombinedDoc.h"
#include "DlgThreadGraph.h"
#include "ThreadProcessTime.h"
#include "DlgColorGraphic.h"
#include "DlgSettingsExpert.h"
#include "afxdialogex.h"
#include "Translation.h"

// CDlgThreadGraph dialog

IMPLEMENT_DYNAMIC(CDlgThreadGraph, CDialog)

CDlgThreadGraph::CDlgThreadGraph(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgThreadGraph::IDD, pParent)
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

	if (theApp.m_pDlgSettingsExpert->m_bEnableThreadGraph)
	{
		m_pThreadProcessTime = (CThreadProcessTime *) AfxBeginThread(RUNTIME_CLASS(CThreadProcessTime));
		CComputerThreadGraphicListItem *pComputerThreadGraphicListItem;
		pComputerThreadGraphicListItem = new CComputerThreadGraphicListItem;

		pComputerThreadGraphicListItem->m_pcThreadRpcId =  BOINCTASKS_PROCESS_TIMING_NAME;
		pComputerThreadGraphicListItem->m_pThreadRpc  = NULL;
		pComputerThreadGraphicListItem->m_ThreadRpcId = m_pThreadProcessTime->m_nThreadID;
		m_lComputer.push_back(pComputerThreadGraphicListItem);
		m_iAddComputer++;
	}
	else
	{
		m_pThreadProcessTime = NULL;
	}
}

CDlgThreadGraph::~CDlgThreadGraph()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
	for (int iPos = 0; iPos < (int) m_lComputer.size(); iPos++)
	{
		delete m_lComputer.at(iPos);
	}
	if (m_pThreadProcessTime)
	{
		m_pThreadProcessTime->PostThreadMessage(UWM_MSG_THREAD_QUIT,0,0);
	}
}

void CDlgThreadGraph::InitialUpdate(CRemoteCombinedDoc *pDoc)
{
	m_pDoc = pDoc;
}

void CDlgThreadGraph::AddComputer(CString sComputer, COLORREF color, int iRepeat, time_t tBegin)
{
	for (int iCountComputer = 0; iCountComputer < (int) m_lComputer.size(); iCountComputer++)
	{
		if (m_lComputer.at(iCountComputer)->m_pcThreadRpcId != NULL)
		{
			if (sComputer ==  m_lComputer.at(iCountComputer)->m_pcThreadRpcId)
			{
				// computer found
				m_window.AddComputer(&m_lComputer.at(iCountComputer)->m_lThreadTime, color, iRepeat, tBegin);
			}
		}
	}
}

void CDlgThreadGraph::Reset()
{
//	for (int iComputers = 0; iComputers < (int) m_lComputer.size(); iComputers++)
//	{
//		if (m_lComputer.at(iComputers)->pDailyXferHistory != NULL)
//		{
//			m_lComputer.at(iComputers)->>pDailyXferHistory->daily_xfers.clear();
//		}
//	}
}

void CDlgThreadGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WND_GRAPHIC, m_window);
	DDX_Control(pDX, IDC_LIST_GRAPHIC_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_GRAPHIC_COMBO_TIME, m_setTime);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_AVERAGE, m_checkAverage);
}

BEGIN_MESSAGE_MAP(CDlgThreadGraph, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()

	ON_LBN_SELCHANGE(IDC_LIST_GRAPHIC_COMPUTERS, &CDlgThreadGraph::OnLbnSelchangeListGraphicComputers)
	ON_CBN_SELCHANGE(IDC_GRAPHIC_COMBO_TIME, &CDlgThreadGraph::OnCbnSelchangeGraphicComboTime)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_AVERAGE, &CDlgThreadGraph::OnBnClickedCheckGraphicAverage)

	ON_MESSAGE(UWM_INITIAL_GRAPHIC,OnInitialGraphic)
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_THREAD_TIMING, OnGraphicsReady)


END_MESSAGE_MAP()


// CDlgThreadGraph message handlers

BOOL CDlgThreadGraph::OnInitDialog()
{
	int	iSelection, iCheck;
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translation

	this->SetWindowText(gszTranslation[PosDialogThreadGraphicTitle]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC1Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC5Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC10Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC30Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC60Minute]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC3Hour]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC6Hour]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC12Hour]);
	m_setTime.AddString(gszTranslation[PosDialogTemperatureGraphicC24Hour]);

	m_checkAverage.SetWindowText(gszTranslation[PosDialogGraphicAverage]);
// Translation

	iSelection = theApp.GetProfileInt(registrySectionGraphic,registryTimeThread, 2);
	m_setTime.SetCurSel(iSelection);
	OnCbnSelchangeGraphicComboTime();

	iCheck = theApp.GetProfileInt(registrySectionGraphic, registryAverageThread, 1);
	m_bCheckAverage = (iCheck != 0);
	m_checkAverage.SetCheck(m_bCheckAverage);

	m_iTimer = 0;
	m_iTimerEnd = 10;

	Reset();

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicThreadPosLeft, 0);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicThreadPosTop, 0);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicThreadPosRight, 800) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryGraphicThreadPosBottom, 540) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER ); //| SWP_NOSIZE

//	SetTimer(UTIMER_DLG_GRAPHIC_THREAD, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgThreadGraph::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicThreadPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicThreadPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicThreadPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryGraphicThreadPosBottom, wp.rcNormalPosition.bottom);
}

void CDlgThreadGraph::OnTimer(UINT_PTR nIDEvent)
{
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
			KillTimer(UTIMER_DLG_GRAPHIC_THREAD);
			m_bTimerActive = false;
		}
		return;
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
		if (m_iNrThreadReturn >= (int) m_lComputer.size())		//m_iNrThreadRpc)
		{
			m_window.Enable(true);
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
			m_window.Enable(false);
			m_bStartUpdate = true;
		}
	}

	if (!m_window.IsPainting())
	{
		if (m_bStartUpdate)
		{
			m_window.Enable(false);
			Reset();
			m_bStartUpdate = false;
			m_iTimer = 0;
			HWND hWnd = this->m_hWnd;
			m_iNrThreadReturn = 0;
			_time64(&m_tTime);

			for (int iCount = 0; iCount < (int) m_lComputer.size() ;iCount ++)
			{
				::PostThreadMessage(m_lComputer.at(iCount)->m_ThreadRpcId,UWM_MSG_THREAD_START_RPC_THREAD_TIMING, (WPARAM) this->m_hWnd,(LPARAM) &m_lComputer.at(iCount)->m_lThreadTime);
				m_bThreadBusy = true;
			}
		}
	}

	m_iTimer++;

	if (m_iTimer > m_iTimerEnd)
	{
		m_iTimer = 0;
//		m_bThreadBusy = false;			// has to be ready by now otherwise it's stuck
	}
}

void CDlgThreadGraph::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

void CDlgThreadGraph::OnSizing(UINT fwSide, LPRECT pRect)
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

void CDlgThreadGraph::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
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

LRESULT CDlgThreadGraph::OnInitialGraphic(WPARAM parm1, LPARAM parm2)
{
	CComputerThreadGraphicListItem *pComputerThreadGraphicListItem;
	char *pc;

//	if (m_iNrThreadRpc > (int) theApp.m_lComputers.size()) return 0;

	pc = (char *) parm1;

	while (m_lComputer.size() <= (UINT) m_iAddComputer)
	{
		pComputerThreadGraphicListItem = new CComputerThreadGraphicListItem;
		m_lComputer.push_back(pComputerThreadGraphicListItem);
	}

	m_lComputer.at(m_iAddComputer)->m_pcThreadRpcId =  pc;
	m_lComputer.at(m_iAddComputer)->m_pThreadRpc  = (CThreadRpc *) parm2;
	m_lComputer.at(m_iAddComputer)->m_ThreadRpcId = m_lComputer.at(m_iAddComputer)->m_pThreadRpc->m_nThreadID;

	m_iAddComputer++;

	// make sure the RPC is called again
	m_bStartUpdate = true;

	return 0;
}

// from every remote/local computer there will be one tp report here
LRESULT CDlgThreadGraph::OnGraphicsReady(WPARAM wParam, LPARAM parm2)
{
	int iCountThread;

	CRpcThreadReturn	*pRpcThreadReturn;
	pRpcThreadReturn = (CRpcThreadReturn *) wParam;

//	if (pRpcThreadReturn->m_iStatus != RPC_STATUS_CONNECTED)
//	{
//		delete pRpcThreadReturn;
//		m_iNrThreadReturn++;
//		return 0;
//	}

	// first check the thread

	for (iCountThread = 0; iCountThread < (int) m_lComputer.size(); iCountThread++)
	{
		if (m_lComputer.at(iCountThread)->m_ThreadRpcId == pRpcThreadReturn->m_iThreadId)
		{
			if (m_listComputers.FindStringExact(0, m_lComputer.at(iCountThread)->m_pcThreadRpcId) ==  LB_ERR)	// check if already in the list
			{
				m_listComputers.AddString(m_lComputer.at(iCountThread)->m_pcThreadRpcId,iCountThread);			//  index start a 0 = computer 1;
			}
			break;
		}
	}

	m_iNrThreadReturn++;

	delete pRpcThreadReturn;
	return 0;
}

void CDlgThreadGraph::UpdateGraphic()
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
			m_listComputers.SetColor(color, iRepeat, iCount, iColorSelected);	// only set computer color 0 = computer 1
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
			color = m_listComputers.GetColor(iComputers, &iRepeat);
			AddComputer(sComputer, color, iRepeat, m_tTime);
			iComputers++;
		}
	}
	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_WINDOW_LINE, m_iLineThickness, 0);
	::PostMessage(m_window.m_hWnd, UWM_MSG_GRAPHIC_AVERAGE, m_bCheckAverage,0);
//	m_window.SetDownUp(m_bShowDown, m_bShowUp);
//	m_window.SetBlockMode(m_bBlockMode);
	m_window.SetColors(*(theApp.m_pDlgColorGraphic->GetColorArray() + COLOR_ARRAY_BACKGROUND), 0, *(theApp.m_pDlgColorGraphic->GetColorArray() + COLOR_ARRAY_SCALE_TEXT), *(theApp.m_pDlgColorGraphic->GetColorArray() + COLOR_ARRAY_GRID_FINE), *(theApp.m_pDlgColorGraphic->GetColorArray() + COLOR_ARRAY_GRID));
	m_window.ShowComputers(iComputers);
}

void CDlgThreadGraph::OnLbnSelchangeListGraphicComputers()
{
	m_bUpdate = true;
}

void CDlgThreadGraph::OnCbnSelchangeGraphicComboTime()
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

	theApp.WriteProfileInt(registrySectionGraphic,registryTimeThread,iSelection);
}

void CDlgThreadGraph::OnShowWindow(BOOL bShow, UINT nStatus)
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CWnd::OnShowWindow(bShow, nStatus);
	m_bStartUpdate = true;			// make sure the RPC is called again

	if (bShow)
	{
		if (!m_bTimerActive)
		{
			SetTimer(UTIMER_DLG_GRAPHIC_THREAD, 1000, 0);
			m_bTimerActive = true;
		}
	}
}

void CDlgThreadGraph::OnBnClickedCheckGraphicAverage()
{
	int		iCheck;

	iCheck = m_checkAverage.GetCheck();
	m_bCheckAverage = (iCheck != 0);
	theApp.WriteProfileInt(registrySectionGraphic, registryAverageThread, iCheck);
	UpdateGraphic();
}
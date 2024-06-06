// DlgScheduler.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgScheduler.h"
#include "afxdialogex.h"


// CDlgScheduler dialog

IMPLEMENT_DYNAMIC(CDlgScheduler, CDialogEx)

CDlgScheduler::CDlgScheduler(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgScheduler::IDD, pParent)
{
	m_iWindowInitialLeftMargin = -1;
	m_lLastWidth = 0;
	m_lLastHeight = 0;

	m_bInvalidate = false;
//	m_bMouseTracking = false;

	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_window.m_bForceActive = false;
	m_window.Setup(48*2,30/2,7,true,false,true);
	m_plInterval = NULL;
}

CDlgScheduler::~CDlgScheduler()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
}

void CDlgScheduler::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registrySchedulerPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registrySchedulerPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registrySchedulerPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registrySchedulerPosBottom, wp.rcNormalPosition.bottom);
}

void CDlgScheduler::Init(CWnd *pWnd, std::deque<CRuleIntervalItem *> *plInterval)
{
	m_pWndOwner = pWnd;
	m_plInterval = plInterval;
}

void CDlgScheduler::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WND_SCHEDULER, m_window);
}


BEGIN_MESSAGE_MAP(CDlgScheduler, CDialogEx)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE( )
	ON_WM_TIMER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDOK, &CDlgScheduler::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgScheduler message handlers

void CDlgScheduler::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bInvalidate)
	{
		this->Invalidate(TRUE);
		m_bInvalidate = false;
	}
}

BOOL CDlgScheduler::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registrySchedulerPosLeft, 0);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registrySchedulerPosTop, 0);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registrySchedulerPosRight, 800) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registrySchedulerPosBottom, 540) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER ); //| SWP_NOSIZE

	SetTimer(UTIMER_DLG_SCHEDULER, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDlgScheduler::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

#define TOP_MARGIN	10
#define LEFT_MARGIN 220

void CDlgScheduler::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
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

//	if (m_bFullScreenGraph)
//	{
//		lLeftMargin = 5;
//		lTopMargin	= 5;
//	}
//	else
//	{
		lLeftMargin= m_iWindowInitialLeftMargin;
		lTopMargin = m_iWindowInitialTopMargin;
//	}

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


void CDlgScheduler::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgScheduler::OnMouseLeave()
{
//	m_bMouseTracking = false;
//	m_window.LeftWindow();

	CDialogEx::OnMouseLeave();
}

void CDlgScheduler::OnLButtonDown(UINT nFlags, CPoint point)
{
//	bool	bShift;
//
//	if (nFlags & MK_SHIFT)
//	{
//		bShift = true;
//	}
//	else bShift = false;
//
//	m_window.Keys(point, m_iWindowInitialTopMargin,m_iWindowInitialLeftMargin, bShift);
//	m_window.IsInBox(point, m_iWindowInitialTopMargin,m_iWindowInitialLeftMargin, true);
//
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgScheduler::OnLButtonUp(UINT nFlags, CPoint point)
{
//	m_window.Keys(point, m_iWindowInitialTopMargin,m_iWindowInitialLeftMargin, false);
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgScheduler::OnBnClickedOk()
{
	m_window.GetTimeInterval(&m_plInterval);
	m_pWndOwner->SendMessage(UWM_MSG_RULES_INTERVAL,0, 0);

	CDialogEx::OnOK();
}

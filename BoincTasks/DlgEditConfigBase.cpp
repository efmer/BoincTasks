// DlgEditConfigBase.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgEditConfigBase.h"
#include "DlgEditConfig.h"
#include "afxdialogex.h"

// DlgEditConfigBase dialog

IMPLEMENT_DYNAMIC(DlgEditConfigBase, CDialogEx)


DlgEditConfigBase::DlgEditConfigBase(CWnd* pParent /*=NULL*/)
{

}


//DlgEditConfigBase::DlgEditConfigBase(CWnd* pParent /*=NULL*/)
//	: CDialogEx(CDlgEditConfig::IDD, pParent)
//{
//
//}


DlgEditConfigBase::~DlgEditConfigBase()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}

}

BOOL DlgEditConfigBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_lLastWidth = 0;
	m_lLastHeight = 0;
	m_iWindowInitialLeftMargin = -1;
	m_iWindowInitialTopMargin = -1;

	//	m_bInvalidate = false;

	m_iThreadId = -1;
	m_bThreadBusy = false;

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryEditConfigPosLeft, 0);
	int top = theApp.GetProfileInt(registrySectionWindowPosition, registryEditConfigPosTop, 0);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryEditConfigPosRight, 800) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryEditConfigPosBottom, 540) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER); //| SWP_NOSIZE

	//	m_window.SetFocus();
	m_window.SetEventMask(m_window.GetEventMask() | ENM_CHANGE);	// report back any changes.
	m_window.LimitText(64000);

	m_bIgnoreSyntax = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void DlgEditConfigBase::InitialUpdate(CRemoteCombinedDoc *pDoc, CString sFile)
{
	m_pDoc = pDoc;
	m_sFileXml = sFile;
}

void DlgEditConfigBase::Check()
{
	CHARRANGE cr;

	int iFirstLine = m_window.GetFirstVisibleLine();
	int iLineIndex = m_window.LineIndex(iFirstLine);

	m_window.GetSel(cr);

	if (m_window.Check(m_sFileXml))
	{
		m_buttonCheckSyntax.EnableWindow(FALSE);
		m_buttonApply.EnableWindow(TRUE);
		m_window.SetSel(iLineIndex, iLineIndex);
		m_window.SetSel(cr);
	}
	else
	{
		//		m_window.SetSel(cr);
	}
	//	m_window.ShowCaret();
	m_window.SetFocus();
	m_window.Invalidate();
}

void DlgEditConfigBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WND_EDIT_CONFIG, m_window);
	DDX_Control(pDX, IDAPPLY, m_buttonApply);
	DDX_Control(pDX, IDCHECK, m_buttonCheckSyntax);
	DDX_Control(pDX, IDC_CHECK_IGNORE_SYNTAX, m_checkIgnoreSyntax);
}


BEGIN_MESSAGE_MAP(DlgEditConfigBase, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_WND_EDIT_CONFIG, &DlgEditConfigBase::OnEnChangeWndEditConfig)

	ON_BN_CLICKED(IDCHECK, &DlgEditConfigBase::OnBnClickedCheck)

	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_CHECK_IGNORE_SYNTAX, &DlgEditConfigBase::OnBnClickedCheckIgnoreSyntax)
END_MESSAGE_MAP()


// CDlgEditConfig message handlers


void DlgEditConfigBase::OnSize(UINT nType, int cx, int cy)
{
	SetWindowPosition(cx, cy, nType);
}

void DlgEditConfigBase::OnSizing(UINT fwSide, LPRECT pRect)
{
	LONG lTop, lBottom, lLeft, lRight, lWidth, lHeight;
	bool bSetWindow = false;
	bool bSetWindowWidth = false;
	bool bSetWindowHeight = false;

	// limit the minimum size of the dialog

	lTop = pRect->top;
	lBottom = pRect->bottom;
	lLeft = pRect->left;
	lRight = pRect->right;
	lWidth = lRight - lLeft;
	lHeight = lBottom - lTop;

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
		pRect->right = pRect->left + 401;
	}
	if (bSetWindowHeight)
	{
		pRect->bottom = pRect->top + 201;
	}

	CDialog::OnSizing(fwSide, pRect);
}


#define TOP_MARGIN	10
#define LEFT_MARGIN 220

void DlgEditConfigBase::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
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

	lTopMargin = 5;
	lBottomMargin = 5;

	//	if (m_bFullScreenGraph)
	//	{
	//		lLeftMargin = 5;
	//		lTopMargin	= 5;
	//	}
	//	else
	//	{
	lLeftMargin = m_iWindowInitialLeftMargin;
	lTopMargin = m_iWindowInitialTopMargin;
	//	}

	lRightMargin = 5;
	lWidthGraphic = lWidth - (lLeftMargin + lRightMargin);
	lHeightGraphic = (lHeight - (lTopMargin + lBottomMargin));

	// position graphic relative to window NOT the screen

	if (m_window.m_hWnd != NULL)
	{
		m_window.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic, FALSE);
	}

	this->Invalidate(TRUE);

	//	m_bInvalidate = true;
}


void DlgEditConfigBase::OnEnChangeWndEditConfig()
{
	m_bNeedSyntaxCheck = true;
	m_buttonCheckSyntax.EnableWindow(TRUE);

	if (!m_bIgnoreSyntax)
	{
		m_buttonApply.EnableWindow(FALSE);
	}
}


void DlgEditConfigBase::OnBnClickedCheck()
{
	Check();
}


void DlgEditConfigBase::OnSetFocus(CWnd* pOldWnd)
{
	//	CDialogEx::OnSetFocus(pOldWnd);

	m_window.SetFocus();
	// TODO: Add your message handler code here
}


void DlgEditConfigBase::OnBnClickedCheckIgnoreSyntax()
{
	int iCheck;

	iCheck = m_checkIgnoreSyntax.GetCheck();

	m_bIgnoreSyntax = (iCheck != 0);

	if (m_bIgnoreSyntax)
	{
		if (!m_bThreadBusy)
		{
			m_buttonApply.EnableWindow(TRUE);
		}
	}
}

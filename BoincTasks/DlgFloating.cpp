// DlgFloating.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "TemplateRemoteTaskView.h"		// base class for BoinCComputerView
//#include "RemoteTaskView.h"
#include "Translation.h"
#include "MainFrm.h"
#include "DlgSettingsWarning.h"
#include "DlgColorWarnings.h"
#include "DlgColorTasks.h"
#include "DlgSettingsView.h"
#include "DlgSettingsGadget.h"
#include "UseCpuGpu.h"
#include "DlgFont.h"
#include "DlgFloating.h"


// CDlgFloating dialog

IMPLEMENT_DYNAMIC(CDlgFloating, CDialogEx)

CDlgFloating::CDlgFloating(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFloating::IDD, pParent)
{
	m_pFont = NULL;
}

CDlgFloating::~CDlgFloating()
{
	delete m_pFont;
}

BOOL CDlgFloating::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_iMode = FLOATER_MODE_UNDEFINED;

//	AddColumns();
	m_bUpdateWindow = true;
	m_bOnTop = false;
	m_bRefreshBusy = false;
	m_iRefreshBusy = 0;
	m_iRefreshTime = 60;
	m_bMoved	= false;
	m_iStayOnTop = 0;

	m_iTimer = 0;
	m_iMoveTimer = 0;
	m_iStartupTimer = 0;
	m_iStartupTry	= 12;	// 2 minute

	tLastRequest	= 0;

	GetPlacement();

	m_iRadio = RADIO_SHOW_ICON;

	SetTimer(UTIMER_DLG_FLOATING, 1000, 0);

	theApp.m_pDlgSettingsGadget->PostMessage(UWM_MSG_GET_FLOATER_DATA,0,0);

	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GADGET);	// 1 = gadget


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgFloating::AddColumnsTasks()
{
	LV_COLUMN	lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iCount = 0; iCount < FLOATER_COLUMNS; iCount++)
	{
		lvc.iSubItem = iCount;
		switch (iCount)
		{

			case 0:
				lvc.pszText	= gszTranslation[PosViewTasksRowProject];
				m_iColumnWidth[0] = ReadColumnWidth(0,120,20);
				lvc.cx = m_iColumnWidth[0];
				lvc.fmt = LVCFMT_LEFT;
			break;
			case 1:
				lvc.pszText	= gszTranslation[PosViewTasksRowApplication];
				m_iColumnWidth[1] = ReadColumnWidth(1,180,20);
				lvc.cx = m_iColumnWidth[1];
				lvc.fmt = LVCFMT_LEFT;
			break;
			case 2:
				lvc.pszText = gszTranslation[PosViewTasksRowProgress];
				m_iColumnWidth[2] = ReadColumnWidth(2,80,20);
				lvc.cx = m_iColumnWidth[2];
				lvc.fmt = LVCFMT_CENTER;
			break;
			case 3:
				lvc.pszText = gszTranslation[PosViewTasksRowComputer];
				m_iColumnWidth[3] = ReadColumnWidth(3,130,20);
				lvc.cx = m_iColumnWidth[3];
				lvc.fmt = LVCFMT_LEFT;
			break;
		}

		m_list.InsertColumn(iCount,&lvc);
	}
}

void CDlgFloating::AddColumnsComputers()
{
	LV_COLUMN	lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iCount = 0; iCount < FLOATER_COLUMNS; iCount++)
	{
		lvc.iSubItem = iCount;
		switch (iCount)
		{
			case 0:
				lvc.pszText	= "Cpu";
				m_iColumnWidth[0] = ReadColumnWidth(0,120,20);
				lvc.cx = m_iColumnWidth[0];
				lvc.fmt = LVCFMT_LEFT;
			break;
			case 1:
				lvc.pszText	= "Gpu";
				m_iColumnWidth[1] = ReadColumnWidth(1,120,20);
				lvc.cx = m_iColumnWidth[1];
				lvc.fmt = LVCFMT_LEFT;
			break;
			case 2:
				lvc.pszText = "";
				m_iColumnWidth[2] = ReadColumnWidth(2,20,20);
				lvc.cx = m_iColumnWidth[2];
				lvc.fmt = LVCFMT_CENTER;
			break;
			case 3:
				lvc.pszText = gszTranslation[PosViewTasksRowComputer];
				m_iColumnWidth[3] = ReadColumnWidth(3,120,20);
				lvc.cx = m_iColumnWidth[3];
				lvc.fmt = LVCFMT_LEFT;
			break;
		}

		m_list.InsertColumn(iCount,&lvc);
	}
}

void CDlgFloating::Refresh()
{
	CDocument *pDoc;
	CView *pView;
	POSITION posDoc, posView;

	if (m_bRefreshBusy) return;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			posView = pDoc->GetFirstViewPosition();
			while (1)
			{
				pView = pDoc->GetNextView(posView);
				if (pView == NULL) break;	
				pView->PostMessage(UWM_MSG_GET_FLOATER_DATA, (WPARAM) m_iMode, (LPARAM) FLOATER_FULL_INFO);
				m_bRefreshBusy = true;
				m_iRefreshBusy = 0;
			}
		}
	}
	else
	{
		int ii = 1;
	}
}

void CDlgFloating::GetPlacement()
{
	int iScreenWidth, iScreenHeight;
	int	iLeft, iTop;

	iScreenWidth		= GetSystemMetrics(SM_CXSCREEN);
	iScreenHeight		= GetSystemMetrics(SM_CYSCREEN);

	iLeft = iScreenWidth - DEFAULT_LABEL_WIDTH;
	iLeft-= 100;
	iTop  = iScreenHeight - DEFAULT_LABEL_HEIGHT;
	iTop -= 100;
	
	iLeft	= theApp.GetProfileInt(registrySectionFloating, registryFloatingLeft, iLeft);
	iTop	= theApp.GetProfileInt(registrySectionFloating, registryFloatingTop, iTop);

	int iWidth	= theApp.GetProfileInt(registrySectionFloating, registryFloatingWidth, DEFAULT_LABEL_WIDTH);
	int iHeight	= theApp.GetProfileInt(registrySectionFloating, registryFloatingHeight, DEFAULT_LABEL_HEIGHT);

	if (iWidth < 20) iWidth = 20;
	if (iHeight< 20) iHeight= 20;

	theApp.ValidScreenPosition(&iLeft, &iTop, &iWidth, &iHeight);

	SetWindowPos(&CWnd::wndBottom, iLeft, iTop, iWidth, iHeight, SWP_NOOWNERZORDER);
}

void CDlgFloating::SavePlacement(void)
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);

	theApp.WriteProfileInt(registrySectionFloating, registryFloatingLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionFloating, registryFloatingTop, wp.rcNormalPosition.top);
	
	int iWidth =  wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	int iHeight=  wp.rcNormalPosition.bottom- wp.rcNormalPosition.top;

	theApp.WriteProfileInt(registrySectionFloating, registryFloatingWidth, iWidth);
	theApp.WriteProfileInt(registrySectionFloating, registryFloatingHeight, iHeight);
}

int CDlgFloating::ReadColumnWidth(int iColumn, int iDefault, int iMinimum)
{
	CString sRegistry;
	char cChar;
	int	iWidth;

	sRegistry = registryColumnFloater;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	iWidth = theApp.GetProfileInt(registrySectionFloating, sRegistry, iDefault);
	if (iWidth < iMinimum)
	{
		iWidth = iMinimum;
	}
	return iWidth;
}

void CDlgFloating::CheckAndSaveColumnsWidth()
{
	int		iWidth, iColumn;
//	bool	bChanged = false;
	char	cChar;
	CString sTxt;

	for (int iCount = 0; iCount < FLOATER_COLUMNS; iCount++)
	{
		iColumn = iCount;// m_iColumnOrder[iCount];
		if (iColumn >= 0)
		{
			iWidth = m_list.GetColumnWidth(iColumn);
			if (m_iColumnWidth[iColumn] != m_list.GetColumnWidth(iColumn))
			{
				m_iColumnWidth[iColumn] = iWidth;
//				bChanged = true;
				sTxt = registryColumnFloater;
				cChar = iCount + 'a';
				sTxt = sTxt + "_" + cChar; 
				// update new width in registry
				theApp.WriteProfileInt(registrySectionFloating, sTxt, iWidth);
			}
		}
	}
}

void CDlgFloating::SetMode(int iNewMode)
{
	if (m_iMode == iNewMode) return;

	m_list.DeleteAllItems();

	for (int iCount = 0; iCount < FLOATER_COLUMNS; iCount++)
	{
		m_list.DeleteColumn(0);
	}

	m_iMode = iNewMode;
	switch (m_iMode)
	{
		case FLOATER_MODE_TASKS:

			AddColumnsTasks();
		break;
		case FLOATER_MODE_COMPUTERS:
			AddColumnsComputers();
		break;
	}
}

void CDlgFloating::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_FLOATING, m_list);
}


BEGIN_MESSAGE_MAP(CDlgFloating, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_SHOWWINDOW()

	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(UWM_MSG_GET_FLOATER_DATA, OnGetFloaterData)
	ON_MESSAGE(UWM_MSG_GET_FLOATER_DATA_READY, OnGetFloaterDataReady)
	ON_MESSAGE(UWM_MSG_SET_FLOATER_MODE, OnSetFloaterMode)
	ON_MESSAGE(UWM_MSG_SET_FLOATER,OnSetFloater)

	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnChangedFont)
	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnChangedFont)


END_MESSAGE_MAP()


// CDlgFloating message handlers

void CDlgFloating::OnClose()
{
	CDialogEx::OnClose();
}

void CDlgFloating::OnShowWindow(BOOL bShow,  UINT nStatus)
{

	if (bShow)
	{
		Refresh();
	}
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CDlgFloating::OnTimer(UINT_PTR nIDEvent)
{
	POSITION posDoc;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();

	if (m_bRefreshBusy)
	{
		if (m_iRefreshBusy++ > 60)
		{
			m_bRefreshBusy = false;
		}
	}

	if (this->IsIconic()) return;
	if (!this->IsWindowVisible())
	{
		if (posDoc != NULL)				// only when combined document is active
		{
			if (m_iRadio != RADIO_SHOW_ICON) this->ShowWindow(SW_SHOW);
		}
		return;
	}

	if (posDoc == NULL)				// only when combined document is active
	{
		this->ShowWindow(SW_HIDE);
	}

	CheckAndSaveColumnsWidth();

	if (m_bUpdateWindow)
	{
//		if (!m_bThreadBusy)			// when busy wait for the update, otherwise it may not be painted.
		{
			this->Invalidate(TRUE);
			m_bUpdateWindow = false;
		}
	}

	// startup only
	if (m_iStartupTry > 0)
	{

		if (m_iStartupTimer++ > 10)
		{
			m_iStartupTry--;
			Refresh();
			m_iStartupTimer = 0;
		}
	}

	// startup only

	if (m_iTimer++ >= m_iRefreshTime)
	{
		Refresh();
		m_iTimer = 0;
	}
	if (m_iMoveTimer == 3)
	{
		this->Invalidate();
		m_list.ShowWindow(SW_SHOW);
	}
	if (m_iMoveTimer++ > 1000) m_iMoveTimer = 10;

	if (m_bMoved)
	{
		SavePlacement();
		m_bMoved = false;
	}

	if (m_bOnTop)
	{
		if (m_iStayOnTop++ > 30)
		{
			// make sure it stays on top
			this->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
			m_iStayOnTop = 0;
		}
	}

}

LRESULT CDlgFloating::OnGetFloaterData(WPARAM wParam,LPARAM lParam)
{
	time_t tSystemTime;

	_time64(&tSystemTime);

	if (tLastRequest < tSystemTime)
	{
		Refresh();
		tLastRequest = tSystemTime + 5;	// not within 5 seconds
	}

	return 0;
}

LRESULT CDlgFloating::OnGetFloaterDataReady(WPARAM wParam,LPARAM lParam)
{
	int			iSize, iPos;
	std::deque<CFloaterItem*> *pListFloater;
	CFloaterItem *pFloaterItem;
	CString		sFormat;
	int			iStatus, iDeadline;
	COLORREF	crColor, *pColorsTasks, *pColorsWarnings;
	bool		bLineColor = false, bAlternatingStripes;
	int			iDetectedGpu;

	iStatus = (int) lParam;
	if (iStatus < 0)	// history busy
	{
		m_bRefreshBusy = false;	
		m_iTimer = m_iRefreshTime-1;	// refresh again in 2 seconds
		return -1;
	}

	pColorsTasks = theApp.m_pDlgColorTasks->GetColorArray();
	pColorsWarnings = theApp.m_pDlgColorWarnings->GetColorArray();

	pListFloater = (std::deque<CFloaterItem*> *) wParam;
	m_list.DeleteAllItems();

	if (pListFloater == NULL)
	{
		m_list.InsertItem(0,"");
		m_list.SetItemText(0,0,gszTranslation[PosViewHistoryStatusToEnable]);
		m_list.SetItemText(0,1,gszTranslation[PosViewHistoryStatusToEnableMenu]);
		m_list.SetItemData(0,RGB(255,0,0));
		return 0;
	}

	iDeadline = theApp.m_pDlgSettingsWarning->m_iDeadlineTime;

	bAlternatingStripes = theApp.m_pDlgSettingsView->m_bAlternatingStriped;
	m_list.m_bUseSkin = theApp.m_pDlgSettingsView->m_bUseSkin;

	if (m_iMode == FLOATER_MODE_TASKS)
	{
		iSize = (int) pListFloater->size();
		iPos = 0;
		for (int iCount = 0; iCount < iSize; iCount++)
		{
			pFloaterItem = pListFloater->at(iPos);
			iPos++;
			m_list.InsertItem(0,"");
			m_list.SetItemText(0,0,pFloaterItem->m_sProjectName);
			double fVersion = pFloaterItem->m_iVersion;
			sFormat.Format("%.2f ", fVersion / 100);
			sFormat += pFloaterItem->m_sUserFriendly;
			if (pFloaterItem->m_sPlannClass.GetLength() > 0)
			{
 				sFormat += " (";
				sFormat += pFloaterItem->m_sPlannClass;
				sFormat += ")";
			}
			m_list.SetItemText(0,1,sFormat);

			sFormat.Format("%.3f", pFloaterItem->m_dPerc);
			char cComma = theApp.m_pDlgSettingsView->m_cComma;
			if (cComma != '.')
			{
				sFormat.Replace('.', cComma);
			}

			m_list.SetItemText(0,2,sFormat);
			m_list.SetItemText(0,3,pFloaterItem->m_sComputer);

			crColor = RGB(255,255,255);

			CUseCpuGpu CpuGpu;
			double	fCpuUse, fGpuUse;
			int		iGpu;
			bool	bCuda;

			CpuGpu.GetCpuGpu(pFloaterItem->m_sUse.GetBuffer(), &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
			pFloaterItem->m_sUse.ReleaseBuffer();

			if (pFloaterItem->m_iState == STATE_RUNNING)
			{
				if (bCuda)
				{
						crColor  = *(pColorsTasks + COLOR_TASKS_ARRAY_RUNNING_GPU);
				}
				else		crColor  = *(pColorsTasks + COLOR_TASKS_ARRAY_RUNNING);
			}
			if (pFloaterItem->m_iState == STATE_RUNNING_HP)	
			{
				crColor  = *(pColorsTasks + COLOR_TASKS_ARRAY_HIGH_PRIORITY);
			}
	
			// deadline 

			if (iDeadline > 0)			// deadline - dlgDeadlin - remaining time
			{
				time_t tTimeDeadline = (time_t) pFloaterItem->m_dReportDeadline;
				struct tm   tmtime;
				localtime_s(&tmtime, &tTimeDeadline );
				__int64 i64TimeDiff;
				time_t tSystemTime, tDeadline;
				_time64( &tSystemTime);
				tDeadline = tTimeDeadline - iDeadline*60;
				tDeadline-= (time_t) pFloaterItem->m_dRemainingTime;
				i64TimeDiff = tSystemTime - tDeadline;
				if (i64TimeDiff > 0) crColor  = *(pColorsWarnings + COLOR_WARNINGS_ARRAY_DEADLINE);
			}
			// deadline

			if (bAlternatingStripes)
			{
				bLineColor = !bLineColor;
				if (bLineColor)	crColor -= RGB(20,20,20);
			}
			m_list.SetItemData(0,crColor);
		}
	}

	if (m_iMode == FLOATER_MODE_COMPUTERS)
	{
		iSize = (int) pListFloater->size();
		iPos = 0;
		for (int iCount = 0; iCount < iSize; iCount++)
		{
			pFloaterItem = pListFloater->at(iPos);
			iPos++;
			m_list.InsertItem(0,"");
			m_list.SetItemText(0,0,pFloaterItem->m_sCpu);
			m_list.SetItemText(0,1,pFloaterItem->m_sGpu);
			m_list.SetItemText(0,2,"");
			m_list.SetItemText(0,3,pFloaterItem->m_sComputer);

			crColor = RGB(255,255,255);

			if (pFloaterItem->m_bWarning)
			{
				crColor = *(pColorsWarnings + COLOR_WARNINGS_ARRAY_TASKS);
			}

			if (bAlternatingStripes)
			{
				bLineColor = !bLineColor;
				if (bLineColor)	crColor -= RGB(20,20,20);
			}
			m_list.SetItemData(0,crColor);
		}
	}


	m_bRefreshBusy = false;
	return 0;
}

LRESULT CDlgFloating::OnSetFloater(WPARAM wParam,LPARAM lParam)
{
	int	iRadioSet;

	iRadioSet = (int) wParam;
	m_iRefreshTime = (int) lParam;

	if (iRadioSet != m_iRadio)
	{
		m_iRadio = iRadioSet;
		if (m_iRadio == RADIO_SHOW_ALWAYS)
		{
			this->ShowWindow(SW_SHOW);
			this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);//SWP_NOOWNERZORDER

			m_bOnTop = false;
		}
		else
		{
			this->SetWindowPos(&CWnd::wndTopMost,0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			m_bOnTop = true;
		}

		if (m_iRadio == RADIO_SHOW_ALWAYS_ON_TOP) this->ShowWindow(SW_SHOW);
	}

	return 0;
}

LRESULT CDlgFloating::OnSetFloaterMode(WPARAM wParam,LPARAM lParam)
{
	int iMode;

	iMode = (int) wParam;
	SetMode(iMode);

	return 0;
}

void CDlgFloating::OnMouseMove( UINT nFlags, CPoint point)
{
//	return;
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);

	CRect rect;
	GetClientRect(&rect);

	int iLeftOffset = rect.Width()/2 - point.x;
	int iTopOffset = rect.Height()/2 - point.y;

//	int iLeftOffset = rect.Width()/2 - point.x;
//	int iTopOffset = point.y - 10;

	int iLeft =wp.rcNormalPosition.left;
	int iTop = wp.rcNormalPosition.top;

	iLeft -= iLeftOffset;
	iTop -= iTopOffset;

	if (nFlags & MK_LBUTTON  )
	{

		m_list.ShowWindow(SW_HIDE);
		::SetCursor(::LoadCursor(NULL,IDC_SIZEALL));
		this->SetWindowPos(&CWnd::wndTopMost, iLeft, iTop, 0, 0,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSIZE);

		m_iMoveTimer = 0;
	}
	else
	{

	}
}

void CDlgFloating::OnLButtonDown(UINT nFlags,CPoint point)
{
	::SetCursor(::LoadCursor(NULL,IDC_SIZEALL));
}

void CDlgFloating::OnLButtonUp(UINT nFlags,CPoint point)
{
	m_bMoved = true;
	m_list.ShowWindow(SW_SHOW);
}

void CDlgFloating::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
	m_bMoved = true;
}

void CDlgFloating::OnSizing(UINT fwSide, LPRECT pRect)
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

	if (lWidth < 200)
	{
		bSetWindow = true;
		bSetWindowWidth = true;
	}
	if (lHeight < 50)
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
		pRect->bottom = pRect->top + 51;
	}

	CDialogEx::OnSizing(fwSide, pRect);
}

#define TOP_MARGIN	0
#define LEFT_MARGIN 0

void CDlgFloating::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
{
	LONG lTopMargin, lBottomMargin, lLeftMargin, lRightMargin, lWidthGraphic, lHeightGraphic;

//	switch (nType)
//	{
//		case SIZE_MAXIMIZED:
//			lTopMargin		= TOP_MARGIN;
//			lBottomMargin	= 10;
//			lLeftMargin		= 10+LEFT_MARGIN;
//			lRightMargin	= 5;
//			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
//			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
//		break;
//		case SIZE_RESTORED:
//			lTopMargin		= TOP_MARGIN;
//			lBottomMargin	= 10;
//			lLeftMargin		= 5+LEFT_MARGIN;
//			lRightMargin	= 5;
//			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
//			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
//		break;
//		default:
			lTopMargin		= TOP_MARGIN;
			lBottomMargin	= TOP_MARGIN;
			lLeftMargin		= LEFT_MARGIN;
			lRightMargin	= LEFT_MARGIN;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
//	}

// position graphic relative to window NOT the screen

	if (m_list.m_hWnd != NULL)
	{
		m_list.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
		m_list.Invalidate(TRUE);
	}

	// set timer to update window

	m_bUpdateWindow = true;

}

LRESULT CDlgFloating::OnChangedFont(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLp;
	pLp = (LOGFONT *) wParam;

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLp);

	m_list.SetFont(m_pFont,TRUE);
	m_list.Invalidate(TRUE);

	return 0;
}
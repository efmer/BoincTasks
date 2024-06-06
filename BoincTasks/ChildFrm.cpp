// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "BoincTasks.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)

	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	m_bDoOncePosition = true;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers


void CChildFrame::OnDestroy() 
{
	WINDOWPLACEMENT wp;


//	if (!m_bDoOncePosition)
	{
		// only when the window has been shown.
		GetWindowPlacement(&wp);

//;		SW_HIDE

		theApp.WriteProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowShow, wp.showCmd);
		theApp.WriteProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowBottom, wp.rcNormalPosition.bottom);
		theApp.WriteProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowLeft, wp.rcNormalPosition.left);
		theApp.WriteProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowRight, wp.rcNormalPosition.right);
		theApp.WriteProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowTop, wp.rcNormalPosition.top);
	}
	CMDIChildWnd::OnDestroy();  
}

void CChildFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	int iShow, iBottom, iLeft, iRight, iTop;

	CMDIChildWnd::OnShowWindow(bShow, nStatus);			// eerst daarna placement

	if (m_bDoOncePosition)
	{
		m_bDoOncePosition = false;
	
		GetWindowText(m_sWindowName);

		if (m_sWindowName == INITIAL_FILE_OPEN_NAME)
		{
			WINDOWPLACEMENT wp;							// always open maximized
			GetWindowPlacement(&wp);
			wp.showCmd = SW_SHOWMAXIMIZED   ;
			SetWindowPlacement(&wp);
			return;
		}

		int iPos = m_sWindowName.Find(',',0);
		if (iPos > 0)
		{
			m_sWindowName = m_sWindowName.Left(iPos);
		}

		m_sWindowName.Replace(" ","");	// remove space

		iShow = theApp.GetProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowShow, -1);
		iBottom = theApp.GetProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowBottom, -1);
		iLeft = theApp.GetProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowLeft, -1);
		iRight = theApp.GetProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowRight, -1);
		iTop = theApp.GetProfileInt(registrySectionWindowPosition, m_sWindowName + registryPositionWindowTop, -1);

		if ((iShow >= 0) && (iBottom >= 0) && (iTop >= 0))
		{
			WINDOWPLACEMENT wp;
			GetWindowPlacement(&wp);
			wp.showCmd = iShow;
			wp.rcNormalPosition.bottom = iBottom;
			wp.rcNormalPosition.left = iLeft;
			wp.rcNormalPosition.right = iRight;
			wp.rcNormalPosition.top = iTop;
			SetWindowPlacement(&wp);
		}
	}
}

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
    if(nID == SC_CLOSE) return;
    if(nID == SC_MAXIMIZE) return;
    if(nID == SC_MINIMIZE) return;
    if(nID == SC_RESTORE) return;
    CMDIChildWnd::OnSysCommand(nID, lParam);
}

/*

	HMENU hMenu;

	hMenu = GetSystemMenu(this->m_pComputerView->m_hWnd, false);
	EnableMenuItem(hMenu, SC_CLOSE, MF_REMOVE);
	EnableMenuItem(hMenu, SC_MAXIMIZE, MF_GRAYED);
	EnableMenuItem(hMenu, SC_MINIMIZE, MF_GRAYED);
	*/
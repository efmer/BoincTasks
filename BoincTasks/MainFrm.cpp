// MainFFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "afxadv.h"
#include "afxpriv.h"
#include "Tlhelp32.h"
#include "Wtsapi32.h"

#include "BoincTasks.h"

#include "MainFrm.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"
#include "ThreadGarbageCollector.h"
#include "ListViewEx.h"					// base class
#include "DlgUpdate.h"
#include "AboutDlg.h"
#include "DlgFloating.h"
#include "DlgSettingsGadget.h"
#include "TemplateRemoteTaskView.h"		// base class for BoinCComputerView
#include "RemoteCombinedDoc.h"
#include "BoincClientManager.h"
#include "TimeString.h"
#include "Translation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnNewTaskbarCreated)
	ON_MESSAGE(UWM_TASKBAR_ACTIVATE, OnActivateTaskbar)

	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()

	ON_MESSAGE(UWM_MSG_DOCUMENT_IS_CLOSING,OnDocumentIsClosing)
	ON_MESSAGE(UWM_MSG_DOCUMENT_HAS_CLOSED,OnDocumentHasClosed)


	ON_MESSAGE(UWM_SET_THREAD_PRIORITY, OnThreadPriority)

	ON_MESSAGE(UWM_MSG_WUS_THAT_ARE_READY, OnWUsThatAreReady)

	ON_MESSAGE(UWM_MSG_GET_FLOATER_DATA_READY, OnGetFloaterDataReady)
	ON_MESSAGE(UWM_MSG_SET_FLOATER, OnSetFloater)
	ON_MESSAGE(UWM_MSG_SET_SNOOZE_CPU_READY, OnSetSnoozeCpuReady)
	ON_MESSAGE(UWM_MSG_SET_SNOOZE_GPU_READY, OnSetSnoozeGpuReady)
	ON_MESSAGE(UWM_MSG_THREAD_SET_SNOOZE_CPU_READY, OnSetSnoozeCpuReady)
	ON_MESSAGE(UWM_MSG_THREAD_SET_SNOOZE_GPU_READY, OnSetSnoozeGpuReady)

	ON_MESSAGE(WM_TASKBAR,OnTaskbar)

	ON_MESSAGE(UWM_MSG_THREAD_ID,OnThreadId)

	ON_MESSAGE(UWM_MSG_FORCE_A_RESTART,OnForceRestart)

	ON_MESSAGE(UWM_MSG_THREAD_GARBAGE_COLLECTOR, onGarbageCollector)
	ON_MESSAGE(UWM_MSG_THREAD_GARBAGE_COLLECTOR_READY, onGarbageCollectorReady)

//	ON_MESSAGE(UWM_MSG_THREAD_THREAD_CONNECT_READY, OnThreadThreadConnectReady)

	ON_COMMAND(ID_TASKBAR_OPEN, &CMainFrame::OnTaskbarOpen)
	ON_COMMAND(ID_TASKBAR_UPDATE, &CMainFrame::OnTaskbarUpdate)
	ON_COMMAND(ID_TASKBAR_ABOUT, &CMainFrame::OnTaskbarAbout)
	ON_COMMAND(ID_TASKBAR_EXIT, &CMainFrame::OnTaskbarExit)

	ON_MESSAGE(UWM_MSG_EXIT, OnExit)


	ON_COMMAND(ID_TASKBAR_SNOOZE_CPU, &CMainFrame::OnTaskbarSnoozeCpu)
	ON_COMMAND(ID_TASKBAR_SNOOZE_GPU, &CMainFrame::OnTaskbarSnoozeGpu)

	ON_MESSAGE(UWM_MSG_NOTICES_ALERT, OnNoticesAlert)

	ON_COMMAND(ID_COMPUTERS_SHOW, &CMainFrame::OnComputersShow)

	ON_COMMAND(ID_COMPUTERS_SHOW, &CMainFrame::OnComputersShow)

	ON_UPDATE_COMMAND_UI(ID_COMPUTERS_SHOW,&CMainFrame::OnUpdateComputers)

//	ON_MESSAGE(WM_VIEW_TILE,OnViewTile)

ON_COMMAND(ID_VIEW_VIEWSELECTIONBAR, &CMainFrame::OnViewViewselectionbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWSELECTIONBAR, &CMainFrame::OnUpdateViewViewselectionbar)
ON_COMMAND(ID_VIEW_OPERATION_TOOLBAR, &CMainFrame::OnViewOperationToolbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_OPERATION_TOOLBAR, &CMainFrame::OnUpdateViewOperationToolbar)
ON_COMMAND(ID_VIEW_TOOLBARALLOW, &CMainFrame::OnViewAllowToolbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARALLOW, &CMainFrame::OnUpdateViewAllowToolbar)

ON_COMMAND(ID_VIEW_GRAPHIC_TOOLBAR, &CMainFrame::OnViewGraphicToolbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPHIC_TOOLBAR, &CMainFrame::OnUpdateViewGraphicToolbar)

ON_UPDATE_COMMAND_UI(ID_TASKBAR_SNOOZE_CPU, &CMainFrame::OnUpdateTaskbarSnoozeCpu)
ON_UPDATE_COMMAND_UI(ID_TASKBAR_SNOOZE_GPU, &CMainFrame::OnUpdateTaskbarSnoozeGpu)

ON_MESSAGE(WM_WTSSESSION_CHANGE, OnWtsSessionChange)

ON_WM_ENDSESSION()
ON_WM_QUERYENDSESSION()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // BoincTasks status
//	IDS_STATUS_EXTRA,
	IDS_PROGRAM_VERSION_PREFIX,
	IDS_PROGRAM_VERSION,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bIsClosing = false;
	m_bDoOnce = false;
	m_bDoOncePosition = true;
	m_iLocalHostThreadId = -1;
	m_sLocalHostThreadId = "";
	m_tGetToolTipData = 0;
	m_iGetTooltipData = 0;
	m_bFloaterOverIcon = false;
	m_iFloaterTime = 0;
	m_iFloaterHideTime = 5;
	m_iFloaterHideOverride = 5;	
	m_bFloaterShown = false;
	m_tFloaterFirst = 0;
	m_bSnoozeCpu = false;
	m_bSnoozeCpuEnable = false;
	m_bSnoozeGpu = false;
	m_bSnoozeGpuEnable = false;
	m_iCurrentIcon = TASKBAR_ICON_NORMAL;
	m_iBlinkIcon = 0;
	m_iRedrawIconCounter = -1;
	m_bCursorIsOverIcon = false;
	m_iSecondTimer = 0;
	m_iBalloonTimeout = 0;
	m_tNextSnoozeUpdate = 0;

	m_iHistoryStateReadyCount = 0;

	m_iResized = 1;

	m_pLogging = new CLoggingMessage;
	m_pcBufferLogFrom = new char [64];

	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_MAINFRAME),RT_GROUP_ICON);
	m_hIconRed = LoadIcon(hInst,MAKEINTRESOURCE(IDR_BoincTasksSuspend));
	m_hIconOrange = LoadIcon(hInst,MAKEINTRESOURCE(IDR_BoincTasksSuspendGpu));
	m_hIconNormal = LoadIcon(hInst,MAKEINTRESOURCE(IDR_MAINFRAME));

	m_bIconStatus = TRUE;	// moet true zijn, de eerste keer moet hij pas bij cdoc gemaakt worden.
	m_iIconCounter = 0;

	m_bForceRestart = false;

	m_bQueryClosingDown = false;
}

CMainFrame::~CMainFrame()
{
	RemoveToolbar();
	delete m_pLogging;
	delete m_pcBufferLogFrom;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString sVersion ;

	RefreshTaskbarNotificationArea();

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// toolbar graphic
	if (!m_wndToolBarGraphic.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), ID_TOOL_TOOLBAR1) ||  !m_wndToolBarGraphic.LoadToolBar(IDR_TOOL_BOINCTASKS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	CBitmap bm, bmDisabled;
	bm.Attach (::LoadImage (::AfxFindResourceHandle(MAKEINTRESOURCE (IDB_TOOLBAR_COMPUTERS), RT_BITMAP), MAKEINTRESOURCE (IDB_TOOLBAR_COMPUTERS), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION)));
	ReplaceBackgroundColor(bm);
	bmDisabled.Attach (::LoadImage (::AfxFindResourceHandle(MAKEINTRESOURCE (IDB_TOOLBAR_COMPUTERS_DISABLED), RT_BITMAP), MAKEINTRESOURCE (IDB_TOOLBAR_COMPUTERS_DISABLED), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION)));
	ReplaceBackgroundColor(bmDisabled);

	m_imagelistToolbar.Create(16, 16, ILC_COLOR24, 6, RGB(255,255,255));
	m_imagelistToolbar.Add(&bm, (CBitmap*) NULL);
	m_imagelistToolbarDisabled.Create(16, 16, ILC_COLOR24, 6, RGB(255,255,255));
	m_imagelistToolbarDisabled.Add(&bmDisabled, (CBitmap*) NULL);

	m_wndToolBarGraphic.GetToolBarCtrl().SetImageList(&m_imagelistToolbar);
	m_wndToolBarGraphic.GetToolBarCtrl().SetDisabledImageList(&m_imagelistToolbarDisabled);
	m_wndToolBarGraphic.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBarGraphic);
	// toolbar graphic

	// toolbar operations resume, suspend, update, abort
	if (!m_wndToolBarOperations.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), ID_TOOL_TOOLBAR3) ||  !m_wndToolBarOperations.LoadToolBar(IDR_TOOL_BOINCTASKS_OPERATIONS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CBitmap bmOperations, bmOperationsDisabled;
	bmOperations.Attach (::LoadImage (::AfxFindResourceHandle(MAKEINTRESOURCE (IDB_TOOLBAR_OPERATIONS24), RT_BITMAP), MAKEINTRESOURCE (IDB_TOOLBAR_OPERATIONS24), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION)));
	ReplaceBackgroundColor(bmOperations);
	bmOperationsDisabled.Attach (::LoadImage (::AfxFindResourceHandle(MAKEINTRESOURCE (IDB_TOOLBAR_OPERATIONS_DISABLED24), RT_BITMAP), MAKEINTRESOURCE (IDB_TOOLBAR_OPERATIONS_DISABLED24), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION)));
	ReplaceBackgroundColor(bmOperationsDisabled);

	m_imagelistToolbarOperations.Create(16, 16, ILC_COLOR24, 8, RGB(255,255,255));
	m_imagelistToolbarOperations.Add(&bmOperations, (CBitmap*) NULL);
	m_imagelistToolbarOperationsDisabled.Create(16, 16, ILC_COLOR24, 8, RGB(255,255,255));
	m_imagelistToolbarOperationsDisabled.Add(&bmOperationsDisabled, (CBitmap*) NULL);

	m_wndToolBarOperations.GetToolBarCtrl().SetImageList(&m_imagelistToolbarOperations);
	m_wndToolBarOperations.GetToolBarCtrl().SetDisabledImageList(&m_imagelistToolbarOperationsDisabled);

	m_wndToolBarOperations.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarLeftOf(&m_wndToolBarOperations, &m_wndToolBarGraphic);
	// toolbar operations resume, suspend, update, abort

	// toolbar allow
	if (!m_wndToolBarAllow.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), ID_TOOL_TOOLBAR2) ||  !m_wndToolBarAllow.LoadToolBar(IDR_TOOL_BOINCTASKS_ALLOW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CBitmap bmAllow, bmAllowDisabled;
	bmAllow.Attach (::LoadImage (::AfxFindResourceHandle(MAKEINTRESOURCE (IDB_TOOLBAR_ALLOW24), RT_BITMAP), MAKEINTRESOURCE (IDB_TOOLBAR_ALLOW24), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION)));
	ReplaceBackgroundColor(bmAllow);
	bmAllowDisabled.Attach (::LoadImage (::AfxFindResourceHandle(MAKEINTRESOURCE (IDB_TOOLBAR_ALLOW_DISABLED24), RT_BITMAP), MAKEINTRESOURCE (IDB_TOOLBAR_ALLOW_DISABLED24), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION)));
	ReplaceBackgroundColor(bmAllowDisabled);

	m_imagelistToolbarAllow.Create(16, 16, ILC_COLOR24, 8, RGB(255,255,255));
	m_imagelistToolbarAllow.Add(&bmAllow, (CBitmap*) NULL);
	m_imagelistToolbarAllowDisabled.Create(16, 16, ILC_COLOR24, 8, RGB(255,255,255));
	m_imagelistToolbarAllowDisabled.Add(&bmAllowDisabled, (CBitmap*) NULL);

	m_wndToolBarAllow.GetToolBarCtrl().SetImageList(&m_imagelistToolbarAllow);
	m_wndToolBarAllow.GetToolBarCtrl().SetDisabledImageList(&m_imagelistToolbarAllowDisabled);

	m_wndToolBarAllow.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarLeftOf(&m_wndToolBarAllow, &m_wndToolBarOperations);
	// toolbar allow

	// view select toolbar
	if (!m_dlgBarViewSelect.Create(this, IDD_DIALOGBAR_TABS,	CBRS_TOP | CBRS_GRIPPER |CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE, IDD_VIEW_DIALOGBAR_TABVIEW))
		{
			TRACE0("Failed to create dialog bar computer\n");
			return -1;		// fail to create
		}

	m_dlgBarViewSelect.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_TOP);
	DockControlBarLeftOf(&m_dlgBarViewSelect, &m_wndToolBarAllow);

	// computer toolbar
	if (!m_dlgBarComputer.Create(this, IDD_DIALOGBAR_COMPUTER, CBRS_LEFT | CBRS_SIZE_DYNAMIC, IDD_VIEW_DIALOGBAR_COMPUTER))
    {
       TRACE0("Failed to create dialogbar\n");
       return -1;
    }
	m_dlgBarComputer.Invalidate(TRUE);

	m_dlgBarComputer.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
//	m_dlgBarComputer.
	EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(&m_dlgBarComputer);

	bool bVersion153 = false;
	if (VerifyBarState("dock_state", 153) == TRUE)		// check if 153 is valid.....
	{
		CDockState dockState;
		dockState.LoadState("dock_state");
		this->SetDockState(dockState);
		bVersion153 = true;
	}

	// project toolbar
	if (!m_dlgBarProject.Create(this, IDD_DIALOGBAR_PROJECT, CBRS_LEFT | CBRS_SIZE_DYNAMIC, IDD_VIEW_DIALOGBAR_PROJECT))
    {
       TRACE0("Failed to create dialogbar\n");
       return -1;
    }
	m_dlgBarProject.Invalidate(TRUE);

	m_dlgBarProject.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_RIGHT);
	DockControlBar(&m_dlgBarProject);

	if (bVersion153)	
	{
		m_dlgBarProject.ShowWindow(SW_HIDE);	// Version 153 does have a valid dock state.
	}

	if (VerifyBarState("dock_state",0) == TRUE)		// only get the state when its valid
	{
		CDockState dockState;
		dockState.LoadState("dock_state");
		this->SetDockState(dockState);
	}
	else
	{
			AfxMessageBox("Sorry... The toolbar and other bars, could not be restored to their original position.");
	}
 
	sVersion.LoadString(IDS_PROGRAM_VERSION);
	m_strTitle += " ";
	m_strTitle += sVersion;

	BOOL bSuccess;

	bSuccess = WTSRegisterSessionNotification(this->m_hWnd,__in  NOTIFY_FOR_THIS_SESSION);
	
	if (bSuccess == FALSE) 
	{
		Logging(USER_SWITCHING, "Unable to detect user switching", LOGGING_NORMAL);	
	}

	return 0;
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;
	
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);
	
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::DockControlBarLeftOf(CDlgBarViewSelect* Bar, CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;
	
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);
	
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::ReplaceBackgroundColor (CBitmap& ioBM)
{
	const int	kImageWidth (16);
	const int	kImageHeight (15);
	const int	kNumImages (8);

	const UINT	kToolBarBitDepth (ILC_COLOR24);

	const RGBTRIPLE	kBackgroundColor = {255, 255, 255};

  // figure out how many pixels there are in the bitmap
	BITMAP		bmInfo;

	VERIFY (ioBM.GetBitmap (&bmInfo));

 // add support for additional bit depths if you choose
	VERIFY (bmInfo.bmBitsPixel == 24);
	VERIFY (bmInfo.bmWidthBytes == (bmInfo.bmWidth * 3));

	const UINT		numPixels (bmInfo.bmHeight * bmInfo.bmWidth);

	// get a pointer to the pixels
	DIBSECTION  ds;

	VERIFY (ioBM.GetObject (sizeof (DIBSECTION), &ds) == sizeof (DIBSECTION));

	RGBTRIPLE*		pixels = reinterpret_cast<RGBTRIPLE*>(ds.dsBm.bmBits);
	VERIFY (pixels != NULL);

 // get the user's preferred button color from the system
	const COLORREF		buttonColor (::GetSysColor (COLOR_BTNFACE));
	const RGBTRIPLE		userBackgroundColor = {
	GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};

 // search through the pixels, substituting the user's button
 // color for any pixel that has the magic background color
	for (UINT i = 0; i < numPixels; ++i)
	{
		if (pixels [i].rgbtBlue == kBackgroundColor.rgbtBlue && pixels [i].rgbtGreen == kBackgroundColor.rgbtGreen && pixels [i].rgbtRed == kBackgroundColor.rgbtRed)
		{
			pixels [i] = userBackgroundColor;
		}
	}
}

// allow the messages to reach us
void CMainFrame::ChangeMessageFilter()
{
	LPFN_CHANGEWINDOWMESSAGEFILTER fnChangeWindowMessageFilter;
	fnChangeWindowMessageFilter =   (LPFN_CHANGEWINDOWMESSAGEFILTER)GetProcAddress(GetModuleHandle(TEXT("user32")),"ChangeWindowMessageFilter"); 

	if (fnChangeWindowMessageFilter != NULL)
	{
		BOOL bFlag = fnChangeWindowMessageFilter((UINT) WM_TASKBARCREATED, MSGFLT_ADD);
		if (bFlag == FALSE)
		{
//			AfxMessageBox("BoincTasks: filter FALSE");
		}
	}
}

BOOL CMainFrame::PreTranslateMessage(MSG *pMsg)
{
//	if (pMsg->message == theApp.WM_TASKBARCREATED)
//	{
//			AfxMessageBox("WM_TASKBARCREATED 1");
//	}
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::SetBoincTasksStatus(char *pszStatus)
{
	if (g_pThreadWebServer != NULL)
	{
		if (g_bWebServerActive)
		{
			CString *psWebServer;
			psWebServer = new CString;
			*psWebServer = pszStatus;
			g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_STATUS,0,(LPARAM) psWebServer);
		}
	}
	if (g_pThreadCloudServer != NULL)
	{
		if (g_bCloudServerRequestData)
		{
			CString *psCloudServer;
			psCloudServer = new CString;
			*psCloudServer = pszStatus;
			g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_STATUS,0,(LPARAM) psCloudServer);
		}
	}

	m_wndStatusBar.SetPaneText(1,pszStatus,TRUE);
}

CStatusBar* CMainFrame::GetBoincTasksStatus()
{
	return &m_wndStatusBar;
}

void CMainFrame::Logging(char *pcFrom, char *pcTxt, int iLoggingType)
{
	strcpy_s(m_pcBufferLogFrom, 63, pcFrom);
	m_pLogging->m_pFrom = m_pcBufferLogFrom;
	m_pLogging->m_pText = pcTxt;
	theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLogging, iLoggingType);
}

BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName, int iVersion)
{
	CControlBarInfo* pInfo;
    CDockState state;
	int iDockedCount;
	state.LoadState(lpszProfileName);


    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
		pInfo = (CControlBarInfo*) state.m_arrBarInfo[i];

//        ASSERT(pInfo != NULL);
		if (pInfo == NULL)
		{
			return FALSE;
		}
		
		iDockedCount = (int) pInfo->m_arrBarID.GetSize();
        if (iDockedCount > 0)
        {
            // dockbar

            for (int j = 0; j < iDockedCount; j++)
            {
                INT_PTR nID = (INT_PTR) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator

                if (nID > 0xFFFF) nID &= 0xFFFF; // placeholder - get the ID


				if ((nID < ID_FIRST_TOOLBAR) || (nID > ID_LAST_TOOLBAR))
				{
					if (nID < AFX_IDW_TOOLBAR)
					{
						return FALSE;	// not the correct toolbar
					}
				}

	            if (GetControlBar((UINT) nID) == NULL)
				{
					return FALSE;
				}
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
		{
            if (GetControlBar(pInfo->m_nBarID) == NULL)
			{
                return FALSE; // invalid bar ID
			}
		}

    }

    return TRUE;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

//BOOL CMainFrame::AddToComputersToolbar(char *szStrings, int buttons)
//{//
//
//	return TRUE;
//}

void CMainFrame::OnClose()
{
	SafeToolBar();
	theApp.m_pMainWnd->ShowWindow(SW_HIDE);
}

void CMainFrame::OnDestroy() 
{
	WINDOWPLACEMENT wp;

	if (!m_bDoOncePosition)
	{
		// only when the window has been shown.
		GetWindowPlacement(&wp);

		theApp.WriteProfileInt(registrySectionWindowPosition, registryPositionMainShow, wp.showCmd);
		theApp.WriteProfileInt(registrySectionWindowPosition, registryPositionMainBottom, wp.rcNormalPosition.bottom);
		theApp.WriteProfileInt(registrySectionWindowPosition, registryPositionMainLeft, wp.rcNormalPosition.left);
		theApp.WriteProfileInt(registrySectionWindowPosition, registryPositionMainRight, wp.rcNormalPosition.right);
		theApp.WriteProfileInt(registrySectionWindowPosition, registryPositionMainTop, wp.rcNormalPosition.top);
	}

	WTSUnRegisterSessionNotification(this->m_hWnd);

	CMDIFrameWnd::OnDestroy();  
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	int iShow, iBottom, iLeft, iRight, iTop;

	if (m_bDoOncePosition)
	{
		m_bDoOncePosition = false;

		iShow = theApp.GetProfileInt(registrySectionWindowPosition, registryPositionMainShow, -1);
		iBottom = theApp.GetProfileInt(registrySectionWindowPosition, registryPositionMainBottom, -1);
		iLeft = theApp.GetProfileInt(registrySectionWindowPosition, registryPositionMainLeft, -1);
		iRight = theApp.GetProfileInt(registrySectionWindowPosition, registryPositionMainRight, -1);
		iTop = theApp.GetProfileInt(registrySectionWindowPosition, registryPositionMainTop, -1);

		if ((iShow >= 0) && (iBottom >= 0) && (iTop >= 0))
		{
			if (iShow == SW_SHOWMINIMIZED)
			{
				iShow = SW_NORMAL;		// never start minimized
			}

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

	if (bShow)
	{
		SendWindowShow();
	}


	CMDIFrameWnd::OnShowWindow(bShow,nStatus);

	// first time hide allow toolbar
	int iForceToHide;
	iForceToHide = theApp.GetProfileInt(registrySectionGeneral, registryHideAllowToolbar, 1);
	theApp.WriteProfileInt(registrySectionGeneral, registryHideAllowToolbar, 0);

	if (iForceToHide)
	{
		// hide the bar the first time
		OnViewAllowToolbar();
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy )
{
	if (nType == SIZE_RESTORED)
	{
		SendWindowShow();
	}

	m_iResized = 1;						// redraw timer

	CMDIFrameWnd::OnSize(nType, cx, cy );

}

void CMainFrame::SendWindowShow()
{
	CView		*pView;
	CDocument	*pDoc;
	POSITION	posDoc, posView;

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
				pView->PostMessage(UWM_MSG_REMOTE_WINDOW_SHOWN, 0,0);
			}
		}
	}
}


void CMainFrame::SafeToolBar()
{
	CDockState dockState;
	this->GetDockState(dockState);
	dockState.SaveState("dock_state");
}

void CMainFrame::CloseAllRemote()
{	
	POSITION	pos;
	CRemoteCombinedDoc *pDocCombined;

	if (m_bIsClosing) return;

	m_iClosingCount = 0;
	pos = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (pos)
	{
		pDocCombined = (CRemoteCombinedDoc*) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(pos);
		pDocCombined->OnDocumentIsClosing();
		m_iClosingCount++;
	}
	m_bIsClosing = true;
}

bool CMainFrame::SetPriority(DWORD dwOwnerPID, DWORD dwPriority) 
{ 
	bool			bReturn;
    HANDLE			hThreadSnap = NULL; 
	HANDLE			hThread;
    THREADENTRY32	thread32        = {0}; 
	int				iError;
//	DWORD			threadCount;
	int				iNrThreadsFound;
 
	iNrThreadsFound = 0;
	bReturn = true;

    // Get all running threads. 
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
    if (hThreadSnap == INVALID_HANDLE_VALUE) 
	{
        return false; 
	}

    thread32.dwSize = sizeof(THREADENTRY32); 
 
	// Search for any threads
	// Suspend of Resume any matching threads

    if (Thread32First(hThreadSnap, &thread32)) 
    { 
        do 
        { 
            if (thread32.th32OwnerProcessID == dwOwnerPID) 
            {
				 // process is running

//				EnablePrivilege(SE_DEBUG_NAME, TRUE);
				hThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION | THREAD_SET_INFORMATION, FALSE, thread32.th32ThreadID); //MAXIMUM_ALLOWED
				if (hThread == NULL)
				{
					iError = GetLastError();
					bReturn = false;
				}
				else
				{
					::SetThreadPriority(hThread, dwPriority);
				}
            } 
        }
        while (Thread32Next(hThreadSnap, &thread32)); 
    } 
    else 
	{
	}

    CloseHandle (hThreadSnap); 
 
	if (iNrThreadsFound == 0)
	{
		bReturn = false;			// no thread found
	}

    return bReturn; 
} 

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG



// CMainFrame message handlers

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	CDocument	*pDoc;
	CView		*pView;
	POSITION	posDoc, posView;

	if (m_bCursorIsOverIcon)
	{
		CPoint point;
		GetCursorPos( &point );
		if (!m_rectOverIcon.PtInRect(point))
		{
			m_bCursorIsOverIcon = false;
			m_tFloaterFirst = 0;
		}
	}

	// 1/10 second interval

	if (m_bCursorIsOverIcon)
	{
		time_t	tSystemTime;
		_time64(&tSystemTime);
		if (tSystemTime > m_tNextSnoozeUpdate)
		{
			m_tNextSnoozeUpdate = tSystemTime + 4;	// every 4 seconds
			::PostThreadMessage(m_iLocalHostThreadId,UWM_MSG_THREAD_GET_SNOOZE_CPU_GPU,(WPARAM) this->m_hWnd, (LPARAM) TRUE);
		}

		if (m_bIconStatus == TRUE)
		{
			if (m_bFloaterOverIcon)						// show floating window over icon
			{
				if (m_iFloaterHideTime >0)				// but only when time > 0
				{
					if (m_iFloaterHideOverride == 0)	// don't show when in hide timer
					{
						if (!m_bFloaterShown)
						{
							if (m_tFloaterFirst == 0)
							{
								m_tFloaterFirst = tSystemTime;
								m_tFloaterFirst += theApp.m_pDlgSettingsGadget->m_iTimeAfter;
							}
						}
						if (m_tFloaterFirst <= tSystemTime)
						{
							posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
							if (posDoc != NULL)				// only when combined document is active
							{
								m_tFloaterFirst = tSystemTime + 1;									// not within 1 second
								theApp.m_pDlgFloating->ShowWindow(SW_SHOW);
// not is in onshow now							theApp.m_pDlgFloating->SendMessage(UWM_MSG_GET_FLOATER_DATA,0,0);	// update data in floating window.
								m_iFloaterTime = 0;
								m_bFloaterShown = true;
//								bShowFloater = true;
							}
						}
					}
				}
			}
		}
	} 


	if (m_iSecondTimer++ < 10)
	{
		CFrameWnd::OnTimer(nIDEvent);
		return;
	}

	if (!m_bIconStatus)								// something wrong with the icon, show it again.
	{
		switch (m_iIconCounter)
		{
			case 1:
				RemoveToolbar();
			break;
			case 4:
				WriteToolbar();
			break;
		}
		m_iIconCounter++;
		if (m_iIconCounter > 10) m_iIconCounter = 0;
	}


	if (m_iBalloonTimeout > 0)
	{
		m_iBalloonTimeout--;
		if (m_iBalloonTimeout == 0)
		{
			ToolbarShowBalloon("", "", NIIF_INFO, 0);			// remove the balloon, the timer fails on Win 7....
		}
	}

	m_iSecondTimer = 0;
	// 1 second interval

	// redraw the background after resizing, to remove stripint in the unused area's of the toolbars.
	if (m_iResized == 0)
	{
		theApp.m_pMainWnd->Invalidate(TRUE);
		m_iResized--;
	}
	else
	{
		if (m_iResized > 0)
		{
			m_iResized--;
		}
	}


	if (m_iFloaterHideOverride > 0)	m_iFloaterHideOverride--;
	else m_iFloaterHideOverride = 0;

	if (m_bFloaterShown)
	{
		if (m_iFloaterTime++ >= m_iFloaterHideTime)
		{
			theApp.m_pDlgFloating->ShowWindow(SW_HIDE);
			m_tFloaterFirst = 0;
			m_bFloaterShown = false;
		}
	}

//	if (theApp.m_bRebootRemoteCombined)
//	{
//		CloseAllRemote(); // closed so reboot everything
//	}

	// blink icon when suspended

	if (m_bSnoozeCpu || m_bSnoozeGpu)
	{
		if (m_iBlinkIcon > 0)
		{
			SetTaskbarIcon(TASKBAR_ICON_NORMAL);
		}
		else
		{
			if (m_bSnoozeCpu) SetTaskbarIcon(TASKBAR_ICON_SUSPEND);
			if (m_bSnoozeGpu) SetTaskbarIcon(TASKBAR_ICON_SUSPEND_GPU);
		}
		m_iBlinkIcon++;
		if (m_iBlinkIcon > 1) m_iBlinkIcon = 0;
	}
	else
	{
		SetTaskbarIcon(TASKBAR_ICON_NORMAL);
	}

	if (m_iRedrawIconCounter > 0)
	{
		m_iRedrawIconCounter--;
		if (m_iRedrawIconCounter == 4)
		{
			RemoveToolbar();
			WriteToolbar();
		}

		if (m_iRedrawIconCounter == 1)
		{
			RemoveToolbar();
			WriteToolbar();
		}

	}

	// get tooltip data every 120 seconds.

	if (m_iGetTooltipData++ > 120)
	{
		time_t	tSystemTime;
		_time64(&tSystemTime);
		if (m_tGetToolTipData <= tSystemTime)
		{
			m_tGetToolTipData = tSystemTime + 2;									// not within 2 seconds
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
						pView->PostMessage(UWM_MSG_GET_FLOATER_DATA, (WPARAM) FLOATER_MODE_UNDEFINED, (LPARAM) FLOATER_TOOLTIP_INFO);
						m_iGetTooltipData = 0;
					}
				}
			}
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

//m_pDocTemplateComputerCombinedRemote


///////////////////////////////////////////////////////////////////////////////////
// Taskbar
///////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumChildProc(__in  HWND hwnd, __in  LPARAM lParam)
{
    RECT r;
	char cBuffer[256]; 
	if (GetClassName(hwnd,  cBuffer, 255))
	{
		if (strcmp(cBuffer, "MSTaskListWClass") == 0)
		{
			int ii = 2;
		}
		if (strcmp(cBuffer, "ToolbarWindow32") == 0)
		{
			::GetClientRect(hwnd,&r);
  			for (LONG x = 0; x < r.right; x += 5)
			{
				for (LONG y = 0; y < r.bottom; y += 5)
				{
					::SendMessage(hwnd,WM_MOUSEMOVE, 0, (y << 16) + x);
				}
			}
		}
	}
	return true;
}

void CMainFrame::RefreshTaskbarNotificationArea()
{
	HWND hTray = ::FindWindowEx( NULL, 0, "Shell_TrayWnd", NULL);
	if (hTray == NULL) return;
	HWND hTrayNotify = ::FindWindowEx(hTray, 0, "TrayNotifyWnd", NULL);
	if (hTrayNotify == NULL) return;
	EnumChildWindows(hTrayNotify,EnumChildProc,  0);
}

void CMainFrame::WriteToolbar()
{
	// handle to icon
	
	CString sIDS;
	CString sTip;
	
	// text for tool tip
	
	sTip.LoadString(IDS_TOOLBAR_TIP);
	sTip += " ";
	sIDS.LoadString(IDS_PROGRAM_VERSION);
	sTip += sIDS;
	    	
	// set NOTIFYCONDATA structure	
 
    m_tnid.cbSize = sizeof(NOTIFYICONDATA); 
	m_tnid.hWnd = m_hWnd;
    m_tnid.uID = IDR_MAINFRAME; 
    m_tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
    m_tnid.uCallbackMessage = WM_TASKBAR; // my user message 

	m_tnid.hIcon = m_hIconNormal;											// LoadIcon(hInst,MAKEINTRESOURCE(IDR_MAINFRAME)); 
    			
	lstrcpyn(m_tnid.szTip, sTip.GetBuffer(), sizeof(m_tnid.szTip)); 
  
    ////////////////////////////////////////////////////////////////	  

	// call to Shell_NotifyIcon with NIM_ADD parameter
	
	m_bIconStatus = Shell_NotifyIcon(NIM_ADD, &m_tnid); 
 
    // free icon 
	
//	if (hIcon) DestroyIcon(hIcon); 

}

LRESULT  CMainFrame::OnActivateTaskbar(WPARAM wParam,LPARAM lParam)
{
	WriteToolbar();

	WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
	ChangeMessageFilter();
	
	return 0;
}

LRESULT CMainFrame::OnNewTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
	m_iRedrawIconCounter = 20;
//	RemoveToolbar();
//	WriteToolbar();
//	AfxMessageBox("WM_TASKBARCREATED OnNewTaskbarCreated");
	return 0;
}

void CMainFrame::SetTaskbarIcon(int iIcon)
{
	BOOL bStatus;

	if (m_iCurrentIcon != iIcon)
	{
		m_iCurrentIcon = iIcon;

		switch(iIcon)
		{
			case TASKBAR_ICON_SUSPEND:
				m_tnid.hIcon = m_hIconRed; 
			break;
			case TASKBAR_ICON_SUSPEND_GPU:
				m_tnid.hIcon = m_hIconOrange; 
			break;
			default:
				m_tnid.hIcon = m_hIconNormal; 
		}
		m_tnid.uFlags = NIF_ICON; 
		bStatus = Shell_NotifyIcon(NIM_MODIFY, &m_tnid);	// TRUE = OK
		if (bStatus == FALSE) m_bIconStatus = FALSE;
	}
}

void CMainFrame::TaskbarMenu()
{
	CString sIDS;

	CMenu taskbarMenu;
	CMenu* pTracker; 

	taskbarMenu.LoadMenu(IDR_MENU_TASKBAR);

	pTracker = taskbarMenu.GetSubMenu(0); 

// translate

	if (m_iHistoryStateReadyCount > 0)
	{
		sIDS.Format(" %d", m_iHistoryStateReadyCount);
		sIDS = gszTranslation[PosGroupMenuExtraUpdateAll] + sIDS;
		pTracker->ModifyMenu(ID_EXTRA_DONETWORKCOMMUNICATION, MF_STRING, ID_EXTRA_DONETWORKCOMMUNICATION, sIDS);
	}
	else pTracker->ModifyMenu(ID_EXTRA_DONETWORKCOMMUNICATION, MF_STRING, ID_EXTRA_DONETWORKCOMMUNICATION, gszTranslation[PosGroupMenuExtraUpdateAll]);

	pTracker->ModifyMenu(ID_TASKBAR_OPEN, MF_STRING, ID_TASKBAR_OPEN, gszTranslation[PosPopUpTaskbarOpen]);
	pTracker->ModifyMenu(ID_TASKBAR_UPDATE, MF_STRING, ID_TASKBAR_UPDATE, gszTranslation[PosPopUpTaskbarUpdate]);
	pTracker->ModifyMenu(ID_TASKBAR_ABOUT, MF_STRING, ID_TASKBAR_ABOUT, gszTranslation[PosPopUpTaskbarAbout]);
	pTracker->ModifyMenu(ID_TASKBAR_SNOOZE_CPU, MF_STRING, ID_TASKBAR_SNOOZE_CPU, gszTranslation[PosPopUpTaskbarSnoozeCpu]);
	pTracker->ModifyMenu(ID_TASKBAR_SNOOZE_GPU, MF_STRING, ID_TASKBAR_SNOOZE_GPU, gszTranslation[PosPopUpTaskbarSnoozeGpu]);
	pTracker->ModifyMenu(ID_TASKBAR_EXIT, MF_STRING, ID_TASKBAR_EXIT, gszTranslation[PosPopUpTaskbarExit]);
// translate

	if (m_bSnoozeCpuEnable)
	{
		if (m_bSnoozeCpu) pTracker->CheckMenuItem(ID_TASKBAR_SNOOZE_CPU,MF_CHECKED );
	}
	if (m_bSnoozeGpuEnable)
	{
		if (m_bSnoozeGpu) pTracker->CheckMenuItem(ID_TASKBAR_SNOOZE_GPU,MF_CHECKED );
	}

	SetForegroundWindow();
	POINT pos;
	GetCursorPos(&pos);
	pTracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pos.x , pos.y , AfxGetMainWnd()); 

	taskbarMenu.DestroyMenu();
}

/*-----------------------------------------------------------------------------

 Abstract : Shows a ballon tip over the tray icon

 Parameters : 
	1. szBallonTitle -> Message title
    2. szBalloonMsg -> Balloon message
	3. nTimeOut -> Time to show the balloon, min 10 secs.
	4. dwIcon -> ICON_WARNING or ICON_ERROR or ICON_INFO
	
 Return Value : BOOL
----------------------------------------------------------------------------*/

// zie C:\temp\SysTray_Src

BOOL CMainFrame::ToolbarShowBalloon(char *szBalloonTitle, PTSTR szBalloonMsg, DWORD dwIcon, UINT nTimeOut)
{
    ASSERT (dwIcon == NIIF_WARNING || dwIcon == NIIF_ERROR || dwIcon == NIIF_INFO || dwIcon == NIIF_NONE);
    
    BOOL bBalloonShown = FALSE;

//	SetForegroundWindow();

//    tnid.cbSize = sizeof(NOTIFYICONDATA);
//    tnid.hWnd = m_hWnd;
//    tnid.uID = IDI_ICON_EFMER;
 
    m_tnid.dwInfoFlags = dwIcon;

    // Set the flags for showing balloon, especially NIF_INFO
//	m_tnid.uFlags |= NIF_INFO;
	m_tnid.uFlags = NIF_INFO;
	
	// Set the time out for the balloon
	m_tnid.uTimeout = nTimeOut;

	// Set the balloon title
	::strcpy_s(m_tnid.szInfoTitle, szBalloonTitle);

	// Set balloon message
	::strcpy_s(m_tnid.szInfo, szBalloonMsg);

    // Show balloon....
	bBalloonShown = ::Shell_NotifyIcon(NIM_MODIFY || NIM_SETFOCUS, &m_tnid);
		
	return bBalloonShown;  
}

void CMainFrame::RemoveToolbar()
{
	DestroyIcon(m_tnid.hIcon);
    m_tnid.hIcon = NULL;					// NULL icon to make icon invisible
    m_tnid.uFlags = NIF_ICON; 
	Shell_NotifyIcon(NIM_MODIFY, &m_tnid);
    Shell_NotifyIcon(NIM_DELETE, &m_tnid);	// verwijder icon

	m_iCurrentIcon = 0;
}


///// taskbar messages

LRESULT  CMainFrame::OnTaskbar(WPARAM wParam,LPARAM lParam)
{	
	bool		bShowFloater = false;
	CPoint		point;

	UINT uMouseMsg = (UINT) lParam;

	UINT test = (UINT) wParam;

	switch (uMouseMsg)
	{
    case WM_LBUTTONDOWN:
		if (m_bFloaterOverIcon)
		{
			m_iFloaterHideOverride = 5;					// hide for 5 seconds
			theApp.m_pDlgFloating->ShowWindow(SW_HIDE);
		}
		OnTaskbarOpen();	
    break;

	case WM_RBUTTONDOWN:
		if (m_bFloaterOverIcon)
		{
			m_iFloaterHideOverride = 5;					// hide for 5 seconds
			theApp.m_pDlgFloating->ShowWindow(SW_HIDE);
		}
		TaskbarMenu();
    break;

	case WM_MOUSEFIRST:
		m_bCursorIsOverIcon = true;
		GetCursorPos(&point );
		m_rectOverIcon.SetRect(point.x-2, point.y-2, point.x+2, point.y+2);

		m_iGetTooltipData = 10000; // update tooltip now.
	break;

	case NIN_BALLOONUSERCLICK:
		theApp.m_pMainWnd->ShowWindow(SW_SHOW);
//		if (theApp.m_pMainWnd->IsIconic())
//		{
//			theApp.m_pMainWnd->ShowWindow(SW_NORMAL);
//		}
		theApp.m_pMainWnd->SetForegroundWindow();
		m_dlgBarViewSelect.PostMessage(UWM_MSG_TAB_VIEW_SELECTED,BAR_SELECTED_NOTICES,0);
		this->m_dlgBarComputer.PostMessage(UWM_MSG_BAR_COMPUTER_SELECT_ALL,0,0);
		ToolbarShowBalloon("", "", NIIF_INFO, 0);
	break;

	case NIN_BALLOONTIMEOUT:
//		if (m_iBalloonTimeout != 0) m_iBalloonTimeout = 30;
		ToolbarShowBalloon("", "", NIIF_INFO, 0);			// remove the balloon
	break;

	case NIN_BALLOONHIDE:
		ToolbarShowBalloon("", "", NIIF_INFO, 0);			// remove the balloon
	break;

	default:
		int ii = 1;	 
	break;

  }
  return 0;
}
void CMainFrame::OnTaskbarOpen()
{
	if (theApp.m_bDocumentOpen)
	{
		if (::SendMessage(theApp.m_pMainWnd->m_hWnd, WM_NULL ,0,0) == 0)	// check if window is valid
		{
			theApp.m_pMainWnd->ShowWindow(SW_SHOW);
//			if (theApp.m_pMainWnd->IsIconic())
//			{
//				theApp.m_pMainWnd->ShowWindow(SW_NORMAL);
//			}
			theApp.m_pMainWnd->SetForegroundWindow();
		}
	}
}

void CMainFrame::OnTaskbarUpdate()
{
	theApp.m_pDlgUpdate->ShowWindow(SW_SHOW);
}

void CMainFrame::OnTaskbarAbout()
{
	theApp.m_pDlgAbout->ShowWindow(SW_SHOW);
}

void CMainFrame::OnTaskbarSnoozeCpu()
{
//	int			iRunMode;
	CDocument	*pDoc;
	CView		*pView;
	POSITION	posDoc, posView;
//	bool		bSnooze;

	if (m_bSnoozeCpu)
	{
		m_cCStatusCpu.task_mode = RUN_MODE_RESTORE;
		m_cCStatusCpu.task_mode_delay = 0;
	}
	else
	{
//		m_cCStatusCpu.task_mode = RUN_MODE_NEVER;
		m_cCStatusCpu.task_mode = RUN_MODE_NEVER;
		m_cCStatusCpu.task_mode_delay = 60*60;
	}

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
				pView->PostMessage(UWM_MSG_SET_SNOOZE_CPU, (WPARAM) m_iLocalHostThreadId, (LPARAM) &m_cCStatusCpu);
				if (theApp.m_pDlgLogging->m_bLogDebugSnooze)
				{
					if (m_cCStatusCpu.task_mode == RUN_MODE_RESTORE)
					{
						Logging("Snooze ", "CPU: RUN_MODE_RESTORE", LOGGING_DEBUG);	
					}
					if (m_cCStatusCpu.task_mode == RUN_MODE_NEVER)
					{
						Logging("Snooze ", "CPU: RUN_MODE_NEVER", LOGGING_DEBUG);	
					}
				}
				if (m_cCStatusCpu.task_mode != RUN_MODE_RESTORE)	// if cpu snooze disable GPU snooze ASAP
				{
					if (m_bSnoozeGpu)
					{
						m_cCStatusGpu.gpu_mode = RUN_MODE_RESTORE;
						m_cCStatusGpu.gpu_mode_delay = 0;
						pView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, (WPARAM) m_iLocalHostThreadId, (LPARAM) &m_cCStatusGpu);
						if (theApp.m_pDlgLogging->m_bLogDebugSnooze) Logging("Snooze ", "GPU: RUN_MODE_RESTORE", LOGGING_DEBUG);	
					}
				}
			}
		}
	}
}

void CMainFrame::OnTaskbarSnoozeGpu()
{
//	int			iRunMode;
	CDocument	*pDoc;
	CView		*pView;
	POSITION	posDoc, posView;
//	bool		bSnooze;

	if (m_bSnoozeGpu)
	{
		m_cCStatusGpu.gpu_mode = RUN_MODE_RESTORE;
		m_cCStatusGpu.gpu_mode_delay = 0;
	}
	else
	{
		m_cCStatusGpu.gpu_mode = RUN_MODE_NEVER;
		m_cCStatusGpu.gpu_mode_delay = 60*60;
	}

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
				pView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, (WPARAM) m_iLocalHostThreadId, (LPARAM) &m_cCStatusGpu);
				if (m_cCStatusGpu.gpu_mode == RUN_MODE_RESTORE)
				{
					if (theApp.m_pDlgLogging->m_bLogDebugSnooze) Logging("Snooze ", "CPU: RUN_MODE_RESTORE", LOGGING_DEBUG);	
				}
				if (m_cCStatusGpu.gpu_mode == RUN_MODE_NEVER)
				{
					if (theApp.m_pDlgLogging->m_bLogDebugSnooze) Logging("Snooze ", "CPU: RUN_MODE_NEVER", LOGGING_DEBUG);	
				}
			}
		}
	}
}

void CMainFrame::OnUpdateTaskbarSnoozeCpu(CCmdUI *pCmdUI)
{
	CString sTxt;
	char	cBuffer[64];

	sTxt = gszTranslation[PosPopUpTaskbarSnoozeCpu];
	pCmdUI->Enable(m_bSnoozeCpuEnable);
	if (m_bSnoozeCpuEnable)
	{
		if (m_cCStatusCpu.task_mode == RUN_MODE_NEVER)
		{
			if (m_cCStatusCpu.task_mode_delay != 0)
			{
				CTimeString timeString;
				timeString.TimeString((int) m_cCStatusCpu.task_mode_delay, cBuffer, 63, ": ", "");
				sTxt = gszTranslation[PosPopUpTaskbarSnoozeCpu];
				sTxt+= cBuffer;
			}
		}
	}
	pCmdUI->SetText(sTxt);
}

void CMainFrame::OnUpdateTaskbarSnoozeGpu(CCmdUI *pCmdUI)
{
	bool	bEnable;
	CString sTxt;
	char	cBuffer[64];

	bEnable = m_bSnoozeGpuEnable;
	if (m_bSnoozeCpu) bEnable = false;

	pCmdUI->Enable(bEnable);

	sTxt = gszTranslation[PosPopUpTaskbarSnoozeGpu];
	if (bEnable)
	{
		if (m_cCStatusGpu.gpu_mode == RUN_MODE_NEVER)
		{
			if (m_cCStatusGpu.gpu_mode_delay != 0)
			{
				CTimeString timeString;
				timeString.TimeString((int) m_cCStatusGpu.gpu_mode_delay, cBuffer, 63, ": ", "");
				sTxt = gszTranslation[PosPopUpTaskbarSnoozeGpu];
				sTxt+= cBuffer;
			}
		}
	}
	pCmdUI->SetText(sTxt);
}

void CMainFrame::OnTaskbarExit()
{	
	CBoincClientManager	cm;
	CLoggingMessage *psLog;
//	bool			bBoincRunning, bResult;
	int				iCheck;
	CString			sFormat;

//	if (m_bQueryClosingDown) return;	// already done this..... now checks before sending the message

	if (theApp.m_pThreadGarbageHistory)
	{
		::PostThreadMessage(theApp.m_pThreadGarbageHistory->m_dThreadId, UWM_MSG_THREAD_QUIT, 0, 0);
	}
	if (theApp.m_pThreadGarbageLog)
	{
		::PostThreadMessage(theApp.m_pThreadGarbageLog->m_dThreadId, UWM_MSG_THREAD_QUIT, 0, 0);
	}

	if (g_pThreadWebServer)
	{
		g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_QUIT,0,0);
	}
	if (g_pThreadCloudServer)
	{
		g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_QUIT,0,0);
	}

	Sleep(1000);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryStopBoincClient, 0);
	if (iCheck)
	{
		psLog = new CLoggingMessage;
		psLog->m_pFrom = "Exit BoincTasks";

		if (theApp.m_bBoincClientStartedByBoincTasks)
		{
			if (m_iLocalHostThreadId > 0)
			{
				psLog->m_pText = CLIENT_MANAGER_BOINC_SHUTTING_DOWN;
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
				::PostThreadMessage(m_iLocalHostThreadId,UWM_MSG_THREAD_QUIT_CLIENT,0,0);	// close down the client
//				cm.Close(m_iLocalHostThreadId, psLog);
			}
		}
		else
		{
			if (cm.IsRunning())
			{
				psLog->m_pText = gszTranslation[PosClientNotStartedBT];
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);	
			}
		}
		delete psLog;
	}

	SafeToolBar();

	CloseAllRemote();

	if (m_iClosingCount == 0)
	{
		CMDIFrameWnd::OnClose();			// no documuments open so close now.
		return;
	}
}

LRESULT CMainFrame::OnExit(WPARAM wParam,LPARAM lParam)
{
	OnTaskbarExit();
	return 0;
}

LRESULT CMainFrame::OnWUsThatAreReady(WPARAM wParam,LPARAM lParam)
{
	m_iHistoryStateReadyCount = (int) wParam;
	return 0;
}

LRESULT CMainFrame::OnSetFloater(WPARAM wParam,LPARAM lParam)
{
	int iRadio;

	iRadio = (int) wParam;
	m_iFloaterHideTime = (int) lParam;

	if (iRadio == RADIO_SHOW_ICON)
	{
		m_bFloaterShown = true;		// may be visible, so hide after time delay
		m_bFloaterOverIcon = true;
	}
	else
	{
		m_bFloaterShown = false;	// don't hide it
		m_bFloaterOverIcon = false;
	}

	SetTimer(UTIMER_DLG_MAINFRM, 100, 0);

	return 0;
}

LRESULT CMainFrame::OnGetFloaterDataReady(WPARAM wParam,LPARAM lParam)
{
	BOOL	bStatus;

	CString sTip, *psTxt;

	psTxt = (CString *) lParam;

	sTip += *psTxt;
	if (sTip.GetLength() == 0) sTip = "BoincTasks";
	
	lstrcpyn(m_tnid.szTip, sTip.GetBuffer(), sizeof(m_tnid.szTip)); 
	sTip.ReleaseBuffer();
    m_tnid.uFlags = NIF_TIP;
	bStatus = Shell_NotifyIcon(NIM_MODIFY, &m_tnid);
	if (bStatus == FALSE) m_bIconStatus = FALSE;	// TRUE = OK
	return 0;
}

LRESULT CMainFrame::OnSetSnoozeCpuReady(WPARAM wParam,LPARAM lParam)
{
	CString sTxt, sFormat;
	char	cBuffer[64];
	CC_STATUS *pCcStatus;

	pCcStatus = (CC_STATUS *) lParam;

	m_cCStatusCpu.task_mode = pCcStatus->task_mode;
	m_cCStatusCpu.task_mode_delay = pCcStatus->task_mode_delay;

	if (pCcStatus->task_mode == -1) m_bSnoozeCpuEnable = false;
	else m_bSnoozeCpuEnable = true;


	if (pCcStatus->task_mode == RUN_MODE_NEVER)
	{
		if (pCcStatus->task_mode_delay == 0)
		{
			m_bSnoozeCpuEnable = false;
		}
		m_bSnoozeCpu = true;
	}
	else
	{
		m_bSnoozeCpu = false;
	}

	if (theApp.m_pDlgLogging->m_bLogDebugSnooze)
	{
		sTxt = "CPU: ";
		switch (m_cCStatusCpu.task_mode)
		{
			case RUN_MODE_ALWAYS:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			case RUN_MODE_AUTO:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			case RUN_MODE_NEVER:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			case RUN_MODE_RESTORE:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			default:
				sFormat.Format("Mode: %d",m_cCStatusCpu.task_mode);
				sTxt += sFormat;
		}
		CTimeString timeString;
		timeString.TimeString((int) m_cCStatusCpu.task_mode_delay, cBuffer, 63, " ,Time: ", " ");
		sTxt += cBuffer;

		if (m_bSnoozeCpu) sTxt += "Snooze active";
		else sTxt += "Snooze not active";
		Logging("Snooze ", sTxt.GetBuffer(), LOGGING_DEBUG);	
	}


	return 0;
}

LRESULT CMainFrame::OnSetSnoozeGpuReady(WPARAM wParam,LPARAM lParam)
{
	CString sTxt, sFormat;
	char	cBuffer[64];
	CC_STATUS *pCcStatus;

	pCcStatus = (CC_STATUS *) lParam;

	m_cCStatusGpu.gpu_mode = pCcStatus->gpu_mode;
	m_cCStatusGpu.gpu_mode_delay = pCcStatus->gpu_mode_delay;

	if (pCcStatus->gpu_mode == -1) m_bSnoozeGpuEnable = false;
	else m_bSnoozeGpuEnable = true;


	if (pCcStatus->gpu_mode == RUN_MODE_NEVER)
	{
		m_bSnoozeGpu = true;
		if (pCcStatus->gpu_mode_delay == 0)
		{
			m_bSnoozeGpuEnable = false;	
		}
	}
	else
	{
		m_bSnoozeGpu = false;
	}

	if (theApp.m_pDlgLogging->m_bLogDebugSnooze)
	{
		sTxt = "GPU: ";
		switch (m_cCStatusGpu.gpu_mode)
		{
			case RUN_MODE_ALWAYS:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			case RUN_MODE_AUTO:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			case RUN_MODE_NEVER:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			case RUN_MODE_RESTORE:
				sTxt += "RUN_MODE_ALWAYS";
			break;
			default:
				sFormat.Format("Mode: %d",m_cCStatusGpu.gpu_mode);
				sTxt += sFormat;
		}
		CTimeString timeString;
		timeString.TimeString((int) pCcStatus->gpu_mode_delay, cBuffer, 63, " ,Time: ", " ");
		sTxt += cBuffer;

		if (m_bSnoozeGpu) sTxt += "Snooze active";
		else sTxt += "Snooze not active";
		Logging("Snooze ", sTxt.GetBuffer(), LOGGING_DEBUG);	
	}

	return 0;
}

LRESULT CMainFrame::OnNoticesAlert(WPARAM wParam,LPARAM lParam)
{
	int iNotices;
	CString sTitle, sFormat;

	iNotices = (int) wParam;

	if (iNotices > 0)
	{
		sFormat.Format(gszTranslation[PosNoticesMessageThereAreNew], iNotices);
		sTitle = "BoincTasks ";
		sTitle+= gszTranslation[PosNoticesMessageBalloonTitle];
		ToolbarShowBalloon(sTitle.GetBuffer(), sFormat.GetBuffer(), NIIF_INFO, 30);					// Vista and up no more than 30 is accepted.
	}
	return 0;
}

LRESULT CMainFrame::OnDocumentIsClosing(WPARAM wParam,LPARAM lParam)
{
	if (m_bIsClosing)
	{
		m_iClosingCount--;
		if (m_iClosingCount == 0)
		{
			// reboot or close
			m_bIsClosing = false;
			if (m_bForceRestart)
			{
//				theApp.m_pDocTemplateComputer->OpenDocumentFile(NULL);	// reboot
				theApp.Connect();
//				theApp.m_bRebootRemoteCombined = false;		// don't do this true means closing down... completely
				m_bIsClosing = false;
				return 0;
			}
/*
			else 
			{
				POSITION posDoc;
				CRemoteCombinedDoc *pDoc;
				posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
				if (posDoc != NULL)
				{
					pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
					if (pDoc != NULL)
					{
//						pDoc->NowCloseDocument();
					}
				}
//				CMDIFrameWnd::OnClose();							// not here the destroy of cdocument send a message to do so.
			}
*/
		}
	}
	return 0;
}

LRESULT CMainFrame::OnDocumentHasClosed(WPARAM wParam,LPARAM lParam)
{
	if (!m_bForceRestart)
	{
		CMDIFrameWnd::OnClose();
		return 0;
	}
	m_bForceRestart = false;
	return 0;
}

LRESULT CMainFrame::OnThreadPriority(WPARAM wParam,LPARAM lParam)
{
	DWORD dwThreadPriority;
	int		iSetting;
	bool	bOk;

	bOk = false;
	
	iSetting = (DWORD) wParam;

	dwThreadPriority = THREAD_PRIORITY_NORMAL;

	if (iSetting == 0) {dwThreadPriority = THREAD_PRIORITY_NORMAL; bOk = true;}
	if (iSetting == 1) {dwThreadPriority = THREAD_PRIORITY_BELOW_NORMAL; bOk = true;}
	if (iSetting == 2) {dwThreadPriority = THREAD_PRIORITY_LOWEST; bOk = true;}

	if (bOk)
	{
		if ( theApp.m_dwThreadPriority != dwThreadPriority)
		{
			theApp.m_dwThreadPriority = dwThreadPriority;
			SetPriority(GetCurrentProcessId(),dwThreadPriority);
		}
	}

	return 0;
}



void CMainFrame::OnComputersShow()
{
//	POSITION pos = theApp.m_pDocTemplateComputer->GetFirstDocPosition();
//	if (!pos)
//	{
//		theApp.m_pDocTemplateComputer->OpenDocumentFile(NULL);
//	}
}

void CMainFrame::OnUpdateComputers(CCmdUI *pCmdUI)
{
//	POSITION pos = theApp.m_pDocTemplateComputer->GetFirstDocPosition();
//	if (!pos)
//	{
//		if (!m_bIsClosing)
//		{
//			pCmdUI->Enable(TRUE);
//			return;
//		}
//	}
	pCmdUI->Enable(FALSE);

}


void CMainFrame::OnViewViewselectionbar()
{
	BOOL bVisible = !m_dlgBarViewSelect.IsVisible();

	if (!bVisible)
	{
		m_dlgBarViewSelect.ShowWindow(SW_HIDE);
		ShowControlBar((CControlBar *) &m_dlgBarViewSelect,FALSE,FALSE);
	}
	else
	{
		m_dlgBarViewSelect.ShowWindow(SW_SHOW);
		ShowControlBar((CControlBar *) &m_dlgBarViewSelect,TRUE,FALSE);
	}

	m_dlgBarViewSelect.Invalidate();

	RecalcLayout(TRUE);
	theApp.m_pMainWnd->Invalidate(TRUE);
}

void CMainFrame::OnUpdateViewViewselectionbar(CCmdUI *pCmdUI)
{
	BOOL bVisible = !m_dlgBarViewSelect.IsVisible();
	pCmdUI->SetCheck(!bVisible);
}

void CMainFrame::OnViewOperationToolbar()
{
	BOOL bVisible = !m_wndToolBarOperations.IsVisible();

	if (!bVisible)
	{
		m_wndToolBarOperations.ShowWindow(SW_HIDE);
		ShowControlBar((CControlBar *) &m_wndToolBarOperations,FALSE,FALSE);
	}
	else
	{
		m_wndToolBarOperations.ShowWindow(SW_SHOW);
		ShowControlBar((CControlBar *) &m_wndToolBarOperations,TRUE,FALSE);
	}

	m_wndToolBarOperations.Invalidate();

	RecalcLayout(TRUE);
	theApp.m_pMainWnd->Invalidate(TRUE);
}

void CMainFrame::OnUpdateViewOperationToolbar(CCmdUI *pCmdUI)
{
	BOOL bVisible = !m_wndToolBarOperations.IsVisible();
	pCmdUI->SetCheck(!bVisible);
}

void CMainFrame::OnViewAllowToolbar()
{
	BOOL bVisible = !m_wndToolBarAllow.IsVisible();

	if (!bVisible)
	{
		m_wndToolBarAllow.ShowWindow(SW_HIDE);
		ShowControlBar((CControlBar *) &m_wndToolBarAllow,FALSE,FALSE);
	}
	else
	{
		m_wndToolBarAllow.ShowWindow(SW_SHOW);
		ShowControlBar((CControlBar *) &m_wndToolBarAllow,TRUE,FALSE);
	}

	m_wndToolBarAllow.Invalidate();

	RecalcLayout(TRUE);
	theApp.m_pMainWnd->Invalidate(TRUE);
}

void CMainFrame::OnUpdateViewAllowToolbar(CCmdUI *pCmdUI)
{
	BOOL bVisible = !m_wndToolBarAllow.IsVisible();
	pCmdUI->SetCheck(!bVisible);
}

void CMainFrame::OnViewGraphicToolbar()
{
	BOOL bVisible = !m_wndToolBarGraphic.IsVisible();

	if (!bVisible)
	{
		m_wndToolBarGraphic.ShowWindow(SW_HIDE);
		ShowControlBar((CControlBar *) &m_wndToolBarGraphic,FALSE,FALSE);
	}
	else
	{
		m_wndToolBarGraphic.ShowWindow(SW_SHOW);
		ShowControlBar((CControlBar *) &m_wndToolBarGraphic,TRUE,FALSE);
	}

	m_wndToolBarGraphic.Invalidate();

	RecalcLayout(TRUE);
	theApp.m_pMainWnd->Invalidate(TRUE);
}

void CMainFrame::OnUpdateViewGraphicToolbar(CCmdUI *pCmdUI)
{
	BOOL bVisible = !m_wndToolBarGraphic.IsVisible();
	pCmdUI->SetCheck(!bVisible);
}

LRESULT  CMainFrame::OnThreadId(WPARAM wParam,LPARAM lParam)
{
	m_iLocalHostThreadId = (int) wParam;

	if (lParam != NULL)
	{
		m_sLocalHostThreadId = (char *) lParam;
	}
	else m_sLocalHostThreadId = "";
	return 0;
}

LRESULT  CMainFrame::onGarbageCollector(WPARAM wParam,LPARAM lParam)
{
	CThreadGarbageCollectorData *pThreadGarbageCollectorData;
	pThreadGarbageCollectorData = (CThreadGarbageCollectorData *) lParam;

	CThreadGarbageCollector *pThreadGarbageCollector;
	pThreadGarbageCollector = (CThreadGarbageCollector *) AfxBeginThread(RUNTIME_CLASS(CThreadGarbageCollector));
	pThreadGarbageCollectorData->m_dThreadId = pThreadGarbageCollector->m_nThreadID;

	::PostThreadMessageA(pThreadGarbageCollectorData->m_dThreadId, UWM_MSG_THREAD_GARBAGE_COLLECTOR, (WPARAM) this->m_hWnd, (LPARAM) pThreadGarbageCollectorData);

	return 0;
}
LRESULT  CMainFrame::onGarbageCollectorReady(WPARAM wParam,LPARAM lParam)
{
	int iThreadId;
	bool	bWarning = false;

	iThreadId = (int) wParam;

	if (theApp.m_pThreadGarbageHistory)
	{
		if (iThreadId == theApp.m_pThreadGarbageHistory->m_dThreadId)
		{
			WriteDebugLog(g_cFileNameLog, theApp.m_pThreadGarbageHistory->m_sLog.GetBuffer(), false);
			bWarning = theApp.m_pThreadGarbageHistory->m_bReadOnly;
			theApp.m_pThreadGarbageHistory->m_sLog.ReleaseBuffer();
			delete theApp.m_pThreadGarbageHistory;
			theApp.m_pThreadGarbageHistory = NULL;
		}
	}
	if (theApp.m_pThreadGarbageLog)	
	{
		if (iThreadId == theApp.m_pThreadGarbageLog->m_dThreadId)
		{
			WriteDebugLog(g_cFileNameLog, theApp.m_pThreadGarbageLog->m_sLog.GetBuffer(), false);
			bWarning = theApp.m_pThreadGarbageLog->m_bReadOnly;
			theApp.m_pThreadGarbageLog->m_sLog.ReleaseBuffer();
			delete theApp.m_pThreadGarbageLog;
			theApp.m_pThreadGarbageLog = NULL;
		}
	}
	if (bWarning)
	{
			AfxMessageBox("A file is read only!\r\nCheck C:\\Users\\username\\AppData\\Roaming\\eFMer\\BoincTasks\\log\\BoincTasks Startup.log");
	}
	return 0;
}

LRESULT  CMainFrame::OnForceRestart(WPARAM wParam,LPARAM lParam)
{
	CloseAllRemote(); // closed so reboot everything
	m_bForceRestart = true;
	return 0;
}

void CMainFrame::GetMessageString(UINT nID, CString& rMessage) const
{
	switch (nID)
	{
		// File Menu
		case ID_FILE_STARTBOINCCLIENT:
			rMessage = gszTranslation[PosGroupMenuFileStartBoincS];
		break;
		case ID_FILE_CLOSEBOINCCLIENTANDALLSCIENCEAPPLICATIONS:
			rMessage = gszTranslation[PosGroupMenuFileStopBoincS];
		break;
		case ID_FILE_READ_COLOR_SETTINGS:
			rMessage = gszTranslation[PosGroupMenuFileReadColorConfigS];
		break;
		case ID_FILE_WRITE_COLOR_SETTINGS:
			rMessage = gszTranslation[PosGroupMenuFileWriteColorConfigS];
		break;
		case ID_TASKBAR_EXIT:
			rMessage = gszTranslation[PosGroupMenuFileExitS];
		break;

		case ID_VIEW_GRAPHIC_TOOLBAR:
			rMessage = gszTranslation[PosGroupMenuViewToolGraphicS];
		break;
		case ID_VIEW_OPERATION_TOOLBAR:
			rMessage = gszTranslation[PosGroupMenuViewToolOperationsS];
		break;
		case ID_VIEW_VIEWSELECTIONBAR:
			rMessage = gszTranslation[PosGroupMenuViewViewSelectionS];
		break;
		case ID_VIEW_COMPUTER_BAR:
			rMessage = gszTranslation[PosGroupMenuViewComputerS];
		break;
		case ID_VIEW_PROJECT_BAR:
			rMessage = gszTranslation[PosGroupMenuViewProjectS];
		break;
		case ID_VIEW_TOOLBARALLOW:
			rMessage = gszTranslation[PosGroupMenuViewAllowS];
		break;
		case ID_VIEW_STATUS_BAR:
			rMessage = gszTranslation[PosGroupMenuViewStatusS];
		break;

//		case ID_COMPUTERS_SHOW:
//			rMessage = gszTranslation[PosGroupMenuComputersShowS];
//		break;

		case ID_SHOW_COMPUTERS:
			rMessage = gszTranslation[PosGroupMenuShowComputersS];
		break;
			
		case ID_SHOW_PROJECTS:
			rMessage = gszTranslation[PosGroupMenuShowProjectsS];
		break;
		case ID_SHOW_TASKS:
			rMessage = gszTranslation[PosGroupMenuShowTasksS];
		break;
		case ID_SHOW_TRANSFERS:
			rMessage = gszTranslation[PosGroupMenuShowTransfersS];
		break;
		case ID_SHOW_MESSAGES:
			rMessage = gszTranslation[PosGroupMenuShowMessagesS];
		break;
		case ID_SHOW_HISTORY:
			rMessage = gszTranslation[PosGroupMenuShowHistoryS];
		break;
		case ID_SHOW_HISTORYLONG:
			rMessage = gszTranslation[PosGroupMenuShowHistoryLongS];
		break;
		case ID_SHOW_NOTICES:
			rMessage = gszTranslation[PosGroupMenuShowNoticesS];
		break;
		case ID_SHOW_STATISTICS:
			rMessage = gszTranslation[PosGroupMenuShowStatisticsS];
		break;
		case ID_SHOW_STATISTICS_TASKS:
			rMessage = gszTranslation[PosGroupMenuShowStatisticsTasksS];
		break;
		case ID_SHOW_TEMPERATURE:
			rMessage = gszTranslation[PosGroupMenuShowTemperatureS];
		break;
		case ID_SHOW_GRAPH_TRANSFER:
			rMessage = gszTranslation[PosGroupMenuShowGraphTransferS];
		break;
		case ID_SHOW_GRAPH_DEADLINE:
			rMessage = gszTranslation[PosGroupMenuShowGraphDeadlineS];
		break;
		case ID_WINDOW_CASCADE:
			rMessage = gszTranslation[PosGroupMenuWindowCascadeS];
		break;
		case ID_WINDOW_TILE_HORZ:
			rMessage = gszTranslation[PosGroupMenuWindowTileS];
		break;
		case ID_APP_ABOUT:
			rMessage = "";
		break;

		case AFX_IDS_MDICHILD:
			rMessage = gszTranslation[PosGroupMenuWindowActivateS];
		break;

		case AFX_IDS_IDLEMESSAGE:
			rMessage = "";			// idle = Ready
		break;

		default:
		CFrameWnd::GetMessageString(nID, rMessage);
	}
}

LRESULT CMainFrame::OnWtsSessionChange(WPARAM wParam, LPARAM lParam)
{
	int		iCount;
	CBoincClientManager	cm;
	CLoggingMessage *psLog;

	switch( wParam )
	{
		case WTS_CONSOLE_CONNECT:
			if (theApp.m_bBoincClientStartedByBoincTasks)
			{
				Logging(USER_SWITCHING, "Session Connect", LOGGING_NORMAL);	
				Sleep(5000);
				for (iCount = 0; iCount < 30; iCount++)
				{
					Sleep(1000);
					if (!cm.IsRunning())
					{
						break;
					}
				}
				cm.StartupBoincCore(theApp.m_pMainFrame->m_iLocalHostThreadId);
			}
		break;
		case WTS_CONSOLE_DISCONNECT:
			if (theApp.m_bBoincClientStartedByBoincTasks)
			{
				Logging(USER_SWITCHING, "Session Disconnect", LOGGING_NORMAL);	
				psLog = new CLoggingMessage;
				psLog->m_pFrom = USER_SWITCHING;
				if (m_iLocalHostThreadId > 0)
				{
					cm.Close(m_iLocalHostThreadId, psLog);
				}
				if (cm.IsRunning())
				{
					Logging(USER_SWITCHING, "Session Disconnect, unable to stop client", LOGGING_NORMAL);	
				}
				delete psLog;
			}
		break;
		case WTS_SESSION_LOCK:
			Logging(USER_SWITCHING, "Session Lock", LOGGING_NORMAL);	
		break;
		case WTS_SESSION_UNLOCK:
			Logging(USER_SWITCHING, "Session Unlock", LOGGING_NORMAL);	
		break;
		
		default:
		break;
	}
	return 0;
}

BOOL CMainFrame::OnQueryEndSession()
{
//	if (!CMDIFrameWnd::OnQueryEndSession())
//		return FALSE;

	WriteDebugLog(g_cFileNameLog, "EXIT- Windows asked BoincTasks to shut down (OnQueryEndSession)\r\n", false);

	return TRUE;
}

void CMainFrame::OnEndSession(BOOL bEnding)
{
	if (bEnding)
	{
/*
		OSVERSIONINFO osvi;

		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osvi);
		if (osvi.dwMajorVersion > 5)	// vista or later
		{
			wchar_t *psReason = L"BoincTasks is shutting down......";
			bEnding = false;

			// dynamicall load fuction, otherwise XP won't start
			HINSTANCE hinstLib; 

			PROC_SHUTDOWN ProcShutdown; 
			BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 
			hinstLib = LoadLibrary(TEXT("User32.dll")); 
			if (hinstLib != NULL) 
			{ 
				ProcShutdown = (PROC_SHUTDOWN) GetProcAddress(hinstLib, "ShutdownBlockReasonCreate"); 
		        if (NULL != ProcShutdown) 
				{
					fRunTimeLinkSuccess = TRUE;
					(ProcShutdown) (this->m_hWnd, psReason); //	ShutdownBlockReasonCreate(this->m_hWnd, psReason);
					WriteDebugLog(g_cFileNameLog, "EXIT- Windows asked BoincTasks to shut down (OnEndSession)\r\n", false);
//					Logging("Exit", "BoincTasks is shutting down, because Windows sends an end session", LOGGING_NORMAL);
				}
		        fFreeResult = FreeLibrary(hinstLib); 
			}
		}
*/
		if (m_bQueryClosingDown == false)
		{
			theApp.m_pMainFrame->PostMessage(UWM_MSG_EXIT, 0, 0);
			//OnTaskbarExit();
			m_bQueryClosingDown = true;
			WriteDebugLog(g_cFileNameLog, "EXIT- BoincTasks entered the final shutting down phase\r\n", false);
//			Logging("Exit", "BoincTasks has shut down", LOGGING_NORMAL);
		}
	}

	CMDIFrameWnd::OnEndSession(bEnding);

	// TODO: Add your message handler code here
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	return CMDIFrameWnd::OnEraseBkgnd(pDC);
}

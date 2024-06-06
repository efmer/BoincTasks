// DlgPrefMain.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"

#include "dlgprefdisk.h"
#include "dlgprefprocessor.h"
#include "dlgprefnetwork.h"
#include "DlgPrefExclusive.h"
#include "Translation.h"
#include "DlgPrefMain.h"


// CDlgPrefMain

IMPLEMENT_DYNAMIC(CDlgPrefMain, CPropertySheet)

CDlgPrefMain::CDlgPrefMain(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDlgPrefExclusive = NULL;
}

CDlgPrefMain::CDlgPrefMain(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDlgPrefExclusive = NULL;
}

CDlgPrefMain::~CDlgPrefMain()
{
	SafeWindowPlacement();
	if (!m_hIcon) delete m_hIcon;
	delete m_pDlgPrefExclusive;
//	delete m_config.m_pConfigV6;
//	delete m_config.m_pLogFlagsV6;
	delete m_config.m_pConfig;
	delete m_config.m_pLogFlags;
}

BOOL CDlgPrefMain::OnInitDialog()
{
	CRect rect;
	int iWidth, iHeight;

	CPropertySheet::OnInitDialog();
	
	// Set the icon	for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_iThreadId = -1;

	m_iThreadBusy = 0;

	m_bDisabled = true;

//	m_config.m_pConfig = new CONFIG_V6;
//	m_config.m_pLogFlags = new LOG_FLAGS_V6;

//	m_config.m_pConfigV6 = NULL;
//	m_config.m_pLogFlagsV6 = NULL;
	m_config.m_pConfig = NULL;
	m_config.m_pLogFlags = NULL;

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryPosPrefLeft, 100);
	int top = theApp.GetProfileInt(registrySectionWindowPosition, registryPosPrefTop, 100);

	GetWindowRect(rect);
	iWidth = rect.Width();
	iHeight = rect.Height();
	theApp.ValidScreenPosition(&left, &top, &iWidth, &iHeight);
	SetWindowPos(&CWnd::wndBottom, left, top, iWidth, iHeight, SWP_NOOWNERZORDER || SWP_NOSIZE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgPrefMain::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosPrefLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosPrefTop, wp.rcNormalPosition.top);
}


void CDlgPrefMain::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		// close window = hide
		ShowWindow(SW_HIDE);
		return;
	}
	CPropertySheet::OnSysCommand(nID, lParam);
}

void CDlgPrefMain::FillDialog()
{
	m_pDlgPrefProcessor->FillDialog(&m_preferenceInfo);
	m_pDlgPrefNetwork->FillDialog(&m_preferenceInfo);
	m_pDlgPrefDisk->FillDialog(&m_preferenceInfo);
}

BEGIN_MESSAGE_MAP(CDlgPrefMain, CPropertySheet)
	ON_WM_SYSCOMMAND()
	ON_MESSAGE (UWM_MSG_PREFERENCE_OPEN_DIALOG, OnPreferenceOpen)
	ON_MESSAGE (UWM_MSG_THREAD_PREFERENCE_READ_READY, OnPreferenceReadReady) 
	ON_MESSAGE (UWM_MSG_THREAD_GET_CONFIG_READY, OnGetConfigReady) 
	ON_MESSAGE (UWM_MSG_THREAD_SET_CONFIG_READY, OnSetConfigReady) 
END_MESSAGE_MAP()


// CDlgPrefMain message handlers

/*
BOOL CDlgPrefMain::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR* pnmh = (NMHDR*)lParam;

	if (TCN_SELCHANGING == pnmh->code)	// tab is about to change
	{
		m_iLastActive = GetActiveIndex ();   // save the current page index
	}
	else
	{
		if (TCN_SELCHANGE == pnmh->code)	// tab has been changed
		{
			int iCurrentPage = GetActiveIndex (); // get the current page index
			if (iCurrentPage == m_iDisabled)
			{
				PostMessage (PSM_SETCURSEL, m_iLastActive); // activate the previous page
			}
		}
	}
	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}
*/

LRESULT CDlgPrefMain::OnPreferenceOpen(WPARAM parm1, LPARAM parm2)
{
// see C:\programma\BoincTasks\BoincTasks\boinc\boinc_source\boinc\clientgui\DlgAdvPreferences.cpp

	ShowWindow(SW_HIDE);					// hide while changing
	m_iThreadId = (int) parm1;
	m_preferenceInfo.prefsMask.clear();

	if (!m_iThreadBusy)
	{
		::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PREFERENCE_READ,(WPARAM) this->m_hWnd,(LPARAM) &m_preferenceInfo);
		m_iThreadBusy = 2;	// two we need this
	}
	return 0;
}


LRESULT CDlgPrefMain::OnPreferenceReadReady(WPARAM parm1, LPARAM parm2)
{
	CString sFormat, sTxt;
	bool	bBoincClientV6;

	bBoincClientV6 = (bool) (parm2 != 0);

	sTxt = gszTranslation[PosDialogBoincSettingsTitle];	//Preference: 

	m_iThreadBusy--;
	sFormat.Format("%s, %s", m_preferenceInfo.m_pcHostName, m_preferenceInfo.m_pcComputerId);
	sFormat = sTxt + sFormat;

	if (m_preferenceInfo.iReturn == ERR_FILE_NOT_FOUND)
	{
		// no file is ok

		m_preferenceInfo.iReturn = BOINC_SUCCESS;
	}

	if (!m_preferenceInfo.iReturn)
	{
		FillDialog();
		ShowWindow(SW_SHOW);
		this->SetWindowText(sFormat);	
	}

	m_config.m_bValid = false;
	if (!bBoincClientV6)
	{
		m_pDlgPrefNetwork->m_textConnectEvery.SetWindowText(gszTranslation[PosDialogBoincSettingsNGeneralMinWorkBuffer]);
		::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_GET_CONFIG, (WPARAM) this->m_hWnd, (LPARAM) &m_config);
	}
	else
	{
		m_pDlgPrefNetwork->m_textConnectEvery.SetWindowText(gszTranslation[PosDialogBoincSettingsNGeneralConnectEvery]);
		if (!m_bDisabled)
		{
			m_bDisabled = true;
			RemovePage(m_pDlgPrefExclusive);
			delete m_pDlgPrefExclusive;
			m_pDlgPrefExclusive = NULL;
			PostMessage (PSM_SETCURSEL, 0);
		}
		m_iThreadBusy--;
	}
	return 0;
}

// we only get here if bBoincClientV6 == false
LRESULT CDlgPrefMain::OnGetConfigReady(WPARAM parm1, LPARAM parm2)
{
	CRpcThreadReturn *pRpcThreadReturn;

	pRpcThreadReturn = (CRpcThreadReturn *) parm1;

	m_iThreadBusy--;

	delete pRpcThreadReturn;

	if (m_bDisabled)
	{
		m_pDlgPrefExclusive	= new CDgPrefExclusive();
		m_pDlgPrefExclusive->m_pDlgPrefMain = this;
		AddPage(m_pDlgPrefExclusive);
		SetActivePage(m_pDlgPrefExclusive);
//		PostMessage (PSM_SETCURSEL, DLG_PREF_EXCLUSIVE_TAB);	// selecting in init and resize the tab
//		::SendMessage(m_hWnd, PSM_RECALCPAGESIZES, 0, 0L);
		m_bDisabled = false;
	}
	SetActivePage(0);
	m_pDlgPrefExclusive->SendMessage(UWM_MSG_DIALOG_BOINC_SETTINGS_FILL,0,(LPARAM) &m_config);

//	CONFIG config;
//	MIOFILE out;
//	char	cBuffer[64000];
//	out.init_buf_write(cBuffer,64000);
//	config.write(out, *m_config.m_pLogFlags);
	return 0;
}

LRESULT CDlgPrefMain::OnSetConfigReady(WPARAM parm1, LPARAM parm2)
{
	BOOL bOk;

	bOk = (BOOL) parm2;
	if (!bOk)
	{
		AfxMessageBox("Unable to set exclusive applications", MB_ICONEXCLAMATION );
	}

	m_iThreadBusy = 0;

	return 0;
}


void CDlgPrefMain::Ok(void)
{
	m_preferenceInfo.prefsMask.clear();

	if (!m_pDlgPrefProcessor->ReadDialog(&m_preferenceInfo)) return;
	if (!m_pDlgPrefNetwork->ReadDialog(&m_preferenceInfo)) return;
	m_pDlgPrefDisk->ReadDialog(&m_preferenceInfo);	

	::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PREFERENCE_WRITE,(WPARAM) this->m_hWnd,(LPARAM) &m_preferenceInfo);

	if (!m_bDisabled)
	{
		if (m_pDlgPrefExclusive->ReadDialog(&m_config))
		{
			::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_SET_CONFIG,(WPARAM) this->m_hWnd,(LPARAM) &m_config);
		}
	}

	this->ShowWindow(SW_HIDE);
}

void CDlgPrefMain::Cancel(void)
{
	this->ShowWindow(SW_HIDE);
}

void CDlgPrefMain::Clear(void)
{
	if (AfxMessageBox(gszTranslation[PosDialogBoincSettingsButtonClearWarning],MB_YESNO) == IDYES)		//"Clear will erase ALL local settings, in ALL tabs! Do you really want to do this?"
	{
		m_preferenceInfo.prefsMask.clear();		// clear masks will clear all entries
		::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PREFERENCE_WRITE,(WPARAM) this->m_hWnd,(LPARAM) &m_preferenceInfo);
		Cancel();
	}
}

BOOL CDlgPrefMain::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			ShowWindow(SW_HIDE);
		}
	}

	return CPropertySheet::PreTranslateMessage(pMsg);
}

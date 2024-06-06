// DlgProxyMain.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgProxyMain.h"
#include "DlgProxyHttp.h"
#include "DlgProxySocks.h"
#include "Translation.h"

// CDlgProxyMain

IMPLEMENT_DYNAMIC(CDlgProxyMain, CPropertySheet)

CDlgProxyMain::CDlgProxyMain(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgProxyMain::CDlgProxyMain(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgProxyMain::~CDlgProxyMain()
{
	if (!m_hIcon) delete m_hIcon;
}

BOOL CDlgProxyMain::OnInitDialog()
{
	CPropertySheet::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_iThreadId = -1;


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgProxyMain::FillDialog()
{
	theApp.m_pDlgProxyHttp->FillDialog(&m_proxyInfo.m_grProxyInfo);
	theApp.m_pDlgProxySocks->FillDialog(&m_proxyInfo.m_grProxyInfo);
//	theApp.m_pDlgPrefDisk->FillDialog(&m_preferenceInfo);
}

BEGIN_MESSAGE_MAP(CDlgProxyMain, CPropertySheet)
	ON_WM_SYSCOMMAND()
	ON_MESSAGE (UWM_MSG_PROXY_OPEN_DIALOG, OnProxyOpen)
	ON_MESSAGE (UWM_MSG_THREAD_PROXY_READ_READY, OnProxyReadReady) 
END_MESSAGE_MAP()


void CDlgProxyMain::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		// close window = hide
		ShowWindow(SW_HIDE);
		return;
	}
	CPropertySheet::OnSysCommand(nID, lParam);
}

// CDlgProxyMain message handlers

LRESULT CDlgProxyMain::OnProxyOpen(WPARAM parm1, LPARAM parm2)
{
// see C:\programma\BoincTasks\BoincTasks\boinc\boinc_source\boinc\clientgui\DlgAdvPreferences.cpp

	ShowWindow(SW_HIDE);					// hide while changing
	m_iThreadId = (int) parm1;

	m_proxyInfo.m_grProxyInfo.clear();
	::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PROXY_READ,(WPARAM) this->m_hWnd,(LPARAM) &m_proxyInfo);
	return 0;
}


LRESULT CDlgProxyMain::OnProxyReadReady(WPARAM parm1, LPARAM parm2)
{
	CString sFormat, sTxt;

	sTxt = gszTranslation[PosDialogProxyTitle];

	sFormat.Format("%s, %s",m_proxyInfo.m_pcHostName, m_proxyInfo.m_pcComputerId);
	sFormat = sTxt + sFormat;

//	if (m_proxyInfo.iReturn == ERR_FILE_NOT_FOUND)
//	{
//		// no file is ok
//
//		m_preferenceInfo.iReturn = BOINC_SUCCESS;
//	}
//
	if (!m_proxyInfo.iReturn)
	{
		FillDialog();
		ShowWindow(SW_SHOW);
		this->SetWindowText(sFormat);	
	}
	return 0;
}

void CDlgProxyMain::Ok(void)
{
	theApp.m_pDlgProxyHttp->ReadDialog(&m_proxyInfo.m_grProxyInfo);
	theApp.m_pDlgProxySocks->ReadDialog(&m_proxyInfo.m_grProxyInfo);

	::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PROXY_WRITE,(WPARAM) this->m_hWnd,(LPARAM) &m_proxyInfo);

	this->ShowWindow(SW_HIDE);
}

void CDlgProxyMain::Cancel(void)
{
	this->ShowWindow(SW_HIDE);
}

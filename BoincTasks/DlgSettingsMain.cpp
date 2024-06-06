// DlgSettingsMain.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsMobile.h"
#include "DlgSettingsMain.h"


// CDlgSettingsMain

IMPLEMENT_DYNAMIC(CDlgSettingsMain, CPropertySheet)

CDlgSettingsMain::CDlgSettingsMain(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgSettingsMain::CDlgSettingsMain(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgSettingsMain::~CDlgSettingsMain()
{
	if (!m_hIcon) delete m_hIcon;
}

BOOL CDlgSettingsMain::OnInitDialog()
{
	CRect rect;
	int iWidth, iHeight;

	CPropertySheet::OnInitDialog();
	
	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryPosSettingsLeft, 100);
	int top = theApp.GetProfileInt(registrySectionWindowPosition, registryPosSettingsTop, 100);

	GetWindowRect(rect);
	iWidth = rect.Width();
	iHeight = rect.Height();
	theApp.ValidScreenPosition(&left, &top, &iWidth, &iHeight);
	SetWindowPos(&CWnd::wndBottom, left, top, iWidth, iHeight, SWP_NOOWNERZORDER | SWP_NOSIZE);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsMain::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosSettingsLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosSettingsTop, wp.rcNormalPosition.top);
}

void CDlgSettingsMain::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		// close window = hide
		theApp.m_pDlgSettingsMobile->GiveWarningDeactivate();
		ShowWindow(SW_HIDE);
		theApp.m_pDlgSettingsMain->SafeWindowPlacement();
		return;
	}
	CPropertySheet::OnSysCommand(nID, lParam);
}


BEGIN_MESSAGE_MAP(CDlgSettingsMain, CPropertySheet)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CDlgSettingsMain message handlers


BOOL CDlgSettingsMain::PreTranslateMessage(MSG* pMsg)
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

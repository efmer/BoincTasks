// DlgSettingsWWW.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "BoincTasks.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsWWW.h"
#include "ThreadCloudServer.h"
#include "ThreadCloudConnect.h"
#include "Translation.h"
#include "afxdialogex.h"
#include "\programma\common\bf\Blowfish.h"

// CDlgSettingsWWW dialog

IMPLEMENT_DYNAMIC(CDlgSettingsWWW, CPropertyPage)

CDlgSettingsWWW::CDlgSettingsWWW()
	: CPropertyPage(CDlgSettingsWWW::IDD)
{
	m_bValidEmailPassword = false;
	m_bValidEmailPasswordSend = false;
}

CDlgSettingsWWW::~CDlgSettingsWWW()
{
}

BOOL CDlgSettingsWWW::OnInitDialog()
{
	CString sFormat;
	CStringA sAKey, sAPassword;
	int		iCheck;

	m_bInitRunning = true;

	CPropertySheet *pSheet;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsCloudTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_textGroupFtp.SetWindowText(gszTranslation[PosGroupDialogSettingsWWWGroup]);
	m_checkEnableWWW.SetWindowText(gszTranslation[PosGroupDialogSettingsWWWEnable]);
	m_staticSeconds.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralSeconds]);
	m_staticRefresh.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralRefresh]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryWWWEnable, 0);
	m_checkEnableWWW.SetCheck(iCheck);
	m_bWWWEnable = (iCheck > 0);
	ShowRefresh(iCheck);

	m_iRefreshTime = theApp.GetProfileInt(registrySectionSettings, registryWWWRefreshTime, 1800);
	if (m_iRefreshTime < 2) m_iRefreshTime = 2;
	sFormat.Format("%d", m_iRefreshTime);
	m_editRefresh.SetWindowText(sFormat);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryCloudEnable, 0);
	if (iCheck > 0) iCheck = 1;
	m_checkEnableCloud.SetCheck(iCheck);
	m_bCloudEnable = (iCheck > 0);

	bool bServerInvalid = false;
	m_sServer = theApp.GetProfileString(registrySectionSettings, registryCloudServer, "seti.nl");


#ifdef _DEBUG
	m_editServer.EnableWindow(TRUE);
	m_checkUseLocalhost.ShowWindow(SW_SHOW);
	iCheck = theApp.GetProfileInt(registrySectionSettings, registryCloudServerDebug,1);
	m_checkUseLocalhost.SetCheck(iCheck);
	m_sServer = CLOUD_SERVER;
	if (iCheck)
	{
		m_sServer = "localhost";
		m_editServer.SetReadOnly(1);
	}
#else
	m_sServer = CLOUD_SERVER;
	m_editServer.EnableWindow(FALSE);
#endif
	m_editServer.SetWindowText(m_sServer);

	m_sCloudEmail = theApp.GetProfileString(registrySectionSettings, registryCloudEmail, "");
	m_editEmail.SetWindowText(m_sCloudEmail);

	sAPassword = theApp.GetProfileString(registrySectionSettings, registryCloudPassword, "");
	if (sAPassword.GetLength() > 0)
	{
		char cOutput[2048];
		sAKey = GetKey();
		CBlowFish bf;
		bf.Decrypt(sAPassword.GetBuffer(), cOutput, (char *) sAKey.GetBuffer(), 2048); sAKey.ReleaseBuffer();sAPassword.ReleaseBuffer();
		sAPassword = cOutput;
		sAPassword = sAPassword.Mid(4, 256);
	}
	m_sCloudPassword = sAPassword;
	m_editPassword.SetWindowText(sAPassword);



//	ValidEmailPassword();

	ShowCloud(m_bCloudEnable);

	m_bInitRunning = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

CStringA CDlgSettingsWWW::GetKey()
{
	CStringA sAKey;

	sAKey = "1%^";
	sAKey+= "acx#$";
	sAKey+= "fe86$!";
	sAKey.SetAt(2,'X');
	sAKey.SetAt(7,'^');
	return sAKey;
}

void CDlgSettingsWWW::ShowRefresh(int iCheck)
{
	int iMode;

	if (iCheck) iMode = SW_SHOW;
	else iMode = SW_HIDE;

	m_editRefresh.ShowWindow(iMode);
	m_staticSeconds.ShowWindow(iMode);
	m_staticRefresh.ShowWindow(iMode);
}

void CDlgSettingsWWW::ShowCloud(int iCheck)
{
	int iMode;

	if (iCheck) iMode = SW_SHOW;
	else iMode = SW_HIDE;

	m_editServer.ShowWindow(iMode);
	m_editPassword.ShowWindow(iMode);
	m_editEmail.ShowWindow(iMode);
	m_staticEmail.ShowWindow(iMode);
	m_staticPassword.ShowWindow(iMode);
	m_editStatus.ShowWindow(iMode);
	m_textGroupFtp.ShowWindow(iMode);
	m_staticServer.ShowWindow(iMode);
	m_staticStatus.ShowWindow(iMode);
	m_imageLock.ShowWindow(iMode);
	m_static_https.ShowWindow(iMode);

	m_buttonDefault.ShowWindow(SW_HIDE);
}

void CDlgSettingsWWW::ValidEmailPassword()
{
	int iPosDot, iPosAt;

	if (m_sCloudPassword.GetLength() > 4)
	{
		iPosAt = m_sCloudEmail.Find("@");
		iPosDot = m_sCloudEmail.Find(".");	

		if (iPosAt > 0 && iPosDot > 0)
		{
			if (!m_bValidEmailPassword)
			{
				m_editStatus.SetWindowText("Valid e-mail address");
				m_bValidEmailPassword = true;
			}
		}
		else
		{
//			if (m_bValidEmailPassword)
			{
				m_bValidEmailPassword = false;
				m_editStatus.SetWindowText("Invalid e-mail address");
			}
		}
	}
	else
	{
//		if (m_bValidEmailPassword)
		{
			m_bValidEmailPassword = false;
			m_editStatus.SetWindowText("Invalid password");
		}
	}

	bool bServerInvalid = false;;

	if (m_sServer.Find("efmer.eu") >= 0) bServerInvalid = true;
	if (m_sServer.Find("efmer.com") >= 0) bServerInvalid = true;

	if (bServerInvalid)
	{
		m_bValidEmailPassword = false;
		m_editStatus.SetWindowText("Server must be: seti.nl");
	}

	if (!m_bInitRunning) 
	{
		StartStopThread();
		SendEmailPassword();
	}
}

void CDlgSettingsWWW::StopThread()
{
	if (g_pThreadCloudServer == NULL)
	{
		return;
	}
	m_checkEnableCloud.EnableWindow(FALSE);
	if (g_pThreadCloudServer != NULL) g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_STOP,0, 0);
	if (g_pThreadCloudServer != NULL) g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_QUIT,0, 0);
}

void CDlgSettingsWWW::StartStopThread()
{
	if (g_pThreadCloudServer == NULL && m_bValidEmailPassword)
	{
		if (m_bCloudEnable)
		{

//			CThreadCloudConnect *test = (CThreadCloudConnect *) AfxBeginThread(RUNTIME_CLASS(CThreadCloudConnect));

			g_pThreadCloudServer = (CThreadCloudServer *) AfxBeginThread(RUNTIME_CLASS(CThreadCloudServer));
			g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_START,(WPARAM) this->m_hWnd, 0 );
		}
	}
	if (!m_bCloudEnable || !m_bValidEmailPassword)
	{
		StopThread();
	}
}

void CDlgSettingsWWW::SendEmailPassword()
{
	if (m_bCloudEnable)
	{
		if (m_bValidEmailPassword != m_bValidEmailPasswordSend)
		{
			int iPort = CLOUD_PORT;
			CString *psServer, *psEmail, *psPassword;
			psServer = new CString;
			*psServer = m_sServer;
			psEmail = new CString;
			psPassword =  new CString;
			if (m_bValidEmailPassword)
			{
				*psEmail = m_sCloudEmail;
				*psPassword = m_sCloudPassword;
			}
			else
			{
				*psEmail = "";
				*psPassword = "";
			}

			if (m_sServer == "localhost")
			{
				iPort = 8080;
			}
			else
			{
				iPort = CLOUD_PORT;
			}
			g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_SERVER, (WPARAM) psServer, iPort);
			g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_EMAIL, (WPARAM) psEmail, (LPARAM) psPassword);
		}
	}
}

void CDlgSettingsWWW::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_WWW_ENABLE, m_checkEnableWWW);
	DDX_Control(pDX, IDC_STATIC_WWW_REFRESH, m_staticRefresh);
	DDX_Control(pDX, IDC_EDIT_WWW_REFRESH, m_editRefresh);
	DDX_Control(pDX, IDC_STATIC_WWW_SECONDS, m_staticSeconds);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDC_CHECK_CLOUD_ENABLE, m_checkEnableCloud);
	//	DDX_Control(pDX, IDC_EDIT_CLOUD_FOLDER, m_editCloudFolder);
	//	DDX_Control(pDX, IDC_BUTTON_CLOUD_FOLDER, m_buttonCloudFolder);
	DDX_Control(pDX, IDC_EDIT_WWW_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_WWW_EMAIL, m_editEmail);
	DDX_Control(pDX, IDC_EDIT_WWW_STATUS, m_editStatus);
	DDX_Control(pDX, IDC_TEXT_WWW_EMAIL, m_staticEmail);
	DDX_Control(pDX, IDC_TEXT_WWW_PASSWORD, m_staticPassword);
	DDX_Control(pDX, IDC_EDIT_WWW_SERVER, m_editServer);
	DDX_Control(pDX, IDC_CHECK_WWW_DEBUG, m_checkUseLocalhost);
	DDX_Control(pDX, IDC_TEXT_WWW_GROUP_FTP, m_textGroupFtp);
	DDX_Control(pDX, IDC_TEXT_WWW_SERVER, m_staticServer);
	DDX_Control(pDX, IDC_TEXT_WWW_STATUS, m_staticStatus);
	DDX_Control(pDX, ID_WWW_DEFAULT_SERVER, m_buttonDefault);
	DDX_Control(pDX, IDC_BITMAP_SSL, m_imageLock);
	DDX_Control(pDX, IDC_TEXT_WWW_HTTPS, m_static_https);
}


BEGIN_MESSAGE_MAP(CDlgSettingsWWW, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_WWW_ENABLE, &CDlgSettingsWWW::OnBnClickedCheckWwwEnable)
	ON_EN_CHANGE(IDC_EDIT_WWW_REFRESH, &CDlgSettingsWWW::OnEnChangeEditWwwRefresh)
	ON_BN_CLICKED(IDOK, &CDlgSettingsWWW::OnBnClickedOk)

	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsWWW::OnBnClickedHelp)

//	ON_BN_CLICKED(IDC_BUTTON_CLOUD_FOLDER, &CDlgSettingsWWW::OnBnClickedButtonCloudFolder)
	ON_BN_CLICKED(IDC_CHECK_CLOUD_ENABLE, &CDlgSettingsWWW::OnBnClickedCheckCloudEnable)
	ON_EN_CHANGE(IDC_EDIT_WWW_EMAIL, &CDlgSettingsWWW::OnEnChangeEditWwwEmail)
	ON_EN_CHANGE(IDC_EDIT_WWW_PASSWORD, &CDlgSettingsWWW::OnEnChangeEditWwwPassword)
	ON_BN_CLICKED(IDC_CHECK_WWW_SHOW, &CDlgSettingsWWW::OnBnClickedCheckWwwShow)

	ON_MESSAGE(UWM_MSG_START_CLOUD, OnStartCloud)
	ON_MESSAGE(UWM_MSG_THREAD_WEB_STATUS_TEXT, OnThreadStatus)
	ON_MESSAGE(UWM_MSG_THREAD_QUIT, OnThreadCloudQuit)

	ON_EN_CHANGE(IDC_EDIT_WWW_SERVER, &CDlgSettingsWWW::OnEnChangeEditWwwServer)
	ON_BN_CLICKED(ID_WWW_DEFAULT_SERVER, &CDlgSettingsWWW::OnBnClickedWwwDefaultServer)
	ON_BN_CLICKED(IDC_CHECK_WWW_DEBUG, &CDlgSettingsWWW::OnBnClickedCheckWwwDebug)
END_MESSAGE_MAP()


// CDlgSettingsWWW message handlers

LRESULT CDlgSettingsWWW::OnStartCloud(WPARAM wParam, LPARAM lParam)
{
	StartStopThread();
	SendEmailPassword();
	return 0;
}

LRESULT CDlgSettingsWWW::OnThreadCloudQuit(WPARAM wParam, LPARAM lParam)
{
	StopThread();					// just in case;
	g_pThreadCloudServer = NULL;	// now it has closed
	m_checkEnableCloud.EnableWindow(TRUE);
	StartStopThread();
//	SendEmailPassword();

	ShowCloud(false);
	return 0;
}

// doesnt arrive
//LRESULT CDlgSettingsWWW::OnThreadCloudQuit(WPARAM wParam, LPARAM lParam)
//{
//	g_pThreadCloudServer = NULL;	// now it has closed
//	m_checkEnableCloud.EnableWindow(TRUE);
//	return 0;
//}



LRESULT CDlgSettingsWWW::OnThreadStatus(WPARAM wParam, LPARAM lParam)
{
	CString *psStatus;
	int		iStatus;

	psStatus = (CString *) wParam;
	iStatus = (int) lParam;

	m_editStatus.SetWindowText(*psStatus);
	delete psStatus;

	if (iStatus == 1)
	{
		m_imageLock.ShowWindow(SW_SHOW);
	}
	else
	{
		m_imageLock.ShowWindow(SW_HIDE);
	}

	return 0;
}

void CDlgSettingsWWW::OnBnClickedCheckWwwEnable()
{
	int iCheck;

	iCheck = m_checkEnableWWW.GetCheck();
	m_bWWWEnable = (iCheck > 0);
	ShowRefresh(iCheck);
	theApp.WriteProfileInt(registrySectionSettings, registryWWWEnable, iCheck);
}


void CDlgSettingsWWW::OnEnChangeEditWwwRefresh()
{
	CString sTxt;

	m_editRefresh.GetWindowText(sTxt);
	m_iRefreshTime = atoi(sTxt);
	if (m_iRefreshTime < 2) m_iRefreshTime = 2;
	theApp.WriteProfileInt(registrySectionSettings, registryWWWRefreshTime, m_iRefreshTime);
}


void CDlgSettingsWWW::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

BOOL CDlgSettingsWWW::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsWWW::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#cloud";

	ShellExecute(NULL,_T("open"),strLink,NULL,NULL,SW_SHOWNORMAL);
}

void CDlgSettingsWWW::OnBnClickedCheckCloudEnable()
{
	int iCheck;

	iCheck = m_checkEnableCloud.GetCheck();
	if (iCheck > 0) iCheck = 1;
	m_bCloudEnable = (iCheck > 0);
	ShowCloud(iCheck);
	theApp.WriteProfileInt(registrySectionSettings, registryCloudEnable, iCheck);

	StartStopThread();
	SendEmailPassword();
}

// only in DEBUG mode
void CDlgSettingsWWW::OnEnChangeEditWwwServer()
{
	m_editServer.GetWindowText(m_sServer);
	theApp.WriteProfileString(registrySectionSettings, registryCloudServer,m_sServer);
	ValidEmailPassword();
}

void CDlgSettingsWWW::OnEnChangeEditWwwEmail()
{
	m_editEmail.GetWindowText(m_sCloudEmail);

	theApp.WriteProfileString(registrySectionSettings, registryCloudEmail,m_sCloudEmail);
	ValidEmailPassword();
}


void CDlgSettingsWWW::OnEnChangeEditWwwPassword()
{
	CStringA sAPassword, sAKey, sARandom;
	char cOutput[2048];
	CBlowFish bf;

	sAKey = GetKey();

	m_editPassword.GetWindowText(m_sCloudPassword);
	sAPassword = m_sCloudPassword;
	sAPassword = sAPassword.Mid(0,256);
	
	if (sAPassword.GetLength() > 0)
	{
		GenerateRandomString((char *) cOutput, 4);
		sARandom = cOutput;
		sAPassword = sARandom + sAPassword;

		bf.Encrypt( sAPassword.GetBuffer(), cOutput, sAKey.GetBuffer(), 2048); sAPassword.ReleaseBuffer();
		sAPassword = cOutput;
	}
	theApp.WriteProfileString(registrySectionSettings, registryCloudPassword,sAPassword);

	ValidEmailPassword();
}


void CDlgSettingsWWW::OnBnClickedCheckWwwShow()
{
	// TODO: Add your control notification handler code here
}

void CDlgSettingsWWW::OnBnClickedWwwDefaultServer()
{
	m_sServer = "seti.nl";
	m_editServer.SetWindowText(m_sServer);
	theApp.WriteProfileString(registrySectionSettings, registryCloudServerDebug,m_sServer);
	ValidEmailPassword();
}

void CDlgSettingsWWW::OnBnClickedCheckWwwDebug()
{
	int iCheck;

	iCheck = m_checkUseLocalhost.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryCloudServerDebug,iCheck);

	if (iCheck)
	{
		m_sServer = "localhost";
		m_editServer.SetReadOnly(1);
	}
	else
	{
		m_sServer = "seti.nl";
		m_editServer.SetReadOnly(0);
	}
	ValidEmailPassword();
}

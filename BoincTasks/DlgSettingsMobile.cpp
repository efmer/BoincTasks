// DlgSettingsMobile.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsMobile.h"
#include "ThreadWebServer.h"
#include "Translation.h"
#include "afxdialogex.h"
#include "\programma\common\bf\Blowfish.h"

// CDlgSettingsMobile dialog

IMPLEMENT_DYNAMIC(CDlgSettingsMobile, CPropertyPage)

CDlgSettingsMobile::CDlgSettingsMobile()
	: CPropertyPage(CDlgSettingsMobile::IDD)
{

}

CDlgSettingsMobile::~CDlgSettingsMobile()
{
}

BOOL CDlgSettingsMobile::OnInitDialog()
{
	CString sFormat;
	CStringA sAKey, sAPassword;
	int		iCheck;

	CPropertySheet *pSheet;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsMobileTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_checkMobileActive.SetWindowText(gszTranslation[PosGroupDialogSettingsMobileEnable]);	
	m_textPassword.SetWindowText(gszTranslation[PosGroupDialogSettingsMobilePassword]);	
	m_checkShow.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralPasswordShow]);
	m_textHeaderFont.SetWindowText(gszTranslation[PosGroupDialogSettingsMobileHeaderFont]);
	m_textFont.SetWindowText(gszTranslation[PosGroupDialogSettingsMobileFont]);
	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	m_iWebServerPort = theApp.GetProfileInt(registrySectionSettings, registryMobilePort, 80);
	if (m_iWebServerPort == 0) m_iWebServerPort = 80;
	sFormat.Format("%d",m_iWebServerPort);
	m_editMobilePort.SetWindowText(sFormat);

	m_cPasswordChar = m_editPassword.GetPasswordChar();

	int iLen;
	char cOutput[2048];
//	char cHex[2048*2];
	CBlowFish bf;
	sAKey = GetKey();

/*
	// make hex
	cHex[0] = 0;
	sAPassword = theApp.GetProfileString(registrySectionGeneral, registryWWWPassword, "");
	bf.MakeHex((unsigned char *) sAPassword.GetBuffer(), cHex, 2048*2);
	sAPassword.ReleaseBuffer();
	sAPassword = cHex;

	// decrypt hex string
	cOutput[0] = 0;
	bf.ExtractHex(cOutput,sAPassword.GetBuffer(), 2048);sAPassword.ReleaseBuffer();
	sAPassword = cOutput;
	cOutput[0] = 0;
	bf.Decrypt(sAPassword.GetBuffer(), cOutput, (char *) sAKey.GetBuffer(), 2048); sAKey.ReleaseBuffer();sAPassword.ReleaseBuffer();
*/

	sAPassword = theApp.GetProfileString(registrySectionGeneral, registryWWWPassword, "");
	iLen = sAPassword.GetLength();
	if (iLen > 0)
	{
		if (iLen < 128)
		{
			bf.Decrypt(sAPassword.GetBuffer(), cOutput, (char *) sAKey.GetBuffer(), 2048); sAKey.ReleaseBuffer();sAPassword.ReleaseBuffer();
			sAPassword = cOutput;
			sAPassword = sAPassword.Mid(16, 256);
		}
		else
		{
			sAPassword = "";
		}
	}
	m_sPassword = sAPassword;
	m_editPassword.SetWindowText(sAPassword);

	// last !!!
	iCheck = theApp.GetProfileInt(registrySectionSettings, registryMobile, 0);
	if (iCheck > 0) iCheck = 1; 
	m_checkMobileActive.SetCheck(iCheck);
	m_bMobileActive = (iCheck > 0);

	m_iHeaderFont = theApp.GetProfileInt(registrySectionSettings, registryMobileHeaderFont, 4);
	m_comboHeaderFont.SetCurSel(m_iHeaderFont);
	m_iHeaderFont += 8;
	m_iFont = theApp.GetProfileInt(registrySectionSettings, registryMobileFont, 2);
	m_comboFont.SetCurSel(m_iFont);
	m_iFont += 8;

	m_bMobileAutoDeactivate = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

CStringA CDlgSettingsMobile::GetKey()
{
	CStringA sAKey;

	sAKey = "1%^";
	sAKey+= "ac#$";
	sAKey+= "fe86$!";
	sAKey.SetAt(2,'3');
	sAKey.SetAt(7,'^');
	return sAKey;
}

void CDlgSettingsMobile::GiveWarningDeactivate()
{
	if (m_bMobileAutoDeactivate)
	{
		if (!m_bMobileActive)
		{
			int iReturn;
			iReturn = AfxMessageBox(gszTranslation[PosGroupDialogSettingsMobileWarning], MB_ICONEXCLAMATION | MB_YESNO);
			if (iReturn == IDYES)
			{
				m_checkMobileActive.SetCheck(TRUE);
				OnBnClickedCheckWebserver();
			}
		}

		m_bMobileAutoDeactivate = false;
	}
}

void CDlgSettingsMobile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_MOBILE_WEBSERVER, m_checkMobileActive);
	DDX_Control(pDX, IDC_EDIT_MOBILE_WEBSERVER, m_editMobilePort);
	DDX_Control(pDX, IDC_TEXT_MOBILE_PASSWORD, m_textPassword);
	DDX_Control(pDX, IDC_EDIT_MOBILE_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_CHECK_MOBILE_SHOW_PASSWORD, m_checkShow);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDC_TEXT_MOBILE_HEADER_FONT, m_textHeaderFont);
	DDX_Control(pDX, IDC_TEXT_MOBILE_FONT, m_textFont);
	DDX_Control(pDX, IDC_COMBO_MOBILE_HEADER_FONT, m_comboHeaderFont);
	DDX_Control(pDX, IDC_COMBO_MOBILE_FONT, m_comboFont);
}


BEGIN_MESSAGE_MAP(CDlgSettingsMobile, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_MOBILE_WEBSERVER, &CDlgSettingsMobile::OnBnClickedCheckWebserver)
	ON_EN_CHANGE(IDC_EDIT_MOBILE_WEBSERVER, &CDlgSettingsMobile::OnEnChangeEditWebserver)
	ON_BN_CLICKED(IDC_CHECK_MOBILE_SHOW_PASSWORD, &CDlgSettingsMobile::OnBnClickedCheckWwwShow)
	ON_EN_CHANGE(IDC_EDIT_MOBILE_PASSWORD, &CDlgSettingsMobile::OnEnChangeEditWwwPassword)

	ON_BN_CLICKED(IDOK, &CDlgSettingsMobile::OnBnClickedOk)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsMobile::OnBnClickedHelp)
	ON_CBN_SELCHANGE(IDC_COMBO_MOBILE_HEADER_FONT, &CDlgSettingsMobile::OnCbnSelchangeComboMobileHeaderFont)
	ON_CBN_SELCHANGE(IDC_COMBO_MOBILE_FONT, &CDlgSettingsMobile::OnCbnSelchangeComboMobileFont)

	ON_MESSAGE(UWM_MSG_DLG_SETTINGS_MOBILE,OnMobile)
END_MESSAGE_MAP()


// CDlgSettingsMobile message handlers

void CDlgSettingsMobile::OnBnClickedCheckWebserver()
{
	int iCheck;

	iCheck = m_checkMobileActive.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryMobile, iCheck);
	m_bMobileActive = (iCheck >0);

	m_bMobileAutoDeactivate = false;

	if (g_pThreadWebServer == NULL)
	{
		if (m_bMobileActive)
		{
			g_pThreadWebServer = (CThreadWebServer *) AfxBeginThread(RUNTIME_CLASS(CThreadWebServer));
			g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_START,0,m_iWebServerPort);		
		}
	}

	if (m_bMobileActive) g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_START,0,m_iWebServerPort);
	else g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_STOP,0, 0);
}

void CDlgSettingsMobile::OnEnChangeEditWebserver()
{
	CString sTxt;

	m_editMobilePort.GetWindowText(sTxt);
	m_iWebServerPort = atoi(sTxt);

	theApp.WriteProfileInt(registrySectionSettings, registryMobilePort, m_iWebServerPort);
	m_checkMobileActive.SetCheck(FALSE);
	m_bMobileActive = false;
	if (m_bMobileActive) m_bMobileAutoDeactivate = true;
	if (g_pThreadWebServer != NULL) g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_STOP,0, 0);	// just in case
}

void CDlgSettingsMobile::OnBnClickedCheckWwwShow()
{
	int iCheck;

	iCheck = m_checkShow.GetCheck();

	if (iCheck) m_editPassword.SetPasswordChar(0);
	else m_editPassword.SetPasswordChar(m_cPasswordChar);

	m_editPassword.Invalidate(TRUE);
}


void CDlgSettingsMobile::OnEnChangeEditWwwPassword()
{
	CStringA sAPassword, sAKey, sARandom;
	char cOutput[2048];
	CBlowFish bf;

	sAKey = GetKey();

	m_editPassword.GetWindowText(m_sPassword);
	sAPassword = m_sPassword;
	sAPassword = sAPassword.Mid(0,256);
	
	if (sAPassword.GetLength() > 0)
	{
		GenerateRandomString((char *) cOutput, 16);
		sARandom = cOutput;
		sAPassword = sARandom + sAPassword;

		bf.Encrypt( sAPassword.GetBuffer(), cOutput, sAKey.GetBuffer(), 2048); sAPassword.ReleaseBuffer();
		sAPassword = cOutput;
	}
	theApp.WriteProfileString(registrySectionGeneral, registryWWWPassword,sAPassword);
//	m_checkMobileActive.SetCheck(FALSE);
//	if (m_bMobileActive) m_bMobileAutoDeactivate = true;
//	m_bMobileActive = false;
//	if (g_pThreadWebServer != NULL) g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_STOP,0, 0);	// just in case
}

void CDlgSettingsMobile::OnBnClickedOk()
{
	GiveWarningDeactivate();
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

BOOL CDlgSettingsMobile::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsMobile::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#mobile";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

void CDlgSettingsMobile::OnCbnSelchangeComboMobileHeaderFont()
{
	m_iHeaderFont = m_comboHeaderFont.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryMobileHeaderFont, m_iHeaderFont);
	m_iHeaderFont+= 8;
}


void CDlgSettingsMobile::OnCbnSelchangeComboMobileFont()
{
	m_iFont = m_comboFont.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryMobileFont, m_iFont);
	m_iFont += 8;
}

LRESULT CDlgSettingsMobile::OnMobile(WPARAM wParam,LPARAM lParam)
{
	int iType, iFontSet, iFontRead;

	iType = (int) wParam;
	iFontSet = (int) lParam;
	iFontRead = -1;

	switch (iType)
	{
		case MOBILE_FONT_HEADER:
			iFontRead = m_comboHeaderFont.GetCurSel() + 8;
			if (iFontSet > 0)
			{
				iFontSet -= 8;
				m_comboHeaderFont.SetCurSel(iFontSet);
				OnCbnSelchangeComboMobileHeaderFont();
			}

		break;
		case MOBILE_FONT_TEXT:
			iFontRead = m_comboFont.GetCurSel() + 8;
			if (iFontSet > 0)
			{
				iFontSet -= 8;
				m_comboFont.SetCurSel(iFontSet);
				OnCbnSelchangeComboMobileFont();
			}
		break;
	}


	return iFontRead;
}
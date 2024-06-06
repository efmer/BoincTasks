// DlgSettingsGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "MainFrm.h"
#include "ThreadBoincClient.h"
#include "DlgSettingsMain.h"
#include "Xml.h"
#include "DlgLogging.h"
#include "DlgSettingsGeneral.h"
#include "Translation.h"
#include "\programma\common\bf\Blowfish.h"

// CDlgSettingsGeneral dialog

IMPLEMENT_DYNAMIC(CDlgSettingsGeneral, CPropertyPage)

CDlgSettingsGeneral::CDlgSettingsGeneral()
	: CPropertyPage(CDlgSettingsGeneral::IDD)
{
	m_bConnecTthrottle = false;
	m_bStartBoincClientAtLoginRunning = false;
	m_bStartBoincClientMessageShown = false;
}

CDlgSettingsGeneral::~CDlgSettingsGeneral()
{
	ATOM hotKey;
	hotKey = GlobalAddAtom(registryHotkeyIdBoincTasksMain);
	UnregisterHotKey(m_hWnd,hotKey);
}

void CDlgSettingsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_START_AT_LOGIN, m_startAtLogin);
	DDX_Control(pDX, IDC_CHECK_START_BOINC_AT_LOGIN, m_startBoincClientAtLogin);
	DDX_Control(pDX, IDC_CHECK_TTHROTTLE, m_connectTthrottle);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLanguage);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_TEXT_GENERAL_LANGUAGE, m_textLanguage);
	DDX_Control(pDX, IDC_HOTKEY_BOINC_TASKS_MAIN, m_hotkeyShowAndHideBoincTasks);
	DDX_Control(pDX, IDC_TEXT_GENERAL_HOTKEY_MAIN, m_tekstHotKeyShowHide);
	DDX_Control(pDX, IDC_CHECK_HIDE_AT_STARTUP, m_checkHideAtStartup);
	DDX_Control(pDX, IDC_TEXT_GENERAL_OS, m_textOs);
	DDX_Control(pDX, IDC_COMBO_GENERAL_OS, m_comboOs);

	DDX_Control(pDX, IDC_CHECK_STOP_BOINC, m_stopBoincClientOnExit);
	DDX_Control(pDX, IDC_TEXT_GENERAL_PASSWORD, m_textPassword);
	DDX_Control(pDX, IDC_EDIT_GENERAL_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_CHECK_GENERAL_SHOW, m_checkShow);
	DDX_Control(pDX, IDC_TEXT_GENERAL_MANAGER, m_textManager);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDC_TEXT_GENERAL_START_DELAY, m_textStartDelay);
	DDX_Control(pDX, IDC_EDIT_GENERAL_START_DELAY, m_editStartDelay);
	DDX_Control(pDX, IDC_TEXT_GENERAL_DELAY_SECONDS, m_textStartDelaySeconds);
}

BOOL CDlgSettingsGeneral::OnInitDialog()
{
	DWORD	hotKey;
	ATOM	hotKeyId;
	CPropertySheet *pSheet;
	int		iCheck, iSelect, iLanguageCode;
	CString sNameEnglish, sName, sId, sFormat;
	CString sStartupKeyValue;
	CStringA sAKey, sAPassword;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsGeneralTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_startAtLogin.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralStartAtLogin]);
	m_stopBoincClientOnExit.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralStopOnExit]);
	m_startBoincClientAtLogin.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralStartBOINC]);

	m_textStartDelay.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralStartBOINCDelay]);
	m_textStartDelaySeconds.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralSeconds]);

	m_connectTthrottle.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralConTThrottle]);
	m_checkHideAtStartup.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralHideStartup]);	

	m_textPassword.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralPassword]);	
	m_checkShow.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralPasswordShow]);	

	m_textLanguage.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralLanguage]);
	m_textOs.SetWindowText(gszTranslation[PosGroupDialogGeneralOs]);

	m_tekstHotKeyShowHide.SetWindowText(gszTranslation[PosGroupDialogGeneralHotKeyMain]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	if (ReadKey(HKEY_USERS,registryPathAllUsers, StartupKeyValueName, &sStartupKeyValue))
	{
		if (sStartupKeyValue.GetLength() > 0) m_startAtLogin.SetCheck(TRUE);
	}

	m_iStartBoincClientAtLoginDelay = theApp.GetProfileInt(registrySectionSettings, registryStartBoincClientDelay, 5);
	if (m_iStartBoincClientAtLoginDelay < 1) m_iStartBoincClientAtLoginDelay = 0;
	sFormat.Format("%d",m_iStartBoincClientAtLoginDelay);
	m_editStartDelay.SetWindowText(sFormat);	

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryStartBoincClient, 1);
	m_startBoincClientAtLogin.SetCheck(iCheck);
	
	m_bStartBoincClientAtLogin = (iCheck != 0);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryStopBoincClient, 0);
	m_stopBoincClientOnExit.SetCheck(iCheck);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryConnectTthrottle, 0);
	m_connectTthrottle.SetCheck(iCheck);
	m_bConnecTthrottle = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryHideAtStartup, 1);
	m_checkHideAtStartup.SetCheck(iCheck);
	m_bHideAtStartup = (iCheck == TRUE);

	m_iLanguageCodeListCount = 0;
	CXml xml("lang\\languages.xml",false,false);
	if (xml.ReadStart("languagelist"))
	{
		while (xml.ReadGroup("language"))
		{
			xml.ReadItem("nameenglish",&sNameEnglish);
			xml.ReadItem("name",&sName);
			xml.ReadItem("id",&sId);
			
			m_iLanguageCodeList[m_iLanguageCodeListCount] = atoi(sId);

			sName =  "(" + sNameEnglish + ") " + sName;
			m_comboLanguage.AddString(sName);
			m_iLanguageCodeListCount++;
			if (m_iLanguageCodeListCount > MAX_LANGUAGES) break;
		}
	}
	else
	{
		CLoggingMessage sLog;
		sLog.m_pFrom = "Dialog Settings (language selection)"; sLog.m_pText = "languages.xml not found or corrupt";theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
	}
	iLanguageCode = theApp.GetProfileInt(registrySectionSettings, registryLanguage, 0);

	// find the language in the list
	iSelect = 0;
	for (int iCount = 0; iCount < m_iLanguageCodeListCount; iCount++)
	{
		if (iLanguageCode == m_iLanguageCodeList[iCount])
		{
			iSelect = iCount;
		}
	}

	m_comboLanguage.SetCurSel(iSelect);

	hotKey = theApp.GetProfileInt(registrySectionSettings, registryHotkeyIdBoincTasksMain, 0);
	m_hotKeyBoincTasksMainIDNow = hotKey;
	if (hotKey != 0)
	{
		m_hotkeyShowAndHideBoincTasks.SetHotKey(LOWORD(hotKey),HIWORD(hotKey));
		hotKeyId = GlobalAddAtom(registryHotkeyIdBoincTasksMain);
		RegisterHotKey(m_hWnd,hotKeyId,HIWORD(hotKey), LOWORD(hotKey));
	}

	m_iOs = theApp.GetProfileInt(registrySectionSettings, registryUsingOs, -1);

	if (m_iOs > OS_SELECTED_MAC || m_iOs < 0) m_iOs = -1;

	int iOs;
	iOs = m_iOs;
	while (iOs == -1)
	{
		int iResult;
		iResult = AfxMessageBox(gszTranslation[PosGroupDialogGeneralOsQuestionW], MB_YESNO);
		if (iResult == IDYES)
		{
			iOs = OS_SELECTED_WINDOWS;
			break;
		}
		else
		{
			iResult = AfxMessageBox(gszTranslation[PosGroupDialogGeneralOsQuestionL], MB_YESNO);
			if (iResult == IDYES)
			{
				iOs = OS_SELECTED_LINUX;
				break;
			}
			else
			{
				iResult = AfxMessageBox(gszTranslation[PosGroupDialogGeneralOsQuestionM], MB_YESNO);
				if (iResult == IDYES)
				{
					iOs = OS_SELECTED_MAC;
					break;
				}
			}
		}
	}
	if (m_iOs == -1)
	{
		m_iOs = iOs;
		theApp.WriteProfileInt(registrySectionSettings, registryUsingOs, m_iOs);
	}
	m_comboOs.SetCurSel(m_iOs);

	m_cPasswordChar = m_editPassword.GetPasswordChar();

	char cOutput[2048];
	CBlowFish bf;
	sAKey = GetKey();
	sAPassword = theApp.GetProfileString(registrySectionGeneral, registryTTPassword, "");
	if (sAPassword.GetLength() > 0)
	{
		bf.Decrypt(sAPassword.GetBuffer(), cOutput, (char *) sAKey.GetBuffer(), 2048); sAKey.ReleaseBuffer();sAPassword.ReleaseBuffer();
		sAPassword = cOutput;
		sAPassword = sAPassword.Mid(4, 256);
	}
	m_sPassword = sAPassword;
	m_editPassword.SetWindowText(sAPassword);

	m_iCheckBoincManager = 10000;

	SetTimer(UTIMER_DLG_SETTING_GENERAL, 2000, 0);	// interval 2 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsGeneral::CheckHotKeysChanged()
{
	DWORD id;
	WORD wVirtualKeyCode, wModifiers;
	BOOL status;

	m_hotkeyShowAndHideBoincTasks.GetHotKey(wVirtualKeyCode, wModifiers);
	id = wVirtualKeyCode | (wModifiers << 16);
	if (id != m_hotKeyBoincTasksMainIDNow)
	{
		m_hotKeyBoincTasksMainIDNow = id;
		status = RegisterHotkey(registryHotkeyIdBoincTasksMain, registryHotkeyIdBoincTasksMain, &m_hotkeyShowAndHideBoincTasks);
	}
}

CStringA CDlgSettingsGeneral::GetKey()
{
	CStringA sAKey;

	sAKey = "1%^";
	sAKey+= "ac#$";
	sAKey+= "fe86$!";
	sAKey.SetAt(2,'3');
	sAKey.SetAt(7,'^');
	return sAKey;
}

BOOL CDlgSettingsGeneral::RegisterHotkey(char *charHotkeyId, char *charRegister, CHotKeyCtrl *memberHotKey)
{
	WORD wVirtualKeyCode, wModifiers;
	memberHotKey->GetHotKey(wVirtualKeyCode, wModifiers);
	DWORD hotKey = wVirtualKeyCode | (wModifiers << 16);
	theApp.WriteProfileInt(registrySectionSettings, charRegister, hotKey);
	ATOM hotKeyId;
//	BOOL status;
	hotKeyId=GlobalAddAtom(charHotkeyId);
	UnregisterHotKey(m_hWnd,hotKeyId);
	if (hotKeyId != 0) return (RegisterHotKey(m_hWnd,hotKeyId,HIWORD(hotKey), LOWORD(hotKey)));
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgSettingsGeneral, CPropertyPage)
	ON_WM_TIMER()
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_BN_CLICKED(IDC_CHECK_START_AT_LOGIN, &CDlgSettingsGeneral::OnBnClickedCheckStartAtLogin)
	ON_BN_CLICKED(IDOK, &CDlgSettingsGeneral::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_START_BOINC_AT_LOGIN, &CDlgSettingsGeneral::OnBnClickedCheckStartBoincAtLogin)
	ON_BN_CLICKED(IDC_CHECK_TTHROTTLE, &CDlgSettingsGeneral::OnBnClickedCheckTthrottle)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, &CDlgSettingsGeneral::OnCbnSelchangeComboLanguage)
	ON_BN_CLICKED(IDC_CHECK_HIDE_AT_STARTUP, &CDlgSettingsGeneral::OnBnClickedCheckHideAtStartup)
	ON_CBN_SELCHANGE(IDC_COMBO_GENERAL_OS, &CDlgSettingsGeneral::OnCbnSelchangeComboGeneralOs)

	ON_BN_CLICKED(IDC_CHECK_STOP_BOINC, &CDlgSettingsGeneral::OnBnClickedCheckStopBoinc)
	ON_BN_CLICKED(IDC_CHECK_GENERAL_SHOW, &CDlgSettingsGeneral::OnBnClickedCheckGeneralShow)
	ON_EN_CHANGE(IDC_EDIT_GENERAL_PASSWORD, &CDlgSettingsGeneral::OnEnChangeEditGeneralPasswod)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsGeneral::OnBnClickedHelp)
	ON_EN_CHANGE(IDC_EDIT_GENERAL_START_DELAY, &CDlgSettingsGeneral::OnEnChangeEditGeneralStartDelay)
END_MESSAGE_MAP()


// CDlgSettingsGeneral message handlers

void CDlgSettingsGeneral::OnTimer(UINT_PTR nIDEvent)	// 2 second timer;
{
	if (!m_bStartBoincClientAtLoginRunning)
	{
		if (m_bStartBoincClientAtLogin)
		{
			if (!m_bStartBoincClientMessageShown)
			{
				if (m_iStartBoincClientAtLoginDelay > 10)
				{
					CLoggingMessage log; 
					log.m_pFrom = "Start BOINC client";
					CString sFormat;
					sFormat.Format("Start BOINC client in: %d seconds", m_iStartBoincClientAtLoginDelay);
					log.m_pText = sFormat.GetBuffer();
					theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
				}
				m_bStartBoincClientMessageShown = true;
			}

			if (m_iStartBoincClientAtLoginDelay <= 0 )
			{
				CThreadBoincClient *pThreadBoincClient;
				int iLocalHostThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;
				// valid even as the thread id = 0;
				pThreadBoincClient = (CThreadBoincClient *) AfxBeginThread(RUNTIME_CLASS(CThreadBoincClient));
				pThreadBoincClient->PostThreadMessage(UWM_MSG_THREAD_START_BOINC_CLIENT, 0, iLocalHostThreadId);
				m_bStartBoincClientAtLoginRunning = true;

			}
			else
			{
				m_iStartBoincClientAtLoginDelay-=2;
			}
		}
	}

	if (this->IsWindowVisible())
	{
		CheckHotKeysChanged();

		// check if there is a conflict with the manager.

		if (m_iCheckBoincManager++ > 30)	// once a minute
		{
			DWORD dManagerDisableAutoStart;
			if (ReadKey(HKEY_CURRENT_USER,"Software\\Space Sciences Laboratory, U.C. Berkeley\\BOINC Manager", "DisableAutoStart", &dManagerDisableAutoStart))
			{
			}

			bool	bWarning = false;
			if (m_bStartBoincClientAtLogin)
			{
				if (dManagerDisableAutoStart == 0)
				{
					bWarning = true;
					m_textManager.ShowWindow(SW_SHOW);
					m_textManager.SetTextColor(RGB(255,0,0));
					m_textManager.SetWindowText(gszTranslation[PosGroupDialogGeneralWarningBoinc]);
					m_textManager.Invalidate();
				}
			}
			if (!bWarning)
			{
				m_textManager.ShowWindow(SW_HIDE);
			}


			m_iCheckBoincManager = 0;
		}
	}
}

void CDlgSettingsGeneral::OnBnClickedCheckStartAtLogin()
{
	int iCheck;
	bool bCheck;
	bool bOk;

	iCheck = m_startAtLogin.GetCheck();

	bCheck = (iCheck != 0);
	bOk = StartupProgram(bCheck);
	if (!bOk)
	{
		// writing reg prohibited
		if (iCheck == 0)
		{
			iCheck = 1;
		}
		else
		{
			iCheck = 0;
		}
		m_startAtLogin.SetCheck(iCheck);	// reverse settinng
		m_startAtLogin.EnableWindow(FALSE);
		AfxMessageBox("You need administrator rights to change startup settings. Run this program as administrator once, to change this setting. Or add this program to the startup programs. ->Start->All Programs->Startup");
		return;
	}

	if (iCheck == 0)
	{
		bCheck = false;
	}
	else
	{
		bCheck = true;
	}

	if (CreateKey(HKEY_USERS, registryPathAllUsers))	// make efmer entry
	{
		if (SetKey(HKEY_USERS,registryPathAllUsers, StartupKeyValueName, StartupKeyValue, bCheck))		
		{
			int i;
			i=2;
		}
	}
}

// set auto startup for running program
bool CDlgSettingsGeneral::StartupProgram(bool startup)
{
	CString		sFileName;
	char		szAppPath[MAX_PATH] = "";
	bool		bOk;

	// filepath of exe
	if (!GetModuleFileNameA(NULL, szAppPath, MAX_PATH))
	{
		return false;
	}

	bOk = false;
	sFileName = szAppPath;

	if (SetKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", StartupKeyName, sFileName, startup))
	{
		return true;
	}
	return false;
}

// set auto startup for running program
bool CDlgSettingsGeneral::SetKey(HKEY key, CString sKeyPath, CString sKeyName, CString sKeyValue, bool bSetKey)
{
	HKEY		hKey;
	CStringA	sFileName;
	char		szAppPath[MAX_PATH] = "";
	bool		bOk;

	bOk = false;

	hKey = NULL;
	LONG lnRes = RegOpenKeyExA(key,sKeyPath.GetBuffer(), 0L, KEY_ALL_ACCESS, &hKey);
	sKeyPath.ReleaseBuffer();
	if( ERROR_SUCCESS != lnRes )
	{
		RegCloseKey (hKey);
		return false;
	}

 	if (bSetKey)	// set key
	{
		lnRes = RegSetValueExA(hKey,sKeyName.GetBuffer(),0,REG_SZ,(BYTE *) sKeyValue.GetBuffer(),sKeyValue.GetLength() );
		sKeyName.ReleaseBuffer();
		if( ERROR_SUCCESS == lnRes )
		{
			bOk = true;
		}
	}
	else
	{
		lnRes = RegSetValueExA(hKey,sKeyName.GetBuffer(),0,REG_SZ,NULL,0);	// dit werkt wel
		sKeyName.ReleaseBuffer();
		if( ERROR_SUCCESS == lnRes )
		{
			bOk = true;
		}
		lnRes = RegDeleteValue(hKey, sKeyName.GetBuffer());					// dit nu ook
		sKeyName.ReleaseBuffer();
		if( ERROR_SUCCESS == lnRes )
		{
			bOk = true;
		}
	}
	RegCloseKey (hKey);
	return bOk;
}

bool CDlgSettingsGeneral::CreateKey(HKEY key, CString sSubKey)
{
	HKEY		hKey;

	if (RegCreateKeyExA(key,sSubKey.GetBuffer(),0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{
		sSubKey.ReleaseBuffer();
		return true;
	}
	sSubKey.ReleaseBuffer();
	return false;
}

bool CDlgSettingsGeneral::ReadKey(HKEY key, CString sKeyPath, CString sKeyName, CString *psKeyValue)
{
	HKEY		hKey;
//	CStringA	sFileName;
//	CStringA	sFileNameRegister;
//	char		szFileName[MAX_PATH] = "";
	WCHAR		szKey[256];
	DWORD		wLen = 256;
	bool		bOk;
	DWORD		wType;

	// filepath of exe
//	GetModuleFileNameA(NULL, szFileName, MAX_PATH);

	bOk = false;
	LONG lnRes = RegOpenKeyExA(key,sKeyPath.GetBuffer(), 0L, KEY_QUERY_VALUE, &hKey);
	sKeyPath.ReleaseBuffer();
	wType = REG_SZ;
	if( ERROR_SUCCESS == lnRes )
	{
		LONG lnRes = RegQueryValueExA(hKey, sKeyName, NULL, &wType, (BYTE*) szKey, &wLen);
		if( ERROR_SUCCESS == lnRes )
		{
			if (wLen == 0) *psKeyValue = "";
			else *psKeyValue = szKey;
			bOk = true;
		}
	}
	RegCloseKey (hKey);
	return bOk;
}

bool CDlgSettingsGeneral::ReadKey(HKEY key, CString sKeyPath, CString sKeyName, DWORD *pdKeyValue)
{
	HKEY		hKey;
	DWORD		wLen = 256;
	bool		bOk;
	DWORD		wType;

	// filepath of exe
	*pdKeyValue = -1;
	bOk = false;
	LONG lnRes = RegOpenKeyExA(key,sKeyPath.GetBuffer(), 0L, KEY_QUERY_VALUE, &hKey);
	sKeyPath.ReleaseBuffer();
	wType = REG_DWORD;
	if( ERROR_SUCCESS == lnRes )
	{
		LONG lnRes = RegQueryValueExA(hKey, sKeyName, NULL, &wType, (BYTE*) pdKeyValue, &wLen);
		if( ERROR_SUCCESS == lnRes )
		{
			bOk = true;
		}
	}
	RegCloseKey (hKey);
	return bOk;
}

LRESULT CDlgSettingsGeneral::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (theApp.m_pMainWnd->IsWindowVisible())
	{
		theApp.m_pMainWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		theApp.m_pMainWnd->ShowWindow(SW_SHOW);
		theApp.m_pMainWnd->SetForegroundWindow();
	}
	return 0;
}

void CDlgSettingsGeneral::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsGeneral::OnBnClickedCheckStartBoincAtLogin()
{
	int iCheck;

	iCheck = m_startBoincClientAtLogin.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryStartBoincClient, iCheck);

	m_bStartBoincClientAtLogin = (iCheck != 0);

	m_iCheckBoincManager = 30000;
}


void CDlgSettingsGeneral::OnBnClickedCheckStopBoinc()
{
	int iCheck;

	iCheck = m_stopBoincClientOnExit.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryStopBoincClient, iCheck);
}

void CDlgSettingsGeneral::OnBnClickedCheckTthrottle()
{
	int iCheck;

	iCheck = m_connectTthrottle.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryConnectTthrottle, iCheck);

	m_bConnecTthrottle = (iCheck == TRUE);
}

void CDlgSettingsGeneral::OnBnClickedCheckHideAtStartup()
{
	int iCheck;

	iCheck = m_checkHideAtStartup.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryHideAtStartup, iCheck);

	m_bHideAtStartup = (iCheck == TRUE);
}

void CDlgSettingsGeneral::OnCbnSelchangeComboLanguage()
{
	int	iSelect, iLanguageCode;

	iSelect = m_comboLanguage.GetCurSel();

	iLanguageCode = m_iLanguageCodeList[iSelect];

	theApp.WriteProfileInt(registrySectionSettings, registryLanguage, iLanguageCode);

//	theApp.GetBoincTasksLanguage(iLanguageCode);
}

void CDlgSettingsGeneral::OnCbnSelchangeComboGeneralOs()
{
	m_iOs = m_comboOs.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryUsingOs, m_iOs);
}


void CDlgSettingsGeneral::OnBnClickedCheckGeneralShow()
{
	int iCheck;

	iCheck = m_checkShow.GetCheck();

	if (iCheck) m_editPassword.SetPasswordChar(0);
	else m_editPassword.SetPasswordChar(m_cPasswordChar);

	m_editPassword.Invalidate(TRUE);
}


void CDlgSettingsGeneral::OnEnChangeEditGeneralPasswod()
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
		GenerateRandomString((char *) cOutput, 4);
		sARandom = cOutput;
		sAPassword = sARandom + sAPassword;

		bf.Encrypt( sAPassword.GetBuffer(), cOutput, sAKey.GetBuffer(), 2048); sAPassword.ReleaseBuffer();
		sAPassword = cOutput;
	}
	theApp.WriteProfileString(registrySectionGeneral, registryTTPassword,sAPassword);
}

void CDlgSettingsGeneral::OnEnChangeEditGeneralStartDelay()
{
	CString sTxt;

	m_editStartDelay.GetWindowText(sTxt);
	m_iStartBoincClientAtLoginDelay = atoi(sTxt);
	if (m_iStartBoincClientAtLoginDelay < 1) m_iStartBoincClientAtLoginDelay = 0;
	theApp.WriteProfileInt(registrySectionSettings, registryStartBoincClientDelay, m_iStartBoincClientAtLoginDelay);
}


/*
void CDlgSettingsGeneral::OnHelpManual()
{
	CString strLink;
	strLink = "http://www.efmer.eu/";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}
*/

BOOL CDlgSettingsGeneral::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsGeneral::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#general";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}




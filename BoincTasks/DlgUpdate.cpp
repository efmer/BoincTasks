
#include "stdafx.h"
#include <stdlib.h>
#include <io.h>
#include "BoincTasks.h"
#include "threadrpc.h"
#include "updatecheck.h"
#include "DlgSettingsNotices.h"
#include "DlgNotices.h"
#include "DlgUpdate.h"
#include "RemoteCombinedNoticesView.h"
#include "RemoteCombinedDoc.h"
#include "MainFrm.h"
#include "Translation.h"

#define DEBUG_UPDATE_TEST false

// CDlgUpdate dialog

CUpdateCallback::CUpdateCallback()
{ 
} 

CUpdateCallback::~CUpdateCallback() 
{ 
} 

IMPLEMENT_DYNAMIC(CDlgUpdate, CDialog)

CDlgUpdate::CDlgUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpdate::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pTimer = 0;
}

CDlgUpdate::~CDlgUpdate()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
}

BOOL CDlgUpdate::OnInitDialog()
{
	CString sTxt;
	int		iCheck;

	CDialog::OnInitDialog();
	
//	AfxInitRichEdit( );

	SetIcon(m_hIcon, TRUE);			// Set big icon 
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translate
	sTxt = "BoincTasks - ";
	sTxt+= gszTranslation[PosDialogUpdateTitle];
	this->SetWindowText(sTxt);
	m_buttonUpdate.SetWindowText(gszTranslation[PosDialogUpdateButtonUpdate]);
	m_buttonCheck.SetWindowText(gszTranslation[PosDialogUpdateButtonCheck]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_checkAuto.SetWindowText(gszTranslation[PosDialogUpdateCheckAuto]);
	m_checkBeta.SetWindowText(gszTranslation[PosDialogUpdateCheckBeta]);
	m_checkUseBrowser.SetWindowText(gszTranslation[PosDialogUpdateCheckUseBrowser]);

	sTxt = gszTranslation[PosDialogUpdateTempFolder];
	sTxt+= ":";
	m_textUpdateTemp.SetWindowText(sTxt);
// Translate

	m_iCheckAuto = theApp.GetProfileInt(registrySectionSettings, registryUpdateAuto, 1);
	m_checkAuto.SetCheck(m_iCheckAuto);
	m_iCheckBeta = theApp.GetProfileInt(registrySectionSettings, registryUpdateBeta, 0);
	m_checkBeta.SetCheck(m_iCheckBeta);

	CheckUpdate();

	m_versionBoincTasks.m_dCurrentVersion = 0;
	m_versionBoincTasks.m_dVersion = 0;
	m_versionBoincTasks.m_dVersionBeta = 0;

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryUpdateUseBrowser, 0);
	m_checkUseBrowser.SetCheck(iCheck);
	m_bUseBrowser = (iCheck != 0);

	CString sCurrent;
	char cBufferCurrent[MAX_PATH];
	GetTempPath(MAX_PATH, cBufferCurrent);

	sCurrent  = theApp.GetProfileString(registrySectionSettings,registryUpdateFolder,cBufferCurrent);
	theApp.WriteProfileString(registrySectionSettings,registryUpdateFolder, sCurrent);
	m_editUpdateFolder.SetWindowText(sCurrent);

//	CheckForNewVersion(true);									// NOT here but check after 1 minute in time.

	m_bStartUpdate = false;

	m_pTimer = SetTimer(UTIMER_DLG_UPDATE, 60*1000, 0);		//First check in 1 minute, the next a bit longer....

	return TRUE;  // return TRUE  unless you set the focus to a control
}

int CDlgUpdate::CheckForNewVersion(bool bForceCheck)
{
	int iVersionCount;
	iVersionCount = 0;
	if (m_iCheckAuto || bForceCheck)
	{
		iVersionCount = GetVersion();
		
		if (iVersionCount > 0)
		{
			//success
			theApp.WriteProfileInt(registrySectionGeneral, registryUpdateCount, 0);		// reset the update counter

			if (m_iCheckBeta == 0)  iVersionCount = 1;	// don't check beta
			CNoticeBoincTasksUpdate noticeBoincTasksUpdate;

			noticeBoincTasksUpdate.m_iType = iVersionCount;
			noticeBoincTasksUpdate.m_dVersion = m_versionBoincTasks.m_dVersion;
			noticeBoincTasksUpdate.m_dVersionBeta = m_versionBoincTasks.m_dVersionBeta;
			noticeBoincTasksUpdate.m_sVersionExe = "http://" + m_versionBoincTasks.m_sVersionExe;
			noticeBoincTasksUpdate.m_sVersionBetaExe = "http://" + m_versionBoincTasks.m_sVersionBetaExe;
			noticeBoincTasksUpdate.m_dCurrentVersion = m_versionBoincTasks.m_dCurrentVersion;

			noticeBoincTasksUpdate.m_dTThrottleVersion = m_versionTThrottle.m_dVersion;
			noticeBoincTasksUpdate.m_dTThrottleVersionBeta = m_versionTThrottle.m_dVersionBeta;
			noticeBoincTasksUpdate.m_sTThrottleVersionExe = "http://" + m_versionTThrottle.m_sVersionExe;
			noticeBoincTasksUpdate.m_sTThrottleVersionBetaExe = "http://" + m_versionTThrottle.m_sVersionBetaExe;
			noticeBoincTasksUpdate.m_iCheckBeta = m_iCheckBeta;

			if (iVersionCount > 1)
			{
				if (m_versionBoincTasks.m_dVersionBeta > m_versionBoincTasks.m_dCurrentVersion)
				{
					if (theApp.m_pDlgSettingsNotices->m_iCheckEvery == 0)										 // 0 = notices disabled
					{
						this->ShowWindow(SW_SHOW);			// beta is newer
						if (theApp.m_pMainWnd != NULL)	theApp.m_pMainWnd->SetForegroundWindow();	
						ShowVersion();
					}
				}
			}
			else
			{
				if (m_versionBoincTasks.m_dVersion > m_versionBoincTasks.m_dCurrentVersion)
				{
					if (theApp.m_pDlgSettingsNotices->m_iCheckEvery == 0) // 0 = notices disabled
					{
						this->ShowWindow(SW_SHOW);			// regular is newer
						if (theApp.m_pMainWnd != NULL) theApp.m_pMainWnd->SetForegroundWindow();	
						ShowVersion();
					}
				}
			}
			if (theApp.m_pDlgSettingsNotices->m_iCheckEvery != 0)												 // !0 = notices enabled
			{
//				noticeBoincTasksUpdate.m_dTThrottleVersionBeta = 4.6;
				SendUpdateNotice(&noticeBoincTasksUpdate);
			}
		}

	}
	return iVersionCount;
}

void CDlgUpdate::SendUpdateNotice(CNoticeBoincTasksUpdate *pNoticeBoincTasksUpdate)
{
	CRemoteCombinedDoc	*pDoc;
	POSITION	posDoc;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			pDoc->m_pDlgNotices->SendMessage(UWM_MSG_NOTICES_BT_UPDATE,0, (LPARAM) pNoticeBoincTasksUpdate);
		}
	}
}


void CDlgUpdate::CheckUpdate()
{
	CString sTxt, sCrLf, sTemp;
	sCrLf = _T("\r\n");

	sTxt = gszTranslation[PosDialogUpdateCheckNew] + sCrLf + sCrLf + gszTranslation[PosDialogUpdateUsesHttp];

	m_textBox.SetWindowText(sTxt);
}

int CDlgUpdate::GetVersion()
{
	int	 iStatus;

	char *pcServer1		= "www.efmer.eu";
	char *pcServer2		= "www.efmer.com";

	char *pcLocationBt	= "download/boinc/boinc_tasks/unified/";
	char *pcFileBt		= "update_info_tasks_32_64.html";
	char *pcProgramBt	= "BOINC_Tasks";

	char *pcLocationTt	= "download/boinc/tthrottle/unified/";
	char *pcFileTt		= "update_info_tthrottle_32_64.html";
	char *pcProgramTt	= "TThrottle";

	bool bFound;
	CString sProgramVersion;

	m_versionBoincTasks.m_sVersionHttp = "";
	m_versionBoincTasks.m_sVersionBetaHttp = "";
	m_versionBoincTasks.m_sVersionExe = "";
	m_versionBoincTasks.m_sVersionBetaExe = "";

	m_versionTThrottle.m_sVersionHttp = "";
	m_versionTThrottle.m_sVersionBetaHttp = "";
	m_versionTThrottle.m_sVersionExe = "";
	m_versionTThrottle.m_sVersionBetaExe = "";


	m_versionBoincTasks.m_dVersion = 0;
	m_versionBoincTasks.m_dVersionBeta = 0;
	m_versionTThrottle.m_dVersion = 0;
	m_versionTThrottle.m_dVersionBeta = 0;

	iStatus = 0;
	CUpdateCheck getVersion;

	// BoincTasks
	bFound = getVersion.GetLatestVersion(pcServer1, pcProgramBt, pcLocationBt, pcFileBt, &m_versionBoincTasks);
	if (!bFound) bFound = getVersion.GetLatestVersion(pcServer2, pcProgramBt, pcLocationBt, pcFileBt, &m_versionBoincTasks);
	if (bFound)
	{
		sProgramVersion.LoadString(IDS_PROGRAM_VERSION);
		m_versionBoincTasks.m_dCurrentVersion = atof(sProgramVersion);
		m_versionBoincTasks.m_dVersion = atof(m_versionBoincTasks.m_sVersionHttp);

		if (m_versionBoincTasks.m_sVersionBetaHttp.GetLength() > 0)
		{
			m_versionBoincTasks.m_dVersionBeta = atof(m_versionBoincTasks.m_sVersionBetaHttp);

			if (DEBUG_UPDATE_TEST)
			{
				m_versionBoincTasks.m_dVersionBeta = 2.00;
			}

			iStatus = 2;
		}
		else
		{
			iStatus = 1;
		}
	}

//	TThrottle
	bFound = getVersion.GetLatestVersion(pcServer1, pcProgramTt, pcLocationTt, pcFileTt, &m_versionTThrottle);
	if (!bFound) bFound = getVersion.GetLatestVersion(pcServer2, pcProgramTt, pcLocationTt, pcFileTt, &m_versionTThrottle);
	if (bFound)
	{
//		sProgramVersion.LoadString(IDS_PROGRAM_VERSION);
		m_versionTThrottle.m_dVersion = atof(m_versionTThrottle.m_sVersionHttp);
//
		if (m_versionTThrottle.m_sVersionBetaHttp.GetLength() > 0)
		{
			m_versionTThrottle.m_dVersionBeta = atof(m_versionTThrottle.m_sVersionBetaHttp);
//			iStatus = 2;
		}
//		iStatus = 1;
	}
	return iStatus;
}

void CDlgUpdate::ShowVersion()
{
	CString sTxt, sTemp, sCrLf, sIDS, sFormat, sProgramVersion, sCurrentVersion, sNewVersion, sNewVersionBeta, sLatestVersion, sLatestVersionBeta;;
	bool	bFound = false;

	sCrLf = _T("\r\n");

	m_buttonUpdate.EnableWindow(FALSE);
	m_updateFolder.EnableWindow(FALSE);

	if (m_versionBoincTasks.m_dVersionBeta > 0)
	{
		sIDS= gszTranslation[PosDialogUpdateLatestBetaVersion];
		sProgramVersion.Format("%.2f", m_versionBoincTasks.m_dVersionBeta);
		sLatestVersionBeta.Format(sIDS, sProgramVersion);
	}

	if (m_versionBoincTasks.m_dVersionBeta > 0)
	{
		sIDS= gszTranslation[PosDialogUpdateNewBetaVersionFound];
		sProgramVersion.Format("%.2f", m_versionBoincTasks.m_dVersionBeta);
		sNewVersionBeta.Format(sIDS, sProgramVersion);
	}

	if (m_versionBoincTasks.m_dVersion > 0)
	{
		sIDS= gszTranslation[PosDialogUpdateLatestVersion];
		sProgramVersion.Format("%.2f", m_versionBoincTasks.m_dVersion);
		sLatestVersion.Format(sIDS, sProgramVersion);
	}

	sIDS= gszTranslation[PosDialogUpdateNewVersionFound];
	sProgramVersion.Format("%.2f", m_versionBoincTasks.m_dVersion);
	sNewVersion.Format(sIDS, sProgramVersion); //"New version of BT found: %s"

	sIDS= gszTranslation[PosDialogUpdateNewBetaVersionFound];
	sProgramVersion.Format("%.2f", m_versionBoincTasks.m_dVersionBeta);
	sNewVersionBeta.Format(sIDS, sProgramVersion); //"New version of BT found: %s"

	sIDS = gszTranslation[PosDialogUpdateCurrentVersion];
	sProgramVersion.Format("%.2f", m_versionBoincTasks.m_dCurrentVersion);
	sCurrentVersion.Format(sIDS, sProgramVersion); //"Current version of BT: %s"

	sTxt += sCurrentVersion;
	sTxt += "  ( " + sLatestVersion + " , " + sLatestVersionBeta + " )";
	sTxt += sCrLf + sCrLf;

	if (m_iCheckBeta)
	{
		if (m_versionBoincTasks.m_dVersionBeta > 0)
		{
			if (m_versionBoincTasks.m_dCurrentVersion < m_versionBoincTasks.m_dVersionBeta)
			{
				if (m_versionBoincTasks.m_dVersion == m_versionBoincTasks.m_dVersionBeta)
				{
					sTxt += sNewVersion + sCrLf;
					sIDS = gszTranslation[PosDialogUpdateClickUpdate];
					sTxt = sTxt + sCrLf + sIDS; //"Click on Update to download this version"
					bFound = true;
				}
				else
				{
					sTxt += sNewVersionBeta + sCrLf;
					sIDS = gszTranslation[PosDialogUpdateClickUpdate];
					sTxt = sTxt + sCrLf + sIDS; //"Click on Update to download this version"
					bFound = true;
				}
			}
			else
			{
				sIDS = gszTranslation[PosDialogUpdateAlreadyCurrent];
				sTxt +=  sCrLf + sIDS; //"You already have the most current version of BT!"
				m_textBox.SetWindowText(sTxt);
				bFound = true;
			}
		}
	}
	else
	{
		if (m_versionBoincTasks.m_dVersion > 0)
		{
			if (m_versionBoincTasks.m_dCurrentVersion < m_versionBoincTasks.m_dVersion)
			{
				sTxt += sNewVersion + sCrLf;
				sIDS = gszTranslation[PosDialogUpdateClickUpdate];
				sTxt = sTxt + sCrLf + sIDS; //"Click on Update to download this version"
				bFound = true;
			}
			else
			{
				sIDS = gszTranslation[PosDialogUpdateAlreadyCurrent];
				sTxt +=  sCrLf + sIDS; //"You already have the most current version of BT!"
				m_textBox.SetWindowText(sTxt);
				bFound = true;
			}
		}
	}

	if (!bFound)
	{
		sIDS = gszTranslation[PosDialogUpdateCantConnect];
		sTxt = sIDS + sCrLf; //"Can't make an internet connection"
		sIDS = gszTranslation[PosDialogUpdateCheckManually]; //"check www.efmer.eu, manually"
		sTxt += sIDS;
//		m_textBox.SetWindowText(sTxt);
	}
	else 
	{
		m_buttonUpdate.EnableWindow(TRUE);
		m_updateFolder.EnableWindow(TRUE);
	}

	m_textBox.SetWindowText(sTxt);
}

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// If the BFFM_INITIALIZED message is received
	// set the path to the start path.
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
		{
			if (NULL != lpData)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			}
		}
	}

	return 0; // The function should always return 0.
}

// HWND is the parent window.
// szCurrent is an optional start folder. Can be NULL.
// szPath receives the selected path on success. Must be MAX_PATH characters in length.
BOOL CDlgUpdate::BrowseForFolder(HWND hwnd, LPCTSTR szCurrent, LPTSTR szPath)
{
	BROWSEINFO   bi = { 0 };
	LPITEMIDLIST pidl;
	TCHAR        szDisplay[MAX_PATH];
	BOOL         retval;

//	CoInitialize();

	bi.hwndOwner      = hwnd;
	bi.pszDisplayName = szDisplay;
	bi.lpszTitle      = TEXT("Please choose a folder.");
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn           = BrowseCallbackProc;
	bi.lParam         = (LPARAM) szCurrent;

	pidl = SHBrowseForFolder(&bi);

	if (NULL != pidl)
	{
		retval = SHGetPathFromIDList(pidl, szPath);
		CoTaskMemFree(pidl);
	}
	else
	{
		retval = FALSE;
	}

	if (!retval)
	{
		szPath[0] = TEXT('\0');
	}

//	CoUninitialize();
	return retval;
}

void CDlgUpdate::Update(bool bDownload)
{
	CString sExe, sHttp, sTxt, sFormat;
	HRESULT result;
	int	iStatus;
	CUpdateCallback callback;

	sExe = m_versionBoincTasks.m_sVersionExe;

	if (m_iCheckBeta)
	{
		if (m_versionBoincTasks.m_sVersionExe.GetLength() > 0)
		{
			sExe = m_versionBoincTasks.m_sVersionBetaExe;
		}
	}

	if (m_bUseBrowser && (bDownload == false))
	{
		ShellExecute(NULL,_T("open"),sExe,NULL,NULL,SW_SHOWNORMAL);
	}
	else
	{
		CString sExeTemp;
		int	iPos;
		iPos = sExe.ReverseFind('/');

		char cBufferCurrent[MAX_PATH];
		GetTempPath(MAX_PATH, cBufferCurrent);
		sExeTemp  = theApp.GetProfileString(registrySectionSettings,registryUpdateFolder,cBufferCurrent);
		sExeTemp+= sExe.Mid(iPos+1);

		sHttp = "http://" + sExe;
		m_progress.SetPos(0);
		result = URLDownloadToFile(NULL, sHttp,  sExeTemp,  0,  &callback);
		if (result == S_OK)
		{
			// ok now check if the file is there
			iStatus = _access(sExeTemp, 0);
			if (iStatus == -1)
			{
				sTxt = gszTranslation[PosDialogUpdateUnableToStore];
				sTxt+= ": ";
				sTxt+= sExeTemp;
				AfxMessageBox(sTxt);
				return;
			}
  
			ShellExecute(NULL,_T("open"),sExeTemp,NULL,NULL,SW_SHOWNORMAL);
		}
		else
		{
			iStatus = result;
			sTxt = gszTranslation[PosDialogUpdateUnableToDownload];
			sTxt+= ": ";
			sTxt+= sHttp;
			sFormat.Format(" (%d)", iStatus);
			sTxt+= sFormat;
			AfxMessageBox(sTxt);
			return;
		}
	}

	theApp.m_pMainFrame->PostMessage(UWM_MSG_EXIT,0,0);
}

void CDlgUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_UPDATE, m_textBox);
	DDX_Control(pDX, IDUPDATE, m_buttonUpdate);
	DDX_Control(pDX, IDUPDATECHECK, m_buttonCheck);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_CHECK_UPDATE_AUTO, m_checkAuto);
	DDX_Control(pDX, IDC_CHECK_UPDATE_BETA, m_checkBeta);
	//	DDX_Control(pDX, IDC_RICHEDIT21_UPDATE, m_richEdit);
	DDX_Control(pDX, IDC_UPDATE_PROGRESS, m_progress);
	DDX_Control(pDX, IDUPDATE_FOLDER, m_updateFolder);
	DDX_Control(pDX, IDC_EDIT1, m_editUpdateFolder);
	DDX_Control(pDX, IDC_TEXT_UPDATE_TEMP, m_textUpdateTemp);
	DDX_Control(pDX, IDC_CHECK_UPDATE_WEB, m_checkUseBrowser);
}


BEGIN_MESSAGE_MAP(CDlgUpdate, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDUPDATE, &CDlgUpdate::OnBnClickedUpdate)
	ON_BN_CLICKED(IDUPDATECHECK, &CDlgUpdate::OnBnClickedUpdatecheck)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_AUTO, &CDlgUpdate::OnBnClickedCheckUpdateAuto)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_BETA, &CDlgUpdate::OnBnClickedCheckUpdateBeta)
	ON_BN_CLICKED(IDUPDATE_FOLDER, &CDlgUpdate::OnBnClickedFolder)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_WEB, &CDlgUpdate::OnBnClickedCheckUpdateWeb)
	ON_MESSAGE (UWM_MSG_NOTICES_BT_UPDATE, OnUpdate)
END_MESSAGE_MAP()


// CDlgUpdate message handlers

void CDlgUpdate::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bStartUpdate)
	{
		Update(true);
		m_bStartUpdate = false;
	}
	else
	{
		// only once a day
		UINT uiLastTime = theApp.GetProfileInt(registrySectionGeneral, registryUpdateTime, 0);
		time_t tSystemTime;
		_time64(&tSystemTime);
		time_t tNextTime = uiLastTime * (60 * 60);	// in hours

		if (tSystemTime > tNextTime)
		{
			CheckForNewVersion(false);
			ShowVersion();					// show it in the dialog
			uiLastTime = (UINT) (tSystemTime / (60*60));	// hour
			theApp.WriteProfileInt(registrySectionGeneral, registryUpdateTime, uiLastTime+24);	// next time + 24 hour
		}
	}
	if (m_pTimer != 0)
	{
		KillTimer(m_pTimer);
	}

	m_pTimer = SetTimer(UTIMER_DLG_UPDATE, 1*60*60*1000, 0);		//1 hour timer
}

HRESULT CUpdateCallback::OnProgress ( ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText ) 
{
	theApp.m_pDlgUpdate->m_progress.SetRange(0, (short) ulProgressMax);
	theApp.m_pDlgUpdate->m_progress.SetPos(ulProgress);
	return S_OK; 
}

LRESULT CDlgUpdate::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	this->ShowWindow(SW_SHOW);
	m_buttonUpdate.ShowWindow(SW_HIDE);
	m_buttonCheck.ShowWindow(SW_HIDE);
	m_buttonOk.ShowWindow(SW_HIDE);

	if (m_pTimer != 0)	KillTimer(m_pTimer);
	m_pTimer = SetTimer(UTIMER_DLG_UPDATE, 2, 0);		//2 second delay
	m_bStartUpdate = true;
	return 0;
}

void CDlgUpdate::OnBnClickedUpdate()
{
	m_buttonUpdate.ShowWindow(SW_HIDE);
	m_buttonCheck.ShowWindow(SW_HIDE);
	m_buttonOk.ShowWindow(SW_HIDE);
	Update(false);
}

void CDlgUpdate::OnBnClickedUpdatecheck()
{
	int iVersionCount;
	iVersionCount = CheckForNewVersion(true);	
	ShowVersion();
}

void CDlgUpdate::OnBnClickedCheckUpdateAuto()
{
	m_iCheckAuto = m_checkAuto.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryUpdateAuto, m_iCheckAuto);
}

void CDlgUpdate::OnBnClickedCheckUpdateBeta()
{
	m_iCheckBeta = m_checkBeta.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryUpdateBeta, m_iCheckBeta);
}

void CDlgUpdate::OnBnClickedCheckUpdateWeb()
{
	int	iCheck;

	iCheck = m_checkUseBrowser.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryUpdateUseBrowser, iCheck);

	m_bUseBrowser = (iCheck != 0);

}

void CDlgUpdate::OnBnClickedFolder()
{
	BOOL	bResult;
	char cBufferSelected[MAX_PATH], cBufferCurrent[MAX_PATH];
	CString sCurrent;
	GetTempPath(MAX_PATH, cBufferCurrent);

	sCurrent  = theApp.GetProfileString(registrySectionSettings,registryUpdateFolder,cBufferCurrent);

	strcpy_s(cBufferCurrent, sCurrent);

	bResult = BrowseForFolder(this->m_hWnd, cBufferCurrent, cBufferSelected);
	if (bResult == IDOK)
	{
		sCurrent = cBufferSelected;
		sCurrent+= "\\";
		theApp.WriteProfileString(registrySectionSettings,registryUpdateFolder, sCurrent);
		m_editUpdateFolder.SetWindowText(sCurrent);
	}
}



// DlgSettingsExpert.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "RemoteCombinedDoc.h"
#include "ListViewEx.h"
#include "TemplateRemoteProjectView.h" 
#include "RemoteCombinedProjectView.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsExpert.h"
#include "MainFrm.h"
#include <direct.h>
#include "Xml.h"
#include "Translation.h"

// CDlgSettingsExpert dialog

IMPLEMENT_DYNAMIC(CDlgSettingsExpert, CPropertyPage)

CDlgSettingsExpert::CDlgSettingsExpert()
	: CPropertyPage(CDlgSettingsExpert::IDD)
{
	m_iReconnectTime = 0;
//	m_iPriority = THREAD_PRIORITY_NORMAL;
}

CDlgSettingsExpert::~CDlgSettingsExpert()
{
}

void CDlgSettingsExpert::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_XML_PATH, m_xmlPath);
	DDX_Control(pDX, IDC_CHECK_XML_PATH, m_checkPath);
	//	DDX_Control(pDX, IDC_CHECK_QUICK_AND_DIRTY, m_quick);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_CHECK_ENCRYPTION, m_checkEncryption);
	DDX_Control(pDX, IDC_EDIT_RECONNECT_TIME, m_editReconnect);
	DDX_Control(pDX, IDC_TEXT_EXPERT_RECONNECT, m_textReconnect);

	DDX_Control(pDX, IDC_EDIT_THREAD_TIMEOUT, m_editThreadTimeout);
	DDX_Control(pDX, IDC_TEXT_EXPERT_THREAD_TIMEOUT, m_textThreadTimeout);

	DDX_Control(pDX, IDC_CHECK_SILENT_REBOOT, m_checkSilent);
	DDX_Control(pDX, IDC_COMBO_PRIORITY, m_comboPriority);
	DDX_Control(pDX, IDC_TEXT_EXPERT_THREAD_PRIORITY, m_textPriority);
	DDX_Control(pDX, IDC_CHECK_THREAD_TIMES, m_checkEnableThreadGraph);
	DDX_Control(pDX, ID_BUTTON_THREAD_GRAPHIC, m_buttonThreadGraph);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDC_EDIT_CPID, m_editCPID);
	DDX_Control(pDX, IDC_EDIT_CPID2, m_editCPID2);
	DDX_Control(pDX, IDC_CHECK_CPID_MANUAL, m_checkCPID);
	DDX_Control(pDX, IDC_TEXT_CPID, m_textCPID);
}
BOOL CDlgSettingsExpert::OnInitDialog()
{
	CString sDirectory;
	int		iCheck;
	CXml	xml("");

	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	m_bUseEncryptionChanged = false;

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsExpertTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	sDirectory = ": ";
	sDirectory+= xml.GetUserDirectory("");
	sDirectory = gszTranslation[PosGroupDialogSettingsExpertOverride] + sDirectory;
	m_checkPath.SetWindowText(sDirectory);

	m_checkEncryption.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertEncryption]);
	m_checkCPID.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertManualCPID]);
	m_textReconnect.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertReconnect]);
	m_textThreadTimeout.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertThreadTimeout]);
	m_textPriority.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertPriority]);

	m_checkSilent.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertSilentReboot]);
	m_checkEnableThreadGraph.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertTimes]);
	m_buttonThreadGraph.SetWindowText(gszTranslation[PosGroupDialogSettingsExpertTimesGraph]);
	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryEncryptionPassword, 0);
	m_checkEncryption.SetCheck(iCheck);
	m_bUseEncryption = (iCheck == TRUE);

	CString sPath;
	sPath = theApp.GetProfileString(registrySectionSettings,registryPath, "");
	m_xmlPath.SetWindowText(sPath);
	m_sPath = sPath;

	if (sPath.GetLength() > 0) m_checkPath.SetCheck(TRUE);
	else m_xmlPath.ShowWindow(SW_HIDE);

	CString sFormat;
	m_iReconnectTime = theApp.GetProfileInt(registrySectionSettings, registryReconnectTime, RECONNECT_INTERVAL);
	sFormat.Format("%d", m_iReconnectTime);
	m_editReconnect.SetWindowText(sFormat);

	m_iThreadTimeout = theApp.GetProfileInt(registrySectionSettings, registryThreadTimeout, TIMEOUT_THREAD_LOCKED_DEFAULT);
	sFormat.Format("%d", m_iThreadTimeout);
	m_editThreadTimeout.SetWindowText(sFormat);
	
	iCheck = theApp.GetProfileInt(registrySectionSettings, registrySilentReboot, 1);
	m_checkSilent.SetCheck(iCheck);
	g_bSilentReboot = (iCheck == 1);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryEnableThreadGraph, 0);
	m_checkEnableThreadGraph.SetCheck(iCheck);
	m_bEnableThreadGraph = (iCheck != 0);
	if (m_bEnableThreadGraph) m_buttonThreadGraph.EnableWindow(TRUE);

	m_comboPriority.AddString(gszTranslation[PosGroupDialogSettingsExpertPriorityNormal]);
	m_comboPriority.AddString(gszTranslation[PosGroupDialogSettingsExpertPriorityBelow]);
	m_comboPriority.AddString(gszTranslation[PosGroupDialogSettingsExpertPriorityLow]);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryThreadPriority, 0);
	m_comboPriority.SetCurSel(iCheck);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryManualCPID, 0);
	ShowCPID(iCheck);
	m_checkCPID.SetCheck(iCheck);
	m_bCpidManual = (iCheck != 0);

	m_sCpid = theApp.GetProfileString(registrySectionSettings, registryCPID, "");
	m_editCPID.SetWindowText(m_sCpid.c_str());
	m_sCpid2 = theApp.GetProfileString(registrySectionSettings, registryCPID2, "");
	m_editCPID2.SetWindowText(m_sCpid2.c_str());

//	if (m_bCpidManual)
//	{
//		SendCPID();
//	}
	SetTimer(UTIMER_DLG_SETTING_EXPERT, 2000, 0);	// interval 2 second

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsExpert::SetReconnect(int iTime)
{
	POSITION	posDoc, posView;
	CView		*pView;
	CDocument	 *pDoc;
	CString		sComputer;

	sComputer = COMPUTER_DUMMY_SET_TIME; // all computers

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	while (posDoc)
	{
		pDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);				// there is only one doc
		if (pDoc != NULL)
		{
			posView = pDoc->GetFirstViewPosition();
			while (posView)
			{
				pView = pDoc->GetNextView(posView);
				if (pView == NULL) break;
				pView->SendMessage(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, (WPARAM) &sComputer,iTime);	// only the view that is active will get it.
			}
		}
	}
}

void CDlgSettingsExpert::SendCPID()
{
	CRemoteCombinedDoc	*pDoc;
	POSITION	posDoc;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			if (!m_bCpidManual)
			{
				pDoc->m_pProjectView->PostMessage(UWM_MSG_THREAD_CPID_CHANGED, NULL, (LPARAM) NULL);
			}
			else
			{
				pDoc->m_pProjectView->PostMessage(UWM_MSG_THREAD_CPID_CHANGED, (WPARAM) &m_sCpid, (LPARAM) &m_sCpid2);
			}
		}
	}
}

void CDlgSettingsExpert::ShowCPID(int iCheck)
{
	int iShow;

	if (iCheck)	iShow = SW_SHOW;
	else iShow = SW_HIDE;

	m_textCPID.ShowWindow(iShow);
	m_editCPID.ShowWindow(iShow);
	m_editCPID2.ShowWindow(iShow);
}


BEGIN_MESSAGE_MAP(CDlgSettingsExpert, CPropertyPage)
	ON_WM_TIMER()
	ON_MESSAGE(UWM_MSG_THREAD_RECONNECT_TIME_RPC,OnGiveReconnectTime)

	ON_BN_CLICKED(IDOK, &CDlgSettingsExpert::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_XML_PATH, &CDlgSettingsExpert::OnBnClickedCheckXmlPath)
//	ON_BN_CLICKED(IDC_CHECK_QUICK_AND_DIRTY, &CDlgSettingsExpert::OnBnClickedCheckQuickAndDirty)
	ON_BN_CLICKED(IDC_CHECK_ENCRYPTION, &CDlgSettingsExpert::OnBnClickedCheckEncryption)
	ON_EN_CHANGE(IDC_EDIT_RECONNECT_TIME, &CDlgSettingsExpert::OnEnChangeEditReconnectTime)
	ON_EN_CHANGE(IDC_EDIT_THREAD_TIMEOUT, &CDlgSettingsExpert::OnEnChangeEditThreadTimeout)
	ON_BN_CLICKED(IDC_CHECK_SILENT_REBOOT, &CDlgSettingsExpert::OnBnClickedCheckSilentReboot)
	ON_CBN_SELCHANGE(IDC_COMBO_PRIORITY, &CDlgSettingsExpert::OnCbnSelchangeComboPriority)
	ON_BN_CLICKED(IDC_CHECK_THREAD_TIMES, &CDlgSettingsExpert::OnBnClickedCheckThreadTimes)
	ON_BN_CLICKED(ID_BUTTON_THREAD_GRAPHIC, &CDlgSettingsExpert::OnBnClickedButtonThreadGraphic)
	ON_WM_HELPINFO()	
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsExpert::OnBnClickedHelp)
	ON_EN_CHANGE(IDC_EDIT_CPID, &CDlgSettingsExpert::OnEnChangeEditCpid)
	ON_EN_CHANGE(IDC_EDIT_CPID2, &CDlgSettingsExpert::OnEnChangeEditCpid2)
	ON_BN_CLICKED(IDC_CHECK_CPID_MANUAL, &CDlgSettingsExpert::OnBnClickedCheckCpidManual)
END_MESSAGE_MAP()


// CDlgSettingsExpert message handlers

void CDlgSettingsExpert::OnTimer(UINT_PTR nIDEvent)
{
	SendCPID();
	KillTimer(UTIMER_DLG_SETTING_EXPERT);
}

LRESULT CDlgSettingsExpert::OnGiveReconnectTime(WPARAM wParam, LPARAM lParam)
{
	SetReconnect(m_iReconnectTime);
	return 0;
}

void CDlgSettingsExpert::OnBnClickedOk()
{
	CString sPath, sTxt;

	if (!m_checkPath.GetCheck()) m_xmlPath.SetWindowText("");

	m_xmlPath.GetWindowText(sPath);


	sPath.Replace(' ',NULL);

	if (sPath.GetLength() > 0)
	{
		if(_chdir(sPath))
		{
			sTxt = sPath + "";
			AfxMessageBox(sPath + " " + gszTranslation[PosGroupDialogSettingsExpertInvalidFolder]);
			return;
		}
	}

	theApp.WriteProfileString(registrySectionSettings,registryPath , sPath);
	m_sPath = sPath;

	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}
void CDlgSettingsExpert::OnBnClickedCheckXmlPath()
{
	if (m_checkPath.GetCheck())
	{
//		m_checkPath.SetCheck(FALSE);
		m_xmlPath.ShowWindow(SW_SHOW);
	}
	else
	{
//		m_checkPath.SetCheck(TRUE);
		m_xmlPath.ShowWindow(SW_HIDE);
	}
	AfxMessageBox(gszTranslation[PosGroupDialogSettingsExpertOverrideWarning]);
}

//void CDlgSettingsExpert::OnBnClickedCheckQuickAndDirty()
//{
//	int iCheck;
//	iCheck = m_quick.GetCheck();
//
//	if (iCheck)	m_iQuickAndDirty = true;
//	else		m_iQuickAndDirty = false;
//
//	theApp.WriteProfileInt(registrySectionSettings,registryQuickAndDirty , iCheck);
//}

void CDlgSettingsExpert::OnBnClickedCheckEncryption()
{
	int iCheck;
	iCheck = m_checkEncryption.GetCheck();

	if (iCheck)	m_bUseEncryption = true;
	else		m_bUseEncryption = false;

	theApp.WriteProfileInt(registrySectionSettings,registryEncryptionPassword , iCheck);


	m_bUseEncryptionChanged = true;
}

void CDlgSettingsExpert::OnEnChangeEditReconnectTime()
{
	CString sTxt;

	m_editReconnect.GetWindowText(sTxt);
	m_iReconnectTime = atoi(sTxt);
	theApp.WriteProfileInt(registrySectionSettings, registryReconnectTime, m_iReconnectTime);
	SetReconnect(m_iReconnectTime);
}

void CDlgSettingsExpert::OnEnChangeEditThreadTimeout()
{
	CString sTxt;

	m_editThreadTimeout.GetWindowText(sTxt);
	m_iThreadTimeout = atoi(sTxt);
	theApp.WriteProfileInt(registrySectionSettings, registryThreadTimeout, m_iThreadTimeout);
	theApp.m_iThreadTimeout = m_iThreadTimeout;
}

void CDlgSettingsExpert::OnBnClickedCheckSilentReboot()
{
	int iCheck;

	iCheck = m_checkSilent.GetCheck();
	g_bSilentReboot = (iCheck == 1);
	theApp.WriteProfileInt(registrySectionSettings, registrySilentReboot, iCheck);
}


void CDlgSettingsExpert::OnCbnSelchangeComboPriority()
{
	int iCurSel;

	iCurSel = m_comboPriority.GetCurSel();
	::PostMessage(theApp.m_pMainFrame->m_hWnd, UWM_SET_THREAD_PRIORITY, iCurSel, 0);
	theApp.WriteProfileInt(registrySectionSettings, registryThreadPriority, iCurSel);
}


void CDlgSettingsExpert::OnBnClickedCheckThreadTimes()
{
	int	iCheck;

	iCheck = m_checkEnableThreadGraph.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryEnableThreadGraph, iCheck);
//	m_bEnableThreadGraph = (iCheck != 0);	only on a restart
}


void CDlgSettingsExpert::OnBnClickedButtonThreadGraphic()
{
	POSITION posDoc, posView ;
	CRemoteCombinedDoc *pDocCombined;
	CView *pView;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc)
	{
		pDocCombined = (CRemoteCombinedDoc*) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		posView = pDocCombined->GetFirstViewPosition();
		while (posView)
		{
			pView = pDocCombined->GetNextView(posView);
			pView->SendMessage(WM_COMMAND, UWM_MSG_SHOW_THREAD_GRAPH,0);	// fist send to the document that sends it to the view
		}
	}
}

BOOL CDlgSettingsExpert::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsExpert::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#expert";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

void CDlgSettingsExpert::OnBnClickedCheckCpidManual()
{
	int iCheck;

	iCheck = m_checkCPID.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryManualCPID, iCheck);
	ShowCPID(iCheck);
	m_bCpidManual = (iCheck != 0);
	KillTimer(UTIMER_DLG_SETTING_EXPERT);
	SetTimer(UTIMER_DLG_SETTING_EXPERT, 10000, 0);	// interval 10 second
}

void CDlgSettingsExpert::OnEnChangeEditCpid()
{
	CString sCPID;

	m_editCPID.GetWindowText(sCPID);
	m_sCpid = sCPID;
	theApp.WriteProfileString(registrySectionSettings, registryCPID, sCPID);
	KillTimer(UTIMER_DLG_SETTING_EXPERT);
	SetTimer(UTIMER_DLG_SETTING_EXPERT, 10000, 0);	// interval 10 second
}


void CDlgSettingsExpert::OnEnChangeEditCpid2()
{
	CString sCPID;

	m_editCPID2.GetWindowText(sCPID);
	m_sCpid2 = sCPID;
	theApp.WriteProfileString(registrySectionSettings, registryCPID2, sCPID);
	KillTimer(UTIMER_DLG_SETTING_EXPERT);
	SetTimer(UTIMER_DLG_SETTING_EXPERT, 10000, 0);	// interval 10 second
}


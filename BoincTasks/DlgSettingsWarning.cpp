// DlgSettingsWarning.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsWarning.h"
#include "NumberFormat.h"
#include "Translation.h"

// CDlgSettingsWarning dialog

IMPLEMENT_DYNAMIC(CDlgSettingsWarning, CPropertyPage)

CDlgSettingsWarning::CDlgSettingsWarning()
	: CPropertyPage(CDlgSettingsWarning::IDD)
{

}

CDlgSettingsWarning::~CDlgSettingsWarning()
{
}

BOOL CDlgSettingsWarning::OnInitDialog()
{	
	CPropertySheet *pSheet;
	CString sTxt;
	int iNr, iCheck;
	CNumberFormat numberFormat;
	
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsWarningsTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_checkCheckpoint.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningCheckpoint]);

	m_textWarning.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningDeadline]);
	m_textDaysAnd.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningDeadlineDays]);
	m_textHours.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningDeadlinehours]);
	m_textWarningIsActive.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningIsActiveWhen]);

	m_groupCpuBetween.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningCpuBetween]);
	m_groupGpuBetween.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningGpuBetween]);
	m_groupCpuPercentageBetween.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningCpuPBetween]);

	m_groupCpuTasks.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningCpuTasks]);
	m_groupGpuTasks.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningGpuTasks]);
	m_groupProject.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningProject]);
	m_groupComputer.SetWindowText(gszTranslation[PosGroupDialogSettingsWarningComputer]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	iNr = theApp.GetProfileInt(registrySectionSettings, registryWarningDeadlineDays, 0);
	if (iNr > 23) iNr = 23;
	if (iNr < 0) iNr = 0;
	m_deadlineDays.SetCurSel(iNr);
	m_iDeadlineTime = iNr*24;

	iNr = theApp.GetProfileInt(registrySectionSettings, registryWarningDeadlineHours, 0);
	if (iNr > 23) iNr = 23;
	if (iNr < 0) iNr = 0;
	m_deadlineHours.SetCurSel(iNr);
	m_iDeadlineTime += iNr;
	m_iDeadlineTime *= 60;

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryWarningCheckpoint, TRUE);
	m_checkCheckpoint.SetCheck(iCheck);
	m_bCheckpoint = (iCheck != 0);
	
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuLow1, "1")	;m_cpuLow1.SetWindowText(sTxt); m_fWarningCpuLow1 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuLow2, "0.01")	;m_cpuLow2.SetWindowText(sTxt); m_fWarningCpuLow2 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuLow3, "")		;m_cpuLow3.SetWindowText(sTxt); m_fWarningCpuLow3 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuLow4, "")		;m_cpuLow4.SetWindowText(sTxt); m_fWarningCpuLow4 = numberFormat.atofLocale(sTxt);

	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuHigh1,"1")	;m_cpuHigh1.SetWindowText(sTxt); m_fWarningCpuHigh1 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuHigh2,"0.5")	;m_cpuHigh2.SetWindowText(sTxt); m_fWarningCpuHigh2 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuHigh3,"")		;m_cpuHigh3.SetWindowText(sTxt); m_fWarningCpuHigh3 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuHigh4,"")		;m_cpuHigh4.SetWindowText(sTxt); m_fWarningCpuHigh4 = numberFormat.atofLocale(sTxt);

	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuLow1, "0")	;m_gpuLow1.SetWindowText(sTxt); m_fWarningGpuLow1 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuLow2, "1")	;m_gpuLow2.SetWindowText(sTxt); m_fWarningGpuLow2 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuLow3, "")		;m_gpuLow3.SetWindowText(sTxt); m_fWarningGpuLow3 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuLow4, "")		;m_gpuLow4.SetWindowText(sTxt); m_fWarningGpuLow4 = numberFormat.atofLocale(sTxt);

	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuHigh1,"0")	;m_gpuHigh1.SetWindowText(sTxt); m_fWarningGpuHigh1 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuHigh2,"1")	;m_gpuHigh2.SetWindowText(sTxt); m_fWarningGpuHigh2 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuHigh3,"")		;m_gpuHigh3.SetWindowText(sTxt); m_fWarningGpuHigh3 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuHigh4,"")		;m_gpuHigh4.SetWindowText(sTxt); m_fWarningGpuHigh4 = numberFormat.atofLocale(sTxt);

	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercLow1,"0")	;m_percLow1.SetWindowText(sTxt); m_fWarningPercLow1 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercLow2,"50")	;m_percLow2.SetWindowText(sTxt); m_fWarningPercLow2 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercLow3,"")		;m_percLow3.SetWindowText(sTxt); m_fWarningPercLow3 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercLow4,"")		;m_percLow4.SetWindowText(sTxt); m_fWarningPercLow4 = numberFormat.atofLocale(sTxt);

	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercHigh1,"50")	;m_percHigh1.SetWindowText(sTxt); m_fWarningPercHigh1 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercHigh2,"100")	;m_percHigh2.SetWindowText(sTxt); m_fWarningPercHigh2 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercHigh3,"")	;m_percHigh3.SetWindowText(sTxt); m_fWarningPercHigh3 = numberFormat.atofLocale(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningPercHigh4,"")	;m_percHigh4.SetWindowText(sTxt); m_fWarningPercHigh4 = numberFormat.atofLocale(sTxt);

	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuTasks1,"")	;m_cpuTasks1.SetWindowText(sTxt); m_iWarningCpuTasks1 = atoi(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuTasks2,"")	;m_cpuTasks2.SetWindowText(sTxt); m_iWarningCpuTasks2 = atoi(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuTasks3,"")	;m_cpuTasks3.SetWindowText(sTxt); m_iWarningCpuTasks3 = atoi(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningCpuTasks4,"")	;m_cpuTasks4.SetWindowText(sTxt); m_iWarningCpuTasks4 = atoi(sTxt);

	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuTasks1,"")	;m_gpuTasks1.SetWindowText(sTxt); m_iWarningGpuTasks1 = atoi(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuTasks2,"")	;m_gpuTasks2.SetWindowText(sTxt); m_iWarningGpuTasks2 = atoi(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuTasks3,"")	;m_gpuTasks3.SetWindowText(sTxt); m_iWarningGpuTasks3 = atoi(sTxt);
	sTxt = theApp.GetProfileString(registrySectionWarnings, registryWarningGpuTasks4,"")	;m_gpuTasks4.SetWindowText(sTxt); m_iWarningGpuTasks4 = atoi(sTxt);

	m_sProject1 = theApp.GetProfileString(registrySectionWarnings, registryWarningProject1, ""); m_project1.SetWindowText(m_sProject1);
	m_sProject2 = theApp.GetProfileString(registrySectionWarnings, registryWarningProject2, ""); m_project2.SetWindowText(m_sProject2);
	m_sProject3 = theApp.GetProfileString(registrySectionWarnings, registryWarningProject3, ""); m_project3.SetWindowText(m_sProject3);
	m_sProject4 = theApp.GetProfileString(registrySectionWarnings, registryWarningProject4, ""); m_project4.SetWindowText(m_sProject4);

	m_sComputer1 = theApp.GetProfileString(registrySectionWarnings, registryWarningComputer1, ""); m_computer1.SetWindowText(m_sComputer1);
	m_sComputer2 = theApp.GetProfileString(registrySectionWarnings, registryWarningComputer2, ""); m_computer2.SetWindowText(m_sComputer2);
	m_sComputer3 = theApp.GetProfileString(registrySectionWarnings, registryWarningComputer3, ""); m_computer3.SetWindowText(m_sComputer3);
	m_sComputer4 = theApp.GetProfileString(registrySectionWarnings, registryWarningComputer4, ""); m_computer4.SetWindowText(m_sComputer4);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsWarning::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEADLINE_DAY, m_deadlineDays);
	DDX_Control(pDX, IDC_COMBO_DEADLINE_HOUR, m_deadlineHours);
	//	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_PERC, m_efficiencyCpuBelow);
	//	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_PERC, m_efficiencyGpuAbove);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_LOW1, m_cpuLow1);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_LOW2, m_cpuLow2);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_LOW4, m_cpuLow4);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_HIGH1, m_cpuHigh1);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_HIGH2, m_cpuHigh2);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_HIGH3, m_cpuHigh3);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_HIGH4, m_cpuHigh4);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_LOW3, m_cpuLow3);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_LOW1, m_gpuLow1);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_LOW2, m_gpuLow2);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_LOW3, m_gpuLow3);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_LOW4, m_gpuLow4);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_HIGH1, m_gpuHigh1);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_HIGH2, m_gpuHigh2);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_HIGH3, m_gpuHigh3);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_HIGH4, m_gpuHigh4);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_LOW1, m_percLow1);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_LOW2, m_percLow2);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_LOW3, m_percLow3);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_LOW4, m_percLow4);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_HIGH1, m_percHigh1);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_HIGH2, m_percHigh2);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_HIGH3, m_percHigh3);
	DDX_Control(pDX, IDC_EDIT_WARNING_USE_HIGH4, m_percHigh4);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_TEXT_WARNING_DEADLINE, m_textWarning);
	DDX_Control(pDX, IDC_TEXT_WARNING_DEADLINE_DAYS, m_textDaysAnd);
	DDX_Control(pDX, IDC_TEXT_WARNING_DEADLINE_HOURS, m_textHours);
	DDX_Control(pDX, IDC_TEXT_WARNING_IS_ACTIVE, m_textWarningIsActive);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_TASKS1, m_cpuTasks1);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_TASKS2, m_cpuTasks2);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_TASKS3, m_cpuTasks3);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_TASKS4, m_cpuTasks4);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_TASKS1, m_gpuTasks1);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_TASKS2, m_gpuTasks2);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_TASKS3, m_gpuTasks3);
	DDX_Control(pDX, IDC_EDIT_WARNING_GPU_TASKS4, m_gpuTasks4);
	DDX_Control(pDX, IDC_EDIT_WARNING_PROJECT1, m_project1);
	DDX_Control(pDX, IDC_EDIT_WARNING_PROJECT2, m_project2);
	DDX_Control(pDX, IDC_EDIT_WARNING_PROJECT3, m_project3);
	DDX_Control(pDX, IDC_EDIT_WARNING_PROJECT4, m_project4);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_COMPUTER1, m_computer1);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_COMPUTER2, m_computer2);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_COMPUTER3, m_computer3);
	DDX_Control(pDX, IDC_EDIT_WARNING_CPU_COMPUTER4, m_computer4);
	DDX_Control(pDX, IDC_TEXT_GROUP_GPU_TASKS, m_groupGpuTasks);
	DDX_Control(pDX, IDC_TEXT_GROUP_CPU_TASKS, m_groupCpuTasks);
	DDX_Control(pDX, IDC_TEXT_GROUP_PROJECT, m_groupProject);
	DDX_Control(pDX, IDC_TEXT_GROUP_COMPUTER, m_groupComputer);
	DDX_Control(pDX, IDC_TEXT_WARNING_GROUP_CPU_BETWEEN, m_groupCpuBetween);
	DDX_Control(pDX, IDC_TEXT_WARNING_GROUP_GPU_BETWEEN, m_groupGpuBetween);
	DDX_Control(pDX, IDC_TEXT_WARNING_GROUP_CPU_PERC_BETWEEN, m_groupCpuPercentageBetween);
	DDX_Control(pDX, IDC_CHECK_WARNING_CHECKPOINT, m_checkCheckpoint);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(CDlgSettingsWarning, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CDlgSettingsWarning::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_DEADLINE_DAY, &CDlgSettingsWarning::OnCbnSelchangeComboDeadlineDay)
	ON_CBN_SELCHANGE(IDC_COMBO_DEADLINE_HOUR, &CDlgSettingsWarning::OnCbnSelchangeComboDeadlineHour)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_HIGH1, &CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_LOW1, &CDlgSettingsWarning::OnEnChangeEditWarningCpuLow1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_LOW2, &CDlgSettingsWarning::OnEnChangeEditWarningCpuLow2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_LOW3, &CDlgSettingsWarning::OnEnChangeEditWarningCpuLow3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_LOW4, &CDlgSettingsWarning::OnEnChangeEditWarningCpuLow4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_HIGH1, &CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_HIGH2, &CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_HIGH3, &CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_HIGH4, &CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_LOW1, &CDlgSettingsWarning::OnEnChangeEditWarningGpuLow1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_LOW2, &CDlgSettingsWarning::OnEnChangeEditWarningGpuLow2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_LOW3, &CDlgSettingsWarning::OnEnChangeEditWarningGpuLow3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_LOW4, &CDlgSettingsWarning::OnEnChangeEditWarningGpuLow4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_HIGH2, &CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_HIGH3, &CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_HIGH4, &CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_LOW1, &CDlgSettingsWarning::OnEnChangeEditWarningUseLow1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_LOW2, &CDlgSettingsWarning::OnEnChangeEditWarningUseLow2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_LOW3, &CDlgSettingsWarning::OnEnChangeEditWarningUseLow3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_LOW4, &CDlgSettingsWarning::OnEnChangeEditWarningUseLow4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_HIGH1, &CDlgSettingsWarning::OnEnChangeEditWarningUseHigh1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_HIGH2, &CDlgSettingsWarning::OnEnChangeEditWarningUseHigh2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_HIGH3, &CDlgSettingsWarning::OnEnChangeEditWarningUseHigh3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_USE_HIGH4, &CDlgSettingsWarning::OnEnChangeEditWarningUseHigh4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_TASKS1, &CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_TASKS2, &CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_TASKS3, &CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_TASKS4, &CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_TASKS1, &CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_TASKS2, &CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_TASKS3, &CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_GPU_TASKS4, &CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_PROJECT1, &CDlgSettingsWarning::OnEnChangeEditWarningProject1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_PROJECT2, &CDlgSettingsWarning::OnEnChangeEditWarningProject2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_PROJECT3, &CDlgSettingsWarning::OnEnChangeEditWarningProject3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_PROJECT4, &CDlgSettingsWarning::OnEnChangeEditWarningProject4)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_COMPUTER1, &CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer1)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_COMPUTER2, &CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer2)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_COMPUTER3, &CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer3)
	ON_EN_CHANGE(IDC_EDIT_WARNING_CPU_COMPUTER4, &CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer4)
	ON_BN_CLICKED(IDC_CHECK_WARNING_CHECKPOINT, &CDlgSettingsWarning::OnBnClickedCheckWarningCheckpoint)

	ON_WM_HELPINFO()	
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsWarning::OnBnClickedHelp)
END_MESSAGE_MAP()


// CDlgSettingsWarning message handlers

void CDlgSettingsWarning::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsWarning::OnCbnSelchangeComboDeadlineDay()
{
	int iNr, iNewDeadline;

	iNr = m_deadlineDays.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryWarningDeadlineDays, iNr);
	iNewDeadline = iNr*24;
	iNr = m_deadlineHours.GetCurSel();
	iNewDeadline += iNr;
	m_iDeadlineTime = iNewDeadline * 60;
}

void CDlgSettingsWarning::OnCbnSelchangeComboDeadlineHour()
{
	int iNr, iNewDeadline;

	iNr = m_deadlineDays.GetCurSel();
	iNewDeadline = iNr*24;
	iNr = m_deadlineHours.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryWarningDeadlineHours, iNr);
	iNewDeadline += iNr;
	m_iDeadlineTime = iNewDeadline * 60;
}

void CDlgSettingsWarning::OnBnClickedCheckWarningCheckpoint()
{
	int iCheck;

	iCheck = m_checkCheckpoint.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryWarningCheckpoint, iCheck);
	m_bCheckpoint = (iCheck != 0);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuLow1()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuLow1.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuLow1 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuLow1, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuLow2()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuLow2.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuLow2 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuLow2, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuLow3()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuLow3.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuLow3 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuLow3, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuLow4()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuLow4.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuLow4 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuLow4, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh1()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuHigh1.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuHigh1 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuHigh1, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh2()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuHigh2.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuHigh2 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuHigh2, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh3()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuHigh3.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuHigh3 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuHigh3, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningCpuHigh4()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_cpuHigh4.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningCpuHigh4 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuHigh4, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuLow1()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuLow1.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuLow1 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuLow1, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuLow2()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuLow2.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuLow2 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuLow2, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuLow3()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuLow3.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuLow3 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuLow3, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuLow4()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuLow4.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuLow4 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuLow4, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh1()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuHigh1.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuHigh1 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuHigh1, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh2()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuHigh2.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuHigh2 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuHigh2, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh3()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuHigh3.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuHigh3 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuHigh3, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningGpuHigh4()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_gpuHigh4.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningGpuHigh4 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuHigh4, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseLow1()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percLow1.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercLow1 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercLow1, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseLow2()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percLow2.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercLow2 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercLow2, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseLow3()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percLow3.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercLow3 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercLow3, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseLow4()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percLow4.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercLow4 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercLow4, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseHigh1()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percHigh1.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercHigh1 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercHigh1, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseHigh2()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percHigh2.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercHigh2 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercHigh2, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseHigh3()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percHigh3.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercHigh3 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercHigh3, sTxt);
}

void CDlgSettingsWarning::OnEnChangeEditWarningUseHigh4()
{
	CString sTxt;
	CNumberFormat numberFormat;

	m_percHigh4.GetWindowText(sTxt);
//	sTxt.Replace(",",".");
	m_fWarningPercHigh4 = numberFormat.atofLocale(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningPercHigh4, sTxt);
}



void CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks1()
{
	CString sTxt;

	m_cpuTasks1.GetWindowText(sTxt);
	m_iWarningCpuTasks1 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuTasks1, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks2()
{
	CString sTxt;

	m_cpuTasks2.GetWindowText(sTxt);
	m_iWarningCpuTasks2 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuTasks2, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks3()
{
	CString sTxt;

	m_cpuTasks3.GetWindowText(sTxt);
	m_iWarningCpuTasks3 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuTasks3, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningCpuTasks4()
{
	CString sTxt;

	m_cpuTasks4.GetWindowText(sTxt);
	m_iWarningCpuTasks4 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningCpuTasks4, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks1()
{
	CString sTxt;

	m_gpuTasks1.GetWindowText(sTxt);
	m_iWarningGpuTasks1 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuTasks1, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks2()
{
	CString sTxt;

	m_gpuTasks2.GetWindowText(sTxt);
	m_iWarningGpuTasks2 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuTasks2, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks3()
{
	CString sTxt;

	m_gpuTasks3.GetWindowText(sTxt);
	m_iWarningGpuTasks3 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuTasks3, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningGpuTasks4()
{
	CString sTxt;

	m_gpuTasks4.GetWindowText(sTxt);
	m_iWarningGpuTasks4 = atoi(sTxt);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningGpuTasks4, sTxt);
}


void CDlgSettingsWarning::OnEnChangeEditWarningProject1()
{
	CString sTxt;

	m_project1.GetWindowText(m_sProject1);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningProject1, m_sProject1);
}


void CDlgSettingsWarning::OnEnChangeEditWarningProject2()
{
	CString sTxt;

	m_project2.GetWindowText(m_sProject2);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningProject2, m_sProject2);
}


void CDlgSettingsWarning::OnEnChangeEditWarningProject3()
{
	CString sTxt;

	m_project3.GetWindowText(m_sProject3);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningProject3, m_sProject3);
}


void CDlgSettingsWarning::OnEnChangeEditWarningProject4()
{
	CString sTxt;

	m_project4.GetWindowText(m_sProject4);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningProject4, m_sProject4);
}


void CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer1()
{
	CString sTxt;

	m_computer1.GetWindowText(m_sComputer1);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningComputer1, m_sComputer1);
}


void CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer2()
{
	CString sTxt;

	m_computer2.GetWindowText(m_sComputer2);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningComputer2, m_sComputer2);
}


void CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer3()
{
	CString sTxt;

	m_computer3.GetWindowText(m_sComputer3);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningComputer3, m_sComputer3);
}


void CDlgSettingsWarning::OnEnChangeEditWarningCpuComputer4()
{
	CString sTxt;

	m_computer4.GetWindowText(m_sComputer4);
	theApp.WriteProfileString(registrySectionWarnings, registryWarningComputer4, m_sComputer4);
}

BOOL CDlgSettingsWarning::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsWarning::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#warnings";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

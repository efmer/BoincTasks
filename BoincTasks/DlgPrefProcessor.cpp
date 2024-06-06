// DlgPrefProcessor.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgPrefProcessor.h"
#include "DlgPrefNetwork.h"
#include "DlgPrefMain.h"
#include "ThreadRpc.h"
#include "NumberFormat.h"
#include "DlgScheduler.h"
#include "Translation.h"

// CDlgPrefProcessor dialog

IMPLEMENT_DYNAMIC(CDlgPrefProcessor, CPropertyPage)

CDlgPrefProcessor::CDlgPrefProcessor()
	: CPropertyPage(CDlgPrefProcessor::IDD)
{
	m_window.Setup(48,30,7,true,false);
}

CDlgPrefProcessor::~CDlgPrefProcessor()
{
}

BOOL CDlgPrefProcessor::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogBoincSettingsTitleProcessor];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_textComputingAllowed.SetWindowText(gszTranslation[PosDialogBoincSettingsPComputingTitle]);
	CheckComputingOnBatteries.SetWindowText(gszTranslation[PosDialogBoincSettingsPComputingBatteries]);
	CheckComputingInUse.SetWindowText(gszTranslation[PosDialogBoincSettingsPComputingUseCpu]);
	CheckComputingInUseGpu.SetWindowText(gszTranslation[PosDialogBoincSettingsPComputingUseGpu]);
	m_textOnlyAfterComputerBeenIdleFor.SetWindowText(gszTranslation[PosDialogBoincSettingsPComputingOnlyIdle]);
	m_textMinutes1.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralMinutes]);
	m_textEverydayBetweenTheHoursOf.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralDayBetween]);
	m_textAnd.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralAnd]);
	m_textNoRestrictionsIfEqual.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralNoRestrictions]);
	m_textWhileProcessorUsageIsLessThan.SetWindowText(gszTranslation[PosDialogBoincSettingsPComputingUsageLess]);
	m_textPerc0MeansNoRestrictions.SetWindowText(gszTranslation[PosDialogBoincSettingsPComputingUsageLessP]);

	m_textDayOfWeekOverride.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralDayOfWeekTitle]);
	m_checkDay[0].SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralSunday]);
	m_checkDay[1].SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralMonday]);
	m_checkDay[2].SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralTuesday]);
	m_checkDay[3].SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralWednesday]);
	m_checkDay[4].SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralThursday]);
	m_checkDay[5].SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralFriday]);
	m_checkDay[6].SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralSaturday]);
	m_textHourMinutes.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralHourMinutes]);
	m_checkGraphic.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralGraphic]);

	m_textOtherOptions.SetWindowText(gszTranslation[PosDialogBoincSettingsPOtherTitle]);
	m_textSwitchBetweenApplicationsEvery.SetWindowText(gszTranslation[PosDialogBoincSettingsPOtherSwitchEvery]);
	m_textMinutes2.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralMinutes]);
	m_textOnMultiprocessorSystemsUseAtMost.SetWindowText(gszTranslation[PosDialogBoincSettingsPOtherMultiprocessors]);
	m_textPercOfTheProcessors.SetWindowText(gszTranslation[PosDialogBoincSettingsPOtherPercProcessor]);
	m_textUseAtMost.SetWindowText(gszTranslation[PosDialogBoincSettingsPOtherUseAtMost]);
	m_textPercCpuTime.SetWindowText(gszTranslation[PosDialogBoincSettingsPOtherPercCpu]);

	m_buttonClear.SetWindowText(gszTranslation[PosDialogCommonButtonsClear]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
	// translations

	int iCheck = theApp.GetProfileInt(registrySectionSettings, registryBoincSettingsProcGraph, 1);
	m_checkGraphic.SetCheck(iCheck);
	ShowGraphic(iCheck == 1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDlgPrefProcessor::FillDialog(CRpcPreferenceInfo *pPreferenceInfo)
{
	//C:\programma\BoincTasks\BoincTasks\boinc\boinc_source\boinc\clientgui\DlgAdvPreferences.cpp
	CString sFormat;
	CNumberFormat numberFormat;
	bool	run_gpu_if_user_active;
	
	GLOBAL_PREFS *pPrefs;

	pPrefs = &pPreferenceInfo->prefs;
	
	CheckComputingOnBatteries.SetCheck(pPrefs->run_on_batteries);
	CheckComputingInUse.SetCheck(pPrefs->run_if_user_active);

	run_gpu_if_user_active = pPrefs->run_gpu_if_user_active;
	CheckComputingInUseGpu.SetCheck(run_gpu_if_user_active);

	if (run_gpu_if_user_active)
	{
		ValueComputingIdleMin.EnableWindow(FALSE);
	}

	sFormat = numberFormat.FormatNumberNT(pPrefs->idle_time_to_run,2);
//	sFormat.Format("%.2f",pPrefs->idle_time_to_run);
	ValueComputingIdleMin.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->cpu_scheduling_period_minutes, 2);
//	sFormat.Format("%.2f",pPrefs->cpu_scheduling_period_minutes);
	ValueOtherSwitchEveryMin.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->max_ncpus_pct, 2);
//	sFormat.Format("%.2f",pPrefs->max_ncpus_pct);
	ValueOtherMultiprocPerc.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->cpu_usage_limit, 2);
//	sFormat.Format("%.2f",pPrefs->cpu_usage_limit);
	ValueOtherUseAtMost.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->suspend_cpu_usage, 0);
//	sFormat.Format("%.0f",pPrefs->suspend_cpu_usage);
	ValueWhileProcessorUsageIsLessThan.SetWindowText(sFormat);

	// everday between the hours of
	m_editTimeFrom.SetWindowText(timeStringDouble.DoubleToTimeString(pPrefs->cpu_times.start_hour));
	m_editTimeTo.SetWindowText(timeStringDouble.DoubleToTimeString(pPrefs->cpu_times.end_hour));

	// day-of-week-override
	for(int i=0; i< 7;i++)
	{
        TIME_SPAN& cpu = pPrefs->cpu_times.week.days[i];
		if(cpu.present)
		{
			m_checkDay[i].SetCheck(TRUE);
			m_editDayOfWeek[i][0].EnableWindow(TRUE);
			m_editDayOfWeek[i][1].EnableWindow(TRUE);
			m_editDayOfWeek[i][0].SetWindowText(timeStringDouble.DoubleToTimeString(cpu.start_hour));
			m_editDayOfWeek[i][1].SetWindowText(timeStringDouble.DoubleToTimeString(cpu.end_hour));
		}
		else
		{
			m_checkDay[i].SetCheck(FALSE);
			m_editDayOfWeek[i][0].EnableWindow(FALSE);
			m_editDayOfWeek[i][1].EnableWindow(FALSE);
			m_editDayOfWeek[i][0].SetWindowText("");
			m_editDayOfWeek[i][1].SetWindowText("");
		}
		m_window.TimeSpanToWindow(i,cpu);
	}
}

extern double clamp_pct(double x);

bool CDlgPrefProcessor::ReadDialog(CRpcPreferenceInfo *pPreferenceInfo)
{
	CString sTxt;
	double	td, dStartTime, dEndTime;
	CNumberFormat numberFormat;

	GLOBAL_PREFS *pPrefs;
	GLOBAL_PREFS_MASK *pMask;

	pPrefs = &pPreferenceInfo->prefs;
	pMask  = &pPreferenceInfo->prefsMask;

	pPrefs->run_on_batteries = (CheckComputingOnBatteries.GetCheck() == TRUE);
	pMask->run_on_batteries=true;

	pPrefs->run_if_user_active = (CheckComputingInUse.GetCheck() == TRUE);
	pMask->run_if_user_active=true;

	pPrefs->run_gpu_if_user_active = (CheckComputingInUseGpu.GetCheck() == TRUE);
	pMask->run_gpu_if_user_active=true;

	ValueComputingIdleMin.GetWindowText(sTxt);
	pPrefs->idle_time_to_run = numberFormat.atofLocale(sTxt);
//	pPrefs->idle_time_to_run = atof(sTxt);
	pMask->idle_time_to_run=true;

	ValueOtherSwitchEveryMin.GetWindowText(sTxt);
	pPrefs->cpu_scheduling_period_minutes = numberFormat.atofLocale(sTxt);
//	pPrefs->cpu_scheduling_period_minutes = atof(sTxt);
	pMask->cpu_scheduling_period_minutes=true;

	ValueOtherMultiprocPerc.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
//	td = atof(sTxt);
    clamp_pct(td);
	pPrefs->max_ncpus_pct = td;
	pMask->max_ncpus_pct=true;

	ValueOtherUseAtMost.GetWindowText(sTxt);
	pPrefs->cpu_usage_limit = numberFormat.atofLocale(sTxt);
//	pPrefs->cpu_usage_limit = atof(sTxt);
	pMask->cpu_usage_limit=true;

	ValueWhileProcessorUsageIsLessThan.GetWindowText(sTxt);
	pPrefs->suspend_cpu_usage = numberFormat.atofLocale(sTxt);
//	pPrefs->suspend_cpu_usage = atof(sTxt);
	pMask->suspend_cpu_usage=true;

	m_editTimeFrom.GetWindowText(sTxt);
	if (!timeStringDouble.TimeStringToDouble(sTxt,&pPrefs->cpu_times.start_hour))	{timeStringDouble.IllegalTime("Processor TimeFrom",sTxt); return false;}
	pMask->start_hour = true;
	m_editTimeTo.GetWindowText(sTxt);
	if (!timeStringDouble.TimeStringToDouble(sTxt,&pPrefs->cpu_times.end_hour))		{timeStringDouble.IllegalTime("Processor TimeTo",sTxt); return false;}
	pMask->end_hour = true;    

	pPrefs->cpu_times.week.clear();
	for(int i=0; i< 7;i++)
	{
		if(m_checkDay[i].GetCheck() == TRUE)
		{
			m_editDayOfWeek[i][0].GetWindowText(sTxt);
			if (!timeStringDouble.TimeStringToDouble(sTxt,&dStartTime))	{timeStringDouble.IllegalTime("Processor TimeFrom Day-of-week-override",sTxt); return false;}
			m_editDayOfWeek[i][1].GetWindowText(sTxt);
			if (!timeStringDouble.TimeStringToDouble(sTxt,&dEndTime))	{timeStringDouble.IllegalTime("Processor TimeTo Day-of-week-override",sTxt); return false;}
			pPrefs->cpu_times.week.set(i,dStartTime,dEndTime);
		}
	}

	return true;
}

void CDlgPrefProcessor::ShowGraphic(bool bEnable)
{
	CString sTxt;
	int iShow, iShowGraphic;

	TIME_SPAN cpu;


	if (bEnable)
	{
		iShow = SW_HIDE;
		iShowGraphic = SW_SHOW;
	}
	else
	{
		iShow = SW_SHOW;
		iShowGraphic = SW_HIDE;
	}

	for(int i=0; i< 7;i++)
	{
		m_checkDay[i].ShowWindow(iShow);
		m_editDayOfWeek[i][0].ShowWindow(iShow);
		m_editDayOfWeek[i][1].ShowWindow(iShow);
		m_divider1.ShowWindow(iShow);
		m_divider2.ShowWindow(iShow);
		m_divider3.ShowWindow(iShow);
		m_divider4.ShowWindow(iShow);
		m_divider5.ShowWindow(iShow);
		m_divider6.ShowWindow(iShow);
		m_divider7.ShowWindow(iShow);
		m_textHourMinutes.ShowWindow(iShow);

		if (bEnable)
		{

			if(m_checkDay[i].GetCheck() == TRUE)
			{
				m_editDayOfWeek[i][0].GetWindowText(sTxt);
				timeStringDouble.TimeStringToDouble(sTxt,&cpu.start_hour);
				m_editDayOfWeek[i][1].GetWindowText(sTxt);
				timeStringDouble.TimeStringToDouble(sTxt,&cpu.end_hour);
			}
			else
			{
				cpu.start_hour = 0;
				cpu.end_hour = 24;
			}
			m_window.TimeSpanToWindow(i,cpu);
		}
	}

	m_window.ShowWindow(iShowGraphic);
}

void CDlgPrefProcessor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_ON_BATTERIES, CheckComputingOnBatteries);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_IN_USE, CheckComputingInUse);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_IN_USE_GPU, CheckComputingInUseGpu);
	DDX_Control(pDX, IDC_VALUE_COMPUTING_IDLE_MIN, ValueComputingIdleMin);
	DDX_Control(pDX, IDC_VALUE_OTHER_SWITCH_EVERY_MIN, ValueOtherSwitchEveryMin);
	DDX_Control(pDX, IDC_VALUE_OTHER_MULTIPROC_PERC, ValueOtherMultiprocPerc);
	DDX_Control(pDX, IDC_VALUE_OTHER_USE_AT_MOST, ValueOtherUseAtMost);

	DDX_Control(pDX, IDC_CHECK_COMPUTING_SUNDAY,	m_checkDay[0]);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_MONDAY,	m_checkDay[1]);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_TUESDAY,	m_checkDay[2]);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_WEDNESDAY, m_checkDay[3]);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_THURSDAY,	m_checkDay[4]);
	DDX_Control(pDX, IDC_CHECK_COMPUTING_FRIDAY,	m_checkDay[5]);
	DDX_Control(pDX, IIDC_CHECK_COMPUTING_SATURDAY, m_checkDay[6]);


	DDX_Control(pDX, IDC_EDIT_COMPUTING_SUNDAY1,	m_editDayOfWeek[0][0]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_SUNDAY2,	m_editDayOfWeek[0][1]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_MONDAY1,	m_editDayOfWeek[1][0]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_MONDAY2,	m_editDayOfWeek[1][1]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_TUESDAY1,	m_editDayOfWeek[2][0]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_TUESDAY2,	m_editDayOfWeek[2][1]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_WEDNESDAY1, m_editDayOfWeek[3][0]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_WEDNESDAY2, m_editDayOfWeek[3][1]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_THURSDAY1,	m_editDayOfWeek[4][0]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_THURSDAY2,	m_editDayOfWeek[4][1]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_FRIDAY1,	m_editDayOfWeek[5][0]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_FRIDAY2,	m_editDayOfWeek[5][1]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_SATURDAY1,	m_editDayOfWeek[6][0]);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_SATURDAY2,	m_editDayOfWeek[6][1]);

	DDX_Control(pDX, IDC_EDIT_COMPUTING_TIME_FROM, m_editTimeFrom);
	DDX_Control(pDX, IDC_EDIT_COMPUTING_TIME_TO, m_editTimeTo);
	DDX_Control(pDX, IDCLEAR, m_buttonClear);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_TITLE_ALLOWED, m_textComputingAllowed);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_ALLOWED_ONLY_AFTER_IDLE, m_textOnlyAfterComputerBeenIdleFor);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_ALLOWED_MINUTES, m_textMinutes1);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_OTHER_MINUTES, m_textMinutes2);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_ALLOWED_EVERY_DAY, m_textEverydayBetweenTheHoursOf);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_ALLOWED_AND, m_textAnd);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_ALLOWED_NO_RESTRICTIONS, m_textNoRestrictionsIfEqual);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_TITLE_OVERRIDE, m_textDayOfWeekOverride);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_OVERRIDE_DAY_MIN, m_textHourMinutes);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_TITLE_OTHER_OPTIONS, m_textOtherOptions);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_OTHER_SWTICH_BEWEEN, m_textSwitchBetweenApplicationsEvery);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_OTHER_MULTI_USE, m_textOnMultiprocessorSystemsUseAtMost);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_OTHER_PERC_PROC, m_textPercOfTheProcessors);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_OTHER_USE_AT_MOST, m_textUseAtMost);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_OTHER_PERC_CPU, m_textPercCpuTime);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_WHILE_USAGE_IS_LESS, m_textWhileProcessorUsageIsLessThan);
	DDX_Control(pDX, IDC_TEXT_BOINC_PROCESSOR_USAGE_PERCENTAGE, m_textPerc0MeansNoRestrictions);
	DDX_Control(pDX, IDC_VALUE_COMPUTING_USAGE_LESS_THAN, ValueWhileProcessorUsageIsLessThan);
	DDX_Control(pDX, IDC_WND_PROCESSOR_SCHEDULER, m_window);
	DDX_Control(pDX, IDC_CHECK_GRAPHICAL, m_checkGraphic);
	DDX_Control(pDX, IDC_DIVIDER1, m_divider1);
	DDX_Control(pDX, IDC_DIVIDER2, m_divider2);
	DDX_Control(pDX, IDC_DIVIDER3, m_divider3);
	DDX_Control(pDX, IDC_DIVIDER4, m_divider4);
	DDX_Control(pDX, IDC_DIVIDER5, m_divider5);
	DDX_Control(pDX, IDC_DIVIDER6, m_divider6);
	DDX_Control(pDX, IDC_DIVIDER7, m_divider7);
}


BEGIN_MESSAGE_MAP(CDlgPrefProcessor, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CDlgPrefProcessor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPrefProcessor::OnBnClickedCancel)
	ON_BN_CLICKED(IDCLEAR, &CDlgPrefProcessor::OnBnClickedClear)
	ON_BN_CLICKED(IDC_CHECK_COMPUTING_MONDAY, &CDlgPrefProcessor::OnBnClickedCheckComputingMonday)
	ON_BN_CLICKED(IDC_CHECK_COMPUTING_TUESDAY, &CDlgPrefProcessor::OnBnClickedCheckComputingTuesday)
	ON_BN_CLICKED(IDC_CHECK_COMPUTING_WEDNESDAY, &CDlgPrefProcessor::OnBnClickedCheckComputingWednesday)
	ON_BN_CLICKED(IDC_CHECK_COMPUTING_THURSDAY, &CDlgPrefProcessor::OnBnClickedCheckComputingThursday)
	ON_BN_CLICKED(IDC_CHECK_COMPUTING_FRIDAY, &CDlgPrefProcessor::OnBnClickedCheckComputingFriday)
	ON_BN_CLICKED(IIDC_CHECK_COMPUTING_SATURDAY, &CDlgPrefProcessor::OnBnClickedCheckComputingSaturday)
	ON_BN_CLICKED(IDC_CHECK_COMPUTING_SUNDAY, &CDlgPrefProcessor::OnBnClickedCheckComputingSunday)
	ON_BN_CLICKED(IDC_CHECK_COMPUTING_IN_USE_GPU, &CDlgPrefProcessor::OnBnClickedCheckComputingInUseGpu)
	ON_BN_CLICKED(ID_ADVANCED_SCHEDULER, &CDlgPrefProcessor::OnBnClickedAdvancedScheduler)
	ON_BN_CLICKED(IDC_CHECK_GRAPHICAL, &CDlgPrefProcessor::OnBnClickedCheckGraphical)
	ON_BN_CLICKED(IDC_WND_PROCESSOR_SCHEDULER, &CDlgPrefProcessor::OnStnClickedWndProcessorScheduler)

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_BOINC_GRAPHIC, OnSetGraphic) 
END_MESSAGE_MAP()


// CDlgPrefProcessor message handlers

LRESULT CDlgPrefProcessor::OnSetGraphic(WPARAM parm1, LPARAM parm2)
{
	int iDay;
	CSchedulerTime *pSchedulerTime;

	iDay = (int) parm1;
	pSchedulerTime = (CSchedulerTime *) parm2;

	if (pSchedulerTime->m_dStart == pSchedulerTime->m_bStop)
	{
		m_checkDay[iDay].SetCheck(FALSE);
		m_editDayOfWeek[iDay][0].EnableWindow(FALSE);
		m_editDayOfWeek[iDay][1].EnableWindow(FALSE);
		m_editDayOfWeek[iDay][0].SetWindowText("");
		m_editDayOfWeek[iDay][1].SetWindowText("");
		return 0;
	}

	m_checkDay[iDay].SetCheck(TRUE);
	m_editDayOfWeek[iDay][0].EnableWindow(TRUE);
	m_editDayOfWeek[iDay][1].EnableWindow(TRUE);
	m_editDayOfWeek[iDay][0].SetWindowText(timeStringDouble.DoubleToTimeString(pSchedulerTime->m_dStart));
	m_editDayOfWeek[iDay][1].SetWindowText(timeStringDouble.DoubleToTimeString(pSchedulerTime->m_bStop));

	return 0;
}

void CDlgPrefProcessor::OnBnClickedOk()
{
	m_pDlgPrefMain->Ok();
}

void CDlgPrefProcessor::OnBnClickedCancel()
{
	m_pDlgPrefMain->Cancel();
}

void CDlgPrefProcessor::OnBnClickedClear()
{
	m_pDlgPrefMain->Clear();
}

void CDlgPrefProcessor::OnBnClickedCheckComputingInUseGpu()
{
	bool run_gpu_if_user_active;

	run_gpu_if_user_active = (CheckComputingInUseGpu.GetCheck() == TRUE);

	if (run_gpu_if_user_active)
	{
		ValueComputingIdleMin.EnableWindow(FALSE);
	}
	else
	{
		ValueComputingIdleMin.EnableWindow(TRUE);
	}

}


void CDlgPrefProcessor::OnBnClickedCheckComputingSunday()
{
	ShowHideDay(0);
}

void CDlgPrefProcessor::OnBnClickedCheckComputingMonday()
{
	ShowHideDay(1);
}

void CDlgPrefProcessor::OnBnClickedCheckComputingTuesday()
{
	ShowHideDay(2);
}

void CDlgPrefProcessor::OnBnClickedCheckComputingWednesday()
{
	ShowHideDay(3);
}

void CDlgPrefProcessor::OnBnClickedCheckComputingThursday()
{
	ShowHideDay(4);
}

void CDlgPrefProcessor::OnBnClickedCheckComputingFriday()
{
	ShowHideDay(5);
}

void CDlgPrefProcessor::OnBnClickedCheckComputingSaturday()
{
	ShowHideDay(6);
}

void CDlgPrefProcessor::ShowHideDay(int iDay)
{
	int iCheck;

	iCheck = m_checkDay[iDay].GetCheck();
	if (iCheck == TRUE)
	{
		m_editDayOfWeek[iDay][0].EnableWindow(TRUE);
		m_editDayOfWeek[iDay][1].EnableWindow(TRUE);
	}
	else
	{
		m_editDayOfWeek[iDay][0].EnableWindow(FALSE);
		m_editDayOfWeek[iDay][1].EnableWindow(FALSE);
	}
	m_editDayOfWeek[iDay][0].SetWindowText("");
	m_editDayOfWeek[iDay][1].SetWindowText("");
}

void CDlgPrefProcessor::OnBnClickedAdvancedScheduler()
{
//	if (m_pDlgScheduler == NULL)
//	{
//		m_pDlgScheduler = new CDlgScheduler;
//		m_pDlgScheduler->Create(IDD_DIALOG_ADVANCED_SCHEDULER);
//	}
	theApp.m_pDlgScheduler->ShowWindow(SW_SHOW);
}


void CDlgPrefProcessor::OnBnClickedCheckGraphical()
{
	CDlgPrefMain *pSheet;
	CDlgPrefNetwork *pPage;
	int iCheck;

	iCheck = m_checkGraphic.GetCheck();

	pSheet = (CDlgPrefMain*) CWnd::GetParent();
	pPage = (CDlgPrefNetwork *) pSheet->GetPage(1);
	pPage->m_checkGraphic.SetCheck(iCheck);

	theApp.WriteProfileInt(registrySectionSettings, registryBoincSettingsProcGraph, iCheck);

	ShowGraphic(iCheck == 1);
	pPage->ShowGraphic(iCheck == 1);
}


void CDlgPrefProcessor::OnStnClickedWndProcessorScheduler()
{
	// TODO: Add your control notification handler code here
}

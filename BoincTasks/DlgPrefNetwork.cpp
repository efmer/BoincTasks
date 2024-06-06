// DlgPrefNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgPrefNetwork.h"
#include "DlgPrefProcessor.h"
#include "DlgPrefMain.h"
#include "ThreadRpc.h"
#include "NumberFormat.h"
#include "Translation.h"

// CDlgPrefNetwork dialog

IMPLEMENT_DYNAMIC(CDlgPrefNetwork, CPropertyPage)

CDlgPrefNetwork::CDlgPrefNetwork()
	: CPropertyPage(CDlgPrefNetwork::IDD)
{
	m_window.Setup(48,30,7,true,false);
}

CDlgPrefNetwork::~CDlgPrefNetwork()
{
}

BOOL CDlgPrefNetwork::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogBoincSettingsTitleNetwork];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_textGeneralOptions.SetWindowText(gszTranslation[PosDialogBoincSettingsNetGeneralTitle]);
	m_textMaxDownloadRate.SetWindowText(gszTranslation[PosDialogBoincSettingsNetGeneralMaxDownload]);
	m_textMaxUploadRate.SetWindowText(gszTranslation[PosDialogBoincSettingsNGeneralMaxUpload]);
	
	m_textTransferAtMost.SetWindowText(gszTranslation[PosDialogBoincSettingsTransferAtMost]);
	m_textMBytesEvery.SetWindowText(gszTranslation[PosDialogBoincSettingsMBytesEvery]);
	m_textDaysEvery.SetWindowText(gszTranslation[PosDialogBoincSettingsTransferAtMostDays]);

	m_textConnectEvery.SetWindowText(gszTranslation[PosDialogBoincSettingsNGeneralConnectEvery]);

	m_textAdditionalWorkBuffer.SetWindowText(gszTranslation[PosDialogBoincSettingsNGeneralWorkBuffer]);
	m_textKbs1.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralKBsec]);
	m_textKbs2.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralKBsec]);
	m_textDays1.SetWindowText(gszTranslation[PosDialogBoincSettingsNGeneralConnectEveryDays]);
	m_textDaysMax.SetWindowText(gszTranslation[PosDialogBoincSettingsNetGeneralDaysMax]);
	m_textNetworkAdditionalBufferWarning.SetWindowText(gszTranslation[PosDialogBoincSettingsNetGeneralWURegulator]);
	m_checkSkipImageVerification.SetWindowText(gszTranslation[PosDialogBoincSettingsNetGeneralSkipImage]);;

	m_textConnectionOptions.SetWindowText(gszTranslation[PosDialogBoincSettingsNetConnectionTitle]);
	m_checkConfirmBeforeConnecting.SetWindowText(gszTranslation[PosDialogBoincSettingsNetConnectionConfirm]);
	m_checkDisconnectWhenDone.SetWindowText(gszTranslation[PosDialogBoincSettingsNetConnectionDWD]);

	m_textNetworkUsageAllowed.SetWindowText(gszTranslation[PosDialogBoincSettingsNetNetworkTitle]);
	m_textEveryDayBetween.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralDayBetween]);
	m_textAnd.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralAnd]);
	m_textNoRestrictionsIfEqual.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralNoRestrictions]);

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

	m_buttonClear.SetWindowText(gszTranslation[PosDialogCommonButtonsClear]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);

	int iCheck = theApp.GetProfileInt(registrySectionSettings, registryBoincSettingsProcGraph, 1);
	m_checkGraphic.SetCheck(iCheck);
	ShowGraphic(iCheck == 1);

	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CDlgPrefNetwork::FillDialog(CRpcPreferenceInfo *pPreferenceInfo)
{
	//C:\programma\BoincTasks\BoincTasks\boinc\boinc_source\boinc\clientgui\DlgAdvPreferences.cpp
	CString sFormat;
	CNumberFormat numberFormat;
	
	GLOBAL_PREFS *pPrefs;

	pPrefs = &pPreferenceInfo->prefs;

	bool bLocalhostAndRegulator = false;
	if (strcmp(LOCALHOST_NAME,pPreferenceInfo->m_pcHostName) == 0)
	{
		if (pPreferenceInfo->m_iWantWorkUnits >0)
		{
			bLocalhostAndRegulator = true;
			if (pPreferenceInfo->m_bBoincClientV6)
			{
				ValueNetworkAdditionalBuffer.EnableWindow(FALSE);
				ValueNetworkConnectEvery.EnableWindow(TRUE);
			}
			else
			{
				ValueNetworkConnectEvery.EnableWindow(FALSE);
				ValueNetworkAdditionalBuffer.EnableWindow(TRUE);
			}
			m_textNetworkAdditionalBufferWarning.ShowWindow(SW_SHOW);
		}
	}

	if (!bLocalhostAndRegulator)
	{
		m_textNetworkAdditionalBufferWarning.ShowWindow(SW_HIDE);
		ValueNetworkAdditionalBuffer.EnableWindow(TRUE);
		ValueNetworkConnectEvery.EnableWindow(TRUE);
	}

	sFormat = numberFormat.FormatNumberNT((pPrefs->max_bytes_sec_down / 1024),2);
//	sFormat.Format("%.2f",(pPrefs->max_bytes_sec_down / 1024));
	ValueNetworkDownloadRate.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT((pPrefs->max_bytes_sec_up / 1024),2);
//	sFormat.Format("%.2f",(pPrefs->max_bytes_sec_up / 1024));
	ValueNetworkUploadRate.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->daily_xfer_limit_mb,2);
//	sFormat.Format("%.2f",pPrefs->daily_xfer_limit_mb);
	ValueNetworkTransfer.SetWindowText(sFormat);

	sFormat.Format("%d",pPrefs->daily_xfer_period_days);
	ValueNetworkTransferDays.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->work_buf_min_days, 2);
//	sFormat.Format("%01.2f",pPrefs->work_buf_min_days);
	ValueNetworkConnectEvery.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->work_buf_additional_days, 2);
//	sFormat.Format("%.2f",pPrefs->work_buf_additional_days);
	ValueNetworkAdditionalBuffer.SetWindowText(sFormat);

	m_checkSkipImageVerification.SetCheck(pPrefs->dont_verify_images);
	m_checkConfirmBeforeConnecting.SetCheck(pPrefs->confirm_before_connecting);
	m_checkDisconnectWhenDone.SetCheck(pPrefs->hangup_if_dialed);

	// everday between the hours of
	m_editTimeFrom.SetWindowText(timeStringDouble.DoubleToTimeString(pPrefs->net_times.start_hour));
	m_editTimeTo.SetWindowText(timeStringDouble.DoubleToTimeString(pPrefs->net_times.end_hour));

	// day-of-week-override
	for(int i=0; i< 7;i++)
	{
        TIME_SPAN& net = pPrefs->net_times.week.days[i];
		if(net.present)
		{
			m_checkDay[i].SetCheck(TRUE);
			m_editDayOfWeek[i][0].EnableWindow(TRUE);
			m_editDayOfWeek[i][1].EnableWindow(TRUE);
			m_editDayOfWeek[i][0].SetWindowText(timeStringDouble.DoubleToTimeString(net.start_hour));
			m_editDayOfWeek[i][1].SetWindowText(timeStringDouble.DoubleToTimeString(net.end_hour));
		}
		else
		{
			m_checkDay[i].SetCheck(FALSE);
			m_editDayOfWeek[i][0].EnableWindow(FALSE);
			m_editDayOfWeek[i][1].EnableWindow(FALSE);
			m_editDayOfWeek[i][0].SetWindowText("");
			m_editDayOfWeek[i][1].SetWindowText("");
		}
		m_window.TimeSpanToWindow(i,net);
	}
}
extern double clamp_pct(double x);

bool CDlgPrefNetwork::ReadDialog(CRpcPreferenceInfo *pPreferenceInfo)
{
	CString sTxt;
	double	td, dStartTime, dEndTime;
	CNumberFormat numberFormat;

	GLOBAL_PREFS *pPrefs;
	GLOBAL_PREFS_MASK *pMask;

	pPrefs = &pPreferenceInfo->prefs;
	pMask  = &pPreferenceInfo->prefsMask;

	ValueNetworkDownloadRate.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
//	td = atof(sTxt);
	td = td * 1024;
	pPrefs->max_bytes_sec_down = td;
	pMask->max_bytes_sec_down=true;

	ValueNetworkUploadRate.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
//	td = atof(sTxt);
	td = td * 1024;
	pPrefs->max_bytes_sec_up = td;
	pMask->max_bytes_sec_up=true;

	ValueNetworkTransfer.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
//	td = atof(sTxt);
	pPrefs->daily_xfer_limit_mb = td;
	pMask->daily_xfer_limit_mb = true;

	ValueNetworkTransferDays.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
//	td = atof(sTxt);
	pPrefs->daily_xfer_period_days = (int) td;
	pMask->daily_xfer_period_days = true;

	ValueNetworkConnectEvery.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
	if (td > 20)
	{
		td = 20;
	}
	pPrefs->work_buf_min_days = td;
	pMask->work_buf_min_days=true;

	ValueNetworkAdditionalBuffer.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
	if (td > 20)
	{
		td = 20;
	}
	pPrefs->work_buf_additional_days = td;
	pMask->work_buf_additional_days = true;

	pPrefs->dont_verify_images = (m_checkSkipImageVerification.GetCheck() == TRUE);
	pMask->dont_verify_images=true;

	pPrefs->confirm_before_connecting = (m_checkConfirmBeforeConnecting.GetCheck() == TRUE);
	pMask->confirm_before_connecting=true;

	pPrefs->hangup_if_dialed = (m_checkDisconnectWhenDone.GetCheck() == TRUE);
	pMask->hangup_if_dialed=true;

	m_editTimeFrom.GetWindowText(sTxt);
	if (!timeStringDouble.TimeStringToDouble(sTxt,&pPrefs->net_times.start_hour))	{timeStringDouble.IllegalTime("Network TimeFrom",sTxt); return false;}
	pMask->net_start_hour = true;
	m_editTimeTo.GetWindowText(sTxt);
	if (!timeStringDouble.TimeStringToDouble(sTxt,&pPrefs->net_times.end_hour))		{timeStringDouble.IllegalTime("NetworkTimeTo",sTxt); return false;}
	pMask->net_end_hour = true;    

	pPrefs->net_times.week.clear();
	for(int i=0; i< 7;i++)
	{
		if(m_checkDay[i].GetCheck() == TRUE)
		{
			m_editDayOfWeek[i][0].GetWindowText(sTxt);
			if (!timeStringDouble.TimeStringToDouble(sTxt,&dStartTime))	{timeStringDouble.IllegalTime("Network TimeFrom Day-of-week-override",sTxt); return false;}
			m_editDayOfWeek[i][1].GetWindowText(sTxt);
			if (!timeStringDouble.TimeStringToDouble(sTxt,&dEndTime))	{timeStringDouble.IllegalTime("Network TimeTo Day-of-week-override",sTxt); return false;}
			pPrefs->net_times.week.set(i,dStartTime,dEndTime);
		}
	}
	return true;
}

void CDlgPrefNetwork::ShowGraphic(bool bEnable)
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

void CDlgPrefNetwork::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VALUE_NETWORK_DOWNLOAD_RATE, ValueNetworkDownloadRate);
	DDX_Control(pDX, IDC_VALUE_NETWORK_UPLOAD_RATE, ValueNetworkUploadRate);
	DDX_Control(pDX, IDC_VALUE_NETWORK_CONNECT_EVERY, ValueNetworkConnectEvery);
	DDX_Control(pDX, IDC_VALUE_NETWORK_ADDITIONAL_BUFFER, ValueNetworkAdditionalBuffer);
	DDX_Control(pDX, IDC_VALUE_NETWORK_ADDITIONAL_BUFFER_WARNING, m_textNetworkAdditionalBufferWarning);
	DDX_Control(pDX, IDC_CHECK_NETWORK_SKIP_VERIFICATION, m_checkSkipImageVerification);
	DDX_Control(pDX, IDC_CHECK_NETWORK_CONFIRM_CONNECTION, m_checkConfirmBeforeConnecting);
	DDX_Control(pDX, IDC_CHECK_NETWORK_DISCONNECT_DONE, m_checkDisconnectWhenDone);

	DDX_Control(pDX, IDC_CHECK_NETWORK_SUNDAY,		m_checkDay[0]);
	DDX_Control(pDX, IDC_CHECK_NETWORK_MONDAY,		m_checkDay[1]);
	DDX_Control(pDX, IDC_CHECK_NETWORK_TUESDAY,		m_checkDay[2]);
	DDX_Control(pDX, IDC_CHECK_NETWORK_WEDNESDAY,	m_checkDay[3]);
	DDX_Control(pDX, IDC_CHECK_NETWORK_THURSDAY,	m_checkDay[4]);
	DDX_Control(pDX, IDC_CHECK_NETWORK_FRIDAY,		m_checkDay[5]);
	DDX_Control(pDX, IIDC_CHECK_NETWORK_SATURDAY,	m_checkDay[6]);


	DDX_Control(pDX, IDC_EDIT_NETWORK_SUNDAY1,		m_editDayOfWeek[0][0]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_SUNDAY2,		m_editDayOfWeek[0][1]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_MONDAY1,		m_editDayOfWeek[1][0]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_MONDAY2,		m_editDayOfWeek[1][1]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_TUESDAY1,		m_editDayOfWeek[2][0]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_TUESDAY2,		m_editDayOfWeek[2][1]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_WEDNESDAY1,	m_editDayOfWeek[3][0]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_WEDNESDAY2,	m_editDayOfWeek[3][1]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_THURSDAY1,	m_editDayOfWeek[4][0]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_THURSDAY2,	m_editDayOfWeek[4][1]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_FRIDAY1,		m_editDayOfWeek[5][0]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_FRIDAY2,		m_editDayOfWeek[5][1]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_SATURDAY1,	m_editDayOfWeek[6][0]);
	DDX_Control(pDX, IDC_EDIT_NETWORK_SATURDAY2,	m_editDayOfWeek[6][1]);

	DDX_Control(pDX, IDC_EDIT_NETWORK_TIME_FROM, m_editTimeFrom);
	DDX_Control(pDX, IDC_EDIT_NETWORK_TIME_TO, m_editTimeTo);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_MAX_DOWNLOAD, m_textMaxDownloadRate);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_MAX_UPLOAD, m_textMaxUploadRate);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_CONNECT_EVERY, m_textConnectEvery);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_ADDITIONAL_WORK_BUFFER, m_textAdditionalWorkBuffer);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_TITLE, m_textGeneralOptions);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_KBS1, m_textKbs1);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_KBS2, m_textKbs2);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_DAYS1, m_textDays1);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_DAYS2, m_textDaysMax);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_CONNECTION_TITLE, m_textConnectionOptions);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_NETWORK_TITLE, m_textNetworkUsageAllowed);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_NETWORK_EVERY_DAY_BETWEEN, m_textEveryDayBetween);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_NETWORK_AND, m_textAnd);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_NETWORK_NO_RESTRICTIONS, m_textNoRestrictionsIfEqual);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_DOW_TITLE, m_textDayOfWeekOverride);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_DOW_HOUR_MIN, m_textHourMinutes);
	DDX_Control(pDX, IDCLEAR, m_buttonClear);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_TRANSFER_AT_MOST, m_textTransferAtMost);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_DAYS_EVERY, m_textDaysEvery);
	DDX_Control(pDX, IDC_VALUE_NETWORK_TRANSFER_AT_MOST, ValueNetworkTransfer);
	DDX_Control(pDX, IDC_TEXT_BOINC_NETWORK_GENERAL_MBS_EVERY, m_textMBytesEvery);
	DDX_Control(pDX, IDC_VALUE_NETWORK_TRANSFER_AT_MOST_EVERY, ValueNetworkTransferDays);
	DDX_Control(pDX, IDC_CHECK_GRAPHICAL, m_checkGraphic);
	DDX_Control(pDX, IDC_WND_PROCESSOR_SCHEDULER, m_window);
	DDX_Control(pDX, IDC_DIVIDER1, m_divider1);
	DDX_Control(pDX, IDC_DIVIDER2, m_divider2);
	DDX_Control(pDX, IDC_DIVIDER3, m_divider3);
	DDX_Control(pDX, IDC_DIVIDER4, m_divider4);
	DDX_Control(pDX, IDC_DIVIDER5, m_divider5);
	DDX_Control(pDX, IDC_DIVIDER6, m_divider6);
	DDX_Control(pDX, IDC_DIVIDER7, m_divider7);
}


BEGIN_MESSAGE_MAP(CDlgPrefNetwork, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CDlgPrefNetwork::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPrefNetwork::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_NETWORK_MONDAY, &CDlgPrefNetwork::OnBnClickedCheckNetworkMonday)
	ON_BN_CLICKED(IDC_CHECK_NETWORK_TUESDAY, &CDlgPrefNetwork::OnBnClickedCheckNetworkTuesday)
	ON_BN_CLICKED(IDC_CHECK_NETWORK_WEDNESDAY, &CDlgPrefNetwork::OnBnClickedCheckNetworkWednesday)
	ON_BN_CLICKED(IDC_CHECK_NETWORK_THURSDAY, &CDlgPrefNetwork::OnBnClickedCheckNetworkThursday)
	ON_BN_CLICKED(IDC_CHECK_NETWORK_FRIDAY, &CDlgPrefNetwork::OnBnClickedCheckNetworkFriday)
	ON_BN_CLICKED(IIDC_CHECK_NETWORK_SATURDAY, &CDlgPrefNetwork::OnBnClickedCheckNetworkSaturday)
	ON_BN_CLICKED(IDC_CHECK_NETWORK_SUNDAY, &CDlgPrefNetwork::OnBnClickedCheckNetworkSunday)
	ON_BN_CLICKED(IDCLEAR, &CDlgPrefNetwork::OnBnClickedClear)
	ON_BN_CLICKED(IDC_CHECK_GRAPHICAL, &CDlgPrefNetwork::OnBnClickedCheckGraphical)
END_MESSAGE_MAP()
void CDlgPrefNetwork::OnBnClickedOk()
{
	m_pDlgPrefMain->Ok();
}

void CDlgPrefNetwork::OnBnClickedCancel()
{
	m_pDlgPrefMain->Cancel();
}

void CDlgPrefNetwork::OnBnClickedCheckNetworkSunday()
{
	ShowHideDay(0);
}

void CDlgPrefNetwork::OnBnClickedCheckNetworkMonday()
{
	ShowHideDay(1);
}

void CDlgPrefNetwork::OnBnClickedCheckNetworkTuesday()
{
	ShowHideDay(2);
}

void CDlgPrefNetwork::OnBnClickedCheckNetworkWednesday()
{
	ShowHideDay(3);
}

void CDlgPrefNetwork::OnBnClickedCheckNetworkThursday()
{
	ShowHideDay(4);
}

void CDlgPrefNetwork::OnBnClickedCheckNetworkFriday()
{
	ShowHideDay(5);
}

void CDlgPrefNetwork::OnBnClickedCheckNetworkSaturday()
{
	ShowHideDay(6);
}

void CDlgPrefNetwork::ShowHideDay(int iDay)
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
void CDlgPrefNetwork::OnBnClickedClear()
{
	m_pDlgPrefMain->Clear();
}


void CDlgPrefNetwork::OnBnClickedCheckGraphical()
{
	CDlgPrefMain *pSheet;
	CDlgPrefProcessor *pPage;
	int iCheck;

	iCheck = m_checkGraphic.GetCheck();

	pSheet = (CDlgPrefMain*) CWnd::GetParent();
	pPage = (CDlgPrefProcessor *) pSheet->GetPage(0);
	pPage->m_checkGraphic.SetCheck(iCheck);

	theApp.WriteProfileInt(registrySectionSettings, registryBoincSettingsProcGraph, iCheck);

	ShowGraphic(iCheck == 1);
	pPage->ShowGraphic(iCheck == 1);
}

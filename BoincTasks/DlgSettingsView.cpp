#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsView.h"
#include "NumberFormat.h"
#include "MultiLanguage.h"
#include "Translation.h"

#include <locale>
// CDlgSettingsView dialog

IMPLEMENT_DYNAMIC(CDlgSettingsView, CPropertyPage)

CDlgSettingsView::CDlgSettingsView()
	: CPropertyPage(CDlgSettingsView::IDD)
{
	m_cComma = '.';
}

CDlgSettingsView::~CDlgSettingsView()
{
}

void CDlgSettingsView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIME_12_24, m_time);
	DDX_Control(pDX, IDC_COMBO_REFRESHRATE, m_refresh);
	DDX_Control(pDX, IDC_EDIT_REFRESHRATE_MANUAL, m_refreshManual);
	DDX_Control(pDX, IDC_STATIC_REFRESHRATE_MANUAL, m_refreshManualText);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_TEXT_VIEW_TIME_FORMAT, m_textTimeFormat);
	DDX_Control(pDX, IDC_TEXT_VIEW_REFRESH, m_textRefreshRate);
	DDX_Control(pDX, IDC_CHECK_HORIZONTAL_LINES, m_checkHorizontalGrid);
	DDX_Control(pDX, IDC_CHECK_VERTICAL_LINES, m_checkVerticalGrid);
	DDX_Control(pDX, IDC_CHECK_STRIPES, m_checkAlternatingStripes);
	DDX_Control(pDX, IDC_TEXT_VIEW_NUMBER_FORMAT, m_TextNumberFormat);
	DDX_Control(pDX, IDC_EDIT_NUMBER_FORMAT, m_editNumberFormat);
	DDX_Control(pDX, IDC_TEXT_VIEW_NUMBER_FORMAT_EXAMPLE, m_TextNumberFormatExample);
	DDX_Control(pDX, IDC_COMBO_TIME_FORMAT, m_timeDhms);
	DDX_Control(pDX, IDC_EDIT_TIME_USER_DEFINED, m_editTimeFormat);
	DDX_Control(pDX, IDC_TEXT_VIEW_NUMBER_FORMAT_TIME, m_TextNumberFormatTime);
	DDX_Control(pDX, IDC_CHECK_BOINC_TRANSLATABLE, m_checkTranslatable);
	DDX_Control(pDX, IDC_CHECK_SKIN, m_checkSkin);
	DDX_Control(pDX, IDC_COMBO_SKIN, m_skinNr);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDC_CHECK_PERC_RECT, m_checkPercRect);
	DDX_Control(pDX, IDC_CHECK_ADJUST_TIME, m_checkAdjustTime);
}

BOOL CDlgSettingsView::OnInitDialog()
{
	int iCheck;
	CString sDefault;

	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsViewTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_textTimeFormat.SetWindowText(gszTranslation[PosGroupDialogSettingsViewTimeFormat]);
	m_textRefreshRate.SetWindowText(gszTranslation[PosGroupDialogSettingsViewRefreshRate]);
	m_refreshManualText.SetWindowText(gszTranslation[PosGroupDialogSettingsViewSeconds]);

	m_time.AddString(gszTranslation[PosGroupDialogSettingsViewTimeFComboSystem]);
	m_time.AddString(gszTranslation[PosGroupDialogSettingsViewTimeFCombo24h]);
	m_time.AddString(gszTranslation[PosGroupDialogSettingsViewTimeFCombo12h]);
	m_time.AddString(gszTranslation[PosGroupDialogSettingsViewTimeFComboUser]);

	m_refresh.AddString(gszTranslation[PosGroupDialogSettingsViewRRateComboSlow]);
	m_refresh.AddString(gszTranslation[PosGroupDialogSettingsViewRRateComboNormal]);
	m_refresh.AddString(gszTranslation[PosGroupDialogSettingsViewRRateComboQuick]);
	m_refresh.AddString(gszTranslation[PosGroupDialogSettingsViewRRateComboManual]);

	m_checkSkin.SetWindowText(gszTranslation[PosGroupDialogSettingsViewSkin]);
	m_checkAlternatingStripes.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksAlternatingStripes]);
	m_checkHorizontalGrid.SetWindowText(gszTranslation[PosGroupDialogSettingsViewGridHorizontal]);
	m_checkVerticalGrid.SetWindowText(gszTranslation[PosGroupDialogSettingsViewGridVertical]);
	m_checkPercRect.SetWindowText(gszTranslation[PosGroupDialogSettingsViewPercRect]);
	m_checkAdjustTime.SetWindowText(gszTranslation[PosGroupDialogSettingsViewAdjustTime]);
	m_checkTranslatable.SetWindowText(gszTranslation[PosGroupDialogSettingsViewTranslatable]);

	m_TextNumberFormat.SetWindowText(gszTranslation[PosGroupDialogSettingsViewNumberFormat]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	int iNr;
	iNr = theApp.GetProfileInt(registrySectionSettings, registryTime,TIME_SYSTEM);
	m_time.SetCurSel(iNr);
	m_iTime = iNr;

	CString sTime, sTimeDummy;
	sTime = theApp.GetProfileString(registrySectionSettings, registryTimeFormat, "");
	if (CheckValidtime(sTime, &sTimeDummy))
	{
		m_editTimeFormat.SetWindowText(sTime);
		m_sTimeFormat = sTime;
	}
	CheckTimeFormatEnabled();

	iNr = theApp.GetProfileInt(registrySectionSettings, registryTimeDhms,TIME_DHMS);
	m_timeDhms.SetCurSel(iNr);
	m_iTimeDhms = iNr;

	iNr = theApp.GetProfileInt(registrySectionSettings, registryRefresh,REFRESH_NORMAL);
	m_refresh.SetCurSel(iNr);
	m_iRefresh = iNr;

	iNr = theApp.GetProfileInt(registrySectionSettings, registryRefreshManual,5);
	CString sTxt;
	sTxt.Format("%d",iNr);
	m_refreshManual.SetWindowText(sTxt);
	m_iRefreshManual = iNr;
	CheckRefreshManualEnabled();

	iCheck = theApp.GetProfileInt(registrySectionSettings, registrySkin, 1);
	m_checkSkin.SetCheck(iCheck);
	m_bUseSkin = (iCheck == TRUE);
	if (m_bUseSkin) m_skinNr.ShowWindow(SW_SHOW);
	else m_skinNr.ShowWindow(SW_HIDE);

	m_iUseSkin = theApp.GetProfileInt(registrySectionSettings, registrySkinNumber, 0);
	m_skinNr.SetCurSel(m_iUseSkin);
	m_iUseSkin++;

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryAlternatingStripes, 0);
	m_checkAlternatingStripes.SetCheck(iCheck);
	m_bAlternatingStriped = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryHorizontalGrid, 0);
	m_checkHorizontalGrid.SetCheck(iCheck);
	m_bUseHorizontalGrid = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryVerticalGrid, 0);
	m_checkVerticalGrid.SetCheck(iCheck);
	m_bUseVerticalGrid = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryPercentageRect, 0);
	m_checkPercRect.SetCheck(iCheck);
	m_bPercRect = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryAdjustTime, 1);
	m_checkAdjustTime.SetCheck(iCheck);
	m_bAdjustTime = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryBoincTranslatable, 1);
	if (theApp.m_pMultiLanguage->m_sLanguageId == "ENU")
	{
		iCheck = 0;
		m_checkTranslatable.EnableWindow(FALSE);
	}

	m_checkTranslatable.SetCheck(iCheck);
	m_bBoincTranslatable = (iCheck == TRUE);

	m_sSeperators = theApp.GetProfileString(registrySectionSettings,registryNumberSeperator, "");
	DecimalPointAndThousandSep(&sDefault);
	if (m_sSeperators.GetLength() == 0)
	{
		m_sSeperators = sDefault;
	}
	if (m_sSeperators.GetLength() > 1)
	{
		m_cThousend = m_sSeperators[1];
	}
	else
	{
		m_cThousend = 0;
	}
	m_cComma = m_sSeperators[0];

	if (m_sSeperators == sDefault) m_editNumberFormat.SetWindowText("");
	else m_editNumberFormat.SetWindowText(m_sSeperators);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

bool CDlgSettingsView::CheckValidtime(CString sTimeToCheck, CString *psTime)
{
	CStringA sValidate;
	int		iTimeCount;
	char	cChar,buffer[64];
	bool	bPercentage, bValid, bTimeValid;
	time_t	tSystemTime;
	struct	tm   tmtime;

	sValidate = "aAbBcdHIjmMpSUwWxXyYzZ";

	iTimeCount = sTimeToCheck.GetLength();

	bTimeValid = true;
	bPercentage = false;
	for (int iCount = 0; iCount < iTimeCount; iCount++)
	{
		cChar = sTimeToCheck[iCount];
		if (bPercentage)
		{
			bValid = false;
			for (int iCountValid = 0; iCountValid < sValidate.GetLength(); iCountValid++)
			{
				if (cChar == sValidate[iCountValid])
				{
					bValid = true;
					break;
				}
			}
			bPercentage = false;
			if (!bValid)
			{
				bTimeValid = false;
			}
		}

		if (cChar == '%')
		{
			bPercentage = true;
		}
	}

	if (bPercentage)
	{
		bTimeValid = false;
	}

	if (bTimeValid)
	{
		_time64( &tSystemTime);
		localtime_s(&tmtime, &tSystemTime);
		strftime(&buffer[0], 40, sTimeToCheck, &tmtime);
		*psTime = buffer;
	}
	else
	{
		*psTime = "??????";
	}

	return bTimeValid;

}

void CDlgSettingsView::CheckRefreshManualEnabled()
{
	if (m_iRefresh == REFRESH_MANUAL)
	{
		m_refreshManual.ShowWindow(SW_SHOW);
		m_refreshManualText.ShowWindow(SW_SHOW);
	}
	else
	{
		m_refreshManual.ShowWindow(SW_HIDE);
		m_refreshManualText.ShowWindow(SW_HIDE);
	}
}

void CDlgSettingsView::CheckTimeFormatEnabled()
{
	bool bShow;

	if (m_iTime == TIME_USER) bShow = true;
	else bShow = false;

	m_editTimeFormat.ShowWindow(bShow);
	m_TextNumberFormatTime.ShowWindow(bShow);
}

void  CDlgSettingsView::DecimalPointAndThousandSep(CString *psDefault)
{
	std::locale native("");
	const std::numpunct<char>& numericFacet = std::use_facet<std::numpunct<char> >(native);

	*psDefault += numericFacet.decimal_point();
	*psDefault += numericFacet.thousands_sep();
}

BEGIN_MESSAGE_MAP(CDlgSettingsView, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_12_24, &CDlgSettingsView::OnCbnSelchangeComboTime1224)
	ON_CBN_SELCHANGE(IDC_COMBO_REFRESHRATE, &CDlgSettingsView::OnCbnSelchangeComboRefreshrate)
	ON_EN_CHANGE(IDC_EDIT_REFRESHRATE_MANUAL, &CDlgSettingsView::OnEnChangeEditRefreshrateManual)
	ON_BN_CLICKED(IDOK, &CDlgSettingsView::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_HORIZONTAL_LINES, &CDlgSettingsView::OnBnClickedCheckHorizontalLines)
	ON_BN_CLICKED(IDC_CHECK_VERTICAL_LINES, &CDlgSettingsView::OnBnClickedCheckVerticalLines)
	ON_BN_CLICKED(IDC_CHECK_STRIPES, &CDlgSettingsView::OnBnClickedCheckStripes)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_FORMAT, &CDlgSettingsView::OnEnChangeEditNumberFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_FORMAT, &CDlgSettingsView::OnCbnSelchangeComboTimeFormat)
	ON_EN_CHANGE(IDC_EDIT_TIME_USER_DEFINED, &CDlgSettingsView::OnEnChangeEditTimeUserDefined)
	ON_BN_CLICKED(IDC_CHECK_BOINC_TRANSLATABLE, &CDlgSettingsView::OnBnClickedCheckBoincTranslatable)
	ON_BN_CLICKED(IDC_CHECK_SKIN, &CDlgSettingsView::OnBnClickedCheckSkin)
	ON_CBN_SELCHANGE(IDC_COMBO_SKIN, &CDlgSettingsView::OnCbnSelchangeComboSkin)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsView::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_CHECK_PERC_RECT, &CDlgSettingsView::OnBnClickedCheckPercRect)
	ON_BN_CLICKED(IDC_CHECK_ADJUST_TIME, &CDlgSettingsView::OnBnClickedCheckAdjustTime)
END_MESSAGE_MAP()


// CDlgSettingsView message handlers

void CDlgSettingsView::OnCbnSelchangeComboTime1224()
{
	int iNr;
	iNr = m_time.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryTime, iNr);
	m_iTime = iNr;

	CheckTimeFormatEnabled();
}

void CDlgSettingsView::OnCbnSelchangeComboTimeFormat()
{
	int iNr;
	iNr = m_timeDhms.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryTimeDhms, iNr);

	m_iTimeDhms = iNr;
}

void CDlgSettingsView::OnCbnSelchangeComboRefreshrate()
{
	int iNr;
	iNr = m_refresh.GetCurSel();

	m_iRefresh = iNr;

	CheckRefreshManualEnabled();

	theApp.WriteProfileInt(registrySectionSettings, registryRefresh, iNr);
}

void CDlgSettingsView::OnEnChangeEditRefreshrateManual()
{
	CString sTxt;
	int	iNr;

	m_refreshManual.GetWindowText(sTxt);

	iNr = atoi(sTxt);

	if (iNr <1) iNr = 1;

	m_iRefreshManual = iNr;
	theApp.WriteProfileInt(registrySectionSettings, registryRefreshManual, iNr);
}

void CDlgSettingsView::OnBnClickedOk()
{
	CString sTxt;
	int	iNr;

	m_refreshManual.GetWindowText(sTxt);
	iNr = atoi(sTxt);
	if (iNr <1) m_refreshManual.SetWindowTextA("1");		// show the lowest setting

	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsView::OnBnClickedCheckHorizontalLines()
{
	int iCheck;

	iCheck = m_checkHorizontalGrid.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryHorizontalGrid, iCheck);

	m_bUseHorizontalGrid = (iCheck == TRUE);
}

void CDlgSettingsView::OnBnClickedCheckVerticalLines()
{
	int iCheck;

	iCheck = m_checkVerticalGrid.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryVerticalGrid, iCheck);

	m_bUseVerticalGrid = (iCheck == TRUE);
}

void CDlgSettingsView::OnBnClickedCheckSkin()
{
	int iCheck;

	iCheck = m_checkSkin.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registrySkin, iCheck);

	m_bUseSkin = (iCheck == TRUE);

	if (m_bUseSkin) m_skinNr.ShowWindow(SW_SHOW);
	else m_skinNr.ShowWindow(SW_HIDE);
}

void CDlgSettingsView::OnCbnSelchangeComboSkin()
{
	m_iUseSkin = m_skinNr.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registrySkinNumber, m_iUseSkin);
	m_iUseSkin++;
}

void CDlgSettingsView::OnBnClickedCheckStripes()
{
	int iCheck;

	iCheck = m_checkAlternatingStripes.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryAlternatingStripes, iCheck);

	m_bAlternatingStriped = (iCheck == TRUE);
}

void CDlgSettingsView::OnBnClickedCheckPercRect()
{
	int iCheck;

	iCheck = m_checkPercRect.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryPercentageRect, iCheck);

	m_bPercRect = (iCheck == TRUE);
}

void CDlgSettingsView::OnBnClickedCheckAdjustTime()
{
	int iCheck;

	iCheck = m_checkAdjustTime.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryAdjustTime, iCheck);
	m_bAdjustTime = (iCheck == TRUE);
}

void CDlgSettingsView::OnBnClickedCheckBoincTranslatable()
{
	int iCheck;

	iCheck = m_checkTranslatable.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryBoincTranslatable, iCheck);

	m_bBoincTranslatable = (iCheck == TRUE);
}


void CDlgSettingsView::OnEnChangeEditNumberFormat()
{
	CNumberFormat numberFormat;	
	CString sNr, sDefault;

	m_editNumberFormat.GetWindowText(m_sSeperators);

	DecimalPointAndThousandSep(&sDefault);
	if (m_sSeperators.GetLength() == 0)
	{
		m_sSeperators = sDefault;
	}
	if (m_sSeperators.GetLength() > 1)
	{
		m_cThousend = m_sSeperators[1];
	}
	else
	{
		m_cThousend = 0;
	}
	m_cComma = m_sSeperators[0];

	if (m_sSeperators == sDefault) 	theApp.WriteProfileString(registrySectionSettings,registryNumberSeperator, "");
	else theApp.WriteProfileString(registrySectionSettings,registryNumberSeperator, m_sSeperators);

	sNr = numberFormat.FormatNumber(1000000.9, 1);
	m_TextNumberFormatExample.SetWindowText(sNr);

}

void CDlgSettingsView::OnEnChangeEditTimeUserDefined()
{
	CString sTime, sTimeFormatted;
	bool	bValid;

	m_editTimeFormat.GetWindowText(sTime);
	bValid = CheckValidtime(sTime, &sTimeFormatted);

	m_TextNumberFormatTime.SetWindowText(sTimeFormatted);

	if (bValid)
	{
		m_sTimeFormat = sTime;
		theApp.WriteProfileString(registrySectionSettings, registryTimeFormat, sTime);
	}
	else m_sTimeFormat = "?????" ;
}

BOOL CDlgSettingsView::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsView::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#view";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}


// DlgSettingsGadget.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgFloating.h"
#include "MainFrm.h"
#include "DlgSettingsGadget.h"
#include "DlgSettingsMain.h"
#include "Translation.h"

// CDlgSettingsGadget dialog

IMPLEMENT_DYNAMIC(CDlgSettingsGadget, CPropertyPage)

CDlgSettingsGadget::CDlgSettingsGadget()
	: CPropertyPage(CDlgSettingsGadget::IDD)
{
	this->m_strHeaderSubTitle = "test";
}

CDlgSettingsGadget::~CDlgSettingsGadget()
{
}

BOOL CDlgSettingsGadget::OnInitDialog()
{
	CString sFormat;

	CPropertySheet *pSheet;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsGadgetTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_radioShowAlwaysOnTop.SetWindowText(gszTranslation[PosGroupDialogSettingsGadgetRadioShowTop]);
	m_radioShowAlways.SetWindowText(gszTranslation[PosGroupDialogSettingsGadgetRadioShow]);
	m_radioShowOverIcon.SetWindowText(gszTranslation[PosGroupDialogSettingsGadgetRadioShowIcon]);
	m_textSecondsFor.SetWindowText(gszTranslation[PosGroupDialogSettingsGadgetRadioShowFor]);
	m_textSeconds1.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralSeconds]);
	m_textSeconds2.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralSeconds]);
	m_textRefreshEvery.SetWindowText(gszTranslation[PosGroupDialogSettingsGeneralRefresh]);

	m_listType.AddString(gszTranslation[PosGroupDialogSettingsGadgetModeTasks]);
	m_listType.AddString(gszTranslation[PosGroupDialogSettingsGadgetModeComputers]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	m_iRadio = theApp.GetProfileInt(registrySectionSettings, registryGadgetShow, RADIO_SHOW_ICON);
	
	switch(m_iRadio)
	{
		case RADIO_SHOW_ALWAYS_ON_TOP:
			m_radioShowAlwaysOnTop.SetCheck(TRUE);
			m_radioShowAlways.SetCheck(FALSE);
			m_radioShowOverIcon.SetCheck(FALSE);

			m_editTime.ShowWindow(SW_HIDE);
			m_editTimeAfter.ShowWindow(SW_HIDE);
			m_textSecondsFor.ShowWindow(SW_HIDE);
			m_textSeconds1.ShowWindow(SW_HIDE);
		break;
		case RADIO_SHOW_ALWAYS:
			m_radioShowAlwaysOnTop.SetCheck(FALSE);
			m_radioShowAlways.SetCheck(TRUE);
			m_radioShowOverIcon.SetCheck(FALSE);

			m_editTime.ShowWindow(SW_HIDE);
			m_editTimeAfter.ShowWindow(SW_HIDE);
			m_textSecondsFor.ShowWindow(SW_HIDE);
			m_textSeconds1.ShowWindow(SW_HIDE);
		break;
		default:
			m_radioShowAlwaysOnTop.SetCheck(FALSE);
			m_radioShowAlways.SetCheck(FALSE);
			m_radioShowOverIcon.SetCheck(TRUE);

			m_editTime.ShowWindow(SW_SHOW);
			m_editTimeAfter.ShowWindow(SW_SHOW);
			m_textSecondsFor.ShowWindow(SW_SHOW);
			m_textSeconds1.ShowWindow(SW_SHOW);
	}

	m_iTime = theApp.GetProfileInt(registrySectionSettings, registryGadgetTime, 1);
	sFormat.Format("%d", m_iTime);
	m_editTime.SetWindowText(sFormat);

	m_iTimeAfter = theApp.GetProfileInt(registrySectionSettings, registryGadgetTimeAfter, 2);
	sFormat.Format("%d", m_iTimeAfter);
	m_editTimeAfter.SetWindowText(sFormat);

	m_iRefreshTime = theApp.GetProfileInt(registrySectionSettings, registryGadgetRefreshTime, 10);
	sFormat.Format("%d", m_iRefreshTime);
	m_editRefreshEvery.SetWindowText(sFormat);


	m_iMode = theApp.GetProfileInt(registrySectionSettings, registryGadgetMode, 0);
	m_listType.SetCurSel(m_iMode);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsGadget::SendGadget()
{
	if (theApp.m_pDlgFloating != NULL)
	{
		theApp.m_pDlgFloating->SendMessage(UWM_MSG_SET_FLOATER,m_iRadio, m_iRefreshTime);
	}
	theApp.m_pMainFrame->SendMessage(UWM_MSG_SET_FLOATER,m_iRadio, m_iTime);
}

void CDlgSettingsGadget::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_GADGET_ON_TOP, m_radioShowAlwaysOnTop);
	DDX_Control(pDX, IDC_RADIO_GADGET_SHOW_ALWAYS, m_radioShowAlways);
	DDX_Control(pDX, IDC_RADIO_GADGET_ON_ICON, m_radioShowOverIcon);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_TEXT_GADGET_SECONDS1, m_textSeconds1);
	DDX_Control(pDX, IDC_TEXT_GADGET_SECONDS2, m_textSeconds2);
	DDX_Control(pDX, IDC_EDIT_ICON_TIME, m_editTime);
	DDX_Control(pDX, IDC_EDIT_REFRESH_TIME, m_editRefreshEvery);
	DDX_Control(pDX, IDC_TEXT_REFRESH_EVERY, m_textRefreshEvery);
	DDX_Control(pDX, IDC_EDIT_ICON_TIME_AFTER, m_editTimeAfter);
	DDX_Control(pDX, IDC_TEXT_GADGET_SECONDS3, m_textSecondsFor);
	DDX_Control(pDX, IDC_COMBO_GADGET_TYPE, m_listType);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(CDlgSettingsGadget, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_ICON_TIME, &CDlgSettingsGadget::OnEnChangeEditIconTime)
	ON_BN_CLICKED(IDC_RADIO_GADGET_ON_TOP, &CDlgSettingsGadget::OnBnClickedRadioGadgetOnTop)
	ON_BN_CLICKED(IDC_RADIO_GADGET_SHOW_ALWAYS, &CDlgSettingsGadget::OnBnClickedRadioGadgetShowAlways)
	ON_BN_CLICKED(IDC_RADIO_GADGET_ON_ICON, &CDlgSettingsGadget::OnBnClickedRadioGadgetOnIcon)
	ON_EN_CHANGE(IDC_EDIT_REFRESH_TIME, &CDlgSettingsGadget::OnEnChangeEditRefreshTime)
	ON_BN_CLICKED(IDOK, &CDlgSettingsGadget::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_ICON_TIME_AFTER, &CDlgSettingsGadget::OnEnChangeEditIconTimeAfter)
	ON_CBN_SELCHANGE(IDC_COMBO_GADGET_TYPE, &CDlgSettingsGadget::OnCbnSelchangeComboGadgetType)
	ON_MESSAGE(UWM_MSG_GET_FLOATER_DATA, OnGetFloaterData)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsGadget::OnBnClickedHelp)
END_MESSAGE_MAP()


// CDlgSettingsGadget message handlers


void CDlgSettingsGadget::OnEnChangeEditIconTimeAfter()
{
	CString sTxt;

	m_editTimeAfter.GetWindowText(sTxt);
	m_iTimeAfter = atoi(sTxt);
	theApp.WriteProfileInt(registrySectionSettings, registryGadgetTimeAfter, m_iTimeAfter);

	SendGadget();
}

void CDlgSettingsGadget::OnEnChangeEditIconTime()
{
	CString sTxt;

	m_editTime.GetWindowText(sTxt);
	m_iTime = atoi(sTxt);
	theApp.WriteProfileInt(registrySectionSettings, registryGadgetTime, m_iTime);

	SendGadget();
}

void CDlgSettingsGadget::OnBnClickedRadioGadgetOnTop()
{
	int iCheck;
	m_iRadio = RADIO_SHOW_ALWAYS_ON_TOP;

	iCheck = m_radioShowAlwaysOnTop.GetCheck();

	m_radioShowAlways.SetCheck(FALSE);
	m_radioShowOverIcon.SetCheck(FALSE);

	m_editTime.ShowWindow(SW_HIDE);
	m_editTimeAfter.ShowWindow(SW_HIDE);
	m_textSecondsFor.ShowWindow(SW_HIDE);
	m_textSeconds1.ShowWindow(SW_HIDE);

	theApp.WriteProfileInt(registrySectionSettings, registryGadgetShow, m_iRadio);
	SendGadget();
}

void CDlgSettingsGadget::OnBnClickedRadioGadgetShowAlways()
{
	int iCheck;
	m_iRadio = RADIO_SHOW_ALWAYS;

	iCheck = m_radioShowAlways.GetCheck();

	m_radioShowAlwaysOnTop.SetCheck(FALSE);
	m_radioShowOverIcon.SetCheck(FALSE);

	m_editTime.ShowWindow(SW_HIDE);
	m_editTimeAfter.ShowWindow(SW_HIDE);
	m_textSecondsFor.ShowWindow(SW_HIDE);
	m_textSeconds1.ShowWindow(SW_HIDE);
	theApp.WriteProfileInt(registrySectionSettings, registryGadgetShow, m_iRadio);
	SendGadget();
}

void CDlgSettingsGadget::OnBnClickedRadioGadgetOnIcon()
{
	int iCheck;
	m_iRadio = RADIO_SHOW_ICON;

	iCheck = m_radioShowOverIcon.GetCheck();

	m_radioShowAlways.SetCheck(FALSE);
	m_radioShowAlwaysOnTop.SetCheck(FALSE);

	m_editTime.ShowWindow(SW_SHOW);
	m_editTimeAfter.ShowWindow(SW_SHOW);
	m_textSecondsFor.ShowWindow(SW_SHOW);
	m_textSeconds1.ShowWindow(SW_SHOW);

	theApp.WriteProfileInt(registrySectionSettings, registryGadgetShow, m_iRadio);
	SendGadget();
}

void CDlgSettingsGadget::OnEnChangeEditRefreshTime()
{
	CString sTxt;

	m_editRefreshEvery.GetWindowText(sTxt);
	m_iRefreshTime = atoi(sTxt);
	theApp.WriteProfileInt(registrySectionSettings, registryGadgetRefreshTime, m_iRefreshTime);
	SendGadget();
}

void CDlgSettingsGadget::OnCbnSelchangeComboGadgetType()
{
	m_iMode = m_listType.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryGadgetMode, m_iMode);
	theApp.m_pDlgFloating->SendMessage(UWM_MSG_SET_FLOATER_MODE,m_iMode, 0);
}

LRESULT CDlgSettingsGadget::OnGetFloaterData(WPARAM wParam,LPARAM lParam)
{
	theApp.m_pDlgFloating->SendMessage(UWM_MSG_SET_FLOATER_MODE,m_iMode, 0);
	SendGadget();

	return 0;
}

void CDlgSettingsGadget::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

BOOL CDlgSettingsGadget::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsGadget::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#gadget";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

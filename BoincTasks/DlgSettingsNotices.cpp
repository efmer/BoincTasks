// DlgSettingsNotices.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
#include "RemoteCombinedDoc.h"
#include "DlgSettingsNotices.h"
#include "afxdialogex.h"
#include "DlgSettingsMain.h"
#include "Translation.h"

// CDlgSettingsNotices dialog

IMPLEMENT_DYNAMIC(CDlgSettingsNotices, CPropertyPage)

CDlgSettingsNotices::CDlgSettingsNotices()
	: CPropertyPage(CDlgSettingsNotices::IDD)
{

}

CDlgSettingsNotices::~CDlgSettingsNotices()
{
}

BOOL CDlgSettingsNotices::OnInitDialog()
{
	CPropertySheet *pSheet;
	CString sFormat;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsNoticesTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_checkHideNotices.SetWindowText(gszTranslation[PosGroupDialogSettingsNoticesHideAfter]);
	m_textDays.SetWindowText(gszTranslation[PosGroupDialogSettingsNoticesDays]);
	m_textCheckEvery.SetWindowText(gszTranslation[PosGroupDialogSettingsNoticesCheckEvery]);
	m_CheckAlert.SetWindowText(gszTranslation[PosGroupDialogSettingsNoticesCheckAlert]);
	m_textMinutes1.SetWindowText(gszTranslation[PosGroupDialogSettingsNoticesMinutes]);
	m_textMinutes2.SetWindowText(gszTranslation[PosGroupDialogSettingsNoticesMinutes]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_textSorting.SetWindowText(gszTranslation[PosGroupDialogSettingsNoticesSort]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	m_iHideNoticeAfter = theApp.GetProfileInt(registrySectionSettings, registryNoticesHideAfter, NOTICES_HIDE_AFTER_DAYS);
	sFormat.Format("%d", m_iHideNoticeAfter);
	m_editHideAfter.SetWindowText(sFormat);
	if (m_iHideNoticeAfter <= 0)
	{
		m_checkHideNotices.SetCheck(FALSE);
		SetNoticesAfter(FALSE);
	}
	else m_checkHideNotices.SetCheck(TRUE);

	m_iAlertEvery = theApp.GetProfileInt(registrySectionSettings, registryNoticesAlertEvery, NOTICES_SHOW_ALERT_EVERY);
	sFormat.Format("%d", m_iAlertEvery);
	m_editAlert.SetWindowText(sFormat);
	if (m_iAlertEvery <= 0)
	{
		m_CheckAlert.SetCheck(FALSE);
		SetNoticesAlert(FALSE);
	}
	else m_CheckAlert.SetCheck(TRUE);

	m_iCheckEvery = theApp.GetProfileInt(registrySectionSettings, registryNoticesUpdateEvery, NOTICES_CHECK_EVERY);
	sFormat.Format("%d", m_iCheckEvery);
	m_editCheckEvery.SetWindowText(sFormat);

	m_listNoticesSort.m_bColor = false;
	m_listNoticesSort.MessagesTo(this->m_hWnd);

	DWORD dwSorting[NOTICES_SORT_COUNT];
	bool bSorting[NOTICES_SORT_COUNT];

	dwSorting[0] = theApp.GetProfileInt(registrySectionSettings, registryNoticesSort0, -1);
	dwSorting[1] = theApp.GetProfileInt(registrySectionSettings, registryNoticesSort1, -1);
	dwSorting[2] = theApp.GetProfileInt(registrySectionSettings, registryNoticesSort2, -1);
	dwSorting[3] = theApp.GetProfileInt(registrySectionSettings, registryNoticesSort3, -1);
	dwSorting[4] = theApp.GetProfileInt(registrySectionSettings, registryNoticesSort4, -1);
	dwSorting[5] = theApp.GetProfileInt(registrySectionSettings, registryNoticesSort5, -1);

	for (int iCount = 0; iCount < NOTICES_SORT_COUNT; iCount++)
	{
		m_sorting[iCount].m_dwSortOn = dwSorting[iCount] >> 4;
		m_sorting[iCount].m_dwSortOperator = dwSorting[iCount] & 0xf;
		bSorting[iCount] = false;
	}


	// check if valid

	bool bValidSortingInfo = true;
	for (int iCount = 0; iCount < NOTICES_SORT_COUNT; iCount++)
	{
		if (m_sorting[iCount].m_dwSortOperator > NOTICES_SORT_OPERATOR_MAX)
		{
			bValidSortingInfo = false;
			break;
		}
		if (m_sorting[iCount].m_dwSortOn > NOTICES_SORT_MAX)
		{
			bValidSortingInfo = false;
			break;
		}
		bSorting[m_sorting[iCount].m_dwSortOn] = true;
	}

	bValidSortingInfo = true;
	for (int iCount = 0; iCount < NOTICES_SORT_COUNT; iCount++)
	{
		if (!bSorting[iCount]) bValidSortingInfo = false;
	}

	if (!bValidSortingInfo)	// not valid use deault
	{
		SortingSetDefault();
	}

	for (int iCount = 0; iCount < NOTICES_SORT_COUNT; iCount++)
	{
		if (m_sorting[iCount].m_dwSortOn == NOTICES_SORT_CATEGORY) m_listNoticesSort.AddString(gszTranslation[PosGroupDialogSettingsNoticesSortCategory], m_sorting[iCount].m_dwSortOperator);
		else
		{
			if (m_sorting[iCount].m_dwSortOn == NOTICES_SORT_PROJECT) m_listNoticesSort.AddString(gszTranslation[PosGroupDialogSettingsNoticesSortProject], m_sorting[iCount].m_dwSortOperator);
			else
			{
				if (m_sorting[iCount].m_dwSortOn == NOTICES_SORT_CREATION) m_listNoticesSort.AddString(gszTranslation[PosGroupDialogSettingsNoticesSortCreation], m_sorting[iCount].m_dwSortOperator);
				else
				{
					if (m_sorting[iCount].m_dwSortOn == NOTICES_SORT_ARRIVAL) m_listNoticesSort.AddString(gszTranslation[PosGroupDialogSettingsNoticesSortArrival], m_sorting[iCount].m_dwSortOperator);
				}
			}
		}
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsNotices::SetNoticesAfter(int iCheck)
{
	CString sFormat;
	int		iShow;

	if (iCheck)
	{
		m_iHideNoticeAfter = NOTICES_HIDE_AFTER_DAYS;
		sFormat.Format("%d", m_iHideNoticeAfter);
		m_editHideAfter.SetWindowText(sFormat);
		iShow = SW_SHOW;
	}
	else
	{
		iShow = SW_HIDE;
		m_iHideNoticeAfter = -1;
	}
	m_editHideAfter.ShowWindow(iShow);
	m_textDays.ShowWindow(iShow);

	theApp.WriteProfileInt(registrySectionSettings, registryNoticesHideAfter, m_iHideNoticeAfter);
}

void CDlgSettingsNotices::SetNoticesAlert(int iCheck)
{
	CString sFormat;
	int		iShow;

	if (iCheck)
	{
		m_iAlertEvery = NOTICES_SHOW_ALERT_EVERY;
		sFormat.Format("%d", m_iAlertEvery);
		m_editAlert.SetWindowText(sFormat);
		iShow = SW_SHOW;
	}
	else
	{
		iShow = SW_HIDE;
		m_iAlertEvery = -1;
	}
	m_editAlert.ShowWindow(iShow);
	m_textMinutes1.ShowWindow(iShow);

	theApp.WriteProfileInt(registrySectionSettings, registryNoticesAlertEvery, m_iAlertEvery);
}

void CDlgSettingsNotices::SortingSetDefault()
{
	m_sorting[0].m_dwSortOn			= NOTICES_SORT_CATEGORY;
	m_sorting[0].m_dwSortOperator	= NOTICES_SORT_OPERATOR_UP;
	m_sorting[1].m_dwSortOn			= NOTICES_SORT_PROJECT;
	m_sorting[1].m_dwSortOperator	= NOTICES_SORT_OPERATOR_UP;
	m_sorting[2].m_dwSortOn			= NOTICES_SORT_CREATION;
	m_sorting[2].m_dwSortOperator	= NOTICES_SORT_OPERATOR_DOWN;
	m_sorting[3].m_dwSortOn			= NOTICES_SORT_ARRIVAL;
	m_sorting[3].m_dwSortOperator	= NOTICES_SORT_OPERATOR_NO;
	m_sorting[4].m_dwSortOn			= NOTICES_SORT_NOTUSED1;
	m_sorting[4].m_dwSortOperator	= NOTICES_SORT_OPERATOR_NO;
	m_sorting[5].m_dwSortOn			= NOTICES_SORT_NOTUSED2;
	m_sorting[5].m_dwSortOperator	= NOTICES_SORT_OPERATOR_NO;

//	SortingWriteRegistry();
}

void CDlgSettingsNotices::SortingUpdate()
{
//	DWORD	dwSorting[NOTICES_SORT_COUNT+1];
	int		iItems;
	DWORD	dwItem;
	char	cBuffer[128];

	iItems = m_listNoticesSort.GetCount();

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (m_listNoticesSort.GetText(iCount, cBuffer) == LB_ERR) {cBuffer[0] = 0;};

		if (strcmp(cBuffer, gszTranslation[PosGroupDialogSettingsNoticesSortCategory]) == 0) 				m_sorting[iCount].m_dwSortOn = NOTICES_SORT_CATEGORY;
		else
		{
			if (strcmp(cBuffer, gszTranslation[PosGroupDialogSettingsNoticesSortProject]) == 0) 			m_sorting[iCount].m_dwSortOn = NOTICES_SORT_PROJECT;
			else
			{
				if (strcmp(cBuffer, gszTranslation[PosGroupDialogSettingsNoticesSortCreation]) == 0)		m_sorting[iCount].m_dwSortOn = NOTICES_SORT_CREATION;
				else
				{
					if (strcmp(cBuffer, gszTranslation[PosGroupDialogSettingsNoticesSortArrival]) == 0) 	m_sorting[iCount].m_dwSortOn = NOTICES_SORT_ARRIVAL;
					else
					{
						SortingSetDefault();		// something is wrong, set to default.
						break;
					}
				}
			}
		}
		dwItem = (DWORD) m_listNoticesSort.GetItemData(iCount);
		if (dwItem > NOTICES_SORT_MAX) dwItem = NOTICES_SORT_OPERATOR_NO;
		m_sorting[iCount].m_dwSortOperator = dwItem;
	}
	SortingWriteRegistry();

	CRemoteCombinedDoc	*pDoc;
	POSITION	posDoc;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			pDoc->m_pDlgNotices->SendMessage(UWM_MSG_NOTICES_REFRESH,0, 0);
		}
	}
}

void CDlgSettingsNotices::SortingWriteRegistry()
{
	DWORD dwSort;

	dwSort = m_sorting[0].m_dwSortOn << 4;
	dwSort+= m_sorting[0].m_dwSortOperator;
	theApp.WriteProfileInt(registrySectionSettings, registryNoticesSort0, dwSort);

	dwSort = m_sorting[1].m_dwSortOn << 4;
	dwSort+= m_sorting[1].m_dwSortOperator;
	theApp.WriteProfileInt(registrySectionSettings, registryNoticesSort1, dwSort);

	dwSort = m_sorting[2].m_dwSortOn << 4;
	dwSort+= m_sorting[2].m_dwSortOperator;
	theApp.WriteProfileInt(registrySectionSettings, registryNoticesSort2, dwSort);

	dwSort = m_sorting[3].m_dwSortOn << 4;
	dwSort+= m_sorting[3].m_dwSortOperator;
	theApp.WriteProfileInt(registrySectionSettings, registryNoticesSort3, dwSort);

	dwSort = m_sorting[4].m_dwSortOn << 4;
	dwSort+= m_sorting[4].m_dwSortOperator;
	theApp.WriteProfileInt(registrySectionSettings, registryNoticesSort4, dwSort);

	dwSort = m_sorting[5].m_dwSortOn << 4;
	dwSort+= m_sorting[5].m_dwSortOperator;
	theApp.WriteProfileInt(registrySectionSettings, registryNoticesSort5, dwSort);
}

void CDlgSettingsNotices::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_CHECK_NOTICES_SHOW_AFTER_DAYS, m_checkHideNotices);
	DDX_Control(pDX, IDC_TEXT_NOTICES_DAYS, m_textDays);
	DDX_Control(pDX, IDC_TEXT_NOTICES_CHECK, m_textCheckEvery);
	DDX_Control(pDX, IDC_CHECK_NOTICES_SHOW_ALERT, m_CheckAlert);
	DDX_Control(pDX, IDC_EDIT_NOTICES_SHOW_AFTER, m_editHideAfter);
	DDX_Control(pDX, IDC_EDIT_NOTICES_ALERT, m_editAlert);
	DDX_Control(pDX, IDC_EDIT_NOTICES_CHECK, m_editCheckEvery);
	DDX_Control(pDX, IDC_TEXT_NOTICES_MINUTES1, m_textMinutes1);
	DDX_Control(pDX, IDC_TEXT_NOTICES_MINUTES2, m_textMinutes2);
	DDX_Control(pDX, IDC_LIST_NOTICES_SORTING, m_listNoticesSort);
	DDX_Control(pDX, IDC_BUTTON_NOTICES_UP, m_buttonUp);
	DDX_Control(pDX, IDC_BUTTON_NOTICES_DOWN, m_buttonDown);
	DDX_Control(pDX, IDC_TEST_NOTICES_SORTING, m_textSorting);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(CDlgSettingsNotices, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_NOTICES_SHOW_AFTER_DAYS, &CDlgSettingsNotices::OnBnClickedCheckNoticesShowAfterDays)
	ON_BN_CLICKED(IDC_CHECK_NOTICES_SHOW_ALERT, &CDlgSettingsNotices::OnBnClickedCheckNoticesShowAlert)
	ON_EN_CHANGE(IDC_EDIT_NOTICES_SHOW_AFTER, &CDlgSettingsNotices::OnEnChangeEditNoticesShowAfter)
	ON_EN_CHANGE(IDC_EDIT_NOTICES_ALERT, &CDlgSettingsNotices::OnEnChangeEditNoticesAlert)
	ON_EN_CHANGE(IDC_EDIT_NOTICES_CHECK, &CDlgSettingsNotices::OnEnChangeEditNoticesCheck)
	ON_BN_CLICKED(IDOK, &CDlgSettingsNotices::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_NOTICES_UP, &CDlgSettingsNotices::OnBnClickedButtonNoticesUp)
//	ON_BN_CLICKED(IDC_BUTTON_NOTICES_DOWN, &CDlgSettingsNotices::OnBnClickedButtonNoticesDown)

	ON_MESSAGE(UWM_MSG_SELECTION_HAS_CHANGED, OnChangedSelection)
	ON_MESSAGE(UWM_MSG_NOTICES_SORT, OnNoticesSort)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsNotices::OnBnClickedHelp)
END_MESSAGE_MAP()


// CDlgSettingsNotices message handlers


void CDlgSettingsNotices::OnBnClickedCheckNoticesShowAfterDays()
{
	int	iCheck;

	iCheck = m_checkHideNotices.GetCheck();
	SetNoticesAfter(iCheck);
}

void CDlgSettingsNotices::OnEnChangeEditNoticesShowAfter()
{
	CString sTxt;
	int		iCheck, iShow;

	m_editHideAfter.GetWindowText(sTxt);
	m_iHideNoticeAfter = atoi(sTxt);

	if (m_iHideNoticeAfter == 0)	iCheck = FALSE;
	else							iCheck = TRUE;

	m_checkHideNotices.SetCheck(iCheck);
	if (iCheck) iShow = SW_SHOW;
	else		iShow = SW_HIDE;
//	m_editHideAfter.ShowWindow(iShow);
//	m_textDays.ShowWindow(iShow);

	theApp.WriteProfileInt(registrySectionSettings, registryNoticesHideAfter, m_iHideNoticeAfter);
}

void CDlgSettingsNotices::OnBnClickedCheckNoticesShowAlert()
{
	int	iCheck;

	iCheck = m_CheckAlert.GetCheck();
	SetNoticesAlert(iCheck);
}

void CDlgSettingsNotices::OnEnChangeEditNoticesAlert()
{
	CString sTxt;
	int		iCheck, iShow;

	m_editAlert.GetWindowText(sTxt);
	m_iAlertEvery = atoi(sTxt);

	if (m_iAlertEvery == 0)	iCheck = FALSE;
	else					iCheck = TRUE;

	m_CheckAlert.SetCheck(iCheck);
	if (iCheck) iShow = SW_SHOW;
	else		iShow = SW_HIDE;
//	m_editAlert.ShowWindow(iShow);
//	m_textMinutes1.ShowWindow(iShow);

	theApp.WriteProfileInt(registrySectionSettings, registryNoticesAlertEvery, m_iAlertEvery);
}


void CDlgSettingsNotices::OnEnChangeEditNoticesCheck()
{
	CString sTxt;

	m_editCheckEvery.GetWindowText(sTxt);
	m_iCheckEvery = atoi(sTxt);

	theApp.WriteProfileInt(registrySectionSettings, registryNoticesUpdateEvery, m_iCheckEvery);
}


void CDlgSettingsNotices::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}


LRESULT CDlgSettingsNotices::OnChangedSelection(WPARAM parm1, LPARAM parm2)
{
	m_buttonUp.ShowWindow(SW_SHOW);
	SortingUpdate();
	return 0;
}

//void CDlgSettingsNotices::OnLbnSelchangeListNoticesSorting()
//{
//	m_buttonUp.ShowWindow(SW_SHOW);
//	m_buttonDown.ShowWindow(SW_SHOW);
//}


void CDlgSettingsNotices::OnBnClickedButtonNoticesUp()
{
	int iItems, iSelected;
	DWORD dwItemData;
	CString sItemTxt;

	iItems = m_listNoticesSort.GetCount();
	iSelected = m_listNoticesSort.GetCurSel();

	if (iSelected > 0)
	{
		m_listNoticesSort.GetText(iSelected, sItemTxt);
		dwItemData = (DWORD) m_listNoticesSort.GetItemData(iSelected);
		m_listNoticesSort.DeleteString(iSelected);
		iSelected--;
		m_listNoticesSort.AddString(iSelected, sItemTxt, dwItemData);
		m_listNoticesSort.SetCurSel(iSelected);
	}

	SortingUpdate();
}


LRESULT CDlgSettingsNotices::OnNoticesSort(WPARAM parm1, LPARAM parm2)
{
	CNoticeSorting *pSorting;

	pSorting = (CNoticeSorting *) parm2;

	for (int iCount = 0; iCount < NOTICES_SORT_COUNT; iCount++)
	{
		pSorting->m_dwSortOn = m_sorting[iCount].m_dwSortOn;
		pSorting->m_dwSortOperator = m_sorting[iCount].m_dwSortOperator;
		pSorting++;
	}
	return 0;
}

BOOL CDlgSettingsNotices::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsNotices::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#notices";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

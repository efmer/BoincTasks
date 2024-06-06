// DlgSettingsHistory.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"	
#include "TemplateRemoteHistoryView.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsHistory.h"
#include "DlgRulesLogging.h"
#include "DlgLogging.h"
#include "ThreadRpc.h"
#include "RemoteCombinedDoc.h"
#include "Translation.h"

// CDlgSettingsHistory dialog

IMPLEMENT_DYNAMIC(CDlgSettingsHistory, CPropertyPage)

CDlgSettingsHistory::CDlgSettingsHistory()
	: CPropertyPage(CDlgSettingsHistory::IDD)
{
	m_bBackupHistory = false;
}

CDlgSettingsHistory::~CDlgSettingsHistory()
{
}

BOOL CDlgSettingsHistory::OnInitDialog()
{
	CPropertySheet *pSheet;
	int iCheck;
	int iSetting, iNr;
	CString sFormat;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	iNr = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsHistoryTitle];
    pTabCtrl->SetItem(iNr,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_apply.SetWindowText(gszTranslation[PosDialogCommonButtonsApply]);

	m_textSequence.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryGroupName]);
	m_checkShow[COLUMN_HISTORY_PROJECT].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryProject]);
	m_checkShow[COLUMN_HISTORY_APPLICATION].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryApplication]);
	m_checkShow[COLUMN_HISTORY_NAME].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryName]);
	m_checkShow[COLUMN_HISTORY_ELAPSED].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryElapsedTim]);
	m_checkShow[COLUMN_HISTORY_COMPLETED].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryCompleted]);
	m_checkShow[COLUMN_HISTORY_REPORTED].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryReported]);
	m_checkShow[COLUMN_HISTORY_USE].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryUse]);
	m_checkShow[COLUMN_HISTORY_RATIO].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryCpuPerc]);
	m_checkShow[COLUMN_HISTORY_STATUS].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryStatus]);
	m_checkShow[COLUMN_HISTORY_COMPUTER].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryComputer]);
	m_checkShow[COLUMN_HISTORY_VIRTUAL].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryVirtual]);
	m_checkShow[COLUMN_HISTORY_MEMORY].SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMemory]);

	m_enableHistoryLogging.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryEnable]);
	m_enableHistorySmartMode.SetWindowText(gszTranslation[PosGroupDialogSettingsHistorySmart]);
	m_checkAccurate.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryAccurate]);

	m_textRemoveAfter.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryRemove]);
	m_textRemoveDays.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryRemoveDays]);
	m_textRemoveHours.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryRemoveHours]);
	m_textMoveHours.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryRemoveHours]);
	m_textMoveDays.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryRemoveDays]);

	m_textMaxUpdateTime.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMaxUpdate]);
	m_textMaxUpdateTimeSeconds.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMaxUpdateSec]);
	m_textMinUpdateTime.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMinUpdate]);
	m_textMinUpdateTimeSeconds.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMaxUpdateSec]);
	m_checkLong.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMoveLong]);
	m_checkBackup.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryBackup]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	ReadColumsFromRegistry();
	m_apply.EnableWindow(FALSE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryHistoryLoggingEnable, 1);
	m_enableHistoryLogging.SetCheck(iCheck);
	m_bHistoryEnabled = (iCheck > 0);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryHistoryLoggingSmartMode, 1);
	m_enableHistorySmartMode.SetCheck(iCheck);
	m_bHistorySmartMode = (iCheck > 0);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryHistoryLoggingAccurate, 0);
	m_checkAccurate.SetCheck(iCheck);
	m_bHistoryLessAccurate = (iCheck > 0);

	m_iHistoryMaxUpdateTime = theApp.GetProfileInt(registrySectionSettings, registryHistoryMaxUpdateTime, UPDATE_HISTORY_TRIGGER_MAX_DEFAULT);
	if (m_iHistoryMaxUpdateTime < UPDATE_HISTORY_TRIGGER_MIN) m_iHistoryMaxUpdateTime = UPDATE_HISTORY_TRIGGER_MIN;
	sFormat.Format("%d",m_iHistoryMaxUpdateTime);
	m_editMaxUpdateTime.SetWindowText(sFormat);
	m_iHistoryMinUpdateTime = theApp.GetProfileInt(registrySectionSettings, registryHistoryMinUpdateTime, UPDATE_HISTORY_TRIGGER_MIN_DEFAULT);
	if (m_iHistoryMinUpdateTime < UPDATE_HISTORY_TRIGGER_MIN) m_iHistoryMinUpdateTime = UPDATE_HISTORY_TRIGGER_MIN;
	sFormat.Format("%d",m_iHistoryMinUpdateTime);
	m_editMinUpdateTime.SetWindowText(sFormat);

	// days
	iSetting = theApp.GetProfileInt(registrySectionSettings, registryHistoryRemoveDays, 14);
	if (iSetting > 33) iSetting = 33;
	if (iSetting < 0) iSetting = 0;
	m_historyDays.SetCurSel(iSetting);
	m_iHistorySettingDays = iSetting;

	iNr = iSetting;
	switch (iSetting)
	{
		case 31:
			iNr = 60;
		break;
		case 32:
			iNr = 120;
		break;
		case 33:
			iNr = 365;
		break;
	}

	if (iNr > 365) iNr = 365;
	if (iNr < 0) iNr = 0;

	m_iHistoryTime = iNr*24;
	m_iHistoryTime*= 60;			// minutes
	// hours

	iSetting = theApp.GetProfileInt(registrySectionSettings, registryHistoryRemoveHours, 0);
	m_historyHours.SetCurSel(iSetting);
	m_iHistorySettingHours = iSetting;

	if (iSetting > 23) iSetting = 23;
	if (iSetting < 0) iSetting = 0;

	m_iHistoryTime+= (iSetting * 60);


	// days
	iSetting = theApp.GetProfileInt(registrySectionSettings, registryHistoryMoveLongDays, 7);
	if (iSetting > 33) iSetting = 33;
	if (iSetting < 0) iSetting = 0;
	m_historyLongDays.SetCurSel(iSetting);
	m_iHistoryLongSettingDays = iSetting;

	iNr = iSetting;
	switch (iSetting)
	{
		case 31:
			iNr = 60;
		break;
		case 32:
			iNr = 120;
		break;
		case 33:
			iNr = 365;
		break;
	}

	if (iNr > 365) iNr = 365;
	if (iNr < 0) iNr = 0;

	m_iHistoryLongTime = iNr*24;
	m_iHistoryLongTime*= 60;			// minutes
	// hours

	iSetting = theApp.GetProfileInt(registrySectionSettings, registryHistoryMoveLongHours, 0);
	m_historyLongHours.SetCurSel(iSetting);
	m_iHistoryLongSettingHours = iSetting;

	if (iSetting > 23) iSetting = 23;
	if (iSetting < 0) iSetting = 0;

	m_iHistoryLongTime+= (iSetting * 60);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryHistoryMoveLong, 0);
	m_checkLong.SetCheck(iCheck);
	m_bMoveToLongHistory = (iCheck > 0);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryHistoryBackup, 0);
	m_checkBackup.SetCheck(iCheck);
	m_bBackupHistory = (iCheck > 0);


////
	LogHistory();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsHistory::SetColumnDefault()
{
	m_iHistoryColumnIn[0] = COLUMN_HISTORY_PROJECT+1;
	m_iHistoryColumnIn[1] = COLUMN_HISTORY_APPLICATION+1;
	m_iHistoryColumnIn[2] = COLUMN_HISTORY_NAME+1;
	m_iHistoryColumnIn[3] = COLUMN_HISTORY_ELAPSED+1;
	m_iHistoryColumnIn[4] = COLUMN_HISTORY_COMPLETED+1;
	m_iHistoryColumnIn[5] = COLUMN_HISTORY_REPORTED+1;
	m_iHistoryColumnIn[6] = COLUMN_HISTORY_USE+1;
	m_iHistoryColumnIn[7] = COLUMN_HISTORY_RATIO+1;
	m_iHistoryColumnIn[8] = COLUMN_HISTORY_STATUS+1;
	m_iHistoryColumnIn[9] = COLUMN_HISTORY_COMPUTER+1;
	m_iHistoryColumnIn[10] = 0;
	m_iHistoryColumnIn[11] = 0;
}


void CDlgSettingsHistory::ReadColumsFromRegistry()
{
	int		iNr;

	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn0, COLUMN_HISTORY_PROJECT+1);	m_iHistoryColumnIn[0] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn1, COLUMN_HISTORY_APPLICATION+1);m_iHistoryColumnIn[1] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn2, COLUMN_HISTORY_NAME+1);		m_iHistoryColumnIn[2] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn3, COLUMN_HISTORY_ELAPSED+1);	m_iHistoryColumnIn[3] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn4, COLUMN_HISTORY_COMPLETED+1);	m_iHistoryColumnIn[4] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn5, COLUMN_HISTORY_REPORTED+1);	m_iHistoryColumnIn[5] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn6, COLUMN_HISTORY_USE+1);		m_iHistoryColumnIn[6] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn7, COLUMN_HISTORY_RATIO+1);		m_iHistoryColumnIn[7] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn8, COLUMN_HISTORY_STATUS+1);		m_iHistoryColumnIn[8] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn9, COLUMN_HISTORY_COMPUTER+1);	m_iHistoryColumnIn[9] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn10, COLUMN_HISTORY_VIRTUAL+1);	m_iHistoryColumnIn[10] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryHistoryColumn11, COLUMN_HISTORY_MEMORY+1);	m_iHistoryColumnIn[11] = iNr;
	
	bool	bDouble = false;
	for (int iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		iNr = m_iHistoryColumnIn[iCount];
		for (int iCountDouble = iCount+1; iCountDouble < NUM_REMOTE_HISTORY_COLUMNS; iCountDouble++)
		{
			if (m_iHistoryColumnIn[iCountDouble] == iNr)
			{
				if (iNr > 0)
				{
					bDouble = true;
				}
			}
		}
	}
	if (bDouble)
	{
		SetColumnDefault();
	}

	HistoryColumns();
	EnableCheck();
}

void CDlgSettingsHistory::WriteColumsToRegistry()
{
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn0, m_iHistoryColumnIn[0]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn1, m_iHistoryColumnIn[1]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn2, m_iHistoryColumnIn[2]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn3, m_iHistoryColumnIn[3]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn4, m_iHistoryColumnIn[4]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn5, m_iHistoryColumnIn[5]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn6, m_iHistoryColumnIn[6]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn7, m_iHistoryColumnIn[7]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn8, m_iHistoryColumnIn[8]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn9, m_iHistoryColumnIn[9]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn10, m_iHistoryColumnIn[10]);
	theApp.WriteProfileInt(registrySectionColumn, registryHistoryColumn11, m_iHistoryColumnIn[11]);
}

void CDlgSettingsHistory::HistoryColumns()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		m_iHistoryColumnLookup[iCount] = -1;
		m_iHistoryColumn[iCount] = -1;
	}
	m_iActiveColumns = 0;
	for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		if (m_iHistoryColumnIn[iCount] > 0)
		{
			if (m_iHistoryColumnIn[iCount] <= NUM_REMOTE_HISTORY_COLUMNS)
			{
				m_iHistoryColumn[m_iActiveColumns] = m_iHistoryColumnIn[iCount] -1;
				m_iActiveColumns++;
			}
		}
	}	
	for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		iNr = m_iHistoryColumn[iCount];
		if (iNr >=0) m_iHistoryColumnLookup[iNr] = iCount;
	}
}

void CDlgSettingsHistory::EnableCheck()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		iNr = m_iHistoryColumn[iCount];
		if (iNr < 0) return;
		if (iNr > NUM_REMOTE_HISTORY_COLUMNS) return;
		m_checkShow[iNr].SetCheck(TRUE);
	}
}

void CDlgSettingsHistory::GetHistoryTime()
{
	int iSettingDays, iSettingHours, iNr, iHistoryTimeNew;//, iNewDeadline;
	int	iBoxResult;

	iSettingDays = m_historyDays.GetCurSel();
	iSettingHours = m_historyHours.GetCurSel();

	iNr = iSettingDays;
	switch (iSettingDays)
	{
		case 31:
			iNr = 60;
		break;
		case 32:
			iNr = 120;
		break;
		case 33:
			iNr = 365;
		break;
	}

	iHistoryTimeNew = iNr*24;
	iHistoryTimeNew*= 60;					// minutes
	iHistoryTimeNew+= (iSettingHours * 60);	// minutes

	if (iHistoryTimeNew < m_iHistoryTime)
	{
		iBoxResult = AfxMessageBox(gszTranslation[PosGroupDialogSettingsHistoryDayWarning],MB_YESNO|MB_ICONEXCLAMATION );
		if (iBoxResult != IDYES)
		{
			m_historyDays.SetCurSel(m_iHistorySettingDays);
			m_historyHours.SetCurSel(m_iHistorySettingHours);
			return;
		}
	}


//	theApp.WriteProfileInt(registrySectionSettings, registryHistoryRemoveDays, iSettingDays);
	m_iHistorySettingDays = iSettingDays;
	m_iHistorySettingHours= iSettingHours;
	m_iHistoryTime = iHistoryTimeNew;
}

void CDlgSettingsHistory::GetHistoryLongTime()
{
	int iSettingDays, iSettingHours, iNr, iHistoryLongTimeNew;

	iSettingDays = m_historyLongDays.GetCurSel();
	iSettingHours = m_historyLongHours.GetCurSel();

	iNr = iSettingDays;
	switch (iSettingDays)
	{
		case 31:
			iNr = 60;
		break;
		case 32:
			iNr = 120;
		break;
		case 33:
			iNr = 365;
		break;
	}

	iHistoryLongTimeNew = iNr*24;
	iHistoryLongTimeNew*= 60;					// minutes
	iHistoryLongTimeNew+= (iSettingHours * 60);	// minutes

	m_iHistoryLongSettingDays = iSettingDays;
	m_iHistoryLongSettingHours= iSettingHours;
	m_iHistoryLongTime = iHistoryLongTimeNew;
}

void CDlgSettingsHistory::LogHistory()
{
	if (!m_bHistoryEnabled)
	{
		CLoggingMessage *psLog;
		psLog = new	CLoggingMessage ;
		CString sTxt;
		sTxt = gszTranslation[PosViewHistoryStatusToEnable];
		sTxt+= gszTranslation[PosViewHistoryStatusToEnableMenu];
		psLog->m_pFrom = "History disabled = rules disabled"; psLog->m_pText = sTxt.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, false);
		delete psLog;
	}
}

void CDlgSettingsHistory::SetColumn(int iColumn, int iCheck)
{
	int		iCount;

	if (iCheck)
	{
		// Check if already there
		for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
		{
			if (m_iHistoryColumnIn[iCount] == iColumn+1) return;
		}

		for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
		{
			if (m_iHistoryColumnIn[iCount] <= 0)
			{
				m_iHistoryColumnIn[iCount] = iColumn+1;
				m_apply.EnableWindow(TRUE);
				break;
			}
		}
	}
	else
	{
		for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
		{
			if (m_iHistoryColumnIn[iCount] == iColumn+1)
			{
				m_iHistoryColumnIn[iCount] = 0;
				m_apply.EnableWindow(TRUE);
			}
		}
	}
	WriteColumsToRegistry();
	HistoryColumns();
}

void CDlgSettingsHistory::SetColumnHide(int iColumn, int iCheck)
{
	CView		*pView;
	CDocument	*pDoc;
	POSITION	posDoc, posView;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			posView = pDoc->GetFirstViewPosition();
			while (1)
			{
				pView = pDoc->GetNextView(posView);
				if (pView == NULL) break;		
				pView->PostMessage(UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH,iColumn,iCheck);
			}
		}
	}
}

void CDlgSettingsHistory::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_0, m_checkShow[COLUMN_HISTORY_PROJECT]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_1, m_checkShow[COLUMN_HISTORY_APPLICATION]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_2, m_checkShow[COLUMN_HISTORY_NAME]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_3, m_checkShow[COLUMN_HISTORY_ELAPSED]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_4, m_checkShow[COLUMN_HISTORY_COMPLETED]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_5, m_checkShow[COLUMN_HISTORY_REPORTED]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_6, m_checkShow[COLUMN_HISTORY_USE]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_7, m_checkShow[COLUMN_HISTORY_RATIO]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_8, m_checkShow[COLUMN_HISTORY_STATUS]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_9, m_checkShow[COLUMN_HISTORY_COMPUTER]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_10, m_checkShow[COLUMN_HISTORY_VIRTUAL]);
	DDX_Control(pDX, IDC_CHECK_HISTORY_COLUMN_11, m_checkShow[COLUMN_HISTORY_MEMORY]);

	DDX_Control(pDX, IDC_CHECK_HISTORY_LOGGING, m_enableHistoryLogging);
	DDX_Control(pDX, IDC_COMBO_HISTORY_DELETE_DAY, m_historyDays);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_TEXT_HISTORY_REMOVE_AFTER, m_textRemoveAfter);
	DDX_Control(pDX, IDC_TEXT_HISTORY_DAYS, m_textRemoveDays);
	DDX_Control(pDX, IDC_COMBO_HISTORY_DELETE_HOUR, m_historyHours);
	DDX_Control(pDX, IDC_TEXT_HISTORY_HOURS, m_textRemoveHours);
	DDX_Control(pDX, IDC_CHECK_HISTORY_SMART, m_enableHistorySmartMode);
	DDX_Control(pDX, IDC_TEXT_HISTORY_MAX_UPDATE, m_textMaxUpdateTime);
	DDX_Control(pDX, IDC_EDIT_HISTORY_MAX_UPDATE, m_editMaxUpdateTime);
	DDX_Control(pDX, IDC_TEXT_HISTORY_MAX_UPDATE_SECONDS, m_textMaxUpdateTimeSeconds);
	DDX_Control(pDX, IDC_TEXT_HISTORY_MIN_UPDATE, m_textMinUpdateTime);
	DDX_Control(pDX, IDC_EDIT_HISTORY_MIN_UPDATE, m_editMinUpdateTime);
	DDX_Control(pDX, IDC_TEXT_HISTORY_MIN_UPDATE_SECONDS, m_textMinUpdateTimeSeconds);
	DDX_Control(pDX, IDC_CHECK_HISTORY_LONG, m_checkLong);
	DDX_Control(pDX, IDC_COMBO_HISTORY_MOVE_HOUR, m_historyLongHours);
	DDX_Control(pDX, IDC_TEXT_HISTORY_HOURS2, m_textMoveHours);
	DDX_Control(pDX, IDC_COMBO_HISTORY_MOVE_DAY, m_historyLongDays);
	DDX_Control(pDX, IDC_TEXT_HISTORY_DAYS2, m_textMoveDays);
	DDX_Control(pDX, IDC_CHECK_HISTORY_NOT_ACCURATE, m_checkAccurate);
	DDX_Control(pDX, IDC_CHECK_HISTORY_BACKUP, m_checkBackup);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDAPPLY, m_apply);
	DDX_Control(pDX, IDC_TEXT_TASKS_SEQUENCE, m_textSequence);
}


BEGIN_MESSAGE_MAP(CDlgSettingsHistory, CPropertyPage)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_GET, OnSettingsHistoryGet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET, OnSettingsHistorySet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistoryWidth) 

	ON_BN_CLICKED(IDC_CHECK_HISTORY_LOGGING, &CDlgSettingsHistory::OnBnClickedCheckHistoryLogging)
	ON_BN_CLICKED(IDAPPLY, &CDlgSettingsHistory::OnBnClickedApply)
	ON_BN_CLICKED(IDOK, &CDlgSettingsHistory::OnBnClickedOk)

	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_0, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn0)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_1, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn1)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_2, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn2)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_3, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn3)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_4, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn4)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_5, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn5)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_6, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn6)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_7, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn7)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_8, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn8)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_9, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn9)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_10, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn10)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_COLUMN_11, &CDlgSettingsHistory::OnBnClickedCheckHistoryColumn11)

	ON_CBN_SELCHANGE(IDC_COMBO_HISTORY_DELETE_DAY, &CDlgSettingsHistory::OnCbnSelchangeComboHistoryDeleteDay)
	ON_CBN_SELCHANGE(IDC_COMBO_HISTORY_DELETE_HOUR, &CDlgSettingsHistory::OnCbnSelchangeComboHistoryDeleteHour)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_SMART, &CDlgSettingsHistory::OnBnClickedCheckHistorySmart)
	ON_EN_CHANGE(IDC_EDIT_HISTORY_MAX_UPDATE, &CDlgSettingsHistory::OnEnChangeEditHistoryMaxUpdate)
	ON_EN_CHANGE(IDC_EDIT_HISTORY_MIN_UPDATE, &CDlgSettingsHistory::OnEnChangeEditHistoryMinUpdate)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_LONG, &CDlgSettingsHistory::OnBnClickedCheckHistoryLong)
	ON_CBN_SELCHANGE(IDC_COMBO_HISTORY_MOVE_HOUR, &CDlgSettingsHistory::OnCbnSelchangeComboHistoryLongHour)
	ON_CBN_SELCHANGE(IDC_COMBO_HISTORY_MOVE_DAY, &CDlgSettingsHistory::OnCbnSelchangeComboHistoryLongDay)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_NOT_ACCURATE, &CDlgSettingsHistory::OnBnClickedCheckHistoryNotAccurate)
	ON_BN_CLICKED(IDC_CHECK_HISTORY_BACKUP, &CDlgSettingsHistory::OnBnClickedCheckHistoryBackup)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsHistory::OnBnClickedHelp)
END_MESSAGE_MAP()

 
// CDlgSettingsHistory message handlers

LRESULT CDlgSettingsHistory::OnSettingsHistoryGet(WPARAM parm1, LPARAM parm2)
{
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		*pArrayColumn = m_iHistoryColumn[iCount];
		pArrayColumn++;
		*pArrayLookup = m_iHistoryColumnLookup[iCount];
		*pArrayLookup++;
	}

	return m_iActiveColumns;
}

LRESULT CDlgSettingsHistory::OnSettingsHistorySet(WPARAM parm1, LPARAM parm2)
{
//	int iNr;
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		m_iHistoryColumnIn[iCount] = (*pArrayColumn)+1;
		pArrayColumn++;
	}
	WriteColumsToRegistry();
	HistoryColumns();

	return 0;
}

LRESULT CDlgSettingsHistory::OnSettingsHistoryWidth(WPARAM parm1, LPARAM parm2)
{
	int iColumn, iWidth;
	int	iNr;

	iColumn = (int) parm1;
	iWidth = (int) parm2;

	iNr = m_iHistoryColumn[iColumn];
	if (iNr < 0) return 0;
	if (iNr > NUM_REMOTE_HISTORY_COLUMNS) return 0;
	if (iWidth > 0)	m_checkShow[iNr].SetCheck(TRUE);
	else			m_checkShow[iNr].SetCheck(FALSE);

	return 0;
}

void CDlgSettingsHistory::OnBnClickedApply()
{
	POSITION posDoc, posView ;
	CRemoteCombinedDoc *pDocCombined;
//	CRemoteDoc *pDoc;
	CView *pView;

	bool	bProject = false, bName = false , bComputer = false;

	m_apply.EnableWindow(FALSE);
	ReadColumsFromRegistry();

	for (int iCount = 0; iCount < m_iActiveColumns; iCount++)
	{
		if (m_iHistoryColumn[iCount] == COLUMN_HISTORY_PROJECT)	bProject = true;
		if (m_iHistoryColumn[iCount] == COLUMN_HISTORY_NAME)		bName = true;
		if (m_iHistoryColumn[iCount] == COLUMN_HISTORY_COMPUTER)	bComputer = true;
	}

	if (bProject && bName && bComputer)
	{
	}
	else
	{
		AfxMessageBox(gszTranslation[PosGroupDialogSettingsTasksMsgBoxExclude],MB_ICONEXCLAMATION);	//"Some functions may not work if you exclude the columns: Project or Computer"
	}

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc)
	{
		pDocCombined = (CRemoteCombinedDoc*) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		posView = pDocCombined->GetFirstViewPosition();
		while (posView)
		{
			pView = pDocCombined->GetNextView(posView);
			pView->SendMessage(WM_COMMAND, UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_HISTORY,0);	// fist send to the document that sends it to the history view
		}
	}
}

void CDlgSettingsHistory::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn0()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_HISTORY_PROJECT].GetCheck();
	SetColumn(COLUMN_HISTORY_PROJECT, 1);
	SetColumnHide(COLUMN_HISTORY_PROJECT, iCheck);
}


void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn1()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_APPLICATION].GetCheck();
	SetColumn(COLUMN_HISTORY_APPLICATION, 1);
	SetColumnHide(COLUMN_HISTORY_APPLICATION, iCheck);
}


void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn2()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_NAME].GetCheck();
//	SetColumn(COLUMN_HISTORY_NAME, 1);
	SetColumn(COLUMN_HISTORY_NAME, iCheck);
	SetColumnHide(COLUMN_HISTORY_NAME, iCheck);
}


void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn3()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_ELAPSED].GetCheck();
	SetColumn(COLUMN_HISTORY_ELAPSED, iCheck);
	SetColumnHide(COLUMN_HISTORY_ELAPSED, iCheck);
}


void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn4()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_COMPLETED].GetCheck();
	SetColumn(COLUMN_HISTORY_COMPLETED, iCheck);
	SetColumnHide(COLUMN_HISTORY_COMPLETED, iCheck);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn5()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_REPORTED].GetCheck();
	SetColumn(COLUMN_HISTORY_REPORTED, iCheck);
	SetColumnHide(COLUMN_HISTORY_REPORTED, iCheck);
}


void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn6()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_USE].GetCheck();
	SetColumn(COLUMN_HISTORY_USE, iCheck);
	SetColumnHide(COLUMN_HISTORY_USE, iCheck);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn7()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_RATIO].GetCheck();
	SetColumn(COLUMN_HISTORY_RATIO, iCheck);
	SetColumnHide(COLUMN_HISTORY_RATIO, iCheck);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn8()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_STATUS].GetCheck();
	SetColumn(COLUMN_HISTORY_STATUS, iCheck);
	SetColumnHide(COLUMN_HISTORY_STATUS, iCheck);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn9()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_COMPUTER].GetCheck();
	SetColumn(COLUMN_HISTORY_COMPUTER, 1);
	SetColumnHide(COLUMN_HISTORY_COMPUTER, iCheck);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn10()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_VIRTUAL].GetCheck();
	SetColumn(COLUMN_HISTORY_VIRTUAL, iCheck);
	SetColumnHide(COLUMN_HISTORY_VIRTUAL, iCheck);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryColumn11()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_HISTORY_MEMORY].GetCheck();
	SetColumn(COLUMN_HISTORY_MEMORY, iCheck);
	SetColumnHide(COLUMN_HISTORY_MEMORY, iCheck);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryLogging()
{
	int iCheck;

	iCheck = m_enableHistoryLogging.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryHistoryLoggingEnable, iCheck);
	m_bHistoryEnabled = (iCheck > 0);

	LogHistory();
}

void CDlgSettingsHistory::OnBnClickedCheckHistorySmart()
{
	int iCheck;

	iCheck = m_enableHistorySmartMode.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryHistoryLoggingSmartMode, iCheck);
	m_bHistorySmartMode = (iCheck > 0);
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryNotAccurate()
{
	int iCheck;

	iCheck = m_checkAccurate.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryHistoryLoggingAccurate, iCheck);
	m_bHistoryLessAccurate = (iCheck > 0);
}

void CDlgSettingsHistory::OnCbnSelchangeComboHistoryDeleteDay()
{
	int iSetting;

	iSetting = m_historyDays.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryHistoryRemoveDays, iSetting);
	GetHistoryTime();

}

void CDlgSettingsHistory::OnCbnSelchangeComboHistoryDeleteHour()
{
	int iSetting;

	iSetting = m_historyHours.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryHistoryRemoveHours, iSetting);
	GetHistoryTime();
}

void CDlgSettingsHistory::OnEnChangeEditHistoryMaxUpdate()
{
	CString sTxt;

	m_editMaxUpdateTime.GetWindowText(sTxt);

	m_iHistoryMaxUpdateTime = atoi(sTxt);
	if (m_iHistoryMaxUpdateTime < UPDATE_HISTORY_TRIGGER_MIN) m_iHistoryMaxUpdateTime = UPDATE_HISTORY_TRIGGER_MIN;

	theApp.WriteProfileInt(registrySectionSettings, registryHistoryMaxUpdateTime, m_iHistoryMaxUpdateTime);
}


void CDlgSettingsHistory::OnEnChangeEditHistoryMinUpdate()
{
	CString sTxt;

	m_editMinUpdateTime.GetWindowText(sTxt);

	m_iHistoryMinUpdateTime = atoi(sTxt);
	if (m_iHistoryMinUpdateTime < UPDATE_HISTORY_TRIGGER_MIN) m_iHistoryMaxUpdateTime = UPDATE_HISTORY_TRIGGER_MIN;

	theApp.WriteProfileInt(registrySectionSettings, registryHistoryMinUpdateTime, m_iHistoryMinUpdateTime);
}


void CDlgSettingsHistory::OnBnClickedCheckHistoryLong()
{
	int	iCheck;

	iCheck = m_checkLong.GetCheck();
	m_bMoveToLongHistory = (iCheck > 0);

	theApp.WriteProfileInt(registrySectionSettings, registryHistoryMoveLong, iCheck);
}


void CDlgSettingsHistory::OnCbnSelchangeComboHistoryLongHour()
{
	int iSetting;

	iSetting = m_historyLongHours.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryHistoryMoveLongHours, iSetting);
	GetHistoryLongTime();
}


void CDlgSettingsHistory::OnCbnSelchangeComboHistoryLongDay()
{
	int iSetting;

	iSetting = m_historyLongDays.GetCurSel();
	theApp.WriteProfileInt(registrySectionSettings, registryHistoryMoveLongDays, iSetting);
	GetHistoryLongTime();
}

void CDlgSettingsHistory::OnBnClickedCheckHistoryBackup()
{
	int	iCheck;

	iCheck = m_checkBackup.GetCheck();
	m_bBackupHistory = (iCheck > 0);

	theApp.WriteProfileInt(registrySectionSettings, registryHistoryBackup, iCheck);
}

BOOL CDlgSettingsHistory::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsHistory::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#history";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

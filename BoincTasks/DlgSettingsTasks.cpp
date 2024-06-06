// DlgSettingsTasks.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsMain.h"
#include "RemoteCombinedDoc.h"
#include "DlgSettingsTasks.h"
#include "TemplateRemoteTaskView.h"
#include "RemoteCombinedTaskView.h"
#include "RemoteCombinedDoc.h"
#include "Translation.h"

// CDlgSettingsTasks dialog

IMPLEMENT_DYNAMIC(CDlgSettingsTasks, CPropertyPage)

CDlgSettingsTasks::CDlgSettingsTasks()
	: CPropertyPage(CDlgSettingsTasks::IDD)
{
	m_iActiveColumns = 0;
}

CDlgSettingsTasks::~CDlgSettingsTasks()
{
}

void CDlgSettingsTasks::SetUserFriendly()
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
				pView->PostMessage(UWM_MSG_SETTING_TASKS, m_bUserFriendly,0);
			}
		}
	}
}


void CDlgSettingsTasks::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_0, m_checkShow[COLUMN_PROJECT]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_1, m_checkShow[COLUMN_APPLICATION]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_2, m_checkShow[COLUMN_NAME]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_3, m_checkShow[COLUMN_ELAPSED]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_4, m_checkShow[COLUMN_CPU_PERCENTAGE]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_5, m_checkShow[COLUMN_PERCENTAGE]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_6, m_checkShow[COLUMN_TIME_LEFT]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_7, m_checkShow[COLUMN_DEADLINE]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_8, m_checkShow[COLUMN_USE]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_9, m_checkShow[COLUMN_STATE]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_10, m_checkShow[COLUMN_TTHROTTLE]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_11, m_checkShow[COLUMN_TEMPERATURE]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_12, m_checkShow[COLUMN_COMPUTER]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_13, m_checkShow[COLUMN_CHECKPOINT]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_14, m_checkShow[COLUMN_DEBT]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_15, m_checkShow[COLUMN_RECEIVED]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_16, m_checkShow[COLUMN_ACCOUNT]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_17, m_checkShow[COLUMN_VIRTUAL_MEMORY]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_18, m_checkShow[COLUMN_MEMORY]);
	DDX_Control(pDX, IDC_CHECK_TASKS_COLUMN_19, m_checkShow[COLUMN_SHARE]);

	DDX_Control(pDX, IDAPPLY, m_apply);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_CHECK_USE_CONDENSE, m_useCondense);
	DDX_Control(pDX, IDC_TEXT_TASKS_SEQUENCE, m_textSequence);
	DDX_Control(pDX, IDC_CHECK_RATIO_LONG_TERM, m_checkRatioLongTerm);
	DDX_Control(pDX, IDC_CHECK_USER_FRIENDLY, m_checkUserFriendly);
	DDX_Control(pDX, IDC_CHECK_DEADLINE_REMAINING, m_checkDeadlineShowRemaining);
	DDX_Control(pDX, IDC_TEXT_TASKS_STATUS_SORTING, m_textStatusSorting);
	DDX_Control(pDX, IDC_LIST_TASKS_STATUS_SORTING, m_listTasksStatusSorting);
	DDX_Control(pDX, IDC_BUTTON_TASKS_UP, m_buttonTasksUp);
	DDX_Control(pDX, IDC_BUTTON_TASKS_DOWN, m_buttonTasksDown);
	DDX_Control(pDX, ID_TASKS_DEFAULT_SORTING, m_buttonDefaultSorting);
	DDX_Control(pDX, IDC_TEXT_TASKS_MAX_UPDATE, m_textMaxUpdate);
	DDX_Control(pDX, IDC_EDIT_TASKS_MAX_UPDATE, m_editMaxUpdate);
	DDX_Control(pDX, IDC_TEXT_TASKS_MAX_UPDATE_SECONDS, m_textSeconds);
	DDX_Control(pDX, IDC_TEXT_TASKS_DIGITS_CPU, m_textCpuDigits);
	DDX_Control(pDX, IDC_TEXT_TASKS_DIGITS_PROGRESS, m_textProgressDigits);
	DDX_Control(pDX, IDC_EDIT_TASKS_DIGITS_CPU, m_editCpuDigits);
	DDX_Control(pDX, IDC_EDIT_TASKS_DIGITS_PROGRESS, m_editProgressDigits);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDC_CHECK_SORT_APPLICATION_NR, m_checkSortOnNr);
}

BOOL CDlgSettingsTasks::OnInitDialog()
{
	CPropertySheet	*pSheet;
	int				iCheck;
	CString			sFormat;

	// translations
	CPropertyPage::OnInitDialog();
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsTasksTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_apply.SetWindowText(gszTranslation[PosDialogCommonButtonsApply]);

	m_textSequence.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksGroupName]);
	m_checkShow[COLUMN_PROJECT].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskProject]);
	m_checkShow[COLUMN_APPLICATION].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskApplication]);
	m_checkShow[COLUMN_NAME].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskName]);
	m_checkShow[COLUMN_ELAPSED].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskElapsedTime]);
	m_checkShow[COLUMN_CPU_PERCENTAGE].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskCpuPerc]);
	m_checkShow[COLUMN_PERCENTAGE].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskProgress]);
	m_checkShow[COLUMN_TIME_LEFT].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskTimeLeft]);
	m_checkShow[COLUMN_DEADLINE].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskDeadline]);
	m_checkShow[COLUMN_USE].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskUse]);
	m_checkShow[COLUMN_STATE].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskStatus]);
	m_checkShow[COLUMN_TTHROTTLE].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskThrottle]);
	m_checkShow[COLUMN_TEMPERATURE].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskTemperature]);
	m_checkShow[COLUMN_COMPUTER].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskComputer]);
	m_checkShow[COLUMN_CHECKPOINT].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskCheckpoint]);
	m_checkShow[COLUMN_DEBT].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskDebt]);
	m_checkShow[COLUMN_RECEIVED].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskReceived]);
	m_checkShow[COLUMN_ACCOUNT].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskAccount]);
	m_checkShow[COLUMN_VIRTUAL_MEMORY].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskVirtual]);
	m_checkShow[COLUMN_MEMORY].SetWindowText(gszTranslation[PosGroupDialogSettingsTaskMemory]);
	m_checkShow[COLUMN_SHARE].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectShare]);

	m_checkUserFriendly.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksCheckUserFriendly]);
	m_useCondense.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksCheckCondense]);
	m_checkRatioLongTerm.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksRatioLong]);
	m_checkDeadlineShowRemaining.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksDeadlineRemaining]);
	m_checkSortOnNr.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksApplicationSortOnNr]);

	m_textStatusSorting.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksSortStatus]);
	m_buttonDefaultSorting.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksButtonDefault]);

	m_textMaxUpdate.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMaxUpdate]);
	m_textSeconds.SetWindowText(gszTranslation[PosGroupDialogSettingsHistoryMaxUpdateSec]);

	m_textCpuDigits.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksCpuPercDigits]);
	m_textProgressDigits.SetWindowText(gszTranslation[PosGroupDialogSettingsTasksCpuProgressDigits]);
	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	ReadColumsFromRegistry();

	m_apply.EnableWindow(FALSE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryUserFriendly, 1);
	m_checkUserFriendly.SetCheck(iCheck);
	m_bUserFriendly = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryCondenseUse, 1);
	m_useCondense.SetCheck(iCheck);
	m_bUseCondense = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryRatioLongTerm, 1);
	m_checkRatioLongTerm.SetCheck(iCheck);
	m_bRatioLongTerm = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryDeadlineShowRemaining, 0);
	m_checkDeadlineShowRemaining.SetCheck(iCheck);
	m_bDeadlineShowRemaining = (iCheck == TRUE);

	iCheck = theApp.GetProfileInt(registrySectionSettings, registrySortApplicationOnNr, 0);
	m_checkSortOnNr.SetCheck(iCheck);
	m_bSortOnApplicationNr = (iCheck == TRUE);

	m_iMaxUpdateTime = theApp.GetProfileInt(registrySectionSettings, registryTasksyMaxUpdateTime, 120);
	if (m_iMaxUpdateTime < 1) m_iMaxUpdateTime = 1;
	if (m_iMaxUpdateTime > 240) m_iMaxUpdateTime = 240;
	sFormat.Format("%d",m_iMaxUpdateTime);
	m_editMaxUpdate.SetWindowText(sFormat);

	m_iCpuDigits = theApp.GetProfileInt(registrySectionSettings, registryTasksCpuDigits, 2);
	if (m_iCpuDigits < 0) m_iCpuDigits = 0;
	if (m_iCpuDigits > 9) m_iCpuDigits = 9;
	sFormat.Format("%d",m_iCpuDigits);
	m_editCpuDigits.SetWindowText(sFormat);

	m_iProgressDigits = theApp.GetProfileInt(registrySectionSettings, registryTasksProgressDigits, 3);
	if (m_iProgressDigits < 0) m_iProgressDigits = 0;
	if (m_iProgressDigits > 9) m_iProgressDigits = 9;
	sFormat.Format("%d",m_iProgressDigits);
	m_editProgressDigits.SetWindowText(sFormat);

	iCheck = m_checkShow[COLUMN_CHECKPOINT].GetCheck();

	m_bCheckpoint = iCheck != 0;


	if (!ReadStatusListRegistry())
	{
		DefaultStatusList();
	}
	FillStatusList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsTasks::DefaultStatusList()
{
	theApp.WriteProfileString(registrySectionColumn, registryTasksStatusSort, "");
	m_lListStatusSorting.clear();
	m_lListStatusSorting.push_back(PosViewTasksStatusAbortedProject);
	m_lListStatusSorting.push_back(PosViewTasksStatusComputation);
	m_lListStatusSorting.push_back(PosViewTasksStatusDownloadError);
	m_lListStatusSorting.push_back(PosViewTasksStatusDownloading);
	m_lListStatusSorting.push_back(PosViewTasksStatusUploading);
	m_lListStatusSorting.push_back(PosViewTasksStatusReadyTR);
	m_lListStatusSorting.push_back(PosViewTasksStatusReadyTS);
	m_lListStatusSorting.push_back(PosViewTasksStatusWaitingTR);
	m_lListStatusSorting.push_back(PosViewTasksStatusRunning);
	m_lListStatusSorting.push_back(PosViewTasksStatusRunningLP);
	m_lListStatusSorting.push_back(PosViewTasksStatusRunningHP);
	m_lListStatusSorting.push_back(PosViewTasksStatusAbortedUser);
	m_lListStatusSorting.push_back(PosViewTasksStatusAborted);
	m_lListStatusSorting.push_back(PosViewTasksStatusNew);
	m_lListStatusSorting.push_back(PosViewTasksStatusSuspended);
	m_lListStatusSorting.push_back(PosViewTasksSuspendProject);
	m_lListStatusSorting.push_back(PosViewTasksSuspendPrefix);
	SortStatusList();
	OffsetStatusList();
}

void CDlgSettingsTasks::FillStatusList()
{
	m_listTasksStatusSorting.ResetContent();

	for (int iCount = 0; iCount < (int) m_lListStatusSorting.size(); iCount++)
	{
		char *pTxt;
		pTxt = gszTranslation[m_lListStatusSorting.at(iCount)+STATUS_MIN];
		m_listTasksStatusSorting.AddString(pTxt);
	}
}

void CDlgSettingsTasks::SortStatusList()
{
	bool bExchange;

	bExchange = true;
	while (bExchange)
	{
		bExchange = false;
		for (int iCount = 0; iCount < (int) m_lListStatusSorting.size()-1; iCount++)
		{
			if (strcmp(gszTranslation[m_lListStatusSorting.at(iCount)],gszTranslation[m_lListStatusSorting.at(iCount+1)]) > 0)
			{
				bExchange = true;
				int iTemp;
				iTemp = m_lListStatusSorting.at(iCount);
				m_lListStatusSorting.at(iCount) = m_lListStatusSorting.at(iCount+1);
				m_lListStatusSorting.at(iCount+1) = iTemp;
			}
		}
	}
}

void CDlgSettingsTasks::OffsetStatusList()
{
	// now make the numbers offsets
	for (int iCount = 0; iCount < (int) m_lListStatusSorting.size(); iCount++)
	{
		m_lListStatusSorting.at(iCount) -= STATUS_MIN;
	}
}

void CDlgSettingsTasks::ReadStatusList()
{
	int iItems, iOffset;
	CString sItemTxt;

	iItems = m_listTasksStatusSorting.GetCount();
	m_lListStatusSorting.clear();

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		m_listTasksStatusSorting.GetText(iCount, sItemTxt);
		iOffset = GetOffsetText(&sItemTxt);
		m_lListStatusSorting.push_back(iOffset);
	}
}

int CDlgSettingsTasks::GetOffsetText(CString *psTxt)
{
	char *pcTxt;
	pcTxt = psTxt->GetBuffer();


	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusAbortedProject]) == 0) return PosViewTasksStatusAbortedProject-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusComputation]) == 0) return PosViewTasksStatusComputation-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusDownloadError]) == 0) return PosViewTasksStatusDownloadError-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusDownloading]) == 0) return PosViewTasksStatusDownloading-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusUploading]) == 0) return PosViewTasksStatusUploading-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusReadyTR]) == 0) return PosViewTasksStatusReadyTR-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusReadyTS]) == 0) return PosViewTasksStatusReadyTS-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusWaitingTR]) == 0) return PosViewTasksStatusWaitingTR-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusRunning]) == 0) return PosViewTasksStatusRunning-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusRunningLP]) == 0) return PosViewTasksStatusRunningLP-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusRunningHP]) == 0) return PosViewTasksStatusRunningHP-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusAbortedUser]) == 0) return PosViewTasksStatusAbortedUser-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusAborted]) == 0) return PosViewTasksStatusAborted-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusNew]) == 0) return PosViewTasksStatusNew-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksStatusSuspended]) == 0) return PosViewTasksStatusSuspended-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksSuspendProject]) == 0) return PosViewTasksSuspendProject-STATUS_MIN;
	if (strcmp(pcTxt,gszTranslation[PosViewTasksSuspendPrefix]) == 0) return PosViewTasksSuspendPrefix-STATUS_MIN;

	psTxt->ReleaseBuffer();
	return 0;
}

bool CDlgSettingsTasks::ValidStatusList()
{
	int iFound;

	iFound = 0;
	for (int iCount = 0; iCount < (int) m_lListStatusSorting.size(); iCount++)
	{
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusAbortedProject-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusComputation-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusDownloadError-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusDownloading-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusUploading-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusReadyTR-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusReadyTS-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusWaitingTR-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusRunning-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusRunningLP-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusRunningHP-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusAbortedUser-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusAborted-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusNew-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksStatusSuspended-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksSuspendProject-STATUS_MIN) iFound++;
		if (m_lListStatusSorting.at(iCount) == PosViewTasksSuspendPrefix-STATUS_MIN) iFound++;
	}
	if (iFound != 17) return false;
	return true;
}

void CDlgSettingsTasks::WriteStatusListRegistry()
{
	char cChar;

	CString sStatusString;
	for (int iCount = 0; iCount < (int) m_lListStatusSorting.size(); iCount++)
	{
		cChar = m_lListStatusSorting.at(iCount) + 'A';
		sStatusString += cChar;
	}
	theApp.WriteProfileString(registrySectionColumn, registryTasksStatusSort, sStatusString);
}

bool CDlgSettingsTasks::ReadStatusListRegistry()
{
	CString sStatusString;
	int iLen;
	char cChar;

	sStatusString = theApp.GetProfileString(registrySectionColumn, registryTasksStatusSort, "");

	iLen = sStatusString.GetLength();
	m_lListStatusSorting.clear();

	for (int iCount = 0; iCount < iLen; iCount++)
	{
		cChar = sStatusString[iCount];
		m_lListStatusSorting.push_back(cChar-'A');
	}
	return ValidStatusList();
}

void CDlgSettingsTasks::SendStatusList()
{
	CRemoteCombinedDoc	*pDoc;
	POSITION	posDoc;
	int	iVal[100];
	std::deque<int> lToSend;

	for (int iCount = 0; iCount < (int) m_lListStatusSorting.size(); iCount++)
	{
		iVal[m_lListStatusSorting.at(iCount)] = iCount;
	}
	for (int iCount = 0; iCount < (int) m_lListStatusSorting.size(); iCount++)
	{
		lToSend.push_back(iVal[iCount]);
	}

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			pDoc->m_pTaskView->SendMessage(UWM_DLG_SETTINGS_TASKS_SORTING_STATUS, STATUS_MIN, (LPARAM) &lToSend);	// Send!!!
		}
	}
}

void CDlgSettingsTasks::ReadColumsFromRegistry()
{
	int		iNr;

	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn0, COLUMN_PROJECT+1);			m_iTasksColumnIn[0] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn1, COLUMN_APPLICATION+1);		m_iTasksColumnIn[1] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn2, COLUMN_NAME+1);				m_iTasksColumnIn[2] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn3, COLUMN_ELAPSED+1);			m_iTasksColumnIn[3] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn4, COLUMN_CPU_PERCENTAGE+1);	m_iTasksColumnIn[4] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn5, COLUMN_PERCENTAGE+1);		m_iTasksColumnIn[5] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn6, COLUMN_TIME_LEFT+1);		m_iTasksColumnIn[6] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn7, COLUMN_DEADLINE+1);			m_iTasksColumnIn[7] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn8,	COLUMN_USE+1);				m_iTasksColumnIn[8] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn9, COLUMN_STATE+1);			m_iTasksColumnIn[9] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn10, 0);						m_iTasksColumnIn[10] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn11, 0);						m_iTasksColumnIn[11] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn12, COLUMN_COMPUTER+1);		m_iTasksColumnIn[12] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn13, 0);						m_iTasksColumnIn[13] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn14, 0);						m_iTasksColumnIn[14] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn15, 0);						m_iTasksColumnIn[15] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn16, 0);						m_iTasksColumnIn[16] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn17, 0);						m_iTasksColumnIn[17] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn18, 0);						m_iTasksColumnIn[18] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTasksColumn19, 0);						m_iTasksColumnIn[19] = iNr;

	CheckIntegrity();
	TaskColumns();
	EnableCheck();
}

void CDlgSettingsTasks::CheckIntegrity()
{
	bool bEqual = false;

	for (int iCount1 = 0; iCount1 < NUM_REMOTE_TASK_COLUMNS; iCount1++)
	{
		for (int iCount2 = iCount1+1; iCount2 < NUM_REMOTE_TASK_COLUMNS; iCount2++)
		{
			if (m_iTasksColumnIn[iCount1] == m_iTasksColumnIn[iCount2])
			{
				if (m_iTasksColumnIn[iCount1] > 0)
				{
					bEqual = true;
				}
			}
		}
	}
	if (!bEqual) return;

	for (int iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		m_iTasksColumnIn[iCount] = 0;
	}

	m_iTasksColumnIn[0] = COLUMN_PROJECT+1;
	m_iTasksColumnIn[1] = COLUMN_APPLICATION+1;
	m_iTasksColumnIn[2] = COLUMN_NAME+1;
	m_iTasksColumnIn[3] = COLUMN_ELAPSED+1;
	m_iTasksColumnIn[4] = COLUMN_CPU_PERCENTAGE+1;
	m_iTasksColumnIn[5] = COLUMN_PERCENTAGE+1;
	m_iTasksColumnIn[6] = COLUMN_TIME_LEFT+1;
	m_iTasksColumnIn[7] = COLUMN_DEADLINE+1;
	m_iTasksColumnIn[8] = COLUMN_USE+1;
	m_iTasksColumnIn[9] = COLUMN_STATE+1;
	m_iTasksColumnIn[12] = COLUMN_COMPUTER+1;
	WriteColumsToRegistry();
}

void CDlgSettingsTasks::WriteColumsToRegistry()
{
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn0, m_iTasksColumnIn[0]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn1, m_iTasksColumnIn[1]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn2, m_iTasksColumnIn[2]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn3, m_iTasksColumnIn[3]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn4, m_iTasksColumnIn[4]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn5, m_iTasksColumnIn[5]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn6, m_iTasksColumnIn[6]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn7, m_iTasksColumnIn[7]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn8, m_iTasksColumnIn[8]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn9, m_iTasksColumnIn[9]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn10,m_iTasksColumnIn[10]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn11,m_iTasksColumnIn[11]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn12,m_iTasksColumnIn[12]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn13,m_iTasksColumnIn[13]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn14,m_iTasksColumnIn[14]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn15,m_iTasksColumnIn[15]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn16,m_iTasksColumnIn[16]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn17,m_iTasksColumnIn[17]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn18, m_iTasksColumnIn[18]);
	theApp.WriteProfileInt(registrySectionColumn, registryTasksColumn19, m_iTasksColumnIn[19]);
}

void CDlgSettingsTasks::TaskColumns()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		m_iTasksColumnLookup[iCount] = -1;
		m_iTasksColumn[iCount] = -1;
	}
	m_iActiveColumns = 0;
	for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		if (m_iTasksColumnIn[iCount] > 0)
		{
			if (m_iTasksColumnIn[iCount] <= NUM_REMOTE_TASK_COLUMNS)
			{
				m_iTasksColumn[m_iActiveColumns] = m_iTasksColumnIn[iCount] -1;
				m_iActiveColumns++;
			}
		}
	}	
	for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		iNr = m_iTasksColumn[iCount];
		if (iNr >=0) m_iTasksColumnLookup[iNr] = iCount;
	}
}

void CDlgSettingsTasks::EnableCheck()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		iNr = m_iTasksColumn[iCount];
		if (iNr < 0) return;
		if (iNr > NUM_REMOTE_TASK_COLUMNS) return;
		m_checkShow[iNr].SetCheck(TRUE);
	}
}

void CDlgSettingsTasks::SetColumn(int iColumn, int iCheck)
{
	int		iCount;

	if (iCheck)
	{
		// Check if already there
		for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
		{
			if (m_iTasksColumnIn[iCount] == iColumn+1) return;
		}

		for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
		{
			if (m_iTasksColumnIn[iCount] <= 0)
			{
				m_iTasksColumnIn[iCount] = iColumn+1;
				m_apply.EnableWindow(TRUE);
				break;
			}
		}
	}
	else
	{
		for (iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
		{
			if (m_iTasksColumnIn[iCount] == iColumn+1)
			{
				m_iTasksColumnIn[iCount] = 0;
				m_apply.EnableWindow(TRUE);
			}
		}
	}
	WriteColumsToRegistry();
	TaskColumns();
}

void CDlgSettingsTasks::SetColumnHide(int iColumn, int iCheck)
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
				pView->PostMessage(UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH,iColumn,iCheck);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CDlgSettingsTasks, CPropertyPage)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_GET, OnSettingsTasksGet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET, OnSettingsTasksSet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksWidth) 
	ON_MESSAGE (UWM_DLG_SETTINGS_TASKS_SORTING_STATUS, OnSettingsTasksSortingStatus) 

	ON_BN_CLICKED(IDAPPLY, &CDlgSettingsTasks::OnBnClickedApply)
	ON_BN_CLICKED(IDOK, &CDlgSettingsTasks::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_USE_CONDENSE, &CDlgSettingsTasks::OnBnClickedCheckUseCondense)
	ON_BN_CLICKED(IDC_CHECK_RATIO_LONG_TERM, &CDlgSettingsTasks::OnBnClickedCheckRatioLongTerm)
	ON_BN_CLICKED(IDC_CHECK_USER_FRIENDLY, &CDlgSettingsTasks::OnBnClickedCheckUserFriendly)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_0, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn0)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_1, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn1)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_2, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn2)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_3, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn3)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_4, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn4)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_5, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn5)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_6, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn6)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_7, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn7)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_8, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn8)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_9, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn9)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_10, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn10)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_11, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn11)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_12, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn12)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_13, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn13)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_14, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn14)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_15, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn15)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_16, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn16)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_17, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn17)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_18, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn18)
	ON_BN_CLICKED(IDC_CHECK_TASKS_COLUMN_19, &CDlgSettingsTasks::OnBnClickedCheckTasksColumn19)

	ON_BN_CLICKED(IDC_CHECK_DEADLINE_REMAINING, &CDlgSettingsTasks::OnBnClickedCheckDeadlineRemaining)
	ON_BN_CLICKED(IDC_BUTTON_TASKS_UP, &CDlgSettingsTasks::OnBnClickedButtonTasksUp)
	ON_LBN_SELCHANGE(IDC_LIST_TASKS_STATUS_SORTING, &CDlgSettingsTasks::OnLbnSelchangeListTasksStatusSorting)
	ON_BN_CLICKED(IDC_BUTTON_TASKS_DOWN, &CDlgSettingsTasks::OnBnClickedButtonTasksDown)
	ON_BN_CLICKED(ID_TASKS_DEFAULT_SORTING, &CDlgSettingsTasks::OnBnClickedTasksDefaultSorting)
	ON_EN_CHANGE(IDC_EDIT_TASKS_MAX_UPDATE, &CDlgSettingsTasks::OnEnChangeEditTasksMaxUpdate)
	ON_EN_CHANGE(IDC_EDIT_TASKS_DIGITS_CPU, &CDlgSettingsTasks::OnEnChangeEditTasksDigitsCpu)
	ON_EN_CHANGE(IDC_EDIT_TASKS_DIGITS_PROGRESS, &CDlgSettingsTasks::OnEnChangeEditTasksDigitsProgress)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsTasks::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_CHECK_SORT_APPLICATION_NR, &CDlgSettingsTasks::OnBnClickedCheckSortApplicationNr)
END_MESSAGE_MAP()

// CDlgSettingsTasks message handlers

LRESULT CDlgSettingsTasks::OnSettingsTasksGet(WPARAM parm1, LPARAM parm2)
{
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		*pArrayColumn = m_iTasksColumn[iCount];
		pArrayColumn++;
		*pArrayLookup = m_iTasksColumnLookup[iCount];
		*pArrayLookup++;
	}

	return m_iActiveColumns;
}

LRESULT CDlgSettingsTasks::OnSettingsTasksSet(WPARAM parm1, LPARAM parm2)
{
//	int iNr;
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_TASK_COLUMNS; iCount++)
	{
		m_iTasksColumnIn[iCount] = (*pArrayColumn)+1;
		pArrayColumn++;
	}
	WriteColumsToRegistry();
	TaskColumns();

	return 0;
}

LRESULT CDlgSettingsTasks::OnSettingsTasksWidth(WPARAM parm1, LPARAM parm2)
{
	int iColumn, iWidth;
	int	iNr;

	iColumn = (int) parm1;
	iWidth = (int) parm2;

	iNr = m_iTasksColumn[iColumn];
	if (iNr < 0) return 0;
	if (iNr > NUM_REMOTE_TASK_COLUMNS) return 0;
	if (iWidth > 0)	m_checkShow[iNr].SetCheck(TRUE);
	else			m_checkShow[iNr].SetCheck(FALSE);

	return 0;
}


LRESULT CDlgSettingsTasks::OnSettingsTasksSortingStatus(WPARAM parm1, LPARAM parm2)
{
	SendStatusList();
	return 0;
}

void CDlgSettingsTasks::OnBnClickedApply()
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
		if (m_iTasksColumn[iCount] == COLUMN_PROJECT)	bProject = true;
		if (m_iTasksColumn[iCount] == COLUMN_NAME)		bName = true;
		if (m_iTasksColumn[iCount] == COLUMN_COMPUTER)	bComputer = true;
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
			pView->SendMessage(WM_COMMAND, UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_TASKS,0);	// fist send to the document that sends it to the task view
		}
	}
}

void CDlgSettingsTasks::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsTasks::OnBnClickedCheckUseCondense()
{
	int iCheck;

	iCheck = m_useCondense.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryCondenseUse, iCheck);

	m_bUseCondense = (iCheck == TRUE);
}

void CDlgSettingsTasks::OnBnClickedCheckRatioLongTerm()
{
	int iCheck;

	iCheck = m_checkRatioLongTerm.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryRatioLongTerm, iCheck);

	m_bRatioLongTerm = (iCheck == TRUE);
}

void CDlgSettingsTasks::OnBnClickedCheckUserFriendly()
{
	int iCheck;

	iCheck = m_checkUserFriendly.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryUserFriendly, iCheck);

	m_bUserFriendly = (iCheck == TRUE);

	SetUserFriendly();	// send all threads
}

void CDlgSettingsTasks::OnBnClickedCheckDeadlineRemaining()
{
	int iCheck;

	iCheck = m_checkDeadlineShowRemaining.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registryDeadlineShowRemaining, iCheck);

	m_bDeadlineShowRemaining = (iCheck == TRUE);
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn0()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT].GetCheck();
	SetColumn(COLUMN_PROJECT, 1);
	SetColumnHide(COLUMN_PROJECT, iCheck);
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn1()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_APPLICATION].GetCheck();
	SetColumn(COLUMN_APPLICATION, 1);
	SetColumnHide(COLUMN_APPLICATION, iCheck);
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn2()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_NAME].GetCheck();
	SetColumn(COLUMN_NAME, 1);
	SetColumnHide(COLUMN_NAME, iCheck);
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn3()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_ELAPSED].GetCheck();
	SetColumn(COLUMN_ELAPSED, iCheck);
	SetColumnHide(COLUMN_ELAPSED, iCheck);
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn4()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_CPU_PERCENTAGE].GetCheck();
	SetColumn(COLUMN_CPU_PERCENTAGE, iCheck);
	SetColumnHide(COLUMN_CPU_PERCENTAGE, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn5()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_PERCENTAGE].GetCheck();
	SetColumn(COLUMN_PERCENTAGE, iCheck);
	SetColumnHide(COLUMN_PERCENTAGE, iCheck);
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn6()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_TIME_LEFT].GetCheck();
	SetColumn(COLUMN_TIME_LEFT, iCheck);
	SetColumnHide(COLUMN_TIME_LEFT, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn7()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_DEADLINE].GetCheck();
	SetColumn(COLUMN_DEADLINE, iCheck);
	SetColumnHide(COLUMN_DEADLINE, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn8()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_USE].GetCheck();
	SetColumn(COLUMN_USE, iCheck);
	SetColumnHide(COLUMN_USE, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn9()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_STATE].GetCheck();
	SetColumn(COLUMN_STATE, iCheck);
	SetColumnHide(COLUMN_STATE, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn10()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_TTHROTTLE].GetCheck();
	SetColumn(COLUMN_TTHROTTLE, iCheck);
	SetColumnHide(COLUMN_TTHROTTLE, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn11()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_TEMPERATURE].GetCheck();
	SetColumn(COLUMN_TEMPERATURE, iCheck);
	SetColumnHide(COLUMN_TEMPERATURE, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn12()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_COMPUTER].GetCheck();
	SetColumn(COLUMN_COMPUTER, 1);
	SetColumnHide(COLUMN_COMPUTER, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn13()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_CHECKPOINT].GetCheck();
	SetColumn(COLUMN_CHECKPOINT, iCheck);
	SetColumnHide(COLUMN_CHECKPOINT, iCheck);

	m_bCheckpoint = iCheck != 0;
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn14()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_DEBT].GetCheck();
	SetColumn(COLUMN_DEBT, iCheck);
	SetColumnHide(COLUMN_DEBT, iCheck);
}


void CDlgSettingsTasks::OnBnClickedCheckTasksColumn15()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_RECEIVED].GetCheck();
	SetColumn(COLUMN_RECEIVED, iCheck);
	SetColumnHide(COLUMN_RECEIVED, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn16()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_ACCOUNT].GetCheck();
	SetColumn(COLUMN_ACCOUNT, iCheck);
	SetColumnHide(COLUMN_ACCOUNT, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn17()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_VIRTUAL_MEMORY].GetCheck();
	SetColumn(COLUMN_VIRTUAL_MEMORY, iCheck);
	SetColumnHide(COLUMN_VIRTUAL_MEMORY, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn18()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_MEMORY].GetCheck();
	SetColumn(COLUMN_MEMORY, iCheck);
	SetColumnHide(COLUMN_MEMORY, iCheck);
}

void CDlgSettingsTasks::OnBnClickedCheckTasksColumn19()
{
	int iCheck;
	iCheck = m_checkShow[COLUMN_SHARE].GetCheck();
	SetColumn(COLUMN_SHARE, iCheck);
	SetColumnHide(COLUMN_SHARE, iCheck);
}

void CDlgSettingsTasks::OnBnClickedButtonTasksUp()
{
	int iItems, iSelected;
	CString sItemTxt;

	iItems = m_listTasksStatusSorting.GetCount();
	iSelected = m_listTasksStatusSorting.GetCurSel();

	if (iSelected > 0)
	{
		m_listTasksStatusSorting.GetText(iSelected, sItemTxt);
		m_listTasksStatusSorting.DeleteString(iSelected);
		iSelected--;
		m_listTasksStatusSorting.InsertString(iSelected,sItemTxt);
		m_listTasksStatusSorting.SetCurSel(iSelected);
	}
	ReadStatusList();
	WriteStatusListRegistry();
	SendStatusList();
}

void CDlgSettingsTasks::OnBnClickedButtonTasksDown()
{
	int iItems, iSelected;
	CString sItemTxt;

	iItems = m_listTasksStatusSorting.GetCount();
	iSelected = m_listTasksStatusSorting.GetCurSel();

	if (iSelected < iItems-1)
	{
		m_listTasksStatusSorting.GetText(iSelected, sItemTxt);
		m_listTasksStatusSorting.DeleteString(iSelected);
		iSelected++;
		m_listTasksStatusSorting.InsertString(iSelected,sItemTxt);
		m_listTasksStatusSorting.SetCurSel(iSelected);
	}
	ReadStatusList();
	WriteStatusListRegistry();
	SendStatusList();
}

void CDlgSettingsTasks::OnLbnSelchangeListTasksStatusSorting()
{
	m_buttonTasksUp.ShowWindow(SW_SHOW);
	m_buttonTasksDown.ShowWindow(SW_SHOW);
}


void CDlgSettingsTasks::OnBnClickedTasksDefaultSorting()
{
	DefaultStatusList();
	FillStatusList();
	SendStatusList();
	m_buttonTasksUp.ShowWindow(SW_HIDE);
	m_buttonTasksDown.ShowWindow(SW_HIDE);
}

void CDlgSettingsTasks::OnBnClickedCheckSortApplicationNr()
{
	int iCheck;

	iCheck = m_checkSortOnNr.GetCheck();
	theApp.WriteProfileInt(registrySectionSettings, registrySortApplicationOnNr, iCheck);

	m_bSortOnApplicationNr = (iCheck == TRUE);
}

void CDlgSettingsTasks::OnEnChangeEditTasksMaxUpdate()
{
	CString sTxt;

	m_editMaxUpdate.GetWindowText(sTxt);

	m_iMaxUpdateTime = atoi(sTxt);
	if (m_iMaxUpdateTime < 1) m_iMaxUpdateTime = 1;
	if (m_iMaxUpdateTime > 240) m_iMaxUpdateTime = 240;

	theApp.WriteProfileInt(registrySectionSettings, registryTasksyMaxUpdateTime, m_iMaxUpdateTime);
}

void CDlgSettingsTasks::OnEnChangeEditTasksDigitsCpu()
{
	CString sTxt;

	m_editCpuDigits.GetWindowText(sTxt);

	m_iCpuDigits = atoi(sTxt);
	if (m_iCpuDigits < 1) m_iCpuDigits = 0;
	if (m_iCpuDigits > 9) m_iCpuDigits = 9;

	theApp.WriteProfileInt(registrySectionSettings, registryTasksCpuDigits, m_iCpuDigits);
}


void CDlgSettingsTasks::OnEnChangeEditTasksDigitsProgress()
{
	CString sTxt;

	m_editProgressDigits.GetWindowText(sTxt);

	m_iProgressDigits = atoi(sTxt);
	if (m_iProgressDigits < 1) m_iProgressDigits = 0;
	if (m_iProgressDigits > 9) m_iProgressDigits = 9;

	theApp.WriteProfileInt(registrySectionSettings, registryTasksProgressDigits, m_iProgressDigits);
}

BOOL CDlgSettingsTasks::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsTasks::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#tasks";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

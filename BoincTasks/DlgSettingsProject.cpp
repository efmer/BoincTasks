// DlgSettingsProject.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"	
#include "TemplateRemoteProjectView.h"
#include "RemoteCombinedDoc.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsProject.h"
#include "Translation.h"
#include "afxdialogex.h"


// CDlgSettingsProject dialog

IMPLEMENT_DYNAMIC(CDlgSettingsProject, CPropertyPage)

CDlgSettingsProject::CDlgSettingsProject()
	: CPropertyPage(CDlgSettingsProject::IDD)
{

}

CDlgSettingsProject::~CDlgSettingsProject()
{
}

void CDlgSettingsProject::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDAPPLY, m_apply);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_TEXT_TASKS_SEQUENCE, m_textGroupShow);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_0, m_checkShow[COLUMN_PROJECT_PROJECT]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_1, m_checkShow[COLUMN_PROJECT_ACCOUNT]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_2, m_checkShow[COLUMN_PROJECT_TEAM]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_3, m_checkShow[COLUMN_PROJECT_USER_CREDITS]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_4, m_checkShow[COLUMN_PROJECT_USER_A_CREDITS]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_5, m_checkShow[COLUMN_PROJECT_HOST_CREDITS]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_6, m_checkShow[COLUMN_PROJECT_HOST_A_CREDITS]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_7, m_checkShow[COLUMN_PROJECT_SHARE]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_8, m_checkShow[COLUMN_PROJECT_TASKS]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_9, m_checkShow[COLUMN_PROJECT_TIME_LEFT]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_10, m_checkShow[COLUMN_PROJECT_STATUS]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_11, m_checkShow[COLUMN_PROJECT_COMPUTER]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_12, m_checkShow[COLUMN_PROJECT_VENUE]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_13, m_checkShow[COLUMN_PROJECT_TASKS_DAY]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_14, m_checkShow[COLUMN_PROJECT_TASKS_WEEK]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_15, m_checkShow[COLUMN_PROJECT_RANK]);
	DDX_Control(pDX, IDC_CHECK_PROJECTS_COLUMN_16, m_checkShow[COLUMN_PROJECT_TEAM_RANK]);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}

BOOL CDlgSettingsProject::OnInitDialog()
{
	CPropertySheet *pSheet;

	// translations
	CPropertyPage::OnInitDialog();
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsProjectsTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_checkShow[COLUMN_PROJECT_PROJECT].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectProject]);
	m_checkShow[COLUMN_PROJECT_ACCOUNT].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectAccount]);
	m_checkShow[COLUMN_PROJECT_TEAM].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectTeam]);
	m_checkShow[COLUMN_PROJECT_USER_CREDITS].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectUserCredits]);
	m_checkShow[COLUMN_PROJECT_USER_A_CREDITS].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectUserACredits]);
	m_checkShow[COLUMN_PROJECT_HOST_CREDITS].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectHostCredits]);
	m_checkShow[COLUMN_PROJECT_HOST_A_CREDITS].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectHostACredits]);
	m_checkShow[COLUMN_PROJECT_SHARE].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectShare]);
	m_checkShow[COLUMN_PROJECT_TASKS].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectTasks]);
	m_checkShow[COLUMN_PROJECT_TIME_LEFT].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectTimeLeft]);
	m_checkShow[COLUMN_PROJECT_STATUS].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectStatus]);
	m_checkShow[COLUMN_PROJECT_COMPUTER].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectComputer]);
	m_checkShow[COLUMN_PROJECT_VENUE].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectVenue]);

	m_checkShow[COLUMN_PROJECT_TASKS_DAY].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectTasksDay]);
	m_checkShow[COLUMN_PROJECT_TASKS_WEEK].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectTasksWeek]);
	m_checkShow[COLUMN_PROJECT_RANK].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectRank]);
	m_checkShow[COLUMN_PROJECT_TEAM_RANK].SetWindowText(gszTranslation[PosGroupDialogSettingsProjectTeamRank]);

	m_textGroupShow.SetWindowText(gszTranslation[PosGroupDialogSettingsProjectsGroupName]);
	m_apply.SetWindowText(gszTranslation[PosDialogCommonButtonsApply]);

	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	ReadColumsFromRegistry();

	m_apply.EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsProject::ReadColumsFromRegistry()
{
	int		iNr;

	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn0, COLUMN_PROJECT_PROJECT+1);		m_iProjectsColumnIn[0] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn1, COLUMN_PROJECT_ACCOUNT+1);		m_iProjectsColumnIn[1] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn2, COLUMN_PROJECT_TEAM+1);			m_iProjectsColumnIn[2] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn3, COLUMN_PROJECT_USER_CREDITS+1);	m_iProjectsColumnIn[3] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn4, COLUMN_PROJECT_USER_A_CREDITS+1);m_iProjectsColumnIn[4] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn5, COLUMN_PROJECT_HOST_CREDITS+1);	m_iProjectsColumnIn[5] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn6, COLUMN_PROJECT_HOST_A_CREDITS+1);m_iProjectsColumnIn[6] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn7, COLUMN_PROJECT_SHARE+1);			m_iProjectsColumnIn[7] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn8,	COLUMN_PROJECT_TASKS+1);		m_iProjectsColumnIn[8] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn9, COLUMN_PROJECT_TIME_LEFT+1);		m_iProjectsColumnIn[9] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn10, COLUMN_PROJECT_STATUS+1);		m_iProjectsColumnIn[10] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn11, COLUMN_PROJECT_COMPUTER+1);		m_iProjectsColumnIn[11] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn12, 0);		m_iProjectsColumnIn[12] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn13, 0);		m_iProjectsColumnIn[13] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn14, 0);		m_iProjectsColumnIn[14] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn15, 0);		m_iProjectsColumnIn[15] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryProjectsColumn16, 0);		m_iProjectsColumnIn[16] = iNr;
	CheckIntegrity();
	ProjectsColumns();
	EnableCheck();
}

void CDlgSettingsProject::WriteColumsToRegistry()
{
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn0, m_iProjectsColumnIn[0]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn1, m_iProjectsColumnIn[1]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn2, m_iProjectsColumnIn[2]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn3, m_iProjectsColumnIn[3]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn4, m_iProjectsColumnIn[4]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn5, m_iProjectsColumnIn[5]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn6, m_iProjectsColumnIn[6]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn7, m_iProjectsColumnIn[7]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn8, m_iProjectsColumnIn[8]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn9, m_iProjectsColumnIn[9]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn10, m_iProjectsColumnIn[10]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn11, m_iProjectsColumnIn[11]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn12, m_iProjectsColumnIn[12]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn13, m_iProjectsColumnIn[13]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn14, m_iProjectsColumnIn[14]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn15, m_iProjectsColumnIn[15]);
	theApp.WriteProfileInt(registrySectionColumn, registryProjectsColumn16, m_iProjectsColumnIn[16]);
}

void CDlgSettingsProject::CheckIntegrity()
{
	bool bEqual = false;

	for (int iCount1 = 0; iCount1 < NUM_REMOTE_PROJECT_COLUMNS; iCount1++)
	{
		for (int iCount2 = iCount1+1; iCount2 < NUM_REMOTE_PROJECT_COLUMNS; iCount2++)
		{
			if (m_iProjectsColumnIn[iCount1] == m_iProjectsColumnIn[iCount2])
			{
				if (m_iProjectsColumnIn[iCount1] > 0)
				{
					bEqual = true;
				}
			}
		}
	}
	if (!bEqual) return;

	for (int iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		m_iProjectsColumnIn[iCount] = 0;
	}

	m_iProjectsColumnIn[0] = COLUMN_PROJECT_PROJECT+1;
	m_iProjectsColumnIn[1] = COLUMN_PROJECT_ACCOUNT+1;
	m_iProjectsColumnIn[2] = COLUMN_PROJECT_TEAM+1;
	m_iProjectsColumnIn[3] = COLUMN_PROJECT_USER_CREDITS+1;
	m_iProjectsColumnIn[4] = COLUMN_PROJECT_USER_A_CREDITS+1;
	m_iProjectsColumnIn[5] = COLUMN_PROJECT_HOST_CREDITS+1;
	m_iProjectsColumnIn[6] = COLUMN_PROJECT_HOST_A_CREDITS+1;
	m_iProjectsColumnIn[7] = COLUMN_PROJECT_SHARE+1;
	m_iProjectsColumnIn[8] = COLUMN_PROJECT_TASKS+1;
	m_iProjectsColumnIn[9] = COLUMN_PROJECT_TIME_LEFT+1;
	m_iProjectsColumnIn[10] = COLUMN_PROJECT_STATUS+1;
	m_iProjectsColumnIn[11] = COLUMN_PROJECT_COMPUTER+1;
	WriteColumsToRegistry();
}

void CDlgSettingsProject::ProjectsColumns()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		m_iProjectsColumnLookup[iCount] = -1;
		m_iProjectsColumn[iCount] = -1;
	}
	m_iActiveColumns = 0;
	for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		if (m_iProjectsColumnIn[iCount] > 0)
		{
			if (m_iProjectsColumnIn[iCount] <= NUM_REMOTE_PROJECT_COLUMNS)
			{
				m_iProjectsColumn[m_iActiveColumns] = m_iProjectsColumnIn[iCount] -1;
				m_iActiveColumns++;
			}
		}
	}	
	for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		iNr = m_iProjectsColumn[iCount];
		if (iNr >=0) m_iProjectsColumnLookup[iNr] = iCount;
	}
}

void CDlgSettingsProject::EnableCheck()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		iNr = m_iProjectsColumn[iCount];
		if (iNr < 0) return;
		if (iNr > NUM_REMOTE_PROJECT_COLUMNS) return;
	
		m_checkShow[iNr].SetCheck(TRUE);
	}
}

void CDlgSettingsProject::SetColumn(int iColumn, int iCheck)
{
	int		iCount;

	if (iCheck)
	{
		for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
		{
			// Check if already there
			if (m_iProjectsColumnIn[iCount] == iColumn+1)
			{
				return;
			}

			if (m_iProjectsColumnIn[iCount] <= 0)
			{
				m_iProjectsColumnIn[iCount] = iColumn+1;
				m_apply.EnableWindow(TRUE);
				break;
			}
		}
	}
	else
	{
		for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
		{
			if (m_iProjectsColumnIn[iCount] == iColumn+1)
			{
				m_iProjectsColumnIn[iCount] = 0;
				m_apply.EnableWindow(TRUE);
			}
		}
	}
	WriteColumsToRegistry();
	ProjectsColumns();
}

void CDlgSettingsProject::SetColumnHide(int iColumn, int iCheck)
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
				pView->PostMessage(UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH,iColumn,iCheck);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CDlgSettingsProject, CPropertyPage)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_GET, OnSettingsProjectsGet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET, OnSettingsProjectsSet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectsWidth) 

	ON_BN_CLICKED(IDOK, &CDlgSettingsProject::OnBnClickedOk)
	ON_BN_CLICKED(IDAPPLY, &CDlgSettingsProject::OnBnClickedApply)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_0, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn0)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_1, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn1)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_2, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn2)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_3, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn3)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_4, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn4)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_5, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn5)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_6, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn6)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_7, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn7)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_8, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn8)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_9, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn9)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_10, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn10)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_11, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn11)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_12, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn12)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_13, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn13)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_14, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn14)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_15, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn15)
	ON_BN_CLICKED(IDC_CHECK_PROJECTS_COLUMN_16, &CDlgSettingsProject::OnBnClickedCheckProjectsColumn16)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsProject::OnBnClickedHelp)

END_MESSAGE_MAP()

LRESULT CDlgSettingsProject::OnSettingsProjectsGet(WPARAM parm1, LPARAM parm2)
{
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		*pArrayColumn = m_iProjectsColumn[iCount];
		pArrayColumn++;
		*pArrayLookup = m_iProjectsColumnLookup[iCount];
		*pArrayLookup++;
	}

	return m_iActiveColumns;
}

LRESULT CDlgSettingsProject::OnSettingsProjectsSet(WPARAM parm1, LPARAM parm2)
{
//	int iNr;
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		m_iProjectsColumnIn[iCount] = (*pArrayColumn)+1;
		pArrayColumn++;
	}
	WriteColumsToRegistry();
	ProjectsColumns();

	return 0;
}

LRESULT CDlgSettingsProject::OnSettingsProjectsWidth(WPARAM parm1, LPARAM parm2)
{
	int iColumn, iWidth;
	int	iNr;

	iColumn = (int) parm1;
	iWidth = (int) parm2;

	iNr = m_iProjectsColumn[iColumn];
	if (iNr < 0) return 0;
	if (iNr > NUM_REMOTE_PROJECT_COLUMNS) return 0;
	if (iWidth > 0)	m_checkShow[iNr].SetCheck(TRUE);
	else			m_checkShow[iNr].SetCheck(FALSE);

	return 0;
}

void CDlgSettingsProject::OnBnClickedApply()
{
	POSITION posDoc, posView ;
	CRemoteCombinedDoc *pDocCombined;
	CView *pView;

	bool	bProject = false, bName = false , bComputer = false;

	m_apply.EnableWindow(FALSE);
	ReadColumsFromRegistry();

	for (int iCount = 0; iCount < m_iActiveColumns; iCount++)
	{
		if (m_iProjectsColumn[iCount] == COLUMN_PROJECT_PROJECT)	bProject = true;
		if (m_iProjectsColumn[iCount] == COLUMN_PROJECT_COMPUTER)	bComputer = true;
	}

	if (bProject && bComputer)
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
			pView->SendMessage(WM_COMMAND, UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_PROJECTS,0);	// fist send to the document that sends it to the view
		}
	}
}

void CDlgSettingsProject::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn0()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_PROJECT].GetCheck();
//	SetColumn(COLUMN_PROJECT_PROJECT, iCheck);
	SetColumn(COLUMN_PROJECT_PROJECT, 1);
	SetColumnHide(COLUMN_PROJECT_PROJECT, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn1()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_ACCOUNT].GetCheck();
	SetColumn(COLUMN_PROJECT_ACCOUNT, iCheck);
	SetColumnHide(COLUMN_PROJECT_ACCOUNT, iCheck);
}


void CDlgSettingsProject::OnBnClickedCheckProjectsColumn2()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_TEAM].GetCheck();
	SetColumn(COLUMN_PROJECT_TEAM, iCheck);
	SetColumnHide(COLUMN_PROJECT_TEAM, iCheck);
}


void CDlgSettingsProject::OnBnClickedCheckProjectsColumn3()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_USER_CREDITS].GetCheck();
	SetColumn(COLUMN_PROJECT_USER_CREDITS, iCheck);
	SetColumnHide(COLUMN_PROJECT_USER_CREDITS, iCheck);
}


void CDlgSettingsProject::OnBnClickedCheckProjectsColumn4()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_USER_A_CREDITS].GetCheck();
	SetColumn(COLUMN_PROJECT_USER_A_CREDITS, iCheck);
	SetColumnHide(COLUMN_PROJECT_USER_A_CREDITS, iCheck);
}


void CDlgSettingsProject::OnBnClickedCheckProjectsColumn5()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_HOST_CREDITS].GetCheck();
	SetColumn(COLUMN_PROJECT_HOST_CREDITS, iCheck);
	SetColumnHide(COLUMN_PROJECT_HOST_CREDITS, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn6()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_HOST_A_CREDITS].GetCheck();
	SetColumn(COLUMN_PROJECT_HOST_A_CREDITS, iCheck);
	SetColumnHide(COLUMN_PROJECT_HOST_A_CREDITS, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn7()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_SHARE].GetCheck();
	SetColumn(COLUMN_PROJECT_SHARE, iCheck);
	SetColumnHide(COLUMN_PROJECT_SHARE, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn8()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_TASKS].GetCheck();
	SetColumn(COLUMN_PROJECT_TASKS, iCheck);
	SetColumnHide(COLUMN_PROJECT_TASKS, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn9()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_TIME_LEFT].GetCheck();
	SetColumn(COLUMN_PROJECT_TIME_LEFT, iCheck);
	SetColumnHide(COLUMN_PROJECT_TIME_LEFT, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn10()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_STATUS].GetCheck();
	SetColumn(COLUMN_PROJECT_STATUS, iCheck);
	SetColumnHide(COLUMN_PROJECT_STATUS, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn11()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_COMPUTER].GetCheck();
	SetColumn(COLUMN_PROJECT_COMPUTER, 1);
	SetColumnHide(COLUMN_PROJECT_COMPUTER, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn12()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_VENUE].GetCheck();
	SetColumn(COLUMN_PROJECT_VENUE, iCheck);
	SetColumnHide(COLUMN_PROJECT_VENUE, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn13()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_TASKS_DAY].GetCheck();
	SetColumn(COLUMN_PROJECT_TASKS_DAY, iCheck);
	SetColumnHide(COLUMN_PROJECT_TASKS_DAY, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn14()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_TASKS_WEEK].GetCheck();
	SetColumn(COLUMN_PROJECT_TASKS_WEEK, iCheck);
	SetColumnHide(COLUMN_PROJECT_TASKS_WEEK, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn15()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_RANK].GetCheck();
	SetColumn(COLUMN_PROJECT_RANK, iCheck);
	SetColumnHide(COLUMN_PROJECT_RANK, iCheck);
}

void CDlgSettingsProject::OnBnClickedCheckProjectsColumn16()
{
	int iCheck;

	iCheck = m_checkShow[COLUMN_PROJECT_TEAM_RANK].GetCheck();
	SetColumn(COLUMN_PROJECT_TEAM_RANK, iCheck);
	SetColumnHide(COLUMN_PROJECT_TEAM_RANK, iCheck);
}

BOOL CDlgSettingsProject::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsProject::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#projects";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}







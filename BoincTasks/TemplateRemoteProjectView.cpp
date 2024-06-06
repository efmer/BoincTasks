// ComputerRemoteProjectsView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"   // base class for BoinCComputerView
#include "TemplateRemoteProjectView.h"
#include "RemoteCombinedDoc.h"
#include "History.h"
#include "threadrpc.h"
#include "DlgColorMain.h"
#include "DlgColorProjects.h"
#include "DlgColorWarnings.h"
#include "DlgSettingsView.h"
#include "DlgSettingsWarning.h"
#include "DlgSettingsProject.h"
#include "DlgProperties.h"
#include "DlgGraphicTasks.h"
#include "SortColumn.h"
#include "Selection.h"
#include "NumberFormat.h"
#include "TimeString.h"
#include "DateConvert.h"
#include "Xml.h"
#include "CreateHtmlWebServerProperties.h"
#include "MainFrm.h"
#include "Translation.h"

// CComputerRemoteProjectsView

IMPLEMENT_DYNCREATE(CTemplateRemoteProjectView, CListViewEx)

static int _gnRemoteProjectColumnWidth[NUM_REMOTE_PROJECT_COLUMNS] =
{
	140,120,150,100,80,100,80,120,60,100,200,100,100,100,100,100,100
};

CTemplateRemoteProjectView::CTemplateRemoteProjectView()
{
	CRpcInfoProjectItem *pRpcInfoProjectItem;
	CComputerProjectListItem *pComputerProjectListItem;
	CHistoryCountItem *pHistoryCountItem;

	this->m_bSelection = FALSE;	// disable default selection

	m_dataDummy.m_color = RGB(255,255,255);
	m_dataDummy.m_cSortPrimary = "";
	m_dataDummy.m_cSortSecondary = "";
	m_dataDummy.m_cSortTertiary = "";

	m_pSortColumn = new CSortColumn;
	m_pSelection = new CSelection;

	m_iProjectPropertiesBusy = PROPERTIES_READY;
	m_bApplyChangesOnColumn = false;

	m_bFirstTime	= true;
	m_bDraged		= false;

	m_tIntervalTasksCount = 0;
	m_tLastTasksCount = 0;
	m_tNextStatsReadWeb = theApp.GetProfileInt(registrySectionGeneral, registryReadStatsTime, 0);
	m_tNextStatsReadWeb*= 3600;
	m_iNextStatsReadInterval = 0;
	m_iThreadStatRequest = 0;
	m_bThreadStatRequestError = false;
	m_bUseStats = false;

	m_iNrColomns = 0;

	m_iTimerWu		= 0;

	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pComputerProjectListItem = new CComputerProjectListItem;
		pRpcInfoProjectItem = new CRpcInfoProjectItem;
		pHistoryCountItem = new CHistoryCountItem;

		m_lProjectItem.push_back(pComputerProjectListItem);
		m_lRpcInfo.push_back(pRpcInfoProjectItem);
		m_lHistoryCount.push_back(pHistoryCountItem);
	}

	CComputerGraphicTasksListItem *pComputerGraphicTasksListItem;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pComputerGraphicTasksListItem = new CComputerGraphicTasksListItem;
		pComputerGraphicTasksListItem->m_pcThreadRpcId = NULL;
		pComputerGraphicTasksListItem->m_pThreadRpc = NULL;
		pComputerGraphicTasksListItem->m_plHistoryProjectTasks = new std::deque<CHistoryProjectTasksItem*>;
		m_lComputer.push_back(pComputerGraphicTasksListItem);
	}

}

CTemplateRemoteProjectView::~CTemplateRemoteProjectView()
{
	for (int iCount = 0; iCount < (int) m_lListData.size(); iCount++)
	{
		delete m_lListData.at(iCount);
	}
//	m_lListData.clear();

	for (int iCount = 0; iCount < (int) m_lRpcInfo.size(); iCount++)
	{
		delete m_lRpcInfo.at(iCount);
	}
//	m_lRpcInfo.clear();

	for (int iCount = 0; iCount < (int) m_lProjectItem.size(); iCount++)
	{
		delete m_lProjectItem.at(iCount);
	}
//	m_lProjectItem.clear();

	for (int iCount = 0; iCount < (int) m_lHistoryCount.size(); iCount++)
	{
		delete m_lHistoryCount.at(iCount);
	}
//	m_lHistoryCount.clear();

	DeleteProjectProperties();
	DeleteProjectTasks();
	DeleteStatsList();

	delete m_pSortColumn;
	delete m_pSelection;
}

void CTemplateRemoteProjectView::DeleteProjectProperties()
{
	for (int iPos = 0; iPos < (int) m_lProjectProperties.size(); iPos++)
	{
		delete m_lProjectProperties.at(iPos);
	}
	m_lProjectProperties.clear();
}

void CTemplateRemoteProjectView::DeleteProjectTasks()
{
	for (int iCount = 0; iCount < (int) m_lComputer.size(); iCount++)
	{
		for (int iCount2 = 0; iCount2 < (int) m_lComputer.at(iCount)->m_plHistoryProjectTasks->size(); iCount2++)
		{
			delete m_lComputer.at(iCount)->m_plHistoryProjectTasks->at(iCount2);
		}
		delete m_lComputer.at(iCount)->m_plHistoryProjectTasks;
		delete m_lComputer.at(iCount);
	}
}

void CTemplateRemoteProjectView::DeleteStatsList()
{
	for (int iPos = 0; iPos < (int) m_lListStats.size(); iPos++)
	{
		delete m_lListStats.at(iPos);
	}
	m_lListStats.clear();
	for (int iPos = 0; iPos < (int) m_lListUrlAlias.size(); iPos++)
	{
		delete m_lListUrlAlias.at(iPos);
	}
	m_lListUrlAlias.clear();
}

void CTemplateRemoteProjectView::OnInitialUpdate()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetExtendedStyle (listCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP);
	AddAllColumns();

	CListViewEx::OnInitialUpdate();
}

void CTemplateRemoteProjectView::AddAllColumns(bool bAdd)
{
	int			iCount, iColumn, iWidth, iFormat, iNr;
	int			iColumnHeader[NUM_REMOTE_PROJECT_COLUMNS+1];
	LV_COLUMN	lvc;
	CString		sTranslation;

	// read back the column sequence

	if (bAdd)
	{
		m_iNrColomns = (int) ::SendMessage(theApp.m_pDlgSettingsProjects->m_hWnd, UWM_MSG_DLG_SETTINGS_PROJECTS_GET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}
	else
	{	
		for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
		{
			iColumnHeader[iCount] = -1;
		}

		for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
		{
			iNr = m_iColumnOrder[iCount];
			if (iNr > NUM_REMOTE_PROJECT_COLUMNS) break;
			if (iNr >=0) iColumnHeader[iNr] = iCount;
		}
		::SendMessage(theApp.m_pDlgSettingsProjects->m_hWnd, UWM_MSG_DLG_SETTINGS_PROJECTS_SET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}

	CListCtrl& listCtrl = GetListCtrl();
	// build columns
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(iCount = 0; iCount<m_iNrColomns; iCount++)
	{
		iColumn = iColumnHeader[iCount];
		if (iColumn >= 0)
		{
			lvc.iSubItem = iCount;
			sTranslation = gszTranslation[PosViewProjecsRowProject+iColumn];
			lvc.pszText = sTranslation.GetBuffer(); sTranslation.ReleaseBuffer();
			iWidth = ReadColumnWidth(iColumn, _gnRemoteProjectColumnWidth[iColumn]);
			m_iColumnWidth[iColumn] = iWidth;
			lvc.cx = m_iColumnWidth[iColumn];
			::SendMessage(theApp.m_pDlgSettingsProjects->m_hWnd, UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, (WPARAM) iCount,(LPARAM) iWidth);

			switch(iColumn)
			{
				case COLUMN_PROJECT_USER_CREDITS:
				case COLUMN_PROJECT_USER_A_CREDITS:
				case COLUMN_PROJECT_HOST_CREDITS:
				case COLUMN_PROJECT_HOST_A_CREDITS:
				case COLUMN_PROJECT_TASKS:
				case COLUMN_PROJECT_TIME_LEFT:
				case COLUMN_PROJECT_TASKS_DAY:
				case COLUMN_PROJECT_TASKS_WEEK:
				case COLUMN_PROJECT_RANK:
				case COLUMN_PROJECT_TEAM_RANK:
					iFormat = LVCFMT_RIGHT;
				break;
				case COLUMN_PROJECT_SHARE:
					iFormat = LVCFMT_CENTER;
				break;

				default:
					iFormat = LVCFMT_LEFT;
			}

			lvc.fmt = iFormat;
			if (bAdd)
			{
				listCtrl.InsertColumn(iCount,&lvc);
//				listCtrl.GetColumn(iCount,&lvc);
//				int ii = 1;
			}
			else
			{
				listCtrl.SetColumn(iCount, &lvc);
			}

		}
	}
	LPINT pnOrder = (LPINT) malloc(m_iNrColomns*sizeof(int));
	if (pnOrder != NULL)
	{
		for (iCount = 0; iCount < m_iNrColomns; iCount++)
		{
			pnOrder[iCount] = iCount; 
		}
		listCtrl.SetColumnOrderArray(m_iNrColomns,pnOrder);
		free(pnOrder);
	}

	m_pSortColumn->SetColumnOrder(m_iNrColomns, &iColumnHeader[0]);
	m_pSortColumn->Setup(&listCtrl,this->m_hWnd, m_iNrColomns, registrySortProjectPrimary, registrySortProjectSecondary, registrySortProjectTertiary, 256, 265, 265 );	// projects, computer, computer

	m_pSelection->Setup(&listCtrl,  &m_lListData, COLUMN_PROJECT_PROJECT, COLUMN_PROJECT_COMPUTER, NUM_REMOTE_PROJECT_COLUMNS, &m_iColumnOrder[0]);

	m_bUseStats = false;
	if (m_iColumnOrder[COLUMN_PROJECT_RANK] > 0) m_bUseStats = true;
	if (m_iColumnOrder[COLUMN_PROJECT_TEAM_RANK] > 0) m_bUseStats = true;

	PostMessage(UWM_MSG_THREAD_START_STATS, 0,0);
}

void CTemplateRemoteProjectView::ReadAllColumnsWidth()
{
	CListCtrl& listCtrl = GetListCtrl();
	int iWidth;

	for(int iCount = 0; iCount<=NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		iWidth = ReadColumnWidth(iCount, _gnRemoteProjectColumnWidth[iCount]);
		listCtrl.SetColumnWidth(m_iColumnOrder[iCount], iWidth);
		m_iColumnWidth[m_iColumnOrder[iCount]] = iWidth;
	}
}

int CTemplateRemoteProjectView::ReadColumnWidth(int iColumn, int iDefault)
{
	CString sRegistry;
	char	cChar;
	int		iWidth;

	sRegistry = registryColumnProjects;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
//	if (iWidth < 20) return iDefault;
	return iWidth;
}

void CTemplateRemoteProjectView::CheckAndSaveColumnsWidth()
{
	int		iWidth, iColumn;
	bool	bChanged = false;
	char	cChar;
	CString sTxt;

	if (m_bFirstTime) return; // wait for init

	CListCtrl& listCtrl = GetListCtrl();

	for (int iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		iColumn = m_iColumnOrder[iCount];
		if (iColumn >= 0)
		{
			iWidth = listCtrl.GetColumnWidth(iColumn);
			if (m_iColumnWidth[iColumn] != listCtrl.GetColumnWidth(iColumn))
			{
				m_iColumnWidth[iColumn] = iWidth;
				bChanged = true;
				sTxt = registryColumnProjects;
				cChar = iCount + 'a';
				sTxt = sTxt + "_" + cChar; 
				// update new width in registry
				theApp.WriteProfileInt(registrySectionColumn, sTxt, iWidth);
				::SendMessage(theApp.m_pDlgSettingsProjects->m_hWnd, UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, (WPARAM) iColumn,(LPARAM) iWidth);
			}
		}
	}
}

void CTemplateRemoteProjectView::GetTasksDayWeek(char *pcComputerName, char *pcProject, int *piDayCpu, double *pdWeekCpu, int *piDayGpu, double *pdWeekGpu)
{
	std::deque<CHistoryProjectTasksItem*> *plHistoryProjectTasks;
	CComputerGraphicTasksListItem *pComputerGraphicTasksListItem;

// MAX_HISTORY_STATISTICS_LEN = now

	*piDayCpu= 0;
	*pdWeekCpu = 0;
	*piDayGpu= 0;
	*pdWeekGpu = 0;

//	comment:	m_lComputer[computer]->m_plHistoryProjectTasks[project]->m_iReceived[MAX_HISTORY_STATISTICS_LEN];

	for (int iPosComputer=0; iPosComputer < (int) m_lComputer.size(); iPosComputer++)
	{
		pComputerGraphicTasksListItem = m_lComputer.at(iPosComputer);
		if (pComputerGraphicTasksListItem->m_pcThreadRpcId == 0) continue;
		if (strcmp(pComputerGraphicTasksListItem->m_pcThreadRpcId, pcComputerName) == 0)
		{
			for (int iPosProject=0; iPosProject < (int) pComputerGraphicTasksListItem->m_plHistoryProjectTasks->size(); iPosProject++)
			{
				plHistoryProjectTasks = pComputerGraphicTasksListItem->m_plHistoryProjectTasks;
				if (strcmp(plHistoryProjectTasks->at(iPosProject)->m_pcProject, pcProject) == 0)
				{
					for (int iPosTime = MAX_HISTORY_STATISTICS_LEN; iPosTime > MAX_HISTORY_STATISTICS_LEN-24; iPosTime--)
					{
						*piDayCpu += plHistoryProjectTasks->at(iPosProject)->m_iReceivedCpu[iPosTime];
						*piDayGpu += plHistoryProjectTasks->at(iPosProject)->m_iReceivedGpu[iPosTime];
					}
					for (int iPosTime = MAX_HISTORY_STATISTICS_LEN; iPosTime > MAX_HISTORY_STATISTICS_LEN-(24*7); iPosTime--)
					{
						*pdWeekCpu += plHistoryProjectTasks->at(iPosProject)->m_iReceivedCpu[iPosTime];
						*pdWeekGpu += plHistoryProjectTasks->at(iPosProject)->m_iReceivedGpu[iPosTime];
					}
				}
			}
		}
	}
}

void CTemplateRemoteProjectView::ProcessStatsXml(CString *psStat)
{
	CString sTemp, sProjectName, sUrl, sUrlAlias;
	int iProjectRankTotalCredit, iProjectRankExpavgCredit;
	int iTeamRankTotalCredit, iTeamRankExpavgCredit;
	bool bFound, bFound1;

	if (psStat->GetLength() == 0) return;

	CXml xml("");

//	DeleteStatsList();	do not delete only add or update

	if (xml.ReadStart(psStat,"user"))
	{
		while (xml.ReadGroup("project",false, true))
		{
			bFound = false;
			xml.ReadItem("name",&sProjectName);
			xml.ReadItem("url",&sUrl);
			xml.ReadItem("project_rank_total_credit",&sTemp);
			iProjectRankTotalCredit = atoi(sTemp);
			xml.ReadItem("project_rank_expavg_credit",&sTemp);
			iProjectRankExpavgCredit = atoi(sTemp);

			xml.ReadItem("team_rank_total_credit",&sTemp);
			iTeamRankTotalCredit = atoi(sTemp);
			xml.ReadItem("team_rank_expavg_credit",&sTemp);
			iTeamRankExpavgCredit = atoi(sTemp);

			for (UINT iPos = 0; iPos < m_lListStats.size(); iPos++)
			{
				bFound1 = false;
				if (m_lListStats.at(iPos)->m_sProjectName == sProjectName || m_lListStats.at(iPos)->m_sUrl == sUrl)
				{
					bFound = true;
					m_lListStats.at(iPos)->m_iProjectRankTotalCredit = iProjectRankTotalCredit;
					m_lListStats.at(iPos)->m_iProjectRankExpavgCredit = iProjectRankExpavgCredit;
					m_lListStats.at(iPos)->m_iTeamRankTotalCredit = iTeamRankTotalCredit;
					m_lListStats.at(iPos)->m_iTeamRankExpavgCredit = iTeamRankExpavgCredit;
					break;
				}
			}

			if (!bFound)
			{
				CListStatsItem *pListStatsItem;
				pListStatsItem = new CListStatsItem;

				pListStatsItem->m_sProjectName = sProjectName;
				pListStatsItem->m_sUrl = sUrl;
				sUrl = Alias(&sUrl);
				if (sUrl != "") pListStatsItem->m_sUrl = sUrl;
				pListStatsItem->m_iProjectRankTotalCredit = iProjectRankTotalCredit;
				pListStatsItem->m_iProjectRankExpavgCredit = iProjectRankExpavgCredit;
				pListStatsItem->m_iTeamRankTotalCredit = iTeamRankTotalCredit;
				pListStatsItem->m_iTeamRankExpavgCredit = iTeamRankExpavgCredit;
				m_lListStats.push_back(pListStatsItem);
			}
		}
	}
}

CString CTemplateRemoteProjectView::Alias(CString *psUrl)
{
	if (m_lListUrlAlias.size() == 0)
	{
		CString sUrl, sUrlAlias;

		CXml xmlAlternative("stats\\stats.xml",false,true,true);
		if (xmlAlternative.ReadStart("stats"))
		{
			while (xmlAlternative.ReadGroup("project"))
			{
				xmlAlternative.ReadItem("master_url",&sUrl);
				xmlAlternative.ReadItem("stats_url",&sUrlAlias);

				CListAliasItem *pListAliasItem;
				pListAliasItem = new CListAliasItem;
				pListAliasItem->m_sUrl = sUrl;
				pListAliasItem->m_sUrlAlias = sUrlAlias;
				m_lListUrlAlias.push_back(pListAliasItem);
			}
		}
	}


	for (int iCount = 0; iCount < (int) m_lListUrlAlias.size(); iCount++)
	{
		if (m_lListUrlAlias.at(iCount)->m_sUrlAlias == *psUrl)
		{
			return m_lListUrlAlias.at(iCount)->m_sUrl;
		}
	}
	return "";
}

int CTemplateRemoteProjectView::GetRank(CString *psProject, CString *psUrl, CString *psRank)
{
	CNumberFormat fNumber;

	*psRank = "?/?";
	psProject->MakeLower();
	psProject->Remove(' ');
	psUrl->MakeLower();
	psUrl->Remove(' ');
	for (UINT iPos = 0; iPos < m_lListStats.size(); iPos++)
	{
		if (m_lListStats.at(iPos)->m_sProjectName == *psProject || m_lListStats.at(iPos)->m_sUrl == *psUrl)
		{
			*psRank = fNumber.FormatNumber(m_lListStats.at(iPos)->m_iProjectRankTotalCredit,0);
			*psRank+= "/";
			*psRank+= fNumber.FormatNumber(m_lListStats.at(iPos)->m_iProjectRankExpavgCredit,0);
			return m_lListStats.at(iPos)->m_iProjectRankTotalCredit;
		}
	}
	return -1;
}

int CTemplateRemoteProjectView::GetTeamRank(CString *psProject, CString *psUrl, CString *psRank)
{
	CNumberFormat fNumber;

	*psRank = "?/?";
	psProject->MakeLower();
	psProject->Remove(' ');
	psUrl->MakeLower();
	psUrl->Remove(' ');
	for (UINT iPos = 0; iPos < m_lListStats.size(); iPos++)
	{
		if (m_lListStats.at(iPos)->m_sProjectName == *psProject || m_lListStats.at(iPos)->m_sUrl == *psUrl)
		{
			*psRank = fNumber.FormatNumber(m_lListStats.at(iPos)->m_iTeamRankTotalCredit,0);
			*psRank+= "/";
			*psRank+= fNumber.FormatNumber(m_lListStats.at(iPos)->m_iTeamRankExpavgCredit,0);
			return m_lListStats.at(iPos)->m_iTeamRankTotalCredit;
		}
	}
	return -1;
}

BEGIN_MESSAGE_MAP(CTemplateRemoteProjectView, CListViewEx)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()

	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickList)
    ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	ON_NOTIFY_EX(HDN_ITEMCHANGED, 0, OnHeaderChanged) 

	ON_MESSAGE(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_PROJECTS,OnSettingsApplyChangesProjects)

	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_READ_PROJECT_DISK,OnProjectPropertiesReady)

	ON_MESSAGE(UWM_MSG_HISTORY_TASKS_COUNT, OnProjectHistoryCount)
END_MESSAGE_MAP()

void CTemplateRemoteProjectView::DisplaySingleItem(CListCtrl *pListCtrl, int iRow, PROJECT *pInfo, char *pcComputerName, int iComputerId)
{
	char			cBuffer[256];
	char			cBuffer2[256];
	char			*pProjectName = "", *pAccountName = "", *pcVenue = "";
//	PROJECT			info;
	bool			bOverride = false, bSelectedFound = false, bWarning = false;
	int				iColumn;
	UINT			iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary,  iSortingColumnTertiary, iSortingOptionTertiary;
	double			dShare, dPerc;
	LV_ITEM			lvi;
	time_t			time;
	struct tm		tmtime;
	CString			sStatus, sTxt;
	CListData		*pData;
	CTimeString		timeString;

	while (iRow >= (int) m_lListData.size())
	{
		pData = new CListData;
		m_lListData.push_back(pData);
	}
	pData = m_lListData.at(iRow);

	pData->m_cSortPrimary = "";		// init strings, because is one of them is invalid the sorting crashes.
	pData->m_cSortSecondary = "";
	pData->m_cSortTertiary = "";

	COLORREF	*pColorsProjects;
	pColorsProjects	= theApp.m_pDlgColorProjects->GetColorArray();

	COLORREF	*pColorsWarnings;
	pColorsWarnings = theApp.m_pDlgColorWarnings->GetColorArray();

	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);

	iColumn = m_iColumnOrder[COLUMN_PROJECT_ACCOUNT];
	if (iColumn >= 0)
	{
		pAccountName = (char *) pInfo->user_name.c_str();
		pListCtrl->SetItemText(iRow,iColumn,pAccountName);
	}
	if (iSortingColumnPrimary == COLUMN_PROJECT_ACCOUNT) pData->m_cSortPrimary = pAccountName;
	if (iSortingColumnSecondary == COLUMN_PROJECT_ACCOUNT) pData->m_cSortSecondary = pAccountName;
	if (iSortingColumnTertiary == COLUMN_PROJECT_ACCOUNT) pData->m_cSortTertiary = pAccountName;

	pProjectName = (char *) pInfo->project_name.c_str();
	if (strlen(pProjectName) == 0)
	{
		pProjectName = (char *) pInfo->master_url.c_str();
	}

	// follow the name
	pData->m_bSelected = m_pSelection->Follow(pProjectName, pcComputerName);


	iColumn = m_iColumnOrder[COLUMN_PROJECT_TEAM];
	if ( iColumn>= 0)
	{
		sTxt = pInfo->team_name.c_str();
		sTxt.Replace("&amp;","&");
		pListCtrl->SetItemText(iRow,iColumn,sTxt);	
	}
	if (iSortingColumnPrimary == COLUMN_PROJECT_TEAM) pData->m_cSortPrimary = (char *) pInfo->team_name.c_str();
	if (iSortingColumnSecondary == COLUMN_PROJECT_TEAM) pData->m_cSortSecondary = (char *) pInfo->team_name.c_str();
	if (iSortingColumnTertiary == COLUMN_PROJECT_TEAM) pData->m_cSortTertiary = (char *) pInfo->team_name.c_str();

	CString sNr;
	iColumn = m_iColumnOrder[COLUMN_PROJECT_USER_CREDITS];
	if ( iColumn>= 0)
	{
		sNr = FormatNumber(pInfo->user_total_credit);
		pListCtrl->SetItemText(iRow,iColumn,sNr);
	}
	if (iSortingColumnPrimary == COLUMN_PROJECT_USER_CREDITS) pData->m_fSortPrimary = (float) pInfo->user_total_credit;
	if (iSortingColumnSecondary == COLUMN_PROJECT_USER_CREDITS) pData->m_fSortSecondary = (float) pInfo->user_total_credit;
	if (iSortingColumnTertiary == COLUMN_PROJECT_USER_CREDITS) pData->m_fSortTertiary = (float) pInfo->user_total_credit;

	iColumn = m_iColumnOrder[COLUMN_PROJECT_USER_A_CREDITS];
	if ( iColumn>= 0)
	{
		sNr = FormatNumber(pInfo->user_expavg_credit, 2);
		pListCtrl->SetItemText(iRow,iColumn,sNr);
	}
	if (iSortingColumnPrimary == COLUMN_PROJECT_USER_A_CREDITS) pData->m_fSortPrimary = (float) pInfo->user_expavg_credit;
	if (iSortingColumnSecondary == COLUMN_PROJECT_USER_A_CREDITS) pData->m_fSortSecondary = (float) pInfo->user_expavg_credit;
	if (iSortingColumnTertiary == COLUMN_PROJECT_USER_A_CREDITS) pData->m_fSortTertiary = (float) pInfo->user_expavg_credit;

	iColumn = m_iColumnOrder[COLUMN_PROJECT_HOST_CREDITS];
	if ( iColumn>= 0)
	{
		sNr = FormatNumber(pInfo->host_total_credit);
		pListCtrl->SetItemText(iRow,iColumn,sNr);
	}
	if (iSortingColumnPrimary == COLUMN_PROJECT_HOST_CREDITS) pData->m_fSortPrimary = (float) pInfo->host_total_credit;
	if (iSortingColumnSecondary == COLUMN_PROJECT_HOST_CREDITS) pData->m_fSortSecondary = (float) pInfo->host_total_credit;
	if (iSortingColumnTertiary == COLUMN_PROJECT_HOST_CREDITS) pData->m_fSortTertiary = (float) pInfo->host_total_credit;

	iColumn = m_iColumnOrder[COLUMN_PROJECT_HOST_A_CREDITS];
	if ( iColumn>= 0)
	{
		sNr = FormatNumber(pInfo->host_expavg_credit, 2);
		pListCtrl->SetItemText(iRow,iColumn,sNr);
	}
	if (iSortingColumnPrimary == COLUMN_PROJECT_HOST_A_CREDITS) pData->m_fSortPrimary = (float) pInfo->host_expavg_credit;
	if (iSortingColumnSecondary == COLUMN_PROJECT_HOST_A_CREDITS) pData->m_fSortSecondary = (float) pInfo->host_expavg_credit;
	if (iSortingColumnTertiary == COLUMN_PROJECT_HOST_A_CREDITS) pData->m_fSortTertiary = (float) pInfo->host_expavg_credit;

	iColumn = m_iColumnOrder[COLUMN_PROJECT_SHARE];
	if ( iColumn>= 0)
	{
		dShare = pInfo->resource_share;
		dPerc =  (dShare*100) / m_dShareTotal;
		cBuffer[0] = PERC_CHAR; 
		cBuffer[1] = 2;
		cBuffer[2] = (char) dPerc;
		cBuffer[2]+= 1;

		CNumberFormat fNumber;
		CString sNumber = fNumber.FormatNumber(dShare,0);
		CString sPerc = fNumber.FormatNumber(dPerc,2);
		_snprintf_s(&cBuffer[3],50,_TRUNCATE,"%s (%s%%)",sNumber, sPerc);
		pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	}
	if (iSortingColumnPrimary == COLUMN_PROJECT_SHARE) pData->m_fSortPrimary = (float) pInfo->resource_share;
	if (iSortingColumnSecondary == COLUMN_PROJECT_SHARE) pData->m_fSortSecondary = (float) pInfo->resource_share;
	if (iSortingColumnTertiary == COLUMN_PROJECT_SHARE) pData->m_fSortTertiary = (float) pInfo->resource_share;

	// Tasks

	int iTasksCpu, iTasksGpu;
	__int64 iTimeLeftCpu, iTimeLeftGpu;
	int	iCoresCpu, iCoresGpu, iCoresLow;

	iColumn = m_iColumnOrder[COLUMN_PROJECT_TASKS];
	if ( iColumn>= 0)
	{
		if (GetTaskCount(pcComputerName, (char *) pInfo->project_name.c_str(), &iTasksCpu, &iTasksGpu, &iTimeLeftCpu, &iTimeLeftGpu, &iCoresCpu, &iCoresGpu, &iCoresLow, &bWarning))
		{
			sNr = FormatNumber(iTasksCpu, 0);
			sNr+= " / ";
			sNr+= FormatNumber(iTasksGpu, 0);
			if (iCoresLow > 0)
			{
				sNr+= " / ";
				sNr+= FormatNumber(iCoresLow, 0);
			}
		}
		else
		{
			sNr = "";
		}

		if (bWarning)
		{
			CString sColor, sText;
			sColor.Format("%d", *(pColorsWarnings + COLOR_WARNINGS_ARRAY_TASKS));
			sColor = HIDE_CHAR + sColor;
			sText = COLOR_CHAR;
			sNr = sText + sNr + sColor;
		}

		pListCtrl->SetItemText(iRow,iColumn,sNr);
	}

	int iTasks;
	if (iTasksCpu <= 0) iTasks = iTasksGpu;
	else iTasks = iTasksCpu;

	if (iSortingColumnPrimary == COLUMN_PROJECT_TASKS) pData->m_iSortPrimary = iTasks;
	if (iSortingColumnSecondary == COLUMN_PROJECT_TASKS) pData->m_iSortSecondary = iTasks;
	if (iSortingColumnTertiary == COLUMN_PROJECT_TASKS) pData->m_iSortTertiary = iTasks;
	// Tasks

	bool	bFetchTasks = false;
	int		iDayCpu, iDayGpu;
	double	dWeekCpu, dWeekGpu;
	int		iSort;

	iColumn = m_iColumnOrder[COLUMN_PROJECT_TASKS_DAY];
	if ((iColumn>= 0) && !m_bThreadTasksBusy)
	{
		GetTasksDayWeek(pcComputerName, (char *) pInfo->project_name.c_str(), &iDayCpu, &dWeekCpu, &iDayGpu, &dWeekGpu);
		bFetchTasks = true;
		sNr = FormatNumber(iDayCpu, 0); 
		sNr+= "/";
		sNr+= FormatNumber(iDayGpu, 0);
		pListCtrl->SetItemText(iRow,iColumn,sNr);
		if (iDayGpu != 0) iSort = iDayGpu;
		else iSort = iDayCpu;
		if (iSortingColumnPrimary == COLUMN_PROJECT_TASKS_DAY) pData->m_iSortPrimary = iDayGpu;
		if (iSortingColumnSecondary == COLUMN_PROJECT_TASKS_DAY) pData->m_iSortSecondary = iDayGpu;
		if (iSortingColumnTertiary == COLUMN_PROJECT_TASKS_DAY) pData->m_iSortTertiary = iDayGpu;
	}
	iColumn = m_iColumnOrder[COLUMN_PROJECT_TASKS_WEEK];
	if ((iColumn>= 0) && !m_bThreadTasksBusy)	// CPU / GPU
	{
		if (!bFetchTasks)
		{
			GetTasksDayWeek(pcComputerName, (char *) pInfo->project_name.c_str(), &iDayCpu, &dWeekCpu, &iDayGpu, &dWeekGpu);
		}
		sNr = FormatNumber(dWeekCpu, 0);
		sNr+= "(";
		sNr+= FormatNumber(dWeekCpu/7, 2);
		sNr+= ")/";
		sNr+= FormatNumber(dWeekGpu, 0);
		sNr+= "(";
		sNr+= FormatNumber(dWeekGpu/7, 2);
		sNr+= ")";
		pListCtrl->SetItemText(iRow,iColumn,sNr);
		if (dWeekGpu != 0) iSort = (int) dWeekGpu;
		else iSort = (int) dWeekCpu;
		if (iSortingColumnPrimary == COLUMN_PROJECT_TASKS_WEEK) pData->m_iSortPrimary = iSort;
		if (iSortingColumnSecondary == COLUMN_PROJECT_TASKS_WEEK) pData->m_iSortSecondary = iSort;
		if (iSortingColumnTertiary == COLUMN_PROJECT_TASKS_WEEK) pData->m_iSortTertiary = iSort;
	}
	iColumn = m_iColumnOrder[COLUMN_PROJECT_RANK];
	if (iColumn>= 0)
	{
		int iRank;
		CString sProject = pInfo->project_name.c_str();
		CString sUrl = pInfo->master_url.c_str();
		iRank = GetRank(&sProject, &sUrl ,&sNr);
		pListCtrl->SetItemText(iRow,iColumn,sNr);
		if (iSortingColumnPrimary == COLUMN_PROJECT_RANK) pData->m_iSortPrimary = iRank;
		if (iSortingColumnSecondary == COLUMN_PROJECT_RANK) pData->m_iSortSecondary = iRank;
		if (iSortingColumnTertiary == COLUMN_PROJECT_RANK) pData->m_iSortTertiary = iRank;
	}
	iColumn = m_iColumnOrder[COLUMN_PROJECT_TEAM_RANK];
	if (iColumn>= 0)
	{
		int iRank;
		CString sProject = pInfo->project_name.c_str();
		CString sUrl = pInfo->master_url.c_str();
		iRank = GetTeamRank(&sProject, &sUrl ,&sNr);
		pListCtrl->SetItemText(iRow,iColumn,sNr);
		if (iSortingColumnPrimary == COLUMN_PROJECT_TEAM_RANK) pData->m_iSortPrimary = iRank;
		if (iSortingColumnSecondary == COLUMN_PROJECT_TEAM_RANK) pData->m_iSortSecondary = iRank;
		if (iSortingColumnTertiary == COLUMN_PROJECT_TEAM_RANK) pData->m_iSortTertiary = iRank;
	}


	// Time Left

	int iTimeLeftDivCpu, iTimeLeftDivGpu;
	if (iCoresCpu <=0 ) iCoresCpu = 1;
	if (iTimeLeftCpu > 0)
	{
		if (iTasksCpu < iCoresCpu)
		{
			iTimeLeftDivCpu = iTasksCpu;
		}
		else
		{
			iTimeLeftDivCpu = iCoresCpu;
		}
		if (iTimeLeftDivCpu > 0) iTimeLeftCpu /= iTimeLeftDivCpu;
		CString sAfter;
		sAfter.Format(" (%d) / ",iTimeLeftDivCpu);
		timeString.TimeString((int) iTimeLeftCpu,&cBuffer[0], 60, "", (char *) sAfter.GetBuffer(),true,true);
	}
	else strcpy_s(cBuffer,60,"- / ");

	if (iCoresGpu <=0 ) iCoresGpu = 1;
	if (iTimeLeftGpu > 0)
	{
		if (iTasksGpu < iCoresGpu)
		{
			iTimeLeftDivGpu = iTasksGpu;
		}
		else
		{
			iTimeLeftDivGpu = iCoresGpu;
		}
		if (iTimeLeftDivGpu > 0) iTimeLeftGpu /= iTimeLeftDivGpu;

		CString sAfter;
		sAfter.Format(" (%d)",iTimeLeftDivGpu);
		timeString.TimeString((int) iTimeLeftGpu,&cBuffer2[0], 60, "", (char *) sAfter.GetBuffer(),true, true);
	}
	else strcpy_s(cBuffer2,60,"-");

	strcat_s(cBuffer,cBuffer2);

	iColumn = m_iColumnOrder[COLUMN_PROJECT_TIME_LEFT];
	if ( iColumn>= 0)
	{
		pListCtrl->SetItemText(iRow,iColumn,cBuffer);
	}

	int iSorting;
	if (iTimeLeftCpu == 0)  iSorting = (int) iTimeLeftGpu;
	else  iSorting = (int) iTimeLeftCpu;

	if (iSortingColumnPrimary == COLUMN_PROJECT_TIME_LEFT) pData->m_iSortPrimary = iSorting;
	if (iSortingColumnSecondary == COLUMN_PROJECT_TIME_LEFT) pData->m_iSortSecondary = iSorting;
	if (iSortingColumnTertiary == COLUMN_PROJECT_TIME_LEFT) pData->m_iSortTertiary = iSorting;
	// Time Left

	sStatus = "";
//	iStateSort = 0;
	if (pInfo->suspended_via_gui)			{sStatus += gszTranslation[PosViewProjecsStatusSuspendedByGui] ;}//iStateSort = 3;}
	if (pInfo->dont_request_more_work)
	{
		if (sStatus != "")			sStatus += ", ";
		sStatus += gszTranslation[PosViewProjecsStatusNoNewTasks];
//		iStateSort = 2;
	}

	if (pInfo->scheduler_rpc_in_progress == true)
	{
		if (sStatus != "")	sStatus += ", ";
		sStatus += gszTranslation[PosViewProjecsStatusInProgress];
	}
	else
	{
		if (pInfo->min_rpc_time != 0)
		{
			CTime ctNext((time_t) pInfo->min_rpc_time);
			CTime ctNow(CTime::GetCurrentTime());
			CTimeSpan toGo = ctNext - ctNow;
			if (toGo.GetTotalSeconds() >= 0)
			{
				if (sStatus != "")	sStatus += ", ";
				sStatus += gszTranslation[PosViewProjecsStatusDefered];
				sStatus += toGo.Format("%H:%M:%S");
				time = GetCurrentTime();
				time = (time_t) pInfo->min_rpc_time;
				localtime_s(&tmtime, &time );
				cBuffer[0] = 0;
				sStatus += cBuffer;
//				iStateSort = 1;
			}
		}
	}

	

	if (pInfo->sched_rpc_pending != 0)
	{
		if (sStatus != "")	sStatus += ", ";

		switch (pInfo->sched_rpc_pending)
		{
			case 1:
				sStatus += gszTranslation[PosViewProjecsStatusUpdating];
			break;
			case 2:
				sStatus += gszTranslation[PosViewProjecsStatusReport];
			break;
			case 3:	// need work
				sStatus += gszTranslation[PosViewProjecsStatusNeedWork];
			break;
			case 4:	
				sStatus += gszTranslation[PosViewProjecsStatusTrickle];
			break;
			case 5:
				if (pInfo->attached_via_acct_mgr)
				{
					sStatus += gszTranslation[PosViewProjecsStatusUpdatingAM];
				}
				else
				{
					sStatus += gszTranslation[PosViewProjecsStatusInitializing];
				}
			break;
			case 6:
				sStatus += gszTranslation[PosViewProjecsStatusAttaching];
			break;
			case 7:
				if (pInfo->scheduler_rpc_in_progress == 0)
				{
					sStatus += gszTranslation[PosViewProjecsStatusProjectReq];
				}
			break;
			default:
				_snprintf_s(&cBuffer[0],50,_TRUNCATE,"sRPCp %d", pInfo->sched_rpc_pending);
				sStatus += cBuffer;
		}
	}

/*

95 #define RPC_REASON_USER_REQ         1
 196 #define RPC_REASON_RESULTS_DUE      2
 197 #define RPC_REASON_NEED_WORK        3
 198 #define RPC_REASON_TRICKLE_UP       4
 199 #define RPC_REASON_ACCT_MGR_REQ     5
 200 #define RPC_REASON_INIT             6
 201 #define RPC_REASON_PROJECT_REQ      7

const char* rpc_reason_string(int reason) {
    switch (reason) {
    case RPC_REASON_USER_REQ: return "Requested by user";
    case RPC_REASON_NEED_WORK: return "To fetch work";
    case RPC_REASON_RESULTS_DUE: return "To report completed tasks";
    case RPC_REASON_TRICKLE_UP: return "To send trickle-up message";
    case RPC_REASON_ACCT_MGR_REQ: return "Requested by account manager";
    case RPC_REASON_INIT: return "Project initialization";
    case RPC_REASON_PROJECT_REQ: return "Requested by project";
    default: return "Unknown reason";
    }
}

const char* suspend_reason_string(int reason) {
    switch (reason) {
    case SUSPEND_REASON_BATTERIES: return "on batteries";
    case SUSPEND_REASON_USER_ACTIVE: return "computer is in use";
    case SUSPEND_REASON_USER_REQ: return "user request";
    case SUSPEND_REASON_TIME_OF_DAY: return "time of day";
    case SUSPEND_REASON_BENCHMARKS: return "CPU benchmarks in progress";
    case SUSPEND_REASON_DISK_SIZE: return "need disk space - check preferences";
    case SUSPEND_REASON_NO_RECENT_INPUT: return "no recent user activity";
    case SUSPEND_REASON_INITIAL_DELAY: return "initial delay";
    case SUSPEND_REASON_EXCLUSIVE_APP_RUNNING: return "an exclusive app is running";
    case SUSPEND_REASON_CPU_USAGE: return "CPU is busy";
    case SUSPEND_REASON_NETWORK_QUOTA_EXCEEDED: return "network bandwidth limit exceeded";
    case SUSPEND_REASON_OS: return "requested by operating system";
    }
    return "unknown reason";
}
*/


	iColumn = m_iColumnOrder[COLUMN_PROJECT_STATUS];
	if ( iColumn>= 0)
	{
		pListCtrl->SetItemText(iRow,iColumn,sStatus);
		if (iSortingColumnPrimary == COLUMN_PROJECT_STATUS) pData->m_sSortPrimary = sStatus;
		if (iSortingColumnSecondary == COLUMN_PROJECT_STATUS) pData->m_sSortSecondary = sStatus;
		if (iSortingColumnTertiary == COLUMN_PROJECT_STATUS) pData->m_sSortTertiary = sStatus;
	}

	iColumn = m_iColumnOrder[COLUMN_PROJECT_COMPUTER];
	if ( iColumn>= 0)
	{
		if (pcComputerName != NULL) pListCtrl->SetItemText(iRow,iColumn, pcComputerName);
	}
	pData->m_iComputerId = iComputerId;

	// colors

	pData->m_color = *(pColorsProjects + COLOR_ARRAY_PROJECT_NORMAL);
	if (pInfo->dont_request_more_work) pData->m_color = *(pColorsProjects + COLOR_ARRAY_PROJECT_NONEWWORK);
//	if (bWarning) pData->m_color = *(pColorsWarnings + COLOR_WARNINGS_ARRAY_TASKS);
	if (pInfo->suspended_via_gui) pData->m_color = *(pColorsProjects + COLOR_ARRAY_PROJECT_SUSPENDED);

	lvi.mask = LVIF_PARAM;
	lvi.iItem = iRow;
	lvi.iSubItem = 0;

	iColumn = m_iColumnOrder[COLUMN_PROJECT_PROJECT];
	if ( iColumn>= 0)
	{
		if (pcComputerName != NULL) pListCtrl->SetItemText(iRow,iColumn, pProjectName);
		if (iSortingColumnPrimary == COLUMN_PROJECT_PROJECT) pData->m_cSortPrimary = pProjectName;
		if (iSortingColumnSecondary == COLUMN_PROJECT_PROJECT) pData->m_cSortSecondary = pProjectName;
		if (iSortingColumnTertiary == COLUMN_PROJECT_PROJECT) pData->m_cSortTertiary = pProjectName;
	}

	iColumn = m_iColumnOrder[COLUMN_PROJECT_VENUE];
	if ( iColumn>= 0)
	{
		pcVenue = (char *) pInfo->venue.c_str();
		pListCtrl->SetItemText(iRow,iColumn, pcVenue);
		if (iSortingColumnPrimary == COLUMN_PROJECT_VENUE) pData->m_cSortPrimary = pcVenue;
		if (iSortingColumnSecondary == COLUMN_PROJECT_VENUE) pData->m_cSortSecondary = pcVenue;
		if (iSortingColumnTertiary == COLUMN_PROJECT_VENUE) pData->m_cSortTertiary = pcVenue;
	}


	lvi.lParam = (LPARAM) pData;
	pListCtrl->SetItem(&lvi);
}

CString CTemplateRemoteProjectView::FormatNumber(double dNr, int iNrComma)
{
	CNumberFormat numberFormat;
	return numberFormat.FormatNumber(dNr, iNrComma);
}

bool CTemplateRemoteProjectView::GetTaskCount(char *pcComputer, char  *pszProject, int *piTasksCpu, int *piTasksGpu, __int64 *piTimeLeftCpu, __int64 *piTimeLeftGpu, int *piCoresCpu, int *piCoresGpu, int *piCoresLow, bool *pbWarning )
{
	int					iSize, iPos;
	bool				bFound;
	CHistoryCountItem	*pHistoryCountItem;
//	CString				sProject;
	CDlgSettingsWarning *pDlgWarning;

//	sProject = pszProject;

	bFound = false;

	*piTasksCpu = 0;
	*piTasksGpu = 0;
	*piTimeLeftCpu = 0;
	*piTimeLeftGpu = 0;
	*piCoresCpu = 0;
	*piCoresGpu = 0;
	*piCoresLow = 0;
	pDlgWarning = theApp.m_pDlgSettingsWarning;

//	for (int iCount = 0; iCount < (int) m_lProjectItem.size(); iCount++)
	{
		iSize = (int) m_lHistoryCount.size();
		for (iPos = 0; iPos < iSize; iPos++)
		{
			pHistoryCountItem = m_lHistoryCount.at(iPos);
			if (pHistoryCountItem->m_pcComputer != NULL)
			{
				if (strcmp(pcComputer, pHistoryCountItem->m_pcComputer) == 0)
				{
					bFound = true;
					if (strcmp(pszProject, pHistoryCountItem->m_pcProject) == 0)
					{
						*piTasksCpu = pHistoryCountItem->m_iTaskCountCpu;
						*piTasksGpu = pHistoryCountItem->m_iTaskCountGpu;
						*piTimeLeftCpu = pHistoryCountItem->m_iTimeLeftCpu;
						*piTimeLeftGpu = pHistoryCountItem->m_iTimeLeftGpu;
						*piCoresCpu = pHistoryCountItem->m_iCoresCpu;
						*piCoresGpu = pHistoryCountItem->m_iCoresGpu;
						*piCoresLow = pHistoryCountItem->m_iTaskCountLow;
	
						CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer1, &pDlgWarning->m_sProject1, pDlgWarning->m_iWarningCpuTasks1, pDlgWarning->m_iWarningGpuTasks1, pbWarning);
						CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer2, &pDlgWarning->m_sProject2, pDlgWarning->m_iWarningCpuTasks2, pDlgWarning->m_iWarningGpuTasks2, pbWarning);
						CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer3, &pDlgWarning->m_sProject3, pDlgWarning->m_iWarningCpuTasks3, pDlgWarning->m_iWarningGpuTasks3, pbWarning);
						CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer4, &pDlgWarning->m_sProject4, pDlgWarning->m_iWarningCpuTasks4, pDlgWarning->m_iWarningGpuTasks4, pbWarning);

						return bFound;
					}
				}
			}
		}
	}
	return bFound;
}

void CTemplateRemoteProjectView::GetTaskCount(char *pcComputer, int *piTasksCpu, int *piTasksGpu, __int64 *piTimeLeftCpu, __int64 *piTimeLeftGpu)
{
	int					iSize, iPos;
//	bool				bFound;
	CHistoryCountItem	*pHistoryCountItem;


	*piTasksCpu = 0;
	*piTasksGpu = 0;
	*piTimeLeftCpu = 0;
	*piTimeLeftGpu = 0;
	*piTasksCpu = 0;
	*piTasksGpu = 0;

	iSize = (int) m_lHistoryCount.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pHistoryCountItem = m_lHistoryCount.at(iPos);
		if (pHistoryCountItem->m_pcComputer != NULL)
		{
			if (strcmp(pcComputer, pHistoryCountItem->m_pcComputer) == 0)
			{
				*piTasksCpu += pHistoryCountItem->m_iTaskCountCpu;
				*piTasksGpu += pHistoryCountItem->m_iTaskCountGpu;
				*piTimeLeftCpu += pHistoryCountItem->m_iTimeLeftCpu;
				*piTimeLeftGpu += pHistoryCountItem->m_iTimeLeftGpu;
			}
		}
	}
}


void CTemplateRemoteProjectView::CheckWarning(CHistoryCountItem *pHistoryCountItem, CString *psComputerWarning, CString *psProjectWarning, int iWarningCpuTasks, int iWarningGpuTasks, bool *pbWarning)
{
	CString sComputer, sComputerWarning;
	CString sProject, sProjectWarning;

	if (psComputerWarning->GetLength() > 0)
	{
		if (pHistoryCountItem != NULL) sComputer = pHistoryCountItem->m_pcComputer;
		sComputerWarning = *psComputerWarning;
		sComputer.MakeLower();
		sComputerWarning.MakeLower();
		if (sComputer.Find(sComputerWarning) < 0)
		{
			return;
		}
	}
	if (psProjectWarning->GetLength() > 0)
	{
		if (pHistoryCountItem != NULL) sProject = pHistoryCountItem->m_pcProject;
		sProjectWarning = *psProjectWarning;
		sProject.MakeLower();
		sProjectWarning.MakeLower();

		if (sProject.Find(sProjectWarning) < 0)
		{
			return;
		}
	}
	// match computer and project

	if (iWarningCpuTasks > 0)
	{
		if (pHistoryCountItem->m_iTaskCountCpu < iWarningCpuTasks)
		{
			*pbWarning = true;
			return;
		}
	}
	if (iWarningGpuTasks > 0)
	{
		if (pHistoryCountItem->m_iTaskCountGpu < iWarningGpuTasks)
		{
			*pbWarning = true;
			return;
		}
	}
}

void CTemplateRemoteProjectView::DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin)
{
	int			iItemCount;
	bool		bMax;
	CListData	*pData;

	m_bUseSkin = bUseSkin;
	m_iUseSkin = iUseSkin;
	if (!bUseAlternatingLines) return;

	CListCtrl& listCtrl = GetListCtrl();
	iItemCount = listCtrl.GetItemCount();


	for (int iCount = 1; iCount < iItemCount; iCount+=2)
	{
		pData = (CListData*) listCtrl.GetItemData(iCount);

		if (bUseSkin)
		{
			bMax = false;
			if ((pData->m_color&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>8)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>16)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
	
			if (bMax) 	pData->m_color -= RGB(COLOR_OFFSET_MIN,COLOR_OFFSET_MIN,COLOR_OFFSET_MIN);
			else pData->m_color -= RGB(COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING);
		}
		else
		{
			pData->m_color -= RGB(20,20,20);
		}
	}
}

void CTemplateRemoteProjectView::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pSortColumn->ClickItem(pNMHDR, pResult);
	m_iTimerWu = 1000;				// 1000 to ensure direct timer message
}

void CTemplateRemoteProjectView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListCtrl& listCtrl = GetListCtrl();

	if (nChar == VK_END)
	{
		m_pSelection->SelectionEnd(&listCtrl);		
	}
	if (nChar == VK_HOME)
	{
		m_pSelection->SelectionBegin(&listCtrl);		
	}

	if (m_pSelection->SelectionNumber() != 0)
	{
		if (nChar == VK_DOWN)
		{
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_PROJECT_PROJECT],m_iColumnOrder[COLUMN_PROJECT_COMPUTER],1);
			return;
		}
		if (nChar == VK_UP)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_PROJECT_PROJECT],m_iColumnOrder[COLUMN_PROJECT_COMPUTER],1);
			return;
		}
		if (nChar == VK_NEXT)
		{
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_PROJECT_PROJECT],m_iColumnOrder[COLUMN_PROJECT_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
		if (nChar == VK_PRIOR)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_PROJECT_PROJECT],m_iColumnOrder[COLUMN_PROJECT_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
	}
	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteProjectView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListViewEx::OnKeyUp(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl& listCtrl = GetListCtrl();
	int row = listCtrl.HitTest( point, NULL );

	m_pSelection->SelectionSet(row);

	CListViewEx::OnLButtonDown(nFlags, point);
}

void CTemplateRemoteProjectView::OnRButtonDown(UINT nFlags, CPoint point)
{
//	if (m_pSelection->SelectionNumber() == 0)
	{
		CListCtrl& listCtrl = GetListCtrl();
		int row = listCtrl.HitTest( point, NULL );
		m_pSelection->SelectionSetRight(row);
	}
	CListViewEx::OnRButtonDown(nFlags, point);
}

BOOL CTemplateRemoteProjectView::OnHeaderEndDrag(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER	*pNMH ;

	pNMH  = (NMHEADER *) pNMHDR;

	if (pNMH->pitem->mask & HDI_ORDER)
	{
		CListCtrl& listCtrl = GetListCtrl();
		int iw = listCtrl.GetColumnWidth(0); 
		listCtrl.PostMessage(LVM_SETCOLUMNWIDTH, 0, (LPARAM) iw); 
		m_bDraged = true;
	}

	return FALSE;
}

BOOL CTemplateRemoteProjectView::OnHeaderChanged(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	int  iCountHeader;
	int  iCount, iDelta, iMove1;
	int  iOrder[50];
	int	 iNumber, iColumnHeader[NUM_REMOTE_PROJECT_COLUMNS+1], iColumnHeaderAfter[NUM_REMOTE_PROJECT_COLUMNS+1];;

	if (!m_bDraged) return 0;
	m_bDraged = false;
	iDelta = -1;

	CListCtrl& listCtrl = GetListCtrl();	
	CHeaderCtrl* pHeaderCtrl = listCtrl.GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		iCountHeader = pHeaderCtrl->GetItemCount();

		LPINT pnOrder = (LPINT) malloc(iCountHeader*sizeof(int));
		ASSERT(pnOrder != NULL);
		if (NULL != pnOrder)
		{
			pHeaderCtrl->GetOrderArray(pnOrder, iCountHeader);
			for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iOrder[iCount] = pnOrder[iCount]; 
			}

			m_pSortColumn->OrderChanged(iCountHeader, pnOrder);

			for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
			{
				iNumber = m_iColumnOrder[iCount];
				if (iNumber > NUM_REMOTE_PROJECT_COLUMNS) break;
				if (iNumber >=0) iColumnHeader[iNumber] = iCount;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iMove1 = pnOrder[iCount];
				iColumnHeaderAfter[iCount] = iColumnHeader[iMove1];
			}

			for (iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
			{
				m_iColumnOrder[iCount] = -1;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iNumber = iColumnHeaderAfter[iCount];
				if (iNumber > NUM_REMOTE_PROJECT_COLUMNS) break;
				if (iNumber >=0) m_iColumnOrder[iNumber] = iCount;
			}


	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				pnOrder[iCount] = iCount; 
			}
			pHeaderCtrl->SetOrderArray(iCountHeader, pnOrder);	// set to 0,1,2, again
			free(pnOrder);
         }
	}

	AddAllColumns(false);

	listCtrl.Invalidate(TRUE);
	m_iTimerWu = 1000;				// 1000 to ensure direct timer message
	return FALSE;
}


LRESULT CTemplateRemoteProjectView::OnSettingsApplyChangesProjects(WPARAM parm1, LPARAM parm2)
{
	m_bApplyChangesOnColumn = true;

	CListCtrl& listCtrl = GetListCtrl();

	// save all column width

	CheckAndSaveColumnsWidth();

	// clear list
	listCtrl.DeleteAllItems();

	// delete all columns
	for (int iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
	{
		if (listCtrl.DeleteColumn(0) != TRUE) break;
	}

	// add all columns

	AddAllColumns();


	// read back the column width
//	ReadAllColumnsWidth();				no no add = now modify

	listCtrl.Invalidate(TRUE);

	return 0;
}


LRESULT CTemplateRemoteProjectView::OnProjectHistoryCount(WPARAM parm1, LPARAM parm2)
{
	UINT	iSize, iPos;
	CHistoryCountItem	*pHistoryCountItemFrom, *pHistoryCountItemTo;

	std::deque<CHistoryRpc*>		*plHistory;
	iSize = (int) m_lHistoryCount.size();
	for (iPos = 0; iPos < iSize; iPos++)	
	{
		pHistoryCountItemTo = m_lHistoryCount.at(iPos);
		delete pHistoryCountItemTo;
	}
	m_lHistoryCount.clear();

	plHistory = (std::deque<CHistoryRpc*> *) parm2;

	for (int iComputer=0; iComputer < (int) plHistory->size(); iComputer++)
	{
		{
			for (iPos = 0; iPos < plHistory->at(iComputer)->m_lHistoryCount.size(); iPos++)
			{
				{
					pHistoryCountItemFrom = plHistory->at(iComputer)->m_lHistoryCount.at(iPos);
	
					pHistoryCountItemTo = new CHistoryCountItem;
					pHistoryCountItemTo->m_iTaskCountCpu = pHistoryCountItemFrom->m_iTaskCountCpu;
					pHistoryCountItemTo->m_iTaskCountGpu = pHistoryCountItemFrom->m_iTaskCountGpu;
					pHistoryCountItemTo->AddProject(pHistoryCountItemFrom->m_pcProject);
					pHistoryCountItemTo->AddComputer(pHistoryCountItemFrom->m_pcComputer);
					pHistoryCountItemTo->m_iCoresCpu = pHistoryCountItemFrom->m_iCoresCpu;
					pHistoryCountItemTo->m_iCoresGpu = pHistoryCountItemFrom->m_iCoresGpu;
					pHistoryCountItemTo->m_iTimeLeftCpu = pHistoryCountItemFrom->m_iTimeLeftCpu;
					pHistoryCountItemTo->m_iTimeLeftGpu = pHistoryCountItemFrom->m_iTimeLeftGpu;
					pHistoryCountItemTo->m_iTaskCountLow = pHistoryCountItemFrom->m_iTaskCountLow;
					m_lHistoryCount.push_back(pHistoryCountItemTo);
				}
			}
		}
	}

	__int64 iTimeLeftCpu, iTimeLeftGpu;
	int iTasksCpu, iTasksGpu;

	// not include all the additional virtual localhost too much overhead.
	GetTaskCount((char *) theApp.m_pMainFrame->m_sLocalHostThreadId.c_str(), &iTasksCpu, &iTasksGpu, &iTimeLeftCpu, &iTimeLeftGpu);
	::PostThreadMessage(theApp.m_pMainFrame->m_iLocalHostThreadId, UWM_MSG_THREAD_DAYS_LEFT, (WPARAM) iTimeLeftCpu, (LPARAM) iTimeLeftGpu);
	::PostThreadMessage(theApp.m_pMainFrame->m_iLocalHostThreadId, UWM_MSG_THREAD_WU_LEFT, (WPARAM) iTasksCpu, (LPARAM) iTasksGpu);

	return 0;
}

void CTemplateRemoteProjectView::ProjectPropertiesGet(int iBusy)
{
	bool bFound;
	int iCount;

	// first get the thread list and process all threads

	m_iProjectPropertiesUniqueThreadsFoundCount = 0;

	for (iCount = 0; iCount < (int) m_lProjectItem.size(); iCount++)
	{
		m_lProjectItem.at(iCount)->m_iProjectPropertiesUniqueThreadId = -1;
	}

	// only one call / thread, because we get all projects back from a single thread.

	for (iCount = 0; iCount < (int) m_lProjectProperties.size(); iCount++)
	{
		bFound = false;
		for (int iCountU = 0; iCountU < m_iProjectPropertiesUniqueThreadsFoundCount; iCountU++)
		{
			if (m_lProjectItem.at(iCountU)->m_iProjectPropertiesUniqueThreadId == m_lProjectProperties.at(iCount)->m_iThreadId) bFound = true;
		}
		if (!bFound)
		{
			m_lProjectItem.at(m_iProjectPropertiesUniqueThreadsFoundCount)->m_iProjectPropertiesUniqueThreadId = m_lProjectProperties.at(iCount)->m_iThreadId;
			m_iProjectPropertiesUniqueThreadsFoundCount++;
		}
	}

	m_iProjectPropertiesThreadsPostedCount = 0;
	m_iProjectPropertiesThreadsReturnCount = 0;
	// send to all unique treads
	for (iCount = 0; iCount < m_iProjectPropertiesUniqueThreadsFoundCount; iCount++)
	{
		m_lProjectItem.at(iCount)->m_projectsDiskUsage.pProjects = &m_lProjectItem.at(iCount)->m_projects;
		m_lProjectItem.at(iCount)->m_projectsDiskUsage.pDiskUsage= &m_lProjectItem.at(iCount)->m_diskUsage;
		m_lProjectItem.at(iCount)->m_projects.clear();
		m_lProjectItem.at(iCount)->m_diskUsage.clear();
		::PostThreadMessage(m_lProjectItem.at(iCount)->m_iProjectPropertiesUniqueThreadId,UWM_MSG_THREAD_START_RPC_READ_PROJECT,(WPARAM) this->m_hWnd, (LPARAM) &m_lProjectItem.at(iCount)->m_projectsDiskUsage);
		m_iProjectPropertiesThreadsPostedCount ++;
		m_iProjectPropertiesBusy = iBusy;
	}
}

LRESULT CTemplateRemoteProjectView::OnProjectPropertiesReady(WPARAM parm1, LPARAM parm2)
{
	CRpcThreadReturn *pRpcThreadReturn;
	CCreateHtmlWebServerProperties createHtmlWebServerProperties;

	pRpcThreadReturn = (CRpcThreadReturn *) parm2;
	delete pRpcThreadReturn;

	m_iProjectPropertiesThreadsReturnCount++;
	if (m_iProjectPropertiesThreadsReturnCount != m_iProjectPropertiesThreadsPostedCount) return 0; // wait for the rest

	// got them all.
	if (m_iProjectPropertiesBusy == PROPERTIES_DIALOG)	ProjectPropertiesReady(NULL);
	else												ProjectPropertiesReady(&createHtmlWebServerProperties);

	DeleteProjectProperties();
	for (int iCount = 0; iCount < (int) m_lProjectItem.size(); iCount++)
	{
		m_lProjectItem.at(iCount)->m_projects.clear();
		m_lProjectItem.at(iCount)->m_diskUsage.clear();
	}

	m_iProjectPropertiesBusy = PROPERTIES_READY;
	return 0;
}

void CTemplateRemoteProjectView::PropertiesAddRow(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties, CString *psColumn0, CString *psColumn1)
{
	if (pCreateHtmlWebServerProperties)
	{
		pCreateHtmlWebServerProperties->Add(psColumn0, psColumn1);
	}
	else theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) psColumn0, (LPARAM) psColumn1);
}

void CTemplateRemoteProjectView::ProjectPropertiesReady(CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties)
{
	int			iThreadId;
	CString		sTxt;
	CString		sComputer;
	CString		sColumn0, sColumn1;
	bool		bFirstTime = true;


	if (pCreateHtmlWebServerProperties == NULL)
	{
		theApp.m_pDlgProperties->SetWindowText(gszTranslation[PosDialogProjectPropTitleProjectP]);		//"Project properties");
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_SETUP,2,(LPARAM) registryPropertiesProject);	// 2 columns
	}

	for (int iSelected = 0; iSelected < (int) m_lProjectProperties.size(); iSelected++)
	{
		iThreadId = m_lProjectProperties.at(iSelected)->m_iThreadId;
		for (int iProjectPos = 0; iProjectPos < (int) m_lProjectItem.size(); iProjectPos++)
		{
			if (iThreadId == m_lProjectItem.at(iProjectPos)->m_iProjectPropertiesUniqueThreadId)
			{
				int iSize = (int) m_lProjectItem.at(iProjectPos)->m_projects.projects.size();
				for (int iProjects = 0; iProjects < iSize; iProjects++)
				{
					if (m_lProjectProperties.at(iSelected)->m_sProject == m_lProjectItem.at(iProjectPos)->m_projects.projects[iProjects]->project_name.c_str())
					{
						if (pCreateHtmlWebServerProperties == NULL)
						{
						if (!bFirstTime) {sColumn0 = "=================================================";sColumn1 = sColumn0; theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);}
						}
						else
						{
							if (!bFirstTime) {sColumn0 = "============";sColumn1 = sColumn0; pCreateHtmlWebServerProperties->Add(&sColumn0, &sColumn1);}
						}
						bFirstTime = false;
						if (sComputer != m_lProjectProperties.at(iSelected)->m_sComputerId)
						{
							sComputer = m_lProjectProperties.at(iSelected)->m_sComputerId;
							sColumn0 = gszTranslation[PosDialogProjectPropComputer];	
							sColumn1 = sComputer;
							PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
						}
						
						sColumn0 = gszTranslation[PosDialogProjectPropProject]; sColumn1 = m_lProjectProperties.at(iSelected)->m_sProject; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
						sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

						ProjectProperties1(m_lProjectItem.at(iProjectPos), iProjects, pCreateHtmlWebServerProperties);

						if ((int) m_lProjectItem.at(iProjectPos)->m_diskUsage.projects.size() == iSize)
						{
							ProjectProperties2(&m_lProjectItem.at(iProjectPos)->m_diskUsage, iProjects, pCreateHtmlWebServerProperties);
						}
					}
				}

			}
		}
	}
	if (pCreateHtmlWebServerProperties) pCreateHtmlWebServerProperties->Finish();
}

void CTemplateRemoteProjectView::ProjectProperties1(CComputerProjectListItem *pComputerProjectListItem, int iProjects, CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties)
{
	CString		sColumn0, sColumn1;
	char		cBuffer[256];
	CNumberFormat numberFormat;
	CTimeString	timeString;
	PROJECT		*pInfo;
	bool		bV6;

	bV6 = pComputerProjectListItem->m_projectsDiskUsage.m_bBoincClientV6;

	pInfo = pComputerProjectListItem->m_projects.projects.at(iProjects);

	sColumn0 = gszTranslation[PosDialogProjectPropGeneral]; sColumn1 = ""; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	sColumn0 = gszTranslation[PosDialogProjectPropMasterUrl]; sColumn1 = pInfo->master_url.c_str(); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropUserName]; sColumn1 = pInfo->user_name.c_str(); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropTeamName]; sColumn1 = pInfo->team_name.c_str(); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropVenue]; sColumn1 = pInfo->venue.c_str(); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropResourseShare]; sColumn1 = numberFormat.FormatNumber(pInfo->resource_share, 0);PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	if (pInfo->upload_backoff)
	{
		timeString.TimeString((int) pInfo->upload_backoff, cBuffer, 50, "", "");
		sColumn0 = gszTranslation[PosDialogProjectPropFileUploadDeferred]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}
	if (pInfo->download_backoff)
	{
		timeString.TimeString((int) pInfo->download_backoff, cBuffer, 50, "", "");
		sColumn0 = gszTranslation[PosDialogProjectPropFileDownloadDeferred]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}


	sColumn0 = gszTranslation[PosDialogProjectPropTasksCompleted]; _snprintf_s(&cBuffer[0],50,_TRUNCATE,"%d",pInfo->njobs_success);sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectTasksError]; _snprintf_s(&cBuffer[0],50,_TRUNCATE,"%d",pInfo->njobs_error);sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);


	sColumn0 = gszTranslation[PosDialogProjectPropComputerId];_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%d",pInfo->hostid); sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropProjectId];sColumn1 = pInfo->external_cpid; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	sColumn0 = gszTranslation[PosDialogProjectPropNonCpuIntensive]; sColumn1 = YesNo(pInfo->non_cpu_intensive); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropSuspendedGui]; sColumn1 = YesNo(pInfo->suspended_via_gui); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropDontReqWork]; sColumn1 = YesNo(pInfo->dont_request_more_work); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropSchedulerInProg]; sColumn1 = YesNo(pInfo->scheduler_rpc_in_progress); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropAttachedAcountM]; sColumn1 = YesNo(pInfo->attached_via_acct_mgr); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropDetachWD]; sColumn1 = YesNo(pInfo->detach_when_done); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropEnded]; sColumn1 = YesNo(pInfo->ended); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropCredit];sColumn1 = ""; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	CString sTotal, sAverage;
	sTotal		= gszTranslation[PosDialogProjectPropTotal];
	sAverage	= gszTranslation[PosDialogProjectPropAverage];

	sColumn0 = gszTranslation[PosDialogProjectPropUserCredit];	sColumn1 = numberFormat.FormatNumber(pInfo->user_total_credit, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = "";												sColumn1 = numberFormat.FormatNumber(pInfo->user_expavg_credit, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropHostCredit];	sColumn1 = numberFormat.FormatNumber(pInfo->host_total_credit, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = "";												sColumn1 = numberFormat.FormatNumber(pInfo->host_expavg_credit, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);	

	sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropMore]; sColumn1 = ""; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	sColumn0 = gszTranslation[PosDialogProjectPropDurationF]; sColumn1 = numberFormat.FormatNumber(pInfo->duration_correction_factor,10); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	if (pComputerProjectListItem->m_projectsDiskUsage.m_bBoincClientV6)
	{
		sColumn0 = gszTranslation[PosDialogProjectPropSchedulePending]; sColumn1 = numberFormat.FormatNumber(pInfo->sched_rpc_pending, 0); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		sColumn0 = gszTranslation[PosDialogProjectPropCpuShortTimeDebt]; sColumn1 = numberFormat.FormatNumber(pInfo->cpu_short_term_debt, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		sColumn0 = gszTranslation[PosDialogProjectPropCpuLongTimeDebt]; sColumn1 = numberFormat.FormatNumber(pInfo->cpu_long_term_debt, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		if (pComputerProjectListItem->m_projectsDiskUsage.bHave_cuda)
		{
			sColumn0 = gszTranslation[PosDialogProjectPropCudaShortTimeDebt]; sColumn1 = numberFormat.FormatNumber(pInfo->cuda_short_term_debt, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
			sColumn0 = gszTranslation[PosDialogProjectPropCudaLongTimeDebt]; sColumn1 = numberFormat.FormatNumber(pInfo->cuda_debt, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		}
		if (pComputerProjectListItem->m_projectsDiskUsage.bHave_ati)
		{
			sColumn0 = gszTranslation[PosDialogProjectPropAtiShortTimeDebt]; sColumn1 = numberFormat.FormatNumber(pInfo->ati_short_term_debt, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
			sColumn0 = gszTranslation[PosDialogProjectPropAtiLongTimeDebt]; sColumn1 = numberFormat.FormatNumber(pInfo->ati_debt, 2); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		}
	}
	else
	{
		sColumn0 = gszTranslation[PosDialogProjectPropSchedulingPriority]; sColumn1 = numberFormat.FormatNumber(pInfo->sched_priority, 2, true); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}

	time_t tSystemTime;
	_time64(&tSystemTime);

	if (pInfo->no_cpu_pref)
	{
		sColumn0 = gszTranslation[PosDialogProjectPropProject]; sColumn1 = gszTranslation[PosDialogProjectPropCpuDontFetch]; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}

	CDateConvert	dateConvert;
//	if (bV6)
	{
		dateConvert.Convert((time_t) pInfo->cpu_backoff_time, cBuffer);
	}
//	else
//	{
//		timeString.TimeString((int) pInfo->cpu_backoff_time, cBuffer, 255, "", "", false, false);
//	}
	sColumn0 = gszTranslation[PosDialogProjectPropCpuBackoff]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	timeString.TimeString((int) pInfo->cpu_backoff_interval, cBuffer, 255, "", "", false, false);
	sColumn0 = gszTranslation[PosDialogProjectPropCpuBackoffInterval]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	if (pComputerProjectListItem->m_projectsDiskUsage.bHave_cuda)
	{
		if (pInfo->no_cuda_pref)
		{
			sColumn0 = gszTranslation[PosDialogProjectPropProject]; sColumn1 = gszTranslation[PosDialogProjectPropAtiDontFetch]; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		}
//		if (bV6)
		{
			dateConvert.Convert((time_t)  pInfo->cuda_backoff_time, cBuffer);
		}
//		else
//		{
//			timeString.TimeString((int) pInfo->cuda_backoff_time, cBuffer, 255, "", "", false, false);
//		}
		sColumn0 = gszTranslation[PosDialogProjectPropCudaBackoff]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		timeString.TimeString((int) pInfo->cuda_backoff_interval, cBuffer, 255, "", "", false, false);
		sColumn0 = gszTranslation[PosDialogProjectPropCpuBackoffInterval]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}

	if (pComputerProjectListItem->m_projectsDiskUsage.bHave_ati)
	{
		if (pInfo->no_ati_pref)
		{
			sColumn0 = gszTranslation[PosDialogProjectPropProject]; sColumn1 = gszTranslation[PosDialogProjectPropAtiDontFetch]; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		}
//		if (bV6)
		{
			dateConvert.Convert((time_t) pInfo->ati_backoff_time, cBuffer);
		}
//		else
//		{
//			timeString.TimeString((int) pInfo->ati_backoff_time, cBuffer, 255, "", "", false, false);
//		}
		sColumn0 = gszTranslation[PosDialogProjectPropAtiBackoff]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
		timeString.TimeString((int) pInfo->ati_backoff_interval, cBuffer, 255, "", "", true, false);
		sColumn0 = gszTranslation[PosDialogProjectPropCpuBackoffInterval]; sColumn1 = cBuffer; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	}
}

void CTemplateRemoteProjectView::ProjectProperties2(DISK_USAGE *pInfo, int iPos, CCreateHtmlWebServerProperties *pCreateHtmlWebServerProperties)
{
	CString		sColumn0, sColumn1;

	sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropDisk]; sColumn1 = ""; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = "";sColumn1 = sColumn0; PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	sColumn0 = gszTranslation[PosDialogProjectPropDiskUse]; sColumn1 = DiskSpace(pInfo->projects[iPos]->disk_usage); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);

	int iSize = (int) pInfo->projects.size();
	double dTotalUsed = 0;
	for (int iCount = 0; iCount < iSize; iCount++)
	{
		dTotalUsed += pInfo->projects[iCount]->disk_usage;
	}
	sColumn0 = gszTranslation[PosDialogProjectPropDiskAll]; sColumn1 = DiskSpace(dTotalUsed); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropDiskAllowed]; sColumn1 = DiskSpace(pInfo->d_allowed); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropDiskBoinc]; sColumn1 = DiskSpace(pInfo->d_boinc); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropDiskFree]; sColumn1 = DiskSpace(pInfo->d_free); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
	sColumn0 = gszTranslation[PosDialogProjectPropDiskTotal]; sColumn1 = DiskSpace(pInfo->d_total); PropertiesAddRow(pCreateHtmlWebServerProperties, &sColumn0, &sColumn1);
}

CString CTemplateRemoteProjectView::DiskSpace(double dSpace)
{
	CString sTxt, sFormat;
	CNumberFormat numberFormat;

	if (dSpace > KILO_BYTE)

	dSpace /= MEGA_BYTE;
	
	if (dSpace > KILO_BYTE)
	{
		dSpace /= KILO_BYTE;
		sTxt = numberFormat.FormatNumber(dSpace, 2);
		sTxt+= " GB";
//		sTxt = "%.2f GB";
	}
	else
	{
//		sTxt = "%.2f MB";
		sTxt = numberFormat.FormatNumber(dSpace, 2);
		sTxt+= " MB";
	}

	sFormat.Format(sTxt, dSpace);
	return sFormat;
}

CString CTemplateRemoteProjectView::YesNo(bool bYesNo)
{
	if (bYesNo) return gszTranslation[PosDialogProjectPropYes];			//"Yes";
	return gszTranslation[PosDialogProjectPropNo];						//"No";
}

CString CTemplateRemoteProjectView::Time(double dTime)
{
	char cBuffer[256];

	if (dTime >1)
	{
		int iHour, iMinute, iSecond;
		iMinute = (int) dTime / 60;
		iSecond = (int) dTime - iMinute*60 ;
		iHour	= iMinute / 60;
		iMinute = iMinute - iHour *60;
		_snprintf_s(cBuffer,50,_TRUNCATE,"%02d:%02d:%02d",iHour,iMinute,iSecond);
	}
	else strcpy_s(cBuffer,10,"-");

	return cBuffer;
}

// CComputerRemoteProjectsView diagnostics

#ifdef _DEBUG
void CTemplateRemoteProjectView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CTemplateRemoteProjectView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


void CTemplateRemoteProjectView::ClipBoard(bool bAll)
{
    HGLOBAL		hglbCopy; 
    LPTSTR		lptstrCopy; 
	CString		sTxt,sClipBoard;
	int			iItems, iNr;
	bool		bSelected;
	
	if ( !OpenClipboard() ) return;
	if( !EmptyClipboard() ) return;
 
	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();

	for (int iCountItems = 0; iCountItems < iItems; iCountItems++)
	{
		bSelected = m_pSelection->SelectionSelected(&listCtrl, iCountItems);

		if (bAll) bSelected = true;
		if (bSelected)
		{
			for (int iCountSearch = 0; iCountSearch < NUM_REMOTE_PROJECT_COLUMNS; iCountSearch++)
			{
				for (int iCount = 0; iCount < NUM_REMOTE_PROJECT_COLUMNS; iCount++)
				{
					iNr = m_iColumnOrder[iCount];
					if (iNr > NUM_REMOTE_PROJECT_COLUMNS) break;
					if (iNr >=0)
					{
						if (iCountSearch == iNr)
						{
							sTxt = listCtrl.GetItemText(iCountItems,iNr);
							RemoveSpecialText(&sTxt);
							sClipBoard += sTxt;
							sClipBoard += "\t";
							break;
						}
					}
				}
			}
			sClipBoard += "\r\n";
		}
	}

	// Allocate a global memory object for the text. 
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (sClipBoard.GetLength() + 1) * sizeof(char)); 
	if (hglbCopy == NULL) 
    { 
		CloseClipboard(); 
        return; 
	} 
 
	// Lock the handle and copy the text to the buffer. 
    lptstrCopy = (LPTSTR) GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, sClipBoard.GetBuffer(), sClipBoard.GetLength() * sizeof(char)); 
	lptstrCopy[sClipBoard.GetLength()] = (char) 0;    // null character 
	GlobalUnlock(hglbCopy); 
 
	// set the locale
	HANDLE hLC=GlobalAlloc(GMEM_MOVEABLE,sizeof(LCID));
	if (hLC)
	{
		PLCID pLc=(PLCID)GlobalLock(hLC);
		if (pLc)
		{
			*pLc = theApp.m_pTranslation->m_lcid;
			GlobalUnlock(hLC);
			if (!SetClipboardData(CF_LOCALE, pLc))	GlobalFree(hLC);
		}
		else
		{
			GlobalFree(hLC);
		}
	}

	// Place the handle on the clipboard. 
	SetClipboardData(CF_TEXT, hglbCopy); 

	CloseClipboard();
}


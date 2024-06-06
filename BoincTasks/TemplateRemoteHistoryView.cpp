// ComputerRemoteProjectsView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"   // base class for BoinCComputerView
#include "TemplateRemoteHistoryView.h"
#include "DlgColorHistory.h"
#include "DlgSettingsView.h"
#include "DlgSettingsHistory.h"
#include "DlgSettingsTasks.h"
#include "OperationItem.h"
#include "SortColumn.h"
#include "Selection.h"
#include "Translation.h"
#include "UseCpuGpu.h"
#include "DateConvert.h"
#include "CondenseResource.h"
#include "History.h"
#include "TimeString.h"

IMPLEMENT_DYNCREATE(CTemplateRemoteHistoryView, CListViewEx)

static int _gnRemoteHistoryColumnWidth[NUM_REMOTE_HISTORY_COLUMNS] =
{
	120,280,200,120,120,120,100,100,100,120,100,100
};

CTemplateRemoteHistoryView::CTemplateRemoteHistoryView()
{
	m_bSelection = FALSE;					// disable default selection

	m_dataDummy.m_color = RGB(255,255,255);
	m_dataDummy.m_cSortPrimary = "";
	m_dataDummy.m_cSortSecondary = "";
	m_dataDummy.m_cSortTertiary = "";

	m_iSortingChanged = 10;		// first 10 times refresh

	m_pSortColumn = new CSortColumn;
	m_pSelection = new CSelection;

	CRpcInfoClient *pRpcInfoClient;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pRpcInfoClient = new CRpcInfoClient;
		pRpcInfoClient->m_bReady = false;
		pRpcInfoClient->m_bReadyCopy = false;
		pRpcInfoClient->m_iLostCount = 0;
		pRpcInfoClient->m_bConnectionLogged = false;
		pRpcInfoClient->m_sComputerId = "";

		m_lComputerRpcInfoClient.push_back(pRpcInfoClient);
	}

	CRpcInfoHistoryItem *pRpcInfoHistoryItem;
	CHistoryRpc	*pHistoryRpc;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pHistoryRpc	= new CHistoryRpc;
//		pHistoryRpc->m_plHistory = new std::deque<CHistoryItem*>;
//		pHistoryRpc->m_plHistoryLong = new std::deque<CHistoryItem*>;
//		pHistoryRpc->m_plHistoryCount = new std::deque<CHistoryCountItem*>;
		m_lComputer.push_back(pHistoryRpc);

		pRpcInfoHistoryItem = new CRpcInfoHistoryItem;
		pRpcInfoHistoryItem->m_pcReadyRpcComputerId = NULL;
		m_lComputerHistoryItem.push_back(pRpcInfoHistoryItem);
	}
	
	m_bDraged = false;

	m_bHistoryForcedUpdate = false;
	m_tNextHistoryUpdateTime = 0;
	m_tNextHistoryWrite = 0;
}

CTemplateRemoteHistoryView::~CTemplateRemoteHistoryView()
{
	CRules rules;
	int iSize, iPos;

	for (int iCount = 0; iCount < (int) m_lListData.size(); iCount++)
	{
		delete m_lListData.at(iCount);
	}
//	m_lListData.clear();

	CRpcInfoClient *pRpcInfoClient;
	for (int iCount = 0; iCount < (int) m_lComputerRpcInfoClient.size(); iCount++)
	{
		// NEVER do this here as it is made in RemoteCombinedDoc delete m_pRpcInfoClient[iCount]->m_pcComputerId;
		pRpcInfoClient = m_lComputerRpcInfoClient.at(iCount);
		delete pRpcInfoClient; 
	}
//	m_lComputerRpcInfoClient.clear();

	for (int iCountComputer = 0; iCountComputer < (int) m_lComputer.size(); iCountComputer++)
	{
		for (int iCount = 0; iCount < (int) m_lComputer.at(iCountComputer)->m_lHistory.size(); iCount++)
		{
			delete m_lComputer.at(iCountComputer)->m_lHistory.at(iCount);
			m_lComputer.at(iCountComputer)->m_lHistory.at(iCount) = NULL;
		}
		for (int iCount = 0; iCount < (int) m_lComputer.at(iCountComputer)->m_lHistoryLong.size(); iCount++)
		{
			delete m_lComputer.at(iCountComputer)->m_lHistoryLong.at(iCount);
			m_lComputer.at(iCountComputer)->m_lHistoryLong.at(iCount) = NULL;
		}
		for (int iCount = 0; iCount < (int) m_lComputer.at(iCountComputer)->m_lHistoryCount.size(); iCount++)
		{
			delete m_lComputer.at(iCountComputer)->m_lHistoryCount.at(iCount);
			m_lComputer.at(iCountComputer)->m_lHistoryCount.at(iCount) = NULL;
		}
//		m_lComputer.at(iCountComputer)->m_plHistory->clear();
//		delete m_lComputer.at(iCountComputer)->m_plHistory;
//		delete m_lComputer.at(iCountComputer)->m_plHistoryLong;
//		delete m_lComputer.at(iCountComputer)->m_plHistoryCount;
		delete m_lComputer.at(iCountComputer);
		m_lComputer.at(iCountComputer) = NULL;
	}

//	for (int iCount = 0; iCount < (int) m_lComputerHistoryRpc.size(); iCount++)
//	{
//		delete m_lComputerHistoryRpc.at(iCount);										// warning the history lists inside are only copies, so don't delete them here.
//	}

	for (int iCount = 0; iCount < (int) m_lComputerHistoryItem.size(); iCount++)
	{
		delete m_lComputerHistoryItem.at(iCount);
	}

	// delete all rules

	iSize = (int) m_lRules.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		rules.DeleteInterval(&m_lRules.at(iPos)->m_plInterval);
		delete m_lRules.at(iPos);
	}
//	m_lRules.clear();

	iSize = (int) m_lRulesActive.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		delete m_lRulesActive.at(iPos);
	}
//	m_lRulesActive.clear();

	// delete any operations that are still pending.
	// almost impossible but still.

	iSize = (int) m_lOperationsQue.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		delete m_lOperationsQue.at(iPos);
	}
//	m_lOperationsQue.clear();


	delete m_pSortColumn;
	delete m_pSelection;
}

void CTemplateRemoteHistoryView::OnInitialUpdate()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetExtendedStyle (listCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP);//|LVS_EX_DOUBLEBUFFER);
	AddAllColumns();
	CListViewEx::OnInitialUpdate();
}

void CTemplateRemoteHistoryView::AddAllColumns(bool bAdd)
{
	int			iCount, iColumn, iWidth, iFormat, iNr;
	int			iColumnHeader[NUM_REMOTE_HISTORY_COLUMNS+1];
	LV_COLUMN	lvc;
	CListData	*pData;

	// read back the column sequence

	if (bAdd)
	{
		m_iNrColomns = (int) ::SendMessage(theApp.m_pDlgSettingsHistory->m_hWnd, UWM_MSG_DLG_SETTINGS_HISTORY_GET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}
	else
	{	
		for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
		{
			iColumnHeader[iCount] = -1;
		}

		for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
		{
			iNr = m_iColumnOrder[iCount];
			if (iNr > NUM_REMOTE_HISTORY_COLUMNS) break;
			if (iNr >=0) iColumnHeader[iNr] = iCount;
		}
		::SendMessage(theApp.m_pDlgSettingsHistory->m_hWnd, UWM_MSG_DLG_SETTINGS_HISTORY_SET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
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
			lvc.pszText = gszTranslation[PosViewHistoryRowProject+iColumn];
			iWidth = ReadColumnWidth(iColumn, _gnRemoteHistoryColumnWidth[iColumn]);
			::SendMessage(theApp.m_pDlgSettingsHistory->m_hWnd, UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, (WPARAM) iCount,(LPARAM) iWidth);

			m_iColumnWidth[iColumn] = iWidth;
			lvc.cx = m_iColumnWidth[iColumn];

			switch(iColumn)
			{
				case COLUMN_HISTORY_ELAPSED:
				case COLUMN_HISTORY_COMPLETED:
				case COLUMN_HISTORY_REPORTED:
					iFormat = LVCFMT_RIGHT;
				break;
				case COLUMN_HISTORY_RATIO:
					iFormat = LVCFMT_CENTER;
				break;
				default:
					iFormat = LVCFMT_LEFT;
			}

			lvc.fmt = iFormat;
			if (bAdd)
			{
				listCtrl.InsertColumn(iCount,&lvc);
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

	if (m_lListData.size() < 1) 
	{
		pData = new CListData;
		m_lListData.push_back(pData);
	}
//	if (m_pData[0] == NULL) m_pData[0] = new CListData;

	if (!theApp.m_pDlgSettingsHistory->m_bHistoryEnabled)
	{
		LV_ITEM		lvi;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;

		lvi.iItem = 0;
		lvi.iSubItem = 0;
		lvi.pszText = "";	
		lvi.iImage = 0;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);

		lvi.lParam = (LPARAM) m_lListData.at(0);
		listCtrl.InsertItem(&lvi);

		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		listCtrl.SetItem(&lvi);

		m_lListData.at(0)->m_color = RGB(255,0,0);
		listCtrl.SetItemText(0,m_iColumnOrder[COLUMN_HISTORY_PROJECT],gszTranslation[PosViewHistoryStatusToEnable]);					//"To enable history:");
		listCtrl.SetItemText(0,m_iColumnOrder[COLUMN_HISTORY_APPLICATION],gszTranslation[PosViewHistoryStatusToEnableMenu]);			//"Extra->Settings->History");
	}

	m_pSortColumn->SetColumnOrder(m_iNrColomns, &iColumnHeader[0]);
	m_pSortColumn->Setup(&listCtrl,this->m_hWnd, m_iNrColomns, registrySortHistoryPrimary, registrySortHistorySecondary, registrySortHistoryTertiary, 517, 265, 265); // perc - computer - computer

	m_pSelection->Setup(&listCtrl,  &m_lListData, COLUMN_HISTORY_NAME, COLUMN_HISTORY_COMPUTER, NUM_REMOTE_HISTORY_COLUMNS ,&m_iColumnOrder[0]);
	m_pSelection->SelectionRemoveAll();
}

void CTemplateRemoteHistoryView::ReadAllColumnWidth()
{
	CListCtrl& listCtrl = GetListCtrl();
	int iWidth;

	for(int iCount = 0; iCount<=NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		iWidth = ReadColumnWidth(iCount, _gnRemoteHistoryColumnWidth[iCount]);
		listCtrl.SetColumnWidth(iCount, iWidth);
		m_iColumnWidth[iCount] = iWidth;
	}
}

int CTemplateRemoteHistoryView::ReadColumnWidth(int iColumn, int iDefault)
{
	CString sRegistry;
	char cChar;

	sRegistry = registryColumnHistory;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	return theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
}

void CTemplateRemoteHistoryView::CheckColumnWidth()
{
	int		iWidth, iColumn;
	bool	bChanged = false;
	char	cChar;
	CString sTxt;

	CListCtrl& listCtrl = GetListCtrl();

	for (int iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		iColumn = m_iColumnOrder[iCount];
		if (iColumn >= 0)
		{
			iWidth = listCtrl.GetColumnWidth(iColumn);
			if (m_iColumnWidth[iColumn] != listCtrl.GetColumnWidth(iColumn))
			{
				m_iColumnWidth[iColumn] = iWidth;
				bChanged = true;
				sTxt = registryColumnHistory;
				cChar = iCount + 'a';
				sTxt = sTxt + "_" + cChar; 
				// update new width in registry
				theApp.WriteProfileInt(registrySectionColumn, sTxt, iWidth);
				::SendMessage(theApp.m_pDlgSettingsHistory->m_hWnd, UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, (WPARAM) iColumn,(LPARAM) iWidth);
			}
		}
	}
}

std::deque<CHistoryRpc*> *CTemplateRemoteHistoryView::GetComputerList()
{
	return &m_lComputer;
}

void CTemplateRemoteHistoryView::DisplayRpc(CListCtrl *pListCtrl, std::deque<CHistoryItem*> *plHistory, int *piRow)
{
	CHistoryItem *pHistoryItem;
	int			iSize, iPos;

	iSize = (int) plHistory->size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pHistoryItem = plHistory->at(iPos);
		if ((pHistoryItem->m_iState&0xffffe) == STATE_RUNNING)			continue;	// don't show
		if ((pHistoryItem->m_iState&0xffffe) == STATE_WAITING_TO_RUN)	continue;	// don't show
		if ((pHistoryItem->m_iState&0xffffe) == STATE_READY_TO_START)	continue;	// don't show

		if (!m_bAllProjectsSelected)
		{
			if (StrStrI(m_sProjectsSelected.c_str(), pHistoryItem->m_pcProject) == NULL)			continue;
		}

		DisplaySingleItem(pListCtrl, pHistoryItem, piRow);
		*piRow += 1;
	}
}

void CTemplateRemoteHistoryView::DisplaySingleItem(CListCtrl *pListCtrl, CHistoryItem *pHistoryItem, int *piRow)
{
	bool	bCuda, bRecovered;
	char	cBuffer[64], cBuffer2[64];
	int		iStateSort, iGpu, iDetectedGpu, iColumn;
	double	fCpuUse, fGpuUse, fCpuUseCeil;
	UINT	iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;
	CString	sFormat, sError;
	COLORREF	*pColorsHistory;
	CListData	*pData;
	CDateConvert dateConvert;

	char cPercString[5];
	cPercString[0] = '%';//		"%.3f";
	cPercString[1] = '.';
	cPercString[2] = '2';
	cPercString[3] = 'f';
	cPercString[4] = 0;

	LV_ITEM		lvi;
	
	bRecovered = false;

	char cComma = theApp.m_pDlgSettingsView->m_cComma;

	while (*piRow >= (int) m_lListData.size())
	{
		pData = new CListData;
		m_lListData.push_back(pData);
	}
	pData = m_lListData.at(*piRow);

	pData->m_cSortPrimary = "";
	pData->m_cSortSecondary = "";
	pData->m_cSortTertiary = "";
	pData->m_bSelected = false;

	pColorsHistory = theApp.m_pDlgColorHistory->GetColorArray();

	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);

	float fVersion;
	fVersion = (float) pHistoryItem->m_iVersion;
	fVersion/= 100;
	CUseCpuGpu CpuGpu;
	if (!CpuGpu.GetCpuGpu(pHistoryItem->m_pcUse, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu))
	{
	}
	fCpuUseCeil = ceil(fCpuUse);

	if (strlen (pHistoryItem->m_pcPlanClass) == 0)
	{
		sFormat.Format("%.2f %s", fVersion, pHistoryItem->m_pcApplication);
	}
	else
	{
		sFormat.Format("%.2f %s (%s)", fVersion, pHistoryItem->m_pcApplication, pHistoryItem->m_pcPlanClass);
	}

	iColumn = m_iColumnOrder[COLUMN_HISTORY_APPLICATION];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn, sFormat);
	if (iSortingColumnPrimary == COLUMN_HISTORY_APPLICATION) pData->m_iSortPrimary = (pHistoryItem->m_iVersion *100)+iGpu;
	if (iSortingColumnSecondary == COLUMN_HISTORY_APPLICATION) pData->m_iSortSecondary = (pHistoryItem->m_iVersion *100)+iGpu;
	if (iSortingColumnTertiary == COLUMN_HISTORY_APPLICATION) pData->m_iSortTertiary = (pHistoryItem->m_iVersion *100)+iGpu;

	iColumn = m_iColumnOrder[COLUMN_HISTORY_NAME];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn, pHistoryItem->m_pcName);
	char *pChar = pHistoryItem->m_pcName;

	if (iSortingColumnPrimary == COLUMN_HISTORY_NAME) pData->m_cSortPrimary = pChar;
	if (iSortingColumnSecondary == COLUMN_HISTORY_NAME) pData->m_cSortSecondary = pChar;
	if (iSortingColumnTertiary == COLUMN_HISTORY_NAME) pData->m_cSortTertiary = pChar;

	// follow the name
	pData->m_bSelected = m_pSelection->Follow(pHistoryItem->m_pcName, pHistoryItem->m_pcComputer);

	CTimeString	timeString;
	timeString.TimeString(pHistoryItem->m_iElapsedTimeCpu,cBuffer, 50, "", "", true);
	timeString.TimeString(pHistoryItem->m_iElapsedTimeGpu,cBuffer2, 50, " (", ")", true);
	strcat_s(cBuffer, 50,cBuffer2);
	iColumn = m_iColumnOrder[COLUMN_HISTORY_ELAPSED];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn, cBuffer);		
	if (iSortingColumnPrimary == COLUMN_HISTORY_ELAPSED) pData->m_iSortPrimary = pHistoryItem->m_iElapsedTimeCpu;
	if (iSortingColumnSecondary == COLUMN_HISTORY_ELAPSED) pData->m_iSortSecondary = pHistoryItem->m_iElapsedTimeCpu;
	if (iSortingColumnTertiary == COLUMN_HISTORY_ELAPSED) pData->m_iSortTertiary = pHistoryItem->m_iElapsedTimeCpu;

// -------------------------------- completed
	
	time_t tTimeCompleted = pHistoryItem->m_i64CompletedTime;
	if (tTimeCompleted > 0)
	{
		dateConvert.Convert(tTimeCompleted, &cBuffer[0]);
	}
	else cBuffer[0] = 0;
	iColumn = m_iColumnOrder[COLUMN_HISTORY_COMPLETED];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn,cBuffer);
	if (iSortingColumnPrimary == COLUMN_HISTORY_COMPLETED) pData->m_iSortPrimary = (int) pHistoryItem->m_i64CompletedTime;
	if (iSortingColumnSecondary == COLUMN_HISTORY_COMPLETED) pData->m_iSortSecondary = (int) pHistoryItem->m_i64CompletedTime;
	if (iSortingColumnTertiary == COLUMN_HISTORY_COMPLETED) pData->m_iSortTertiary = (int) pHistoryItem->m_i64CompletedTime;
// -------------------------------- completed


// -------------------------------- reported
	
	time_t tTimeReported = pHistoryItem->m_i64ReportedTime;
	if (tTimeReported > 0)
	{
		dateConvert.Convert(tTimeReported, &cBuffer[0]);
	}
	else cBuffer[0] = 0;
	iColumn = m_iColumnOrder[COLUMN_HISTORY_REPORTED];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn,cBuffer);
	if (iSortingColumnPrimary == COLUMN_HISTORY_REPORTED) pData->m_iSortPrimary = (int) pHistoryItem->m_i64ReportedTime;
	if (iSortingColumnSecondary == COLUMN_HISTORY_REPORTED) pData->m_iSortSecondary = (int) pHistoryItem->m_i64ReportedTime;
	if (iSortingColumnTertiary == COLUMN_HISTORY_REPORTED) pData->m_iSortTertiary = (int) pHistoryItem->m_i64ReportedTime;
// -------------------------------- reported

// -------------------------------- use

	bool	bCondenseUse;
	bCondenseUse = theApp.m_pDlgSettingsTasks->m_bUseCondense;

	CString sUse;
	sUse = pHistoryItem->m_pcUse;

	if (bCondenseUse)
	{
		CCondenseResource condenseResource;
		condenseResource.Condense(&sUse);
	}
	if (cComma != '.')
	{
		sUse.Replace('.', cComma);
	}

	iColumn = m_iColumnOrder[COLUMN_HISTORY_USE];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn,sUse);
	pChar = pHistoryItem->m_pcUse;
	if (iSortingColumnPrimary == COLUMN_HISTORY_USE) pData->m_cSortPrimary = pChar;
	if (iSortingColumnSecondary == COLUMN_HISTORY_USE) pData->m_cSortSecondary = pChar;
	if (iSortingColumnTertiary == COLUMN_HISTORY_USE) pData->m_cSortTertiary = pChar;
// -------------------------------- use

// -------------------------------- ratio cpu%

	iColumn = m_iColumnOrder[COLUMN_HISTORY_RATIO];
	if ((m_iColumnWidth[iColumn] > 1) && (iColumn >= 0))
	{
		double dRatioPerc;
		if (pHistoryItem->m_iElapsedTimeCpu > 0)
		{
			dRatioPerc = pHistoryItem->m_iElapsedTimeGpu;
			dRatioPerc/= pHistoryItem->m_iElapsedTimeCpu;
		}
		else
		{
			dRatioPerc = 0;
		}
		dRatioPerc*= 100;
		if (fCpuUseCeil > 0)
		{
			dRatioPerc = (float) (dRatioPerc / fCpuUseCeil);
		}

		if (dRatioPerc < 0)  dRatioPerc = 0;

		if (dRatioPerc > 100) dRatioPerc = 100;

		cPercString[2] = theApp.m_pDlgSettingsTasks->m_iCpuDigits + '0';

		cBuffer[1] = 2;
		if (dRatioPerc == 0) cBuffer[1] = 1;
		_snprintf_s(&cBuffer[3],50,_TRUNCATE,cPercString,dRatioPerc);

		if (cComma != '.')
		{
			char *cFind = strchr(cBuffer, '.'); 
			if (cFind) *cFind = cComma;
		}

		if (theApp.m_bNoCleanup)
		{
			_snprintf_s(&cBuffer[3],50,_TRUNCATE,cPercString,dRatioPerc);
			if (dRatioPerc > 100) dRatioPerc = 100;							// here !!! after the value but before the bar value is set.
			if (dRatioPerc < 0) dRatioPerc = 0;
		}
		else
		{
			if (dRatioPerc > 100) dRatioPerc = 100;							// here !!! after the value but before the bar value is set.
			if (dRatioPerc < 0) dRatioPerc = 0;
			_snprintf_s(&cBuffer[3],50,_TRUNCATE,cPercString,dRatioPerc);
		}

		cBuffer[0] = PERC_CHAR; 
		cBuffer[2] = (char) dRatioPerc;
		cBuffer[2]+= 1;

		if (cComma != '.')
		{
			char *cFind = strchr(cBuffer, '.'); 
			if (cFind) *cFind = cComma;
		}

		if (iColumn >= 0)  pListCtrl->SetItemText(*piRow,iColumn,cBuffer);

		if (iSortingColumnPrimary == COLUMN_HISTORY_RATIO) pData->m_fSortPrimary = dRatioPerc;
		if (iSortingColumnSecondary == COLUMN_HISTORY_RATIO) pData->m_fSortSecondary = dRatioPerc;
		if (iSortingColumnTertiary == COLUMN_HISTORY_RATIO) pData->m_fSortTertiary = dRatioPerc;
	}
	else
	{
		pListCtrl->SetItemText(*piRow,iColumn,"");
	}

// -------------------------------- ratio cpu%

	double dMemory;

// -------------------------------- Virtual Memory
	iColumn = m_iColumnOrder[COLUMN_HISTORY_VIRTUAL];
	dMemory = 0;

	if (iColumn >= 0)
	{
		if (pHistoryItem->m_dVirtualMemory > 0)
		{
			dMemory = pHistoryItem->m_dVirtualMemory/MEGA_BYTE;
			_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.2f MB",dMemory);
		}
		else
		{
			cBuffer[0] = 0;
		}
		pListCtrl->SetItemText(*piRow,iColumn,cBuffer);
	}
	if (iSortingColumnPrimary == COLUMN_HISTORY_VIRTUAL) pData->m_fSortPrimary = dMemory;
	if (iSortingColumnSecondary == COLUMN_HISTORY_VIRTUAL) pData->m_fSortSecondary = dMemory;
	if (iSortingColumnTertiary == COLUMN_HISTORY_VIRTUAL) pData->m_fSortTertiary = dMemory;
// -------------------------------- Virtual Memory

// -------------------------------- Memory
	iColumn = m_iColumnOrder[COLUMN_HISTORY_MEMORY];
	dMemory = 0;
	if (iColumn >= 0)
	{
		if (pHistoryItem->m_dMemory > 0)
		{
			dMemory = pHistoryItem->m_dMemory/MEGA_BYTE;
			_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.2f MB",dMemory);
		}
		else
		{
			cBuffer[0] = 0;
		}
		pListCtrl->SetItemText(*piRow,iColumn,cBuffer);
	}
	if (iSortingColumnPrimary == COLUMN_HISTORY_MEMORY) pData->m_fSortPrimary = dMemory;
	if (iSortingColumnSecondary == COLUMN_HISTORY_MEMORY) pData->m_fSortSecondary = dMemory;
	if (iSortingColumnTertiary == COLUMN_HISTORY_MEMORY) pData->m_fSortTertiary = dMemory;
// -------------------------------- Memory

// -------------------------------- state

	switch (pHistoryItem->m_iState&0xfffffe)	// remove high priority
	{
		case STATE_UPLOADING:
			if (pHistoryItem->m_i64ReportedTime > 0)
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewHistoryStatusReportedOk]);										//"Reported: Ok (u)");
				strcat_s(cBuffer,54," *");

				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_REPORTED_GPU);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_REPORTED);
				iStateSort = 6;
			}
			else
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusUploading]);											//"Uploading");
				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_UPLOAD_GPU);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_UPLOAD);
				iStateSort = 7;
			}

		break;

		case STATE_MISSED_RECOVERED:
			bRecovered = true;
		case STATE_READY_TO_REPORT:
			if (pHistoryItem->m_i64ReportedTime > 0)
			{
				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_REPORTED_GPU);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_REPORTED);
				strcpy_s(cBuffer,50,gszTranslation[PosViewHistoryStatusReportedOk]);										//"Reported: Ok");
				iStateSort = 6;
			}
			else
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusReadyTR]);												//"Ready to report");
				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_READY_TO_REPORT);
				iStateSort = 3;
			}
			if (bRecovered) strcat_s(cBuffer,50," +");
		break;

		case STATE_MISSED_OLD:


		case STATE_MISSED:
		case STATE_MISSED2:
			strcpy_s(cBuffer,50,gszTranslation[PosViewHistoryMessageMissed]);												//"Missed");
			if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_MISSED_GPU);
			else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_MISSED);
			iStateSort = 3;
		break;

		case STATE_ABORTED:
		case STATE_ABORTED1:
		case STATE_ABORTED2:
			if (pHistoryItem->m_i64ReportedTime > 0)
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewHistoryStatusReportedAb]);										//"Reported: Aborted");
				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_ABORT);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_ABORT);
				iStateSort = 4;
			}
			else
			{
				strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAborted]);												//"Aborted");
				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_ABORT);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_ABORT);
				iStateSort = 1;
			}
			int iExit;//, iSignal;
			iExit = pHistoryItem->m_iExitStatus >> 16;

			switch (iExit)
			{
				case ERR_ABORTED_BY_PROJECT:
					strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAbortedProject]);		//"Aborted by project");
				break;
				case ERR_ABORTED_VIA_GUI:
				case EXIT_ABORTED_BY_CLIENT:
					strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAbortedUser]);			//"Aborted by user");
				break;
				default:
					if (iExit != 0)
					{
						strcpy_s(cBuffer2,50,gszTranslation[PosViewTasksStatusAborted]);		//;"Aborted");
						strcat_s(cBuffer2,60," (%d)");
						_snprintf_s(cBuffer,60,_TRUNCATE,cBuffer2, iExit);
					}
					else
					{
						strcpy_s(cBuffer,50,gszTranslation[PosViewTasksStatusAborted]);			//;"Aborted");
					}

			}

		break;
		case STATE_COMPUTATON_ERROR:
//			int iExit;//, iSignal;
			iExit = pHistoryItem->m_iExitStatus >> 16;
			if (iExit == ERR_ABORTED_BY_PROJECT) sError = gszTranslation[PosViewTasksStatusAbortedProject];
			if (strstr(pHistoryItem->m_pcProject, "SETI") != NULL)
			{
				if (iExit == -6)	sError = "VLAR";
				if (iExit == -177)	sError ="Maximum time exceeded";
				if (iExit == -12)	sError = "Unknown";	// unknow is an OK error
			}

			if (pHistoryItem->m_i64ReportedTime > 0)
			{
				_snprintf_s(cBuffer,60,_TRUNCATE,gszTranslation[PosViewHistoryStatusReportedCompErr],iExit, sError);			//"Reported: Computation error (%d,%s)"
				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_REPORTED_ERROR);
				iStateSort = 5;
			}
			else
			{
				_snprintf_s(cBuffer,60,_TRUNCATE,gszTranslation[PosViewHistoryStatusCompErr],iExit, sError);				//"Computation error (%d,%s)"
				if (bCuda)	pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_ERROR_GPU);
				else		pData->m_color  = *(pColorsHistory + COLOR_HISTORY_ARRAY_ERROR);
				iStateSort = 2;
			}
		break;
		default:
			_snprintf_s(cBuffer,60,_TRUNCATE,"%x-state: ",pHistoryItem->m_iState);
			pData->m_color  = RGB(255,255,255);
			iStateSort = 0;
	}

	iColumn = m_iColumnOrder[COLUMN_HISTORY_STATUS];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn,cBuffer);
	if (iSortingColumnPrimary == COLUMN_HISTORY_STATUS) pData->m_iSortPrimary =  iStateSort;
	if (iSortingColumnSecondary == COLUMN_HISTORY_STATUS) pData->m_iSortSecondary =  iStateSort;
	if (iSortingColumnTertiary == COLUMN_HISTORY_STATUS) pData->m_iSortTertiary =  iStateSort;
// -------------------------------- state

	iColumn = m_iColumnOrder[COLUMN_HISTORY_COMPUTER];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn,pHistoryItem->m_pcComputer);
	pChar = pHistoryItem->m_pcComputer;
	if (iSortingColumnPrimary == COLUMN_HISTORY_COMPUTER) pData->m_cSortPrimary = pChar;
	if (iSortingColumnSecondary == COLUMN_HISTORY_COMPUTER) pData->m_cSortSecondary = pChar;
	if (iSortingColumnTertiary == COLUMN_HISTORY_COMPUTER) pData->m_cSortTertiary = pChar;

//	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.mask = LVIF_PARAM;
	lvi.iItem = *piRow;
	lvi.iSubItem = 0;

	iColumn = m_iColumnOrder[COLUMN_HISTORY_PROJECT];
	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn,pHistoryItem->m_pcProject);

	pChar = pHistoryItem->m_pcProject;
	lvi.pszText = pChar;
	if (iSortingColumnPrimary == COLUMN_HISTORY_PROJECT) pData->m_cSortPrimary = pChar;
	if (iSortingColumnSecondary == COLUMN_HISTORY_PROJECT) pData->m_cSortSecondary = pChar;
	if (iSortingColumnTertiary == COLUMN_HISTORY_PROJECT) pData->m_cSortTertiary = pChar;

	lvi.lParam = (LPARAM) pData;
	pListCtrl->SetItem(&lvi);
}

void CTemplateRemoteHistoryView::DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin)
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

void CTemplateRemoteHistoryView::TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore, char *pAfter)
{
	int iDay, iHour, iMinute, iSecond;

	if (iTime > 0)
	{
		iMinute = iTime / 60;
		iSecond = iTime - iMinute*60 ;
		iHour	= iMinute / 60;
		iMinute = iMinute - iHour *60;
		iDay	= iHour / 24;
		iHour	= iHour - iDay * 24;

		if (iDay > 0) _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02dd,%02d:%02d:%02d%s",pBefore,iDay,iHour,iMinute,iSecond,pAfter);
		else _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02d:%02d:%02d%s",pBefore, iHour,iMinute,iSecond, pAfter);
	}
 	else strcpy_s(pBuffer,iNrBuffer,"");
}

BEGIN_MESSAGE_MAP(CTemplateRemoteHistoryView, CListViewEx)

	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
//
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickList)
    ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	ON_NOTIFY_EX(HDN_ITEMCHANGED, 0, OnHeaderChanged) 
	ON_MESSAGE(UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_HISTORY,OnSettingsApplyChangesHistory)
//	ON_NOTIFY_EX(HDN_ITEMCLICK, 0, OnHeaderClick) 
END_MESSAGE_MAP()

// CComputerRemoteProjectsView diagnostics

#ifdef _DEBUG
void CTemplateRemoteHistoryView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CTemplateRemoteHistoryView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CTemplateRemoteHistoryView message handlers

void CTemplateRemoteHistoryView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl& listCtrl = GetListCtrl();
	int row = listCtrl.HitTest( point, NULL );
	m_pSelection->SelectionSet(row);
	m_iSortingChanged = 4;				// 4 updates

	Invalidate(FALSE);					// redraw the selection again.	// added 1.00
}


void CTemplateRemoteHistoryView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
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
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_HISTORY_NAME],m_iColumnOrder[COLUMN_HISTORY_COMPUTER],1);
			return;
		}
		if (nChar == VK_UP)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_HISTORY_NAME],m_iColumnOrder[COLUMN_HISTORY_COMPUTER],1);
			return;
		}
		if (nChar == VK_NEXT)
		{
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_HISTORY_NAME],m_iColumnOrder[COLUMN_HISTORY_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
		if (nChar == VK_PRIOR)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_HISTORY_NAME], m_iColumnOrder[COLUMN_HISTORY_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
	}
	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteHistoryView::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pSortColumn->ClickItem(pNMHDR, pResult);
	m_tNextHistoryUpdateTime = 0;		// direct action
	m_iSortingChanged = 4;				// 4 updates
}

void CTemplateRemoteHistoryView::OnRButtonDown(UINT nFlags, CPoint point)
{
//	if (m_pSelection->SelectionNumber() == 0)
	{
		CListCtrl& listCtrl = GetListCtrl();
		int row = listCtrl.HitTest( point, NULL );
		m_pSelection->SelectionSetRight(row);
	}
	
	CListViewEx::OnRButtonDown(nFlags, point);
}

BOOL CTemplateRemoteHistoryView::OnHeaderEndDrag(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
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

BOOL CTemplateRemoteHistoryView::OnHeaderChanged(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	int  iCountHeader;
	int  iCount, iDelta, iMove1;
	int  iOrder[50];
	int	 iNumber, iColumnHeader[NUM_REMOTE_HISTORY_COLUMNS+1], iColumnHeaderAfter[NUM_REMOTE_HISTORY_COLUMNS+1];;

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

			for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
			{
				iNumber = m_iColumnOrder[iCount];
				if (iNumber > NUM_REMOTE_HISTORY_COLUMNS) break;
				if (iNumber >=0) iColumnHeader[iNumber] = iCount;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iMove1 = pnOrder[iCount];
				iColumnHeaderAfter[iCount] = iColumnHeader[iMove1];
			}

			for (iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
			{
				m_iColumnOrder[iCount] = -1;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iNumber = iColumnHeaderAfter[iCount];
				if (iNumber > NUM_REMOTE_HISTORY_COLUMNS) break;
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
	m_tNextHistoryUpdateTime = 0;				// to ensure direct timer message
	m_iSortingChanged = 2;
	return FALSE;
}

LRESULT CTemplateRemoteHistoryView::OnSettingsApplyChangesHistory(WPARAM parm1, LPARAM parm2)
{
	m_bApplyChangesOnColumn = true;

	CListCtrl& listCtrl = GetListCtrl();

	// save all column width

	CheckColumnWidth();

	// clear list
	listCtrl.DeleteAllItems();

	// delete all columns
	for (int iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
	{
		if (listCtrl.DeleteColumn(0) != TRUE) break;
	}

	// add all columns

	AddAllColumns();

	// read back the column width
//	ReadAllColumnsWidth();			no no add is now modify

	listCtrl.Invalidate(TRUE);

	m_tNextHistoryUpdateTime = 0;	// direct action
	m_iSortingChanged = 2;

	return 0;
}


/*
BOOL CTemplateRemoteHistoryView::OnHeaderClick(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	CListCtrl& listCtrl = GetListCtrl();	
	CHeaderCtrl* pHeaderCtrl = listCtrl.GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		int ii = 1;
	}

	return FALSE;
}
*/

void CTemplateRemoteHistoryView::ClipBoard(bool bAll)
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
			for (int iCountSearch = 0; iCountSearch < NUM_REMOTE_HISTORY_COLUMNS; iCountSearch++)
			{
				for (int iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
				{
					iNr = m_iColumnOrder[iCount];
					if (iNr > NUM_REMOTE_HISTORY_COLUMNS) break;
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


// DlgHistory.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"

#include "ListViewEx.h"   // base class for BoinCComputerView
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "RemoteCombinedDoc.h"
#include "DlgFont.h"
#include "DlgSettingsTasks.h"
#include "DlgSettingsView.h"
#include "DlgColorHistory.h"

#include "DlgHistory.h"
#include "afxdialogex.h"
#include "UseCpuGpu.h"
#include "CondenseResource.h"
#include "DateConvert.h"
#include "TimeString.h"
#include "SortColumn.h"
#include "Translation.h"

// CDlgHistory dialog

IMPLEMENT_DYNAMIC(CDlgHistory, CDialogEx)

CDlgHistory::CDlgHistory(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHistory::IDD, pParent)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bUpdateWindow = false;

	m_iWindowInitialLeftMargin = -1;

	m_iTimer = 1000;
	m_iMaxTimer = 1;
	m_bThreadLocked = false;
	m_bDraw = false;

	m_plHistoryLong = NULL;
	m_plHistoryLongFetch = NULL;
}

CDlgHistory::~CDlgHistory()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}

	ClearHistory(m_plHistoryLong);
	ClearHistory(m_plHistoryLongFetch);

	delete m_plHistoryLong;
	delete m_plHistoryLongFetch;
}

void CDlgHistory::ClearHistory(std::deque<CHistoryItem*> *plHistoryLong)
{
	if (plHistoryLong == NULL) return;

	for (int iPos = 0; iPos < (int) plHistoryLong->size(); iPos++)
	{
		delete plHistoryLong->at(iPos);
	}
	plHistoryLong->clear();
}

void CDlgHistory::InitialUpdate(CRemoteCombinedDoc *pDoc)
{
	m_pDoc = pDoc;
}

BOOL CDlgHistory::OnInitDialog()
{
	int iCheck, iWidth, iFormat;

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translation

	this->SetWindowText(gszTranslation[PosDialogHistoryLongTitle]);
	m_textComputers.SetWindowText(gszTranslation[PosDialogGraphicTitleComputers]);
	m_textProjects.SetWindowText(gszTranslation[PosDialogGraphicTitleProjects]);
	m_checkSingleMultiple.SetWindowText(gszTranslation[PosDialogGraphicMultiple]);
// Translation

	RestoreWindowPlacement();

	LV_COLUMN	lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iColumn = 0; iColumn < NUM_REMOTE_HISTORY_COLUMNS; iColumn++)
	{
		switch(iColumn)
		{
			case COLUMN_HISTORY_ELAPSED:
			case COLUMN_HISTORY_COMPLETED:
			case COLUMN_HISTORY_REPORTED:
			case COLUMN_HISTORY_VIRTUAL:
			case COLUMN_HISTORY_MEMORY:
				iFormat = LVCFMT_RIGHT;
			break;
				case COLUMN_HISTORY_RATIO:
					iFormat = LVCFMT_CENTER;
				break;
			default:
			iFormat = LVCFMT_LEFT;
		}

		lvc.pszText = gszTranslation[PosViewHistoryRowProject+iColumn];
		lvc.iSubItem = iColumn;

		lvc.fmt = iFormat;

		iWidth =  ReadColumnWidth(iColumn, 200);;	//theApp.GetProfileInt(registrySectionColumn, registryColumnDlgHistory, 250);

		lvc.cx = iWidth;
		m_list.InsertColumn(iColumn,&lvc);
	}

	iCheck = theApp.GetProfileInt(registrySectionHistoryDialog, registrySingleMultiple, 1);
	m_checkSingleMultiple.SetCheck(iCheck);
	m_listProjects.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);

	CTime time;
	m_dateTimeTo.GetTime(time);
	time -= 86400*30;
	m_dateTimeFrom.SetTime(&time);

	m_list.m_pSortColumn->Setup(&m_list ,m_list.m_hWnd, NUM_REMOTE_HISTORY_COLUMNS, registrySortHistoryLongPrimary, registrySortHistoryLongSecondary, registrySortHistoryLongTertiary, 517, 265, 265); // perc - computer - computer

	m_list.SetFocus();
	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);	// request new font.

	SetTimer(UTIMER_DLG_HISTORY, 1000, 0);								// One second timer

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#define TOP_MARGIN_DLG_HISTORY	10
#define LEFT_MARGIN_DLG_HISTORY 0

void CDlgHistory::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
{
	LONG lTopMargin, lBottomMargin, lLeftMargin, lRightMargin, lWidthGraphic, lHeightGraphic;


	if (m_iWindowInitialLeftMargin == -1)
	{
		if (m_list.m_hWnd != NULL)
		{
			WINDOWPLACEMENT wp;
			m_list.GetWindowPlacement(&wp);
			m_iWindowInitialLeftMargin = wp.rcNormalPosition.left;
			m_iWindowInitialTopMargin = wp.rcNormalPosition.top;
		}
	}

	lLeftMargin= m_iWindowInitialLeftMargin;
	lTopMargin = m_iWindowInitialTopMargin;
	
	lBottomMargin	= 10;
	lRightMargin	= 5;

	lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
	lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));

// position graphic relative to window NOT the screen

	if (m_list.m_hWnd != NULL)
	{
		m_list.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
		m_list.Invalidate(TRUE);
	}

	// set timer to update window

	m_bUpdateWindow = true;
	this->Invalidate();
}

void CDlgHistory::RestoreWindowPlacement()
{
	CString sRegistry;

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryHistoryPosLeft, 100);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryHistoryPosTop, 100);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryHistoryPosRight, 100+1000) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryHistoryPosBottom, 100+800) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER );
}

void CDlgHistory::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	CString sRegistry;

	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryHistoryPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryHistoryPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryHistoryPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryHistoryPosBottom, wp.rcNormalPosition.bottom);
	SafeColumnWidth();
}

int CDlgHistory::ReadColumnWidth(int iColumn, int iDefault)
{
	CString	sTxt, sRegistry;
	char	cChar;
	int		iWidth;

	sRegistry = registryColumnDlgHistory;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
	m_iColumnWidth[iColumn] = iWidth;
	return iWidth;
}

void CDlgHistory::SafeColumnWidth()
{
	int		iWidth;
	CString sRegistry;
	char	cChar;

	for (int iColumn = 0; iColumn < NUM_REMOTE_HISTORY_COLUMNS; iColumn++)
	{
		iWidth = m_list.GetColumnWidth(iColumn);
		if (m_iColumnWidth[iColumn] != iWidth)
		{
			m_iColumnWidth[iColumn] = iWidth;
			sRegistry = registryColumnDlgHistory;
			cChar = iColumn + 'a';
			sRegistry = sRegistry + "_" + cChar; 
			// update new width in registry
			theApp.WriteProfileInt(registrySectionColumn, sRegistry, iWidth);
		}
	}
}

void CDlgHistory::AddAllItems()
{
//	CDateConvert dateConvert;
//	char	cBuffer[64];
	int	iFoundPos, iItemsComputers, iItemsProjects;
	int	iListSize;
	int	iItemsAdded;
	int	iHour, iMinutes, iSeconds;
	time_t	t_timeFrom, t_timeTo;

	CTime timeFrom, timeTo;

	m_dateTimeFrom.GetTime(timeFrom);
	m_dateTimeTo.GetTime(timeTo);

	iHour = timeFrom.GetHour();
	iMinutes = timeFrom.GetMinute();
	iSeconds = timeFrom.GetSecond();
	timeFrom-= iHour * 3600;			// to begin of day
	timeFrom-= iMinutes * 60;
	timeFrom-= iSeconds;
//	iHour = timeFrom.GetHour();
//	iMinutes = timeFrom.GetMinute();
//	iSeconds = timeFrom.GetSecond();

	t_timeFrom = timeFrom.GetTime();	// to end of day
//	dateConvert.Convert(t_timeFrom, &cBuffer[0]);


	iHour = timeTo.GetHour();
	iMinutes = timeTo.GetMinute();
	iSeconds = timeTo.GetSecond();
	iHour = 23 - iHour;
	iMinutes = 59 - iMinutes;
	iSeconds = 59 - iSeconds;

	timeTo+= iHour * 3600; 
	timeTo+= iMinutes * 60;
	timeTo+= iSeconds;
//	iHour = timeTo.GetHour();
//	iMinutes = timeTo.GetMinute();
//	iSeconds = timeTo.GetSecond();

	t_timeTo = timeTo.GetTime();
//	dateConvert.Convert(t_timeTo, &cBuffer[0]);

	iItemsComputers = m_listComputers.GetCount();
	iItemsProjects  = m_listProjects.GetCount();

	iListSize = m_list.GetItemCount();

	iItemsAdded = 0;
	for (int iPos = 0; iPos < (int) m_plHistoryLong->size(); iPos++)
	{
		if (m_plHistoryLong->at(iPos)->m_i64ReportedTime > t_timeFrom && m_plHistoryLong->at(iPos)->m_i64ReportedTime < t_timeTo)
		{
			iFoundPos = m_listComputers.FindStringExact(-1,m_plHistoryLong->at(iPos)->m_pcComputer);
			if (iFoundPos != LB_ERR)
			{
				if (m_listComputers.GetSel(iFoundPos) > 0)	// selected computer
				{
					iFoundPos = m_listProjects.FindStringExact(-1,m_plHistoryLong->at(iPos)->m_pcProject);
					if (iFoundPos != LB_ERR)
					{
						if (m_listProjects.GetSel(iFoundPos) > 0)	// selected computer and project
						{
							// expand list if needed
							while (iItemsAdded >= iListSize)
							{
								m_list.InsertItem(iListSize,"init");
								m_list.SetItemData(iListSize, NULL);
								iListSize++;
							}
							AddItem(m_plHistoryLong->at(iPos), iItemsAdded);
							iItemsAdded++;
						}
					}
				}
			}
		}
	}
	while (iItemsAdded <= iListSize)
	{
		m_list.DeleteItem(iListSize);
		iListSize--;
	}

	m_list.m_bUseSkin = theApp.m_pDlgSettingsView->m_bUseSkin;
	m_list.m_iUseSkin = theApp.m_pDlgSettingsView->m_iUseSkin;
	m_list.m_bUseHorizontalGrid = theApp.m_pDlgSettingsView->m_bUseHorizontalGrid;
	m_list.m_bUseVerticalGrid = theApp.m_pDlgSettingsView->m_bUseVerticalGrid;

	m_list.Sort();


	m_list.Invalidate(TRUE);
}

void CDlgHistory::AddItem(CHistoryItem *pHistoryItem, int iPosAdd)
{
//	int		iSortingColumnPrimary, iSortingColumnSecondary, iSortingColumnTertiary;
	char	cBuffer[64], cBuffer2[64];
	UINT iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;
	int		iStateSort;
	CDateConvert dateConvert;
	CUseCpuGpu	CpuGpu;
	COLORREF	*pColorsHistory;
	CString		sError;
	CListData	*pData;
	bool		bCuda, bRecovered;

	bRecovered = false;

	char cPercString[5];
	cPercString[0] = '%';//		"%.3f";
	cPercString[1] = '.';
	cPercString[2] = '2';
	cPercString[3] = 'f';
	cPercString[4] = 0;

	char cComma = theApp.m_pDlgSettingsView->m_cComma;
	m_list.m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);
	pColorsHistory = theApp.m_pDlgColorHistory->GetColorArray();

	while (iPosAdd >= (int) m_list.m_lListData.size())
	{
		pData = new CListData;
		m_list.m_lListData.push_back(pData);
	}

	pData = m_list.m_lListData.at(iPosAdd);

	double fCpuUse, fGpuUse, fCpuUseCeil;
	int	iGpu, iDetectedGpu;
	if (!CpuGpu.GetCpuGpu(pHistoryItem->m_pcUse, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu))
	{
	}
	fCpuUseCeil = ceil(fCpuUse);

// -------------------------------- project
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_PROJECT,pHistoryItem->m_pcProject);
	if (iSortingColumnPrimary == COLUMN_HISTORY_PROJECT) pData->m_cSortPrimary = pHistoryItem->m_pcProject;
	if (iSortingColumnSecondary == COLUMN_HISTORY_PROJECT) pData->m_cSortSecondary = pHistoryItem->m_pcProject;
	if (iSortingColumnTertiary == COLUMN_HISTORY_PROJECT) pData->m_cSortTertiary = pHistoryItem->m_pcProject;
// -------------------------------- project

// -------------------------------- application
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_APPLICATION,pHistoryItem->m_pcApplication);
	if (iSortingColumnPrimary == COLUMN_HISTORY_APPLICATION) pData->m_iSortPrimary = (pHistoryItem->m_iVersion *100)+iGpu;
	if (iSortingColumnSecondary == COLUMN_HISTORY_APPLICATION) pData->m_iSortSecondary = (pHistoryItem->m_iVersion *100)+iGpu;
	if (iSortingColumnTertiary == COLUMN_HISTORY_APPLICATION) pData->m_iSortTertiary = (pHistoryItem->m_iVersion *100)+iGpu;
// -------------------------------- application

// -------------------------------- name
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_NAME,pHistoryItem->m_pcName);
	if (iSortingColumnPrimary == COLUMN_HISTORY_NAME) pData->m_cSortPrimary = pHistoryItem->m_pcName;
	if (iSortingColumnSecondary == COLUMN_HISTORY_NAME) pData->m_cSortSecondary = pHistoryItem->m_pcName;
	if (iSortingColumnTertiary == COLUMN_HISTORY_NAME) pData->m_cSortTertiary = pHistoryItem->m_pcName;
// -------------------------------- name

// -------------------------------- elapsed
	CTimeString	timeString;
	timeString.TimeString(pHistoryItem->m_iElapsedTimeCpu,cBuffer, 50, "", "", true);
	timeString.TimeString(pHistoryItem->m_iElapsedTimeGpu,cBuffer2, 50, " (", ")", true);
	strcat_s(cBuffer, 50,cBuffer2);
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_ELAPSED,cBuffer);
	if (iSortingColumnPrimary == COLUMN_HISTORY_ELAPSED) pData->m_iSortPrimary = pHistoryItem->m_iElapsedTimeCpu;
	if (iSortingColumnSecondary == COLUMN_HISTORY_ELAPSED) pData->m_iSortSecondary = pHistoryItem->m_iElapsedTimeCpu;
	if (iSortingColumnTertiary == COLUMN_HISTORY_ELAPSED) pData->m_iSortTertiary = pHistoryItem->m_iElapsedTimeCpu;
// -------------------------------- elapsed

// -------------------------------- completed
	dateConvert.Convert(pHistoryItem->m_i64CompletedTime, &cBuffer[0]);
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_COMPLETED,cBuffer);
	if (iSortingColumnPrimary == COLUMN_HISTORY_COMPLETED) pData->m_iSortPrimary = (int) pHistoryItem->m_i64CompletedTime;
	if (iSortingColumnSecondary == COLUMN_HISTORY_COMPLETED) pData->m_iSortSecondary = (int) pHistoryItem->m_i64CompletedTime;
	if (iSortingColumnTertiary == COLUMN_HISTORY_COMPLETED) pData->m_iSortTertiary = (int) pHistoryItem->m_i64CompletedTime;
// -------------------------------- completed

// -------------------------------- reported
	dateConvert.Convert(pHistoryItem->m_i64ReportedTime, &cBuffer[0]);
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_REPORTED,cBuffer);
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
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_USE,sUse);
	if (iSortingColumnPrimary == COLUMN_HISTORY_USE) pData->m_cSortPrimary = pHistoryItem->m_pcUse;
	if (iSortingColumnSecondary == COLUMN_HISTORY_USE) pData->m_cSortSecondary = pHistoryItem->m_pcUse;
	if (iSortingColumnTertiary == COLUMN_HISTORY_USE) pData->m_cSortTertiary = pHistoryItem->m_pcUse;
// -------------------------------- use

// -------------------------------- ratio cpu%

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

	if (theApp.m_bNoCleanup)
	{
		_snprintf_s(cBuffer,50,_TRUNCATE,cPercString,dRatioPerc);
		if (dRatioPerc > 100) dRatioPerc = 100;							// here !!! after the value but before the bar value is set.
		if (dRatioPerc < 0) dRatioPerc = 0;
	}
	else
	{
		if (dRatioPerc > 100) dRatioPerc = 100;							// here !!! after the value but before the bar value is set.
		if (dRatioPerc < 0) dRatioPerc = 0;
		_snprintf_s(cBuffer,50,_TRUNCATE,cPercString,dRatioPerc);
	}

	if (cComma != '.')
	{
		char *cFind = strchr(cBuffer, '.'); 
		if (cFind) *cFind = cComma;
	}

	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_RATIO,cBuffer);

	if (iSortingColumnPrimary == COLUMN_HISTORY_RATIO) pData->m_fSortPrimary = dRatioPerc;
	if (iSortingColumnSecondary == COLUMN_HISTORY_RATIO) pData->m_fSortSecondary = dRatioPerc;
	if (iSortingColumnTertiary == COLUMN_HISTORY_RATIO) pData->m_fSortTertiary = dRatioPerc;

// -------------------------------- ratio cpu%

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

//	iColumn = m_iColumnOrder[COLUMN_HISTORY_STATUS];
//	if (iColumn >= 0) pListCtrl->SetItemText(*piRow,iColumn,cBuffer);
	if (iSortingColumnPrimary == COLUMN_HISTORY_STATUS) pData->m_iSortPrimary =  iStateSort;
	if (iSortingColumnSecondary == COLUMN_HISTORY_STATUS) pData->m_iSortSecondary =  iStateSort;
	if (iSortingColumnTertiary == COLUMN_HISTORY_STATUS) pData->m_iSortTertiary =  iStateSort;
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_STATUS,cBuffer);
// -------------------------------- state

// -------------------------------- computer
	m_list.SetItemText(iPosAdd,COLUMN_HISTORY_COMPUTER,pHistoryItem->m_pcComputer);
	if (iSortingColumnPrimary == COLUMN_HISTORY_COMPUTER) pData->m_cSortPrimary = pHistoryItem->m_pcComputer;
	if (iSortingColumnSecondary == COLUMN_HISTORY_COMPUTER) pData->m_cSortSecondary = pHistoryItem->m_pcComputer;
	if (iSortingColumnTertiary == COLUMN_HISTORY_COMPUTER) pData->m_cSortTertiary = pHistoryItem->m_pcComputer;
// -------------------------------- computer
	m_list.SetItemData(iPosAdd,(DWORD_PTR) pData);
}

void CDlgHistory::ClipBoard(bool bAll)
{
    HGLOBAL		hglbCopy; 
    LPTSTR		lptstrCopy; 
	CString		sTxt,sClipBoard;
	int			iItems, iNr;
	bool		bSelected;
	
	if ( !OpenClipboard() ) return;
	if( !EmptyClipboard() ) return;
 
	iItems = m_list.GetItemCount();

	for (int iCountItems = 0; iCountItems < iItems; iCountItems++)
	{
		bSelected = m_list.m_pSelection->SelectionSelected(&m_list, iCountItems);

		if (bAll) bSelected = true;
		if (bSelected)
		{
			for (int iCountSearch = 0; iCountSearch < NUM_REMOTE_HISTORY_COLUMNS; iCountSearch++)
			{
				for (int iCount = 0; iCount < NUM_REMOTE_HISTORY_COLUMNS; iCount++)
				{
					iNr = iCount;
//					iNr = m_iColumnOrder[iCount];
//					if (iNr > NUM_REMOTE_HISTORY_COLUMNS) break;
//					if (iNr >=0)
					{
						if (iCountSearch == iNr)
						{
							sTxt = m_list.GetItemText(iCountItems,iNr);
//							RemoveSpecialText(&sTxt);
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

void CDlgHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HISTORY, m_list);
	DDX_Control(pDX, IDC_LIST_HISTORY_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_LIST_HISTORY_PROJECTS, m_listProjects);
	DDX_Control(pDX, IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, m_checkSingleMultiple);
	DDX_Control(pDX, IDC_HISTORY_DATETIME_FROM, m_dateTimeFrom);
	DDX_Control(pDX, IDC_HISTORY_DATETIME_TO, m_dateTimeTo);
	DDX_Control(pDX, IDC_TEXT_HISTORY_GROUP_COMPUTERS, m_textComputers);
	DDX_Control(pDX, IDC_TEXT_HISTORY_GROUP_PROJECTS, m_textProjects);
}


BEGIN_MESSAGE_MAP(CDlgHistory, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(UWM_MSG_LONG_HISTORY,OnLongHistory)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnChangedFont)
	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnChangedFont)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_HISTORY, &CDlgHistory::OnLvnColumnclickListHistory)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_SINGLE_MULTIPLE, &CDlgHistory::OnBnClickedCheckGraphicSingleMultiple)
	ON_LBN_SELCHANGE(IDC_LIST_HISTORY_COMPUTERS, &CDlgHistory::OnLbnSelchangeListHistoryComputers)
	ON_LBN_SELCHANGE(IDC_LIST_HISTORY_PROJECTS, &CDlgHistory::OnLbnSelchangeListHistoryProjects)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_HISTORY_DATETIME_FROM, &CDlgHistory::OnDtnDatetimechangeHistoryDatetimeFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_HISTORY_DATETIME_TO, &CDlgHistory::OnDtnDatetimechangeHistoryDatetimeTo)

	ON_COMMAND(ID_HISTORY_COPYALLTOCLIPBOARD, OnClipBoardAll)
	ON_COMMAND(ID_HISTORY_COPYSELECTEDTOCLIPBOARD, OnClipBoardSelected)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_HISTORY, &CDlgHistory::OnNMRClickListHistory)
END_MESSAGE_MAP()

void CDlgHistory::OnTimer(UINT_PTR nIDEvent)
{
	if (!this->IsWindowVisible()) return;

	if (m_bThreadLocked) return;

	if (m_bDraw)
	{
		AddAllItems();
		m_bDraw = false;
	}

	if (m_iTimer++ > m_iMaxTimer)
	{
		ClearHistory(m_plHistoryLongFetch);
		if (m_plHistoryLongFetch == NULL)
		{
			m_plHistoryLongFetch = new std::deque<CHistoryItem*>;
		}
		m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_LONG_HISTORY, (WPARAM) this->m_hWnd, (LPARAM) m_plHistoryLongFetch);
		m_bThreadLocked = true;
		m_iTimer = 0;
		m_iMaxTimer+=4;
		if (m_iMaxTimer > MAX_HISTORY_DLG_TIMER)
		{
			m_iMaxTimer = MAX_HISTORY_DLG_TIMER;
		}
		m_bDraw = true;
		SafeColumnWidth();
	}

}

void CDlgHistory::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		m_iTimer = 1000;
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}

void CDlgHistory::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

void CDlgHistory::OnSizing(UINT fwSide, LPRECT pRect)
{
	LONG lTop, lBottom,lLeft, lRight, lWidth, lHeight;
	bool bSetWindow = false;
	bool bSetWindowWidth = false;
	bool bSetWindowHeight = false;

// limit the minimum size of the dialog

	lTop	= pRect->top;
	lBottom = pRect->bottom;
	lLeft	= pRect->left;
	lRight	= pRect->right;
	lWidth	= lRight - lLeft;
	lHeight	= lBottom - lTop;

	if (lWidth < 100)
	{
		bSetWindow = true;
		bSetWindowWidth = true;
	}
	if (lHeight < 100)
	{
		bSetWindow = true;
		bSetWindowHeight = true;
	}

	if (bSetWindowWidth)
	{
		pRect->right  = pRect->left+ 101;
	}
	if (bSetWindowHeight)
	{
		pRect->bottom = pRect->top + 101;
	}

	CDialog::OnSizing(fwSide, pRect);
}

LRESULT CDlgHistory::OnLongHistory(WPARAM wParam,LPARAM lParam)
{
	CDateConvert dateConvert;
	CUseCpuGpu	CpuGpu;
	CString		sError;

	ClearHistory(m_plHistoryLong);
	if (m_plHistoryLong == NULL)
	{
		m_plHistoryLong = new std::deque<CHistoryItem*>;
	}

	std::deque<CHistoryItem*> *plHistoryLongTemp;

	plHistoryLongTemp = m_plHistoryLong;
	m_plHistoryLong = m_plHistoryLongFetch;
	m_plHistoryLongFetch = plHistoryLongTemp;

	// add computer and project to list
	for (int iPos = 0; iPos < (int) m_plHistoryLong->size(); iPos++)
	{
		if (m_listComputers.FindStringExact(0,m_plHistoryLong->at(iPos)->m_pcComputer) ==  LB_ERR)	// check if already in the list
		{
			m_listComputers.AddString(m_plHistoryLong->at(iPos)->m_pcComputer,0);
		}
		if (m_listProjects.FindStringExact(0,m_plHistoryLong->at(iPos)->m_pcProject) ==  LB_ERR)	// check if already in the list
		{
			m_listProjects.AddString(m_plHistoryLong->at(iPos)->m_pcProject,0);
		}
	}

	m_bThreadLocked = false;
	return 0;
}

LRESULT CDlgHistory::OnChangedFont(WPARAM wParam,LPARAM lParam)
{
	m_list.PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam, lParam);

//	LOGFONT	*pLp;
//	pLp = (LOGFONT *) wParam;
//
//	delete m_pFont;
//	m_pFont = new CFont;
//	m_pFont->CreateFontIndirect(pLp);
//
//	m_list.SetFont(m_pFont,TRUE);
//	m_list.Invalidate(TRUE);

	return 0;
}


void CDlgHistory::OnLvnColumnclickListHistory(NMHDR *pNMHDR, LRESULT *pResult)
{
	int		iColumn;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	iColumn = pNMLV->iSubItem;

	m_list.m_pSortColumn->ClickItem(pNMHDR, pResult, iColumn);

	m_bDraw = true;
	*pResult = 0;
}

void CDlgHistory::OnNMRClickListHistory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_list.m_pSelection->SelectionSetRight(pNMItemActivate->iItem);
//	m_bDraw = true;
	*pResult = 0;
}

void CDlgHistory::OnBnClickedCheckGraphicSingleMultiple()
{
	int		iCheck;

	iCheck = m_checkSingleMultiple.GetCheck();
	theApp.WriteProfileInt(registrySectionHistoryDialog, registrySingleMultiple, iCheck);

	m_listProjects.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
	m_listComputers.PostMessage(UWM_MSG_SELECTION_MODE,0,iCheck);
}


void CDlgHistory::OnLbnSelchangeListHistoryComputers()
{
	m_bDraw = true;
}


void CDlgHistory::OnLbnSelchangeListHistoryProjects()
{
	m_bDraw = true;
}


void CDlgHistory::OnDtnDatetimechangeHistoryDatetimeFrom(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	m_bDraw = true;
	*pResult = 0;
}


void CDlgHistory::OnDtnDatetimechangeHistoryDatetimeTo(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	m_bDraw = true;
	*pResult = 0;
}

void CDlgHistory::OnContextMenu(CWnd* pWnd, CPoint point) 
{ 
	if (m_list.m_pSelection->SelectionNumber() == 0) return;

	//our menu
	CMenu contextMenu; 
	//the actual popup menu which is the first
	//sub menu item of IDR_CONTEXT
	CMenu* tracker; 

	CPoint startPoint;

	// check if there are any selected items.
//	if (m_pSelection->SelectionNumber() == 0) return;

	startPoint = point;
	startPoint.y += 10;

	//at first we'll load our menu
	contextMenu.LoadMenu(IDR_MENU_HISTORY_DLG_LONG); 
 
	//we have to translate the mouse coordinates first:
	//these are relative to the window (or view) but the context menu
	//needs coordinates that are relative to the whole screen
	ClientToScreen(&point);
 
	//tracker now points to IDR_CONTEXT's first submenu 
	//GetSubMenu(0) -&gt; returns the first submenu,
	// GetSubMenu(1)-&gt; second one etc..
	tracker = contextMenu.GetSubMenu(0); 

//Translate
	tracker->ModifyMenu(ID_HISTORY_COPYALLTOCLIPBOARD,MF_STRING,ID_HISTORY_COPYALLTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopyAll]);
	tracker->ModifyMenu(ID_HISTORY_COPYSELECTEDTOCLIPBOARD,MF_STRING,ID_HISTORY_COPYSELECTEDTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopySelected]);

//Translate

	//show the context menu
	tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y ,this);//; AfxGetMainWnd()); 

	//has always to be called (function of base class)
//   CListViewEx::OnRButtonDown(nFlags, point); 
}

void CDlgHistory::OnClipBoardAll()
{
	ClipBoard(true);
}

void CDlgHistory::OnClipBoardSelected()
{
	ClipBoard(false);
}



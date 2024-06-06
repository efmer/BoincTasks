// RemoteTransferView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
#include "ListViewEx.h"
#include "TemplateRemoteTaskView.h"	
#include "TemplateRemoteProjectView.h" 
#include "RemoteCombinedDoc.h"
#include "RemoteCombinedTaskView.h"
#include "RemoteCombinedProjectView.h"
#include "TemplateRemoteMessageView.h"
#include "RemoteCombinedMessageView.h"
#include "TemplateRemoteTransferView.h" 
#include "RemoteCombinedTransferView.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "TemplateRemoteComputerView.h"
#include "RemoteCombinedComputerView.h"
#include "RemoteCombinedNoticesView.h"

#include "RemoteCombinedDoc.h"
#include "DlgPrefMain.h"
#include "DlgProxyMain.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsTransfer.h"
#include "DlgSettingsView.h"
#include "DlgFont.h"
#include "SortColumn.h"
#include "Selection.h"
#include "Translation.h"
#include "MainFrm.h"
#include "StringCopyNew.h"
#include "SortTransfer.h"

#include "CreateHtmlWebServer.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"
#include "WebServerProcess.h"

// CRemoteCombinedTransferView

IMPLEMENT_DYNCREATE(CRemoteCombinedTransferView, CTemplateRemoteTransferView)

CRemoteCombinedTransferView::CRemoteCombinedTransferView()
{
	m_iThreadLocked	= 0;
	m_bThreadBusy	= false;
	m_bFirstTime	= true;
	m_bDocumentIsClosing = false;

//	m_bAllProjectsSelected = true;

	m_pFont = NULL;

	m_pDoc			= NULL;
	m_iNrColomns	= NUM_REMOTE_TRANSFER_COLUMNS;

//	m_hWndPreference = theApp.m_pDlgPrefMain->m_hWnd;
	m_hWndProxy = theApp.m_pDlgProxyMain->m_hWnd;

	CRpcInfoTransferItem *pRpcInfoTransferItem;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pRpcInfoTransferItem = new CRpcInfoTransferItem;
		m_lRpcInfo.push_back(pRpcInfoTransferItem);
	}
}

CRemoteCombinedTransferView::~CRemoteCombinedTransferView()
{
	delete m_pFont;

	for (int iCount = 0; iCount < (int) m_lRpcInfo.size(); iCount++)
	{
		delete m_lRpcInfo.at(iCount);
	}
//	m_lRpcInfo.clear();
}

void CRemoteCombinedTransferView::OnInitialUpdate()
{
	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL,(WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);
	CTemplateRemoteTransferView::OnInitialUpdate();
}

void CRemoteCombinedTransferView::InitialUpdate(CRemoteCombinedDoc* pDoc)
{
	m_pDoc = pDoc;
}

BEGIN_MESSAGE_MAP(CRemoteCombinedTransferView, CTemplateRemoteTransferView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_MESSAGE (UWM_MSG_TIMER_TRANSFER, OnTimerTransfer) 
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_TRANSFER, OnReadyRpc)

	ON_MESSAGE (UWM_MSG_SETTING_TASKS, OnSettingsTasks)

	ON_MESSAGE (UWM_MSG_REMOTE_TRANSFER_SWITCH, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_WINDOW_SHOWN, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_COLUMN_TRANSFERS, OnColumnChanged)

	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelTabView)
	ON_MESSAGE(UWM_MSG_VIEW_REFRESH,OnRefresh)

	ON_MESSAGE (UWM_MSG_GET_FLOATER_DATA, OnFloaterData)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_CPU, OnSetSnoozeCpu)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_GPU, OnSetSnoozeGpu)	

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistorySetWidth)

	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, OnComputerBarSelectionChanged)

	ON_COMMAND(ID_TRANSFER_COPYALLTOCLIPBOARD, &CRemoteCombinedTransferView::OnCopyalltoclipboard)
	ON_COMMAND(ID_TRANSFER_COPYSELECTEDTOCLIPBOARD, &CRemoteCombinedTransferView::OnCopyselectedtoclipboard)

	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnFontChangeAll)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnFontHasChanged)

	ON_MESSAGE (UWM_MSG_PREFERENCE_DIALOG, OnPreferenceDialog)
	ON_MESSAGE (UWM_MSG_PROXY_DIALOG, OnProxyDialog)
	ON_WM_CONTEXTMENU()
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_READ_PROJECT, OnReadyProject)
	ON_COMMAND(ID_WWW_1, &CRemoteCombinedTransferView::OnWww1)
	ON_COMMAND(ID_WWW_2, &CRemoteCombinedTransferView::OnWww2)
	ON_COMMAND(ID_WWW_3, &CRemoteCombinedTransferView::OnWww3)
	ON_COMMAND(ID_WWW_4, &CRemoteCombinedTransferView::OnWww4)
	ON_COMMAND(ID_WWW_5, &CRemoteCombinedTransferView::OnWww5)
	ON_COMMAND(ID_WWW_6, &CRemoteCombinedTransferView::OnWww6)
	ON_COMMAND(ID_WWW_7, &CRemoteCombinedTransferView::OnWww7)
	ON_COMMAND(ID_WWW_8, &CRemoteCombinedTransferView::OnWww8)
	ON_COMMAND(ID_WWW_9, &CRemoteCombinedTransferView::OnWww9)
	ON_COMMAND(ID_WWW_10, &CRemoteCombinedTransferView::OnWww10)
	ON_COMMAND(ID_WWW_11, &CRemoteCombinedTransferView::OnWww11)
	ON_COMMAND(ID_WWW_12, &CRemoteCombinedTransferView::OnWww12)
	ON_COMMAND(ID_WWW_13, &CRemoteCombinedTransferView::OnWww13)
	ON_COMMAND(ID_WWW_14, &CRemoteCombinedTransferView::OnWww14)
	ON_COMMAND(ID_WWW_15, &CRemoteCombinedTransferView::OnWww15)
	ON_COMMAND(ID_WWW_16, &CRemoteCombinedTransferView::OnWww16)
	ON_COMMAND(ID_WWW_17, &CRemoteCombinedTransferView::OnWww17)
	ON_COMMAND(ID_WWW_18, &CRemoteCombinedTransferView::OnWww18)
	ON_COMMAND(ID_WWW_19, &CRemoteCombinedTransferView::OnWww19)
	ON_COMMAND(ID_WWW_20, &CRemoteCombinedTransferView::OnWww20)
	
	ON_COMMAND(ID_TRANSFER_RETRY, &CRemoteCombinedTransferView::OnTransferRetry)
	ON_COMMAND(ID_TRANSFER_RETRYALL, &CRemoteCombinedTransferView::OnTransferRetryall)
	ON_COMMAND(ID_TRANSFER_ABORT, &CRemoteCombinedTransferView::OnTransferAbort)

//	ON_MESSAGE (UWM_MSG_NOTICES_BT_UPDATE, OnBoincTasksVersion)	
//	ON_MESSAGE (UWM_MSG_NOTICES_BT_RULE, OnBoincTasksRule)	
//	ON_MESSAGE (UWM_MSG_NOTICES_BT_RULE, OnBoincTasksRule)	

	ON_MESSAGE (UWM_DLG_SEARCH_COMPUTER_READY, OnSearchComputerReady)	

	ON_MESSAGE (UWM_MSG_WEB_ITEM_SELECTED, OnWebItemSelected)
	ON_MESSAGE(UWM_MSG_WEB_OPERATION,OnWebOperation)
	ON_MESSAGE(UWM_MSG_WEB_SORT,OnWebSort)
// There is no dialog yet	ON_COMMAND(ID_HEADER_SETHEADERITEMS, &CRemoteCombinedTransferView::OnHeaderSetItems)

//	ON_MESSAGE (UWM_MSG_THREAD_SET_RPC_PROJECT_LIST, OnSetProjectList)	

	ON_COMMAND(ID_COMPUTERS_DETECT, &CRemoteCombinedTransferView::OnComputersDetect)

END_MESSAGE_MAP()
// CRemoteCombinedTransferView diagnostics

#ifdef _DEBUG
void CRemoteCombinedTransferView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteCombinedTransferView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CRemoteCombinedTransferView message handlers

LRESULT CRemoteCombinedTransferView::OnTimerTransfer(WPARAM parm1, LPARAM parm2)
{
	char	*pcComputerId;
	int iViewActive, iNrRpc, iThreadId;
	HWND hWnd;
	CRemoteCombinedTransferView *pView;
	bool	bVisible;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	bVisible = theApp.m_pMainWnd->IsWindowVisible() == TRUE;

	CheckAndSaveColumnsWidth();

	if (m_bThreadBusy)		// check for lockup
	{
		if (bVisible && m_pDoc->m_iViewActive == TRANSFER_VIEW_ACTIVE)
		{
			CString sFormat;
			if (m_iThreadLocked > DELAY_SHOW_UPDATE_TIMER) sFormat.Format(" %d", m_iThreadLocked);
			sFormat = gszTranslation[PosWindowStatusMessageStatusUpdating] + sFormat;
			theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
			sFormat.ReleaseBuffer();
		}
		if (m_iThreadLocked++ > theApp.m_iThreadTimeout) {m_iThreadLocked = 0; m_bThreadBusy = false;}
		return 0;
	}
	m_iThreadLocked = 0;

	if (m_bThreadBusy) return 0;
	if (m_pDoc == NULL) return 0;

	iNrRpc = m_pDoc->GetRpcSelectedComputers();
	iViewActive = m_pDoc->m_iViewActive;

	m_iSendRpcCount = 0;		//iNrRpc;
	m_iReadyRpcCount = 0;
	m_iTransferTotal = 0;

	pView = m_pDoc->m_pTransferView;
	hWnd = pView->m_hWnd;

	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		m_pDoc->GetRpcThreadId(&iThreadId, iCount);
		if (m_pDoc->GetComputerIdStringWithIdNumber(iThreadId, &pcComputerId))
		{// get the computer id string for the thread
			int iResult = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, (WPARAM) pcComputerId, 0);
			if (iResult == TRUE)
		//	if (theApp.m_pMainFrame->m_dlgBarComputer.FindSelected(&sComputerId))	// check if the computer is selected
			{
				::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_TRANSFER,(WPARAM) hWnd,0); // return naar project view!
				m_iSendRpcCount++;
				m_bThreadBusy = true;	
			}
		}
	}
	if (m_iSendRpcCount == 0)
	{
		CListCtrl& listCtrl = GetListCtrl();
		listCtrl.DeleteAllItems();
	}
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnSwitch(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (parm2 == REFRESH_VIEW_NOW) 
	{
//		m_iTimerWu = 1000;	// direct action
	}

	theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnSettingsTasks(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_pDoc != NULL) m_pDoc->SettingsTasks(parm1 != 0);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnReadyRpc(WPARAM parm1, LPARAM parm2)
{
	LV_ITEM				 lvi;
	UINT				iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary,  iSortingColumnTertiary, iSortingOptionTertiary;
	int					iNrTransfer, iItemsAdd, iItemsAddTemp, iAddPos, iNrItems, iStatus, iRow, iCount, iTemp;
	CRpcThreadReturn			*pThreadReturn;
	bool				bOverride = false, bSorting, bNumberChanged = false;//, bSelectedFound = false;
	CThreadRpc			*pThreadRpc;
	char				*pcComputerId;

	iStatus = (int) parm1;
	pThreadReturn = (CRpcThreadReturn *)	parm2;

	if (m_bDocumentIsClosing)
	{
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusClosing]);
		delete pThreadReturn;
		return 0;			// everything is closing down
	}
	
	iNrTransfer = pThreadReturn->m_iNumber;
	m_iTransferTotal+= iNrTransfer;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_iNrTransfer = iNrTransfer;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_pcComputerId = pThreadReturn->m_pcComputerId;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_pThread = pThreadReturn->m_pThread;
	m_lRpcInfo.at(m_iReadyRpcCount)->m_iStatusRpc = iStatus;
	
	m_iReadyRpcCount++;

	theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_CONNECT, (WPARAM) pThreadReturn->m_pcComputerId, pThreadReturn->m_iStatus);

	delete pThreadReturn;
	pThreadReturn = NULL;

	if (m_iReadyRpcCount != m_iSendRpcCount) return 0;	// wait for all results to come in

	CDlgSettingsView *pSettingsView;
	pSettingsView = theApp.m_pDlgSettingsView;
	m_bUseHorizontalGrid = pSettingsView->m_bUseHorizontalGrid;
	m_bUseVerticalGrid = pSettingsView ->m_bUseVerticalGrid;

	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary,  &iSortingColumnTertiary, &iSortingOptionTertiary);
	
	// setup sorting
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		m_lRpcInfo.at(iCount)->m_iSortIndex= iCount;
	}
	
	bool bSortingAZ = true;
	if (iSortingColumnSecondary == COLUMN_TRANSFER_COMPUTER)	if (iSortingOptionSecondary == SORTING_ZA) bSortingAZ = false;
	if (iSortingColumnTertiary  == COLUMN_TRANSFER_COMPUTER)	if (iSortingOptionTertiary  == SORTING_ZA) bSortingAZ = false;

	// no sorting if there is less than 2
	if (m_iReadyRpcCount > 1)
	{
		bSorting = true;
		while (bSorting)
		{
			bSorting = false;
			for (iCount = 0; iCount < m_iReadyRpcCount-1; iCount++)
			{
				if (bSortingAZ)
				{
					if (strcmp(m_lRpcInfo.at(m_lRpcInfo.at(iCount)->m_iSortIndex)->m_pcComputerId, m_lRpcInfo.at(m_lRpcInfo.at(iCount+1)->m_iSortIndex)->m_pcComputerId) > 0)
					{
						iTemp = m_lRpcInfo.at(iCount)->m_iSortIndex;
						m_lRpcInfo.at(iCount)->m_iSortIndex = m_lRpcInfo.at(iCount+1)->m_iSortIndex;
						m_lRpcInfo.at(iCount+1)->m_iSortIndex = iTemp;
						bSorting = true;
					}
				}
				else
				{
					if (strcmp(m_lRpcInfo.at(m_lRpcInfo.at(iCount)->m_iSortIndex)->m_pcComputerId, m_lRpcInfo.at(m_lRpcInfo.at(iCount+1)->m_iSortIndex)->m_pcComputerId) < 0)
					{
						iTemp = m_lRpcInfo.at(iCount)->m_iSortIndex;
						m_lRpcInfo.at(iCount)->m_iSortIndex = m_lRpcInfo.at(iCount+1)->m_iSortIndex;
						m_lRpcInfo.at(iCount+1)->m_iSortIndex = iTemp;
						bSorting = true;
					}
				}
			}
		}
	}

	m_pDoc->DisplayStatus(m_iReadyRpcCount, &m_lRpcInfo.at(0)->m_iStatusRpc);// , m_rpcInfo.m_pcComputerId[0]);


	CListCtrl& listCtrl = GetListCtrl();

//	if (iNrTransfers == 0) return FALSE;

	iNrItems = listCtrl.GetItemCount();
	iItemsAdd =  iNrItems - m_iTransferTotal;

//	if (m_bFirstTime)
//	{
//
//	}

//	if (!m_bFirstTime)
	{
		if (iItemsAdd > 0)
		{
			bNumberChanged = true;
			// delete list items
			bOverride = true;
			while (iItemsAdd > 0)
			{
				listCtrl.DeleteItem(iNrItems-1);
				iNrItems--;
				iItemsAdd--;
			}
			
		}
		else
		{
			if (iItemsAdd < 0)
			{
				bNumberChanged = true;
				iItemsAddTemp = iItemsAdd;
				// add list items
				bOverride = true;
				while (iItemsAddTemp < 0)
				{
					lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
					iAddPos =  iNrItems-1;
					if (iAddPos < 0) iAddPos = 0;
					lvi.iItem = iAddPos;							// add at the end
					lvi.iSubItem = 0;
					lvi.pszText = "";// info.m_pszProject;	

					lvi.iImage = iAddPos;							// add at the end
					lvi.stateMask = LVIS_STATEIMAGEMASK;
					lvi.state = INDEXTOSTATEIMAGEMASK(1);

					lvi.lParam = (LPARAM) &m_dataDummy;

					listCtrl.InsertItem(&lvi);
					iNrItems++;
					iItemsAddTemp++;
				}
			}
		}
	}

	// for every computer

	iRow = 0;
	for (int iComputer = 0; iComputer < m_iReadyRpcCount; iComputer++)
	{
		pThreadRpc = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_pThread;
		iNrTransfer = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_iNrTransfer;
		pcComputerId = m_lRpcInfo.at(m_lRpcInfo.at(iComputer)->m_iSortIndex)->m_pcComputerId;
		DisplayRpc(pThreadRpc, iNrTransfer, &iRow, pcComputerId, iComputer);
	}

	m_bFirstTime = false;

	if (bNumberChanged)	Invalidate();

	if (m_iTransferTotal > 1)
	{
		DWORD iSorting = (iSortingColumnPrimary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionPrimary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnSecondary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionSecondary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnTertiary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionTertiary&0xf);
		listCtrl.SortItems(TransferSorting,iSorting);
	}
	DisplayMakeLineColors(theApp.m_pDlgSettingsView->m_bAlternatingStriped, theApp.m_pDlgSettingsView->m_bUseSkin, theApp.m_pDlgSettingsView->m_iUseSkin);

	if (g_bWebServerActive)
	{
		if (g_pcWebServerHtml == NULL)
		{
			CCreateHtmlWebServer createHtml;
			char *pcHtml;
			if (createHtml.Create(PosBarViewSelectTransfers, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TRANSFER_COLUMNS, NULL, PosGroupViewTasksTransfer, &pcHtml))
			{
				g_pcWebServerHtml = pcHtml;
				g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_TRANSFERS);
				g_tWebServerHtml = GetTickCount()/100;
			}
		}
	}
	if (g_bCloudServerRequestData)
	{
		if (g_pcCloudServerHtml == NULL)
		{
			if (g_iCloudServerHtmlTabProcessed[TAB_TRANSFERS] == TRUE)
			{
				g_iCloudServerHtmlTabProcessed[TAB_TRANSFERS] = FALSE;
				CCreateHtmlWebServer createHtml;
				char *pcHtml;
				if (createHtml.Create(PosBarViewSelectTransfers, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TRANSFER_COLUMNS, NULL, PosGroupViewTasksTransfer, &pcHtml))
				{
					g_pcCloudServerHtml = pcHtml;
					g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_TRANSFERS);
					g_tCloudServerHtml = GetTickCount()/100;
				}
			}
		}
	}

	m_bThreadBusy = false;

	CString sTxt;
	sTxt = TEXT_STATUS_WAITING;
	switch (m_iWaitingTurner)
	{
		case 0:
			sTxt += " -";
		break;
		case 1:
			sTxt += " \\";
		break;
		case 2:
			sTxt += " |";
		break;
		case 3:
			sTxt += " /";
		break;
	}
	m_iWaitingTurner++;
	if (m_iWaitingTurner > 3) m_iWaitingTurner = 0;

	theApp.m_pMainFrame->SetBoincTasksStatus(sTxt.GetBuffer());
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnColumnChanged(WPARAM parm1, LPARAM parm2)
{
	HWND hHnd;
	int	 *pColumn;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_bFirstTime) return 0;		// wait for the 1e init

	hHnd = this->m_hWnd;

	if (hHnd != (HWND) parm2)
	{
		CListCtrl& listCtrl = GetListCtrl();
		pColumn = (int *) parm1;		
		for (int iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
		{
			listCtrl.SetColumnWidth(iCount, *pColumn);
			pColumn++;
		}
	}

	return 0;
}

void CRemoteCombinedTransferView::OnContextMenu(CWnd* pWnd, CPoint point) 
{ 
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	CRect	rect;
	int		iRetVal;
	CPoint	clientPoint, startPoint;
	bool	bHeader;

	bHeader = false;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	clientPoint = point;
	ScreenToClient(&clientPoint);
	HDHITTESTINFO hdHitIfo;
	memset(&hdHitIfo, 0, sizeof(HDHITTESTINFO));
	for(int i = 0; i < pHeader->GetItemCount(); i++)
	{
		hdHitIfo.pt = clientPoint;
		// The hit test depends on whether the header item is visible.
		iRetVal = pHeader->HitTest(&hdHitIfo);
		if (hdHitIfo.iItem >= 0)
		{
			bHeader = true;
		}
	}

	startPoint = point;
	startPoint.y += 10;

	//our menu
	CMenu contextMenu; 
	//the actual popup menu which is the first
	//sub menu item of IDR_CONTEXT
	CMenu* tracker; 

	if (bHeader)
	{
		contextMenu.LoadMenu(IDR_MENU_CONTEXT_HEADER); 	
		tracker = contextMenu.GetSubMenu(0); 
		tracker->ModifyMenu(ID_HEADER_SETHEADERITEMS,MF_STRING,ID_HEADER_SETHEADERITEMS,gszTranslation[PosPopUpHeaderEnable]);
		tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y , AfxGetMainWnd()); 
		return;
	}
	
	CListCtrl& listCtrl = GetListCtrl();

	m_contextMenu.SetColumnOrder(NUM_REMOTE_TRANSFER_COLUMNS, &m_iColumnOrder[0]);
	m_contextMenu.ContextMenuCombined(CONTEXT_MENU_TRANSFERS, m_pDoc, &listCtrl, m_pSelection, point, this, IDR_MENU_TRANSFER);
} 

LRESULT CRemoteCombinedTransferView::OnReadyProject(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_contextMenu.Ready((int) parm1);
	return 0;
}

void CRemoteCombinedTransferView::OnWww1() {m_contextMenu.WWW(0);}
void CRemoteCombinedTransferView::OnWww2() {m_contextMenu.WWW(1);}
void CRemoteCombinedTransferView::OnWww3() {m_contextMenu.WWW(2);}
void CRemoteCombinedTransferView::OnWww4() {m_contextMenu.WWW(3);}
void CRemoteCombinedTransferView::OnWww5() {m_contextMenu.WWW(4);}
void CRemoteCombinedTransferView::OnWww6() {m_contextMenu.WWW(5);}
void CRemoteCombinedTransferView::OnWww7() {m_contextMenu.WWW(6);}
void CRemoteCombinedTransferView::OnWww8() {m_contextMenu.WWW(7);}
void CRemoteCombinedTransferView::OnWww9() {m_contextMenu.WWW(8);}
void CRemoteCombinedTransferView::OnWww10() {m_contextMenu.WWW(9);}
void CRemoteCombinedTransferView::OnWww11() {m_contextMenu.WWW(10);}
void CRemoteCombinedTransferView::OnWww12() {m_contextMenu.WWW(11);}
void CRemoteCombinedTransferView::OnWww13() {m_contextMenu.WWW(12);}
void CRemoteCombinedTransferView::OnWww14() {m_contextMenu.WWW(13);}
void CRemoteCombinedTransferView::OnWww15() {m_contextMenu.WWW(14);}
void CRemoteCombinedTransferView::OnWww16() {m_contextMenu.WWW(15);}
void CRemoteCombinedTransferView::OnWww17() {m_contextMenu.WWW(16);}
void CRemoteCombinedTransferView::OnWww18() {m_contextMenu.WWW(17);}
void CRemoteCombinedTransferView::OnWww19() {m_contextMenu.WWW(18);}
void CRemoteCombinedTransferView::OnWww20() {m_contextMenu.WWW(19);}

void CRemoteCombinedTransferView::OnTransferRetry()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostTransferOperation(TRANSFER_OP_RETRY);
}

void CRemoteCombinedTransferView::OnTransferRetryall()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	PostTransferOperation(TRANSFER_OP_RETRY_ALL);
}

void CRemoteCombinedTransferView::OnTransferAbort()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	int iReturn = AfxMessageBox(gszTranslation[PosDialogTransferAbortQuestionAbort], MB_YESNO | MB_ICONEXCLAMATION);
	if (iReturn != IDYES) return;
	PostTransferOperation(TRANSFER_OP_ABORT);
}

void CRemoteCombinedTransferView::PostTransferOperation(int iOperation)
{
	bool	bSelected;
	int		iCount, iNrItems, iOperationSend, iOrder;

	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	if (iOperation == TRANSFER_OP_RETRY_ALL) iOperationSend = TRANSFER_OP_RETRY;
	else iOperationSend = iOperation;

	// now copy all the project names to a seperate array, just in case something changes

	if (iNrItems > MAX_TRANSFER) iNrItems = MAX_TRANSFER;

	m_iSelectedTransferItems = 0;
	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		bSelected =m_pSelection->SelectionSelected(&listCtrl, iCount);
//		iState = listCtrl.GetItemState(iCount,LVIS_SELECTED);

		if (iOperation == TRANSFER_OP_RETRY_ALL) bSelected = true;
		if (bSelected)
		{
			iOrder = m_iColumnOrder[COLUMN_TRANSFER_COMPUTER];if (iOrder >=  0) m_sSelectedTransferItemsComputer[m_iSelectedTransferItems] = listCtrl.GetItemText(iCount, iOrder);
			iOrder = m_iColumnOrder[COLUMN_TRANSFER_FILE];if (iOrder >= 0) m_sSelectedTransferItemsName[m_iSelectedTransferItems] = listCtrl.GetItemText(iCount, iOrder);
			m_iSelectedTransferItems++;
		}
	}

	if (m_iSelectedTransferItems == 0) return;

	if (m_iSelectedTransferItems > 100) m_iSelectedTransferItems = 100;

	CWaitCursor wait;
	for (iCount = 0; iCount < m_iSelectedTransferItems; iCount++)
	{
		bool bFound;
		int  iThreadId;
		bFound = m_pDoc->GetRpcThreadId(&m_sSelectedTransferItemsComputer[iCount], &iThreadId);
		if (bFound)
		{
			char *pcSelectedTransferItemsName;
			CStringCopyNew(&pcSelectedTransferItemsName, m_sSelectedTransferItemsName[iCount].GetBuffer(), false);  m_sSelectedTransferItemsName[iCount].ReleaseBuffer();
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_TRANSFER_OPERATION_BYNAME, iOperationSend, (LPARAM) pcSelectedTransferItemsName);
		}
		Sleep(100);
	}	
}

LRESULT CRemoteCombinedTransferView::OnPreferenceDialog(WPARAM parm1, LPARAM parm2)
{
	CListCtrl& listCtrl = GetListCtrl();
	CString sComputer;
	int	iThreadId, nItem;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	nItem = 0;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = listCtrl.GetNextSelectedItem(pos);
	}


	sComputer = listCtrl.GetItemText(nItem,m_iColumnOrder[COLUMN_TRANSFER_COMPUTER]);
	bool bFound = m_pDoc->GetRpcThreadId(&sComputer, &iThreadId);
	if (!bFound)
	{
		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
		if (iThreadId > 0) bFound = true;
	}
	if (bFound)
	{
		// found computer, now tell the Preference Dialog
		m_pDoc->m_pDlgPrefMain->PostMessage(UWM_MSG_PREFERENCE_OPEN_DIALOG, iThreadId, 0);
		return 0;
	}
	
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnProxyDialog(WPARAM parm1, LPARAM parm2)
{
	CListCtrl& listCtrl = GetListCtrl();
	CString sComputer;
	int	iThreadId, nItem;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	nItem = 0;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = listCtrl.GetNextSelectedItem(pos);
	}


	sComputer = listCtrl.GetItemText(nItem,m_iColumnOrder[COLUMN_TRANSFER_COMPUTER]);
	bool bFound = m_pDoc->GetRpcThreadId(&sComputer, &iThreadId);
	if (!bFound)
	{
		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
		if (iThreadId > 0) bFound = true;
	}
	if (bFound)
	{
		::PostMessage(m_hWndProxy,UWM_MSG_PROXY_OPEN_DIALOG, iThreadId, 0);
		return 0;
	}
	
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnSelTabView(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_pDoc == NULL) return 0;
	m_pDoc->OnShowView((int) wParam, (int) lParam);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam)
{
	CString *psComputer;
	int		iThreadId, iReconnect;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	psComputer = (CString *) wParam;
	iReconnect = (int) lParam;

	if (psComputer->GetLength() > 0)
	{
		m_pDoc->GetRpcThreadId(psComputer, &iThreadId);
		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,0);
	}
	else
	{
		int iNrRpc = m_pDoc->GetRpcSelectedComputers();
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);	
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,iReconnect);
		}
	}

	theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	return 0;
}

void CRemoteCombinedTransferView::OnHeaderSetItems()
{
	theApp.m_pDlgSettingsMain->SetActivePage(theApp.m_pDlgSettingsTransfer);
	theApp.m_pDlgSettingsMain->ShowWindow(SW_SHOW);
}

LRESULT CRemoteCombinedTransferView::OnFloaterData(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_GET_FLOATER_DATA, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_CPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return -1;			// everything is closing down.
	return m_pDoc->m_pProjectView->PostMessage(UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, wParam, lParam);
}

LRESULT CRemoteCombinedTransferView::OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, wParam, lParam);
	return 0;
}

//LRESULT CRemoteCombinedTransferView::OnBoincTasksVersion(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_UPDATE,parm1,parm2);
//	return 0;
//}

//LRESULT CRemoteCombinedTransferView::OnBoincTasksRule(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_RULE,parm1,parm2);
//	return 0;
//}

LRESULT CRemoteCombinedTransferView::OnSearchComputerReady(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pComputerView->SendMessage(UWM_DLG_SEARCH_COMPUTER_READY,parm1,parm2);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnFontChangeAll(WPARAM wParam,LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->m_pComputerView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pProjectView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTransferView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pMessageView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);

	return 0;
}

LRESULT CRemoteCombinedTransferView::OnFontHasChanged(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLf;
	pLf = (LOGFONT *) wParam;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLf);
	SetFont(m_pFont,TRUE);

	return 0;
}

void CRemoteCombinedTransferView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	PostTransferOperation(TRANSFER_OP_RETRY);
}

void CRemoteCombinedTransferView::OnSize(UINT nType, int cx, int cy )
{
	theApp.m_pMainFrame->m_dlgBarComputer.PostMessage(UWM_MSG_NEW_SIZE, cx, cy);
	CTemplateRemoteTransferView::OnSize(nType, cx, cy );
}

void CRemoteCombinedTransferView::OnCopyalltoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	ClipBoard(true);
}

void CRemoteCombinedTransferView::OnCopyselectedtoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	ClipBoard(false);
}

LRESULT CRemoteCombinedTransferView::OnWebItemSelected(WPARAM wParam,LPARAM lParam)
{
	int	iRow, iOption;
	CHtmlExchange *pHtmlExchange;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	pHtmlExchange = (CHtmlExchange *) lParam;
	iRow = pHtmlExchange->m_iRow;
	iOption = pHtmlExchange->m_iOption;

	m_pSelection->SelectionSet(iRow, iOption);

	if (pHtmlExchange->m_bSendData)
	{
		if (!m_bThreadBusy)
		{
			CListCtrl& listCtrl = GetListCtrl();
			CCreateHtmlWebServer createHtml;
			createHtml.Create(PosBarViewSelectTransfers, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TRANSFER_COLUMNS, NULL, PosGroupViewTasksTransfer, &pHtmlExchange->m_pcHtml);
			pHtmlExchange->tTime = GetTickCount()/100;
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CRemoteCombinedTransferView::OnWebOperation(WPARAM parm1, LPARAM parm2)
{
	int iOperation;

	iOperation = (int) parm1;
	PostTransferOperation(iOperation);
	return 0;
}

LRESULT CRemoteCombinedTransferView::OnWebSort(WPARAM parm1, LPARAM parm2)
{
	char	*pcSort;
	BOOL	bHtml;
	char	*pcHtml = NULL;

	pcSort = (char *) parm2;
	bHtml = (BOOL) parm1;

	m_pSortColumn->WebSorting(pcSort);

	if (bHtml == FALSE) return NULL;

	if (!m_bThreadBusy)
	{
		CListCtrl& listCtrl = GetListCtrl();
		CCreateHtmlWebServer createHtml;
		createHtml.Create(PosBarViewSelectTransfers, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TRANSFER_COLUMNS, NULL, PosGroupViewTasksTransfer, &pcHtml);
	}
	return (LRESULT) pcHtml;
}

//LRESULT CRemoteCombinedTransferView::OnSetProjectList(WPARAM parm1, LPARAM parm2)
//{
//	m_sProjectsSelected = *(std::string*) parm2;
//	if (m_sProjectsSelected.length() == 0)	m_bAllProjectsSelected = true;
//	else m_bAllProjectsSelected = false;
//
//	return 0;
//}

void CRemoteCombinedTransferView::OnComputersDetect()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SELECTED,BAR_SELECTED_COMPUTERS,0);
	m_pDoc->m_pComputerView->PostMessage(WM_COMMAND, ID_COMPUTERS_DETECT,0);
}
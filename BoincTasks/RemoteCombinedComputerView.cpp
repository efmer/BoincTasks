// RemoteTransferView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "xml.h"
#include "threadrpc.h"
#include "ThreadRpcClient.h"
#include "ListViewEx.h"					// base class for BoinCComputerView
#include "TemplateRemoteTaskView.h"		// base class for BoinCComputerView
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
#include "DlgPrefMain.h"
#include "DlgSettingsView.h"
#include "DlgSettingsExpert.h"
#include "DlgSettingsGeneral.h"
#include "DlgPrefMain.h"
#include "DlgProxyMain.h"
#include "DlgProperties.h"
#include "DlgFont.h"
#include "SortColumn.h"
#include "Selection.h"
#include "Translation.h"
#include "MainFrm.h"
#include "history.h"
#include "TimeString.h"
#include "DateConvert.h"

#include "CreateHtmlWebServer.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"

//#/include "afxcmn.h"
//#//include "winsock2.h"
#include "ListCtrlFindComputers.h"
//#include "afxwin.h"
#include "DlgSearchComputers.h"


#include "computer.h"


//	Computer	Secure			IP Address			Port			Password				BOINC			BOINC platform	TThrottle		Status	

static int _gnRemoteComputerColumnFmt[NUM_REMOTE_COMPUTER_COLUMNS] =
{
	LVCFMT_LEFT	,LVCFMT_LEFT	,LVCFMT_LEFT		,LVCFMT_LEFT	,LVCFMT_LEFT			,LVCFMT_LEFT	,LVCFMT_LEFT	,LVCFMT_LEFT	,LVCFMT_LEFT,	 LVCFMT_LEFT
};

static int _gnRemoteComputerColumnWidth[NUM_REMOTE_COMPUTER_COLUMNS] =
{
	160			,100			,120				,100			,100					,100			,100			,100			,100			,100			,100
};

// CRemoteCombinedComputerView

IMPLEMENT_DYNCREATE(CRemoteCombinedComputerView, CTemplateRemoteComputerView)

CRemoteCombinedComputerView::CRemoteCombinedComputerView()
{
	m_iThreadLocked	= 0;
	m_bThreadBusy	= false;
	m_bFirstTime	= true;
	m_bDocumentIsClosing = false;

	m_pFont = NULL;

	m_pDoc			= NULL;
	m_iNrColomns	= NUM_REMOTE_COMPUTER_COLUMNS;

	m_iTimerComputer = 1000;				// 1000 to ensure direct timer message
	m_LastSwitchedTime = 0;

	CRpcInfoComputerItem *pRpcInfo;
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

		pRpcInfo = new CRpcInfoComputerItem;
		pRpcInfo->m_pThread = NULL;
		pRpcInfo->m_pcComputerId = NULL;
		pRpcInfo->m_timeStats.previous_uptime = -1; // check only the last one if filled.
		m_lComputerRpcInfo.push_back(pRpcInfo);
	}
//	m_hWndPreference = theApp.m_pDlgPrefMain->m_hWnd;
	m_hWndProxy = theApp.m_pDlgProxyMain->m_hWnd;
}

CRemoteCombinedComputerView::~CRemoteCombinedComputerView()
{	
	delete m_pFont;
	for (int iCount = 0; iCount < (int) m_lComputerRpcInfoClient.size(); iCount++)
	{
		//Never do this here as it is made in RemoteCombinedDoc delete m_pRpcInfoClient[iCount]->m_pcComputerId;
		delete m_lComputerRpcInfoClient.at(iCount);
	}
//	m_lComputerRpcInfoClient.clear();
	for (int iCount = 0; iCount < (int) m_lComputerRpcInfo.size(); iCount++)
	{
		//Never do this here as it is made in RemoteCombinedDoc delete m_pRpcInfoClient[iCount]->m_pcComputerId;
		delete m_lComputerRpcInfo.at(iCount);
	}
//	m_lComputerRpcInfo.clear();
}

void CRemoteCombinedComputerView::OnInitialUpdate()
{
	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL,(WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);
	CTemplateRemoteComputerView::OnInitialUpdate();
}

void CRemoteCombinedComputerView::InitialUpdate(CRemoteCombinedDoc* pDoc)
{
	LV_COLUMN			lvc;
	int					i;

	m_pDoc = pDoc;

	CListCtrl& listCtrl = GetListCtrl();

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(i = 0; i<NUM_REMOTE_COMPUTER_COLUMNS; i++)
	{
		m_iColumnOrder[i] = i;
		lvc.iSubItem = i;
		lvc.pszText = gszTranslation[PosViewComputerGroup+i];	
		m_iColumnWidth[i] = ReadColumnWidth(i, _gnRemoteComputerColumnWidth[i]);
		lvc.cx = m_iColumnWidth[i];

		lvc.fmt = _gnRemoteComputerColumnFmt[i];
		listCtrl.InsertColumn(i,&lvc);
	}

	m_pSortColumn->Setup(&listCtrl,this->m_hWnd, m_iNrColomns, registrySortComputerPrimary, registrySortComputerSecondary, registrySortComputerTertiary ,262,263, 263);
//	m_pSelection->Setup(&listCtrl,  m_pData, MAX_COMPUTERS_IN_LIST, COLUMN_COMPUTER_IP, COLUMN_COMPUTER_IP, -1, NULL);

	FillList();
	m_bFirstTime = false;
}


void CRemoteCombinedComputerView::ReadAllColumnWidth()
{
	CListCtrl& listCtrl = GetListCtrl();
	int iWidth;

	for(int iCount = 0; iCount<=NUM_REMOTE_COMPUTER_COLUMNS; iCount++)
	{
		iWidth = ReadColumnWidth(iCount, _gnRemoteComputerColumnWidth[iCount]);
		listCtrl.SetColumnWidth(iCount, iWidth);
		m_iColumnWidth[iCount] = iWidth;
	}
}

int CRemoteCombinedComputerView::ReadColumnWidth(int iColumn, int iDefault)
{
	CString sRegistry;
	char cChar;

	sRegistry = registryColumnComputer;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	return theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
}

void CRemoteCombinedComputerView::CheckColumnWidth()
{
	int		iWidth;
	bool	bChanged = false;
	char	cChar;
	CString sTxt;

	if (m_bFirstTime) return; // wait for init

	CListCtrl& listCtrl = GetListCtrl();

	for (int iCount = 0; iCount < NUM_REMOTE_COMPUTER_COLUMNS; iCount++)
	{
		iWidth = listCtrl.GetColumnWidth(iCount);
		if (m_iColumnWidth[iCount] != listCtrl.GetColumnWidth(iCount))
		{
			m_iColumnWidth[iCount] = iWidth;
			bChanged = true;
			sTxt = registryColumnComputer;
			cChar = iCount + 'a';
			sTxt = sTxt + "_" + cChar; 
			// update new width in registry
			theApp.WriteProfileInt(registrySectionColumn, sTxt, iWidth);
		}
	}
}

void CRemoteCombinedComputerView::FillList()
{
	int			iSize, iPos;
	LV_COLUMN	lvc;
	LVITEMW		lvitem;
	std::deque<CComputer*> *plComputers;
	int			i, iNrComputers;
	CString		sTxt, sPassChar, sComputerPassword, sComputerId, sMAC, sSpecialChar;
	int			iEncryptedConnection;

	CComputer	*pComputer;

	CListCtrl& ListCtrl = GetListCtrl();

	CDlgSettingsView *pSettingsView;
	pSettingsView = theApp.m_pDlgSettingsView;
	this->m_bUseHorizontalGrid = pSettingsView->m_bUseHorizontalGrid;
	this->m_bUseVerticalGrid = pSettingsView ->m_bUseVerticalGrid;

	m_StateImageList.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
	ListCtrl.SetImageList(&m_StateImageList, LVSIL_STATE);

	plComputers = &theApp.m_lComputers;

	iNrComputers = (int) plComputers->size();

	iSize = (int) plComputers->size();
	iPos = 0;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;


// insert items

	LV_ITEM lvi;

	for(i = 0; i < iNrComputers; i++)
	{
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
		lvi.iItem = i;
		lvi.iSubItem = 0;

		pComputer = plComputers->at(iPos);
		iPos++;

		sComputerId = pComputer->m_sComputerId;
		if (sComputerId.GetLength() > 0)
		{
			sSpecialChar = HIDE_SECOND;
			sComputerId = sSpecialChar + sComputerId;
			sSpecialChar = HIDE_CHAR;
			sComputerId = sComputerId + sSpecialChar;
			sComputerId += pComputer->m_sComputerId;	// for name changing, to keep the old name as well.
		}
		sMAC = pComputer->m_sComputerMAC;
		lvi.pszText = sComputerId.GetBuffer();// _gszItem[i][0];
		sComputerId.ReleaseBuffer();

		iEncryptedConnection = pComputer->m_iEncryptedConnection;


		lvi.iImage = i;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		if (pComputer->m_iComputerChecked) 	lvi.state = INDEXTOSTATEIMAGEMASK(2);	// 1 unchecked, 2 checked
		else lvi.state = INDEXTOSTATEIMAGEMASK(1);

		lvi.lParam = (LPARAM) &m_dataDummy;

		ListCtrl.InsertItem(&lvi);

		ListCtrl.SetItemText(i,COLUMN_COMPUTER_ID,sComputerId);
		ListCtrl.SetItemText(i,COLUMN_COMPUTER_IP,pComputer->m_sComputerIp);
		ListCtrl.SetItemText(i,COLUMN_COMPUTER_MAC,sMAC);
		ListCtrl.SetItemText(i,COLUMN_COMPUTER_GROUP,pComputer->m_sComputerGroup);

		sPassChar =  PASS_CHAR;
		sComputerPassword = sPassChar + pComputer->m_sComputerPassword;
		ListCtrl.SetItemText(i,COLUMN_COMPUTER_PASS,sComputerPassword);

		CString sEncryptedConnection = "";
		if (iEncryptedConnection == 1)
		{
			sEncryptedConnection = "yes";
		}
		ListCtrl.SetItemText(i, COLUMN_COMPUTER_CON_ENCRYPTION, sEncryptedConnection);

		if (pComputer->m_iPort == -1) sTxt = "";
		else sTxt.Format("%d",pComputer->m_iPort);
		ListCtrl.SetItemText(i,COLUMN_COMPUTER_PORT,sTxt);

	}

	StartSorting();
	ListCtrl.Invalidate(TRUE);
}

void CRemoteCombinedComputerView::SaveComputerList()
{
	int		iSize, iPos;
	bool bChanged;
	int	iNrComputers,  iChecked;
	CString sTxt, sComputerId, sComputerGroup, sComputerIp, sComputerMAC, sComputerIdHidden, sComputerPassword, sComputerEncryption;
//	XOSITION pos;
	CComputer	*pComputer;

	bChanged = ListChanged(true);

	// empty computer list
	iSize = (int) theApp.m_lComputers.size();
	iPos = 0;
	iNrComputers = (int) theApp.m_lComputers.size();

	for (int iCount = 0; iCount < iNrComputers; iCount++)
	{
		pComputer = theApp.m_lComputers.at(iPos);
		iPos++;
		delete pComputer;
	}
	theApp.m_lComputers.clear();

	CListCtrl& listCtrl = GetListCtrl();
	iNrComputers = listCtrl.GetItemCount();

	// check if the computer name has changed
	for (int iCountList = 0; iCountList < iNrComputers; iCountList++)
	{
		sComputerId = listCtrl.GetItemText( iCountList, COLUMN_COMPUTER_ID);
		sComputerId = ExtractHidden(&sComputerId,&sComputerIdHidden);
		if (sComputerIdHidden != "")
		{
			if (sComputerIdHidden != sComputerId)
			{
				RenameFile(sComputerId, sComputerIdHidden, false);	// short time
				RenameFile(sComputerId, sComputerIdHidden, true); // long time
			}
		}
	}
	
	for (int iCountList = 0; iCountList < iNrComputers; iCountList++)
	{
		pComputer = new CComputer;

		sTxt = listCtrl.GetItemText( iCountList, COLUMN_COMPUTER_PORT);
		if (sTxt.GetLength() == 0) pComputer->m_iPort = -1;
		else pComputer->m_iPort = atoi (sTxt);

		sComputerId = listCtrl.GetItemText( iCountList, COLUMN_COMPUTER_ID);
		sComputerId = RemoveHidden(sComputerId);
		sComputerGroup =  listCtrl.GetItemText( iCountList, COLUMN_COMPUTER_GROUP);
		sComputerIp = listCtrl.GetItemText( iCountList, COLUMN_COMPUTER_IP);
		sComputerMAC = listCtrl.GetItemText( iCountList, COLUMN_COMPUTER_MAC);
		sComputerMAC = RemoveHidden(sComputerMAC);
		sComputerMAC.Replace(":","-");
		sComputerMAC.Replace(" ","");
		sComputerMAC.Replace(",",";");
		sComputerPassword = listCtrl.GetItemText( iCountList, COLUMN_COMPUTER_PASS);
		sComputerEncryption = listCtrl.GetItemText(iCountList, COLUMN_COMPUTER_CON_ENCRYPTION);
	
		pComputer->m_sComputerId = sComputerId;
		pComputer->m_sComputerGroup = sComputerGroup;
		pComputer->m_sComputerIp = sComputerIp;
		pComputer->m_sComputerMAC = sComputerMAC;
		if (sComputerPassword.GetAt(0) == PASS_CHAR)
		{
			sComputerPassword = sComputerPassword.Mid(1); // remove Password char if present.
		}
		pComputer->m_sComputerPassword = sComputerPassword;
		if (sComputerEncryption.GetLength() > 0)
		{
			pComputer->m_iEncryptedConnection = 1;
		}
		else
		{
			pComputer->m_iEncryptedConnection = 0;
		}

		if (listCtrl.GetItemState(iCountList,LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))	iChecked = TRUE;
		else iChecked = FALSE;				
		pComputer->m_iComputerChecked = iChecked;

		theApp.m_lComputers.push_back(pComputer);
	}
	if (bChanged)
	{
		theApp.WriteXml();
		m_bChanged = false;
		m_bWasEditSubLabel = false;
	}
}

bool CRemoteCombinedComputerView::ListChanged(bool bEncrypted)
{

	if (m_bChanged)
	{
		return true;
	}
	if (m_bWasEditSubLabel)
	{
		return true;
	}

	if (bEncrypted)
	{
		if (theApp.m_pDlgSettingsExpert->m_bUseEncryptionChanged)
		{
			return true;
		}
	}
	return false;
}

CString CRemoteCombinedComputerView::RemoveHidden(CString sTxt)
{
	CString sDummy;

	return ExtractHidden(&sTxt, &sDummy);
}


CString CRemoteCombinedComputerView::ExtractHidden(CString *psTxt, CString *psTxtHidden)
{
	CString sTxt;

	sTxt = *psTxt;
	*psTxtHidden = "";

	if (sTxt.GetAt(0) == HIDE_SECOND || sTxt.GetAt(0) == COLOR_CHAR)
	{
		sTxt = sTxt.Mid(1);

		int iHide = sTxt.Find(HIDE_CHAR,1);
		if (iHide > 0)
		{
			*psTxtHidden = sTxt.Mid(iHide+1);
			return sTxt.Left(iHide);
		}
	}
	return sTxt;
}

void CRemoteCombinedComputerView::RenameFile(CString sNow, CString sOld, bool bHistoryLong)
{
	CString sDirectory, sFileNameNow, sFileNameNow1, sFileNameNow2, sFileNameOld, sFileNameOld1, sFileNameOld2 ;
	CXml	xml("");

	if (bHistoryLong)
	{
		sFileNameNow =  HISTORY_FILENAME_LONG;
		sFileNameOld =  HISTORY_FILENAME_LONG;
	}
	else
	{
		sFileNameNow =  HISTORY_FILENAME;
		sFileNameOld =  HISTORY_FILENAME;
	}
	
	sDirectory = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectory += "\\history\\";

	sNow.Replace(' ','_');

	sFileNameNow+= "_";
	sFileNameNow+= sNow;
	sFileNameNow+= ".cvs";

	sFileNameNow1 = sDirectory + sFileNameNow + "1";
	sFileNameNow2 = sDirectory + sFileNameNow + "2";
	sFileNameNow = sDirectory + sFileNameNow;

	sOld.Replace(' ','_');

	sFileNameOld+= "_";
	sFileNameOld+= sOld;
	sFileNameOld+= ".cvs";

	sFileNameOld1 = sDirectory + sFileNameOld + "1";
	sFileNameOld2 = sDirectory + sFileNameOld + "2";
	sFileNameOld = sDirectory + sFileNameOld;



	try
	{
		CFile::Rename(sFileNameOld, sFileNameNow);
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}
	try
	{
		CFile::Rename(sFileNameOld1, sFileNameNow1);
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}
	try
	{
		CFile::Rename(sFileNameOld2, sFileNameNow2);
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}
}

CString CRemoteCombinedComputerView::GetComputerStatus(int iStatus)
{
	CString sStatus;

	switch (iStatus)
	{
		case RPC_STATUS_ERROR_CANNOT_CONNECT:
			return  gszTranslation[PosWindowStatusMessageCombinedNotConnected];
		break;
		case RPC_STATUS_ERROR_READ:
			sStatus = gszTranslation[PosWindowStatusMessageCombinedNotConnected];
			sStatus+= " (R)";
			return sStatus;
		break;
		case RPC_STATUS_ERROR_AUTHORIZATION:
			return gszTranslation[PosWindowStatusMessageCombinedPassWord];
		break;
		case RPC_STATUS_ERROR_READING_STATE:
			return gszTranslation[PosWindowStatusMessageCombinedState];
		break;
		case RPC_STATUS_NONE:
			return gszTranslation[PosWindowStatusMessageCombinedConnecting];
		break;	
		case RPC_STATUS_CONNECTED:
			return gszTranslation[PosWindowStatusMessageCombinedConnected];
		break;
		case RPC_STATUS_ERROR_LOST_CONNECTION:
			return gszTranslation[PosWindowStatusMessageCombinedLostConnection];
		break;

	}
	return "??";
}


BEGIN_MESSAGE_MAP(CRemoteCombinedComputerView, CTemplateRemoteComputerView)
	ON_WM_SIZE()
	ON_MESSAGE (UWM_MSG_TIMER_COMPUTER, OnTimerComputers) 
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_COMPUTER_VIEW, OnReadyRpc)
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_CLIENT, OnReadyRpcClient)

	ON_MESSAGE (UWM_MSG_SETTING_TASKS, OnSettingsTasks)

	ON_MESSAGE (UWM_MSG_REMOTE_COMPUTER_SWITCH, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_WINDOW_SHOWN, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_COLUMN_COMPUTERS, OnColumnChanged)

	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelTabView)
	ON_MESSAGE(UWM_MSG_VIEW_REFRESH,OnRefresh)

	ON_MESSAGE (UWM_MSG_GET_FLOATER_DATA, OnFloaterData)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_CPU, OnSetSnoozeCpu)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_GPU, OnSetSnoozeGpu)

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistorySetWidth)

	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, OnComputerBarSelectionChanged)

	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnFontChangeAll)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnFontHasChanged)

	ON_COMMAND(ID_COMPUTERS_ADD, &CRemoteCombinedComputerView::OnComputersAdd)
	ON_COMMAND(ID_COMPUTERS_REMOVE, &CRemoteCombinedComputerView::OnComputersRemove)
	ON_COMMAND(ID_COMPUTERS_DETECT, &CRemoteCombinedComputerView::OnComputersDetect)

	ON_MESSAGE (UWM_MSG_PREFERENCE_DIALOG, OnPreferenceDialog)
	ON_MESSAGE (UWM_MSG_PROXY_DIALOG, OnProxyDialog)

	ON_COMMAND(ID_COMPUTER_PROPERTIES, &CRemoteCombinedComputerView::OnComputerProperties)
	ON_UPDATE_COMMAND_UI(ID_COMPUTER_PROPERTIES, &CRemoteCombinedComputerView::OnUpdateComputerProperties)

	ON_WM_CONTEXTMENU()

//	ON_MESSAGE (UWM_MSG_NOTICES_BT_UPDATE, OnBoincTasksVersion)	
//	ON_MESSAGE (UWM_MSG_NOTICES_BT_RULE, OnBoincTasksRule)	

	ON_MESSAGE (UWM_DLG_SEARCH_COMPUTER_READY, OnSearchComputerReady)	
	ON_MESSAGE (UWM_MSG_WEB_ITEM_SELECTED, OnWebItemSelected)
	ON_MESSAGE(UWM_MSG_WEB_SORT,OnWebSort)
END_MESSAGE_MAP()

// CRemoteCombinedComputerView diagnostics

#ifdef _DEBUG
void CRemoteCombinedComputerView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteCombinedComputerView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CRemoteCombinedComputerView message handlers

LRESULT CRemoteCombinedComputerView::OnTimerComputers(WPARAM parm1, LPARAM parm2)
{
	CString  sStatus;
	char	*pcComputerId;
	int iViewActive, iNrRpc, iThreadId;
	HWND hWnd;
	CRemoteCombinedComputerView *pView;
	bool	bVisible;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	bVisible = theApp.m_pMainWnd->IsWindowVisible() == TRUE;

	CheckColumnWidth();

	if (m_bThreadBusy)
	{
		if (bVisible && m_pDoc->m_iViewActive == COMPUTER_VIEW_ACTIVE)
		{
			if (m_iThreadLocked > DELAY_SHOW_UPDATE_TIMER) sStatus.Format(" %d", m_iThreadLocked);
			sStatus = gszTranslation[PosWindowStatusMessageStatusUpdating] + sStatus;
			theApp.m_pMainFrame->SetBoincTasksStatus(sStatus.GetBuffer());
			sStatus.ReleaseBuffer();
		}
	}
	else
	{
		if (m_iTimerComputer <= 6)
		{
			sStatus.Format(gszTranslation[PosWindowStatusMessageStatusUpdateInSec], 6-m_iTimerComputer);	
			theApp.m_pMainFrame->SetBoincTasksStatus(sStatus.GetBuffer());
		}
	}

	if (m_bThreadBusy)		// check for lockup
	{
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

	pView = m_pDoc->m_pComputerView;
	hWnd = pView->m_hWnd;

	if (m_iTimerComputer++ >= 5)		// update every 5 seconds
	{
		m_iTimerComputer = 0;
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);
			if (m_pDoc->GetComputerIdStringWithIdNumber(iThreadId, &pcComputerId))
			{// get the computer id string for the thread
				{
					::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_COMPUTER_VIEW,(WPARAM) hWnd, 0); // return naar computer view!
					m_iSendRpcCount++;
					m_bThreadBusy = true;	
				}
			}
		}

		if (theApp.m_pDlgSettingsGeneral->m_bConnecTthrottle)	// check if TThrottle is enabled
		{
			int iNrTThrottleRpc;
			iNrTThrottleRpc = m_pDoc->GetRpcSelectedComputers();
			SendTThrottleRequest(iNrTThrottleRpc);
		}
	}




	return 0;
}

void CRemoteCombinedComputerView::SendTThrottleRequest(int iNrTThrottleRpc, bool bExcludeFromList)
{
	char	*pcComputerId;
	HWND	hWnd;
	int		iTThrottleThreadId;

	hWnd = this->m_hWnd;

	for (int iCount = 0; iCount < iNrTThrottleRpc; iCount++)
	{
		m_pDoc->GetTThrottleRpcThreadId(&iTThrottleThreadId, iCount);
		if (bExcludeFromList)
		{
			if (m_pDoc->GetTThrottleComputerIdStringWithIdNumber(iTThrottleThreadId, &pcComputerId))
			{// get the computer id string for the thread
				int iResult = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, (WPARAM) pcComputerId, 0);
				if (iResult == TRUE)
				{
					m_lComputerRpcInfoClient.at(iCount)->m_bReady = false;
					m_lComputerRpcInfoClient.at(iCount)->m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
					::PostThreadMessage(iTThrottleThreadId,UWM_MSG_THREAD_START_RPC_CLIENT,(WPARAM) hWnd, (LPARAM) m_lComputerRpcInfoClient.at(iCount));
				}
			}
		}
		else
		{
			m_lComputerRpcInfoClient.at(iCount)->m_bReady = false;
			m_lComputerRpcInfoClient.at(iCount)->m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
			::PostThreadMessage(iTThrottleThreadId,UWM_MSG_THREAD_START_RPC_CLIENT,(WPARAM) hWnd,0);
		}
	}
}


LRESULT CRemoteCombinedComputerView::OnReadyRpc(WPARAM parm1, LPARAM parm2)
{

	int					iStatus,iCount;
	CRpcThreadReturn	*pThreadReturn;
	bool				bOverride = false, bNumberChanged = false;//, bSelectedFound = false;
	CString				sStatus, sStatusTThrottle;

	iStatus = (int) parm1;
	pThreadReturn = (CRpcThreadReturn *)	parm2;

	if (m_bDocumentIsClosing)
	{
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusClosing]);
		delete pThreadReturn;
		return 0;			// everything is closing down
	}
	
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_pcComputerId = pThreadReturn->m_pcComputerId;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_pThread = pThreadReturn->m_pThread;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_iStatusRpc = iStatus;

	// get time stats info
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_pThread->PostThreadMessage(UWM_MSG_THREAD_GET_TIME_STATS,(WPARAM) this->m_hWnd, (LPARAM) &m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_timeStats);

	if (iStatus == RPC_STATUS_CONNECTED)
	{
		CString sTxt;
		sTxt.Format("%d.%d.%d",pThreadReturn->m_pBoincVersion->major, pThreadReturn->m_pBoincVersion->minor, pThreadReturn->m_pBoincVersion->release);
		m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_sBoincVersion = sTxt;
		m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_sBoincPlatform = pThreadReturn->m_pcHostName;
	}
	else
	{
		m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_sBoincVersion = "";
		m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_sBoincPlatform = "";
	}

	m_iReadyRpcCount++;

	theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_CONNECT, (WPARAM) pThreadReturn->m_pcComputerId, pThreadReturn->m_iStatus);

	delete pThreadReturn;
	pThreadReturn = NULL;

	if (m_iReadyRpcCount != m_iSendRpcCount) return 0;	// wait for all results to come in

	CDlgSettingsView *pSettingsView;
	pSettingsView = theApp.m_pDlgSettingsView;
	this->m_bUseHorizontalGrid = pSettingsView->m_bUseHorizontalGrid;
	this->m_bUseVerticalGrid = pSettingsView ->m_bUseVerticalGrid;
	
	// setup sorting
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		m_lComputerRpcInfo.at(iCount)->m_iSortIndex = iCount;
	}

	CListCtrl& listCtrl = GetListCtrl();
	int iListCount = listCtrl.GetItemCount();
	CString sComputerId;

	for (int iCountList = 0; iCountList < iListCount; iCountList++)
	{
		sComputerId = listCtrl.GetItemText(iCountList,COLUMN_COMPUTER_ID);
		sComputerId = RemoveHidden(sComputerId);
		for (int iCount = 0; iCount < m_iReadyRpcCount; iCount++)
		{
			if (sComputerId == m_lComputerRpcInfo.at(iCount)->m_pcComputerId)
			{
				CString sOld;
				sOld = listCtrl.GetItemText(iCountList,COLUMN_COMPUTER_BOINC);
				if (sOld != m_lComputerRpcInfo.at(iCount)->m_sBoincVersion)
				{
					listCtrl.SetItemText(iCountList,COLUMN_COMPUTER_BOINC, m_lComputerRpcInfo.at(iCount)->m_sBoincVersion);
				}
				sOld = listCtrl.GetItemText(iCountList,COLUMN_COMPUTER_BOINC_PLATFORM);
				if (sOld != m_lComputerRpcInfo.at(iCount)->m_sBoincPlatform)
				{
					listCtrl.SetItemText(iCountList,COLUMN_COMPUTER_BOINC_PLATFORM, m_lComputerRpcInfo.at(iCount)->m_sBoincPlatform);
				}

				if (theApp.m_pDlgSettingsGeneral->m_bConnecTthrottle)	// check if TThrottle is enabled
				{
					// add the TThrottle connection status
					CString sComputerIdTThrottle;
					for (int iCountTThrottle = 0; iCountTThrottle < (int) m_lComputerRpcInfoClient.size(); iCountTThrottle++)
					{
						sComputerIdTThrottle = "";
						sComputerIdTThrottle = m_lComputerRpcInfoClient.at(iCountTThrottle)->m_sComputerId.c_str();
						if (sComputerId == sComputerIdTThrottle)
						{
							if ( m_lComputerRpcInfoClient.at(iCountTThrottle)->m_bReadyCopy)	sStatusTThrottle = gszTranslation[PosWindowStatusMessageCombinedConnected];	
							else
							{
								sStatusTThrottle= gszTranslation[PosWindowStatusMessageCombinedNotConnected];
							}
						}
					}
				}
				else sStatusTThrottle = "";

				sStatus = GetComputerStatus(m_lComputerRpcInfo.at(iCount)->m_iStatusRpc);

				if (sStatus != sStatusTThrottle)
				{
					if (sStatusTThrottle != "") sStatus+= ", ";
					sStatus+= sStatusTThrottle;
				}
				sOld = listCtrl.GetItemText(iCountList,COLUMN_COMPUTER_STATUS);
				if (sOld != sStatus)
				{
					listCtrl.SetItemText(iCountList,COLUMN_COMPUTER_STATUS, sStatus);
				}
			}
		}
	}

	if (!m_bWasEditSubLabel) StartSorting();


	if (g_bWebServerActive)
	{
		if (!m_bDocumentIsClosing && (g_pcWebServerHtml == NULL))
		{
			CCreateHtmlWebServer createHtml;
			char *pcHtml;
			if (createHtml.Create(PosBarViewSelectComputers, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_COMPUTER_COLUMNS, NULL, PosGroupViewComputer, &pcHtml))
			{
				g_pcWebServerHtml = pcHtml;
				g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_COMPUTER);
				g_tWebServerHtml = GetTickCount()/100;
			}
		}
	}

	if (g_bCloudServerRequestData)
	{
		if (!m_bDocumentIsClosing && (g_pcCloudServerHtml == NULL))
		{
			if (g_iCloudServerHtmlTabProcessed[TAB_COMPUTER] == TRUE)
			{
				g_iCloudServerHtmlTabProcessed[TAB_COMPUTER] = FALSE;
				CCreateHtmlWebServer createHtml;
				char *pcHtml;
				if (createHtml.Create(PosBarViewSelectComputers, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_COMPUTER_COLUMNS, NULL, PosGroupViewComputer, &pcHtml))
				{
					g_pcCloudServerHtml = pcHtml;
					if (g_pThreadCloudServer != NULL) g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_COMPUTER);
					g_tCloudServerHtml = GetTickCount()/100;
				}
			}
		}
	}

	m_pDoc->DisplayStatus(m_iReadyRpcCount, &m_lComputerRpcInfo.at(0)->m_iStatusRpc);
	m_bThreadBusy = false;

	return 0;
}

LRESULT CRemoteCombinedComputerView::OnReadyRpcClient(WPARAM parm1, LPARAM parm2)
{
	CRpcInfoClient *pInfoClient;
	CString sFrom, sText, sVersion, sComputerIdTThrottle;

	if (m_bDocumentIsClosing)
	{
		return 0;			// everything is closing down.
	}

	pInfoClient = (CRpcInfoClient *) parm1;
	if (pInfoClient->m_iStatus == 0)		// valid
	{
		pInfoClient->m_bReady = true;
		pInfoClient->m_bReadyCopy = true;
		pInfoClient->m_fCpuThrottleCopy = pInfoClient->m_fCpuThrottle;
		pInfoClient->m_fGpuThrottleCopy = pInfoClient->m_fGpuThrottle;
		pInfoClient->m_fTopCpuTemperatureCopy = pInfoClient->m_fTopCpuTemperature;
		pInfoClient->m_fTopGpuTemperatureCopy = pInfoClient->m_fTopGpuTemperature;
		pInfoClient->m_iLostCount = 0;

		for (int iCount = 0; iCount < MAX_GPU; iCount++)
		{
			pInfoClient->m_fGpuTemperatureCopy[iCount] = pInfoClient->m_fGpuTemperature[iCount];
		}
	}
	else
	{
		if (pInfoClient->m_iLostCount > 3)
		{
			pInfoClient->m_bReadyCopy = false;				// after 3 times lost invalidate
		}
		else
		{
			pInfoClient->m_iLostCount += 1;
		}
	}

	CListCtrl& listCtrl = GetListCtrl();
	int iListCount = listCtrl.GetItemCount();
	CString sComputerId;

	for (int iCountList = 0; iCountList < iListCount; iCountList++)
	{
		sComputerId = listCtrl.GetItemText(iCountList,COLUMN_COMPUTER_ID);
		sComputerId = RemoveHidden(sComputerId);

		sComputerIdTThrottle = pInfoClient->m_sComputerId.c_str();

		if (sComputerId == sComputerIdTThrottle)
		{

			if (pInfoClient->m_bReady)
			{
				if (pInfoClient->m_fVersion >= 0) sVersion.Format("%.2f", pInfoClient->m_fVersion);
				else sVersion = "?";
				CString sOld;
				sOld = listCtrl.GetItemText(iCountList,COLUMN_COMPUTER_TTHROTTLE);
				if (sOld != sVersion)
				{
					listCtrl.SetItemText(iCountList,COLUMN_COMPUTER_TTHROTTLE, sVersion);
				}
			}
		}

	}

	return 0;
}

// switch
void CRemoteCombinedComputerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView )
{
	if (!bActivate)
	{
		EndInPlaceEdit();
		if (ListChanged(false))
		{
			theApp.m_pMainFrame->PostMessage(UWM_MSG_FORCE_A_RESTART,0,0);
//			theApp.m_bRebootRemoteCombined = true;
		}
	}
}

LRESULT CRemoteCombinedComputerView::OnSwitch(WPARAM parm1, LPARAM parm2)
{
	int iTimeToUpdate;

	time_t tSystemTime;
	_time64(&tSystemTime);

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (parm2 == REFRESH_VIEW_NOW) 
	{
		m_iTimerComputer = 1000;					// direct action
	}

	iTimeToUpdate = (int) (m_LastSwitchedTime - tSystemTime);
	if (iTimeToUpdate <= 0) 
	{
		m_LastSwitchedTime = tSystemTime + 10;		// don't update within 5 seconds.
		m_iTimerComputer = 1000;					// 1000 to ensure direct timer message
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	}
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnSettingsTasks(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing)
	{
		return 0;			// everything is closing down.
	}
	if (m_pDoc != NULL) m_pDoc->SettingsTasks(parm1 != 0);
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnColumnChanged(WPARAM parm1, LPARAM parm2)
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
		for (int iCount = 0; iCount < NUM_REMOTE_COMPUTER_COLUMNS; iCount++)
		{
			listCtrl.SetColumnWidth(iCount, *pColumn);
			pColumn++;
		}
	}

	return 0;
}

void CRemoteCombinedComputerView::OnComputersAdd()
{
	CListCtrl& listCtrl = GetListCtrl();
	LV_ITEM lvi;
	int		iItems;
	CString sComputerId, sSpecialChar;

	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	iItems = listCtrl.GetItemCount();

	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
	lvi.iItem = iItems;
	lvi.iSubItem = 0;
	lvi.pszText = "0";

	lvi.iImage = iItems;
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK(2);

	lvi.lParam = (LPARAM) &m_dataDummy;

	listCtrl.InsertItem(&lvi);

	sComputerId = gszTranslation[PosViewComputerAddNew];

	listCtrl.SetItemText(iItems,COLUMN_COMPUTER_ID,sComputerId);		//"new"
	listCtrl.SetItemText(iItems,COLUMN_COMPUTER_IP,"");
	listCtrl.SetItemText(iItems,COLUMN_COMPUTER_GROUP,"");

	CString sTxt;
	sTxt = PASS_CHAR;
	listCtrl.SetItemText(iItems,COLUMN_COMPUTER_PASS,sTxt);

	StartSorting();

	m_bChanged = true;
}


void CRemoteCombinedComputerView::OnComputersRemove()
{
	int iSelected;
	POSITION pos;

	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	CListCtrl& listCtrl = GetListCtrl();

	pos = listCtrl.GetFirstSelectedItemPosition();
	iSelected =  listCtrl.GetNextSelectedItem(pos);

	RemoveInplace();

	listCtrl.DeleteItem(iSelected);
	this->Invalidate(TRUE);

	m_bChanged = true;
}

void CRemoteCombinedComputerView::OnComputersDetect()
{
//	CString sName, sIp, sPassword;

	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	DeleteComputerFind();

//	CListCtrl& listCtrl = GetListCtrl();
	RemoveInplace();

	if (m_pDlgSearchComputer == NULL)
	{
		m_pDlgSearchComputer = new CDlgSearchComputers;
		m_pDlgSearchComputer->Create(IDD_DIALOG_FIND_COMPUTER);
	}
	m_pDlgSearchComputer->SendMessage(UWM_DLG_SEARCH_COMPUTER_START, (WPARAM) this->m_hWnd, (LPARAM) &m_lListComputerFind);
	m_pDlgSearchComputer->ShowWindow(SW_SHOW);
}

LRESULT CRemoteCombinedComputerView::OnPreferenceDialog(WPARAM parm1, LPARAM parm2)
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


	sComputer = listCtrl.GetItemText(nItem,COLUMN_COMPUTER_ID);
	sComputer = RemoveHidden(sComputer);
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

LRESULT CRemoteCombinedComputerView::OnProxyDialog(WPARAM parm1, LPARAM parm2)
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

	sComputer = listCtrl.GetItemText(nItem,COLUMN_COMPUTER_ID);
	sComputer = RemoveHidden(sComputer);
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

LRESULT CRemoteCombinedComputerView::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	m_iTimerComputer = 1000;				// 1000 to ensure direct timer message
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnSelTabView(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	m_pDoc->OnShowView((int) wParam, (int) lParam);
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam)
{
	CString *psComputer;
	int		iThreadId, iReconnect;

	psComputer = (CString *) wParam;
	iReconnect = (int) lParam;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
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
	m_iTimerComputer = 1000;				// 1000 to ensure direct timer message
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnFloaterData(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_GET_FLOATER_DATA, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_CPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedComputerView::OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return -1;			// everything is closing down.
	return m_pDoc->m_pProjectView->PostMessage(UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, wParam, lParam);
}

LRESULT CRemoteCombinedComputerView::OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, wParam, lParam);
	return 0;
}

//LRESULT CRemoteCombinedComputerView::OnBoincTasksVersion(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_UPDATE,parm1,parm2);
//	return 0;
//}

//LRESULT CRemoteCombinedComputerView::OnBoincTasksRule(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_RULE,parm1,parm2);
//	return 0;
//}

LRESULT CRemoteCombinedComputerView::OnSearchComputerReady(WPARAM parm1, LPARAM parm2)
{
	int iCountSearch, iItems, iCount;
	CString sName, sIp, sMAC, sTemp, sPassword;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	CListCtrl& listCtrl = GetListCtrl();

	for (iCountSearch = 0; iCountSearch < (int) m_lListComputerFind.size(); iCountSearch++)
		{
			sName = m_lListComputerFind.at(iCountSearch)->m_sName;
			sIp = m_lListComputerFind.at(iCountSearch)->m_sIp;
			sMAC = m_lListComputerFind.at(iCountSearch)->m_sMAC;
			sPassword = m_lListComputerFind.at(iCountSearch)->m_sPassword;
			
			iItems = listCtrl.GetItemCount();
			bool bDouble = false;
			CString sNameCheck, sIpCheck;
			for (iCount = 0; iCount < iItems; iCount++)
			{
				sNameCheck = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_ID);
				sNameCheck = RemoveHidden(sNameCheck);
				sIpCheck =  listCtrl.GetItemText(iCount,COLUMN_COMPUTER_IP);
				if (sNameCheck.CompareNoCase(sName) == 0) bDouble = true;
				if (sIpCheck == sIp) bDouble = true;
				if(bDouble) break;
			}
			if (!bDouble)
			{
				LV_ITEM lvi;
	
				lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
				lvi.iItem = iItems;
				lvi.iSubItem = 0;
				lvi.pszText = "0";

				lvi.iImage = iItems;
				lvi.stateMask = LVIS_STATEIMAGEMASK;	
				lvi.state = INDEXTOSTATEIMAGEMASK(2);

				lvi.lParam = (LPARAM) &m_dataDummy;

				listCtrl.InsertItem(&lvi);

				listCtrl.SetItemText(iItems,COLUMN_COMPUTER_ID,sName);
				listCtrl.SetItemText(iItems,COLUMN_COMPUTER_IP,sIp);
				listCtrl.SetItemText(iItems,COLUMN_COMPUTER_MAC,sMAC);

				CString sTxt;
				sTxt = PASS_CHAR;
				if (sPassword.GetLength() > 0)
				{
//					listCtrl.SetItemText(iItems,COLUMN_COMPUTER_PASS,sPassword);
					sPassword = sTxt + sPassword;
				}
				else
				{
					sPassword = sTxt;
				}
				listCtrl.SetItemText(iItems,COLUMN_COMPUTER_PASS,sPassword);
				listCtrl.SetItemText(iItems,COLUMN_COMPUTER_GROUP,"");

				m_bChanged = true;
			}
			else
			{
				if (sName.CompareNoCase(sNameCheck) == 0)	// double check
				{
					sTemp = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_MAC);
					if (sTemp != sMAC)
					{
						listCtrl.SetItemText(iCount,COLUMN_COMPUTER_MAC,sMAC);
						m_bChanged = true;
					}
					CString sTxt;
					sTxt = PASS_CHAR;
					if (sPassword.GetLength() > 0)
					{
//						listCtrl.SetItemText(iItems,COLUMN_COMPUTER_PASS,sPassword);
						sPassword = sTxt + sPassword;
						listCtrl.SetItemText(iCount,COLUMN_COMPUTER_PASS,sPassword);
						m_bChanged = true;
					}
				}
			}
		}


	return 0;
}

LRESULT CRemoteCombinedComputerView::OnFontChangeAll(WPARAM wParam,LPARAM lParam)
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

LRESULT CRemoteCombinedComputerView::OnFontHasChanged(WPARAM wParam,LPARAM lParam)
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

void CRemoteCombinedComputerView::OnContextMenu(CWnd* pWnd, CPoint point) 
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

	contextMenu.LoadMenu(IDR_MENU_CONTEXT_COMPUTER); 	
	tracker = contextMenu.GetSubMenu(0); 
	tracker->ModifyMenu(ID_COMPUTER_PROPERTIES,MF_STRING,ID_COMPUTER_PROPERTIES,gszTranslation[PosPopUpComputerProperties]);
	tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y , AfxGetMainWnd()); 
}

void CRemoteCombinedComputerView::OnComputerProperties()
{
	char		cBuffer[8192+1];
	HOST_INFO	*pHostInfo;
	CString		sComputer, sColumn0, sColumn1;
	int			iItem;
	double		dClientRunTime, dTemp;
	CTimeString	timeString;
	CDateConvert dateConvert;

	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	if (m_pDoc == NULL) return;

	CListCtrl& listCtrl = GetListCtrl();
	iItem = 0;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		iItem = listCtrl.GetNextSelectedItem(pos);
	}
	sComputer = listCtrl.GetItemText(iItem,COLUMN_COMPUTER_ID);
	sComputer = RemoveHidden(sComputer);

	if (!m_pDoc->GetRpcThreadConnected(&sComputer)) return;	// not connected

	pHostInfo = m_pDoc->GetRpcThreadHostInfo(&sComputer);
	if (pHostInfo != NULL)
	{
		theApp.m_pDlgProperties->SetWindowText(gszTranslation[PosDialogComputerPropTitle]);
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_SETUP,2,(LPARAM) registryPropertiesComputer);	// 2 columns
		sColumn0 = ""; sColumn1 = "";theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);

		sColumn0 = gszTranslation[PosDialogComputerPropComputer];
		_snprintf_s(cBuffer,1024,_TRUNCATE,"%s, %s, %s",sComputer, pHostInfo->domain_name, pHostInfo->ip_addr);
		sColumn1 = cBuffer;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);

		// CPID
		sColumn0 = "CPID";
		sColumn1 = pHostInfo->host_cpid;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);

		// CPU
		sColumn0 = gszTranslation[PosDialogComputerPropCPU];
		_snprintf_s(cBuffer,1024,_TRUNCATE,"Processors: %d, %s, %s", pHostInfo->p_ncpus, pHostInfo->p_vendor, pHostInfo->p_model);
		sColumn1 = cBuffer;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
		_snprintf_s(cBuffer,1024,_TRUNCATE,"%s", pHostInfo->p_features);
		sColumn1 = cBuffer;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
		// OS
		sColumn0 = gszTranslation[PosDialogComputerPropOs];
		_snprintf_s(cBuffer,1024,_TRUNCATE,"%s, %s", pHostInfo->os_name, pHostInfo->os_version);
		sColumn1 = cBuffer;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
		// Memory
		sColumn0 = gszTranslation[PosDialogComputerPropMemory];
		_snprintf_s(cBuffer,1024,_TRUNCATE,"%.2f Gb, Virtual: %.2f Gb", pHostInfo->m_nbytes/GIGA_BYTE, pHostInfo->m_swap/GIGA_BYTE);
		sColumn1 = cBuffer;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
		// Disk
		sColumn0 = gszTranslation[PosDialogComputerPropDisk];
		_snprintf_s(cBuffer,1024,_TRUNCATE,"Used: %.2f Gb, Free: %.2f Gb", pHostInfo->d_total/GIGA_BYTE, pHostInfo->d_free/GIGA_BYTE);
		sColumn1 = cBuffer;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
		// GPU
		pHostInfo->_coprocs.summary_string(cBuffer, 8192);
		sColumn0 = gszTranslation[PosDialogComputerPropGPU];
		sColumn1 = cBuffer;
		theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
	}
	
	for (int iCount = 0; iCount < (int) m_lComputerRpcInfo.size(); iCount++)
	{
		if (m_lComputerRpcInfo.at(iCount)->m_pcComputerId == sComputer)
		{
			// Header
			sColumn0 = "";
			sColumn1 = "";
			theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
			sColumn0 = gszTranslation[PosDialogComputerPropTime];
			theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
			sColumn0 = "";
			theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);

			// BOINC run %
			dClientRunTime = m_lComputerRpcInfo.at(iCount)->m_timeStats.on_frac;
			if (dClientRunTime > 0)
			{
				sColumn0 = gszTranslation[PosDialogComputerPropTimeBoinc];
				_snprintf_s(cBuffer,1024,_TRUNCATE,"%.2f", dClientRunTime * 100);
				sColumn1 = cBuffer;
				theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
				// Internet %
				dTemp =  m_lComputerRpcInfo.at(iCount)->m_timeStats.connected_frac;
				if (dTemp > 0)
				{
					sColumn0 = gszTranslation[PosDialogComputerPropTimeInternet];
					_snprintf_s(cBuffer,1024,_TRUNCATE,"%.2f", (dTemp * dClientRunTime) * 100);
					sColumn1 = cBuffer;
					theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
				}
				// CPU %
				dTemp =  m_lComputerRpcInfo.at(iCount)->m_timeStats.active_frac;
				if (dTemp > 0)
				{
					sColumn0 = gszTranslation[PosDialogComputerPropTimeCPU];
					_snprintf_s(cBuffer,1024,_TRUNCATE,"%.2f", (dTemp * dClientRunTime) * 100);
					sColumn1 = cBuffer;
					theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
				}
				// GPU %
				dTemp =  m_lComputerRpcInfo.at(iCount)->m_timeStats.gpu_active_frac;
				if (dTemp > 0)
				{
					sColumn0 = gszTranslation[PosDialogComputerPropTimeGPU];
					_snprintf_s(cBuffer,1024,_TRUNCATE,"%.2f", (dTemp * dClientRunTime) * 100);
					sColumn1 = cBuffer;
					theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
				}
				// Start time
				dTemp =  m_lComputerRpcInfo.at(iCount)->m_timeStats.client_start_time;
				if (dTemp > 0)
				{				
					sColumn0 = gszTranslation[PosDialogComputerPropTimeStart];

					dateConvert.Convert((time_t) dTemp, &cBuffer[0]);	
					sColumn1 = cBuffer;
					theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
				}
				// Up time
				dTemp =  m_lComputerRpcInfo.at(iCount)->m_timeStats.previous_uptime;
				if (dTemp > 0)
				{
					sColumn0 = gszTranslation[PosDialogComputerPropTimeUpTime];
					timeString.TimeString((int) dTemp, cBuffer, 50, "", "");
					sColumn1 = cBuffer;
					theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
				}
			}
			else
			{
				sColumn1 = "???";
				theApp.m_pDlgProperties->SendMessage(UWM_MSG_DLG_PROPERTIES_ADD_ROW, (WPARAM) &sColumn0, (LPARAM) &sColumn1);
			}
			break;
		}
	}
}
void CRemoteCombinedComputerView::OnUpdateComputerProperties(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CRemoteCombinedComputerView::OnSize(UINT nType, int cx, int cy )
{
	theApp.m_pMainFrame->m_dlgBarComputer.PostMessage(UWM_MSG_NEW_SIZE, cx, cy);
	CTemplateRemoteComputerView::OnSize(nType, cx, cy );
}

LRESULT CRemoteCombinedComputerView::OnWebItemSelected(WPARAM wParam,LPARAM lParam)
{
//	int iRow, iOption;
//
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

//	iRow = (int) wParam;
//	iOption = (int) lParam;
//
//	m_pSelection->SelectionSet(iRow, iOption);
//
//	char *pcHtml = NULL;
//	if (!m_bThreadBusy)
//	{
//		CListCtrl& listCtrl = GetListCtrl();
//		CCreateHtmlWebServer createHtml;
//		createHtml.Create(PosBarViewSelectTasks, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TASK_COLUMNS, PosGroupViewTasks, &pcHtml);
//	}
//	return (LRESULT) pcHtml;

	return NULL;
}

LRESULT CRemoteCombinedComputerView::OnWebSort(WPARAM parm1, LPARAM parm2)
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
		createHtml.Create(PosBarViewSelectComputers, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_COMPUTER_COLUMNS, NULL, PosGroupViewComputer, &pcHtml);
		m_iTimerComputer = 1000;				// 1000 to ensure direct timer message
	}
	return (LRESULT) pcHtml;
}
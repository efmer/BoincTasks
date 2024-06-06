// RemoteMView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
//#include "ComputerDoc.h"
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
#include "DlgProxyMain.h"
#include "DlgSettingsMessage.h"
#include "DlgSettingsView.h"
#include "DlgFont.h"
#include "DlgSearch.h"
#include "ThreadRpc.h"
#include "Translation.h"
#include "MainFrm.h"

#include "CreateHtmlWebServer.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"

//static _TCHAR *_gszRemoteCombinedMessageColumnLabel[NUM_REMOTE_COMBINED_MESSAGE_COLUMNS] =
//{
//	"Nr"		,"Project"		,"Time"			,"Message"		,"Computer"
//};

// CRemoteCombinedMessageView

IMPLEMENT_DYNCREATE(CRemoteCombinedMessageView, CTemplateRemoteMessageView)

CRemoteCombinedMessageView::CRemoteCombinedMessageView()
{
	m_bFirstTime	= true;

	m_iThreadLocked	= 0;
	m_bThreadBusy	= false;

	m_iWaitingTurner= 0;

	m_pThreadRpc	= NULL;
	m_pDoc			= NULL;

	m_iSelected		= -1;

	m_bWebserverNeedsUpdate = true;
	m_bCloudserverNeedsUpdate = true;

	m_iThreadRpcId	= -1;
	m_bThreadRpcInit= false;

	m_bDocumentIsClosing = false;

	m_pFont = NULL;

	m_data[MESSAGE_COLOR_NORMAL].m_color		= RGB(255,255,255);
	m_data[MESSAGE_COLOR_PRIORITY].m_color		= RGB(255,140,140);;
	m_data[MESSAGE_COLOR_NORMAL_LINE].m_color	= RGB(235,235,235);
	m_data[MESSAGE_COLOR_PRIORITY_LINE].m_color	= RGB(235,120,120);

	// the following 6 just in case they are set in the rpc call.
	m_data[MESSAGE_COLOR_1].m_color				= RGB(255,255,255);
	m_data[MESSAGE_COLOR_1_LINE].m_color		= RGB(255,255,255);
	m_data[MESSAGE_COLOR_2].m_color				= RGB(255,255,255);
	m_data[MESSAGE_COLOR_2_LINE].m_color		= RGB(255,255,255);
	m_data[MESSAGE_COLOR_3].m_color				= RGB(255,255,255);
	m_data[MESSAGE_COLOR_3_LINE].m_color		= RGB(255,255,255);

//	m_hWndPreference = theApp.m_pDlgPrefMain->m_hWnd;
	m_hWndProxy = theApp.m_pDlgProxyMain->m_hWnd;

	CRpcInfoMessageItem *pRpcInfoMessageItem;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pRpcInfoMessageItem = new CRpcInfoMessageItem;
		m_lComputerRpcInfo.push_back(pRpcInfoMessageItem);
	}
}

CRemoteCombinedMessageView::~CRemoteCombinedMessageView()
{
	delete m_pFont;

	for (int iCount = 0; iCount < (int) m_lComputerRpcInfo.size(); iCount++)
	{
		delete m_lComputerRpcInfo.at(iCount);
	}
//	m_lComputerRpcInfo.clear();
	ClearProjectFilterList();
}

void CRemoteCombinedMessageView::ClearProjectFilterList()
{
	for (int iPos = 0; iPos < (int) m_lProjectFilter.size(); iPos++)
	{
		delete m_lProjectFilter.at(iPos);
	}
	m_lProjectFilter.clear();
}

void CRemoteCombinedMessageView::OnInitialUpdate()
{
	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL,(WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);
	CTemplateRemoteMessageView::OnInitialUpdate();
}

void CRemoteCombinedMessageView::InitialUpdate(CRemoteCombinedDoc* pDoc)
{
	m_pDoc = pDoc;
}

BEGIN_MESSAGE_MAP(CRemoteCombinedMessageView, CTemplateRemoteMessageView)
	ON_WM_SIZE()

	ON_MESSAGE (UWM_MSG_TIMER_MESSAGE, OnTimerDlg) 
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_MESSAGE, OnReadyRpc)

	ON_MESSAGE (UWM_MSG_SETTING_TASKS, OnSettingsTasks)

	ON_MESSAGE (UWM_MSG_REMOTE_MESSAGE_SWITCH, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_WINDOW_SHOWN, OnSwitch)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_MESSAGE (UWM_MSG_REMOTE_COLUMN_MESSAGES, OnColumnChanged)

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_MESSAGES_INVALIDATE, OnMessagesInvalidate)

	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelTabView)
	ON_MESSAGE(UWM_MSG_VIEW_REFRESH,OnRefresh)
	
	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, OnComputerBarSelectionChanged)

	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnFontChangeAll)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnFontHasChanged)

	ON_MESSAGE (UWM_MSG_GET_FLOATER_DATA, OnFloaterData)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_CPU, OnSetSnoozeCpu)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_GPU, OnSetSnoozeGpu)

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistorySetWidth)

	ON_MESSAGE (UWM_MSG_PREFERENCE_DIALOG, OnPreferenceDialog)
	ON_MESSAGE (UWM_MSG_PROXY_DIALOG, OnProxyDialog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MESSAGE_CLIPBOARD_SELECTED, &CRemoteCombinedMessageView::OnClipBoardSelected)
	ON_COMMAND(ID_MESSAGE_CLIPBOARD_ALL, &CRemoteCombinedMessageView::OnClipBoardAll)
	ON_COMMAND(ID_MESSAGE_SEARCH, &CRemoteCombinedMessageView::OnSearch)
	

	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+0, &CRemoteCombinedMessageView::OnComputerSelected0)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+1, &CRemoteCombinedMessageView::OnComputerSelected1)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+2, &CRemoteCombinedMessageView::OnComputerSelected2)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+3, &CRemoteCombinedMessageView::OnComputerSelected3)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+4, &CRemoteCombinedMessageView::OnComputerSelected4)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+5, &CRemoteCombinedMessageView::OnComputerSelected5)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+6, &CRemoteCombinedMessageView::OnComputerSelected6)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+7, &CRemoteCombinedMessageView::OnComputerSelected7)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+8, &CRemoteCombinedMessageView::OnComputerSelected8)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+9, &CRemoteCombinedMessageView::OnComputerSelected9)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+10, &CRemoteCombinedMessageView::OnComputerSelected10)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+11, &CRemoteCombinedMessageView::OnComputerSelected11)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+12, &CRemoteCombinedMessageView::OnComputerSelected12)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+13, &CRemoteCombinedMessageView::OnComputerSelected13)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+14, &CRemoteCombinedMessageView::OnComputerSelected14)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+15, &CRemoteCombinedMessageView::OnComputerSelected15)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+16, &CRemoteCombinedMessageView::OnComputerSelected16)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+17, &CRemoteCombinedMessageView::OnComputerSelected17)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+18, &CRemoteCombinedMessageView::OnComputerSelected18)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+19, &CRemoteCombinedMessageView::OnComputerSelected19)
	ON_COMMAND(UWM_MSG_MESSAGE_COMPUTER_SELECTED+20, &CRemoteCombinedMessageView::OnComputerSelected20)

	ON_COMMAND(ID_MESSAGE_PROJECT_FILTER_NONE, &CRemoteCombinedMessageView::OnFilterProjectNone)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_PROJECT_FILTER_NONE,&CRemoteCombinedMessageView::OnUpdateFilterProjectNone)
	ON_COMMAND(ID_MESSAGE_PROJECT_FILTER_GENERAL, &CRemoteCombinedMessageView::OnFilterProjectGeneral)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_PROJECT_FILTER_GENERAL,&CRemoteCombinedMessageView::OnUpdateFilterProjectGeneral)


	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+0, &CRemoteCombinedMessageView::OnFilterProject0)	
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+1, &CRemoteCombinedMessageView::OnFilterProject1)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+2, &CRemoteCombinedMessageView::OnFilterProject2)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+3, &CRemoteCombinedMessageView::OnFilterProject3)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+4, &CRemoteCombinedMessageView::OnFilterProject4)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+5, &CRemoteCombinedMessageView::OnFilterProject5)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+6, &CRemoteCombinedMessageView::OnFilterProject6)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+7, &CRemoteCombinedMessageView::OnFilterProject7)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+8, &CRemoteCombinedMessageView::OnFilterProject8)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+9, &CRemoteCombinedMessageView::OnFilterProject9)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+10, &CRemoteCombinedMessageView::OnFilterProject10)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+11, &CRemoteCombinedMessageView::OnFilterProject11)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+12, &CRemoteCombinedMessageView::OnFilterProject12)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+13, &CRemoteCombinedMessageView::OnFilterProject13)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+14, &CRemoteCombinedMessageView::OnFilterProject14)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+15, &CRemoteCombinedMessageView::OnFilterProject15)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+16, &CRemoteCombinedMessageView::OnFilterProject16)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+17, &CRemoteCombinedMessageView::OnFilterProject17)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+18, &CRemoteCombinedMessageView::OnFilterProject18)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+19, &CRemoteCombinedMessageView::OnFilterProject19)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+20, &CRemoteCombinedMessageView::OnFilterProject20)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+21, &CRemoteCombinedMessageView::OnFilterProject21)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+22, &CRemoteCombinedMessageView::OnFilterProject22)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+23, &CRemoteCombinedMessageView::OnFilterProject23)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+24, &CRemoteCombinedMessageView::OnFilterProject24)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+25, &CRemoteCombinedMessageView::OnFilterProject25)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+26, &CRemoteCombinedMessageView::OnFilterProject26)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+27, &CRemoteCombinedMessageView::OnFilterProject27)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+28, &CRemoteCombinedMessageView::OnFilterProject28)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+29, &CRemoteCombinedMessageView::OnFilterProject29)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+30, &CRemoteCombinedMessageView::OnFilterProject30)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+31, &CRemoteCombinedMessageView::OnFilterProject31)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+32, &CRemoteCombinedMessageView::OnFilterProject32)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+33, &CRemoteCombinedMessageView::OnFilterProject33)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+34, &CRemoteCombinedMessageView::OnFilterProject34)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+35, &CRemoteCombinedMessageView::OnFilterProject35)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+36, &CRemoteCombinedMessageView::OnFilterProject36)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+37, &CRemoteCombinedMessageView::OnFilterProject37)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+38, &CRemoteCombinedMessageView::OnFilterProject38)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+39, &CRemoteCombinedMessageView::OnFilterProject39)

	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+40, &CRemoteCombinedMessageView::OnFilterProject40)	
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+41, &CRemoteCombinedMessageView::OnFilterProject41)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+42, &CRemoteCombinedMessageView::OnFilterProject42)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+43, &CRemoteCombinedMessageView::OnFilterProject43)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+44, &CRemoteCombinedMessageView::OnFilterProject44)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+45, &CRemoteCombinedMessageView::OnFilterProject45)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+46, &CRemoteCombinedMessageView::OnFilterProject46)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+47, &CRemoteCombinedMessageView::OnFilterProject47)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+48, &CRemoteCombinedMessageView::OnFilterProject48)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+49, &CRemoteCombinedMessageView::OnFilterProject49)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+50, &CRemoteCombinedMessageView::OnFilterProject50)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+51, &CRemoteCombinedMessageView::OnFilterProject51)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+52, &CRemoteCombinedMessageView::OnFilterProject52)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+53, &CRemoteCombinedMessageView::OnFilterProject53)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+54, &CRemoteCombinedMessageView::OnFilterProject54)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+55, &CRemoteCombinedMessageView::OnFilterProject55)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+56, &CRemoteCombinedMessageView::OnFilterProject56)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+57, &CRemoteCombinedMessageView::OnFilterProject57)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+58, &CRemoteCombinedMessageView::OnFilterProject58)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+59, &CRemoteCombinedMessageView::OnFilterProject59)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+60, &CRemoteCombinedMessageView::OnFilterProject60)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+61, &CRemoteCombinedMessageView::OnFilterProject61)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+62, &CRemoteCombinedMessageView::OnFilterProject62)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+63, &CRemoteCombinedMessageView::OnFilterProject63)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+64, &CRemoteCombinedMessageView::OnFilterProject64)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+65, &CRemoteCombinedMessageView::OnFilterProject65)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+66, &CRemoteCombinedMessageView::OnFilterProject66)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+67, &CRemoteCombinedMessageView::OnFilterProject67)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+68, &CRemoteCombinedMessageView::OnFilterProject68)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+69, &CRemoteCombinedMessageView::OnFilterProject69)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+70, &CRemoteCombinedMessageView::OnFilterProject70)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+71, &CRemoteCombinedMessageView::OnFilterProject71)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+72, &CRemoteCombinedMessageView::OnFilterProject72)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+73, &CRemoteCombinedMessageView::OnFilterProject73)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+74, &CRemoteCombinedMessageView::OnFilterProject74)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+75, &CRemoteCombinedMessageView::OnFilterProject75)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+76, &CRemoteCombinedMessageView::OnFilterProject76)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+77, &CRemoteCombinedMessageView::OnFilterProject77)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+78, &CRemoteCombinedMessageView::OnFilterProject78)
	ON_COMMAND(UWM_MSG_MESSAGE_PROJECT_FILTER+79, &CRemoteCombinedMessageView::OnFilterProject79)

	ON_MESSAGE (UWM_DLG_SEARCH_COMPUTER_READY, OnSearchComputerReady)	
	ON_MESSAGE (UWM_MSG_WEB_ITEM_SELECTED, OnWebItemSelected)
	ON_MESSAGE(UWM_MSG_WEB_SORT,OnWebSort)
	ON_COMMAND(ID_COMPUTERS_DETECT, &CRemoteCombinedMessageView::OnComputersDetect)
END_MESSAGE_MAP()

LRESULT CRemoteCombinedMessageView::OnTimerDlg(WPARAM parm1, LPARAM parm2)
{
	int		iNrRpc;
	int		iThreadRpcId;
	bool	bUpdateAll;
	HWND	hWnd;
	CRemoteCombinedMessageView *pView;
	CString sSelectedComputer, sFormat;
	int		iSelectedComputer, iThreadId;
	bool	bVisible;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	bVisible = theApp.m_pMainWnd->IsWindowVisible() == TRUE;

	CheckAndSaveColumnsWidth();

	bUpdateAll = (parm1 == PARAM_MESSAGE_TRICKLE);

	if (m_bThreadBusy)		// check for lockup
	{
		if (bVisible && m_pDoc->m_iViewActive == MESSAGE_VIEW_ACTIVE)
		{
			CString sFormat;
			if (m_iThreadLocked > DELAY_SHOW_UPDATE_TIMER) sFormat.Format(" %d", m_iThreadLocked);
			sFormat = gszTranslation[PosWindowStatusMessageStatusUpdating] + sFormat;
			theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
			sFormat.ReleaseBuffer();
		}
		if (m_iThreadLocked++ > 90) {m_iThreadLocked = 0; m_bThreadBusy = false;}
		return 0;
	}
	m_iThreadLocked = 0;

	if (m_bThreadBusy) return 0;
	if (m_pDoc == NULL) return 0;
	
	iNrRpc = m_pDoc->GetRpcSelectedComputers();

	m_iSendRpcCount = 0;			
	m_iReadyRpcCount = 0;
	m_iMessagesTotal = 0;

	pView = m_pDoc->m_pMessageView;
	hWnd = pView->m_hWnd;

	if (m_bMessagesCorrupt)
	{
		m_bThreadRpcInit = true;
		m_bMessagesCorrupt = false;
	}

	iSelectedComputer = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_GET_SELECTED, (WPARAM) &sSelectedComputer, 0);
	if (iSelectedComputer == 1) // only if one is selected
	{
		if (m_pDoc->GetRpcThreadId(&sSelectedComputer, &iThreadId))
		{
			if (m_iThreadRpcId != iThreadId)
			{
				ClearProjectFilterList();
				m_bFilterAllProjects = true;

				m_iThreadRpcId = iThreadId;
				m_bThreadRpcInit = true;
			}
		}
	}
	else
	{
		if (m_iThreadRpcId == -1)
		{
			iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;
			if (m_iThreadRpcId != iThreadId)
			{
				m_iThreadRpcId = iThreadId;
				m_bThreadRpcInit = true;
			}
		}
	}

	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		m_pDoc->GetRpcThreadId(&iThreadRpcId, iCount);
		if (bUpdateAll || m_iThreadRpcId == iThreadRpcId)
		{
			int iInit;
			if (m_bThreadRpcInit) iInit = 1;
			else iInit = 0;

			CRpcThreadSendMessage *pRpcThreadSendMessage;
			pRpcThreadSendMessage = new CRpcThreadSendMessage;
			pRpcThreadSendMessage->m_iInit = iInit;
			pRpcThreadSendMessage->m_iKeepMessages = theApp.m_pDlgSettingsMessage->m_iKeepMessages;

			::PostThreadMessage(iThreadRpcId,UWM_MSG_THREAD_START_RPC_MESSAGE,(WPARAM) hWnd, (LPARAM) pRpcThreadSendMessage);	// return naar message view!
			m_bThreadBusy = true;	
			m_iSendRpcCount++;
			m_bThreadRpcInit= false;
		}
	}

	return 0;
}

LRESULT CRemoteCombinedMessageView::OnReadyRpc(WPARAM parm1, LPARAM parm2)
{
	LV_ITEM					lvi;
//	LV_COLUMN				lvc;
	int						iCount, iNrMessages, iItemsAdd, iItemsAddTemp, iNrItems, iAddPos, iStatus, iTemp, iRow, iLastReadMessages, iNrItemsInitial, iInit, iNrFilter;
//	char					cBuffer[256];
	CRpcThreadReturn		*pThreadReturn;
//	CRpcThreadInfoMessage	*pInfoThread;
	CRpcMessageInfo			info;
	CThreadRpc				*pThreadRpc;
	bool					bLineColor = false, bOverride = false, bSorting;
	char					*pcComputerId;
	CString					sProject;

	iStatus = (int) parm1;
	pThreadReturn = (CRpcThreadReturn *)	parm2;

	if (m_bDocumentIsClosing)
	{
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusClosing]);
		delete pThreadReturn;
		return 0;			// everything is closing down.
	}
	
	iNrMessages = pThreadReturn->m_iNumber;
	m_iMessagesTotal+= iNrMessages;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_iNrMessages = iNrMessages;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_iLastReadMessages = pThreadReturn->m_iNrLastRead;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_pcComputerId = pThreadReturn->m_pcComputerId;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_iThreadId = pThreadReturn->m_iThreadId;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_pThread = pThreadReturn->m_pThread;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_iStatusRpc = iStatus;
	m_lComputerRpcInfo.at(m_iReadyRpcCount)->m_iInit = pThreadReturn->m_iInit;
	m_iReadyRpcCount++;

	theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_CONNECT, (WPARAM) pThreadReturn->m_pcComputerId, pThreadReturn->m_iStatus);

	delete pThreadReturn;
	pThreadReturn = NULL;

	if (m_iReadyRpcCount != m_iSendRpcCount) return 0;	// wait for all results to come in

	CDlgSettingsView *pSettingsView;
	pSettingsView = theApp.m_pDlgSettingsView;
	m_bUseHorizontalGrid = pSettingsView->m_bUseHorizontalGrid;
	m_bUseVerticalGrid = pSettingsView ->m_bUseVerticalGrid;

	// setup sorting
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		m_lComputerRpcInfo.at(iCount)->m_iSortIndex = iCount;
	}
	
	// no sorting if there is less than 2
	if (m_iReadyRpcCount > 1)
	{
		bSorting = true;
		while (bSorting)
		{
			bSorting = false;
			for (iCount = 0; iCount < m_iReadyRpcCount-1; iCount++)
			{
				if (strcmp(m_lComputerRpcInfo.at(m_lComputerRpcInfo.at(iCount)->m_iSortIndex)->m_pcComputerId, m_lComputerRpcInfo.at(m_lComputerRpcInfo.at(iCount+1)->m_iSortIndex)->m_pcComputerId) > 0)
				{
					iTemp = m_lComputerRpcInfo.at(iCount)->m_iSortIndex;
					m_lComputerRpcInfo.at(iCount)->m_iSortIndex = m_lComputerRpcInfo.at(iCount+1)->m_iSortIndex;
					m_lComputerRpcInfo.at(iCount+1)->m_iSortIndex = iTemp;
					bSorting = true;
				}
			}
		}
	}

	m_pDoc->DisplayStatus(m_iReadyRpcCount, &m_lComputerRpcInfo.at(0)->m_iStatusRpc);//, &m_rpcInfo.m_pcComputerId[0]);

	// find selected computer
	iNrMessages = 0;
	iNrFilter = 0;
	iLastReadMessages = 0;
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		if (m_iThreadRpcId == m_lComputerRpcInfo.at(iCount)->m_iThreadId)
		{
			iNrMessages = m_lComputerRpcInfo.at(iCount)->m_iNrMessages;
			iInit = m_lComputerRpcInfo.at(iCount)->m_iInit;
			if ((iInit > 0) || m_bForceRefreshAll)
			{
				iLastReadMessages = 1;
			}
			else
			{
				iLastReadMessages = m_lComputerRpcInfo.at(iCount)->m_iLastReadMessages;
			}

			// filter only a specific project

			for (int iCountFilter = 0; iCountFilter < iNrMessages; iCountFilter++)
			{
				m_lComputerRpcInfo.at(iCount)->m_pThread->GetRpcMessage(iCountFilter,&info);
				sProject = info.m_pszProject;
				if (IsFiltered(&sProject))
				{
					iNrFilter++;
				}
			}
			// filter only a specific project
		}
		else 
		{
			// wrong thread
			m_bThreadBusy = false;
			theApp.m_pMainFrame->SetBoincTasksStatus(TEXT_STATUS_WAITING);
			return FALSE;
		}
	}

	CListCtrl& listCtrl = GetListCtrl();
	if ( iInit != 0)
	{
//		listCtrl.DeleteAllItems();
	}

	iNrItems = listCtrl.GetItemCount();
	iNrItemsInitial = iNrItems;
	iItemsAdd = iNrItems - iNrMessages;						// and the messages
	iItemsAdd+= iNrFilter;

	m_iSelected = iNrItems -1;

	if (iItemsAdd > 0)
	{
		iItemsAddTemp = iItemsAdd;
		// delete list items
		bOverride = true;
		while (iItemsAddTemp > 0)
		{
			int iLast;
			iLast = listCtrl.GetItemCount();
			if (m_iSortingPrimary == SORTING_AZ)	listCtrl.DeleteItem(0);
			else									listCtrl.DeleteItem(iLast-1);
			iNrItems--;
			iItemsAddTemp--;
		}
	}
	else
	{
		if (iItemsAdd < 0)
		{
			iItemsAddTemp = iItemsAdd;
			// add list items
			bOverride = true;
			while (iItemsAddTemp < 0)
			{
				int iLast;
				iLast = listCtrl.GetItemCount();
				lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
				iAddPos =  iNrItems-1;
				if (iAddPos < 0) iAddPos = 0;
				if (m_iSortingPrimary == SORTING_AZ)
				{
					lvi.iItem = iLast;							// add at the end
				}
				else
				{
					lvi.iItem = 0;								// add at the beginning
				}
				lvi.iSubItem = 0;
				lvi.iImage = 0;
				lvi.pszText = MESSAGE_INIT_TEXT;				// info.m_pszProject;	
				lvi.iImage = iAddPos;							// add at the end
				lvi.stateMask = LVIS_STATEIMAGEMASK;
				lvi.state = INDEXTOSTATEIMAGEMASK(1);
				lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_NORMAL];
				listCtrl.InsertItem(&lvi);
				iNrItems++;
				iItemsAddTemp++;
			}
		}
	}

/*
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxx test init
	iAddPos = 10;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
	lvi.iItem = iAddPos;
	lvi.iSubItem = 0;
	lvi.iImage = 0;
	lvi.pszText = MESSAGE_INIT_TEXT;
	lvi.iImage = iAddPos;	
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK(1);
	lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_NORMAL];
	listCtrl.InsertItem(&lvi);
	// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*/

	// for every computer

	bool	bFoundComputer = false, bChanged = false;
	iRow = 0;
	for (int iComputer = 0; iComputer < m_iReadyRpcCount; iComputer++)
	{
		pThreadRpc = m_lComputerRpcInfo.at(m_lComputerRpcInfo.at(iComputer)->m_iSortIndex)->m_pThread;
		pcComputerId = m_lComputerRpcInfo.at(m_lComputerRpcInfo.at(iComputer)->m_iSortIndex)->m_pcComputerId;

		// only computers

		if (m_iThreadRpcId == m_lComputerRpcInfo.at(m_lComputerRpcInfo.at(iComputer)->m_iSortIndex)->m_iThreadId)
		{
			bChanged = DisplayRpc(pThreadRpc, iNrMessages, iNrFilter, iLastReadMessages, pcComputerId);
			// check if there are items that are still in the init state
//			m_bMessagesCorrupt = false;
			if (CheckIfMessagesCorrupt(&listCtrl))
			{
				bChanged = DisplayRpc(pThreadRpc, iNrMessages, iNrFilter, 1, pcComputerId);			// from the beginning
			}
		}
	}

	// if on of the last items is selected keep end visible

 	iNrItems = listCtrl.GetItemCount();


	// check if end selected
	if (listCtrl.GetItemState(iNrItemsInitial-1,LVIS_SELECTED))
	{
		listCtrl.EnsureVisible(iNrItems-1,FALSE);							// take new end
	}
	else
	{
		if (m_bAlwaysShow)// no selection, keep end or begin in sight, but only if not scrolled.
		{
			if (listCtrl.GetSelectedCount() == 0)
			{
				if (m_iSortingPrimary == SORTING_AZ)	listCtrl.EnsureVisible(iNrItems-1,FALSE);							// take new end
				else									listCtrl.EnsureVisible(0,FALSE);									// take new begin
			}
		}
	}
	if (iItemsAdd !=0) Invalidate();

	if (g_bWebServerActive)
	{
		if (bChanged) m_bWebserverNeedsUpdate = true;
		if (m_bWebserverNeedsUpdate)
		{
			if (!m_bDocumentIsClosing && (g_pcWebServerHtml == NULL))
			{
				m_bWebserverNeedsUpdate = false;
				CCreateHtmlWebServer createHtml;
				char *pcHtml;
				if (createHtml.Create(PosBarViewSelectMessages, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_MESSAGE_COLUMNS, NULL, PosGroupViewTasksMessages, &pcHtml))
				{
					g_pcWebServerHtml = pcHtml;
					g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_MESSAGES);
					g_tWebServerHtml = GetTickCount()/100;
				}
			}
		}
	}

	if (g_bCloudServerRequestData)
	{
		char *pcHtml;
		if (bChanged) m_bCloudserverNeedsUpdate = true;
		if (m_bCloudserverNeedsUpdate)
		{
			if (g_pcCloudServerHtml == NULL)
			{
				if (g_iCloudServerHtmlTabProcessed[TAB_MESSAGES] == TRUE)
				{
					g_iCloudServerHtmlTabProcessed[TAB_MESSAGES] = FALSE;
					m_bCloudserverNeedsUpdate = false;
					CCreateHtmlWebServer createHtml;
					if (createHtml.Create(PosBarViewSelectMessages, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_MESSAGE_COLUMNS, NULL, PosGroupViewTasksMessages, &pcHtml))
					{
						g_pcCloudServerHtml = pcHtml;
						g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_MESSAGES);
						g_tCloudServerHtml = GetTickCount()/100;
					}
				}
			}
		}
		else
		{
			if (g_pcCloudServerHtml == NULL)
			{
				if (g_iCloudServerHtmlTabProcessed[TAB_MESSAGES] == TRUE)
				{
					g_iCloudServerHtmlTabProcessed[TAB_MESSAGES] = FALSE;
					CString sTxt = "<equ>";
					int iLen =sTxt.GetLength()+1;
					pcHtml = new char [iLen];
					strcpy_s(pcHtml, iLen, sTxt.GetBuffer());
					g_pcCloudServerHtml = pcHtml;
					g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_MESSAGES);
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

	m_bFirstTime = false;
	m_bForceRefreshAll = false;
	return 0;
}

// CRemoteCombinedMessageView diagnostics

#ifdef _DEBUG
void CRemoteCombinedMessageView::AssertValid() const
{
	CTemplateRemoteMessageView::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteCombinedMessageView::Dump(CDumpContext& dc) const
{
	CTemplateRemoteMessageView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRemoteCombinedMessageView message handlers


void CRemoteCombinedMessageView::OnContextMenu(CWnd* pWnd, CPoint point) 
{ 
	int		iPos;
	CPoint	clientPoint, startPoint;
	int iItems, iFlags, iRetVal;
	bool bSelected, bHeader;

	if (m_pDoc == NULL) return;

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


	//our menu
	CMenu popupMenu, popupProjectMenu; 

	int iNr;
	char *pc;
	iNr = m_pDoc->GetRpcSelectedComputers();

	if (iNr == 0) return;

	contextMenu.CreateMenu();
	popupMenu.CreatePopupMenu();
	popupProjectMenu.CreatePopupMenu();

	popupProjectMenu.AppendMenu(MF_ENABLED|MF_STRING,ID_MESSAGE_PROJECT_FILTER_NONE, gszTranslation[PosPopUpMessagesShowOnlyAll]);
	popupProjectMenu.AppendMenu(MF_ENABLED|MF_STRING,ID_MESSAGE_PROJECT_FILTER_GENERAL, gszTranslation[PosPopUpMessagesIncludeGeneral]);// "Include general messages");
	popupProjectMenu.AppendMenu(MF_SEPARATOR,0,"");


	CListCtrl& listCtrl = GetListCtrl();
	iItems = listCtrl.GetItemCount();
	bSelected = false;
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (listCtrl.GetItemState(iCount,LVIS_SELECTED))
		{
			bSelected = true;
			break;
		}
	}

	int		iProjectsFound = 0;

//	ClearProjectFilterList();

	CString sProjectName;

	// add filter items

	int iColumn;
	iColumn = m_iColumnOrder[COLUMN_MESSAGE_PROJECT];

	if (iColumn >= 0)
	{
		for (int iCount = 0; iCount < iItems; iCount++)
		{
			bool bDouble = false;
			sProjectName = listCtrl.GetItemText(iCount, iColumn);
			if (sProjectName != "")
			{
				for (int iCountProjects = 0; iCountProjects < (int) m_lProjectFilter.size(); iCountProjects++)
				{
					if (sProjectName == m_lProjectFilter.at(iCountProjects)->m_sProject)		//m_sProjectFilterList[iCountProjects])
					{
						bDouble = true;
						break;
					}
				}
				if (!bDouble)
				{
//					if (iProjectsFound >= MAX_MESSAGE_PROJECT_FILTER) break;
//	
//					iFlags = MF_ENABLED|MF_STRING;
//					if (m_sProjectFilter != "")
//					{
//						iFlags += MF_CHECKED;
//					}
					
					CProjectFilterItem *pProjectItem = new CProjectFilterItem;
					pProjectItem->m_sProject = sProjectName;
					pProjectItem->m_bSelected = false;
					m_lProjectFilter.push_back(pProjectItem);

//					popupProjectMenu.AppendMenu(iFlags,UWM_MSG_MESSAGE_PROJECT_FILTER+iProjectsFound,sProjectName);
//					m_sProjectFilterList[iProjectsFound] = sProjectName;
//					iProjectsFound++;
				}
			}
		}
	}

	// Sort the list
	if (m_lProjectFilter.size() > 2)
	{
		bool	bSorted = true;
		while (bSorted)
		{
			bSorted = false;
			for (iPos = 0; iPos < (int) m_lProjectFilter.size()-1; iPos++)
			{
				if (m_lProjectFilter.at(iPos)->m_sProject.CompareNoCase(m_lProjectFilter.at(iPos+1)->m_sProject) > 0)
				{
					CProjectFilterItem *pTemp;
					pTemp = m_lProjectFilter.at(iPos);
					m_lProjectFilter.at(iPos) = m_lProjectFilter.at(iPos+1);
					m_lProjectFilter.at(iPos+1) = pTemp;
					bSorted = true;
				}
			}
		}
	}

	for (iPos = 0; iPos < (int) m_lProjectFilter.size(); iPos++)
	{
//		if (iProjectsFound >= MAX_MESSAGE_PROJECT_FILTER) break;
		iFlags = MF_ENABLED|MF_STRING;
		if (m_lProjectFilter.at(iPos)->m_bSelected)
		{
			iFlags += MF_CHECKED;
		}

		popupProjectMenu.AppendMenu(iFlags,UWM_MSG_MESSAGE_PROJECT_FILTER+iPos,m_lProjectFilter.at(iPos)->m_sProject);
	}


	popupMenu.AppendMenu(MF_POPUP,(UINT_PTR) popupProjectMenu.m_hMenu, gszTranslation[PosPopUpMessagesShowOnly]);
	popupMenu.AppendMenu(MF_SEPARATOR,0,"");
	popupMenu.AppendMenu(MF_ENABLED|MF_STRING,ID_MESSAGE_SEARCH,gszTranslation[PosPopUpMessagesSearch]);
	popupMenu.AppendMenu(MF_SEPARATOR,0,"");
	popupMenu.AppendMenu(MF_ENABLED|MF_STRING,ID_MESSAGE_CLIPBOARD_ALL,gszTranslation[PosPopUpMessagesCopyAll]);
	if (bSelected) popupMenu.AppendMenu(MF_ENABLED|MF_STRING,ID_MESSAGE_CLIPBOARD_SELECTED,gszTranslation[PosPopUpMessagesCopySelected]);
	popupMenu.AppendMenu(MF_SEPARATOR,0,"");

	for (int iCount = 0; iCount < iNr; iCount++)
	{
		m_pDoc->GetComputerIds(&pc, iCount);
		popupMenu.AppendMenu(MF_ENABLED|MF_STRING,UWM_MSG_MESSAGE_COMPUTER_SELECTED+iCount, pc);
		pc++;
	}

	contextMenu.AppendMenu(MF_POPUP, (UINT_PTR) popupMenu.m_hMenu, _T("-"));


	//we have to translate the mouse coordinates first:
	//these are relative to the window (or view) but the context menu
	//needs coordinates that are relative to the whole screen
	ClientToScreen(&point);
 
	//tracker now points to IDR_CONTEXT's first submenu 
	//GetSubMenu(0) -&gt; returns the first submenu,
	// GetSubMenu(1)-&gt; second one etc..
	tracker = contextMenu.GetSubMenu(0); 

	//show the context menu
	tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y , AfxGetMainWnd()); 

	//has always to be called (function of base class)
//   CListViewEx::OnRButtonDown(nFlags, point); 

	popupMenu.DestroyMenu();
	contextMenu.DestroyMenu();
	popupProjectMenu.DestroyMenu();
}


void CRemoteCombinedMessageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl& listCtrl = GetListCtrl();

	int row = listCtrl.HitTest( point, NULL );

//	m_sSelected = listCtrl.GetItemText(row, COLUMN_NAME);
	m_iSelected = row;

	CTemplateRemoteMessageView::OnLButtonDown(nFlags, point);

//	listCtrl.setsel
//	m_bChangedSelected = true;
//	OnTimerDlg(0,0);
}

LRESULT CRemoteCombinedMessageView::OnSwitch(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (parm2 == REFRESH_VIEW_NOW) 
	{
//		m_iTimerWu = 1000;	// direct action
	}

	m_iSelected = -1;
	m_bCloudserverNeedsUpdate = true;
	theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnSettingsTasks(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing)
	{
		return 0;			// everything is closing down.
	}
	if (m_pDoc != NULL) m_pDoc->SettingsTasks(parm1 != 0);
	return 0;
}

void CRemoteCombinedMessageView::OnSize(UINT nType, int cx, int cy )
{
	theApp.m_pMainFrame->m_dlgBarComputer.PostMessage(UWM_MSG_NEW_SIZE, cx, cy);
	CTemplateRemoteMessageView::OnSize(nType, cx, cy );
}

LRESULT CRemoteCombinedMessageView::OnColumnChanged(WPARAM parm1, LPARAM parm2)
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
		for (int iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
		{
			listCtrl.SetColumnWidth(iCount, *pColumn);
			pColumn++;
		}
	}
	return 0;
}

void CRemoteCombinedMessageView::OnFilterProjectNone()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	m_bFilterAllProjects = !m_bFilterAllProjects;

	if (m_bFilterAllProjects)
	{
		for (int iPos = 0; iPos < (int) m_lProjectFilter.size(); iPos++)
		{
			m_lProjectFilter.at(iPos)->m_bSelected = false;
		}
	}
	m_bFilterChanged = true;
}

void CRemoteCombinedMessageView::OnUpdateFilterProjectNone(CCmdUI* pCmdUI)
{
	if (m_bFilterAllProjects)	pCmdUI->SetCheck(TRUE);
	else pCmdUI->SetCheck(FALSE);
}

void CRemoteCombinedMessageView::OnFilterProjectGeneral()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	if (m_bFilterGeneral) m_bFilterGeneral = false;
	else m_bFilterGeneral = true;

	m_bFilterChanged = true;
}

void CRemoteCombinedMessageView::OnUpdateFilterProjectGeneral(CCmdUI* pCmdUI)
{
//	if (m_sProjectFilter == "")
//	{
//		pCmdUI->Enable(FALSE);
//		pCmdUI->SetCheck(TRUE);
//		return;
//	}
//
//	pCmdUI->Enable(TRUE);

	if (m_bFilterGeneral)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CRemoteCombinedMessageView::OnFilterProject0(){ FilterProject(0); }
void CRemoteCombinedMessageView::OnFilterProject1(){ FilterProject(1); }
void CRemoteCombinedMessageView::OnFilterProject2(){ FilterProject(2); }
void CRemoteCombinedMessageView::OnFilterProject3(){ FilterProject(3); }
void CRemoteCombinedMessageView::OnFilterProject4(){ FilterProject(4); }
void CRemoteCombinedMessageView::OnFilterProject5(){ FilterProject(5); }
void CRemoteCombinedMessageView::OnFilterProject6(){ FilterProject(6); }
void CRemoteCombinedMessageView::OnFilterProject7(){ FilterProject(7); }
void CRemoteCombinedMessageView::OnFilterProject8(){ FilterProject(8); }
void CRemoteCombinedMessageView::OnFilterProject9(){ FilterProject(9); }
void CRemoteCombinedMessageView::OnFilterProject10(){ FilterProject(10); }
void CRemoteCombinedMessageView::OnFilterProject11(){ FilterProject(11); }
void CRemoteCombinedMessageView::OnFilterProject12(){ FilterProject(12); }
void CRemoteCombinedMessageView::OnFilterProject13(){ FilterProject(13); }
void CRemoteCombinedMessageView::OnFilterProject14(){ FilterProject(14); }
void CRemoteCombinedMessageView::OnFilterProject15(){ FilterProject(15); }
void CRemoteCombinedMessageView::OnFilterProject16(){ FilterProject(16); }
void CRemoteCombinedMessageView::OnFilterProject17(){ FilterProject(17); }
void CRemoteCombinedMessageView::OnFilterProject18(){ FilterProject(18); }
void CRemoteCombinedMessageView::OnFilterProject19(){ FilterProject(19); }
void CRemoteCombinedMessageView::OnFilterProject20(){ FilterProject(20); }
void CRemoteCombinedMessageView::OnFilterProject21(){ FilterProject(21); }
void CRemoteCombinedMessageView::OnFilterProject22(){ FilterProject(22); }
void CRemoteCombinedMessageView::OnFilterProject23(){ FilterProject(23); }
void CRemoteCombinedMessageView::OnFilterProject24(){ FilterProject(24); }
void CRemoteCombinedMessageView::OnFilterProject25(){ FilterProject(25); }
void CRemoteCombinedMessageView::OnFilterProject26(){ FilterProject(26); }
void CRemoteCombinedMessageView::OnFilterProject27(){ FilterProject(27); }
void CRemoteCombinedMessageView::OnFilterProject28(){ FilterProject(28); }
void CRemoteCombinedMessageView::OnFilterProject29(){ FilterProject(29); }
void CRemoteCombinedMessageView::OnFilterProject30(){ FilterProject(30); }
void CRemoteCombinedMessageView::OnFilterProject31(){ FilterProject(31); }
void CRemoteCombinedMessageView::OnFilterProject32(){ FilterProject(32); }
void CRemoteCombinedMessageView::OnFilterProject33(){ FilterProject(33); }
void CRemoteCombinedMessageView::OnFilterProject34(){ FilterProject(34); }
void CRemoteCombinedMessageView::OnFilterProject35(){ FilterProject(35); }
void CRemoteCombinedMessageView::OnFilterProject36(){ FilterProject(36); }
void CRemoteCombinedMessageView::OnFilterProject37(){ FilterProject(37); }
void CRemoteCombinedMessageView::OnFilterProject38(){ FilterProject(38); }
void CRemoteCombinedMessageView::OnFilterProject39(){ FilterProject(39); }
void CRemoteCombinedMessageView::OnFilterProject40(){ FilterProject(40); }
void CRemoteCombinedMessageView::OnFilterProject41(){ FilterProject(41); }
void CRemoteCombinedMessageView::OnFilterProject42(){ FilterProject(42); }
void CRemoteCombinedMessageView::OnFilterProject43(){ FilterProject(43); }
void CRemoteCombinedMessageView::OnFilterProject44(){ FilterProject(44); }
void CRemoteCombinedMessageView::OnFilterProject45(){ FilterProject(45); }
void CRemoteCombinedMessageView::OnFilterProject46(){ FilterProject(46); }
void CRemoteCombinedMessageView::OnFilterProject47(){ FilterProject(47); }
void CRemoteCombinedMessageView::OnFilterProject48(){ FilterProject(48); }
void CRemoteCombinedMessageView::OnFilterProject49(){ FilterProject(49); }
void CRemoteCombinedMessageView::OnFilterProject50(){ FilterProject(50); }
void CRemoteCombinedMessageView::OnFilterProject51(){ FilterProject(51); }
void CRemoteCombinedMessageView::OnFilterProject52(){ FilterProject(52); }
void CRemoteCombinedMessageView::OnFilterProject53(){ FilterProject(53); }
void CRemoteCombinedMessageView::OnFilterProject54(){ FilterProject(54); }
void CRemoteCombinedMessageView::OnFilterProject55(){ FilterProject(55); }
void CRemoteCombinedMessageView::OnFilterProject56(){ FilterProject(56); }
void CRemoteCombinedMessageView::OnFilterProject57(){ FilterProject(57); }
void CRemoteCombinedMessageView::OnFilterProject58(){ FilterProject(58); }
void CRemoteCombinedMessageView::OnFilterProject59(){ FilterProject(59); }
void CRemoteCombinedMessageView::OnFilterProject60(){ FilterProject(60); }
void CRemoteCombinedMessageView::OnFilterProject61(){ FilterProject(61); }
void CRemoteCombinedMessageView::OnFilterProject62(){ FilterProject(62); }
void CRemoteCombinedMessageView::OnFilterProject63(){ FilterProject(63); }
void CRemoteCombinedMessageView::OnFilterProject64(){ FilterProject(64); }
void CRemoteCombinedMessageView::OnFilterProject65(){ FilterProject(65); }
void CRemoteCombinedMessageView::OnFilterProject66(){ FilterProject(66); }
void CRemoteCombinedMessageView::OnFilterProject67(){ FilterProject(67); }
void CRemoteCombinedMessageView::OnFilterProject68(){ FilterProject(68); }
void CRemoteCombinedMessageView::OnFilterProject69(){ FilterProject(69); }
void CRemoteCombinedMessageView::OnFilterProject70(){ FilterProject(70); }
void CRemoteCombinedMessageView::OnFilterProject71(){ FilterProject(71); }
void CRemoteCombinedMessageView::OnFilterProject72(){ FilterProject(72); }
void CRemoteCombinedMessageView::OnFilterProject73(){ FilterProject(73); }
void CRemoteCombinedMessageView::OnFilterProject74(){ FilterProject(74); }
void CRemoteCombinedMessageView::OnFilterProject75(){ FilterProject(75); }
void CRemoteCombinedMessageView::OnFilterProject76(){ FilterProject(76); }
void CRemoteCombinedMessageView::OnFilterProject77(){ FilterProject(77); }
void CRemoteCombinedMessageView::OnFilterProject78(){ FilterProject(78); }
void CRemoteCombinedMessageView::OnFilterProject79(){ FilterProject(79); }


void CRemoteCombinedMessageView::FilterProject(int iFilter)
{
	if (iFilter > (int) m_lProjectFilter.size()) return;

	m_lProjectFilter.at(iFilter)->m_bSelected = !m_lProjectFilter.at(iFilter)->m_bSelected;
	m_bFilterAllProjects = false;

//	bool bSelected = true; 
//	for (int iPos=0; iPos < m_lProjectFilter.size(); iPos++)
//	{
//		if (!m_lProjectFilter.at(iPos)->m_bSelected) bSelected = false;
//	}
//	if (bSelected) m_bFilterAllProjects = true;
//	else m_bFilterAllProjects = false;
	m_bFilterChanged = true;
}

void CRemoteCombinedMessageView::OnComputerSelected0(){ ComputerSelect(0); }
void CRemoteCombinedMessageView::OnComputerSelected1(){ ComputerSelect(1); }
void CRemoteCombinedMessageView::OnComputerSelected2(){ ComputerSelect(2); }
void CRemoteCombinedMessageView::OnComputerSelected3(){ ComputerSelect(3); }
void CRemoteCombinedMessageView::OnComputerSelected4(){ ComputerSelect(4); }
void CRemoteCombinedMessageView::OnComputerSelected5(){ ComputerSelect(5); }
void CRemoteCombinedMessageView::OnComputerSelected6(){ ComputerSelect(6); }
void CRemoteCombinedMessageView::OnComputerSelected7(){ ComputerSelect(7); }
void CRemoteCombinedMessageView::OnComputerSelected8(){ ComputerSelect(8); }
void CRemoteCombinedMessageView::OnComputerSelected9(){ ComputerSelect(9); }
void CRemoteCombinedMessageView::OnComputerSelected10(){ ComputerSelect(10); }
void CRemoteCombinedMessageView::OnComputerSelected11(){ ComputerSelect(11); }
void CRemoteCombinedMessageView::OnComputerSelected12(){ ComputerSelect(12); }
void CRemoteCombinedMessageView::OnComputerSelected13(){ ComputerSelect(13); }
void CRemoteCombinedMessageView::OnComputerSelected14(){ ComputerSelect(14); }
void CRemoteCombinedMessageView::OnComputerSelected15(){ ComputerSelect(15); }
void CRemoteCombinedMessageView::OnComputerSelected16(){ ComputerSelect(16); }
void CRemoteCombinedMessageView::OnComputerSelected17(){ ComputerSelect(17); }
void CRemoteCombinedMessageView::OnComputerSelected18(){ ComputerSelect(18); }
void CRemoteCombinedMessageView::OnComputerSelected19(){ ComputerSelect(19); }
void CRemoteCombinedMessageView::OnComputerSelected20(){ ComputerSelect(20); }

void CRemoteCombinedMessageView::ComputerSelect(int iSelected)
{
	int	iThreadRpcId;

	m_pDoc->GetRpcThreadId(&iThreadRpcId, iSelected);

	if (m_iThreadRpcId == iThreadRpcId) return; // already selected 

	m_iThreadRpcId = iThreadRpcId;
	m_bThreadRpcInit = true;

	m_iSelected = -1;

//	ClearProjectFilterList();
//	m_bProjectFilterAll = true;
}

LRESULT CRemoteCombinedMessageView::OnPreferenceDialog(WPARAM parm1, LPARAM parm2)
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

	int iColumn;
	iColumn = m_iColumnOrder[COLUMN_MESSAGE_COMPUTER];
	if (iColumn >=0) sComputer = listCtrl.GetItemText(nItem, iColumn);
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

LRESULT CRemoteCombinedMessageView::OnProxyDialog(WPARAM parm1, LPARAM parm2)
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

	int iColumn;
	iColumn = m_iColumnOrder[COLUMN_MESSAGE_COMPUTER];
	if (iColumn >= 0) sComputer = listCtrl.GetItemText(nItem, iColumn);
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

LRESULT CRemoteCombinedMessageView::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnSelTabView(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	m_pDoc->OnShowView((int) wParam, (int) lParam);
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam)
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

//	m_iTimerWu = 1000;				// 1000 to ensure direct timer message
	theApp.m_pMainFrame->SetBoincTasksStatus( gszTranslation[PosWindowStatusMessageStatusSwitched]);

	return 0;
}

void CRemoteCombinedMessageView::OnClipBoardSelected()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	ClipBoard(false);
}

void CRemoteCombinedMessageView::OnClipBoardAll()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.
	ClipBoard(true);
}

void CRemoteCombinedMessageView::OnSearch()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.


	int iColumn;
	int iItems;
	CString sSearch, sTxt;
	CDlgSearch dlg;
	INT_PTR iRet = dlg.DoModal();
	if (iRet == IDOK)
	{
		sSearch = dlg.m_sSearch;
		sSearch = sSearch.MakeLower();
		CListCtrl& listCtrl = GetListCtrl();
		iItems = listCtrl.GetItemCount();
		iColumn = m_iColumnOrder[COLUMN_MESSAGE_MESSAGE];
		if (iColumn >= 0)
		{
			for (int iCount = 0; iCount < iItems; iCount++)
			{
				listCtrl.SetItemState(iCount, 0, LVIS_SELECTED);
				sTxt = listCtrl.GetItemText(iCount, iColumn);
				sTxt = sTxt.MakeLower();
				if (sTxt.Find(sSearch,0) >=0)
				{
					listCtrl.SetItemState(iCount, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
		}
	}
}

LRESULT CRemoteCombinedMessageView::OnFloaterData(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_GET_FLOATER_DATA, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return -1;			// everything is closing down.
	return m_pDoc->m_pProjectView->PostMessage(UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, wParam, lParam);
}

LRESULT CRemoteCombinedMessageView::OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, wParam, lParam);
	return 0;
}

//LRESULT CRemoteCombinedMessageView::OnBoincTasksVersion(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_UPDATE,parm1,parm2);
//	return 0;
//}

//LRESULT CRemoteCombinedMessageView::OnBoincTasksRule(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_RULE,parm1,parm2);
//	return 0;
//}

LRESULT CRemoteCombinedMessageView::OnSearchComputerReady(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pComputerView->SendMessage(UWM_DLG_SEARCH_COMPUTER_READY,parm1,parm2);
	return 0;
}

LRESULT CRemoteCombinedMessageView::OnFontChangeAll(WPARAM wParam,LPARAM lParam)
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

LRESULT CRemoteCombinedMessageView::OnFontHasChanged(WPARAM wParam,LPARAM lParam)
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

LRESULT CRemoteCombinedMessageView::OnWebItemSelected(WPARAM wParam,LPARAM lParam)
{
/*
	int iRow, iOption;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	iRow = (int) wParam;
	iOption = (int) lParam;

	if (iRow >=0) m_iSelected = iRow;
//	m_pSelection->SelectionSet(iRow, iOption);

	char *pcHtml = NULL;
	if (!m_bThreadBusy)
	{
		CListCtrl& listCtrl = GetListCtrl();
		CCreateHtmlWebServer createHtml;
		createHtml.Create(PosBarViewSelectTasks, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_TASK_COLUMNS, PosGroupViewTasks, &pcHtml);
	}
	return (LRESULT) pcHtml;
*/

	return NULL;
}

LRESULT CRemoteCombinedMessageView::OnWebSort(WPARAM parm1, LPARAM parm2)
{
	char	*pcSort;
	BOOL	bHtml;
	int		iColumn;
	char	*pcHtml = NULL;

	pcSort = (char *) parm2;
	bHtml = (BOOL) parm1;

	iColumn = atoi(pcSort);
	if (iColumn == 0)
	{
		if (m_iSortingPrimary == SORTING_AZ) m_iSortingPrimary = SORTING_ZA;
		else m_iSortingPrimary = SORTING_AZ;
		SetSortIndicator(m_iColumnOrder[COLUMN_MESSAGE_NR], m_iSortingPrimary);
		m_bMessagesCorrupt = true;	// before return, needs to be updated
		if (bHtml == FALSE) return NULL;

		if (!m_bThreadBusy)
		{
			CListCtrl& listCtrl = GetListCtrl();
			CCreateHtmlWebServer createHtml;
			createHtml.Create(PosBarViewSelectMessages, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_MESSAGE_COLUMNS, NULL, PosGroupViewTasksMessages, &pcHtml);
		}

	}
	return (LRESULT) pcHtml;
}

LRESULT CRemoteCombinedMessageView::OnMessagesInvalidate(WPARAM parm1, LPARAM parm2)
{
	m_bMessagesCorrupt = true;
	return 0;
}

void CRemoteCombinedMessageView::OnComputersDetect()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SELECTED,BAR_SELECTED_COMPUTERS,0);

	m_pDoc->m_pComputerView->PostMessage(WM_COMMAND, ID_COMPUTERS_DETECT,0);
}
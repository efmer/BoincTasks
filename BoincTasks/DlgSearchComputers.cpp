// DlgSearchComputers.cpp : implementation file
//

#include "stdafx.h"
#include "Iphlpapi.h"
#include "BoincTasks.h"
#include "DlgSearchComputers.h"

#include "afxcmn.h"
#include "ListCtrlFindComputers.h"
#include "afxwin.h"

#include "DlgFont.h"

#include "RpcClient.h"
#include "Translation.h"
#include "ThreadRpcClient.h"
#include "ThreadRpcSearch.h"
#include "ThreadHostSearch.h"
#include "StringCopyNew.h"
#include "DlgLogging.h"

static int _gnSearchComputersFmt[NUM_SEARCH_COMPUTERS_COLUMNS] =
{
	LVCFMT_LEFT,	LVCFMT_LEFT,	LVCFMT_LEFT,	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT
};

// password is 0 = hidden
static int _gnSearchComputersWidth[NUM_SEARCH_COMPUTERS_COLUMNS] =
{
	200,			130,			150,				120,			80,				80,	0
};

// CDlgSearchComputers dialog

IMPLEMENT_DYNAMIC(CDlgSearchComputers, CDialog)

CDlgSearchComputers::CDlgSearchComputers(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchComputers::IDD, pParent)
{
	m_iThreadHostSearchID = 0;

	m_iThreadBusy = 0;
	m_bButtonsEnabled = true;
	m_bCancel = false;
	m_iThreadResult = 0;
	m_iProgress = 0;
	m_sPassWord = "";

	m_hMainWwd = NULL;
	m_plListComputerFind = NULL;

	for (int iCount = 0; iCount < 255+1; iCount++)	//0- 255
	{
		m_pThreadInfo[iCount] = NULL;
	}
	m_pThreadSearchTThrottleBoinc = new CRpcInfoClientSearch;

	m_bLog = theApp.m_pDlgLogging->m_bLogDebugSearchComputers;

	m_pLogging = new CLoggingMessage;

	m_pFont = new CFont;
}

CDlgSearchComputers::~CDlgSearchComputers()
{
	for (int iCount = 0; iCount < 255+1; iCount++)
	{
		if (m_pThreadInfo[iCount] != NULL)
		{
			delete m_pThreadInfo[iCount];
		}
	}
	if (m_pThreadSearchTThrottleBoinc != NULL) delete m_pThreadSearchTThrottleBoinc;
	delete m_pLogging;
	delete m_pFont;
}


BOOL CDlgSearchComputers::OnInitDialog()
{
	CString sFormat;
	char szHostName[MAX_PATH];
	DWORD ip = INADDR_NONE;

	CDialog::OnInitDialog();

	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);	// request new font.

	AddColumns();
	m_iConnectionNumber = -1;

	m_StateImageList.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
	m_listComputers.SetImageList(&m_StateImageList, LVSIL_STATE);

	m_bValidAddress = false;

	if (theApp.m_plLocalhostIp->size() > 0)
	{
		m_sLocalHostIp = theApp.m_plLocalhostIp->at(0).c_str();	// ONLY take the first 
		gethostname(szHostName, MAX_PATH);
		ip = inet_addr(m_sLocalHostIp);
		if (ip != INADDR_NONE)
		{
			m_address.S_un.S_addr = ip;

			m_addressFromCtrl.SetAddress(m_address.S_un.S_un_b.s_b1, m_address.S_un.S_un_b.s_b2, m_address.S_un.S_un_b.s_b3, 1);

			m_sLocalHostIp.Format("%d.%d.%d.%d", m_address.S_un.S_un_b.s_b1, m_address.S_un.S_un_b.s_b2, m_address.S_un.S_un_b.s_b3, m_address.S_un.S_un_b.s_b4);
			m_sLocalHostName = szHostName;

			m_editAddressTo.SetWindowText("255");
			m_bValidAddress = true;
		}

	}

	// translation
	this->SetWindowText(gszTranslation[PosDialogSearchComputerTitle]);
	m_buttonAddSelected.SetWindowText(gszTranslation[PosDialogSearchComputerButtonAdd]);
	m_buttonSearchAgain.SetWindowText(gszTranslation[PosDialogSearchComputerButtonSearchAgain]);
	m_buttonSearchRange.SetWindowText(gszTranslation[PosDialogSearchComputerButtonAddressRange]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);

	m_textAddress.SetWindowText(gszTranslation[PosDialogSearchComputerAddress]);
	m_textPassword.SetWindowText(gszTranslation[PosDialogSearchComputerPassword]);
	// translation

	m_barProgress.SetRange(0,10);
	m_barProgress.ShowWindow(SW_HIDE);
	m_editThreadLeft.ShowWindow(SW_HIDE);

	StartFullAddressScan();

	m_listComputers.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	SetTimer(UTIMER_DLG_SEARCH_COMPUTERS, 200, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSearchComputers::AddColumns()
{
	LV_COLUMN	lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iCount = 0; iCount < NUM_SEARCH_COMPUTERS_COLUMNS; iCount++)
	{
		lvc.iSubItem = iCount;
		switch (iCount)
		{
			case SEARCH_COLUMN_COMPUTER:
				lvc.pszText	= gszTranslation[PosDialogSearchComputerComputer];
			break;
			case SEARCH_COLUMN_IP:
				lvc.pszText	= gszTranslation[PosDialogSearchComputerIP];
			break;
			case SEARCH_COLUMN_MAC:
				lvc.pszText = "MAC";
			break;
			case SEARCH_COLUMN_BOINC:
				lvc.pszText = "BOINC";
			break;
			case SEARCH_COLUMN_TTHROTTLE:
				lvc.pszText = "TThrottle";
			break;
			case SEARCH_COLUMN_TOOLBOX:
				lvc.pszText = "Toolbox";
			break;
			case SEARCH_COLUMN_PASSWORD:
				lvc.pszText = "Password";
			break;
		}
		lvc.fmt = _gnSearchComputersFmt[iCount];
		lvc.cx = _gnSearchComputersWidth[iCount];
		m_listComputers.InsertColumn(iCount,&lvc);
	}
}

void CDlgSearchComputers::StartFullAddressScan()
{

}

void CDlgSearchComputers::Search()
{
	CThreadRpcClient	*pThreadClient;
	CThreadHostSearch	*pThreadHostSearch;

	m_listComputers.DeleteAllItems();

	if (m_iThreadHostSearchID != 0) return;
	if (m_pThreadSearchTThrottleBoinc->m_iThreadID != 0) return;

	pThreadHostSearch = (CThreadHostSearch *) AfxBeginThread(RUNTIME_CLASS(CThreadHostSearch));
	m_iThreadHostSearchID =  pThreadHostSearch->m_nThreadID;

	pThreadClient = (CThreadRpcClient *) AfxBeginThread(RUNTIME_CLASS(CThreadRpcClient));
	m_pThreadSearchTThrottleBoinc->m_iThreadID = pThreadClient->m_nThreadID; 
	m_pThreadSearchTThrottleBoinc->m_bAutoExit = false;
	m_pThreadSearchTThrottleBoinc->m_iTimeOut = 5;

//	::PostThreadMessage(m_iThreadHostSearchID, UWM_MSG_THREAD_HOSTSEARCH,(WPARAM) this->m_hWnd, (LPARAM) &m_searchInfo);

	m_iSearchState = STATE_SEARCH_FINDNAME;

	m_listComputers.InsertItem(0,"");
	m_listComputers.SetItemText(0,0,m_sLocalHostName);
	m_listComputers.SetItemText(0,SEARCH_COLUMN_IP,LOCALHOST_NAME);
	m_listComputers.SetItemState(0,INDEX_GRAYED,LVIS_STATEIMAGEMASK);

//	CStringCopyNew(&m_pThreadSearchTThrottleBoinc->m_pcIp, LOCALHOST_NAME, true);
	m_pThreadSearchTThrottleBoinc->m_sIp = LOCALHOST_NAME;
	m_pThreadSearchTThrottleBoinc->m_iPort = 31416;
	m_pThreadSearchTThrottleBoinc->m_iTimeOut = 5;
//	m_pThreadSearchTThrottleBoinc->AddPassWord(m_sPassWord.GetBuffer()); m_sPassWord.ReleaseBuffer();
	m_pThreadSearchTThrottleBoinc->m_sPassWord = m_sPassWord;
	::PostThreadMessage(m_pThreadSearchTThrottleBoinc->m_iThreadID, UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT,(WPARAM) this->m_hWnd, (LPARAM) m_pThreadSearchTThrottleBoinc);
}

void CDlgSearchComputers::CheckIllegal()
{
	CString sName;
	int		iNr;//, iPos;
	bool	bUncheck;

	iNr = m_listComputers.GetItemCount();

	for (int iCount = 0; iCount < iNr; iCount++)
	{
		bUncheck = true;

		sName = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_BOINC);
		if (sName.GetLength() > 0)	bUncheck = false;	// something BOINC is always ok
		sName = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_TTHROTTLE);
		if (sName.GetLength() > 0)	bUncheck = false;	// something TThrottle is always ok
		sName = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_IP);
		if (sName.GetLength() ==0) bUncheck = true;		// an empty IP Adress is always invalid

		if (m_listComputers.GetItemState(iCount,LVIS_STATEIMAGEMASK) == INDEX_CHECKED)
		{
			if (bUncheck)	m_listComputers.SetItemState(iCount,INDEX_GRAYED,LVIS_STATEIMAGEMASK);
		}
		if (m_listComputers.GetItemState(iCount,LVIS_STATEIMAGEMASK) == INDEX_GRAYED)
		{
			if (!bUncheck)	m_listComputers.SetItemState(iCount,INDEX_UNCHECKED,LVIS_STATEIMAGEMASK);
		}
	}
}

void CDlgSearchComputers::DisableButtons()
{
	m_buttonAddSelected.EnableWindow(FALSE);
	m_buttonSearchAgain.EnableWindow(FALSE);

	m_addressFromCtrl.EnableWindow(FALSE);
	m_editAddressTo.EnableWindow(FALSE);

	m_bButtonsEnabled = false;
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogSearchComputerButtonStop]);
}

void CDlgSearchComputers::EnableButtons()
{
	m_buttonAddSelected.EnableWindow(TRUE);
	m_buttonSearchAgain.EnableWindow(TRUE);
	m_buttonSearchRange.EnableWindow(TRUE);

	m_addressFromCtrl.EnableWindow(TRUE);
	m_editAddressTo.EnableWindow(TRUE);

	m_buttonCancel.EnableWindow(TRUE);

	m_bButtonsEnabled = true;
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
}

CString CDlgSearchComputers::ConnectionStatus(int iConnectionStatus)
{
	CString sStatus;

	switch (iConnectionStatus)
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
		case RPC_STATUS_CONNECTED:
			return gszTranslation[PosWindowStatusMessageCombinedConnected];
		break;

		default:
			return  gszTranslation[PosWindowStatusMessageCombinedNotConnected];
	}
//		case RPC_STATUS_ERROR_LOST_CONNECTION:
//			return gszTranslation[PosWindowStatusMessageCombinedLostConnection];
//		break;

}

void CDlgSearchComputers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIND_COMPUTERS, m_listComputers);
	DDX_Control(pDX, IDC_IPADDRESS_FROM, m_addressFromCtrl);
	DDX_Control(pDX, IDC_EDIT_ADDRESS_TO, m_editAddressTo);
	DDX_Control(pDX, IDOK, m_buttonAddSelected);
	DDX_Control(pDX, IDSEARCH_AGAIN, m_buttonSearchAgain);
	DDX_Control(pDX, IDSEARCH_ADDRESS, m_buttonSearchRange);
	DDX_Control(pDX, IDC_TEXT_SEARCH_ADDRESS, m_textAddress);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_PROGRESS_SEARCH_COMPUTER, m_barProgress);
	DDX_Control(pDX, IDC_TEXT_SEARCH_PASSWORD, m_textPassword);
	DDX_Control(pDX, IDC_EDIT_SEARCH_COMPUTER_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_THREAD_LEFT_COUNTER, m_editThreadLeft);
}


BEGIN_MESSAGE_MAP(CDlgSearchComputers, CDialog)
	ON_WM_SYSCOMMAND()	
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDSEARCH_AGAIN, &CDlgSearchComputers::OnBnClickedSearchagain)
	ON_BN_CLICKED(IDOK, &CDlgSearchComputers::OnBnClickedOk)
	ON_BN_CLICKED(IDSEARCH_ADDRESS, &CDlgSearchComputers::OnBnClickedAddress)

	ON_MESSAGE(UWM_MSG_THREAD_QUIT_READY, OnClientReady)
	ON_MESSAGE(UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_READY, OnCheckTThrottleBoincReady)

	ON_MESSAGE(UWM_MSG_THREAD_HOSTSEARCH_READY, OnHostSearchReady)
	ON_MESSAGE(UWM_MSG_THREAD_HOSTSEARCH_QUIT, OnHostSearchQuit)

	ON_BN_CLICKED(IDCANCEL, &CDlgSearchComputers::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_SEARCH_COMPUTER_PASSWORD, &CDlgSearchComputers::OnEnChangeEditSearchComputerPassword)

	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnChangedFont)
	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnChangedFont)

	ON_MESSAGE(UWM_DLG_SEARCH_COMPUTER_START,OnSearchComputerStart)

	ON_COMMAND(ID_FIND_COMPUTERS_COPYIP,OnClipBoardIp)
	ON_COMMAND(ID_FIND_COMPUTERS_COPYMAC,OnClipBoardMac)
	ON_COMMAND(ID_FIND_COMPUTERS_COPYALLTOCLIPBOARD, OnClipBoardAll)
END_MESSAGE_MAP()


// CDlgSearchComputers message handlers

void CDlgSearchComputers::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		// close window = cancel
		OnBnClickedCancel();
		return;
	}
	CDialog::OnSysCommand(nID, lParam);
}

void CDlgSearchComputers::OnTimer(UINT_PTR nIDEvent)
{
//	struct in_addr *ptr;	// To retrieve the IP Address
//	struct hostent *host;
	CString sIp;

//	bool	bBoinc, bTThrottle;
//	int		iNr;
	CString sNameId, sNameIp,sConnected, sFormat;

	if (m_iThreadBusy <= 0)
	{
		if (m_iSearchState == STATE_SEARCH_ADDRESS_RANGE)
		{
			EnableButtons();
			m_barProgress.ShowWindow(SW_HIDE);
			m_editThreadLeft.ShowWindow(SW_HIDE);
			m_iSearchState = STATE_SEARCH_STOPPED;
		}
	}

	if (m_iSearchState == STATE_SEARCH_ADDRESS_RANGE)
	{
		m_barProgress.SetPos(m_iProgress);
		sFormat.Format("%d",m_iThreadBusy);
		m_editThreadLeft.SetWindowText(sFormat);
		if (m_iProgress++ > 10) m_iProgress = 0;
	}

	if (m_iSearchState == STATE_SEARCH_FINDNAME)
	{
		m_barProgress.SetPos(m_iProgress);
		if (m_iProgress++ > 10) m_iProgress = 0;
	}

	bool bCancel = false;

	if (m_bCancel)
	{
		if (m_iThreadBusy <= 0)
		{
			if (m_iThreadHostSearchID == 0)
			{
				if (m_pThreadSearchTThrottleBoinc->m_iThreadID == 0)
				{
					bCancel = true;
				}
			}
		}
	
		if (bCancel)
		{
			this->ShowWindow(SW_HIDE);
			m_bCancel = false;
		}
		else
		{
			m_barProgress.SetPos(m_iProgress);
			if (m_iProgress++ > 10) m_iProgress = 0;
		}
	}

	if (m_iSearchState == STATE_SEARCH_STOPPED)
	{
		CheckIllegal();
	}

}

LRESULT CDlgSearchComputers::OnCheckTThrottleBoincReady(WPARAM parm1, LPARAM parm2)
{
	CRpcInfoClientSearch	*pInfo;
	int						iResult, iResultTemp, iNr, iItemFound;
	bool					bAlready = false;
	CString					sNameIp, sHostName, sStatus, sFormat;
	std::string				sIp;

	IPAddr					DestIp = 0;
	ULONG					MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG					PhysAddrLen = 6;  /* default to length of six bytes */
	DWORD					dwRetVal;
	BYTE					*bytePhysAddr;
	CString					sMac;

	pInfo = (CRpcInfoClientSearch *) parm1;
	sIp = pInfo->m_sIp;
	iResult = (int) parm2;

	if (pInfo->m_sFoundHostName.length() > 0)
	{
		sHostName = pInfo->m_sFoundHostName.c_str();
	}
	else
	{
		sHostName = pInfo->m_sHostID.c_str();
	}

	if (m_iSearchState == STATE_SEARCH_FINDNAME)
	{
		if (iResult != 0)
		{
			iNr = m_listComputers.GetItemCount();	
			for (int iCount = 0; iCount < iNr; iCount++)
			{
				sNameIp = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_IP);
				if (sNameIp == sIp.c_str())
				{
					iResultTemp = iResult & BIT_TRUE_BOINC;
					if (iResultTemp)
					{
						sStatus = gszTranslation[PosDialogSearchComputerYes];
						sStatus+= " ," +  ConnectionStatus(pInfo->m_iConnectionStatus);;
						m_listComputers.SetItemText(iCount,SEARCH_COLUMN_BOINC,sStatus);
					}
					else m_listComputers.SetItemText(0,SEARCH_COLUMN_BOINC,"");
					iResultTemp = iResult & BIT_TRUE_TTHROTTLE;
					if (iResultTemp) 	m_listComputers.SetItemText(iCount,SEARCH_COLUMN_TTHROTTLE,gszTranslation[PosDialogSearchComputerYes]);
					else m_listComputers.SetItemText(0,SEARCH_COLUMN_TTHROTTLE,"");
				}
			}
		}

		if (m_bLog)
		{
			char *pcBufferFrom = new char [64];
			char *pcBufferLog = new char [128];
			_snprintf_s(pcBufferLog,127,_TRUNCATE,"Host: %s" ,sHostName);
			strcpy_s(pcBufferFrom, 63, "STATE_SEARCH_FINDNAME");
			m_pLogging->m_pFrom = pcBufferFrom;
			m_pLogging->m_pText = pcBufferLog;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLogging, LOGGING_DEBUG);
			delete pcBufferLog;
			delete pcBufferFrom;
		}
		::PostThreadMessage(m_iThreadHostSearchID, UWM_MSG_THREAD_HOSTSEARCH,(WPARAM) this->m_hWnd, (LPARAM) &m_searchInfo);	// get next
		return 0;
	}

	if (m_bLog)
	{
		char *pcBufferFrom = new char [64];
		char *pcBufferLog = new char [128];
		_snprintf_s(pcBufferLog,127,_TRUNCATE,"Host: %s, IP: %s, Port: %d, iResult: %d, Threads Left: %d" ,sHostName, pInfo->m_sIp.c_str(), pInfo->m_iPort, iResult, m_iThreadBusy-1);
		strcpy_s(pcBufferFrom, 63, "STATE_SEARCH_ADDRESS_RANGE");
		m_pLogging->m_pFrom = pcBufferFrom;
		m_pLogging->m_pText = pcBufferLog;
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) m_pLogging, LOGGING_DEBUG);
		delete pcBufferLog;
		delete pcBufferFrom;
	}

	m_iThreadBusy--;

	iItemFound = 0;

	if (sHostName.GetLength() == 0)
	{
		sHostName = sIp.c_str();
	}

	if (iResult != 0)
	{
		DestIp = inet_addr(pInfo->m_sIp.c_str());
		memset(&MacAddr, 0xff, sizeof (MacAddr));
		//printf("Sending ARP request for IP address: %s\n", DestIpString);

		dwRetVal = SendARP(DestIp, INADDR_ANY, &MacAddr, &PhysAddrLen);
		if (dwRetVal == NO_ERROR)
		{
			bytePhysAddr = (BYTE *) & MacAddr;
			if (PhysAddrLen)
			{
				for (int i = 0; i < (int) PhysAddrLen; i++)
				{
					if (i == (PhysAddrLen - 1)) sFormat.Format("%.2X", (int) bytePhysAddr[i]);
					else sFormat.Format("%.2X-", (int) bytePhysAddr[i]);
					sMac += sFormat;
				}
			}
		}

		iNr = m_listComputers.GetItemCount();

		for (int iCount = 0; iCount < iNr; iCount++)
		{
			sNameIp = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_COMPUTER);
			if (sNameIp == sHostName)
			{
				bAlready = true;
				iItemFound = iCount;
			}
			sNameIp = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_IP);
			if (sNameIp == sIp.c_str())
			{
				bAlready = true;
				iItemFound = iCount;
			}


		}
		if (!bAlready)
		{
			m_listComputers.InsertItem(SEARCH_COLUMN_COMPUTER,"");
		}
		m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_COMPUTER,sHostName);
		m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_IP,sIp.c_str());
		m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_MAC,sMac);
		iResultTemp = iResult & BIT_TRUE_BOINC;
		if (iResultTemp)
		{
			sStatus = gszTranslation[PosDialogSearchComputerYes];
			sStatus+= " ," +  ConnectionStatus(pInfo->m_iConnectionStatus);;
			m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_BOINC,sStatus);
		}
		else m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_BOINC,"");
		iResultTemp = iResult & BIT_TRUE_TTHROTTLE;
		if (iResultTemp) 
		{
			m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_TTHROTTLE,gszTranslation[PosDialogSearchComputerYes]);
		}
		else
		{
			m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_TTHROTTLE,"");
		}
		iResultTemp = iResult & BIT_TRUE_TOOLBOX;
		if (iResultTemp) 	m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_TOOLBOX,gszTranslation[PosDialogSearchComputerYes]);
		else m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_TOOLBOX,"");

		if (pInfo->m_sPassWord.length() > 0)
		{
			m_listComputers.SetItemText(iItemFound,SEARCH_COLUMN_PASSWORD,pInfo->m_sPassWord.c_str());
		}


		m_listComputers.SetItemState(iItemFound,INDEX_GRAYED,LVIS_STATEIMAGEMASK);
	}

	return 0;
}

LRESULT CDlgSearchComputers::OnClientReady(WPARAM parm1, LPARAM parm2)
{
	m_pThreadSearchTThrottleBoinc->m_iThreadID = 0;


	return 0;
}

LRESULT CDlgSearchComputers::OnHostSearchReady(WPARAM parm1, LPARAM parm2)
{
	CHostSearchInfo	 *pInfo;
	CString sHostNameFull, sHostName, sIp;
//	int	iNr;

	pInfo = (CHostSearchInfo*) parm1;
	sHostNameFull = pInfo->m_pcHostNameFull;
	sHostName = pInfo->m_pcHostName;
	sIp = pInfo->m_pcHostName;

	m_listComputers.InsertItem(SEARCH_COLUMN_COMPUTER,"");
	m_listComputers.SetItemText(0,SEARCH_COLUMN_COMPUTER,sHostNameFull);
	m_listComputers.SetItemText(0,SEARCH_COLUMN_IP,sHostName);
	m_listComputers.SetItemState(0,INDEX_GRAYED,LVIS_STATEIMAGEMASK);

	// now check if Boinc or TThrottle are running

	if (sIp != "")
	{
//		CStringCopyNew(&m_pThreadSearchTThrottleBoinc->m_pcIp, sIp.GetBuffer()); sIp.ReleaseBuffer();
		m_pThreadSearchTThrottleBoinc->m_sIp = sIp;
		m_pThreadSearchTThrottleBoinc->m_iPort = 31416;
		m_pThreadSearchTThrottleBoinc->m_iTimeOut = 5;
//		m_pThreadSearchTThrottleBoinc->AddPassWord(m_sPassWord.GetBuffer()); m_sPassWord.ReleaseBuffer();
		m_pThreadSearchTThrottleBoinc->m_sPassWord = m_sPassWord;
		::PostThreadMessage(m_pThreadSearchTThrottleBoinc->m_iThreadID, UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT,(WPARAM) this->m_hWnd, (LPARAM) m_pThreadSearchTThrottleBoinc);
	}
	else
	{
		// don't need this empty one.
		::PostThreadMessage(m_iThreadHostSearchID, UWM_MSG_THREAD_HOSTSEARCH,(WPARAM) this->m_hWnd, (LPARAM) &m_searchInfo);	// get next
	}
	return 0;
}

LRESULT CDlgSearchComputers::OnHostSearchQuit(WPARAM parm1, LPARAM parm2)
{
	EnableButtons();
	m_barProgress.ShowWindow(SW_HIDE);
	m_editThreadLeft.ShowWindow(SW_HIDE);
	m_iSearchState = STATE_SEARCH_STOPPED;

	m_iThreadHostSearchID = 0;

	::PostThreadMessage(m_pThreadSearchTThrottleBoinc->m_iThreadID, UWM_MSG_THREAD_QUIT,(WPARAM) this->m_hWnd, 0);
	
	return 0;
}



void CDlgSearchComputers::OnBnClickedSearchagain()
{
	DisableButtons();

	m_iProgress = 0;
	m_barProgress.ShowWindow(SW_SHOW);
	m_editThreadLeft.ShowWindow(SW_HIDE);

	m_buttonSearchRange.EnableWindow(FALSE);
	m_iSearchState = STATE_SEARCH_FINDNAME;
	m_listComputers.DeleteAllItems();

	Search();
}

void CDlgSearchComputers::OnBnClickedOk()
{
	CString sTxt;
	int	iItems, iCount;
	CListComputerFindItem	*pListComputerFindItem;


	if (m_plListComputerFind == NULL) return;

	CheckIllegal();

// move selected computers to string array

	m_iComputersSelected = 0;
	iItems = m_listComputers.GetItemCount();
	for (iCount = 0; iCount < iItems; iCount++)
	{
		if (m_listComputers.GetItemState(iCount,LVIS_STATEIMAGEMASK) == INDEX_CHECKED)
		{
			pListComputerFindItem = new CListComputerFindItem;
			pListComputerFindItem->m_sIp = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_IP);
			if (pListComputerFindItem->m_sIp == "127.0.0.1")
			{
					pListComputerFindItem->m_sIp = LOCALHOST_NAME;
			}

			pListComputerFindItem->m_sPassword = m_sPassWord;	// global password, may be overwritten by the password from the Toolbox

			if (pListComputerFindItem->m_sIp.Find(LOCALHOST_NAME) < 0)		// only add on non localhost computers
			{
				sTxt = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_MAC);
				if (sTxt.GetLength() > 2)
				{
					pListComputerFindItem->m_sMAC = sTxt;
				}
				sTxt =  m_listComputers.GetItemText(iCount,SEARCH_COLUMN_PASSWORD);
				if (sTxt.GetLength() > 0)
				{
					pListComputerFindItem->m_sPassword = sTxt;
				}
			}
			pListComputerFindItem->m_sName = m_listComputers.GetItemText(iCount,SEARCH_COLUMN_COMPUTER);
			m_plListComputerFind->push_back(pListComputerFindItem);
			if (m_iComputersSelected >= MAX_SEARCH_COMPUTERS) break;
			m_iComputersSelected++;
		}
	}

	::PostMessage(m_hMainWwd, UWM_DLG_SEARCH_COMPUTER_READY,0,0);

	this->ShowWindow(SW_HIDE);
//	OnOK();
}

void CDlgSearchComputers::OnBnClickedAddress()
{
	CThreadRpcClient		*pThreadClient;

	CString sIp, sFormat;

	m_iThreadBusy = 0;

	m_editAddressTo.GetWindowText(sFormat);
	int iAddress = atoi(sFormat);
	if (iAddress > 255 || iAddress < 1)
	{
		iAddress = 255;
		sFormat.Format("%d", iAddress);
		m_editAddressTo.SetWindowText(sFormat);
	}

	m_addressFromCtrl.GetAddress(m_addressFrom.S_un.S_un_b.s_b1, m_addressFrom.S_un.S_un_b.s_b2, m_addressFrom.S_un.S_un_b.s_b3, m_addressFrom.S_un.S_un_b.s_b4);
	
	for (int iCount = m_addressFrom.S_un.S_un_b.s_b4; iCount < iAddress; iCount++)
	{
		int a = m_addressFrom.S_un.S_un_b.s_b1;  // 211
		int b = m_addressFrom.S_un.S_un_b.s_b2;  // 40
		int c = m_addressFrom.S_un.S_un_b.s_b3;  // 35
		int d = m_addressFrom.S_un.S_un_b.s_b4;  // 76
		sIp.Format("%d.%d.%d.%d",a,b,c,d);
		m_addressFrom.S_un.S_un_b.s_b4++;

		if (sIp ==  m_sLocalHostIp)
		{
			sIp = LOCALHOST_NAME;
		}

		pThreadClient = (CThreadRpcClient *) AfxBeginThread(RUNTIME_CLASS(CThreadRpcClient));
		if (m_pThreadInfo[iCount] != NULL)
		{
			delete m_pThreadInfo[iCount];
		}
		m_pThreadInfo[iCount] = new CRpcInfoClientSearch;

		m_pThreadInfo[iCount]->m_iThreadID =  pThreadClient->m_nThreadID;
//		CStringCopyNew(&m_pThreadInfo[iCount]->m_pcIp, sIp.GetBuffer()); sIp.ReleaseBuffer();
		m_pThreadInfo[iCount]->m_sIp = sIp;
		m_pThreadInfo[iCount]->m_iPort = 31416;
		m_pThreadInfo[iCount]->m_iTimeOut = 5;
		m_pThreadInfo[iCount]->m_bAutoExit = true;
//		m_pThreadInfo[iCount]->AddPassWord(m_sPassWord.GetBuffer()); m_sPassWord.ReleaseBuffer();
		m_pThreadInfo[iCount]->m_sPassWord = m_sPassWord;
		::PostThreadMessage(m_pThreadInfo[iCount]->m_iThreadID, UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT,(WPARAM) this->m_hWnd, (LPARAM) m_pThreadInfo[iCount]);
		m_iThreadBusy++;
	}
	
	DisableButtons();
	m_buttonSearchRange.EnableWindow(FALSE);
	m_buttonCancel.EnableWindow(FALSE);

	m_iProgress = 0;
	m_barProgress.ShowWindow(SW_SHOW);
	m_editThreadLeft.ShowWindow(SW_SHOW);

	m_iSearchState = STATE_SEARCH_ADDRESS_RANGE;
}

void CDlgSearchComputers::OnBnClickedCancel()
{
	if (m_iSearchState == STATE_SEARCH_ADDRESS_RANGE)
	{
		return;
	}

	if (m_iSearchState == STATE_SEARCH_FINDNAME)
	{
		if (m_iThreadHostSearchID != 0)
		{
			::PostThreadMessage(m_iThreadHostSearchID, UWM_MSG_THREAD_QUIT,(WPARAM) this->m_hWnd,0);
		}
		EnableButtons();
		m_barProgress.ShowWindow(SW_HIDE);
		m_editThreadLeft.ShowWindow(SW_HIDE);
		m_iSearchState = STATE_SEARCH_STOPPED;
		return;
	}

	m_buttonCancel.EnableWindow(FALSE);

	m_bCancel = true;
	DisableButtons();
	m_bButtonsEnabled = true;					// don't enable again

	m_barProgress.ShowWindow(SW_SHOW);
	m_editThreadLeft.ShowWindow(SW_SHOW);
	m_iProgress= 0;

	m_buttonSearchRange.EnableWindow(FALSE);
	m_iSearchState = STATE_SEARCH_STOPPED;		// force idle

	if (m_iThreadHostSearchID != 0)
	{
		::PostThreadMessage(m_iThreadHostSearchID, UWM_MSG_THREAD_QUIT,(WPARAM) this->m_hWnd,0);
	}
	if (m_pThreadSearchTThrottleBoinc->m_iThreadID != 0)
	{
		::PostThreadMessage(m_pThreadSearchTThrottleBoinc->m_iThreadID, UWM_MSG_THREAD_QUIT,(WPARAM) this->m_hWnd,0);
	}
}


void CDlgSearchComputers::OnEnChangeEditSearchComputerPassword()
{
	m_editPassword.GetWindowText(m_sPassWord);
}

LRESULT CDlgSearchComputers::OnChangedFont(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLp;
	pLp = (LOGFONT *) wParam;

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLp);

	m_listComputers.SetFont(m_pFont,TRUE);
	m_listComputers.Invalidate(TRUE);

	return 0;
}

LRESULT CDlgSearchComputers::OnSearchComputerStart(WPARAM wParam,LPARAM lParam)
{
	m_hMainWwd = (HWND) wParam;
	m_plListComputerFind = (std::deque<CListComputerFindItem*> *) lParam;
	m_bCancel = false;
	m_iProgress = 0;
	EnableButtons();
	return 0;
}

void CDlgSearchComputers::OnContextMenu(CWnd* pWnd, CPoint point) 
{ 
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
	contextMenu.LoadMenu(IDR_MENU_COMPUTER); 
 
	//we have to translate the mouse coordinates first:
	//these are relative to the window (or view) but the context menu
	//needs coordinates that are relative to the whole screen
	ClientToScreen(&point);
 
	//tracker now points to IDR_CONTEXT's first submenu 
	//GetSubMenu(0) -&gt; returns the first submenu,
	// GetSubMenu(1)-&gt; second one etc..
	tracker = contextMenu.GetSubMenu(0); 

//Translate
	tracker->ModifyMenu(ID_FIND_COMPUTERS_COPYIP,MF_STRING,ID_FIND_COMPUTERS_COPYIP,gszTranslation[PosDialogSearchComputerCopyIp]);
	tracker->ModifyMenu(ID_FIND_COMPUTERS_COPYMAC,MF_STRING,ID_FIND_COMPUTERS_COPYMAC,gszTranslation[PosDialogSearchComputerCopyMAC]);
	tracker->ModifyMenu(ID_FIND_COMPUTERS_COPYALLTOCLIPBOARD,MF_STRING,ID_FIND_COMPUTERS_COPYALLTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopyAll]);
//Translate

	//show the context menu
	tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y ,this);//; AfxGetMainWnd()); 

	//has always to be called (function of base class)
//   CListViewEx::OnRButtonDown(nFlags, point); 
}

void CDlgSearchComputers::OnClipBoardIp()
{
	ClipBoard(1);;
}
void CDlgSearchComputers::OnClipBoardMac()
{
	ClipBoard(2);
}

void CDlgSearchComputers::OnClipBoardAll()
{
	ClipBoard(101);
}

void CDlgSearchComputers::ClipBoard(int iColumn)
{
    HGLOBAL		hglbCopy; 
    LPTSTR		lptstrCopy; 
	CString		sTxt,sClipBoard;
	int			iItems;
//	bool		bSelected;

	if ( !OpenClipboard() ) return;
	if( !EmptyClipboard() ) return;
	
	iItems = m_listComputers.GetItemCount();

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (iColumn > 100)
		{
			sTxt = m_listComputers.GetItemText(iCount,0);
			sClipBoard += sTxt;
			sClipBoard += ",";
			sTxt = m_listComputers.GetItemText(iCount,1);
			sClipBoard += sTxt;
			sClipBoard += ",";
			sTxt = m_listComputers.GetItemText(iCount,2);
			sClipBoard += sTxt;
			sClipBoard += "\r\n";
		}
		else
		{
			if (m_listComputers.GetItemState(iCount,LVIS_SELECTED))
			{
				sTxt = m_listComputers.GetItemText(iCount,iColumn);
				sClipBoard += sTxt;
			}
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
 	if (!SetClipboardData(CF_TEXT, hglbCopy)) GlobalFree(hglbCopy);; 

	CloseClipboard();
}
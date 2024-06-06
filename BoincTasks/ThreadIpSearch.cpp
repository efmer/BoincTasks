// ThreadConnect.cpp : implementation file
//

#include "stdafx.h"
#include "Iphlpapi.h"
#include "BoincTasks.h"
#include "ThreadRpc.h"
#include "ThreadRpcClient.h"
#include "ThreadRpcClientSearch.h"
#include "ThreadIpSearch.h"
#include "DlgLogging.h"


// CThreadIpSearch

IMPLEMENT_DYNCREATE(CThreadIpSearch, CWinThread)

CThreadIpSearch::CThreadIpSearch()
{
	for (int iCount = 0; iCount < 255+1; iCount++)	//0- 255
	{
		m_pThreadInfo[iCount] = NULL;
	}

	m_iThreadBusy = 0;
	m_bStartSearchThreadsRequest = false;
	m_bExit = false;
	m_tNextSearchThreads = 0; 
	m_iNextSearchThreadsInterval = 15;
	m_iTimeOut = 5;
	localhostIpSequence = 0;

	m_iLoggingCount = 0;
	m_pWndLogging = theApp.m_pDlgLogging;
	m_pcBufferLog = new char[4096];
	m_pcBufferLogFrom = new char[64];
	m_pLogging = new CLoggingMessage;
}

CThreadIpSearch::~CThreadIpSearch()
{
	for (int iCount = 0; iCount < 255+1; iCount++)
	{
		if (m_pThreadInfo[iCount] != NULL)
		{
			delete m_pThreadInfo[iCount];
		}
	}
	delete m_pLogging;
	delete m_pcBufferLog;
	delete m_pcBufferLogFrom;
}

BOOL CThreadIpSearch::InitInstance()
{
	HWND	hWnd;
	int		iThread;
	int		iMsg;
	time_t	tSystemTime;

	iThread = 0;
	hWnd = NULL;

	while(!m_bExit)
	{
		MSG msg;
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			iMsg = msg.message;
			if (iMsg > 0)
			{
				if (iMsg == UWM_MSG_THREAD_START_IP_SCAN)
				{
					m_plLocalhostIp = (std::deque<std::string> *) msg.wParam;
					m_iTimeOut = 3;	// 1 not, 2 works, 3 safe (w8)
					m_bStartSearchThreadsRequest = true;	
				}
	
				if (iMsg == UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_READY)
				{
					CRpcInfoClientSearch	*pInfo;
					int						iFound;

					pInfo = (CRpcInfoClientSearch *) msg.wParam;
					iFound = (int) msg.lParam;
					if (iFound)
					{
						m_lListIpScan.push_back(pInfo->m_sIp);
					}
					m_iThreadBusy--;
					if (m_iThreadBusy == 0)
					{
						m_lListIp.clear();
						std::string sIP = "";

						for (int iPos = 0; iPos < (int)m_lListIpScan.size(); iPos++)
						{
							m_lListIp.push_back(m_lListIpScan.at(iPos));
							if (iPos != 0) sIP += " - ";
							sIP += m_lListIpScan.at(iPos).c_str();
						}
						if (m_iLoggingCount < 4)
						{
							_snprintf_s(m_pcBufferLog, 4090, _TRUNCATE, "Localhost IP: %s, IP found: %s", m_plLocalhostIp->at(localhostIpSequence).c_str(), sIP.c_str());
							Logging("Search IP range:", m_pcBufferLog, LOGGING_DEBUG);
							m_iLoggingCount++;
						}

						localhostIpSequence++;

						SearchThreadsDelete();
					}
				}

				if (iMsg == UWM_MSG_THREAD_START_IP_LIST)
				{
					int	iThreadOrWnd;
					std::deque<std::string> *plListIp;

					iThreadOrWnd = (int) msg.lParam;
					if (iThreadOrWnd ==1)
					{
						iThread = (int) msg.wParam;
						hWnd = NULL;
					}
					else
					{
						hWnd = (HWND) msg.wParam;
						iThread = 0;
					}

					m_bStartSearchThreadsRequest = true;

					if (m_lListIp.size() == 0)
					{
						if (iThread > 0)	::PostThreadMessage(iThread, UWM_MSG_THREAD_START_IP_LIST_READY, 0, NULL);
						else				::PostMessage(hWnd, UWM_MSG_THREAD_START_IP_LIST_READY, 0, NULL);
					}
					else
					{
						plListIp = new (std::deque<std::string>);
						for (int iPos = 0; iPos < (int) m_lListIp.size(); iPos++)
						{
							plListIp->push_back(m_lListIp.at(iPos));
						}
						if (iThread > 0)	::PostThreadMessage(iThread, UWM_MSG_THREAD_START_IP_LIST_READY, 0, (LPARAM) plListIp);
						else				::PostMessage(hWnd, UWM_MSG_THREAD_START_IP_LIST_READY, 0, (LPARAM) plListIp);
					}
				}

				if (iMsg == UWM_MSG_THREAD_START_IP_LIST_READY)
				{
					std::deque<std::string> *plListIp;

					plListIp = (std::deque<std::string>*) msg.lParam;

					if (plListIp != NULL)
					{
						m_lListIp.clear();
						for (int iPos = 0; iPos < (int) plListIp->size(); iPos++)
						{
							m_lListIp.push_back(plListIp->at(iPos));
						}
						delete plListIp;
					}
				}


				if (iMsg == UWM_MSG_THREAD_QUIT)
				{
					int iThreadId;
						
					iThreadId = (int) msg.wParam;;
					::PostThreadMessage(iThreadId, UWM_MSG_THREAD_QUIT_READY , 0, 0);
					m_bExit = true;
				}
			}
		}

		if (m_bStartSearchThreadsRequest)
		{
			if (m_iThreadBusy == 0)
			{
				_time64(&tSystemTime);
				if (tSystemTime > m_tNextSearchThreads)
				{
					m_bStartSearchThreadsRequest = false;
					m_lListIpScan.clear();
					SearchThreadsFire();
					m_tNextSearchThreads = tSystemTime + m_iNextSearchThreadsInterval;		
					if (m_iNextSearchThreadsInterval < 120)										// every 2 minutes
					{
						m_iNextSearchThreadsInterval += 15;										// add 15 seconds / time
					}
				}
			}
		}

		Sleep(10);
	}

	return FALSE;
}

int CThreadIpSearch::SocketReady(int socket)
{
	fd_set fds;
	struct timeval timeout;
	int rc, result;

	/* Set time limit. */
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	/* Create a descriptor set containing our socket.  */
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	rc = select(sizeof(fds)*8, &fds, &fds, &fds, &timeout);	// 1 = ready, 0 = timeout
	if (rc==-1)
	{
		return -1;
	}

	result = 0;
//	Sleep(3000);
	return result;
}

int CThreadIpSearch::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

/*
std::string CThreadIpSearch::FindIp(char *pcMAC)
{
	std::string sMAC;

	for(int iPos = 0; iPos < m_lListIp.size(); iPos++)
	{
		if (CheckValidMac((char *) m_lListIp.at(iPos).c_str(), pcMAC))
		{
			return m_lListIp.at(iPos);
		}	
	}
	return "";
}
*/

/*
std::string CThreadIpSearch::IpToMAC(char *pcIP)
{
	IPAddr					DestIp = 0;
	ULONG					MacAddr[2];       // for 6-byte hardware addresses
	ULONG					PhysAddrLen = 6;  // default to length of six bytes
	DWORD					dwRetVal;
	BYTE					*bytePhysAddr;
	std::string				sMAC;
	char					cBuffer[32];

	DestIp = inet_addr(pcIP);
	memset(&MacAddr, 0xff, sizeof (MacAddr));

	dwRetVal = SendARP(DestIp, INADDR_ANY, &MacAddr, &PhysAddrLen);
	if (dwRetVal == NO_ERROR)
	{
		bytePhysAddr = (BYTE *) & MacAddr;
		if (PhysAddrLen)
		{
			for (int i = 0; i < (int) PhysAddrLen; i++)
			{
				if (i == (PhysAddrLen - 1))		_snprintf_s(cBuffer,32,_TRUNCATE,"%.2X\n", (int) bytePhysAddr[i]);	// sFormat.Format("%.2X\n", (int) bytePhysAddr[i]);
				else _snprintf_s(cBuffer,32,_TRUNCATE, "%.2X-", (int) bytePhysAddr[i]);
				sMAC += cBuffer;
			}
		}
	}
	return sMAC;
}
*/

/*
bool CThreadIpSearch::CheckValidMac(char *pcIP, char *pcMAC)
{
	std::string sMAC;

	if (strlen(pcMAC) < 2)
	{
		return false;
	}

	sMAC = IpToMAC(pcIP);
	if (sMAC.find(pcMAC) != std::string::npos)
	{
		return true;
	}

	return false;
}
*/

void CThreadIpSearch::SearchThreadsFire()
{
	CThreadRpcClientSearch	*pThreadClient;
	std::string			sAddress;
//	CString				sIp, sFormat;
//	char				szHostName[MAX_PATH];
	char				cBuffer[32];
	struct	in_addr		address;
	int					iLocalhost;
	unsigned long		ip = INADDR_NONE;

	m_iThreadBusy = 0;
	
	if (localhostIpSequence >= m_plLocalhostIp->size())
	{
		localhostIpSequence = 0;
	}

	if (strcmp(m_plLocalhostIp->at(localhostIpSequence).c_str(), "192.168.56.1") == 0)
	{
		if (m_iLoggingCount < 4)
		{
			_snprintf_s(m_pcBufferLog, 127, _TRUNCATE, "Localhost IP: %s", m_plLocalhostIp->at(localhostIpSequence).c_str());
			Logging("Search IP range, skipped known VBox:", m_pcBufferLog, LOGGING_DEBUG);
		}
		localhostIpSequence++;	// skip known VBox
	}

	if (localhostIpSequence >= m_plLocalhostIp->size())	// needs to be here...
	{
		localhostIpSequence = 0;
	}

	if (m_plLocalhostIp->size() == 0)
	{
		return;
	}
	ip = inet_addr(m_plLocalhostIp->at(localhostIpSequence).c_str());
	if (ip == INADDR_NONE)
	{
			return;
	}

	address.S_un.S_addr = ip;
	iLocalhost = address.S_un.S_un_b.s_b4;

	for (int iAddress = 0; iAddress <= 255; iAddress++)
	{
		_snprintf_s(cBuffer, 32, _TRUNCATE, "%d.%d.%d.%d", address.S_un.S_un_b.s_b1, address.S_un.S_un_b.s_b2, address.S_un.S_un_b.s_b3, iAddress);

		pThreadClient = (CThreadRpcClientSearch *)AfxBeginThread(RUNTIME_CLASS(CThreadRpcClientSearch));
		if (m_pThreadInfo[iAddress] != NULL)
		{
			delete m_pThreadInfo[iAddress];
		}
		m_pThreadInfo[iAddress] = new CRpcInfoClientSearch;

		m_pThreadInfo[iAddress]->m_iThreadID = pThreadClient->m_nThreadID;
		m_pThreadInfo[iAddress]->m_sIp = cBuffer;
		m_pThreadInfo[iAddress]->m_iPort = 31416;
		m_pThreadInfo[iAddress]->m_iTimeOut = m_iTimeOut;	// timeout is not in seconds
		m_pThreadInfo[iAddress]->m_bAutoExit = true;
		m_pThreadInfo[iAddress]->m_sPassWord = "";	//m_sPassWord;
		::PostThreadMessage(m_pThreadInfo[iAddress]->m_iThreadID, UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_THREAD, (WPARAM) this->m_nThreadID, (LPARAM)m_pThreadInfo[iAddress]);
		m_iThreadBusy++;
	}
	m_iTimeOut = 5;
}

void CThreadIpSearch::SearchThreadsDelete()
{
	for (int iCount = 0; iCount < 255+1; iCount++)
	{
		if (m_pThreadInfo[iCount] != NULL)
		{
			delete m_pThreadInfo[iCount];
			m_pThreadInfo[iCount] = NULL;
		}
	}
}

void CThreadIpSearch::Logging(char *pcFrom, char *pcTxt, int iLoggingType)
{
	strcpy_s(m_pcBufferLogFrom, 63, pcFrom);
	m_pLogging->m_pFrom = m_pcBufferLogFrom;
	m_pLogging->m_pText = pcTxt;
	m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT, (WPARAM)m_pLogging, iLoggingType);
}

BEGIN_MESSAGE_MAP(CThreadIpSearch, CWinThread)
END_MESSAGE_MAP()


// CThreadIpSearch message handlers

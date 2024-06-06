// ThreadConnect.cpp : implementation file
//

#include "stdafx.h"
#include "Iphlpapi.h"
#include "BoincTasks.h"
#include "ThreadRpc.h"
#include "ThreadRpcClient.h"
#include "ThreadIpSearch.h"
#include "ThreadConnect.h"
#include "Mac.h"

// CThreadConnect

IMPLEMENT_DYNCREATE(CThreadConnect, CWinThread)

CThreadConnect::CThreadConnect()
{
//	for (int iCount = 0; iCount < 255+1; iCount++)	//0- 255
//	{
//		m_pThreadInfo[iCount] = NULL;
//	}

//	m_iThreadBusy = 0;
//	m_bStartSearchThreadsRequest = false;
	m_bExit = false;
	m_bDelayConnect = false;
	m_tNextSearchThreads = 0; 
}

CThreadConnect::~CThreadConnect()
{
/*
	for (int iCount = 0; iCount < 255+1; iCount++)
	{
		if (m_pThreadInfo[iCount] != NULL)
		{
			delete m_pThreadInfo[iCount];
		}
	}
*/
}

BOOL CThreadConnect::InitInstance()
{
	HWND	hWnd;
//	int		iThread;
	int		iMsg;
	time_t	tSystemTime;
	CRpcThreadConnectData *pThreadConnectData;

	while(!m_bExit)
	{
		MSG msg;
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			iMsg = msg.message;
			if (iMsg > 0)
			{
				if (iMsg == UWM_MSG_THREAD_THREAD_CONNECT)
				{
					hWnd = (HWND) msg.wParam;
					pThreadConnectData = (CRpcThreadConnectData *) msg.lParam;
	
					if(!m_bDelayConnect)
					{
						//if (strstr(pThreadConnectData->m_pcHostName, MAC_NAME) != NULL)
						if (pThreadConnectData->m_iValidMac > 0)
						{
							if (m_lListIp.size() == 0)
							{
								m_bDelayConnect = true;
							}
						}

						if (!m_bDelayConnect)
						{
							Connect(pThreadConnectData);
							m_lListIp.clear();
							::PostThreadMessage(pThreadConnectData->m_iThreadId, UWM_MSG_THREAD_THREAD_CONNECT_READY , pThreadConnectData->m_iThreadId, (LPARAM) pThreadConnectData);
						}
						else
						{
							_time64(&tSystemTime);
							m_tNextConnect = tSystemTime + 5;	// seconds delay.
							m_iDelayConnectRetry = 0;
						}
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
		Sleep(10);

		if (m_bDelayConnect)
		{
			_time64(&tSystemTime);
			if (tSystemTime > m_tNextConnect)
			{
				if (m_iDelayConnectRetry < 5)
				{
					if (m_lListIp.size() == 0)
					{
						theApp.m_pThreadIpSearch->PostThreadMessage(UWM_MSG_THREAD_START_IP_LIST, this->m_nThreadID, 1);
						m_iDelayConnectRetry++;
						m_tNextConnect = tSystemTime + 5;	// seconds delay.	= 5 + 5*5
					}
					else 
					{
						m_iDelayConnectRetry = 10000;
					}
				}
				else
				{
					Connect(pThreadConnectData);
					m_lListIp.clear();
					::PostThreadMessage(pThreadConnectData->m_iThreadId, UWM_MSG_THREAD_THREAD_CONNECT_READY , pThreadConnectData->m_iThreadId, (LPARAM) pThreadConnectData);
					m_bDelayConnect = false;
				}
			}
		}
	}

	return FALSE;
}

int CThreadConnect::SocketReady(int socket)
{
	fd_set fds;
	struct timeval timeout;
	int rc = 0, result;

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

bool CThreadConnect::Connect(CRpcThreadConnectData *pThreadConnectData)
{
	char		*pcPassword;
	std::string	sHostName;
	int			iEncryptedConnection;
	int			iPort;
    int			retval; //, i;
	RPC_CLIENT	*pRpc;

//	pThreadConnectData
	sHostName = pThreadConnectData->m_pcHostName;
	pcPassword = pThreadConnectData->m_pcPassword;
	iEncryptedConnection = pThreadConnectData->m_iEncryptedConnection;
	iPort = pThreadConnectData->m_iPort;
	pRpc = pThreadConnectData->m_pRpc;

	_strlwr((char *) sHostName.c_str());		// to lower
	if (sHostName.find(LOCALHOST_NAME) != std::string::npos)
	{
		sHostName = LOCALHOST_NAME;
	}
	if (sHostName.find("192.168.10.66") != std::string::npos)
	{
		int i = 0;// TODO testing only
	}

	if (pThreadConnectData->m_iValidMac > 0)
//	if (sHostName == MAC_NAME)
	{
		sHostName = FindIp(pThreadConnectData->m_pcMAC);
		if (sHostName.length() == 0)
		{
			pThreadConnectData->m_sFoundIpWithMac = "";
			pThreadConnectData->m_retval = ERR_SELECT;
			pThreadConnectData->m_iRpcStatus = RPC_STATUS_ERROR_CANNOT_CONNECT;
			pThreadConnectData->m_pcErrorMessage = "Connect, no IP found for MAC address";
			return false;
		}
		pThreadConnectData->m_sFoundIpWithMac = sHostName;
	}

    retval = pRpc->init(sHostName.c_str(), iPort);
    if (retval)
	{
		if (retval == ERR_READ)pRpc->close();
		pThreadConnectData->m_retval = retval;
		pThreadConnectData->m_iRpcStatus = RPC_STATUS_ERROR_CANNOT_CONNECT;
		pThreadConnectData->m_pcErrorMessage = "Connect, init";
        return false;
    }

	if (pcPassword)
	{
		if (strcmp(pThreadConnectData->m_pcHostName, LOCALHOST_NAME) != 0)
		{
			if (SocketReady(pRpc->sock) != 0)	// only if not the localhost
			{
				pRpc->close();
				pThreadConnectData->m_retval = ERR_SELECT;
				pThreadConnectData->m_iRpcStatus = RPC_STATUS_ERROR_CANNOT_CONNECT;
				pThreadConnectData->m_pcErrorMessage = "Connect, init";
				return false;
			}
		}

		retval = pRpc->authorize(pcPassword);
		if (retval)
		{
			if (retval == ERR_READ) pRpc->close();

			pThreadConnectData->m_retval = retval;
			pThreadConnectData->m_iRpcStatus = RPC_STATUS_ERROR_AUTHORIZATION;
			pThreadConnectData->m_pcErrorMessage = "Connect, password";
			return false;
		}
	}

	pThreadConnectData->m_retval = retval;
	pThreadConnectData->m_iRpcStatus = RPC_STATUS_CONNECTED;
	pThreadConnectData->m_pcErrorMessage = "";
	return true;
}

int CThreadConnect::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

std::string CThreadConnect::FindIp(char *pcMAC)
{
	std::string sMAC;

	for(int iPos = 0; iPos < (int) m_lListIp.size(); iPos++)
	{
		if (CheckValidMac((char *) m_lListIp.at(iPos).c_str(), pcMAC))
		{
			return m_lListIp.at(iPos);
		}	
	}
	return "";
}

/*
std::string CThreadConnect::FindIp(char *pcMAC)
{
	char		szHostName[MAX_PATH];
	struct hostent *host;
	struct	in_addr		address;
	struct	in_addr *ptr;	// To retrieve the IP Address
	char		cBuffer[32];
	std::string	sMAC, sIP;

	MSG		msg;

	gethostname(szHostName, MAX_PATH );
	host = gethostbyname(szHostName);
	if(host != NULL)
	{
		ptr = (struct in_addr *) host->h_addr_list[0];
		address.S_un.S_un_b.s_b1 = ptr->S_un.S_un_b.s_b1; 
		address.S_un.S_un_b.s_b2 = ptr->S_un.S_un_b.s_b2;
		address.S_un.S_un_b.s_b3 = ptr->S_un.S_un_b.s_b3;
		address.S_un.S_un_b.s_b4 = 0;		//ptr->S_un.S_un_b.s_b4;

		// base address

		for (int iAddress = 0; iAddress <= 255; iAddress++)
		{
			_snprintf_s(cBuffer,32,_TRUNCATE,"%d.%d.%d.%d",address.S_un.S_un_b.s_b1,address.S_un.S_un_b.s_b2,address.S_un.S_un_b.s_b3,iAddress);
			sMAC = IpToMAC(cBuffer);
			if (sMAC.find(*pcMAC) !=  std::string::npos)
			{
				sIP = cBuffer;
				return sIP;
			}

			if ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ))
			{
				if (msg.message == UWM_MSG_THREAD_QUIT)
				{
					m_bExit = true;
					return sIP;
				}
			}
		}
	}
	return sIP;
}
*/

std::string CThreadConnect::IpToMAC(char *pcIP)
{
	IPAddr					DestIp = 0;
	ULONG					MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG					PhysAddrLen = 6;  /* default to length of six bytes */
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

bool CThreadConnect::CheckValidMac(char *pcIP, char *pcMAC)
{
	CMac	mac;

	std::string sMAC;

	if (strlen(pcMAC) < 2)
	{
		return false;
	}

	std::deque<std::string> lMac;
	mac.Extract(pcMAC, &lMac);

	for (int iPos = 0; iPos < (int) lMac.size(); iPos++)
	{
		sMAC = IpToMAC(pcIP);
		if (sMAC.find(lMac.at(iPos).c_str()) != std::string::npos)
		{
			return true;
		}
	}
	return false;
}

BEGIN_MESSAGE_MAP(CThreadConnect, CWinThread)
END_MESSAGE_MAP()


// CThreadConnect message handlers

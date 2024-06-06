// WebServer2.cpp : implementation file
//

//D:\programma\boinc\client\gui_rpc_server_ops.cpp
//D:\programma\boinc\client\gui_rpc_server.cpp
//D:\programma\boinc\client\client_state.cpp


#include "stdafx.h"
#include "error_numbers.h"
#include "network.h"

#include "BoincTasks.h"
#include "WebServer2.h"

#include "DlgLogging.h"
#include "WebServerProcess.h"

CConnection::CConnection(int s, CWebServerProcess *pWebServerProcess)
{
	m_iRequestNbytes = 0;
	m_iSock = s;
	m_pWebServerProcess = pWebServerProcess;
}

CConnection::~CConnection() {
    boinc_close_socket(m_iSock);
}

int CConnection::HandleRequest()
{
	CStringA sFormat ,sFrom;
	int iRecieved, iLen, iErr;
	std::string sRecieved;
	BOOL	bSocketError;
	BYTE	*pcSendBuffer;

	sFrom = "Mobile: HandleRequest";

    int iLeft = SOCKET_DATA_SIZE - m_iRequestNbytes;
	iRecieved = recv(m_iSock, (m_cRecieveBuffer+m_iRequestNbytes), iLeft, 0);

    if (iRecieved <= 0)
	{
		iErr = WSAGetLastError();
		if (iErr == ERROR_SUCCESS)
		{
			sFormat.Format("Closing socket.", iErr);
			SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
			m_iRequestNbytes = 0;
			return ERR_FREAD;
		}
		if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
		{
			sFormat.Format("ERROR: recieve failed. error = %d", iErr);
			SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
		}
		m_iRequestNbytes = 0;
		return ERR_READ;
	}

	if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
	{
		sFormat.Format("Recieved %d bytes.", iRecieved);  
		SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
	}

	sRecieved = (std::string) m_cRecieveBuffer;

	pcSendBuffer = m_pWebServerProcess->Process(&sRecieved, &iLen);
	bSocketError = SendData(pcSendBuffer, iLen);
	if (bSocketError)
	{
		delete pcSendBuffer;
		m_iRequestNbytes = 0;
		return ERR_WRITE;
	}

	m_iRequestNbytes = 0;

	delete pcSendBuffer;
	return BOINC_SUCCESS;
}

#define SOCKET_SEND_LENGTH 32768
BOOL CConnection::SendData(BYTE *pbSend, int iLen)
{
    BOOL bSocketError = FALSE;
    int nBytesSent;
    int iSendLen, iTotalToSend, iSendOffset, iErr;
	CStringA sFormat, sFrom;
	bool	bFirst;
    
	sFrom = "Mobile: SendData";
	bFirst = true;
	iSendOffset = 0;
	iTotalToSend = (int) iLen + 1;

	while (1)
	{
		// check if there's any data received in the buffer after the last one
		// we sent out.
		if (!bFirst)
		{
			if (iTotalToSend <= 0)
			{
				return bSocketError;
			}
		 }

		if (iTotalToSend > SOCKET_SEND_LENGTH)
		{
			iSendLen = SOCKET_SEND_LENGTH;
		}
		else
		{
			iSendLen = iTotalToSend;
		}


		// if the data buffer contains new data, begin sending from the start
		// otherwise, begin sending from the point we left in the last call.
		nBytesSent = send(m_iSock, (char *) (pbSend + iSendOffset), iSendLen, 0);
		if (nBytesSent == SOCKET_ERROR)
		{
			iErr = WSAGetLastError();
			if (iErr == WSAEWOULDBLOCK)
			{
				if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
				{
					sFormat = "send got WSAEWOULDBLOCK. Will retry send later ...";
					SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
				}
				bSocketError = FALSE; // not a real error.
			}
			else
			{
				if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
				{
					sFormat.Format("ERROR: send failed. error = %d", iErr);
					SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
				}
				bSocketError = TRUE;
			}
			return bSocketError;
		}

		// update the stats.
		iSendOffset += nBytesSent;
		iTotalToSend -= nBytesSent;

		bFirst = false;

		if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
		{
			sFormat.Format("Sent %d bytes. Remaining = %d bytes.", nBytesSent, iTotalToSend);  
			SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
		}
	}
//	return bSocketError;
}


void CConnection::SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug)
{
	CLoggingMessage *psLog;

	psLog = new	CLoggingMessage;
	psLog->m_pFrom = psFrom->GetBuffer(); psLog->m_pText = psMessage->GetBuffer();
	theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, iDebug);
	delete psLog;
}


// CWebServer2

CWebServer2::CWebServer2()
{
    m_lsock = -1;
}

CWebServer2::~CWebServer2()
{
	CloseSocket();
	Clear();
}

void CWebServer2::CloseSocket()
{
	boinc_close_socket(m_lsock);
}

void CWebServer2::Clear()
{
	for (int iPos = 0; iPos < (int) m_lConnections.size(); iPos++)
	{
		delete m_lConnections.at(iPos);
	}
	m_lConnections.clear();
}


bool CWebServer2::Connect(CWebServerProcess *pWebServerProcess, int iPort)
{
	CStringA sFormat ,sFrom;
    int		iNrSockets, iIdle;
    struct timeval tv;
	bool	bSet;

	sFrom = "CWebServer2::Connect";

	if (Init(iPort))
	{
		iIdle = 0;

		while (1)
		{
			bool action = true;

			m_allFds.zero();
			ConnectionFds.zero();
			FD_SET(m_lsock, &ConnectionFds.read_fds);
			GetFdset(ConnectionFds, m_allFds);

			tv.tv_sec = HEART_BEAT_INTERVAL;
			tv.tv_usec = 0;
			iNrSockets = select(ConnectionFds.max_fd+1, &ConnectionFds.read_fds, &ConnectionFds.write_fds, &ConnectionFds.exc_fds,  &tv);
			//printf("select in %d out %d\n", all_fds.max_fd, n);

			// Note: curl apparently likes to have curl_multi_perform()
			// (called from net_xfers->got_select())
			// called pretty often, even if no descriptors are enabled.
			// So do the "if (n==0) break" AFTER the got_selects().

			if (iNrSockets > 0)
			{
				bSet = GotSelect(pWebServerProcess, ConnectionFds);
			}
			else
			{
				bSet = false;
			}


			if (bSet)
			{
				iIdle = 0;
			}
			else
			{
				HandleRequests();
				iIdle++;
				if (iIdle > 100)
				{
					boinc_close_socket(m_lsock);
					m_lsock = -1;
					Clear();
					iIdle = 0;
					if (!Init(iPort))
					{
						return false;
					}
				}

			}

			Sleep(10);

			// check if there is a quit message
			MSG msg;
			while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))		// make sure to flush the peek buffer here.
			{
				if (msg.message == UWM_MSG_THREAD_QUIT)
				{
					if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
					{
						sFormat = "Recieved a quit message, stopping...";
						SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
					}
					boinc_close_socket(m_lsock);
					return false;
				}
			}
		}
	}

	return false;
}

bool CWebServer2::Init(int iPort)
{
	CStringA sFormat ,sFrom;
	int retval;
    sockaddr_in addr;

	sFrom = "Mobile: CWebServer2::Init";

	retval = boinc_socket(m_lsock);
	if (retval)
	{
		if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
		{
			sFormat = "failed to create socket";
			SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
		}
        return false;
    }
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int one = 1;
	setsockopt(m_lsock, SOL_SOCKET, SO_REUSEADDR, (char*)&one, 4);
    retval = bind(m_lsock, (const sockaddr*)(&addr), (BOINC_SOCKLEN_T)sizeof(addr));

	if (retval)
	{
		if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
		{
			sFormat.Format("bind to port %d failed: %d", htons(addr.sin_port), retval);
			SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
		}
        boinc_close_socket(m_lsock);
        m_lsock = -1;
        return false;
    }

	if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
	{
		sFormat.Format("Listening on port %d", htons(addr.sin_port));
		SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
	}

    retval = listen(m_lsock, 999);
    if (retval)
	{
		if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
		{
			sFormat.Format("Listen failed: %d", retval);
			SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);    
        }
        boinc_close_socket(m_lsock);
        m_lsock = -1;
        return false;
    }

	return true;
}

bool CWebServer2::GotSelect(CWebServerProcess *pWebServerProcess, FDSET_GROUP& fg )
{
	CStringA sFormat ,sFrom;

    int sock;
    vector<CConnection*>::iterator iter;
    CConnection* gr;
//    bool is_local = false;

	sFrom = "Mobile: CWebServer2::GotSelect";

	if (m_lsock < 0) return false;

	if (FD_ISSET(m_lsock, &fg.read_fds))
	{
		struct sockaddr_storage addr;

		BOINC_SOCKLEN_T addr_len = sizeof(addr);
		sock = (int) accept(m_lsock, (struct sockaddr*)&addr, (BOINC_SOCKLEN_T*)&addr_len);
		if (sock == -1)
		{
			return false;
		}

		if (m_lConnections.size() > 10)
		{
			int ii = 1;
		}

		gr = new CConnection(sock, pWebServerProcess);
		Insert(gr);

		// delete connections with failed sockets
		//
		iter = m_lConnections.begin();
		while (iter != m_lConnections.end())
		{
			gr = *iter;
			if (FD_ISSET(gr->m_iSock, &fg.exc_fds))
			{
				delete gr;
				iter = m_lConnections.erase(iter);
				continue;
			}
			iter++;
		}

		// handle RPCs on connections with pending requests
		//

		HandleRequests();
		return true;
	}

	return false;
}

bool CWebServer2::HandleRequests()
{
	CString sFormat, sFrom;
	int retval;
	CConnection* gr;
	vector<CConnection*>::iterator iter;
	bool	bOk = false;

	sFrom = "CWebServer2::HandleRequest";

	iter = m_lConnections.begin();
	while (iter != m_lConnections.end())
	{
		gr = *iter;
		if (FD_ISSET(gr->m_iSock, &ConnectionFds.read_fds))
		{
			retval = gr->HandleRequest();
			if (retval)
			{
				if (retval != ERR_FREAD)	// not a read error after all just a closed socket
				{
					if (theApp.m_pDlgLogging->m_bLogDebugMobileConnect)
					{
						sFormat.Format("[HandleRequest] handler returned %d, closing socket", retval);
						SendLogging(&sFrom, &sFormat, LOGGING_DEBUG);
					}
				}
				delete gr;
				iter = m_lConnections.erase(iter);
				continue;
			}
			bOk = true;
		}
		iter++;
	}
	return bOk;
}

void CWebServer2::GetFdset(FDSET_GROUP& fg, FDSET_GROUP& all)
{
    unsigned int i;
    CConnection* gr;

    if (m_lsock < 0) return;
    for (i=0; i < m_lConnections.size(); i++) {
        gr = m_lConnections[i];
        int s = gr->m_iSock;
        FD_SET(s, &fg.read_fds);
        FD_SET(s, &fg.exc_fds);
        if (s > fg.max_fd) fg.max_fd = s;

        FD_SET(s, &all.read_fds);
        FD_SET(s, &all.exc_fds);
        if (s > all.max_fd) all.max_fd = s;
    }
    FD_SET(m_lsock, &fg.read_fds);
    if (m_lsock > fg.max_fd) fg.max_fd = m_lsock;
    FD_SET(m_lsock, &all.read_fds);
    if (m_lsock > all.max_fd) all.max_fd = m_lsock;
}

int CWebServer2::Insert(CConnection* p)
{
	m_lConnections.push_back(p);
	return 0;
}

void CWebServer2::SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug)
{
	CLoggingMessage *psLog;

	psLog = new	CLoggingMessage;
	psLog->m_pFrom = psFrom->GetBuffer(); psLog->m_pText = psMessage->GetBuffer();
	theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, iDebug);
	delete psLog;
}
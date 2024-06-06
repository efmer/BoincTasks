// RpcClient.cpp : implementation file
//

#include "stdafx.h"
#ifdef BoincTasks
#include "BoincTasks.h"
#include "DlgLogging.h"
#else
#include "client.h"
#endif
#include "RpcClient.h"
#include <winsock2.h>   // for Winsock API
#include <windows.h>    // for Win32 APIs and types
#include <ws2tcpip.h>   // for IPv6 support
#include <wspiapi.h>    // for IPv6 support
#include <Mswsock.h>

// CRpcClient

CRpcClient::CRpcClient()
{
	m_bWSAStartup = false;
	pWndLogging = theApp.m_pDlgLogging;
	m_bConnected = false;
	m_clientContext.sock = INVALID_SOCKET;
	m_pRes = NULL;

	m_pcReceiveBuffer = new char [DEFAULT_RECV_BUFFER_SIZE+1];

	m_pcBufferFrom = new char [BUFFER_FROM_LEN+1];
	m_pcBufferLog = new char [BUFFER_LOG_LEN+1];
}

CRpcClient::~CRpcClient()
{
	delete m_pcReceiveBuffer;
	ClearAddress();

	delete m_pcBufferFrom;
	delete m_pcBufferLog;

}

bool CRpcClient::Initialize()
{
	return true;
}

void CRpcClient::Cleanup()
{
}

bool CRpcClient::Connect(char *pcServer, char *pcPort, int iSetTimeout, bool bIpV6)
{
	if (m_bConnected) return true;		// already connected


    // fill up the default arguments and let the user options override these.

	if (bIpV6) m_clientContext.addressFamily = AF_INET6;
	else m_clientContext.addressFamily = AF_INET;
	m_clientContext.m_sServer = pcServer;

	m_clientContext.m_sServer.MakeLower();
	if (m_clientContext.m_sServer.Find(LOCALHOST_NAME) >= 0)
	{
		m_clientContext.m_sServer = LOCALHOST_NAME;
	}

	m_clientContext.szPort = pcPort;
    m_clientContext.sendBufSize = DEFAULT_SEND_BUFFER_SIZE+10;
    m_clientContext.recvBufSize = DEFAULT_RECV_BUFFER_SIZE+10;    
  
	// create a socket that's connected to the server.

	m_clientContext.sock = CreateClientSocket(iSetTimeout);	

	if (m_clientContext.sock == INVALID_SOCKET) 
	{
		if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
		{
			strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,"Connect");
			strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"Invalid Socket");
			SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
		}
		return false;
	}

	m_bConnected = true;
	return true;
}

void CRpcClient::Disconnect()
{
	// close the client socket.    
	if (m_bConnected)
	{
		closesocket(m_clientContext.sock);  
		if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
		{
			char pcFormat[128];
			int socketNr = (int)m_clientContext.sock;
			_snprintf_s(pcFormat,127,_TRUNCATE,"Closed socket %d. Total Bytes Recd = %d, Total Bytes Sent = %d", socketNr, m_clientContext.nBytesRecd, m_clientContext.sendBufSize -  m_clientContext.nBytesRemainingToBeSent);
			strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,"Disconnect");
			strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,pcFormat);
			SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG); 
		}
	}

	m_bConnected = false;
}

void CRpcClient::ClearAddress()
{
//	CString sFormat, sFrom;

//	sFrom = "ClearAddress";
    if (m_pRes)
    {
        freeaddrinfo(m_pRes);
		m_pRes = NULL;
		if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
		{
			strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,"ClearAddress");
			strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"Freed the memory allocated for res by getaddrinfo");
			SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
		}
    }
}

bool CRpcClient::SendReceive(char *pcSend)
{
	bool	bOk;
//	CString sFormat ,sFrom;
	char	*pcFrom = "SendReceive";

	bool bReceived = false;

	bOk = true;

	// allocate and initialize the buffer to be sent.
	if (PrepareSendBuffer(pcSend, (int) (strlen(pcSend) + 1)) == FALSE)
	{
		bOk = false;
		if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
		{
			strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
			strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"PrepareSendBuffer == FALSE");
			SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
		}
	}
	// allocate and initialize a buffer for receiving.
	
	if (bOk)
	{
		if (PrepareRecvBuffer() == FALSE)
		{
			bOk = false;
		}
	}

	if (bOk)
	{
		if (DoSendUntilDone())
		{
			DoShutDown();							//$#$#$#$#$#$
			if (DoRecvUntilDone())
			{
				bReceived = true;
				strncpy_s(m_pcReceiveBuffer, DEFAULT_RECV_BUFFER_SIZE, m_clientContext.pRecvBuf, DEFAULT_RECV_BUFFER_SIZE-1);
				*(m_pcReceiveBuffer + DEFAULT_RECV_BUFFER_SIZE-1) = '\0';	// make sure the string is always terminated correctly
				if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
				{
					char pcFormat[256];
					_snprintf_s(pcFormat,127,_TRUNCATE,"Received from Server: %s", m_clientContext.pRecvBuf);
					strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
					strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN, pcFormat);
					SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
				}
			}
		}
		else
		{
			bReceived = false;
		}
	}


	// free the send and recv buffers.
	FreeSendBuffer();    
	FreeRecvBuffer();

	return bReceived;
}

/*
    This function create a socket for the client and connects it to the
    server.
*/
SOCKET CRpcClient::CreateClientSocket(int iSetTimeout)
{
//	HANDLE  hEvent;
//	WSANETWORKEVENTS ne;


//	CString sFormat ,sFrom;
    struct addrinfo hints;
    struct addrinfo *pAddr;
    SOCKET clientSock = INVALID_SOCKET;
    int i;
    int rc;
	bool	bOk;
	bool	bConnectedOk = false;
	char	*pcFrom = "TThrottle CreateClientSocket";

	bOk = true;
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = m_clientContext.addressFamily;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;

    // get the local addresses that are suitable for connecting to the
    // given server address.
//	res = NULL; ##$$

	if (m_pRes == NULL) //##$$
	{
		if (getaddrinfo(m_clientContext.m_sServer, m_clientContext.szPort, &hints, &m_pRes) != NO_ERROR) 
		{
			if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
			{
				char pcFormat[128];
				_snprintf_s(pcFormat,127,_TRUNCATE,"getaddrinfo failed: %s", WSAErrorString(WSAGetLastError()));
				strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
				strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN, pcFormat);
				SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
			}
			bOk = false;
		}
	}
	if (bOk)
	{
		if (m_pRes == NULL)
		{	
			if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
			{
				strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
				strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN, "getaddrinfo returned res = NULL");
				SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
			}
			bOk = false;
		}
	}

	if (bOk)
	{
		for (pAddr = m_pRes, i = 1; pAddr != NULL; pAddr = pAddr->ai_next, i++)
		{	
			if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
			{
				char pcFormat[128];
				_snprintf_s(pcFormat,127,_TRUNCATE,"Processing Address %d returned by getaddrinfo : ", i);
				strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
				strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN, pcFormat);
				SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
			}
			clientSock = WSASocket(pAddr->ai_family, pAddr->ai_socktype, pAddr->ai_protocol, NULL, NULL, WSA_FLAG_OVERLAPPED);
			if (clientSock == INVALID_SOCKET)
			{
				if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
				{
					char pcFormat[128];
					_snprintf_s(pcFormat,127,_TRUNCATE,"WSASocket failed.\r\nError = %s", WSAErrorString(WSAGetLastError()));
					strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
					strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN, pcFormat);
					SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
					strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"Ignoring this address and continuing with the next.");
					SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
				}
            
				// anyway, let's continue with other addresses.
				continue;
			}
			if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
			{
				char pcFormat[128];
				int socketNr = (int)clientSock;
				_snprintf_s(pcFormat,127,_TRUNCATE,"Created client socket with handle = %d", socketNr);
				strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
				strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,pcFormat);
				SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
			}

			if (clientSock != INVALID_SOCKET)
			{

			}

			if (iSetTimeout)
			{
				// new in 0.93

				OVERLAPPED			olOverlap;
				GUID				GuidConnectEx	= WSAID_CONNECTEX;
				LPFN_CONNECTEX		lpfnConnectEx	= NULL;
				DWORD				dwBytes			= 0;
				DWORD				dwErr;
				int					iTimeOut;
	
				rc = WSAIoctl(clientSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidConnectEx, sizeof(GuidConnectEx),  &lpfnConnectEx, sizeof(lpfnConnectEx),   &dwBytes, NULL, NULL); 
	
				memset(&olOverlap,0,sizeof(OVERLAPPED));
				if (lpfnConnectEx != NULL)
				{
					struct sockaddr_in addrAny;
					memset(&addrAny, 0, sizeof(addrAny));
					addrAny.sin_addr.S_un.S_addr = 0;
					addrAny.sin_family = pAddr->ai_family;
					addrAny.sin_port = 0;
	
					rc = bind( clientSock, (SOCKADDR*) &addrAny, sizeof(addrAny));
					if (!rc)
					{
						rc = lpfnConnectEx(clientSock, pAddr->ai_addr, (int)  pAddr->ai_addrlen,   NULL,  0, NULL,  &olOverlap);
						if(!rc)
						{
	
							dwErr = WSAGetLastError();
							if(dwErr == ERROR_IO_PENDING)
							{
								iTimeOut = 0;
								while (iTimeOut < iSetTimeout)
								{
									int ii = 1;
									int seconds;
									int bytes = sizeof(seconds);
									rc = getsockopt( clientSock, SOL_SOCKET, SO_CONNECT_TIME, (char *)&seconds, (PINT)&bytes );
									if ( rc != NO_ERROR )
									{
										//printf( "getsockopt(SO_CONNECT_TIME) failed with error: %ld\n", WSAGetLastError() );
										break;
									}
									if (seconds == 0xFFFFFFFF)
									{
										//printf("Connection not established yet\n");
									}
									else
									{
										//printf("Connection has been established %ld seconds\n",  seconds);
										bConnectedOk = true;
										break;
									}
									Sleep(1000);
									iTimeOut++;
								}
							}
						 }
					}
					else // bind
					{
						dwErr = WSAGetLastError();
						int ii = 1;
					}
				}
			}
			// end new


			if (iSetTimeout)
			{
				if (bConnectedOk)
				{
					closesocket(clientSock);
					clientSock = INVALID_SOCKET;
					clientSock = WSASocket(pAddr->ai_family, pAddr->ai_socktype, pAddr->ai_protocol, NULL, NULL, 0);
					rc = connect(clientSock, pAddr->ai_addr, (int) pAddr->ai_addrlen);
				}
				else
				{
					rc = SOCKET_ERROR;
				}
			}
			else
			{
				rc = connect(clientSock, pAddr->ai_addr, (int) pAddr->ai_addrlen);
			}

			if (rc == SOCKET_ERROR)
			{ 
				if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
				{
					char pcFormat[128];
					_snprintf_s(pcFormat,127,_TRUNCATE,"CreateClientSocket: connect failed.\r\nError = %s", WSAErrorString(WSAGetLastError()));
					strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
					strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,pcFormat);
					SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
				}
				closesocket(clientSock);
				clientSock = INVALID_SOCKET;
				continue;
			}
			if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
			{
				strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
				strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"CreateClientSocket: Connected successfully");
				SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
			}
			break;
		}
	}

    return clientSock;
}

/*
    Try sending data to server once. Return the error, if any.
*/
int CRpcClient::DoSendOnce()
{
    int nBytesSent;    
    int startPosition;
    int err = 0;
//	CString sFormat ,sFrom;    
//	sFrom = "TThrottle DoSendOnce";

    // send from the position where we left off last.
    startPosition = m_clientContext.sendBufSize - 
                    m_clientContext.nBytesRemainingToBeSent;
    nBytesSent = send(m_clientContext.sock, 
                      m_clientContext.pSendBuf + startPosition, 
                      m_clientContext.nBytesRemainingToBeSent,
                      0);
    
    if (nBytesSent == SOCKET_ERROR)
    {
        err = WSAGetLastError();
		return err;
    }

    // remember where to begin send next time.
    m_clientContext.nBytesRemainingToBeSent -= nBytesSent;
	if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
	{
		char *pcFrom = "TThrottle DoSendOnce";
		char pcFormat[128];
		_snprintf_s(pcFormat,127,_TRUNCATE,"Sent %d bytes so far\n", startPosition + nBytesSent);
		strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
		strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,pcFormat);
		SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
	}
    return err;
}


/*
    Try receiving from the server. This function returns the number of bytes
    received (unlike DoSendOnce which returns the error code).
*/
int CRpcClient::DoRecvOnce()
{
    int nBytesRecd;    
//	CString sFormat ,sFrom;
//	sFrom = "TThrottle DoRecvOnce";
	char *pcFrom = "TThrottle DoRecvOnce";

    // receive into the same global receive buffer, overwriting the previous
    // contents, as we are only interested in the number of bytes received
    // for verification.
    nBytesRecd = recv(m_clientContext.sock, 
                      m_clientContext.pRecvBuf, 
                      m_clientContext.recvBufSize,
                      0);
    
    if (nBytesRecd == SOCKET_ERROR)
    {
		if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
		{
			strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
			strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN, WSAErrorString(WSAGetLastError()));
			SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
		}
		return nBytesRecd;
    }

    // update the stats.
    m_clientContext.nBytesRecd += nBytesRecd;
	if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
	{
		char pcFormat[128];
		_snprintf_s(pcFormat,127,_TRUNCATE,"Recd %d bytes so far\n", m_clientContext.nBytesRecd);
		strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
		strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,pcFormat);
		SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
	}
    return nBytesRecd;
}


/*
    Repeat sending the data to the server until all the data has been
    completely sent to the server.
*/
bool CRpcClient::DoSendUntilDone()
{
    int err;
//	CString sFormat ,sFrom;

	char	*pcFrom = "TThrottle DoSendUntilDone";
    do
    {
        // Get the job done by callnig DoSendOnce.
        err = DoSendOnce();

        // handle error cases as appropriate.
        switch(err)
        {
            case 0:
                if (m_clientContext.nBytesRemainingToBeSent == 0)
                {
					if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
					{
						strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
						strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"Send completed");
						SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
					}
                    return true;
                }
                break;
               
            case WSAEWOULDBLOCK:
				if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
				{
					strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
					strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"WSAEWOULDBLOCK, Waiting 1 second before retrying send ...");
					SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
				}
                Sleep(1000);
                break;

            default:
                // other errors.
				if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
				{
					char pcFormat[128];
					_snprintf_s(pcFormat,127,_TRUNCATE,"ERROR: send failed: Error = %d\n", err);
					strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
					strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,pcFormat);
					SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
				}
                return false;
        }
    } while (TRUE);
}


/*
    Receive data on the socket until recv returns 0 or error.
*/
bool CRpcClient::DoRecvUntilDone()
{
    int err;
//	CString sFormat ,sFrom;

	char *pcFrom = "TThrottle DoRecvUntilDone";
    do
    {
        // get the job done by calling DoRecvOnce.
        switch(DoRecvOnce())
        {
            case 0:
				if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
				{
					strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
					strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"Recv returned 0. Remote socket must have been gracefully closed.");
					SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
				}
                return true;
            case SOCKET_ERROR:
                err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK)
                {
					if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
					{
						strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
						strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"WSAEWOULDBLOCK, Waiting 1 second before retrying send...");
						SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
					}
                    Sleep(1000);
                }
                else
                {
					if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
					{
						strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
						strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN, "Socket error");
						SendLogging(m_pcBufferFrom, WSAErrorString(err), LOGGING_DEBUG);
					}
                    return false;
                }
                break;

            default:
                // > 0 bytes read. let's continue.
                break;
        }
    } while (1);
}


/*
    This functions shuts down the socket before closing it so that
    it would send a FIN packet on the TCP connection to the remote side
    to indicate that there won't be any more data sent on this socket.
    Only if this side shuts down, the remote side will get a 0 for a
    recv and conclude that the client is done with sending data.
*/
void CRpcClient::DoShutDown()
{
//	CString sFormat ,sFrom;
//	sFrom = "TThrottle DoShutDown";

    if (shutdown(m_clientContext.sock, SD_SEND) == SOCKET_ERROR)
	{
		char *pcFrom = "TThrottle DoShutDown";
		char pcFormat[128];
		_snprintf_s(pcFormat,127,_TRUNCATE,"shutdown failed. err = %d\n", WSAGetLastError());
		strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,pcFrom);
		strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,pcFormat);
		SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
	}
}

/*
    Allocate a buffer to use for sending data and initialize the buffer
    with suitable initial values.
*/
BOOL CRpcClient::PrepareSendBuffer(char *pSend, int iSend)
{
    BOOL bSuccess = FALSE;
    
	m_clientContext.sendBufSize = iSend;
    m_clientContext.pSendBuf = (char *) malloc(m_clientContext.sendBufSize + 1);
    if (m_clientContext.pSendBuf == NULL)
    {
		_snprintf_s(m_pcBufferLog,127,_TRUNCATE,"shutdown failed. err = %d\n", WSAGetLastError());
		strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,"TThrottle PrepareSendBuffer");
		SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
		return bSuccess;
    }
    
//	sFormat.Format("Allocated Send Buffer: %p\n", m_clientContext.pSendBuf);
//	SendLogging(&sFrom, &sFormat);

	strcpy_s(m_clientContext.pSendBuf,iSend,pSend);

    // fill up with some info. here we are just sending 1 character.
//    memset(m_clientContext.pSendBuf, 'H', m_clientContext.sendBufSize);   
    m_clientContext.pSendBuf[m_clientContext.sendBufSize] = '\0';
    m_clientContext.nBytesRemainingToBeSent = m_clientContext.sendBufSize;
    
    bSuccess = TRUE;
    return bSuccess;
}

/*
    Deallocate the buffer that was used for sending.
*/
void CRpcClient::FreeSendBuffer()
{
    if (m_clientContext.pSendBuf != NULL)
    {
        free(m_clientContext.pSendBuf);
        m_clientContext.pSendBuf = NULL;
    }
}


/*
    Allocate a buffer for receiving data from the server.
    Currently since we are not using the received data, we'll just receive
    all the data in a big one-time buffer for all the receives and overwrite
    the same buffer each time. The count of received bytes is what we
    are presently interested in.
*/
BOOL CRpcClient::PrepareRecvBuffer()
{
//	CString sFormat ,sFrom;
    BOOL bSuccess = FALSE;
   
    m_clientContext.pRecvBuf = (char *) malloc(m_clientContext.recvBufSize + 1);
    if (m_clientContext.pRecvBuf == NULL)
    {
		strcpy_s(m_pcBufferFrom,BUFFER_FROM_LEN,"TThrottle PrepareRecvBuffer");
		strcpy_s(m_pcBufferLog,BUFFER_LOG_LEN,"malloc failed.");
		SendLogging(m_pcBufferFrom, m_pcBufferLog, LOGGING_DEBUG);
		return bSuccess;
    }
    
//	sFormat.Format("Allocated Recv Buffer: %p\n", m_clientContext.pRecvBuf);
//	SendLogging(&sFrom, &sFormat);
    
    memset(m_clientContext.pRecvBuf, 0, m_clientContext.recvBufSize + 1);   
    m_clientContext.nBytesRecd = 0;
    
    bSuccess = TRUE;
    return bSuccess;
}


/*
    Deallocate the buffer used for receiving.
*/
void CRpcClient::FreeRecvBuffer()
{
//	CString sFormat ,sFrom;

//	sFrom = "FreeRecvBuffer";
    if (m_clientContext.pRecvBuf != NULL)
    {
        free(m_clientContext.pRecvBuf);
//		sFormat.Format("Freed Recv Buffer : %p\n", m_clientContext.pRecvBuf);
//		SendLogging(&sFrom, &sFormat);
        m_clientContext.pRecvBuf = NULL;
    }
}

/*
    Prints the given socket address in a printable string format.
*/
/*
CStringA CRpcClient::GetAddressString(LPSOCKADDR pSockAddr, DWORD dwSockAddrLen)
{
    // INET6_ADDRSTRLEN is the maximum size of a valid IPv6 address 
    // including port,colons,NULL,etc.

	CStringA sFormat;
    char buf[INET6_ADDRSTRLEN];
    DWORD dwBufSize = 0;    

    memset(buf,0,sizeof(buf));
    dwBufSize = sizeof(buf);

    // This function converts the pSockAddr to a printable format into buf.   
    if (WSAAddressToStringA(pSockAddr, dwSockAddrLen, NULL, buf, &dwBufSize) == SOCKET_ERROR)
    {
		sFormat.Format("ERROR: WSAAddressToString failed %d \n", WSAGetLastError());
        return sFormat;
    }

	sFormat.Format("%s\n", buf);
	return sFormat;
}
*/

char *CRpcClient::WSAErrorString(int iError)
{
//	CStringA sFormat;

	switch(iError)
	{
		case WSAECONNREFUSED:
			return "No connection could be made because the target machine actively refused it";
		break;
		case WSAETIMEDOUT:
			return "A connection attempt failed because the connected party did not properly respond after a period of time,\r\nor established connection failed because connected host has failed to respond\r\n";
		break;
		case WSAHOST_NOT_FOUND:
			return "No such host is known";
		break;
		case WSAECONNRESET:
			return "An existing connection was forcibly closed by the remote host";
		break;
		case WSAEADDRINUSE:
			return "Only one usage of each socket address is normally permitted.";
		break;
	}
//	sFormat.Format("Unknown: %d", iError);
//	return sFormat;
	return "unknown";
}

void CRpcClient::SendLogging(char *pcFrom, char *pcMessage, int iDebug)
{
	CLoggingMessage *psLog;
	
	if (::IsWindow(theApp.m_pDlgLogging->m_hWnd))
	{	
		psLog = new CLoggingMessage;
		psLog->m_pFrom =  pcFrom; psLog->m_pText = pcMessage;
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, iDebug);
		delete psLog;
	}
}

/*
void CRpcClient::SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug)
{
	CString sComputerId;

	CLoggingMessage *psLog;
	psLog = new CLoggingMessage;

	sComputerId = m_pcComputerId;

	*psFrom = sComputerId + ", " + *psFrom;
	psLog->m_pFrom =  psFrom->GetBuffer(); psLog->m_pText = psMessage->GetBuffer();
	pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, iDebug);

	delete psLog;
}
*/
#pragma once

//#define LOGGING_NORMAL		0
//#define LOGGING_DEBUG		1

// 4 AF_INET
// 6 AF_INET6

#define DEFAULT_SEND_BUFFER_SIZE		256
// added 1024 because of a buffer too small exception
#define DEFAULT_RECV_BUFFER_SIZE		((1024*16)+1024)+1024

#define BUFFER_FROM_LEN		256
#define BUFFER_LOG_LEN		255


#include <winsock2.h>   // for Winsock API

typedef struct _ClientContext
{
    BYTE    addressFamily;
    CString m_sServer;
    char    *szPort;
    LONG    sendBufSize;
    LONG    recvBufSize;    

    char    *pSendBuf;
    char    *pRecvBuf;    
    int     nBytesRemainingToBeSent;
    int     nBytesRecd;
    SOCKET  sock;
    
} ClientContext;

#ifdef BoincTasks
#else
class CLoggingMessage
{
public:
	char *m_pFrom;
	char *m_pText;
};
#endif


// CRpcClient

class CRpcClient : public CObject
{

public:
	CRpcClient();
	virtual ~CRpcClient();

	bool	Initialize();
	bool	Connect(char *pcServer, char *pcPort, int iSetTimeout , bool bIpV6);
	void	Disconnect();
	void	ClearAddress();
	bool	SendReceive(char *pcSend);

	void	Cleanup();

	char	*m_pcReceiveBuffer;

	CWnd	*pWndLogging;
	std::string	m_sComputerId;
protected:
	SOCKET	CreateClientSocket(int iTimeout);
	CStringA GetAddressString(LPSOCKADDR pSockAddr, DWORD dwSockAddrLen);
	char	*WSAErrorString(int iError);
	bool	m_bWSAStartup;

	int		DoSendOnce();
	int		DoRecvOnce();
	bool	DoSendUntilDone();
	bool	DoRecvUntilDone();
	void	DoShutDown();

	BOOL	PrepareSendBuffer(char *pSend, int iSendMax);
	void	FreeSendBuffer();
	BOOL	PrepareRecvBuffer();
	void	FreeRecvBuffer();

	char	*m_pcBufferFrom;
	char	*m_pcBufferLog;

	void	SendLogging(char *pcFrom, char *pcMessage, int iDebug = 1);
//	void	SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug = 1);


	struct addrinfo *m_pRes;
	bool	m_bConnected;

	ClientContext m_clientContext;
};



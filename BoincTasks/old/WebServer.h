#pragma once

#include <winsock2.h>   // for Winsock API

const BYTE NON_BLOCKING_ACCEPT = 1;


// The default address family to bind to.
#define DEFAULT_ADDRESS_FAMILY          AF_UNSPEC
// The default interface to listen to.
#define DEFAULT_INTERFACE               NULL
// The default port to listen to.
#define DEFAULT_PORT                    "80"
// The default type of accept to be done.
#define DEFAULT_TYPE_OF_ACCEPT          NON_BLOCKING_ACCEPT


// This is the number of clients that can be accomodated in the FD_SETs that
// are used in the select command. In spite of the name referring to the
// max number of clients, the actual number of clients that will be
// accomodated will be a little less because the select will also include the
// server interfaces on which the app is listening.
// If you want to increase this value, first undefine FD_SETSIZE and redefine
// FD_SETSIZE to the required value before including winsock2.h
#define MAX_CLIENTS             FD_SETSIZE

// This is the default size of the data buffer that'll be used in recv
// each time. In real apps, this value will be much higher.
#define RECV_DATA_SIZE          (16*1024)

// This structure defines the contents of the data buffer that'll be used
// to store the messages that are received from and sent to the client.
typedef struct _DATA_BUFFER
{
    char buf[RECV_DATA_SIZE];  // the data buffer
    int  dataSize;             // length of the actual data present
    int  sendOffset;           // position of the next byte in buf to be sent
    BOOL isNewData;            // TRUE if buf has not been fully sent
} DATA_BUFFER;

// This structure holds the information for each socket the server creates.
// The server maintains a list of these structures and operates on them.
typedef struct _SOCK_INFO
{
    SOCKET sock;                // socket handle
    BOOL   isSocketListening;   // TRUE if the socket is listening
    struct _SOCK_INFO *prev,    // previous structure in the list
                      *next;    // next structure in the list
    DATA_BUFFER recdData;       // details of data buffer that's recd/sent
    int     nTotalRecd;         // total number of bytes recd so far
    int     nTotalSent;         // total number of bytes sent so far
    BOOL    isFdCloseRecd;      // TRUE if FD_CLOSE event was received
} SOCK_INFO, *PSOCK_INFO;

// This structure bundles all the global variables needed between 
// different functions into a global context.
typedef struct _AcceptContext
{
    BYTE    addressFamily;      // Address Family requested
    char    *szInterface;       // Interface to listen on
    char    *szPort;            // Port to listen on
    BYTE    typeOfAccept;       // Which type of accept should be done
    SOCK_INFO *pSockList;       // List of the listening/accepted sockets
    HWND    hAcceptWindow;      // Handle to the hidden accept window
} AcceptContext;

//class CServerSendThread
//{
//ublic:
//	CStringA *m_pMessage;
//	CStringA *m_pPort;
//};


// CRpcServer command target
class CWebServerProcess;
class CWebServer : public CObject
{
public:
	CWebServer();
	virtual ~CWebServer();

	void	Cleanup();
	bool	Connect(CWebServerProcess *pWebServerProcess, std::string *psPort);

//	CWnd	*m_pLoggingWnd;

	CStringA m_sAKey;

protected:
	bool	CreateListeningSockets();
	void	DestroyListeningSockets();
	void	NonBlockingAccept();
	PSOCK_INFO ProcessAcceptEvent(PSOCK_INFO pSockInfo);
	BOOL	ProcessReadEvent(PSOCK_INFO pSockInfo);
	BOOL	SendData(PSOCK_INFO pSockInfo, BYTE *pbSend, int iLen);

	PSOCK_INFO AllocAndInitSockInfo();
	void	FreeSockInfo(PSOCK_INFO pSockInfo);
	void	AddSockInfoToList(PSOCK_INFO *ppHead, PSOCK_INFO pNewSockInfo);
	void	DeleteSockInfoFromList(PSOCK_INFO *ppHead, PSOCK_INFO pDelSockInfo);

	CStringA GetAddressString(LPSOCKADDR pSockAddr, DWORD dwSockAddrLen);
	CStringA WSAErrorString(int iError);
	void	SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug = 0);

	AcceptContext m_acceptContext;

	CWebServerProcess *m_pWebServerProcess;

	char	m_cFileNameLog[MAX_PATH+32];

};



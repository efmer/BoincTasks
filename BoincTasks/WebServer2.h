#pragma once

#include "network.h"

#define SOCKET_DATA_SIZE          (1024*25)
#define HEART_BEAT_INTERVAL			15

class CWebServerProcess;
class CConnection
{
public:
    int		m_iSock;
	char	m_cRecieveBuffer[SOCKET_DATA_SIZE+1];
    int		m_iRequestNbytes;
	CWebServerProcess *m_pWebServerProcess;
private:
//    bool notice_refresh;
        // next time we get a get_notices RPC,
        // send a -1 seqno, then the whole list
public:
	int HandleRequest();
    CConnection(int, CWebServerProcess *pWebServerProcess);
    ~CConnection();
	BOOL SendData(BYTE *pbSend, int iLen);
	void SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug);

};


// CWebServer2 command target

class CWebServerProcess;
class CWebServer2 : public CObject
{
public:
	CWebServer2();
	virtual ~CWebServer2();
	void	Clear();
	bool	Connect(CWebServerProcess *pWebServerProcess, int iPort);
	void	CloseSocket();
	int		m_lsock;
private:
	bool Init(int iPort);
	bool GotSelect(CWebServerProcess *pWebServerProcess, FDSET_GROUP& fg);
	int	 Insert(CConnection* p);
	bool HandleRequests();
	void GetFdset(FDSET_GROUP& fg, FDSET_GROUP& all);
	void SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug);

	fd_set m_readFds;
	FDSET_GROUP m_allFds;
	FDSET_GROUP ConnectionFds;

    std::vector<CConnection*> m_lConnections;
};



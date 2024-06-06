#pragma once


class CRpcInfoClientSearch;

// CThreadConnect

class CThreadConnect : public CWinThread
{
	DECLARE_DYNCREATE(CThreadConnect)

protected:
	CThreadConnect();           // protected constructor used by dynamic creation
	virtual		~CThreadConnect();
	int			SocketReady(int socket);
	bool		Connect(CRpcThreadConnectData *pThreadConnectData);

	std::string FindIp(char *pcMAC);
	std::string IpToMAC(char *pcIp);
	bool		CheckValidMac(char *pcIP, char *pcMAC);
//	void		SearchThreadsFire();
//	void		SearchThreadsDelete();

	bool	m_bExit;

//	int		m_iThreadBusy;
//	bool	m_bStartSearchThreadsRequest;

//	CRpcInfoClientSearch	*m_pThreadInfo[255+2];

	std::deque<std::string> m_lListIp;
//	std::deque<std::string> m_lListIpScan;

	bool	m_bDelayConnect;
	time_t	m_tNextConnect;
	time_t	m_tNextSearchThreads;
	int		m_iDelayConnectRetry;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};



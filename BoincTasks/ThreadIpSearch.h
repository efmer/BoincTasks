#pragma once


class CRpcInfoClientSearch;

// CThreadIpSearch

class CThreadIpSearch : public CWinThread
{
	DECLARE_DYNCREATE(CThreadIpSearch)

protected:
	CThreadIpSearch();           // protected constructor used by dynamic creation
	virtual		~CThreadIpSearch();
	int			SocketReady(int socket);

//	std::string FindIp(char *pcMAC);
//	std::string IpToMAC(char *pcIp);
//	bool		CheckValidMac(char *pcIP, char *pcMAC);
	void		SearchThreadsFire();
	void		SearchThreadsDelete();

	bool	m_bExit;

	int		m_iThreadBusy;
	bool	m_bStartSearchThreadsRequest;

	int		m_iTimeOut;

//	std::string *m_psLocalhostIp;
	std::deque<std::string> *m_plLocalhostIp;
	int		localhostIpSequence;

	CRpcInfoClientSearch	*m_pThreadInfo[255+2];

	std::deque<std::string> m_lListIp;
	std::deque<std::string> m_lListIpScan;

	time_t	m_tNextSearchThreads;
	int		m_iNextSearchThreadsInterval;

	int		m_iLoggingCount;
	CDlgLogging *m_pWndLogging;
	CLoggingMessage	*m_pLogging;
	char *m_pcBufferLog;
	char *m_pcBufferLogFrom;
	void Logging(char *pcFrom, char *pcTxt, int iLoggingType);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};



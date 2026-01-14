#pragma once

class CWebServerProcess;

// CThreadWebServerConnection

class CThreadWebServerConnection : public CWinThread
{
	DECLARE_DYNCREATE(CThreadWebServerConnection)

protected:
	CThreadWebServerConnection();           // protected constructor used by dynamic creation


public:
	virtual ~CThreadWebServerConnection();
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void	CloseSocket();

protected:
	DECLARE_MESSAGE_MAP()

	CWebServerProcess *m_pWebServerProcess;
	CWebServer2 *m_pWebServer;

	bool	m_bExit;
};



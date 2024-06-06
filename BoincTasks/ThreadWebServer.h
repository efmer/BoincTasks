#pragma once

#define WEBSERVER_TIMEOUT			60

#define WEBSERVER_LOGIN_NO_PASSWORD	-1
#define WEBSERVER_LOGIN_AUTO		1
#define WEBSERVER_LOGIN_NEEDED		2
#define WEBSERVER_LOGIN_PASSWORD	3
#define WEBSERVER_LOGIN_FAILED		4

#define WEBSERVER_SELECT_SINGLE		0
#define WEBSERVER_SELECT_MULTIPLE	1
#define WEBSERVER_SELECT_FILTER		2

#define WEBSERVER_LOG_NAME	"BoincTasks Mobile"

extern time_t	g_tLastMobileActivety;
extern CThreadWebServer	*g_pThreadWebServer;
extern bool		g_bThreadWebServerIsRunning;
extern bool		g_bWebServerActive;
extern char		*g_pcWebServerProperties;
extern char		*g_pcWebServerHtml;
extern DWORD	g_tWebServerHtml;
extern char		*g_pcWebServerHtmlTab[7];
extern int		g_iWebServerHtmlTabSend[7];
extern DWORD	g_tWebServerHtmlTabSend[7];
extern char		g_pcStatusToSend[128];
extern int		g_iWebServerLoginDelay;

// CThreadWebServer

class CThreadWebServerConnection;
class CThreadWebServer : public CWinThread
{
	DECLARE_DYNCREATE(CThreadWebServer)

protected:
	CThreadWebServer();           // protected constructor used by dynamic creation
	virtual ~CThreadWebServer();

	HANDLE m_hThreadWebServer;

	bool m_bExit;
	bool m_bServerIsRunning;


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();


protected:
	CThreadWebServerConnection	*m_pThreadWebServerConnection;

	bool	m_bStart;
	int		m_iStartDelay;
	int		m_iOs;
	int		m_iPort;

	bool	m_bThreadBusy;

	DECLARE_MESSAGE_MAP()
};



#pragma once


#define CLOUDSERVER_TIMEOUT			60

#define CLOUDSERVER_LOG_NAME	"BoincTasks cloud"
//#define DRIVE_SIGNATURE_FILE	"_-_-_-_bt_id_-_-_-_.bt_id_file"

#define HANDSHAKE_FILE			"bt.txt"

#define CLIENT_REQUEST_NONE		-1

#define	RQ_ERROR				-2
#define	RQ_TIMEOUT				-3
#define RQ_LOGIN				1
#define RQ_TAB_DATA				2
#define RQ_COMPUTER_DIALOG		3
#define RQ_COMPUTER_SELECT		4
#define RQ_SELECT_MULTIPLE		5
#define RQ_REPORT				6
#define RQ_PROJECT_PROPERTIES	7
#define RQ_TASK_PROPERTIES		8
#define RQ_PROJECT_OPERATION	9
#define RQ_TRANSFER_OPERATION	10
#define RQ_TASK_OPERATION		11
#define RQ_FILTER_GET			12
#define RQ_FILTER_SET			13
#define RQ_SORT					14
#define RQ_FONT					15

#define	RQ_GRAPH_GETDATA		100
#define	RQ_GRAPH_COMPUTER		101
#define RQ_GRAPH_COMPUTER_SELECT 102
#define RQ_GRAPH_PROJECT		103
#define RQ_GRAPH_PROJECT_SELECT	104
#define RQ_GRAPH_PROJECT_SINGLE	105
#define RQ_GRAPH_SET_TYPE		106

#define	RQ_COMPUTER_DATA		400
#define	RQ_COMPUTER_RECONNECT	401

#define CLOUD_SELECT_SINGLE		0
#define CLOUD_SELECT_MULTIPLE	1
#define CLOUD_SELECT_FILTER		2

#define CLOUD_STATUS_LOGIN			0
#define CLOUD_STATUS_LOGIN_FAILED	1
#define CLOUD_STATUS_DATA_READ		3
#define CLOUD_STATUS_DATA_WRITE		4
#define CLOUD_STATUS_FINAL_TIMEOUT	5
#define CLOUD_STATUS_QUIT			6


#define STATUS_WRITE_DATA_PROCESSED	-1
#define STATUS_WRITE_DATA_TAB_NULL -2
#define STATUS_WRITE_DATA_OK 1

extern time_t	g_tLastCloudActivety;

class CRemoteCombinedDoc;
class CThreadCloudServer;

extern CThreadCloudServer	*g_pThreadCloudServer;
extern bool		g_bThreadCloudServerIsRunning;
extern bool		g_bCloudServerAwake;
extern bool		g_bCloudServerRequestData;
// keep the name webserverproperties.
extern char		*g_pcWebServerProperties;
extern char		*g_pcCloudServerHtml;
extern DWORD	g_tCloudServerHtml;
//extern char		*g_pcWebServerHtmlTab[7];
//extern int		g_iCloudServerHtmlTabSend[7];
extern int		g_iCloudServerHtmlTabProcessed[7];
extern DWORD	g_tCloudServerHtmlTabSend[7];
//extern char		g_pcStatusToSend[128];
//extern int		g_iWebServerLoginDelay;

// CThreadWebServer
class CThreadCloudConnect;
//class CHtmlConnect;
class CXml;
//class CBase64;
class CThreadCloudServer : public CWinThread
{
	DECLARE_DYNCREATE(CThreadCloudServer)

protected:
	CThreadCloudServer();           // protected constructor used by dynamic creation
	virtual ~CThreadCloudServer();
	int		ReadData(CString *psData);

	void	SelectRows();
	bool	ReadData(CString sCommand, CString *psData);
	int		WriteData(int iClientRequest, CString *psData = NULL);
	void	GetComputerTree(std::string *psData);
	CString	ExtractXmlTag(char **pcFind);
	CView	*pViewGetView(CDocument *pDoc, CRuntimeClass *pClass);
	CRemoteCombinedDoc	*pGetDoc();
	void	CheckRightTab();

//	CString	Login();
	CString GetErrorText(CString sResponce);
//	CString	PasswordScramble(CString sPassword);
//	CString PasswordDeScramble(CString sPasswordE);

	CString	UploadId(CString *psData);
	void	Logging(CString sTxt, int iMode, bool bAlways = false);

	bool	m_bExit;
//	bool	m_bServerIsRunning;
	CXml	*m_pXml;
//	CBase64	*m_pBase64;

	std::deque<int>		m_lListSelected;

	CString	m_sLastTime;

	CThreadCloudConnect *m_pThreadCloudConnect;


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
//
	bool	m_bActive;
	bool	m_bLoggedIn;

	int		m_iCloudServerHtmlCurrentTab;
//	bool	m_bHandshakeDetected;
//	bool	m_bHighSpeedMode;
//	bool	m_bPasswordError;
//	int		m_iPasswordError;
//	int		m_iLowSpeedRetry;

	int		m_iWuTotal;

	bool	m_bWebServerSelectMultiple;

	int		m_iIdClient;
	int		m_iIdClientLast;
	int		m_iRequest;
	int		m_iRequestTimeout;

	bool	m_bLogDebugCloud;

	HWND	m_hWnd;

	DECLARE_MESSAGE_MAP()
};



#pragma once

class CHtmlConnect;
class CXml;
class CBase64;

// CThreadCloudConnect

class CThreadCloudConnect : public CWinThread
{
	DECLARE_DYNCREATE(CThreadCloudConnect)

protected:
	CThreadCloudConnect();           // protected constructor used by dynamic creation
	virtual ~CThreadCloudConnect();

	CString	Login();
	CString LoginActual();
	bool	CheckActivity();
	bool	ReadData(CString *psData);
	bool	WriteData(int iClientRequest, CString *psData = NULL);
	CString	PasswordScramble(CString sPassword);
	CString PasswordDeScramble(CString sPasswordE);
	CString ExtractXmlTag(char **pcFind);
	CString ExtractXmlPart(CString *psTxt, CString sTag);
	CString GetErrorText(CString sResponce);
	void	Logging(CString sTxt, int iMode, bool bAlways = false);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	bool	m_bExit;
	bool	m_bHighSpeedMode;
	int		m_iSlowSpeedDelay;
	int		m_iLoggingRetry;
	int		m_iLoginTimeout;
	bool	m_bPasswordError;
	int		m_iPasswordError;

	DWORD	m_dwThread;

//	CString m_sServer;
	CString m_sEmail;
	CString m_sPassword;
	bool	m_bReconnect;
	CString	m_sLoginID;
	bool	m_bLoggedIn;
	int		m_iTimeout;

	HWND			m_hWnd;

	CBase64			*m_pBase64;
	CXml			*m_pXml;
	CHtmlConnect	*m_pHtmlConnect;

	CString			m_sLastTime;
	int				m_iIdClient;

	bool			m_bLogDebugCloud;

	DECLARE_MESSAGE_MAP()
};



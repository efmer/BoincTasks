// ThreadCloudConnect.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgLogging.h"
#include "Base64.h"
#include "HtmlConnect.h"
//#include "Xml.h"
#include "DlgLogging.h"

#include "RemoteCombinedDoc.h"

#include "ThreadCloudConnect.h"
#include "ThreadCloudServer.h"
#include "ThreadCloudConnect.h"


// CThreadCloudConnect

IMPLEMENT_DYNCREATE(CThreadCloudConnect, CWinThread)

CThreadCloudConnect::CThreadCloudConnect()
{
	m_bExit = false;
	m_bHighSpeedMode = false;
	m_iSlowSpeedDelay = 0;
	m_dwThread = 0;
	m_sEmail = "";
	m_sPassword = "";
	m_bReconnect = false;
	m_iLoggingRetry = 10000000;
	m_iLoginTimeout = 0;
	m_sLoginID = "";
	m_bLoggedIn = false;
	m_iTimeout = 0;
	m_bPasswordError = true;
	m_iPasswordError = 0;
	m_hWnd = NULL;
	m_pBase64 = new CBase64;
	m_pHtmlConnect = new CHtmlConnect;
	m_bLogDebugCloud = theApp.m_pDlgLogging->m_bLogDebugCloud;

//	m_bHandshakeDetected = false;
	m_sLastTime = "null";
	m_iIdClient = -1;
}

CThreadCloudConnect::~CThreadCloudConnect()
{
	delete m_pHtmlConnect;
	delete m_pBase64;
}

BOOL CThreadCloudConnect::InitInstance()
{
	MSG		msg;
	time_t	tSytemTime;
	int		iClientRequest, iPort;
	CString sFormat, *psServer, *psEmail, *psPassword, *psStatus, *psBtrq;

	while(!m_bExit)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			switch (msg.message)
			{
				case UWM_MSG_THREAD_QUIT:
					::PostThreadMessage(m_dwThread, UWM_MSG_THREAD_CLOUD_STATUS, CLOUD_STATUS_QUIT, 0);
					m_bExit = true;
				break;
				case UWM_MSG_THREAD_WEB_START:
					m_hWnd		= (HWND) msg.wParam;
					m_dwThread	= (int) msg.lParam;
					m_sLoginID = "";
					m_bLoggedIn = false;
				break;
				case UWM_MSG_THREAD_WEB_SERVER:
					psServer = (CString *) msg.wParam;
					iPort = (int) msg.lParam;
					m_pHtmlConnect->SetServer(psServer,iPort);	
					delete psServer;
				break;
				case UWM_MSG_THREAD_WEB_EMAIL:
					psEmail = (CString *) msg.wParam;
					psPassword = (CString *) msg.lParam;
					if (*psEmail != m_sEmail) m_bReconnect = true;
					if (*psPassword != m_sPassword) m_bReconnect = true;
					m_sEmail = *psEmail;
					m_sPassword = *psPassword;
					delete psEmail;
					delete psPassword;
					m_sLoginID  ="";
					m_bLoggedIn = false;
					psStatus = new CString;
					*psStatus = "Logged out.";
					m_bPasswordError = false;
					Logging(*psStatus, LOGGING_DEBUG);
					::PostMessage(m_hWnd, UWM_MSG_THREAD_WEB_STATUS_TEXT, (WPARAM) psStatus, 0);
				break;
				case UWM_MSG_THREAD_CLOUD_WRITE:
					psBtrq = (CString *) msg.lParam;
					iClientRequest = (int) msg.wParam;
					WriteData(iClientRequest, psBtrq);
				break;
			}
		}

		if (!m_bLoggedIn)
		{
			if (!m_bPasswordError)
			{
				if (m_iLoggingRetry++ > 300)	// every 30 seconds
				{
					m_iLoggingRetry = 0;
					Logging("Logging in ", LOGGING_DEBUG);
					CString sResult = Login();
					CString *psStatus = new CString;
					if (sResult != "<OK>")
					{
						*psStatus = "Logging in failed: " + sResult;
 						if (sResult.Find("Password") >= 0)
						{
							m_bPasswordError = true;
							m_iPasswordError = 0;
						}
						if (m_iLoginTimeout > 0)
						{
							CString sFormat;
							sFormat.Format("Cloud timeout: %d", m_iLoginTimeout);
							Logging(sFormat, LOGGING_DEBUG);
						}
						if (m_iLoginTimeout> 4)
						{
							::PostThreadMessage(m_dwThread, UWM_MSG_THREAD_CLOUD_STATUS, CLOUD_STATUS_FINAL_TIMEOUT, 0);
							m_iLoginTimeout = 0; // thread should shut down.
							m_bExit = true;	// always shut down
						}

						::PostThreadMessage(m_dwThread, UWM_MSG_THREAD_CLOUD_STATUS, CLOUD_STATUS_LOGIN_FAILED, 0);
					}
					else
					{
						m_bPasswordError = false;
						*psStatus = "Logged in successfully.";
						::PostThreadMessage(m_dwThread, UWM_MSG_THREAD_CLOUD_STATUS, CLOUD_STATUS_LOGIN, 1);
					}
			
					Logging(*psStatus, LOGGING_DEBUG);
					int iStatus;
					if (m_bLoggedIn) iStatus = 1;
					else iStatus = 0;
					::PostMessage(m_hWnd, UWM_MSG_THREAD_WEB_STATUS_TEXT, (WPARAM)psStatus, iStatus);
				}
			}
			else
			{
				if (m_iPasswordError++ > 600)	// every 60 seconds
				{
					m_bPasswordError = false;
					m_iPasswordError = 0;
				}
			}
		}
		else	// logged in
		{
			if (m_bHighSpeedMode)
			{
				if (CheckActivity())
				{
					_time64( &tSytemTime);
					g_tLastCloudActivety = tSytemTime;
				}

				_time64( &tSytemTime);
				if (tSytemTime-240 > g_tLastCloudActivety)
				{
					Logging("Sleep mode", LOGGING_NORMAL , true);
					g_bCloudServerRequestData = false;
					m_bHighSpeedMode = false;
				//	m_bHandshakeDetected = false;
					g_bCloudServerAwake = false;
				}
			}
			else
			{
				if (m_iSlowSpeedDelay++ > 300)	// every 30 seconds
				{
					m_iSlowSpeedDelay = 0;
					if (CheckActivity())
					{
						Logging("Wakeup", LOGGING_NORMAL, true);
						_time64( &tSytemTime);
						g_tLastCloudActivety = tSytemTime;
					//	m_bHandshakeDetected = true;
						m_bHighSpeedMode = true;
						g_bCloudServerAwake = true;
						theApp.m_pMainWnd->ShowWindow(SW_HIDE);
					}
				}
			}
		}
		Sleep(100);
	}

	Logging("Cloud connect thread closed", LOGGING_DEBUG);
	Sleep(3000);
	return FALSE;
}

CString CThreadCloudConnect::Login()
{
	CString sResponce = "";
	CString sJson = "{}";	// not used anyway
	sResponce = m_pHtmlConnect->PostDataJson("serverrunning.php", &sJson, false);
	if (sResponce != "OK")
	{
		Logging("Cloud connect login: NOT OK server NOT running", LOGGING_DEBUG);
		return sResponce;
	}
	Logging("Cloud connect login: OK server found running", LOGGING_DEBUG);
	return LoginActual();
}

CString CThreadCloudConnect::LoginActual()
{
	char	*pcFound;
	CString sFound, sId, sToken, sVersion;

	CString sResponce = "";
	if (m_sEmail.GetLength() > 0)	// valid email and password.
	{
// valid: {"action":0,"email":"fred@efmer.eu","password":"Rudolf"}			
		CString sPassword, sPassWordOrg, sDebug;
		sPassword = PasswordScramble(m_sPassword);
		sPassWordOrg = PasswordDeScramble(sPassword);
		sVersion = CLOUD_VERSION;
//		sVersion.LoadString(IDS_PROGRAM_VERSION);
//		sVersion.Replace(".", "");
		sToken = "fj3OKJH48";
		CString sJson =	"{\"action\": 0, \"email\":\"" + m_sEmail + "\",\"password\":\"" + sPassword + "\",\"token\":\"" + sToken + "\",\"version\":\"" + sVersion + "\"}";
 		sResponce = m_pHtmlConnect->PostDataJson("login_bt.php", &sJson);

		sDebug = "Cloud connect LoginActual: Respose: " + sResponce;
		Logging(sDebug, LOGGING_DEBUG);

		pcFound = sResponce.GetBuffer();
		sFound = ExtractXmlTag(&pcFound);
		if (sFound == "ID")
		{
			sId = pcFound;
			if (sId.Find("</ID>") > 0)
			{
				sId.Replace("</ID>","");
				m_sLoginID = sId;
				m_bLoggedIn = true;
				return "<OK>";
			}
		}
		m_sLoginID = "";
		m_bLoggedIn = false;

		if (sResponce.GetLength() < 2)
		{
			m_iLoginTimeout++;
		}
		else
		{
			m_iLoginTimeout = 0;
		}

		if (sResponce.Find(EXEPTION_TEXT) >= 0)
		{
			m_iLoginTimeout++;
		}
		else
		{
			m_iLoginTimeout = 0;
		}

		return GetErrorText(sResponce);
	}
	m_sLoginID = "";
	m_bLoggedIn = false;
	m_iLoginTimeout++;
	return "Something is wrong, will timeout soon.";
}

bool CThreadCloudConnect::CheckActivity()
{
	CFile				file;
	CString				sHandshake, sTxt, sFormat, sHtml, sState, sComputer, sGroup, *psData;
	std::string			sData;

	if (!ReadData(&sHandshake))
	{
		return false;
	}

	m_iIdClient = -1;

	psData = new CString;
	*psData = sHandshake;
	::PostThreadMessage(m_dwThread, UWM_MSG_THREAD_CLOUD_STATUS, CLOUD_STATUS_DATA_READ, (LPARAM) psData);
	return true;
}

bool CThreadCloudConnect::ReadData(CString *psData)
{
	CString sResponce, sCommand;
	sCommand = "dummy";

	sResponce = m_pHtmlConnect->PostData("download_bt.php", &sCommand, m_sLoginID);
	
	if (sResponce.Find("<ERROR_INVALID_ID>") >= 0)
	{
		CString *psStatus = new CString;
		*psStatus = "The password is no longer valid.";
		Logging(*psStatus, LOGGING_DEBUG);
		::PostMessage(m_hWnd, UWM_MSG_THREAD_WEB_STATUS_TEXT, (WPARAM) psStatus, 0);
		return false;
	}

	if (sResponce.Find(EXEPTION_TEXT) >= 0)
	{
		if (m_iTimeout++ == 4)
		{
			CString *psStatus = new CString;
			*psStatus = "Logged out: Connection timeout.";
			Logging(*psStatus, LOGGING_DEBUG);
			::PostMessage(m_hWnd, UWM_MSG_THREAD_WEB_STATUS_TEXT, (WPARAM) psStatus, 0);
			Logging("Sleep mode", LOGGING_NORMAL , true);
		}

		if (m_iTimeout >= 4)
		{
			m_bLoggedIn = false;
			m_bPasswordError = false; 
			m_bHighSpeedMode = false;
			return false;
		}
	}

//	int iUp;
	CString sFound;

//	CString sText = "<up>1</up><time>1406472444</time><data><btrq><rqt><t>1</t><i>2</i></rqt></btrq></data>";
//	CString sData = ExtractXmlPart(&sText, "data");

	sFound = ExtractXmlPart(&sResponce, "up");
	if (sFound.GetLength() == 0) return false; 
	if (sFound == '0') return false;

	*psData = ExtractXmlPart(&sResponce, "data");
	if (psData->GetLength() == 0) return false; 

	m_iTimeout = 0;
	return true;
}

bool CThreadCloudConnect::WriteData(int iClientRequest, CString *psData)
{
	CString sResponce;
	CString sDummy = "";
	int		iPos;

	if (psData == NULL)
	{
		psData = &sDummy;
	}

	sResponce = m_pHtmlConnect->PostData("upload_bt.php", psData, m_sLoginID);
	iPos = sResponce.Find("<OK>");
	if (iPos < 0)
	{
		Sleep(100);
		sResponce = m_pHtmlConnect->PostData("upload_bt.php", psData, m_sLoginID);
		iPos = sResponce.Find("<OK>");
		if (iPos < 0)
		{
			return false;
		}
	}

	return true;
}

CString CThreadCloudConnect::PasswordScramble(CString sPassword)
{
	int			i,iLen;
	char		cChar;
	std::string sPasswordE;
	CString	str;
	
	sPasswordE = m_pBase64->Encode((BYTE*) sPassword.GetBuffer(), sPassword.GetLength());	
	
	iLen = (int) sPasswordE.length();
	for (i=0;i<iLen-1;i=i+2)
	{
        cChar = sPasswordE[i];
        sPasswordE[i] = sPasswordE[i+1];
        sPasswordE[i+1] = cChar;
	}

	str = sPasswordE.c_str();
	return str;	
}

CString CThreadCloudConnect::PasswordDeScramble(CString sPasswordE)
{
	int			i, iLen;
	char		cChar;
	std::string	sToDecode, sDecoded;
	CString		str;	
	
	sToDecode = sPasswordE;

	iLen = (int) sToDecode.length();
	for (i=0;i<iLen-1;i=i+2)
	{
		cChar = sPasswordE[i];
        sToDecode[i] = sToDecode[i+1];
        sToDecode[i+1] = cChar;
	}
	
	sDecoded = m_pBase64->Decode(sToDecode);
	str = sDecoded.c_str();
	return str;	
}

CString CThreadCloudConnect::ExtractXmlTag(char **pcFind)
{
	char *pcFoundBegin, *pcFoundEnd;

	pcFoundBegin = (char *) strstr(*pcFind, "<");
	if (pcFoundBegin)
	{
		pcFoundBegin++;
		pcFoundEnd = (char *) strstr(pcFoundBegin, ">");
		if (pcFoundEnd)
		{
			*pcFind = pcFoundEnd + 1;
			int iLen = (int) (pcFoundEnd - pcFoundBegin);
			CString sFound;
			sFound = pcFoundBegin;
			sFound = sFound.Mid(0,iLen);
			return sFound;
		}
	}
	return NULL;
}

CString CThreadCloudConnect::ExtractXmlPart(CString *psTxt, CString sTag)
{
	CString sTagBegin, sTagEnd;
	int iPosBegin, iPosEnd, iStart, iLen;

	sTagBegin = "<" + sTag + ">";
	sTagEnd = "</" + sTag + ">";

	iPosBegin = psTxt->Find(sTagBegin);
	iPosEnd = psTxt->Find(sTagEnd);

	if ((iPosBegin >=0) && (iPosEnd >=0))
	{
		if (iPosEnd > iPosBegin)
		{
			iStart = iPosBegin + sTagBegin.GetLength();
			iLen = iPosEnd - iStart; 
			return psTxt->Mid(iStart, iLen) ;
		}
	}
	return "";
}

CString CThreadCloudConnect::GetErrorText(CString sResponce)
{
	if (sResponce.GetLength() < 2)
	{
		return "Empty string from server.";
	}

	if (sResponce.Find("404") >= 0)
	{
		return "Unable to connect to server (404 Not Found).";
	}
	if (sResponce.Find("<ERROR_PASSWORD>") >= 0)
	{
		return "Password error.";
	}
	if (sResponce.Find("<ERR_CHECK>") >= 0)
	{
		return "Check error";
	}
	if (sResponce.Find("<ERROR_NOT_FOUND>") >= 0)
	{
		return "No account found. Use the browser to create a new account.";
	}
	if (sResponce.Find("<STATE>0<STATE>") >= 0)
	{
		return "Account is NOT activated.";
	}
	if (sResponce.Find(EXEPTION_TEXT) >= 0)
	{
		sResponce.Replace(EXEPTION_TEXT, "");
		return "Connection timeout: " + sResponce;
	}
	if (sResponce.Find("<ERR_DB>") >= 0)
	{
		return "Database connection error.";
	}

 	return "Unknown error";
}


void  CThreadCloudConnect::Logging(CString sTxt, int iMode, bool bAlways)
{
	if (bAlways || m_bLogDebugCloud)
	{
		CString sFrom = CLOUDSERVER_LOG_NAME;
		CString sFormat;
		CLoggingMessage sLog;
		sFormat.Format(" (thread Cloud Connect: %d)", m_nThreadID);
		sFrom += sFormat;
		sLog.m_pFrom = sFrom.GetBuffer();
		sLog.m_pText = sTxt.GetBuffer();
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, iMode);
		sTxt.ReleaseBuffer();
		sFrom.ReleaseBuffer();
	}
}


int CThreadCloudConnect::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadCloudConnect, CWinThread)
END_MESSAGE_MAP()


// CThreadCloudConnect message handlers

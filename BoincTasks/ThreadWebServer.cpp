// ThreadWebServer.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include "BoincTasks.h"

#include "DlgLogging.h"
#include "DlgSettingsExpert.h"
#include "DlgSettingsGeneral.h"
#include "DlgSettingsMobile.h"
#include "WebServerProcess.h"

#include "ThreadWebServer.h"
#include "DlgBarComputer.h"
#include "ThreadWebServerConnection.h"


CThreadWebServer	*g_pThreadWebServer = NULL; 

bool	g_bThreadWebServerIsRunning = false;

bool	g_bWebServerRunning = false;
bool	g_bWebServerActive = false;
char	*g_pcWebServerHtmlTab[7];
int		g_iWebServerHtmlTabSend[7];
char	g_pcStatusToSend[128];
DWORD	g_tWebServerHtmlTabSend[7];

time_t	g_tLastMobileActivety = 0;
char	*g_pcWebServerProperties = NULL;
char	*g_pcWebServerHtml = NULL;
DWORD	g_tWebServerHtml = 0;
int		g_iWebServerLoginDelay = 0;

// CThreadWebServer

IMPLEMENT_DYNCREATE(CThreadWebServer, CWinThread)

CThreadWebServer::CThreadWebServer()
{
	g_bThreadWebServerIsRunning = true;
	m_pThreadWebServerConnection = NULL;

	m_bExit = false;
	m_bServerIsRunning = false;
	m_hThreadWebServer = NULL;
	for (int iCount = 0; iCount < 6; iCount++)
	{
//		delete g_pcWebServerHtmlTab[iCount];	no not here
		g_pcWebServerHtmlTab[iCount] = NULL;
		g_iWebServerHtmlTabSend[iCount] = FALSE;
		g_tWebServerHtmlTabSend[iCount] = 0;
	}
	m_bStart = false;
	m_iStartDelay = 0;
	m_iOs = -1;
}

CThreadWebServer::~CThreadWebServer()
{
}

BOOL CThreadWebServer::InitInstance()
{
	MSG		msg;
	CString	sPath, sFormat;
	time_t	tSytemTime;
	m_bThreadBusy = false;
	m_iPort = 80;
//	bool bThreadQuiting = false;

	if (theApp.m_pDlgSettingsGeneral != NULL) m_iOs = theApp.m_pDlgSettingsGeneral->m_iOs;

	while(!m_bExit)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				if (m_pThreadWebServerConnection)
				{
					m_pThreadWebServerConnection->PostThreadMessage(UWM_MSG_THREAD_QUIT,0,0);
					m_pThreadWebServerConnection->CloseSocket();
					m_bThreadBusy = true;
				}
				m_pThreadWebServerConnection = NULL;
				g_bWebServerRunning = false;
				g_bWebServerActive = false;
				m_bExit = true;
				m_bStart = false;
				m_iStartDelay = 0;
			}
			if (msg.message == UWM_MSG_THREAD_WEB_START)
			{
				m_iPort = (int) msg.lParam;
				if (m_iPort >= 80)
				{
					m_bStart = true;
					m_iStartDelay = 0;
				}
			}
			if (msg.message == UWM_MSG_THREAD_WEB_STOP)
			{
				if (m_pThreadWebServerConnection != NULL) 
				{
					if (m_pThreadWebServerConnection) m_pThreadWebServerConnection->PostThreadMessage(UWM_MSG_THREAD_QUIT,0, 0);	// ends up in WebServer2
					m_pThreadWebServerConnection = NULL;
					m_bThreadBusy = true;
				}

				g_bWebServerRunning = false;
				g_bWebServerActive = false;
				m_bStart = false;
				m_iStartDelay = 0;
			}
			if (msg.message == UWM_MSG_THREAD_QUIT_READY)
			{
				CLoggingMessage sLog;
				sLog.m_pFrom = WEBSERVER_LOG_NAME;
				sLog.m_pText = "Stopped";
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
				m_bThreadBusy = false;
			}

			if (msg.message == UWM_MSG_THREAD_WEB_HTML)
			{
				int iTab = (int) msg.lParam;

				if (g_pcWebServerHtmlTab[iTab] != NULL) delete g_pcWebServerHtmlTab[iTab];
				g_pcWebServerHtmlTab[iTab] = g_pcWebServerHtml;
				g_iWebServerHtmlTabSend[iTab] = FALSE;
				g_tWebServerHtmlTabSend[iTab] = g_tWebServerHtml;
				g_pcWebServerHtml = NULL;

			}

			if (msg.message == UWM_MSG_THREAD_WEB_STATUS)
			{
				CString *pStatus = (CString *) msg.lParam;
				strcpy_s(&g_pcStatusToSend[0], 128, pStatus->GetBuffer());
				g_pcStatusToSend[127] = 0;
				delete pStatus;
			}

		}
		Sleep(10);		// not more than 10, effects whole message loop
		if (g_iWebServerLoginDelay > 0)
		{
			g_iWebServerLoginDelay -= 2;
		}
		if (g_bWebServerActive)
		{
			_time64( &tSytemTime);
			if (tSytemTime-60 > g_tLastMobileActivety)
			{
				CLoggingMessage sLog;
				sLog.m_pFrom = WEBSERVER_LOG_NAME;
				sLog.m_pText = "Sleep mode";
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
				g_bWebServerActive = false;
			}
		}

		if (theApp.m_pDlgSettingsGeneral != NULL)
		{
			if (m_iOs != theApp.m_pDlgSettingsGeneral->m_iOs)
			{
				PostThreadMessage(UWM_MSG_THREAD_WEB_STOP,0,0);
				PostThreadMessage(UWM_MSG_THREAD_WEB_START, 0, m_iPort);
				m_iOs = theApp.m_pDlgSettingsGeneral->m_iOs;
			}
		}

		if (!m_bThreadBusy && m_bStart && m_pThreadWebServerConnection == NULL)
		{
			if (m_iStartDelay++ > 600)
			{
				char	cBuffer[64];
				CLoggingMessage sLog;
				sLog.m_pFrom = WEBSERVER_LOG_NAME;

				_snprintf_s(cBuffer,64,_TRUNCATE,"%d", m_iPort);
				sFormat = "Starting on port: ";
				sFormat += cBuffer;
				sLog.m_pText = sFormat.GetBuffer();
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
				sFormat.ReleaseBuffer();

				m_pThreadWebServerConnection = (CThreadWebServerConnection *) AfxBeginThread(RUNTIME_CLASS(CThreadWebServerConnection));
				m_pThreadWebServerConnection->PostThreadMessage(UWM_MSG_THREAD_WEB_ID, 0, (LPARAM) m_nThreadID);
				m_pThreadWebServerConnection->PostThreadMessage(UWM_MSG_THREAD_WEB_START, m_iOs, (LPARAM) m_iPort);
				m_bStart = false;
				m_iStartDelay = 0;
			}
		}
	}

	int iTimeOut = 0;
	while (m_bThreadBusy)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT_READY)
			{
				m_bThreadBusy = false;
			}
		}

		Sleep(1000);
		if (iTimeOut++ > WEBSERVER_TIMEOUT) break;
	}

	for (int iCount = 0; iCount < 6; iCount++)
	{
		if (g_pcWebServerHtmlTab[iCount]  != NULL)	delete g_pcWebServerHtmlTab[iCount];
		g_pcWebServerHtmlTab[iCount] = NULL; 
	}
	delete g_pcWebServerHtml;
	g_pcWebServerHtml = NULL;
	g_pThreadWebServer = NULL;

	return FALSE;
}

int CThreadWebServer::ExitInstance()
{
	g_bThreadWebServerIsRunning = false;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadWebServer, CWinThread)
END_MESSAGE_MAP()


// CThreadWebServer message handlers

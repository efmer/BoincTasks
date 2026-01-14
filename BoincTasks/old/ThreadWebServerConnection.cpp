// ThreadWebServerConnection.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "WebServer.h"
#include "WebServer2.h"
#include "WebServerProcess.h"
#include "ThreadWebServer.h"
#include "ThreadWebServerConnection.h"


// CThreadWebServerConnection

IMPLEMENT_DYNCREATE(CThreadWebServerConnection, CWinThread)

CThreadWebServerConnection::CThreadWebServerConnection()
{
	m_bExit = false;
	m_pWebServerProcess = new CWebServerProcess;
	m_pWebServer = new CWebServer2;
}

CThreadWebServerConnection::~CThreadWebServerConnection()
{
	delete m_pWebServer;
	delete m_pWebServerProcess;
}

BOOL CThreadWebServerConnection::InitInstance()
{
	MSG		msg;
	int		iThreadId = 0;

	while(!m_bExit)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
//			if (msg.message == UWM_MSG_THREAD_QUIT)		// this message is read in WebServer(2)

			if (msg.message == UWM_MSG_THREAD_WEB_ID)
			{
				iThreadId = (int) msg.lParam;
			}
			if (msg.message == UWM_MSG_THREAD_WEB_START)
			{
				char	cBuffer[64];
				int iPort, iOs;
				iPort = (int) msg.lParam;
				iOs = (int) msg.wParam;

				_snprintf_s(cBuffer,64,_TRUNCATE,"%d", iPort);

				m_pWebServerProcess->ResetPassword();
				m_pWebServer->Connect(m_pWebServerProcess, iPort);


			//	m_pWebServerProcess->Start(m_pWebServer, iOs, cBuffer, iPort);
				m_bExit = true;
			}
		}
		Sleep(100);
	}

	::PostThreadMessage(iThreadId, UWM_MSG_THREAD_QUIT_READY, 0, 0);

	return FALSE;
}

void CThreadWebServerConnection::CloseSocket()
{
	m_pWebServer->CloseSocket();
}

int CThreadWebServerConnection::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadWebServerConnection, CWinThread)
END_MESSAGE_MAP()


// CThreadWebServerConnection message handlers

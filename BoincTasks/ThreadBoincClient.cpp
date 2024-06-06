// ThreadBoincClient.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "BoincClientManager.h"
#include "DlgLogging.h"
#include "ThreadBoincClient.h"


// CThreadBoincClient

IMPLEMENT_DYNCREATE(CThreadBoincClient, CWinThread)

CThreadBoincClient::CThreadBoincClient()
{
}

CThreadBoincClient::~CThreadBoincClient()
{
}

BOOL CThreadBoincClient::InitInstance()
{
	bool	bExit;
	int		iThreadLocalHost;
	HWND	hWnd;
	MSG		msg;

	CBoincClientManager	cm;
	CLoggingMessage *psLog;

	hWnd = NULL;
	bExit = false;

	while(!bExit)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_STOP_BOINC_CLIENT)
			{
				iThreadLocalHost = (int) msg.lParam;
				psLog = new CLoggingMessage;
				psLog->m_pFrom = "Shut down BoincTasks";
				if (iThreadLocalHost > 0)
				{
					cm.Close(iThreadLocalHost, psLog);
				}
				delete psLog;
				bExit = true;
			}
			if (msg.message == UWM_MSG_THREAD_START_BOINC_CLIENT)
			{
				iThreadLocalHost = (int) msg.lParam;
				cm.StartupBoincCore(iThreadLocalHost);
				::PostThreadMessage(iThreadLocalHost, UWM_MSG_THREAD_RECONNECT_RPC, 0, 0);	// force a reconnect

				bExit = true;
			}
		}
		Sleep(100);
	}

	return FALSE;
}

int CThreadBoincClient::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadBoincClient, CWinThread)
END_MESSAGE_MAP()


// CThreadBoincClient message handlers

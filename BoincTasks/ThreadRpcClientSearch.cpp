// ThreadRpcClientSearch.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "RpcClient.h"
#include "ThreadRpcClient.h"
#include "ThreadRpcClientSearch.h"


// CThreadRpcClientSearch

IMPLEMENT_DYNCREATE(CThreadRpcClientSearch, CWinThread)

CThreadRpcClientSearch::CThreadRpcClientSearch()
{
	m_pRpcClient = new CRpcClient;
}

CThreadRpcClientSearch::~CThreadRpcClientSearch()
{
	delete m_pRpcClient;
}

BOOL CThreadRpcClientSearch::InitInstance()
{
	bool bConnected = false, bReceived;
	CString sMessage;
	int		iThread;
	int		iMsg;

	sMessage = "BT";
	iThread = 0;
	m_bExit = false;

	while (!m_bExit)
	{
		MSG msg;
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			iMsg = msg.message;
			if (iMsg > 0)
			{
				if (iMsg == UWM_MSG_THREAD_QUIT)
				{
					m_bExit = true;
				}

				if (iMsg == UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_THREAD)
				{
					char	cPort[64];
					char	*pcMessage = "dummy";
					int		iFound;

					iThread = (int) msg.wParam;
					m_pInfo = (CRpcInfoClientSearch *) msg.lParam;
					m_iPort = m_pInfo->m_iPort;
					iFound = 0;

					m_pRpcClient->Disconnect();
					m_pRpcClient->ClearAddress();
					_snprintf_s(cPort, 63,_TRUNCATE,"%d", m_iPort);

					if (m_pRpcClient->Connect((char *) m_pInfo->m_sIp.c_str(), cPort, m_pInfo->m_iTimeOut, false))
					{
						bReceived = m_pRpcClient->SendReceive(pcMessage);
						if (bReceived)
						{
							iFound = 1;
						}
					}
					m_pRpcClient->Disconnect();
					m_pRpcClient->ClearAddress();
					::PostThreadMessage(iThread, UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_READY, (WPARAM) m_pInfo, (LPARAM) iFound);
					m_bExit = true;	// always exit
				}
			}
		}
		Sleep (1);
	}

	m_pRpcClient->Disconnect();
	m_pRpcClient->Cleanup();

	Sleep(500);		// time needed for the threads to handle the post messages. Otherwise we get memory leaks
	return FALSE;
}
int CThreadRpcClientSearch::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadRpcClientSearch, CWinThread)
END_MESSAGE_MAP()


// CThreadRpcClientSearch message handlers

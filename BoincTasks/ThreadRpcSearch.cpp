// ThreadRpc.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadRpcSearch.h"
#include "ThreadRpcClient.h"
#include "StringCopyNew.h"

// CThreadRpc

IMPLEMENT_DYNCREATE(CThreadRpcSearch, CWinThread)

CThreadRpcSearch::CThreadRpcSearch()
{
	m_bExit = false;
	m_pRpc	 = new RPC_CLIENT;
	m_pState = new CC_STATE;
	m_pHi	 = new HOST_INFO;

	m_iConnectionStatus = RPC_STATUS_NONE;
}

CThreadRpcSearch::~CThreadRpcSearch()
{
	delete m_pState;
	m_pState = NULL;
	delete m_pRpc;
	m_pRpc = NULL;
	delete m_pHi;
	m_pHi = NULL;
}

BOOL CThreadRpcSearch::InitInstance()
{
	HWND hWnd;

	while(!m_bExit)
	{
		MSG msg;
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				hWnd = (HWND) msg.wParam;
				m_bExit = true;
			}

			if (msg.message == UWM_MSG_THREAD_CONNECT_RPC)
			{
				int iThreadId;
				CRpcInfoClientSearch *pInfo;

				iThreadId = (int) msg.wParam;
				pInfo = (CRpcInfoClientSearch *) msg.lParam;
				m_sPassword = pInfo->m_sPassWord;
				m_sHostName = pInfo->m_sIp;
				Connect();

				if (m_iConnectionStatus == RPC_STATUS_CONNECTED)
				{
					pInfo->m_sHostID = m_pHi->domain_name;
				}

				::PostThreadMessage(iThreadId,UWM_MSG_THREAD_CONNECT_RPC_READY, m_iConnectionStatus,0);
				m_bExit = true;	// close at once
			}
		}
		Sleep(10);
	}

//	if (m_sAHostName == "localhost") PostMessage(theApp.m_pMainFrame->m_hWnd,UWM_MSG_THREAD_ID,-1,0);	// localhost no longer valid
//
//
//	::PostMessage(hWnd,UWM_MSG_THREAD_QUIT_READY,this->m_nThreadID,0);

	return FALSE;
}

int CThreadRpcSearch::ExitInstance()
{
	return CWinThread::ExitInstance();
}

bool CThreadRpcSearch::Connect()
{
//	char	cBufferLogging[255];

//	char passwd_buf[1024+1], hostname_buf[256], *hostname;
//	char* passwd; //, *p;
    int retval; //, i;

//	passwd_buf[0] = 0;
//	hostname_buf[0] = 0;
//	passwd = passwd_buf;
//	hostname = hostname_buf;

	if (m_iPort <= 0) m_iPort = 31416;

//	if (strlen(m_pcHostName) > 0)
//	{
//		strcpy_s (hostname_buf,200, m_pcHostName);
//	}
//	else hostname = NULL;

//	if (strlen(m_pcPassword) > 0)
//	{
//		strcpy_s (passwd_buf,1024,m_sAPassword.GetBuffer());
//	}
//	else passwd = NULL;

    retval = m_pRpc->init(m_sHostName.c_str(), m_iPort);
    if (retval)
	{
		if (retval == ERR_READ)m_pRpc->close();
		m_iConnectionStatus = GetError(retval, RPC_STATUS_ERROR_CANNOT_CONNECT);
        return false;
    }

	if (m_sPassword.length() > 0)
	{
		retval = m_pRpc->authorize(m_sPassword.c_str());
		if (retval)
		{
			if (retval == ERR_READ)m_pRpc->close();
			m_iConnectionStatus = GetError(retval, RPC_STATUS_ERROR_AUTHORIZATION);
			return false;
		}
	}

	retval = retval = m_pRpc->get_state(*m_pState);
	if (retval)
	{
		if (retval == ERR_READ) m_pRpc->close();	
		m_iConnectionStatus = GetError(retval, RPC_STATUS_ERROR_READING_STATE);
		return false;
	}

	retval = m_pRpc->get_host_info(*m_pHi);
	if (retval)
	{
		if (retval == ERR_READ) m_pRpc->close();
		m_iConnectionStatus = GetError(retval, RPC_STATUS_ERROR_READING_HOSTID);
		return false;
	}

	m_iConnectionStatus = RPC_STATUS_CONNECTED;

	return true;
}

int CThreadRpcSearch::GetError(int retval, int iFallback)
{
		if (retval == ERR_READ)
		{
			return RPC_STATUS_ERROR_READ;
		}
		if (retval == ERR_AUTHENTICATOR)
		{
			return RPC_STATUS_ERROR_AUTHORIZATION;
		}
		return iFallback;
}


BEGIN_MESSAGE_MAP(CThreadRpcSearch, CWinThread)
//	ON_THREAD_MESSAGE (WM_MSG_THREAD_START_RPC_TASK, OnStartRpcTask) 
END_MESSAGE_MAP()


// CThreadRpc message handlers


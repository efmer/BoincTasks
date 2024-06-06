// Rpc.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "Rpc.h"


// CRpc

IMPLEMENT_DYNCREATE(CRpc, CWinThread)

CRpc::CRpc()
{
}

CRpc::~CRpc()
{
}

BOOL CRpc::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CRpc::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRpc, CWinThread)
END_MESSAGE_MAP()


// CRpc message handlers

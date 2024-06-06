// ThreadProcessTime.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadRpc.h"
#include "DlgThreadGraph.h"
#include "ThreadProcessTime.h"


// CThreadProcessTime

IMPLEMENT_DYNCREATE(CThreadProcessTime, CWinThread)

CThreadProcessTime::CThreadProcessTime()
{
}

CThreadProcessTime::~CThreadProcessTime()
{
}

BOOL CThreadProcessTime::InitInstance()
{
	bool	bExit;
	HWND	hWnd;
	MSG		msg;
	time_t	tSystemTime;
	time_t	tNextProcessTime;
	__int64	i64LastProcessTime;
	int		iProcessTimes;
	std::deque<float> lProcessTime;

	hWnd = NULL;
	tNextProcessTime = 0;
	i64LastProcessTime = 0;

	_time64(&tSystemTime);
	tNextProcessTime = tSystemTime + THREAD_TIME_RATE;

	iProcessTimes = 0;
	bExit = false;

	while(!bExit)
	{
		// calculate thread times
		if (iProcessTimes++ > 10)
		{
			iProcessTimes = 0;
			_time64(&tSystemTime);
			if (tSystemTime > tNextProcessTime)
			{
				FILETIME	ftKernelTime;
				FILETIME	ftUserTime;
				FILETIME	ftExit;
				FILETIME	ftCreation;
				GetProcessTimes(GetCurrentProcess(),  &ftCreation,  &ftExit,  &ftKernelTime,  &ftUserTime);	//100 nSec = 0.1 uSec per count
	
				ULARGE_INTEGER uli;
				ULONGLONG	llTemp;
				__int64		i64TimeKernel, i64TimeUser, i64Time;
				DWORD		dwTime;
				float		fPerc;

				uli.HighPart = ftKernelTime.dwHighDateTime;
				uli.LowPart = ftKernelTime.dwLowDateTime;
				llTemp = uli.QuadPart;
				i64TimeKernel = (__int64) (llTemp / 10000);		// in mSec

				uli.HighPart = ftUserTime.dwHighDateTime;
				uli.LowPart = ftUserTime.dwLowDateTime;
				llTemp = uli.QuadPart;
				i64TimeUser = (__int64) (llTemp / 10000);		// in mSec
				i64Time = i64TimeKernel + i64TimeUser;

				dwTime = (DWORD) (i64Time - i64LastProcessTime);
				i64LastProcessTime = i64Time;

				fPerc = (float) dwTime;
				fPerc*= 100;
				fPerc/= THREAD_TIME_RATE*1000;
				lProcessTime.push_back(fPerc);

				tNextProcessTime += THREAD_TIME_RATE;
			}
		}
		// calculate thread times

		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				hWnd = (HWND) msg.wParam;
				bExit = true;
			}
			if (msg.message == UWM_MSG_THREAD_START_RPC_THREAD_TIMING)
			{
				hWnd = (HWND) msg.wParam;

				std::deque<float> *plThreadTime;
				plThreadTime = (std::deque<float> *) msg.lParam;

				for (UINT iPos = 0; iPos < lProcessTime.size(); iPos++)
				{
					plThreadTime->push_back(lProcessTime.at(iPos));
				}
				lProcessTime.clear();
				
				CRpcThreadReturn *pThreadReturn  = new CRpcThreadReturn;
				pThreadReturn->m_pcHostName = BOINCTASKS_PROCESS_TIMING_NAME; 
				pThreadReturn->m_pcComputerId= BOINCTASKS_PROCESS_TIMING_NAME;
				pThreadReturn->m_iThreadId = this->m_nThreadID;
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_THREAD_TIMING, (WPARAM) pThreadReturn, (LPARAM) 0);
			}

		}
		Sleep(4);
	}

	return FALSE;
}

int CThreadProcessTime::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcessTime, CWinThread)
END_MESSAGE_MAP()


// CThreadProcessTime message handlers

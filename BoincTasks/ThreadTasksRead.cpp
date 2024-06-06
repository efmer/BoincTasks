// ThreadRead.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadTasksRead.h"
#include "dlgprogress.h"
#include "webread.h"
#include "TasksDoc.h"
#include "ThreadStatusAnimation.h"
#include "mainfrm.h"


// CThreadTasksRead

IMPLEMENT_DYNCREATE(CThreadTasksRead, CWinThread)

CThreadTasksRead::CThreadTasksRead()
{
	m_bStart = false;
	m_bExit	 = false;
}

CThreadTasksRead::~CThreadTasksRead()
{
}

BOOL CThreadTasksRead::InitInstance()
{
	int iPage = 0, iRead = 0;
	CWebRead webRead;

/*
//	CBoincTasksDoc* pDoc = GetDocument()

	CString sHostId = L"4510067";			// lt
//	CString sHostId = L"4476623";			// fred giga vista


	while (theApp.m_pMainWnd == NULL)
	{
		Sleep(100);
	}
	theApp.m_pThreadTasksAnimation->SetText(L"Initializing...");

	while (!m_bExit)
	{
		while (!m_bStart)
		{	
			Sleep(100);
		}	

		theApp.m_pThreadTasksAnimation->SetText(L"Connecting...");
		while (webRead.ReadPage(sHostId,iPage,&iRead, m_pWu))
		{
			CString sTxt;
			sTxt.Format(L"Reading Tasks (computer: %s) : %d",sHostId, iRead);
			theApp.m_pThreadTasksAnimation->SetText(sTxt);
			iPage++;
		}

		theApp.m_pThreadTasksAnimation->SetText(L"");

		sHostId = L"Host ID: " + sHostId;
		m_pDoc->ThreadStopped(sHostId);
		m_bStart = false;

		theApp.m_pMainFrame->SetMessageText(L"");

		if (m_pDoc != NULL)	m_pDoc->ResultsReady();

//		m_pDoc->>SetTitle(sHostId);
	}
	*/
	return TRUE;
}

int CThreadTasksRead::Start(CTasksDoc *pDoc, CList <CWorkUnit*,CWorkUnit*> *pWu)
{
	m_pDoc = pDoc;
	m_pWu = pWu;
	m_bStart = true;
	return 0;
}


int CThreadTasksRead::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadTasksRead, CWinThread)
END_MESSAGE_MAP()


// CThreadTasksRead message handlers

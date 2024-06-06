// BoincTasksDoc.cpp : implementation of the CTasksCoc class
//

#include "stdafx.h"
#include "BoincTasks.h"

#include "TasksDoc.h"
#include "ListViewEx.h"   // base class for BoincTasksView
#include "TasksView.h"

#include "dlgprogress.h"
#include "webread.h"
#include "threadScheduler.h"

#include "workunit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTasksDoc

IMPLEMENT_DYNCREATE(CTasksDoc, CDocument)

BEGIN_MESSAGE_MAP(CTasksDoc, CDocument)
END_MESSAGE_MAP()


// CTasksCoc construction/destruction

CTasksDoc::CTasksDoc()
{
	// TODO: add one-time construction code here
	m_bThreadRunning = false;
}

CTasksDoc::~CTasksDoc()
{
	POSITION	pos;
	int			iNrWu;
	CWorkUnit	*pWu;

	// remove the whole workunit list made with new
	pos = m_lWorkUnits.GetHeadPosition();
	iNrWu = (int) m_lWorkUnits.GetCount();

	for (int iCount = 0; iCount < iNrWu; iCount++)
	{
		pWu = m_lWorkUnits.GetNext(pos);
		delete pWu;
	}
	m_lWorkUnits.RemoveAll();
}

BOOL CTasksDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

//	ThreadStart();
	this->SetTitle("Tasks");

	return TRUE;
}

BOOL CTasksDoc::OnOpenDocument( LPCTSTR lpszPathName)
{
	m_sId = lpszPathName;
	int iPos = m_sId.Find(",",0);
	if (iPos >=0)
	{
		m_sId = m_sId.Mid(0,iPos);
		return CDocument::OnNewDocument();
	}
	return FALSE;
}

 void CTasksDoc::OnCloseDocument()
 {
	// do not close when the thread is still busy
	if (m_bThreadRunning)
	{
		AfxMessageBox("Error: Unable to close while busy!");
		return;
	}

	CDocument::OnCloseDocument();
 }


void CTasksDoc::ReadTasks(int iNrComputersChecked, int iListSelectedComputers[])
{
	m_bThreadRunning = true;
	theApp.m_pThreadScheduler->StartTasks(this, &m_lWorkUnits, m_sId);
}

void CTasksDoc::ThreadStart()
{
//	theApp.m_pThreadScheduler->StartTasks(this, &m_lWorkUnits);
//	m_bThreadRunning = true;
}

void CTasksDoc::ThreadStopped(CString sTxt)
{
//	m_bThreadRunning = false;
//
//	POSITION pos = GetFirstViewPosition();
//	while (pos != NULL)
//	{
//		CView* pView = GetNextView(pos);
//		pView->Invalidate(); 
//		pView->SetWindowText(sTxt);
//	}  
}

void CTasksDoc::ResultsReady(void)
{
	POSITION	pos = GetFirstViewPosition();

	while (pos != NULL)
	{
		CTasksView* pView = (CTasksView*) GetNextView(pos);
		pView->NewResults(); 
	}

	// now the results of the WU are in go look for the WU info

//	theApp.m_pThreadScheduler->StartTasksInfo(this, &m_lWorkUnits);

	m_bThreadRunning = false;
}

void CTasksDoc::ResultsInfoReady(void)
{
	POSITION	pos = GetFirstViewPosition();

	while (pos != NULL)
	{
		CTasksView* pView = (CTasksView*) GetNextView(pos);
		pView->UpdateResults(); 
	}
}

// CTasksCoc serialization

void CTasksDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTasksCoc diagnostics

#ifdef _DEBUG
void CTasksDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTasksDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTasksCoc commands

// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "BoincTasks.h"
#include "CombinedChildFrm.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CCombinedChildFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CCombinedChildFrame, CChildFrame)

	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()

END_MESSAGE_MAP()


// CChildFrame construction/destruction

CCombinedChildFrame::CCombinedChildFrame()
{
}

CCombinedChildFrame::~CCombinedChildFrame()
{
}


BOOL CCombinedChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
//	cs.hMenu = NULL;

	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CChildFrame::PreCreateWindow(cs) )
		return FALSE;

//	m_hMenuDefault = NULL;
//	m_hMenuShared = NULL;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CCombinedChildFrame::AssertValid() const
{
	CChildFrame::AssertValid();
}

void CCombinedChildFrame::Dump(CDumpContext& dc) const
{
	CChildFrame::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers


void CCombinedChildFrame::OnDestroy() 
{
	CChildFrame::OnDestroy();  
}

void CCombinedChildFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
//	int iId;

	CChildFrame::OnShowWindow(bShow, nStatus);			// eerst daarna placement
}



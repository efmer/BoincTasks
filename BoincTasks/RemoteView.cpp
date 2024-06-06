// RemoteView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "RemoteView.h"


// CRemoteView

IMPLEMENT_DYNCREATE(CRemoteView, CListView)

CRemoteView::CRemoteView()
{

}

CRemoteView::~CRemoteView()
{
}

BEGIN_MESSAGE_MAP(CRemoteView, CListView)
END_MESSAGE_MAP()


// CRemoteView diagnostics

#ifdef _DEBUG
void CRemoteView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRemoteView message handlers

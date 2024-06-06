// DlgTimer.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgTimer.h"


// CDlgTimer dialog

IMPLEMENT_DYNAMIC(CDlgTimer, CDialog)

CDlgTimer::CDlgTimer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTimer::IDD, pParent)
{

	m_pAttachedWnd = NULL;
}

CDlgTimer::~CDlgTimer()
{
}

BOOL CDlgTimer::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(UTIMER_DLG, 1000, 0);	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgTimer::AttachWnd(CWnd *pWnd)
{
	m_pAttachedWnd = pWnd;
}


void CDlgTimer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTimer, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgTimer message handlers

void CDlgTimer::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pAttachedWnd != NULL) 
	{
		::PostMessage(m_pAttachedWnd->m_hWnd,UWM_MSG_TIMER_DLG,0,0);
	}
}
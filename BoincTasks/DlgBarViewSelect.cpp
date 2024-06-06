// DlgBarViewSelect.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "MainFrm.h"
#include "ThreadRpc.h"
//#include "ComputerDoc.h"
#include "ListViewEx.h"
#include "TemplateRemoteProjectView.h"
#include "TemplateRemoteTransferView.h"
#include "TemplateRemoteTaskView.h"
#include "TemplateRemoteMessageView.h"
#include "TemplateRemoteHistoryView.h"
#include "TemplateRemoteComputerView.h"
#include "RemoteCombinedProjectView.h"
#include "RemoteCombinedTransferView.h"
#include "RemoteCombinedTaskView.h"
#include "RemoteCombinedMessageView.h"
#include "RemoteCombinedHistoryView.h"
#include "RemoteCombinedComputerView.h"
#include "RemoteCombinedNoticesView.h"

#include "DlgBarViewSelect.h"
#include "Translation.h"

// DlgBarViewSelect dialog

CDlgBarViewSelect::CDlgBarViewSelect()
{
	m_iOpenWindows = 0;
	m_iNoticesCount = 0;
	m_bEnable = false;
}

BOOL CDlgBarViewSelect::Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	BOOL bReturn = CDialogBar::Create(pParent, nIDTemplate, nStyle, nID);

//	if(bReturn)
//	{
//
//	}
	return bReturn;
//	return true;
}

CDlgBarViewSelect::~CDlgBarViewSelect()
{
}

void CDlgBarViewSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SELECT_VIEW, m_tab);
}


BEGIN_MESSAGE_MAP(CDlgBarViewSelect, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_WM_TIMER()
	ON_MESSAGE(UWM_MSG_TAB_VIEW_OPEN,OnBarViewOpen)
	ON_MESSAGE(UWM_MSG_TAB_VIEW_CLOSE,OnBarViewClose)
	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelChanged)
	ON_MESSAGE(UWM_MSG_TAB_VIEW_SET_TAB,OnBarViewSetTab)
	ON_MESSAGE(UWM_MSG_TAB_VIEW_NOTICES_COUNT,OnBarViewSetNoticedCount)

	ON_MESSAGE(UWM_MSG_KEY,OnKey)

//	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// DlgBarViewSelect message handlers

LRESULT CDlgBarViewSelect::OnInitDialog ( WPARAM wParam, LPARAM lParam)
{
	CRect	rect;
	int		iWidth;
	LONG bRet = (LONG) HandleInitDialog(wParam, lParam);

    if (!UpdateData(FALSE))
    {
		TRACE0("Warning: UpdateData failed during dialog init.\n");
    }

	m_imageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_VIEWICONS);
	m_imageList.Add(&cBmp, RGB(255,255,255));
	cBmp.DeleteObject();

	this->GetClientRect(&rect);

	iWidth = (rect.Width()-60)/7;
	m_tab.SetImageList(&m_imageList);

	int iWidthAdjust = rect.Width() - (iWidth * 7);

	m_tab.SetMinTabWidth(iWidth);	// 140

	m_tab.InsertItem(0,gszTranslation[PosBarViewSelectNotices],6);
	m_tab.InsertItem(0,gszTranslation[PosBarViewSelectHistory],5);	
	m_tab.InsertItem(0,gszTranslation[PosBarViewSelectMessages],4);
	m_tab.InsertItem(0,gszTranslation[PosBarViewSelectTransfers],3);
	m_tab.InsertItem(0,gszTranslation[PosBarViewSelectTasks],2);
	m_tab.InsertItem(0,gszTranslation[PosBarViewSelectProjects],1);	
	m_tab.InsertItem(0,gszTranslation[PosBarViewSelectComputers],0);

	m_tab.SetCurSel(1);

	SetTimer(UTIMER_DLG_BAR_VIEW, 1000, 0);			// interval 1 seconds

	CRect	rectTab;
	m_tab.GetClientRect(rectTab);

	rect.right -= iWidthAdjust;
	rect.right += 30;
	this->MoveWindow(rect);

    return bRet;

//	::IsolationAwareInitCommonControls();
}

void CDlgBarViewSelect::OnTimer(UINT_PTR nIDEvent)
{
	BOOL bEnable = FALSE;
	int  iSelected = -1;

	CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();
	if (pMainWnd == NULL) return;
	CMDIChildWnd* pChild = (CMDIChildWnd*)pMainWnd->MDIGetActive();
	if (pChild != NULL)
	{
		CView* pActiveView = pChild->GetActiveView();
		if (pActiveView == NULL) return;

		if (pActiveView->IsWindowVisible()) 
		{
			if (pActiveView->IsKindOf(RUNTIME_CLASS(CRemoteCombinedComputerView)))
			{
				bEnable = TRUE;
				iSelected = BAR_SELECTED_COMPUTERS;
			}
			if (pActiveView->IsKindOf(RUNTIME_CLASS(CRemoteCombinedProjectView)))
			{
				bEnable = TRUE;
				iSelected = BAR_SELECTED_PROJECTS;
			}
			if (pActiveView->IsKindOf(RUNTIME_CLASS(CRemoteCombinedTaskView)))
			{
				bEnable = TRUE;
				iSelected = BAR_SELECTED_TASKS;
			}
			if (pActiveView->IsKindOf(RUNTIME_CLASS(CRemoteCombinedTransferView)))
			{
				bEnable = TRUE;
				iSelected = BAR_SELECTED_TRANSFERS;
			}
			
			if (pActiveView->IsKindOf(RUNTIME_CLASS(CRemoteCombinedMessageView)))
			{
				bEnable = TRUE;
				iSelected = BAR_SELECTED_MESSAGES;
			}
			if (pActiveView->IsKindOf(RUNTIME_CLASS(CRemoteCombinedHistoryView)))
			{
				bEnable = TRUE;
				iSelected = BAR_SELECTED_HISTORY;
			}
			if (pActiveView->IsKindOf(RUNTIME_CLASS(CRemoteCombinedNoticesView)))
			{
				bEnable = TRUE;
				iSelected = BAR_SELECTED_NOTICES;
			}
		}
	}

	if (bEnable)
	{
		if (iSelected != m_tab.GetCurSel()) // out of sync
		{
			m_tab.SetCurSel(iSelected);
			m_tab.Invalidate(TRUE);	
		}
	}

	if (bEnable != m_tab.m_bEnable)
	{
		m_tab.EnableWindow(bEnable);
		m_tab.Invalidate(TRUE);
		m_tab.m_bEnable = bEnable;
	}
}

LRESULT CDlgBarViewSelect::OnSelChanged(WPARAM wParam, LPARAM lParam)
{
	CDocument *pDoc;
	CView *pView;
	CWnd *pWnd;
	POSITION posDoc, posView;

	pWnd = GetActiveWindow();

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	while (posDoc)
	{
		pDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);				// there is only one doc
		if (pDoc != NULL)
		{
			posView = pDoc->GetFirstViewPosition();
			while (posView)
			{
				pView = pDoc->GetNextView(posView);
				if (pView == NULL) break;
				pView->PostMessage(UWM_MSG_TAB_VIEW_SELECTED, wParam, lParam);				// only the view that is active will get it.
			}
		}
	}

	return 0;
}

LRESULT CDlgBarViewSelect::OnBarViewSetTab(WPARAM wParam, LPARAM lParam)
{
	m_tab.SetCurSel((int) wParam);
	m_tab.Invalidate(TRUE);
	return 0;
}

LRESULT CDlgBarViewSelect::OnBarViewSetNoticedCount(WPARAM wParam, LPARAM lParam)
{
	int iNoticesCount;
	CString sTxt;

	iNoticesCount = (int) wParam;

	if (m_iNoticesCount != iNoticesCount)
	{
		m_iNoticesCount = iNoticesCount;
		if (iNoticesCount > 0) sTxt.Format("%d %s", iNoticesCount, gszTranslation[PosBarViewSelectNotices]);
		else sTxt = gszTranslation[PosBarViewSelectNotices];

		TCITEM tabCtrlItem;
		tabCtrlItem.pszText = sTxt.GetBuffer();
		tabCtrlItem.mask = TCIF_TEXT;
		m_tab.SetItem(BAR_SELECTED_NOTICES, &tabCtrlItem);
		m_tab.Invalidate(TRUE);
	}
	return 0;
}

LRESULT CDlgBarViewSelect::OnBarViewOpen(WPARAM wParam, LPARAM lParam)
{
	m_iOpenWindows++;
	m_tab.Invalidate(TRUE);
	return 0;
}

LRESULT CDlgBarViewSelect::OnBarViewClose(WPARAM wParam, LPARAM lParam)
{
	m_iOpenWindows--;
	if (m_iOpenWindows < 0) m_iOpenWindows = 0;
	m_tab.Invalidate(TRUE);
	return 0;
}

LRESULT CDlgBarViewSelect::OnKey(WPARAM wParam, LPARAM lParam)
{
	int iKey, iPressed;

	iKey = (int) wParam;
	iPressed = (int) lParam;

	if (iKey == VK_TAB)
	{
		bool bControlKey = ((GetKeyState(VK_LCONTROL) &0x80) >0) || ((GetKeyState(VK_RCONTROL) &0x80) > 0);
		bool bShiftKey = ((GetKeyState(VK_LSHIFT)&0x80) >0) || ((GetKeyState(VK_RSHIFT) &0x80) >0);

		if (bControlKey)
		{
			int iCurSel;
			iCurSel = m_tab.GetCurSel();
			if (bShiftKey)	// back
			{
				iCurSel--;
				if (iCurSel < BAR_SELECTED_COMPUTERS)
				{
					iCurSel = BAR_SELECTED_NOTICES;
				}
			}
			else			// further
			{
				iCurSel++;
				if (iCurSel > BAR_SELECTED_NOTICES)
				{
					iCurSel = BAR_SELECTED_COMPUTERS;
				}
			}
			OnSelChanged(iCurSel,0);
		//	m_tab.SetCurSel(iCurSel);
		//	m_tab.Invalidate(TRUE);
		}

	}
	return 0;

}

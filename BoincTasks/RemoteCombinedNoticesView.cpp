// RemoteCombinedNoticesView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"
#include "RemoteCombinedDoc.h"
#include "TemplateRemoteTaskView.h"	
#include "RemoteCombinedTaskView.h"
#include "TemplateRemoteProjectView.h"	
#include "RemoteCombinedProjectView.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "RemoteCombinedNoticesView.h"
#include "TemplateRemoteComputerView.h"
#include "RemoteCombinedComputerView.h"
#include "TemplateRemoteTransferView.h"
#include "RemoteCombinedTransferView.h"
#include "TemplateRemoteMessageView.h"
#include "RemoteCombinedMessageView.h"
#include "DlgNotices.h" 
#include "DlgRulesLogging.h"
#include "MainFrm.h"
#include "DlgUpdate.h"
#include "Translation.h"


// CRemoteCombinedNoticesView

IMPLEMENT_DYNCREATE(CRemoteCombinedNoticesView, CHtmlView)

CRemoteCombinedNoticesView::CRemoteCombinedNoticesView()
{
	m_bDocumentIsClosing = false;
}

CRemoteCombinedNoticesView::~CRemoteCombinedNoticesView()
{

}

BOOL CRemoteCombinedNoticesView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// create the view window itself
	m_pCreateContext = pContext;
	if (!CView::Create(lpszClassName, lpszWindowName,
				dwStyle, rect, pParentWnd,  nID, pContext))
	{
		return FALSE;
	}

	// assure that control containment is on
	AfxEnableControlContainer();

	RECT rectClient;
	GetClientRect(&rectClient);

//	static const CLSID CLSID_MozillaBrowser= { 0x1339B54C, 0x3453, 0x11D2, { 0x93, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

 //   const CLSID CLSID_MozillaBrowser = { 0x1339B54C, 0x3453, 0x11D2, { 0x93, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

	CLSID currentBrowser;
	currentBrowser = CLSID_WebBrowser;

// CLSID_WebBrowser

	// create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
	if (!m_wndBrowser.CreateControl(currentBrowser, lpszWindowName,
				WS_VISIBLE | WS_CHILD, rectClient, this, AFX_IDW_PANE_FIRST))
	{
		DestroyWindow();
		return FALSE;
	}

	// cache the dispinterface
	LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		m_pBrowserApp = NULL;
		m_wndBrowser.DestroyWindow();
		DestroyWindow();
		return FALSE;
	}
	return TRUE;
}

void CRemoteCombinedNoticesView::OnInitialUpdate()
{
//	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL,(WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);
	CHtmlView::OnInitialUpdate();
}

void CRemoteCombinedNoticesView::InitialUpdate(CRemoteCombinedDoc* pDoc)
{
	m_pDoc = pDoc;
}

void CRemoteCombinedNoticesView::NavigateHtml(CString *psHtml)
{
	Navigate2(*psHtml);
}

void CRemoteCombinedNoticesView::SendLogging(char *pcFrom, char *pcMessage, int iDebug)
{
	CLoggingMessage *psLog;
	psLog = new CLoggingMessage;

	psLog->m_pFrom =  pcFrom; psLog->m_pText = pcMessage;
	theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, iDebug);

	delete psLog;
}

void CRemoteCombinedNoticesView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRemoteCombinedNoticesView, CHtmlView)
	ON_WM_SIZE()

	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelTabView)	

	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, OnComputerBarSelectionChanged)

	ON_MESSAGE (UWM_MSG_REMOTE_NOTICES_SWITCH, OnSwitch)
//	ON_MESSAGE (UWM_MSG_REMOTE_WINDOW_SHOWN, OnSwitch)

	ON_MESSAGE (UWM_MSG_GET_FLOATER_DATA, OnFloaterData)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_CPU, OnSetSnoozeCpu)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_GPU, OnSetSnoozeGpu)	

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistorySetWidth)

//	ON_MESSAGE (UWM_MSG_NOTICES_BT_UPDATE, OnBoincTasksVersion)	
//	ON_MESSAGE (UWM_MSG_NOTICES_BT_RULE, OnBoincTasksRule)	
	ON_MESSAGE (UWM_DLG_SEARCH_COMPUTER_READY, OnSearchComputerReady)

	ON_MESSAGE (UWM_MSG_NOTICES_REFRESH, OnRefresh)	
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_COMPUTERS_DETECT, &CRemoteCombinedNoticesView::OnComputersDetect)
END_MESSAGE_MAP()


// CRemoteCombinedNoticesView diagnostics

#ifdef _DEBUG
void CRemoteCombinedNoticesView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CRemoteCombinedNoticesView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CRemoteCombinedNoticesView message handlers

HRESULT CRemoteCombinedNoticesView::OnTranslateUrl( DWORD dwTranslate,  OLECHAR* pchURLIn,  OLECHAR** ppchURLOut )
{
	wchar_t	*pwChar;
	wchar_t *pwCollapse = L"__collapse__";
	wchar_t *pwDelete = L"__delete__";
	wchar_t *pwRules = L"__bt_rules__";
	wchar_t *pwUpdate = L"__update__";
	int		iNumber;

	if (m_bDocumentIsClosing) 	return S_FALSE;			// everything is closing down.

	pwChar = wcsstr(pchURLIn,pwRules);
	if (pwChar != NULL)
	{
		theApp.m_pDlgRulesLogging->PostMessage(UWM_MSG_SHOW_RULES_DIALOG,0,0);
	}

	pwChar = wcsstr(pchURLIn,pwUpdate);
	if (pwChar != NULL)
	{
		*ppchURLOut = (OLECHAR *) CoTaskMemAlloc(10); wcscpy_s(*ppchURLOut,3,L"xx");
		theApp.m_pDlgUpdate->PostMessage(UWM_MSG_NOTICES_BT_UPDATE,0,0);
		return S_OK ;
	}


	pwChar = wcsstr(pchURLIn,pwCollapse);
	if (pwChar != NULL)
	{
		pwChar += wcslen(pwCollapse);
		iNumber = _wtoi(pwChar);


		int bFound;
		bFound = (int) m_pDoc->m_pDlgNotices->SendMessage(UWM_MSG_NOTICES_SET_COLLAPSE_DELETE,0,iNumber);	// 0 = collapse
		if (bFound)
		{
			*ppchURLOut = (OLECHAR *) CoTaskMemAlloc(10);wcscpy_s(*ppchURLOut,3,L"xx");
			if (theApp.m_pDlgLogging->m_bLogDebugNotices)
			{
				CString sTxt;
				sTxt.Format("Collapsed Nr: %d, S_OK", iNumber);
				SendLogging("OnTranslateUrl", sTxt.GetBuffer(), LOGGING_DEBUG);
				sTxt.ReleaseBuffer();
			}
			return S_OK ;
		}
	}

	pwChar = wcsstr(pchURLIn,pwDelete);
	if (pwChar != NULL)
	{
		pwChar += wcslen(pwDelete);
		iNumber = _wtoi(pwChar);

		int bFound;
		bFound = (int) m_pDoc->m_pDlgNotices->SendMessage(UWM_MSG_NOTICES_SET_COLLAPSE_DELETE,1,iNumber);	// 1 = delete
		if (bFound)
		{
			*ppchURLOut = (OLECHAR *) CoTaskMemAlloc(10); wcscpy_s(*ppchURLOut,3,L"xx");
			if (theApp.m_pDlgLogging->m_bLogDebugNotices)
			{
				CString sTxt;
				sTxt.Format("Delete Nr: %d, S_OK", iNumber);
				SendLogging("OnTranslateUrl", sTxt.GetBuffer(), LOGGING_DEBUG);
				sTxt.ReleaseBuffer();
			}
			return S_OK ;
		}
	}

	pwChar = wcsstr(pchURLIn, L"notices");
	if (pwChar == NULL)
	{
		ShellExecuteW(NULL,L"open",pchURLIn,NULL,NULL,SW_SHOWNORMAL);
		*ppchURLOut = (OLECHAR *) CoTaskMemAlloc(10);wcscpy_s(*ppchURLOut,3,L"xx");
		if (theApp.m_pDlgLogging->m_bLogDebugNotices)
		{
			CStringA sTxt;
			sTxt = "Open, S_OK ";
			sTxt += (CStringA) pchURLIn;
			SendLogging("OnTranslateUrl", sTxt.GetBuffer(), LOGGING_DEBUG);
			sTxt.ReleaseBuffer();
		}
		return S_OK ;
	}

	if (theApp.m_pDlgLogging->m_bLogDebugNotices)
	{
		CStringA sTxt;
		sTxt = (CStringA) pchURLIn;
		SendLogging("OnTranslateUrl", sTxt.GetBuffer(), LOGGING_DEBUG);
		sTxt.ReleaseBuffer();
	}

	BYTE *pByte;
	CStringA sTxt;
	sTxt = "in Hex: ";
	pByte = (BYTE *) pchURLIn;
	for (int iCount = 0; iCount < 50; iCount++)
	{
		CStringA sFormat;
		sFormat.Format("%x",*pByte);
		pByte++;
		sTxt += sFormat;
		sTxt += ",";
	}
	if (theApp.m_pDlgLogging->m_bLogDebugNotices)
	{
		SendLogging("OnTranslateUrl", sTxt.GetBuffer(), LOGGING_DEBUG);
		sTxt.ReleaseBuffer();
	}

	return S_FALSE;
	// S_OK = translated
}

LRESULT CRemoteCombinedNoticesView::OnSwitch(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pDlgNotices->PostMessage(UWM_MSG_NOTICES_REFRESH,0,0);
	theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam)
{
	CString *psComputer;
	int		iThreadId, iReconnect;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	psComputer = (CString *) wParam;
	iReconnect = (int) lParam;

	if (psComputer->GetLength() > 0)
	{
		m_pDoc->GetRpcThreadId(psComputer, &iThreadId);
		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,0);
	}
	else
	{
		int iNrRpc = m_pDoc->GetRpcSelectedComputers();		
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);	
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,iReconnect);
		}
	}
	theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	m_pDoc->m_pDlgNotices->PostMessage(UWM_MSG_NOTICES_REFRESH,0,0);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnSelTabView(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	m_pDoc->OnShowView((int) wParam, (int) lParam);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnFloaterData(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_GET_FLOATER_DATA, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_CPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return -1;			// everything is closing down.
	return m_pDoc->m_pProjectView->PostMessage(UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, wParam, lParam);
}

LRESULT CRemoteCombinedNoticesView::OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedNoticesView::OnSearchComputerReady(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pComputerView->SendMessage(UWM_DLG_SEARCH_COMPUTER_READY,parm1,parm2);
	return 0;
}


LRESULT CRemoteCombinedNoticesView::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	CString *psHtml;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	psHtml = (CString *) wParam;

	if (lParam == 0)
	{
		Navigate2(*psHtml);
	}
	else
	{
		Refresh2(REFRESH_COMPLETELY);
	}
	return 0;
}

void CRemoteCombinedNoticesView::OnSize(UINT nType, int cx, int cy )
{
	theApp.m_pMainFrame->m_dlgBarComputer.PostMessage(UWM_MSG_NEW_SIZE, cx, cy);
	CHtmlView::OnSize(nType, cx, cy );
}

void CRemoteCombinedNoticesView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,1);
		break;
	}

	CHtmlView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRemoteCombinedNoticesView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,1);
		break;
	}

	CHtmlView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CRemoteCombinedNoticesView::OnComputersDetect()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SELECTED,BAR_SELECTED_COMPUTERS,0);

	m_pDoc->m_pComputerView->PostMessage(WM_COMMAND, ID_COMPUTERS_DETECT,0);
}
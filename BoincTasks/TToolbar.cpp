// TToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "TToolbar.h"
#include "Translation.h"

// CTToolbar

IMPLEMENT_DYNAMIC(CTToolBar, CToolBar)

CTToolBar::CTToolBar()
{

}

CTToolBar::~CTToolBar()
{
}


BEGIN_MESSAGE_MAP(CTToolBar, CToolBar)
END_MESSAGE_MAP()



// CTToolbar message handlers



BOOL CTToolBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    LPNMHDR pNMHDR = ((LPNMHDR)lParam);
    TOOLTIPTEXT *pTTT=(TOOLTIPTEXT *)pNMHDR;
    UINT t = TTN_NEEDTEXT;
    if(pNMHDR->code == t) 
    {  
        UINT nID = (UINT) pNMHDR->idFrom;
        if(!(pTTT->uFlags & TTF_IDISHWND))
        {
			pTTT->hinst = NULL;
			switch(nID)
			{
				case ID_SHOW_STATISTICS: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarGraphCredit];
				break;
				case ID_SHOW_STATISTICS_TASKS: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarGraphTasks];
				break;
				case ID_SHOW_TEMPERATURE: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarGraphTemperature];
				break;
				case ID_SHOW_GRAPH_TRANSFER: 
					pTTT->lpszText = gszTranslation[PosGroupMenuShowGraphTransfer];
				break;
				case ID_SHOW_GRAPH_DEADLINE: 
					pTTT->lpszText = gszTranslation[PosGroupMenuShowGraphDeadline];
				break;
				case ID_SHOW_HISTORYLONG: 
					pTTT->lpszText = gszTranslation[PosGroupMenuShowHistoryLong];
				break;
				case ID_TOOLBAR_SUSPEND: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsSuspend];
				break;
				case ID_TOOLBAR_RESUME: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsResume];
				break;
				case ID_TOOLBAR_ABORT: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsAbort];
				break;
				case ID_TOOLBAR_PROJECT_UPDATE: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsUpdateProject];
				break;
				case ID_TOOLBAR_PROJECT_SUSPEND: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsSuspendProject];
				break;
				case ID_TOOLBAR_PROJECT_RESUME: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsResumeProject];
				break;
				case ID_TOOLBAR_PROJECT_NONEW: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsNoNewWorkProject];
				break;
				case ID_TOOLBAR_PROJECT_ALLOWNEW: 
					pTTT->lpszText = gszTranslation[PosGroupToolbarOperationsAllowNewProject];
				break;

				case ID_TOOLBAR_ALLOW_NETWORK_PREF:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowNetworkPref];
				break;
				case ID_TOOLBAR_ALLOW_NETWORK_NEVER:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowNetworkNever];
				break;
				case ID_TOOLBAR_ALLOW_CPU_PREF:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowCpuPref];
				break;
				case ID_TOOLBAR_ALLOW_CPU_SNOOZE:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowCpuSnooze];
				break;
				case ID_TOOLBAR_ALLOW_CPU_NEVER:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowCpuNever];
				break;
				case ID_TOOLBAR_ALLOW_GPU_PREF:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowGpuPref];
				break;
				case ID_TOOLBAR_ALLOW_GPU_SNOOZE:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowGpuSnooze];
				break;
				case ID_TOOLBAR_ALLOW_GPU_NEVER:
					pTTT->lpszText = gszTranslation[PosGroupToolbarAllowGpuNever];
				break;

				default:
				return CToolBar::OnNotify(wParam, lParam, pResult);
			}
			return TRUE;
        }
    }
    return CToolBar::OnNotify(wParam, lParam, pResult);
}

//BOOL CTToolBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CWnd::OnNotify(wParam, lParam, pResult);
//}

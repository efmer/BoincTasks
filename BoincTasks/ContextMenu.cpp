// ContextMenu.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"   // base class
#include "threadrpc.h"
#include "RemoteCombinedDoc.h"
#include "DlgSettingsTasks.h"
#include "Selection.h"
#include "ContextMenu.h"
#include "Translation.h"
#include "Translate.h"

// CContextMenu

CContextMenu::CContextMenu()
{
	// set order for 1 = 1 
	for (int iCount = 0; iCount < 20; iCount++)
	{
		m_iColumnOrder[iCount] = iCount;
	}
	m_bThreadLocked = false;
}

CContextMenu::~CContextMenu()
{
}


// CContextMenu member functions

void CContextMenu::SetColumnOrder(int iOrderCount, int *pOrder)
{
	for (int iCount = 0; iCount < iOrderCount; iCount++)
	{
		m_iColumnOrder[iCount] = *pOrder;
		pOrder++;
	}
}

void CContextMenu::ContextMenuCombined(int iWhatContectMenu, CRemoteCombinedDoc *pDoc, CListCtrl *pListCtrl, CSelection *pSelection, CPoint point, CWnd * pWnd, int iMenuId)
{
	int iSelectedTaskItems;
	int iCount, iNrItems, iThreadId;
	CString	sTask;
	CPoint startPoint;

	// check if there are any selected items.
	if (pSelection->SelectionNumber() == 0) return;

	m_iWhatContectMenu = iWhatContectMenu;

	m_bFilter = false;
	startPoint = point;
	startPoint.y += 10;
	//we have to translate the mouse coordinates first:
	//these are relative to the window (or view) but the context menu
	//needs coordinates that are relative to the whole screen
	pWnd->ClientToScreen(&point);

	m_startPoint = startPoint;
	m_pWnd = pWnd;

	if (pDoc == NULL) return;

	iNrItems = pListCtrl->GetItemCount();

	iSelectedTaskItems = 0;
	for (iCount = 0; iCount < iNrItems; iCount++)
	{
		if (pSelection->SelectionSelected(pListCtrl, iCount))
		{
			if (iWhatContectMenu == CONTEXT_MENU_PROJECTS)
			{
				m_sComputer = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_COMPUTER]);
				m_sProject = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT_PROJECT]);
			}
			if (iWhatContectMenu == CONTEXT_MENU_TASKS)
			{
				m_sComputer = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_COMPUTER]);
				m_sProject = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_PROJECT]);
				sTask = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_NAME]);
				if (sTask.Find(gszTranslation[PosViewTasksFilterTasks]) >= 0) m_bFilter = true;
			}
			if (iWhatContectMenu == CONTEXT_MENU_TRANSFERS)
			{
				m_sComputer = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_TRANSFER_COMPUTER]);
				m_sProject = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_TRANSFER_PROJECT]);
			}
			if (iWhatContectMenu == CONTEXT_MENU_HISTORY)
			{
				m_sComputer = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_HISTORY_COMPUTER]);
				m_sProject = pListCtrl->GetItemText(iCount, m_iColumnOrder[COLUMN_HISTORY_PROJECT]);
			}

			iSelectedTaskItems++;
			break;
		}
	}

	if (iSelectedTaskItems == 0) return;

	if (m_sComputer.GetLength() == 0)
	{
		AfxMessageBox(gszTranslation[PosGroupDialogSettingsTasksMsgBoxExclude],MB_ICONEXCLAMATION);
		return;
	}


	bool bFound = pDoc->GetRpcThreadId(&m_sComputer, &iThreadId);
	if (!bFound)
	{
		return;
	}

	m_iMenuId = iMenuId;
	m_projects.clear();

	if (m_bThreadLocked)
	{
		return;
	}


//	if (iWhatContectMenu == CONTEXT_MENU_HISTORY)	// no www
//	{
//		m_bThreadLocked = true;	// not really a tread but the locked state is needed anyway.
//		Ready(1);
//		return;
//	}

	m_projectsDiskUsage.pProjects = &m_projects;
	m_projectsDiskUsage.pDiskUsage = NULL;
	::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_READ_PROJECT,(WPARAM) pWnd->m_hWnd, (LPARAM) &m_projectsDiskUsage);		// return is by OnReadyProject (UWM_MSG_THREAD_READY_RPC_READ_PROJECT) in the RemoteCombinedxxxView.cpp
	m_bThreadLocked = true;
}

void CContextMenu::Ready(int iStatus)
{
	int iCount;
	CMenu	*tracker, *translate, *pMenu; 
	CString sMenuTextLow;
	std::string *psMenuText;
	CTranslate	translateWWW;

	int iProjects, iMenuId;

	// delete m_pWaitCursor;

	if (iStatus < 0)
	{
		return;	// not connected
	}

	if (!m_bThreadLocked)
	{
		return;			// no request!
	}
	m_bThreadLocked = false;	// here because there are plenty of returns

//	//at first we'll load our menu	

	m_menu.DestroyMenu();
	CString sTxt;
	bool bForum;
	m_menu.LoadMenu(m_iMenuId); 
	PROJECT *pProject = NULL;
	int iCountAdd = 0;
	int iWwwOffset = 2;

	for (iCount = 0; iCount < URL_PROJECTS_MAX; iCount++)
	{
		m_psUrl[iCount] = NULL; 
		m_psMenu[iCount] = NULL;
	}

	iProjects = (int) m_projects.projects.size();
	for (iCount = 0; iCount < iProjects; iCount++)
	{
		if (m_sProject == m_projects.projects[iCount]->project_name.c_str())
		{
			pProject = m_projects.projects[iCount];
			break;
		}
	}
	if (pProject == NULL)
	{
		m_bThreadLocked = false;
		return;
	}

	// first insert the main url
	m_psUrl[0] = (char*)pProject->master_url.c_str();
	m_psMenu[0] = (char*)pProject->project_name.c_str();
	m_sForum = m_psUrl[0];
	m_sForum+= "forum_index.php";
	m_psUrl[1] = (char*) m_sForum.c_str();
	m_psMenu[1] = "Forum";

	int iSizeUrl = (int)pProject->gui_urls.size();
	if (iSizeUrl > URL_PROJECTS_MAX) iSizeUrl = URL_PROJECTS_MAX;
	iCountAdd = iWwwOffset;
	for (iCount = 0; iCount < iSizeUrl; iCount++)						// 0 = used by the general url 1 = forum;
	{
		psMenuText = &pProject->gui_urls[iCount].name;
		sMenuTextLow = psMenuText->c_str();
		sMenuTextLow = sMenuTextLow.MakeLower();
		bForum = false;
		if (sMenuTextLow.Find("forum") >= 0) bForum = true;
		if (sMenuTextLow.Find("message board") >= 0) bForum = true;
		if (bForum)
		{
			m_psUrl[1] = (char *) pProject->gui_urls[iCount].url.c_str();
			m_psMenu[1] = (char*)pProject->gui_urls[iCount].name.c_str();
		}
		else
		{
			m_psUrl[iCountAdd] = (char *) pProject->gui_urls[iCount].url.c_str();
			m_psMenu[iCountAdd] =(char *) psMenuText->c_str();
			iCountAdd++;
		}
	}

	pMenu = m_menu.GetSubMenu(0);
	int pos = FindMenuItem(pMenu, _T("WWW"));
	if (pos == -1) return;
	pMenu = pMenu->GetSubMenu(pos);
	if (pMenu == NULL) return;

	// item 0
	sTxt = m_psMenu[0];
	translateWWW.TranslateWWW(&sTxt);
	m_menu.ModifyMenu(ID_WWW_1, MF_STRING, ID_WWW_1, sTxt);

	for (iCount = 1; iCount < iCountAdd; iCount++)
	{
		switch (iCount)
		{
//		case 0:
//			iMenuId = ID_WWW_1;
//			break;
		case 1:
			iMenuId = ID_WWW_2;
			break;
		case 2:
			iMenuId = ID_WWW_3;
			break;
		case 3:
			iMenuId = ID_WWW_4;
			break;
		case 4:
			iMenuId = ID_WWW_5;
			break;
		case 5:
			iMenuId = ID_WWW_6;
			break;
		case 6:
			iMenuId = ID_WWW_7;
			break;
		case 7:
			iMenuId = ID_WWW_8;
			break;
		case 8:
			iMenuId = ID_WWW_9;
			break;
		case 9:
			iMenuId = ID_WWW_10;
			break;
		case 10:
			iMenuId = ID_WWW_11;
			break;
		case 11:
			iMenuId = ID_WWW_12;
			break;
		case 12:
			iMenuId = ID_WWW_13;
			break;
		case 13:
			iMenuId = ID_WWW_14;
			break;
		case 14:
			iMenuId = ID_WWW_15;
			break;
		case 15:
			iMenuId = ID_WWW_16;
			break;
		case 16:
			iMenuId = ID_WWW_17;
			break;
		case 17:
			iMenuId = ID_WWW_18;
			break;
		case 18:
			iMenuId = ID_WWW_19;
			break;
		case 19:
			iMenuId = ID_WWW_20;
			break;
		}
		sTxt = m_psMenu[iCount];
		translateWWW.TranslateWWW(&sTxt);

		pMenu->InsertMenu(iCount, MF_BYPOSITION, iMenuId, sTxt);
	}

//		m_menu.ModifyMenu(iMenuId, MF_STRING, iMenuId, sTxt);		


	// translations
	translate = m_menu.GetSubMenu(0);
	if (m_iWhatContectMenu == CONTEXT_MENU_TASKS)
	{
		translate->ModifyMenu(ID_TASK_SUSPEND,MF_STRING,ID_TASK_SUSPEND,gszTranslation[PosGroupPopUpTasksSuspend]);

		if (theApp.m_pDlgSettingsTasks->m_bCheckpoint)
		{
			translate->ModifyMenu(ID_TASK_SUSPEND_CP,MF_STRING,ID_TASK_SUSPEND_CP,gszTranslation[PosGroupPopUpTasksSuspendCp]);
		}
		else
		{
			translate->RemoveMenu(ID_TASK_SUSPEND_CP, MF_BYCOMMAND);
		}

//		translate->ModifyMenu(ID_TASK_SUSPEND_CP,MF_SEPARATOR,ID_TASK_SUSPEND_CP,"");			// hide suspend on checkpoint

		translate->ModifyMenu(ID_TASK_RESUME,MF_STRING,ID_TASK_RESUME,gszTranslation[PosGroupPopUpTasksResume]);
		translate->ModifyMenu(ID_TASK_ABORT,MF_STRING,ID_TASK_ABORT,gszTranslation[PosGroupPopUpTasksAbort]);
		translate->ModifyMenu(ID_TASK_PROPERTIES,MF_STRING,ID_TASK_PROPERTIES,gszTranslation[PosGroupPopUpTasksPoperties]);
		translate->ModifyMenu(ID_TASK_COPYALLTOCLIPBOARD,MF_STRING,ID_TASK_COPYALLTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopyAll]);
		translate->ModifyMenu(ID_TASK_COPYSELECTEDTOCLIPBOARD,MF_STRING,ID_TASK_COPYSELECTEDTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopySelected]);

		if (m_bFilter)
		{
			translate->ModifyMenu(ID_TASK_PROPERTIES,MF_SEPARATOR,ID_TASK_PROPERTIES,"");			// hide the properties
		}

		translate->ModifyMenu(ID_TASK_PROJECTUPDATE,MF_STRING,ID_TASK_PROJECTUPDATE,gszTranslation[PosGroupPopUpTasksProjectUpdate]);
		translate->ModifyMenu(ID_TASK_PROJECTSUSPEND,MF_STRING,ID_TASK_PROJECTSUSPEND,gszTranslation[PosGroupPopUpTasksProjectSuspend]);
		translate->ModifyMenu(ID_TASK_PROJECTRESUME,MF_STRING,ID_TASK_PROJECTRESUME,gszTranslation[PosGroupPopUpTasksProjectResume]);

		UINT iSubmenu = translate->GetMenuItemID(9);
		translate->ModifyMenu(9,MF_BYPOSITION,iSubmenu,gszTranslation[PosGroupPopUpTasksFilter]);
		translate->ModifyMenu(ID_FILTERON_READYTOREPORT,MF_STRING,ID_FILTERON_READYTOREPORT,gszTranslation[PosGroupPopUpTasksFilterReadyTR]);
		translate->ModifyMenu(ID_FILTERON_WAITINGTORUN,MF_STRING,ID_FILTERON_WAITINGTORUN,gszTranslation[PosGroupPopUpTasksFilterWaitingTR]);
		translate->ModifyMenu(ID_FILTER_READYTOSTART,MF_STRING,ID_FILTER_READYTOSTART,gszTranslation[PosGroupPopUpTasksFilterReadyTS]);
		translate->ModifyMenu(ID_FILTERON_UPLOAD,MF_STRING,ID_FILTERON_UPLOAD,gszTranslation[PosGroupPopUpTasksFilterUpload]);
		translate->ModifyMenu(ID_FILTERON_DOWNLOAD,MF_STRING,ID_FILTERON_DOWNLOAD,gszTranslation[PosGroupPopUpTasksFilterDownload]);
		translate->ModifyMenu(ID_FILTERON_COMPUTATIONERROR,MF_STRING,ID_FILTERON_COMPUTATIONERROR,gszTranslation[PosGroupPopUpTasksFilterComputation]);
		translate->ModifyMenu(ID_FILTERON_SUSPENDED,MF_STRING,ID_FILTERON_SUSPENDED,gszTranslation[PosGroupPopUpTasksFilterSuspended]);
		translate->ModifyMenu(ID_FILTERON_ABORTED, MF_STRING, ID_FILTERON_ABORTED, gszTranslation[PosGroupPopUpTasksFilterAborted]);
		translate->ModifyMenu(ID_FILTERON_RUNNING, MF_STRING, ID_FILTERON_RUNNING, gszTranslation[PosGroupPopUpTasksFilterRunning]);

		translate->ModifyMenu(ID_TASK_SHOWACTIVETASKS, MF_STRING, ID_TASK_SHOWACTIVETASKS, gszTranslation[PosGroupPopUpTasksShowActive]);
		translate->ModifyMenu(ID_TASK_SHOW_NON_CPU_INTENSIVE, MF_STRING, ID_TASK_SHOW_NON_CPU_INTENSIVE, gszTranslation[PosGroupPopUpTasksShowNonCpuIntensive]);
		translate->ModifyMenu(ID_TASK_SHOWCPU, MF_STRING, ID_TASK_SHOWCPU, gszTranslation[PosGroupPopUpTasksShowCpu]);
		translate->ModifyMenu(ID_TASK_SHOWGPU, MF_STRING, ID_TASK_SHOWGPU, gszTranslation[PosGroupPopUpTasksShowGpu]);

		translate->ModifyMenu(ID_TASK_SHOWGRAPHIC, MF_STRING, ID_TASK_SHOWGRAPHIC, gszTranslation[PosGroupPopUpTasksShowGraphic]);

		translate->ModifyMenu(ID_TASK_ADDRULE,MF_STRING,ID_TASK_ADDRULE,gszTranslation[PosGroupPopUpTasksAddRule]);
	}
	
	if (m_iWhatContectMenu == CONTEXT_MENU_PROJECTS)
	{
		translate->ModifyMenu(ID_PROJECTS_UPDATE,MF_STRING,ID_PROJECTS_UPDATE,gszTranslation[PosGroupPopUpProjectsUpdate]);
		translate->ModifyMenu(ID_PROJECTS_SUSPEND,MF_STRING,ID_PROJECTS_SUSPEND,gszTranslation[PosGroupPopUpProjectsSuspend]);
		translate->ModifyMenu(ID_PROJECTS_RESUME,MF_STRING,ID_PROJECTS_RESUME,gszTranslation[PosGroupPopUpProjectsResume]);
		translate->ModifyMenu(ID_PROJECTS_NOMOREWORK,MF_STRING,ID_PROJECTS_NOMOREWORK,gszTranslation[PosGroupPopUpProjectsNoNewTasks]);
		translate->ModifyMenu(ID_PROJECTS_ALLOWWORK,MF_STRING,ID_PROJECTS_ALLOWWORK,gszTranslation[PosGroupPopUpProjectsAllowNewTasks]);
		translate->ModifyMenu(ID_PROJECTS_PROPERTIES,MF_STRING,ID_PROJECTS_PROPERTIES,gszTranslation[PosGroupPopUpProjectsProperties]);

		UINT iSubmenu = translate->GetMenuItemID(7);
		translate->ModifyMenu(7,MF_BYPOSITION,iSubmenu,gszTranslation[PosGroupPopUpProjectsAddAddRemove]); //Add/remove

		translate->ModifyMenu(ID_PROJECTS_ADDNEWPROJECT,MF_STRING,ID_PROJECTS_ADDNEWPROJECT,gszTranslation[PosGroupPopUpProjectsAddAddNew]);
		translate->ModifyMenu(ID_PROJECTS_RESET,MF_STRING,ID_PROJECTS_RESET,gszTranslation[PosGroupPopUpProjectsAddReset]);
		translate->ModifyMenu(ID_PROJECTS_DETACH,MF_STRING,ID_PROJECTS_DETACH,gszTranslation[PosGroupPopUpProjectsAddDetach]);

		translate->ModifyMenu(ID_PROJECTS_COPYALLTOCLIPBOARD,MF_STRING,ID_PROJECTS_COPYALLTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopyAll]);
		translate->ModifyMenu(ID_PROJECTS_COPYSELECTEDTOCLIPBOARD,MF_STRING,ID_PROJECTS_COPYSELECTEDTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopySelected]);
	}

	if (m_iWhatContectMenu == CONTEXT_MENU_TRANSFERS)
	{
		translate->ModifyMenu(ID_TRANSFER_RETRY,MF_STRING,ID_TRANSFER_RETRY,gszTranslation[PosPopUpTransfersRetry]);
		translate->ModifyMenu(ID_TRANSFER_RETRYALL,MF_STRING,ID_TRANSFER_RETRYALL,gszTranslation[PosPopUpTransfersRetryAll]);
		translate->ModifyMenu(ID_TRANSFER_ABORT,MF_STRING,ID_TRANSFER_ABORT,gszTranslation[PosPopUpTransfersAbort]);

		translate->ModifyMenu(ID_TRANSFER_COPYALLTOCLIPBOARD,MF_STRING,ID_TRANSFER_COPYALLTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopyAll]);
		translate->ModifyMenu(ID_TRANSFER_COPYSELECTEDTOCLIPBOARD,MF_STRING,ID_TRANSFER_COPYSELECTEDTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopySelected]);
	}

	if (m_iWhatContectMenu == CONTEXT_MENU_HISTORY)
	{
		translate->ModifyMenu(ID_SHOW_HISTORYLONG,MF_STRING,ID_SHOW_HISTORYLONG,gszTranslation[PosGroupMenuShowHistoryLong]);
		translate->ModifyMenu(ID_HISTORY_COPYALLTOCLIPBOARD,MF_STRING,ID_HISTORY_COPYALLTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopyAll]);
		translate->ModifyMenu(ID_HISTORY_COPYSELECTEDTOCLIPBOARD,MF_STRING,ID_HISTORY_COPYSELECTEDTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopySelected]);
	}

	//tracker now points to IDR_CONTEXT's first submenu 
	//GetSubMenu(0) -&gt; returns the first submenu,
	// GetSubMenu(1)-&gt; second one etc..
	tracker = m_menu.GetSubMenu(0); 
	//show the context menu
	tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, m_startPoint.x , m_startPoint.y , AfxGetMainWnd()); 

//	m_bThreadLocked = false;
}

int CContextMenu::FindMenuItem(CMenu* Menu, LPCTSTR MenuString)
{
	ASSERT(Menu);
	ASSERT(::IsMenu(Menu->GetSafeHmenu()));
	int count = Menu->GetMenuItemCount();
	for (int i = 0; i < count; i++)
	{
		CString str;
		if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&
			(strcmp(str, MenuString) == 0))
			return i;
	}
	return -1;
}

void CContextMenu::WWW(int iNr)
{
	int iUrlLen;

	if (iNr >= URL_PROJECTS_MAX) return;
	if (iNr < 0) return;
	if (m_psUrl[iNr] == NULL) return;

	iUrlLen = (int) strlen(m_psUrl[iNr]);
	if (iUrlLen <= 0) return;

	ShellExecute(NULL,_T("open"), m_psUrl[iNr],NULL,NULL,SW_SHOWNORMAL);
}
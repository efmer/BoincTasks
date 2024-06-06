// DlgBarComputer.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include <afxpriv.h>
#include "MainFrm.h"
#include "RemoteCombinedDoc.h"
#include "listviewex.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "DlgBarProject.h"
#include "ThreadRpc.h"
#include "Translation.h"


CDlgBarProject::CDlgBarProject()
{
	Init(SIDEBAR_PROJECT);

	ResetScaling();
	m_bWebSingleSelection = true;
	m_bThreadBusy = false;
	m_iProjectRefresh = 0;
	m_iProjectRefreshInterval = 0;
	m_bSelectAllProjects = true;
	m_iSendRefresh = 0;
	m_bSendRefresh = false;
}

void CDlgBarProject::ResetScaling()
{
	m_iMakeRoom = BAR_PROJECT_MAKE_ROOM;
	m_iNoLongerSizing = 0;
	m_bSizing = false;
	m_bSizingEnded = false;
	m_bMouseKeyReleased = false;
	m_bMouseKeyPressedSecondTime = false;
}

BOOL CDlgBarProject::Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID, BOOL bChange)
{
	if (!CDlgBarSide::Create(pParentWnd, nIDTemplate, nStyle, nID)) return FALSE;

	m_bChangeDockedSize = bChange;

	m_sizeDocked.cx = theApp.GetProfileInt(registrySectionWindowPosition, registryPosBarProjectCx,m_sizeDefault.cx);
	m_sizeDocked.cy = theApp.GetProfileInt(registrySectionWindowPosition, registryPosBarProjectCy,m_sizeDefault.cy);

	if (m_sizeDocked.cx < 50) m_sizeDocked.cx = 50;	// otherwise the bar may be gone.

	m_sizeFloating = m_sizeDocked;// = C//m_sizeDefault;

	ScaleList(m_sizeDocked);

	m_list.PostMessage(UWM_MSG_WND, (WPARAM) this->m_hWnd, 0);

	return TRUE;
}

CDlgBarProject::~CDlgBarProject()
{
	RemoveTree();
	DeleteProjectList();

	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosBarProjectCx, m_sizeDocked.cx);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosBarProjectCy, m_sizeDocked.cy);
}

void CDlgBarProject::DeleteProjectList(void)
{
	int iSize = (int) m_lProjects.size();
	for (int i = 0; i < iSize; i++)
	{
		delete m_lProjects.at(i);
	}
	m_lProjects.clear();
}

void CDlgBarProject::AddProject(CString sGroupIn, CString sProjectId)
{
	CString sGroup;
	int		iFoundGroup; 
	int		iPosFound1, iPosFoundNext;
	bool	bNext, bExpand, bSelected;
	CBarTreeGroupItem *pBarTreeGroupItem;

	// group;group;group;

	bNext = true;
	iPosFoundNext = 0;
	while (bNext)
	{
		bNext = false;
		bExpand = true;
		bSelected = false;
		iPosFound1 =sGroupIn.Find(";",iPosFoundNext);
		if (iPosFound1 >=0)
		{
			sGroup = sGroupIn.Mid(iPosFoundNext, iPosFound1-iPosFoundNext);
			iPosFoundNext = iPosFound1+1;
			bNext = true;
		}
		else
		{
			sGroup = sGroupIn.Mid(iPosFoundNext);
		}

		if (sGroup.GetLength() == 0) sGroup = gszTranslation[PosBarProjectsAllProjects];
	
		if (sGroup.GetAt(0) == '>')
		{
			bExpand = false;
			sGroup = sGroup.Mid(1);
		}
		else
		{
			if (sGroup.GetAt(0) == '<')
			{
				bSelected = true;
				sGroup = sGroup.Mid(1);	
			}
		}

		pBarTreeGroupItem = NULL;
		for (int iCount = 0; iCount < (int) m_lListBarTree.size(); iCount++)
		{
			if (m_lListBarTree.at(iCount)->m_sGroup == sGroup)
			{
				pBarTreeGroupItem = m_lListBarTree.at(iCount);
				if (!bExpand) pBarTreeGroupItem->m_bExpand = false;
				if (bSelected) pBarTreeGroupItem->m_bSelected = true;
				iFoundGroup = iCount;
				break;
			}
		}
	
		if (pBarTreeGroupItem == NULL)
		{
			pBarTreeGroupItem = new CBarTreeGroupItem;
			pBarTreeGroupItem->m_bExpand = bExpand;
			pBarTreeGroupItem->m_bSelected = bSelected;
			pBarTreeGroupItem->m_sGroup = sGroup;
			m_lListBarTree.push_back(pBarTreeGroupItem);
			iFoundGroup = 0;
		}
	
		// check for double

//		bool bFound = false;
//		for (int iPos = 0; iPos < pComputerTreeGroupItem->m_lList.size(); iPos++)
//		{
//			if (sProjectId.CompareNoCase(pComputerTreeGroupItem->m_lList.at(iPos)->m_sComputer) == 0)
//			{
//				bFound = true;
//				break;
//			}
//		}
//
//		if (!bFound)
//		{
			CBarTreeItem	*pBarTreeItem;
			pBarTreeItem = new CBarTreeItem;
	
			pBarTreeItem->m_sComputer = sProjectId;
			pBarTreeItem->m_iState = 0;
			pBarTreeItem->m_iImage = 1;
			pBarTreeGroupItem->m_lList.push_back(pBarTreeItem);
//		}
	}
}

void CDlgBarProject::AddProjectComplete()
{
	Sort();
	MakeTree();
}

void CDlgBarProject::SelectAllProject()
{
	this->SetFocus();

	if (m_list.GetSelectedCount() == 0)
	{
		if (m_lListBarTree.size() > 0)
		{
			m_lListBarTree.at(0)->m_bExpand = true;

			int iItemCount = m_list.GetItemCount();
			m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);			// select only the first group
			MakeTree();
			CheckGroupSelected();
		}
	}
}

// check if the group is selected, if so deselect all items in the group.
void CDlgBarProject::CheckGroupSelected()
{
	int			iItems, iState;
	bool		bGroupSelected;
	DWORD		dwData;

	iItems = m_list.GetItemCount();

	bGroupSelected = false;
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		dwData = (DWORD) m_list.GetItemData(iCount);
		if (dwData == BAR_TREE_GROUP)
		{
			iState = m_list.GetItemState(iCount, LVIS_SELECTED);
			if (iState == LVIS_SELECTED)
			{
				bGroupSelected = true;
			}
			else
			{
				bGroupSelected = false;
			}
		}
		else
		{
			if (bGroupSelected)
			{
				m_list.SetItemState(iCount, 0, LVIS_SELECTED);		// the group is selected, so delete the rest.
			}
		}
	}
}

// if all items in a group are selected, select the group.
void CDlgBarProject::CheckAllItemsSelected()
{
	int			iItems, iState, iGroup;
	bool		bItemSelected, bGroupSelected, bAllSelected;
	DWORD		dwData;

	iItems = m_list.GetItemCount();

	bGroupSelected = false;
	bAllSelected = false;
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		iState = m_list.GetItemState(iCount, LVIS_SELECTED);
		bItemSelected = (iState == LVIS_SELECTED);

		dwData = (DWORD) m_list.GetItemData(iCount);
		if (dwData == BAR_TREE_GROUP)
		{
			if (bAllSelected)
			{
				iGroup = SelectGroup(iGroup);
			}
			else
			{
				iGroup = iCount;
			}
			bAllSelected = true;
			bGroupSelected = false;
			if (bItemSelected)
			{
				iGroup = iCount;
				bGroupSelected = true;
			}
		}
		else
		{
			if (!bItemSelected) bAllSelected = false;
		}
	}
	if (bAllSelected) SelectGroup(iGroup);
}

// select the group and deselect all items in it
int CDlgBarProject::SelectGroup(int iGroup)
{
	CString sGroup;
	int iItems;
	DWORD dwData;
	CBarTreeGroupItem *pBarTreeGroupItem;

	iItems = m_list.GetItemCount();
	sGroup = m_list.GetItemText(iGroup,0);
	m_list.SetItemState(iGroup, LVIS_SELECTED, LVIS_SELECTED);

	for (int iCountGroup = 0; iCountGroup < (int) m_lListBarTree.size(); iCountGroup++)
	{
		pBarTreeGroupItem = m_lListBarTree.at(iCountGroup);
		if (sGroup == pBarTreeGroupItem->m_sGroup)
		{
			pBarTreeGroupItem->m_bSelected = true;
			break;
		}
	}

	for (int iCount = iGroup+1; iCount < iItems; iCount++)
	{
		dwData = (DWORD) m_list.GetItemData(iCount);
		if (dwData != BAR_TREE_GROUP)
		{
			m_list.SetItemState(iCount, 0, LVIS_SELECTED);
		}
		else
		{
			return iCount;
		}
	}
	return iItems;
}

// select all items in a group and deselect the group

void CDlgBarProject::SelectAllProjectsInGroup(int iGroup)
{
	int iItems;
	DWORD dwData;
	CBarTreeGroupItem *pBarTreeGroupItem;
	CString sGroup;

	iItems = m_list.GetItemCount();

	m_list.SetItemState(iGroup, 0, LVIS_SELECTED);
	sGroup = m_list.GetItemText(iGroup,0);
	for (int iCountGroup = 0; iCountGroup < (int) m_lListBarTree.size(); iCountGroup++)
	{
		pBarTreeGroupItem = m_lListBarTree.at(iCountGroup);
		if (sGroup == pBarTreeGroupItem->m_sGroup)
		{
			pBarTreeGroupItem->m_bSelected = false;
			break;
		}
	}

	for (int iCount = iGroup+1; iCount < iItems; iCount++)
	{
		dwData = (DWORD) m_list.GetItemData(iCount);
		if (dwData != BAR_TREE_GROUP)
		{
			m_list.SetItemState(iCount, LVIS_SELECTED, LVIS_SELECTED);
		}
		else
		{
			return;
		}
	}
}

// 

void CDlgBarProject::CheckGroupExpanded(CString *psGroup)
{
	for (int iCountGroup = 0; iCountGroup < (int) m_lListBarTree.size(); iCountGroup++)
	{
		if (*psGroup == m_lListBarTree.at(iCountGroup)->m_sGroup)
		{
			if (!m_lListBarTree.at(iCountGroup)->m_bExpand)
			{
				m_lListBarTree.at(iCountGroup)->m_bExpand = true;
				CheckGroupSelected();
				MakeTree();
			}
			break;
		}
	}
}


CSize CDlgBarProject::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	if (IsFloating())
    {
		// Get the parent mini frame wnd
		CMiniDockFrameWnd* pFrame = ((CMiniDockFrameWnd*) m_pDockBar->GetParent());
		CRect rcFrame;
		pFrame->GetClientRect(&rcFrame);
		// Is its size (0,0)? If so, it was just floated
		CSize sizerc(rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);
		if (!((sizerc.cx <= 0) && (sizerc.cy <= 0))) return ((m_bChangeDockedSize) ? m_sizeDocked = m_sizeFloating = sizerc : m_sizeFloating = sizerc);
		else
		{
			// Modify Style when dialog bar is first floated
			pFrame->ModifyStyle(MFS_MOVEFRAME, WS_THICKFRAME, 0);
			m_dwStyle |= MFS_THICKFRAME;
			m_dwStyle |= WS_THICKFRAME;
			m_dwStyle &= ~MFS_MOVEFRAME;
			// Return last floated size
			return m_sizeFloating;
		}
	}

	if (bStretch) // if not docked stretch to fit
	{
		return CSize(bHorz ? 32767 : m_sizeDocked.cx, bHorz ? m_sizeDocked.cy : 32767);
	}
	else return m_sizeDocked;   
}

CSize CDlgBarProject::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	CSize returnSize;

	// Return default if it is being docked or floated
	if ((dwMode & LM_VERTDOCK) || (dwMode & LM_HORZDOCK))
	{
		if (dwMode & LM_STRETCH) // if not docked stretch to fit
		{
			return CSize((dwMode & LM_HORZ) ? 32767 : m_sizeDocked.cx, (dwMode & LM_HORZ) ? m_sizeDocked.cy : 32767);
		}
		else
		{
			returnSize = m_sizeDocked;
			if (m_bSizing)
			{
				returnSize.cx += m_iMakeRoom;
			}
			returnSize.cy = 0x7fff;
			return returnSize;
		}
	}

	// no longer docked

	m_bSizing = false;

	if (dwMode & LM_MRUWIDTH)
	{
		return m_sizeFloating;
	}
	// In all other cases, accept the dynamic length
	if (dwMode & LM_LENGTHY)
	{
           return CSize(m_sizeFloating.cx, (m_bChangeDockedSize) ?  m_sizeFloating.cy = m_sizeDocked.cy = nLength : m_sizeFloating.cy = nLength);
	}
	else
	{
		return CSize((m_bChangeDockedSize) ?  m_sizeFloating.cx = m_sizeDocked.cx = nLength :  m_sizeFloating.cx = nLength, m_sizeFloating.cy);
	}
}



void CDlgBarProject::RequestProjectList()
{
	POSITION			posDoc;
	CRemoteCombinedDoc	*pDoc;
	int					iNrRpc, iThreadId;

	int iSize = (int) m_lProjects.size();

	for (int iPos = 0; iPos < iSize; iPos++)
	{
		m_lProjects.at(iPos)->m_bFound = false;
	}

	m_iThreadCount = 0;
	m_bAddProjects = false;
	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	while (posDoc)
	{
		pDoc = (CRemoteCombinedDoc*) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);				// there is only one doc
		if (pDoc != NULL)
		{
			iNrRpc = pDoc->GetRpcSelectedComputers();	
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				pDoc->GetRpcThreadId(&iThreadId, iCount);
				::PostThreadMessage(iThreadId,	UWM_MSG_THREAD_START_RPC_PROJECT_LIST, (WPARAM) this->m_hWnd, 0);
				m_bThreadBusy = true;
				m_iThreadCount++;
			}
		}
	}
}

void CDlgBarProject::SendSelectedProjectList()
{
	POSITION			posDoc;
	CRemoteCombinedDoc	*pDoc;
	int					iNrRpc, iThreadId;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	while (posDoc)
	{
		pDoc = (CRemoteCombinedDoc*) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);				// there is only one doc
		if (pDoc != NULL)
		{
			pDoc->m_pHistoryView->PostMessage(UWM_MSG_THREAD_SET_RPC_PROJECT_LIST, (WPARAM) this->m_hWnd, (LPARAM) &m_sProjectsSelected);
			iNrRpc = pDoc->GetRpcSelectedComputers();	
			for (int iCount = 0; iCount < iNrRpc; iCount++)
			{
				pDoc->GetRpcThreadId(&iThreadId, iCount);
				::PostThreadMessage(iThreadId,	UWM_MSG_THREAD_SET_RPC_PROJECT_LIST, (WPARAM) this->m_hWnd, (LPARAM) &m_sProjectsSelectedUrl);
			}
		}
	}
}

void CDlgBarProject::DoDataExchange(CDataExchange* pDX)
{
	CDlgBarSide::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_BAR_COMPUTERS, m_list);
}


BEGIN_MESSAGE_MAP(CDlgBarProject, CDlgBarSide)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZING()
	ON_WM_MOVING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()

	ON_MESSAGE(UWM_MSG_NEW_SIZE, OnNewSize)
	ON_MESSAGE(UWM_MSG_CLICKED_ON_ICON, OnClickedOnIcon)
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_PROJECT_LIST, OnThreadProjectList)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAR_COMPUTERS, &CDlgBarProject::OnNMClickListBarProjects)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BAR_COMPUTERS, &CDlgBarProject::OnLvnItemchangedListBarComputers)
//	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CDlgBarProject message handlers



LRESULT CDlgBarProject::OnInitDialog ( WPARAM wParam, LPARAM lParam)
{
	LONG bRet = (LONG) HandleInitDialog(wParam, lParam);

    if (!UpdateData(FALSE))
    {
		TRACE0("Warning: UpdateData failed during dialog init.\n");
    }

	m_imageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 4, 1);
	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_PROJECTTREE);
	m_imageList.Add(&cBmp, RGB(255,255,255));
	cBmp.DeleteObject();

	m_list.SetImageList(&m_imageList,LVSIL_SMALL);

	this->ShowWindow(SW_SHOW);

	SetTimer(UTIMER_DLG_BAR_PROJECT, 100, 0);

	return bRet;
}

void CDlgBarProject::OnTimer(UINT_PTR nIDEvent)
{
	POSITION	posDoc, posView;
	CView		*pView;
	CDocument	*pDoc;

	if (this->IsIconic()) return;
	if (!this->IsWindowVisible()) return;
	
	if (m_bSizingEnded)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x80) == 0)
		{
			ResetScaling();
			theApp.m_pMainFrame->RecalcLayout(TRUE);
			theApp.m_pMainWnd->Invalidate(TRUE);
			this->Invalidate();
		}
	}

	if	(m_bSizing)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
		{
			m_iNoLongerSizing = 0;		// mouse button still pressed

			if (m_bMouseKeyReleased)
			{
				m_bMouseKeyPressedSecondTime = true;
			}
		}
		else
		{
			// release
			m_bMouseKeyReleased = true;
			if (m_bMouseKeyPressedSecondTime)
			{
				m_iNoLongerSizing = 10000;
			}
		}

		m_iNoLongerSizing++;
		if (m_iNoLongerSizing > 25)
		{
			m_bSizingEnded = true;
			m_iMakeRoom = 0;
		}
	}

	if (m_iProjectRefresh++ > m_iProjectRefreshInterval)
	{
		if (m_iProjectRefreshInterval < 120)
		{
			m_iProjectRefreshInterval += 1;
		}
		if (!m_bThreadBusy)
		{
			m_iProjectRefresh = 0;
			RequestProjectList();
		}
	}

	if (m_bSendRefresh)
	{
		if (m_iSendRefresh-- < 0)
		{
			m_bSendRefresh = false;
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
						pView->SendMessage(UWM_MSG_VIEW_REFRESH, 0, 1);		// 1 is force refresh on history.
					}
				}
			}
		}
	}
}

LRESULT CDlgBarProject::OnThreadProjectList ( WPARAM wParam, LPARAM lParam)
{
	bool bFound;
//	bool bAddProjects;

	std::deque<std::string> *plProjects;
	CProjectBarList *pProjectBarListItem;


	plProjects = (std::deque<std::string>*) lParam;

	for (int iPos = 0; iPos < (int) plProjects->size(); iPos+=2)
	{
		bFound = false;
		for (int iPos2 = 0; iPos2 < (int) m_lProjects.size(); iPos2++)
		{
			if (StrStrI(plProjects->at(iPos).c_str(), m_lProjects.at(iPos2)->m_sProjectName.c_str()) != NULL)
			{
				bFound = true;
				m_lProjects.at(iPos2)->m_bFound = true;					 
				break;
			}
		}

		if (!bFound)
		{
			if (plProjects->at(iPos).size() > 2)	// when adding a project the project name may be empty
			{
				m_bAddProjects = true;	
				CProjectBarList *pProjectBarListItem = new CProjectBarList;
				pProjectBarListItem->m_sProjectName = plProjects->at(iPos);
				pProjectBarListItem->m_sProjectUrl = plProjects->at(iPos + 1);
				pProjectBarListItem->m_bFound = true;
				m_lProjects.push_back(pProjectBarListItem);
			}
		}
	}

	delete plProjects;

	m_iThreadCount--;
	if (m_iThreadCount == 0)
	{
		for (int iPos2 = 0; iPos2 < (int)m_lProjects.size(); iPos2++)
		{
			pProjectBarListItem = m_lProjects.at(iPos2);
			 
			if (!pProjectBarListItem->m_bFound)
			{
				m_bAddProjects = true;
				m_lProjects.erase(m_lProjects.begin() + iPos2);
				delete pProjectBarListItem;
			}
		}

		m_bAddProjects = true;
		if (m_bAddProjects)
		{
			RemoveTree();
			for (int iPos2 = 0; iPos2 < (int)m_lProjects.size(); iPos2++)
			{
				AddProject("", m_lProjects.at(iPos2)->m_sProjectName.c_str());
			}

			AddProjectComplete();
		}
		if (m_bSelectAllProjects)
		{
			SelectAllProject();
			m_bSelectAllProjects = false;
		}
		m_bThreadBusy = false;
	}

	return 0;
}

void CDlgBarProject::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
//	lpMMI->ptMaxSize.x = 1000;
//	lpMMI->ptMaxTrackSize.x = 1000;
}

LRESULT CDlgBarProject::OnNewSize ( WPARAM wParam, LPARAM lParam)
{
	int iCy;

	iCy = (int) lParam;
	if (iCy > 0)
	{
		CRect rect;
		m_sizeDocked.cy = iCy;
		ScaleList(m_sizeDocked);

		theApp.m_pMainFrame->RecalcLayout(TRUE);
		theApp.m_pMainWnd->Invalidate(TRUE);
		this->Invalidate();
	}
	return 0;
}

LRESULT CDlgBarProject::OnClickedOnIcon ( WPARAM wParam, LPARAM lParam)
{
	int iItemIcon;

	iItemIcon = (int) lParam;
	ClickedOnIcon(iItemIcon);
	return 0;
}

void CDlgBarProject::OnNMClickListBarProjects(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION	pos;
//	CView		*pView;
//	CDocument	*pDoc;
	CString		sProject;
	int			iNr, iState;
	DWORD		dwData;
	std::string sText, sUrl;

	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR); 
	sProject = m_list.GetItemText(pNMIA->iItem,0);
	dwData = (DWORD) m_list.GetItemData(pNMIA->iItem);	
	iState = m_list.GetItemState(pNMIA->iItem, LVIS_SELECTED);
	if (dwData == BAR_TREE_GROUP)
	{
		if (iState == LVIS_SELECTED)
		{
			// make sure the group is expanded
			CheckGroupExpanded(&sProject);
		}
	}

	iNr = m_list.GetItemCount();
	pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	CheckGroupSelected();

	m_sProjectsSelectedUrl = "";
	m_sProjectsSelected = "";
	if (gszTranslation[PosBarProjectsAllProjects] != sProject)
	{
		for (int iPos = 0; iPos < iNr; iPos++)
		{
			iState = m_list.GetItemState(iPos, LVIS_SELECTED);
			if (iState == LVIS_SELECTED)
			{
				dwData = (DWORD) m_list.GetItemData(iPos);	
				if (dwData == BAR_TREE_GROUP)
				{
					m_sProjectsSelectedUrl = "";
					m_sProjectsSelected = "";
					break;
				}

				sText = m_list.GetItemText(iPos,0);

				// get url
				sUrl = "";
				for (int iPos2 = 0; iPos2 < (int) m_lProjects.size(); iPos2++)
				{
					if (StrStrI(m_lProjects.at(iPos2)->m_sProjectName.c_str(), sText.c_str()) != NULL)
					{
						sUrl = m_lProjects.at(iPos2)->m_sProjectUrl;
						break;
					}
				}
				if (m_sProjectsSelectedUrl.length() > 0) m_sProjectsSelectedUrl += ",";
				m_sProjectsSelectedUrl += sUrl;
				if (m_sProjectsSelected.length() > 0) m_sProjectsSelected += ",";
				m_sProjectsSelected += sText;

			}
		}
	}

//	_strlwr((char *) m_sProjectsSelected.c_str());		// to lower

	SendSelectedProjectList();

	m_iSendRefresh = 5;
	m_bSendRefresh = true;

	*pResult = 0;
}

void CDlgBarProject::OnLvnItemchangedListBarComputers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

/*
void CDlgBarProject::OnSizing(UINT fwSide, LPRECT pRect)
{
	LONG lTop, lBottom,lLeft, lRight, lWidth, lHeight;
	bool bSetWindow = false;
	bool bSetWindowWidth = false;
	bool bSetWindowHeight = false;


// limit the minimum size of the dialog

	lTop	= pRect->top;
	lBottom = pRect->bottom;
	lLeft	= pRect->left;
	lRight	= pRect->right;
	lWidth	= lRight - lLeft;
	lHeight	= lBottom - lTop;

	if (lWidth < 40)
	{
		bSetWindow = true;
		bSetWindowWidth = true;
	}
	if (lHeight < 20)
	{
		bSetWindow = true;
		bSetWindowHeight = true;
	}

	if (bSetWindowWidth)
	{
		pRect->right  = pRect->left+ 41;
	}
	if (bSetWindowHeight)
	{
		pRect->bottom = pRect->top + 21;
	}

	if (this->IsFloating())
	{
	}
	else
	{
		if (pRect->left < 100)
		{
			pRect->left = 0;			// force left
		}
	}


	CDlgBarSide::OnSizing(fwSide, pRect);

	m_sizeDocked.cx = pRect->right - pRect->left;
	m_sizeDocked.cy = pRect->bottom - pRect->top;

	if (!m_bSizing)
	{

		m_iMakeRoom = m_sizeDocked.cx + BAR_PROJECT_MAKE_ROOM;
		if (m_sizeDocked.cx + m_iMakeRoom > BAR_PROJECT_MAKE_ROOM * 2)
		{
			m_iMakeRoom = BAR_PROJECT_MAKE_ROOM; 
		}
	}
	m_bSizing = true;
	m_iNoLongerSizing = 0;

	m_sizeDefault = m_sizeDocked;

	CSize scaleList;
	scaleList.cx = pRect->right;
	scaleList.cy = pRect->bottom;


	ScaleList(scaleList);

	if (m_iMakeRoom > 100)
	{
		theApp.m_pMainFrame->RecalcLayout(TRUE);
		theApp.m_pMainWnd->Invalidate(TRUE);
		this->Invalidate();
	}
}
*/


/*
void CDlgBarProject::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,1);
		break;
	}

	CDlgBarSide::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDlgBarProject::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,0);
		break;
	}

	CDlgBarSide::OnKeyUp(nChar, nRepCnt, nFlags);
}
*/
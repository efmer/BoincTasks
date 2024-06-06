// DlgBarComputer.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include <afxpriv.h>
#include "MainFrm.h"
#include "DlgBarComputer.h"
#include "ThreadRpc.h"
#include "Translation.h"


CDlgBarComputer::CDlgBarComputer()
{
	Init(SIDEBAR_COMPUTER);

	ResetScaling();
	m_bWebSingleSelection = true;
}

void CDlgBarComputer::ResetScaling()
{
	m_iMakeRoom = BAR_COMPUTER_MAKE_ROOM;
	m_iNoLongerSizing = 0;
	m_bSizing = false;
	m_bSizingEnded = false;
	m_bMouseKeyReleased = false;
	m_bMouseKeyPressedSecondTime = false;
}

BOOL CDlgBarComputer::Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID, BOOL bChange)
{
	if (!CDlgBarSide::Create(pParentWnd, nIDTemplate, nStyle, nID)) return FALSE;

	m_bChangeDockedSize = bChange;

	m_sizeDocked.cx = theApp.GetProfileInt(registrySectionWindowPosition, registryPosBarComputerCx,m_sizeDefault.cx);
	m_sizeDocked.cy = theApp.GetProfileInt(registrySectionWindowPosition, registryPosBarComputerCy,m_sizeDefault.cy);

	if (m_sizeDocked.cx < 50) m_sizeDocked.cx = 50;	// otherwise the bar may be gone.

	m_sizeFloating = m_sizeDocked;// = C//m_sizeDefault;

	ScaleList(m_sizeDocked);

	m_list.PostMessage(UWM_MSG_WND, (WPARAM) this->m_hWnd, 0);

	return TRUE;
}

CDlgBarComputer::~CDlgBarComputer()
{
	RemoveTree();
	
	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosBarComputerCx, m_sizeDocked.cx);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryPosBarComputerCy, m_sizeDocked.cy);
}

void CDlgBarComputer::AddComputer(CString sGroupIn, CString sComputerId)
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

		if (sGroup.GetLength() == 0) sGroup = gszTranslation[PosBarComputersAllComputers];
	
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
	
		CBarTreeItem	*pBarTreeItem;
		pBarTreeItem = new CBarTreeItem;
	
		pBarTreeItem->m_sComputer = sComputerId;
		pBarTreeItem->m_iState = 0;
		pBarTreeItem->m_iImage = 1;
		pBarTreeGroupItem->m_lList.push_back(pBarTreeItem);
	}
}

void CDlgBarComputer::AddComputerComplete()
{
	Sort();
	MakeTree();
}

void CDlgBarComputer::SelectAllComputer()
{
	this->SetFocus();

	if (m_list.GetSelectedCount() == 0)
	{
		if (m_lListBarTree.size() > 0)
		{
			m_lListBarTree.at(0)->m_bExpand = true;

			int iItemCount = m_list.GetItemCount();
			if (iItemCount == 2)
			{							// 2 items select the 1e computer
				m_list.SetItemState(1, LVIS_SELECTED, LVIS_SELECTED);		// select the first computer
				return;
			}
			m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);			// select only the first group
			MakeTree();
			CheckGroupSelected();
		}
	}
}

// check if the group is selected, if so deselect all items in the group.
void CDlgBarComputer::CheckGroupSelected()
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
void CDlgBarComputer::CheckAllItemsSelected()
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
int CDlgBarComputer::SelectGroup(int iGroup)
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

void CDlgBarComputer::SelectAllComputersInGroup(int iGroup)
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

void CDlgBarComputer::CheckGroupExpanded(CString *psGroup)
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

CSize CDlgBarComputer::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
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

CSize CDlgBarComputer::CalcDynamicLayout(int nLength, DWORD dwMode)
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

CString CDlgBarComputer::GetComputerSelected()
{
	CString sComputer;
	sComputer = "";

	int			iItems, iState, iSelected;
	bool		bGroupSelected;
	DWORD		dwData;

	iItems = m_list.GetItemCount();

	iSelected = 0;
	bGroupSelected = false;
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		dwData = (DWORD)m_list.GetItemData(iCount);
		if (dwData == BAR_TREE_GROUP)
		{
			iState = m_list.GetItemState(iCount, LVIS_SELECTED);
			if (iState == LVIS_SELECTED) bGroupSelected = true;
			else bGroupSelected = false;
		}
		else
		{
			if (bGroupSelected)	// group is selected so all of them are selected
			{
				sComputer = m_list.GetItemText(iCount, 0);
				return sComputer;
			}
			else
			{
				iState = m_list.GetItemState(iCount, LVIS_SELECTED);
				if (iState == LVIS_SELECTED)
				{
					sComputer = m_list.GetItemText(iCount, 0);
					return sComputer;
				}
			}
		}
	}	
	return "";
}

void CDlgBarComputer::DoDataExchange(CDataExchange* pDX)
{
	CDlgBarSide::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_BAR_COMPUTERS, m_list);
}


BEGIN_MESSAGE_MAP(CDlgBarComputer, CDlgBarSide)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZING()
	ON_WM_MOVING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()

	ON_MESSAGE(UWM_MSG_NEW_SIZE, OnNewSize)
	ON_MESSAGE(UWM_MSG_CLICKED_ON_ICON, OnClickedOnIcon)
	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, OnCheckComputerSelected)
	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_GET_SELECTED, OnGetComputerSelected)
	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SET_CONNECT, OnComputerConnected)
	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SET_SELECTED, OnComputerSelect)
	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_TREE, OnComputerTree)
	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_GET_TREE, OnComputerGetTree)

	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECT_ALL, OnSelectAllComputers)

	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAR_COMPUTERS, &CDlgBarComputer::OnNMClickListBarComputers)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BAR_COMPUTERS, &CDlgBarComputer::OnLvnItemchangedListBarComputers)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CDlgBarComputer message handlers


LRESULT CDlgBarComputer::OnInitDialog ( WPARAM wParam, LPARAM lParam)
{
	LONG bRet = (LONG) HandleInitDialog(wParam, lParam);

    if (!UpdateData(FALSE))
    {
		TRACE0("Warning: UpdateData failed during dialog init.\n");
    }

	m_imageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 4, 1);
	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_COMPUTERTREE);
	m_imageList.Add(&cBmp, RGB(255,255,255));
	cBmp.DeleteObject();

	m_list.SetImageList(&m_imageList,LVSIL_SMALL);

	this->ShowWindow(SW_SHOW);

	SetTimer(UTIMER_DLG_BAR_COMPUTER, 100, 0);

	return bRet;
}

void CDlgBarComputer::OnTimer(UINT_PTR nIDEvent)
{
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
}

void CDlgBarComputer::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
//	lpMMI->ptMaxSize.x = 1000;
//	lpMMI->ptMaxTrackSize.x = 1000;
}

LRESULT CDlgBarComputer::OnCheckComputerSelected ( WPARAM wParam, LPARAM lParam)
{
	char		*pcComputer;
	int			iState, iItems;
	bool		bGroupSelected;
	CString		sComputer;
	DWORD		dwData;

	pcComputer = (char *) wParam;

	iItems = m_list.GetItemCount();
	bGroupSelected = false;
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		dwData = (DWORD) m_list.GetItemData(iCount);
		if (dwData == BAR_TREE_GROUP)
		{
			iState = m_list.GetItemState(iCount, LVIS_SELECTED);
			if (iState == LVIS_SELECTED) bGroupSelected = true;
			else bGroupSelected = false;
		}
		else
		{
			if (bGroupSelected)	// group is selected so all of them are selected
			{
				sComputer = m_list.GetItemText(iCount,0);
				if (sComputer == pcComputer) return TRUE;
			}
			else
			{
				iState = m_list.GetItemState(iCount, LVIS_SELECTED);
				if (iState == LVIS_SELECTED)
				{
					sComputer = m_list.GetItemText(iCount,0);
					if (sComputer == pcComputer) return TRUE;
				}
			}
		}
	}
	return FALSE;
}

LRESULT CDlgBarComputer::OnNewSize ( WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgBarComputer::OnClickedOnIcon ( WPARAM wParam, LPARAM lParam)
{
	int iItemIcon;

	iItemIcon = (int) lParam;
	ClickedOnIcon(iItemIcon);
	return 0;
}

void CDlgBarComputer::OnNMClickListBarComputers(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION	pos, posDoc, posView;
	CView		*pView;
	CDocument	 *pDoc;
	CString		sComputer;
	int			iNr, iState;
	DWORD		dwData;

	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR); 
	sComputer = m_list.GetItemText(pNMIA->iItem,0);
	dwData = (DWORD) m_list.GetItemData(pNMIA->iItem);	
	iState = m_list.GetItemState(pNMIA->iItem, LVIS_SELECTED);
	if (dwData == BAR_TREE_GROUP)
	{
		if (iState == LVIS_SELECTED)
		{
			// make sure the group is expanded
			CheckGroupExpanded(&sComputer);
		}
	}

	iNr = m_list.GetItemCount();
	pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	CheckGroupSelected();

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
				pView->SendMessage(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, (WPARAM) &sComputer,0);	// only the view that is active will get it.
			}
		}
	}

	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgBarComputer::OnLvnItemchangedListBarComputers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

LRESULT CDlgBarComputer::OnGetComputerSelected ( WPARAM wParam, LPARAM lParam)
{
	CString		*psComputer, sComputer;

	psComputer = (CString *) wParam;
	sComputer = "";

	int			iItems, iState, iSelected;
	bool		bGroupSelected;
	DWORD		dwData;

	iItems = m_list.GetItemCount();

	iSelected = 0;
	bGroupSelected = false;
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		dwData = (DWORD) m_list.GetItemData(iCount);
		if (dwData == BAR_TREE_GROUP)
		{
			iState = m_list.GetItemState(iCount, LVIS_SELECTED);
			if (iState == LVIS_SELECTED) bGroupSelected = true;
			else bGroupSelected = false;
		}
		else
		{
			if (bGroupSelected)	// group is selected so all of them are selected
			{
				sComputer = m_list.GetItemText(iCount,0);
				iSelected++;
			}
			else
			{
				iState = m_list.GetItemState(iCount, LVIS_SELECTED);
				if (iState == LVIS_SELECTED)
				{
					sComputer = m_list.GetItemText(iCount,0);
					iSelected++;
				}
			}
		}
	}
	*psComputer = sComputer;
	return iSelected;
}

LRESULT CDlgBarComputer::OnComputerConnected ( WPARAM wParam, LPARAM lParam)
{
	char		*pcComputer;
	int			iStatus, iImage, iNrFound;
	DWORD		dwData;
	LVFINDINFO	info;
	LV_ITEM		lvi;

	pcComputer = (char *) wParam;
	iStatus = (int) lParam;

	if (iStatus == RPC_STATUS_CONNECTED)
	{
			iImage = 1;										// no icon
	}
	else
	{
		if (iStatus == RPC_STATUS_ERROR_LOST_CONNECTION)
		{
			iImage = 3;										// lost icon
		}
		else
		{
				iImage = 2;									// not connected icon
		}
	}
	info.flags = LVFI_STRING;
	info.psz = pcComputer;

	iNrFound = 0;
	while(iNrFound >= 0)
	{
		iNrFound = m_list.FindItem(&info, iNrFound);
		if (iNrFound >= 0)
		{
			dwData = (DWORD) m_list.GetItemData(iNrFound);
			if (dwData == BAR_TREE_ITEM)
			{
				lvi.mask = LVIF_IMAGE;
				lvi.iItem = iNrFound;
				lvi.iSubItem = 0;
				lvi.iImage = iImage;
				m_list.SetItem(&lvi);
			}
		}
	}

	return 0;
}

LRESULT CDlgBarComputer::OnComputerSelect ( WPARAM wParam, LPARAM lParam)
{
	POSITION	posDoc, posView;
	CView		*pView;
	CDocument	*pDoc;
	CComputerSetItem *pComputerSetItem;
	int			iItems, iState, iGroup;
	DWORD		dwData;
	bool		bGroupSelected, bGroupMatch, bItemSelected;
	CString		sItem;

	pComputerSetItem = (CComputerSetItem *) wParam;

	if (pComputerSetItem->m_sGroup == "__single_selection__")
	{
		m_bWebSingleSelection = !m_bWebSingleSelection;
		return TRUE;
	}

	bGroupSelected = false;
	bGroupMatch = false;
	iItems = m_list.GetItemCount();
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		dwData = (DWORD) m_list.GetItemData(iCount);
		iState = m_list.GetItemState(iCount, LVIS_SELECTED);
		bItemSelected = (iState == LVIS_SELECTED);
		sItem = m_list.GetItemText(iCount,0);

		if (dwData == BAR_TREE_GROUP)
		{
			bGroupMatch = false;
			if (pComputerSetItem->m_sGroup == sItem)
			{
				bGroupMatch = true;
				iGroup = iCount;
			}
			else
			{
				bGroupMatch = false;
			}

			bGroupSelected = bItemSelected;
		}
		else
		{
			if (bGroupMatch)
			{
				if (sItem == pComputerSetItem->m_sComputer)
				{
					this->SetFocus();
					if (m_bWebSingleSelection)
					{
						m_list.SetItemState(iCount, LVIS_SELECTED, LVIS_SELECTED);
					}
					else
					{
						// if the group is selected -> select all items first
						if (bGroupSelected)
						{
							SelectAllComputersInGroup(iGroup);
							m_list.SetItemState(iCount, 0, LVIS_SELECTED);	// deselect the one computer
						}
						else
						{
							if (iState == LVIS_SELECTED)	m_list.SetItemState(iCount, 0, LVIS_SELECTED);
							else m_list.SetItemState(iCount, LVIS_SELECTED, LVIS_SELECTED);
						}
					}
					m_list.SetItemState(iGroup, 0, LVIS_SELECTED);
					if (!m_bWebSingleSelection)
					{
						CheckAllItemsSelected();	// if all itemms are selected, select group instead
					}
				}
				else
				{
					if (m_bWebSingleSelection) 	m_list.SetItemState(iCount, 0, LVIS_SELECTED);
				}
			}
		}
	}
	delete pComputerSetItem;

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
				pView->SendMessage(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, (WPARAM) &sItem,0);	// only the view that is active will get it.
			}
		}
	}

	return FALSE;
}


LRESULT CDlgBarComputer::OnSelectAllComputers ( WPARAM wParam, LPARAM lParam)
{
	SelectAllComputer();
	return 0;
}

// open and close tree
LRESULT CDlgBarComputer::OnComputerTree ( WPARAM wParam, LPARAM lParam)
{
	CString *psGroup, sGroup;
	int		iStateSet, iState, iItems;
	DWORD	dwData;
	bool	bItemSelected;
	CBarTreeGroupItem	*pBarTreeGroupItem;

	psGroup = (CString *) wParam;
	iStateSet = (int) lParam;

	// single selection = select group
	if (m_bWebSingleSelection)
	{
	}


	// check if there is an item in the group selected. If so don't collapse the group.

	bItemSelected = false;
	iItems = m_list.GetItemCount();
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		dwData = (DWORD) m_list.GetItemData(iCount);
		if (dwData == BAR_TREE_GROUP)
		{
			sGroup = m_list.GetItemText(iCount,0);
			if (sGroup == *psGroup)
			{
				for (int iCountItem = iCount+1; iCountItem < iItems; iCountItem++)
				{
					iState = m_list.GetItemState(iCountItem, LVIS_SELECTED);
					bItemSelected = (iState == LVIS_SELECTED);
					if (bItemSelected) break;
					sGroup = m_list.GetItemText(iCountItem,0);
					dwData = (DWORD) m_list.GetItemData(iCountItem);
					if (dwData == BAR_TREE_GROUP)
					{
						break;
					}
				}
				break;
			}
		}
	}

	for (int iCountGroup = 0; iCountGroup < (int) m_lListBarTree.size(); iCountGroup++)
	{
		pBarTreeGroupItem = m_lListBarTree.at(iCountGroup);
		if (*psGroup == pBarTreeGroupItem->m_sGroup)
		{
			if (pBarTreeGroupItem->m_bSelected || bItemSelected) pBarTreeGroupItem->m_bExpand;// always expand a selected group
			else
			{
				pBarTreeGroupItem->m_bExpand = (iStateSet == 1);
			}
			break;
		}
	}
	MakeTree();

	return 0;
}

// get tree list pointer
LRESULT CDlgBarComputer::OnComputerGetTree ( WPARAM wParam, LPARAM lParam)
{
	MakeTree(); // update everything
	return (LRESULT) &m_lListBarTree;
}

void CDlgBarComputer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,1);
		break;
	}

	CDlgBarSide::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDlgBarComputer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
			theApp.m_pMainFrame->m_dlgBarViewSelect.PostMessage(UWM_MSG_KEY,VK_TAB,0);
		break;
	}

	CDlgBarSide::OnKeyUp(nChar, nRepCnt, nFlags);
}

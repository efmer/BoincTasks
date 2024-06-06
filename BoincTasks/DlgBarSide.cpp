// DlgBarSide.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include <afxpriv.h>
#include "MainFrm.h"
#include "DlgBarSide.h"
#include "DlgBarComputer.h"
#include "DlgBarProject.h"
#include "ThreadRpc.h"
#include "Translation.h"

// CDlgBarSide

IMPLEMENT_DYNAMIC(CDlgBarSide, CDialogBar)

CDlgBarSide::CDlgBarSide()
{
	m_bInvalidate = false;
}

CDlgBarSide::~CDlgBarSide()
{
}

void CDlgBarSide::Init(int iSidebar)
{
	m_iTypeSideBar = iSidebar;
}

void CDlgBarSide::RemoveTree()
{
	CString sComputer;

	for (int iCountGroup = 0; iCountGroup < (int)m_lListBarTree.size(); iCountGroup++)
	{
		for (int iCountItem = 0; iCountItem < (int)m_lListBarTree.at(iCountGroup)->m_lList.size(); iCountItem++)
		{
			delete m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem);
			m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem) = NULL;
		}
		delete m_lListBarTree.at(iCountGroup);
		m_lListBarTree.at(iCountGroup) = NULL;
	}
	m_lListBarTree.clear();
}

void CDlgBarSide::ClickedOnIcon(int iItem)
{
	CBarTreeGroupItem	*pComputerTreeGroupItem;
	CString				sGroup;
	DWORD				dwData;

	dwData = (DWORD)m_list.GetItemData(iItem);
	sGroup = m_list.GetItemText(iItem, 0);
	for (int iCountGroup = 0; iCountGroup < (int)m_lListBarTree.size(); iCountGroup++)
	{
		pComputerTreeGroupItem = m_lListBarTree.at(iCountGroup);
		if (sGroup == pComputerTreeGroupItem->m_sGroup)
		{
			if (dwData == BAR_TREE_GROUP)
			{
				pComputerTreeGroupItem->m_bExpand = !pComputerTreeGroupItem->m_bExpand;
			}
		}
	}
	MakeTree();
}

void CDlgBarSide::MakeTree()
{
	CString					sGroup;
	CBarTreeGroupItem		*pBarTreeGroupItem;
	int						iItemCount;
	LVITEM					lvItem;
	char					cBuffer[255];

	this->SetFocus();

	// get the icons first
	iItemCount = m_list.GetItemCount();
	lvItem.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT | LVIF_STATE;
	lvItem.stateMask = LVIS_SELECTED;
	lvItem.cchTextMax = 255;
	lvItem.pszText = cBuffer;
	for (int iCount = 0; iCount < iItemCount; iCount++)
	{
		lvItem.iItem = iCount;
		lvItem.iSubItem = 0;
		m_list.GetItem(&lvItem);
		if (lvItem.lParam == BAR_TREE_ITEM)
		{
			for (int iCountGroup = 0; iCountGroup < (int)m_lListBarTree.size(); iCountGroup++)
			{
				pBarTreeGroupItem = m_lListBarTree.at(iCountGroup);
				if (sGroup == pBarTreeGroupItem->m_sGroup)
				{
					for (int iCountItem = 0; iCountItem < (int)pBarTreeGroupItem->m_lList.size(); iCountItem++)
					{
						if (cBuffer == pBarTreeGroupItem->m_lList.at(iCountItem)->m_sComputer)
						{
							pBarTreeGroupItem->m_lList.at(iCountItem)->m_iImage = lvItem.iImage;
							pBarTreeGroupItem->m_lList.at(iCountItem)->m_iState = lvItem.state;
						}
					}
				}
			}
		}
		else
		{
			sGroup = lvItem.pszText;
			for (int iCountGroup = 0; iCountGroup < (int)m_lListBarTree.size(); iCountGroup++)
			{
				if (sGroup == m_lListBarTree.at(iCountGroup)->m_sGroup)
				{
					if (lvItem.state == LVIS_SELECTED)	m_lListBarTree.at(iCountGroup)->m_bSelected = true;
					else m_lListBarTree.at(iCountGroup)->m_bSelected = false;
				}
			}
		}
	}

	iItemCount = 0;
	m_list.DeleteAllItems();
	for (int iCountGroup = 0; iCountGroup < (int)m_lListBarTree.size(); iCountGroup++)
	{
		pBarTreeGroupItem = m_lListBarTree.at(iCountGroup);
		lvItem.pszText = pBarTreeGroupItem->m_sGroup.GetBuffer();
		lvItem.cchTextMax = pBarTreeGroupItem->m_sGroup.GetLength();
		lvItem.iImage = 0;
		lvItem.iItem = iItemCount;
		lvItem.iSubItem = 0;
		lvItem.lParam = BAR_TREE_GROUP;

		if (pBarTreeGroupItem->m_bSelected && pBarTreeGroupItem->m_bExpand)
		{
			lvItem.state = LVIS_SELECTED;
		}
		else
		{
			lvItem.state = 0;
		}

		//		lvItem.mask = LVIF_IMAGE|LVIF_PARAM|LVIF_TEXT;
		m_list.InsertItem(&lvItem);
		pBarTreeGroupItem->m_sGroup.ReleaseBuffer();
		iItemCount++;
		if (pBarTreeGroupItem->m_bExpand)
		{
			for (int iCountItem = 0; iCountItem < (int)pBarTreeGroupItem->m_lList.size(); iCountItem++)
			{
				lvItem.lParam = BAR_TREE_ITEM;
				lvItem.iItem = iItemCount;
				lvItem.pszText = pBarTreeGroupItem->m_lList.at(iCountItem)->m_sComputer.GetBuffer();
				lvItem.cchTextMax = pBarTreeGroupItem->m_lList.at(iCountItem)->m_sComputer.GetLength();
				lvItem.iImage = pBarTreeGroupItem->m_lList.at(iCountItem)->m_iImage;
				lvItem.state = pBarTreeGroupItem->m_lList.at(iCountItem)->m_iState;
				m_list.InsertItem(&lvItem);
				pBarTreeGroupItem->m_lList.at(iCountItem)->m_sComputer.ReleaseBuffer();
				iItemCount++;
			}
		}
	}
	m_list.Invalidate(TRUE);
}

void CDlgBarSide::Sort()
{
	CBarTreeGroupItem	*pBarTreeGroupItem;
	CBarTreeItem		*pBarTreeItem;
	bool					bExchange;

	// sort groups
	bExchange = true;
	while (bExchange)
	{
		bExchange = false;
		for (int iCountGroup = 0; iCountGroup < (int)m_lListBarTree.size() - 1; iCountGroup++)
		{
			if (strcmp(m_lListBarTree.at(iCountGroup)->m_sGroup, m_lListBarTree.at(iCountGroup + 1)->m_sGroup) > 0)
			{
				bExchange = true;
				pBarTreeGroupItem = m_lListBarTree.at(iCountGroup);
				m_lListBarTree.at(iCountGroup) = m_lListBarTree.at(iCountGroup + 1);
				m_lListBarTree.at(iCountGroup + 1) = pBarTreeGroupItem;
			}
		}
	}

	// sort items
	for (int iCountGroup = 0; iCountGroup < (int)m_lListBarTree.size(); iCountGroup++)
	{
		bExchange = true;
		while (bExchange)
		{
			bExchange = false;
			for (int iCountItem = 0; iCountItem < (int)m_lListBarTree.at(iCountGroup)->m_lList.size() - 1; iCountItem++)
			{
				if (strcmp(m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem)->m_sComputer, m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem + 1)->m_sComputer) > 0)
				{
					bExchange = true;
					pBarTreeItem = m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem);
					m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem) = m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem + 1);
					m_lListBarTree.at(iCountGroup)->m_lList.at(iCountItem + 1) = pBarTreeItem;
				}
			}
		}
	}
}


BEGIN_MESSAGE_MAP(CDlgBarSide, CDialogBar)
END_MESSAGE_MAP()



// CDlgBarSide message handlers



void CDlgBarSide::OnSize(UINT nType, int cx, int cy)
{
	if (m_bSizing)
	{
		m_iMakeRoom = 100;
	}
	ScaleList(m_sizeDocked);
	CDialogBar::OnSize(nType, cx, cy);
}

#define BAR_MINIMUM_WIDTH 80

void CDlgBarSide::OnSizing(UINT fwSide, LPRECT pRect)
{
	LONG lTop, lBottom, lLeft, lRight, lWidth, lHeight;
	bool bSetWindow = false;
	bool bSetWindowWidth = false;
	bool bSetWindowHeight = false;


	// limit the minimum size of the dialog

	lTop = pRect->top;
	lBottom = pRect->bottom;
	lLeft = pRect->left;
	lRight = pRect->right;
	lWidth = lRight - lLeft;
	lHeight = lBottom - lTop;

	if (lWidth < BAR_MINIMUM_WIDTH)
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
		pRect->right = pRect->left + BAR_MINIMUM_WIDTH + 1;
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

	m_sizeDocked.cx = pRect->right - pRect->left;
	m_sizeDocked.cy = pRect->bottom - pRect->top;

	if (!m_bSizing)
	{

		m_iMakeRoom = m_sizeDocked.cx + BAR_COMPUTER_MAKE_ROOM;
		if (m_sizeDocked.cx + m_iMakeRoom > BAR_COMPUTER_MAKE_ROOM * 2)
		{
			m_iMakeRoom = BAR_COMPUTER_MAKE_ROOM;
		}
	}
	m_bSizing = true;
	m_iNoLongerSizing = 0;

	m_sizeDefault = m_sizeDocked;

	CSize scaleList;
	scaleList.cx = pRect->right;
	scaleList.cy = pRect->bottom;


	ScaleList(m_sizeDocked);

	if (m_iMakeRoom > 100)
	{
		m_bInvalidate = true;
		theApp.m_pMainFrame->RecalcLayout(TRUE);
		theApp.m_pMainWnd->Invalidate(TRUE);
		this->Invalidate();
	}
	CDialogBar::OnSizing(fwSide, pRect);
}


void CDlgBarSide::ScaleList(CSize size)
{
	if (IsWindow(m_list))
	{
		CStatusBar *pStatusBar;
		CRect rect;

		pStatusBar = theApp.m_pMainFrame->GetBoincTasksStatus();
		pStatusBar->GetWindowRect(&rect);
		size.cy += rect.Height();
		m_list.SetWindowPos(this, BAR_COMPUTER_LEFT_BORDER, BAR_COMPUTER_TOP_BORDER, size.cx - BAR_COMPUTER_LEFT_BORDER - BAR_COMPUTER_RIGHT_BORDER, size.cy - BAR_COMPUTER_TOP_BORDER - BAR_COMPUTER_BOTTOM_BORDER, SWP_NOZORDER);
	}
}


void CDlgBarSide::OnMoving(UINT nSide, LPRECT lpRect)
{

}
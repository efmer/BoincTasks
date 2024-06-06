// RemoteTransferView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
#include "ListViewEx.h"					// base class
#include "TemplateRemoteComputerView.h" // base class
#include "DlgSettingsView.h"
#include "SortComputer.h"
#include "SortColumn.h"
#include "Selection.h"
#include "DlgSearchComputers.h"
#include "Translation.h"

// CTemplateRemoteComputerView

IMPLEMENT_DYNCREATE(CTemplateRemoteComputerView, CListViewEx)

CTemplateRemoteComputerView::CTemplateRemoteComputerView()
{
//	for (int iCount = 0; iCount < MAX_COMPUTERS_IN_LIST; iCount++)
//	{
//		m_pData[iCount] = NULL;				// clear data pointer table
//	}

	this->m_bSelection = FALSE;	// disable default selection

	m_dataDummy.m_color = RGB(255,255,255);
	m_dataDummy.m_cSortPrimary = "";
	m_dataDummy.m_cSortSecondary = "";
	m_dataDummy.m_cSortTertiary = "";

	m_pSortColumn = new CSortColumn;
//	m_pSelection = new CSelection;


	m_bInPlaceEdit = true;
	m_bStateIcons = TRUE;
	m_nCheckedItem = -1;
	m_bChanged = false;
	m_bSelection = true;
	bFlickerFree = false;

	m_pDlgSearchComputer = NULL;

}

CTemplateRemoteComputerView::~CTemplateRemoteComputerView()
{
	for (int iCount = 0; iCount < (int) m_lListData.size(); iCount++)
	{
		delete m_lListData.at(iCount);
	}
//	m_lListData.clear();

	DeleteComputerFind();

	if (m_pDlgSearchComputer) delete m_pDlgSearchComputer;

	delete m_pSortColumn;
}

void CTemplateRemoteComputerView::DeleteComputerFind()
{
	for (int iPos = 0; iPos < (int) m_lListComputerFind.size(); iPos++)
	{
		delete m_lListComputerFind.at(iPos);
	}
	m_lListComputerFind.clear();
}

void CTemplateRemoteComputerView::OnInitialUpdate()
{
	CListViewEx::OnInitialUpdate();
}

BEGIN_MESSAGE_MAP(CTemplateRemoteComputerView, CListViewEx)
	ON_WM_SYSCOMMAND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()

	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickList)

	ON_COMMAND(ID_VIEW_STATEICONS, OnViewStateIcons)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATEICONS, OnUpdateViewStateIcons)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CTemplateRemoteComputerView diagnostics

#ifdef _DEBUG
void CTemplateRemoteComputerView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CTemplateRemoteComputerView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CTemplateRemoteComputerView message handlers

void CTemplateRemoteComputerView::DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin)
{
	int			iItemCount;
	bool		bMax;
	CListData	*pData;

	m_bUseSkin = bUseSkin;
	m_iUseSkin = iUseSkin;

	CListCtrl& listCtrl = GetListCtrl();
	iItemCount = listCtrl.GetItemCount();

	if (bUseAlternatingLines)
	{
		for (int iCount = 1; iCount < iItemCount; iCount+=2)
		{
			pData = (CListData*) listCtrl.GetItemData(iCount);
			if (bUseSkin)
			{
				bMax = false;
				if ((pData->m_color&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
				if (((pData->m_color>>8)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
				if (((pData->m_color>>16)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
		
				if (bMax) 	pData->m_color -= RGB(COLOR_OFFSET_MIN,COLOR_OFFSET_MIN,COLOR_OFFSET_MIN);
				else pData->m_color -= RGB(COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING);
			}
			else
			{
				pData->m_color -= RGB(20,20,20);
			}
		}
	}

	for (int iCount = 0; iCount < iItemCount; iCount++)
	{
		if (listCtrl.GetItemState(iCount,LVIS_STATEIMAGEMASK) != INDEXTOSTATEIMAGEMASK(2))
		{
			pData = (CListData*) listCtrl.GetItemData(iCount);
			pData->m_color = RGB(150,150,150);
		}
	}

}

void CTemplateRemoteComputerView::StartSorting()
{
	CString sGroup, sComputers, sIpAddress, sMAC, sPort, sBOINC, sTThrottle, sStatus, sDays, sWanted;
	CListData			*pData;
	UINT				iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary,  iSortingColumnTertiary, iSortingOptionTertiary;

	CListCtrl& listCtrl = GetListCtrl();
	int iListCount = listCtrl.GetItemCount();

	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary,  &iSortingColumnTertiary, &iSortingOptionTertiary);


	for (int iCount = 0; iCount < iListCount; iCount++)
	{
		if ((int) m_lListData.size() <= iCount)
		{
			CListData *pListData;
			pListData = new CListData;
			m_lListData.push_back(pListData);
		}
		pData = m_lListData.at(iCount);
		listCtrl.SetItemData(iCount,(DWORD_PTR) pData);
		pData->m_color = RGB(255,255,255);

		sGroup = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_GROUP);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_GROUP) pData->m_sSortPrimary = sGroup;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_GROUP) pData->m_sSortSecondary = sGroup;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_GROUP) pData->m_sSortTertiary = sGroup;

		sComputers = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_ID);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_ID) pData->m_sSortPrimary = sComputers;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_ID) pData->m_sSortSecondary = sComputers;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_ID) pData->m_sSortTertiary = sComputers;
		
		sIpAddress = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_IP);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_IP) pData->m_sSortPrimary = sIpAddress;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_IP) pData->m_sSortSecondary = sIpAddress;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_IP) pData->m_sSortTertiary = sIpAddress;

		sMAC = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_MAC);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_MAC) pData->m_sSortPrimary = sMAC;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_MAC) pData->m_sSortSecondary = sMAC;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_MAC) pData->m_sSortTertiary = sMAC;

		sPort = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_PORT);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_PORT) pData->m_sSortPrimary = sPort;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_PORT) pData->m_sSortSecondary = sPort;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_PORT) pData->m_sSortTertiary = sPort;

		sBOINC = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_BOINC);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_BOINC) pData->m_sSortPrimary = sBOINC;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_BOINC) pData->m_sSortSecondary = sBOINC;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_BOINC) pData->m_sSortTertiary = sBOINC;

		sBOINC = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_BOINC_PLATFORM);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_BOINC_PLATFORM) pData->m_sSortPrimary = sBOINC;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_BOINC_PLATFORM) pData->m_sSortSecondary = sBOINC;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_BOINC_PLATFORM) pData->m_sSortTertiary = sBOINC;

		sTThrottle = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_TTHROTTLE);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_TTHROTTLE) pData->m_sSortPrimary = sTThrottle;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_TTHROTTLE) pData->m_sSortSecondary = sTThrottle;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_TTHROTTLE) pData->m_sSortTertiary = sTThrottle;

		sStatus = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_STATUS);
		if (iSortingColumnPrimary == COLUMN_COMPUTER_STATUS) pData->m_sSortPrimary = sStatus;
		if (iSortingColumnSecondary == COLUMN_COMPUTER_STATUS) pData->m_sSortSecondary = sStatus;
		if (iSortingColumnTertiary == COLUMN_COMPUTER_STATUS) pData->m_sSortTertiary = sStatus;

//		sDays = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_DAYS);
//		if (iSortingColumnPrimary == COLUMN_COMPUTER_DAYS) pData->m_sSortPrimary = sDays;
//		if (iSortingColumnSecondary == COLUMN_COMPUTER_DAYS) pData->m_sSortSecondary = sDays;
//		if (iSortingColumnTertiary == COLUMN_COMPUTER_DAYS) pData->m_sSortTertiary = sDays;

//		sWanted = listCtrl.GetItemText(iCount,COLUMN_COMPUTER_WU);
//		if (iSortingColumnPrimary == COLUMN_COMPUTER_WU) pData->m_sSortPrimary = sWanted;
//		if (iSortingColumnSecondary == COLUMN_COMPUTER_WU) pData->m_sSortSecondary = sWanted;
//		if (iSortingColumnTertiary == COLUMN_COMPUTER_WU) pData->m_sSortTertiary = sWanted;
	}



	if (listCtrl.GetItemCount() > 1)
	{
		DWORD iSorting = (iSortingColumnPrimary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionPrimary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnSecondary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionSecondary&0xf);
		iSorting <<= 6;
		iSorting += (iSortingColumnTertiary&0x3f);
		iSorting <<= 4;
		iSorting += (iSortingOptionTertiary&0xf);
		listCtrl.SortItems(ComputerSorting,iSorting);
	}
	DisplayMakeLineColors(theApp.m_pDlgSettingsView->m_bAlternatingStriped, theApp.m_pDlgSettingsView->m_bUseSkin, theApp.m_pDlgSettingsView->m_iUseSkin);
}


void CTemplateRemoteComputerView::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pSortColumn->ClickItem(pNMHDR, pResult);

	StartSorting();

//	m_iTimerWu = 1000;				// 1000 to ensure direct timer message
}

void CTemplateRemoteComputerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteComputerView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListViewEx::OnKeyUp(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteComputerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListViewEx::OnRButtonDown(nFlags, point);
}

void CTemplateRemoteComputerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT uFlags = 0;
	int nHitItem = GetListCtrl().HitTest(point, &uFlags);

	// we need additional checking in owner-draw mode
	// because we only get LVHT_ONITEM
	BOOL bHit = FALSE;
	if (uFlags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))
	{
		CRect rect;
		GetListCtrl().GetItemRect(nHitItem, rect, LVIR_ICON);

		// check if hit was on a state icon
		if (m_bStateIcons && point.x < rect.left) bHit = TRUE;
	}
	else if (uFlags & LVHT_ONITEMSTATEICON)	bHit = TRUE;

	if (bHit) CheckItem(nHitItem);
	else CListViewEx::OnLButtonDown(nFlags, point);


	if (bHit) m_bChanged = true;
}

void CTemplateRemoteComputerView::CheckItem(int nNewCheckedItem)
{
	CListCtrl& ListCtrl = GetListCtrl();

// reset if there is checked item

//	if (m_nCheckedItem != -1)
	{

		if (ListCtrl.GetItemState(nNewCheckedItem,LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))
		{
			ListCtrl.SetItemState(nNewCheckedItem,INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK);
		}
		else
		{
			ListCtrl.SetItemState(nNewCheckedItem, INDEXTOSTATEIMAGEMASK(2),	LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK);
		}
	}
}

void CTemplateRemoteComputerView::OnViewStateIcons()
{
	m_bStateIcons =! m_bStateIcons;

	CListCtrl& ListCtrl = GetListCtrl();

	if (m_bStateIcons)
	{
		ListCtrl.SetImageList(&m_StateImageList, LVSIL_STATE);
		ListCtrl.SetItemState(m_nCheckedItem, 0, LVIS_OVERLAYMASK);
	}
	else
	{
		ListCtrl.SetImageList(NULL,LVSIL_STATE);
		if (m_nCheckedItem != -1)
		{
			ListCtrl.SetItemState(m_nCheckedItem,
				INDEXTOOVERLAYMASK(1), LVIS_OVERLAYMASK);
		}
	}
}

void CTemplateRemoteComputerView::OnUpdateViewStateIcons(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bStateIcons);
}
void CTemplateRemoteComputerView::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		// close window = cancel		return;
	}
	CListViewEx::OnSysCommand(nID, lParam);
}

BOOL CTemplateRemoteComputerView::OnEraseBkgnd(CDC* pDC)
{
	return CListViewEx::OnEraseBkgnd(pDC);
}

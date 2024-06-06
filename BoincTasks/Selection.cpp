#include "StdAfx.h"
#include "ListViewEx.h"
#include "Selection.h"


CSelectionItem::CSelectionItem()
{
	m_pcComputer = NULL;
	m_pcTask = NULL;
}

CSelectionItem::~CSelectionItem()
{
	if (m_pcComputer != NULL)	delete m_pcComputer;
	if (m_pcTask != NULL)	delete m_pcTask;
}

void CSelectionItem::AddComputer(char *pcComputer)
{
	Add(&m_pcComputer, pcComputer);
}

void CSelectionItem::AddTask(char *pcTask)
{
	Add(&m_pcTask, pcTask);
}

void CSelectionItem::Add(char **pcDest, char *pcSource)
{
	int iLen;

	if (*pcDest != NULL)
	{
		delete *pcDest;
		*pcDest = NULL;
	}

	if (pcSource == NULL) return;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
}


CSelection::CSelection(void)
{
	// set order for 1 = 1 
	for (int iCount = 0; iCount < 20; iCount++)
	{
		m_iColumnOrder[iCount] = iCount;
	}
	ClearSelection();
}

CSelection::~CSelection(void)
{
	ClearSelection();
}

void CSelection::ClearSelection()
{
	for (int iCount = 0; iCount < (int) m_lSelected.size(); iCount++)
	{
		delete m_lSelected.at(iCount);
	}
	m_lSelected.clear();
}

void CSelection::Setup(CListCtrl *pListCtrl, std::deque<CListData*> *plData, int iColumnFollowTask, int iColumnFollowComputer, int iColumnCount, int *pOrder)
{
	int iCount;

	m_pListCtrl = pListCtrl;
	m_iColumnFollowTask = iColumnFollowTask;
	m_iColumnFollowComputer = iColumnFollowComputer;
//	m_iMaxData = iMaxData;
	m_plData = plData;

	if (pOrder != NULL)
	{
		// new column order
		for (iCount = 0; iCount < iColumnCount; iCount++)
		{
			m_iColumnOrder[iCount] = *pOrder;
			pOrder++;
		}
	}
	SelectionRemoveAll();
}

// selection with the right mouse button
void CSelection::SelectionSetRight(int iItemSelected)
{
	// check if over an already selected line.
	// if so do nothing.
	bool	bSelectedRemoved = false;

	if (SelectionSelected(m_pListCtrl, iItemSelected))
	{
		return;
	}
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = m_pListCtrl->GetNextSelectedItem(pos);
			m_pListCtrl->SetItemState(nItem, 0, LVIS_SELECTED);
			bSelectedRemoved = true;
		}
	}
	if (bSelectedRemoved) m_pListCtrl->Invalidate();

	SelectionSet(iItemSelected);
}

void CSelection::SelectionSet(int iItemSelected, int iOption)
{
	char	cSelectedTask[256], cSelectedComputer[256];
	int		iSelectionLast;
	bool	bControlKey, bShiftKey;

	if (iOption)
	{
		bControlKey = true;
		bShiftKey = false;
	}
	else
	{
		bControlKey = ((GetKeyState(VK_LCONTROL) &0x80) >0) || ((GetKeyState(VK_RCONTROL) &0x80) > 0);
		bShiftKey = ((GetKeyState(VK_LSHIFT)&0x80) >0) || ((GetKeyState(VK_RSHIFT) &0x80) >0);
	}

	if (bShiftKey)
	{
		if (iItemSelected >=0)
		{
			iSelectionLast = SelectionGetLast(m_pListCtrl);
			if (iSelectionLast >=0)
			{
				// select every from the first to the last or last from the first
				if (!bControlKey)
				{
					SelectionRemoveAll();
				}
				SelectionSelectFromTo(m_pListCtrl, iItemSelected, iSelectionLast);
				m_pListCtrl->Invalidate(FALSE);
				return;
			}
			else
			{
				// nothing is selected this will mark the last selection.
				m_pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowTask],cSelectedTask,255);
				m_pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowComputer],cSelectedComputer,255);
				SelectionLast(cSelectedTask, cSelectedComputer);
				m_pListCtrl->Invalidate(FALSE);
			}
		}
	}
	else
	{
		// any click not shift will mark the last selection.
		m_pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowTask],cSelectedTask,255);
		m_pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowComputer],cSelectedComputer,255);
		SelectionLast(cSelectedTask, cSelectedComputer);
	}

	m_pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowTask],cSelectedTask,255);				// keep following the name
	m_pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowComputer],cSelectedComputer,255);		// keep following the name

	if (iItemSelected >=0)
	{
		if (!bControlKey)
		{
			SelectionRemoveAll();
		}
		else
		{
			if (SelectionSelected(m_pListCtrl, iItemSelected))
			{
				// already there = remove
				SelectionDelete(m_pListCtrl, iItemSelected);
				return;
			}
		}
		SelectionAdd(iItemSelected, cSelectedTask, cSelectedComputer);
	}
}

bool CSelection::SelectionAdd(int iItemSelected, char *pcSelectedTask, char *pcSelectedComputer)
{
	CListData	*pData;
	CSelectionItem *pSelectionItem;

	pSelectionItem = new CSelectionItem;

	pSelectionItem->AddComputer(pcSelectedComputer);
	pSelectionItem->AddTask(pcSelectedTask);
	m_lSelected.push_back(pSelectionItem);

	pData = (CListData *) m_pListCtrl->GetItemData(iItemSelected);
	if (pData != NULL)
	{
		pData->m_bSelected = true;
		m_pListCtrl->SetItemData(iItemSelected,(DWORD_PTR) pData);
	}

	return true;
}

bool CSelection::SelectionRemoveAll()
{
	CRect rcLabel;

	for (int iCount = 0; iCount < (int) m_plData->size(); iCount++)
	{
		m_plData->at(iCount)->m_bSelected = false;
	}

	ClearSelection();
	return true;
}

void CSelection::SelectionLast(char *pcSelectedTask, char *pcSelectedComputer)
{
	m_sSelectedLastTask = pcSelectedTask;
	m_sSelectedLastComputer = pcSelectedComputer;
}

void CSelection::SelectionLastRemove()
{
	m_sSelectedLastTask = "";
	m_sSelectedLastComputer = "";
}

int CSelection::SelectionGetLast(CListCtrl *pListCtrl)
{
	char cBuffer[256];
//	CListData	*pData;

	int iNrItems = pListCtrl->GetItemCount();

	for (int iCount = 0; iCount < iNrItems; iCount++)
	{
		pListCtrl->GetItemText(iCount, m_iColumnOrder[m_iColumnFollowTask],cBuffer,255);
		if (m_sSelectedLastTask == cBuffer)
		{
			pListCtrl->GetItemText(iCount, m_iColumnOrder[m_iColumnFollowComputer],cBuffer,255);
			if (m_sSelectedLastComputer == cBuffer)
			{
				// found now check if still selected
				if (SelectionSelected(pListCtrl, iCount)) return iCount;
				// remove last selection if not selected
				SelectionLastRemove();
				return -1;
			}
		}
	}
	// remove last selection if not selected
	SelectionLastRemove();
	return -1;
}

void CSelection::SelectionSelectFromTo(CListCtrl *pListCtrl,int iFirst, int iLast)
{	
	char cBufferTask[256], cBufferComputer[256];
		
	if (iFirst > iLast)
	{
		int iTemp;
		iTemp = iFirst;
		iFirst = iLast;
		iLast = iTemp;
	}

	int	iNrItems = pListCtrl->GetItemCount();
	if (iLast > iNrItems) iLast = iNrItems;
	if (iFirst > iNrItems) iLast = iNrItems;

	for (int iCount = iFirst; iCount <= iLast; iCount++)
	{
		pListCtrl->GetItemText(iCount, m_iColumnOrder[m_iColumnFollowTask],cBufferTask,255);
		pListCtrl->GetItemText(iCount, m_iColumnOrder[m_iColumnFollowComputer],cBufferComputer,255);
		SelectionAdd(iCount,cBufferTask, cBufferComputer);
	}
}

bool CSelection::SelectionSelected(CListCtrl *pListCtrl, int iItem)
{
	CListData	*pData;
	if (iItem < 0 ) return false;
	pData = (CListData *) pListCtrl->GetItemData(iItem);
	if (pData)	return pData->m_bSelected;
	return false;
}

void CSelection::SelectionDelete(CListCtrl *pListCtrl, int iItemSelected)
{
	char cBufferTask[256], cBufferComputer[256];
	int	iDelete;

	CListData	*pData;
	pData = (CListData *) pListCtrl->GetItemData(iItemSelected);
	pData->m_bSelected = false;
	pListCtrl->SetItemData(iItemSelected,(DWORD_PTR) pData);
	pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowTask],cBufferTask,255);
	pListCtrl->GetItemText(iItemSelected, m_iColumnOrder[m_iColumnFollowComputer],cBufferComputer,255);	

	iDelete = -1;

	for (int iCount=0; iCount < (int) m_lSelected.size(); iCount++)
	{
		if (strcmp(m_lSelected.at(iCount)->m_pcTask, cBufferTask) == 0)
		{
			if (strcmp(m_lSelected.at(iCount)->m_pcComputer, cBufferComputer) == 0)
			{
				delete m_lSelected.at(iCount);
				m_lSelected.erase(m_lSelected.begin()+iCount);
				break;
			}
		}
	}
}

int CSelection::SelectionNumber()
{
	return (int) m_lSelected.size();
}

bool CSelection::Follow(char *pcTaskName, char *pcComputerName)
{
	for (int iSelectedCount=0; iSelectedCount < (int) m_lSelected.size(); iSelectedCount++)
	{
		if (strcmp(m_lSelected.at(iSelectedCount)->m_pcTask, pcTaskName) == 0)
		{
			if (strcmp(m_lSelected.at(iSelectedCount)->m_pcComputer, pcComputerName) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

void CSelection::SelectionMoveDown(CListCtrl *pListCtrl, int iTaskColumn, int iComputerColumn, int iNrLines)
{
	CListData	*pData;
	int			iItems, iNextItem;
	CString		sTask, sComputer;

	iItems = pListCtrl->GetItemCount();
	if (iItems == 0) return;

	for (int iMoveLines = 0; iMoveLines < iNrLines; iMoveLines++)
	{
		ClearSelection();
		for (int iCount = iItems-1; iCount >= 0; iCount--)
		{
			pData = (CListData *) pListCtrl->GetItemData(iCount);
			if (pData->m_bSelected) 
			{
				if (iCount <= iItems-2)
				{
					iNextItem = iCount+1;
					pData->m_bSelected = false;
					pData = (CListData *) pListCtrl->GetItemData(iNextItem);
					pData->m_bSelected = true;
				}
				else
				{
					iNextItem = iCount;
				}
				sTask = pListCtrl->GetItemText(iNextItem,iTaskColumn);
				sComputer = pListCtrl->GetItemText(iNextItem,iComputerColumn);
				SelectionAdd(iNextItem, sTask.GetBuffer(), sComputer.GetBuffer());
				sTask.ReleaseBuffer();
				sComputer.ReleaseBuffer();
				pListCtrl->EnsureVisible(iNextItem,FALSE);
				pListCtrl->Invalidate();
			}
		}
	}
}

void CSelection::SelectionMoveUp(CListCtrl *pListCtrl, int iTaskColumn, int iComputerColumn, int iNrLines)
{
	CListData	*pData;
	int			iItems, iNextItem;
	CString		sTask, sComputer;

	iItems = pListCtrl->GetItemCount();
	if (iItems == 0) return;

	for (int iMoveLines = 0; iMoveLines < iNrLines; iMoveLines++)
	{
		ClearSelection();
		for (int iCount = 0; iCount < iItems; iCount++)
		{
			pData = (CListData *) pListCtrl->GetItemData(iCount);
			if (pData->m_bSelected) 
			{
				if (iCount >= 1)
				{
					iNextItem = iCount-1;
					pData->m_bSelected = false;
					pData = (CListData *) pListCtrl->GetItemData(iNextItem);
					pData->m_bSelected = true;
				}
				else
				{
					iNextItem = iCount;
				}

				pData->m_bSelected = false;
				pData = (CListData *) pListCtrl->GetItemData(iNextItem);
				pData->m_bSelected = true;
	
				sTask = pListCtrl->GetItemText(iNextItem,iTaskColumn);
				sComputer = pListCtrl->GetItemText(iNextItem,iComputerColumn);
				SelectionAdd(iNextItem, sTask.GetBuffer(), sComputer.GetBuffer());
				sTask.ReleaseBuffer();
				sComputer.ReleaseBuffer();
				pListCtrl->EnsureVisible(iNextItem,FALSE);
				pListCtrl->Invalidate();
			}
		}
	}
}

void CSelection::SelectionBegin(CListCtrl *pListCtrl)
{
	int		iItem;
	SHORT	iShiftKey;

	iShiftKey = GetKeyState(VK_SHIFT);
	iShiftKey &= 0x8000;

	iItem = SelectionGetLast(pListCtrl);
	SelectionRemoveAll();

//	iItems = pListCtrl->GetItemCount();

	if (iShiftKey)
	{
		if (iItem < 0) iItem = 0;	// first
	}
	else
	{
		iItem = 0;
	}
	SelectionSelectFromTo(m_pListCtrl, 0, iItem);
	m_pListCtrl->Invalidate(FALSE);
}

void CSelection::SelectionEnd(CListCtrl *pListCtrl)
{
	int		iItems, iItem;
	SHORT	iShiftKey;

	iShiftKey = GetKeyState(VK_SHIFT);
	iShiftKey &= 0x8000;

	iItem = SelectionGetLast(pListCtrl);
	SelectionRemoveAll();

	iItems = pListCtrl->GetItemCount();

	if (iShiftKey)
	{
		if (iItem < 0) iItem = iItems-1;	// last
	}
	else
	{
		iItem = iItems-1;
	}
	SelectionSelectFromTo(m_pListCtrl, iItem, iItems);
	m_pListCtrl->Invalidate(FALSE);
}

CString CSelection::GetLastSelected(void)
{
	return m_sSelectedLastTask;
}


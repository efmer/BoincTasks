#include "StdAfx.h"
#include "BoincTasks.h"
#include "SortColumn.h"
#include "ThreadRpc.h"

CSortColumn::CSortColumn(void)
{
	// set order for 1 = 1 
	for (int iCount = 0; iCount < MAX_SORTING_COLUMN; iCount++)
	{
		m_iColumnOrder[iCount] = iCount;
	}
}

CSortColumn::~CSortColumn(void)
{
}

void CSortColumn::SetColumnOrder(int iOrderCount, int *pOrder)
{	
	int iCount;
	for (iCount = 0; iCount < iOrderCount; iCount++)
	{
		m_iColumnOrder[iCount] = *pOrder;
		pOrder++;
	}
}

void CSortColumn::OrderChanged(int iOrderCount, int *pOrder)
{
	int iCount, iNewColumnPrimary, iNewColumnSecondary, iNewColumnTertiary;

	iNewColumnPrimary = 0;
	iNewColumnSecondary = 0;
	iNewColumnTertiary = 0;

	for (iCount = 0; iCount < iOrderCount; iCount++)
	{
		if (m_iSortingColumnPrimary == *(pOrder+iCount))
		{
			iNewColumnPrimary = iCount;
		}
		if (m_iSortingColumnSecondary == *(pOrder+iCount))
		{
			iNewColumnSecondary = iCount;
		}
		if (m_iSortingColumnTertiary == *(pOrder+iCount))
		{
			iNewColumnTertiary = iCount;
		}

	}
	m_iSortingColumnPrimary = iNewColumnPrimary;
	m_iSortingColumnSecondary = iNewColumnSecondary;
	m_iSortingColumnTertiary = iNewColumnTertiary;
	WriteRegistry();	// update sorting
}

void CSortColumn::Setup(CListCtrl *pListCtrl, HWND hWnd, UINT iNrColums, char *pRegistrySortPrimary, char *pRegistrySortSecondary, char *pRegistrySortTertiary, int iDefaultPrimary, int iDefaultSecondary, int iDefaultTertiary)
{
	int iSort;

	m_pListCtrl = pListCtrl;
	m_hWnd = hWnd;
	m_iNrColomns = iNrColums;

	m_pRegistrySortPrimary = pRegistrySortPrimary;
	m_pRegistrySortSecondary = pRegistrySortSecondary;
	m_pRegistrySortTertiary = pRegistrySortTertiary;

	iSort = theApp.GetProfileInt(registrySectionSorting, pRegistrySortPrimary, iDefaultPrimary);
	m_iSortingColumnPrimary = iSort & 0xff;
	m_iSortingOptionPrimary = iSort >> 8;
	if (m_iSortingColumnPrimary >= iNrColums)
	{
		m_iSortingOptionPrimary = SORTING_NONE;
		m_iSortingColumnPrimary = SORTING_COLUMN_END;
		m_iSortingColumnPrimary = 0;
	}

	iSort = theApp.GetProfileInt(registrySectionSorting, pRegistrySortSecondary, iDefaultSecondary);	// computer
	m_iSortingColumnSecondary = iSort & 0xff;
	m_iSortingOptionSecondary = iSort >> 8;
	if (m_iSortingColumnSecondary >= iNrColums)
	{
		m_iSortingOptionSecondary = SORTING_NONE;
		m_iSortingColumnSecondary = SORTING_COLUMN_END;
		m_iSortingColumnSecondary = 0;
	}

	iSort = theApp.GetProfileInt(registrySectionSorting, pRegistrySortTertiary, iDefaultTertiary);		// computer
	m_iSortingColumnTertiary = iSort & 0xff;
	m_iSortingOptionTertiary = iSort >> 8;
	if (m_iSortingColumnTertiary >= iNrColums)
	{
		m_iSortingOptionTertiary = SORTING_NONE;
		m_iSortingColumnTertiary = SORTING_COLUMN_END;
		m_iSortingColumnTertiary = 0;
	}

	if (m_iSortingColumnPrimary == m_iSortingColumnSecondary )
	{
		m_iSortingOptionSecondary = SORTING_NONE;
		m_iSortingColumnSecondary = SORTING_COLUMN_END;
	}
	if (m_iSortingColumnPrimary == m_iSortingColumnTertiary )
	{
		m_iSortingOptionTertiary = SORTING_NONE;
		m_iSortingColumnTertiary = SORTING_COLUMN_END;
	}
	if (m_iSortingColumnSecondary== m_iSortingColumnTertiary )
	{
		m_iSortingOptionTertiary = SORTING_NONE;
		m_iSortingColumnTertiary = SORTING_COLUMN_END;
	}

	RemoveDoubles(&m_iSortingColumnPrimary, &m_iSortingColumnSecondary, &m_iSortingColumnTertiary, &m_iSortingOptionSecondary, &m_iSortingOptionTertiary);
	SetSortingIndicator(m_iSortingColumnPrimary, m_iSortingOptionPrimary, m_iSortingColumnSecondary, m_iSortingOptionSecondary, m_iSortingColumnTertiary, m_iSortingOptionTertiary);
}

void CSortColumn::RemoveDoubles(UINT *piColumnPrimary, UINT *piColumnSecondary, UINT *piColumnTertiary, UINT *piSortingSecondary, UINT *piSortingTertiary)
{
	if (*piColumnPrimary == *piColumnSecondary)
	{
		// remove secondary
		*piColumnSecondary = SORTING_COLUMN_END;
		*piSortingSecondary = SORTING_NONE;
	}
	if (*piColumnPrimary == *piColumnTertiary)
	{
		// remove tertiary
		*piColumnTertiary = SORTING_COLUMN_END;
		*piSortingTertiary = SORTING_NONE;
	}
	if (*piColumnSecondary != SORTING_COLUMN_END)
	{
		if (*piColumnSecondary == *piColumnTertiary)
		{
			// remove tertiary
			*piColumnTertiary = SORTING_COLUMN_END;
			*piSortingTertiary = SORTING_NONE;
		}
	}
}

void CSortColumn::GetColumnOption(UINT *piSortingColumnPrimary, UINT *piSortingOptionPrimary, UINT *piSortingColumnSecondary, UINT *piSortingOptionSecondary, UINT *piSortingColumnTertiary, UINT *piSortingOptionTertiary)
{
	if (m_iSortingColumnPrimary > MAX_SORTING_COLUMN) m_iSortingColumnPrimary = 0;
	if (m_iSortingColumnSecondary > MAX_SORTING_COLUMN) m_iSortingColumnSecondary = 0;
	if (m_iSortingColumnTertiary > MAX_SORTING_COLUMN) m_iSortingColumnTertiary = 0;

	*piSortingColumnPrimary = m_iColumnOrder[m_iSortingColumnPrimary];
	*piSortingOptionPrimary = m_iSortingOptionPrimary;
	*piSortingColumnSecondary = m_iColumnOrder[m_iSortingColumnSecondary];
	*piSortingOptionSecondary = m_iSortingOptionSecondary;
	*piSortingColumnTertiary = m_iColumnOrder[m_iSortingColumnTertiary];
	*piSortingOptionTertiary = m_iSortingOptionTertiary;

	if (*piSortingColumnPrimary > MAX_SORTING_COLUMN) *piSortingColumnPrimary = 0;
	if (*piSortingColumnSecondary > MAX_SORTING_COLUMN) *piSortingColumnSecondary = 0;
	if (*piSortingColumnTertiary > MAX_SORTING_COLUMN) *piSortingColumnTertiary = 0;
}

void CSortColumn::ClickItem(NMHDR* pNMHDR, LRESULT* pResult, int iColumn)
{
	CString sTxt, sSortUp, sSortDown;
	bool	bRControlKey, bLControlKey;

	int iState = GetKeyState(VK_LCONTROL) & 0x80;

	bLControlKey = (GetKeyState(VK_LCONTROL) &0x80) > 0;

	// right control key or left shift key for third
	bRControlKey = (GetKeyState(VK_RCONTROL) &0x80) > 0;
	if (!bRControlKey)
	{
		bRControlKey = (GetKeyState(VK_LSHIFT) & 0x80) > 0;
	}

	if (iColumn == -1)
	{
		LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
		iColumn = phdr->iItem;
	}

	SetSorting(iColumn, bLControlKey, bRControlKey);
}

void CSortColumn::SetSorting(int iColumn, bool bLControlKey, bool bRControlKey)
{
	LV_COLUMN	lvc;
	CString sTxt, sSortUp, sSortDown;
	bool	bSortingChanged = false;
	char	cBuffer[255];
	int		iSort;

	if (bLControlKey) 
	{
		iSort = SORT_SECONDARY;
		sSortDown = SORT_STRING_DOWN_SECONDARY;
		sSortUp	= SORT_STRING_UP_SECONDARY;
	}
	else
	{
		if (bRControlKey)
		{
			iSort = SORT_TERTIARY;
			sSortDown = SORT_STRING_DOWN_TERTIARY;
			sSortUp	= SORT_STRING_UP_TERTIARY;
		}
		else
		{
			iSort = SORT_PRIMARY;
			sSortDown = SORT_STRING_DOWN_PRIMARY;
			sSortUp	= SORT_STRING_UP_PRIMARY;
		}
	}

	cBuffer[0] = 0;

	lvc.pszText = &cBuffer[0];
	lvc.cchTextMax = 254;
	lvc.mask = LVCF_TEXT;
	m_pListCtrl->GetColumn(iColumn,&lvc);

	sTxt = lvc.pszText;

	if (iSort == SORT_PRIMARY) {m_iSortingOptionPrimary = SORTING_NONE;	m_iSortingColumnPrimary = iColumn;}
	if (iSort == SORT_SECONDARY) {m_iSortingOptionSecondary= SORTING_NONE;m_iSortingColumnSecondary = iColumn;}
	if (iSort == SORT_TERTIARY) {m_iSortingOptionTertiary= SORTING_NONE; m_iSortingColumnTertiary = iColumn;}

	if (sTxt.Find(sSortUp) >=0)
	{
		if (iSort == SORT_PRIMARY)	m_iSortingOptionPrimary = SORTING_ZA;
		if (iSort == SORT_SECONDARY)m_iSortingOptionSecondary = SORTING_NONE;		// 3 way toggle on secondary
		if (iSort == SORT_TERTIARY) m_iSortingOptionTertiary = SORTING_NONE;		// 3 way toggle on tertiary
		bSortingChanged = true;
	}
	if (sTxt.Find(sSortDown) >=0)
	{
		if (iSort == SORT_PRIMARY)	m_iSortingOptionPrimary = SORTING_AZ;
		if (iSort == SORT_SECONDARY)m_iSortingOptionSecondary = SORTING_AZ;
		if (iSort == SORT_TERTIARY) m_iSortingOptionTertiary = SORTING_AZ;
		bSortingChanged = true;
	}

	if (!bSortingChanged)
	{	
		//oops found nothing
		if (iSort == SORT_PRIMARY)	m_iSortingOptionPrimary = SORTING_ZA;
		if (iSort == SORT_SECONDARY)m_iSortingOptionSecondary = SORTING_ZA;		
		if (iSort == SORT_TERTIARY) m_iSortingOptionTertiary = SORTING_ZA;	
		bSortingChanged = true;
	}

	if (m_iSortingColumnSecondary == m_iSortingColumnPrimary) m_iSortingOptionSecondary = SORTING_NONE;
	if (m_iSortingColumnTertiary  == m_iSortingColumnPrimary)	m_iSortingOptionTertiary = SORTING_NONE;
	if (m_iSortingColumnTertiary  == m_iSortingColumnSecondary)
	{
		if (m_iSortingOptionSecondary != SORTING_NONE) m_iSortingOptionTertiary = SORTING_NONE;
	}
	RemoveDoubles(&m_iSortingColumnPrimary, &m_iSortingColumnSecondary, &m_iSortingColumnTertiary, &m_iSortingOptionSecondary, &m_iSortingOptionTertiary);
	SetSortingIndicator(m_iSortingColumnPrimary, m_iSortingOptionPrimary, m_iSortingColumnSecondary, m_iSortingOptionSecondary, m_iSortingColumnTertiary, m_iSortingOptionTertiary);

	if (bSortingChanged)
	{
		::PostMessage(this->m_hWnd,WM_TIMER,0,0);
	}

	WriteRegistry();
}

void CSortColumn::ToggleSorting(int iColumn)
{
	bool	bFound = false, bRControlKey = false, bLControlKey = false;

	if (m_iSortingColumnPrimary == iColumn)
	{
		bFound = true;
	}
	else
	{
		if (m_iSortingColumnSecondary == iColumn)
		{
			bFound = true;
			bLControlKey = true;
		}
		else
		{
			if (m_iSortingColumnTertiary == iColumn)
			{
				bFound = true;
				bRControlKey = true;
			}
		}
	}

	if (bFound) 	SetSorting(iColumn, bLControlKey, bRControlKey);
}

void CSortColumn::WebSorting(char *pcSort)
{
	char *pcFound;
	bool bFound = false;
	bool bLControlKey = false, bRControlKey = false;
	int	iColumn;

	iColumn = atoi(pcSort);
//	iColumn = m_iColumnOrder[iColumn];
	pcFound = (char *) strstr(pcSort, "><");
	if (pcFound)
	{
		pcFound+= 2;
		if (strstr(pcFound, "first"))
		{
			bFound = true;
		}
		if (strstr(pcFound, "second"))
		{
			bFound = true;
			bLControlKey = true;
		}
		if (strstr(pcFound, "third"))
		{
			bFound = true;
			bRControlKey = true;
		}
		if (bFound) SetSorting(iColumn, bLControlKey, bRControlKey);
		else ToggleSorting(iColumn);
	}
}

void CSortColumn::WriteRegistry()
{
	int iSort;

	iSort = m_iSortingOptionPrimary;
	iSort <<= 8;
	iSort += m_iSortingColumnPrimary&0xff;
	theApp.WriteProfileInt(registrySectionSorting, m_pRegistrySortPrimary, iSort);

	iSort = m_iSortingOptionSecondary;
	iSort <<= 8;
	iSort += m_iSortingColumnSecondary&0xff;
	theApp.WriteProfileInt(registrySectionSorting, m_pRegistrySortSecondary, iSort);

	iSort = m_iSortingOptionTertiary;
	iSort <<= 8;
	iSort += m_iSortingColumnTertiary&0xff;
	theApp.WriteProfileInt(registrySectionSorting, m_pRegistrySortTertiary, iSort);
}

void CSortColumn::SetSortingIndicator(int iColumnPrimary, int iSortingPrimary, int iColumnSecondary, int iSortingSecondary, int iColumnTertiary, int iSortingTertiary)
{
	char cBuffer[255];
	LV_COLUMN	lvc;
	CString		sTxt, sSort;

//	CListCtrl& listCtrl = GetListCtrl();
	
//	if (iColumnSecondary == iColumnPrimary) iColumnSecondary = -1;

	// remove the sort indicators

	for(int i = 0; i< m_iNrColomns; i++)
	{
		lvc.pszText = &cBuffer[0];
		lvc.cchTextMax = 254;
		lvc.mask = LVCF_TEXT;
		m_pListCtrl->GetColumn(i,&lvc);
		sTxt = lvc.pszText;

		sTxt.Replace(SORT_STRING_UP_SECONDARY,"");
		sTxt.Replace(SORT_STRING_DOWN_SECONDARY,"");
		sTxt.Replace(SORT_STRING_NONE_SECONDARY,"");
		sTxt.Replace(SORT_STRING_UP_TERTIARY,"");
		sTxt.Replace(SORT_STRING_DOWN_TERTIARY,"");
		sTxt.Replace(SORT_STRING_NONE_TERTIARY,"");
		sTxt.Replace(SORT_STRING_UP_PRIMARY,"");
		sTxt.Replace(SORT_STRING_DOWN_PRIMARY,"");

		lvc.iSubItem = i;
		lvc.pszText = sTxt.GetBuffer(); //_gszColumnLabel[i];
		m_pListCtrl->SetColumn(i,&lvc);
		sTxt.ReleaseBuffer();
	}

	// now insert the new one
	// primary

	if (iColumnPrimary >=0)
	{
		sSort = "";
		if (iSortingPrimary == SORTING_ZA) sSort = SORT_STRING_DOWN_PRIMARY;
		if (iSortingPrimary == SORTING_AZ) sSort = SORT_STRING_UP_PRIMARY;

		lvc.pszText = &cBuffer[0];
		lvc.cchTextMax = 254;
		lvc.mask = LVCF_TEXT;
		m_pListCtrl->GetColumn(iColumnPrimary,&lvc);
		sTxt = lvc.pszText;
		sTxt = sSort + sTxt;	

		lvc.iSubItem = iColumnPrimary;
		lvc.pszText = sTxt.GetBuffer();
		m_pListCtrl->SetColumn(iColumnPrimary,&lvc);
		sTxt.ReleaseBuffer();
	}

	// secondary

	if (iColumnSecondary >= 0)
	{
		sSort = "";
		if (iSortingSecondary == SORTING_ZA) sSort = SORT_STRING_DOWN_SECONDARY;
		if (iSortingSecondary == SORTING_AZ) sSort = SORT_STRING_UP_SECONDARY;
		if (iSortingSecondary == SORTING_NONE) sSort = SORT_STRING_NONE_SECONDARY;
		lvc.pszText = &cBuffer[0];
		lvc.cchTextMax = 254;
		lvc.mask = LVCF_TEXT;
		m_pListCtrl->GetColumn(iColumnSecondary,&lvc);
		sTxt = lvc.pszText;

		sTxt = sSort + sTxt;

		lvc.iSubItem = iColumnSecondary;
		lvc.pszText = sTxt.GetBuffer();
		m_pListCtrl->SetColumn(iColumnSecondary,&lvc);
		sTxt.ReleaseBuffer();
	}
	// secondary

	if (iColumnTertiary >= 0)
	{
		sSort = "";
		if (iSortingTertiary == SORTING_ZA) sSort = SORT_STRING_DOWN_TERTIARY;
		if (iSortingTertiary == SORTING_AZ) sSort = SORT_STRING_UP_TERTIARY;
		if (iSortingTertiary == SORTING_NONE) sSort = SORT_STRING_NONE_TERTIARY;
		lvc.pszText = &cBuffer[0];
		lvc.cchTextMax = 254;
		lvc.mask = LVCF_TEXT;
		m_pListCtrl->GetColumn(iColumnTertiary,&lvc);
		sTxt = lvc.pszText;

		sTxt = sSort + sTxt;

		lvc.iSubItem = iColumnTertiary;
		lvc.pszText = sTxt.GetBuffer();
		m_pListCtrl->SetColumn(iColumnTertiary,&lvc);
		sTxt.ReleaseBuffer();
	}

}
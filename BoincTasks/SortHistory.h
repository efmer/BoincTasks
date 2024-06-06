#pragma once

// Sort items by associated lParam

//////// History
///////////////////////////////////////////////////////////////////////////////////////
static int CALLBACK HistorySorting(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int	iPrimary, iSecondary, iTertiary;
//	char cBuffer1[32], cBuffer2[32];
	double fPrimary, fSecondary, fTertiary;

	CListData *pListData1, *pListData2;
	pListData1 = (CListData *) lParam1;
	pListData2 = (CListData *) lParam2;

	if (pListData1 == 0) return 0;
	if (pListData2 == 0) return 0;

	int iSortingColumnPrimary, iSortingOptionPrimary;
	int iSortingColumnSecondary, iSortingOptionSecondary;
	int iSortingColumnTertiary, iSortingOptionTertiary;

	int iSorting = (int) lParamSort;
	iSortingColumnPrimary = (iSorting >> 24)&0x3f;	//4+6+4+6+4
	iSortingOptionPrimary = (iSorting >> 20)&0xf;	//4+6+4+6
	iSortingColumnSecondary = (iSorting >> 14)&0x3f;//4+6+4
	iSortingOptionSecondary = (iSorting >> 10)&0xf;	//4+6
	iSortingColumnTertiary = (iSorting >> 4)&0x3f;	//4
	iSortingOptionTertiary = (iSorting >> 0)&0xf;	//0

	if (iSortingOptionTertiary != SORTING_NONE)
	{
		switch(iSortingColumnTertiary)
		{
			case COLUMN_HISTORY_PROJECT:
			case COLUMN_HISTORY_COMPUTER:
			case COLUMN_HISTORY_NAME:
			case COLUMN_HISTORY_USE:
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = _stricmp(pListData1->m_cSortTertiary,pListData2->m_cSortTertiary);
				else										iTertiary = _stricmp(pListData2->m_cSortTertiary,pListData1->m_cSortTertiary);
			break;

			case COLUMN_HISTORY_APPLICATION:
			case COLUMN_HISTORY_STATUS:
			case COLUMN_HISTORY_COMPLETED:
			case COLUMN_HISTORY_REPORTED:
			case COLUMN_HISTORY_ELAPSED:
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary= (int) (pListData1->m_iSortTertiary - pListData2->m_iSortTertiary);
				else										iTertiary = (int) (pListData2->m_iSortTertiary - pListData1->m_iSortTertiary);
			break;

			case COLUMN_HISTORY_RATIO:
			case COLUMN_HISTORY_VIRTUAL:
			case COLUMN_HISTORY_MEMORY:
				if (iSortingOptionTertiary == SORTING_AZ)	fTertiary = pListData1->m_fSortTertiary - pListData2->m_fSortTertiary;
				else										fTertiary = pListData2->m_fSortTertiary - pListData1->m_fSortTertiary;
				iTertiary = (int) (fTertiary * 1000);
			break;

//			case COLUMN_COMPUTER:		
//				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
//				iTertiary = 0;
//			break;

			default:
				iTertiary = 0;
				break;
		}
	}
	else iTertiary = 0;

	if (iSortingOptionSecondary != SORTING_NONE)
	{
		switch(iSortingColumnSecondary)
		{
			case COLUMN_HISTORY_PROJECT:
			case COLUMN_HISTORY_COMPUTER:
			case COLUMN_HISTORY_NAME:
			case COLUMN_HISTORY_USE:
				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = _stricmp(pListData1->m_cSortSecondary,pListData2->m_cSortSecondary);
				else										iSecondary = _stricmp(pListData2->m_cSortSecondary,pListData1->m_cSortSecondary);
			break;

			case COLUMN_HISTORY_APPLICATION:
			case COLUMN_HISTORY_STATUS:
			case COLUMN_HISTORY_COMPLETED:
			case COLUMN_HISTORY_REPORTED:
			case COLUMN_HISTORY_ELAPSED:
				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = (int) (pListData1->m_iSortSecondary - pListData2->m_iSortSecondary);
				else										iSecondary = (int) (pListData2->m_iSortSecondary - pListData1->m_iSortSecondary);
			break;

			case COLUMN_HISTORY_RATIO:
			case COLUMN_HISTORY_VIRTUAL:
			case COLUMN_HISTORY_MEMORY:
				if (iSortingOptionSecondary == SORTING_AZ)	fSecondary = pListData1->m_fSortSecondary - pListData2->m_fSortSecondary;
				else										fSecondary = pListData2->m_fSortSecondary - pListData1->m_fSortSecondary;
				iSecondary = (int) (fSecondary * 1000);
			break;

//			case COLUMN_COMPUTER:		
//				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
//				iSecondary = 0;
//			break;

			default:
				iSecondary = 0;
				break;
		}
	}
	else iSecondary = 0;

	// primary sorting

	if (iSortingOptionPrimary != SORTING_NONE)
	{
		switch(iSortingColumnPrimary)
		{
	
			case COLUMN_HISTORY_PROJECT:	
			case COLUMN_HISTORY_COMPUTER:
			case COLUMN_HISTORY_NAME:
			case COLUMN_HISTORY_USE:
				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = _stricmp(pListData1->m_cSortPrimary,pListData2->m_cSortPrimary);
				else										iPrimary = _stricmp(pListData2->m_cSortPrimary,pListData1->m_cSortPrimary);
			break;

			case COLUMN_HISTORY_APPLICATION:
			case COLUMN_HISTORY_STATUS:
			case COLUMN_HISTORY_COMPLETED:
			case COLUMN_HISTORY_REPORTED:
			case COLUMN_HISTORY_ELAPSED:
				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = (int) (pListData1->m_iSortPrimary - pListData2->m_iSortPrimary);
				else										iPrimary = (int) (pListData2->m_iSortPrimary- pListData1->m_iSortPrimary);
			break;

			case COLUMN_HISTORY_RATIO:
			case COLUMN_HISTORY_VIRTUAL:
			case COLUMN_HISTORY_MEMORY:
				if (iSortingOptionPrimary == SORTING_AZ)	fPrimary = pListData1->m_fSortPrimary - pListData2->m_fSortPrimary;
				else										fPrimary = pListData2->m_fSortPrimary - pListData1->m_fSortPrimary;
				iPrimary = (int) (fPrimary * 1000);
			break;

//			case COLUMN_COMPUTER:		
//				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
//				iPrimary = 0;
//			break;

			default:
				iPrimary = 0;
				break;
		}
	}
	else iPrimary = 0;

	if (iPrimary == 0)
	{
		if (iSecondary == 0) return iTertiary;
		else return iSecondary;
	}
	return iPrimary;
}
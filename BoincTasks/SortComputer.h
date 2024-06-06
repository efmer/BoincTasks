#pragma once

// Sort items by associated lParam

//////// Computer
///////////////////////////////////////////////////////////////////////////////////////
static int CALLBACK ComputerSorting(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int	iPrimary, iSecondary, iTertiary;

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
			case COLUMN_COMPUTER_GROUP:
			case COLUMN_COMPUTER_ID:
			case COLUMN_COMPUTER_IP:
			case COLUMN_COMPUTER_MAC:
			case COLUMN_COMPUTER_PORT:
			case COLUMN_COMPUTER_BOINC:
			case COLUMN_COMPUTER_BOINC_PLATFORM:
			case COLUMN_COMPUTER_TTHROTTLE:
			case COLUMN_COMPUTER_STATUS:
//			case COLUMN_COMPUTER_DAYS:
//			case COLUMN_COMPUTER_WU:
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = pListData1->m_sSortTertiary.CompareNoCase(pListData2->m_sSortTertiary);
				else										iTertiary = pListData2->m_sSortTertiary.CompareNoCase(pListData1->m_sSortTertiary);
			break;
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
			case COLUMN_COMPUTER_GROUP:
			case COLUMN_COMPUTER_ID:
			case COLUMN_COMPUTER_IP:
			case COLUMN_COMPUTER_MAC:
			case COLUMN_COMPUTER_PORT:
			case COLUMN_COMPUTER_BOINC:
			case COLUMN_COMPUTER_BOINC_PLATFORM:
			case COLUMN_COMPUTER_TTHROTTLE:
			case COLUMN_COMPUTER_STATUS:
//			case COLUMN_COMPUTER_DAYS:
//			case COLUMN_COMPUTER_WU:
				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = pListData1->m_sSortSecondary.CompareNoCase(pListData2->m_sSortSecondary);
				else										iSecondary = pListData2->m_sSortSecondary.CompareNoCase(pListData1->m_sSortSecondary);
			break;
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
			case COLUMN_COMPUTER_GROUP:
			case COLUMN_COMPUTER_ID:
			case COLUMN_COMPUTER_IP:
			case COLUMN_COMPUTER_MAC:
			case COLUMN_COMPUTER_PORT:
			case COLUMN_COMPUTER_BOINC:
			case COLUMN_COMPUTER_BOINC_PLATFORM:
			case COLUMN_COMPUTER_TTHROTTLE:
			case COLUMN_COMPUTER_STATUS:
//			case COLUMN_COMPUTER_DAYS:
//			case COLUMN_COMPUTER_WU:
				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = pListData1->m_sSortPrimary.CompareNoCase(pListData2->m_sSortPrimary);
				else										iPrimary = pListData2->m_sSortPrimary.CompareNoCase(pListData1->m_sSortPrimary);
			break;

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

	return 0;
}

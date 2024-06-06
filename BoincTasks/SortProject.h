#pragma once

// Sort items by associated lParam

//////// Project
///////////////////////////////////////////////////////////////////////////////////////
static int CALLBACK ProjectSorting(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int	iPrimary, iSecondary, iTertiary;
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
			case COLUMN_PROJECT_PROJECT:
			case COLUMN_PROJECT_ACCOUNT:
			case COLUMN_PROJECT_TEAM:
			case COLUMN_PROJECT_VENUE:
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = _stricmp(pListData1->m_cSortTertiary,pListData2->m_cSortTertiary);
				else										iTertiary = _stricmp(pListData2->m_cSortTertiary,pListData1->m_cSortTertiary);
			break;
			
			case COLUMN_PROJECT_USER_CREDITS:
			case COLUMN_PROJECT_USER_A_CREDITS:
			case COLUMN_PROJECT_HOST_CREDITS:
			case COLUMN_PROJECT_HOST_A_CREDITS:
			case COLUMN_PROJECT_SHARE:
				if (iSortingOptionTertiary == SORTING_AZ)	fTertiary = pListData1->m_fSortTertiary - pListData2->m_fSortTertiary;
				else										fTertiary = pListData2->m_fSortTertiary - pListData1->m_fSortTertiary;
				if (fTertiary > 0) iTertiary = 1;
				else
				{
					if (fTertiary < 0) iTertiary = -1;
					else iTertiary = 0;
				}
			break;

			case COLUMN_PROJECT_TASKS:
			case COLUMN_PROJECT_TIME_LEFT:	// sort on integer !!!!
			case COLUMN_PROJECT_TASKS_DAY:
			case COLUMN_PROJECT_TASKS_WEEK:
			case COLUMN_PROJECT_RANK:
			case COLUMN_PROJECT_TEAM_RANK:
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = (int) (pListData1->m_iSortTertiary - pListData2->m_iSortTertiary);
				else										iTertiary = (int) (pListData2->m_iSortTertiary - pListData1->m_iSortTertiary);
			break;

			case COLUMN_PROJECT_COMPUTER:		
				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
				iTertiary = 0;
			break;

			case COLUMN_PROJECT_STATUS:
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
			case COLUMN_PROJECT_PROJECT:
			case COLUMN_PROJECT_ACCOUNT:
			case COLUMN_PROJECT_TEAM:
			case COLUMN_PROJECT_VENUE:
				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = _stricmp(pListData1->m_cSortSecondary,pListData2->m_cSortSecondary);
				else										iSecondary = _stricmp(pListData2->m_cSortSecondary,pListData1->m_cSortSecondary);
			break;
		
			case COLUMN_PROJECT_USER_CREDITS:
			case COLUMN_PROJECT_USER_A_CREDITS:
			case COLUMN_PROJECT_HOST_CREDITS:
			case COLUMN_PROJECT_HOST_A_CREDITS:
			case COLUMN_PROJECT_SHARE:
				if (iSortingOptionSecondary == SORTING_AZ)	fSecondary = pListData1->m_fSortSecondary - pListData2->m_fSortSecondary;
				else										fSecondary = pListData2->m_fSortSecondary - pListData1->m_fSortSecondary;
				if (fSecondary > 0) iSecondary = 1;
				else
				{
					if (fSecondary < 0) iSecondary = -1;
					else iSecondary = 0;
				}
			break;

			case COLUMN_PROJECT_TASKS:
			case COLUMN_PROJECT_TIME_LEFT:	// sort on integer !!!!
			case COLUMN_PROJECT_TASKS_DAY:
			case COLUMN_PROJECT_TASKS_WEEK:
			case COLUMN_PROJECT_RANK:
			case COLUMN_PROJECT_TEAM_RANK:
				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = (int) (pListData1->m_iSortSecondary - pListData2->m_iSortSecondary);
				else										iSecondary = (int) (pListData2->m_iSortSecondary - pListData1->m_iSortSecondary);
			break;

			case COLUMN_PROJECT_COMPUTER:		
				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
				iSecondary = 0;
			break;

			case COLUMN_PROJECT_STATUS:
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
			case COLUMN_PROJECT_PROJECT:
			case COLUMN_PROJECT_ACCOUNT:
			case COLUMN_PROJECT_TEAM:
			case COLUMN_PROJECT_VENUE:
				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = _stricmp(pListData1->m_cSortPrimary,pListData2->m_cSortPrimary);
				else										iPrimary = _stricmp(pListData2->m_cSortPrimary,pListData1->m_cSortPrimary);
			break;

			case COLUMN_PROJECT_USER_CREDITS:
			case COLUMN_PROJECT_USER_A_CREDITS:
			case COLUMN_PROJECT_HOST_CREDITS:
			case COLUMN_PROJECT_HOST_A_CREDITS:
			case COLUMN_PROJECT_SHARE:
				if (iSortingOptionPrimary == SORTING_AZ)	fPrimary = pListData1->m_fSortPrimary - pListData2->m_fSortPrimary;
				else										fPrimary = pListData2->m_fSortPrimary - pListData1->m_fSortPrimary;
				if (fPrimary > 0) iPrimary = 1;
				else
				{
					if (fPrimary < 0) iPrimary = -1;
					else iPrimary = 0;
				}
			break;

			case COLUMN_PROJECT_TASKS:
			case COLUMN_PROJECT_TIME_LEFT:	// sort on integer !!!!
			case COLUMN_PROJECT_TASKS_DAY:
			case COLUMN_PROJECT_TASKS_WEEK:
			case COLUMN_PROJECT_RANK:
			case COLUMN_PROJECT_TEAM_RANK:
				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = (int) (pListData1->m_iSortPrimary - pListData2->m_iSortPrimary);
				else										iPrimary = (int) (pListData2->m_iSortPrimary- pListData1->m_iSortPrimary);
			break;

			case COLUMN_PROJECT_COMPUTER:		
				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
				iPrimary = 0;
			break;

			case COLUMN_PROJECT_STATUS:
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
}
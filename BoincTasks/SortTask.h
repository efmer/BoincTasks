#pragma once

// Sort items by associated lParam

//////// Tasks
///////////////////////////////////////////////////////////////////////////////////////
static int CALLBACK TaskSorting(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int	iPrimary, iSecondary, iTertiary;
	char cBuffer1[32], cBuffer2[32];
	double fPrimary, fSecondary, fTertiary;

	CListData *pListData1, *pListData2;
	pListData1 = (CListData *) lParam1;
	pListData2 = (CListData *) lParam2;

	if (pListData1 == 0) return 0;
	if (pListData2 == 0) return 0;

	int iSortingColumnPrimary, iSortingOptionPrimary;
	int iSortingColumnSecondary, iSortingOptionSecondary;
	int iSortingColumnTertiary, iSortingOptionTertiary;

	UINT iSorting = (UINT) lParamSort;
	iSortingColumnPrimary = (iSorting >> 24)&0x3f;	//4+6+4+6+4
	iSortingOptionPrimary = (iSorting >> 20)&0xf;	//4+6+4+6
	iSortingColumnSecondary = (iSorting >> 14)&0x3f;//4+6+4
	iSortingOptionSecondary = (iSorting >> 10)&0xf;	//4+6
	iSortingColumnTertiary = (iSorting >> 4)&0x3f;	//4
	iSortingOptionTertiary = (iSorting >> 0)&0xf;	//0

	if (iSortingOptionTertiary != SORTING_NONE)
	{
		switch (iSortingColumnTertiary)
		{
		case COLUMN_PROJECT:
			if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = _stricmp(pListData1->m_cSortTertiary, pListData2->m_cSortTertiary);
			else										iTertiary = _stricmp(pListData2->m_cSortTertiary, pListData1->m_cSortTertiary);
			break;

		case COLUMN_NAME:
			if (pListData1->m_iSortTertiary > 0)
			{
				_sntprintf_s(cBuffer1, 30, _TRUNCATE, "%d <Tasks>", pListData1->m_iSortTertiary); pListData1->m_cSortTertiary = cBuffer1;
			}
			if (pListData2->m_iSortTertiary > 0)
			{
				_snprintf_s(cBuffer2, 30, _TRUNCATE, "%d <Tasks>", pListData2->m_iSortTertiary); pListData2->m_cSortTertiary = cBuffer2;
			}

			if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = _stricmp(pListData1->m_cSortTertiary, pListData2->m_cSortTertiary);
			else										iTertiary = _stricmp(pListData2->m_cSortTertiary, pListData1->m_cSortTertiary);
			break;

		case COLUMN_APPLICATION:
			if (theApp.m_pDlgSettingsTasks->m_bSortOnApplicationNr)
			{
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = (int)(pListData1->m_iSortTertiary - pListData2->m_iSortTertiary);
				else										iTertiary = (int)(pListData2->m_iSortTertiary - pListData1->m_iSortTertiary);
			}
			else
			{			
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = _stricmp(pListData1->m_sSortTertiary, pListData2->m_sSortTertiary);
				else										iTertiary = _stricmp(pListData2->m_sSortTertiary, pListData1->m_sSortTertiary);
			}
			break;

			case COLUMN_USE:
			case COLUMN_DEADLINE:
			case COLUMN_TIME_LEFT:
			case COLUMN_ELAPSED:
			case COLUMN_TTHROTTLE:
			case COLUMN_TEMPERATURE:
			case COLUMN_STATE:	
				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary= (int) (pListData1->m_iSortTertiary - pListData2->m_iSortTertiary);
				else										iTertiary = (int) (pListData2->m_iSortTertiary - pListData1->m_iSortTertiary);
			break;

			case COLUMN_CPU_PERCENTAGE:
			case COLUMN_PERCENTAGE:
			case COLUMN_MEMORY:
				if (iSortingOptionTertiary == SORTING_AZ)	fTertiary = pListData1->m_fSortTertiary - pListData2->m_fSortTertiary;
				else										fTertiary = pListData2->m_fSortTertiary - pListData1->m_fSortTertiary;
				iTertiary = (int) (fTertiary * 10000);
			break;

			case COLUMN_SHARE:
				if (iSortingOptionTertiary == SORTING_AZ)	fTertiary = pListData1->m_fSortTertiary - pListData2->m_fSortTertiary;
				else										fTertiary = pListData2->m_fSortTertiary - pListData1->m_fSortTertiary;
				iTertiary = (int)(fTertiary * 10);
				break;

			case COLUMN_DEBT:
			case COLUMN_CHECKPOINT:
			case COLUMN_RECEIVED:
			case COLUMN_VIRTUAL_MEMORY:
				if (iSortingOptionTertiary == SORTING_AZ)	fTertiary = pListData1->m_fSortTertiary - pListData2->m_fSortTertiary;
				else										fTertiary = pListData2->m_fSortTertiary - pListData1->m_fSortTertiary;
				iTertiary = (int) (fTertiary * 1000);
			break;

			case COLUMN_COMPUTER:		
				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
				iTertiary = 0;
			break;

//			case COLUMN_STATE:	
//				if (iSortingOptionTertiary == SORTING_AZ)	iTertiary = pListData1->m_sSortTertiary.CompareNoCase(pListData2->m_sSortTertiary);
//				else										iTertiary = pListData2->m_sSortTertiary.CompareNoCase(pListData1->m_sSortTertiary);
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
			case COLUMN_PROJECT:
				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = _stricmp(pListData1->m_cSortSecondary,pListData2->m_cSortSecondary);
				else										iSecondary = _stricmp(pListData2->m_cSortSecondary,pListData1->m_cSortSecondary);
			break;

			case COLUMN_NAME:
				if (pListData1->m_iSortSecondary > 0)
				{
					_snprintf_s(cBuffer1,20,_TRUNCATE,"%d <Tasks>", pListData1->m_iSortSecondary); pListData1->m_cSortSecondary = cBuffer1;
				}
				if (pListData2->m_iSortSecondary > 0)
				{
					_snprintf_s(cBuffer2,20,_TRUNCATE,"%d <Tasks>", pListData2->m_iSortSecondary); pListData2->m_cSortSecondary = cBuffer2;
				}

				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = _stricmp(pListData1->m_cSortSecondary,pListData2->m_cSortSecondary);
				else										iSecondary = _stricmp(pListData2->m_cSortSecondary,pListData1->m_cSortSecondary);
			break;

			case COLUMN_APPLICATION:
				if (theApp.m_pDlgSettingsTasks->m_bSortOnApplicationNr)
				{
					if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = (int)(pListData1->m_iSortSecondary - pListData2->m_iSortSecondary);
					else										iSecondary = (int)(pListData2->m_iSortSecondary - pListData1->m_iSortSecondary);
				}
				else
				{
					if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = _stricmp(pListData1->m_sSortSecondary, pListData2->m_sSortSecondary);
					else										iSecondary = _stricmp(pListData2->m_sSortSecondary, pListData1->m_sSortSecondary);
				}
			break;

			case COLUMN_USE:
			case COLUMN_DEADLINE:
			case COLUMN_TIME_LEFT:
			case COLUMN_ELAPSED:
			case COLUMN_TTHROTTLE:
			case COLUMN_TEMPERATURE:
			case COLUMN_STATE:	
				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = (int) (pListData1->m_iSortSecondary - pListData2->m_iSortSecondary);
				else										iSecondary = (int) (pListData2->m_iSortSecondary - pListData1->m_iSortSecondary);
			break;

			case COLUMN_CPU_PERCENTAGE:
			case COLUMN_PERCENTAGE:
			case COLUMN_MEMORY:
				if (iSortingOptionSecondary == SORTING_AZ)	fSecondary = pListData1->m_fSortSecondary - pListData2->m_fSortSecondary;
				else										fSecondary = pListData2->m_fSortSecondary - pListData1->m_fSortSecondary;
				iSecondary = (int) (fSecondary * 10000);
			break;

			case COLUMN_SHARE:
				if (iSortingOptionSecondary == SORTING_AZ)	fSecondary = pListData1->m_fSortSecondary - pListData2->m_fSortSecondary;
				else										fSecondary = pListData2->m_fSortSecondary - pListData1->m_fSortSecondary;
				iSecondary = (int)(fSecondary * 10);
				break;

			case COLUMN_DEBT:
			case COLUMN_CHECKPOINT:
			case COLUMN_RECEIVED:
			case COLUMN_VIRTUAL_MEMORY:
				if (iSortingOptionSecondary == SORTING_AZ)	fSecondary = pListData1->m_fSortSecondary - pListData2->m_fSortSecondary;
				else										fSecondary = pListData2->m_fSortSecondary - pListData1->m_fSortSecondary;
				iSecondary = (int) (fSecondary * 1000);
			break;

			case COLUMN_COMPUTER:		
				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
				iSecondary = 0;
			break;

//			case COLUMN_STATE:	
//				if (iSortingOptionSecondary == SORTING_AZ)	iSecondary = pListData1->m_sSortSecondary.CompareNoCase(pListData2->m_sSortSecondary);
//				else										iSecondary = pListData2->m_sSortSecondary.CompareNoCase(pListData1->m_sSortSecondary);
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
	
			case COLUMN_PROJECT:	
				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = _stricmp(pListData1->m_cSortPrimary,pListData2->m_cSortPrimary);
				else										iPrimary = _stricmp(pListData2->m_cSortPrimary,pListData1->m_cSortPrimary);
			break;

			case COLUMN_NAME:
				if (pListData1->m_iSortPrimary > 0)
				{
					_snprintf_s(cBuffer1,20,_TRUNCATE,"%d [Tasks]", pListData1->m_iSortPrimary); pListData1->m_cSortPrimary = cBuffer1;
				}
				if (pListData2->m_iSortPrimary > 0)
				{
					_snprintf_s(cBuffer2,20,_TRUNCATE,"%d [Tasks]", pListData2->m_iSortPrimary); pListData2->m_cSortPrimary = cBuffer2;
				}

				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = _stricmp(pListData1->m_cSortPrimary,pListData2->m_cSortPrimary);
				else										iPrimary = _stricmp(pListData2->m_cSortPrimary,pListData1->m_cSortPrimary);
			break;

			case COLUMN_APPLICATION:
				if (theApp.m_pDlgSettingsTasks->m_bSortOnApplicationNr)
				{
					if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = (int)(pListData1->m_iSortPrimary - pListData2->m_iSortPrimary);
					else										iPrimary = (int)(pListData2->m_iSortPrimary - pListData1->m_iSortPrimary);
				}
				else
				{
					if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = _stricmp(pListData1->m_sSortPrimary, pListData2->m_sSortPrimary);
					else										iPrimary = _stricmp(pListData2->m_sSortPrimary, pListData1->m_sSortPrimary);
				}
			break;

			case COLUMN_USE:
			case COLUMN_DEADLINE:
			case COLUMN_TIME_LEFT:
			case COLUMN_ELAPSED:
			case COLUMN_TTHROTTLE:
			case COLUMN_TEMPERATURE:
			case COLUMN_STATE:	
				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = (int) (pListData1->m_iSortPrimary - pListData2->m_iSortPrimary);
				else										iPrimary = (int) (pListData2->m_iSortPrimary- pListData1->m_iSortPrimary);
			break;

			case COLUMN_CPU_PERCENTAGE:
			case COLUMN_PERCENTAGE:
			case COLUMN_MEMORY:
				if (iSortingOptionPrimary == SORTING_AZ)	fPrimary = pListData1->m_fSortPrimary - pListData2->m_fSortPrimary;
				else										fPrimary = pListData2->m_fSortPrimary - pListData1->m_fSortPrimary;
				iPrimary = (int) (fPrimary * 10000);
			break;

			case COLUMN_SHARE:
				if (iSortingOptionPrimary == SORTING_AZ)	fPrimary = pListData1->m_fSortPrimary - pListData2->m_fSortPrimary;
				else										fPrimary = pListData2->m_fSortPrimary - pListData1->m_fSortPrimary;
				iPrimary = (int)(fPrimary * 10);
				break;

			case COLUMN_DEBT:
			case COLUMN_CHECKPOINT:
			case COLUMN_RECEIVED:
			case COLUMN_VIRTUAL_MEMORY:
				if (iSortingOptionPrimary == SORTING_AZ)	fPrimary = pListData1->m_fSortPrimary - pListData2->m_fSortPrimary;
				else										fPrimary = pListData2->m_fSortPrimary - pListData1->m_fSortPrimary;
				iPrimary = (int) (fPrimary * 1000);
			break;

			case COLUMN_COMPUTER:		
				if (pListData1->m_iComputerId != pListData2->m_iComputerId) return 0;
				iPrimary = 0;
			break;

//			case COLUMN_STATE:	
///				if (iSortingOptionPrimary == SORTING_AZ)	iPrimary = pListData1->m_sSortPrimary.CompareNoCase(pListData2->m_sSortPrimary);
//				else										iPrimary = pListData2->m_sSortPrimary.CompareNoCase(pListData1->m_sSortPrimary);
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
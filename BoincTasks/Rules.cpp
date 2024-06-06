// Rules.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "Rules.h"
#include "Xml.h"
#include "Translation.h"

// CRules

CRules::CRules()
{
}

CRules::~CRules()
{
}


// CRules member functions


int CRules::Type(CString *psType)
{
	if (*psType == gszTranslation[PosDialogRulesTypeElapsed])		return RULES_TYPE_ELAPSED;
	if (*psType == gszTranslation[PosDialogRulesTypeCpuP])			return RULES_TYPE_CPUP;
	if (*psType == gszTranslation[PosDialogRulesTypeProgress])		return RULES_TYPE_PROGRESS;
	if (*psType == gszTranslation[PosDialogRulesTypeProgressDelta])	return RULES_TYPE_PROGRESS_DELTA;
	if (*psType == gszTranslation[PosDialogRulesTypeTimeLeft])		return RULES_TYPE_TIMELEFT;
	if (*psType == gszTranslation[PosDialogRulesTypeConnection])	return RULES_TYPE_CONNECTION;
	if (*psType == gszTranslation[PosDialogRulesTypeUse])			return RULES_TYPE_USE;
	if (*psType == gszTranslation[PosDialogRulesTypeTemperature])	return RULES_TYPE_TEMPERATURE;
	if (*psType == gszTranslation[PosDialogRulesTypeStatus])		return RULES_TYPE_STATUS;
	if (*psType == gszTranslation[PosDialogRulesTypeTime])			return RULES_TYPE_TIME_INTERVAL;
	if (*psType == gszTranslation[PosDialogRulesTypeDeadline])		return RULES_TYPE_DEADLINE;
	if (*psType == gszTranslation[PosDialogRulesTypeTimeLeftProject]) return RULES_TYPE_TIMELEFT_PROJECT;

	return RULES_TYPE_NONE;
}

void CRules::Type(int iType, CString *psType)
{
	switch (iType)
	{
		case RULES_TYPE_ELAPSED:
			*psType = gszTranslation[PosDialogRulesTypeElapsed];
		break;
		case RULES_TYPE_CPUP:
			*psType = gszTranslation[PosDialogRulesTypeCpuP];
		break;
		case RULES_TYPE_PROGRESS:
			*psType = gszTranslation[PosDialogRulesTypeProgress];
		break;
		case RULES_TYPE_PROGRESS_DELTA:
			*psType = gszTranslation[PosDialogRulesTypeProgressDelta];
		break;
		case RULES_TYPE_TIMELEFT:
			*psType = gszTranslation[PosDialogRulesTypeTimeLeft];
		break;
		case RULES_TYPE_CONNECTION:
			*psType = gszTranslation[PosDialogRulesTypeConnection];
		break;
		case RULES_TYPE_USE:
			*psType = gszTranslation[PosDialogRulesTypeUse];
		break;
		case RULES_TYPE_TEMPERATURE:
			*psType = gszTranslation[PosDialogRulesTypeTemperature];
		break;
		case RULES_TYPE_STATUS:
			*psType = gszTranslation[PosDialogRulesTypeStatus];
		break;
		case RULES_TYPE_TIME_INTERVAL:
			*psType = gszTranslation[PosDialogRulesTypeTime];
		break;
		case RULES_TYPE_DEADLINE:
			*psType = gszTranslation[PosDialogRulesTypeDeadline];
		break;
		case RULES_TYPE_TIMELEFT_PROJECT:
			*psType = gszTranslation[PosDialogRulesTypeTimeLeftProject];
		break;

		default:
			*psType = "??";
	}
}

int CRules::TypeClass(int iType)
{
	int iClass;

	switch (iType)
	{
		case RULES_TYPE_ELAPSED:
			iClass = RULE_TYPE_CLASS_TIME_DAY;
		break;
		case RULES_TYPE_CPUP:
			iClass = RULE_TYPE_CLASS_PERCENTAGE;
		break;
		case RULES_TYPE_PROGRESS:
		case RULES_TYPE_PROGRESS_DELTA:
			iClass = RULE_TYPE_CLASS_PERCENTAGE;
		break;
		case RULES_TYPE_TIMELEFT:
			iClass = RULE_TYPE_CLASS_TIME_DAY;
		break;
		case RULES_TYPE_CONNECTION:
			iClass = RULE_TYPE_CLASS_CONNECTION;
		break;
		case RULES_TYPE_USE:
			iClass = RULE_TYPE_CLASS_USE;
		break;
		case RULES_TYPE_TEMPERATURE:
			iClass = RULE_TYPE_CLASS_TEMPERATURE;
		break;
		case RULES_TYPE_STATUS:
			iClass = RULE_TYPE_CLASS_STATUS;
		break;
		case RULES_TYPE_TIME_INTERVAL:
			iClass = RULE_TYPE_CLASS_INTERVAL;
		break;
		case RULES_TYPE_DEADLINE:
			iClass = RULE_TYPE_CLASS_TIME_DAY;
		break;
		case RULES_TYPE_TIMELEFT_PROJECT:
			iClass = RULE_TYPE_CLASS_TIME_DAY;
		break;

		default:
			iClass = RULE_TYPE_CLASS_UNKNOWN;

	}
	return iClass;
}

void CRules::Operator(int iType, int iOperator, CString *psOperator)
{
	if (iType == RULES_TYPE_TIME_INTERVAL)
	{
		*psOperator = "";
		return;
	}

	switch (iOperator)
	{
		case RULE_OPERATOR_IS:
			*psOperator = "=";
		break;
		case RULE_OPERATOR_MORE:
			*psOperator = ">";
		break;
		case RULE_OPERATOR_LESS:
			*psOperator = "<";
		break;
		case RULE_OPERATOR_NOTEQUAL:
			*psOperator = "<>";
		break;
		case RULE_OPERATOR_NOLONGEREQUAL:
			*psOperator = "=<>";
		break;
		default:
			*psOperator = "??";
	}
}

int CRules::EventType(CString *psEvent)
{
	if (*psEvent == gszTranslation[PosDialogRulesEventSuspendProject])	return EVENT_TYPE_SUSPEND_PROJECT;
	if (*psEvent == gszTranslation[PosDialogRulesEventResumeProject])	return EVENT_TYPE_RESUME_PROJECT;
	if (*psEvent == gszTranslation[PosDialogRulesEventSnooze])			return EVENT_TYPE_SNOOZE;
	if (*psEvent == gszTranslation[PosDialogRulesEventSnoozeGpu])		return EVENT_TYPE_SNOOZE_GPU;
	if (*psEvent == gszTranslation[PosDialogRulesEventSuspendTask])		return EVENT_TYPE_SUSPEND_TASK;
	if (*psEvent == gszTranslation[PosDialogRulesEventNoNewWork])		return EVENT_TYPE_NO_NEW_WORK;
	if (*psEvent == gszTranslation[PosDialogRulesEventAllowNewWork])	return EVENT_TYPE_ALLOW_NEW_WORK;
	if (*psEvent == gszTranslation[PosDialogRulesEventProgram])			return EVENT_TYPE_PROGRAM;

	if (*psEvent == gszTranslation[PosDialogRulesEventSuspendNetwork])	return EVENT_TYPE_SUSPEND_NETWORK;
	if (*psEvent == gszTranslation[PosDialogRulesEventResumeNetwork])	return EVENT_TYPE_RESUME_NETWORK;


	return EVENT_NONE;
}

void CRules::EventType(int iType, CString *psType)
{
	switch (iType)
	{
		case EVENT_TYPE_SUSPEND_PROJECT:
			*psType = gszTranslation[PosDialogRulesEventSuspendProject];
		break;
		case EVENT_TYPE_RESUME_PROJECT:
			*psType = gszTranslation[PosDialogRulesEventResumeProject];
		break;
		case EVENT_TYPE_SNOOZE:
			*psType = gszTranslation[PosDialogRulesEventSnooze];
		break;
		case EVENT_TYPE_CANCEL_SNOOZE:
			*psType = "X ";
			*psType+= gszTranslation[PosDialogRulesEventSnooze];
		break;
		case EVENT_TYPE_SNOOZE_GPU:
			*psType = gszTranslation[PosDialogRulesEventSnoozeGpu];
		break;
		case EVENT_TYPE_CANCEL_SNOOZE_GPU:
			*psType = "X ";
			*psType+= gszTranslation[PosDialogRulesEventSnoozeGpu];
		break;
		case EVENT_TYPE_NO_NEW_WORK:
			*psType = gszTranslation[PosDialogRulesEventNoNewWork];
		break;
		case EVENT_TYPE_ALLOW_NEW_WORK:
			*psType = gszTranslation[PosDialogRulesEventAllowNewWork];
		break;
		case EVENT_TYPE_SUSPEND_TASK:
			*psType = gszTranslation[PosDialogRulesEventSuspendTask];
		break;
		case EVENT_TYPE_PROGRAM:
			*psType = gszTranslation[PosDialogRulesEventProgram];
		break;
		case EVENT_TYPE_SUSPEND_NETWORK:
			*psType = gszTranslation[PosDialogRulesEventSuspendNetwork];
		break;
		case EVENT_TYPE_RESUME_NETWORK:
			*psType = gszTranslation[PosDialogRulesEventResumeNetwork];
		break;

		break;
		default:
			*psType = "??";
	}
}

int CRules::EventClass(int iType)
{
	int iClass;

	switch (iType)
	{
		case EVENT_TYPE_SUSPEND_PROJECT:
		case EVENT_TYPE_RESUME_PROJECT:
		case EVENT_TYPE_SNOOZE:
		case EVENT_TYPE_SNOOZE_GPU:
		case EVENT_TYPE_SUSPEND_TASK:
		case EVENT_TYPE_NO_NEW_WORK:
		case EVENT_TYPE_ALLOW_NEW_WORK:
		case EVENT_TYPE_SUSPEND_NETWORK:
		case EVENT_TYPE_RESUME_NETWORK:
			iClass = EVENT_CLASS_INTERNAL;
		break;
		case EVENT_TYPE_PROGRAM:
			iClass = EVENT_CLASS_EXTERNAL;
		break;
		default:
			iClass = EVENT_CLASS_UNKNOWN;
	}
	return iClass;
}

void CRules::Value(int iTypeClass, _int64 iValue, double dValue, CString *psText)
{
	int		iValueTemp;

	switch (iTypeClass)
	{
		case RULE_TYPE_CLASS_TIME_DAY:
			TimeString(iValue, psText);
		break;
		case RULE_TYPE_CLASS_PERCENTAGE:
			iValueTemp = (int) dValue;
			if ((double) iValueTemp == dValue)
			{
				psText->Format("%.0f %%" , dValue);
			}
			else
			{
				psText->Format("%.2f %%" , dValue);
			}
		break;
		case RULE_TYPE_CLASS_TEMPERATURE:
			iValueTemp = (int) dValue;
			if ((double) iValueTemp == dValue)
			{
				psText->Format("%.0f °C" , dValue);
			}
			else
			{
				psText->Format("%.1f °C" , dValue);
			}
		break;
		case RULE_TYPE_CLASS_USE:
			iValueTemp = (int) dValue;
			if ((double) iValueTemp == dValue)
			{
				psText->Format("%.0f Cpu + %.0f Gpu" ,dValue, (double) (iValue/1000));
			}
			else
			{
				psText->Format("%.2f Cpu + %.0f Gpu" ,dValue, (double) (iValue/1000));
			}
		break;
		case RULE_TYPE_CLASS_STATUS:
			iValueTemp = (int) dValue;
			psText->Format("%d,%d" ,iValueTemp, iValue);
		break;
		case RULE_TYPE_CLASS_INTERVAL:

		break;
		default:
			*psText = "";
	}

}

void CRules::TimeString(__int64 iTime, CString *psTime)
{
	int		iDay, iHour, iMinute, iSecond;
	bool	bNegative;

	if (iTime < 0)
	{
		bNegative = true;
		iTime = abs(iTime);
	}
	else bNegative = false;

	TimeExtract(iTime, &iDay, &iHour, &iMinute, &iSecond);

	if (iDay > 0 || iDay < 1) psTime->Format("%02dd,%02d:%02d:%02d",iDay,iHour,iMinute,iSecond);
	else psTime->Format("%02d:%02d:%02d", iHour,iMinute,iSecond);

	if (bNegative) *psTime = "-" + *psTime;
}

void CRules::TimeExtract(__int64 iTime, int *piDay, int *piHour, int *piMinute, int *piSecond)
{
		*piMinute = (int) (iTime / 60);
		*piSecond = (int) iTime - (*piMinute * 60) ;
		*piHour	= *piMinute / 60;
		*piMinute = *piMinute - (*piHour * 60);
		*piDay	= *piHour / 24;
		*piHour	= *piHour - (*piDay * 24);
}

void CRules::EventExtract(CString *psEvent, CString *psProgram, CString *psParameters)
{
	int iFound;

	*psParameters = "";

	iFound = psEvent->Find(",");
	if (iFound >= 0)
	{
		*psProgram = psEvent->Left(iFound);
		*psParameters = psEvent->Mid(iFound);
	}
	else 
	{
		*psProgram = *psEvent;
	}

}

void CRules::EventProgramFullPath(CString *psProgram, CString *psFullPath)
{
	int iFound;
	CString sPath, sDirectory;
	CXml	xml("");


	sPath = theApp.GetProfileString(registrySectionSettings,registryPath, "");

	sDirectory = xml.GetUserDirectory(sPath,false );

	iFound = psProgram->Find(":");
	if (iFound >= 0)
	{
		*psFullPath = *psProgram;
		return;
	}
	sDirectory = xml.GetUserDirectory(sPath,false );
	*psFullPath = sDirectory + "\\" + *psProgram;
}

bool CRules::EventProgramCheckValid(CString *psProgramFull)
{
	CFile	file;

	CFileException e;
	if( !file.Open(psProgramFull->GetBuffer(), CFile::modeRead, &e ) )
	{
		return false;
	}
	file.Close();
	return true;
}

bool CRules::Event(int iEvent, CString *psEvent, CString *psResult)
{
	BOOL	bResult;
	CString sProgram, sParameters, sFormat;

	SHELLEXECUTEINFO info;
    memset(&info, 0, sizeof(SHELLEXECUTEINFO));

	EventExtract(psEvent, &sProgram, &sParameters);
	if (sProgram.GetLength() > 0)
	{
		info.cbSize = sizeof(SHELLEXECUTEINFO);
		info.fMask = SEE_MASK_FLAG_NO_UI;
		info.lpVerb = _T ("open");
		info.lpFile = sProgram;
		info.lpParameters = sParameters;
		info.nShow = SW_SHOW;
	    bResult = ShellExecuteEx ( &info );
		if (bResult == FALSE)
		{
			*psResult = "ERROR: Program did not run: " + sProgram;
			int iError;
			iError = GetLastError();
			sFormat.Format("Error code: %d", iError);
			return false;
		}
	}
	else
	{
		*psResult = "ERROR: No program (length = 0)";
		return false;
	}
	*psResult = "Program executed ok: " + sProgram + " (" + sParameters + ")";
	return true;
}

bool CRules::InInterval(int iDayCurrent, int iHourCurrent, int iMinuteCurrent, CRuleIntervalItem *pRuleIntervalItem)
{
	int iDay, iHour, iMinute, iSecond;

	TimeExtract(pRuleIntervalItem->m_iStartTime, &iDay, &iHour, &iMinute, &iSecond);

	if (iDayCurrent != iDay) return false;
	if (iHourCurrent < iHour) return false;
	if (iHourCurrent == iHour)
	{
		if (iMinuteCurrent < iMinute) return false;
	}

	TimeExtract(pRuleIntervalItem->m_iStopTime, &iDay, &iHour, &iMinute, &iSecond);

	if (iHourCurrent > iHour) return false;
	if (iHourCurrent == iHour)
	{
		if (iMinuteCurrent > iMinute) return false;
	}

	return true;
}


void CRules::IntervalString(std::deque<CRuleIntervalItem *> *plInterval, CString *psText)
{
	int	iLineCount;

	*psText = "";

	if (plInterval == NULL) return;

	iLineCount = 0;

	for (int iInterval = 0; iInterval < (int) plInterval->size(); iInterval++)
	{
		if (iLineCount++ == 2)
		{
			iLineCount = 0;
			*psText += "\r\n";
		}

		if (iInterval >= 8)
		{
			*psText += "...";
			return;
		}
		IntervalStringSingle(plInterval->at(iInterval), psText);
	}
}

void CRules::IntervalStringSingle(CRuleIntervalItem *pRuleIntervalItem, CString *psText)
{
	CString sTimeStart, sTimeStop;
	
	TimeString(pRuleIntervalItem->m_iStartTime,&sTimeStart);
	TimeString(pRuleIntervalItem->m_iStopTime,&sTimeStop);
	*psText += "[" + sTimeStart + ", " + sTimeStop + "]";
}

void CRules::DeleteInterval(std::deque<CRuleIntervalItem *> **plInterval)
{
//	std::deque<CRuleIntervalItem *> *plInterval;
//	plInterval = *plIntervalOrg;

	if ((*plInterval) != NULL)
	{
		for (int iInterval = 0; iInterval < (int) (*plInterval)->size(); iInterval++)
		{
			delete (*plInterval)->at(iInterval);
		}
		delete (*plInterval);
		(*plInterval) = NULL;
	}
}

void CRules::ShowRule(CRuleItem *pRuleItem, CString *psRule)
{
	CString sTxt;

	for (int iCount = 0; iCount < RULE_ITEMS; iCount++)
	{
		if (pRuleItem->m_iType[iCount] == RULES_TYPE_NONE) continue;

		if (psRule->GetLength() > 0)
		{
			*psRule += " | ";
		}

		Type(pRuleItem->m_iType[iCount],&sTxt);

		*psRule += sTxt;
		*psRule += " ";

		Operator(pRuleItem->m_iType[iCount], pRuleItem->m_iOperator[iCount],&sTxt);

		*psRule += sTxt;
		*psRule += " ";

		Value(TypeClass(pRuleItem->m_iType[iCount]), pRuleItem->m_iValue[iCount], pRuleItem->m_dValue[iCount], &sTxt);

		*psRule += sTxt;
	}
}
// DlgRules.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgRules.h"
#include "afxdialogex.h"
#include "Rules.h"
#include "DlgSettingsRules.h"
#include "DlgRulesLogging.h"
#include "DlgScheduler.h"
#include "Translation.h"

// CDlgRules dialog

IMPLEMENT_DYNAMIC(CDlgRules, CDialog)

CDlgRules::CDlgRules(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRules::IDD, pParent)
{
	m_color = RGB(255,60,100);
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_windowScheduler.Setup(48*2,30/2,7,false, true,true);
}

CDlgRules::~CDlgRules()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
}

BOOL CDlgRules::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// translation
	SetWindowText(gszTranslation[PosDialogRulesTitle]);
	m_textName.SetWindowText(gszTranslation[PosDialogRulesName]);
	m_textComputer.SetWindowText(gszTranslation[PosDialogRulesComputer]);
	m_textProject.SetWindowText(gszTranslation[PosDialogRulesProject]);
	m_textApplication.SetWindowText(gszTranslation[PosDialogRulesApplication]);

	m_textGroupType.SetWindowText(gszTranslation[PosDialogRulesGroupType]);
	m_textGroupOperator.SetWindowText(gszTranslation[PosDialogRulesGroupOperator]);
	m_textGroupValue.SetWindowText(gszTranslation[PosDialogRulesGroupValue]);
	m_textGroupTime.SetWindowText(gszTranslation[PosDialogRulesGroupTime]);
	m_textGroupEvent.SetWindowText(gszTranslation[PosDialogRulesGroupEvent]);

	m_buttonColor.SetWindowText(gszTranslation[PosDialogRulesButtonColor]);
	m_buttonCheck.SetWindowText(gszTranslation[PosDialogRulesButtonCheck]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
	m_buttonAdd.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_checkShowMessage.SetWindowText(gszTranslation[PosDialogRulesEventMessage]);
	m_checkShowMessageNotice.SetWindowText(gszTranslation[PosDialogRulesEventMessageNotice]);
	m_checkShowMessageNone.SetWindowText(gszTranslation[PosDialogRulesEventMessageNone]);


// translation
	AddRuleTypes(0,&m_listRulesType1);
	AddRuleTypes(1,&m_listRulesType2);
	AddRuleTypes(2,&m_listRulesType3);

	m_listEvent.AddString(gszTranslation[PosDialogRulesEventSuspendProject]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventResumeProject]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventSuspendTask]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventSnooze]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventSnoozeGpu]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventNoNewWork]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventAllowNewWork]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventProgram]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventResumeNetwork]);
	m_listEvent.AddString(gszTranslation[PosDialogRulesEventSuspendNetwork]);

	for (int iCount = 0; iCount < RULE_ITEMS;iCount++)
	{
		m_iType[iCount] = RULES_TYPE_NONE;
		m_iTypeClass[iCount] = RULE_TYPE_CLASS_UNKNOWN;
		m_iOperator[iCount] = RULE_OPERATOR_NONE;
	}

	m_checkShowMessageNone.SetCheck(TRUE);
	m_iEventInternal = EVENT_NONE;
	m_iEventExternal = EVENT_NONE;
	m_iEventShow = EVENT_SHOW_NONE;

	if (m_bSetup)
	{
		SetItems(0, m_pRuleItem);
	}
	else
	{
		m_editComputer.SetWindowText(m_sComputer);
		m_editProject.SetWindowText(m_sProject);
		m_editApplication.SetWindowText(m_sApplication);
		SetInterval(&m_windowScheduler, NULL);
		SetInterval(&theApp.m_pDlgScheduler->m_window, NULL);
	}

	m_wndColor.SetColor(m_color);
	m_wndColor.Invalidate(TRUE);

//	SetTimer(UTIMER_DLG_RULES, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgRules::AddRuleTypes(int iNr, CListBox *pListBox)
{
	pListBox->AddString(gszTranslation[PosDialogRulesTypeElapsed]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeCpuP]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeProgress]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeTimeLeft]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeProgressDelta]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeUse]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeTemperature]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeStatus]);

	if (iNr == 0)
	{
		pListBox->AddString(gszTranslation[PosDialogRulesTypeTime]);
	}

	pListBox->AddString(gszTranslation[PosDialogRulesTypeConnection]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeDeadline]);
	pListBox->AddString(gszTranslation[PosDialogRulesTypeTimeLeftProject]);
}

bool CDlgRules::CheckDouble(CString *psTxt)
{
	int iDouble;

	iDouble = 0;

	for (int iCount = 0; iCount < RULE_ITEMS-1; iCount++)
	{
		if (m_iType[iCount] == m_iType[iCount+1])
		{
			iDouble++;
			if (m_iType[iCount] == RULES_TYPE_NONE) continue;
			if (m_iType[iCount] == RULES_TYPE_TIME_INTERVAL) 
			{
				if (iDouble < 2) continue;	//2 allowed
			}
			CRules rules;
			rules.Type(m_iType[iCount], psTxt);
			*psTxt = gszTranslation[PosDialogRulesMsgIdenticalType];		//  "Identical type" + ": " + *psTxt;
			*psTxt+= ".";
			return true;
		}
	}
	return false;
}

bool CDlgRules::CheckEmpty(CString *psTxt)
{
	for (int iCount = 0; iCount < RULE_ITEMS; iCount++)
	{
		if (m_iType[iCount] != RULES_TYPE_NONE)
		{
			return false;
		}
	}

	*psTxt = gszTranslation[PosDialogRulesMsgNotRules];							//"There are no rules.";
	*psTxt+= ".";
	return true;
}

bool CDlgRules::CheckEvent(CString *psTxt)
{
	if (m_iType[0] == RULES_TYPE_TIME_INTERVAL)
	{
		*psTxt = gszTranslation[PosDialogRulesNotAllowed];
		*psTxt+= ": ";
		switch (m_iEventInternal)
		{
			case EVENT_TYPE_SUSPEND_TASK:
				*psTxt+= gszTranslation[PosDialogRulesEventSuspendTask];
				return false;
			break;
			case EVENT_TYPE_PROGRAM:
				*psTxt+= gszTranslation[PosDialogRulesEventProgram];
				return false;
			break;
			case EVENT_TYPE_RESUME_PROJECT:
				*psTxt+= gszTranslation[PosDialogRulesEventResumeProject];
				return false;
			break;
			case EVENT_TYPE_ALLOW_NEW_WORK:
				*psTxt+= gszTranslation[PosDialogRulesEventAllowNewWork];
				return false;
			break;
			case EVENT_TYPE_SUSPEND_NETWORK:
				*psTxt += gszTranslation[PosDialogRulesEventSuspendNetwork];
				return false;
				break;
			case EVENT_TYPE_RESUME_NETWORK:
				*psTxt += gszTranslation[PosDialogRulesEventResumeNetwork];
				return false;
				break;
		}
	}
	*psTxt = "";
	return true;
}

void CDlgRules::CheckAndFillOperator(int iPosition, CListBox *pListRulesType,CListBox *pListRulesOperator)
{
	int		iCurrentSelection, iType, iTypeClass, iLen, iItems, iSelectionCount;
	CString sSelection;
	bool	bShow = true;

	iCurrentSelection = pListRulesType->GetCurSel();
	if (iCurrentSelection != LB_ERR)
	{
		iLen = pListRulesType->GetTextLen(iCurrentSelection);
		pListRulesType->GetText(iCurrentSelection, sSelection.GetBuffer(iLen));
		sSelection.ReleaseBuffer();
	}

	// remove everthing except the current
	iItems = pListRulesType->GetCount();
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (iCount != iCurrentSelection)
		{
			pListRulesType->SetSel(iCount,FALSE);
		}
	}
	iSelectionCount = pListRulesType->GetSelCount();

	if (iSelectionCount == 0) sSelection = "";

	ClassifyType(&sSelection, &iType, &iTypeClass);
	m_iTypeClass[iPosition] = iTypeClass;
	m_iType[iPosition] = iType;

	iItems = pListRulesOperator->GetCount();
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		pListRulesOperator->DeleteString(0);
	}

	if (iSelectionCount)
	{
		switch (iTypeClass)
		{
			case RULE_TYPE_CLASS_USE:
				pListRulesOperator->AddString("=");
			break;
			case RULE_TYPE_CLASS_TIME_DAY:
			case RULE_TYPE_CLASS_TEMPERATURE:
				pListRulesOperator->AddString(">");
				pListRulesOperator->AddString("<");
			break;
			case RULE_TYPE_CLASS_INTERVAL:
				if (&m_listRulesOperator1 == pListRulesOperator)
				{
					bShow = false;
				}
			break;
			case RULE_TYPE_CLASS_CONNECTION:
				pListRulesOperator->AddString("=<>");
				pListRulesOperator->AddString("<>");
			break;
			default:
				pListRulesOperator->AddString("=");
				pListRulesOperator->AddString(">");
				pListRulesOperator->AddString("<");
		}
	}

	if (iPosition == 0)
	{
		ShowScheduler(bShow);
	}

	m_iOperator[iPosition] = RULE_OPERATOR_NONE;
}

void CDlgRules::CheckOperator(CListBox *pListRulesOperator, int *piOperator)
{
	int iCurrentSelection, iLen;
	CString sSelection;

	iCurrentSelection = pListRulesOperator->GetCurSel();
	if (iCurrentSelection != LB_ERR)
	{
		iLen = pListRulesOperator->GetTextLen(iCurrentSelection);
		pListRulesOperator->GetText(iCurrentSelection, sSelection.GetBuffer(iLen));
		sSelection.ReleaseBuffer();
	}

	if (sSelection == "=")
	{
		*piOperator = RULE_OPERATOR_IS;
		return;
	}
	if (sSelection == ">")
	{
		*piOperator = RULE_OPERATOR_MORE;
		return;
	}
	if (sSelection == "<")
	{
		*piOperator = RULE_OPERATOR_LESS;
		return;
	}
	if (sSelection == "<>")
	{
		*piOperator = RULE_OPERATOR_NOTEQUAL;
		return;
	}
	if (sSelection == "=<>")
	{
		*piOperator = RULE_OPERATOR_NOLONGEREQUAL;
		return;
	}
}

void CDlgRules::CheckForIncompatible()
{
	int iCurrentSelection, iEvenClassSelection, iEventType, iEventClass;
	int iLen, iItems;
	CString sSelection;
	CRules rules;

	iCurrentSelection = m_listEvent.GetCurSel();
	if (iCurrentSelection != LB_ERR)
	{
		iLen = m_listEvent.GetTextLen(iCurrentSelection);
		m_listEvent.GetText(iCurrentSelection, sSelection.GetBuffer(iLen));
		sSelection.ReleaseBuffer();
	}

	iEvenClassSelection = rules.EventClass(rules.EventType(&sSelection));

	// remove everthing except the current
	iItems = m_listEvent.GetCount();
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (iCount != iCurrentSelection)
		{
			iLen = m_listEvent.GetTextLen(iCount);
			m_listEvent.GetText(iCount, sSelection.GetBuffer(iLen));
			sSelection.ReleaseBuffer();
			iEventClass = rules.EventClass(rules.EventType(&sSelection));

			if (iEvenClassSelection == iEventClass)
			{
				m_listEvent.SetSel(iCount,FALSE);
			}
		}
	}

	m_iEventInternal = EVENT_NONE;
	m_iEventExternal = EVENT_NONE;

	iItems = m_listEvent.GetCount();
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		if (m_listEvent.GetSel(iCount))
		{
			iLen = m_listEvent.GetTextLen(iCount);
			m_listEvent.GetText(iCount, sSelection.GetBuffer(iLen));
			sSelection.ReleaseBuffer();

			iEventType = rules.EventType(&sSelection);
			iEventClass= rules.EventClass(iEventType);
			if (iEventClass == EVENT_CLASS_INTERNAL)
			{
				m_iEventInternal = iEventType;
			}
			if (iEventClass == EVENT_CLASS_EXTERNAL)
			{
				m_iEventExternal = iEventType;
			}
		}
	}	
}

bool CDlgRules::CheckParameters(CString *psError)
{
	CRules rules;
	CString sProgram, sParameters, sProgramFull;

	if (m_iEventInternal == EVENT_TYPE_SNOOZE || m_iEventInternal == EVENT_TYPE_SNOOZE_GPU)
	{
		__int64 i64Time;
		if (!GetTime(&m_sEvent, &i64Time))
		{
			*psError = gszTranslation[PosDialogRulesMsgInvalidTime];				//"Invalid time
			*psError += ": " + m_sEvent;
			*psError += " (12:34:56)\r\n";
			return false;
		}
		m_iSnooze = i64Time;
	}
	if (m_iEventExternal == EVENT_NONE) return true;

	rules.EventExtract(&m_sEvent, &sProgram, &sParameters);

	if (sProgram.GetLength() == 0)
	{
		*psError = gszTranslation[PosDialogRulesMsgNoProgram];	//"No program, or batch file found.";
		*psError+= " (";
		*psError+= gszTranslation[PosDialogRulesMsgEmpty];		//" (empty)";
		*psError+= ").\r\n";
		return false;
	}
	rules.EventProgramFullPath(&sProgram, &sProgramFull);
	if (!rules.EventProgramCheckValid(&sProgramFull))
	{
		*psError = gszTranslation[PosDialogRulesMsgNoProgram];	//"No program, or batch file found.";
		*psError+= ":\r\n";
		*psError+= sProgramFull.Mid(0,46);
		*psError+= "\r\n";
		*psError+= sProgramFull.Mid(46);
		return false;
	}
	return true;
}

void CDlgRules::ClassifyType(CString *psText, int *piType, int *piTypeClass)
{
	CRules rules;

	*piType = rules.Type(psText);
	*piTypeClass = rules.TypeClass(*piType);
}

bool CDlgRules::CheckDigits(CString *psTxt)
{
	int iLen;

	iLen = psTxt->GetLength();
	for (int iCount = 0; iCount < iLen; iCount++)
	{
		if (!isdigit(psTxt->GetAt(iCount)))
		{
			if (iCount == 0)
			{
				if (psTxt->GetAt(iCount) == '-')
				{
					continue;
				}
			}
			return false;
		}
	}
	return true;
}

bool CDlgRules::GetTime(CString *psValue, __int64 *pi64Time)
{
	int iNr, iLen, iFoundPosSec, iFoundPosMin, iFoundPosHour, iTotalCharCount;
	__int64 iTime;
//	char *pChar;
	CString sTime, sDay, sHour, sMin, sSec;
	
	*pi64Time = -1;

	iTotalCharCount = 0;

	sTime = *psValue;
	sTime.Remove(' ');
	sTime.MakeLower();
	sTime.Replace("d,","h");
	sTime.Replace("d","h");

	iLen = sTime.GetLength();

	iFoundPosHour = -1;
	iFoundPosMin = -1;

	int iFirstPosHour = 0;
	int iFirstPosMin = 0;
	int iFirstPosSec = 0;

	// second
	iFoundPosSec = sTime.ReverseFind(':');
	if (iFoundPosSec >=0)
	{
		sTime.SetAt(iFoundPosSec,'s');				// second
		iFoundPosMin = sTime.ReverseFind(':');
		if (iFoundPosMin >=0)
		{
			sTime.SetAt(iFoundPosMin,'m');			// minute	
		}
	}

	iFoundPosHour = sTime.ReverseFind('h');
	if (iFoundPosHour > iFoundPosSec) return false;
	if (iFoundPosHour > iFoundPosMin) return false;

	if (iFoundPosHour >= 0)
	{
		iFirstPosHour = iFoundPosHour+1;
		sDay = sTime.Mid(0,iFoundPosHour);
	}

	if (iFoundPosMin >= 0)
	{
		iFirstPosMin = iFoundPosMin+1;
		sHour = sTime.Mid(iFirstPosHour, iFoundPosMin-iFirstPosHour);
	}

	if (iFoundPosSec >= 0)
	{
		iFirstPosSec = iFoundPosSec+1;
		sMin = sTime.Mid(iFirstPosMin, iFoundPosSec-iFirstPosMin);
	}

	sSec = sTime.Mid(iFirstPosSec, 4);

	if (!CheckDigits(&sDay)) return false;
	if (!CheckDigits(&sHour)) return false;
	if (!CheckDigits(&sMin)) return false;
	if (!CheckDigits(&sSec)) return false;

	bool bMinus = false;

	iTime = atoi(sSec.GetBuffer());
	if (iTime > 59) return false;
	iNr = atoi(sMin.GetBuffer());
	if (iNr > 59) return false;
	iTime += (iNr * 60);
	iNr = atoi(sHour.GetBuffer());
	if (iNr > 23) return false;
	iTime += (iNr * 60 * 60);
	iNr = atoi(sDay.GetBuffer());
	if (iNr < 0) bMinus = true;
	iNr  = abs(iNr);
	iTime += (iNr * 24 * 60 * 60);
	if (bMinus)
	{
		iTime = -iTime;
	}

	*pi64Time = iTime;
	return true;
}

bool CDlgRules::GetUse(CString *psUse, double *pdValue, _int64 *piValue)
{
	int iFoundPos;
	double dValue;
	CString sUse, sGpu;

	sUse = *psUse;
	sUse.Remove(' ');
	
	*pdValue = atof(sUse.GetBuffer());
	
	iFoundPos = sUse.Find('+');
	if (iFoundPos < 0) return false;

	sGpu = sUse.Mid(iFoundPos, 100);
	dValue = atof(sGpu.GetBuffer());

	*piValue = (int) (dValue * 1000);

	return true;
}

bool CDlgRules::GetStatus(CString *psStatus, double *pdStatus, _int64 *piCount)
{
	int iFoundPos;
	CString sStatusNr, sStatusCount;

	iFoundPos = psStatus->Find(",");
	if (iFoundPos < 0) return false;

	sStatusNr = psStatus->Left(iFoundPos);
	sStatusCount = psStatus->Mid(iFoundPos+1);

	*pdStatus = atof(sStatusNr.GetBuffer());
	*piCount = atoi(sStatusCount);

	if (*pdStatus <= 0) return false;
//	if (*piCount <= 0) return false;

	return true;
}

bool CDlgRules::GetRule(int iPosition, CString *psError)
{
	CRules rules;
	CString sTxt;
	bool	bError;

	if (m_iType[iPosition] == RULES_TYPE_NONE)
	{
		m_iOperator[iPosition] = RULE_OPERATOR_NONE;
		m_dValue[iPosition] = -1;
		m_iValue[iPosition] = -1;
		return false;
	}

	if (m_iType[iPosition] != RULES_TYPE_TIME_INTERVAL)
	{
		if (m_iOperator[iPosition] == RULE_OPERATOR_NONE)
		{
			rules.Type(m_iType[iPosition], &sTxt);
			*psError += gszTranslation[PosDialogRulesMsgOperatorNotSelected];				//"Operator not selected after
			*psError += ": " + sTxt;
			*psError += "\r\n";
			return false;
		}
	}

	switch (m_iTypeClass[iPosition])
	{
		case RULE_TYPE_CLASS_TIME_DAY:
			m_dValue[iPosition] = -1;
			if (!GetTime(&m_sValue[iPosition], &m_iValue[iPosition]))
			{
				*psError = gszTranslation[PosDialogRulesMsgInvalidTime];				//"Invalid time
				*psError+= ": " + m_sValue[iPosition];
				*psError += " (1d,12:34:56)\r\n";
				return false;
			}
		break;
		case RULE_TYPE_CLASS_INTERVAL:
			m_dValue[iPosition] = -1;
			m_iValue[iPosition] = -1;


//			GetTime(&m_sValue[iPosition], &m_iValue[iPosition]);
//			if (m_iValue[iPosition] < 0 || m_iValue[iPosition] > (60*60*24))
//			{
//				*psError = gszTranslation[PosDialogRulesMsgInvalidTime];				//"Invalid time
//				*psError+= ": " + m_sValue[iPosition];
//				*psError += " (12:34:56)\r\n";
//				return false;
//			}
			return true;
		break;
		case RULE_TYPE_CLASS_TEMPERATURE:
			if (m_sComputer.GetLength() == 0)
			{
				*psError = gszTranslation[PosDialogRulesMsgTempRuleComputer];			//"A temperature rule must have a computer name";
				*psError+= ".";
				return false;
			}
		case RULE_TYPE_CLASS_PERCENTAGE:
			m_iValue[iPosition] = -1;
			m_dValue[iPosition] = atof(m_sValue[iPosition].GetBuffer());
		break;
		case RULE_TYPE_CLASS_USE:
			if (!GetUse(&m_sValue[iPosition], &m_dValue[iPosition], &m_iValue[iPosition]))
			{
				*psError = gszTranslation[PosDialogRulesMsgInvalidUse];					//"Invalid use
				*psError+= ": " + m_sValue[iPosition];
				*psError += " (0.04 Cpu + 1 Gpu ; 1 Cpu + 0 Gpu)\r\n";
			}
		break;
		case RULE_TYPE_CLASS_STATUS:
			if (!GetStatus(&m_sValue[iPosition], &m_dValue[iPosition], &m_iValue[iPosition]))
			{
				*psError = gszTranslation[PosDialogRulesMsgInvalidStatus];				//"Invalid status
				*psError+= ": " + m_sValue[iPosition];
				*psError+= gszTranslation[PosDialogRulesMsgInvalidStatus2];
//				*psError += "\r\nSyntax: 3,4 = status,count\r\n";
//				*psError += "3 = error, 4 = uploading, 5 = Ready, 6 = aborted
				*psError += "\r\n";
			}
		break;
		case RULE_TYPE_CLASS_CONNECTION:
			bError = false;
			if (m_iOperator[1] != RULE_OPERATOR_NONE) bError = true;
			if (m_iOperator[2] != RULE_OPERATOR_NONE) bError = true;
			if (bError)
			{
				*psError = gszTranslation[PosDialogRulesMsgOnlyFirstType];				//"Only the first type may be used";
				*psError+= ".";
				return false;
			}
			m_editProject.SetWindowText("");
			m_editApplication.SetWindowText("");
			m_editValue1.SetWindowText("");
		break;
	}

	return true;
}

void CDlgRules::ShowRule(int iPosition, CString *psRule)
{
	CString sTxt;

	if (m_iType[iPosition] == RULES_TYPE_NONE) return;

	CRules rules;
	rules.Type(m_iType[iPosition],&sTxt);

	*psRule += sTxt;
	*psRule += " ";

	rules.Operator(m_iType[iPosition], m_iOperator[iPosition],&sTxt);

	*psRule += sTxt;
	*psRule += " ";

	rules.Value(m_iTypeClass[iPosition], m_iValue[iPosition], m_dValue[iPosition], &sTxt);
	*psRule += sTxt;

	if (iPosition == 0)
	{
		if (m_iType[0] == RULES_TYPE_TIME_INTERVAL)
		{
			rules.IntervalString(m_plInterval, &sTxt);
			*psRule += sTxt;
		}
	}
}

void CDlgRules::ShowRuleRest(CString *psRule)
{
	CRules rules;
	CString sTxt;

	rules.TimeString(m_iTime, &sTxt);
	*psRule += sTxt;

	if ((m_iEventInternal == EVENT_TYPE_SNOOZE) || (m_iEventInternal == EVENT_TYPE_SNOOZE_GPU))
	{
		rules.TimeString(m_iSnooze, &sTxt);
		*psRule += ",";
		*psRule += gszTranslation[PosDialogRulesMsgSnoozeFor];
		*psRule += ": ";
		*psRule += sTxt;
	}

}

bool CDlgRules::ValidRules()
{
	UINT	iPos;
	CString sError, sRule, sTxt;

	m_editRulesStatus.SetWindowText("");
	
	m_editName.GetWindowText(m_sName);
	if (m_sName.GetLength() == 0)
	{
		m_editRulesStatus.SetWindowText(gszTranslation[PosDialogRulesMsgRuleNameEmpty]);	//"Rule name is empty.");
		return false;
	}

	// if rule name = the only one;
	CRuleItem	*pRuleItem;

	for (iPos = 0; iPos <  m_plRules->size(); iPos++)
	{
		pRuleItem = m_plRules->at(iPos);
		if (pRuleItem->m_sName == m_sName)
		{
			if (m_pRuleItem != pRuleItem)
			{
				m_editRulesStatus.SetWindowText(gszTranslation[PosDialogRulesMsgRuleNameInUse]);	//"Rule name is already in use.");
				return false;
			}
		}
	}

	m_editComputer.GetWindowText(m_sComputer);
	m_editProject.GetWindowText(m_sProject);
	m_editApplication.GetWindowText(m_sApplication);

	m_editValue1.GetWindowText(m_sValue[0]);
	m_editValue2.GetWindowText(m_sValue[1]);
	m_editValue3.GetWindowText(m_sValue[2]);

	if (CheckDouble(&sError))
	{
		m_editRulesStatus.SetWindowText(sError);
		return false;
	}

	if (CheckEmpty(&sError))
	{
		m_editRulesStatus.SetWindowText(sError);
		return false;
	}

	GetRule(0, &sError);
	GetRule(1, &sError);
	GetRule(2, &sError);

	if (sError.GetLength() > 0)
	{
		m_editRulesStatus.SetWindowText(sError);
		return false;
	}

	m_editTime1.GetWindowText(sTxt);

	if (!GetTime(&sTxt, &m_iTime))
	{
		sError = gszTranslation[PosDialogRulesMsgInvalidTime];						//"Invalid time: "
		sError+= ": ";
		sError+= sTxt;
		sError += " (1d,12:34:56)\r\n";
		m_editRulesStatus.SetWindowText(sError);
		return false;
	}

	if (!CheckEvent(&sError))
	{
		m_editRulesStatus.SetWindowText(sError);
		return false;
	}

	m_editEvent.GetWindowText(m_sEvent);

	m_sEvent.Remove('\n');
	m_sEvent.Remove('\r');

	if (!CheckParameters(&sError))
	{
		m_editRulesStatus.SetWindowText(sError);
		return false;
	}

	return true;
}

void CDlgRules::SetItems(int iPosition, CRuleItem *pRuleItem)
{
	CRules rules;
	CString sTxt;

	if (pRuleItem->m_plInterval != NULL)
	{
		SetInterval(&m_windowScheduler, pRuleItem->m_plInterval);
		SetInterval(&theApp.m_pDlgScheduler->m_window, pRuleItem->m_plInterval);
	}

	m_editName.SetWindowText(pRuleItem->m_sName);
	m_editComputer.SetWindowText(pRuleItem->m_sComputer);
	m_editProject.SetWindowText(pRuleItem->m_sProject);
	m_editApplication.SetWindowText(pRuleItem->m_sApplication);

	rules.Type(pRuleItem->m_iType[0], &sTxt);
	SelectListboxType(&m_listRulesType1, &sTxt);
	OnLbnSelchangeListRulesType1();
	rules.Type(pRuleItem->m_iType[1], &sTxt);
	SelectListboxType(&m_listRulesType2, &sTxt);
	OnLbnSelchangeListRulesType2();
	rules.Type(pRuleItem->m_iType[2], &sTxt);
	SelectListboxType(&m_listRulesType3, &sTxt);
	OnLbnSelchangeListRulesType3();

	rules.Operator(pRuleItem->m_iType[0], pRuleItem->m_iOperator[0], &sTxt);
	SelectListboxOperator(&m_listRulesOperator1, &sTxt);
	OnLbnSelchangeListRulesOperator1();
	rules.Operator(pRuleItem->m_iType[1], pRuleItem->m_iOperator[1], &sTxt);
	SelectListboxOperator(&m_listRulesOperator2, &sTxt);
	OnLbnSelchangeListRulesOperator2();
	rules.Operator(pRuleItem->m_iType[2], pRuleItem->m_iOperator[2], &sTxt);
	SelectListboxOperator(&m_listRulesOperator3, &sTxt);
	OnLbnSelchangeListRulesOperator3();

	int iClass;
	if (pRuleItem->m_iType[0] != RULES_TYPE_NONE)
	{
		iClass = rules.TypeClass(pRuleItem->m_iType[0]);
		rules.Value(iClass, pRuleItem->m_iValue[0], pRuleItem->m_dValue[0], &sTxt);
	}
	else sTxt = "";
	m_editValue1.SetWindowText(sTxt);

	if (pRuleItem->m_iType[1] != RULES_TYPE_NONE)
	{
		iClass = rules.TypeClass(pRuleItem->m_iType[1]);
		rules.Value(iClass, pRuleItem->m_iValue[1], pRuleItem->m_dValue[1], &sTxt);
	}
	else sTxt = "";
	m_editValue2.SetWindowText(sTxt);

	if (pRuleItem->m_iType[2] != RULES_TYPE_NONE)
	{
		iClass = rules.TypeClass(pRuleItem->m_iType[2]);
		rules.Value(iClass, pRuleItem->m_iValue[2], pRuleItem->m_dValue[2], &sTxt);
	}
	else sTxt = "";
	m_editValue3.SetWindowText(sTxt);

	rules.TimeString(pRuleItem->m_iTime, &sTxt);
	m_editTime1.SetWindowText(sTxt);

	m_iEventInternal = pRuleItem->m_iEventTypeInternal;
	SelectListBoxEvent(m_iEventInternal);
	m_iEventExternal = pRuleItem->m_iEventTypeExternal;
	SelectListBoxEvent(m_iEventExternal);

	m_editEvent.SetWindowText(pRuleItem->m_sEvent);

	m_color = pRuleItem->m_color;
	m_wndColor.SetColor(m_color);
	m_wndColor.Invalidate(TRUE);

	m_iEventShow = pRuleItem->m_iEventShow;
	bool bCheck;
	bCheck = (m_iEventShow == EVENT_SHOW_LOGGING);

	m_checkShowMessage.SetCheck(FALSE);
	m_checkShowMessageNotice.SetCheck(FALSE);
	m_checkShowMessageNone.SetCheck(FALSE);

	switch (m_iEventShow)
	{
		case EVENT_SHOW_LOGGING:
			m_checkShowMessage.SetCheck(TRUE);
		break;
		case EVENT_SHOW_NOTICE:
			m_checkShowMessageNotice.SetCheck(TRUE);
		break;
		default:
			m_checkShowMessageNone.SetCheck(TRUE);
	}

//	m_checkShowMessage.SetCheck(bCheck);
}

void CDlgRules::SetInterval(CWndScheduler *pWndScheduler, std::deque<CRuleIntervalItem *> *plInterval)
{
	pWndScheduler->ClearInterval();
	if (plInterval != NULL)
	{
		for (int iInterval = 0; iInterval < (int) plInterval->size(); iInterval++)
		{
			pWndScheduler->AddInterval(plInterval->at(iInterval)->m_iStartTime, plInterval->at(iInterval)->m_iStopTime);
		}
	}
}

void CDlgRules::SelectListboxType(CListBox *pListBox, CString *pTxt)
{
	int iIndex;

	iIndex = pListBox->FindStringExact(0,*pTxt);
	if (iIndex >= 0)
	{
		pListBox->SetSel(iIndex,TRUE);
	}
}

void CDlgRules::SelectListboxOperator(CListBox *pListBox, CString *pTxt)
{
	int iIndex;

	iIndex = pListBox->FindStringExact(0,*pTxt);
	if (iIndex >= 0)
	{
		pListBox->SetCurSel(iIndex);
	}
}

void CDlgRules::SelectListBoxEvent(int iEvent)
{
	CRules rules;
	CString sSelection;
	int		iIndex;

	rules.EventType(iEvent,&sSelection);

	iIndex = m_listEvent.FindStringExact(0,sSelection);
	if (iIndex >= 0)
	{
		m_listEvent.SetSel(iIndex,TRUE);
	}
}

void CDlgRules::ShowScheduler(bool bShow)
{
	if (bShow)
	{
		m_listRulesOperator1.ShowWindow(SW_SHOW);
		m_editValue1.ShowWindow(SW_SHOW);
		m_windowScheduler.ShowWindow(SW_HIDE);
	}
	else
	{
		m_listRulesOperator1.ShowWindow(SW_HIDE);
		m_editValue1.ShowWindow(SW_HIDE);
		m_windowScheduler.ShowWindow(SW_SHOW);
	}
}

void CDlgRules::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RULES_TYPE_1, m_listRulesType1);
	DDX_Control(pDX, IDC_LIST_RULES_TYPE_2, m_listRulesType2);
	DDX_Control(pDX, IDC_LIST_RULES_TYPE_3, m_listRulesType3);
	DDX_Control(pDX, IDC_LIST_RULES_OPERATOR_1, m_listRulesOperator1);
	DDX_Control(pDX, IDC_LIST_RULES_OPERATOR_2, m_listRulesOperator2);
	DDX_Control(pDX, IDC_LIST_RULES_OPERATOR_3, m_listRulesOperator3);
	DDX_Control(pDX, IDC_TEXT_RULES_COMPUTER, m_textComputer);
	DDX_Control(pDX, IDC_TEXT_RULES_PROJECT, m_textProject);
	DDX_Control(pDX, IDC_TEXT_RULES_APPLICATION, m_textApplication);
	DDX_Control(pDX, IDC_EDIT_RULES_VALUE_1, m_editValue1);
	DDX_Control(pDX, IDC_EDIT_RULES_VALUE_2, m_editValue2);
	DDX_Control(pDX, IDC_EDIT_RULES_VALUE_3, m_editValue3);
	DDX_Control(pDX, IDC_EDIT_RULES_FINAL, m_editRulesStatus);
	DDX_Control(pDX, IDC_TEXT_RULES_NAME, m_textName);
	DDX_Control(pDX, IDC_EDIT_RULES_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_RULES_COMPUTER, m_editComputer);
	DDX_Control(pDX, IDC_EDIT_RULES_PROJECT, m_editProject);
	DDX_Control(pDX, IDC_EDIT_RULES_APPLICATION, m_editApplication);
	DDX_Control(pDX, IDC_EDIT_RULES_TIME_1, m_editTime1);
	DDX_Control(pDX, IDC_BUTTON_RULES_CHECK, m_buttonCheck);
	DDX_Control(pDX, IDC_BUTTON_RULES_OK, m_buttonAdd);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_BUTTON_RULES_COLOR, m_buttonColor);
	DDX_Control(pDX, IDC_WND_COLOR_RULES, m_wndColor);
	DDX_Control(pDX, IDC_LIST_RULES_EVENT, m_listEvent);
	DDX_Control(pDX, IDC_EDIT_RULES_EVENT, m_editEvent);
	DDX_Control(pDX, IDC_CHECK_RULES_SHOW, m_checkShowMessage);
	DDX_Control(pDX, IDC_CHECK_RULES_SHOW_NOTICE, m_checkShowMessageNotice);
	DDX_Control(pDX, IDC_CHECK_RULES_SHOW_NONE, m_checkShowMessageNone);
	DDX_Control(pDX, IDC_TEXT_RULES_GROUP_TYPE, m_textGroupType);
	DDX_Control(pDX, IDC_TEXT_RULES_GROUP_OPERATOR, m_textGroupOperator);
	DDX_Control(pDX, IDC_TEXT_RULES_GROUP_VALUE, m_textGroupValue);
	DDX_Control(pDX, IDC_TEXT_RULES_GROUP_TIME, m_textGroupTime);
	DDX_Control(pDX, IDC_TEXT_RULES_GROUP_EVENT, m_textGroupEvent);
	DDX_Control(pDX, IDC_WND_RULES_SCHEDULER, m_windowScheduler);

}


BEGIN_MESSAGE_MAP(CDlgRules, CDialog)
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_LIST_RULES_TYPE_1, &CDlgRules::OnLbnSelchangeListRulesType1)
	ON_LBN_SELCHANGE(IDC_LIST_RULES_TYPE_2, &CDlgRules::OnLbnSelchangeListRulesType2)
	ON_LBN_SELCHANGE(IDC_LIST_RULES_TYPE_3, &CDlgRules::OnLbnSelchangeListRulesType3)
	ON_LBN_SELCHANGE(IDC_LIST_RULES_OPERATOR_1, &CDlgRules::OnLbnSelchangeListRulesOperator1)
	ON_LBN_SELCHANGE(IDC_LIST_RULES_OPERATOR_2, &CDlgRules::OnLbnSelchangeListRulesOperator2)
	ON_LBN_SELCHANGE(IDC_LIST_RULES_OPERATOR_3, &CDlgRules::OnLbnSelchangeListRulesOperator3)
	ON_BN_CLICKED(IDC_BUTTON_RULES_CHECK, &CDlgRules::OnBnClickedButtonRulesCheck)
	ON_BN_CLICKED(IDC_BUTTON_RULES_OK, &CDlgRules::OnBnClickedButtonRulesAdd)
	ON_BN_CLICKED(IDC_BUTTON_RULES_COLOR, &CDlgRules::OnBnClickedButtonRulesColor)
	ON_LBN_SELCHANGE(IDC_LIST_RULES_EVENT, &CDlgRules::OnLbnSelchangeListRulesEvent)
	ON_BN_CLICKED(IDC_CHECK_RULES_SHOW, &CDlgRules::OnBnClickedCheckRulesShow)
	ON_BN_CLICKED(IDC_CHECK_RULES_SHOW_NOTICE, &CDlgRules::OnBnClickedCheckRulesShowNotice)
	ON_BN_CLICKED(IDC_CHECK_RULES_SHOW_NONE, &CDlgRules::OnBnClickedCheckRulesShowNone)
	ON_MESSAGE (UWM_MSG_BUTTON, OnButton) 
	ON_MESSAGE (UWM_MSG_RULES_INTERVAL, OnNewInterval) 
END_MESSAGE_MAP()


// CDlgRules message handlers

void CDlgRules::OnTimer(UINT_PTR nIDEvent)
{
//	if (m_iSetup2 >=  0)
//	{
//		m_iSetup2--;
//	}
//
//	if (m_iSetup2 ==  0)
//	{
//		m_iSetup2 = -1;
//		SetItems(0, m_pRuleItem);
//	}

//	if (m_bSetup)
//	{
//		m_bSetup = false;
//		SetItems2(0, m_pRuleItem);
//		m_iSetup2 = 4;
//	}

}

void CDlgRules::OnLbnSelchangeListRulesType1()
{
	CheckAndFillOperator(0,&m_listRulesType1, &m_listRulesOperator1);
}

void CDlgRules::OnLbnSelchangeListRulesType2()
{
	CheckAndFillOperator(1,&m_listRulesType2, &m_listRulesOperator2);
}

void CDlgRules::OnLbnSelchangeListRulesType3()
{
	CheckAndFillOperator(2,&m_listRulesType3, &m_listRulesOperator3);
}

void CDlgRules::OnLbnSelchangeListRulesOperator1()
{
	CheckOperator(&m_listRulesOperator1, &m_iOperator[0]);
}

void CDlgRules::OnLbnSelchangeListRulesOperator2()
{
	CheckOperator(&m_listRulesOperator2, &m_iOperator[1]);
}

void CDlgRules::OnLbnSelchangeListRulesOperator3()
{
	CheckOperator(&m_listRulesOperator3, &m_iOperator[2]);
}

void CDlgRules::OnLbnSelchangeListRulesEvent()
{
	CheckForIncompatible();
}


void CDlgRules::OnBnClickedButtonRulesCheck()
{
	CString sRule;

	if (!ValidRules()) return;
	
	ShowRule(0, &sRule);
	sRule += "\r\n";
	ShowRule(1, &sRule);
	sRule += "\r\n";
	ShowRule(2, &sRule);
	sRule += "\r\n";
	ShowRuleRest(&sRule);
	m_editRulesStatus.SetWindowText(sRule);
}


void CDlgRules::OnBnClickedButtonRulesAdd()
{
	CString sTxt;

	if (!ValidRules()) return;
	
	m_sRule = "";
	ShowRule(0, &m_sRule);
	ShowRule(1, &sTxt);
	if (m_sRule.GetLength() > 0)
	{
		if (sTxt.GetLength() > 0)
		{
			m_sRule += ",";
			m_sRule += sTxt;
		}
	}
	sTxt = "";
	ShowRule(2, &sTxt);
	if (m_sRule.GetLength() > 0)
	{
		if (sTxt.GetLength() > 0)
		{
			m_sRule += ",";
			m_sRule += sTxt;
		}
	}

	CDialog::OnOK();
}


void CDlgRules::OnBnClickedCancel()
{
	CDialog::OnCancel();
}




void CDlgRules::OnBnClickedButtonRulesColor()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color,CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color = dlg.GetColor();
		m_wndColor.SetColor(m_color);
		m_wndColor.Invalidate(TRUE);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgRules::OnBnClickedCheckRulesShow()
{
	m_iEventShow = EVENT_SHOW_LOGGING;
}

void CDlgRules::OnBnClickedCheckRulesShowNotice()
{
	m_iEventShow = EVENT_SHOW_NOTICE;
}


void CDlgRules::OnBnClickedCheckRulesShowNone()
{
	m_iEventShow = EVENT_SHOW_NONE;
}

LRESULT CDlgRules::OnButton(WPARAM parm1, LPARAM parm2)
{
	if (m_plInterval == NULL)
	{
		m_plInterval = new std::deque<CRuleIntervalItem *>;
	}

	theApp.m_pDlgScheduler->Init(this, m_plInterval);
	theApp.m_pDlgScheduler->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CDlgRules::OnNewInterval(WPARAM parm1, LPARAM parm2)
{
	CRules rules;

	if (m_plInterval != NULL)
	{
		SetInterval(&m_windowScheduler, m_plInterval);
	}

	m_windowScheduler.Invalidate(TRUE);

	theApp.m_pDlgScheduler->SafeWindowPlacement();

	return 0;
}



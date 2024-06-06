// RemoteCombinedProjectsView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
//#include "ComputerDoc.h"
#include "ListViewEx.h"					// base class for BoinCComputerView
#include "TemplateRemoteTaskView.h"		// base class for BoinCComputerView
#include "TemplateRemoteProjectView.h" 
#include "RemoteCombinedDoc.h"
#include "RemoteCombinedTaskView.h"
#include "RemoteCombinedProjectView.h"
#include "TemplateRemoteMessageView.h"
#include "RemoteCombinedMessageView.h"
#include "TemplateRemoteTransferView.h" 
#include "RemoteCombinedTransferView.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
#include "TemplateRemoteComputerView.h"
#include "RemoteCombinedComputerView.h"
#include "RemoteCombinedNoticesView.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsHistory.h"
#include "DlgSettingsView.h"
#include "DlgSettingsRules.h"
#include "DlgSettingsGeneral.h"
#include "DlgSettingsWarning.h"
#include "DlgRulesLogging.h"
#include "DlgNotices.h" 
#include "DlgPrefMain.h"
#include "DlgProxyMain.h"
#include "SortColumn.h"
#include "selection.h"
#include "History.h"
#include "MainFrm.h"
#include "Translation.h"
#include "SortColumn.h"
#include "Selection.h"
#include "UseCpuGpu.h"
#include "NumberFormat.h"
#include "DlgFloating.h"
#include "DlgLogging.h"
#include "DlgFont.h"
#include "DlgHistory.h"
#include "SortHistory.h"
#include "OperationItem.h"
#include "Wildcard.h"
#include "CreateHtmlWebServer.h"
#include "ThreadWebServer.h"
#include "ThreadCloudServer.h"
#include "Json.h"
#include "WebServerProcess.h"

// CRemoteProjectsView

IMPLEMENT_DYNCREATE(CRemoteCombinedHistoryView, CTemplateRemoteHistoryView)

CRemoteCombinedHistoryView::CRemoteCombinedHistoryView()
{
	m_iThreadLocked = 0;
	m_bThreadBusy	= false;
	m_bAddedFromFile= false;

	m_bRuleTemperature = false;
	m_bRuleWallClock	= false;
	m_bRuleStatus = false;
	m_bRuleConnection = false;
	m_bRuleTimeLeft = false;

	m_bOperationsQueLocked = false;

	m_bDocumentIsClosing = false;

	m_iHistoryStateReadyCount = 0;

	m_pDoc			= NULL;

	m_bAllProjectsSelected = true;

	m_iNrColomns = NUM_REMOTE_HISTORY_COLUMNS;

	m_bTooLargeShown = false;

	m_dataDummy.m_bSelected = false;
	m_dataDummy.m_color = RGB(255,255,255);

	m_dataWarning.m_bSelected = false;
	m_dataWarning.m_color = RGB(255,128,64);

	m_pHistory		= new CHistory;

	m_iTimeLeftLowest = 60;				// 60 = 30 sec initial interval as soon as there is a connection the new time is used based on WU's
	m_iTimeLeftLowestPrevious = 60;		// 60 = 30 sec initial interval
	m_LastSwitchedTime = 0;

	m_pFont = NULL;
	m_hWndProxy = theApp.m_pDlgProxyMain->m_hWnd;

	m_bRequestHistoryLong = false;
}

CRemoteCombinedHistoryView::~CRemoteCombinedHistoryView()
{
//	int					iPos, iSize;

	delete m_pHistory;

	ClearFloaterList();
	
	delete m_pFont;
}

void CRemoteCombinedHistoryView::OnInitialUpdate()
{
	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL,(WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);
	CTemplateRemoteHistoryView::OnInitialUpdate();
}

void CRemoteCombinedHistoryView::InitialUpdate(CRemoteCombinedDoc* pDoc)
{
	m_pDoc = pDoc;

	char 	*pcComputerId;
	int		iNrRpc;
	iNrRpc = m_pDoc->GetRpcSelectedComputers();

	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		m_pDoc->GetComputerIds(&pcComputerId, iCount);
		if (!m_pHistory->Read(pcComputerId, &m_lComputer.at(iCount)->m_lHistory))
		{
			Sleep(200);	// try a second time.
			if (!m_pHistory->Read(pcComputerId, &m_lComputer.at(iCount)->m_lHistory))
			{
			}
			else
			{
				m_bAddedFromFile = true;
			}
		}
		else
		{
			m_bAddedFromFile = true;
		}

		if (!m_pHistory->Read(pcComputerId, &m_lComputer.at(iCount)->m_lHistoryLong, true))
		{
			Sleep(200);	// try a second time.
			if (!m_pHistory->Read(pcComputerId, &m_lComputer.at(iCount)->m_lHistoryLong, true))
			{
			}
		}

		m_pHistory->CleanupLong(&m_lComputer.at(iCount)->m_lHistoryLong);
		m_lComputer.at(iCount)->m_bHistoryLongNeedsWrite = false;
	}
}

bool CRemoteCombinedHistoryView::GetOperationFromQue()
{
	int				iPos, iNr;
	CLoggingMessage sLog;
	CString			sFrom, sTxt, sComputer;
	CC_STATUS		*pC_STATUS;

	if (m_bOperationsQueLocked) return true;
	for (iPos = 0; iPos < (int) m_lOperationsQue.size(); iPos++)
	{
		// get first one and start
		int iThreadId;
		m_pOperationItemToThread = m_lOperationsQue.at(iPos);
		
		// find computer

		m_pDoc->GetRpcThreadId(m_pOperationItemToThread->m_pcComputer, &iThreadId);

		if (iThreadId != 0)
		{
			if (m_pOperationItemToThread->m_iTypeOperation == OPERATION_PROJECT)
			{
				::PostThreadMessage(iThreadId,UWM_MSG_THREAD_PROJECT_OPERATION_SEND_READY, (WPARAM) this->m_hWnd, (LPARAM) m_pOperationItemToThread);
			}
			else
			{
//				bool bOk = false;
				pC_STATUS = NULL;
				if (m_pOperationItemToThread->m_iTypeOperation == OPERATION_COMPUTER)
				{
					sComputer = m_pOperationItemToThread->m_pcComputer;
					iNr = m_pDoc->GetComputerNumber(&sComputer);
					if (iNr >=0)
					{
						pC_STATUS = m_pDoc->GetHostStatus(iNr);

						switch(m_pOperationItemToThread->m_iOperation)
						{
							case COMPUTER_SNOOZE:
								pC_STATUS->task_mode = RUN_MODE_NEVER;
								pC_STATUS->task_mode_delay = m_pOperationItemToThread->m_iTime;
							break;
							case COMPUTER_SNOOZE_GPU:
								pC_STATUS->task_mode = RUN_MODE_NEVER;
								pC_STATUS->task_mode_delay = m_pOperationItemToThread->m_iTime;
							break;
							case COMPUTER_SNOOZE_CANCEL_GPU:
								pC_STATUS->task_mode = RUN_MODE_AUTO;
								pC_STATUS->task_mode_delay = 0;
							break;
							case COMPUTER_SUSPEND_NETWORK:
								pC_STATUS->network_mode = RUN_MODE_NEVER;
							break;
							case COMPUTER_RESUME_NETWORK:
								pC_STATUS->network_mode = RUN_MODE_ALWAYS;
							break;
							default: // COMPUTER_SNOOZE_CANCEL
								pC_STATUS->task_mode = RUN_MODE_AUTO;
								pC_STATUS->task_mode_delay = 0;
						}
						m_pOperationItemToThread->pC_STATUS = pC_STATUS;
						::PostThreadMessage(iThreadId, UWM_MSG_THREAD_COMPUTER_OPERATION_SEND_READY, (WPARAM) this->m_hWnd, (LPARAM)m_pOperationItemToThread);
							m_bOperationsQueLocked = true;
						return true;			// only one
					}
					return false;
				}
				else
				{
					::PostThreadMessage(iThreadId,UWM_MSG_THREAD_TASK_OPERATION_SEND_READY, (WPARAM) this->m_hWnd, (LPARAM) m_pOperationItemToThread);
				}
			}
			m_bOperationsQueLocked = true;
			return true;			// only one
		}
		else
		{
			sFrom = "Rule: ";
			sFrom+= m_pOperationItemToThread->m_pcDescription;
			sTxt = "The computer could not be found: ";
			sTxt+= m_pOperationItemToThread->m_pcComputer;
			sLog.m_pFrom = sFrom.GetBuffer(); sLog.m_pText = sTxt.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);

			// clear this item from the list
			delete m_pOperationItemToThread;
			m_pOperationItemToThread = NULL;
			m_lOperationsQue.erase(m_lOperationsQue.begin() + iPos);
//			m_lOperationsQue.RemoveAt(posLast);
			return false;
		}
	}
	return false;
}


void CRemoteCombinedHistoryView::RulesCheckAll()
{
	CHistoryItem *pHistoryItem;
	CRuleItem *pRuleItem;
	UINT		iSize, iPos;

	std::deque<CHistoryItem*> *plHistory;
	bool	bChanged;
	LRESULT result;
	CString sComputer;
	bool	bComputerFound;
	int		iNrRpc;
//	int		*piDummy;
	CString sName, sRule;
	CLoggingMessage sLog;

	result  = theApp.m_pDlgSettingsRules->SendMessage(UWM_MSG_RULES_GET_COPY,(WPARAM) &m_lRules,0);	// get a copy of the rules
	bChanged = (result != 0);

	iNrRpc = m_pDoc->GetRpcSelectedComputers();
		
//	m_pDoc->GetRpcThreadId(&piDummy);

	if (bChanged)
	{
		m_bRuleTemperature = false;
		m_bRuleWallClock = false;
		m_bRuleStatus = false;
		m_bRuleConnection = false;
		m_bRuleTimeLeft = false;

		// rules changed so delete all active items.
		iSize = (int) m_lRulesActive.size();
		for (iPos = 0; iPos < iSize; iPos++)
		{
			delete m_lRulesActive.at(iPos);
		}
		m_lRulesActive.clear();

		iSize = (int) m_lRules.size();

		sName = "Rule(s)";
		sRule.Format("Active: %d", iSize);
		sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sRule.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		sName.ReleaseBuffer();
		sRule.ReleaseBuffer();

		// log the changed rules

		for (iPos = 0; iPos < iSize; iPos++)
		{
			pRuleItem = m_lRules.at(iPos);
//			pRuleItem->m_plInterval = NULL;

			sRule = pRuleItem->m_sComputer;
			sRule+= ", ";
			sRule+= pRuleItem->m_sProject;
			sRule+= ", ";
			sRule+= pRuleItem->m_sApplication; 
			sRule+= ", ";
			CRules rules;
			rules.ShowRule(pRuleItem, &sRule);

			sName = "Rule: " + pRuleItem->m_sName;

			sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sRule.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
			sName.ReleaseBuffer();
			sRule.ReleaseBuffer();
			
			// check for a temperature rule
			for (int iCount = 0; iCount < RULE_ITEMS; iCount++)
			{
				if (pRuleItem->m_iType[iCount] == RULES_TYPE_TEMPERATURE)
				{
					m_bRuleTemperature = true;
				}
				if (pRuleItem->m_iType[iCount] == RULES_TYPE_TIME_INTERVAL)
				{
					m_bRuleWallClock = true;
				}
				if (pRuleItem->m_iType[iCount] == RULES_TYPE_STATUS)
				{
					m_bRuleStatus = true;
				}
				if (pRuleItem->m_iType[iCount] == RULES_TYPE_CONNECTION)
				{
					m_bRuleConnection = true;
				}
				if (pRuleItem->m_iType[iCount] == RULES_TYPE_TIMELEFT_PROJECT)
				{
					m_bRuleTimeLeft = true;
				}
			}

			sComputer = pRuleItem->m_sComputer;
			if (sComputer.GetLength() > 0)
			{
				bComputerFound = false;
				for (int iCount = 0; iCount < iNrRpc; iCount++)
				{
					if (sComputer == m_lComputerHistoryItem.at(iCount)->m_pcReadyRpcComputerId)
					{
						bComputerFound = true;
					}
				}
				if (!bComputerFound)
				{
					sComputer = "This rule will not work! No matching computer found for: " + sComputer;
					sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sComputer.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
					sName.ReleaseBuffer();
					sComputer.ReleaseBuffer();
				}
			}
		}
		sName = "==============================================================================";
		sRule = "";
		sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sRule.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		sName.ReleaseBuffer();
		sRule.ReleaseBuffer();
	}
	RulesActiveReset();

	// tell the notices that the rules are gone.
	m_pDoc->m_pDlgNotices->SendMessage(UWM_MSG_NOTICES_BT_RULE, 0, (LPARAM) &m_lRulesActive);

	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		plHistory = &m_lComputer.at(iCount)->m_lHistory;
		for (iPos = 0; iPos < plHistory->size(); iPos++)
		{
			pHistoryItem = plHistory->at(iPos);
			if((pHistoryItem->m_iState&0xffffe) == STATE_RUNNING)	// rules only apply to running tasks
			{
				RulesTripped(pHistoryItem);
			}
		}
	}

	if (m_bRuleWallClock)
	{
		RulesWallClock();
	}

	if (m_bRuleStatus)
	{
		RulesStatus();
	}

	if (m_bRuleConnection)
	{
		RulesConnection();
	}

	if (m_bRuleTimeLeft)
	{
		RulesTimeLeft();
	}

	RulesActiveEvent();
	RulesActiveRemove();
}

void CRemoteCombinedHistoryView::RulesWallClock()
{
	CRuleItem *pRuleItem;
	int		iCount;
	int	iSize, iPos;
	CString sTxt;

	iSize = (int) m_lRules.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRuleItem = m_lRules.at(iPos);
		for (iCount = 0; iCount < RULE_ITEMS; iCount++)
		{
			if (pRuleItem->m_iType[iCount] == RULES_TYPE_TIME_INTERVAL)
			{
				sTxt = "";
				if (RulesCheck(NULL, pRuleItem, &sTxt))
				{
					CHistoryItem historyItem;
					historyItem.AddComputer(pRuleItem->m_sComputer.GetBuffer()); pRuleItem->m_sComputer.ReleaseBuffer();
					historyItem.AddProject(pRuleItem->m_sProject.GetBuffer()); pRuleItem->m_sProject.ReleaseBuffer();
					historyItem.AddName(pRuleItem->m_sName.GetBuffer()); pRuleItem->m_sName.ReleaseBuffer();
					RulesActiveAdd(&historyItem, pRuleItem, &sTxt);
				}
			}
		}
	}
}

void CRemoteCombinedHistoryView::RulesStatus()
{
	int iSize, iPos;
	CRuleItem *pRuleItem;
	int		iCount;
	CString sTxt;

	iSize = (int) m_lRules.size();

	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRuleItem = m_lRules.at(iPos);
		for (iCount = 0; iCount < RULE_ITEMS; iCount++)
		{
			if (pRuleItem->m_iType[iCount] == RULES_TYPE_STATUS)
			{
				sTxt = "";
				if (RulesCheck(NULL, pRuleItem, &sTxt))
				{
					CHistoryItem historyItem;
					historyItem.AddComputer(pRuleItem->m_sComputer.GetBuffer());pRuleItem->m_sComputer.ReleaseBuffer();
					historyItem.AddProject(pRuleItem->m_sProject.GetBuffer());pRuleItem->m_sProject.ReleaseBuffer();
					RulesActiveAdd(&historyItem, pRuleItem, &sTxt, true);
					return;
				}
			}
		}
	}
}

void CRemoteCombinedHistoryView::RulesConnection()
{
	int		iSize, iPos;
	CRuleItem *pRuleItem;
	int		iCount;
	CString sTxt;

	iSize = (int) m_lRules.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRuleItem = m_lRules.at(iPos);
		for (iCount = 0; iCount < RULE_ITEMS; iCount++)
		{
			if (pRuleItem->m_iType[iCount] == RULES_TYPE_CONNECTION)
			{
				sTxt = "";
				if (RuleCheck(0, NULL, pRuleItem, &sTxt))		// there is only one
				{
					RulesActiveAdd(pRuleItem, &sTxt, false, false);
				}
			}
		}
	}
}

void CRemoteCombinedHistoryView::RulesTimeLeft()
{
	int		iSize, iPos;
	CRuleItem *pRuleItem;
	int		iCount;
	CString sTxt;

	iSize = (int) m_lRules.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRuleItem = m_lRules.at(iPos);
		for (iCount = 0; iCount < RULE_ITEMS; iCount++)
		{
			if (pRuleItem->m_iType[iCount] == RULES_TYPE_TIMELEFT_PROJECT)
			{
				sTxt = "";
				if (RuleCheck(0, NULL, pRuleItem, &sTxt))		// there is only one
				{
					RulesActiveAdd(pRuleItem, &sTxt, false, true);
				}
			}
		}
	}
}


bool CRemoteCombinedHistoryView::RulesTripped(CHistoryItem *pHistoryItem)
{
	int		iSize, iPos;
	CRuleItem *pRuleItem;
	bool	bEqual;
	CString sPlanClass, sApplication;
	float fVersion;
	CString sTxt;
	CWildcard	wc;

	iSize = (int) m_lRules.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRuleItem = m_lRules.at(iPos);

		if (pRuleItem->m_iType[0] == RULES_TYPE_TIMELEFT_PROJECT)
		{
			return false;
		}

		if (pRuleItem->m_iType[0] == RULES_TYPE_CONNECTION)
		{
			bEqual = false;
		}
		else
		{
			bEqual = true;
			if (pRuleItem->m_sComputer.GetLength() > 0) 
			{
				if (pHistoryItem->m_pcComputer != pRuleItem->m_sComputer)
				{
					bEqual = false;
				}
			}
			if (pRuleItem->m_sProject.GetLength() > 0) 
			{
				if (strstr(pHistoryItem->m_pcProject, pRuleItem->m_sProject) == NULL)
				{
					bEqual = false;
				}
			}

			if (pRuleItem->m_sApplication.GetLength() > 0) 
			{
				fVersion = (float) pHistoryItem->m_iVersion;
				fVersion/= 100;
	
				sPlanClass = pHistoryItem->m_pcPlanClass;
				if (sPlanClass.GetLength() == 0)
				{
					sApplication.Format("%.2f %s", fVersion, pHistoryItem->m_pcApplication);
				}
				else
				{
					sApplication.Format("%.2f %s (%s)", fVersion, pHistoryItem->m_pcApplication, sPlanClass);
				}
		
				if (pRuleItem->m_bApplicationWc)
				{
					if (!wc.Compare(pRuleItem->m_sApplication.GetBuffer(), sApplication.GetBuffer()))
					{
						bEqual = false;
					}
					pRuleItem->m_sApplication.ReleaseBuffer();
					sApplication.ReleaseBuffer();
				}
				else
				{
					if (sApplication.Find(pRuleItem->m_sApplication) < 0)
					{
						bEqual = false;
					}
				}
			}
		}

		if (bEqual)
		{
			if (RulesCheck(pHistoryItem, pRuleItem, &sTxt))
			{
				// rule active 
				// Warning more than one rule may be active.
				RulesActiveAdd(pHistoryItem, pRuleItem, &sTxt);
			}
		}
	}
	return false;
}

bool CRemoteCombinedHistoryView::RulesCheck(CHistoryItem *pHistoryItem, CRuleItem *pRuleItem, CString *psTxt)
{
	if (RuleCheck(0, pHistoryItem, pRuleItem, psTxt))
	{
		if (RuleCheck(1, pHistoryItem, pRuleItem, psTxt))
		{
			if (RuleCheck(2, pHistoryItem, pRuleItem, psTxt))
			{
				return true;
			}
		}
	}
	return false;
}

bool CRemoteCombinedHistoryView::RuleCheck(int iRuleItem, CHistoryItem *pHistoryItem, CRuleItem *pRuleItem, CString *psTxt)
{
	int iRuleType, iRuleOperator;
	CUseCpuGpu CpuGpu;
	double fCpuUse, fGpuUse, dRatio;
	int		iGpu, iNrTThrottleRpc;
	bool	bCuda;
	bool	bTempFound;
	bool	bStatus;
	bool	bActivated;
	int		iWallClockInterval;
	CString sFormat, sProject, sType;
	CRules rules;
	float	fTemperature, fTemperatureCpu, fTemperatureGpu;
	CHistoryRunItem *pHistoryRunItem;
	UINT	iPos;
	int		iDeadlineTimeLeft;
	__int64	iTimeLeft;
	int		iDayCurrent;
	int		iHourCurrent;
	int		iMinuteCurrent;

	time_t tSystemTime;
	_time64( &tSystemTime);
	CTime	tTime(tSystemTime);
//	int		iHour, iMinute;
	int		iDetectedGpu;

	iRuleType = pRuleItem->m_iType[iRuleItem];
	iRuleOperator = pRuleItem->m_iOperator[iRuleItem];
	switch (iRuleType)
	{
		case RULES_TYPE_DEADLINE:
			if (pHistoryItem == NULL) return false;
			iDeadlineTimeLeft = (int) (pHistoryItem->m_iDeadline - tSystemTime);
			iDeadlineTimeLeft-= (int) (pHistoryItem->m_iTimeLeft);
//			iDeadlineTimeLeft-= 4129206;	test only
			if (iRuleOperator == RULE_OPERATOR_MORE)
			{
				if (iDeadlineTimeLeft > pRuleItem->m_iValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_TIME_DAY, iDeadlineTimeLeft, 0, &sFormat);
					*psTxt+= "(Deadline >" + sFormat + ")";
					return true;
				}
			}
			if (iRuleOperator == RULE_OPERATOR_LESS)
			{
				if (iDeadlineTimeLeft < pRuleItem->m_iValue[iRuleItem])
				{			
					rules.Value(RULE_TYPE_CLASS_TIME_DAY, pRuleItem->m_iValue[iRuleItem], 0, &sFormat);
					rules.Value(RULE_TYPE_CLASS_TIME_DAY, iDeadlineTimeLeft, 0, &sFormat);
					if (iDeadlineTimeLeft < pRuleItem->m_iValue[iRuleItem])
					{
						rules.Value(RULE_TYPE_CLASS_TIME_DAY, iDeadlineTimeLeft, 0, &sFormat);
						*psTxt+= "(Deadline <" + sFormat + ")";
						return true;
					}
				}
			}
		break;
		case RULES_TYPE_ELAPSED:
			if (pHistoryItem == NULL) return false;
			if (iRuleOperator == RULE_OPERATOR_MORE)
			{
				if (pHistoryItem->m_iElapsedTimeCpu > pRuleItem->m_iValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_TIME_DAY, pHistoryItem->m_iElapsedTimeCpu, 0, &sFormat);
					*psTxt+= "(Elapsed >" + sFormat + ")";
					return true;
				}
			}
			else
			{
				if (pHistoryItem->m_iElapsedTimeCpu < pRuleItem->m_iValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_TIME_DAY, pHistoryItem->m_iElapsedTimeCpu, 0, &sFormat);
					*psTxt+= "(Elapsed <" + sFormat + ")";
					return true;
				}
			}
		break;
		case RULES_TYPE_CPUP:
			if (pHistoryItem == NULL) return false;
			pHistoryRunItem = pHistoryItem->m_pHistoryRunItem;
			if (pHistoryRunItem == NULL) return false;
			
			if (pHistoryRunItem->m_iPrev4ElapsedTimeCpu != 0)
			{
				iWallClockInterval = pHistoryItem->m_iElapsedTimeCpu - pHistoryRunItem->m_iPrev4ElapsedTimeCpu;
				dRatio = pHistoryItem->m_iElapsedTimeGpu - pHistoryRunItem->m_iPrev4ElapsedTimeGpu;
				dRatio /= iWallClockInterval;
				dRatio *= 100;
				if (iRuleOperator == RULE_OPERATOR_MORE)
				{
					if (dRatio > pRuleItem->m_dValue[iRuleItem])
					{
						rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, dRatio, &sFormat);
						*psTxt+= "(CPU% > " + sFormat + ")";
						return true;
					}
				}
				if(iRuleOperator == RULE_OPERATOR_LESS)
				{
					if (dRatio < pRuleItem->m_dValue[iRuleItem])
					{
						rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, dRatio, &sFormat);
						*psTxt+= "(CPU% < " + sFormat + ")";
						return true;
					}
				}
				if (iRuleOperator == RULE_OPERATOR_IS)
				{
					if (dRatio = pRuleItem->m_dValue[iRuleItem])
					{
						rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, dRatio, &sFormat);
						*psTxt+= "(CPU% = " + sFormat + ")";
						return true;
					}
				}
			}
		break;
		case RULES_TYPE_PROGRESS:
			if (pHistoryItem == NULL) return false;
			if (iRuleOperator == RULE_OPERATOR_MORE)
			{
				if (pHistoryItem->m_dFraction_done > pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, pHistoryItem->m_dFraction_done, &sFormat);
					*psTxt+= "(Progress > " + sFormat + ")";
					return true;
				}
			}
			if (iRuleOperator == RULE_OPERATOR_LESS)
			{
				if (pHistoryItem->m_dFraction_done < pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, pHistoryItem->m_dFraction_done, &sFormat);
					*psTxt+= "(Progress < " + sFormat + ")";
					return true;
				}
			}
			if (iRuleOperator == RULE_OPERATOR_IS)
			{
				if (pHistoryItem->m_dFraction_done = pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, pHistoryItem->m_dFraction_done, &sFormat);
					*psTxt+= "(Progress = " + sFormat + ")";
					return true;
				}
			}
		break;
		case RULES_TYPE_PROGRESS_DELTA:
			if (pHistoryItem == NULL) return false;
			double dDelta;
			int iWallClockInterval;
			pHistoryRunItem = pHistoryItem->m_pHistoryRunItem;
			if (pHistoryRunItem == NULL) return false;

			if (pHistoryRunItem->m_dPrev4Fraction_done < 0) return false;

			iWallClockInterval = pHistoryItem->m_iElapsedTimeCpu - pHistoryRunItem->m_iPrev4ElapsedTimeCpu;
			dDelta =  pHistoryItem->m_dFraction_done - pHistoryRunItem->m_dPrev4Fraction_done;

			dDelta /= iWallClockInterval;
			dDelta *= 60;

			if (iRuleOperator == RULE_OPERATOR_MORE)
			{
				if (dDelta > pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, dDelta, &sFormat);
					*psTxt+= "(Progress / min > " + sFormat + ")";
					return true;
				}
			}
			if (iRuleOperator == RULE_OPERATOR_LESS)
			{
				if (dDelta < pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, dDelta, &sFormat);
					*psTxt+= "(Progress / min < " + sFormat + ")";
					return true;
				}
			}
			if (iRuleOperator == RULE_OPERATOR_IS)
			{
				if (dDelta = pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, dDelta, &sFormat);
					*psTxt+= "(Progress / min = " + sFormat + ")";
					return true;
				}
			}
		break;
		case RULES_TYPE_TIMELEFT:
			if (pHistoryItem == NULL) return false;
			if (iRuleOperator == RULE_OPERATOR_MORE)
			{
				if (pHistoryItem->m_iTimeLeft > pRuleItem->m_iValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_TIME_DAY, pHistoryItem->m_iTimeLeft, 0, &sFormat);
					*psTxt+= "(Time left >" + sFormat + ")";
					return true;
				}
			}
			else
			{
				if (pHistoryItem->m_iTimeLeft < pRuleItem->m_iValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_TIME_DAY, pHistoryItem->m_iTimeLeft, 0, &sFormat);
					*psTxt+= "(Time left <" + sFormat + ")";
					return true;
				}
			}
		break;
		case RULES_TYPE_USE:
			if (pHistoryItem == NULL) return false;
			CpuGpu.GetCpuGpu(pHistoryItem->m_pcUse, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
			if (fCpuUse == pRuleItem->m_dValue[iRuleItem])
			{
				if (fGpuUse == ((double) pRuleItem->m_iValue[iRuleItem] / (double) 1000))
				{
					rules.Value(RULE_TYPE_CLASS_USE, pRuleItem->m_iValue[iRuleItem], pRuleItem->m_dValue[iRuleItem], &sFormat);
					*psTxt+= "(Use: " + sFormat + ")";
					return true;
				}
			}
		break;
		case RULES_TYPE_TEMPERATURE:
			if (pHistoryItem == NULL) return false;
			bTempFound = false;

			iNrTThrottleRpc = m_pDoc->GetRpcSelectedComputers();
			for (int iCount = 0; iCount < iNrTThrottleRpc; iCount++)
			{
				if (pRuleItem->m_sComputer == m_lComputerRpcInfoClient.at(iCount)->m_sComputerId.c_str())
				{
					if (m_lComputerRpcInfoClient.at(iCount)->m_bReadyCopy)
					{
						fTemperatureCpu = (float) m_lComputerRpcInfoClient.at(iCount)->m_fTopCpuTemperatureCopy;
						fTemperatureGpu = (float) m_lComputerRpcInfoClient.at(iCount)->m_fTopGpuTemperatureCopy;
						bTempFound = true;
					}
				}
			}
			if (!bTempFound) return false;

			CpuGpu.GetCpuGpu(pHistoryItem->m_pcUse, &fCpuUse, &fGpuUse, &iGpu, &bCuda, &iDetectedGpu);
			if (bCuda)
			{
				fTemperature = fTemperatureGpu;
			}
			else
			{
				fTemperature = fTemperatureCpu;
			}

			if (iRuleOperator == RULE_OPERATOR_MORE)
			{
				if (fTemperature > pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_TEMPERATURE, 0, fTemperature, &sFormat);
					*psTxt+= "(Temperature >" + sFormat + ")";
					return true;
				}
			}
			else
			{
				if (fTemperature < pRuleItem->m_dValue[iRuleItem])
				{
					rules.Value(RULE_TYPE_CLASS_TEMPERATURE, 0, fTemperature, &sFormat);
					*psTxt+= "(Temperature <" + sFormat + ")";
					return true;
				}
			}
		break;
		case RULES_TYPE_STATUS:
			if (pHistoryItem != NULL) return false;
			int	iArrayPos, iStatus, iStatusCount, iFound;

			std::deque<CHistoryItem*> *plHistory;
//			CXList <CHistoryItem*,CHistoryItem*> *plHistory;
			time_t tTrigger;


			iArrayPos = m_pDoc->GetRpcArrayPos(&pRuleItem->m_sComputer);
			if (iArrayPos >= 0)
			{
				iStatusCount = (int) pRuleItem->m_iValue[iRuleItem];
				iStatus = (int) pRuleItem->m_dValue[iRuleItem];
				tTrigger = tSystemTime - pRuleItem->m_iTime;
				sProject = pRuleItem->m_sProject;
				iFound = 0;

				plHistory = &m_lComputer.at(iArrayPos)->m_lHistory;
//				iSize = (int) plHistory->size();

				for (iPos = 0; iPos < plHistory->size(); iPos++)
				{
					pHistoryItem = plHistory->at(iPos);
					if (pHistoryItem->m_iState >> 16 == iStatus)
					{
						if (pHistoryItem->m_pcProject == sProject)
						{
							if (pHistoryItem->m_i64CompletedTime >  tTrigger)
							{
								iFound++;
							}
						}
					}
				}
				if (iRuleOperator == RULE_OPERATOR_MORE)
				{
					if (iFound > iStatusCount)
					{
						rules.Value(RULE_TYPE_CLASS_STATUS, pRuleItem->m_iValue[iRuleItem], pRuleItem->m_dValue[iRuleItem], &sFormat);
						*psTxt+= "(Status >" + sFormat + ")";
						return true;
					}
				}
				if (iRuleOperator == RULE_OPERATOR_LESS)
				{
					if (iFound < iStatusCount)
					{
						rules.Value(RULE_TYPE_CLASS_STATUS, pRuleItem->m_iValue[iRuleItem], pRuleItem->m_dValue[iRuleItem], &sFormat);
						*psTxt+= "(Status <" + sFormat + ")";
						return true;
					}
				}
				if (iRuleOperator == RULE_OPERATOR_IS)	
				{
					if (iFound == iStatusCount)
					{
						rules.Value(RULE_TYPE_CLASS_STATUS, pRuleItem->m_iValue[iRuleItem], pRuleItem->m_dValue[iRuleItem], &sFormat);
						*psTxt+= "(Status =" + sFormat + ")";
						return true;
					}
				}
			}
		break;
		case RULES_TYPE_TIME_INTERVAL:
			if (pHistoryItem != NULL) return false;

			if (pRuleItem->m_plInterval == NULL)
			{
				return false;
			}

			iDayCurrent = tTime.GetDayOfWeek()-1;
			iHourCurrent = tTime.GetHour();
			iMinuteCurrent = tTime.GetMinute();

			if (!pRuleItem->m_iEventTypeInternal == 0)
			{
				rules.EventType(pRuleItem->m_iEventTypeInternal, &sType);
			}
			else
			{
				sType = "Missing event";
			}
			bActivated = true;
			for (int iInterval = 0; iInterval < (int) pRuleItem->m_plInterval->size(); iInterval++)
			{
				if (pRuleItem->m_plInterval->at(iInterval)->m_bInvers)
				{
					if (rules.InInterval(iDayCurrent, iHourCurrent, iMinuteCurrent, pRuleItem->m_plInterval->at(iInterval)))
					{
						bActivated = false;
						break;
					}
				}
				else
				{
					bActivated = false;
					if (rules.InInterval(iDayCurrent, iHourCurrent, iMinuteCurrent, pRuleItem->m_plInterval->at(iInterval)))
					{
						rules.IntervalStringSingle(pRuleItem->m_plInterval->at(iInterval), &sFormat);
						*psTxt+= "(Wallclock interval: " + sFormat + ", " + sType + ")";
						return true;
					}
				}
			}

			if (bActivated)
			{
				*psTxt+= "(Wallclock interval: " + sType + ")";
				return true;
			}

		break;

		case RULES_TYPE_CONNECTION:
			bStatus = false;
			for (int iCount = 0; iCount < m_iReadyRpcCount; iCount++)
			{
				if ((pRuleItem->m_sComputer == "") ||  (m_lComputerHistoryItem.at(iCount)->m_pcReadyRpcComputerId == pRuleItem->m_sComputer)) 
				{
					if (iRuleOperator == RULE_OPERATOR_NOTEQUAL)
					{
						if (m_lComputerHistoryItem.at(iCount)->m_iReadyRpcConnectionStatus != RPC_STATUS_CONNECTED)
						{
//							if (m_iReadyRpcConnectionStatus[iCount] != RPC_STATUS_ERROR_LOST_CONNECTION)
							{
								*psTxt+= "(Connection: Not connected: ";
								*psTxt+= m_lComputerHistoryItem.at(iCount)->m_pcReadyRpcComputerId;
								*psTxt+= ") " ;
								bStatus = true;
							}
						}
					}
					if (iRuleOperator == RULE_OPERATOR_NOLONGEREQUAL)
					{
						if (m_lComputerHistoryItem.at(iCount)->m_iReadyRpcConnectionStatus == RPC_STATUS_ERROR_LOST_CONNECTION)
						{
							*psTxt+= "(Connection: Lost connection: ";
							*psTxt+= m_lComputerHistoryItem.at(iCount)->m_pcReadyRpcComputerId;
							*psTxt+= ") " ;
							bStatus = true;
						}
					}
				}
			}
			return bStatus;
		break;
		case RULES_TYPE_TIMELEFT_PROJECT:
			bStatus = false;
			for (int iCount = 0; iCount < m_iReadyRpcCount; iCount++)
			{
				if (m_lComputer.at(iCount)->m_lHistoryCount.size() > 0)
				{
					if (m_lComputer.at(iCount)->m_lHistoryCount.at(0)->m_pcComputer == pRuleItem->m_sComputer) 
					{
						for (int iProject = 0; iProject < (int) m_lComputer.at(iCount)->m_lHistoryCount.size(); iProject++)
						{
							if (m_lComputer.at(iCount)->m_lHistoryCount.at(iProject)->m_pcProject == pRuleItem->m_sProject) 
							{
								iTimeLeft = m_lComputer.at(iCount)->m_lHistoryCount.at(iProject)->m_iTimeLeftCpu;
								iTimeLeft+= m_lComputer.at(iCount)->m_lHistoryCount.at(iProject)->m_iTimeLeftGpu;
								if (iRuleOperator == RULE_OPERATOR_MORE)
								{
									if (iTimeLeft > pRuleItem->m_iValue[iRuleItem])
									{
										rules.Value(RULE_TYPE_CLASS_TIME_DAY, iTimeLeft, 0, &sFormat);
										*psTxt+= "(Time left project >" + sFormat + ")";

										bStatus = true;
										break;
									}
								}
								if (iRuleOperator == RULE_OPERATOR_LESS)
								{
									if (iTimeLeft < pRuleItem->m_iValue[iRuleItem])
									{
										rules.Value(RULE_TYPE_CLASS_TIME_DAY, iTimeLeft, 0, &sFormat);
										*psTxt+= "(Time left project <" + sFormat + ")";
										bStatus = true;
										break;
									}
								}
								break;
							}
						}
						break;
					}
				}
			}
			return bStatus;	
		break;

		case RULES_TYPE_NONE:
			return true;		// trip rules that are not active
		break;
	}
	return false;
}

void CRemoteCombinedHistoryView::RulesActiveReset()
{
	UINT	iPos;

	for (iPos = 0; iPos < m_lRulesActive.size(); iPos++)
	{
		m_lRulesActive.at(iPos)->m_bActive = false;
	}
}

// rule based on the connection
void CRemoteCombinedHistoryView::RulesActiveAdd(CRuleItem *pRuleItem, CString *psTxt, bool bTimeNull, bool bProject)
{
	int		iSize, iPos;
	CActiveRuleItem* pActiveRuleItem;
	bool	bActive;
	time_t tSystemTime;

	_time64(&tSystemTime);

	bActive = false;

	iSize = (int) m_lRulesActive.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pActiveRuleItem = m_lRulesActive.at(iPos);

		if (pRuleItem->m_sName == pActiveRuleItem->m_sRuleName)
		{
			if (pRuleItem->m_sComputer == pActiveRuleItem->m_sComputer)
			{
				bActive = true;
				pActiveRuleItem->m_bActive = true;
				break;
			}
		}
	}

	if (!bActive)
	{
		pActiveRuleItem = new CActiveRuleItem;
		pActiveRuleItem->m_bActive = true;
		pActiveRuleItem->m_color = pRuleItem->m_color;
		pActiveRuleItem->m_sRuleName = pRuleItem->m_sName;
		pActiveRuleItem->m_iTime = (int) pRuleItem->m_iValue[0];
		pActiveRuleItem->m_iSnooze = (int)pRuleItem->m_iSnooze;

		if (bTimeNull)
		{
			pActiveRuleItem->m_iTriggerTime = tSystemTime;
		}
		else
		{
			pActiveRuleItem->m_iTriggerTime = tSystemTime + pRuleItem->m_iTime;	// wall clock + time
		}

		pActiveRuleItem->m_sTaskName = "";							// pHistoryItem->m_pcName;

		pActiveRuleItem->m_sComputer = pRuleItem->m_sComputer;		// pHistoryItem->m_pcComputer;

		if (bProject)
		{
			pActiveRuleItem->m_sProject = pRuleItem->m_sProject;
		}
		else
		{
			pActiveRuleItem->m_sProject = "";
		}
		pActiveRuleItem->m_sApplication = "";						// pHistoryItem->m_pcApplication;
		pActiveRuleItem->m_iElapsedTime = 0;						// pHistoryItem->m_iElapsedTimeCpu;
		pActiveRuleItem->m_fProgress = 0;							// pHistoryItem->m_dFraction_done;

		pActiveRuleItem->m_sRuleText = *psTxt;

		pActiveRuleItem->m_bEventTriggered = false;

		pActiveRuleItem->m_iEventShow = pRuleItem->m_iEventShow;
		pActiveRuleItem->m_iEventTypeInternal = pRuleItem->m_iEventTypeInternal;
		pActiveRuleItem->m_iEventTypeExternal = pRuleItem->m_iEventTypeExternal;
		pActiveRuleItem->m_sEvent = pRuleItem->m_sEvent;

//		m_lRulesActive.AddTail(pActiveRuleItem);
		m_lRulesActive.push_back(pActiveRuleItem);
	}
}


// rule based on the history
void CRemoteCombinedHistoryView::RulesActiveAdd(CHistoryItem *pHistoryItem, CRuleItem *pRuleItem, CString *psTxt, bool bTimeNull)
{
	UINT	iPos;
	CActiveRuleItem* pActiveRuleItem;
	bool	bActive;
	time_t tSystemTime;

	_time64(&tSystemTime);

	if (pRuleItem->m_iBackoffUntill > tSystemTime)
	{
		return;		// don't add in backup
	}

	pRuleItem->m_iBackoffUntill = 0;
	bActive = false;

	for (iPos = 0; iPos < m_lRulesActive.size(); iPos++)
	{
		pActiveRuleItem = m_lRulesActive.at(iPos);
		if (pHistoryItem->m_pcName != NULL)
		{
			if (pActiveRuleItem->m_sTaskName == pHistoryItem->m_pcName)	// task still there
			{
				bActive = true;
				pActiveRuleItem->m_bActive = true;
			}
		}
	}
	if (!bActive)
	{
		pActiveRuleItem = new CActiveRuleItem;
		pActiveRuleItem->m_bActive = true;
		pActiveRuleItem->m_color = pRuleItem->m_color;
		pActiveRuleItem->m_sRuleName = pRuleItem->m_sName;
		pActiveRuleItem->m_iTime = (int) pRuleItem->m_iValue[0];
		pActiveRuleItem->m_iSnooze = (int) pRuleItem->m_iSnooze;

		if (bTimeNull)
		{
			pActiveRuleItem->m_iTriggerTime = tSystemTime;
		}
		else
		{
			pActiveRuleItem->m_iTriggerTime = tSystemTime + pRuleItem->m_iTime;	// wall clock + time
		}

		pActiveRuleItem->m_sTaskName = pHistoryItem->m_pcName;

		pActiveRuleItem->m_sComputer = pHistoryItem->m_pcComputer;
		pActiveRuleItem->m_sProject = pHistoryItem->m_pcProject;
		pActiveRuleItem->m_sApplication = pHistoryItem->m_pcApplication;
		pActiveRuleItem->m_iElapsedTime = pHistoryItem->m_iElapsedTimeCpu;
		pActiveRuleItem->m_fProgress = pHistoryItem->m_dFraction_done;

		pActiveRuleItem->m_sRuleText = *psTxt;

		pActiveRuleItem->m_bEventTriggered = false;

		pActiveRuleItem->m_iEventShow = pRuleItem->m_iEventShow;
		pActiveRuleItem->m_iEventTypeInternal = pRuleItem->m_iEventTypeInternal;
		pActiveRuleItem->m_iEventTypeExternal = pRuleItem->m_iEventTypeExternal;
		pActiveRuleItem->m_sEvent = pRuleItem->m_sEvent;

		m_lRulesActive.push_back(pActiveRuleItem);
	}
}

void CRemoteCombinedHistoryView::RulesActiveRemove()
{
	UINT			iPos;
	CActiveRuleItem* pActiveRuleItem;

	for (iPos = 0; iPos < m_lRulesActive.size(); iPos++)
	{
		pActiveRuleItem = m_lRulesActive.at(iPos);
		if (!pActiveRuleItem->m_bActive)
		{
			// no longer found remove

			CLoggingMessage sLog;
			CString sName, sFormat;
			sName = "Rule: " + pActiveRuleItem->m_sRuleName;
			sFormat = "No longer active: ";
			if (pActiveRuleItem->m_sComputer.GetLength() > 0)
			{
				sFormat += pActiveRuleItem->m_sComputer;
				sFormat += ", ";
			}
			if (pActiveRuleItem->m_sProject.GetLength() > 0)
			{
				sFormat += pActiveRuleItem->m_sProject;
				sFormat += ", ";
			}
			if (pActiveRuleItem->m_sApplication.GetLength() > 0)
			{
				sFormat += pActiveRuleItem->m_sApplication;
				sFormat += ", ";
			}
//			if (pActiveRuleItem->m_sTaskName.GetLength() > 0)
//			{
//				sFormat += pActiveRuleItem->m_sTaskName;
//				sFormat += ", ";
//			}
			sFormat += pActiveRuleItem->m_sRuleText;
			sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sFormat.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_DEBUG);
			sFormat.ReleaseBuffer();

			if (pActiveRuleItem->m_iEventShow == EVENT_SHOW_NOTICE)
			{
				m_pDoc->m_pDlgNotices->SendMessage(UWM_MSG_NOTICES_BT_RULE, 0, (LPARAM) &m_lRulesActive);
			}	

			delete pActiveRuleItem;
			m_lRulesActive.erase(m_lRulesActive.begin() + iPos);
		}
	}
}

void CRemoteCombinedHistoryView::RulesActiveEvent()
{
	UINT	iPos;
	CActiveRuleItem* pActiveRuleItem;
	CRules rules;
	CString sSnooze;

	time_t tSystemTime;
	_time64(&tSystemTime);

	for (iPos = 0; iPos < m_lRulesActive.size(); iPos++)
	{
		pActiveRuleItem = m_lRulesActive.at(iPos);

		if (tSystemTime > pActiveRuleItem->m_iTriggerTime)
		{
			if (!pActiveRuleItem->m_bEventTriggered)
			{
				// triggered
				pActiveRuleItem->m_bEventTriggered = true;

				CLoggingMessage sLog;
				CString sFormat, sElapsed, sProgress;

				rules.Value(RULE_TYPE_CLASS_TIME_DAY, pActiveRuleItem->m_iElapsedTime, 0, &sElapsed);
				rules.Value(RULE_TYPE_CLASS_PERCENTAGE, 0, pActiveRuleItem->m_fProgress, &sProgress);

				sFormat = pActiveRuleItem->m_sComputer;
				if (pActiveRuleItem->m_sComputer != "")	sFormat+= ", ";
				sFormat+= pActiveRuleItem->m_sProject;
				if (pActiveRuleItem->m_sProject != "")	sFormat+= ", ";
				sFormat+= pActiveRuleItem->m_sApplication;
				if (pActiveRuleItem->m_sApplication != "") 	sFormat+= ", ";
//				sFormat+= pActiveRuleItem->m_sTaskName;
//				if (pActiveRuleItem->m_sTaskName != "") sFormat+= ", ";
				sFormat+= pActiveRuleItem->m_sRuleText;
				if (pActiveRuleItem->m_sRuleText != "") 	sFormat+= ", ";

				CString sName;
				sName = "Rule: " + pActiveRuleItem->m_sRuleName;
				sName+= ", trigger";

				sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sFormat.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_DEBUG);
				sFormat.ReleaseBuffer();

				// execute events.
				int		iShowLog;
				bool	bNewOperation;
				CString sEvent;
				bNewOperation = false;

				iShowLog = pActiveRuleItem->m_iEventShow;

				COperationItem *pOperationItem = NULL;
				if (pActiveRuleItem->m_iEventTypeInternal != EVENT_NONE)
				{
					pOperationItem = new COperationItem;

					pOperationItem->AddDescription(pActiveRuleItem->m_sRuleName.GetBuffer()); pActiveRuleItem->m_sRuleName.ReleaseBuffer();
					pOperationItem->AddComputer(pActiveRuleItem->m_sComputer.GetBuffer()); pActiveRuleItem->m_sComputer.ReleaseBuffer();
					pOperationItem->AddProject(pActiveRuleItem->m_sProject.GetBuffer()); pActiveRuleItem->m_sProject.ReleaseBuffer();
					pOperationItem->AddTask(pActiveRuleItem->m_sTaskName.GetBuffer()); pActiveRuleItem->m_sTaskName.ReleaseBuffer();
					bNewOperation = true;
				}

				rules.EventType(pActiveRuleItem->m_iEventTypeInternal, &sEvent);
				sEvent = sEvent;

				if (pOperationItem != NULL)	pOperationItem->m_iEventTypeInternal = pActiveRuleItem->m_iEventTypeInternal;

				sSnooze = "";

				switch (pActiveRuleItem->m_iEventTypeInternal)
				{
					case EVENT_TYPE_SUSPEND_PROJECT:
						pOperationItem->m_iTypeOperation = OPERATION_PROJECT;
						pOperationItem->m_iOperation = PROJECT_OP_SUSPEND;
//						sEvent = "Project suspended";
					break;
					case EVENT_TYPE_RESUME_PROJECT:
						pOperationItem->m_iTypeOperation = OPERATION_PROJECT;
						pOperationItem->m_iOperation = PROJECT_OP_RESUME;
//						sEvent = "Project resumed";
					break;
					case EVENT_TYPE_SNOOZE:
						pOperationItem->m_iTypeOperation = OPERATION_COMPUTER;
						pOperationItem->m_iOperation = COMPUTER_SNOOZE;
						pOperationItem->m_iTime = (int) pActiveRuleItem->m_iSnooze;
						sSnooze.Format("(For: %d sec)", pOperationItem->m_iTime);
					break;
					case EVENT_TYPE_SNOOZE_GPU:
						pOperationItem->m_iTypeOperation = OPERATION_COMPUTER;
						pOperationItem->m_iOperation = COMPUTER_SNOOZE_GPU;
						pOperationItem->m_iTime = (int) pActiveRuleItem->m_iSnooze;
						sSnooze.Format("GPU For: %d sec)", pOperationItem->m_iTime);
					break;
					case EVENT_TYPE_CANCEL_SNOOZE:
						pOperationItem->m_iTypeOperation = OPERATION_COMPUTER;
						pOperationItem->m_iOperation = COMPUTER_SNOOZE_CANCEL;
						pOperationItem->m_iTime = (int) pActiveRuleItem->m_iSnooze;
//						sEvent = "Cancel snooze";
					break;
					case EVENT_TYPE_CANCEL_SNOOZE_GPU:
						pOperationItem->m_iTypeOperation = OPERATION_COMPUTER;
						pOperationItem->m_iOperation = COMPUTER_SNOOZE_CANCEL_GPU;
						pOperationItem->m_iTime = pActiveRuleItem->m_iSnooze;
//						sEvent = "Cancel snooze GPU";
					break;
					case EVENT_TYPE_NO_NEW_WORK:
						pOperationItem->m_iTypeOperation = OPERATION_PROJECT;
						pOperationItem->m_iOperation = PROJECT_OP_NOMOREWORK;
//						sEvent = "Project no new work";
					break;
					case EVENT_TYPE_ALLOW_NEW_WORK:
						pOperationItem->m_iTypeOperation = OPERATION_PROJECT;
						pOperationItem->m_iOperation = PROJECT_OP_ALLOWMOREWORK;
//						sEvent = "Project allow new work";
					break;
					case EVENT_TYPE_SUSPEND_TASK:
						pOperationItem->m_iTypeOperation = OPERATION_TASK;
						pOperationItem->m_iOperation = TASKS_OP_SUSPEND;
//						sEvent = "Task suspended";
					break;
					case EVENT_TYPE_SUSPEND_NETWORK:
						pOperationItem->m_iTypeOperation = OPERATION_COMPUTER;
						pOperationItem->m_iOperation = COMPUTER_SUSPEND_NETWORK;
						//						sEvent = "Project allow new work";
						break;
					case EVENT_TYPE_RESUME_NETWORK:
						pOperationItem->m_iTypeOperation = OPERATION_COMPUTER;
						pOperationItem->m_iOperation = COMPUTER_RESUME_NETWORK;
						//						sEvent = "Task suspended";
						break;
				}


				if (bNewOperation)
				{
					m_lOperationsQue.push_back(pOperationItem);
					if (GetOperationFromQue())
					{
						sName = "Rule: " + pActiveRuleItem->m_sRuleName;
						sName += ", from que";
						sName += sSnooze;
						sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sEvent.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_DEBUG);
						sEvent.ReleaseBuffer();
						sName.ReleaseBuffer();
					}
				}

				if (pActiveRuleItem->m_iEventTypeExternal == EVENT_TYPE_PROGRAM)
				{
					CString sError;
					if(!rules.Event(pActiveRuleItem->m_iEventTypeExternal, &pActiveRuleItem->m_sEvent, &sError))
					{
						if (iShowLog == EVENT_SHOW_NONE) iShowLog = EVENT_SHOW_LOGGING;
					}
					sName = "Rule: " + pActiveRuleItem->m_sRuleName;
					sLog.m_pFrom = sName.GetBuffer(); sLog.m_pText = sError.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
					sName.ReleaseBuffer();
					sError.ReleaseBuffer();
				}

				if (iShowLog == EVENT_SHOW_LOGGING)
				{
					theApp.m_pDlgRulesLogging->PostMessage(UWM_MSG_SHOW_RULES_DIALOG,0,0);
				}
				else
				{
					if (iShowLog == EVENT_SHOW_NOTICE)
					{
						m_pDoc->m_pDlgNotices->SendMessage(UWM_MSG_NOTICES_BT_RULE, 0, (LPARAM) &m_lRulesActive);
					}
				}
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CRemoteCombinedHistoryView, CTemplateRemoteHistoryView)
	ON_WM_SIZE()
	ON_COMMAND(ID_HEADER_SETHEADERITEMS, &CRemoteCombinedHistoryView::OnHeaderSetItems)
	ON_WM_CONTEXTMENU()
	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_READ_PROJECT, OnReadyProject)
	ON_COMMAND(ID_WWW_1, &CRemoteCombinedHistoryView::OnWww1)
	ON_COMMAND(ID_WWW_2, &CRemoteCombinedHistoryView::OnWww2)
	ON_COMMAND(ID_WWW_3, &CRemoteCombinedHistoryView::OnWww3)
	ON_COMMAND(ID_WWW_4, &CRemoteCombinedHistoryView::OnWww4)
	ON_COMMAND(ID_WWW_5, &CRemoteCombinedHistoryView::OnWww5)
	ON_COMMAND(ID_WWW_6, &CRemoteCombinedHistoryView::OnWww6)
	ON_COMMAND(ID_WWW_7, &CRemoteCombinedHistoryView::OnWww7)
	ON_COMMAND(ID_WWW_8, &CRemoteCombinedHistoryView::OnWww8)
	ON_COMMAND(ID_WWW_9, &CRemoteCombinedHistoryView::OnWww9)
	ON_COMMAND(ID_WWW_10, &CRemoteCombinedHistoryView::OnWww10)
	ON_COMMAND(ID_WWW_11, &CRemoteCombinedHistoryView::OnWww11)
	ON_COMMAND(ID_WWW_12, &CRemoteCombinedHistoryView::OnWww12)
	ON_COMMAND(ID_WWW_13, &CRemoteCombinedHistoryView::OnWww13)
	ON_COMMAND(ID_WWW_14, &CRemoteCombinedHistoryView::OnWww14)
	ON_COMMAND(ID_WWW_15, &CRemoteCombinedHistoryView::OnWww15)
	ON_COMMAND(ID_WWW_16, &CRemoteCombinedHistoryView::OnWww16)
	ON_COMMAND(ID_WWW_17, &CRemoteCombinedHistoryView::OnWww17)
	ON_COMMAND(ID_WWW_18, &CRemoteCombinedHistoryView::OnWww18)
	ON_COMMAND(ID_WWW_19, &CRemoteCombinedHistoryView::OnWww19)
	ON_COMMAND(ID_WWW_20, &CRemoteCombinedHistoryView::OnWww20)

	ON_MESSAGE (UWM_MSG_PREFERENCE_DIALOG, OnPreferenceDialog)
	ON_MESSAGE (UWM_MSG_PROXY_DIALOG, OnProxyDialog)

	ON_MESSAGE (UWM_MSG_GET_FLOATER_DATA, OnFloaterData)
	ON_MESSAGE (UWM_MSG_GET_CLOUD_DATA, OnCloudData)


	ON_MESSAGE (UWM_MSG_SET_SNOOZE_CPU, OnSetSnoozeCpu)
	ON_MESSAGE (UWM_MSG_SET_SNOOZE_GPU, OnSetSnoozeGpu)

	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, OnSettingsTasksSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, OnSettingsProjectSetWidth)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH, OnSettingsHistorySetWidth)

	ON_MESSAGE (UWM_MSG_THREAD_TASK_OPERATION_READY, OnOperationReady)
	ON_MESSAGE (UWM_MSG_THREAD_PROJECT_OPERATION_READY, OnOperationReady)
	ON_MESSAGE (UWM_MSG_THREAD_COMPUTER_OPERATION_READY, OnOperationReady)

	ON_MESSAGE(UWM_MSG_TAB_VIEW_SELECTED,OnSelTabView)
	ON_MESSAGE(UWM_MSG_VIEW_REFRESH,OnRefresh)

	ON_MESSAGE(UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED, OnComputerBarSelectionChanged)

	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnFontChangeAll)
	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnFontHasChanged)

	ON_MESSAGE(UWM_MSG_TIMER_HYSTORY,OnTimerHistory)
	ON_MESSAGE(UWM_MSG_THREAD_RPC_HISTORY_READY,OnRpcHistoryReady)
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_CLIENT, OnReadyRpcClient)

	ON_MESSAGE (UWM_MSG_SETTING_TASKS, OnSettingsTasks)

	ON_MESSAGE (UWM_MSG_REMOTE_HISTORY_SWITCH, OnSwitch)
	ON_MESSAGE (UWM_MSG_REMOTE_WINDOW_SHOWN, OnSwitch)

	ON_MESSAGE (UWM_MSG_HISTORY_SOMETHING_HAPPENED, OnSomethingHappened)

	ON_COMMAND(ID_HISTORY_COPYALLTOCLIPBOARD, &CRemoteCombinedHistoryView::OnCopyalltoclipboard)
	ON_COMMAND(ID_HISTORY_COPYSELECTEDTOCLIPBOARD, &CRemoteCombinedHistoryView::OnCopyselectedtoclipboard)

	ON_MESSAGE (UWM_DLG_SEARCH_COMPUTER_READY, OnSearchComputerReady)
	ON_MESSAGE (UWM_MSG_WEB_ITEM_SELECTED, OnWebItemSelected)
	ON_MESSAGE(UWM_MSG_WEB_SORT,OnWebSort)

	ON_MESSAGE (UWM_MSG_LONG_HISTORY, OnRequestLongHistory)	

	ON_MESSAGE (UWM_MSG_THREAD_SET_RPC_PROJECT_LIST, OnSetProjectList)	
	
	ON_COMMAND(ID_COMPUTERS_DETECT, &CRemoteCombinedHistoryView::OnComputersDetect)

//	ON_MESSAGE (UWM_MSG_NOTICES_BT_UPDATE, OnBoincTasksVersion)	
END_MESSAGE_MAP()


#ifdef _DEBUG
void CRemoteCombinedHistoryView::AssertValid() const
{
	CTemplateRemoteHistoryView::AssertValid();
}

#ifndef _WIN32_WCE
void CRemoteCombinedHistoryView::Dump(CDumpContext& dc) const
{
	CTemplateRemoteHistoryView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRemoteCombinedHistoryView message handlers

LRESULT CRemoteCombinedHistoryView::OnOperationReady(WPARAM wParam, LPARAM lParam)
{
	int		iPos;
	CLoggingMessage sLog;
	CString sFrom, sTxt, *psLog, sEvent;
	CRules	rules;

	psLog =  (CString *) wParam;

	sFrom = "Rule: ";
	sFrom+= m_pOperationItemToThread->m_pcDescription;

	sTxt = 	"Activated: ";

	if (m_pOperationItemToThread->m_bResult)
	{
		sTxt += "OK, ";
		if (m_pOperationItemToThread->m_pcProject)
		{
			if (strlen(m_pOperationItemToThread->m_pcProject) > 0)
			{
				sTxt+= "Project: ";
				sTxt+= m_pOperationItemToThread->m_pcProject;
				sTxt+= ", ";
			}
		}
	}
	else 
	{
		if (psLog) sTxt += *psLog;
		sTxt += "FAILED, ";

		// move the rule back to inactive;
		UINT	iPos;
		for (iPos = 0; iPos < m_lRulesActive.size(); iPos++)
		{
			if (m_lRulesActive.at(iPos)->m_sRuleName == m_pOperationItemToThread->m_pcDescription)
			{
				m_lRulesActive.at(iPos)->m_bActive = false;
			}
		}
		for (iPos = 0; iPos < m_lRules.size(); iPos++)
		{
			if (m_lRules.at(iPos)->m_sName == m_pOperationItemToThread->m_pcDescription)
			{
				time_t tSystemTime;
				_time64(&tSystemTime);
				m_lRules.at(iPos)->m_iBackoffUntill = tSystemTime + 60 * 15;
				sTxt += "retry in about 15 minutes, ";
			}
		}

	}
	sTxt+= m_pOperationItemToThread->m_pcComputer;
	rules.EventType(m_pOperationItemToThread->m_iEventTypeInternal, &sEvent);
	sTxt+= ", ";
	sTxt+= sEvent;
	sLog.m_pFrom = sFrom.GetBuffer(); sLog.m_pText = sTxt.GetBuffer();theApp.m_pDlgRulesLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);

	if (psLog) delete psLog;

	m_bOperationsQueLocked = false;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	for (iPos = 0; iPos < (int) m_lOperationsQue.size(); iPos++)
	{
		// get matching and remove it from the que list
		COperationItem *pOperationItem;
		pOperationItem = m_lOperationsQue.at(iPos);
		if (m_pOperationItemToThread == pOperationItem)
		{
			delete m_pOperationItemToThread;
			m_lOperationsQue.erase(m_lOperationsQue.begin() + iPos);
			m_pOperationItemToThread = NULL;
			return 1;
		}
	}
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnPreferenceDialog(WPARAM parm1, LPARAM parm2)
{
	CListCtrl& listCtrl = GetListCtrl();
	CString sComputer;
	int	iThreadId, nItem;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_pDoc == NULL) return 0;

	nItem = 0;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = listCtrl.GetNextSelectedItem(pos);
	}
	sComputer = listCtrl.GetItemText(nItem, m_iColumnOrder[COLUMN_HISTORY_COMPUTER]);
	bool bFound = m_pDoc->GetRpcThreadId(&sComputer, &iThreadId);		
	if (!bFound)
	{
		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
		if (iThreadId > 0) bFound = true;
	}
	if (bFound)
	{
		// found computer, now tell the Preference Dialog
		m_pDoc->m_pDlgPrefMain->PostMessage(UWM_MSG_PREFERENCE_OPEN_DIALOG, iThreadId, 0);
		return 0;
	}
	
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnProxyDialog(WPARAM parm1, LPARAM parm2)
{
	CListCtrl& listCtrl = GetListCtrl();
	CString sComputer;
	int	iThreadId, nItem;


	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	if (m_pDoc == NULL) return 0;

	nItem = 0;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = listCtrl.GetNextSelectedItem(pos);
	}

	sComputer = listCtrl.GetItemText(nItem,m_iColumnOrder[COLUMN_HISTORY_COMPUTER]);
	bool bFound = m_pDoc->GetRpcThreadId(&sComputer, &iThreadId);
	if (!bFound)
	{
		iThreadId = theApp.m_pMainFrame->m_iLocalHostThreadId;			// try to fallback on the localhost.
		if (iThreadId > 0) bFound = true;
	}
	if (bFound)
	{
		::PostMessage(m_hWndProxy,UWM_MSG_PROXY_OPEN_DIALOG, iThreadId, 0);
		return 0;
	}
	
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 1)
	{
		m_iSortingChanged	= 1;
	}

	m_tNextHistoryUpdateTime = 0;				// direct action
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnSelTabView(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_pDoc == NULL) return 0;

	m_pDoc->OnShowView((int) wParam, (int) lParam);
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnSwitch(WPARAM parm1, LPARAM parm2)
{
	int iTimeToUpdate;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (parm2 == REFRESH_VIEW_NOW) 
	{
		m_tNextHistoryUpdateTime = 0;				// direct action
	}

	time_t tSystemTime;
	_time64(&tSystemTime);
	iTimeToUpdate = (int) (m_LastSwitchedTime - tSystemTime);

	m_LastSwitchedTime = tSystemTime + 10;			// don't update within 10 seconds.

	if (iTimeToUpdate <= 0) 
	{
		m_tNextHistoryUpdateTime = 0;				// direct action
		m_iSortingChanged	= 4;					// 4x refresh
		if (theApp.m_pDlgSettingsHistory->m_bHistoryEnabled) theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
		else												theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusHistoryDisabled]);
	}
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnSomethingHappened(WPARAM parm1, LPARAM parm2)
{

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	time_t tSystemTime, tNextHistoryUpdateTime;
	_time64( &tSystemTime);

	tNextHistoryUpdateTime = tSystemTime + 5;

	if (m_tNextHistoryUpdateTime > tNextHistoryUpdateTime)
	{
		m_tNextHistoryUpdateTime = tNextHistoryUpdateTime;
	}
	m_bHistoryForcedUpdate = true;
	return 0;

}

LRESULT CRemoteCombinedHistoryView::OnSettingsTasks(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing)
	{
		return 0;			// everything is closing down.
	}
	if (m_pDoc != NULL) m_pDoc->SettingsTasks(parm1 != 0);
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam)
{
	CString *psComputer;
	int		iThreadId, iReconnect;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	psComputer = (CString *) wParam;
	iReconnect = (int) lParam;

	if (psComputer->GetLength() > 0)
	{
		m_pDoc->GetRpcThreadId(psComputer, &iThreadId);
		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,0);
	}
	else
	{
		int iNrRpc = m_pDoc->GetRpcSelectedComputers();	
		for (int iCount = 0; iCount < iNrRpc; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);	
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,iReconnect);
		}
	}

	m_tNextHistoryUpdateTime = 0; // ensure direct timer message
	
	m_iSortingChanged	= 4;			// 4x refresh
	if (theApp.m_pDlgSettingsHistory->m_bHistoryEnabled) theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusSwitched]);
	else												theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusHistoryDisabled]);
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnTimerHistory(WPARAM wParam, LPARAM lParam)
{
	CString		sFormat, sStatus;
	bool		bIconic;
	int			iNrRpc, iTimeToUpdate;

	if (m_pDoc == NULL) return 0;
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	GetOperationFromQue();

	bIconic = !this->IsWindowVisible();
			
	iNrRpc = m_pDoc->GetRpcSelectedComputers();

	if (m_bThreadBusy)		// check for lockup
	{
		if (!bIconic && m_pDoc->m_iViewActive == HISTORY_VIEW_ACTIVE)
		{
			if (m_iThreadLocked > DELAY_SHOW_UPDATE_TIMER) sFormat.Format(" %d", m_iThreadLocked);
			sFormat = gszTranslation[PosWindowStatusMessageStatusUpdating] + sFormat;
			theApp.m_pMainFrame->SetBoincTasksStatus(sFormat.GetBuffer());
			sFormat.ReleaseBuffer();
		}
		if (m_iThreadLocked++ > theApp.m_iThreadTimeout)
		{
			CLoggingMessage sLog;
			CString sFrom, sTxt;
//			theApp.m_bRebootRemoteCombined = true;		// reload
			m_iThreadLocked = 0;
			sFrom = "OnTimerHistory";
			sTxt = "Thread timeout, will restart.";
			sLog.m_pFrom = sFrom.GetBuffer(); sLog.m_pText = sTxt.GetBuffer();theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
			theApp.m_pMainFrame->PostMessage(UWM_MSG_FORCE_A_RESTART,0,0);
		}

		return (int) m_bThreadBusy;
	}
	m_iThreadLocked = 0;


	if (m_bRequestHistoryLong)
	{
		std::deque<CHistoryItem*> *plHistoryLong;
		CHistoryItem *pHistoryItem;

		for (int iPosComputer = 0; iPosComputer < (int) m_lComputer.size(); iPosComputer++)
		{
			plHistoryLong = &m_lComputer.at(iPosComputer)->m_lHistoryLong;
			for (int iPos = 0; iPos < (int) plHistoryLong->size(); iPos++)
			{
				pHistoryItem = new CHistoryItem;
				pHistoryItem->CopyItem(plHistoryLong->at(iPos));
				m_plHistoryLong->push_back(pHistoryItem);
			}
		}
		::PostMessage(m_hWndRequestHistoryLong, UWM_MSG_LONG_HISTORY, 0,0);
		m_bRequestHistoryLong = false;
	}

	time_t tSystemTime;
	_time64(&tSystemTime);

	iTimeToUpdate = (int) (m_tNextHistoryUpdateTime - tSystemTime);
	
	if (iTimeToUpdate <= 0) 
	{
		SendWuRequest(iNrRpc, false);

		bool bTThrottle;
		bTThrottle = false;

		if (theApp.m_pDlgSettingsGeneral->m_bConnecTthrottle)	// check if TThrottle is enabled
		{
			if (m_bRuleTemperature)
			{
				bTThrottle = true;
			}
		}
		if (bTThrottle)
		{
			int iNrTThrottleRpc;
			iNrTThrottleRpc = m_pDoc->GetRpcSelectedComputers();
			SendTThrottleRequest(iNrTThrottleRpc);
		}
		else
		{
			for (int iCount = 0; iCount < (int) m_lComputer.size(); iCount++)
			{
				m_lComputerRpcInfoClient.at(iCount)->m_bReady = false;
				m_lComputerRpcInfoClient.at(iCount)->m_bReadyCopy = false;
				m_lComputerRpcInfoClient.at(iCount)->m_bConnectionLogged = false;
				// NEVER do this here as it is made in RemoteCombinedDoc	delete m_pRpcInfoClient[iCount]->m_pcComputerId;
				m_lComputerRpcInfoClient.at(iCount)->m_sComputerId = "";
			}
		}
		if (m_iThreadLocked > 1) sStatus.Format(" %d", m_iThreadLocked);
		sStatus = TEXT_STATUS_UPDATING + sStatus;
	}
	else
	{
		if (!bIconic)
		{
			if (m_pDoc->m_iViewActive == HISTORY_VIEW_ACTIVE)
			{
				CheckColumnWidth();
				sStatus.Format(gszTranslation[PosWindowStatusMessageStatusUpdateInSec], iTimeToUpdate);		//(UPDATE_HISTORY_TRIGGER-m_iHistoryTimer)+1);				//"Update in %d seconds"
			}
		}
	}

	if (!bIconic)
	{
		if (m_pDoc->m_iViewActive == HISTORY_VIEW_ACTIVE)
		{
			theApp.m_pMainFrame->SetBoincTasksStatus(sStatus.GetBuffer());
		}
	}

	return (int) m_bThreadBusy;
}

void CRemoteCombinedHistoryView::SendWuRequest(int iNrRpc, bool bExcludeFromList)
{
	CString sComputerId;
	HWND	hWnd;
	int		iThreadId;

	hWnd = this->m_hWnd;
		
	m_iHistoryStateReadyCount = 0;

	m_iSendRpcCount = 0;
	m_iReadyRpcCount = 0;
	m_iAddedRpcCount = 0;
	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		m_pDoc->GetRpcThreadId(&iThreadId, iCount);
		m_lComputer.at(iCount)->m_iDeleteTime = theApp.m_pDlgSettingsHistory->m_iHistoryTime;
		m_lComputer.at(iCount)->m_bSmartMode = theApp.m_pDlgSettingsHistory->m_bHistorySmartMode;
		m_lComputer.at(iCount)->m_bAdjustTime = theApp.m_pDlgSettingsView->m_bAdjustTime;
		m_lComputer.at(iCount)->m_bHistoryLessAccurate = theApp.m_pDlgSettingsHistory->m_bHistoryLessAccurate;
		m_lComputer.at(iCount)->m_bMoveToLongHistory = theApp.m_pDlgSettingsHistory->m_bMoveToLongHistory; 
		m_lComputer.at(iCount)->m_iMovetoLongHistoryTime = theApp.m_pDlgSettingsHistory->m_iHistoryLongTime;

		if (m_bHistoryForcedUpdate)
		{
			m_lComputer.at(iCount)->m_iUpdateMaxTime = 2;
			m_lComputer.at(iCount)->m_bHistoryForceUpdate = true;
		}
		else
		{
			m_lComputer.at(iCount)->m_iUpdateMaxTime = theApp.m_pDlgSettingsHistory->m_iHistoryMaxUpdateTime;
			m_lComputer.at(iCount)->m_bHistoryForceUpdate = false;
		}

		m_lComputerHistoryItem.at(iCount)->m_iSendRpcThreadId = iThreadId;

		UINT iSizeTemp;
		iSizeTemp = (UINT) m_lComputer.at(iCount)->m_lHistory.size();
		iSizeTemp = (UINT) m_lComputer.at(iCount)->m_lHistoryCount.size();
		iSizeTemp = (UINT) m_lComputer.at(iCount)->m_lHistoryLong.size();

		::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RPC_HISTORY,(WPARAM) hWnd, (LPARAM) m_lComputer.at(iCount));

		m_iSendRpcCount++;
		m_bThreadBusy = true;	
	}
	m_iTimeLeftLowest = -1;
	m_bHistoryForcedUpdate = false;
}

void CRemoteCombinedHistoryView::SendTThrottleRequest(int iNrTThrottleRpc)
{
	CString sComputerId;
	HWND	hWnd;
	int		iTThrottleThreadId;

	hWnd = this->m_hWnd;

	for (int iCount = 0; iCount < iNrTThrottleRpc; iCount++)
	{
		m_pDoc->GetTThrottleRpcThreadId(&iTThrottleThreadId, iCount);
		m_lComputerRpcInfoClient.at(iCount)->m_bReady = false;
		m_lComputerRpcInfoClient.at(iCount)->m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
		::PostThreadMessage(iTThrottleThreadId,UWM_MSG_THREAD_START_RPC_CLIENT,(WPARAM) hWnd, (LPARAM) m_lComputerRpcInfoClient.at(iCount));
	}
}

void CRemoteCombinedHistoryView::ClearFloaterList()
{
	int		iSize, iPos;

	m_sTip = "";

	iSize = (int) m_lFloater.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		delete  m_lFloater.at(iPos);
	}
	m_lFloater.clear();
}

void CRemoteCombinedHistoryView::TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore, char *pAfter)
{
	int iDay, iHour, iMinute, iSecond;

	if (iTime > 0)
	{
		iMinute = iTime / 60;
		iSecond = iTime - iMinute*60 ;
		iHour	= iMinute / 60;
		iMinute = iMinute - iHour *60;
		iDay	= iHour / 24;
		iHour	= iHour - iDay * 24;

		if (iDay > 0) _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02dd,%02d:%02d:%02d%s",pBefore,iDay,iHour,iMinute,iSecond,pAfter);
		else _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02d:%02d:%02d%s",pBefore, iHour,iMinute,iSecond, pAfter);
	}
 	else strcpy_s(pBuffer,iNrBuffer,"");
}

void CRemoteCombinedHistoryView::CheckWarning(CHistoryCountItem *pHistoryCountItem, CString *psComputerWarning, CString *psProjectWarning, int iWarningCpuTasks, int iWarningGpuTasks, bool *pbWarning)
{
	CString sComputer, sComputerWarning;
	CString sProject, sProjectWarning;

	if (psComputerWarning->GetLength() > 0)
	{
		sComputer = pHistoryCountItem->m_pcComputer;
		sComputerWarning = *psComputerWarning;
		sComputer.MakeLower();
		sComputerWarning.MakeLower();
		if (sComputer.Find(sComputerWarning) < 0)
		{
			return;
		}
	}
	if (psProjectWarning->GetLength() > 0)
	{
		sProject = pHistoryCountItem->m_pcProject;
		sProjectWarning = *psProjectWarning;
		sProject.MakeLower();
		sProjectWarning.MakeLower();

		if (sProject.Find(sProjectWarning) < 0)
		{
			return;
		}
	}
	// match computer and project

	if (iWarningCpuTasks > 0)
	{
		if (pHistoryCountItem->m_iTaskCountCpu < iWarningCpuTasks)
		{
			*pbWarning = true;
			return;
		}
	}
	if (iWarningGpuTasks > 0)
	{
		if (pHistoryCountItem->m_iTaskCountGpu < iWarningGpuTasks)
		{
			*pbWarning = true;
			return;
		}
	}
}

void CRemoteCombinedHistoryView::Sort(CListCtrl *plistCtrl)
{
		int		iListCount;
		UINT	iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;

		iListCount = plistCtrl->GetItemCount();

		m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);

		if (iListCount > 1)
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
	
			int iTopItem = plistCtrl->GetTopIndex();

			plistCtrl->SortItems(HistorySorting,iSorting);
			plistCtrl->EnsureVisible(iTopItem, FALSE);			// otherwise, there is a scroll to the botton om the right mouse button.
		}
		DisplayMakeLineColors(theApp.m_pDlgSettingsView->m_bAlternatingStriped, theApp.m_pDlgSettingsView->m_bUseSkin, theApp.m_pDlgSettingsView->m_iUseSkin);
}


LRESULT CRemoteCombinedHistoryView::OnRpcHistoryReady(WPARAM wParam, LPARAM lParam)
{
	LV_ITEM		lvi;
	bool		bNumberChanged = false;
	CRpcThreadReturn	*pThreadReturn;
	int			iNrItems, iCountList, iItemsAdd, iItemsAddTemp, iAddPos, iCount;

	std::deque<CHistoryItem*>	*plHistory;
//	CXList		<CHistoryItem*,CHistoryItem*> *plHistory;
	CHistoryItem *pHistoryItem;
//	XOSITION		pos;
	int			iSize, iPos;

	pThreadReturn = (CRpcThreadReturn *) lParam;

	if (m_bDocumentIsClosing)
	{
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusClosing]);
		delete pThreadReturn;
		return 0;			// everything is closing down.
	}

	if (pThreadReturn->m_iStatus == RPC_STATUS_CONNECTED) m_iHistoryStateReadyCount += pThreadReturn->m_iNumber;

	m_lComputerHistoryItem.at(m_iReadyRpcCount)->m_pcReadyRpcComputerId = pThreadReturn->m_pcComputerId;
	m_lComputerHistoryItem.at(m_iReadyRpcCount)->m_iReadyRpcThreadId = pThreadReturn->m_iThreadId;
	m_lComputerHistoryItem.at(m_iReadyRpcCount)->m_iReadyRpcConnectionStatus = pThreadReturn->m_iStatus;
	m_lComputerHistoryItem.at(m_iReadyRpcCount)->m_iReadyRpcFoundCount = (int) wParam;
	m_iAddedRpcCount += (int) wParam;
	m_iReadyRpcCount++;

	if (m_iTimeLeftLowest == -1)
	{
		m_iTimeLeftLowest = pThreadReturn->m_iTimeLeftLowest;
	}
	else
	{
		if (pThreadReturn->m_iTimeLeftLowest != -1)
		{
			if (pThreadReturn->m_iTimeLeftLowest < m_iTimeLeftLowest)
			{
				m_iTimeLeftLowest = pThreadReturn->m_iTimeLeftLowest;
			}
		}
	}


	theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_CONNECT, (WPARAM) pThreadReturn->m_pcComputerId, pThreadReturn->m_iStatus);

	delete pThreadReturn;
	pThreadReturn = NULL;

	if (m_iSendRpcCount != m_iReadyRpcCount) return 0;

	time_t tSystemTime;
	_time64( &tSystemTime);
	int		iUpdateTime;

	iUpdateTime = m_iTimeLeftLowest / 2;

	if (m_iTimeLeftLowestPrevious < m_iTimeLeftLowest)
	{
		iUpdateTime = m_iTimeLeftLowestPrevious / 2;
	}

	if (iUpdateTime < UPDATE_HISTORY_TRIGGER_MIN)
	{
		iUpdateTime = UPDATE_HISTORY_TRIGGER_MIN;
	}

	int iHistoryMaxTime = theApp.m_pDlgSettingsHistory->m_iHistoryMaxUpdateTime;

	if (iUpdateTime > iHistoryMaxTime)
	{
		iUpdateTime = iHistoryMaxTime;
	}
	m_iTimeLeftLowestPrevious = m_iTimeLeftLowest;
	if (iUpdateTime < theApp.m_pDlgSettingsHistory->m_iHistoryMinUpdateTime) iUpdateTime = theApp.m_pDlgSettingsHistory->m_iHistoryMinUpdateTime;
	m_tNextHistoryUpdateTime = tSystemTime += iUpdateTime;

	RulesCheckAll();

	CDlgSettingsView *pSettingsView;
	pSettingsView = theApp.m_pDlgSettingsView;
	m_bUseHorizontalGrid = pSettingsView->m_bUseHorizontalGrid;
	m_bUseVerticalGrid = pSettingsView ->m_bUseVerticalGrid;

	m_pDoc->m_pProjectView->SendMessage(UWM_MSG_HISTORY_TASKS_COUNT,0,(LPARAM) &m_lComputer);			// send project wu count info to the project view
	m_pDoc->m_pTaskView->SendMessage(UWM_MSG_HISTORY_TASKS,0,(LPARAM) &m_lComputer);

	if (m_iAddedRpcCount == 0)
	{
		if (!m_bAddedFromFile)
		{
			if (m_iSortingChanged-- <= 0)
			{
				m_bThreadBusy = false;	
				return 0;
			}
		}
	}

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	bool	bTooLarge;
	int		iTooLarge;
	bTooLarge = false;
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		m_lComputerHistoryItem.at(iCount)->m_bReadyRpcSelected = false;
		if (m_lComputer.at(iCount)->m_lHistory.size() > 10000)
		{
			bTooLarge = true;
			iTooLarge = (int) m_lComputer.at(iCount)->m_lHistory.size();
		}
	}

	bool	bWriteAlways = false;
	if (tSystemTime > m_tNextHistoryWrite)
	{
		m_tNextHistoryWrite = tSystemTime + HISTORY_BACKUP_TIME;
		bWriteAlways = true;
	}

	iCountList = 0;
	for (iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		for (int iCountMatch = 0; iCountMatch < m_iReadyRpcCount; iCountMatch++)
		{
			if (m_lComputerHistoryItem.at(iCountMatch)->m_iReadyRpcThreadId == m_lComputerHistoryItem.at(iCount)->m_iSendRpcThreadId)
			{
				if (m_lComputerHistoryItem.at(iCountMatch)->m_iReadyRpcFoundCount > 0)
				{
					// write to long history first
					if (m_lComputer.at(iCount)->m_bHistoryLongNeedsWrite || bWriteAlways)
					{
						m_pHistory->Write(&m_lComputer.at(iCount)->m_lHistoryLong, m_lComputerHistoryItem.at(iCountMatch)->m_pcReadyRpcComputerId, true);
						m_lComputer.at(iCount)->m_bHistoryLongNeedsWrite = false;
					}

					m_pHistory->Write(&m_lComputer.at(iCount)->m_lHistory, m_lComputerHistoryItem.at(iCountMatch)->m_pcReadyRpcComputerId);		// write to disk
				}
				int iResult = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, (WPARAM) m_lComputerHistoryItem.at(iCountMatch)->m_pcReadyRpcComputerId, 0);
				if (iResult == TRUE)
				{
					iCountList += (int) m_lComputer.at(iCount)->m_lHistory.size();
					m_lComputerHistoryItem.at(iCount)->m_bReadyRpcSelected = true;

					// remove state running
					plHistory = &m_lComputer.at(iCount)->m_lHistory;
					iSize = (int) plHistory->size();
					for (iPos = 0; iPos < iSize; iPos++)
					{
						pHistoryItem = plHistory->at(iPos);
						if((pHistoryItem->m_iState&0xffffe) == STATE_RUNNING)		// don't show running states
						{
							iCountList--;
						}
						if((pHistoryItem->m_iState&0xffffe) == STATE_WAITING_TO_RUN)// don't show
						{
							iCountList--;
						}

						
						// OK only set the counter for to be removed projects.
						if (!m_bAllProjectsSelected)
						{
							if (StrStrI(m_sProjectsSelected.c_str(), pHistoryItem->m_pcProject) == NULL)
							{
								iCountList--;
							}
						}
					}
				}
			}		
		}
	}

	bool bVisible = theApp.m_pMainWnd->IsWindowVisible() == TRUE;
	if (g_bWebServerActive) bVisible = true;
	if (g_bCloudServerRequestData) bVisible = true;
	int iViewActive = m_pDoc->m_iViewActive;
	if (iViewActive != HISTORY_VIEW_ACTIVE) bVisible = false;

	if (bVisible)
	{
		iItemsAdd = iNrItems - iCountList;

		if (iItemsAdd > 0)				// delete list items
		{
			bNumberChanged = true;
			iItemsAddTemp = iItemsAdd;
			while (iItemsAddTemp > 0)
			{
				listCtrl.DeleteItem(iNrItems-1);
				iNrItems--;
				iItemsAddTemp--;
			}	
		}
			else
			{
			if (iItemsAdd < 0)			// add list items
			{
				bNumberChanged = true;
				while (iItemsAdd < 0)
				{
					lvi.mask = LVIF_TEXT | LVIF_PARAM;
					iAddPos =  iNrItems-1;
					if (iAddPos < 0) iAddPos = 0;
					lvi.iItem = iAddPos;								// add at the end
					lvi.iSubItem = 0;
					lvi.pszText = "Initializing";
					lvi.lParam = (LPARAM) &m_dataDummy;
					listCtrl.InsertItem(&lvi);
					iNrItems++;
					iItemsAdd++;
				}
			}
		}

		int iRow = 0;
		for (int iCount = 0; iCount < m_iReadyRpcCount; iCount++)
		{
			if (m_lComputerHistoryItem.at(iCount)->m_bReadyRpcSelected)
			{
				DisplayRpc(&listCtrl, &m_lComputer.at(iCount)->m_lHistory, &iRow);
			}
		}
	
		if (iRow == 0) listCtrl.DeleteAllItems();

		Sort(&listCtrl);

		if (bTooLarge)
		{
			if (!m_bTooLargeShown)
			{
				CString sWarning;
				sWarning.Format(" %d", iTooLarge);
				sWarning = gszTranslation[PosViewHistoryMessageTooLarge] + sWarning;
				AfxMessageBox(sWarning,MB_ICONEXCLAMATION);
				m_bTooLargeShown = true;
			}
		}

		if (bNumberChanged)	Invalidate();

		if (g_bWebServerActive)
		{
			if (!m_bDocumentIsClosing && (g_pcWebServerHtml == NULL))
			{
				CCreateHtmlWebServer createHtml;
				char *pcHtml;
				if (createHtml.Create(PosBarViewSelectHistory, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_HISTORY_COLUMNS, NULL, PosGroupViewTasksHistory, &pcHtml))
				{
					g_pcWebServerHtml = pcHtml;
					g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_HISTORY);
					g_tWebServerHtml = GetTickCount()/100;
				}
			}
		}
		if (g_bCloudServerRequestData)
		{
			if (!m_bDocumentIsClosing && (g_pcCloudServerHtml == NULL))
			{
				if (g_iCloudServerHtmlTabProcessed[TAB_HISTORY] == TRUE)
				{
					g_iCloudServerHtmlTabProcessed[TAB_HISTORY] = FALSE;
					CCreateHtmlWebServer createHtml;
					char *pcHtml;
					if (createHtml.Create(PosBarViewSelectHistory, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_HISTORY_COLUMNS, NULL, PosGroupViewTasksHistory, &pcHtml))
					{
						g_pcCloudServerHtml = pcHtml;
						g_pThreadCloudServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,TAB_HISTORY);
						g_tCloudServerHtml = GetTickCount()/100;
					}
				}
			}
		}
	}

	theApp.m_pMainFrame->SendMessage(UWM_MSG_WUS_THAT_ARE_READY,(WPARAM) m_iHistoryStateReadyCount, 0);	// total WU's that are ready

	m_bAddedFromFile = false;

	_time64( &tSystemTime);
	m_tNextHistoryUpdateTime = tSystemTime += iUpdateTime;	// reset the time to include the refresh time.
	m_bThreadBusy = false;	
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnReadyRpcClient(WPARAM parm1, LPARAM parm2)
{
	CRpcInfoClient *pInfoClient;
	CString sFrom, sText;

	if (m_bDocumentIsClosing)
	{
		return 0;			// everything is closing down.
	}

	pInfoClient = (CRpcInfoClient *) parm1;
	if (pInfoClient->m_iStatus == 0)		// valid
	{
		pInfoClient->m_bReady = true;
		pInfoClient->m_bReadyCopy = true;
		pInfoClient->m_fCpuThrottleCopy = pInfoClient->m_fCpuThrottle;
		pInfoClient->m_fGpuThrottleCopy = pInfoClient->m_fGpuThrottle;
		pInfoClient->m_fTopCpuTemperatureCopy = pInfoClient->m_fTopCpuTemperature;
		pInfoClient->m_fTopGpuTemperatureCopy = pInfoClient->m_fTopGpuTemperature;
		pInfoClient->m_iLostCount = 0;

		for (int iCount = 0; iCount < MAX_GPU; iCount++)
		{
			pInfoClient->m_fGpuTemperatureCopy[iCount] = pInfoClient->m_fGpuTemperature[iCount];
		}
	}
	else
	{
		if (pInfoClient->m_iLostCount > 10)
		{
			pInfoClient->m_bReadyCopy = false;				// after 3 times lost invalidate
		}
		else
		{
			pInfoClient->m_iLostCount += 1;
		}
	}

	return 0;
}

void CRemoteCombinedHistoryView::OnContextMenu(CWnd* pWnd, CPoint point) 
{ 
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	CRect	rect;
	int		iRetVal;
	CPoint	clientPoint, startPoint;
	bool	bHeader;

	bHeader = false;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	clientPoint = point;
	ScreenToClient(&clientPoint);
	HDHITTESTINFO hdHitIfo;
	memset(&hdHitIfo, 0, sizeof(HDHITTESTINFO));
	for(int i = 0; i < pHeader->GetItemCount(); i++)
	{
		hdHitIfo.pt = clientPoint;
		// The hit test depends on whether the header item is visible.
		iRetVal = pHeader->HitTest(&hdHitIfo);
		if (hdHitIfo.iItem >= 0)
		{
			bHeader = true;
		}
	}

	startPoint = point;
	startPoint.y += 10;

	//our menu
	CMenu contextMenu; 
	//the actual popup menu which is the first
	//sub menu item of IDR_CONTEXT
	CMenu* tracker; 

	if (bHeader)
	{
		contextMenu.LoadMenu(IDR_MENU_CONTEXT_HEADER);
		tracker = contextMenu.GetSubMenu(0); 
		// Translate
		tracker->ModifyMenu(ID_HEADER_SETHEADERITEMS,MF_STRING,ID_HEADER_SETHEADERITEMS,gszTranslation[PosPopUpHeaderEnable]);
		tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y , AfxGetMainWnd()); 
		return;
	}

	CListCtrl& listCtrl = GetListCtrl();

	m_contextMenu.SetColumnOrder(NUM_REMOTE_HISTORY_COLUMNS, &m_iColumnOrder[0]);
	m_contextMenu.ContextMenuCombined(CONTEXT_MENU_HISTORY, m_pDoc, &listCtrl, m_pSelection, point, this, IDR_MENU_HISTORY);
} 


LRESULT CRemoteCombinedHistoryView::OnReadyProject(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_contextMenu.Ready((int) parm1);
	return 0;
}

void CRemoteCombinedHistoryView::OnWww1() {m_contextMenu.WWW(0);}
void CRemoteCombinedHistoryView::OnWww2() {m_contextMenu.WWW(1);}
void CRemoteCombinedHistoryView::OnWww3() {m_contextMenu.WWW(2);}
void CRemoteCombinedHistoryView::OnWww4() {m_contextMenu.WWW(3);}
void CRemoteCombinedHistoryView::OnWww5() {m_contextMenu.WWW(4);}
void CRemoteCombinedHistoryView::OnWww6() {m_contextMenu.WWW(5);}
void CRemoteCombinedHistoryView::OnWww7() {m_contextMenu.WWW(6);}
void CRemoteCombinedHistoryView::OnWww8() {m_contextMenu.WWW(7);}
void CRemoteCombinedHistoryView::OnWww9() {m_contextMenu.WWW(8);}
void CRemoteCombinedHistoryView::OnWww10() {m_contextMenu.WWW(9);}
void CRemoteCombinedHistoryView::OnWww11() {m_contextMenu.WWW(10);}
void CRemoteCombinedHistoryView::OnWww12() {m_contextMenu.WWW(11);}
void CRemoteCombinedHistoryView::OnWww13() {m_contextMenu.WWW(12);}
void CRemoteCombinedHistoryView::OnWww14() {m_contextMenu.WWW(13);}
void CRemoteCombinedHistoryView::OnWww15() {m_contextMenu.WWW(14);}
void CRemoteCombinedHistoryView::OnWww16() {m_contextMenu.WWW(15);}
void CRemoteCombinedHistoryView::OnWww17() {m_contextMenu.WWW(16);}
void CRemoteCombinedHistoryView::OnWww18() {m_contextMenu.WWW(17);}
void CRemoteCombinedHistoryView::OnWww19() {m_contextMenu.WWW(18);}
void CRemoteCombinedHistoryView::OnWww20() {m_contextMenu.WWW(19);}

// a update request from the gadget.
LRESULT CRemoteCombinedHistoryView::OnFloaterData(WPARAM wParam, LPARAM lParam)
{
	int				iNrComputer, iNrComputerTotal, iComputers, iConnectedComputers, iMode, iCommand;
	CHistoryItem	*pHistoryItem;
	CHistoryCountItem *pHistoryCountItem;
	CString			sFormat, sNr;
	bool			bComputerFound;
	int				iTasksCpu, iTasksGpu, iTimeLeftCpu, iTimeLeftGpu, iCoresCpu, iCoresGpu;
	bool			bWarning;
	CUseCpuGpu		cpugpu;
	char			cBuffer[255];
	UINT			iSize, iPos;
	CDlgSettingsWarning *pDlgWarning;
	CNumberFormat	numberFormat;

	iMode = (int) wParam;
	iCommand= (int) lParam;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_bThreadBusy)
	{
		if (iCommand == FLOATER_FULL_INFO)
		{
			theApp.m_pDlgFloating->SendMessage(UWM_MSG_GET_FLOATER_DATA_READY,(WPARAM) &m_lFloater, -1);
		}
		return -1;		// forget it....
	}

	pDlgWarning = theApp.m_pDlgSettingsWarning;

	ClearFloaterList();

	if (!theApp.m_pDlgSettingsHistory->m_bHistoryEnabled)
	{
		if (iCommand == FLOATER_TOOLTIP_INFO)	theApp.m_pMainFrame->SendMessage(UWM_MSG_GET_FLOATER_DATA_READY,(WPARAM) NULL,(LPARAM) &m_sTip);
		if (iCommand == FLOATER_FULL_INFO)	theApp.m_pDlgFloating->SendMessage(UWM_MSG_GET_FLOATER_DATA_READY,(WPARAM) NULL, 0);
		return 0;
	}

// get nr of connected computers

	iConnectedComputers = 0;
	for (int iPos = 0; iPos < (int) m_lComputerHistoryItem.size(); iPos++)
	{
		if (m_lComputerHistoryItem.at(iPos)->m_iReadyRpcConnectionStatus == RPC_STATUS_CONNECTED)
		{
			iConnectedComputers++;
		}
	}

	m_sTip = gszTranslation[PosGroupTaskbarMessageComputers];
	sFormat.Format(": %d\r\n", iConnectedComputers);
	m_sTip += sFormat;

	if (iConnectedComputers < 10)
	{
		if (iConnectedComputers > 4) 
		{
			m_sTip += gszTranslation[PosGroupTaskbarMessageRunning];
			m_sTip += ": ";
		}
	}

	iNrComputerTotal = 0;
	iComputers = 0;

	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		iSize = (int) m_lComputer.at(iCount)->m_lHistory.size();
		bComputerFound = false;
		iNrComputer = 0;
		for (iPos = 0; iPos < iSize; iPos++)
		{
			pHistoryItem = m_lComputer.at(iCount)->m_lHistory.at(iPos);
			if((pHistoryItem->m_iState&0xffffe) == STATE_RUNNING)
			{
				if (iMode == FLOATER_MODE_TASKS)
				{
					CFloaterItem *pFloaterItem;
					pFloaterItem = new CFloaterItem;
					pFloaterItem->m_sComputer = pHistoryItem->m_pcComputer;
					pFloaterItem->m_sProjectName = pHistoryItem->m_pcProject;
					pFloaterItem->m_sUserFriendly= pHistoryItem->m_pcName;
					pFloaterItem->m_iVersion = pHistoryItem->m_iVersion;
					pFloaterItem->m_sPlannClass = pHistoryItem->m_pcPlanClass;
					pFloaterItem->m_iState = pHistoryItem->m_iState;
					pFloaterItem->m_dPerc = pHistoryItem->m_dFraction_done;
					pFloaterItem->m_dRemainingTime = pHistoryItem->m_iTimeLeft;
					pFloaterItem->m_dReportDeadline= pHistoryItem->m_iDeadline;
					pFloaterItem->m_sUse = pHistoryItem->m_pcUse;

					m_lFloater.push_front(pFloaterItem);// .push_back(pFloaterItem);
				}
				iNrComputer++;
				iNrComputerTotal++;
				bComputerFound = true;
			}
		}

		if (bComputerFound)
		{
			iComputers++;

			if (iConnectedComputers < 10)
			{
				if (iConnectedComputers > 4) 
				{
					sFormat.Format(" %d", iNrComputer);
					m_sTip+= sFormat;
				}
				else
				{
					m_sTip += pHistoryItem->m_pcComputer;
					m_sTip += ", ";
					m_sTip += gszTranslation[PosGroupTaskbarMessageRunning];
					sFormat.Format(": %d\r\n", iNrComputer);
					m_sTip += sFormat;
				}
			}
		}
	}
	if (iMode == FLOATER_MODE_COMPUTERS)
	{
		CString sComputer = "";

		iComputers = 0;
		for (int iCount = 0; iCount < (int) m_lComputer.size(); iCount++)
		{
			bComputerFound = false;
			iNrComputer = 0;

			iTasksCpu = 0;
			iTasksGpu = 0;
			iTimeLeftCpu = 0;
			iTimeLeftGpu = 0;
			iCoresCpu = 0;
			iCoresGpu = 0;
			bWarning = false;

			for (iPos = 0; iPos < m_lComputer.at(iCount)->m_lHistoryCount.size(); iPos++)
			{
				pHistoryCountItem = m_lComputer.at(iCount)->m_lHistoryCount.at(iPos);
				sComputer = pHistoryCountItem->m_pcComputer;

				iTasksCpu += pHistoryCountItem->m_iTaskCountCpu;
				iTasksGpu += pHistoryCountItem->m_iTaskCountGpu;
				iTimeLeftCpu += (int) pHistoryCountItem->m_iTimeLeftCpu;
				iTimeLeftGpu += (int) pHistoryCountItem->m_iTimeLeftGpu;
				iCoresCpu = pHistoryCountItem->m_iCoresCpu;
				iCoresGpu = pHistoryCountItem->m_iCoresGpu;
				bComputerFound = true;
				CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer1, &pDlgWarning->m_sProject1, pDlgWarning->m_iWarningCpuTasks1, pDlgWarning->m_iWarningGpuTasks1, &bWarning);
				CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer2, &pDlgWarning->m_sProject2, pDlgWarning->m_iWarningCpuTasks2, pDlgWarning->m_iWarningGpuTasks2, &bWarning);
				CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer3, &pDlgWarning->m_sProject3, pDlgWarning->m_iWarningCpuTasks3, pDlgWarning->m_iWarningGpuTasks3, &bWarning);
				CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer4, &pDlgWarning->m_sProject4, pDlgWarning->m_iWarningCpuTasks4, pDlgWarning->m_iWarningGpuTasks4, &bWarning);
			}

			if (bComputerFound)
			{
				CFloaterItem *pFloaterItem;
				pFloaterItem = new CFloaterItem;
				pFloaterItem->m_sComputer = sComputer;
				
				if (iCoresCpu > 0) iTimeLeftCpu /= iCoresCpu;
				TimeString((int) iTimeLeftCpu,&cBuffer[0], 60, "", "");
				sNr = numberFormat.FormatNumber(iTasksCpu,0);
				sFormat.Format("%s / %s (%d)", sNr, cBuffer, iCoresCpu);
				pFloaterItem->m_sCpu = sFormat;

				if (iCoresGpu >0) iTimeLeftGpu /= iCoresGpu;
				TimeString((int) iTimeLeftGpu,&cBuffer[0], 60, "", "");
				sNr = numberFormat.FormatNumber(iTasksGpu,0);
				sFormat.Format("%s / %s (%d)", sNr, cBuffer, iCoresGpu);
				pFloaterItem->m_sGpu = sFormat;
				pFloaterItem->m_bWarning = bWarning;
				
				m_lFloater.push_front(pFloaterItem);//.push_back(pFloaterItem);
				iComputers++;
			}
		}
	}

	if (iConnectedComputers > 4)
	{
		if (iConnectedComputers > 1)
		{
			sFormat.Format(" =%d",iNrComputerTotal);
			m_sTip+= sFormat;
		}
	}

	theApp.m_pMainFrame->SendMessage(UWM_MSG_GET_FLOATER_DATA_READY,(WPARAM) &m_lFloater,(LPARAM) &m_sTip);	// as this info is available send it always.
	if (iCommand == FLOATER_FULL_INFO)	theApp.m_pDlgFloating->SendMessage(UWM_MSG_GET_FLOATER_DATA_READY,(WPARAM) &m_lFloater, 0);

//	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_GET_FLOATER_DATA, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnCloudData(WPARAM wParam, LPARAM lParam)
{
	std::deque<std::string> listItems, list;
	CJson		json;
	int			iPos, iThread, iComputers, iNrComputer, iTasksCpu, iTasksGpu, iTimeLeftCpu, iTimeLeftGpu, iCoresCpu, iCoresGpu;
	CString		*psData, sFormat, sNr;
	std::string sString, sComputer;
	char		cBuffer[255];
	bool		bWarning;
	CHistoryCountItem *pHistoryCountItem;
	CDlgSettingsWarning *pDlgWarning;
	CNumberFormat	numberFormat;

	iThread = (int) wParam;
	psData = (CString *) lParam;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	if (m_bThreadBusy)
	{
		return FALSE;
	}

	pDlgWarning = theApp.m_pDlgSettingsWarning;

	listItems.push_back("com");
	listItems.push_back("con");
	listItems.push_back("cpu");
	listItems.push_back("gpu");
	listItems.push_back("wrn");

	iComputers = 0;
	for (int iCount = 0; iCount < (int) m_lComputer.size(); iCount++)
	{
//		bComputerFound = false;
		iNrComputer = 0;
		iTasksCpu = 0;
		iTasksGpu = 0;
		iTimeLeftCpu = 0;
		iTimeLeftGpu = 0;
		iCoresCpu = 0;
		iCoresGpu = 0;
		bWarning = false;
		sComputer = m_lComputer.at(iCount)->m_sComputerId;
		for (iPos = 0; iPos < (int) m_lComputer.at(iCount)->m_lHistoryCount.size(); iPos++)
		{
			pHistoryCountItem = m_lComputer.at(iCount)->m_lHistoryCount.at(iPos);
//			sComputer = pHistoryCountItem->m_pcComputer;
			iTasksCpu += pHistoryCountItem->m_iTaskCountCpu;
			iTasksGpu += pHistoryCountItem->m_iTaskCountGpu;
			iTimeLeftCpu += (int) pHistoryCountItem->m_iTimeLeftCpu;
			iTimeLeftGpu += (int) pHistoryCountItem->m_iTimeLeftGpu;
			iCoresCpu = pHistoryCountItem->m_iCoresCpu;
			iCoresGpu = pHistoryCountItem->m_iCoresGpu;
//			bComputerFound = true;
			CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer1, &pDlgWarning->m_sProject1, pDlgWarning->m_iWarningCpuTasks1, pDlgWarning->m_iWarningGpuTasks1, &bWarning);
			CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer2, &pDlgWarning->m_sProject2, pDlgWarning->m_iWarningCpuTasks2, pDlgWarning->m_iWarningGpuTasks2, &bWarning);
			CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer3, &pDlgWarning->m_sProject3, pDlgWarning->m_iWarningCpuTasks3, pDlgWarning->m_iWarningGpuTasks3, &bWarning);
			CheckWarning(pHistoryCountItem, &pDlgWarning->m_sComputer4, &pDlgWarning->m_sProject4, pDlgWarning->m_iWarningCpuTasks4, pDlgWarning->m_iWarningGpuTasks4, &bWarning);
		}

		if (sComputer.length() > 0)
		{
			list.push_back(sComputer);
			// check for connection status
			sString = "-1";
			for (int iPos = 0; iPos < (int) m_lComputerHistoryItem.size(); iPos++)
			{
				if (m_lComputerHistoryItem.at(iPos)->m_pcReadyRpcComputerId != NULL)
				{
					if (sComputer == m_lComputerHistoryItem.at(iPos)->m_pcReadyRpcComputerId)
					{
						sFormat.Format("%d",m_lComputerHistoryItem.at(iPos)->m_iReadyRpcConnectionStatus);
						sString = sFormat;
					}
				}
			}
			list.push_back(sString);

			if (iCoresCpu > 0) iTimeLeftCpu /= iCoresCpu;
			TimeString((int) iTimeLeftCpu,&cBuffer[0], 60, "", "");
			sNr = numberFormat.FormatNumber(iTasksCpu,0);
			sFormat.Format("%s / %s (%d)", sNr, cBuffer, iCoresCpu);
			sString = sFormat;
			list.push_back(sString);

			if (iCoresGpu >0) iTimeLeftGpu /= iCoresGpu;
			TimeString((int) iTimeLeftGpu,&cBuffer[0], 60, "", "");
			sNr = numberFormat.FormatNumber(iTasksGpu,0);
			sFormat.Format("%s / %s (%d)", sNr, cBuffer, iCoresGpu);
			sString = sFormat;
			list.push_back(sString);

			if (bWarning) sString = "y";
			else sString = "";
			list.push_back(sString);
			iComputers++;
		}
	}

/*

	listItems.push_back("com");
	listItems.push_back("con");

	for (int iPos = 0; iPos < (int) m_lComputerHistoryItem.size(); iPos++)
	{
		if (m_lComputerHistoryItem.at(iPos)->m_pcReadyRpcComputerId != NULL)
		{
			sString = m_lComputerHistoryItem.at(iPos)->m_pcReadyRpcComputerId;
			list.push_back(sString);
			sFormat.Format("%d",m_lComputerHistoryItem.at(iPos)->m_iReadyRpcConnectionStatus);
			sString = sFormat;
			list.push_back(sString);
		}
	}
*/
	json.Array("computers", &listItems, &list);

	*psData = json.Get().c_str();
	return TRUE;
}

LRESULT CRemoteCombinedHistoryView::OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_CPU, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_SET_SNOOZE_GPU, wParam, lParam);
	return 0;
}


LRESULT CRemoteCombinedHistoryView::OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH, wParam, lParam);
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return -1;			// everything is closing down.
	return m_pDoc->m_pProjectView->PostMessage(UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH, wParam, lParam);
}

LRESULT CRemoteCombinedHistoryView::OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam)
{
	int		iColumnSet, iCheck, iLen, iLenMax;
	CString	sTxt;

	iColumnSet = (int) wParam;
	iCheck = (int) lParam;

	CListCtrl& listCtrl = GetListCtrl();

	if (iCheck)
	{
		int iItems = listCtrl.GetItemCount();
		iLenMax = 0;
		for (int iCount = 0; iCount < iItems; iCount++)
		{
			sTxt = listCtrl.GetItemText(iCount, m_iColumnOrder[iColumnSet]);
			RemoveSpecialText(&sTxt);
			iLen = listCtrl.GetStringWidth(sTxt);
			if (iLen > iLenMax) iLenMax = iLen;
		}
		if (iLenMax == 0) iLenMax = 90;

		if (m_iColumnOrder[iColumnSet] == 0) iLenMax += 14;
		else iLenMax +=12;	// margins

		listCtrl.SetColumnWidth(m_iColumnOrder[iColumnSet],iLenMax);
	}
	else
	{
		listCtrl.SetColumnWidth(m_iColumnOrder[iColumnSet],0);
	}

	Invalidate(TRUE);
	return 0;
}

//LRESULT CRemoteCombinedHistoryView::OnBoincTasksVersion(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_UPDATE,parm1,parm2);
//	return 0;
//}

//LRESULT CRemoteCombinedComputerView::OnBoincTasksRule(WPARAM parm1, LPARAM parm2)
//{
//	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
//	m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_BT_RULE,parm1,parm2);
//	return 0;
//}

LRESULT CRemoteCombinedHistoryView::OnSearchComputerReady(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.
	m_pDoc->m_pComputerView->SendMessage(UWM_DLG_SEARCH_COMPUTER_READY,parm1,parm2);
	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnFontChangeAll(WPARAM wParam,LPARAM lParam)
{
	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->m_pComputerView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pProjectView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTaskView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pTransferView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pMessageView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);
	m_pDoc->m_pHistoryView->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam,lParam);

	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnFontHasChanged(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLf;
	pLf = (LOGFONT *) wParam;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	m_pDoc->m_pDlgHistory->PostMessage(UWM_MSG_FONT_HAS_CHANGED, wParam, lParam);

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLf);
	SetFont(m_pFont,TRUE);

	return 0;
}

LRESULT CRemoteCombinedHistoryView::OnRequestLongHistory(WPARAM wParam,LPARAM lParam)
{
	m_bRequestHistoryLong = true;
	m_hWndRequestHistoryLong = (HWND) wParam;
	m_plHistoryLong= (std::deque<CHistoryItem*> *) lParam;

	return 0;
}


void CRemoteCombinedHistoryView::OnSize(UINT nType, int cx, int cy )
{
	theApp.m_pMainFrame->m_dlgBarComputer.PostMessage(UWM_MSG_NEW_SIZE, cx, cy);
	CTemplateRemoteHistoryView::OnSize(nType, cx, cy );
}

void CRemoteCombinedHistoryView::OnHeaderSetItems()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	theApp.m_pDlgSettingsMain->SetActivePage(theApp.m_pDlgSettingsHistory);
	theApp.m_pDlgSettingsMain->ShowWindow(SW_SHOW);
}

void CRemoteCombinedHistoryView::OnCopyalltoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	ClipBoard(true);
}

void CRemoteCombinedHistoryView::OnCopyselectedtoclipboard()
{
	if (m_bDocumentIsClosing) 	return;			// everything is closing down.

	ClipBoard(false);
}

LRESULT CRemoteCombinedHistoryView::OnWebItemSelected(WPARAM wParam,LPARAM lParam)
{
	int	iRow, iOption;
	CHtmlExchange *pHtmlExchange;

	if (m_bDocumentIsClosing) 	return 0;			// everything is closing down.

	pHtmlExchange = (CHtmlExchange *) lParam;
	iRow = pHtmlExchange->m_iRow;
	iOption = pHtmlExchange->m_iOption;

	m_pSelection->SelectionSet(iRow, iOption);
	if (pHtmlExchange->m_bSendData)
	{
		if (!m_bThreadBusy)
		{
			CListCtrl& listCtrl = GetListCtrl();
			CCreateHtmlWebServer createHtml;
			createHtml.Create(PosBarViewSelectHistory, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_HISTORY_COLUMNS, NULL, PosGroupViewTasksHistory, &pHtmlExchange->m_pcHtml);
			if (pHtmlExchange->m_pcHtml == NULL)
			{
				// oeps
				int ii = 1;
			}
			pHtmlExchange->tTime = GetTickCount()/100;
			return TRUE;
		}
		else
		{
			int ii =1;
			// oeps
		}
	}
	else
	{
		int ii =1;
		// oeps
	}
	return FALSE;
}

LRESULT CRemoteCombinedHistoryView::OnWebSort(WPARAM parm1, LPARAM parm2)
{
	char	*pcSort;
	BOOL	bHtml;
	char	*pcHtml = NULL;

	pcSort = (char *) parm2;
	bHtml = (BOOL) parm1;

	m_pSortColumn->WebSorting(pcSort);
	if (bHtml == FALSE) return NULL;

	if (!m_bThreadBusy)
	{
		CListCtrl& listCtrl = GetListCtrl();
		Sort(&listCtrl);
		CCreateHtmlWebServer createHtml;
		createHtml.Create(PosBarViewSelectHistory, this, &listCtrl, &m_iColumnOrder[0], NUM_REMOTE_HISTORY_COLUMNS, NULL, PosGroupViewTasksHistory, &pcHtml);
		m_tNextHistoryUpdateTime = 0; // ensure direct timer message
	}
	else
	{
		// oeps
		int ii = 1;
	}
	return (LRESULT) pcHtml;
}

LRESULT CRemoteCombinedHistoryView::OnSetProjectList(WPARAM parm1, LPARAM parm2)
{
	m_sProjectsSelected = *(std::string*) parm2;
	if (m_sProjectsSelected.length() == 0)	m_bAllProjectsSelected = true;
	else m_bAllProjectsSelected = false;

	return 0;
}

void CRemoteCombinedHistoryView::OnComputersDetect()
{
	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_SELECTED,BAR_SELECTED_COMPUTERS,0);

	m_pDoc->m_pComputerView->PostMessage(WM_COMMAND, ID_COMPUTERS_DETECT,0);
}
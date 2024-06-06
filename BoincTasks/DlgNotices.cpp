// DlgNotices.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
#include "RemoteCombinedDoc.h"
#include "ThreadRpcClient.h"
#include "DlgNotices.h"
#include "DlgSettingsNotices.h"
#include "DlgSettingsGeneral.h"
#include "DlgRulesLogging.h"
#include "DlgLogging.h"
#include "afxdialogex.h"
#include "CreateHtmlNotices.h"
#include "Xml.h"
#include "AnsiToUtf8.h"
#include "MainFrm.h"
#include "rules.h"
#include "RemoteCombinedNoticesView.h"
#include "TemplateRemoteTaskView.h"
#include "RemoteCombinedTaskView.h"
#include "ThreadWebServer.h"
#include "PushCheck.h"
#include "Translation.h"

// CDlgNotices dialog

IMPLEMENT_DYNAMIC(CDlgNotices, CDialogEx)

CDlgNotices::CDlgNotices(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgNotices::IDD, pParent)
{
	time_t tSystemTime;
	_time64(&tSystemTime);

	m_pDoc			= NULL;
	m_bThreadBusy	= false;
	m_iThreadLocked = 0;
	m_iThreadRpcId = 0;

	m_tLastPushNotices = tSystemTime;
	m_tPushNoticesInterval = 1;
	m_bPushNotices = false;

	m_bDocumentIsClosing = false;

	m_bNavigate2Done = false;
	m_tLastUpdate = 0;
	m_tLastNoticeAlert = 0;
	m_sNoticesText = "";
	m_iNewNotices = 0;
	m_bDisabled = false;

	m_bCheckTThrottle = false;
	CRpcThreadInfoNotices *pRpcThreadInfoNotices;
	CRpcInfoClient *pRpcInfoClient;
	CRpcInfoNoticesItem *pRpcInfoNoticesItem;
	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		pRpcThreadInfoNotices = new CRpcThreadInfoNotices;
		pRpcThreadInfoNotices->m_pNotices = new NOTICES;
		pRpcThreadInfoNotices->m_iNrRead = 0;
		pRpcThreadInfoNotices->m_iRpcPosition = -1;
		m_lComputerRpcInfo.push_back(pRpcThreadInfoNotices);

		pRpcInfoClient = new CRpcInfoClient;
		pRpcInfoClient->m_bReady = false;
		pRpcInfoClient->m_bReadyCopy = false;
		pRpcInfoClient->m_iLostCount = 0;
		pRpcInfoClient->m_bConnectionLogged = false;
		pRpcInfoClient->m_sComputerId = "";
		m_lComputerRpcInfoClient.push_back(pRpcInfoClient);

		pRpcInfoNoticesItem = new CRpcInfoNoticesItem;
		m_lRpcInfoNotices.push_back(pRpcInfoNoticesItem);
	}
	m_noticeBoincTasksUpdate.m_iType = -1;
	m_noticeBoincTasksUpdate.m_bRead = true;
	m_pNoticeUpdate = new CNoticesInfoItem;
	m_pNoticeUpdate->m_pNotice = NULL;
	m_pNoticeUpdateTThrottle = new CNoticesInfoItem;
	m_pNoticeUpdateTThrottle->m_pNotice = NULL;
	m_pNoticeRules = new CNoticesInfoItem;
	m_pNoticeRules->m_pNotice = NULL;
	
	m_noticeBoincTasksRules.m_iActive = 0;
	m_noticeBoincTasksRules.m_iActiveNew = 0;

	m_bNoticesCollapseDeleteChanged = false;

	m_iIDNotices = NOTICES_ID_FIRST;

	ReadXml();
}

CDlgNotices::~CDlgNotices()
{
	int iPos, iPosComputer;
	CNoticesInfoItem	*pNoticesInfoItem;

	WriteXml();

	for (iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
	{
		pNoticesInfoItem = m_lNotices.at(iPos);
		if (pNoticesInfoItem->m_pNotice != NULL)
		{
			delete pNoticesInfoItem->m_pNotice;
		}
		for (iPosComputer = 0; iPosComputer < (int) m_lNotices.at(iPos)->m_lComputer.size(); iPosComputer++)
		{
			delete m_lNotices.at(iPos)->m_lComputer.at(iPosComputer);
		}

		delete pNoticesInfoItem;
	}
	m_lNotices.clear();

	CRpcThreadInfoNotices *pRpcThreadInfoNotices;
	for (int iCount = 0; iCount < (int) m_lComputerRpcInfo.size(); iCount++)
	{
		pRpcThreadInfoNotices = m_lComputerRpcInfo.at(iCount);
		if (pRpcThreadInfoNotices->m_pNotices != NULL)
		{
			delete pRpcThreadInfoNotices->m_pNotices;
		}
		delete pRpcThreadInfoNotices;
	}
	m_lComputerRpcInfo.clear();

	for (int iCount = 0; iCount < (int) m_lComputerRpcInfoClient.size(); iCount++)
	{
		delete m_lComputerRpcInfoClient.at(iCount);
	}
	m_lComputerRpcInfoClient.clear();

	for (int iCount = 0; iCount < (int) m_lRpcInfoNotices.size(); iCount++)
	{
		delete m_lRpcInfoNotices.at(iCount);
	}
	m_lRpcInfoNotices.clear();

	for (int iPosPush = 0; iPosPush < (int) m_lNoticesPush.size(); iPosPush++)
	{
		delete m_lNoticesPush.at(iPosPush);
	}
	m_lNoticesPush.clear();

	delete m_pNoticeUpdate->m_pNotice;
	delete m_pNoticeUpdate;
	delete m_pNoticeUpdateTThrottle->m_pNotice;
	delete m_pNoticeUpdateTThrottle;

	delete m_pNoticeRules->m_pNotice;
	delete m_pNoticeRules;
}

int CDlgNotices::ReadXml(void)
{
	CString		sTemp, sXmlName, sDirectory, sFile, sProject, sTitle;
	int			iSize;
	double		dTime;
	bool		bDeleteItem, bFoundNotice, bReadNotice;
	CNoticesInfoItem	*pNoticesInfoItem;

	pNoticesInfoItem = NULL;

	time_t tSystemTime, tTimeDelete;
	_time64(&tSystemTime);

	tSystemTime -= 60*60*24*180;					// keep for 180 days

	iSize = 0;
	sXmlName = "notices.xml";

	CXml xml(sXmlName,true,false);
	if (xml.ReadStart("notices"))
	{
		while (xml.ReadGroup("notice"))
		{
			// first read back the time, to check if an item is still in use
			xml.ReadItem("added_time",&sTemp);
			dTime = atof(sTemp);

			xml.ReadItem("project",&sProject);
			xml.ReadItem("title",&sTitle);

			bReadNotice = true;
			if (sProject.GetLength() == 0)
			{
				bReadNotice = false;
			}
			if (sTitle.GetLength() == 0)
			{
				bReadNotice = false;
			}
			
			tTimeDelete = tSystemTime - 60*60*24*180;						// keep for 180 days
			if (sProject == NOTICES_LOAD_ANYWAY)
			{
//				if (sTitle == NOTICES_NEW_BOINC_VERSION)
//				{
//					sProject = "";
//					sTitle = "";
//					tTimeDelete = tSystemTime - 60*60*24*7;					// keep for 7 days
//				}
//				else
				{
					sProject = "";
//					sTitle = "";
					tTimeDelete = tSystemTime - 60*60*24*2;					// keep for 7 days
				}
			}

			if (dTime > tTimeDelete)
			{
				bDeleteItem = true;
				pNoticesInfoItem = new CNoticesInfoItem;
				pNoticesInfoItem->m_dTime = dTime;
				pNoticesInfoItem->m_iID = m_iIDNotices;
				pNoticesInfoItem->m_pNotice = NULL;
				pNoticesInfoItem->sProject = sProject;
				pNoticesInfoItem->sTitle = sTitle;
				xml.ReadItem("checksum",&sTemp);
				pNoticesInfoItem->m_iChecksum = atoi(sTemp);
				xml.ReadItem("collapsed",&sTemp);
				pNoticesInfoItem->m_bCollapsed = (atoi(sTemp) == 1);
				xml.ReadItem("deleted",&sTemp);
				pNoticesInfoItem->m_bDeleted = (atoi(sTemp) == 1);

				pNoticesInfoItem->m_bAllowDelete = false;
	
				if (bReadNotice)
				{
					// now check to make sure the item isn't double
					bFoundNotice = false;
					for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
					{
						if (m_lNotices.at(iPos)->sProject ==  pNoticesInfoItem->sProject)
						{
							if (m_lNotices.at(iPos)->sTitle == pNoticesInfoItem->sTitle)
							{
								if (m_lNotices.at(iPos)->m_iChecksum == pNoticesInfoItem->m_iChecksum)
								{
									bFoundNotice = true;
									break;
								}
							}
						}
					}
					if (!bFoundNotice)
					{
						m_lNotices.push_back(pNoticesInfoItem);
						bDeleteItem = false;
						m_iIDNotices++;
						iSize++;
					}
				}
				if (bDeleteItem)
				{
					delete pNoticesInfoItem;
					pNoticesInfoItem = NULL;
				}
			}
		}
	}
	return iSize;
}

int CDlgNotices::ReadXmlPush(CStringA *psATxt)
{
	CString		sTemp, sXmlName, sDirectory, sFile, sProject, sTitle, sTag, sDescription;
	CStringA	sTempA;
	int			iSize;
	bool		bDeleteItem;
	NOTICE		*pNotice;
	int			iYear,iMonth, iDay, iHour, iMin, iSec, iDls;

	pNotice = NULL;

	time_t tSystemTime;//, tTimeDelete;
	_time64(&tSystemTime);


	iSize = 0;

	sTag = "notices";
	CXml xml(false,true);
	xml.ReadStart(psATxt, sTag);

	if (xml.ReadStart("notices"))
	{
		while (xml.ReadGroup("notice"))
		{
			xml.ReadItem("create_time_year",&sTemp);
			iYear = atoi(sTemp);
			if (iYear <= 0) iYear = 2013;
			xml.ReadItem("create_time_month",&sTemp);
			iMonth = atoi(sTemp);
			if (iMonth <=0) iMonth = 1;
			xml.ReadItem("create_time_day",&sTemp);
			iDay = atoi(sTemp);
			if (iDay <=0) iDay = 1;

			iHour = 12;
			iMin = 0;
			iSec = 0;
			iDls = -1;

			CTime time(iYear,iMonth,iDay, iHour, iMin, iSec, iDls);
//			CTime time;
//			time = 0;

//			dTime = atof(sTemp);

			xml.ReadItem("project",&sProject);
			xml.ReadItem("title",&sTitle);
			xml.ReadItem("description",&sDescription);

			pNotice = new NOTICE;

			pNotice->arrival_time = (double) tSystemTime;
			pNotice->create_time = (double) time.GetTime();
			strcpy_s(pNotice->category,64, "BT");

			strcpy_s(pNotice->project_name,64,sProject);
			strcpy_s(pNotice->title, 256, sTitle);
			pNotice->description = sDescription;

			m_lNoticesPush.push_back(pNotice);
			bDeleteItem = false;
			iSize++;
		}
	}
	return iSize;
}

void CDlgNotices::WriteXml(void)
{
	CStringA			sATxt, sAValue;
	CString				sXmlName, sDirectory, sTitle;
	CNoticesInfoItem	*pNoticesInfoItem;

	sXmlName = "notices.xml";

	CXml xml(sXmlName);
	if (xml.WriteStart("notices"))
	{
		for(int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
		{
			xml.WriteBeginGroup("notice");

			pNoticesInfoItem = m_lNotices.at(iPos);

			sATxt.Format("%f",pNoticesInfoItem->m_dTime);
			xml.WriteItem("added_time",sATxt);

			sTitle = pNoticesInfoItem->sTitle;
			bool bLoadAnyway = false;
			if (pNoticesInfoItem->sProject.GetLength() == 0)
			{
				if ((pNoticesInfoItem->sTitle == gszTranslation[PosNoticesCatBoincTasks]) && !bLoadAnyway)
				{
					xml.WriteItem("project",NOTICES_LOAD_ANYWAY);
					bLoadAnyway = true;
				}

				if ((pNoticesInfoItem->sTitle == gszTranslation[PosNoticesCatTThrottle]) && !bLoadAnyway)
				{
					xml.WriteItem("project",NOTICES_LOAD_ANYWAY);
					bLoadAnyway = true;
				}

				if (!bLoadAnyway)
				{
					if (pNoticesInfoItem->m_pNotice != NULL)
					{
//						if (strstr(pNoticesInfoItem->m_pNotice->description.c_str(), NOTICES_DETECT_NEW_BOINC_VERSION) != NULL)
//						{
//							xml.WriteItem("project",NOTICES_LOAD_ANYWAY);
//							sTitle = NOTICES_NEW_BOINC_VERSION;
//							bLoadAnyway = true;
//						}
//						else
						{
							if (strstr(pNoticesInfoItem->m_pNotice->category, "client") != NULL)		// Alert Notice from BOINC only once.
							{
								xml.WriteItem("project",NOTICES_LOAD_ANYWAY);
								sTitle = gszTranslation[PosNoticesCatClient];
								bLoadAnyway = true;
							}
						}
					}
				}
			}

			if (!bLoadAnyway)
			{
				xml.WriteItem("project",pNoticesInfoItem->sProject);
			}
			xml.WriteItem("title", sTitle);
			sATxt.Format("%d",pNoticesInfoItem->m_iChecksum);
			xml.WriteItem("checksum",sATxt);


// not used by read

			if (pNoticesInfoItem->m_pNotice != NULL)
			{
				xml.WriteItem("category",pNoticesInfoItem->m_pNotice->category);
				sATxt.Format("%f",pNoticesInfoItem->m_pNotice->create_time);
				xml.WriteItem("created",sATxt);
				sATxt.Format("%f",pNoticesInfoItem->m_pNotice->arrival_time);
				xml.WriteItem("arrival",sATxt);
			}
// not used by read

			if (pNoticesInfoItem->m_bCollapsed) sATxt = "1";
			else sATxt = "0";
			xml.WriteItem("collapsed",sATxt);
			if (pNoticesInfoItem->m_bDeleted) sATxt = "1";
			else sATxt = "0";
			xml.WriteItem("deleted",sATxt);

			xml.WriteEndGroup();
		}
		xml.WriteEnd();
	}
}

void CDlgNotices::EmptyNotice(CString sTxt)
{
	CCreateHtmlNotices createHtmlNotices;
	bool	bCreated, bVisible;
	CString sHtml;

	bVisible = false;
	if (m_pDoc->m_iViewActive == NOTICES_VIEW_ACTIVE)
	{
		if (IsWindowVisible() == TRUE)
		{
			bVisible = true;
		}
	}

	if (bVisible)
	{
		if (sTxt != m_sNoticesText)
		{
			m_sNoticesText = sTxt;

			bCreated = createHtmlNotices.Create(&m_sNoticesText, &sHtml);
			if (bCreated)
			{
				if (!m_bNavigate2Done)							//!!! EMPTY
				{
//					Navigate2(sHtml);
					m_bNavigate2Done = true;
				}
				else
				{
//					Refresh2(REFRESH_COMPLETELY);				//!!! EMPTY
				}
				m_tLastUpdate = 0;
//				m_iNoticesCheck	= 0;
			}
		}
		theApp.m_pMainFrame->SetBoincTasksStatus("");
	}
}

UINT CDlgNotices::CheckSumDescription(char *pDescription)
{
	int iSize;
	UINT uiChecksum;

	uiChecksum = 0;
	iSize = (int) strlen(pDescription);
	for (int iCount = 0; iCount < iSize; iCount++)
	{
		uiChecksum += (UINT) *pDescription;
		pDescription++;
	}
	return uiChecksum;
}

void CDlgNotices::AddNoticesItem(NOTICE *pNoticeClient, int iIDNotices, bool bFirst)
{
	CNoticesInfoItem	*pNoticesInfoItem;
	NOTICE				*pNotice;

	pNoticesInfoItem = new CNoticesInfoItem;
	pNotice = new NOTICE;
	
	pNoticesInfoItem->m_iID = iIDNotices;

	time_t tSystemTime;
	_time64(&tSystemTime);
	pNoticesInfoItem->m_dTime =  (double) tSystemTime;
	
	pNoticesInfoItem->m_iChecksum = CheckSumDescription((char *) pNoticeClient->description.c_str());
//	if (pNoticesInfoItem->m_pNotice  != NULL)
//	{
//		int iProblem = 1;
//	}
	pNoticesInfoItem->m_pNotice = pNotice;
	pNoticesInfoItem->sProject = pNoticeClient->project_name;

	if (strlen(pNoticeClient->title) == 0)
	{
		if (strstr(pNoticeClient->category, "client") != NULL)
		{
			pNoticesInfoItem->sTitle = gszTranslation[PosNoticesCatClient];
		}
	}
	else
	{
			pNoticesInfoItem->sTitle = pNoticeClient->title;
	}
	pNoticesInfoItem->m_bCollapsed = false;
	pNoticesInfoItem->m_bDeleted = false;

	if (strlen(pNotice->category) > 0)	pNoticesInfoItem->m_bAllowDelete = false;
	else	pNoticesInfoItem->m_bAllowDelete = true;

	CopyNotice(pNoticesInfoItem->m_pNotice, pNoticeClient);

	if (bFirst) m_lNotices.push_front(pNoticesInfoItem);
	else m_lNotices.push_back(pNoticesInfoItem);
}

void CDlgNotices::CopyNotice(NOTICE *pNoticeTo, NOTICE *pNoticeFrom)
{
	pNoticeTo->arrival_time = pNoticeFrom->arrival_time;
	strcpy_s(pNoticeTo->category,64, pNoticeFrom->category);
	pNoticeTo->create_time = pNoticeFrom->create_time;
	pNoticeTo->description = pNoticeFrom->description;
	strcpy_s(pNoticeTo->feed_url,256,pNoticeFrom->feed_url);
	strcpy_s(pNoticeTo->guid,256,pNoticeFrom->guid);
	pNoticeTo->is_private = pNoticeFrom->is_private;
	pNoticeTo->keep = pNoticeFrom->keep;
	strcpy_s(pNoticeTo->link,256,pNoticeFrom->link);
	strcpy_s(pNoticeTo->project_name,64,pNoticeFrom->project_name);
	pNoticeTo->seqno = -1;
	strcpy_s(pNoticeTo->title,256,pNoticeFrom->title);
}

int CDlgNotices::Compare(CNoticesInfoItem *pNoticesInfoItem1, CNoticesInfoItem *pNoticesInfoItem2, int iCountCompare)
{
	DWORD dwSortOn, dwSortOperator;
	int		i1Len, i2Len, iCompare;

	// when the second is 0 exchange and move the 0 to the bottom.
	if (pNoticesInfoItem1->m_pNotice == 0) return 0;
	if (pNoticesInfoItem2->m_pNotice == 0) return 1;

	dwSortOperator = m_sorting[iCountCompare].m_dwSortOperator;
	if (dwSortOperator == NOTICES_SORT_OPERATOR_NO) return 0;

	dwSortOn = m_sorting[iCountCompare].m_dwSortOn;

	switch (dwSortOn)
	{
		case NOTICES_SORT_CATEGORY:
			i1Len = (int) strlen(pNoticesInfoItem1->m_pNotice->category);
			i2Len = (int) strlen(pNoticesInfoItem2->m_pNotice->category);
			if (i1Len < i2Len)		// empty category last
			{
				return 1;
			}
			else
			{
				if (i1Len > i2Len)
				{
					return -1;
				}
				else
				{
					if (dwSortOperator == NOTICES_SORT_OPERATOR_UP)	iCompare = strcmp (pNoticesInfoItem1->m_pNotice->category, pNoticesInfoItem2->m_pNotice->category);
					else											iCompare = strcmp (pNoticesInfoItem2->m_pNotice->category, pNoticesInfoItem1->m_pNotice->category);
					return iCompare;
				}
			}

		break;
		case NOTICES_SORT_PROJECT:
			if (dwSortOperator == NOTICES_SORT_OPERATOR_UP)	iCompare = strcmp (pNoticesInfoItem1->m_pNotice->project_name, pNoticesInfoItem2->m_pNotice->project_name);
			else											iCompare = strcmp (pNoticesInfoItem2->m_pNotice->project_name, pNoticesInfoItem1->m_pNotice->project_name);
			return iCompare;
		break;
		case NOTICES_SORT_CREATION:
			if (pNoticesInfoItem1->m_pNotice->create_time == pNoticesInfoItem2->m_pNotice->create_time) return 0;
			if (dwSortOperator == NOTICES_SORT_OPERATOR_UP)
			{
				if (pNoticesInfoItem1->m_pNotice->create_time > pNoticesInfoItem2->m_pNotice->create_time)
				{
					return 1;
				}
			}
			else
			{
				if (pNoticesInfoItem1->m_pNotice->create_time < pNoticesInfoItem2->m_pNotice->create_time)
				{
					return 1;
				}
			}
			return -1;
		break;
		case NOTICES_SORT_ARRIVAL:
			if (pNoticesInfoItem1->m_pNotice->arrival_time == pNoticesInfoItem2->m_pNotice->arrival_time) return 0;
			if (dwSortOperator == NOTICES_SORT_OPERATOR_UP)
			{
				if (pNoticesInfoItem1->m_pNotice->arrival_time > pNoticesInfoItem2->m_pNotice->arrival_time)
				{
					return 1;
				}
			}
			else
			{
				if (pNoticesInfoItem1->m_pNotice->arrival_time < pNoticesInfoItem2->m_pNotice->arrival_time)
				{
					return 1;
				}
			}
			return -1;
		break;
	}
	return 0;
}

void CDlgNotices::SortNotices()
{
	CNoticesInfoItem	*pNoticesInfoItem0, *pNoticesInfoItem1, *pNoticesInfoTemp;
	int					iCompare;//, i1Len, i2Len;
	bool				bSorted;
	bool				bExchange;

	bSorted = true;

	if (m_lNotices.size() < 2) return;			// no sense to sort 1 notice.

	theApp.m_pDlgSettingsNotices->SendMessage(UWM_MSG_NOTICES_SORT, 0, (LPARAM) &m_sorting[0]);

	while (bSorted)
	{
		bSorted = false;
		for (int iCount=0; iCount < (int) m_lNotices.size()-1; iCount++)
		{
			pNoticesInfoItem0 = m_lNotices.at(iCount);
//			if (pNoticesInfoItem0->m_pNotice == NULL)
//			{
//				continue;
//			}

			pNoticesInfoItem1 = m_lNotices.at(iCount+1);
//			if (pNoticesInfoItem1->m_pNotice == NULL)
//			{
//				continue;
//			}

			bExchange = false;

			for (int iCountCompare = 0; iCountCompare < NOTICES_SORT_COUNT; iCountCompare++)
			{
				iCompare = Compare(pNoticesInfoItem0, pNoticesInfoItem1, iCountCompare);
				if (iCompare > 0)
				{
					bExchange = true;
					break;
				}
				else
				{
					if (iCompare < 0)
					{
						break;
					}
				}
			}

			if (bExchange)
			{
				pNoticesInfoTemp = pNoticesInfoItem0;				// 0 -> temp0, 1 -> 0, temp0 -> 1
				m_lNotices.at(iCount) = pNoticesInfoItem1;
				m_lNotices.at(iCount+1) = pNoticesInfoTemp;
				bSorted = true;
			}
		}
	}
}

void CDlgNotices::InitialUpdate(CRemoteCombinedDoc* pDoc)
{
	m_pDoc = pDoc;
}

void CDlgNotices::SendLogging(char *pcFrom, char *pcMessage, int iDebug)
{
	CLoggingMessage *psLog;
	psLog = new CLoggingMessage;

	psLog->m_pFrom =  pcFrom; psLog->m_pText = pcMessage;
	theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, iDebug);

	delete psLog;
}

void CDlgNotices::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNotices, CDialogEx)
	ON_MESSAGE (UWM_MSG_TIMER_NOTICES, OnTimerNotices) 

	ON_MESSAGE (UWM_MSG_THREAD_READY_RPC_NOTICES, OnReadyRpc)	
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_CLIENT, OnReadyRpcClient)

	ON_MESSAGE (UWM_MSG_NOTICES_BT_UPDATE, OnBoincTasksVersion)	
	ON_MESSAGE (UWM_MSG_NOTICES_BT_RULE, OnBoincTasksRule)	

	ON_MESSAGE (UWM_MSG_NOTICES_SET_COLLAPSE_DELETE, OnSetCollapseOrDelete)	
	ON_MESSAGE (UWM_MSG_NOTICES_REFRESH, OnRefresh)	
END_MESSAGE_MAP()


// CDlgNotices message handlers

LRESULT CDlgNotices::OnTimerNotices(WPARAM parm1, LPARAM parm2)
{
	char		*pcComputerId;
	CString		sStatus, sSelectedComputer, sTxt;
	CAnsiToUtf8	AnsiToUtf8;
	bool		bVisible, bStatus;

	int			iNrRpc, iNrTThrottleRpc, iThreadId, iTThrottleThreadId;

	if (m_pDoc == NULL) return 0;
	if (m_bDocumentIsClosing) return 0;			// everything is closing down.

	if (theApp.m_pDlgSettingsNotices->m_iCheckEvery == 0)
	{
		CString sEmpty;
		sEmpty = "<b>";
		sEmpty+= AnsiToUtf8.ToUtf8(gszTranslation[PosNoticeDisabledUser]);
		sEmpty+= "</b>";
		EmptyNotice(sEmpty);
		m_bDisabled = true;
		return 0;		// disabled
	}

	if (m_bDisabled)
	{
		CString sEmpty;
		EmptyNotice(sEmpty);			// clear notices
		m_bDisabled = false;
//		m_iNoticesCheck = 0;
		return 0;	
	}

	bVisible = false;
	if (m_pDoc->m_iViewActive == NOTICES_VIEW_ACTIVE)
	{
		if (m_pDoc->m_pNoticesView->IsWindowVisible() == TRUE)
		{
			bVisible = true;
		}
	}

	if (theApp.m_pDlgLogging->m_bLogDebugNoticesTimer)
	{
		CString sTxt;
		sTxt.Format("Busy %d, Locked %d, Visible %d", m_bThreadBusy, m_iThreadLocked, bVisible);
		SendLogging("Notices timer", sTxt.GetBuffer(), LOGGING_DEBUG);
		sTxt.ReleaseBuffer();
	}

	if (m_bThreadBusy)		// check for lockup
	{
		if (m_iThreadLocked++ > theApp.m_iThreadTimeout) {m_iThreadLocked = 0; m_bThreadBusy = false;}
		return 0;
	}
	m_iThreadLocked = 0;

	if (m_bThreadBusy)
	{
		if (bVisible)
		{
			theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusUpdating]);
		}
		return 0;
	}

	if (m_pDoc == NULL) return 0;

	time_t tSystemTime;
	_time64(&tSystemTime);
	time_t tLeft, tNextUpdate;


	if (m_noticeBoincTasksRules.m_iActiveNew != m_noticeBoincTasksRules.m_iActive)		// when there are rule notices speed up things
	{
		tNextUpdate = NOTICES_UPDATE_VISIBLE;
	}
	else
	{
		if (bVisible)	tNextUpdate = NOTICES_UPDATE_VISIBLE;
		else
		{
			tNextUpdate = theApp.m_pDlgSettingsNotices->m_iCheckEvery*60;
			if (tNextUpdate == 0) tNextUpdate = NOTICES_UPDATE_VISIBLE*10;
		}
	}

	tLeft =  ( m_tLastUpdate + tNextUpdate) - tSystemTime;

	if (theApp.m_pDlgLogging->m_bLogDebugNoticesTimer)
	{
		CString sTxt;
		sTxt.Format("Time left %d", tLeft);
		SendLogging("Notices timer", sTxt.GetBuffer(), LOGGING_DEBUG);
		sTxt.ReleaseBuffer();
	}

	if (tLeft > 0)
	{
		if (bVisible)
		{
			sStatus.Format(gszTranslation[PosWindowStatusMessageStatusUpdateInSec], tLeft);
			theApp.m_pMainFrame->SetBoincTasksStatus(sStatus.GetBuffer());
		}
		return 0;
	}

	if (bVisible) theApp.m_pMainFrame->SetBoincTasksStatus(TEXT_STATUS_WAITING);

	m_tLastUpdate = tSystemTime;

	iNrRpc = m_pDoc->GetRpcSelectedComputers();
	iNrTThrottleRpc = m_pDoc->GetRpcSelectedComputers();

	m_iSendRpcCount = 0;			
	m_iReadyRpcCount = 0;

	for (int iCount = 0; iCount < (int) theApp.m_lComputers.size(); iCount++)
	{
		m_lComputerRpcInfo.at(iCount)->m_iNrRead = 0;
		m_lComputerRpcInfo.at(iCount)->m_iRpcPosition = -1;
		m_lComputerRpcInfo.at(iCount)->m_pNotices->clear();
	}

	int iResult;
	for (int iCount = 0; iCount < iNrRpc; iCount++)
	{
		iResult = FALSE;

		m_pDoc->GetRpcThreadId(&iThreadId, iCount);

		if (m_pDoc->GetComputerIdStringWithIdNumber(iThreadId, &pcComputerId))
		{// get the computer id string for the thread
			iResult = (int) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_CHECK_SELECTED, (WPARAM) pcComputerId, 0);
		}
		if (iResult == TRUE || !bVisible)			// not visible send to all
		{
			m_lComputerRpcInfo.at(iCount)->m_iRpcPosition = iCount;
			::PostThreadMessage(iThreadId,UWM_MSG_THREAD_START_RPC_NOTICES, (WPARAM) this->m_hWnd, (LPARAM) m_lComputerRpcInfo.at(iCount));

			if (m_bCheckTThrottle)
			{
				if (theApp.m_pDlgSettingsGeneral->m_bConnecTthrottle)	// check if TThrottle is enabled
				{
					m_lComputerRpcInfoClient.at(iCount)->m_bReady = false;
					m_pDoc->GetTThrottleRpcThreadId(&iTThrottleThreadId,iCount);
					m_lComputerRpcInfoClient.at(iCount)->m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
					::PostThreadMessage(iTThrottleThreadId,UWM_MSG_THREAD_START_RPC_CLIENT,(WPARAM) this->m_hWnd, (LPARAM) m_lComputerRpcInfoClient.at(iCount));
				}
			}

			m_iSendRpcCount++;
			m_bThreadBusy = true;	
		}
	}
	m_bCheckTThrottle = false;

	// check push notices.

	if ( m_tLastPushNotices < tSystemTime)
	{
		// get push notices
		CPushCheck pushCheck;
		bStatus = pushCheck.ReadPushNotice(&sTxt);
		if (bStatus)
		{
			// read from server
			m_tPushNoticesInterval = NOTICES_PUSH_INTERVAL;	// every day

			int iNotices = ReadXmlPush(&sTxt);
			m_bPushNotices = true;
			if (theApp.m_pDlgLogging->m_bLogDebugNotices)
			{
				CString sTxtLog;
				sTxtLog.Format("Read from server %d bytes, found %d notices", sTxt.GetLength(), iNotices);
				SendLogging("Notices", sTxtLog.GetBuffer(), LOGGING_DEBUG);
				sTxtLog.ReleaseBuffer();
			}
		}
		else
		{
			m_tPushNoticesInterval += 60;
			if (m_tPushNoticesInterval > NOTICES_PUSH_INTERVAL)
			{
				m_tPushNoticesInterval = NOTICES_PUSH_INTERVAL;
			}
		}
		m_tLastPushNotices = tSystemTime + m_tPushNoticesInterval;
	}
	//

	return 0;
}

LRESULT CDlgNotices::OnReadyRpc(WPARAM parm1, LPARAM parm2)
{
	CAnsiToUtf8			AnsiToUtf8;
	CCreateHtmlNotices createHtmlNotices;
	bool	bCreated;//, bIsLocalhost;
	int		iConnectionStatus;
	int		iStatus;
	CRpcThreadReturn	*pThreadReturn;
	CString	sHtml, sTitle;
	NOTICE	*pNotice, *pNoticeClient, *pNoticePush;
	CNoticesInfoItem *pNoticesInfoItem;
	bool	bNoticeAlreadyThere = false;
	bool	bFoundNewNotice = false;
	int		iNotices = 0;
	CString	sComputer, sHtmlString;

	char	*pcInsertNoticeMore				= "__InsertNoticeMore__";
	char	*pcInsertNoticeLink				= "__InsertNoticeLink__";

	iConnectionStatus = (int) parm1;
	iStatus = (int) parm1;
	pThreadReturn = (CRpcThreadReturn *)	parm2;

	if (m_bDocumentIsClosing)
	{
		theApp.m_pMainFrame->SetBoincTasksStatus(gszTranslation[PosWindowStatusMessageStatusClosing]);
		delete pThreadReturn;
		return 0;			// everything is closing down.
	}

	bool bVisible;
	bVisible = (m_pDoc->m_iViewActive == NOTICES_VIEW_ACTIVE);

	if (m_sNoticesText != "") m_sNoticesText = "";

	m_lRpcInfoNotices.at(m_iReadyRpcCount)->m_pcComputerId = pThreadReturn->m_pcComputerId;
	m_lRpcInfoNotices.at(m_iReadyRpcCount)->m_pThread = pThreadReturn->m_pThread;
	m_lRpcInfoNotices.at(m_iReadyRpcCount)->m_iStatusRpc = iStatus;
	if (theApp.m_pDlgLogging->m_bLogDebugNotices)
	{
		CString sTxt;
		sTxt.Format("Count ready %d, Status %d, Computer:",m_iReadyRpcCount, iStatus);
		sTxt += pThreadReturn->m_pcComputerId;
		SendLogging("OnReadyRpc", sTxt.GetBuffer(), LOGGING_DEBUG);
		sTxt.ReleaseBuffer();
	}
	m_iReadyRpcCount++;

	theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_CONNECT, (WPARAM) pThreadReturn->m_pcComputerId, pThreadReturn->m_iStatus);

	if (m_iReadyRpcCount != m_iSendRpcCount)
	{
		delete pThreadReturn;
		return 0;	// wait for all results to come in. In this case it's one one.
	}

	for (int iCount = 0; iCount < m_iReadyRpcCount; iCount++)
	{
		if (m_lRpcInfoNotices.at(iCount)->m_iStatusRpc == RPC_STATUS_CONNECTED)
		{
			if (sComputer.GetLength() > 0) sComputer += ", ";
			sComputer += m_lRpcInfoNotices.at(iCount)->m_pcComputerId;
		}
	}

	// delete the old notices
	for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
	{
		if (m_lNotices.at(iPos)->m_pNotice != NULL)
		{
			delete m_lNotices.at(iPos)->m_pNotice;
			m_lNotices.at(iPos)->m_pNotice = NULL;
		}
		for (int iPosComputer = 0; iPosComputer < (int) m_lNotices.at(iPos)->m_lComputer.size(); iPosComputer++)
		{
			delete m_lNotices.at(iPos)->m_lComputer.at(iPosComputer);
		}
		m_lNotices.at(iPos)->m_lComputer.clear();
	}

	pNoticesInfoItem = NULL;

	// insert push messages

	for (int iPosPush = 0; iPosPush < (int) m_lNoticesPush.size(); iPosPush++)
	{
		pNoticePush = m_lNoticesPush.at(iPosPush);
		bNoticeAlreadyThere = false;
		for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
		{
			pNoticesInfoItem = m_lNotices.at(iPos);
			if (pNoticesInfoItem->sProject ==  pNoticePush->project_name)
			{
				if (pNoticesInfoItem->sTitle == pNoticePush->title)
				{
					if (pNoticesInfoItem->m_iChecksum == CheckSumDescription((char *) pNoticePush->description.c_str()))
					{
						if (!pNoticesInfoItem->m_bDeleted)
						{
							if (pNoticesInfoItem->m_pNotice == NULL)
							{
							// already there
								pNotice = new NOTICE;
								m_lNotices.at(iPos)->m_pNotice = pNotice;
								CopyNotice(pNotice, pNoticePush);
								m_lNotices.at(iPos)->m_bAllowDelete = false;
								iNotices++;
								bNoticeAlreadyThere = true;
								pNoticesInfoItem->m_bAllowDelete = true;
							}
							else
							{
								// already there
								bNoticeAlreadyThere = true;
								iNotices++;
								pNoticesInfoItem->m_bAllowDelete = true;
							}
						}
						else
						{
							if (pNoticesInfoItem->m_pNotice != NULL)
							{
								delete pNoticesInfoItem->m_pNotice;
								pNoticesInfoItem->m_pNotice = NULL;
							}
						}
						break;
					}
				}
			}
		}
		if (!bNoticeAlreadyThere)
		{
			bool bAddItem = true;

			if (pNoticesInfoItem != NULL)
			{
				if (pNoticesInfoItem->m_bDeleted)
				{
					bAddItem = false;
				}
			}
			if (bAddItem)
			{
				AddNoticesItem(pNoticePush, m_iIDNotices, true);
				m_iIDNotices++;
				m_iNewNotices++;
				iNotices++;
				bFoundNewNotice = true;
			}
		}
		m_bPushNotices = false;
	}

	// end insert push messages

	// insert BT rules
	if (m_noticeBoincTasksRules.m_iActiveNew > 0)
	{
		time_t tSystemTime;
		_time64(&tSystemTime);

		m_noticeBoincTasksRules.m_iActive = m_noticeBoincTasksRules.m_iActiveNew;

		m_pNoticeRules->m_iID = NOTICES_ID_BT_RULES;

		CString sTxt;
		sTxt.Format("Active BoincTasks rule(s): %d<br>", m_noticeBoincTasksRules.m_iActive);
		if (m_pNoticeRules->m_pNotice == NULL)
		{
			pNotice = new NOTICE;
			m_pNoticeRules->m_pNotice = pNotice;
		}

		m_pNoticeRules->m_pNotice->arrival_time = (double) tSystemTime;
		m_pNoticeRules->m_pNotice->create_time = (double) m_noticeBoincTasksRules.m_iTime;
//		m_pNoticeRules->m_pNotice->description = AnsiToUtf8.ToUtf8(&sTxt);
		m_pNoticeRules->m_dTime = 0;

		strcpy_s(m_pNoticeRules->m_pNotice->link, 255, "__bt_rules__");
		strcpy_s(m_pNoticeRules->m_pNotice->title, 255, AnsiToUtf8.ToUtf8(gszTranslation[PosNoticesCatBoincTasks]));
		sTxt += m_noticeBoincTasksRules.m_sRuleText;
		m_pNoticeRules->m_pNotice->description = AnsiToUtf8.ToUtf8(&sTxt);
		strcpy_s(m_pNoticeRules->m_pNotice->category, 64, "_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_BR");

		m_pNoticeRules->m_iChecksum = CheckSumDescription((char *) m_pNoticeRules->m_pNotice->description.c_str());
		m_pNoticeRules->m_bAllowDelete = false;
	}
	else
	{
		if (m_pNoticeRules->m_pNotice != NULL)			// no active rules, remove them
		{
			delete m_pNoticeRules->m_pNotice;
			m_pNoticeRules->m_pNotice = NULL;
		}
	}

	bNoticeAlreadyThere = false;
	pNoticeClient = m_pNoticeRules->m_pNotice;
	if (pNoticeClient != NULL)
	{
		for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
		{
			pNoticesInfoItem = m_lNotices.at(iPos);
			if (pNoticesInfoItem->sProject ==  pNoticeClient->project_name)
			{
				if (pNoticesInfoItem->sTitle == pNoticeClient->title)
				{
					if (pNoticesInfoItem->m_iChecksum == CheckSumDescription((char *) pNoticeClient->description.c_str()))
					{
						// already there
						pNotice = new NOTICE;
						if (pNoticesInfoItem->m_pNotice != NULL)
						{
							int iproblem = 1;
						}
						pNoticesInfoItem->m_pNotice = pNotice;
						CopyNotice(pNoticesInfoItem->m_pNotice, pNoticeClient);
						pNoticesInfoItem->m_bAllowDelete = false;
						bNoticeAlreadyThere = true;
						iNotices++;
					}
				}
			}
		}
		if (!bNoticeAlreadyThere)
		{
			AddNoticesItem(pNoticeClient, m_iIDNotices, true);
			m_iIDNotices++;
			m_iNewNotices++;
			iNotices++;
			bFoundNewNotice = true;
		}
	}
	// end insert BT rules
	
	// insert BT version
	if (m_noticeBoincTasksUpdate.m_iType > 0)			// && m_pNoticeUpdate->m_pNotice == NULL)
	{
		time_t tSystemTime;
		_time64(&tSystemTime);
		CString sLink, sExe;

		sLink = "<a href=\x22__InsertNoticeLink__\x22 target=\x22_new\x22>__InsertNoticeMore__</a>";

		m_pNoticeUpdate->m_iID = NOTICES_ID_BT_VERSION;

		CString sTxt, sIDS, sProgramVersion, sNewVersion, sNewVersionBeta, sCurrentVersion;

		sIDS = gszTranslation[PosDialogUpdateCurrentVersion];
		sProgramVersion.Format("%.2f",m_noticeBoincTasksUpdate.m_dCurrentVersion);
		sCurrentVersion.Format(sIDS, sProgramVersion); //"Current version of BT: %s"
		sTxt += sCurrentVersion;
		sTxt += "\r\n";

		bool bFound = false;		if (m_noticeBoincTasksUpdate.m_iCheckBeta == 0)
		{
			if (CompareDouble(m_noticeBoincTasksUpdate.m_dVersion, m_noticeBoincTasksUpdate.m_dCurrentVersion, 100) == COMPARE_LARGER)
			{
				sIDS= gszTranslation[PosDialogUpdateNewVersionFound];
				sProgramVersion.Format("%.2f", m_noticeBoincTasksUpdate.m_dVersion);
				sNewVersion.Format(sIDS, sProgramVersion); //"New version of BT found: %s"
				sTxt += sNewVersion;
				sTxt += ". ";
				sIDS= gszTranslation[PosDialogUpdateLocation];
				sTxt += sIDS;
				sTxt += ": ";
//				sTxt += sLink;
				bFound = true;
			}
		}
		else
		{
			if (CompareDouble(m_noticeBoincTasksUpdate.m_dVersionBeta, m_noticeBoincTasksUpdate.m_dCurrentVersion, 100) == COMPARE_LARGER)
			{
				sIDS= gszTranslation[PosDialogUpdateNewBetaVersionFound];
				sProgramVersion.Format("%.2f", m_noticeBoincTasksUpdate.m_dVersionBeta);
				sNewVersion.Format(sIDS, sProgramVersion); //"New version of BT found: %s"
				sTxt += sNewVersion;
				sTxt += ". ";
				bFound = true;
			}
		}

		if (bFound)
		{
			sTxt += "<a href=\x22";
			sTxt += "__update__";
			sTxt += "\x22>";
			sTxt += gszTranslation[PosDialogUpdateDirect];
			sTxt += "</a><br><br>";

			sIDS= gszTranslation[PosDialogUpdateLocation];
			sTxt += sIDS;
			sTxt += ": ";
			sTxt += sLink;
			sTxt.Replace(pcInsertNoticeMore,gszTranslation[PosDialogUpdateEurope]);

			if (m_noticeBoincTasksUpdate.m_iCheckBeta) sExe = m_noticeBoincTasksUpdate.m_sVersionBetaExe;
			else sExe = m_noticeBoincTasksUpdate.m_sVersionExe;
			sExe.Replace(".com",".eu");
			sTxt.Replace(pcInsertNoticeLink,sExe);

			sTxt += ", ";
			sTxt += sLink;
			sTxt.Replace(pcInsertNoticeMore,gszTranslation[PosDialogUpdateNorthAmerica]);
			sExe.Replace(".eu",".com");
			sTxt.Replace(pcInsertNoticeLink,sExe);

			if (m_pNoticeUpdate->m_pNotice == NULL)
			{
				pNotice = new NOTICE;
				m_pNoticeUpdate->m_pNotice = pNotice;
			}
	
			m_pNoticeUpdate->m_pNotice->arrival_time = (double) tSystemTime;
			m_pNoticeUpdate->m_pNotice->create_time = (double) tSystemTime;
			m_pNoticeUpdate->m_pNotice->description = AnsiToUtf8.ToUtf8(&sTxt);
			m_pNoticeUpdate->m_dTime = 0;
	
			strcpy_s(m_pNoticeUpdate->m_pNotice->link, 255, "https://efmer.com/download-boinctasks/");
			strcpy_s(m_pNoticeUpdate->m_pNotice->title, 255, AnsiToUtf8.ToUtf8(gszTranslation[PosNoticesCatBoincTasks]));
			strcpy_s(m_pNoticeUpdate->m_pNotice->category, 64, "_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_BT");

			m_pNoticeUpdate->m_iChecksum = CheckSumDescription((char *) m_pNoticeUpdate->m_pNotice->description.c_str());
			m_pNoticeUpdate->m_bAllowDelete = false;
		}
	}

	bNoticeAlreadyThere = false;
	pNoticeClient = m_pNoticeUpdate->m_pNotice;
	if (pNoticeClient != NULL)
	{
		for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
		{
			pNoticesInfoItem = m_lNotices.at(iPos);
			if (pNoticesInfoItem->sProject ==  pNoticeClient->project_name)
			{
				if (pNoticesInfoItem->sTitle == pNoticeClient->title)
				{
					if (pNoticesInfoItem->m_iChecksum == CheckSumDescription((char *) pNoticeClient->description.c_str()))
					{
						// already there
						pNotice = new NOTICE;
						if (pNoticesInfoItem->m_pNotice != NULL)
						{
							int iProblem = 1;
						}
						pNoticesInfoItem->m_pNotice = pNotice;
						CopyNotice(pNoticesInfoItem->m_pNotice, pNoticeClient);
						pNoticesInfoItem->m_bAllowDelete = false;
						bNoticeAlreadyThere = true;
						iNotices++;
					}
				}
			}
		}
		if (!bNoticeAlreadyThere)
		{
			AddNoticesItem(pNoticeClient, m_iIDNotices, true);
			m_iIDNotices++;
			m_iNewNotices++;
			iNotices++;
			bFoundNewNotice = true;
		}
	}
	// end insert BT version

	// insert TThrottle version

	if (m_noticeBoincTasksUpdate.m_iType > 0)				// && m_pNoticeUpdateTThrottle->m_pNotice == NULL)
	{
		time_t tSystemTime;
		_time64(&tSystemTime);
		CString sLink, sExe;

		sLink = "<a href=\x22__InsertNoticeLink__\x22 target=\x22_new\x22>__InsertNoticeMore__</a>";

		m_pNoticeUpdateTThrottle->m_iID = NOTICES_ID_TT_VERSION;
		bool	bFoundNewTThrottleVersion = false;
		CString sDescription, sTxt, sIDS, sProgramVersion, sNewVersion, sNewVersionBeta, sCurrentVersion;
		for (int iCountTThrottle = 0; iCountTThrottle < (int) theApp.m_lComputers.size(); iCountTThrottle++)
		{
			if (m_lComputerRpcInfoClient.at(iCountTThrottle)->m_bReady)
			{
				bool	bFound = false;
				sTxt = "";

				if (m_lComputerRpcInfoClient.at(iCountTThrottle)->m_fVersion >0)
				{
					if (CompareDouble(m_noticeBoincTasksUpdate.m_dTThrottleVersion, m_lComputerRpcInfoClient.at(iCountTThrottle)->m_fVersion, 100) == COMPARE_LARGER)
					{
						bFound = true;
					}
					if (m_noticeBoincTasksUpdate.m_iCheckBeta)
					{
						if (CompareDouble(m_noticeBoincTasksUpdate.m_dTThrottleVersionBeta, m_lComputerRpcInfoClient.at(iCountTThrottle)->m_fVersion, 100) == COMPARE_LARGER)
						{
							bFound = true;
						}
					}
				}
				if (bFound)
				{
					sDescription += m_lComputerRpcInfoClient.at(iCountTThrottle)->m_sComputerId.c_str();
					sDescription += ": ";
					sIDS = gszTranslation[PosDialogUpdateCurrentVersion];
					sProgramVersion.Format("%.2f", m_lComputerRpcInfoClient.at(iCountTThrottle)->m_fVersion);
					sCurrentVersion.Format(sIDS, sProgramVersion); //"Current version of BT: %s"
					sDescription += sCurrentVersion;
					sDescription += "<br>";
					bFoundNewTThrottleVersion = true;
				}
			}
		}

		if (bFoundNewTThrottleVersion == true)
		{
			if (m_noticeBoincTasksUpdate.m_iCheckBeta)
			{
				sIDS= gszTranslation[PosDialogUpdateNewBetaVersionFound];
				sProgramVersion.Format("%.2f", m_noticeBoincTasksUpdate.m_dTThrottleVersionBeta);
				sNewVersion.Format(sIDS, sProgramVersion); 
				sTxt += sNewVersion;
				sTxt += ". ";
			}
			else
			{
				sIDS= gszTranslation[PosDialogUpdateNewVersionFound];
				sProgramVersion.Format("%.2f", m_noticeBoincTasksUpdate.m_dTThrottleVersion);
				sNewVersion.Format(sIDS, sProgramVersion); 
				sTxt = sNewVersion;
				sTxt += ". ";
			}

			sIDS= gszTranslation[PosDialogUpdateLocation];
			sTxt += sIDS;
			sTxt += ": ";
			sTxt += sLink;
			sTxt.Replace(pcInsertNoticeMore,gszTranslation[PosDialogUpdateEurope]);

			if (m_noticeBoincTasksUpdate.m_iCheckBeta) sExe = m_noticeBoincTasksUpdate.m_sTThrottleVersionBetaExe;
			else sExe = m_noticeBoincTasksUpdate.m_sTThrottleVersionExe;
			sExe.Replace(".com",".eu");
			sTxt.Replace(pcInsertNoticeLink,sExe);

			sTxt += ", ";
			sTxt += sLink;
			sTxt.Replace(pcInsertNoticeMore,gszTranslation[PosDialogUpdateNorthAmerica]);
//			sExe = m_noticeBoincTasksUpdate.m_dTThrottleVersionBeta;
			sExe.Replace(".eu",".com");
			sTxt.Replace(pcInsertNoticeLink,sExe);
			sDescription += sTxt;

			if (m_pNoticeUpdateTThrottle->m_pNotice == NULL)
			{
				pNotice = new NOTICE;
				m_pNoticeUpdateTThrottle->m_pNotice = pNotice;
			}

			m_pNoticeUpdateTThrottle->m_pNotice->arrival_time = (double) tSystemTime;
			m_pNoticeUpdateTThrottle->m_pNotice->create_time = (double) tSystemTime;
			m_pNoticeUpdateTThrottle->m_pNotice->description = AnsiToUtf8.ToUtf8(&sDescription);
			m_pNoticeUpdateTThrottle->m_dTime = 0;

			strcpy_s(m_pNoticeUpdateTThrottle->m_pNotice->link, 255, "https://efmer.com/download-tthrottle/");
			strcpy_s(m_pNoticeUpdateTThrottle->m_pNotice->title, 255, AnsiToUtf8.ToUtf8(gszTranslation[PosNoticesCatTThrottle]));
			strcpy_s(m_pNoticeUpdateTThrottle->m_pNotice->category, 64, "_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_TT");

			m_pNoticeUpdateTThrottle->m_iChecksum = CheckSumDescription((char *) m_pNoticeUpdateTThrottle->m_pNotice->description.c_str());
			m_pNoticeUpdateTThrottle->m_bAllowDelete = false;
		}
	}

	bNoticeAlreadyThere = false;
	pNoticeClient = m_pNoticeUpdateTThrottle->m_pNotice;
	if (pNoticeClient != NULL)
	{
		for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
		{
			pNoticesInfoItem = m_lNotices.at(iPos);
			if (pNoticesInfoItem->sProject ==  pNoticeClient->project_name)
			{
				if (pNoticesInfoItem->sTitle == pNoticeClient->title)
				{
					if (pNoticesInfoItem->m_iChecksum == CheckSumDescription((char *) pNoticeClient->description.c_str()))
					{
						// already there
						pNotice = new NOTICE;
						if (pNoticesInfoItem->m_pNotice != NULL)
						{
							int iProblem = 1;
						}
						pNoticesInfoItem->m_pNotice = pNotice;
						CopyNotice(pNoticesInfoItem->m_pNotice, pNoticeClient);
						pNoticesInfoItem->m_bAllowDelete = false;
						bNoticeAlreadyThere = true;
						iNotices++;
					}
				} 
			}
		}
		if (!bNoticeAlreadyThere)
		{
			AddNoticesItem(pNoticeClient, m_iIDNotices, true);
			m_iIDNotices++;
			m_iNewNotices++;
			iNotices++;
			bFoundNewNotice = true;
		}
	}
	// end insert TThrottle version

	for (int iCountComputer = 0; iCountComputer < (int) theApp.m_lComputers.size(); iCountComputer++)
	{
		for (int iPosClient = 0; iPosClient < (int) m_lComputerRpcInfo.at(iCountComputer)->m_pNotices->notices.size(); iPosClient++)
		{
			pNoticeClient = m_lComputerRpcInfo.at(iCountComputer)->m_pNotices->notices[iPosClient];
			sTitle = pNoticeClient->title;
//			if (strstr(pNoticeClient->description.c_str(), NOTICES_DETECT_NEW_BOINC_VERSION) != NULL)
//			{
//				sTitle = NOTICES_NEW_BOINC_VERSION;
//			}
//			else
			{
				if (strstr(pNoticeClient->category, "client") != NULL)
				{
					sTitle = gszTranslation[PosNoticesCatClient];
				}
			}

			bNoticeAlreadyThere = false;
			for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
			{
				pNoticesInfoItem = m_lNotices.at(iPos);
				if (pNoticesInfoItem->sProject ==  pNoticeClient->project_name)
				{
					if (pNoticesInfoItem->sTitle == sTitle)
					{
						if (pNoticesInfoItem->m_iChecksum == CheckSumDescription((char *) pNoticeClient->description.c_str()))
						{
							// already there
	
							CString *psComputer = new CString;
							char *pcComputer;
							m_pDoc->GetComputerIds(&pcComputer, m_lComputerRpcInfo.at(iCountComputer)->m_iRpcPosition);
							*psComputer = pcComputer;
							pNoticesInfoItem->m_lComputer.push_back(psComputer);

							time_t tSystemTime;
							_time64(&tSystemTime);
							pNoticesInfoItem->m_dTime = (double) tSystemTime;

							if (!pNoticesInfoItem->m_bDeleted)
							{
								if (pNoticesInfoItem->m_pNotice == NULL)
								{
									pNotice = new NOTICE;
									pNoticesInfoItem->m_pNotice = pNotice;
									CopyNotice(pNoticesInfoItem->m_pNotice, pNoticeClient);
								}
								if (strlen(pNoticesInfoItem->m_pNotice->category) > 0)	pNoticesInfoItem->m_bAllowDelete = false;
								else	pNoticesInfoItem->m_bAllowDelete = true;
							}
							else
							{
								if (pNoticesInfoItem->m_pNotice != NULL)
								{
									delete pNoticesInfoItem->m_pNotice;
									pNoticesInfoItem->m_pNotice = NULL;
								}
							}
							bNoticeAlreadyThere = true;
							iNotices++;
						}
					}
				}
			}

			if (!bNoticeAlreadyThere)
			{
				// not yet in the list
	
				AddNoticesItem(pNoticeClient, m_iIDNotices);
				m_iIDNotices++;
				m_iNewNotices++;
				iNotices++;
				bFoundNewNotice = true;
			}
		}
	}

	if (bFoundNewNotice || m_bNoticesCollapseDeleteChanged)
	{
		WriteXml();
		m_bNoticesCollapseDeleteChanged = false;
	}


	if (m_iNewNotices > 0 && bVisible)
	{
		m_iNewNotices = 0;	// remove the notices
		::PostMessage(theApp.m_pMainFrame->m_hWnd, UWM_MSG_NOTICES_ALERT, m_iNewNotices, 0);
	}

	HWND hWnd = theApp.m_pMainFrame->m_dlgBarViewSelect.m_hWnd;
	::PostMessage(hWnd,UWM_MSG_TAB_VIEW_NOTICES_COUNT,m_iNewNotices,0);

//	if (bIsLocalhost)
	{
		int iAlertEvery = theApp.m_pDlgSettingsNotices->m_iAlertEvery;
		if (iAlertEvery > 0) 
		{
			time_t tSystemTime, tLeft;
			_time64(&tSystemTime);
			tLeft =  (m_tLastNoticeAlert + (iAlertEvery*60)) - tSystemTime;
			if (tLeft <= 0)
			{
				m_tLastNoticeAlert = tSystemTime;
				::PostMessage(theApp.m_pMainFrame->m_hWnd, UWM_MSG_NOTICES_ALERT, m_iNewNotices, 0);
			}
		}
	}

	SortNotices();

	time_t tSystemTime;
	double dTimeShowFrom;
	_time64(&tSystemTime);
	if (theApp.m_pDlgSettingsNotices->m_iHideNoticeAfter > 0)	dTimeShowFrom = (double) (tSystemTime - (theApp.m_pDlgSettingsNotices->m_iHideNoticeAfter*60*60*24));
	else dTimeShowFrom = 0;

	if (iNotices > 0)
	{
		if (g_bWebServerActive) bCreated = createHtmlNotices.Create(&m_lNotices, &sHtml, &sHtmlString, sComputer.GetBuffer(), dTimeShowFrom);
		else  bCreated = createHtmlNotices.Create(&m_lNotices, &sHtml, NULL, sComputer.GetBuffer(), dTimeShowFrom);
	}
	else
	{
		bCreated = createHtmlNotices.Create(NULL, &sHtml, NULL, pThreadReturn->m_pcComputerId, 0);
	}
	if (bCreated)
	{
		if (!m_bNavigate2Done)
		{
			m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_REFRESH, (WPARAM) &sHtml, 0);
//			Navigate2(sHtml);
			m_bNavigate2Done = true;
		}
		else
		{
			if (theApp.m_pDlgLogging->m_bLogDebugNotices)
			{
				CString sTxt;
				sTxt.Format("Os: %d", theApp.m_pDlgSettingsGeneral->m_iOs);
				SendLogging("Refresh", sTxt.GetBuffer(), LOGGING_DEBUG);
				sTxt.ReleaseBuffer();
			}

			if (theApp.m_pDlgSettingsGeneral->m_iOs == OS_SELECTED_WINDOWS)
			{
				m_pDoc->m_pNoticesView->SendMessage(UWM_MSG_NOTICES_REFRESH, (WPARAM) &sHtml, 1);
//				Refresh2(REFRESH_COMPLETELY);
			}
			else
			{
				m_pDoc->m_pTaskView->SendMessage(UWM_MSG_NOTICES_NEW, (WPARAM) this->m_hWnd, (LPARAM) &sHtml);
//				Navigate(sHtml);
			}
		}
	}

	delete pThreadReturn;
	pThreadReturn = NULL;

	if (g_bWebServerActive)
	{
		if (g_pcWebServerHtml == NULL)
		{
			char *pcHtmlNew;
			int iLen =sHtmlString.GetLength()+1;
			pcHtmlNew = new char [iLen];
			strcpy_s(pcHtmlNew, iLen, sHtmlString.GetBuffer());
			g_pcWebServerHtml = pcHtmlNew;
			g_pThreadWebServer->PostThreadMessage(UWM_MSG_THREAD_WEB_HTML,0,6);
			g_tWebServerHtml = GetTickCount();
		}
	}

	m_bThreadBusy = false;
	m_noticeBoincTasksUpdate.m_bRead = true;
	return 0;
}

LRESULT CDlgNotices::OnReadyRpcClient(WPARAM parm1, LPARAM parm2)
{
	CRpcInfoClient *pInfoClient;
	CString sFrom, sText, sVersion, sComputerIdTThrottle;

	if (m_bDocumentIsClosing) return 0;			// everything is closing down.

	pInfoClient = (CRpcInfoClient *) parm1;
	if (pInfoClient->m_iStatus == 0)		// valid
	{
		pInfoClient->m_bReady = true;
		pInfoClient->m_bReadyCopy = true;
		pInfoClient->m_iLostCount = 0;
	}
	else
	{
		if (pInfoClient->m_iLostCount > 3)
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

LRESULT CDlgNotices::OnBoincTasksVersion(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) return 0;			// everything is closing down.

	CNoticeBoincTasksUpdate *pNoticeBoincTasksUpdate;

	pNoticeBoincTasksUpdate = (CNoticeBoincTasksUpdate *) parm2;

	m_noticeBoincTasksUpdate.m_dCurrentVersion = pNoticeBoincTasksUpdate->m_dCurrentVersion;
	m_noticeBoincTasksUpdate.m_dVersion = pNoticeBoincTasksUpdate->m_dVersion;
	m_noticeBoincTasksUpdate.m_dVersionBeta = pNoticeBoincTasksUpdate->m_dVersionBeta;
	m_noticeBoincTasksUpdate.m_sVersionExe = pNoticeBoincTasksUpdate->m_sVersionExe;
	m_noticeBoincTasksUpdate.m_sVersionBetaExe = pNoticeBoincTasksUpdate->m_sVersionBetaExe;
	m_noticeBoincTasksUpdate.m_iType = pNoticeBoincTasksUpdate->m_iType;

	m_noticeBoincTasksUpdate.m_dTThrottleVersion = pNoticeBoincTasksUpdate->m_dTThrottleVersion;
	m_noticeBoincTasksUpdate.m_dTThrottleVersionBeta = pNoticeBoincTasksUpdate->m_dTThrottleVersionBeta;
	m_noticeBoincTasksUpdate.m_sTThrottleVersionExe = pNoticeBoincTasksUpdate->m_sTThrottleVersionExe;
	m_noticeBoincTasksUpdate.m_sTThrottleVersionBetaExe = pNoticeBoincTasksUpdate->m_sTThrottleVersionBetaExe;

	m_noticeBoincTasksUpdate.m_iCheckBeta = pNoticeBoincTasksUpdate->m_iCheckBeta;
	m_noticeBoincTasksUpdate.m_bRead = false;

	m_bCheckTThrottle = true;
	return 0;
}

LRESULT CDlgNotices::OnBoincTasksRule(WPARAM parm1, LPARAM parm2)
{
	int	iSize, iPos;
	std::deque<CActiveRuleItem*> *plActiveRule;
	CString		sRuleText;

	if (m_bDocumentIsClosing) return 0;			// everything is closing down.

	plActiveRule = (std::deque<CActiveRuleItem*> *) parm2;

	int iActiveRules;
	iActiveRules = 0;
	iSize = (int) plActiveRule->size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		if (plActiveRule->at(iPos)->m_bEventTriggered)
		{
			m_noticeBoincTasksRules.m_iTime = plActiveRule->at(iPos)->m_iTriggerTime;
			if (plActiveRule->at(iPos)->m_iEventShow == EVENT_SHOW_NOTICE)
			{
				iActiveRules++;															// triggered and set as show in notices
				sRuleText += plActiveRule->at(iPos)->m_sRuleName;
				sRuleText += ", ";
				sRuleText += plActiveRule->at(iPos)->m_sRuleText;
				sRuleText += "<br>";
			}
		}
	}
	m_noticeBoincTasksRules.m_iActiveNew = iActiveRules;
	m_noticeBoincTasksRules.m_sRuleText = sRuleText;

	return 0;
}

LRESULT CDlgNotices::OnRefresh(WPARAM parm1, LPARAM parm2)
{
	if (m_bDocumentIsClosing) return 0;			// everything is closing down.

	m_tLastUpdate = 0;	

	return 0;
}

LRESULT CDlgNotices::OnSetCollapseOrDelete(WPARAM parm1, LPARAM parm2)
{
	int iNumber;

	if (m_bDocumentIsClosing) return 0;			// everything is closing down.

	iNumber = (int) parm2;

	for (int iPos = 0; iPos < (int) m_lNotices.size(); iPos++)
	{
		if (m_lNotices.at(iPos)->m_iID == iNumber)
		{
			if (parm1 == 0)
			{
				m_lNotices.at(iPos)->m_bCollapsed = !m_lNotices.at(iPos)->m_bCollapsed;
			}
			else
			{
				m_lNotices.at(iPos)->m_bDeleted = true;
			}
			m_tLastUpdate = 0;
			m_bNoticesCollapseDeleteChanged = true;
			return TRUE;
		}
	}
	return FALSE;
}



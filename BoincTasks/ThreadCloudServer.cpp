f// ThreadWebServer.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include <direct.h>
#include <share.h>
#include "BoincTasks.h"
#include "Xml.h"
#include "DlgLogging.h"
#include "RemoteCombinedDoc.h"
#include "TemplateRemoteTaskView.h"
#include "RemoteCombinedTaskView.h"
#include "TemplateRemoteProjectView.h"
#include "RemoteCombinedProjectView.h"
#include "TemplateRemoteTransferView.h"
#include "RemoteCombinedTransferView.h"
#include "TemplateRemoteHistoryView.h"
#include "RemoteCombinedHistoryView.h"
//#include "HtmlConnect.h"
//#include "Base64.h"
#include "MainFrm.h"
#include "CreateHtmlWebServerComputerSelect.h"
#include "CreateHtmlWebServerFilter.h"
#include "CreateHtmlWebServerGraphComputerSelect.h"
#include "CreateHtmlWebServerGraphProjectSelect.h"
#include "CreateHtmlWebServerGraph.h"
#include "DlgGraphic.h"
#include "DlgSettingsMobile.h"
#include "Translation.h"
#include "ThreadCloudConnect.h"
#include "ThreadCloudServer.h"


CThreadCloudServer	*g_pThreadCloudServer = NULL; 

bool	g_bThreadCloudServerIsRunning = false;
bool	g_bCloudServerRunning = false;

bool	g_bCloudServerRequestData = false;
bool	g_bCloudServerAwake = false;
char	*g_pcCloudServerHtmlTab[7];
//int		g_iCloudServerHtmlTabSend[7];
int		g_iCloudServerHtmlTabProcessed[7];
DWORD	g_tCloudServerHtmlTabSend[7];

time_t	g_tLastCloudActivety = 0;
char	*g_pcCloudServerHtml = NULL;
DWORD	g_tCloudServerHtml = 0;

// CThreadWebServer

IMPLEMENT_DYNCREATE(CThreadCloudServer, CWinThread)

CThreadCloudServer::CThreadCloudServer()
{
	m_bLogDebugCloud = theApp.m_pDlgLogging->m_bLogDebugCloud;

	g_bThreadCloudServerIsRunning = true;
	m_bExit = false;

	m_sLastTime = "";

	for (int iCount = 0; iCount < 6; iCount++)
	{
//		delete g_pcWebServerHtmlTab[iCount];	no not here
		g_pcCloudServerHtmlTab[iCount] = NULL;
//		g_iCloudServerHtmlTabSend[iCount] = FALSE;
		g_iCloudServerHtmlTabProcessed[iCount] = TRUE;
		g_tCloudServerHtmlTabSend[iCount] = 0;
	}
	m_iCloudServerHtmlCurrentTab = TAB_TASKS;
	m_bLoggedIn = false;
	m_bWebServerSelectMultiple = false;
	m_iWuTotal = -1;

	m_pXml = new CXml(false,true);
	m_pThreadCloudConnect = NULL;

	m_iRequest = CLIENT_REQUEST_NONE;
	m_iIdClientLast = -999;	// anything exept m_iRequest
}

CThreadCloudServer::~CThreadCloudServer()
{
	delete m_pXml;
}

BOOL CThreadCloudServer::InitInstance()
{
	MSG		msg;
	int		iTab, iStatus;
	CString sFormat, *psStatus, *psData;


	while(!m_bExit)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			switch (msg.message)
			{
				case UWM_MSG_THREAD_QUIT:
					g_bCloudServerRunning = false;
					g_bCloudServerRequestData = false;
//					m_bExit = true;		// wait for the connect to quit
					if (m_pThreadCloudConnect != NULL)	m_pThreadCloudConnect->PostThreadMessage(UWM_MSG_THREAD_QUIT, 0, 0);
				break;
				case UWM_MSG_THREAD_WEB_START:
					m_hWnd = (HWND) msg.wParam;
					g_bCloudServerRequestData = false;			// active is the high speed mode, for now we will check if there is a connection first.
//					m_sLoginID = "";
					if (m_pThreadCloudConnect == NULL)
					{
						m_pThreadCloudConnect = (CThreadCloudConnect *) AfxBeginThread(RUNTIME_CLASS(CThreadCloudConnect));
						m_pThreadCloudConnect->PostThreadMessage(UWM_MSG_THREAD_WEB_START, msg.wParam, this->m_nThreadID);
					}
				break;
				case UWM_MSG_THREAD_WEB_SERVER:
					if (m_pThreadCloudConnect != NULL) m_pThreadCloudConnect->PostThreadMessage(UWM_MSG_THREAD_WEB_SERVER,msg.wParam,msg.lParam);
				break;
				case UWM_MSG_THREAD_WEB_EMAIL:
					if (m_pThreadCloudConnect != NULL) m_pThreadCloudConnect->PostThreadMessage(UWM_MSG_THREAD_WEB_EMAIL,msg.wParam,msg.lParam);
				break;
				case UWM_MSG_THREAD_WEB_STOP:
					g_bCloudServerRunning = false;
					g_bCloudServerRequestData = false;
					if (m_pThreadCloudConnect != NULL) m_pThreadCloudConnect->PostThreadMessage(UWM_MSG_THREAD_QUIT, 0, 0);
					m_pThreadCloudConnect = NULL;
				break;
				case UWM_MSG_THREAD_WEB_HTML:
					iTab = (int) msg.lParam;
					if (iTab == TAB_TASKS)
					{
						m_iWuTotal =  (int) msg.wParam;
					}

					g_bCloudServerRunning = false;	// stop once the data is recieved.
					if (g_pcCloudServerHtmlTab[iTab] != NULL) delete g_pcCloudServerHtmlTab[iTab];
					g_pcCloudServerHtmlTab[iTab] = g_pcCloudServerHtml;
//					g_iCloudServerHtmlTabSend[iTab] = FALSE;
					g_tCloudServerHtmlTabSend[iTab] = g_tCloudServerHtml;
					g_iCloudServerHtmlTabProcessed[iTab] = FALSE;
					g_pcCloudServerHtml = NULL;
				break;

				case UWM_MSG_THREAD_WEB_STATUS:
					psStatus = (CString *) msg.lParam;
//					strcpy_s(&g_pcStatusToSend[0], 128, pStatus->GetBuffer());
//					g_pcStatusToSend[127] = 0;
					delete psStatus;
				break;
				case UWM_MSG_THREAD_CLOUD_STATUS:
					switch (msg.wParam)
					{
						case CLOUD_STATUS_LOGIN:
							m_bLoggedIn = true;
						break;
						case CLOUD_STATUS_LOGIN_FAILED:
							m_bLoggedIn = false;
						break;
						case CLOUD_STATUS_DATA_READ:
							psData = (CString *) msg.lParam;
							m_iRequest = ReadData(psData);
							m_iRequestTimeout = 0;
						break;
						case CLOUD_STATUS_FINAL_TIMEOUT:
							Logging("Cloud timeout final.", LOGGING_DEBUG);
						case CLOUD_STATUS_QUIT:
							::PostMessage(m_hWnd, UWM_MSG_THREAD_QUIT, 0, 0);
							m_pThreadCloudConnect = NULL;
							Logging("Cloud quit.", LOGGING_DEBUG);
							m_bExit = true;
						break;
				}
				break;
			}
		}
		
		if (m_iRequest != CLIENT_REQUEST_NONE)
		{
			iStatus = WriteData(m_iRequest);
			if (iStatus == STATUS_WRITE_DATA_OK)
			{
				Logging(sFormat, LOGGING_DEBUG);
				g_bCloudServerRequestData = false;
				m_iRequest = CLIENT_REQUEST_NONE;
			}
			else
			{
//				Logging("Wait for tab data",LOGGING_DEBUG);
				m_iRequestTimeout++;
				if (m_iRequestTimeout > 100)
				{
					WriteData(RQ_TIMEOUT);
					g_bCloudServerRequestData = false;
					m_iRequest = CLIENT_REQUEST_NONE;			
				}
			}
		}
		Sleep(100);
	}

	for (int iCount = 0; iCount < 6; iCount++)
	{
		if (g_pcCloudServerHtmlTab[iCount]  != NULL)	delete g_pcCloudServerHtmlTab[iCount];
		g_pcCloudServerHtmlTab[iCount] = NULL; 
	}
	delete g_pcCloudServerHtml;
	g_pcCloudServerHtml = NULL;
	g_pThreadCloudServer = NULL;
	Logging("Cloud Server thread closed", LOGGING_DEBUG);
	Sleep(2000);
	return FALSE;
}
/*
void  CThreadCloudServer::SleepPeek(int iSleep)
{
	MSG		msg;

	while (iSleep)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				return;
			}
		}
		Sleep(100);
		iSleep--;
	}
}
*/

int CThreadCloudServer::ExitInstance()
{
	g_bThreadCloudServerIsRunning = false;
	return CWinThread::ExitInstance();
}

int CThreadCloudServer::ReadData(CString *psData)
{
	CFile				file;
	CString				sHandshake, sTxt, sFormat, sHtml, sState, sComputer, sGroup, sDat, sColumn;
	std::string			sData;
	int					iNr, iSelectedTab, iRequestType, iPos, iOperation, iFont, iFontHeader, iThreadId;
	CView				*pView;
	CRemoteCombinedDoc	*pDoc;
	POSITION			posView;
	char				*pcFound;
	CHtmlExchange		htmlExchange;
	bool				bEqual;
		
	iRequestType = -1;

	sHandshake = *psData;
	delete psData;

	if (m_pXml->ReadStart(&sHandshake, "btrq"))
	{
		if (m_pXml->ReadGroup("rqt", true))
		{
			m_pXml->ReadItem("t",&sTxt);
			iRequestType = atoi(sTxt);
			sFormat = "Request type: ";
			switch(iRequestType)
			{
				case RQ_LOGIN:
					sFormat += "Login";
				break;
				case RQ_TAB_DATA:
					sFormat += "Data";
				break;
				default:
				sFormat += sTxt;
			}

			m_pXml->ReadItem("i",&sTxt);
			sFormat = sFormat +  ", ID: " + sTxt;
			m_pXml->ReadItem("d",&sDat);
			m_pXml->ReadItem("cl",&sColumn);
			Logging(sFormat, LOGGING_DEBUG);
			m_iIdClient = atoi(sTxt);
		}
	}

	if (m_iIdClient == m_iIdClientLast)
	{
		bEqual = true;
	}
	else
	{
		bEqual = false;
	}
	m_iIdClientLast = m_iIdClient;

	switch(iRequestType)
	{
		case RQ_LOGIN:
			Logging("Request login", LOGGING_DEBUG);
			return RQ_LOGIN;
		break;
		case RQ_COMPUTER_DIALOG:
			GetComputerTree(&sData);
			sHtml = sData.c_str();
			WriteData(RQ_COMPUTER_DIALOG, &sHtml);
			return CLIENT_REQUEST_NONE;
		break;
		case RQ_COMPUTER_SELECT:
			iPos = sHandshake.Find("</btrq>");
			if (iPos > 0)
			{
				sTxt = sHandshake.Mid(iPos+7);
				pcFound = sTxt.GetBuffer();
				sState = ExtractXmlTag(&pcFound);
				if (sState.GetLength() > 0)
				{
					sGroup = ExtractXmlTag(&pcFound);
					if (sGroup.GetLength() > 0)
					{
						sComputer = ExtractXmlTag(&pcFound);
						if (sComputer.GetLength() > 0)
						{
							CComputerSetItem *pComputerSetItem;
							pComputerSetItem = new CComputerSetItem;
							if (sState == "true") pComputerSetItem->m_bSet = true;
							else pComputerSetItem->m_bSet = false;
							pComputerSetItem->m_sGroup = sGroup;
							pComputerSetItem->m_sComputer = sComputer;
							BOOL bSingle;
							if (!bEqual) bSingle = (BOOL) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_SELECTED, (WPARAM) pComputerSetItem, 0);
						}
					}
				}
				GetComputerTree(&sData);
				sHtml = sData.c_str();
				WriteData(RQ_COMPUTER_DIALOG, &sHtml);
				return CLIENT_REQUEST_NONE;
			}
			return CLIENT_REQUEST_NONE;
		break;
		case RQ_SELECT_MULTIPLE:
			iPos = sHandshake.Find("</btrq>");
			if (iPos > 0)
			{
				sTxt = sHandshake.Mid(iPos+7);
				pcFound = sTxt.GetBuffer();
				sState = ExtractXmlTag(&pcFound);
				if (sState.GetLength() > 0)
				{
					if (sState.Find("sel_multiple") >=0)
					{
						m_bWebServerSelectMultiple = true;
					}
					else
					{
						if (sState.Find("sel_single") >=0)
						{
							m_bWebServerSelectMultiple = false;
						}
						else
						{
							iPos = sState.Find("sel_filter");
							if (iPos >= 0)
							{
								pDoc = pGetDoc();
								if (pDoc != NULL)
								{
									sTxt = sHandshake.Mid(iPos+10);
									pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
									if (pView)
									{
										htmlExchange.m_iOption = CLOUD_SELECT_FILTER;
										htmlExchange.m_bSendData = false;
										if (!bEqual) pView->SendMessage(UWM_MSG_WEB_ITEM_SELECTED, 0, (LPARAM) &htmlExchange); // WEBSERVER_SELECT_FILTER);
									}
								}
							}
						}
					}
				}
			}
			return RQ_SELECT_MULTIPLE;
		break;
		case RQ_TASK_OPERATION:
		case RQ_PROJECT_OPERATION:
		case RQ_TRANSFER_OPERATION:
			iOperation = -1;
			iPos = sHandshake.Find("</btrq>");
			if (iPos > 0)
			{
				sTxt = sHandshake.Mid(iPos+7);
				pDoc = pGetDoc();
				if (pDoc != NULL)
				{
					if (iRequestType == RQ_TASK_OPERATION)
					{
						pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
						if (pView != NULL)
						{
							if (sTxt.Find("suspend") >= 0) iOperation = TASKS_OP_SUSPEND;
							if (sTxt.Find("resume") >= 0) iOperation = TASKS_OP_RESUME;
							if (sTxt.Find("abort") >= 0) iOperation = TASKS_OP_ABORT;
							if (!bEqual) 
							{
								if (iOperation > 0)	pView->SendMessage(UWM_MSG_WEB_OPERATION, iOperation, 0);
							}
						}
						return RQ_TASK_OPERATION;
					}
					if (iRequestType == RQ_PROJECT_OPERATION)
					{
						pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedProjectView));
						if (pView != NULL)
						{
							if (sTxt.Find("update") >= 0) iOperation = PROJECT_OP_UPDATE;
							if (sTxt.Find("suspend") >= 0) iOperation = PROJECT_OP_SUSPEND;
							if (sTxt.Find("resume") >= 0) iOperation = PROJECT_OP_RESUME;
							if (sTxt.Find("nomorework") >= 0) iOperation = PROJECT_OP_NOMOREWORK;
							if (sTxt.Find("allowwork") >= 0) iOperation = PROJECT_OP_ALLOWMOREWORK;
							if (!bEqual) 
							{
								if (iOperation > 0)	pView->SendMessage(UWM_MSG_WEB_OPERATION, iOperation, 0);
							}
						}
						return RQ_PROJECT_OPERATION;
					}
					if (iRequestType == RQ_TRANSFER_OPERATION)
					{
						pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTransferView));
						if (pView != NULL)
						{
							if (sTxt.Find("retry") >= 0) iOperation = TRANSFER_OP_RETRY;
							if (sTxt.Find("retryall") >= 0) iOperation = TRANSFER_OP_RETRY_ALL;
							if (sTxt.Find("abort") >= 0) iOperation = TRANSFER_OP_ABORT;
							if (!bEqual) 
							{
								if (iOperation > 0)	pView->SendMessage(UWM_MSG_WEB_OPERATION, iOperation, 0);
							}
						}
						return RQ_TRANSFER_OPERATION;
					}
				}
			}
			return CLIENT_REQUEST_NONE;
		break;
		case RQ_FILTER_GET:
			return RQ_FILTER_GET;
		break;
		case RQ_FILTER_SET:
			if (m_pXml->ReadStart(&sHandshake, "btrq"))
			{
				if (m_pXml->ReadGroup("fi", true))
				{
					m_pXml->ReadItem("dt",&sTxt);
					sHtml+= sTxt;
				}
			}
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
				if (pView != NULL)
				{
					if (!bEqual) pView->SendMessage(UWM_MSG_WEB_FILTER, TRUE, (LPARAM) sHtml.GetBuffer());
				}
			}
			return RQ_FILTER_SET;
		break;
		case RQ_PROJECT_PROPERTIES:
			return RQ_PROJECT_PROPERTIES;
		break;
		case RQ_TASK_PROPERTIES:
			return RQ_TASK_PROPERTIES;
		break;
		case RQ_REPORT:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				pDoc->OnExtraDonetworkcommunication();
			}
			return RQ_REPORT;
		break;
		case RQ_SORT:
			iPos = sHandshake.Find("</btrq>");
			if (iPos > 0)
			{
				sTxt = sHandshake.Mid(iPos+7);
				pcFound = sTxt.GetBuffer();
				pcFound++; // past the < the get the number
				pDoc = pGetDoc();
				if (pDoc != NULL)
				{
					pView = pViewGetView(pDoc, NULL);
					if ((pView != NULL))
					{
						if (!bEqual) pView->SendMessage(UWM_MSG_WEB_SORT, FALSE, (LPARAM) pcFound);
					}
				}
			}
			return RQ_SORT;
		break;
		case RQ_FONT:
			iPos = sHandshake.Find("</btrq>");
			if (iPos >= 0)
			{
				sTxt = sHandshake.Mid(iPos+7);
				iFontHeader = atoi(sTxt);
				iPos = sTxt.Find(",");
				if (iPos >= 0)
				{
					sTxt = sTxt.Mid(iPos+1);
					iFont = atoi(sTxt);
					theApp.m_pDlgSettingsMobile->SendMessage(UWM_MSG_DLG_SETTINGS_MOBILE, MOBILE_FONT_HEADER, iFontHeader);
					theApp.m_pDlgSettingsMobile->SendMessage(UWM_MSG_DLG_SETTINGS_MOBILE, MOBILE_FONT_TEXT, iFont);
				}
			}
			return RQ_FONT;
		break;
			case RQ_GRAPH_GETDATA:
			return RQ_GRAPH_GETDATA;
		break;
		case RQ_GRAPH_COMPUTER:
			return RQ_GRAPH_COMPUTER;
		break;
		case RQ_GRAPH_COMPUTER_SELECT:
			iPos = sHandshake.Find("</btrq>");
			if (iPos >= 0)
			{
				pDoc = pGetDoc();
				if (pDoc != NULL)
				{
					sTxt = sHandshake.Mid(iPos+7);
					if (!bEqual) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SELECT_COMPUTER, (LPARAM) sTxt.GetBuffer());					
				}
			}
			return RQ_GRAPH_COMPUTER_SELECT;
		break;
			case RQ_GRAPH_PROJECT:
			return RQ_GRAPH_PROJECT;
		break;
		case RQ_GRAPH_PROJECT_SELECT:
			iPos = sHandshake.Find("</btrq>");
			if (iPos > 0)
			{
				sTxt = sHandshake.Mid(iPos+7);
				pDoc = pGetDoc();
				if (pDoc != NULL)
				{			
					if (!bEqual) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SELECT_PROJECT, (LPARAM) sTxt.GetBuffer());
				}
			}
			return RQ_GRAPH_PROJECT_SELECT;
		break;
		case RQ_GRAPH_PROJECT_SINGLE:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{			
				if (!bEqual) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SET_PROJECT_MODE, 0);
			}
			return RQ_GRAPH_PROJECT_SINGLE;
		break;
			case RQ_GRAPH_SET_TYPE:
			iPos = sHandshake.Find("</btrq>");
			sTxt = sHandshake.Mid(iPos+7);
			if (iPos >= 0)
			{
				pDoc = pGetDoc();
				if (pDoc != NULL)
				{
					if (!bEqual) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SET_TYPE, (LPARAM) sTxt.GetBuffer());
				}
			}
			return RQ_GRAPH_SET_TYPE;
		break;
		case RQ_COMPUTER_DATA:
			return RQ_COMPUTER_DATA;
		break;
		case RQ_COMPUTER_RECONNECT:
			if (sDat.GetLength() > 0)
			{
				pDoc = pGetDoc();
				if (pDoc != NULL)
				{
					pDoc->GetRpcThreadId(sDat.GetBuffer(), &iThreadId);
					::PostThreadMessage(iThreadId,UWM_MSG_THREAD_RECONNECT_RPC,0,0);
				}
			}
			return RQ_COMPUTER_RECONNECT;
		break;
	}

	bool	bFirstSelected = true; 
	CString sSelected;
	if (m_pXml->ReadStart(&sHandshake, "btrq"))
	{
		while (m_pXml->ReadGroup("sel", true))
		{
			m_pXml->ReadItem("i",&sTxt);
			iNr = atoi(sTxt);				
			if (iNr > 0)
			{
				if (bFirstSelected)
				{
					bFirstSelected  = false;
					sFormat.Format("Select row: %d", iNr);
					sSelected = sFormat;
				}
				else
				{
					sFormat.Format(" , %d", iNr);
					sSelected+= sFormat;
				}
				m_lListSelected.push_back(iNr);
			}
		}
		if (sSelected.GetLength() > 0)
		{
			Logging(sSelected, LOGGING_DEBUG);
		}
	}
	
	if (m_lListSelected.size() > 0)
	{
		if (!bEqual) SelectRows();
		m_lListSelected.clear();
		iRequestType = RQ_TAB_DATA;
//		WriteData(RQ_TAB_DATA);
//		return CLIENT_REQUEST_NONE;
	}

	iSelectedTab = TAB_TASKS;

	if (iRequestType == RQ_TAB_DATA)
	{
		pDoc = pGetDoc();
		if (m_pXml->ReadStart(&sHandshake, "btrq"))
		{
			if (m_pXml->ReadGroup("t",true))
			{
				m_pXml->ReadItem("i",&sTxt);
				iNr = atoi(sTxt);
				if (iNr >= TAB_COMPUTER)
				{
					if (iNr <= TAB_HISTORY)
					{
						iSelectedTab = iNr;
					}
				}
			}
			if (m_pXml->ReadGroup("cl", true))
			{
				m_pXml->ReadItem("t",&sColumn);
				if (sColumn.GetLength() > 0)
				{
					if (pDoc != NULL)
					{
						CString *psColumn = new CString;
						*psColumn = sColumn;
						pDoc->m_pTaskView->SendMessage(UWM_MSG_SET_CLOUD_COLUMN, 0, (LPARAM) psColumn);
					}
				}
			}
		}

		g_bCloudServerRequestData = true;

		if (m_iCloudServerHtmlCurrentTab != iSelectedTab)
		{
			m_iCloudServerHtmlCurrentTab = iSelectedTab;
			if (pDoc != NULL)
			{
				posView = pDoc->GetFirstViewPosition();
				while (1)
				{
					pView = pDoc->GetNextView(posView);
					if (pView == NULL) break;
					pView->SendMessage(UWM_MSG_TAB_VIEW_SELECTED, m_iCloudServerHtmlCurrentTab, 1);
				}
			}
			return RQ_TAB_DATA;
		}
		if (pDoc != NULL)
		{
			posView = pDoc->GetFirstViewPosition();
			while (1)
			{
				pView = pDoc->GetNextView(posView);
				if (pView == NULL) break;
				pView->SendMessage(UWM_MSG_VIEW_REFRESH, 0, 1);		// last parameter 1 make sure the history gets updated
				sFormat = "Send update view";
				Logging(sFormat, LOGGING_DEBUG);
			}
		}
		return RQ_TAB_DATA;
	}

	return CLIENT_REQUEST_NONE;
}

void CThreadCloudServer::SelectRows()
{
	CView				*pView;
	CRemoteCombinedDoc	*pDoc;
	POSITION			posView;
	CHtmlExchange htmlExchange;
	CString				sTxt;
	int					iPos, iSelect;

	htmlExchange.m_pcHtml = NULL;

	pDoc = pGetDoc();
	if (pDoc == NULL) return;

	iPos = (int) m_lListSelected.size();
	iPos--;

	while (iPos >= 0)
	{
		posView = pDoc->GetFirstViewPosition();
		while (1)
		{
			iSelect = m_lListSelected.at(iPos);
			iSelect-= 2;
			pView = pDoc->GetNextView(posView);
			if (pView == NULL) break;
			int	iMultiple; 
			if (m_bWebServerSelectMultiple) iMultiple = CLOUD_SELECT_MULTIPLE;
			else iMultiple = CLOUD_SELECT_SINGLE;
			for (int iRetry = 0; iRetry < 4; iRetry++)
			{
				htmlExchange.m_iOption = iMultiple;
				htmlExchange.m_iRow = iSelect;
				htmlExchange.m_pcHtml = NULL;
				htmlExchange.tTime = 0;
				if (iPos == 0)
				{
					htmlExchange.m_bSendData = true;
				}
				else
				{
					htmlExchange.m_bSendData = false;
				}
				pView->SendMessage(UWM_MSG_WEB_ITEM_SELECTED, 0, (LPARAM) &htmlExchange);
				if (htmlExchange.m_bSendData)
				{
					if (htmlExchange.m_pcHtml != NULL)
					{
//						g_iCloudServerHtmlTabSend[m_iCloudServerHtmlCurrentTab] = FALSE;
						if (g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab] != NULL)
						{
							delete g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab];
						}
						g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab] = htmlExchange.m_pcHtml;
						WriteData(RQ_TAB_DATA);
						return;
					}
					else
					{
						// oeps
						int ii = 1;
					}
				}
				else
				{
					break;
				}
				Sleep(200);
			}
		}
		iPos--;
	}
}

int CThreadCloudServer::WriteData(int iClientRequest, CString *psData)
{

	CRemoteCombinedDoc	*pDoc;
	CView				*pView;
	std::string			sBase64, sJson;
	CString				sFormat, *psBtrq, sFilter;
	int					iTimeout, iSingle;
	BOOL				bResult;
	CreateHtmlWebServerGraph createHtmlWebServerGraph;
	CreateHtmlWebServerGraphProjectSelect createHtmlWebServerGraphProjectSelect;
	CComputerGraphicType *pwebType;

	CString version = CLOUD_VERSION;
	psBtrq = new CString;

	switch (iClientRequest)
	{
		case RQ_ERROR:
			*psBtrq = "<btrq>";
			*psBtrq+= "<error>1</error>";
			*psBtrq+= "</btrq>";
			if (psData != NULL) *psBtrq = *psBtrq + *psData;
		break;
		case RQ_TIMEOUT:
			*psBtrq = "<btrq>";
			*psBtrq+= "<timeout>1</timeout>";
			*psBtrq+= "</btrq>";
			if (psData != NULL) *psBtrq = *psBtrq + *psData;
		break;

		case RQ_LOGIN:
			*psBtrq = "<btrq>";
			sFormat.Format("%d", m_iIdClient);
			*psBtrq+= "<i>" +  sFormat + "</i>";
			sFormat.LoadString(IDS_PROGRAM_VERSION);
			*psBtrq+= "<btver>" + sFormat + "</btver>";

//			iPos = (int) theApp.m_pDlgSettingsMobile->SendMessage(UWM_MSG_DLG_SETTINGS_MOBILE, MOBILE_FONT_HEADER, -1);
//			sFormat.Format("%d", iPos);
//			*psBtrq+= "<fh>" +  sFormat + "</fh>";
//			iPos = (int) theApp.m_pDlgSettingsMobile->SendMessage(UWM_MSG_DLG_SETTINGS_MOBILE, MOBILE_FONT_TEXT, -1);
//			sFormat.Format("%d", iPos);
//			*psBtrq+= "<f>" +  sFormat + "</f>";
		
			sFormat.Format("%d", m_bWebServerSelectMultiple);
			*psBtrq+= "<ms>" + sFormat + "</ms>";

			*psBtrq+= "</btrq>";
		break;

		case RQ_COMPUTER_DIALOG:
		case RQ_COMPUTER_SELECT:
			*psBtrq = UploadId(psData);
		break;
		case RQ_SELECT_MULTIPLE:
		case RQ_PROJECT_OPERATION:
		case RQ_TRANSFER_OPERATION:
		case RQ_TASK_OPERATION:
		case RQ_REPORT:
		case RQ_FONT:
		case RQ_SORT:
			*psBtrq = UploadId(psData);
		break;

		case RQ_FILTER_GET:
		case RQ_FILTER_SET:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
				if (pView != NULL)
				{
					CreateHtmlWebServerFilter webServerFilter;
					sFilter = webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterReadyTR], FILTER_BIT_POS_READY_TO_REPORT, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_READY_TO_REPORT));
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterWaitingTR], FILTER_BIT_POS_WAITING_TO_RUN, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_WAITING_TO_RUN));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterReadyTS], FILTER_BIT_POS_READY_TO_START, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_READY_TO_START));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterDownload], FILTER_BIT_POS_DOWNLOAD, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_DOWNLOAD));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterUpload], FILTER_BIT_POS_UPLOAD, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_UPLOAD));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterSuspended], FILTER_BIT_POS_SUSPEND, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_SUSPEND));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterComputation], FILTER_BIT_POS_COMPUTATION, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_COMPUTATION));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterAborted], FILTER_BIT_POS_ABORTED, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_ABORTED));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksFilterRunning], FILTER_BIT_POS_RUNNING, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_RUNNING));	
					sFilter+=webServerFilter.Create(gszTranslation[PosGroupPopUpTasksShowActive], FILTER_BIT_POS_SHOW_RUNNING_TASKS, (int) pView->SendMessage(UWM_MSG_WEB_FILTER, FALSE, FILTER_BIT_POS_SHOW_RUNNING_TASKS));	
				}
			}
			*psBtrq = UploadId(&sFilter);
		break;
		case RQ_PROJECT_PROPERTIES:
		case RQ_TASK_PROPERTIES:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				*psBtrq = "Nothing selected.";
				bResult = FALSE;
				if (iClientRequest == RQ_PROJECT_PROPERTIES)
				{
					pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedProjectView));
				}
				else
				{
					pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
				}
				if (pView != NULL)
				{
					bResult = (BOOL) pView->SendMessage(UWM_MSG_WEB_PROPERTIES,0,0);
				}
				if (bResult == TRUE)
				{
					iTimeout = 0;
					while (iTimeout++ < 10)
					{
						if (g_pcWebServerProperties)
						{
							*psBtrq = g_pcWebServerProperties;
							delete g_pcWebServerProperties;
							g_pcWebServerProperties = NULL;
							break;
						}
						Sleep(500);
					}
				}
			}
			*psBtrq = UploadId(psBtrq);
		break;

		case RQ_GRAPH_GETDATA:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				createHtmlWebServerGraph.Json(pDoc, &sJson);
				*psBtrq = sJson.c_str();
			}
			else
			{
				// oeps
				int ii = 1;
			}
			*psBtrq = UploadId(psBtrq);
		break;

		case RQ_GRAPH_COMPUTER:
		case RQ_GRAPH_COMPUTER_SELECT:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				CreateHtmlWebServerGraphComputerSelect createHtmlWebServerGraphComputerSelect;
				createHtmlWebServerGraphComputerSelect.Create(pDoc, psBtrq);
			}
			*psBtrq = UploadId(psBtrq);
		break;

		case RQ_GRAPH_PROJECT:
		case RQ_GRAPH_PROJECT_SELECT:
		case RQ_GRAPH_PROJECT_SINGLE:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				iSingle = (int) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_PROJECT_MODE, 0);
				createHtmlWebServerGraphProjectSelect.Create(pDoc, psBtrq, (iSingle == TRUE));
			}
			*psBtrq = UploadId(psBtrq);
		break;

		case RQ_GRAPH_SET_TYPE:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				pwebType = (CComputerGraphicType *) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_TYPE, 0);
				*psBtrq = createHtmlWebServerGraph.CreateTypeData(pwebType);
			}
			*psBtrq = UploadId(psBtrq);
		break;

		case RQ_COMPUTER_DATA:
		case RQ_COMPUTER_RECONNECT:
			pDoc = pGetDoc();
			if (pDoc != NULL)
			{
				pView = pDoc->m_pHistoryView;
				if (pView != NULL)
				{
					bResult = (BOOL) pView->SendMessage(UWM_MSG_GET_CLOUD_DATA,this->m_nThreadID, (LPARAM) psBtrq);
					if (bResult == FALSE)
					{
						*psBtrq = UploadId(psBtrq);	// no data but report back.
					}
				}
			}
		break;

		default:
			CheckRightTab();
			if (g_iCloudServerHtmlTabProcessed[m_iCloudServerHtmlCurrentTab] != FALSE)
			{
				return STATUS_WRITE_DATA_PROCESSED;
			}
			if (g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab] == NULL)
			{
				return STATUS_WRITE_DATA_TAB_NULL;
			}

			g_bCloudServerRequestData = false;
//			if (g_iCloudServerHtmlTabProcessed[m_iCloudServerHtmlCurrentTab] == FALSE)
//			{
//				if (g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab] != NULL)
//				{

			sFormat.Format("%d", m_iIdClient);
			*psBtrq = "<btrq>";
			*psBtrq+= "<i>" +  sFormat + "</i>";
			sFormat.Format("%d", m_iCloudServerHtmlCurrentTab);			
			*psBtrq+= "<tab>" +  sFormat + "</tab>";
			if (m_iCloudServerHtmlCurrentTab == TAB_TASKS)
			{
				sFormat.Format("%d", m_iWuTotal);
				*psBtrq+= "<wut>" + sFormat + "</wut>";					
			}
			sFormat.Format("%d", theApp.m_pMainFrame->m_iHistoryStateReadyCount);
			*psBtrq+= "<rtr>" + sFormat + "</rtr>";	
			*psBtrq+= "</btrq>";
			*psBtrq = *psBtrq + g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab];
			delete g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab];
			g_pcCloudServerHtmlTab[m_iCloudServerHtmlCurrentTab] = NULL;
			g_iCloudServerHtmlTabProcessed[m_iCloudServerHtmlCurrentTab] = TRUE;
	}
	sFormat.Format("Write type: %d, ID: %d",iClientRequest, m_iIdClient);
	Logging(sFormat, LOGGING_DEBUG);

	m_pThreadCloudConnect->PostThreadMessage(UWM_MSG_THREAD_CLOUD_WRITE, iClientRequest, (LPARAM) psBtrq);
	return STATUS_WRITE_DATA_OK;
}

void CThreadCloudServer::CheckRightTab()
{
	int					iCurrentTab, iViewActive;

	CRemoteCombinedDoc	*pDoc;
	CView				*pView;
	POSITION			posView;

	pDoc = pGetDoc();
	if (pDoc == NULL) return;
	
	iViewActive = pDoc->m_iViewActive;
	switch(iViewActive)
	{
		case TASK_VIEW_ACTIVE:
			iCurrentTab = TAB_TASKS;
		break;
		case MESSAGE_VIEW_ACTIVE:
			iCurrentTab = TAB_MESSAGES;
		break;
		case PROJECT_VIEW_ACTIVE:
			iCurrentTab = TAB_PROJECTS;
		break;
		case TRANSFER_VIEW_ACTIVE:
			iCurrentTab = TAB_TRANSFERS;
		break;
		case HISTORY_VIEW_ACTIVE:
			iCurrentTab = TAB_HISTORY;
		break;
		case COMPUTER_VIEW_ACTIVE:
			iCurrentTab = TAB_COMPUTER;
		break;
		default:
			iCurrentTab = -1;
	}

	if (m_iCloudServerHtmlCurrentTab != iCurrentTab)
	{
		posView = pDoc->GetFirstViewPosition();
		while (1)
		{
			pView = pDoc->GetNextView(posView);
			if (pView == NULL) break;
			pView->SendMessage(UWM_MSG_TAB_VIEW_SELECTED, m_iCloudServerHtmlCurrentTab, 1);
		}		
	}

}

void CThreadCloudServer::GetComputerTree(std::string *psData)
{
	CString sData;
	CCreateHtmlWebServerComputerSelect createHtmlWebServerComputerSelect;
	std::deque<CBarTreeGroupItem*> *plListBarTree;
	plListBarTree = (std::deque<CBarTreeGroupItem*> *) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_GET_TREE, 0, 0);
	createHtmlWebServerComputerSelect.Create(plListBarTree, &sData);
	*psData = sData;
}

CString CThreadCloudServer::ExtractXmlTag(char **pcFind)
{
	char *pcFoundBegin, *pcFoundEnd;

	pcFoundBegin = (char *) strstr(*pcFind, "<");
	if (pcFoundBegin)
	{
		pcFoundBegin++;
		pcFoundEnd = (char *) strstr(pcFoundBegin, ">");
		if (pcFoundEnd)
		{
			*pcFind = pcFoundEnd + 1;
			int iLen = (int) (pcFoundEnd - pcFoundBegin);
			CString sFound;
			sFound = pcFoundBegin;
			sFound = sFound.Mid(0,iLen);
			return sFound;
		}
	}
	return NULL;
}


CView *CThreadCloudServer::pViewGetView(CDocument *pDoc, CRuntimeClass *pClass)
{
	CView		*pView;
	POSITION	posView;
	posView = pDoc->GetFirstViewPosition();
	while (posView)
	{
		pView = pDoc->GetNextView(posView);
		if (pView != NULL)
		{		
			if (pClass != NULL)
			{
				if (pView->IsKindOf(pClass)) return pView;
			}
			else
			{
				 return pView;
			}
		}
	}

	return NULL;
}

CRemoteCombinedDoc	*CThreadCloudServer::pGetDoc()
{
	CRemoteCombinedDoc	*pDoc;
	POSITION	posDoc;
	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
	}
	return pDoc;
}

CString CThreadCloudServer::UploadId(CString *psData)
{
	CString sFormat, sBtrq;
	sFormat.Format("%d", m_iIdClient);
	sBtrq = "<btrq>";
	sBtrq+= "<i>" +  sFormat + "</i>";
	sBtrq+= "</btrq>";
	if (psData != NULL) sBtrq = sBtrq + *psData;
//	sFormat = m_pHtmlConnect->PostData("upload_bt.php", &sBtrq, m_sLoginID);
	return sBtrq;
}

void  CThreadCloudServer::Logging(CString sTxt, int iMode, bool bAlways)
{
	if (bAlways || m_bLogDebugCloud)
	{
		CString sFrom = CLOUDSERVER_LOG_NAME;
		CString sFormat;
		CLoggingMessage sLog;
		sFormat.Format(" (thread Cloud Server: %d)", m_nThreadID);
		sFrom += sFormat;
		sLog.m_pFrom = sFrom.GetBuffer();
		sLog.m_pText = sTxt.GetBuffer();
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, iMode);
		sTxt.ReleaseBuffer();
		sFrom.ReleaseBuffer();
	}
}


BEGIN_MESSAGE_MAP(CThreadCloudServer, CWinThread)
END_MESSAGE_MAP()


// CThreadWebServer message handlers

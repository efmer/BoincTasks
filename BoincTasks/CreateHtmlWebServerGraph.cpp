// CreateHtmlWebServerGraph.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadRpc.h"
#include "RemoteCombinedDoc.h"
#include "DlgGraphic.h"
#include "CreateHtmlWebServerButton.h"
#include "CreateHtmlWebServerGraph.h"
#include "Translation.h"
#include "AnsiToUtf8.h"
#include "DlgLogging.h"

// CreateHtmlWebServerGraph

CreateHtmlWebServerGraph::CreateHtmlWebServerGraph()
{
}

CreateHtmlWebServerGraph::~CreateHtmlWebServerGraph()
{
}


// CreateHtmlWebServerGraph member functions

bool CreateHtmlWebServerGraph::Create(std::string *psHtml)
{
	CFile				file;
	int					iReadCount;
	CStringA			sFile, sFileTemplate, sFileNew;
	CLoggingMessage		log;
	char				cBuffer[MAX_PATH+32], *pcFound;
	BYTE				bBuffer[50001];
	char				cBufferLogging[129];
	CStringA			sPath, sFormat;
//	CAnsiToUtf8			ansiToUtf8;
//	std::deque<CComputerGraphicListItem*>	*plComputer;

	bool				bAddProject = false;

	DWORD dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\graph.html");

	if (file.Open(cBuffer, CFile::modeRead))
	{
		iReadCount = file.Read(bBuffer, 50000);
		if (iReadCount > 0)
		{	
			bBuffer[iReadCount] = 0;
			*psHtml = (char*) bBuffer;
		}
		file.Close();
	}
	else
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to find: %s", cBuffer);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	sFormat.LoadString(IDS_PROGRAM_VERSION);
	sFormat = "BoincTasks " + sFormat;
//	psHtml->Replace("__InsertBoincTasks__",sFormat);
	StringReplace(psHtml, "__InsertBoincTasks__", sFormat.GetBuffer());

// testing only

/*
	dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\testing_graph.html");

	CFileException e;
	if( !file.Open( cBuffer, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		return false;
	}

	file.Write(psHtml->GetBuffer(),psHtml->GetLength());
	file.Close();
*/

	return true;
}

// validate on: http://jsonlint.com/

bool CreateHtmlWebServerGraph::Json(CRemoteCombinedDoc	*pDoc, std::string *psJson)
{
	bool									bFirst, bAddProject = false, bFound;
	int										iComputerSelected = 0;
	std::deque<CComputerGraphicListItem*>	*plComputer;
	std::deque<CSelectedItem*>				*plProjectSelect;
	std::deque<CSelectedItem*>				*plComputerSelect;
	CComputerGraphicType					*pWebType;
	PROJECT									*pProject;
	CString									sData, sComputerName, sProjectName, sGroup, sFormat;
	int										iSequenceNr;

	*psJson = "";
	pWebType = (CComputerGraphicType *) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_TYPE, 0);
	plComputerSelect= (std::deque<CSelectedItem*>*) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_COMPUTER, 0);
	plProjectSelect = (std::deque<CSelectedItem*>*) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_PROJECT, 0);
	plComputer = (std::deque<CComputerGraphicListItem*> *) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET,GRAPHIC_GET_LIST,0);

	iSequenceNr = 0;
//	sData += "{";
	bFirst = true;
	for (int iComputer = 0; iComputer < (int) plComputer->size(); iComputer++)
	{
		if (plComputer->at(iComputer)->m_pcThreadRpcId != NULL)
		{
			bFound = false;
			sComputerName = plComputer->at(iComputer)->m_pcThreadRpcId;
			iComputerSelected = 0;
			for (int iComputerSelect = 0; iComputerSelect < (int) plComputerSelect->size(); iComputerSelect++)
			{
				if (plComputerSelect->at(iComputerSelect)->m_bSelected)
				{
					iComputerSelected++;
					if (sComputerName == plComputerSelect->at(iComputerSelect)->m_sName)
					{
						bFound = true;
					}
				}
			}
			if (!bFound) continue;

			//bFirst = true;
			for (int iProject = 0; iProject < MAX_PROJECTS; iProject++)
			{
				pProject = plComputer->at(iComputer)->m_pProjects[iProject];
				if (pProject == NULL) break;
				sProjectName = plComputer->at(iComputer)->m_sThreadRpcName[iProject];

				bFound = false;
				for (int iProjectSelect = 0; iProjectSelect < (int) plProjectSelect->size(); iProjectSelect++)
				{
					if (sProjectName == plProjectSelect->at(iProjectSelect)->m_sName)
					{
						if (plProjectSelect->at(iProjectSelect)->m_bSelected)
						{
							bFound = true;
							break;
						}
					}
				}
				if (!bFound) continue;
				if (bFirst)
				{
					sData += "{";
					bFirst = false;
				}
				else sData += ",";
				sFormat.Format("\x22%d\x22:", iSequenceNr);// iProject);
				iSequenceNr++;
				sData += sFormat;
				if (iComputerSelected > 1) sProjectName = "[" + sComputerName + "]";
				else sProjectName = "";
				sProjectName+= plComputer->at(iComputer)->m_sThreadRpcName[iProject];

				AddProject(&sGroup, &sProjectName, &pProject->statistics, pWebType);
				sData += sGroup;
				bAddProject = true;
			//	break;
			}
		}
	}

	if (bAddProject)
	{
		sData += "}";
	}

	(*psJson) += sData;

// testing only
/*

	CFile	file;
	DWORD	dwSize;
	char	cBuffer[MAX_PATH+1];
	char	*pcFound;

	dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\testing_graph.json");

	CFileException e;
	if( !file.Open( cBuffer, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		return false;
	}

	int iCount = (int) psJson->length();
	file.Write(psJson->c_str(),iCount);
	file.Close();
*/

	return true;
}

void CreateHtmlWebServerGraph::AddProject(CString *psGroup, CString *psProjectName, std::vector<DAILY_STATS> *pStatistics, CComputerGraphicType *pWebType)
{
	int		iSize;
	double	dDay, dCredits;
	CString sFormat;
	bool	bFirst;

	iSize = (int) pStatistics->size();
	if (iSize > 1)
	{
		*psGroup =		"[{\x22name\x22 :\x22__NAME__\x22 ,\x22";
		(*psGroup)+=	"data\x22 :[";
		psGroup->Replace("__NAME__",*psProjectName);
		bFirst = true;
		for (int iCount = 0; iCount < iSize; iCount++)
		{
			if (bFirst)	bFirst = false;
			else (*psGroup)+= ",";

			dDay = pStatistics->at(iCount).day;

			if (pWebType->m_iHostOrUser == GRAPHIC_HOST)
			{
				if (pWebType->m_iTotalOrAverage == GRAPHIC_TOTAL) dCredits = pStatistics->at(iCount).host_total_credit;
				else dCredits = pStatistics->at(iCount).host_expavg_credit;
			}
			else
			{
				if (pWebType->m_iTotalOrAverage == GRAPHIC_TOTAL) dCredits = pStatistics->at(iCount).user_total_credit;
				else dCredits = pStatistics->at(iCount).user_expavg_credit;
			}
			
			//[Date.UTC(1970,  9, 27), 0   ]
			// data
			CTime ctime((time_t) dDay);
			sFormat = ctime.Format("\x22%Y,%m,%d,");
			*(psGroup) += sFormat;
			// value
			sFormat.Format("%.2f\x22", dCredits);
			(*psGroup) += sFormat;
		}
		(*psGroup)+= "]}]";
	}
	else
	{
		*psGroup = "[]";
	}
}

CString CreateHtmlWebServerGraph::CreateTypeData(CComputerGraphicType *pwebType)
{
	int iUser, iHost, iTotal, iAverage;
	CString sData;
//	CreateHtmlWebServerButton createHtmlWebServerButton;
	CAnsiToUtf8			ansiToUtf8;

	if (pwebType->m_iHostOrUser == GRAPHIC_HOST)
	{
		iHost = 1;
		iUser = 0;
	}
	else
	{
		iHost = 0;
		iUser = 1;
	}
	if (pwebType->m_iTotalOrAverage == GRAPHIC_TOTAL)
	{
		iTotal = 1;
		iAverage = 0;
	}
	else
	{
		iTotal = 0;
		iAverage = 1;
	}

	sData.Format("%d?%d?%d?%d", iUser, iHost, iTotal, iAverage);


//	sButton+= createHtmlWebServerButton.Create(ansiToUtf8.ToUtf8(gszTranslation[PosDialogGraphicRadioUser]).GetBuffer(),GRAPHIC_TYPE_USER,"buttonType(\x22__ActionName__\x22)", "b", bUser);
//	sButton+= createHtmlWebServerButton.Create(ansiToUtf8.ToUtf8(gszTranslation[PosDialogGraphicRadioComputer]).GetBuffer(),GRAPHIC_TYPE_COMPUTER,"buttonType(\x22__ActionName__\x22)", "b", bHost);
//	sButton+= createHtmlWebServerButton.Create(ansiToUtf8.ToUtf8(gszTranslation[PosDialogGraphicRadioTotal]).GetBuffer(),GRAPHIC_TYPE_TOTAL,"buttonType(\x22__ActionName__\x22)", "b", bTotal);
//	sButton+= createHtmlWebServerButton.Create(ansiToUtf8.ToUtf8(gszTranslation[PosDialogGraphicRadioAverage]).GetBuffer(),GRAPHIC_TYPE_AVERATE,"buttonType(\x22__ActionName__\x22)", "b", bAverage);
	return sData;
}

/*
bool CreateHtmlWebServerGraph::Create(CString *psHtml)
{
	CFile				file;
	int					iReadCount;
	CStringA			sFile, sFileTemplate, sFileNew;
	CLoggingMessage		log;
	char				cBuffer[MAX_PATH+32], *pcFound;
	BYTE				bBuffer[50001];
	char				cBufferLogging[129];
	CStringA			sPath, sFormat;
	CAnsiToUtf8			AnsiToUtf8;
	CRemoteCombinedDoc	*pDoc;
	POSITION			posDoc;
	std::deque<CComputerGraphicListItem*>	*plComputer;
	PROJECT				*pProject;
	bool				bFirst, bAddProject = false;

	DWORD dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\graph.html");

	if (file.Open(cBuffer, CFile::modeRead))
	{
		iReadCount = file.Read(bBuffer, 50000);
		if (iReadCount > 0)
		{	
			bBuffer[iReadCount] = 0;
			*psHtml = bBuffer;
		}
		file.Close();
	}
	else
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to find: %s", cBuffer);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	CString sData, sGroup;
	CString sProjectName;
	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
	}
	if (pDoc == NULL) return false;
	plComputer = (std::deque<CComputerGraphicListItem*> *) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET,GRAPHIC_GET_LIST,0);
	int iComputer = 1;
	if (plComputer->at(iComputer)->m_pcThreadRpcId != NULL)
	{
		bFirst = true;
		for (int iCount = 0; iCount < MAX_PROJECTS; iCount++)
		{
			pProject = plComputer->at(iComputer)->m_pProjects[iCount];
			if (pProject == NULL) break;
			if (bFirst)	bFirst = false;
			else sData += ",";
			sProjectName = plComputer->at(iComputer)->m_sThreadRpcName[iCount];
			AddProject(&sGroup, &sProjectName, &pProject->statistics);
			sData += sGroup;
			bAddProject = true;
		}
	}

	if (bAddProject)
	{
		sData += GRAPH_DATA_GROUP_END_ALL;
	}
	psHtml->Replace("__series__", sData);

	sFormat.LoadString(IDS_PROGRAM_VERSION);
	sFormat = "BoincTasks " + sFormat;
	psHtml->Replace("__InsertBoincTasks__",sFormat);

// testing only

	dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\testing_graph.html");

	CFileException e;
	if( !file.Open( cBuffer, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		return false;
	}

	file.Write(psHtml->GetBuffer(),psHtml->GetLength());
	file.Close();

	return true;
}

void CreateHtmlWebServerGraph::MakeGroup(CString *psGroup, CString *psProjectName, CString sItem1, CString sItem2,CString sItem3)
{
	*psGroup = GRAPH_DATA_GROUP_BEGIN;
	psGroup->Replace("__NAME__",*psProjectName);

	(*psGroup) += sItem1;
	(*psGroup)+= ",";
	(*psGroup) += sItem2;
	(*psGroup)+= ",";
	(*psGroup) += sItem3;
	(*psGroup)+= GRAPH_DATA_GROUP_END;
}

void CreateHtmlWebServerGraph::AddGroup(CString *psHtml, CString *psData, int iItemType)
{

	if (iItemType != GRAPH_ITEM_FIRST)
	{
		(*psData) += ",";
	}

	if (iItemType != GRAPH_ITEM_LAST) 
	{
		(*psData) += "__series__";
	}
	else
	{
		(*psData) += "]";
	}

	psHtml->Replace("__series__", *psData);
}

void CreateHtmlWebServerGraph::AddProject(CString *psGroup, CString *psProjectName, std::vector<DAILY_STATS> *pStatistics)
{
	int		iSize;
	double	dDay, dCredits;
	CString sFormat;
	bool	bFirst;

	iSize = (int) pStatistics->size();
	if (iSize > 1)
	{
		*psGroup = GRAPH_DATA_GROUP_BEGIN;
		psGroup->Replace("__NAME__",*psProjectName);
		bFirst = true;
		for (int iCount = 0; iCount < iSize; iCount++)
		{
			if (bFirst)	bFirst = false;
			else (*psGroup)+= ",";

			dDay = pStatistics->at(iCount).day;
			dCredits = pStatistics->at(iCount).host_expavg_credit;
			//[Date.UTC(1970,  9, 27), 0   ]
			// data
			CTime ctime((time_t) dDay);
			sFormat = ctime.Format("[Date.UTC(%Y,%m, %d),");
			*(psGroup) += sFormat;
			// value
			sFormat.Format("%.2f]",dCredits);
			(*psGroup) += sFormat;
		}
		(*psGroup)+= "]}";
	}
}
*/
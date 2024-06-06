// CreateHtml.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include "BoincTasks.h"
#include "CreateHtmlWebServerComputerSelect.h"
//#include "listviewex.h"
//#include "DlgColorGeneral.h"
//#include "DlgSettingsExpert.h"
//#include "DlgSettingsWWW.h"
#include "DlgBarComputer.h"
#include "DateConvert.h"
#include "Translation.h"
#include "AnsiToUtf8.h"

#include "MainFrm.h"

// CCreateHtmlComputerSelect

CCreateHtmlWebServerComputerSelect::CCreateHtmlWebServerComputerSelect()
{
}

CCreateHtmlWebServerComputerSelect::~CCreateHtmlWebServerComputerSelect()
{
}

void CCreateHtmlWebServerComputerSelect::Create(std::deque<CBarTreeGroupItem*> *pBarTreeGroupItem,  CString *psData)
{
	CFile				file;
	int					iPosBegin, iPosEnd, iReadCount;
	CStringA			sFile, sFileNew, sInsertCollapsableTemplate, sInsertCheckboxTemplate, sItemText, sFind, sRefreshrate;
	CLoggingMessage		log;
	char				cBuffer[MAX_PATH+32], *pcFound;
	BYTE				bBuffer[50001];
	char				cBufferLogging[129];
	CStringA			sPath, sFormat;
	CString				sInsertCheckboxTemplateBegin, sInsertCheckboxTemplateEnd;
	CAnsiToUtf8			ansiToUtf8;


	bool bWebSingleSelection = theApp.m_pMainFrame->m_dlgBarComputer.m_bWebSingleSelection;

	char	*pcInsertCollapsableTemplateBegin = "__InsertCollapsableTemplateBegin__";
	char	*pcInsertCollapsableTemplateEnd = "__InsertCollapsableTemplateEnd__";

	sInsertCheckboxTemplateBegin = "__InsertButtonTemplateBegin__";
	sInsertCheckboxTemplateEnd = "__InsertButtonTemplateEnd__";

	char	*pcInsertCollapsable = "__insertCollapsable__";
	char	*pcInsertCheckbox = "__insertCheckbox";


	DWORD dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\select_computers_template.html");

	if (file.Open(cBuffer, CFile::modeRead))
	{
		iReadCount = file.Read(bBuffer, 50000);
		if (iReadCount > 0)
		{	
			bBuffer[iReadCount] = 0;
			*psData = (char *) bBuffer;
		}
		file.Close();
	}
	else
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html select computers create";
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to find: %s", sFile);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return;
	}

	bool bFoundTemplate;
	bFoundTemplate = false;
	iPosBegin = psData->Find(pcInsertCollapsableTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertCollapsableTemplateBegin);
		iPosEnd = psData->Find(pcInsertCollapsableTemplateEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertCollapsableTemplate = psData->Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			log.m_pText = "ERROR: __InsertCollapsableTemplateBegin(End)__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return;
	}


	bFoundTemplate = false;
	iPosBegin = psData->Find(sInsertCheckboxTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(sInsertCheckboxTemplateBegin);
		iPosEnd = psData->Find(sInsertCheckboxTemplateEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertCheckboxTemplate = psData->Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			log.m_pText = "ERROR: __InsertxxTemplateBegin(End)__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return;
	}


	CString sInsert;
	for (int iCountGroup = 0; iCountGroup < (int) pBarTreeGroupItem->size(); iCountGroup++)
	{
		CString sGroup, sTxt;
		sInsert += sInsertCollapsableTemplate;
		sGroup = "<" + pBarTreeGroupItem->at(iCountGroup)->m_sGroup + ">";
		bool bGroupSelected = pBarTreeGroupItem->at(iCountGroup)->m_bSelected;
		sInsert.Replace("__computerId__", sGroup);
		sInsert.Replace("__computerName__", pBarTreeGroupItem->at(iCountGroup)->m_sGroup);
		if (pBarTreeGroupItem->at(iCountGroup)->m_bExpand) sTxt = "data-collapsed='false'";
		else sTxt = "";
		sInsert.Replace("__collapsed__",sTxt);

		for (int iCountComputer = 0; iCountComputer < (int) pBarTreeGroupItem->at(iCountGroup)->m_lList.size(); iCountComputer++)
		{
			sInsert += sInsertCheckboxTemplate;
			sInsert.Replace("__computerGroup__", pBarTreeGroupItem->at(iCountGroup)->m_sGroup);
			sTxt = "<" + pBarTreeGroupItem->at(iCountGroup)->m_lList.at(iCountComputer)->m_sComputer + ">";
			sInsert.Replace("__computerName__", sTxt);

			sTxt = pBarTreeGroupItem->at(iCountGroup)->m_lList.at(iCountComputer)->m_sComputer;
			int iImage = pBarTreeGroupItem->at(iCountGroup)->m_lList.at(iCountComputer)->m_iImage ;

			if (iImage == 2) sTxt += " - " + ansiToUtf8.ToUtf8(gszTranslation[PosWindowStatusMessageCombinedNotConnected]);
			if (iImage == 3) sTxt += " - " + ansiToUtf8.ToUtf8(gszTranslation[PosWindowStatusMessageCombinedLostConnection]);

			sInsert.Replace("__computerText__", sTxt);

			CString sChecked, sUnchecked;

			sUnchecked = "checkbox-off";
			sChecked = "check";

			if (bGroupSelected)  sTxt = sChecked;
			else
			{
				int iState = pBarTreeGroupItem->at(iCountGroup)->m_lList.at(iCountComputer)->m_iState;
				if (iState == LVIS_SELECTED) sTxt = sChecked;
				else sTxt = sUnchecked;
			}
			sInsert.Replace("__checked__" , sTxt);

			if (pBarTreeGroupItem->at(iCountGroup)->m_lList.at(iCountComputer)->m_iImage == 1) sTxt = 	sTxt = "data-theme='b'";
			else sTxt = "data-theme='c'";
			sInsert.Replace("__theme__", sTxt);
			
		}
		sInsert += "</div>";
	}

	psData->Replace("__insert__", sInsert);
	
	if (bWebSingleSelection) psData->Replace("__ss_checked__", "check");
	psData->Replace("__ss_checked__", "checkbox-off");

	psData->Replace("_T_SingleSelection_", ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileSingleSelection]));


// testing only

/*
	dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\testing_computer_select.html");

	CFileException e;
	if( !file.Open( cBuffer, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		return;
	}

	file.Write(psData->GetBuffer(),psData->GetLength());
	file.Close();
*/
}





// CCreateHtmlComputerSelect member functions

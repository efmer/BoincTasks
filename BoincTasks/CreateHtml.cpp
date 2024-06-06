// CreateHtml.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
//#include <stdlib.h>
//#include <stdio.h>
#include "BoincTasks.h"
#include "CreateHtml.h"
#include "listviewex.h"
#include "TemplateRemoteTaskView.h"
#include "RemoteCombinedTaskView.h"
#include "Xml.h"
#include "DlgSettingsExpert.h"
#include "DlgSettingsWWW.h"
#include "DateConvert.h"
#include "Translation.h"
#include "AnsiToUtf8.h"

// CCreateHtml

CCreateHtml::CCreateHtml()
{
}

CCreateHtml::~CCreateHtml()
{
}

bool CCreateHtml::CreateMultiple(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1])
{
	CString sDirectory, sFile, sFileDest, sFormat;
	CXml	xml("");

	sDirectory = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectory += "\\html\\";
	_mkdir(sDirectory);
	sFile = sDirectory + "tasks_template.html";
	sFileDest = sDirectory + "tasks.html";
	Create(&sFile, &sFileDest, pRemoteTaskView, pListCtrl, &iColumnOrder[0]);

	for (int iPos = 0; iPos < 10; iPos++)
	{
		sFormat.Format("tasks_template%d.html", iPos);
		sFile = sDirectory + sFormat;
		if (_access(sFile, 0)  != -1)
		{
			sFormat.Format("tasks%d.html", iPos);
			sFileDest = sDirectory + sFormat;
			Create(&sFile, &sFileDest, pRemoteTaskView, pListCtrl, &iColumnOrder[0]);
		}
	}
	return true;
}

bool CCreateHtml::Create(CString *psFile, CString *psDest, CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1])
{
	int		iReadCount, iPosBegin, iPosEnd, iItemCount;
	CFile	file;
	BYTE	bBuffer[50001];
	CString sFileTemplate, sFileNew, sInsertHeaderTemplate, sInsertSubHeaderTemplate, sInsertRowTemplate, sInsertRow, sItemtext, sFind, sRefreshrate;
	char				cBufferLogging[129];
	CLoggingMessage		log;
	CAnsiToUtf8	ansiToUtf8;
	bool	bInsertHeader;

	char	*pcInsertRowTemplateBegin = "__InsertRowTemplateBegin__";
	char	*pcInsertHeaderTemplateBegin = "__InsertHeaderTemplateBegin__";
	char	*pcInsertSubHeaderTemplateBegin = "__InsertSubHeaderTemplateBegin__";
	char	*pcInsertSubHeaderOnColumn = "__InsertSubHeaderOnColumn__";
	char	*pcInsertHeader = "__InsertHeader__";
	char	*pcInsertRow = "__InsertRow__";

	m_sChanged = "";
	m_iChanged = -1;

	if (file.Open(*psFile, CFile::modeRead))
	{
		iReadCount = file.Read(bBuffer, 50000);
		if (iReadCount > 0)
		{	
			bBuffer[iReadCount] = 0;
			sFileTemplate = bBuffer;
		}
		file.Close();
	}
	else
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to find: %s", *psFile);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bool bFoundTemplate;
	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertRowTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertRowTemplateBegin);
		iPosEnd = sFileTemplate.Find("__InsertRowTemplateEnd__",0);
		if (iPosEnd >= 0)
		{
			sInsertRowTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			log.m_pText = "ERROR: __InsertRowTemplateBegin__ or __InsertRowTemplateEnd__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertHeaderTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertHeaderTemplateBegin);
		iPosEnd = sFileTemplate.Find("__InsertHeaderTemplateEnd__",0);
		if (iPosEnd >= 0)
		{
			sInsertHeaderTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			log.m_pText = "ERROR: __InsertHeaderTemplateBegin__ or __InsertHeaderTemplateEnd__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

// the sub header is optional
	iPosBegin = sFileTemplate.Find(pcInsertSubHeaderTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertSubHeaderTemplateBegin);
		iPosEnd = sFileTemplate.Find("__InsertSubHeaderTemplateEnd__",0);
		if (iPosEnd >= 0)
		{
			sInsertSubHeaderTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
		}
	}
	iPosBegin = sFileTemplate.Find(pcInsertSubHeaderOnColumn,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertSubHeaderOnColumn);
		CString sNr;
		sNr = sFileTemplate.Mid(iPosBegin);
		m_iChanged = atoi(sNr);
	}


// end sub header

	iItemCount = pListCtrl->GetItemCount();
	if (iItemCount == 0) return false;

	sInsertRow = sInsertHeaderTemplate;
	for (int iCountRow = 0; iCountRow < NUM_REMOTE_TASK_COLUMNS; iCountRow++)
	{
		ReplaceHeader(pRemoteTaskView, pListCtrl, &m_iColumnOrder[0], iCountRow, &sInsertRow);
	}

	for (int iCountRow = 0; iCountRow < NUM_REMOTE_TASK_COLUMNS; iCountRow++)
	{
		ReplaceHeader(pRemoteTaskView, pListCtrl, &m_iColumnOrder[0], iCountRow, &sInsertSubHeaderTemplate);
	}

	sFileNew = sFileTemplate;

	time_t tSystemTime;
	_time64(&tSystemTime);

	CDateConvert dateConvert;
	char cBuffer[64];
	dateConvert.Convert(tSystemTime, &cBuffer[0]);
	sFileNew.Replace("__TimeStamp__", cBuffer);
		
	sRefreshrate.Format("%d", theApp.m_pDlgSettingsWWW->m_iRefreshTime+1);
	sFileNew.Replace("__RefreshTime__", sRefreshrate);

	sFileNew.Replace(pcInsertHeader, sInsertRow);
	for (int iCount = 0; iCount < iItemCount; iCount++)
	{
		sInsertRow = sInsertRowTemplate;
		bInsertHeader = false;
		for (int iCountRow = 0; iCountRow < NUM_REMOTE_TASK_COLUMNS; iCountRow++)
		{
			if (ReplaceRow(pRemoteTaskView, pListCtrl, &m_iColumnOrder[0], iCount, iCountRow, &sInsertRow))
			{
				bInsertHeader = true;
			}
		}
		if (bInsertHeader)
		{
			sInsertRow = sInsertSubHeaderTemplate + sInsertRow;
		}

		sInsertRow+= pcInsertRow;
//		sInsertRow = ansiToUtf8.ToUtf8(&sInsertRow);	// already done
		sFileNew.Replace(pcInsertRow, sInsertRow);
	}

	CFileException e;
	if( !file.Open( psDest->GetBuffer(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
//		CString sTxt;
//
//		sTxt = "Unable to write to xml file, ";
//		sTxt+= sFile;
//		sTxt+= "Look in Help->Show Log for more details.";
//
//		AfxMessageBox(sTxt);
		return false;
	}

	sFileNew.Replace(pcInsertRow, "");
	int iLen = sFileNew.GetLength();
	file.Write(sFileNew,iLen);
	file.Close();

	return true;
}

//BGCOLOR="#ff0000"

void CCreateHtml::ReplaceHeader(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1], int iRow, CString *psInsertRow)
{
	CString sFind, sItemText;
	CAnsiToUtf8	ansiToUtf8;

	sFind = "";
	switch (iRow)
	{
		case COLUMN_PROJECT:
			sFind = "Project";
			sItemText = gszTranslation[PosViewTasksRowProject];
		break;
		case COLUMN_APPLICATION:
			sFind = "Application";
			sItemText = gszTranslation[PosViewTasksRowApplication];
		break;
		case COLUMN_NAME:
			sFind = "Name";
			sItemText = gszTranslation[PosViewTasksRowName];
		break;
		case COLUMN_ELAPSED:
			sFind = "Elapsed";
			sItemText = gszTranslation[PosViewTasksRowElapsed];
		break;
		case COLUMN_CPU_PERCENTAGE:
			sFind = "CpuPercentage";
			sItemText = gszTranslation[PosViewTasksRowCpuP];
		break;
		case COLUMN_PERCENTAGE:
			sFind = "Percentage";
			sItemText = gszTranslation[PosViewTasksRowProgress];
		break;
		case COLUMN_TIME_LEFT:
			sFind = "Timeleft";
			sItemText = gszTranslation[PosViewTasksRowTimeLeft];
		break;
		case COLUMN_DEADLINE:
			sFind = "Deadline";
			sItemText = gszTranslation[PosViewTasksRowDeadline];
		break;
		case COLUMN_USE:
			sFind = "Use";
			sItemText = gszTranslation[PosViewTasksRowUse];
		break;
		case COLUMN_STATE:
			sFind = "State";
			sItemText = gszTranslation[PosViewTasksRowStatus];
		break;
		case COLUMN_TTHROTTLE:
			sFind = "Tthrottle";
			sItemText = gszTranslation[PosViewTasksRowThrottle];
		break;
		case COLUMN_TEMPERATURE:
			sFind = "Temperature";
			sItemText = gszTranslation[PosViewTasksRowTemperature];
		break;
		case COLUMN_COMPUTER:
			sFind = "Computer";
			sItemText = gszTranslation[PosViewTasksRowComputer];
		break;
		case COLUMN_CHECKPOINT:
			sFind = "Checkpoint";
			sItemText = gszTranslation[PosViewTasksRowCheckpoint];
		break;
		case COLUMN_DEBT:
			sFind = "Debt";
			sItemText = gszTranslation[PosViewTasksRowDebt];
		break;
	}
	if (sFind.GetLength()  > 0)
	{
		sFind = "__" + sFind + "__";
		sItemText = ansiToUtf8.ToUtf8(&sItemText);
		psInsertRow->Replace(sFind,sItemText);
	}
}

bool CCreateHtml::ReplaceRow(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1], int iItem, int iRow, CString *psInsertRow)
{
	CString sFind, sFindCell, sItemText;
	int		iColumn;
	CAnsiToUtf8	ansiToUtf8;
	bool	bChanged = false;


	sFind = "";
	switch (iRow)
	{
		case COLUMN_PROJECT:
			sFind = "Project";
		break;
		case COLUMN_APPLICATION:
			sFind = "Application";
		break;
		case COLUMN_NAME:
			sFind = "Name";
		break;
		case COLUMN_ELAPSED:
			sFind = "Elapsed";
		break;
		case COLUMN_CPU_PERCENTAGE:
			sFind = "CpuPercentage";
		break;
		case COLUMN_PERCENTAGE:
			sFind = "Percentage";
		break;
		case COLUMN_TIME_LEFT:
			sFind = "Timeleft";
		break;
		case COLUMN_DEADLINE:
			sFind = "Deadline";
		break;
		case COLUMN_USE:
			sFind = "Use";
		break;
		case COLUMN_STATE:
			sFind = "State";
		break;
		case COLUMN_TTHROTTLE:
			sFind = "Tthrottle";
		break;
		case COLUMN_TEMPERATURE:
			sFind = "Temperature";
		break;
		case COLUMN_COMPUTER:
			sFind = "Computer";
		break;
		case COLUMN_CHECKPOINT:
			sFind = "Checkpoint";
		break;
		case COLUMN_DEBT:
			sFind = "Debt";
		break;
	}
	if (sFind.GetLength()  > 0)
	{
		sFindCell = "__" + sFind + "Color__";
		sFind = "__" + sFind + "__";
		iColumn = m_iColumnOrder[iRow];
		if (iColumn >= 0)
		{
			sItemText = pListCtrl->GetItemText(iItem,iColumn);
			if (m_iChanged == iRow)
			{
				if (m_sChanged.GetLength() == 0)
				{
					m_sChanged = sItemText;
				}
				else
				{
					if (m_sChanged != sItemText)
					{
						bChanged = true;
						m_sChanged = sItemText;
					}
				}
			}
		}
		else sItemText = "";
		pRemoteTaskView->RemoveSpecialText(&sItemText);

		sItemText = ansiToUtf8.ToUtf8(&sItemText);
		psInsertRow->Replace(sFind,sItemText);

		CListData *pData;
		pData = (CListData*) pListCtrl->GetItemData(iItem);
		COLORREF color;
		int r,g,b;
		r = (pData->m_color&0x0000ff) << 16 ;
		g = (pData->m_color&0x00ff00);
		b = (pData->m_color&0xff0000) >> 16;
		color = r|g|b;

		sItemText.Format("%6X", color);
		psInsertRow->Replace(sFindCell,sItemText);

	}
	return bChanged;
}

bool CCreateHtml::StartFtp()
{
	CString sProgramDir, sDataDir, sFullProgramDir;
	PROCESS_INFORMATION pi;
	STARTUPINFO         si;
	BOOL                bProcessStarted;
	char				cBufferLogging[129];
	CLoggingMessage		log;
	CXml	xml("");

	sProgramDir = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sProgramDir += "\\html\\";

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	sDataDir = sProgramDir;

	sFullProgramDir = sProgramDir + "ftp.bat" ;
	bProcessStarted = CreateProcess( NULL, sFullProgramDir.GetBuffer(), NULL, NULL, FALSE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, sDataDir, &si, &pi);

	if (!bProcessStarted)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Start Ftp";
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to start: %s", sFullProgramDir);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}
	return true;
}


// CCreateHtml member functions

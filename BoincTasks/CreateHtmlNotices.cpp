// CreateHtmlNotices.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include "BoincTasks.h"
#include "threadrpc.h"
#include "Xml.h"
#include "DlgNotices.h"
#include "DlgLogging.h"
#include "DlgSettingsGeneral.h"
#include "DlgSettingsExpert.h"
#include "DlgUpdate.h"
#include "CreateHtmlNotices.h"
#include "DateConvert.h"
#include "Translate.h"
#include "Translation.h"
#include "ThreadRpc.h"
#include "AnsiToUtf8.h"


// CCreateHtmlNotices

CCreateHtmlNotices::CCreateHtmlNotices()
{

}

CCreateHtmlNotices::~CCreateHtmlNotices()
{
}

bool CCreateHtmlNotices::Create(std::deque<CNoticesInfoItem*> *plNotices,  CString *psFileHtml, CString *psStringHtml, char *pcComputer, double dTimeShowFrom)	
{
	int					iReadCount, iPosBegin, iPosEnd;
	CFile				file;
	BYTE				bBuffer[50001];
	CString				sDirectoryLocal,sDirectoryUser, sFileFrom, sFileTo, sFileTemplate, sInsertNoticesTemplate, sInsertNoticesCollapsedTemplate, sInsertNoticesTemplateMore, sInsertNoticesTemplateImageStyle, sInsertOneNotice, sFileNew, sFind;
	char				cBufferLogging[129];
	CLoggingMessage		log;
	CAnsiToUtf8			AnsiToUtf8;

	char	*pcInsertNoticeTemplateBegin	= "__InsertNoticeTemplateBegin__";
	char	*pcInsertNoticeTemplateEnd		= "__InsertNoticeTemplateEnd__";

	char	*pcInsertNoticeCollapsedTemplateBegin	= "__InsertNoticeCollapsedTemplateBegin__";
	char	*pcInsertNoticeCollapsedTemplateEnd		= "__InsertNoticeCollapsedTemplateEnd__";


	char	*pcInsertNoticeTemplateMoreBegin= "__InsertNoticeTemplateMoreBegin__";
	char	*pcInsertNoticeTemplateMoreEnd	= "__InsertNoticeTemplateMoreEnd__";

//	char	*pcInsertNoticeCollapse			= "__InsertNoticeCollapse__";
	char	*pcInsertNotice					= "__InsertNotice__";
//	char	*pcInsertNoticeMoreLink			= "__InsertNoticeMoreLink__";

	char	*pcInsertNoticesTemplateImageStyleBegin	= "__InsertNoticeTemplateImageStyleBegin__";
	char	*pcInsertNoticesTemplateImageStyleEnd	= "__InsertNoticeTemplateImageStyleEnd__";

	CXml	xml("");

	sDirectoryLocal = xml.GetLocalDirectory();
	sDirectoryLocal  += "\\html\\";
	sDirectoryUser = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectoryUser += "\\html\\";
	_mkdir(sDirectoryUser);

	sFileFrom = sDirectoryLocal + "notices.html";
	BOOL	bFileOpen;


	bFileOpen = file.Open(sFileFrom, CFile::modeRead);

	if (!bFileOpen)
	{
		sFileFrom = sDirectoryLocal + "notices_template.html";
	}

	if (!bFileOpen)
	{
		bFileOpen = file.Open(sFileFrom, CFile::modeRead);
	}

	if (bFileOpen)
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
			log.m_pFrom = "Html notices create";
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to find: %s", sFileFrom);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bool bFoundTemplate;
	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertNoticeTemplateMoreBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertNoticeTemplateMoreBegin);
		iPosEnd = sFileTemplate.Find(pcInsertNoticeTemplateMoreEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertNoticesTemplateMore = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html notices create";
			log.m_pText = "ERROR: __InsertNoticeTemplateMore__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertNoticeTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertNoticeTemplateBegin);
		iPosEnd = sFileTemplate.Find(pcInsertNoticeTemplateEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertNoticesTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html notices create";
			log.m_pText = "ERROR: __InsertNoticeTempate__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertNoticeCollapsedTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertNoticeCollapsedTemplateBegin);
		iPosEnd = sFileTemplate.Find(pcInsertNoticeCollapsedTemplateEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertNoticesCollapsedTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html notices create";
			log.m_pText = "ERROR: __InsertNoticeCollapsedTempate__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertNoticesTemplateImageStyleBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertNoticesTemplateImageStyleBegin);
		iPosEnd = sFileTemplate.Find(pcInsertNoticesTemplateImageStyleEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertNoticesTemplateImageStyle = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html notices create";
			log.m_pText = "ERROR: __InsertNoticeTemplateImageStyleBegin__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

//	iItemCount = (int) pNotices->notices.size();
//	if (iItemCount == 0) return false;

	sFileNew = sFileTemplate;
	sFileNew.Replace("__InsertComputer__",pcComputer);
	sFileNew.Replace("__InsertCharSetCode__","utf-8");//gszTranslation[PosCharSet]);		// windows-1252

	if (plNotices != NULL)
	{
		for (int iPos = 0; iPos < (int) plNotices->size(); iPos++)
		{
			if (plNotices->at(iPos)->m_pNotice != NULL)
			{
				if (plNotices->at(iPos)->m_pNotice->create_time > dTimeShowFrom)
				{
					if (plNotices->at(iPos)->m_bCollapsed)	sInsertOneNotice = sInsertNoticesCollapsedTemplate;
					else									sInsertOneNotice = sInsertNoticesTemplate;
					ReplaceNotice(plNotices->at(iPos), &sInsertOneNotice, &sInsertNoticesTemplateMore, &sInsertNoticesTemplateImageStyle);

					// computers / notice
					CString sComputersFound;
					for (int iPosComputer = 0; iPosComputer < (int) plNotices->at(iPos)->m_lComputer.size(); iPosComputer++)
					{
						if (sComputersFound.GetLength() > 0) sComputersFound += ", ";
						sComputersFound += *plNotices->at(iPos)->m_lComputer.at(iPosComputer);
					}
					sInsertOneNotice.Replace("__InsertNoticeComputers__", sComputersFound);
					

					sInsertOneNotice += pcInsertNotice;
					sFileNew.Replace(pcInsertNotice, sInsertOneNotice);
				}
			}
		}
	}
	else
	{
		sFileNew += AnsiToUtf8.ToUtf8(gszTranslation[PosNoticeNoNotices]);
		sFileNew += ".";
	}


	sFileTo = sDirectoryUser + "notices.html";
	*psFileHtml = sFileTo;

	CFileException e;
	if( !file.Open( sFileTo.GetBuffer(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		return false;
	}

	sFileNew.Replace(pcInsertNotice, "");
	int iLen = sFileNew.GetLength();
	file.Write(sFileNew,iLen);
	file.Close();

	if (psStringHtml) *psStringHtml = sFileNew;



	return true;
}

bool CCreateHtmlNotices::Create(CString *psTxt,  CString *psHtml)
{
	CFile	file;	
	int		iReadCount;
	CString sDirectoryLocal, sDirectoryUser, sFileNew, sFileTo, sFileFrom, sFileTemplate, sFormat;
	BYTE	bBuffer[50001];
	char				cBufferLogging[129];
	CLoggingMessage		log;


	CXml	xml("");

	sDirectoryLocal = xml.GetLocalDirectory();
	sDirectoryLocal  += "\\html\\";
	sDirectoryUser = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectoryUser += "\\html\\";
	_mkdir(sDirectoryUser);

	sFileFrom = sDirectoryLocal + "notices.html";
	BOOL	bFileOpen;


	bFileOpen = file.Open(sFileFrom, CFile::modeRead);

	if (!bFileOpen)
	{
		sFileFrom = sDirectoryLocal + "notices_template.html";
	}

	if (!bFileOpen)
	{
		bFileOpen = file.Open(sFileFrom, CFile::modeRead);
	}


	if (bFileOpen)
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
			log.m_pFrom = "Html notices create";
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to find: %s", sFileFrom);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	sFileNew = sFileTemplate;
	sFileNew.Replace("__InsertComputer__","");
	sFileNew.Replace("__InsertCharSetCode__","utf-8");//gszTranslation[PosCharSet]);		// windows-1252
//	sFileNew.Replace("__InsertNoticeMoreLink__", "");
//	sFileNew.Replace("__InsertNoticeLink__", "");
//	sFileNew.Replace("__InsertNoticeMore__", "");

	sFileNew.Replace("__InsertNotice__ ", *psTxt);

	sFileTo = sDirectoryUser + "notices.html";
	*psHtml = sFileTo;

	CFileException e;
	if( !file.Open( sFileTo.GetBuffer(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
			return false;
	}
	int iLen = sFileNew.GetLength();
	file.Write(sFileNew,iLen);
	file.Close();
	return true;
}

void CCreateHtmlNotices::ReplaceNotice(CNoticesInfoItem* pNoticesInfoItem, CString *psInsertOneNotice, CString *psInsertMoreLink, CString *psInsertImageStyle)
{
	time_t tTime;
	CDateConvert dateConvert;
	char cBuffer[64];
	CString sCheck, sCategory, sAnsi, sDirectoryLocal, sFormat;
	CTranslate			translate;
	CAnsiToUtf8			AnsiToUtf8;
	CXml	xml("");

	char	*pInsertNoticeMoreLink = "__InsertNoticeMoreLink__";

	sDirectoryLocal = xml.GetLocalDirectory();
	sDirectoryLocal  += "\\html\\";

	sCheck = pNoticesInfoItem->m_pNotice->project_name;
	if (sCheck.GetLength() > 0)
	{
		sCheck += ": ";
	}
	sCategory = pNoticesInfoItem->m_pNotice->category;
	sCategory = sCategory.MakeLower();
	if (sCategory == "scheduler")
	{
		sCheck += AnsiToUtf8.ToUtf8(gszTranslation[PosNoticesCatScheduler]);
		if (strlen(pNoticesInfoItem->m_pNotice->title) > 0)
		{
			sCheck += ", ";
		}
	}
	if (sCategory == "client")
	{
		sCheck += AnsiToUtf8.ToUtf8(gszTranslation[PosNoticesCatClient]);
		if (strlen(pNoticesInfoItem->m_pNotice->title) > 0)
		{
			sCheck += ", ";
		}
	}
	
	sCheck += pNoticesInfoItem->m_pNotice->title;
	translate.TranslateBody(&sCheck,true);							// translate to UFT8
	psInsertOneNotice->Replace("__InsertNoticeTitle__", sCheck);

	sCheck = "<a href=\x22";
	sCheck+= "__collapse__";
	sFormat.Format("%d",pNoticesInfoItem->m_iID);
	sCheck+= sFormat;
	sCheck+= "\x22 ";
	sCheck+= *psInsertImageStyle;
	sCheck+= " target=\x22_blank\x22> <img src=\x22";
	sCheck+= sDirectoryLocal;
	if (pNoticesInfoItem->m_bCollapsed) sCheck+= "expand.png";
	else sCheck+= "collapse.png";
	sCheck+= "\x22 style=\x22";
	sCheck+= "border: 0p;x22 alt=\x22";
	sCheck+= "expand/collapse\x22></a>";

	if (theApp.m_pDlgSettingsGeneral->m_iOs == OS_SELECTED_WINDOWS)	psInsertOneNotice->Replace("__InsertNoticeCollapse__", sCheck);
	else psInsertOneNotice->Replace("__InsertNoticeCollapse__", "");

	if (pNoticesInfoItem->m_bCollapsed && pNoticesInfoItem->m_bAllowDelete)
	{
		sCheck= "<a href=\x22";
		sCheck+= "__delete__";
		sFormat.Format("%d",pNoticesInfoItem->m_iID);
		sCheck+= sFormat;
		sCheck+= "\x22 ";
		sCheck+= *psInsertImageStyle;
		sCheck+= " target=\x22_blank\x22> <img src=\x22";
		sCheck+= sDirectoryLocal;
		sCheck+= "delete.png";
		sCheck+= "\x22 style=\x22";
		sCheck+= "border: 0px;\x22 alt=\x22";
		sCheck+= "delete\x22></a>";
		psInsertOneNotice->Replace("__InsertNoticeDelete__", sCheck);
	}
	else psInsertOneNotice->Replace("__InsertNoticeDelete__", "");

	sCheck = pNoticesInfoItem->m_pNotice->description.c_str();
	AddLineBreak(&sCheck);
	translate.TranslateBody(&sCheck,true);							// translate to UFT8

	psInsertOneNotice->Replace("__InsertNoticeBody__", sCheck);

	tTime = (time_t) pNoticesInfoItem->m_pNotice->create_time;
	dateConvert.Convert(tTime, &cBuffer[0]);
	psInsertOneNotice->Replace("__InsertNoticeTimeCreated__", cBuffer);

	tTime = (time_t) pNoticesInfoItem->m_pNotice->arrival_time;
	dateConvert.Convert(tTime, &cBuffer[0]);
	psInsertOneNotice->Replace("__InsertNoticeTimeArrival__", cBuffer);

	if (strlen(pNoticesInfoItem->m_pNotice->link) > 7)
	{
		psInsertOneNotice->Replace(pInsertNoticeMoreLink, *psInsertMoreLink);
		psInsertOneNotice->Replace("__InsertNoticeLink__", pNoticesInfoItem->m_pNotice->link);
		psInsertOneNotice->Replace("__InsertNoticeMore__", AnsiToUtf8.ToUtf8(gszTranslation[PosNoticesMessageMore]));
	}
	else
	{
		psInsertOneNotice->Replace(pInsertNoticeMoreLink, "");
	}

	/*
	// remove all iframes	
	CString low = psInsertOneNotice->MakeLower();
	int iPosBegin = low.Find("<iframe", 0);
	if (iPosBegin > 0)
	{
		int iPosEnd = low.Find("iframe>", iPosBegin);
		if (iPosEnd > iPosBegin)
		{
			CString left = psInsertOneNotice->Left(iPosBegin);
			CString right = psInsertOneNotice->Mid(iPosEnd+7);
			*psInsertOneNotice = left + right;
		}
	}
	*/
}

void CCreateHtmlNotices::AddLineBreak(CString *psCheck)
{
	psCheck->Replace("\n","<BR>");
}

// CCreateHtmlNotices member functions

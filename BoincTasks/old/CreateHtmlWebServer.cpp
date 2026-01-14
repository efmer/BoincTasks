// CreateHtml.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include "BoincTasks.h"
#include "CreateHtmlWebServer.h"
#include "listviewex.h"
#include "DlgColorGeneral.h"
#include "DlgSettingsExpert.h"
#include "DlgSettingsMobile.h"
#include "DateConvert.h"
#include "Translation.h"
#include "AnsiToUtf8.h"

// CCreateHtml

CCreateHtmlWebServer::CCreateHtmlWebServer()
{
}

CCreateHtmlWebServer::~CCreateHtmlWebServer()
{
}

bool CCreateHtmlWebServer::Create(int iPosHeader, CListViewEx *pView, CListCtrl *pListCtrl, int *piColomnOrder, int iColumnsMax, CString *psSelectedColumns, int iPosTranslation, char **pcHtml)
{
	CFile				file;
	int					iPosBegin, iPosEnd, iItemCount, iColumnCount, iReadCount, iColumn;
	CStringA			sFile, sFileTemplate, sFileNew, sInsertHeaderTemplate, sInsertRowBeginTemplate, sInsertRowEndTemplate, sInsertCellTemplate, sInsertRow, sItemText, sCelText, sColumn, sFind, sRefreshrate;
	CLoggingMessage		log;
	char				cBuffer[MAX_PATH+32], *pcFound;
	BYTE				bBuffer[50001];
	char				cBufferLogging[129];
	CStringA			sPath, sFormat;
	CAnsiToUtf8			AnsiToUtf8;

	CString sFullSelect  ="00;01;02;03;04;05;06;07;08;09;10;11;12;13;14;15;16;17;18;19";

	char	*pcInsertRowBeginTemplateBegin	= "__InsertRowBeginTemplateBegin__";
	char	*pcInsertRowBeginTemplateEnd	= "__InsertRowBeginTemplateEnd__";
	char	*pcInsertRowEndTemplateBegin	= "__InsertRowEndTemplateBegin__";
	char	*pcInsertRowEndTemplateEnd		= "__InsertRowEndTemplateEnd__";

	char	*pcInsertColTemplateBegin		= "__InsertColTemplateBegin__";
	char	*pcInsertColTemplateEnd			= "__InsertColTemplateEnd__";
	char	*pcInsertHeaderTemplateBegin	= "__InsertHeaderTemplateBegin__";
	char	*pcInsertHeaderTemplateEnd		= "__InsertHeaderTemplateEnd__";

	char	*pcInsertCellClass	= "__InsertTableCellClass__";

//	char	*pcInsertHeaderFont	= "__InsertHeaderFontSize__";
//	char	*pcInsertFont		= "__InsertFontSize__";

	char	*pcInsertFontColor			= "__InsertFontColor__";
	char	*pcInsertFontColorSelected	= "__InsertFontColorSelected__";

	char	*pcInsertHeader		= "__InsertHeader__";
	char	*pcInsertRow		= "__InsertRow__";

	DWORD dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;

	strcat_s(cBuffer,"\\webserver\\list_template.html");

	if (file.Open(cBuffer, CFile::modeRead))
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
			_snprintf_s(cBufferLogging,127,_TRUNCATE,"ERROR: Unable to find: %s", cBuffer);log.m_pText = cBufferLogging;
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	if (psSelectedColumns == NULL)
	{
		psSelectedColumns = &sFullSelect;
	}


	bool bFoundTemplate;
	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertRowBeginTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertRowBeginTemplateBegin);
		iPosEnd = sFileTemplate.Find(pcInsertRowBeginTemplateEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertRowBeginTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			log.m_pText = "ERROR: __InsertRowBeginTemplateBegin(End)__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertRowEndTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertRowEndTemplateBegin);
		iPosEnd = sFileTemplate.Find(pcInsertRowEndTemplateEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertRowEndTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			log.m_pText = "ERROR: __InsertRowEndTemplateBegin(End)__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}


	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertColTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertColTemplateBegin);
		iPosEnd = sFileTemplate.Find(pcInsertColTemplateEnd,0);
		if (iPosEnd >= 0)
		{
			sInsertCellTemplate = sFileTemplate.Mid(iPosBegin,iPosEnd-iPosBegin);
			bFoundTemplate = true;
		}
	}
	if (!bFoundTemplate)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			log.m_pFrom = "Html create";
			log.m_pText = "ERROR: __InsertColTemplateBegin(End)__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	bFoundTemplate = false;
	iPosBegin = sFileTemplate.Find(pcInsertHeaderTemplateBegin,0);
	if (iPosBegin >= 0)
	{
		iPosBegin += (int) strlen(pcInsertHeaderTemplateBegin);
		iPosEnd = sFileTemplate.Find(pcInsertHeaderTemplateEnd,0);
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
			log.m_pText = "ERROR: __InsertHeaderTemplateBegin(End)__ not found.";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_DEBUG);
		}
		return false;
	}

	int	iColumnOrder[20];
	int	iColumnFormat[20];
	int iColumnWidth[20];
	for(int iCount = 0; iCount < iColumnsMax; iCount++)
	{
		iColumnOrder[iCount] = -1;
		iColumnFormat[iCount] = -1;
	}
	for(int iCount = 0; iCount < iColumnsMax; iCount++)
	{
		if ((*(piColomnOrder+iCount) >=0) && (*(piColomnOrder+iCount) < iColumnsMax))
		{
			iColumnOrder[*(piColomnOrder+iCount)] = iCount;
		}
	}

	CDC dc;
	dc.CreateCompatibleDC(NULL);

	LV_COLUMN	lvc;
	lvc.pszText = &cBuffer[0];
	lvc.cchTextMax = 254;
	lvc.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	CString sHeader;
	sHeader = "<tr class=boinctasks_table_header>";
	iColumnCount = 0;
	for(int iCount = 0; iCount < iColumnsMax; iCount++)
	{
		iColumn = iColumnOrder[iCount];
		if (iColumn >= 0)
		{
			sColumn.Format("%02d",iColumn);
			if (psSelectedColumns->Find(sColumn) >=0)
			{
				lvc.iSubItem = iColumnCount;
				pListCtrl->GetColumn(iColumnCount, &lvc);
				iColumnFormat[iColumnCount] = lvc.fmt & LVCFMT_JUSTIFYMASK;
				iColumnWidth[iColumnCount] = lvc.cx;
				sCelText = lvc.pszText;
				if (iColumnWidth[iColumnCount] == 0)
				{
					iColumnCount++;	// invisible but still a column
					continue;
				}
				sItemText.Format("%d", lvc.cx);
				sFormat.Format("%d", iCount);
				sHeader += sInsertHeaderTemplate;
				sHeader.Replace("__Nr__", sFormat);
				sHeader.Replace("__Item__", AnsiToUtf8.ToUtf8(&sCelText));
			}
			iColumnCount++;
		}
	}
	sHeader += "<tr>\r\n";

//	sFormat.Format("%d", theApp.m_pDlgSettingsMobile->m_iHeaderFont);
//	sFileTemplate.Replace(pcInsertHeaderFont,sFormat);
//	sFormat.Format("%d", theApp.m_pDlgSettingsMobile->m_iFont);
//	sFileTemplate.Replace(pcInsertFont,sFormat);

	COLORREF color;
	COLORREF *pColorArray;

	pColorArray = theApp.m_pDlgColorGeneral->GetColorArray();
	color = *(pColorArray+COLOR_GENERAL_ARRAY_FONT_SELECTED);
	color = Rgb(color);
	sFormat.Format("%6X", color);
	sFileTemplate.Replace(pcInsertFontColorSelected,sFormat);

	color = *(pColorArray+COLOR_GENERAL_ARRAY_FONT);
	color = Rgb(color);
	sFormat.Format("%6X", color);
	sFileTemplate.Replace(pcInsertFontColor,sFormat);

	sFileNew = sFileTemplate;
	sFileNew.Replace("__Header__",AnsiToUtf8.ToUtf8(gszTranslation[iPosHeader]));
	sFileNew.Replace(pcInsertHeader, sHeader);

	bool bAdd = false;

	iItemCount = pListCtrl->GetItemCount();
	for (int iItem = 0; iItem < iItemCount; iItem++)
	{
		for(int iCount = 0; iCount < iColumnsMax; iCount++)
		{
			iColumn = iColumnOrder[iCount];
			if (iColumn >= 0)
			{
				sFormat.Format("%02d",iCount);
				if (psSelectedColumns->Find(sFormat) >=0)
				{
					bAdd = true;
					break;
				}
			}
		}
		if (!bAdd) continue;

		CListData *pData;
//		int r,g,b;
		pData = (CListData*) pListCtrl->GetItemData(iItem);
		if (pData->m_bSelected)
		{
			pColorArray = theApp.m_pDlgColorGeneral->GetColorArray();
			color = *(pColorArray+COLOR_GENERAL_ARRAY_BACK_SELECTED);
			color = Rgb(color);
		}
		else
		{
			color = Rgb(pData->m_color);
		}

		sInsertRow = sInsertRowBeginTemplate;

		iColumnCount = 0;
		for(int iCount = 0; iCount < iColumnsMax; iCount++)
		{
			iColumn = iColumnOrder[iCount];
			if (iColumn >= 0)
			{
				sColumn.Format("%02d",iColumn);
				if (psSelectedColumns->Find(sColumn) >=0)
				{
					sCelText = pListCtrl->GetItemText(iItem,iColumnCount);
					if (iColumnWidth[iColumnCount] == 0)
					{
						iColumnCount++;
						continue;
					}
					sInsertRow += sInsertCellTemplate;
					sItemText.Format("%6X", color);
					sInsertRow.Replace("__ItemColor__",sItemText);
					switch (iColumnFormat[iColumnCount])
					{
						case LVCFMT_RIGHT:
							sItemText = "right";
						break;
						case LVCFMT_CENTER:
							sItemText = "center";
						break;
						default:
							sItemText = "left";
					}
					sInsertRow.Replace("__ItemAlign__",sItemText);
					pView->RemoveSpecialText(&sCelText);
					sItemText = pView->MakeShortString(&dc,sCelText,iColumnWidth[iColumnCount],-24);
					sItemText = AnsiToUtf8.ToUtf8(&sItemText);
					sInsertRow.Replace("__Item__",sItemText);
				}
				iColumnCount++;
			}
		}

		if (pData->m_bSelected)	sInsertRow.Replace(pcInsertCellClass, "bt_cSelected");
		else					sInsertRow.Replace(pcInsertCellClass, "bt_c");

		sInsertRow += sInsertRowEndTemplate;
//		sInsertRow += "</tr>\r\n";
		sInsertRow+= pcInsertRow;
		sFileNew.Replace(pcInsertRow, sInsertRow);
	}


	sFileNew.Replace(pcInsertRow,"");
	RemoveComment(&sFileNew);
	sFileNew.Replace("\r","");
	sFileNew.Replace("\n","");
	sFileNew.Replace("\t","");

	char *pcHtmlNew;
	int iLen =sFileNew.GetLength()+1;
	pcHtmlNew = new char [iLen];
	strcpy_s(pcHtmlNew, iLen, sFileNew.GetBuffer());
	*pcHtml = pcHtmlNew;

// testing only

/*
	dwSize = GetModuleFileName (NULL, cBuffer, MAX_PATH);	// not 0 terminated
	cBuffer[dwSize] = 0;
	pcFound = strrchr(cBuffer, '\\');
	if (pcFound != NULL) *pcFound = 0;
	strcat_s(cBuffer,"\\webserver\\testing_data.html");

	CFileException e;
	if( !file.Open( cBuffer, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		return true;
	}

	iLen = (int) strlen(pcHtmlNew);
	file.Write(pcHtmlNew,iLen);
	file.Close();
*/

	return true;
}

void CCreateHtmlWebServer::RemoveComment(CString *psHtml)
{
	int iBegin, iEnd;
	CString sPart1, sPart2;

	while (1)
	{
		iBegin = psHtml->Find("<!--",0);
		iEnd = psHtml->Find("-->",0);

		if (iBegin<0) break;
		if (iEnd<0) break;

		if (iBegin > iEnd) break;

		sPart1 = psHtml->Mid(0,iBegin);
		sPart2 = psHtml->Mid(iEnd+3);

		*psHtml = sPart1 + sPart2;
	}
	return;
}

COLORREF CCreateHtmlWebServer::Rgb(COLORREF color)
{
	int r,g,b;
	r = (color&0x0000ff) << 16 ;
	g = (color&0x00ff00);
	b = (color&0xff0000) >> 16;
	return r|g|b;
}

// CCreateHtml member functions

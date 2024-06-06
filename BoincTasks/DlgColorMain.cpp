// DlgColorMain.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorGeneral.h"
#include "DlgColorProjects.h"
#include "DlgColorTasks.h"
#include "DlgColorMessages.h"
#include "DlgColorHistory.h"
#include "DlgColorWarnings.h"
#include "DlgColorGraphic.h"
#include "DlgColorTemperature.h"
#include "DlgFont.h"

#include "Xml.h"



// CDlgColorMain


IMPLEMENT_DYNAMIC(CDlgColorMain, CPropertySheet)

CDlgColorMain::CDlgColorMain(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgColorMain::CDlgColorMain(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgColorMain::~CDlgColorMain()
{
	if (!m_hIcon) delete m_hIcon;
}

BOOL CDlgColorMain::OnInitDialog()
{
	CPropertySheet::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon



	return TRUE;  // return TRUE  unless you set the focus to a control
}

bool CDlgColorMain::ReadXml(void)
{
	CString		sTemp;
	COLORREF	*pColorArray;
	bool		bReturn;

	bReturn = false;

	CXml xml(XML_COLOR_FILE,true,false,false);
	if (xml.ReadStart("colors"))
	{
		bReturn = true;
		if (xml.ReadGroup(XML_COLOR_GENERAL))
		{
			pColorArray = theApp.m_pDlgColorGeneral->GetColorArray();
			if (xml.ReadItem(XML_COLOR_GENERAL_GRID,&sTemp)) {*(pColorArray+COLOR_GENERAL_ARRAY_GRID) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_GENERAL_FONT,&sTemp)) {*(pColorArray+COLOR_GENERAL_ARRAY_FONT) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_GENERAL_FONT_SELECTED,&sTemp)) {*(pColorArray+COLOR_GENERAL_ARRAY_FONT_SELECTED) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_GENERAL_BACK_SELECTED,&sTemp)) {*(pColorArray+COLOR_GENERAL_ARRAY_BACK_SELECTED) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_GENERAL_BAR,&sTemp)) {*(pColorArray+COLOR_GENERAL_ARRAY_BAR) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_GENERAL_BAR_OUTLINE,&sTemp)) {*(pColorArray+COLOR_GENERAL_ARRAY_BAR_OUTLINE) = strtol(sTemp,NULL,16);}
			theApp.m_pDlgColorGeneral->SaveRegistry();
		}
		else bReturn = false;
		if (xml.ReadGroup(XML_COLOR_PROJECTS))
		{
			pColorArray = theApp.m_pDlgColorProjects->GetColorArray();
			if (xml.ReadItem(XML_COLOR_PROJECTS_NORMAL,&sTemp)) {*(pColorArray+COLOR_ARRAY_PROJECT_NORMAL) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_PROJECTS_SUSPENDED,&sTemp)) {*(pColorArray+COLOR_ARRAY_PROJECT_SUSPENDED) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_PROJECTS_NO_WORK,&sTemp)) {*(pColorArray+COLOR_ARRAY_PROJECT_NONEWWORK) = strtol(sTemp,NULL,16);} 
			theApp.m_pDlgColorProjects->SaveRegistry();
		}
		else bReturn = false;
		if (xml.ReadGroup(XML_COLOR_TASKS))
		{
			pColorArray = theApp.m_pDlgColorTasks->GetColorArray();
			if (xml.ReadItem(XML_COLOR_TASKS_ABORT,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_ABORT) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_UD,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_UD_GPU,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_READY_TO_REPORT,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_REPORT) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_READY_TO_REPORT_GPU,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_RUNNING,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_RUNNING) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_RUNNING_GPU,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_RUNNING_GPU) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_HIGH_P,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_HIGH_PRIORITY) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_HIGH_P_GPU,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_WAITING_TO_RUN,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_WAITING_TO_RUN) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_WAITING_TO_RUN_GPU,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TASKS_READY_TO_START,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_START) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TASKS_READY_TO_START_GPU,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_START_GPU) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TASKS_ERROR,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_ERROR) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TASKS_ERROR_GUP,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_ERROR_GPU) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TASKS_SUSPENDED,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_SUSPENDED) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TASKS_SUSPENDED_GPU,&sTemp)) {*(pColorArray+COLOR_TASKS_ARRAY_SUSPENDED_GPU) = strtol(sTemp,NULL,16);} 
			theApp.m_pDlgColorTasks->SaveRegistry();
		}
		else bReturn = false;
		if (xml.ReadGroup(XML_COLOR_MESSAGES))
		{
			pColorArray = theApp.m_pDlgColorMessages->GetColorArray();
			if (xml.ReadItem(XML_COLOR_MESSAGES_PRIORITY_1,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_1) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_MESSAGES_PRIORITY_2,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_2) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_MESSAGES_PRIORITY_3,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_3) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_1,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_1) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_2,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_2) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_3,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_3) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_4,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_4) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_5,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_5) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_6,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_6) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_7,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_7) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_8,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_8) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_9,&sTemp)) {*(pColorArray+COLOR_MESSAGES_ARRAY_HL_9) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_MESSAGES_HIGHLIGHS_10,&sTemp)){*(pColorArray+COLOR_MESSAGES_ARRAY_HL_10)= strtol(sTemp,NULL,16);}

			theApp.m_pDlgColorMessages->SaveRegistry();
		}
		else bReturn = false;
		if (xml.ReadGroup(XML_COLOR_HISTORY))
		{
			pColorArray = theApp.m_pDlgColorHistory->GetColorArray();
			if (xml.ReadItem(XML_COLOR_HISTORY_UPLOAD,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_UPLOAD) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_UPLOAD_GPU,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_UPLOAD_GPU) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_READY_TO_REPORT,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_READY_TO_REPORT) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_READY_TO_REPORT_GPU,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_ABORT,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_ABORT) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_ERROR,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_ERROR) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_ERROR_GPU,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_ERROR_GPU) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_REPORTED,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_REPORTED_GPU,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED_GPU) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_REPORTED_ERROR,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED_ERROR) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_HISTORY_REPORTED_ERROR_GPU,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_HISTORY_MISSED,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_MISSED) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_HISTORY_MISSED_GPU,&sTemp)) {*(pColorArray+COLOR_HISTORY_ARRAY_MISSED_GPU) = strtol(sTemp,NULL,16);}

			theApp.m_pDlgColorHistory->SaveRegistry();
		}
		else bReturn = false;
		if (xml.ReadGroup(XML_COLOR_WARNINGS))
		{
			pColorArray = theApp.m_pDlgColorWarnings->GetColorArray();
			if (xml.ReadItem(XML_COLOR_WARNINGS_RATIO,&sTemp)) {*(pColorArray+COLOR_WARNINGS_ARRAY_ERROR_RATIO) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_WARNINGS_RATIO_GPU,&sTemp)) {*(pColorArray+COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_WARNINGS_DEADLINE,&sTemp)) {*(pColorArray+COLOR_WARNINGS_ARRAY_DEADLINE) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_WARNINGS_DEADLINE_RUNNING,&sTemp)) {*(pColorArray+COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_WARNINGS_TASKS,&sTemp)) {*(pColorArray+COLOR_WARNINGS_ARRAY_TASKS) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_WARNINGS_CHECKPOINT,&sTemp)) {*(pColorArray+COLOR_WARNINGS_ARRAY_CHECKPOINT) = strtol(sTemp,NULL,16);}
			theApp.m_pDlgColorWarnings->SaveRegistry();
		}
		else bReturn = false;

		if (xml.ReadGroup(XML_COLOR_GRAPHIC))
		{
			pColorArray = theApp.m_pDlgColorGraphic->GetColorArray();
			if (xml.ReadItem(XML_COLOR_GRAPHIC_BACKGROUND,&sTemp)) {*(pColorArray+COLOR_ARRAY_BACKGROUND) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_GRAPHIC_PROJECT_TEXT,&sTemp)) {*(pColorArray+COLOR_ARRAY_PROJECT_TEXT) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_GRAPHIC_SCALE_TEXT,&sTemp)) {*(pColorArray+COLOR_ARRAY_SCALE_TEXT) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_GRAPHIC_GRID,&sTemp)) {*(pColorArray+COLOR_ARRAY_GRID) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_GRAPHIC_AVERAGE,&sTemp)) {*(pColorArray+COLOR_ARRAY_AVERAGE) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER1,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER1) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER2,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER2) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER3,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER3) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER4,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER4) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER5,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER5) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER6,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER6) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER7,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER7) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER8,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER8) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER9,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER9) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER10,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER10) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER11,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER11) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER12,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER12) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER13,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER13) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER14,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER14) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER15,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER15) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER16,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER16) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER17,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER17) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER18,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER18) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER19,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER19) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_GRAPHIC_COMPUTER20,&sTemp)) {*(pColorArray+COLOR_ARRAY_COMPUTER20) = strtol(sTemp,NULL,16);} 
			theApp.m_pDlgColorGraphic->SaveRegistry();
		}

		if (xml.ReadGroup(XML_COLOR_TEMPERATURE))
		{
			pColorArray = theApp.m_pDlgColorTemperature->GetColorArray();
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE0,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_1) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE1,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_2) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE2,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_3) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE3,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_4) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE4,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_5) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE5,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_6) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE6,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_7) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE7,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_8) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE8,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_9) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE9,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_10) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE10,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_11) = strtol(sTemp,NULL,16);} 	
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CORE11,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_12) = strtol(sTemp,NULL,16);} 	

			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU0,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_1) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU1,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_2) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU2,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_3) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU3,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_4) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU4,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_5) = strtol(sTemp,NULL,16);} 
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU5,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_6) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU6,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_7) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU7,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_8) = strtol(sTemp,NULL,16);}

			if (xml.ReadItem(XML_COLOR_TEMPERATURE_CPU_TTHROTTLE,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU_TTHROTTLE,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_BACK,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_ARRAY_BACK) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GRID_FINE,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_ARRAY_GRID_FINE) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GRID,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_ARRAY_GRID) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_TEXT,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_ARRAY_TEXT) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_MAX,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_MAX) = strtol(sTemp,NULL,16);}
			if (xml.ReadItem(XML_COLOR_TEMPERATURE_GPU_MAX,&sTemp)) {*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_MAX) = strtol(sTemp,NULL,16);}
			theApp.m_pDlgColorTemperature->SaveRegistry();
		}
		// no else false, as there are older version who don't have this enty

		if (xml.ReadGroup(XML_COLOR_FONT))
		{
			bool	bSystemFont = true;
			BYTE	buffer[256], bufferGadget[256];
			int		iSize, iSizeGadget;

			if (xml.ReadItem(XML_COLOR_FONT_SYSTEM,&sTemp)) {if (sTemp == "0") bSystemFont = false;}
			xml.ReadItemBin(XML_COLOR_FONT_LF, &buffer[0], &iSize);
			xml.ReadItemBin(XML_COLOR_FONT_GADGET_LF, &bufferGadget[0], &iSizeGadget);

			if (iSize == sizeof(LOGFONT) && iSizeGadget == sizeof(LOGFONT))
			{
				theApp.m_pDlgFont->SetFont(bSystemFont, &buffer[0], &bufferGadget[0]);
			}
		}
		else bReturn = false;
	}

	return bReturn;
}

bool CDlgColorMain::WriteXml(void)
{
	CStringA	sATxt, sAValue;
	COLORREF	*pColorArray;
	bool		bReturn;

	bReturn = false;
	CXml xml(XML_COLOR_FILE);
	if (xml.WriteStart("colors"))
	{
		bReturn = true;
		xml.WriteBeginGroup(XML_COLOR_GENERAL);
		pColorArray = theApp.m_pDlgColorGeneral->GetColorArray();
		sATxt.Format("%x",*(pColorArray+COLOR_GENERAL_ARRAY_GRID));			xml.WriteItem(XML_COLOR_GENERAL_GRID,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_GENERAL_ARRAY_FONT));			xml.WriteItem(XML_COLOR_GENERAL_FONT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_GENERAL_ARRAY_FONT_SELECTED));xml.WriteItem(XML_COLOR_GENERAL_FONT_SELECTED,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_GENERAL_ARRAY_BACK_SELECTED));xml.WriteItem(XML_COLOR_GENERAL_BACK_SELECTED,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_GENERAL_ARRAY_BAR));			xml.WriteItem(XML_COLOR_GENERAL_BAR,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_GENERAL_ARRAY_BAR_OUTLINE));	xml.WriteItem(XML_COLOR_GENERAL_BAR_OUTLINE,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_PROJECTS);
		pColorArray = theApp.m_pDlgColorProjects->GetColorArray();
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_PROJECT_NORMAL));		xml.WriteItem(XML_COLOR_PROJECTS_NORMAL,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_PROJECT_SUSPENDED));	xml.WriteItem(XML_COLOR_PROJECTS_SUSPENDED,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_PROJECT_NONEWWORK));	xml.WriteItem(XML_COLOR_PROJECTS_NO_WORK,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_TASKS);
		pColorArray = theApp.m_pDlgColorTasks->GetColorArray();	
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_ABORT));				xml.WriteItem(XML_COLOR_TASKS_ABORT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD));	xml.WriteItem(XML_COLOR_TASKS_UD,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU));xml.WriteItem(XML_COLOR_TASKS_UD_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_REPORT));	xml.WriteItem(XML_COLOR_TASKS_READY_TO_REPORT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU));xml.WriteItem(XML_COLOR_TASKS_READY_TO_REPORT_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_RUNNING));			xml.WriteItem(XML_COLOR_TASKS_RUNNING,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_RUNNING_GPU));		xml.WriteItem(XML_COLOR_TASKS_RUNNING_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_HIGH_PRIORITY));		xml.WriteItem(XML_COLOR_TASKS_HIGH_P,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU));	xml.WriteItem(XML_COLOR_TASKS_HIGH_P_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_WAITING_TO_RUN));		xml.WriteItem(XML_COLOR_TASKS_WAITING_TO_RUN,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU));	xml.WriteItem(XML_COLOR_TASKS_WAITING_TO_RUN_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_START));		xml.WriteItem(XML_COLOR_TASKS_READY_TO_START,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_READY_TO_START_GPU));	xml.WriteItem(XML_COLOR_TASKS_READY_TO_START_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_ERROR));				xml.WriteItem(XML_COLOR_TASKS_ERROR,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_ERROR_GPU));			xml.WriteItem(XML_COLOR_TASKS_ERROR_GUP,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_SUSPENDED));			xml.WriteItem(XML_COLOR_TASKS_SUSPENDED,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TASKS_ARRAY_SUSPENDED_GPU));		xml.WriteItem(XML_COLOR_TASKS_SUSPENDED_GPU,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_MESSAGES);
		pColorArray = theApp.m_pDlgColorMessages->GetColorArray();
		sATxt.Format("%x",*(pColorArray+COLOR_MESSAGES_ARRAY_1));				xml.WriteItem(XML_COLOR_MESSAGES_PRIORITY_1,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_MESSAGES_ARRAY_2));				xml.WriteItem(XML_COLOR_MESSAGES_PRIORITY_2,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_MESSAGES_ARRAY_3));				xml.WriteItem(XML_COLOR_MESSAGES_PRIORITY_3,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_MESSAGES_ARRAY_HL_1));			xml.WriteItem(XML_COLOR_MESSAGES_HIGHLIGHS_1,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_MESSAGES_ARRAY_HL_2));			xml.WriteItem(XML_COLOR_MESSAGES_HIGHLIGHS_2,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_MESSAGES_ARRAY_HL_3));			xml.WriteItem(XML_COLOR_MESSAGES_HIGHLIGHS_3,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_HISTORY);
		pColorArray = theApp.m_pDlgColorHistory->GetColorArray();
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_UPLOAD));				xml.WriteItem(XML_COLOR_HISTORY_UPLOAD,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_UPLOAD_GPU));			xml.WriteItem(XML_COLOR_HISTORY_UPLOAD_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_READY_TO_REPORT));		xml.WriteItem(XML_COLOR_HISTORY_READY_TO_REPORT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU));	xml.WriteItem(XML_COLOR_HISTORY_READY_TO_REPORT_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_ABORT));				xml.WriteItem(XML_COLOR_HISTORY_ABORT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_ERROR));				xml.WriteItem(XML_COLOR_HISTORY_ERROR,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_ERROR_GPU));			xml.WriteItem(XML_COLOR_HISTORY_ERROR_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED));				xml.WriteItem(XML_COLOR_HISTORY_REPORTED,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED_GPU));			xml.WriteItem(XML_COLOR_HISTORY_REPORTED_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED_ERROR));		xml.WriteItem(XML_COLOR_HISTORY_REPORTED_ERROR,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU));	xml.WriteItem(XML_COLOR_HISTORY_REPORTED_ERROR_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_MISSED));				xml.WriteItem(XML_COLOR_HISTORY_MISSED,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_HISTORY_ARRAY_MISSED_GPU));			xml.WriteItem(XML_COLOR_HISTORY_MISSED_GPU,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_WARNINGS);
		pColorArray = theApp.m_pDlgColorWarnings->GetColorArray();
		sATxt.Format("%x",*(pColorArray+COLOR_WARNINGS_ARRAY_ERROR_RATIO));		xml.WriteItem(XML_COLOR_WARNINGS_RATIO,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU));	xml.WriteItem(XML_COLOR_WARNINGS_RATIO_GPU,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_WARNINGS_ARRAY_DEADLINE));		xml.WriteItem(XML_COLOR_WARNINGS_DEADLINE,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING));xml.WriteItem(XML_COLOR_WARNINGS_DEADLINE_RUNNING,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_WARNINGS_ARRAY_TASKS));			xml.WriteItem(XML_COLOR_WARNINGS_TASKS,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_WARNINGS_ARRAY_CHECKPOINT));		xml.WriteItem(XML_COLOR_WARNINGS_CHECKPOINT,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_GRAPHIC);pColorArray = theApp.m_pDlgColorGraphic->GetColorArray();
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_BACKGROUND)); xml.WriteItem(XML_COLOR_GRAPHIC_BACKGROUND,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_PROJECT_TEXT)); xml.WriteItem(XML_COLOR_GRAPHIC_PROJECT_TEXT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_SCALE_TEXT)); xml.WriteItem(XML_COLOR_GRAPHIC_SCALE_TEXT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_GRID)); xml.WriteItem(XML_COLOR_GRAPHIC_GRID,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_AVERAGE)); xml.WriteItem(XML_COLOR_GRAPHIC_AVERAGE,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER1)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER1,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER2)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER2,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER3)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER3,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER4)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER4,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER5)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER5,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER6)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER6,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER7)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER7,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER8)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER8,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER9)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER9,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER10)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER10,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER11)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER11,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER12)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER12,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER13)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER13,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER14)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER14,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER15)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER15,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER16)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER16,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER17)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER17,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER18)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER18,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER19)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER19,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_ARRAY_COMPUTER20)); xml.WriteItem(XML_COLOR_GRAPHIC_COMPUTER20,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_TEMPERATURE);
		pColorArray = theApp.m_pDlgColorTemperature->GetColorArray();
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_1)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE0,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_2)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE1,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_3)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE2,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_4)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE3,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_5)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE4,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_6)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE5,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_7)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE6,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_8)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE7,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_9)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE8,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_10)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE9,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_11)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE10,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_12)); xml.WriteItem(XML_COLOR_TEMPERATURE_CORE11,sATxt,1);

		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_1)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU0,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_2)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU1,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_3)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU2,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_4)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU3,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_5)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU4,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_6)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU5,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_7)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU6,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_8)); xml.WriteItem(XML_COLOR_TEMPERATURE_GPU7,sATxt,1);

		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE))	; xml.WriteItem(XML_COLOR_TEMPERATURE_CPU_TTHROTTLE,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE))	; xml.WriteItem(XML_COLOR_TEMPERATURE_GPU_TTHROTTLE,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_ARRAY_BACK))			; xml.WriteItem(XML_COLOR_TEMPERATURE_BACK,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_ARRAY_GRID_FINE))		; xml.WriteItem(XML_COLOR_TEMPERATURE_GRID_FINE,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_ARRAY_GRID))			; xml.WriteItem(XML_COLOR_TEMPERATURE_GRID,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_ARRAY_TEXT))			; xml.WriteItem(XML_COLOR_TEMPERATURE_TEXT,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_CPU_ARRAY_MAX))		; xml.WriteItem(XML_COLOR_TEMPERATURE_MAX,sATxt,1);
		sATxt.Format("%x",*(pColorArray+COLOR_TEMPERATURE_GPU_ARRAY_MAX))		; xml.WriteItem(XML_COLOR_TEMPERATURE_GPU_MAX,sATxt,1);
		xml.WriteEndGroup();

		xml.WriteBeginGroup(XML_COLOR_FONT);
		sATxt.Format("%x", theApp.m_pDlgFont->m_bSystemFont);
		xml.WriteItem(XML_COLOR_FONT_SYSTEM, sATxt,1);
		xml.WriteItemBin(XML_COLOR_FONT_LF,(BYTE *) &theApp.m_pDlgFont->m_logFont,sizeof(LOGFONT));
		xml.WriteItemBin(XML_COLOR_FONT_GADGET_LF,(BYTE *) &theApp.m_pDlgFont->m_logFontGadget,sizeof(LOGFONT));

//		xml.WriteItem("font",theApp.m_pDlgFont->m_sFont);
//		sATxt.Format("%d",theApp.m_pDlgFont->m_iFontSize);
//		xml.WriteItem("font_size",sATxt);
//		xml.WriteItem("font_gadget",theApp.m_pDlgFont->m_sFontGadget);
//		sATxt.Format("%d",theApp.m_pDlgFont->m_iFontSizeGadget);
//		xml.WriteItem("font_size_gadget",sATxt);
		xml.WriteEndGroup();
	}
	xml.WriteEnd();
	return bReturn;
}


void CDlgColorMain::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		// close window = hide
		ShowWindow(SW_HIDE);
		return;
	}
	CPropertySheet::OnSysCommand(nID, lParam);
}

BEGIN_MESSAGE_MAP(CDlgColorMain, CPropertySheet)
	ON_MESSAGE(UWM_MSG_WRITE_SETTINGS,OnWriteSettings)
	ON_MESSAGE(UWM_MSG_READ_SETTINGS,OnReadSettings)
END_MESSAGE_MAP()


// CDlgColorMain message handlers

LRESULT  CDlgColorMain::OnReadSettings(WPARAM wParam,LPARAM lParam)
{
	bool bReturn;

	bReturn = ReadXml();

	if (bReturn) return TRUE;

	return FALSE;
}

LRESULT  CDlgColorMain::OnWriteSettings(WPARAM wParam,LPARAM lParam)
{
	bool bReturn;

	bReturn = WriteXml();

	if (bReturn) return TRUE;

	return FALSE;
}

BOOL CDlgColorMain::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			ShowWindow(SW_HIDE);
		}
	}
	return CPropertySheet::PreTranslateMessage(pMsg);
}

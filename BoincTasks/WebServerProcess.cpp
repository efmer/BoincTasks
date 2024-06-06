// WebServerProcess.cpp : implementation file
//

#include "stdafx.h"
#include <time.h>
#include "BoincTasks.h"
#include "listviewex.h"
#include "RemoteCombinedDoc.h"
#include "TemplateRemoteTaskView.h"
#include "RemoteCombinedTaskView.h"
#include "TemplateRemoteProjectView.h"
#include "RemoteCombinedProjectView.h"
#include "TemplateRemoteTransferView.h"
#include "RemoteCombinedTransferView.h"
#include "DlgLogging.h"
#include "CreateHtmlWebServerComputerSelect.h"
#include "CreateHtmlWebServerFilter.h"
#include "CreateHtmlWebServerButton.h"
#include "CreateHtmlWebServerGraph.h"
#include "CreateHtmlWebServerGraphProjectSelect.h"
#include "CreateHtmlWebServerGraphComputerSelect.h"
#include "DlgGraphic.h"
#include "DlgBarViewSelect.h"
#include "DlgSettingsGeneral.h"
#include "DlgSettingsMobile.h"
#include "ThreadWebServer.h"
#include "DlgSettingsMobile.h"
#include "MainFrm.h"
#include "ThreadWebServer.h"
#include "WebServer.h"
#include "WebServer2.h"
#include "WebServerProcess.h"
#include "Translation.h"
#include "AnsiToUtf8.h"

#include "\programma\common\bf\Blowfish.h"

// CWebServerProcess

IMPLEMENT_DYNAMIC(CWebServerProcess, CWnd)

CWebServerProcess::CWebServerProcess()
{
	char	strExePath [MAX_PATH], *pcFound;
	CString	sPath, sFormat;

	m_sCrLf = "\r\n";
	g_tLastMobileActivety = 0;

	DWORD dwSize = GetModuleFileName (NULL, strExePath, MAX_PATH);	// not 0 terminated
	strExePath[dwSize] = 0;
	strcpy_s(m_cWebFolderBuffer, strExePath);
	pcFound = strrchr(m_cWebFolderBuffer, '\\');
	if (pcFound != NULL)
	{
		*pcFound = 0;
	}
	strcat_s(m_cWebFolderBuffer,"\\webserver\\");
	m_iWebServerLogin = WEBSERVER_LOGIN_AUTO;
	m_bWebServerSelectMultiple = false;
	m_iWebServerHtmlCurrentTab = 2;
	m_pbFileDataBuffer = NULL;
	m_iFileDataBuferLen = 0;
}

CWebServerProcess::~CWebServerProcess()
{
	if (m_pbFileDataBuffer)	delete m_pbFileDataBuffer;
}

void CWebServerProcess::LoginHtmlAuto()
{
	CAnsiToUtf8 ansiToUtf8;
	string sToBeReplaced, sToBeReplacedWith;

	SetMessageFile("login_auto.html");
	sToBeReplaced = "_T_LoginHeaderAuto_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginHeaderAuto]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T__LoginAuto_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginLoginAuto]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginHeader_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginHeader]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginKeep_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginKeep]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginStore_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginStore]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginPassword_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginPassword]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
		int iLen;
	iLen = (int) m_htmlHeaderSend.m_sContent.length();
	SetMessageLength(iLen);
}

void CWebServerProcess::LoginHtmlNormal()
{
	CAnsiToUtf8 ansiToUtf8;
	string sToBeReplaced, sToBeReplacedWith;

	SetMessageFile("login.html");
	sToBeReplaced = "_T_LoginHeaderAuto_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginHeaderAuto]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T__LoginAuto_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginLoginAuto]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginHeader_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginHeader]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginKeep_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginKeep]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginStore_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginStore]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
	sToBeReplaced = "_T_LoginPassword_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginPassword]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
		int iLen;
	iLen = (int) m_htmlHeaderSend.m_sContent.length();
	SetMessageLength(iLen);
}

void CWebServerProcess::Get()
{
	string	sFormat, sRequest, sFileName, sMessage;
	CAnsiToUtf8 ansiToUtf8;

	sMessage = "";

	string sUrl = m_htmlHeaderRecieve.m_sUrl;
	string sHost = m_htmlHeaderRecieve.m_sHost;

	SetMimeType("text/html", false);
	if (sUrl.find(".ico") != string::npos) SetMimeType (HTML_MIME_ICO, true);
	else
	{
		if (sUrl.find(".js") != string::npos)	SetMimeType (HTML_MIME_JS, false);
		else
		{
			if (sUrl.find(".css") != string::npos)	SetMimeType (HTML_MIME_CSS, false);
			else
			{
				if (sUrl.find(".gif") != string::npos)	SetMimeType (HTML_MIME_GIF, true);
				else
				{
					if (sUrl.find(".png") != string::npos)	SetMimeType (HTML_MIME_PNG, true);
				}
			}
		}
	}

	if (sUrl.length() > 500)
	{
		return;
	}

	if (sUrl.find("favicon.ico") != string::npos)
	{
		SetMessageFile("images/favicon.ico");
		return;
	}

	// debug mode
	if (sUrl.find("debug.js") != string::npos)
	{
		SetMimeType(HTML_MIME_JS, false);
		if (theApp.m_pDlgLogging->m_bLogDebugMobileScript)	sMessage = "var debug = 'y';";
		else sMessage ="var debug = 'n';";
		SetMessage(&sMessage);
		return;
	}

	// apple icon
	if (sUrl.find("apple-touch-icon") != string::npos)
	{
		SetMessageFile("images/eFMerBoincTasks.png");
		return;
	}

	if (sUrl.find("login.html") != string::npos)
	{
		LoginHtmlNormal();
		return;
	}
	if (sUrl.compare("login_failed.html") == 0)
	{
		SetMessageFile(sUrl);
		string sToBeReplacedWith, sToBeReplaced;
		sToBeReplaced = "_T_LoginHeaderFailed_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginHeaderFailed]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
		sToBeReplaced = "_T_LoginNeedPassword_";sToBeReplacedWith = ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileLoginNeedPassword]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);

		int iLen;
		iLen = (int) m_htmlHeaderSend.m_sContent.length();
		SetMessageLength(iLen);
		return;
	}



	if (sUrl.compare("bt.html") == 0)
	{
#define BT_HTML_TRANSLATIONS 29
		char *cTranslate[BT_HTML_TRANSLATIONS][2] = {
		{"_T_Computer_"		, gszTranslation[PosBarViewSelectComputers]},
		{"_T_Projects_"		, gszTranslation[PosBarViewSelectProjects]},
		{"_T_Tasks_"		, gszTranslation[PosBarViewSelectTasks]},
		{"_T_Transfers_"	, gszTranslation[PosBarViewSelectTransfers]},
		{"_T_Messages_"		, gszTranslation[PosBarViewSelectMessages]},
		{"_T_History_"		, gszTranslation[PosBarViewSelectHistory]},
		{"_T_Options_"		, gszTranslation[PosGroupMobileButtonOptions]},
		{"_T_Logout_"		, gszTranslation[PosGroupMobileLogout]},
		{"_T_CancelButton_"	, gszTranslation[PosDialogCommonButtonsCancel]},
		{"_T_MobileHeaderFont_", gszTranslation[PosGroupDialogSettingsMobileHeaderFont]},
		{"_T_MobileFont_"	, gszTranslation[PosGroupDialogSettingsMobileFont]},
		{"_T_FontSize_"		, gszTranslation[PosGroupMobileButtonFontSize]},
		{"_T_MultipleSelections_", gszTranslation[PosGroupMobileButtonMultipleSelections]},
		{"_T_Poperties_"	, gszTranslation[PosGroupPopUpTasksPoperties]},
		{"_T_Filter_"		, gszTranslation[PosGroupMobileButtonFilter]},
		{"_T_ButtonAbort_"	, gszTranslation[PosGroupPopUpTasksAbort]},
		{"_T_AbortQuestion_", gszTranslation[PosGroupDialogTransferAbort]},
		{"_T_Sort_"			, gszTranslation[PosGroupMobileSort]},
		{"_T_Sort1_"		, gszTranslation[PosGroupMobileSort1]},
		{"_T_Sort2_"		, gszTranslation[PosGroupMobileSort2]},
		{"_T_Sort3_"		, gszTranslation[PosGroupMobileSort3]},
		{"_T_Graph_"		, gszTranslation[PosGroupMobileButtonGraph]},
		{"_T_ButtonToggle_" , gszTranslation[PosGroupMobileButtonToggleFilter]},
		{"_T_Button_Yes_"	, gszTranslation[PosDialogCommonButtonsYesButton]},
		{"_T_Button_No_"	, gszTranslation[PosDialogCommonButtonsNoButton]},
		{"_T_ButtonClose_"	, gszTranslation[PosDialogCommonButtonsOk]},
		{"_T_ButtonNoWork_"	, gszTranslation[PosGroupPopUpProjectsNoNewTasks]},
		{"_T_ButtonAllowWork_"	, gszTranslation[PosGroupPopUpProjectsAllowNewTasks]},
		{"_T_ButtonFullScreen_"	, gszTranslation[PosGroupMobileButtonToggleFullScreen]}
		};
		
		SetMessageFile(sUrl);
		string sToBeReplacedWith, sToBeReplaced;
		CString sVersion;
		sVersion.LoadString(IDS_PROGRAM_VERSION);
		sToBeReplacedWith = "BoincTasks " + sVersion;
		sToBeReplaced = BT_HTML_HEADER;
		StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);

		for (int iCount = 0; iCount < BT_HTML_TRANSLATIONS; iCount++)
		{
			sToBeReplaced = cTranslate[iCount][0];
			sToBeReplacedWith = ansiToUtf8.ToUtf8(cTranslate[iCount][1]);
			StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
		}

		int iLen;
		iLen = (int) m_htmlHeaderSend.m_sContent.length();
		SetMessageLength(iLen);
		m_htmlHeaderSend.m_bTimeValid = false;		// do not cache this.
		return;
	}

	if (sUrl.compare("graph.html") == 0)
	{
#define GRAPH_HTML_TRANSLATIONS 8
		char *cTranslate[BT_HTML_TRANSLATIONS][3] = {
		{"_T_ButtonDone_"		, gszTranslation[PosGroupMobileButtonGraphDone]},
		{"_T_ButtonSComputer_"	, gszTranslation[PosGroupMobileButtonGraphSComputer]},
		{"_T_ButtonSProject_"	, gszTranslation[PosGroupMobileButtonGraphSProject]},
		{"_T_ButtonClose_"		, gszTranslation[PosDialogCommonButtonsCancel]},
		{"_T_ButtonUser"		, gszTranslation[PosDialogGraphicRadioUser]},
		{"_T_ButtonComputer"	, gszTranslation[PosDialogGraphicRadioComputer]},
		{"_T_ButtonTotal"		, gszTranslation[PosDialogGraphicRadioTotal]},
		{"_T_ButtonAverage"		, gszTranslation[PosDialogGraphicRadioAverage]}
		};
		
		SetMessageFile(sUrl);
		string sToBeReplacedWith, sToBeReplaced;

		CString sVersion;
		sVersion.LoadString(IDS_PROGRAM_VERSION);
		sToBeReplacedWith = "BoincTasks " + sVersion;
		sToBeReplaced = BT_HTML_HEADER;
		StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);

		for (int iCount = 0; iCount < GRAPH_HTML_TRANSLATIONS; iCount++)
		{
			sToBeReplaced = cTranslate[iCount][0];sToBeReplacedWith = ansiToUtf8.ToUtf8(cTranslate[iCount][1]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
		}

		int iLen;
		iLen = (int) m_htmlHeaderSend.m_sContent.length();
		SetMessageLength(iLen);
		m_htmlHeaderSend.m_bTimeValid = false;		// do not cache this.
		return;
	}

	if (sUrl.compare("js/boinctasks_mobile.js") == 0)
	{
#define BT_JS_TRANSLATIONS 8
		char *cTranslate[BT_JS_TRANSLATIONS][2] = {
			{"_T_Update_"	, gszTranslation[PosGroupPopUpProjectsUpdate]},
			{"_T_Suspend_"	, gszTranslation[PosGroupPopUpTasksSuspend]},
			{"_T_Resume_"	, gszTranslation[PosGroupPopUpTasksResume]},
			{"_T_Retry_"	, gszTranslation[PosPopUpTransfersRetry]},
			{"_T_RetryAll_"	, gszTranslation[PosPopUpTransfersRetryAll]},
			{"_T_ButtonMore_", gszTranslation[PosGroupMobileButtonMore]},
			{"_T_ButtonLess_", gszTranslation[PosGroupMobileButtonLess]},
			{"_T_Report_"	, gszTranslation[PosGroupMobileButtonReport]}
		};

		SetMessageFile(sUrl);
		string sToBeReplacedWith, sToBeReplaced;
		for (int iCount = 0; iCount < BT_JS_TRANSLATIONS; iCount++)
		{
			sToBeReplaced = cTranslate[iCount][0];sToBeReplacedWith = ansiToUtf8.ToUtf8(cTranslate[iCount][1]);StringReplace(&m_htmlHeaderSend.m_sContent, &sToBeReplaced, &sToBeReplacedWith);
		}

		int iLen;
		iLen = (int) m_htmlHeaderSend.m_sContent.length();
		SetMessageLength(iLen);
		return;
	}

	if (sUrl.compare("graph.html") == 0)
	{
		CreateHtmlWebServerGraph createHtmlWebServerGraph;

		string sHtml;
		createHtmlWebServerGraph.Create(&sHtml);
		SetMessage(&sHtml);
		return;
	}

	if (sUrl == "graph.json")
	{
		CreateHtmlWebServerGraph createHtmlWebServerGraph;

		string sHtml;
		CRemoteCombinedDoc	*pDoc;
		POSITION	posDoc;
		posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
		if (posDoc != NULL)
		{
			pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		}
		if (pDoc == NULL)
		{
			return;
		}
		createHtmlWebServerGraph.Json(pDoc, &sHtml);
		SetMessage(&sHtml);
		return;
	}


	if (sUrl.length() > 0)
	{
		SetMessageFile(sUrl);
		return;
	}
	else
	{
		m_bWebServerSelectMultiple = false;
		LoginHtmlAuto();
		return;
	}

	// login check

	if (m_iWebServerLogin == WEBSERVER_LOGIN_FAILED)
	{
		SetMessageFile("login_failed.html");
		return;
	}

	if (m_iWebServerLogin == WEBSERVER_LOGIN_AUTO)
	{
		m_bWebServerSelectMultiple = false;
		LoginHtmlAuto();
		return;
	}

	if (m_iWebServerLogin == WEBSERVER_LOGIN_NEEDED)
	{
		m_bWebServerSelectMultiple = false;
		LoginHtmlNormal();
		return;
	}
}

void CWebServerProcess::Post()
{
	CView		*pView;
	CRemoteCombinedDoc	*pDoc;
	POSITION	posDoc, posView;
	bool		bSessionValid;
	char		cBuffer[512+1];
	char		cBufferFormat[64];

	char *pcFound, *pcFoundSub;

	char *pcFindAgent=				"<__bt__agent__>";
	char *pcFindPassword =			"<__bt__password__>";
	char *pcFindPasswordEncrypted =	"<__bt__password_e__>";
	char *pcFindContinue =			"<__bt__continue__>";
	char *pcFindSwitch =			"<__bt__switch__><";
	char *pcFindSelectRow =			"<__bt__row__><";

	char *pcFindTaskOperation =		"<__bt__task_operation__><";
	char *pcFindProjectOperation =	"<__bt__project_operation__><";
	char *pcFindTransferOperation=	"<__bt__transfer_operation__><";
	char *pcFindOperationUpdate =	"update";
	char *pcFindOperationSuspend =	"suspend";
	char *pcFindOperationResume =	"resume";
	char *pcFindOperationRetry =	"retry";
	char *pcFindOperationRetryAll =	"retry_all";
	char *pcFindOperationAbort =	"abort";
	char *pcFindOperationNoMoreWork="nomorework";
	char *pcFindOperationAllowMoreWork="allowmorework";

	char *pcFindSelect =			"<__bt_select__>";
	char *pcFindSelectMultiple =	"sel_multiple";
	char *pcFingSelectSingle =		"sel_single";
	char *pcFindSelectFilter =		"sel_filter";

	char *pcFindProjectProperties =	"<__bt__project_properties__>";
	char *pcFindProjectGetCompleted ="<__bt__report_get_completed__>";
	char *pcFindProjectCompleted =	"<__bt__report_completed__>";
	char *pcFindTaskProperties =	"<__bt__tasks_properties__>";
	char *pcFindTransferProperties ="<__bt__transfer_properties__>";

	char *pcFindGetProperties =		"<__bt__get_properties__>";
	//char *pcFindData =				"<__bt__data__>";
	char *pcFindFooter =			"<__bt__footer__>";
	char *pcFindSelectComputers =	"<__bt__select_computers__>";
	char *pcFindSelectComputersEnd ="<__bt__select_computers_end__>";
	char *pcFindComputers =			"<__bt__computers__>";
	char *pcFindComputersExpand =	"<__bt__computers_expand__>";
	char *pcFindComputersCollapse =	"<__bt__computers_collapse__>";
	char *pcFindGetFilter =			"<__bt__getfilter__>";
	char *pcFindSetFilter =			"<__bt__setfilter__>";
	char *pcFindSort =				"<__bt_sort__><";

	char *pcFindPopupGraphProject=	"<__bt__popup_graph_project__>";
	char *pcFindSelectGraphProject=	"<__bt__select_graph_projects__>";
	char *pcFindSingleGraphProjects="<__bt__single_graph_projects__>";
	char *pcFindPopupGraphComputer=	"<__bt__popup_graph_computer__>";
	char *pcFindSelectGraphComputer="<__bt__select_graph_computers__>";
	char *pcFindSetGraphType =		"<__bt__graph_set_type__>";
	char *pcFindGetGraphType =		"<__bt__graph_get_type__>";

	char *pcFindColumnTasks	=		"<__bt__column__><tasks>";

//	char *pcFindFontHeader =		"<__bt__font_header__><";
//	char *pcFindFontText =			"<__bt__font_text__><";

	string sMessage;
	sMessage = "";
	SetMimeType("text/html", false);

	pDoc = NULL;
	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
	}
	if (pDoc == NULL) return;
	pView = NULL;

	if (!g_bWebServerActive)
	{
		g_bWebServerActive = true;
		CLoggingMessage sLog;
		sLog.m_pFrom = WEBSERVER_LOG_NAME;
		sLog.m_pText = "Awake";
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
	}
	_time64(&g_tLastMobileActivety);

	bSessionValid = false;
	cBuffer[512] = 0;
	strcpy_s(cBuffer, 512, m_htmlHeaderRecieve.m_sUrl.c_str());

	if (m_sAWebServerPasswordClear.compare(theApp.m_pDlgSettingsMobile->m_sPassword) != 0)
	{
		// password changed

		ResetPassword();
	}

	if (strlen(m_cRandomString) > 0)
	{
//		if (m_htmlHeaderRecieve.m_sUrl.find(m_cRandomString) != string::npos)
//		{
//			bSessionValid = true;
//		}

		if (m_htmlHeaderRecieve.m_sParameter.find(m_cRandomString) 	!= string::npos)
		{
//			pcFound+= strlen(m_cRandomString);
//			strcpy_s(cBuffer, 512, pcFound);		
			bSessionValid = true;
		}
	}

	if (theApp.m_pDlgSettingsMobile->m_sPassword.GetLength() == 0) bSessionValid = true;

	// agent
	pcFound = (char *) strstr(cBuffer, pcFindAgent);
	if (pcFound)
	{
		pcFound+= strlen(pcFindAgent);
		
		CLoggingMessage sLog;
		sLog.m_pFrom = WEBSERVER_LOG_NAME;
		sLog.m_pText = pcFound;
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_DEBUG);	

		sMessage = "";
		SetMessage(&sMessage);
		return;
	}
	


	// login failed sends continue on pressing the ok button
	pcFound = (char *) strstr(cBuffer, pcFindContinue);
	if (pcFound)
	{
		LoginHtmlNormal();
//		SetMessageFile("login.html");
		m_iWebServerLogin = WEBSERVER_LOGIN_NEEDED;
		return;
	}

	// check for a cookie encrypted password
	pcFound = (char *) strstr(cBuffer, pcFindPasswordEncrypted);
	if (pcFound)
	{
		CStringA sAKey, sAPassword;
		char cOutput[2048];
		CBlowFish bf;

		pcFound+= strlen(pcFindPasswordEncrypted);
		sAPassword = pcFound;		//theApp.GetProfileString(registrySectionGeneral, registryWWWPassword, "");
		sAKey = "1%^";
		sAKey+= "ac#$";
		sAKey+= "fe86$!";
		sAKey.SetAt(2,'3');
		sAKey.SetAt(7,'^');

		int iLen = sAPassword.GetLength();
		if (iLen > 0 && iLen < 128)
		{
			cOutput[0] = 0;
			bf.ExtractHex(cOutput,sAPassword.GetBuffer(), 2048);sAPassword.ReleaseBuffer();
			sAPassword = cOutput;
			bf.Decrypt(sAPassword.GetBuffer(), cOutput, (char *) sAKey.GetBuffer(), 2048); sAKey.ReleaseBuffer();sAPassword.ReleaseBuffer();
			sAPassword = cOutput;
			sAPassword = sAPassword.Mid(16, 256);
		}

//		pcFound+= strlen(pcFindPassword);
		bool bPasswordOk = false;

		if (theApp.m_pDlgSettingsMobile->m_sPassword.GetLength() == 0) 
		{
			CLoggingMessage sLog;
			sLog.m_pFrom = WEBSERVER_LOG_NAME;
			sLog.m_pText = "Login OK, no password needed";
			theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);

			m_iWebServerLogin = WEBSERVER_LOGIN_NO_PASSWORD;
			::GenerateRandomStringHex(m_cRandomString,128);
			bPasswordOk = true;
			g_iWebServerLoginDelay = 0;
		}
		else
		{
			if (sAPassword == theApp.m_pDlgSettingsMobile->m_sPassword)
			{
				bPasswordOk = true;
				m_iWebServerLogin = WEBSERVER_LOGIN_PASSWORD;
				::GenerateRandomStringHex(m_cRandomString,128);
				g_iWebServerLoginDelay = 0;

				CLoggingMessage sLog;
				sLog.m_pFrom = WEBSERVER_LOG_NAME;
				sLog.m_pText = "Login OK";
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
			}
			else
			{
				m_iWebServerLogin = WEBSERVER_LOGIN_FAILED;
				m_cRandomString[0] = 0;
				sMessage = m_cRandomString;
				SetMessage(&sMessage);
				Sleep(g_iWebServerLoginDelay);
				g_iWebServerLoginDelay+= 4000;

				if (g_iWebServerLoginDelay > 10000)
				{
					g_iWebServerLoginDelay = 10000;
				}

				CString sFormat;
				sFormat.Format("Loging failed (%d)", g_iWebServerLoginDelay);
				CLoggingMessage sLog;
				sLog.m_pFrom = WEBSERVER_LOG_NAME;
				sLog.m_pText = sFormat.GetBuffer();
				theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
				sMessage = "";
				SetMessage(&sMessage);
				return;
			}
		}
		sMessage = m_cRandomString;
		SetMessage(&sMessage);
		return;
	}

	// set the password from the login page
    pcFound = (char *) strstr(cBuffer, pcFindPassword);
	if (pcFound)
	{
		string sPassword;
		pcFound+= strlen(pcFindPassword);
		if (pcFound == m_sAWebServerPasswordClear)
		{
			m_iWebServerLogin = WEBSERVER_LOGIN_PASSWORD;
			sPassword = m_sAWebServerPasswordHex;
		}
		else
		{
			m_iWebServerLogin = WEBSERVER_LOGIN_FAILED;
			sPassword = "";
		}
		SetMessage(&sPassword);
		return;
	}

	// if random is valid, you are logged in.
	if (bSessionValid)
	{
		if (m_iWebServerLogin != WEBSERVER_LOGIN_NO_PASSWORD)
		{
			m_iWebServerLogin = WEBSERVER_LOGIN_PASSWORD;
		}
	}

/*
    pcFound = (char *) strstr(cBuffer, pcFindData);
	if (pcFound)
	{
		if (bSessionValid)
		{
			if (g_iWebServerHtmlTabSend[m_iWebServerHtmlCurrentTab] == FALSE)
			{
				if (g_pcWebServerHtmlTab[m_iWebServerHtmlCurrentTab] != NULL)
				{
					sMessage = g_pcWebServerHtmlTab[m_iWebServerHtmlCurrentTab];
					SetMessage(&sMessage);
					g_iWebServerHtmlTabSend[m_iWebServerHtmlCurrentTab] = TRUE;
					return;
				}
			}
			sMessage = "";
			SetMessage(&sMessage);
			return;
		}
		else
		{
			sMessage = "__InvalidRandom__";
			SetMessage(&sMessage);
			return;
		}
	}
*/

    pcFound = (char *) strstr(cBuffer, pcFindFooter);
	if (pcFound)
	{
		if (bSessionValid)				// tab _[?]_ format time _[?]_ data
		{
			sMessage = "-1_[?]_0_[?]_0";		// split is een string: _[?]_
			if (g_iWebServerHtmlTabSend[m_iWebServerHtmlCurrentTab] == FALSE)
			{
				if (g_pcWebServerHtmlTab[m_iWebServerHtmlCurrentTab] != NULL)
				{
					char cBufferFormat[64];
					_snprintf_s(cBufferFormat,64,_TRUNCATE,"%d_[?]_%d_[?]_", m_iWebServerHtmlCurrentTab, g_tWebServerHtmlTabSend[m_iWebServerHtmlCurrentTab]);	
					sMessage = cBufferFormat;
					sMessage+= g_pcWebServerHtmlTab[m_iWebServerHtmlCurrentTab];
					g_iWebServerHtmlTabSend[m_iWebServerHtmlCurrentTab] = TRUE;
				}
			}
			sMessage += "_[?]_";
			sMessage += g_pcStatusToSend;
			SetMessage(&sMessage);
			return;
		}
		else
		{
			sMessage = "__InvalidRandom__";
			SetMessage(&sMessage);
			return;
		}

	}

    pcFound = (char *) strstr(cBuffer, pcFindSelectComputers);
	if (pcFound && bSessionValid)
	{
		string sData;
		GetComputerTree(&sData);
		SetMessage(&sData);
		return;
	}

    pcFound = (char *) strstr(cBuffer, pcFindSelectComputersEnd);
	if (pcFound && bSessionValid)
	{
		sMessage = "";
		SetMessage(&sMessage);
		return;
	}

    pcFound = (char *) strstr(cBuffer, pcFindComputers);
	if (pcFound && bSessionValid)
	{
		CString sState, sGroup, sComputer;
		pcFound+= strlen(pcFindComputers);

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
					bSingle = (BOOL) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_SET_SELECTED, (WPARAM) pComputerSetItem, 0);
				}
			}
		}
		string sData;
		GetComputerTree(&sData);
		SetMessage(&sData);
		return;
	}
	
    pcFound = (char *) strstr(cBuffer, pcFindComputersExpand);
	if (pcFound && bSessionValid)
	{
		CString sGroup;
		pcFound+= strlen(pcFindComputersExpand);
		sGroup = ExtractXmlTag(&pcFound);
		if (sGroup.GetLength() > 0)
		{
			theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_TREE, (WPARAM) &sGroup,1);
		}
		sMessage = "";
		SetMessage(&sMessage);
		return;
	}

    pcFound = (char *) strstr(cBuffer, pcFindComputersCollapse);
	if (pcFound && bSessionValid)
	{
		CString sGroup;

		pcFound+= strlen(pcFindComputersCollapse);
		sGroup = ExtractXmlTag(&pcFound);
		if (sGroup.GetLength() > 0)
		{
			theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_TREE, (WPARAM) &sGroup,0);
		}
		sMessage = "";
		SetMessage(&sMessage);
		return;
	}

    pcFound = (char *) strstr(cBuffer, pcFindSwitch);
	if (pcFound && bSessionValid)
	{
		pcFound+= strlen(pcFindSwitch);
		m_iWebServerHtmlCurrentTab = atoi(pcFound);

		for (int iCount = 0; iCount < 6; iCount++)
		{
			g_iWebServerHtmlTabSend[iCount] = FALSE;
		}

		posView = pDoc->GetFirstViewPosition();
		while (1)
		{
			pView = pDoc->GetNextView(posView);
			if (pView == NULL) break;
			pView->SendMessage(UWM_MSG_TAB_VIEW_SELECTED, m_iWebServerHtmlCurrentTab, 0);
		}
		sMessage = "";
		SetMessage(&sMessage);
		return;
	}

    pcFound = (char *) strstr(cBuffer, pcFindSelectRow);
	if (pcFound && bSessionValid)
	{
		
		sMessage = "0_[?]_";
		CHtmlExchange htmlExchange;
		pcFound+= strlen(pcFindSelectRow);
		int iSelect = atoi(pcFound);
		iSelect-=2;
		posView = pDoc->GetFirstViewPosition();
		while (1)
		{
			pView = pDoc->GetNextView(posView);
			if (pView == NULL) break;

			int	iMultiple; 
			if (m_bWebServerSelectMultiple) iMultiple = WEBSERVER_SELECT_MULTIPLE;
			else iMultiple = WEBSERVER_SELECT_SINGLE;

			for (int iRetry = 0; iRetry < 2; iRetry++)
			{
				htmlExchange.m_iOption = iMultiple;
				htmlExchange.m_iRow = iSelect;
				htmlExchange.m_pcHtml = NULL;
				htmlExchange.tTime = 0;
				htmlExchange.m_bSendData = true;
				pView->SendMessage(UWM_MSG_WEB_ITEM_SELECTED, 0, (LPARAM) &htmlExchange);
				if (htmlExchange.m_pcHtml)
				{
					_snprintf_s(cBufferFormat,64,_TRUNCATE,"%d_[?]_", htmlExchange.tTime);	
					sMessage = cBufferFormat;
					sMessage+= htmlExchange.m_pcHtml;
					SetMessage(&sMessage);
					delete htmlExchange.m_pcHtml;
					return;
				}
				Sleep(500);
			}
		}
		SetMessage(&sMessage);
		delete htmlExchange.m_pcHtml;
		return;
	}

	// Tasks operations
    pcFound = (char *) strstr(cBuffer, pcFindTaskOperation);
	if (pcFound && bSessionValid)
	{
		pcFound+= strlen(pcFindTaskOperation);

		pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));

		int iOperation = -1;
		if (pView != NULL)
		{
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationSuspend);
			if (pcFoundSub)	iOperation = TASKS_OP_SUSPEND;
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationResume);
			if (pcFoundSub)	iOperation = TASKS_OP_RESUME;
			if (iOperation > 0)	pView->SendMessage(UWM_MSG_WEB_OPERATION, iOperation, 0);
		}
		sMessage = "";
		SetMessage(&sMessage);
		return;
	}

	// Tasks properties
    pcFound = (char *) strstr(cBuffer, pcFindTaskProperties);
	if (pcFound && bSessionValid)
	{
		sMessage = "";
		pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
		if (pView != NULL)
		{
			BOOL bResult;
			bResult = (BOOL) pView->SendMessage(UWM_MSG_WEB_PROPERTIES,0,0);
			if (bResult == TRUE)
			{
				sMessage = "TRUE";	
			}
		}
		SetMessage(&sMessage);
		return;
	}

	// Project properties
    pcFound = (char *) strstr(cBuffer, pcFindProjectProperties);
	if (pcFound && bSessionValid)
	{
		sMessage = "";
		pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedProjectView));
		if (pView != NULL)
		{
			BOOL bResult;
			bResult = (BOOL) pView->SendMessage(UWM_MSG_WEB_PROPERTIES,0,0);
			if (bResult == TRUE)
			{
				sMessage = "TRUE";	
			}
		}
		SetMessage(&sMessage);
		return;
	}

	// Project get completed

    pcFound = (char *) strstr(cBuffer, pcFindProjectGetCompleted);
	if (pcFound && bSessionValid)
	{
		_snprintf_s(cBufferFormat,64,_TRUNCATE,"%d", theApp.m_pMainFrame->m_iHistoryStateReadyCount);	
		sMessage = cBufferFormat;
		SetMessage(&sMessage);
		return;
	}

	// Project report completed
    pcFound = (char *) strstr(cBuffer, pcFindProjectCompleted);
	if (pcFound && bSessionValid)
	{
		pDoc->OnExtraDonetworkcommunication();
	}

	// properties data
    pcFound = (char *) strstr(cBuffer, pcFindGetProperties);
	if (pcFound && bSessionValid)
	{
		if (g_pcWebServerProperties)
		{
			sMessage = g_pcWebServerProperties;
			delete g_pcWebServerProperties;
			g_pcWebServerProperties = NULL;
		}
		SetMessage(&sMessage);
		return;
	}

	// Project operations
    pcFound = (char *) strstr(cBuffer, pcFindProjectOperation);
	if (pcFound && bSessionValid)
	{
		int iOperation = -1;
		pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedProjectView));

		if (pView != NULL)
		{
			pcFound+= strlen(pcFindProjectOperation);
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationUpdate);
			if (pcFoundSub)	iOperation = PROJECT_OP_UPDATE;
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationSuspend);
			if (pcFoundSub)	iOperation = PROJECT_OP_SUSPEND;
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationResume);
			if (pcFoundSub)	iOperation = PROJECT_OP_RESUME;
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationNoMoreWork);
			if (pcFoundSub)	iOperation = PROJECT_OP_NOMOREWORK;
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationAllowMoreWork);
			if (pcFoundSub)	iOperation = PROJECT_OP_ALLOWMOREWORK;

			if (iOperation > 0)	pView->SendMessage(UWM_MSG_WEB_OPERATION, iOperation, 0);
		}
		sMessage = "";
		SetMessage(&sMessage);
		return;
	}

	// Transfer operations
    pcFound = (char *) strstr(cBuffer, pcFindTransferOperation);
	if (pcFound && bSessionValid)
	{
		int iOperation = -1;
		pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTransferView));

		if (pView != NULL)
		{
			pcFound+= strlen(pcFindTransferOperation);
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationRetry);
			if (pcFoundSub)	iOperation = TRANSFER_OP_RETRY;
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationRetryAll);
			if (pcFoundSub)	iOperation = TRANSFER_OP_RETRY_ALL;
			pcFoundSub = (char *) strstr(pcFound, pcFindOperationAbort);
			if (pcFoundSub)	iOperation = TRANSFER_OP_ABORT;
			if (iOperation > 0)	pView->SendMessage(UWM_MSG_WEB_OPERATION, iOperation, 0);
		}
		sMessage = "";
		SetMessage(&sMessage);
		return;
	}

	// Filter
    pcFound = (char *) strstr(cBuffer, pcFindSelect);
	if (pcFound && bSessionValid)
	{
		pcFoundSub = (char *) strstr(pcFound, pcFindSelectMultiple);
		if (pcFoundSub) m_bWebServerSelectMultiple = true;
		else
		{
			pcFoundSub = (char *) strstr(pcFound, pcFingSelectSingle);
			if (pcFoundSub) m_bWebServerSelectMultiple = false;
			else
			{
				pcFoundSub = (char *) strstr(pcFound, pcFindSelectFilter);
				if (pcFoundSub)
				{
					pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
					if (pView)
					{
						CHtmlExchange htmlExchange;
						htmlExchange.m_iOption = WEBSERVER_SELECT_FILTER;
						htmlExchange.m_bSendData = false;
						pView->SendMessage(UWM_MSG_WEB_ITEM_SELECTED, 0, (LPARAM) &htmlExchange); // WEBSERVER_SELECT_FILTER);
					}
				}
			}
		}
	}


	// Filter dialog
    pcFound = (char *) strstr(cBuffer, pcFindGetFilter);
	if (pcFound && bSessionValid)
	{
		pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));

		if (pView != NULL)
		{
			CreateHtmlWebServerFilter webServerFilter;
			CString sFilter;
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
			sMessage = sFilter;
		}
		SetMessage(&sMessage);
		return;
	}

    pcFound = (char *) strstr(cBuffer, pcFindSetFilter);
	if (pcFound && bSessionValid)
	{
		pcFound+= strlen(pcFindSetFilter);
		pView = pViewGetView(pDoc, RUNTIME_CLASS(CRemoteCombinedTaskView));
		if (pView != NULL)
		{
			pView->SendMessage(UWM_MSG_WEB_FILTER, TRUE, (LPARAM) pcFound);
		}
	}

	// sorting
    pcFound = (char *) strstr(cBuffer, pcFindSort);
	if (pcFound && bSessionValid)
	{
		char *pcHtml = NULL;
		pcFound+= strlen(pcFindSort);
		pView = pViewGetView(pDoc, NULL);

		if ((pView != NULL))
		{
			pcHtml = (char *) pView->SendMessage(UWM_MSG_WEB_SORT, TRUE, (LPARAM) pcFound);
			if (pcHtml)
			{
				sMessage = pcHtml;
				SetMessage(&sMessage);;
			}
		}
		delete pcHtml;
	}

	// Graph select computer popup
    pcFound = (char *) strstr(cBuffer, pcFindPopupGraphComputer);
	if (pcFound)
	{
		CString sData;
		CreateHtmlWebServerGraphComputerSelect createHtmlWebServerGraphComputerSelect;
		createHtmlWebServerGraphComputerSelect.Create(pDoc, &sData);
		sMessage = sData;
		SetMessage(&sMessage);;
		return;	
	}

	// Graph select computer item
    pcFound = (char *) strstr(cBuffer, pcFindSelectGraphComputer);
	if (pcFound)
	{
		pcFound+= strlen(pcFindSelectGraphComputer);
		pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SELECT_COMPUTER, (LPARAM) pcFound);
		sMessage = "";
		SetMessage(&sMessage);
		return;	
	}

	// Graph select project popup
    pcFound = (char *) strstr(cBuffer, pcFindPopupGraphProject);
	if (pcFound)
	{
		CString sData;
		int		iSingle;

		CreateHtmlWebServerGraphProjectSelect createHtmlWebServerGraphProjectSelect;
		iSingle = (int) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_PROJECT_MODE, 0);
		createHtmlWebServerGraphProjectSelect.Create(pDoc, &sData, (iSingle == TRUE));
		sMessage = sData;
		SetMessage(&sMessage);
		return;	
	}

	// Graph select project item
    pcFound = (char *) strstr(cBuffer, pcFindSelectGraphProject);
	if (pcFound)
	{
		pcFound+= strlen(pcFindSelectGraphProject);
		pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SELECT_PROJECT, (LPARAM) pcFound);
		sMessage = "";
		SetMessage(&sMessage);
		return;	
	}

	// Graph select single / multiple items
    pcFound = (char *) strstr(cBuffer, pcFindSingleGraphProjects);
	if (pcFound)
	{
		pcFound+= strlen(pcFindSingleGraphProjects);
		pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SET_PROJECT_MODE, 0);
		sMessage = "";
		SetMessage(&sMessage);
		return;	
	}

	// Graph set type buttons
    pcFound = (char *) strstr(cBuffer, pcFindSetGraphType);
	if (pcFound)
	{
		pcFound+= strlen(pcFindSetGraphType);
		pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_SET_TYPE, (LPARAM) pcFound);
		sMessage = "";
		SetMessage(&sMessage);
		return;	
	}

	// Graph get type buttons
    pcFound = (char *) strstr(cBuffer, pcFindGetGraphType);
	if (pcFound)
	{
		CreateHtmlWebServerGraph createHtmlWebServerGraph;
		CComputerGraphicType *pwebType;
		pwebType = (CComputerGraphicType *) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_TYPE, 0);
		sMessage = createHtmlWebServerGraph.CreateTypeData(pwebType);
		SetMessage(&sMessage);
		return;	
	}

    pcFound = (char *) strstr(cBuffer, pcFindColumnTasks);
	if (pcFound)
	{
		pcFound+= strlen(pcFindColumnTasks);
		CString *psColumn = new CString;
		*psColumn = pcFound;
		pDoc->m_pTaskView->SendMessage(UWM_MSG_SET_CLOUD_COLUMN, 0, (LPARAM) psColumn);
		return;
	}


	// get / set fonts
/*
    pcFound = (char *) strstr(cBuffer, pcFindFontHeader);
	if (pcFound)
	{
		int iFont;
		pcFound+= strlen(pcFindFontHeader);
		iFont = atoi(pcFound);
		iFont = (int) theApp.m_pDlgSettingsMobile->SendMessage(UWM_MSG_DLG_SETTINGS_MOBILE, MOBILE_FONT_HEADER, iFont);
		_snprintf_s(cBufferFormat,64,_TRUNCATE,"%d", iFont);	
		sMessage = cBufferFormat;
		SetMessage(&sMessage);
		return;	
	}
    pcFound = (char *) strstr(cBuffer, pcFindFontText);
	if (pcFound)
	{
		int iFont;
		pcFound+= strlen(pcFindFontText);
		iFont = atoi(pcFound);
		iFont = (int) theApp.m_pDlgSettingsMobile->SendMessage(UWM_MSG_DLG_SETTINGS_MOBILE, MOBILE_FONT_TEXT, iFont);
		_snprintf_s(cBufferFormat,64,_TRUNCATE,"%d", iFont);	
		sMessage = cBufferFormat;
		SetMessage(&sMessage);
		return;	
	}
*/

	SetMessage(&sMessage);
	return;

}

/*
void CWebServerProcess::Start(CWebServer2 *pWebServer, int iOs, string sPort, int iPort)
{
//	if ((iOs == OS_SELECTED_MAC) | (iOs == OS_SELECTED_LINUX))
//	{
		pWebServer->Connect(this, iPort);
//	}
//	else
//	{
//		CWebServer	webServer;
//		webServer.Connect(this, &sPort);
//	}
}
*/

BYTE *CWebServerProcess::Process(string *psRecieved, int *piLen)
{
	bool	bOK = true;
	string	sLine, sRecieved;
	BYTE	*psSend;

	sRecieved = *psRecieved;

	ResetSend();

	m_htmlHeaderRecieve.m_iMethod = HTML_METHOD_CODE_UNDEFINED;
	m_htmlHeaderRecieve.m_sUrl = "";
	m_htmlHeaderRecieve.m_sAccept = "";
	m_htmlHeaderRecieve.m_sHost = "";
	m_htmlHeaderRecieve.m_sUserAgent = "";
	m_htmlHeaderRecieve.m_sCookieSession = "";

	bOK = GetLine(psRecieved, &sLine);
	if (bOK) ProcessMethod(&sLine);

	while (bOK)
	{
		bOK = GetLine(psRecieved, &sLine);
		ProcessLine(&sLine);
	}

	switch (m_htmlHeaderRecieve.m_iMethod)
	{
		case HTML_METHOD_CODE_GET:
			Get();
		break;
		case HTML_METHOD_CODE_POST:
			Post();
		break;
	}
	psSend = Finish(piLen);

	return psSend;
}

bool CWebServerProcess::GetLine(string *psRecieved, string *psLine)
{
	int	iPos;
	iPos = (int) psRecieved->find(m_sCrLf);
	if (iPos != string::npos)
	{
		*psLine = psRecieved->substr(0,iPos);
		*psRecieved = psRecieved->substr(iPos+2);
		return true;
	}
	*psLine = "";
	return false;
}

void CWebServerProcess::ProcessMethod(string *psLine)
{
	int	iPos, iPosEnd;
	iPos = (int) psLine->find(HTML_METHOD_GET);
	if (iPos != string::npos)
	{
		m_htmlHeaderRecieve.m_iMethod = HTML_METHOD_CODE_GET;
		string sMethod;
		iPos += (int) strlen(HTML_METHOD_GET);
		sMethod = psLine->substr(iPos);
		StripSpace(&sMethod);
		iPos = (int) sMethod.find("/");
		if (iPos != string::npos)
		{
			iPosEnd = (int) sMethod.find(" "); 
			if (iPosEnd != string::npos)
			{
				m_htmlHeaderRecieve.m_sUrl = sMethod.substr(iPos+1,iPosEnd-1);
				m_htmlHeaderRecieve.m_sParameter = ExtractParameter(&m_htmlHeaderRecieve.m_sUrl);
			}
		}
		return;
	}
	else
	{
		iPos = (int) psLine->find(HTML_METHOD_POST);
		if (iPos != string::npos)
		{
			m_htmlHeaderRecieve.m_iMethod = HTML_METHOD_CODE_POST;
			string sMethod;
			iPos += (int) strlen(HTML_METHOD_POST);
			sMethod = psLine->substr(iPos);
			StripSpace(&sMethod);
			iPos = (int) sMethod.find("/");
			if (iPos != string::npos)
			{
				iPosEnd = (int) sMethod.find(" "); 
				if (iPosEnd != string::npos)
				{
					m_htmlHeaderRecieve.m_sUrl = sMethod.substr(iPos+1,iPosEnd-1);
					ReplaceCode(&m_htmlHeaderRecieve.m_sUrl);
					m_htmlHeaderRecieve.m_sParameter = ExtractParameter(&m_htmlHeaderRecieve.m_sUrl);
				}
			}
			return;
		}
	}
}

void CWebServerProcess::ProcessLine(string *psLine)
{
	int	iPos;

	iPos = (int) psLine->find(HTML_HEADER_HOST);
	if (iPos != string::npos)
	{
		iPos += (int) strlen(HTML_HEADER_HOST);
		m_htmlHeaderRecieve.m_sHost = psLine->substr(iPos);
		return;
	}
	iPos = (int) psLine->find(HTML_HEADER_USER_AGENT);
	if (iPos != string::npos)
	{
		iPos += (int) strlen(HTML_HEADER_USER_AGENT);
		m_htmlHeaderRecieve.m_sUserAgent = psLine->substr(iPos);
		m_htmlHeaderRecieve.m_sHost;
		return;
	}
	iPos = (int) psLine->find(HTML_HEADER_ACCEPT);
	if (iPos != string::npos)
	{
		iPos += (int) strlen(HTML_HEADER_ACCEPT);
		m_htmlHeaderRecieve.m_sAccept = psLine->substr(iPos);
		return;
	}
	iPos = (int) psLine->find(HTML_HEADER_COOKIE);
	if (iPos != string::npos)
	{
		iPos += (int) strlen(HTML_HEADER_COOKIE);

		string sCookie;
		sCookie = psLine->substr(iPos);

		iPos = (int) sCookie.find(HTML_COOKIE_SESSION);
		if (iPos != string::npos)
		{
			iPos += (int) strlen(HTML_COOKIE_SESSION);
			m_htmlHeaderRecieve.m_sCookieSession = sCookie.substr(iPos);
		}
		return;
	}
}

void CWebServerProcess::StripSpace(string *psLine)
{
	int iLen = (int) strlen(psLine->c_str());
	while (iLen > 0)
	{
		if (psLine->at(0) == ' ')
		{
			*psLine = psLine->substr(1);
			iLen = (int) strlen(psLine->c_str());
		}
		else return;
	}
}

void CWebServerProcess::ReplaceCode(string *psText)
{
	StringReplace(psText, "%3C", "<");
	StringReplace(psText, "%3E", ">");
	StringReplace(psText, "%20", " ");
}

string CWebServerProcess::ExtractParameter(string *psUrl)
{
	int iPos;

	iPos = (int) psUrl->find("?");
	if (iPos != string::npos)
	{
		string sParameter;
		sParameter = psUrl->substr(iPos+1, string::npos);
		*psUrl = psUrl->substr(0, iPos);
		return sParameter;
	}
	return "";
}

void CWebServerProcess::SetMimeType(string sMime, bool bBinairy)
{
	m_htmlHeaderSend.m_sMime = "Content-Type: ";
	m_htmlHeaderSend.m_sMime+= sMime;
	m_htmlHeaderSend.m_bBinairy = bBinairy;
}

void CWebServerProcess::SetMessageLength(int iLen)
{
	char cBuffer[64];

	_snprintf_s(cBuffer,64,_TRUNCATE,"Content-Length: %d", iLen);
	m_htmlHeaderSend.m_sContentLength = cBuffer;
}

void CWebServerProcess::SetMessage(string *psMessage)
{
	int iLen;
	iLen = (int) psMessage->length();
	SetMessageLength(iLen);
	m_htmlHeaderSend.m_sContent = *psMessage;
}

#define FILE_READ_TEMP_BUFFER 32768
//32768
bool CWebServerProcess::SetMessageFile(string sFileNamePart)
{
	CFile	file;
	CFileException ex;
	CFileStatus	fileStatus;
	string	sFileName;
	int		iReadCount, iTotalReadCount;
	BYTE	bBuffer[FILE_READ_TEMP_BUFFER+1], *pbBufferTemp;

	if (m_pbFileDataBuffer == NULL)
	{
		m_iFileDataBuferLen = FILE_READ_TEMP_BUFFER*8 +1;
		m_pbFileDataBuffer = new BYTE [m_iFileDataBuferLen];
	}
	
	sFileName = m_cWebFolderBuffer;
	sFileName+= sFileNamePart;

	m_htmlHeaderSend.m_pbContent = NULL;
	iTotalReadCount = 0;
	if (file.Open(sFileName.c_str(), CFile::typeBinary | CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		file.GetStatus(fileStatus);
		fileStatus.m_mtime.GetGmtTm(&m_htmlHeaderSend.m_tmtime);
		m_htmlHeaderSend.m_bTimeValid = true;

		iReadCount = file.Read(bBuffer, FILE_READ_TEMP_BUFFER);
		while (iReadCount)
		{
			if ((iTotalReadCount + iReadCount) > m_iFileDataBuferLen)
			{
				m_iFileDataBuferLen += FILE_READ_TEMP_BUFFER +1;
				pbBufferTemp = m_pbFileDataBuffer;
				m_pbFileDataBuffer = new BYTE [m_iFileDataBuferLen];
				memcpy(m_pbFileDataBuffer, pbBufferTemp, iTotalReadCount); // copy everthing from the old buffer in the new one
				delete pbBufferTemp;
			}

			memcpy(m_pbFileDataBuffer+iTotalReadCount, bBuffer, iReadCount); 
			iTotalReadCount += iReadCount;
			iReadCount = file.Read(bBuffer, FILE_READ_TEMP_BUFFER);
		}
		file.Close();
	}
	else
	{
		string sError;
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		return false;
	}
	*(m_pbFileDataBuffer+iTotalReadCount) = 0;

	if (m_htmlHeaderSend.m_bBinairy)
	{
		SetMessageLength(iTotalReadCount);
		m_htmlHeaderSend.m_iByteContentLength = iTotalReadCount;
		m_htmlHeaderSend.m_pbContent = m_pbFileDataBuffer;
	}
	else
	{
		string sMessage;
		sMessage = (char *) m_pbFileDataBuffer;
		SetMessage(&sMessage);
		m_htmlHeaderSend.m_bBinairy = false;
		m_htmlHeaderSend.m_pbContent = NULL;
	}
	return true;
}

void CWebServerProcess::ResetSend()
{
//	;

	m_htmlHeaderSend.m_pbContent = NULL;
	m_htmlHeaderSend.m_bBinairy = false;
	m_htmlHeaderSend.m_iByteContentLength = 0;
	m_htmlHeaderSend.m_sContent = "";
//	_time64(&tSystemTime);
//	gmtime_s(&m_htmlHeaderSend.m_tmtime, &tSystemTime);
	m_htmlHeaderSend.m_bTimeValid = false;
}

BYTE *CWebServerProcess::Finish(int *piLen)
{
	int		iLen;
	string	sSend;
	char	cBuffer[64];
	time_t tSystemTime;
	struct	tm   tmtime;
	char	*cLocale;
	BYTE	*pBufferBegin, *pBuffer;

	sSend += "HTTP/1.1 200 OK";
	AddCrLf(&sSend);

	cLocale = setlocale(LC_TIME, "English");
//  Date: Mon, 23 May 2005 22:38:34 GMT
	_time64(&tSystemTime);
	gmtime_s(&tmtime, &tSystemTime);
	strftime(cBuffer, 64, "%a, %d %b %Y %H:%M:%S GMT", &tmtime);
	sSend += "Date: ";
	sSend += cBuffer;
	AddCrLf(&sSend);

	sSend += "eFMer BoincTasks Mobile";
	AddCrLf(&sSend);

	if (m_htmlHeaderSend.m_bTimeValid)
	{
		strftime(cBuffer, 64, "%a, %d %b %Y %H:%M:%S GMT", &m_htmlHeaderSend.m_tmtime);
		sSend += "Last-Modified: ";
		sSend += cBuffer;
		AddCrLf(&sSend);
	}
	else
	{
		sSend += "Cache-Control: no-cache";
		AddCrLf(&sSend);

		strftime(cBuffer, 64, "%a, %d %b %Y %H:%M:%S GMT", &tmtime);
		sSend += "Expires: ";
		sSend += cBuffer;
		AddCrLf(&sSend);
	}

//Expires: Mon 06 Oct 2008 12:43:48 GMT



// Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT
	//Sun, 06 Nov 1994 08:49:37 GMT

	sSend += m_htmlHeaderSend.m_sMime;
	AddCrLf(&sSend);
	sSend += m_htmlHeaderSend.m_sContentLength;
	AddCrLf(&sSend);
	sSend += "Connection: keep-alive";
	AddCrLf(&sSend);
	AddCrLf(&sSend);

	if (!m_htmlHeaderSend.m_bBinairy)
	{
		sSend += m_htmlHeaderSend.m_sContent;
	}

	int iToReserve;

	iToReserve = m_htmlHeaderSend.m_iByteContentLength;
	iToReserve+= (int) sSend.length();
	iToReserve+= 100;

	pBufferBegin = new BYTE [iToReserve];
	pBuffer = pBufferBegin;

	iLen = (int) sSend.length();
	*piLen = iLen;
	memcpy(pBufferBegin, sSend.c_str(), iLen);

	pBuffer += iLen;
	*piLen += m_htmlHeaderSend.m_iByteContentLength;
	memcpy(pBuffer, m_htmlHeaderSend.m_pbContent, m_htmlHeaderSend.m_iByteContentLength);
	pBuffer += m_htmlHeaderSend.m_iByteContentLength;
	iLen = (int) m_sCrLf.length();
	*piLen += iLen;
	memcpy(pBuffer, m_sCrLf.c_str(), iLen);
	pBuffer += iLen;
	*pBuffer = 0;	// add 0 just in case

	m_htmlHeaderSend.m_pbContent = NULL;

	setlocale(LC_TIME, cLocale);

	return pBufferBegin;
}

void CWebServerProcess::AddCrLf(string *psSend)
{
	*psSend += m_sCrLf;
}

void CWebServerProcess::ResetPassword()
{
	CBlowFish bf;
	string sPassword;
	char cHex[2048*2];

	cHex[0] = 0;
	m_sAWebServerPasswordClear = theApp.m_pDlgSettingsMobile->m_sPassword;
	sPassword = theApp.GetProfileString(registrySectionGeneral, registryWWWPassword, "");
	bf.MakeHex((unsigned char *) sPassword.c_str(), cHex, 2048*2);
	m_sAWebServerPasswordHex = cHex;

	::GenerateRandomStringHex(m_cRandomString,128);

	if (m_sAWebServerPasswordHex.length() == 0)
	{
		m_iWebServerLogin = WEBSERVER_LOGIN_NO_PASSWORD;
	}
	else
	{
		m_iWebServerLogin = WEBSERVER_LOGIN_AUTO;
	}
}


void CWebServerProcess::GetComputerTree(string *psData)
{
	CString sData;
	CCreateHtmlWebServerComputerSelect createHtmlWebServerComputerSelect;
	std::deque<CBarTreeGroupItem*> *plListBarTree;
	plListBarTree = (std::deque<CBarTreeGroupItem*> *) theApp.m_pMainFrame->m_dlgBarComputer.SendMessage(UWM_MSG_BAR_COMPUTER_GET_TREE, 0, 0);
	createHtmlWebServerComputerSelect.Create(plListBarTree, &sData);
	*psData = sData;
}

CString CWebServerProcess::ExtractXmlTag(char **pcFind)
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

CView *CWebServerProcess::pViewGetView(CDocument *pDoc, CRuntimeClass *pClass)
{
	CView		*pView;
	POSITION	posView;
	posView = pDoc->GetFirstViewPosition();
	while (posView)
	{
		pView = pDoc->GetNextView(posView);
		if (pView != NULL) break;		
	}
	if (pView != NULL)
	{
		if (pClass != NULL)
		{
			if (!pView->IsKindOf(pClass)) pView = NULL;
		}
	}
	return pView;
}

BEGIN_MESSAGE_MAP(CWebServerProcess, CWnd)
END_MESSAGE_MAP()



// CWebServerProcess message handlers



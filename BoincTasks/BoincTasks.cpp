// BoincTasks.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <direct.h>
#include "Iphlpapi.h" //
#include <winsock2.h>
#include <ws2tcpip.h>
#include "registry.h"

#include "BoincTasks.h"
#include "MainFrm.h"

#include "RemoteDocTemplate.h"

#include "ChildFrm.h"
#include "CombinedChildFrm.h"
#include "ListViewEx.h"					// base class for BoincTasksView
#include "TemplateRemoteHistoryView.h"
#include "TemplateRemoteTaskView.h"		// base class for BoinCComputerView
#include "RemoteCombinedTaskView.h"
#include "TemplateRemoteProjectView.h"
#include "TemplateRemoteMessageView.h"
#include "TemplateRemoteTransferView.h"
#include "RemoteCombinedDoc.h"
#include "dlgupdate.h"
#include "aboutdlg.h"
#include "DlgFloating.h"

#include "DlgComputerLocation.h"
#include "dlgLogging.h"
#include "dlgRulesLogging.h"
#include "dlgProperties.h"
#include "dlgGraphic.h"
#include "DlgColorMain.h"
#include "DlgColorGeneral.h"
#include "DlgColorProjects.h"
#include "DlgColorTasks.h"
#include "DlgColorWarnings.h"
#include "DlgColorMessages.h"
#include "DlgColorHistory.h"
#include "DlgColorGraphic.h"
#include "DlgColorTemperature.h"
#include "DlgFont.h"

#include "DlgProxyMain.h"
#include "DlgProxyHttp.h"
#include "DlgProxySocks.h"

#include "DlgSettingsMain.h"
#include "DlgSettingsGeneral.h"
#include "DlgSettingsView.h"
#include "DlgSettingsProject.h"
#include "DlgSettingsTasks.h"
#include "DlgSettingsHistory.h"
#include "DlgSettingsNotices.h"
#include "DlgSettingsWarning.h"
#include "DlgSettingsGadget.h"
#include "DlgSettingsWWW.h"
#include "DlgSettingsMobile.h"
#include "DlgSettingsExpert.h"
#include "DlgSettingsRules.h"
#include "DlgSettingsMessage.h"
#include "DlgSettingsTransfer.h"
#include "DlgAddProject.h"
#include "DlgAddAccountManager.h"
#include "DlgSetDebt.h"

#include "ThreadWebServer.h"
#include "ThreadGarbageCollector.h"
#include "ThreadBoincClient.h"
#include "ThreadIpSearch.h"
#include "Xml.h"
#include "DlgYesNo.h"
#include "computer.h"

#include "DlgScheduler.h"

#include "MultiLanguage.h"
#include "Translation.h"

#include "Xml.h"

#include "DbgHelp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool	g_bSilentReboot = false;
char	g_cFileNameLog[MAX_PATH+32];
//bool	g_bBoincClientDebtValid = false;




void StringReplace(std::string *psSource, char *pcToBeReplaced, char *psToBeReplacedWith)
{
	std::string sToBeReplaced, sToReplaceWidth;
	sToBeReplaced = pcToBeReplaced;
	sToReplaceWidth = psToBeReplacedWith;
	StringReplace(psSource, &sToBeReplaced, &sToReplaceWidth);
}


void StringReplace(std::string *psSource, std::string *psToBeReplaced, std::string *psToBeReplacedWith)
{
	bool bFound;

	bFound = true;
	while (bFound)
	{
		int iFound = (int) psSource->find(*psToBeReplaced);
		if (iFound != std::string::npos)
		{
			int iLenReplace = (int) psToBeReplaced->length();
			psSource->replace(iFound, (iLenReplace), *psToBeReplacedWith);
		}
		else
		{
			bFound = false;
		}
	}
}

int CompareDouble(double d1, double d2, int imul)
{
	int i1;
	int i2;

	d1 *= imul;
	d2 *= imul;
	i1 = (int) ceil(d1-0.5);
	i2 = (int) ceil(d2-0.5);

	if (i1 > i2) return COMPARE_LARGER;
	if (i1 < i2) return COMPARE_SMALLER;

	return COMPARE_EQUAL;

}

void GenerateRandomString(char *s, int len)
{
	char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()_+=-`:;|<>?,./|";

	srand ((unsigned int) time(NULL) );
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

void GenerateRandomStringHex(char *s, int len)
{
	char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	srand ((unsigned int) time(NULL) );
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

bool ReadFromFile(CString *psText, CString *psFileName)
{
	int		iReadCount;
	CFile	file;
	BYTE	bBuffer[50001];
	if (file.Open(psFileName->GetBuffer(), CFile::modeRead))
	{
		iReadCount = file.Read(bBuffer, 50000);
		if (iReadCount > 0)
		{	
			bBuffer[iReadCount] = 0;
			*psText = bBuffer;
		}
		file.Close();
		psFileName->ReleaseBuffer();
		return true;
	}
	psFileName->ReleaseBuffer();
	return false;
}


VOID WriteDebugLog(char* pFileName, char* pMsg, bool bNew)
{
	FILE* pLog;

	if (strlen(pFileName) == 0) return;

	pLog = NULL;
	try
	{
		SYSTEMTIME oT;
		::GetLocalTime(&oT);
		if (bNew)
		{
			fopen_s(&pLog, pFileName,"w");
		}
		else		fopen_s(&pLog, pFileName,"a");
		if (pLog)
		{
			fprintf(pLog,"%02d/%02d/%04d, %02d:%02d:%02d -- %s",oT.wMonth,oT.wDay,oT.wYear,oT.wHour,oT.wMinute,oT.wSecond,pMsg); 
			fclose(pLog);
		}
	} catch(...) {}
}

void myInvalidParameterHandler(const wchar_t* expression,  const wchar_t* function,   const wchar_t* file,  unsigned int line, uintptr_t pReserved)
{
	CString sFormat;

	sFormat.Format("Invalid parameter detected in function %S File: %S Line: %d\r\nExpression: %S", function, file, line, expression);
	AfxMessageBox(sFormat);
	g_bSilentReboot = false;
	abort();
}


CString GetDumpFileName()
{
	CString sTxt, sVersion, sDirectory;
	CXml	xml("");

	CString sPath;
	sPath = theApp.GetProfileString(registrySectionSettings,registryPath, "");

	sDirectory = xml.GetUserDirectory(sPath,false );
	sDirectory += "\\crash\\";
	_mkdir(sDirectory);

	time_t tSystemTime;
	_time64(&tSystemTime);

	CTime time(tSystemTime);
	sTxt = "BoincTasks_";
	sVersion.LoadString(IDS_PROGRAM_VERSION);
	sVersion.Remove('.');
	sTxt+= sVersion;
	sTxt+= "_";
	sTxt+= time.FormatGmt("%d-%m-%Y_%H_%M_%S");
	sTxt+= ".dmp";

	sTxt = sDirectory + sTxt;

	return sTxt;
}
//BoincTasks Startup
CString GetStartupLogFileName(CString sTxt)
{
	CString sVersion, sDirectory;
	CXml	xml("");

	CString sPath;
	sPath = theApp.GetProfileString(registrySectionSettings,registryPath, "");

	sDirectory = xml.GetUserDirectory(sPath,false );
	sDirectory += "\\log\\";
	_mkdir(sDirectory);

	time_t tSystemTime;
	_time64(&tSystemTime);

	CTime time(tSystemTime);
	sTxt = sDirectory + sTxt;
	return sTxt;
}

bool StartExe(CString *psFullFileName, CString *psDataDirectory)
{
	PROCESS_INFORMATION pi;
    STARTUPINFO         si;
    BOOL                bProcessStarted;

    memset(&pi, 0, sizeof(pi));
    memset(&si, 0, sizeof(si));

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    bProcessStarted = CreateProcess(
    NULL,
    psFullFileName->GetBuffer(),
    NULL,
    NULL,
    FALSE,
    CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW,
    NULL,
    *psDataDirectory,
    &si,
	&pi);
	return (bProcessStarted == TRUE);
}

void CreateMiniDump( EXCEPTION_POINTERS* pep ) 
{
	// Open the file 

	CString sFormat, sFile;

	sFile = GetDumpFileName();

	HANDLE hFile = CreateFile(sFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE; 

		MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | 
		                                          MiniDumpWithFullMemoryInfo | 
		                                          MiniDumpWithHandleData | 
		                                          MiniDumpWithThreadInfo | 
		                                          MiniDumpWithUnloadedModules ); 

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 ); 

		if( !rv ) 
		{
			sFormat.Format("Dump file: %s, failed. Error: %u", sFile, GetLastError()) ; 
		}
		else 
		{
			sFormat.Format("Dump file: %s, created succesfully",sFile); 
		}
		// Close the file 

		sFormat = "Sorry but BoincTasks has crashed.\r\n\r\n" + sFormat;
		sFormat = sFormat + "\r\n\r\n To help us solve this problem, please send the dump file (.dmp) to boinc@efmer.eu.";

		CloseHandle( hFile ); 
		if (!g_bSilentReboot) AfxMessageBox(sFormat);

	}
	else 
	{
		sFormat.Format("Dump file: %s file  failed. Error: %u", sFile, GetLastError() ); 
		if (!g_bSilentReboot) AfxMessageBox(sFormat);
	}

}


#if defined _M_X64 || defined _M_IX86
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI 
  MyDummySetUnhandledExceptionFilter(
  LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
  return NULL;
}
#else
#error "This code works only for x86 and x64!"
#endif
 
BOOL PreventSetUnhandledExceptionFilter()
{
  HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll")); 
  if (hKernel32 == NULL) return FALSE;
  void *pOrgEntry = GetProcAddress(hKernel32, 
    "SetUnhandledExceptionFilter");
  if(pOrgEntry == NULL) return FALSE;
 
  DWORD dwOldProtect = 0;
  SIZE_T jmpSize = 5;
#ifdef _M_X64
  jmpSize = 13;
#endif
  BOOL bProt = VirtualProtect(pOrgEntry, jmpSize, 
    PAGE_EXECUTE_READWRITE, &dwOldProtect);
  BYTE newJump[20];
  void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
#ifdef _M_IX86
  DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;
  dwOrgEntryAddr += jmpSize; // add 5 for 5 op-codes for jmp rel32
  DWORD dwNewEntryAddr = (DWORD) pNewFunc;
  DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;
  // JMP rel32: Jump near, relative, displacement relative to next instruction.
  newJump[0] = 0xE9;  // JMP rel32
  memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
#elif _M_X64
  // We must use R10 or R11, because these are "scratch" registers 
  // which need not to be preserved accross function calls
  // For more info see: Register Usage for x64 64-Bit
  // http://msdn.microsoft.com/en-us/library/ms794547.aspx
  // Thanks to Matthew Smith!!!
  newJump[0] = 0x49;  // MOV R11, ...
  newJump[1] = 0xBB;  // ...
  memcpy(&newJump[2], &pNewFunc, sizeof (pNewFunc));
  //pCur += sizeof (ULONG_PTR);
  newJump[10] = 0x41;  // JMP R11, ...
  newJump[11] = 0xFF;  // ...
  newJump[12] = 0xE3;  // ...
#endif
  SIZE_T bytesWritten;
  BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
    pOrgEntry, newJump, jmpSize, &bytesWritten);
 
  if (bProt != FALSE)
  {
    DWORD dwBuf;
    VirtualProtect(pOrgEntry, jmpSize, dwOldProtect, &dwBuf);
  }
  return bRet;
}
 
LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *lpTopLevelExceptionFilter)
{
	char		strExePath[MAX_PATH];
	CString		sPath, sFile, sData, sTxt;
	int			iFoundPos;

	CreateMiniDump( lpTopLevelExceptionFilter );
	if (!g_bSilentReboot) FatalAppExit(0, _T("Unhandled Exception occured!"));
	else
	{
		Sleep(5000);	// wait for 5 seconds to restart
		DWORD dwSize = GetModuleFileName (NULL, strExePath, MAX_PATH);	// not 0 terminated
		strExePath[dwSize] = 0;
		sPath = strExePath;
		iFoundPos = sPath.ReverseFind('\\');
		if (iFoundPos > 0)
		{
			int iShow;
			sData = sPath.Mid(0,iFoundPos);
			if (theApp.m_pMainWnd->IsWindowVisible())	iShow = 1;
			else iShow = 0;
			theApp.WriteProfileInt(registrySectionSettings,registryStartShow, iShow);
			if (!StartExe(&sPath, &sData))
			{
				sTxt = "ERROR Unable to restart BoincTasks: ";
				sTxt+= sPath;
				sTxt+= " in: ";
				sTxt+= sData;
				AfxMessageBox(sTxt);
			}
		}
	}
	return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH;
}



// CBoincTasksApp

BEGIN_MESSAGE_MAP(CBoincTasksApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CBoincTasksApp::OnAppAbout)
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
//	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
//	ON_COMMAND(ID_AUTOSTARTUP_SELECTEDCOMPUTERS, &CBoincTasksApp::OnAutostartup)
//	ON_UPDATE_COMMAND_UI(ID_AUTOSTARTUP_SELECTEDCOMPUTERS, &CBoincTasksApp::OnUpdateAutostartup)
//	ON_COMMAND(ID_AUTOSTARTUP_COMBINED, &CBoincTasksApp::OnAutostartupCombined)
//	ON_UPDATE_COMMAND_UI(ID_AUTOSTARTUP_COMBINED, &CBoincTasksApp::OnUpdateAutostartupCombined)
	ON_COMMAND(ID_HELP_MANUAL, &CBoincTasksApp::OnHelpManual)
	ON_COMMAND(ID_HELP_FORUM, &CBoincTasksApp::OnHelpForum)
	ON_COMMAND(ID_HELP_BOINCWEBSITE, &CBoincTasksApp::OnHelpBoincWebsite)
	ON_COMMAND(ID_FILE_CLOSEBOINCCLIENTANDALLSCIENCEAPPLICATIONS, &CBoincTasksApp::OnFileCloseboincclientandallscienceapplications)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSEBOINCCLIENTANDALLSCIENCEAPPLICATIONS, &CBoincTasksApp::OnUpdateFileCloseboincclientandallscienceapplications)
	ON_COMMAND(ID_FILE_STARTBOINCCLIENT, &CBoincTasksApp::OnFileStartboincclient)
	ON_UPDATE_COMMAND_UI(ID_FILE_STARTBOINCCLIENT, &CBoincTasksApp::OnUpdateFileOnFileStartboincclient)
	ON_COMMAND(ID_FILE_READ_COLOR_SETTINGS, &CBoincTasksApp::OnFileReadColorSettings)
	ON_COMMAND(ID_FILE_WRITE_COLOR_SETTINGS, &CBoincTasksApp::OnFileWriteColorSettings)

	ON_COMMAND(ID_HELP, &CBoincTasksApp::OnHelpManual)
END_MESSAGE_MAP()


// CBoincTasksApp construction

CBoincTasksApp::CBoincTasksApp()
{
	m_bExit = false;
	m_bBoincClientStartedByBoincTasks = false;

	m_bDocumentOpen = false;
	m_bDocumentFirstTimeOpen = true;
	
	m_pDlgFloating = NULL;

	m_bAutoRefreshCheat = false;
	m_iAutoRefreshTime = 0;
	m_iDownloadRefreshTime = 0;
	m_iUploadRefreshTime = 0;
	m_iRefreshRefreshTime = 0;
	m_iRefreshRefreshASAP = 0;

	m_bNoCleanup = false;

	m_bRegulatorActive = false;
	m_iRegulatorMaxTasks = 0;
	m_dRegulatorDaysWorkRequested = 0;
	m_dRegulatorMinDaysGpu = 0;
	m_dRegulatorMinDaysCpu = 0;
	m_dRegulatorDaysWorkRequestedMin = 0;

	m_pThreadGarbageHistory = NULL;
	m_pThreadGarbageLog = NULL;

	m_dwThreadPriority = THREAD_PRIORITY_NORMAL;

	m_iThreadTimeout = TIMEOUT_THREAD_LOCKED_DEFAULT;
}


// The one and only CBoincTasksApp object

CBoincTasksApp theApp;


// CBoincTasksApp initialization

BOOL CBoincTasksApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.

	int		iLangIdSelected;
	CString sLogName, sLogNameBak1, sLogNameBak2;
	
	sLogName = GetStartupLogFileName("BoincTasks Startup");
	sLogNameBak1 = sLogName + "1.log";
	sLogNameBak2 = sLogName + "2.log";
	sLogName += ".log";

	g_cFileNameLog[0] = 0;
	strcat_s(g_cFileNameLog, MAX_PATH+32, sLogName.GetBuffer());
	m_bDebug = true; 

	try
	{
		CFile::Remove(sLogNameBak2);
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}
	try
	{
		CFile::Rename(sLogNameBak1, sLogNameBak2);			// 1 -> 2
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}
	try
	{
		CFile::Rename(sLogName, sLogNameBak1);			// normal -> 1
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}

	CString sVersion, sTxt;
	sVersion.LoadString(IDS_PROGRAM_VERSION);
	sVersion = "BoincTasks Version: " + sVersion;
	sTxt = "Startup: " + sVersion + "\r\n";
	WriteDebugLog(g_cFileNameLog, sTxt.GetBuffer(), true);	sTxt.ReleaseBuffer();

	// break on memory allocation (debug) memory leaks
//	_crtBreakAlloc = 15109;

	CLoggingMessage sLog;


	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxInitRichEdit2();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey(_T("eFMer"));

	iLangIdSelected = GetProfileInt(registrySectionSettings, registryLanguage, 0);
	if (iLangIdSelected == 1033) SetLocale("English_United States.1252");
	else SetLocale("");

	WriteDebugLog(g_cFileNameLog, "Set exeption handler\r\n", false);
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	BOOL bRet = PreventSetUnhandledExceptionFilter();

	ULONG lLanguageNumber = 0;
	int iPosition;
	CString sArg = this->m_lpCmdLine ;
	CString sArgUpper;
	sArgUpper = sArg;
	sArgUpper.MakeUpper();
	sTxt = "Read arg: " + sArgUpper+ "\r\n";
	WriteDebugLog(g_cFileNameLog, sTxt.GetBuffer(), false); sTxt.ReleaseBuffer();

	iPosition = sArgUpper.Find("/LOCATION");
	if (iPosition != -1)
	{
		iPosition += 10;
		m_sLocation = sArg.GetBuffer() + iPosition;
		m_sLocation.Remove(L' ');
	}

	iPosition = sArgUpper.Find("/SHOW");
	if (iPosition != -1)
	{
		m_bShowAtStartup = true;
	}	
	else
	{
		m_bShowAtStartup = false;
	}

	sArg.ReleaseBuffer();

	theApp.m_sCPID = theApp.GetProfileString(registrySectionGeneral,registryCPID, "");

	/*
	WriteDebugLog(g_cFileNameLog, "Language detection\r\n", false);
	m_pDlgLogging = NULL;
	m_pMultiLanguage = new CMultiLanguage;

	sTxt.Format("Language registry: %d\r\n", iLangIdSelected);	WriteDebugLog(g_cFileNameLog, sTxt.GetBuffer(), false); sTxt.ReleaseBuffer();

	if (iLangIdSelected > 20) m_pMultiLanguage->AddLangId(iLangIdSelected);		// selected first
	else iLangIdSelected = 0;

	sTxt.Format("Language selected: %d\r\n", iLangIdSelected);	WriteDebugLog(g_cFileNameLog, sTxt.GetBuffer(), false); sTxt.ReleaseBuffer();

	m_pMultiLanguage->DetectLangID(&lIdUser, &lIdSystem);
	m_pMultiLanguage->AddLangId(1033);		// fallback is English
	m_pTranslation = new CTranslation;
	CString sTranslationLog;
	if (!m_pTranslation->Open(&sTranslationLog))
	{
		sTranslationLog = "Fatal error, missing file: BoincTasks_ENU.btlang\r\nTry installing BoincTasks again.\r\n" + sTranslationLog;
		AfxMessageBox(sTranslationLog);
		WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);
		return FALSE;
	}
	if (sTranslationLog.GetLength() > 0) WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);

	WriteDebugLog(g_cFileNameLog, "Read translation\r\n", false);
	if (!m_pTranslation->Read(&sTranslationLog))
	{
		m_pTranslation->Close();
		WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);
		WriteDebugLog(g_cFileNameLog, "Close translation, return FALSE\r\n", false);
		return FALSE;
	}
	m_pTranslation->Close();	
	WriteDebugLog(g_cFileNameLog, "Close translation, OK\r\n", false);

	LPCTSTR pstrOwnerClass = AfxRegisterWndClass(0);
	wndInvisible.CreateEx(0, pstrOwnerClass, "", WS_POPUP, CW_USEDEFAULT,	CW_USEDEFAULT,	CW_USEDEFAULT,	CW_USEDEFAULT, NULL, 0);

	m_pDlgLogging = new CDlgLogging;
	m_pDlgLogging->Create(IDD_DIALOG_LOG,&wndInvisible);

	WriteDebugLog(g_cFileNameLog, "Started logging\r\n", false);

	m_pDlgRulesLogging = new CDlgRulesLogging;
	m_pDlgRulesLogging->Create(IDD_DIALOG_LOG_RULES,&wndInvisible);

	WriteDebugLog(g_cFileNameLog, "Open translation override\r\n", false);
	if (m_pTranslation->Open(&sTranslationLog, true))	// override
	{
		sLog.m_pFrom = "Language"; sLog.m_pText = "BoincTasks_OVERRIDE.btlang, found.";m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		if (!m_pTranslation->Read(&sTranslationLog))
		{
			m_pTranslation->Close();
			WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);
			WriteDebugLog(g_cFileNameLog, "Close translation override, return FALSE\r\n", false);
			return FALSE;
		}
		m_pTranslation->Close();
	}
	WriteDebugLog(g_cFileNameLog, "Close translation override, OK\r\n", false);

	TCHAR szLangCodeUser[4]; szLangCodeUser[0] = 0;
	TCHAR szLangCodeSystem[4]; szLangCodeSystem[0] = 0; 
	TCHAR szLangCodeSelected[4]; szLangCodeSelected[0] = 0;
	TCHAR szLangCodeUsed[4]; szLangCodeUsed[0] = 0;
	CString sFormat;
	::GetLocaleInfoA(lIdUser, LOCALE_SABBREVLANGNAME, szLangCodeUser, 4);
	::GetLocaleInfoA(lIdSystem, LOCALE_SABBREVLANGNAME, szLangCodeSystem, 4);

	if (iLangIdSelected != 0) ::GetLocaleInfoA(iLangIdSelected, LOCALE_SABBREVLANGNAME, szLangCodeSelected, 4);
	else strcpy_s(szLangCodeSelected,"");

	::GetLocaleInfoA(m_pTranslation->m_lcid, LOCALE_SABBREVLANGNAME, szLangCodeUsed, 4);

	sFormat.Format("User: %d (%s), System: %d (%s), Selected: %d (%s) Used: %d (%s)", lIdUser, &szLangCodeUser[0], lIdSystem, szLangCodeSystem, iLangIdSelected, szLangCodeSelected, m_pTranslation->m_lcid, szLangCodeUsed);
	sLog.m_pFrom = "Language"; sLog.m_pText = sFormat.GetBuffer();m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
	sFormat.ReleaseBuffer();

	sFormat += "\r\n";
	WriteDebugLog(g_cFileNameLog, sFormat.GetBuffer(), false);
	*/

	LPCTSTR pstrOwnerClass = AfxRegisterWndClass(0);
	wndInvisible.CreateEx(0, pstrOwnerClass, "", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0);

	m_pDlgLogging = new CDlgLogging;
	m_pDlgLogging->Create(IDD_DIALOG_LOG, &wndInvisible);

	m_pDlgRulesLogging = new CDlgRulesLogging;
	m_pDlgRulesLogging->Create(IDD_DIALOG_LOG_RULES, &wndInvisible);

	if (!GetBoincTasksLanguage(iLangIdSelected))
	{
		return FALSE;
	}
	m_pDlgLogging->Translate();
	m_pDlgRulesLogging->Translate();

	m_hMutex = CreateMutex(0, FALSE, _T("eFMer BoincTasks - running"));

    // Failed to create the mutex object: there's an instance of this 
    // application running!
    if ((m_hMutex==0) || GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED)
    {
		WriteDebugLog(g_cFileNameLog, "Mutex set1\r\n", false);
		CloseHandle(m_hMutex);
		m_hMutex = 0;
		Sleep(2000);
		m_hMutex = CreateMutex(0, FALSE, _T("eFMer BoincTasks - running"));
		bool bFoundWindow = false;
		char buffer[256];
		CString sTxt;
		if ((m_hMutex==0) || GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED)
		{
			WriteDebugLog(g_cFileNameLog, "Mutex set2\r\n", false);
			HWND hWnd = ::GetTopWindow(0 );
			while ( hWnd )
			{
				::GetWindowText(hWnd,buffer,255);
				sTxt = buffer;
				if (sTxt.GetLength() > 0)
				{
					if (sTxt.Find("BoincTasks",0) >=0)
					{
						if (sTxt.Find("[",0) >=0)
						{
							if (sTxt.Find("Microsoft") < 0)	// not the debugger
							{
								::ShowWindow(hWnd,SW_SHOW);
								bFoundWindow = (::SetForegroundWindow(hWnd) != 0);
							}
						}
					}
				}
				hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
			}
			if (!bFoundWindow)
			{
//				AfxMessageBox("ERROR: eFMer BoincTasks is already running. \r\nYou can only run one copy of this program");
				WriteDebugLog(g_cFileNameLog, "Already running\r\n", false);
			}
			else
			{
				WriteDebugLog(g_cFileNameLog, "Show running BT\r\n", false);
			}
			return FALSE;
		}
	}
	WriteDebugLog(g_cFileNameLog, "Mutex is not set\r\n", false);

	int iExitOk;
	iExitOk = theApp.GetProfileInt(registrySectionGeneral,registryExitOk, TRUE);
	theApp.WriteProfileInt(registrySectionGeneral,registryExitOk, FALSE);

#ifdef DEBUG
	iExitOk = TRUE;
#endif

	if (iExitOk != TRUE)
	{
		WriteDebugLog(g_cFileNameLog, "BoincTasks didn't close down properly, restarting....\r\n", false);
		m_bRestart = true;
		Sleep(1000);	// wait for 1 second to unlock files?
		return FALSE;
	}
	m_bRestart = false;

	int iDumpCount;
	iDumpCount = DeleteFiles(CRASH_FOLDER, 60*60*24*14);	// 14 days

	if (iDumpCount >= 10)
	{
		CString sCrashError = "More than 10 crashes in the last 12 hours, aborting........\r\nCheck the crash files in C:/Users/username/AppData/Roaming/eFMer/BoincTasks/crash\r\n\r\nRemoving the crash files, of the last 12 hours, will allow you to start BoincTasks again.";
		AfxMessageBox(sCrashError);
		WriteDebugLog(g_cFileNameLog, sCrashError.GetBuffer(), false);
		return FALSE;
	}
	WriteDebugLog(g_cFileNameLog, "Deleted crash files\r\n", false);

	int	retval;
	WSADATA wsdata;
	retval = WSAStartup( MAKEWORD( 2, 2 ), &wsdata);
//	retval = WSAStartup( MAKEWORD( 1, 1 ), &wsdata);
	if (retval)
	{
		CString sFormat =  "Startup";
		sLog.m_pFrom = "WSAStartup failed"; sLog.m_pText = sFormat.GetBuffer();m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		sFormat.ReleaseBuffer();
	}
	m_bWSAStartup = true;
	WriteDebugLog(g_cFileNameLog, "WSAStartup OK\r\n", false);	

	m_pThreadIpSearch = (CThreadIpSearch *) AfxBeginThread(RUNTIME_CLASS(CThreadIpSearch));
	m_plLocalhostIp = GetLocalhostIp();
	m_pThreadIpSearch->PostThreadMessage(UWM_MSG_THREAD_START_IP_SCAN, (WPARAM)m_plLocalhostIp, 0);

// Translate 
	CMenu menu, *pTranslate;

	m_hMenuRemote = ::LoadMenu(m_hInstance, MAKEINTRESOURCE(IDR_BoincTasksRemote));
	menu.Attach(m_hMenuRemote);

	// File Menu
	menu.ModifyMenu(0,MF_BYPOSITION,0, gszTranslation[PosGroupMenuFile]);
	pTranslate = menu.GetSubMenu(0);
	pTranslate->ModifyMenu(ID_FILE_STARTBOINCCLIENT, MF_STRING, ID_FILE_STARTBOINCCLIENT, gszTranslation[PosGroupMenuFileStartBoinc]);
	pTranslate->ModifyMenu(ID_FILE_CLOSEBOINCCLIENTANDALLSCIENCEAPPLICATIONS, MF_STRING, ID_FILE_CLOSEBOINCCLIENTANDALLSCIENCEAPPLICATIONS, gszTranslation[PosGroupMenuFileStopBoinc]);
	pTranslate->ModifyMenu(ID_FILE_READ_COLOR_SETTINGS, MF_STRING, ID_FILE_READ_COLOR_SETTINGS, gszTranslation[PosGroupMenuFileReadColorConfig]);
	pTranslate->ModifyMenu(ID_FILE_WRITE_COLOR_SETTINGS, MF_STRING, ID_FILE_WRITE_COLOR_SETTINGS, gszTranslation[PosGroupMenuFileWriteColorConfig]);
	
	pTranslate->ModifyMenu(ID_TASKBAR_EXIT, MF_STRING, ID_TASKBAR_EXIT, gszTranslation[PosGroupMenuFileExit]);

	// View Menu
	menu.ModifyMenu(1,MF_BYPOSITION,0, gszTranslation[PosGroupMenuView]);
	pTranslate = menu.GetSubMenu(1);
	pTranslate->ModifyMenu(ID_VIEW_GRAPHIC_TOOLBAR, MF_STRING, ID_VIEW_GRAPHIC_TOOLBAR, gszTranslation[PosGroupMenuViewToolGraphic]);
	pTranslate->ModifyMenu(ID_VIEW_OPERATION_TOOLBAR, MF_STRING, ID_VIEW_OPERATION_TOOLBAR, gszTranslation[PosGroupMenuViewToolOperations]);
	pTranslate->ModifyMenu(ID_VIEW_VIEWSELECTIONBAR, MF_STRING, ID_VIEW_VIEWSELECTIONBAR, gszTranslation[PosGroupMenuViewViewSelection]);
	pTranslate->ModifyMenu(ID_VIEW_COMPUTER_BAR, MF_STRING, ID_VIEW_COMPUTER_BAR, gszTranslation[PosGroupMenuViewComputer]);
	pTranslate->ModifyMenu(ID_VIEW_PROJECT_BAR, MF_STRING, ID_VIEW_PROJECT_BAR, gszTranslation[PosGroupMenuViewProject]);
	pTranslate->ModifyMenu(ID_VIEW_TOOLBARALLOW, MF_STRING, ID_VIEW_TOOLBARALLOW, gszTranslation[PosGroupMenuViewAllow]);
	pTranslate->ModifyMenu(ID_VIEW_STATUS_BAR, MF_STRING, ID_VIEW_STATUS_BAR, gszTranslation[PosGroupMenuViewStatus]);

	// Computers
	menu.ModifyMenu(2,MF_BYPOSITION,0, gszTranslation[PosGroupMenuComputer]);
	pTranslate = menu.GetSubMenu(2);
//	pTranslate->ModifyMenu(ID_COMPUTERS_SHOW, MF_STRING, ID_COMPUTERS_SHOW, gszTranslation[PosGroupMenuComputersShow]);
	pTranslate->ModifyMenu(ID_COMPUTERS_DETECT, MF_STRING, ID_COMPUTERS_DETECT, gszTranslation[PosMenuComputerDetect]);
	pTranslate->ModifyMenu(ID_COMPUTERS_ADD, MF_STRING, ID_COMPUTERS_ADD, gszTranslation[PosMenuComputerAdd]);
	pTranslate->ModifyMenu(ID_COMPUTERS_REMOVE, MF_STRING, ID_COMPUTERS_REMOVE, gszTranslation[PosMenuComputerRemove]);
	// Show
	menu.ModifyMenu(3,MF_BYPOSITION,0, gszTranslation[PosGroupMenuShow]);
	pTranslate = menu.GetSubMenu(3);

	pTranslate->ModifyMenu(ID_SHOW_COMPUTERS, MF_STRING, ID_SHOW_COMPUTERS, gszTranslation[PosGroupMenuShowComputers]);
	pTranslate->ModifyMenu(ID_SHOW_PROJECTS, MF_STRING, ID_SHOW_PROJECTS, gszTranslation[PosGroupMenuShowProjects]);
	pTranslate->ModifyMenu(ID_SHOW_TASKS, MF_STRING, ID_SHOW_TASKS, gszTranslation[PosGroupMenuShowTasks]);
	pTranslate->ModifyMenu(ID_SHOW_TRANSFERS, MF_STRING, ID_SHOW_TRANSFERS, gszTranslation[PosGroupMenuShowTransfers]);
	pTranslate->ModifyMenu(ID_SHOW_MESSAGES, MF_STRING, ID_SHOW_MESSAGES, gszTranslation[PosGroupMenuShowMessages]);
	pTranslate->ModifyMenu(ID_SHOW_HISTORY, MF_STRING, ID_SHOW_HISTORY, gszTranslation[PosGroupMenuShowHistory]);
	pTranslate->ModifyMenu(ID_SHOW_HISTORYLONG, MF_STRING, ID_SHOW_HISTORYLONG, gszTranslation[PosGroupMenuShowHistoryLong]);
	pTranslate->ModifyMenu(ID_SHOW_NOTICES, MF_STRING, ID_SHOW_NOTICES, gszTranslation[PosGroupMenuShowNotices]);

	pTranslate->ModifyMenu(ID_SHOW_STATISTICS, MF_STRING, ID_SHOW_STATISTICS, gszTranslation[PosGroupMenuShowStatistics]);
	pTranslate->ModifyMenu(ID_SHOW_STATISTICS_TASKS, MF_STRING, ID_SHOW_STATISTICS_TASKS, gszTranslation[PosGroupMenuShowStatisticsTasks]);
	pTranslate->ModifyMenu(ID_SHOW_TEMPERATURE, MF_STRING, ID_SHOW_TEMPERATURE, gszTranslation[PosGroupMenuShowTemperature]);
	pTranslate->ModifyMenu(ID_SHOW_GRAPH_TRANSFER, MF_STRING, ID_SHOW_GRAPH_TRANSFER, gszTranslation[PosGroupMenuShowGraphTransfer]);
	pTranslate->ModifyMenu(ID_SHOW_GRAPH_DEADLINE, MF_STRING, ID_SHOW_GRAPH_DEADLINE, gszTranslation[PosGroupMenuShowGraphDeadline]);
	
	pTranslate->ModifyMenu(ID_HELP_SHOWLOG_REMOTE, MF_STRING, ID_HELP_SHOWLOG_REMOTE, gszTranslation[PosGroupMenuHelpLog]);
	pTranslate->ModifyMenu(ID_HELP_SHOWLOG_RULES, MF_STRING, ID_HELP_SHOWLOG_RULES, gszTranslation[PosGroupMenuHelpLogRules]);

	// Projects
	menu.ModifyMenu(4,MF_BYPOSITION,0, gszTranslation[PosGroupMenuProjects]);
	pTranslate = menu.GetSubMenu(4);
	pTranslate->ModifyMenu(ID_PROJECTS_ADDNEWPROJECT, MF_STRING, ID_PROJECTS_ADDNEWPROJECT, gszTranslation[PosGroupMenuProjectsAdd]);
	pTranslate->ModifyMenu(ID_PROJECTS_ADD_PROJECT_MANAGER, MF_STRING, ID_PROJECTS_ADD_PROJECT_MANAGER, gszTranslation[PosGroupMenuProjectsAddAccountM]);
//	pTranslate->ModifyMenu(ID_PROJECTS_SYNCHRONIZE, MF_STRING, ID_PROJECTS_SYNCHRONIZE, gszTranslation[PosGroupMenuProjectsSynchronize]);	

	pTranslate->ModifyMenu(ID_PROJECTS_SETDEBT, MF_STRING, ID_PROJECTS_SETDEBT, gszTranslation[PosGroupMenuProjectsSetDebt]);
	pTranslate->ModifyMenu(ID_EXTRA_DONETWORKCOMMUNICATION, MF_STRING, ID_EXTRA_DONETWORKCOMMUNICATION, gszTranslation[PosGroupMenuExtraUpdateAll]);

	pTranslate->InsertMenu(3, MF_BYPOSITION, ID_PROJECTS_SYNCHRONIZE_DIALOG, gszTranslation[PosGroupMenuProjectsSynchronize]); //"Read config file"

//	CMenu menuSynchronize;
//	menuSynchronize.CreateMenu();
//	m_hMenuSynchronize = menuSynchronize.GetSafeHmenu(); 
//	pTranslate->InsertMenu(3, MF_BYPOSITION | MF_POPUP,  (UINT_PTR) m_hMenuSynchronize, gszTranslation[PosGroupMenuProjectsSynchronize]);
//	menuSynchronize.Detach();

	// Extra
	menu.ModifyMenu(5,MF_BYPOSITION,0, gszTranslation[PosGroupMenuExtra]);
	pTranslate = menu.GetSubMenu(5);
	pTranslate->ModifyMenu(ID_EXTRA_SETTINGS, MF_STRING, ID_EXTRA_SETTINGS, gszTranslation[PosGroupMenuExtraSetting]);
	pTranslate->ModifyMenu(ID_COLORS_TASK, MF_STRING, ID_COLORS_TASK, gszTranslation[PosGroupMenuExtraColors]);
	pTranslate->ModifyMenu(ID_EXTRA_PREFERENCES, MF_STRING, ID_EXTRA_PREFERENCES, gszTranslation[PosGroupMenuExtraPreference]);

	pTranslate->ModifyMenu(ID_EXTRA_PROXY, MF_STRING, ID_EXTRA_PROXY, gszTranslation[PosGroupMenuExtraProxy]);

	pTranslate->ModifyMenu(6, MF_BYPOSITION, 0, gszTranslation[PosGroupPopUpTasksFilter]);
	pTranslate->ModifyMenu(ID_FILTERON_READYTOREPORT, MF_STRING, ID_FILTERON_READYTOREPORT, gszTranslation[PosGroupPopUpTasksFilterReadyTR]);
	pTranslate->ModifyMenu(ID_FILTERON_WAITINGTORUN, MF_STRING, ID_FILTERON_WAITINGTORUN, gszTranslation[PosGroupPopUpTasksFilterWaitingTR]);
	pTranslate->ModifyMenu(ID_FILTER_READYTOSTART, MF_STRING, ID_FILTER_READYTOSTART, gszTranslation[PosGroupPopUpTasksFilterReadyTS]);
	pTranslate->ModifyMenu(ID_FILTERON_UPLOAD, MF_STRING, ID_FILTERON_UPLOAD, gszTranslation[PosGroupPopUpTasksFilterUpload]);
	pTranslate->ModifyMenu(ID_FILTERON_DOWNLOAD, MF_STRING, ID_FILTERON_DOWNLOAD, gszTranslation[PosGroupPopUpTasksFilterDownload]);
	pTranslate->ModifyMenu(ID_FILTERON_COMPUTATIONERROR, MF_STRING, ID_FILTERON_COMPUTATIONERROR, gszTranslation[PosGroupPopUpTasksFilterComputation]);
	pTranslate->ModifyMenu(ID_FILTERON_SUSPENDED, MF_STRING, ID_FILTERON_SUSPENDED, gszTranslation[PosGroupPopUpTasksFilterSuspended]);
	pTranslate->ModifyMenu(ID_FILTERON_ABORTED, MF_STRING, ID_FILTERON_ABORTED, gszTranslation[PosGroupPopUpTasksFilterAborted]);
	pTranslate->ModifyMenu(ID_FILTERON_RUNNING, MF_STRING, ID_FILTERON_RUNNING, gszTranslation[PosGroupPopUpTasksFilterRunning]);

	pTranslate->ModifyMenu(ID_TASK_SHOWACTIVETASKS, MF_STRING, ID_TASK_SHOWACTIVETASKS, gszTranslation[PosGroupPopUpTasksShowActive]);
	pTranslate->ModifyMenu(ID_TASK_SHOW_NON_CPU_INTENSIVE, MF_STRING, ID_TASK_SHOW_NON_CPU_INTENSIVE, gszTranslation[PosGroupPopUpTasksShowNonCpuIntensive]);
	pTranslate->ModifyMenu(ID_TASK_SHOWCPU, MF_STRING, ID_TASK_SHOWCPU, gszTranslation[PosGroupPopUpTasksShowCpu]);
	pTranslate->ModifyMenu(ID_TASK_SHOWGPU, MF_STRING, ID_TASK_SHOWGPU, gszTranslation[PosGroupPopUpTasksShowGpu]);
	pTranslate->InsertMenu(11, MF_BYPOSITION | MF_SEPARATOR); 

	pTranslate->InsertMenu(12, MF_BYPOSITION | MF_SEPARATOR); 
	pTranslate->InsertMenu(12, MF_BYPOSITION, ID_COMPUTER_BENCHMARK_DIALOG, gszTranslation[PosGroupMenuExtraBenchmark]);
	pTranslate->InsertMenu(13, MF_BYPOSITION | MF_SEPARATOR);
	// config
	pTranslate->InsertMenu(14, MF_BYPOSITION,  ID_COMPUTER_EDIT_CONFIG, gszTranslation[PosGroupMenuExtraEditConfig]);	//"Edit config");
	pTranslate->InsertMenu(15, MF_BYPOSITION, ID_COMPUTER_EDIT_APP_INFO, gszTranslation[PosGroupMenuExtraEditAppInfo]);	//"Edit app info");
	pTranslate->InsertMenu(17, MF_BYPOSITION, ID_COMPUTER_READ_CONFIG_DIALOG, gszTranslation[PosGroupMenuExtraReadConfig]); //"Read config file"
	pTranslate->InsertMenu(18, MF_BYPOSITION | MF_SEPARATOR); 
	// config

	pTranslate->InsertMenu(19, MF_BYPOSITION | MF_SEPARATOR); 
	pTranslate->InsertMenu(20, MF_BYPOSITION, ID_COMPUTER_NET_DIALOG, gszTranslation[PosGroupMenuExtraAllowNetwork]);

	pTranslate->InsertMenu(21, MF_BYPOSITION, ID_COMPUTER_ALLOW_DIALOG, gszTranslation[PosGroupMenuExtraAllowToRun]);	//"Allow to run"

	pTranslate->InsertMenu(22, MF_BYPOSITION, ID_COMPUTER_ALLOW_GPU_DIALOG, gszTranslation[PosGroupMenuExtraAllowToRunGpu]);	//"Allow to run"

	// Help
	menu.ModifyMenu(6,MF_BYPOSITION,0, gszTranslation[PosGroupMenuHelp]);
	pTranslate = menu.GetSubMenu(6);
	pTranslate->ModifyMenu(ID_HELP_MANUAL, MF_STRING, ID_HELP_MANUAL, gszTranslation[PosGroupMenuHelpManual]);
	pTranslate->ModifyMenu(ID_HELP_FORUM, MF_STRING, ID_HELP_FORUM, gszTranslation[PosGroupMenuHelpForum]);
	pTranslate->ModifyMenu(ID_HELP_BOINCWEBSITE, MF_STRING, ID_HELP_BOINCWEBSITE, gszTranslation[PosGroupMenuHelpBoincWebsite]);
	pTranslate->ModifyMenu(ID_APP_ABOUT, MF_STRING, ID_APP_ABOUT, gszTranslation[PosGroupMenuHelpAbout]);
	pTranslate->ModifyMenu(ID_HELP_UPDATE, MF_STRING, ID_HELP_UPDATE, gszTranslation[PosGroupMenuHelpUpdate]);


	menu.Detach();
// Translate
	WriteDebugLog(g_cFileNameLog, "Menu translations done\r\n", false);	

	menu.Detach();

	m_pDocTemplateComputerCombinedRemote = new CRemoteDocTemplate(IDR_BoincTasksRemote, /*IDR_BoincTasksRemote,*/
		RUNTIME_CLASS(CRemoteCombinedDoc),
		RUNTIME_CLASS(CCombinedChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CRemoteCombinedTaskView));
	if (!m_pDocTemplateComputerCombinedRemote)	return FALSE;

	DestroyMenu(m_pDocTemplateComputerCombinedRemote->m_hMenuShared);
	m_pDocTemplateComputerCombinedRemote->m_hMenuShared = m_hMenuRemote;
	AddDocTemplate(m_pDocTemplateComputerCombinedRemote);



	m_pDlgProxyMain		= new CDlgProxyMain("Proxy"); // translation someplace else 
	m_pDlgProxyHttp		= new CDlgProxyHttp();
	m_pDlgProxySocks	= new CDlgProxySocks();

	m_pDlgSettingsMain		= new CDlgSettingsMain(gszTranslation[PosGroupDialogSettingsTitle]);
	m_pDlgSettingsGeneral	= new CDlgSettingsGeneral;
	m_pDlgSettingsView		= new CDlgSettingsView;
	m_pDlgSettingsProjects	= new CDlgSettingsProject;
	m_pDlgSettingsTasks		= new CDlgSettingsTasks;
	m_pDlgSettingsTransfer	= new CDlgSettingsTransfer;
	m_pDlgSettingsMessage	= new CDlgSettingsMessage;
	m_pDlgSettingsHistory	= new CDlgSettingsHistory;
	m_pDlgSettingsNotices	= new CDlgSettingsNotices;
	m_pDlgSettingsWarning	= new CDlgSettingsWarning;
	m_pDlgSettingsRules		= new CDlgSettingsRules;
	m_pDlgSettingsGadget	= new CDlgSettingsGadget;
	m_pDlgSettingsWWW		= new CDlgSettingsWWW;
	m_pDlgSettingsMobile	= new CDlgSettingsMobile;
	m_pDlgSettingsExpert	= new CDlgSettingsExpert;

	m_pDlgSettingsMainDummy	= new CDlgSettingsMain("");

	m_pDlgColorMain			= new CDlgColorMain(gszTranslation[PosDialogColorTitle]);
	m_pDlgColorGeneral		= new CDlgColorGeneral;
	m_pDlgColorProjects		= new CDlgColorProjects;
	m_pDlgColorTasks		= new CDlgColorTasks;
	m_pDlgColorWarnings		= new CDlgColorWarnings;
	m_pDlgColorMessages		= new CDlgColorMessages;
	m_pDlgColorHistory		= new CDlgColorHistory;
	m_pDlgColorGraphic		= new CDlgColorGraphic;
	m_pDlgColorTemperature	= new CDlgColorTemperature;
	m_pDlgFont				= new CDlgFont;

	m_pDlgUpdate = new CDlgUpdate;
	m_pDlgUpdate->Create(IDD_DLGUPDATE);

	m_pDlgAbout = new CAboutDlg;
	m_pDlgAbout->Create(IDD_ABOUTBOX);

//	m_pDlgSettings = new CDlgSettings;
//	m_pDlgSettings->Create(IDD_DIALOG_SETTINGS);

//	m_pDlgGraphic = new CDlgGraphic;						in document !!!!!!!
//	m_pDlgGraphic->Create(IDD_DIALOG_GRAPHIC);

//	m_pDlgColor = new CDlgColor;
//	m_pDlgColor->Create(IDD_DIALOG_COLOR);

	m_pDlgAddProject = new CDlgAddProject;
	m_pDlgAddProject->Create(IDD_DIALOG_ADD_PROJECT1);

	m_pDlgAddAccountManager = new CDlgAddAccountManager;
	m_pDlgAddAccountManager->Create(IDD_DIALOG_ACCOUNT_MANAGER);

	m_pDlgSetDebt = new CDlgSetDebt;
	m_pDlgSetDebt->Create(IDD_DIALOG_SET_DEBT);

	m_pDlgProperties = new CDlgProperties;
	m_pDlgProperties->Create(IDD_DIALOG_PROPERTIES, &wndInvisible);

	m_pDlgScheduler = new CDlgScheduler;
	m_pDlgScheduler->Create(IDD_DIALOG_ADVANCED_SCHEDULER, &wndInvisible);
//	m_pDlgScheduler->ShowWindow(SW_SHOW);

	// create main MDI Frame window
	m_pMainFrame = new CMainFrame;
	bool bLoad = m_pMainFrame->LoadFrame(IDR_MAINFRAME);
	if (!bLoad)
	{
//		delete m_pMainFrame;	// deleting might cause a crash
		m_pMainFrame = NULL;
		WriteDebugLog(g_cFileNameLog, "Creating main window FAILED\r\n", false);	
		return FALSE;
	}
	m_pMainWnd = m_pMainFrame;
	WriteDebugLog(g_cFileNameLog, "Created main window\r\n", false);	

	UINT uiLastTime = theApp.GetProfileInt(registrySectionGeneral, registryGarbageTime, 0);
	time_t tSystemTime;
	_time64(&tSystemTime);
	time_t tNextTime = uiLastTime * (60 * 60);	// in hours

	if (tSystemTime > tNextTime)
	{
// garbage collector
		CString sPathGarbage, sFolderGarbage;
		sPathGarbage = theApp.GetProfileString(registrySectionSettings,registryPath, "");
		CXml	xmlGarbage("");
		sFolderGarbage = xmlGarbage.GetUserDirectory(sPathGarbage,false );
		// history backup
		CString sFolderHistory;
	
		sFolderHistory = sFolderGarbage;
		sFolderHistory+= HISTORY_FOLDER;
		sFolderHistory+= HISTORY_FOLDER_BACKUP;

		m_pThreadGarbageHistory = new CThreadGarbageCollectorData;
		m_pThreadGarbageHistory->m_dThreadId = 0;
		m_pThreadGarbageHistory->m_sFolder = sFolderHistory;
		m_pThreadGarbageHistory->m_iTimeToRemove = 60*60*24*14;	// 14 days
		m_pThreadGarbageHistory->m_sFind = "*.*";
		m_pThreadGarbageHistory->m_bReadOnly = false;
		::PostMessage(theApp.m_pMainFrame->m_hWnd, UWM_MSG_THREAD_GARBAGE_COLLECTOR, (WPARAM) 0, (LPARAM) m_pThreadGarbageHistory);		
		WriteDebugLog(g_cFileNameLog, "Start garbage collector to deleted history files\r\n", false);

		// logging
		CString sFolderLogging;
		sFolderLogging = sFolderGarbage;
		sFolderLogging+= LOGGING_FOLDER;

		m_pThreadGarbageLog = new CThreadGarbageCollectorData;
		m_pThreadGarbageLog->m_dThreadId = 0;
		m_pThreadGarbageLog->m_sFolder = sFolderLogging;
		m_pThreadGarbageLog->m_iTimeToRemove = 60*60*24*120;	// 120 days
		m_pThreadGarbageLog->m_sFind = "*.*";
		m_pThreadGarbageLog->m_bReadOnly = false;
		::PostMessage(theApp.m_pMainFrame->m_hWnd, UWM_MSG_THREAD_GARBAGE_COLLECTOR, (WPARAM) 0, (LPARAM) m_pThreadGarbageLog);	
		WriteDebugLog(g_cFileNameLog, "Start garbage collector to deleted logging files\r\n", false);

		uiLastTime = (UINT) (tSystemTime / (60*60));	// hour
		theApp.WriteProfileInt(registrySectionGeneral, registryGarbageTime, uiLastTime+24);	// next time + 24 hour
	}
// garbage collector
	else
	{
		WriteDebugLog(g_cFileNameLog, "Skip garbage collector, runs only once a day, at startup\r\n", false);
	}
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);

// Dispatch commands specified on the command line.  Will return FALSE if
//	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
//	if (!ProcessShellCommand(cmdInfo)) return FALSE;
	// The main window has been initialized, so show and update it

	m_pDlgSettingsMain->AddPage(m_pDlgSettingsGeneral);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsView);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsProjects);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsTasks);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsMessage);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsHistory);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsNotices);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsWarning);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsGadget);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsRules);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsWWW);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsMobile);
	m_pDlgSettingsMain->AddPage(m_pDlgSettingsExpert);


	m_pDlgSettingsMainDummy->AddPage(m_pDlgSettingsTransfer);

	m_pDlgSettingsMain->Create(m_pMainWnd,WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX ,0);	// system anders geen icoon
	m_pDlgSettingsMainDummy->Create(m_pMainWnd,WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX ,0);	// system anders geen icoon

	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsExpert);						// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsGadget);						// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsView);							// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsProjects);						// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsTasks);							// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsMessage);						// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsHistory);						// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsNotices);
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsWarning);						// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsRules);							// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsWWW);							// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsMobile);							// init dialog
	m_pDlgSettingsMain->SetActivePage(m_pDlgSettingsGeneral);						// init dialog

	m_pDlgSettingsMainDummy->SetActivePage(m_pDlgSettingsTransfer);						// init dialog

	m_pDlgColorMain->AddPage(m_pDlgColorGeneral);
	m_pDlgColorMain->AddPage(m_pDlgColorProjects);
	m_pDlgColorMain->AddPage(m_pDlgColorTasks);
	m_pDlgColorMain->AddPage(m_pDlgColorMessages);
	m_pDlgColorMain->AddPage(m_pDlgColorHistory);
	m_pDlgColorMain->AddPage(m_pDlgColorWarnings);
	m_pDlgColorMain->AddPage(m_pDlgColorGraphic);
	m_pDlgColorMain->AddPage(m_pDlgColorTemperature);
	m_pDlgColorMain->AddPage(m_pDlgFont);
	m_pDlgColorMain->Create(m_pMainWnd,WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX ,0);	// system anders geen icoon
	m_pDlgColorMain->SetActivePage(m_pDlgFont);									// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorWarnings);						// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorHistory);							// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorTasks);							// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorMessages);						// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorProjects);						// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorGraphic);							// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorTemperature);						// init dialog
	m_pDlgColorMain->SetActivePage(m_pDlgColorGeneral);							// init dialog

//	m_pDlgPrefMain->AddPage(m_pDlgPrefProcessor);
//	m_pDlgPrefMain->AddPage(m_pDlgPrefNetwork);
//	m_pDlgPrefMain->AddPage(m_pDlgPrefDisk);
//	m_pDlgPrefMain->AddPage(m_pDlgPrefExclusive);
//	m_pDlgPrefMain->Create(m_pMainWnd,WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX ,0);	// system anders geen icoon
//	m_pDlgPrefMain->SetActivePage(m_pDlgPrefExclusive);
//	m_pDlgPrefMain->SetActivePage(m_pDlgPrefNetwork);							// init dialog
//	m_pDlgPrefMain->SetActivePage(m_pDlgPrefDisk);								// init dialog
//	m_pDlgPrefMain->SetActivePage(m_pDlgPrefProcessor);							// init dialog

	m_pDlgProxyMain->AddPage(m_pDlgProxyHttp);
	m_pDlgProxyMain->AddPage(m_pDlgProxySocks);
	m_pDlgProxyMain->Create(m_pMainWnd,WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX ,0);	// system anders geen icoon
	m_pDlgProxyMain->SetActivePage(m_pDlgProxySocks);							// init dialog
	m_pDlgProxyMain->SetActivePage(m_pDlgProxyHttp);							// init dialog

	m_pDlgFloating = new CDlgFloating;											// After the settings dialog
	m_pDlgFloating->Create(IDD_DIALOG_FLOATING);

	m_pMainFrame->ShowWindow(SW_HIDE); 
	m_pMainFrame->UpdateWindow();

	InitEncryption();

	ReadXml();
	ReadConfigXml();
	FirstTimeLocalHost();

	WriteDebugLog(g_cFileNameLog, "Connect to BOINC clients\r\n", false);	
	Connect();

	// check for update
	int updateCount = GetProfileInt(registrySectionGeneral, registryUpdateCount, 200);
	updateCount++;
	if (updateCount > 100)
	{
		m_pDlgUpdate->ShowWindow(SW_SHOW);
		updateCount = 0;
	}
	WriteProfileInt(registrySectionGeneral, registryUpdateCount, updateCount);	

//	g_bSilentReboot = true;

	int iShow;
	iShow = GetProfileInt(registrySectionSettings,registryStartShow, 0);
	if (iShow)
	{
		theApp.WriteProfileInt(registrySectionSettings,registryStartShow, 0);
		theApp.m_pMainWnd->ShowWindow(SW_SHOW);
//		if (theApp.m_pMainWnd->IsIconic())
//		{
//			theApp.m_pMainWnd->ShowWindow(SW_NORMAL);
//		}
		theApp.m_pMainWnd->SetForegroundWindow();	
	}

	if (g_bSilentReboot)
	{
		_invalid_parameter_handler oldHandler, newHandler;
		newHandler = myInvalidParameterHandler;
		oldHandler = _set_invalid_parameter_handler(newHandler);

		// Disable the message box for assertions.
		_CrtSetReportMode(_CRT_ASSERT, 0);
	}

	int iCurSel = theApp.GetProfileInt(registrySectionSettings, registryThreadPriority, 0);
	::PostMessage(theApp.m_pMainFrame->m_hWnd, UWM_SET_THREAD_PRIORITY, iCurSel, 0);

	WriteDebugLog(g_cFileNameLog, "Startup finished\r\n", false);

//	g_cFileNameLog[0] = 0;	// needed by garbage collector

	return TRUE;
}

bool CBoincTasksApp::GetBoincTasksLanguage(int iLangIdSelected)
{
	CLoggingMessage sLog;
	LANGID	lIdUser, lIdSystem;
	CString sTxt = "";
	WriteDebugLog(g_cFileNameLog, "Language detection\r\n", false);
	theApp.m_pMultiLanguage = new CMultiLanguage;

	sTxt.Format("Language registry: %d\r\n", iLangIdSelected);	WriteDebugLog(g_cFileNameLog, sTxt.GetBuffer(), false); sTxt.ReleaseBuffer();

	if (iLangIdSelected > 20) theApp.m_pMultiLanguage->AddLangId(iLangIdSelected);		// selected first
	else iLangIdSelected = 0;

	sTxt.Format("Language selected: %d\r\n", iLangIdSelected);	WriteDebugLog(g_cFileNameLog, sTxt.GetBuffer(), false); sTxt.ReleaseBuffer();

	theApp.m_pMultiLanguage->DetectLangID(&lIdUser, &lIdSystem);
	theApp.m_pMultiLanguage->AddLangId(1033);		// fallback is English
	theApp.m_pTranslation = new CTranslation;
	CString sTranslationLog;
	if (!theApp.m_pTranslation->Open(&sTranslationLog))
	{
		sTranslationLog = "Fatal error, missing file: BoincTasks_ENU.btlang\r\nTry installing BoincTasks again.\r\n" + sTranslationLog;
		AfxMessageBox(sTranslationLog);
		WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);
		return FALSE;
	}
	if (sTranslationLog.GetLength() > 0) WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);

	WriteDebugLog(g_cFileNameLog, "Read translation\r\n", false);
	if (!theApp.m_pTranslation->Read(&sTranslationLog))
	{
		theApp.m_pTranslation->Close();
		WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);
		WriteDebugLog(g_cFileNameLog, "Close translation, return FALSE\r\n", false);
		return FALSE;
	}
	theApp.m_pTranslation->Close();
	WriteDebugLog(g_cFileNameLog, "Close translation, OK\r\n", false);

	WriteDebugLog(g_cFileNameLog, "Started logging\r\n", false);

	WriteDebugLog(g_cFileNameLog, "Open translation override\r\n", false);
	if (theApp.m_pTranslation->Open(&sTranslationLog, true))	// override
	{
		sLog.m_pFrom = "Language"; sLog.m_pText = "BoincTasks_OVERRIDE.btlang, found."; theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT, (WPARAM)&sLog, LOGGING_NORMAL);
		if (!theApp.m_pTranslation->Read(&sTranslationLog))
		{
			theApp.m_pTranslation->Close();
			WriteDebugLog(g_cFileNameLog, sTranslationLog.GetBuffer(), false);
			WriteDebugLog(g_cFileNameLog, "Close translation override, return FALSE\r\n", false);
			return FALSE;
		}
		theApp.m_pTranslation->Close();
	}
	WriteDebugLog(g_cFileNameLog, "Close translation override, OK\r\n", false);

	TCHAR szLangCodeUser[4]; szLangCodeUser[0] = 0;
	TCHAR szLangCodeSystem[4]; szLangCodeSystem[0] = 0;
	TCHAR szLangCodeSelected[4]; szLangCodeSelected[0] = 0;
	TCHAR szLangCodeUsed[4]; szLangCodeUsed[0] = 0;
	CString sFormat;
	::GetLocaleInfoA(lIdUser, LOCALE_SABBREVLANGNAME, szLangCodeUser, 4);
	::GetLocaleInfoA(lIdSystem, LOCALE_SABBREVLANGNAME, szLangCodeSystem, 4);

	if (iLangIdSelected != 0) ::GetLocaleInfoA(iLangIdSelected, LOCALE_SABBREVLANGNAME, szLangCodeSelected, 4);
	else strcpy_s(szLangCodeSelected, "");

	::GetLocaleInfoA(theApp.m_pTranslation->m_lcid, LOCALE_SABBREVLANGNAME, szLangCodeUsed, 4);

	sFormat.Format("User: %d (%s), System: %d (%s), Selected: %d (%s) Used: %d (%s)", lIdUser, &szLangCodeUser[0], lIdSystem, szLangCodeSystem, iLangIdSelected, szLangCodeSelected, theApp.m_pTranslation->m_lcid, szLangCodeUsed);
	sLog.m_pFrom = "Language"; sLog.m_pText = sFormat.GetBuffer(); theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT, (WPARAM)&sLog, LOGGING_NORMAL);
	sFormat.ReleaseBuffer();

	sFormat += "\r\n";
	WriteDebugLog(g_cFileNameLog, sFormat.GetBuffer(), false);
	return TRUE;
}



//std::string	CBoincTasksApp::GetLocalhostIp()
std::deque<std::string> *CBoincTasksApp::GetLocalhostIp()
{
	CLoggingMessage log;
	CString sFrom = "Get localhost IP";
	CStringA sTemp;
	std::deque<std::string> *plLocalhost;
	DWORD dwRetval;
	struct addrinfo hints;
	struct addrinfo *ptr = NULL;
	struct addrinfo *result = NULL;
	struct sockaddr_in  *sockaddr_ipv4;
	char				szHostName[MAX_PATH];

	// Declare and initialize variables
	PIP_INTERFACE_INFO pInfo = NULL;
	ULONG ulOutBufLen = 0;
	DWORD dwRetVal = 0;
	int iReturn = 1;
	char *pLocalhost;

	gethostname(szHostName, MAX_PATH);

	//--------------------------------
	// Setup the hints address info structure
	// which is passed to the getaddrinfo() function
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	plLocalhost = new std::deque<std::string>;

	dwRetval = getaddrinfo(szHostName, NULL, &hints, &result);
	if (dwRetval != 0)
	{
		log.m_pFrom = sFrom.GetBuffer(); log.m_pText = "ERROR: Failed to get IP address from LOCALHOST."; m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT, (WPARAM)&log, LOGGING_NORMAL);
		WSACleanup();
		return plLocalhost;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		if (ptr->ai_family == AF_INET)
		{
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			pLocalhost = inet_ntoa(sockaddr_ipv4->sin_addr);			
			sTemp = "Found IP: ";
			sTemp+= pLocalhost;
			log.m_pFrom = sFrom.GetBuffer(); log.m_pText = sTemp.GetBuffer(); m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT, (WPARAM)&log, LOGGING_NORMAL);
			std::string sString = pLocalhost;
			plLocalhost->push_back(sString);
		}
	}
	return plLocalhost;
}

void CBoincTasksApp::Connect()
{
	int			iSize, iPos;
	int			iNrComputers;
	CComputer	*pComputer;
	CString		sDouble;
	CString		sComputerIpFind, sFormat;
	int			iLocalhostCount;

	iSize = (int) theApp.m_lComputers.size();
	iPos = 0;
	iNrComputers = (int) theApp.m_lComputers.size();
	bool	bEmpty = false;

	iLocalhostCount = 0;

	for (int iComputer1 = 0; iComputer1 < iNrComputers; iComputer1++)
	{
		pComputer = theApp.m_lComputers.at(iComputer1);
		if (pComputer->m_sComputerId.GetLength() == 0)
		{
			bEmpty = true;
			UnCheckComputer("");
		}

		// check for anything localhost and change it into localhost
		sComputerIpFind = pComputer->m_sComputerIp;
		bool	bLocalhost;
		bLocalhost = false;

		sComputerIpFind = sComputerIpFind.MakeLower();
		if (sComputerIpFind.Find("127.0.0.1") >= 0)
		{
			bLocalhost = true;
		}

		sComputerIpFind.Replace(' ','_');
		if (sComputerIpFind== LOCALHOST_NAME)
		{
			bLocalhost = true;
		}
		if (bLocalhost)
		{
			iLocalhostCount++;
			pComputer->m_sComputerIp = LOCALHOST_NAME;
			if (iLocalhostCount > 1)
			{
				sFormat.Format("%d",iLocalhostCount);
			}
			pComputer->m_sComputerIp+= sFormat;
		}
		for (int iComputer2 = 0; iComputer2 < iNrComputers; iComputer2++)
		{
			if (pComputer->m_sComputerId == theApp.m_lComputers.at(iComputer2)->m_sComputerId)
			{
				if (iComputer1 != iComputer2)
				{
					sDouble = pComputer->m_sComputerId;
					UnCheckComputer(sDouble);
				}
			}
		}
	}
	if (bEmpty)
	{
		AfxMessageBox(gszTranslation[PosViewComputerErrorEmptyId]);		//ERROR: Connect combined: Found an empty Computer ID, please give it an unique name.
	}
	if (sDouble.GetLength() > 0)
	{
		CString sFormat, sTxt;
		sTxt = gszTranslation[PosViewComputerErrorIdenticalId];				//ERROR: Connect combined: Found 2 Computer ID's: %s, that are identical\n\rThis is not allowed. Change the Computer ID: %s, into an unique name", sCheck, sCheck);
		sFormat.Format(sTxt,sDouble, sDouble);
		AfxMessageBox(sFormat);
	}

	CRemoteCombinedDoc *pRemoteTaskCombinedDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->OpenDocumentFile(INITIAL_FILE_OPEN_NAME);
	pRemoteTaskCombinedDoc->Begin();
}

void CBoincTasksApp::UnCheckComputer(CString sName)
{
	int			iSize, iPos;
//	XOSITION	pos;
	CComputer	*pComputer;
	int			iNrComputers;

	iSize = (int) theApp.m_lComputers.size();
	iPos = 0;

	iNrComputers = (int) theApp.m_lComputers.size();
	for (int iCount = 0; iCount < iNrComputers; iCount++)
	{
		pComputer = theApp.m_lComputers.at(iPos);
		iPos++;
		if (sName == pComputer->m_sComputerId)
		{
			pComputer->m_iComputerChecked = 0;
		}
	}
}


// only the first time: Add localhost
void CBoincTasksApp::FirstTimeLocalHost()
{
	int			iSize, iPos;
	bool		bLocalHost = false;
//	XOSITION	pos;
	int			iNrComputers;
	CComputer	*pComputer;
	char szHostName[MAX_PATH];
	struct hostent *host;
	CString		sLocalhostName;

	int iFirstTime = GetProfileInt(registrySectionGeneral, registryFirstTime, TRUE);
	if (iFirstTime == TRUE)
	{

		gethostname( szHostName, MAX_PATH );
		host = gethostbyname(szHostName);
		if(host != NULL)
		{
			sLocalhostName = host->h_name;
		}
		else
		{
			sLocalhostName = gszTranslation[PosViewComputerAddThis];				//"this";
		}

		WriteProfileInt(registrySectionGeneral,registryFirstTime, FALSE);

		iSize = (int) m_lComputers.size();
		iPos = 0;
		iNrComputers = (int) m_lComputers.size();

		for(int i = 0; i < iNrComputers; i++)
		{
			pComputer = m_lComputers.at(iPos);
			iPos++;
			if (pComputer->m_sComputerIp == LOCALHOST_NAME)
			{
				bLocalHost = true;
			}
		}
		if (!bLocalHost)
		{
			pComputer = new CComputer;
			pComputer->m_sComputerIp = LOCALHOST_NAME;
			pComputer->m_iPort = -1;
			pComputer->m_sComputerId = sLocalhostName;
			pComputer->m_iComputerChecked = true;
			pComputer->m_sComputerPassword = "";
//			pComputer->m_iComputerId = 1;
//			pComputer->m_iDays = -1;
//			pComputer->m_iWu = -1;
			pComputer->m_sComputerGroup = "";
			m_lComputers.push_back(pComputer);
			WriteXml();
		}
	}
}

int CBoincTasksApp::ReadXml(void)
{
	char		szEncryptionBuffer[1024+1];
	CString		sTemp, sComputerId, sComputerGroup, sComputerIp, sComputerMAC, sComputerPassword, sXmlName, sDirectory, sFile;
	CStringA	sAComputerPassword, sUseEncryption, sComputerEncryptedConnection;
	int			iNrComputers, iComputerChecked, iComputerPort, iTranslateSize;
	CComputer	*pComputer;
	bool		bUseEncryption;

	iNrComputers = 0;
	sXmlName = "computers.xml";

	if (m_sLocation.GetLength() > 0)
	{
		sXmlName = "\\locations\\";
		sXmlName+= m_sLocation;
		sXmlName+= ".xml";

		CXml xmlTemp("",true,false);
		sDirectory = xmlTemp.GetDirectory();
		sFile = sDirectory + sXmlName;

		sTemp = "computers.xml in Location:" + sFile + "\r\n";
		WriteDebugLog(g_cFileNameLog, sTemp.GetBuffer(), false);
		sTemp.ReleaseBuffer();

		CFile	file;
		CFileException e;
		if( !file.Open( sFile.GetBuffer(), CFile::modeRead, &e ) )
		{
			CLoggingMessage sLog;
			CString sError;
			sError = "Unable to open location file";
			sError+= sFile;
			sError+= " Using empty list";
			sLog.m_pFrom = "Computer location"; sLog.m_pText = sError.GetBuffer();m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		}
		else 
		{
			file.Close();
		}
	}

	CXml xml(sXmlName,true,false);

	if (xml.ReadStart("computers"))
	{
		while (xml.ReadGroup("computer"))
		{
			xml.ReadItem("id_name",&sComputerId);
			if (sComputerId.GetLength() > 0)
			{
				pComputer = new CComputer;

				xml.ReadItem("id_group",&sComputerGroup);

//				xml.ReadItem("id",&sTemp);
//				iComputerId = atoi(sTemp);
//				if (iComputerId < 0) iComputerId = 0;

				xml.ReadItem("ip",&sComputerIp);
				xml.ReadItem("mac",&sComputerMAC);

				// check for anything localhost and change it into localhost
				bool	bLocalhost;
				bLocalhost = false;
				CString sComputerIpFind;
				sComputerIpFind = sComputerIp.MakeLower();

				if (sComputerIpFind.Find("127.0.0.1") >= 0)
				{
					bLocalhost = true;
				}
				sComputerIpFind.Replace(' ','_');
				if (sComputerIpFind == LOCALHOST_NAME)
				{
					bLocalhost = true;
				}
	
				if (bLocalhost)
				{
					sComputerIp = LOCALHOST_NAME;
					sComputerMAC = "";		// No mac when localhost
				}

				sTemp = "computers.xml IP: ";
				sTemp+= sComputerIp;
				sTemp+= "\r\n";
				WriteDebugLog(g_cFileNameLog, sTemp.GetBuffer(), false);
				sTemp.ReleaseBuffer();

				xml.ReadItem("password",&sComputerPassword);

				sUseEncryption = "";
				xml.ReadItem("encryption", &sUseEncryption);
				sUseEncryption = sUseEncryption.MakeLower();
				if (sUseEncryption == "yes")
				{
					bUseEncryption = true;
				}
				else
				{
					bUseEncryption = false;
				}


				xml.ReadItem("checked",&sTemp);
				iComputerChecked = atoi(sTemp);
				if (iComputerChecked < 0) iComputerChecked = 0;
	
				xml.ReadItem("port",&sTemp);
				iComputerPort = atoi(sTemp);
				if (iComputerPort <= 0) iComputerPort = -1;

				sAComputerPassword= sComputerPassword;
				iTranslateSize = TranslateFromXml(sAComputerPassword.GetBuffer(), szEncryptionBuffer);

				if (bUseEncryption)
				{
					Decrypt(szEncryptionBuffer, iTranslateSize);
					sComputerPassword = szEncryptionBuffer;
				}

				xml.ReadItem("encrypted_connection", &sComputerEncryptedConnection);

				iNrComputers ++;

				pComputer->m_sComputerGroup = sComputerGroup;
				pComputer->m_sComputerId = sComputerId;
				pComputer->m_sComputerIp = sComputerIp.MakeLower();
				sComputerMAC.Replace(":","-");
				sComputerMAC.Replace(" ","");
				sComputerMAC.Replace(",",";");
				pComputer->m_sComputerMAC = sComputerMAC.MakeUpper();
				pComputer->m_sComputerPassword = sComputerPassword;

				int iComputerEncryptedConnection = 0;
				if (sComputerEncryptedConnection == "yes")
				{
					iComputerEncryptedConnection = 1;
				}
				pComputer->m_iEncryptedConnection = iComputerEncryptedConnection;
				pComputer->m_iComputerChecked = iComputerChecked;

				pComputer->m_iPort = iComputerPort;
				m_lComputers.push_back(pComputer);
			}
		}
	}

	// sort

	if (m_lComputers.size() > 2)
	{
		bool bSorting = true;
		while (bSorting)
		{
			bSorting = false;
			for (int iPos = 0; iPos < (int) m_lComputers.size()-1; iPos++)
			{
				// only sort in different groups
				if (m_lComputers.at(iPos)->m_sComputerGroup == m_lComputers.at(iPos)->m_sComputerGroup)
				{
					if (m_lComputers.at(iPos)->m_sComputerId.Compare(m_lComputers.at(iPos+1)->m_sComputerId) > 0)
					{
						pComputer = m_lComputers.at(iPos);
						m_lComputers.at(iPos) = m_lComputers.at(iPos+1);
						m_lComputers.at(iPos+1) = pComputer;
						bSorting = true;
					}
				}
			}
		}
	}

	return iNrComputers;
}

void CBoincTasksApp::ReadConfigXml()
{
	CString		sNr, sFile, sApplication, sProject, sTxt;
	char		cBufferLogging[255];
	CLoggingMessage log;	

	log.m_pFrom = "config.xml";
	log.m_pText = cBufferLogging;
	sFile = "config.xml";
	CXml xml(sFile,false,true);

	if (xml.ReadStart("config"))
	{
		while (xml.ReadGroup("refresh",true))
		{
			xml.ReadItem("auto_cheat",&sProject);
			theApp.m_iAutoRefreshTime = atoi(sProject);
			if (theApp.m_iAutoRefreshTime > 0)
			{
				theApp.m_bAutoRefreshCheat = true;
				if (theApp.m_iAutoRefreshTime < 300) theApp.m_iAutoRefreshTime = 300;
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"auto cheat, max: %d Seconds",theApp.m_iAutoRefreshTime);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
			}
			else
			{
				xml.ReadItem("auto",&sProject);
				theApp.m_iAutoRefreshTime = atoi(sProject);
				if (theApp.m_iAutoRefreshTime > 0)
				{
					if (theApp.m_iAutoRefreshTime < 300) theApp.m_iAutoRefreshTime = 300;
					_snprintf_s(cBufferLogging,254,_TRUNCATE,"localhost auto download/upload refresh, max: %d Seconds",theApp.m_iAutoRefreshTime);
					m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
				}
				else
				{
					xml.ReadItem("downloads",&sProject);				// disabled in auto mode
					theApp.m_iDownloadRefreshTime = atoi(sProject);
					if (theApp.m_iDownloadRefreshTime > 0)
					{
						if (theApp.m_iDownloadRefreshTime < 600) theApp.m_iDownloadRefreshTime = 600;
						_snprintf_s(cBufferLogging,254,_TRUNCATE,"localhost download refresh: %d Seconds",theApp.m_iDownloadRefreshTime);
						m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
					}
					xml.ReadItem("uploads",&sProject);
					theApp.m_iUploadRefreshTime = atoi(sProject);
					if (theApp.m_iUploadRefreshTime > 0)
					{
						if (theApp.m_iUploadRefreshTime < 600) theApp.m_iUploadRefreshTime = 600;
						_snprintf_s(cBufferLogging,254,_TRUNCATE,"localhost upload refresh: %d Seconds",theApp.m_iUploadRefreshTime);
						m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
					}
				}
			}
			xml.ReadItem("refresh",&sProject);
			theApp.m_iRefreshRefreshTime = atoi(sProject);
			if (theApp.m_iRefreshRefreshTime > 0)
			{
				if (theApp.m_iRefreshRefreshTime < 600) theApp.m_iRefreshRefreshTime = 600;
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"localhost refresh: %d Seconds",theApp.m_iRefreshRefreshTime);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
			}

			xml.ReadItem("refresh_asap",&sProject);
			theApp.m_iRefreshRefreshASAP = atoi(sProject);
			if (theApp.m_iRefreshRefreshASAP > 0)
			{
				if (theApp.m_iRefreshRefreshASAP < 400) theApp.m_iRefreshRefreshASAP = MAX_REFRESH_ASAP;
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"localhost refresh ASAP: %d Seconds",theApp.m_iRefreshRefreshASAP);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
			}
		}
	}

	if (xml.ReadStart("config"))
	{
		while (xml.ReadGroup("options",true))
		{
			xml.ReadItem("no_cleanup",&sProject);
			theApp.m_bNoCleanup = (atoi(sProject) == 1);

			if (theApp.m_bNoCleanup)
			{
				strcpy_s(cBufferLogging, "no_cleanup");
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
			}
		}
	}

	if (xml.ReadStart("config"))
	{
		while (xml.ReadGroup("regulator",true))
		{
			int iCpuCores, iGpuCores;
			xml.ReadItem("max_tasks",&sProject);
			theApp.m_iRegulatorMaxTasks = atoi(sProject);
			xml.ReadItem("days_work_requested",&sProject);
			theApp.m_dRegulatorDaysWorkRequested = atof(sProject);
			xml.ReadItem("min_days_gpu",&sProject);
			theApp.m_dRegulatorMinDaysGpu = atof(sProject);
			xml.ReadItem("min_days_cpu",&sProject);
			theApp.m_dRegulatorMinDaysCpu = atof(sProject);
			xml.ReadItem("days_work_requested_min",&sProject);
			theApp.m_dRegulatorDaysWorkRequestedMin = atof(sProject);

			xml.ReadItem("cpu_cores",&sProject);
			iCpuCores = atoi(sProject);
			xml.ReadItem("gpu_cores",&sProject);
			iGpuCores = atoi(sProject);
		
			CString sMissing;
			if (theApp.m_iRegulatorMaxTasks == 0)			sMissing += "<max_tasks>";
			if (theApp.m_dRegulatorDaysWorkRequested == 0)	sMissing += "<days_work_requested>";
			if (theApp.m_dRegulatorMinDaysGpu == 0)			sMissing += "<min_days_gpu>";
			if (theApp.m_dRegulatorMinDaysCpu == 0)			sMissing += "<min_days_cpu>";
			if (theApp.m_dRegulatorDaysWorkRequestedMin == 0)sMissing += "<days_work_requested_min>";
			if (iCpuCores == 0)								sMissing += "<cpu_cores>";
			if (iGpuCores == 0)								sMissing += "<gpu_cores>";

			if (sMissing.GetLength() > 0)
			{
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"<regulator> Missing tag: %s",sMissing);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
			}
			else
			{
				theApp.m_bRegulatorActive = true;
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"Regulator is active, with max tasks: %d, requested work buffer: %.2f days",theApp.m_iRegulatorMaxTasks, theApp.m_dRegulatorDaysWorkRequested);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"If GPU work is above %0.2f days and CPU work is below %0.2f days, set work buffer to %.2f days",theApp.m_dRegulatorMinDaysGpu, theApp.m_dRegulatorMinDaysCpu, theApp.m_dRegulatorDaysWorkRequestedMin);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);

				theApp.m_dRegulatorMinDaysCpu *= iCpuCores;
				theApp.m_dRegulatorMinDaysGpu *= iGpuCores;
			}
		}
	}

	if (xml.ReadStart("config"))
	{
		std::string sComputer;
		while (xml.ReadGroup("localhost",true))
		{
			xml.ReadItem("computer",&sTxt);
			if (sTxt.GetLength() > 0)
			{
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"localhost: %s", sTxt);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
				sComputer = sTxt;
				m_lLocalhost.push_back(sComputer);
			}
		}
	}

	if (xml.ReadStart("config"))
	{
		while (xml.ReadGroup("seti",true))
		{
			xml.ReadItem("vlar",&sTxt);

			if (sTxt == "suspend") theApp.m_sVlar = "suspend";
			if (sTxt == "abort") theApp.m_sVlar = "abort";

			if (theApp.m_sVlar != "")
			{
				_snprintf_s(cBufferLogging,254,_TRUNCATE,"SETI: VLAR's: %s", theApp.m_sVlar);
				m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
			}
		}
	}

	// any config read errors.
	if (xml.m_sLog.GetLength() > 0)
	{
		log.m_pText = xml.m_sLog.GetBuffer();
		m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, LOGGING_NORMAL);
	}

}


void CBoincTasksApp::WriteXml(void)
{
	int			iSize, iPos;
	char		szEncryptionBuffer[1024+1];
	CStringA	sATxt, sAValue, sAComputerPassword;
	CString		sComputerPassword, sComputerPasswordCheck, sXmlName, sDirectory;
	int			iNrComputers;
	CComputer	*pComputer;
//	XOSITION	pos;
	bool		bUseEncryption;

	bUseEncryption = m_pDlgSettingsExpert->m_bUseEncryption;

	sXmlName = "computers.xml";

	if (m_sLocation.GetLength() > 0)
	{
		sXmlName = "\\locations\\";
		sXmlName+= m_sLocation;
		sXmlName+= ".xml";

		CXml xmlTemp("",true,false);
		sDirectory = xmlTemp.GetDirectory();
		sDirectory+= "\\locations\\";
		_mkdir(sDirectory);
	}

	CXml xml(sXmlName);
	if (xml.WriteStart("computers"))
	{
		iSize = (int) m_lComputers.size();
		iPos = 0;
		iNrComputers = (int) m_lComputers.size();

		for(int i = 0; i < iNrComputers; i++)
		{
			xml.WriteBeginGroup("computer");
			pComputer = m_lComputers.at(iPos);
			iPos++;

			// send info to the views

//			sATxt.Format("%d",pComputer->m_iComputerId);
//			xml.WriteItem("id",sATxt);
			xml.WriteItem("id_name",(CStringA) pComputer->m_sComputerId,1);
			xml.WriteItem("id_group",(CStringA) pComputer->m_sComputerGroup,1);
			xml.WriteItem("ip",(CStringA) pComputer->m_sComputerIp.MakeLower(),1);
			xml.WriteItem("mac",(CStringA) pComputer->m_sComputerMAC.MakeUpper(),1);
			sATxt.Format("%d",pComputer->m_iComputerChecked);
			xml.WriteItem("checked",sATxt,1);

			sATxt.Format("%d",pComputer->m_iPort);
			xml.WriteItem("port",sATxt,1);

			sComputerPassword = pComputer->m_sComputerPassword;
			sAComputerPassword = sComputerPassword;

			CStringA sEncryptedConnection = "";
			if (pComputer->m_iEncryptedConnection == 1)
			{
				sEncryptedConnection = "yes";
			}


			xml.WriteItem("encrypted_connection", sEncryptedConnection, 1);

			if (bUseEncryption)
			{
				if (sAComputerPassword.GetLength() < 1024)
				{
					strcpy_s(szEncryptionBuffer,1024,sAComputerPassword);
					int iActualSize = Encrypt(szEncryptionBuffer,1024);
					sAComputerPassword = TranslateToXml(szEncryptionBuffer, iActualSize);
					xml.WriteItem("password", sAComputerPassword,1);

					// check the encrypt/decrypt
					int iTranslateSize = TranslateFromXml(sAComputerPassword.GetBuffer(), szEncryptionBuffer);
					Decrypt(szEncryptionBuffer, iTranslateSize);

					if (sComputerPassword != szEncryptionBuffer)
					{	
						CString sTxt;
						sTxt = "Name: ";
						sTxt += pComputer->m_sComputerId;
						sTxt += ", IP: ";
						sTxt+= pComputer->m_sComputerIp;	

						sTxt += "\r\nPlease report this in: Encrypt/Decrypt error, before: ";
						sTxt += sComputerPassword;
						sTxt += " after: ";
						sTxt += sComputerPasswordCheck;
						CLoggingMessage sLog;
						sLog.m_pFrom = "computers"; sLog.m_pText = sTxt.GetBuffer();m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
						AfxMessageBox(sTxt);
					}
				}
				else
				{
					CLoggingMessage sLog;
					sLog.m_pFrom = "computers"; sLog.m_pText = "Password > 1024 characters";m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
				}
			}
			else
			{
				xml.WriteItem("password", sAComputerPassword,1);
				xml.WriteItem("encryption", "no",1);
			}

			xml.WriteEndGroup();
		}
		xml.WriteEnd();
	}
}

void CBoincTasksApp::InitEncryption(void)
{
	char cBufferLogging[255];

	m_sEncryptKey = L"ADE157WCW09#uhelfn";
	m_iInitialisationSuccess=CryptAcquireContext(&m_cspContext,NULL,NULL,PROV_RSA_FULL,0);

	if(m_iInitialisationSuccess)
	{
		m_iInitialisationSuccess=CryptCreateHash(m_cspContext,CALG_MD5,0,0,&m_hashData);
	}
	else
	{
		unsigned int iError = GetLastError();
		CLoggingMessage sLog;	
		_snprintf_s(cBufferLogging,254,_TRUNCATE,"Error %x",iError);
		sLog.m_pFrom = "ERROR: CryptAcquireContext";
		sLog.m_pText = &cBufferLogging[0];
		m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_DEBUG);
	}
	
	if(m_iInitialisationSuccess)
	{
//		m_iInitialisationSuccess=CryptHashData(m_hashData,(const unsigned char *)m_sEncryptKey.GetBuffer(256),m_sEncryptKey.GetLength(),0);
		m_iInitialisationSuccess=CryptHashData(m_hashData,(const unsigned char *)m_sEncryptKey.GetBuffer(),m_sEncryptKey.GetLength(),0);
	}
	m_sEncryptKey.ReleaseBuffer();

	if(m_iInitialisationSuccess)
	{
		m_iInitialisationSuccess=CryptDeriveKey(m_cspContext,CALG_RC2,m_hashData,0,&m_keyEncryption);
	}
}

// warning the encryption is producing 0's in the string so the actualsize is VERY important
int  CBoincTasksApp::Encrypt(char *msg,long orginalSize)
{
	long actualSize= (long) strlen(msg);

	if(m_iInitialisationSuccess)
	{
		 if(!CryptEncrypt(m_keyEncryption,NULL,TRUE,0,(unsigned char *)msg,(unsigned long *)&actualSize,orginalSize))
		 {
			 return orginalSize;
		 }
		 msg[actualSize]='\0';
		 return actualSize; 
	}
	return actualSize;
}

int  CBoincTasksApp::Decrypt(char *dest,DWORD actualSize)
{
	DWORD orginalSize;

	orginalSize = actualSize;
	if(m_iInitialisationSuccess && dest!=NULL)
	{
		if(!CryptDecrypt(m_keyEncryption,0,true,0,(unsigned char *)dest,(unsigned long *)&actualSize))
		{
			
			DWORD dwError = GetLastError();
			*(dest+actualSize)='\0';
//			ERROR_SUCCESS
			return actualSize;
			 
		}
		*(dest+actualSize)='\0';
		return actualSize;
	}

	*(dest+orginalSize)='\0';
	return orginalSize;
}

// this is not a string but a buffer with 0's in it.
CStringA CBoincTasksApp::TranslateToXml(char *pBuffer, int iActualSize)
{
	CStringA sATxtXml, sAFormat;
	char	cChar;

	sATxtXml = "";
	for (int iCount = 0; iCount < iActualSize; iCount++)
	{
		cChar = *(pBuffer+iCount);
		sAFormat.Format("%%%d",cChar);
		sATxtXml += sAFormat;
	}
	return sATxtXml;
}
int CBoincTasksApp::TranslateFromXml(char *pBufferIn, char *pBufferOut)
{
	int			iPosIn, iPosOut, iLenTotal, iLenConverted, iNrConverted;
	char		cChar;
	CStringA	sAConverted, sAFormat, sAValue;
	bool		bMinus, bNumberComplete;

	iLenTotal =  (int) strlen((char *) pBufferIn);

	iPosIn = 0;
	iPosOut= 0;
	while (iPosIn < iLenTotal)
	{
		// get sequence %xxx% 

		cChar = *(pBufferIn + iPosIn);
		if (cChar != '%' || cChar == 0) 
		{
			return iPosOut;
		}
		iPosIn++; // past %
		// get number %number% or %number0
		iNrConverted = 0;
		iLenConverted = 0;
		bNumberComplete = false;
		bMinus = false;
		while (!bNumberComplete)
		{
			cChar = *(pBufferIn + iPosIn);
			if (cChar == '%' || cChar == 0) bNumberComplete = true;
			else 
			{
				iPosIn++;
				iLenConverted++;
				if (cChar == '-') bMinus = true;
				else
				{
					iNrConverted *= 10;
					iNrConverted += (int) (cChar - '0');
				}
			}
		}
		if (iLenConverted == 0)
		{
			return iPosOut;
		}
		if (bMinus)
		{
			iNrConverted = 0 - iNrConverted;
		}
		*(pBufferOut + iPosOut) = (char) iNrConverted;
		iPosOut++;
	}
	return iPosOut;
}

int CBoincTasksApp::ExitInstance()
{
	delete 	m_pDlgFloating;		// asap
	m_pDlgFloating = NULL;

	Sleep(1000);	// wait for all the threads to close

	if (m_pThreadGarbageHistory)
	{
		delete m_pThreadGarbageHistory;
	}
	if (m_pThreadGarbageLog)
	{
		delete m_pThreadGarbageLog;
	}
	if (m_pThreadIpSearch)
	{
		m_pThreadIpSearch->PostThreadMessage(UWM_MSG_THREAD_QUIT,0,0);
//		delete m_pThreadConnect;
		m_pThreadIpSearch = NULL;
	}

	delete m_pDlgUpdate; m_pDlgUpdate = NULL;
	delete m_pDlgAbout; m_pDlgAbout = NULL;


	if (m_pDlgProperties != NULL)	{m_pDlgProperties->SafeWindowPlacement();  delete m_pDlgProperties;};
//	if (m_pDlgComputerLocation != NULL)	{m_pDlgComputerLocation->SafeWindowPlacement();m_pDlgComputerLocation->SafeColumn();  delete m_pDlgComputerLocation;};

	if (m_pDlgScheduler != NULL) {m_pDlgScheduler->SafeWindowPlacement();  delete m_pDlgScheduler;};

	delete m_pDlgAddProject; m_pDlgAddProject = NULL;
	delete m_pDlgAddAccountManager; m_pDlgAddAccountManager = NULL;
	delete 	m_pDlgSetDebt; m_pDlgSetDebt = NULL;

//	delete m_pDlgPrefDisk; m_pDlgPrefDisk = NULL;
//	delete m_pDlgPrefNetwork; m_pDlgPrefNetwork = NULL;
//	delete m_pDlgPrefProcessor; m_pDlgPrefProcessor = NULL;
//	delete m_pDlgPrefExclusive; m_pDlgPrefProcessor = NULL;
//	delete m_pDlgPrefMain; m_pDlgPrefMain = NULL;
	
	delete m_pDlgProxyHttp; m_pDlgProxyHttp = NULL;
	delete m_pDlgProxySocks; m_pDlgProxySocks = NULL;
	delete m_pDlgProxyMain; m_pDlgProxyMain = NULL;

	delete m_pDlgSettingsGeneral; m_pDlgSettingsGeneral = NULL;
	delete m_pDlgSettingsView; m_pDlgSettingsView = NULL;
	delete m_pDlgSettingsProjects; m_pDlgSettingsProjects = NULL;
	delete m_pDlgSettingsWarning; m_pDlgSettingsWarning = NULL;
	delete m_pDlgSettingsRules; m_pDlgSettingsRules = NULL;
	delete m_pDlgSettingsHistory; m_pDlgSettingsHistory = NULL;
	delete m_pDlgSettingsNotices; m_pDlgSettingsNotices = NULL;
	delete m_pDlgSettingsTasks; m_pDlgSettingsTasks = NULL;
	delete m_pDlgSettingsMessage; m_pDlgSettingsMessage = NULL;
	delete m_pDlgSettingsTransfer; m_pDlgSettingsTransfer = NULL;
	delete m_pDlgSettingsGadget; m_pDlgSettingsGadget = NULL;
	delete m_pDlgSettingsWWW; m_pDlgSettingsWWW = NULL;
	delete m_pDlgSettingsMobile; m_pDlgSettingsMobile = NULL;
	delete m_pDlgSettingsExpert; m_pDlgSettingsExpert = NULL;
	delete m_pDlgSettingsMain; m_pDlgSettingsMain = NULL;
	delete m_pDlgSettingsMainDummy; m_pDlgSettingsMainDummy = NULL;

	delete m_pDlgColorGeneral; m_pDlgColorGeneral = NULL;
	delete m_pDlgColorProjects; m_pDlgColorProjects = NULL;
	delete m_pDlgColorTasks; m_pDlgColorTasks = NULL;
	delete m_pDlgColorWarnings; m_pDlgColorWarnings = NULL;
	delete m_pDlgColorMessages; m_pDlgColorMessages = NULL;
	delete m_pDlgColorHistory; m_pDlgColorHistory = NULL;
	delete m_pDlgColorGraphic; m_pDlgColorGraphic = NULL;
	delete m_pDlgColorTemperature; m_pDlgColorTemperature = NULL;
	delete m_pDlgFont; m_pDlgFont = NULL;
	delete m_pDlgColorMain; m_pDlgColorMain = NULL;

	// loggin last, because some programs use it.
	if (m_pDlgLogging != NULL)		{m_pDlgLogging->SafeWindowPlacement(); delete m_pDlgLogging;};
	if (m_pDlgRulesLogging != NULL)	{m_pDlgRulesLogging->SafeWindowPlacement(); delete m_pDlgRulesLogging;};

	wndInvisible.DestroyWindow();

	EmptyComputerList(&m_lComputers);


	delete 	m_pMultiLanguage;
	delete 	m_pTranslation;

	delete m_plLocalhostIp;

	if (m_bWSAStartup) 	WSACleanup();

	::DestroyMenu(m_hMenuRemote);
//	::DestroyMenu(m_hMenuAllow);
//	::DestroyMenu(m_hMenuAllowGpu);
//	::DestroyMenu(m_hMenuNetwork);
//	::DestroyMenu(m_hMenuSynchronize);
//	::DestroyMenu(m_hMenuBenchmark);
//	::DestroyMenu(m_hMenuReadConfig);

//	CryptReleaseContext(m_cspContext,0); // TODO crashed in 2019

	theApp.WriteProfileInt(registrySectionGeneral,registryExitOk, TRUE);

	CloseHandle(m_hMutex);

	if (m_bRestart)
	{
		Reboot();
	}

	return CWinApp::ExitInstance();
}

void CBoincTasksApp::Reboot()
{
	char		strExePath[MAX_PATH];
	CString		sPath, sFile, sData, sTxt;
	int			iFoundPos;

	DWORD dwSize = GetModuleFileName (NULL, strExePath, MAX_PATH);	// not 0 terminated
	strExePath[dwSize] = 0;
	sPath = strExePath;
	iFoundPos = sPath.ReverseFind('\\');
	if (iFoundPos > 0)
	{
		sData = sPath.Mid(0,iFoundPos);
		StartExe(&sPath, &sData);
	}
}

void CBoincTasksApp::EmptyComputerList(std::deque<CComputer*> *plComputers)
{
	int			iSize, iPos;
	CComputer	*pComputer;

	iSize = (int) plComputers->size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pComputer = plComputers->at(iPos);
		delete pComputer;
	}
//	plComputers->clear();
}

// CBoincTasksApp message handlers


void CBoincTasksApp::OnAppAbout()
{
	m_pDlgAbout->ShowWindow(SW_SHOW);
}

void CBoincTasksApp::OnHelpManual()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks-how-to-start/";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

void CBoincTasksApp::OnHelpForum()
{
	CString strLink;
	strLink = "https://forum.efmer.com/";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

void CBoincTasksApp::OnHelpBoincWebsite()
{
	CString strLink;
	strLink = "http://boinc.berkeley.edu/";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}


#define STR_SZ (sizeof(szBuf) / sizeof(char))
void CBoincTasksApp::SetLocale(CString sLanguage)
{
	char szBuf[256], szLocale[256];

	if (sLanguage.GetLength() > 0)
	{
		_tsetlocale(LC_TIME, sLanguage);
		return;
	}

	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, szBuf, STR_SZ);
	_tcscpy_s(szLocale,255, szBuf);
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, szBuf, STR_SZ);
	if (_tcsclen(szBuf) != 0){
		_tcscat_s(szLocale, 255, _T("_"));
		_tcscat_s(szLocale, 255, szBuf);
	}
	::GetLocaleInfo(LOCALE_USER_DEFAULT,  LOCALE_IDEFAULTANSICODEPAGE, szBuf, STR_SZ);
	if (_tcsclen(szBuf) != 0)
	{
		_tcscat_s(szLocale, 255, _T("."));
		_tcscat_s(szLocale, 255, szBuf);
	}

//	_tsetlocale(LC_ALL, szLocale);
	_tsetlocale(LC_TIME, szLocale);

}

CString CBoincTasksApp::GetPassword(CString *pDir)
{
	CString sDir, sPassword;
	CFile file;
	BYTE	bBuffer[100];
	int		iReadCount;

	sDir = GetBoincDirectory((char *) &"DATADIR");
	if (sDir.GetLength() == 0)
	{
		return "";
	}

	sDir += "\\gui_rpc_auth.cfg";

	if( !file.Open( sDir.GetBuffer(), CFile::modeRead) )
	{
		CLoggingMessage sLog;
		sLog.m_pFrom = "computers"; sLog.m_pText = "Unable to read gui_rpc_auth.cfg, is there such file?";m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		return "";
	}

	bBuffer[0] = 0;
	iReadCount = file.Read(bBuffer, 99);
	if (iReadCount > 0)
	{
		bBuffer[iReadCount] = 0;			// ?? set end of string 
		sPassword = bBuffer;
		if (pDir != NULL)
		{
			*pDir = sDir;
		}
		return sPassword;
	}
	CLoggingMessage sLog;
	sLog.m_pFrom = "computers"; sLog.m_pText = "Unable to read password";m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
	return "";
}


CString CBoincTasksApp::GetBoincDirectory(char *pKey)
{
	HKEY	hKey;
	DWORD	wType;
	LONG	lnRes;
	DWORD	wLen = 256;
	char	scFileNameRegister[256];
	char	*sKey = "Software\\Space Sciences Laboratory, U.C. Berkeley\\BOINC Setup";

	scFileNameRegister[0] = 0;

	lnRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE,sKey, 0L, KEY_READ, &hKey);
	if (lnRes == ERROR_SUCCESS)
	{
		wType = REG_SZ;
		LONG lnRes = RegQueryValueEx(hKey, pKey, NULL, &wType,(BYTE *) scFileNameRegister, &wLen);


//		wcscpy(m_swFileNameRegister,L"C:\\temp\\boinc_test\\BOINC");


		if( ERROR_SUCCESS == lnRes )
		{
			RegCloseKey (hKey);
			return scFileNameRegister;
		}
	}

	// x64 testing
	lnRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE,sKey, 0L, KEY_READ, &hKey);
	if (lnRes == ERROR_SUCCESS)
	{
		wType = REG_SZ;
		LONG lnRes = RegQueryValueEx(hKey, pKey, NULL, &wType,(BYTE *) scFileNameRegister, &wLen);
		if( ERROR_SUCCESS == lnRes )
		{
			RegCloseKey (hKey);
			return scFileNameRegister;
		}
	}

	// access 64 registry in 32 bit program
	lnRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE,sKey, 0L, KEY_READ | KEY_WOW64_64KEY, &hKey);
	if (lnRes == ERROR_SUCCESS)
	{
		wType = REG_SZ;
		LONG lnRes = RegQueryValueEx(hKey, pKey, NULL, &wType,(BYTE *) scFileNameRegister, &wLen);
		if( ERROR_SUCCESS == lnRes )
		{
			RegCloseKey (hKey);
			return scFileNameRegister;
		}
	}

	return scFileNameRegister;
}

int CBoincTasksApp::DeleteFiles(CString sDirectoryAppend, int iTimeToRemove)
{
	CFileFind	finder;
	CString		sDirectory, sFileFind, sFileName, sFormat;
	time_t		tRemoveTime, t12Hour;
	CTime		tFile;
	CXml		xml("");
	int			iRemoveCount;

	CString sPath;
	sPath = theApp.GetProfileString(registrySectionSettings,registryPath, "");

	sDirectory = xml.GetUserDirectory(sPath,false );
	sDirectory += sDirectoryAppend;
	sFileFind = sDirectory + "*.*";

	_time64(&tRemoveTime);
	tRemoveTime -= iTimeToRemove;
	_time64(&t12Hour);
	t12Hour -= 60*60*12;
	iRemoveCount = 0;

	sFormat = "DeleteFiles in folders:";
	sFormat+= sFileFind;
	sFormat+= "\r\n";
	WriteDebugLog(g_cFileNameLog, sFormat.GetBuffer(), false);
	sFormat.ReleaseBuffer();

	time_t tTimeout;
	_time64(&tTimeout);
	tTimeout += 20;	// 20 second timeout

	BOOL bWorking = finder.FindFile(sFileFind);
	while (bWorking)
	{
		time_t tSystemtime;
		_time64(&tSystemtime);
		if (tSystemtime > tTimeout)
		{
			WriteDebugLog(g_cFileNameLog, "Timeout\r\n", false);
			break;
		}

		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			sFileName = finder.GetFileName();
			sFileName = sDirectory + sFileName;
			CFile cfile;
			if (cfile.Open(sFileName, CFile::modeRead | CFile::shareDenyNone))
			{
				CFileStatus status;
				if(cfile.GetStatus(status))    // virtual member function
				{
					tFile = status.m_mtime;
				    time_t tTime = tFile.GetTime();
					cfile.Close();

					if (tTime < tRemoveTime)
					{
						try	{CFile::Remove(sFileName);} catch(CFileException* pEx ) {pEx->Delete();}
						sFormat = "Delete:";
						sFormat+= sFileName;
						sFormat+= "\r\n";
						WriteDebugLog(g_cFileNameLog, sFormat.GetBuffer(), false);
						sFormat.ReleaseBuffer();
					}

					if (tTime > t12Hour)
					{
						iRemoveCount++;
					}

				}
				else cfile.Close();
			}
		}
	}
	return iRemoveCount;
}

void CBoincTasksApp::OnFileCloseboincclientandallscienceapplications()
{
	int iResult;

	iResult = AfxMessageBox(gszTranslation[PosClientQuitQuestion],MB_YESNO);		//"Quit the BOINC client and stop all science projects?",MB_YESNO);xx

	if (iResult == IDYES)
	{
		int iLocalHostThreadId = m_pMainFrame->m_iLocalHostThreadId;
		if (iLocalHostThreadId > 0)
		{
			CThreadBoincClient *pThreadBoincClient;

			pThreadBoincClient = (CThreadBoincClient *) AfxBeginThread(RUNTIME_CLASS(CThreadBoincClient));
			pThreadBoincClient->PostThreadMessage(UWM_MSG_THREAD_STOP_BOINC_CLIENT, 0, iLocalHostThreadId);
		}
	}
}

void CBoincTasksApp::OnUpdateFileCloseboincclientandallscienceapplications(CCmdUI *pCmdUI)
{
	if (m_pMainFrame->m_iLocalHostThreadId > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CBoincTasksApp::OnFileStartboincclient()
{
	CThreadBoincClient *pThreadBoincClient;

	int iLocalHostThreadId = m_pMainFrame->m_iLocalHostThreadId;
	if (iLocalHostThreadId > 0)
	{
		pThreadBoincClient = (CThreadBoincClient *) AfxBeginThread(RUNTIME_CLASS(CThreadBoincClient));
		pThreadBoincClient->PostThreadMessage(UWM_MSG_THREAD_START_BOINC_CLIENT, 0, iLocalHostThreadId);
	}
}

void CBoincTasksApp::OnUpdateFileOnFileStartboincclient(CCmdUI *pCmdUI)
{
	if (m_pMainFrame->m_iLocalHostThreadId > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CBoincTasksApp::OnFileReadColorSettings()
{
	LRESULT lResult;
	int		iResult;
	CString	sDirectory;

	CXml xml(XML_COLOR_FILE);
	sDirectory = xml.GetDirectory();
	sDirectory = sDirectory + "\\";
	sDirectory = sDirectory + XML_COLOR_FILE;

	sDirectory = "\r\n\r\n" + sDirectory;
	sDirectory = gszTranslation[PosDialogColorReadSettingsWarning] + sDirectory;

//	iResult = AfxMessageBox(gszTranslation[PosDialogColorReadSettingsWarning],MB_YESNO | MB_ICONEXCLAMATION);
	iResult = AfxMessageBox(sDirectory, MB_YESNO | MB_ICONEXCLAMATION);
	if (iResult == IDYES  )
	{
		lResult = ::SendMessage(theApp.m_pDlgColorMain->m_hWnd,UWM_MSG_READ_SETTINGS,0,0);
		if (lResult == FALSE) 
		{
			AfxMessageBox(gszTranslation[PosDialogColorFailedSettingsWarning]);
		}
	}
}

void CBoincTasksApp::OnFileWriteColorSettings()
{
	LRESULT lResult;
	int		iResult;
	CString	sDirectory;

	CXml xml(XML_COLOR_FILE);
	sDirectory = xml.GetDirectory();
	sDirectory = sDirectory + "\\";
	sDirectory = sDirectory + XML_COLOR_FILE;

	sDirectory = "\r\n\r\n" + sDirectory;
	sDirectory = gszTranslation[PosDialogColorWriteSettingsWarning] + sDirectory;

//	iResult = AfxMessageBox(gszTranslation[PosDialogColorWriteSettingsWarning], MB_YESNO | MB_ICONEXCLAMATION);
	iResult = AfxMessageBox(sDirectory, MB_YESNO | MB_ICONEXCLAMATION);
	if (iResult == IDYES  )
	{
		lResult = ::SendMessage(theApp.m_pDlgColorMain->m_hWnd,UWM_MSG_WRITE_SETTINGS,0,0);
		if (lResult == FALSE) 
		{
			AfxMessageBox(gszTranslation[PosDialogColorFailedSettingsWarning]);
		}
	}
}

bool CBoincTasksApp::ValidScreenPosition(int *pLeft, int *pTop, int *pWidth, int *pHeight, int iLrMargin, int iUdMargin)
{
	int iWidth, iHeight;
	bool bOutOfRange = false;

	iWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	iHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);



	if (*pLeft < -iLrMargin) bOutOfRange = true;
	if (*pTop < -iLrMargin) bOutOfRange = true;
	if (*pWidth < 10) bOutOfRange = true;
	if (*pHeight < 10) bOutOfRange = true;

	if (*pLeft > iWidth - iLrMargin) bOutOfRange = true;
	if (*pHeight > iHeight - iUdMargin) bOutOfRange = true;

	if (bOutOfRange)
	{
		*pLeft = 50;
		*pTop = 50;
		*pWidth = 400;
		*pHeight = 400;
	}

	return bOutOfRange;
}
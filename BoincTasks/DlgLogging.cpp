// DlgLogging.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgLogging.h"
#include "Translation.h"
#include <direct.h>
#include "MemoryUse.h"
#include "xml.h"
#include "DlgDebug.h"

// CDlgLogging dialog

IMPLEMENT_DYNAMIC(CDlgLogging, CDialog)

CDlgLogging::CDlgLogging(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogging::IDD, pParent)
{
	CString sNr;
//	int		iNr;
	
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_psLogging = new CString;

	m_bFileOpen = false;

	m_bFirstTimeMemoryLog = false;
	m_bMemoryTest = false;
	MemoryTestClear(true);

	m_bDebugFullMode = false;
	m_bLogDebugForce = false;

	m_pDlgDebug = new CDlgDebug;
	m_pDlgDebug->m_pLogging = this;
	m_pDlgDebug->Create(IDD_DIALOG_DEBUG);
}

CDlgLogging::~CDlgLogging()
{
	if (m_bFileOpen)
	{
		m_file.Close();
		m_bFileOpen = false;
	}

	if (!m_hIcon)
	{
		delete m_hIcon;
	}

	MemoryTestClear();

	delete m_psLogging;
	delete m_pDlgDebug;
}

BOOL CDlgLogging::OnInitDialog()
{
	CString sVersion, sFormat;
	int	iCheck;
	int iHeight, iWidth;
	CRect rect;

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	iCheck = theApp.GetProfileInt(registrySectionSettings, registryDebugLogging, 0);
	m_checkEnableDebug.SetCheck(iCheck);
	m_bDebugMode = (iCheck == TRUE);

	if (m_bDebugMode) m_buttonSetDebug.ShowWindow(SW_SHOW);


	if (m_bLogDebugForce)
	{
		m_bDebugMode = true;
	}

	AddLoggingInfo();

	m_fullDebug.ShowWindow(SW_HIDE);

	m_sLogFileName = GetLogName();
	OpenLog(true);	// write start text;
	CloseLog();

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryLoggingPosLeft, 100);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryLoggingPosTop, 100);
	GetWindowRect(rect);
	iWidth = rect.Width();
	iHeight = rect.Height();
	theApp.ValidScreenPosition(&left, &top, &iWidth, &iHeight);

	SetWindowPos(&CWnd::wndBottom, left, top, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE ); //| SWP_NOSIZE

	sVersion.LoadString(IDS_PROGRAM_VERSION);
	sFormat = "BoincTasks version: ";
	sFormat+= sVersion;
	AddLogging("",sFormat, true);

	SetTimer(UTIMER_DLG_LOGGING, 30000, 0);	// interval 10 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgLogging::Translate()
{
	this->SetWindowText(gszTranslation[PosDialogLoggingTitle]);
	m_checkEnableDebug.SetWindowText(gszTranslation[PosDialogLoggingEnableDebug]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
	m_buttonClear.SetWindowText(gszTranslation[PosDialogCommonButtonsClear]);
}

void CDlgLogging::AddLoggingDebug(CString sFrom, CString sLog, bool bTime)
{
	if (m_bDebugMode) AddLogging(sFrom, sLog, bTime);
}

void CDlgLogging::AddLoggingDebugFull(CString sFrom, CString sLog, bool bTime)
{
	if (m_bDebugFullMode) AddLogging(sFrom, sLog, bTime);
}

void CDlgLogging::AddLogging(CString sFrom, CString sLog, bool bTime, bool bFileOnly)
{
	CString sTxt;
	int iLen;

	CTime time = CTime::GetCurrentTime();   

	CString sTime = time.Format(_T("%d %B %Y - %H:%M:%S "));
	if (bTime)
	{
		sTxt = sTime + sTxt ;
	}

	if (sFrom.GetLength() > 0)
	{
		sTxt += sFrom;
		sTxt +=  " ---- ";
	}

	sTxt += sLog;

	iLen = m_psLogging->GetLength();

	if (iLen > LOGGING_MAX_BUFFER)
	{
		int  iPos = m_psLogging->Find("\r\n",100);
		if (iPos < 0) 
		{
			*m_psLogging= "";
		}
		*m_psLogging = m_psLogging->Mid(iPos);
		iLen = m_psLogging->GetLength();
	}

	sTxt += "\r\n";

	if (!bFileOnly)
	{
		*m_psLogging += sTxt;
		ShowLoggingText();
	}
	AddLoggingToFile(&sTxt);
}

void CDlgLogging::ShowLoggingText()
{
	if (m_logging.m_hWnd != NULL)							// logging is active before the dlg window is created
	{
		m_logging.SetWindowText(*m_psLogging);
		m_logging.LineScroll(0xffffff,0);
	}
}

void CDlgLogging::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLoggingPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLoggingPosTop, wp.rcNormalPosition.top);
}

BOOL CDlgLogging::PreTranslateMessage(MSG* pMsg)
{
	CString sMagic = "MAGIC";
	CString sSilver = "SILVER BULLET";
	int iLenSilver = sSilver.GetLength();
	CString sMemory = "MEMORY TEST";
	int iLenMemory = sMemory.GetLength();

	if(pMsg->message==WM_KEYDOWN)
	{
		m_sMagic += (char) pMsg->wParam;
		sMagic = sMagic.Left(m_sMagic.GetLength());
		if (m_sMagic == sMagic)
		{
			if (m_sMagic.GetLength() == 5)
			{
				if (m_bDebugFullMode)
				{
					m_fullDebug.ShowWindow(SW_HIDE);
					m_bDebugFullMode = false;
				}
				else
				{
					m_fullDebug.ShowWindow(SW_SHOW);
					m_bDebugFullMode = true;
				}
				m_sMagic = "";
			}
		}
		else
		{
			m_sMagic = "";
		}

		m_sSilver += (char) pMsg->wParam;
		sSilver = sSilver.Left(m_sSilver.GetLength());
		if (m_sSilver == sSilver)
		{
			if (m_sSilver.GetLength() == iLenSilver)
			{
				int iResult;
				iResult = AfxMessageBox("You sure you want to do a crash test?\r\nYes will crash BoincTasks!", MB_YESNO);
				if (iResult == IDYES)
				{
					char *pCrash;
					pCrash = NULL;
					*pCrash = ' ';		// crash
				}
				m_sSilver = "";
			}
		}
		else
		{
			m_sSilver = "";
		}

		m_sMemory += (char) pMsg->wParam;
		sMemory = sMemory.Left(m_sMemory.GetLength());
		if (m_sMemory == sMemory)
		{
			if (m_sMemory.GetLength() == iLenMemory)
			{
				if (!m_bMemoryTest)
				{
					int iResult;
					iResult = AfxMessageBox("You sure you want to do start a memory test?\r\nThis may crash BoincTasks when the computer is low on memory!", MB_YESNO);
					if (iResult == IDYES)
					{
						m_bMemoryTest = true;
					}
				}
				else
				{
					AfxMessageBox("Memory test stoppen.");
					m_bMemoryTest = false;
				}

				m_sMemory = "";
			}
		}
		else
		{
			m_sMemory = "";
		}

	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgLogging::MemoryTestClear(bool bInit)
{
	CString sFrom, sLog;

	if (bInit)
	{
		for (int iCount = 0; iCount < MAX_MEMORY_TEST; iCount++)
		{
			m_pcMemoryTest[iCount] = NULL;
		}
		m_iMemoryTest = 0;
	}

	if (m_iMemoryTest > 0)
	{
		sFrom = "Memory test";
		sLog = "Allocated: 0 Mbytes"; 
		AddLogging(sFrom, sLog, true);
	}

	for (int iCount = 0; iCount < m_iMemoryTest; iCount++)
	{
		if (m_pcMemoryTest[iCount] != NULL)
		{
			delete m_pcMemoryTest[iCount];
			m_pcMemoryTest[iCount] = NULL;
		}
	}
	m_iMemoryTest = 0;
}

void CDlgLogging::MemoryTest()
{
	CString sFrom, sLog;

	if (m_iMemoryTest < MAX_MEMORY_TEST)
	{
		m_pcMemoryTest[m_iMemoryTest] = new char [MEMORY_TEST_ALLOCATION];
		if (m_pcMemoryTest[m_iMemoryTest] == NULL)
		{
			sFrom = "Memory test ERROR";
			sLog = "Memory couldn't be allocated!"; 
			AddLogging(sFrom, sLog, true);
		}
		m_iMemoryTest++;
	}
	else
	{
		MemoryTestClear();
	}
	sFrom = "Memory test";
	sLog.Format("Allocated: %dMbytes", m_iMemoryTest * (MEMORY_TEST_ALLOCATION/1000000)); 
	AddLogging(sFrom, sLog, true);
}

void CDlgLogging::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOGGING, m_logging);
	DDX_Control(pDX, IDC_CHECK_DEBUG, m_checkEnableDebug);
	DDX_Control(pDX, IDC_FULL_DEBUG_MODE, m_fullDebug);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCLEAR, m_buttonClear);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDDEBUG_FLAGS, m_buttonSetDebug);
}


BEGIN_MESSAGE_MAP(CDlgLogging, CDialog)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_LOGGING, &CDlgLogging::OnEnChangeEditLogging)
	ON_BN_CLICKED(IDC_CHECK_DEBUG, &CDlgLogging::OnBnClickedCheckDebug)
	ON_MESSAGE (UWM_MSG_LOGGING_TEXT, OnLoggingText) 
	ON_BN_CLICKED(IDCLEAR, &CDlgLogging::OnBnClickedClear)
	ON_BN_CLICKED(IDDEBUG_FLAGS, &CDlgLogging::OnBnClickedFlags)
END_MESSAGE_MAP()


// CDlgLogging message handlers

void CDlgLogging::OnTimer(UINT_PTR nIDEvent)
{
	CString sTxt;

	if (m_bMemoryTest)
	{
		m_bFirstTimeMemoryLog = false;
		MemoryTest();
	}
	else
	{
		MemoryTestClear();
	}


	if (!m_bFirstTimeMemoryLog)
	{
		CMemoryUse memoryUse;
		memoryUse.GetCurrentUsage(&sTxt);
		AddLoggingDebug("Memory", sTxt);
		m_bFirstTimeMemoryLog = true;
	}
}


LRESULT CDlgLogging::OnLoggingText(WPARAM parm1, LPARAM parm2)
{
	CLoggingMessage *pLog;
	int	 iType; 

	pLog = (CLoggingMessage *) parm1;
	iType = (int) parm2;
	if (iType == LOGGING_DEBUG)
	{
		AddLoggingDebug(pLog->m_pFrom, pLog->m_pText);
		return LOGGING_DEBUG;
	}
	if (iType == LOGGING_DEBUGFULL)
	{
		AddLoggingDebugFull(pLog->m_pFrom, pLog->m_pText);
		return LOGGING_DEBUGFULL;
	}
//	if (iType ==LOGGING_DEBUG_TASKS)
//	{
//		if (m_bLogDebugTasks) AddLogging(pLog->m_pFrom, pLog->m_pText, true, m_bLogDebugFileOnly);
//		return LOGGING_DEBUG_TASKS;
//	}

	AddLogging(pLog->m_pFrom, pLog->m_pText);
	return LOGGING_NORMAL;
}

void CDlgLogging::OnEnChangeEditLogging()
{
}

void CDlgLogging::OnBnClickedCheckDebug()
{
	int iCheck;
	iCheck = m_checkEnableDebug.GetCheck();

	if (iCheck)
	{
		m_bDebugMode = true;
		AddLoggingInfo();
		m_buttonSetDebug.ShowWindow(SW_SHOW);

	}
	else
	{
		m_bDebugMode = false;
		AddLogging("Logging","Stop debug mode", true, false);
		m_buttonSetDebug.ShowWindow(SW_HIDE);
		m_pDlgDebug->ShowWindow(SW_HIDE);
	}

	theApp.WriteProfileInt(registrySectionSettings,registryDebugLogging , iCheck);

}

void CDlgLogging::OnBnClickedClear()
{
	*m_psLogging = "";
	ShowLoggingText();
}

CString CDlgLogging::GetLogName()
{
	CString sTxt, sDirectory;
	CXml	xml("");

	CString sPath;
	sPath = theApp.GetProfileString(registrySectionSettings,registryPath, "");

	sDirectory = xml.GetUserDirectory(sPath,false );
	sDirectory += LOGGING_FOLDER;
	_mkdir(sDirectory);

	time_t tSystemTime;
	_time64(&tSystemTime);

	CTime time(tSystemTime);
	sTxt = time.Format("%d-%m-%Y");
	sTxt+= ".log";

	sTxt = sDirectory + sTxt;

	return sTxt;
}


void CDlgLogging::OpenLog(bool bFirstTime)
{
	CString sTxt;

	if (m_bFileOpen) return;

	CFileException e;
	if( !m_file.Open(m_sLogFileName.GetBuffer(), CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate , &e ) )
	{
		m_bFileOpen = false;
		return;
	}
	m_bFileOpen = true;

	if (bFirstTime)
	{
//		m_file.SeekToEnd();
		sTxt = "\r\nStart logging\r\n\r\n";
		AddLoggingToFile(&sTxt);
	}
}

void CDlgLogging::CloseLog()
{
	if (!m_bFileOpen) return;
	
	m_file.Close();
	m_bFileOpen = false;
}


void CDlgLogging::CheckDayChanged()
{
	CString sCurrent;

	sCurrent = GetLogName();
	if (sCurrent != m_sLogFileName)
	{
		m_sLogFileName = sCurrent;
		CloseLog();
		OpenLog(true);
		CloseLog();
	}
}


void CDlgLogging::AddLoggingToFile(CString *psTxt)
{
	if (!m_bFileOpen)
	{
		OpenLog(false);
	}

	if (m_bFileOpen)
	{
		CheckDayChanged();
		if (m_bFileOpen)	// must check again
		{
			m_file.SeekToEnd();
			m_file.Write(*psTxt,psTxt->GetLength());
			m_file.Flush();
		}
	}
	CloseLog();
}

void CDlgLogging::AddLoggingInfo()
{
	if (m_bDebugMode)
	{
		AddLogging("Logging","Start debug mode", true, false);

		if (m_bLogDebugForce)			AddLogging("Logging","forced debug mode", true, false);	

		if (m_bLogDebugFileOnly)		AddLogging("Logging","fileonly", true, false);	
		if (m_bLogDebugTasks)			AddLogging("Logging","tasks", true, false);
		if (m_bLogDebugTasksShow)		AddLogging("Logging","tasks_show", true, false);
		if (m_bLogDebugDirectory)		AddLogging("Logging","directory", true, false);
		if (m_bLogDebugTThrottle)		AddLogging("Logging","tthrottle_communication", true, false);
		if (m_bLogDebugTThrottleData)	AddLogging("Logging","tthrottle_data", true, false);
		if (m_bLogDebugTThrottleRead)	AddLogging("Logging","tthrottle_read", true, false);
		if (m_bLogDebugTThrottleGraphic)AddLogging("Logging","tthrottle_graphic", true, false);
		if (m_bLogDebugSnooze)			AddLogging("Logging","snooze", true, false);
		if (m_bLogDebugTemperatureGpu)	AddLogging("Logging","temperature_gpu", true, false);
		if (m_bLogDebugHistoryFetching)	AddLogging("Logging","history_fetching", true, false);
		if (m_bLogDebugSearchComputers)	AddLogging("Logging","search_computers", true, false);
		if (m_bLogDebugConnecting)		AddLogging("Logging","connecting", true, false);
		if (m_bLogDebugState)			AddLogging("Logging","state", true, false);
		if (m_bLogDebugClosing)			AddLogging("Logging","closing", true, false);
		if (m_bLogDebugNotices)			AddLogging("Logging","notices", true, false);
		if (m_bLogDebugNoticesTimer)	AddLogging("Logging","notices_timer", true, false);
		if (m_bLogDebugMobile)			AddLogging("Logging","mobile", true, false);
		if (m_bLogDebugMobileConnect)	AddLogging("Logging","mobile_connect", true, false);
		if (m_bLogDebugMobileConnectDump) AddLogging("Logging","mobile_connect_dump", true, false);
		if (m_bLogDebugCloud)			AddLogging("Logging","cloud", true, false);

		if (m_bLogDebugLocalDirectory)	AddLogging("Logging","local_directory", true, false);
		if (m_bLogDebugHeapCheck)		AddLogging("Logging","heap_check", true, false);
		if (m_bLogDebugMobileScript)	AddLogging("Logging","mobile_script", true, false);
		AddLogging("","", true, false);
	}

#ifdef TESTBUILD
	CString sTestBuild = TESTBUILD;
	AddLogging("****************************************************************", "", true, false);
	AddLogging("****************************************************************", "", true, false);
	AddLogging("****************************************************************", "", true, false);
	AddLogging("TestBuild", sTestBuild, true, false);
	AddLogging("****************************************************************", "", true, false);
	AddLogging("****************************************************************", "", true, false);
	AddLogging("****************************************************************", "", true, false);
#endif

}

void CDlgLogging::OnBnClickedFlags()
{
	m_pDlgDebug->ShowWindow(SW_SHOW);
}

// DlgRulesLogging.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"	
#include "TemplateRemoteHistoryView.h"
#include "DlgRulesLogging.h"
#include "DlgLogging.h"
#include "xml.h"
#include <direct.h>
#include "afxdialogex.h"
#include "DlgSettingsHistory.h"
#include "Translation.h"


// CDlgRulesLogging dialog

IMPLEMENT_DYNAMIC(CDlgRulesLogging, CDialog)

CDlgRulesLogging::CDlgRulesLogging(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRulesLogging::IDD, pParent)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_psLogging = new CString;

	m_bFileOpen = false;
}

CDlgRulesLogging::~CDlgRulesLogging()
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

	delete m_psLogging;
}

BOOL CDlgRulesLogging::OnInitDialog()
{
	int		iCheck;
	CString sTxt;
	int iHeight, iWidth;
	CRect rect;

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	iCheck = theApp.GetProfileInt(registrySectionSettings, registryDebugLoggingRules, 0);
	m_checkEnableDebug.SetCheck(iCheck);
	m_bDebugMode = (iCheck == TRUE);

	m_sLogFileName = GetLogName();
	OpenLog();

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryLoggingRulesPosLeft, 200);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryLoggingRulesPosTop, 200);

	GetWindowRect(rect);
	iWidth = rect.Width();
	iHeight = rect.Height();
	theApp.ValidScreenPosition(&left, &top, &iWidth, &iHeight);
	
	SetWindowPos(&CWnd::wndBottom, left, top, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE ); //| SWP_NOSIZE

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgRulesLogging::Translate()
{
	this->SetWindowText(gszTranslation[PosDialogLoggingRulesTitle]);
	m_checkEnableDebug.SetWindowText(gszTranslation[PosDialogLoggingEnableDebug]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
	m_buttonClear.SetWindowText(gszTranslation[PosDialogCommonButtonsClear]);
}

void CDlgRulesLogging::AddLoggingDebug(CString sFrom, CString sLog, bool bTime)
{
	if (m_bDebugMode) AddLogging(sFrom, sLog, bTime);
}

void CDlgRulesLogging::AddLogging(CString sFrom, CString sLog, bool bTime, bool bFileOnly)
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

	if (iLen > LOGGING_RULES_MAX_BUFFER)
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

void CDlgRulesLogging::ShowLoggingText()
{
	if (m_logging.m_hWnd != NULL)							// logging is active before the dlg window is created
	{
		m_logging.SetWindowText(*m_psLogging);
		m_logging.LineScroll(0xffffff,0);
	}
}

void CDlgRulesLogging::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLoggingRulesPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLoggingRulesPosTop, wp.rcNormalPosition.top);
}


void CDlgRulesLogging::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCLEAR, m_buttonClear);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_EDIT_RULES_LOGGING, m_logging);
	DDX_Control(pDX, IDC_CHECK_DEBUG, m_checkEnableDebug);
}


BEGIN_MESSAGE_MAP(CDlgRulesLogging, CDialog)
	ON_MESSAGE (UWM_MSG_LOGGING_TEXT, OnLoggingText) 
	ON_BN_CLICKED(IDCLEAR, &CDlgRulesLogging::OnBnClickedClear)
	ON_MESSAGE (UWM_MSG_SHOW_RULES_DIALOG, OnShowDialog)	
	ON_BN_CLICKED(IDC_CHECK_DEBUG, &CDlgRulesLogging::OnBnClickedCheckDebug)
END_MESSAGE_MAP()


// DlgRulesLogging message handlers

LRESULT CDlgRulesLogging::OnShowDialog(WPARAM parm1, LPARAM parm2)
{
	theApp.m_pMainWnd->ShowWindow(SW_SHOW);
	theApp.m_pMainWnd->SetForegroundWindow();
	ShowWindow(SW_SHOW);
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	return 0;
}

LRESULT CDlgRulesLogging::OnLoggingText(WPARAM parm1, LPARAM parm2)
{
/*
	CLoggingMessage *pLog;
	bool	bTime; 

	pLog = (CLoggingMessage *) parm1;
	bTime = (parm2 != 0);


	AddLogging(pLog->m_pFrom, pLog->m_pText, bTime, false);

	return 0;
*/

	CLoggingMessage *pLog;
	int	 iType; 

	pLog = (CLoggingMessage *) parm1;
	iType = (int) parm2;
	if (iType == LOGGING_DEBUG)
	{
		AddLoggingDebug(pLog->m_pFrom, pLog->m_pText,true);
		return LOGGING_DEBUG;
	}
//	if (iType == LOGGING_DEBUGFULL)
//	{
//		AddLoggingDebugFull(pLog->m_pFrom, pLog->m_pText);
//		return LOGGING_DEBUGFULL;
//	}

	AddLogging(pLog->m_pFrom, pLog->m_pText);
	return LOGGING_NORMAL;
}
void CDlgRulesLogging::OnBnClickedClear()
{
	*m_psLogging = "";
	ShowLoggingText();
}

CString CDlgRulesLogging::GetLogName()
{
	CString sTxt, sDirectory;
	CXml	xml("");

	CString sPath;
	sPath = theApp.GetProfileString(registrySectionSettings,registryPath, "");

	sDirectory = xml.GetUserDirectory(sPath,false );
	sDirectory += "\\log\\";
	_mkdir(sDirectory);

	time_t tSystemTime;
	_time64(&tSystemTime);

	sTxt+= "rules-";
	CTime time(tSystemTime);
	sTxt += time.Format("%d-%m-%Y");
	sTxt+= ".log";

	sTxt = sDirectory + sTxt;

	return sTxt;
}


void CDlgRulesLogging::OpenLog()
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
	m_file.SeekToEnd();
	sTxt = "\r\nStart logging\r\n\r\n";
	AddLoggingToFile(&sTxt);

}

void CDlgRulesLogging::CheckDayChanged()
{
	CString sCurrent;

	if (m_bFileOpen)
	{
		sCurrent = GetLogName();
		if (sCurrent != m_sLogFileName)
		{
			m_file.Close();
			m_bFileOpen = false;
			m_sLogFileName = sCurrent;
			OpenLog();
		}
	}
}


void CDlgRulesLogging::AddLoggingToFile(CString *psTxt)
{
	if (m_bFileOpen)
	{
		CheckDayChanged();
		if (m_bFileOpen)	// must check again
		{
//			m_file.SeekToEnd();
			m_file.Write(*psTxt,psTxt->GetLength());
//			m_file.Flush();
		}
	}
}

void CDlgRulesLogging::OnBnClickedCheckDebug()
{
	int iCheck;
	iCheck = m_checkEnableDebug.GetCheck();

	if (iCheck)
	{
		m_bDebugMode = true;
//		AddLoggingInfo();
	}
	else
	{
		m_bDebugMode = false;
		AddLogging("Logging","Stop debug mode", true, false);
	}

	theApp.WriteProfileInt(registrySectionSettings,registryDebugLoggingRules , iCheck);

}

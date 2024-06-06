// DlgEditAppInfoConfig.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgEditAppInfoConfig.h"
#include "afxdialogex.h"


// CDlgEditAppInfoConfig dialog

IMPLEMENT_DYNAMIC(CDlgEditAppInfoConfig, DlgEditConfigBase)

CDlgEditAppInfoConfig::CDlgEditAppInfoConfig(CWnd* pParent /*=NULL*/)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgEditAppInfoConfig::~CDlgEditAppInfoConfig()
{
	DeleteNew();
}

void CDlgEditAppInfoConfig::DeleteNew()
{
//	delete m_AppConfig.m_pAppInfo;
//	delete m_AppConfig.m_pLogFlags;
//	m_AppConfig.m_pAppInfo = NULL;
//	m_AppConfig.m_pLogFlags = NULL;
	m_AppConfig.m_sRawData = "";
}

BOOL CDlgEditAppInfoConfig::OnInitDialog()
{
	DlgEditConfigBase::OnInitDialog();

//	m_AppConfig.m_pAppInfo = new APP_CONFIGS;
//	m_AppConfig.m_pLogFlags = new LOG_FLAGS;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgEditAppInfoConfig::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditAppInfoPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditAppInfoPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditAppInfoPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditAppInfoPosBottom, wp.rcNormalPosition.bottom);
}

void CDlgEditAppInfoConfig::OnBnClickedApply()
{
	bool	bSend;
	CString sTxt;
	CHARRANGE cr;

	if (m_bThreadBusy) return;

	int iFirstLine = m_window.GetFirstVisibleLine();
	int iLineIndex = m_window.LineIndex(iFirstLine);

	m_window.GetSel(cr);

	bSend = false;
	if (m_window.Check("app_config_editor.xml"))
	{
		bSend = true;
	}
	else
	{
		if (m_bIgnoreSyntax)
		{
			bSend = true;
		}
	}

	if (bSend)
	{
		m_window.HideSelection(TRUE, TRUE);
		m_window.SetSel(0, -1);
		sTxt = m_window.GetSelText();

		m_AppConfig.m_sRawData = sTxt.GetBuffer();
		sTxt.ReleaseBuffer();

		m_buttonApply.EnableWindow(FALSE);
		m_bThreadBusy = true;
		::PostThreadMessage(m_iThreadId, UWM_MSG_THREAD_SET_APP_CONFIG, (WPARAM) this->m_hWnd, (LPARAM)&m_AppConfig);
	}

	m_window.HideSelection(FALSE, TRUE);

	m_window.SetSel(iLineIndex, iLineIndex);
	m_window.SetSel(cr);
	m_window.SetFocus();
	m_window.Invalidate();
}

int CDlgEditAppInfoConfig::Parse(XML_PARSER& xp, MSG_VEC& mv)
{
	if (!xp.parse_start("app_config")) {
		mv.push_back("Missing <app_config> in app_config.xml, just paste the new text here.");
		return ERR_XML_PARSE;
	}
	return BOINC_SUCCESS;
}

void CDlgEditAppInfoConfig::DoDataExchange(CDataExchange* pDX)
{
	DlgEditConfigBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEditAppInfoConfig, DlgEditConfigBase)
	ON_BN_CLICKED(IDAPPLY, &CDlgEditAppInfoConfig::OnBnClickedApply)

	ON_MESSAGE(UWM_MSG_EDIT_APPP_INFO_OPEN_DIALOG, OnEditConfigOpen)
	ON_MESSAGE(UWM_MSG_THREAD_GET_APP_CONFIG_READY, OnConfigReadReady)
	ON_MESSAGE(UWM_MSG_THREAD_SET_APP_CONFIG_READY, OnConfigWriteReady)
END_MESSAGE_MAP()


// CDlgEditAppInfoConfig message handlers

LRESULT CDlgEditAppInfoConfig::OnEditConfigOpen(WPARAM parm1, LPARAM parm2)
{
	ShowWindow(SW_HIDE);					// hide while changing
	m_iThreadId = (int)parm1;

	if (!m_bThreadBusy)
	{
		CString *pProject = (CString *)parm2;
		m_AppConfig.m_sProject = *pProject;
		::PostThreadMessage(m_iThreadId, UWM_MSG_THREAD_GET_APP_CONFIG, (WPARAM) this->m_hWnd, (LPARAM)&m_AppConfig);
		m_bThreadBusy = true;
	}
	return 0;
}

LRESULT CDlgEditAppInfoConfig::OnConfigReadReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt, sBoincVersion;

	CRpcThreadReturn *pRpcThreadReturn;
	pRpcThreadReturn = (CRpcThreadReturn *)parm1;

	sTxt = pRpcThreadReturn->m_pcComputerId;

	sTxt = "BoincTasks App_config.xml - " + sTxt;
	this->SetWindowText(sTxt);

//	char	cBuffer[64000];
//	MIOFILE out;
//	out.init_buf_write(cBuffer, 64000);

//	LOG_FLAGS logFlags;
//	m_AppConfig.m_pAppInfo->write(out);

	m_bNeedSyntaxCheck = false;
	if (m_AppConfig.m_bValid)
	{
		if (m_AppConfig.m_sRawData.find("<error>") != std::string::npos)
		{
			std::string error = "AppConfig";
			MessageBox(m_AppConfig.m_sRawData.c_str(), "AppConfig", MB_ICONERROR | MB_OK);
			m_window.SetWindowText("");
		}
		else
		{
			m_window.SetWindowText(m_AppConfig.m_sRawData.c_str());
		}
	}
	else
	{
		m_window.SetWindowText("Data read is invalid");
	}
	Check();
	m_bThreadBusy = false;

	ShowWindow(SW_SHOW);
	delete pRpcThreadReturn;
	return 0;
}

LRESULT CDlgEditAppInfoConfig::OnConfigWriteReady(WPARAM parm1, LPARAM parm2)
{
	m_buttonApply.EnableWindow(TRUE);
	m_bThreadBusy = false;

	return 0;
}
// DlgEditConfig.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgEditConfig.h"

// CDlgEditConfig dialog

IMPLEMENT_DYNAMIC(CDlgEditConfig, DlgEditConfigBase)

CDlgEditConfig::CDlgEditConfig(CWnd* pParent /*=NULL*/) 
{

	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgEditConfig::~CDlgEditConfig()
{

	delete m_config.m_pConfig;
	delete m_config.m_pLogFlags;
}

BOOL CDlgEditConfig::OnInitDialog()
{
	DlgEditConfigBase::OnInitDialog();

	m_config.m_pConfig = NULL;
	m_config.m_pLogFlags = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgEditConfig::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditConfigPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditConfigPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditConfigPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryEditConfigPosBottom, wp.rcNormalPosition.bottom);
}

void CDlgEditConfig::OnBnClickedApply()
{
	bool	bSend;
	CString sTxt;
	CHARRANGE cr;

	if (m_bThreadBusy) return;

	int iFirstLine = m_window.GetFirstVisibleLine();
	int iLineIndex = m_window.LineIndex(iFirstLine);

	m_window.GetSel(cr);

	bSend = false;
	if (m_window.Check("config_editor.xml"))
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

		m_buttonApply.EnableWindow(FALSE);
		m_bThreadBusy = true;

		m_config.m_sRawData = sTxt.GetBuffer();
		sTxt.ReleaseBuffer();
		::PostThreadMessage(m_iThreadId, UWM_MSG_THREAD_SET_CONFIG, (WPARAM)this->m_hWnd, (LPARAM)&m_config);
	}

	m_window.HideSelection(FALSE, TRUE);

	m_window.SetSel(iLineIndex, iLineIndex);
	m_window.SetSel(cr);
	m_window.SetFocus();
	m_window.Invalidate();
}


BEGIN_MESSAGE_MAP(CDlgEditConfig, DlgEditConfigBase)
	ON_BN_CLICKED(IDAPPLY, &CDlgEditConfig::OnBnClickedApply)

	ON_MESSAGE(UWM_MSG_EDIT_CONFIG_OPEN_DIALOG, OnEditConfigOpen)
	ON_MESSAGE(UWM_MSG_THREAD_GET_CONFIG_READY, OnConfigReadReady)
	ON_MESSAGE(UWM_MSG_THREAD_SET_CONFIG_READY, OnConfigWriteReady)
END_MESSAGE_MAP()


// CDlgEditConfig message handlers

LRESULT CDlgEditConfig::OnEditConfigOpen(WPARAM parm1, LPARAM parm2)
{
	ShowWindow(SW_HIDE);					// hide while changing
	m_iThreadId = (int)parm1;

	if (!m_bThreadBusy)
	{
		::PostThreadMessage(m_iThreadId, UWM_MSG_THREAD_GET_CONFIG, (WPARAM) this->m_hWnd, (LPARAM)&m_config);
		m_bThreadBusy = true;
	}
	return 0;
}

LRESULT CDlgEditConfig::OnConfigReadReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt, sBoincVersion;

	CRpcThreadReturn* pRpcThreadReturn;
	pRpcThreadReturn = (CRpcThreadReturn*)parm1;

	sTxt = pRpcThreadReturn->m_pcComputerId;

	sTxt = "BoincTasks  cc_config.xml - " + sTxt;
	this->SetWindowText(sTxt);

	if (m_config.m_bValid)
	{
		if (m_config.m_sRawData.find("<error>") != std::string::npos)
		{
			std::string error = "AppConfig";
			MessageBox(m_config.m_sRawData.c_str(), "Config", MB_ICONERROR | MB_OK);
			m_window.SetWindowText("");
		}
		else
		{
			m_window.SetWindowText(m_config.m_sRawData.c_str());
		}
	}
	else
	{
		m_window.SetWindowText("Data read is invalid");
	}

	/*
	char	cBuffer[64000];
	MIOFILE out;
	out.init_buf_write(cBuffer, 64000);

	LOG_FLAGS logFlags;
	m_config.m_pConfig->write(out, *m_config.m_pLogFlags);

	xxx m_config.m_sRawData;
	*/

//	m_bNeedSyntaxCheck = false;
//	m_window.SetWindowText(cBuffer);
	Check();
	m_bThreadBusy = false;

	ShowWindow(SW_SHOW);
	delete pRpcThreadReturn;
	return 0;
}

LRESULT CDlgEditConfig::OnConfigWriteReady(WPARAM parm1, LPARAM parm2)
{
	m_buttonApply.EnableWindow(TRUE);
	m_bThreadBusy = false;

	return 0;
}

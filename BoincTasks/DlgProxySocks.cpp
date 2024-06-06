// DlgProxySocks.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgProxyMain.h"
#include "DlgProxyHttp.h"
#include "DlgProxySocks.h"
#include "afxdialogex.h"
#include "Translation.h"
#include "ThreadRpc.h"

// CDlgProxySocks dialog

IMPLEMENT_DYNAMIC(CDlgProxySocks, CPropertyPage)

CDlgProxySocks::CDlgProxySocks()
	: CPropertyPage(CDlgProxySocks::IDD)
{

}

CDlgProxySocks::~CDlgProxySocks()
{
}

BOOL CDlgProxySocks::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogProxySocksTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_checkProxyUse.SetWindowText(gszTranslation[PosDialogProxySocksConnect]);
	m_textProxySocksGroupHttp.SetWindowText(gszTranslation[PosDialogProxySocksGroup]);
	m_textProxySocksServer.SetWindowText(gszTranslation[PosDialogProxySocksServer]);
	m_textProxySocksPort.SetWindowText(gszTranslation[PosDialogProxySocksPort]);
	m_textProxySocksNoFor.SetWindowText(gszTranslation[PosDialogProxySocksNoFor]);

	m_textProxySocksGroupLeaveEmpty.SetWindowText(gszTranslation[PosDialogProxySocksGroupUser]);
	m_textProxySocksUser.SetWindowText(gszTranslation[PosDialogProxySocksUser]);
	m_textProxySocksPassword.SetWindowText(gszTranslation[PosDialogProxySocksPassword]);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);

	// translations

	m_bNoProxy = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgProxySocks::Enable(bool bEnable)
{
//	m_buttonOk.EnableWindow(bEnable);

	m_editProxySocksServer.EnableWindow(bEnable);
	m_editProxySocksPort.EnableWindow(bEnable);
	m_editProxySocksNoProxy.EnableWindow(bEnable);
	m_editProxySocksUser.EnableWindow(bEnable);
	m_editProxySocksPassword.EnableWindow(bEnable);
}

void CDlgProxySocks::FillDialog(GR_PROXY_INFO *pGrProxyInfo)
{
	bool bCheck;

	bCheck = pGrProxyInfo->use_socks_proxy;
	Enable(bCheck);
	m_checkProxyUse.SetCheck(bCheck);

	m_editProxySocksServer.SetWindowText(pGrProxyInfo->socks_server_name.c_str());
	int iPort = pGrProxyInfo->socks_server_port;
	CStringA sFormat;
	sFormat.Format("%d",iPort);
	m_editProxySocksPort.SetWindowText(sFormat);
	m_editProxySocksNoProxy.SetWindowText(pGrProxyInfo->noproxy_hosts.c_str());
	m_editProxySocksUser.SetWindowText(pGrProxyInfo->socks5_user_name.c_str());
	m_editProxySocksPassword.SetWindowText(pGrProxyInfo->socks5_user_passwd.c_str());

	m_bNoProxy = false;		// this one is set but shouldn't
}

bool CDlgProxySocks::ReadDialog(GR_PROXY_INFO *pGrProxyInfo)
{
	CString sTxt;
	bool bCheck;
	bCheck = m_checkProxyUse.GetCheck() == TRUE;
	pGrProxyInfo->use_socks_proxy = bCheck;

	m_editProxySocksServer.GetWindowText(sTxt);
	pGrProxyInfo->socks_server_name = sTxt;

	m_editProxySocksPort.GetWindowText(sTxt);
	pGrProxyInfo->socks_server_port = atoi(sTxt);

	if (m_bNoProxy)
	{
		m_editProxySocksNoProxy.GetWindowText(sTxt);
		pGrProxyInfo->noproxy_hosts = sTxt;
	}

	m_editProxySocksUser.GetWindowText(sTxt);
	pGrProxyInfo->socks5_user_name = sTxt;

	m_editProxySocksPassword.GetWindowText(sTxt);
	pGrProxyInfo->socks5_user_passwd = sTxt;
	
	return true;
}

void CDlgProxySocks::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_PROXY_SOCKS_GROUP_HTTP, m_textProxySocksGroupHttp);
	DDX_Control(pDX, IDC_TEXT_PROXY_SOCKS_SERVER, m_textProxySocksServer);
	DDX_Control(pDX, IDC_TEXT_PROXY_SOCKS_PORT, m_textProxySocksPort);
	DDX_Control(pDX, IDC_TEXT_PROXY_SOCKS_NO_FOR, m_textProxySocksNoFor);
	DDX_Control(pDX, IDC_TEXT_PROXY_SOCKS_USER, m_textProxySocksUser);
	DDX_Control(pDX, IDC_TEXT_PROXY_SOCKS_GROUP_LEAVE_EMPTY, m_textProxySocksGroupLeaveEmpty);
	DDX_Control(pDX, IDC_TEXT_PROXY_SOCKS_PASSWORD, m_textProxySocksPassword);
	DDX_Control(pDX, IDC_EDIT_PROXY_SOCKS_SERVER, m_editProxySocksServer);
	DDX_Control(pDX, IDC_EDIT_PROXY_SOCKS_PORT, m_editProxySocksPort);
	DDX_Control(pDX, IDC_EDIT_PROXY_SOCKS_NO_PROXY, m_editProxySocksNoProxy);
	DDX_Control(pDX, IDC_EDIT_PROXY_SOCKS_USER, m_editProxySocksUser);
	DDX_Control(pDX, IDC_EDIT_PROXY_SOCKS_PASSWORD, m_editProxySocksPassword);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_CHECK_PROXY_SOCKS_USE, m_checkProxyUse);
}


BEGIN_MESSAGE_MAP(CDlgProxySocks, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_PROXY_SOCKS_USE, &CDlgProxySocks::OnBnClickedCheckProxySocksUse)
	ON_BN_CLICKED(IDOK, &CDlgProxySocks::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgProxySocks::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_PROXY_SOCKS_SERVER, &CDlgProxySocks::OnEnChangeEditProxySocksServer)
	ON_EN_CHANGE(IDC_EDIT_PROXY_SOCKS_PORT, &CDlgProxySocks::OnEnChangeEditProxySocksPort)
	ON_EN_CHANGE(IDC_EDIT_PROXY_SOCKS_NO_PROXY, &CDlgProxySocks::OnEnChangeEditProxySocksNoProxy)
	ON_EN_CHANGE(IDC_EDIT_PROXY_SOCKS_USER, &CDlgProxySocks::OnEnChangeEditProxySocksUser)
	ON_EN_CHANGE(IDC_EDIT_PROXY_SOCKS_PASSWORD, &CDlgProxySocks::OnEnChangeEditProxySocksPassword)
END_MESSAGE_MAP()


// CDlgProxySocks message handlers

void CDlgProxySocks::OnBnClickedCheckProxySocksUse()
{
	bool bCheck;

	bCheck = (m_checkProxyUse.GetCheck() == TRUE); 

	Enable(bCheck);
}


void CDlgProxySocks::OnBnClickedOk()
{
	theApp.m_pDlgProxyMain->Ok();
}


void CDlgProxySocks::OnBnClickedCancel()
{
	theApp.m_pDlgProxyMain->Cancel();
}

void CDlgProxySocks::OnEnChangeEditProxySocksServer()
{

}


void CDlgProxySocks::OnEnChangeEditProxySocksPort()
{

}


void CDlgProxySocks::OnEnChangeEditProxySocksNoProxy()
{
	m_bNoProxy = true;
}


void CDlgProxySocks::OnEnChangeEditProxySocksUser()
{

}


void CDlgProxySocks::OnEnChangeEditProxySocksPassword()
{

}

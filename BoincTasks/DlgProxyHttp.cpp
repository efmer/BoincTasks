// DlgProxyHttp.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgProxyMain.h"
#include "DlgProxySocks.h"
#include "DlgProxyHttp.h"
#include "afxdialogex.h"
#include "Translation.h"
#include "ThreadRpc.h"


// CDlgProxyHttp dialog

IMPLEMENT_DYNAMIC(CDlgProxyHttp, CPropertyPage)

CDlgProxyHttp::CDlgProxyHttp()
	: CPropertyPage(CDlgProxyHttp::IDD)
{

}

CDlgProxyHttp::~CDlgProxyHttp()
{
}

BOOL CDlgProxyHttp::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogProxyHttpTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_checkProxyUse.SetWindowText(gszTranslation[PosDialogProxyHttpConnect]);

	m_textProxyHttpGroupHttp.SetWindowText(gszTranslation[PosDialogProxyHttpGroup]);
	m_textProxyHttpServer.SetWindowText(gszTranslation[PosDialogProxyHttpServer]);
	m_textProxyHttpPort.SetWindowText(gszTranslation[PosDialogProxyHttpPort]);
	m_textProxyHttpNoFor.SetWindowText(gszTranslation[PosDialogProxyHttpNoFor]);


	m_textProxyHttpGroupLeaveEmpty.SetWindowText(gszTranslation[PosDialogProxyHttpGroupUser]);
	m_textProxyHttpUser.SetWindowText(gszTranslation[PosDialogProxyHttpUser]);
	m_textProxyHttpPassword.SetWindowText(gszTranslation[PosDialogProxyHttpPassword]);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);

	// translations

	m_bNoProxy = false;


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgProxyHttp::Enable(bool bEnable)
{
//	m_buttonOk.EnableWindow(bEnable);

	m_editProxyHttpServer.EnableWindow(bEnable);
	m_editProxyHttpPort.EnableWindow(bEnable);
	m_editProxyHttpNoProxy.EnableWindow(bEnable);
	m_editProxyHttpUser.EnableWindow(bEnable);
	m_editProxyHttpPassword.EnableWindow(bEnable);
}

void CDlgProxyHttp::FillDialog(GR_PROXY_INFO *pGrProxyInfo)
{
	bool bCheck;

	bCheck = pGrProxyInfo->use_http_proxy;
	Enable(bCheck);
	m_checkProxyUse.SetCheck(bCheck);

	m_editProxyHttpServer.SetWindowText(pGrProxyInfo->http_server_name.c_str());
	int iPort = pGrProxyInfo->http_server_port;
	CStringA sFormat;
	sFormat.Format("%d",iPort);
	m_editProxyHttpPort.SetWindowText(sFormat);
	m_editProxyHttpNoProxy.SetWindowText(pGrProxyInfo->noproxy_hosts.c_str());
	m_editProxyHttpUser.SetWindowText(pGrProxyInfo->http_user_name.c_str());
	m_editProxyHttpPassword.SetWindowText(pGrProxyInfo->http_user_passwd.c_str());

	m_bNoProxy = false;		// this one is set but shouldn't

}

bool CDlgProxyHttp::ReadDialog(GR_PROXY_INFO *pGrProxyInfo)
{
	CString sTxt;
	bool bCheck;

	bCheck = m_checkProxyUse.GetCheck() == TRUE;
	pGrProxyInfo->use_http_proxy = bCheck;

	m_editProxyHttpServer.GetWindowText(sTxt);
	pGrProxyInfo->http_server_name = sTxt;

	m_editProxyHttpPort.GetWindowText(sTxt);
	pGrProxyInfo->http_server_port = atoi(sTxt);

	if (m_bNoProxy)
	{
		m_editProxyHttpNoProxy.GetWindowText(sTxt);
		pGrProxyInfo->noproxy_hosts = sTxt;
	}

	m_editProxyHttpUser.GetWindowText(sTxt);
	pGrProxyInfo->http_user_name = sTxt;

	m_editProxyHttpPassword.GetWindowText(sTxt);
	pGrProxyInfo->http_user_passwd = sTxt;
	
	return true;
}

BOOL CDlgProxyHttp::PreTranslateMessage(MSG* pMsg)	// catch enter and esc
{
	if (pMsg->wParam == VK_ESCAPE)
	{
		theApp.m_pDlgProxyMain->Cancel();
	}

	if (pMsg->wParam == VK_RETURN)
	{
		theApp.m_pDlgProxyMain->Ok();
	}	
	return CWnd::PreTranslateMessage(pMsg);
}

void CDlgProxyHttp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_PROXY_HTTP_GROUP_HTTP, m_textProxyHttpGroupHttp);
	DDX_Control(pDX, IDC_TEXT_PROXY_HTTP_SERVER, m_textProxyHttpServer);
	DDX_Control(pDX, IDC_TEXT_PROXY_HTTP_PORT, m_textProxyHttpPort);
	DDX_Control(pDX, IDC_TEXT_PROXY_HTTP_NO_FOR, m_textProxyHttpNoFor);
	DDX_Control(pDX, IDC_TEXT_PROXY_HTTP_USER, m_textProxyHttpUser);
	DDX_Control(pDX, IDC_TEXT_PROXY_HTTP_GROUP_LEAVE_EMPTY, m_textProxyHttpGroupLeaveEmpty);
	DDX_Control(pDX, IDC_TEXT_PROXY_HTTP_PASSWORD, m_textProxyHttpPassword);
	DDX_Control(pDX, IDC_EDIT_PROXY_HTTP_SERVER, m_editProxyHttpServer);
	DDX_Control(pDX, IDC_EDIT_PROXY_HTTP_PORT, m_editProxyHttpPort);
	DDX_Control(pDX, IDC_EDIT_PROXY_HTTP_NO_PROXY, m_editProxyHttpNoProxy);
	DDX_Control(pDX, IDC_EDIT_PROXY_HTTP_USER, m_editProxyHttpUser);
	DDX_Control(pDX, IDC_EDIT_PROXY_HTTP_PASSWORD, m_editProxyHttpPassword);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_CHECK_PROXY_HTTP_USE, m_checkProxyUse);
}


BEGIN_MESSAGE_MAP(CDlgProxyHttp, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_PROXY_HTTP_USE, &CDlgProxyHttp::OnBnClickedCheckProxyHttpUse)
	ON_BN_CLICKED(IDOK, &CDlgProxyHttp::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgProxyHttp::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_PROXY_HTTP_SERVER, &CDlgProxyHttp::OnEnChangeEditProxyHttpServer)
	ON_EN_CHANGE(IDC_EDIT_PROXY_HTTP_PORT, &CDlgProxyHttp::OnEnChangeEditProxyHttpPort)
	ON_EN_CHANGE(IDC_EDIT_PROXY_HTTP_NO_PROXY, &CDlgProxyHttp::OnEnChangeEditProxyHttpNoProxy)
	ON_EN_CHANGE(IDC_EDIT_PROXY_HTTP_USER, &CDlgProxyHttp::OnEnChangeEditProxyHttpUser)
	ON_EN_CHANGE(IDC_EDIT_PROXY_HTTP_PASSWORD, &CDlgProxyHttp::OnEnChangeEditProxyHttpPassword)
END_MESSAGE_MAP()


// CDlgProxyHttp message handlers


void CDlgProxyHttp::OnBnClickedCheckProxyHttpUse()
{
	bool bCheck;

	bCheck = (m_checkProxyUse.GetCheck() == TRUE); 

	Enable(bCheck);
}


void CDlgProxyHttp::OnBnClickedOk()
{
	theApp.m_pDlgProxyMain->Ok();
}


void CDlgProxyHttp::OnBnClickedCancel()
{
	theApp.m_pDlgProxyMain->Cancel();
}


void CDlgProxyHttp::OnEnChangeEditProxyHttpServer()
{

}


void CDlgProxyHttp::OnEnChangeEditProxyHttpPort()
{

}


void CDlgProxyHttp::OnEnChangeEditProxyHttpNoProxy()
{
	m_bNoProxy = true;
}


void CDlgProxyHttp::OnEnChangeEditProxyHttpUser()
{

}


void CDlgProxyHttp::OnEnChangeEditProxyHttpPassword()
{

}

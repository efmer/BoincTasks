// CDlgHtmlAccept.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgHtmlAccept.h"


// CDlgHtmlAccept dialog

IMPLEMENT_DYNCREATE(CDlgHtmlAccept, CDHtmlDialog)

CDlgHtmlAccept::CDlgHtmlAccept(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_CDlgHtmlAccept, 119, pParent)
{

}

CDlgHtmlAccept::~CDlgHtmlAccept()
{
}

void CDlgHtmlAccept::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDlgHtmlAccept::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDlgHtmlAccept, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgHtmlAccept)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDlgHtmlAccept message handlers

HRESULT CDlgHtmlAccept::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDlgHtmlAccept::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

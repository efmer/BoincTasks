// DlgSearch.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSearch.h"
#include "afxdialogex.h"

#include "Translation.h"


// CDlgSearch dialog

IMPLEMENT_DYNAMIC(CDlgSearch, CDialogEx)

CDlgSearch::CDlgSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSearch::IDD, pParent)
{

}

CDlgSearch::~CDlgSearch()
{
}

void CDlgSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_EDIT_SEARCH_MESSAGE, m_editSearch);
}


BEGIN_MESSAGE_MAP(CDlgSearch, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSearch::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSearch message handlers


BOOL CDlgSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

// Translation
	
	SetWindowText(gszTranslation[PosDialogMessageSearchTitle]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

//	this->NextDlgCtrl();
//	this->NextDlgCtrl();
//	this->SetFocus();
//	CWnd *pDlgItem = GetDlgItem(IDC_EDIT_SEARCH_MESSAGE);
//	GotoDlgCtrl(pDlgItem); 

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgSearch::OnBnClickedOk()
{
	m_editSearch.GetWindowText(m_sSearch);
	CDialogEx::OnOK();
}

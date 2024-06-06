// DlgColorProjects.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorProjects.h"
#include "Translation.h"

// CDlgColorProjects dialog

IMPLEMENT_DYNAMIC(CDlgColorProjects, CPropertyPage)

CDlgColorProjects::CDlgColorProjects()
	: CPropertyPage(CDlgColorProjects::IDD)
{

}

CDlgColorProjects::~CDlgColorProjects()
{
}

BOOL CDlgColorProjects::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	m_colorProjects[COLOR_ARRAY_PROJECT_NORMAL]		= theApp.GetProfileInt(registrySectionColor, registryColorProjectNormal, COLOR_PROJECT_NORMAL);
	m_colorProjects[COLOR_ARRAY_PROJECT_SUSPENDED]	= theApp.GetProfileInt(registrySectionColor, registryColorProjectSuspended, COLOR_PROJECT_SUSPENDED);
	m_colorProjects[COLOR_ARRAY_PROJECT_NONEWWORK]	= theApp.GetProfileInt(registrySectionColor, registryColorProjectNoNewWork, COLOR_PROJECT_NONEWWORK);

	m_bColorWindowUpdate= true;

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleProjects];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonRunning.SetWindowText(gszTranslation[PosDialogColorProjectsRunning]);
	m_buttonSuspended.SetWindowText(gszTranslation[PosDialogColorProjectsSuspended]);
	m_buttonNoNewTasks.SetWindowText(gszTranslation[PosDialogColorProjectsNoNewTasks]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
// Translation

	SetTimer(UTIMER_DLG_COLOR_PROJECTS, 1000, 0);	// interval 1 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgColorProjects::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_COLOR_PROJECT, m_wndProjects[COLOR_ARRAY_PROJECT_NORMAL]);
	DDX_Control(pDX, IDC_WND_COLOR_PROJECT_SUSPENDED, m_wndProjects[COLOR_ARRAY_PROJECT_SUSPENDED]);
	DDX_Control(pDX, IDC_WND_COLOR_PROJECT_NONEW, m_wndProjects[COLOR_ARRAY_PROJECT_NONEWWORK]);
	DDX_Control(pDX, IDC_BUTTON_COLOR_PROJECT, m_buttonRunning);
	DDX_Control(pDX, IDC_BUTTON_COLOR_PROJECT_SUSPENDED, m_buttonSuspended);
	DDX_Control(pDX, IDC_BUTTON_COLOR_PROJECT_NONEW, m_buttonNoNewTasks);
	DDX_Control(pDX, IDOK, m_buttonOk);
}

COLORREF *CDlgColorProjects::GetColorArray()
{
	return &m_colorProjects[0];
}

void CDlgColorProjects::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColor,registryColorProjectNormal , m_colorProjects[COLOR_ARRAY_PROJECT_NORMAL]);
	theApp.WriteProfileInt(registrySectionColor,registryColorProjectSuspended , m_colorProjects[COLOR_ARRAY_PROJECT_SUSPENDED]);
	theApp.WriteProfileInt(registrySectionColor,registryColorProjectNoNewWork , m_colorProjects[COLOR_ARRAY_PROJECT_NONEWWORK]);
	m_bColorWindowUpdate= true;
}

BEGIN_MESSAGE_MAP(CDlgColorProjects, CPropertyPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COLOR_PROJECT, &CDlgColorProjects::OnBnClickedButtonColorProject)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_PROJECT_SUSPENDED, &CDlgColorProjects::OnBnClickedButtonColorProjectSuspended)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_PROJECT_NONEW, &CDlgColorProjects::OnBnClickedButtonColorProjectNoNew)
	ON_BN_CLICKED(IDOK, &CDlgColorProjects::OnBnClickedOk)
END_MESSAGE_MAP()

// CDlgColorProjects message handlers

void CDlgColorProjects::OnTimer(UINT_PTR nIDEvent)
{
	int iCount;
	if (m_bColorWindowUpdate)
	{
		m_bColorWindowUpdate = false;
		for (iCount = 0; iCount < COLOR_ARRAY_NUMBER_PROJECTS; iCount++)
		{
			m_wndProjects[iCount].SetColor(m_colorProjects[iCount]);
			m_wndProjects[iCount].Invalidate(TRUE);
		}
//		::PostMessage(m_hWnd, WM_GRAPHIC_COLOR_CHANGED, 0, 0);
	}
}

void CDlgColorProjects::OnBnClickedButtonColorProject()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorProjects[COLOR_ARRAY_PROJECT_NORMAL],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorProjects[COLOR_ARRAY_PROJECT_NORMAL] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorProjectNormal , m_colorProjects[COLOR_ARRAY_PROJECT_NORMAL]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorProjects::OnBnClickedButtonColorProjectSuspended()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorProjects[COLOR_ARRAY_PROJECT_SUSPENDED],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorProjects[COLOR_ARRAY_PROJECT_SUSPENDED] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorProjectSuspended , m_colorProjects[COLOR_ARRAY_PROJECT_SUSPENDED]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorProjects::OnBnClickedButtonColorProjectNoNew()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorProjects[COLOR_ARRAY_PROJECT_NONEWWORK],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorProjects[COLOR_ARRAY_PROJECT_NONEWWORK] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorProjectNoNewWork , m_colorProjects[COLOR_ARRAY_PROJECT_NONEWWORK]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

COLORREF CDlgColorProjects::Correct(COLORREF color)
{
	int r,g,b;

	r = color >> 16; 
	g = (color >> 8) & 0xff;
	b = color & 0xff;

	if ((r) < 20) r = 20;
	if ((g) < 20) g = 20;
	if ((b) < 20) b = 20;

	color = (r << 16) | (g << 8) | b;
	return color;

}

void CDlgColorProjects::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}

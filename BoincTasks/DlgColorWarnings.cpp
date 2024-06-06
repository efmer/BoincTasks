// DlgColorWarnings.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorWarnings.h"
#include "Translation.h"

// CDlgColorWarnings dialog

IMPLEMENT_DYNAMIC(CDlgColorWarnings, CPropertyPage)

CDlgColorWarnings::CDlgColorWarnings()
	: CPropertyPage(CDlgColorWarnings::IDD)
{

}

CDlgColorWarnings::~CDlgColorWarnings()
{
}

void CDlgColorWarnings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_COLOR_ERROR_RATIO, m_wndWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO]);
	DDX_Control(pDX, IDC_WND_COLOR_ERROR_RATIO_GPU, m_wndWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_DEADLINE_WARNING, m_wndWarnings[COLOR_WARNINGS_ARRAY_DEADLINE]);
	DDX_Control(pDX, IDC_WND_COLOR_DEADLINE_WARNING_RUNNING, m_wndWarnings[COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING]);
	DDX_Control(pDX, IDC_WND_COLOR_COUNT_WARNING, m_wndWarnings[COLOR_WARNINGS_ARRAY_TASKS]);
	DDX_Control(pDX, IDC_WND_COLOR_CHECKPOINT_WARNING, m_wndWarnings[COLOR_WARNINGS_ARRAY_CHECKPOINT]);


	DDX_Control(pDX, IDC_BUTTON_COLOR_ERROR_RATIO, m_buttonErrorCpuRatio);
	DDX_Control(pDX, IDC_BUTTON_COLOR_ERROR_RATIO_GPU, m_buttonErrorGpuRatio);
	DDX_Control(pDX, IDC_BUTTON_COLOR_DEADLINE_WARNING, m_buttonDeadlineWarning);
	DDX_Control(pDX, IDC_BUTTON_COLOR_DEADLINE_WARNING_RUNNING, m_buttonDeadlineWarningRunning);
	DDX_Control(pDX, IDC_BUTTON_COLOR_COUNT_WARNING, m_buttonTaskWarning);
	DDX_Control(pDX, IDC_BUTTON_COLOR_CHECKPOINT_WARNING, m_buttonCheckpointWarning);
	DDX_Control(pDX, IDOK, m_buttonOk);

}

BOOL CDlgColorWarnings::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO] = theApp.GetProfileInt(registrySectionColor, registryColorWarningsErrorRatio, COLOR_WARNINGS_ERROR_RATIO);
	m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU] = theApp.GetProfileInt(registrySectionColor, registryColorWarningsErrorRatioGpu, COLOR_WARNINGS_ERROR_RATIO_GPU);
	m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE] = theApp.GetProfileInt(registrySectionColor, registryColorWarningsDeadline, COLOR_WARNINGS_DEADLINE);
	m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING] = theApp.GetProfileInt(registrySectionColor, registryColorWarningsDeadlineRunning, COLOR_WARNINGS_DEADLINE);
	m_colorWarnings[COLOR_WARNINGS_ARRAY_TASKS] = theApp.GetProfileInt(registrySectionColor, registryColorWarningsTasks, COLOR_WARNINGS_TASKS);
	m_colorWarnings[COLOR_WARNINGS_ARRAY_CHECKPOINT] = theApp.GetProfileInt(registrySectionColor, registryColorWarningsCheckpoint, COLOR_WARNINGS_CHECKPOINT);

	m_bColorWindowUpdate= true;

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleWarnings];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonErrorCpuRatio.SetWindowText(gszTranslation[PosDialogColorCpuRatio]);
	m_buttonErrorGpuRatio.SetWindowText(gszTranslation[PosDialogColorGpuRatio]);
	m_buttonDeadlineWarning.SetWindowText(gszTranslation[PosDialogColorDeadline]);
	m_buttonDeadlineWarningRunning.SetWindowText(gszTranslation[PosDialogColorDeadlineRunning]);
	m_buttonTaskWarning.SetWindowText(gszTranslation[PosDialogColorTasks]);
	m_buttonCheckpointWarning.SetWindowText(gszTranslation[PosDialogColorCheckpoint]);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

// Translation

	SetTimer(UTIMER_DLG_COLOR_WARNINGS, 1000, 0);	// interval 1 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

COLORREF *CDlgColorWarnings::GetColorArray()
{
	return &m_colorWarnings[0];
}

void CDlgColorWarnings::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColor,registryColorWarningsErrorRatio , m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO]);
	theApp.WriteProfileInt(registrySectionColor,registryColorWarningsErrorRatioGpu , m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorWarningsDeadline , m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE]);
	theApp.WriteProfileInt(registrySectionColor,registryColorWarningsDeadlineRunning , m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING]);
	theApp.WriteProfileInt(registrySectionColor,registryColorWarningsTasks , m_colorWarnings[COLOR_WARNINGS_ARRAY_TASKS]);
	theApp.WriteProfileInt(registrySectionColor,registryColorWarningsCheckpoint , m_colorWarnings[COLOR_WARNINGS_ARRAY_CHECKPOINT]);
	m_bColorWindowUpdate= true;
}

BEGIN_MESSAGE_MAP(CDlgColorWarnings, CPropertyPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ERROR_RATIO, &CDlgColorWarnings::OnBnClickedButtonColorErrorRatio)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ERROR_RATIO_GPU, &CDlgColorWarnings::OnBnClickedButtonColorErrorRatioGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_DEADLINE_WARNING, &CDlgColorWarnings::OnBnClickedButtonColorDeadlineWarning)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_DEADLINE_WARNING_RUNNING, &CDlgColorWarnings::OnBnClickedButtonColorDeadlineWarningRunning)
	ON_BN_CLICKED(IDOK, &CDlgColorWarnings::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_COUNT_WARNING, &CDlgColorWarnings::OnBnClickedButtonColorCountWarning)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_CHECKPOINT_WARNING, &CDlgColorWarnings::OnBnClickedButtonColorCheckpointWarning)
END_MESSAGE_MAP()


// CDlgColorWarnings message handlers

void CDlgColorWarnings::OnTimer(UINT_PTR nIDEvent)
{
	int iCount;
	if (m_bColorWindowUpdate)
	{
		m_bColorWindowUpdate = false;
		for (iCount = 0; iCount < COLOR_ARRAY_NUMBER_WARNINGS; iCount++)
		{
			m_wndWarnings[iCount].SetColor(m_colorWarnings[iCount]);
			m_wndWarnings[iCount].Invalidate(TRUE);
		}
//		::PostMessage(m_hWnd, WM_GRAPHIC_COLOR_CHANGED, 0, 0);
	}
}

void CDlgColorWarnings::OnBnClickedButtonColorErrorRatio()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorWarningsErrorRatio , m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorWarnings::OnBnClickedButtonColorErrorRatioGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorWarningsErrorRatioGpu , m_colorWarnings[COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorWarnings::OnBnClickedButtonColorDeadlineWarning()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorWarningsDeadline , m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorWarnings::OnBnClickedButtonColorDeadlineWarningRunning()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorWarningsDeadlineRunning , m_colorWarnings[COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorWarnings::OnBnClickedButtonColorCountWarning()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorWarnings[COLOR_WARNINGS_ARRAY_TASKS],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorWarnings[COLOR_WARNINGS_ARRAY_TASKS] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorWarningsTasks , m_colorWarnings[COLOR_WARNINGS_ARRAY_TASKS]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorWarnings::OnBnClickedButtonColorCheckpointWarning()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorWarnings[COLOR_WARNINGS_ARRAY_CHECKPOINT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorWarnings[COLOR_WARNINGS_ARRAY_CHECKPOINT] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorWarningsCheckpoint , m_colorWarnings[COLOR_WARNINGS_ARRAY_CHECKPOINT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


COLORREF CDlgColorWarnings::Correct(COLORREF color)
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

void CDlgColorWarnings::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}




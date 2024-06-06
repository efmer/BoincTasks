// DlgColorHistory.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorHistory.h"
#include "Translation.h"

// CDlgColorHistory dialog

IMPLEMENT_DYNAMIC(CDlgColorHistory, CPropertyPage)

CDlgColorHistory::CDlgColorHistory()
	: CPropertyPage(CDlgColorHistory::IDD)
{

}

CDlgColorHistory::~CDlgColorHistory()
{
}

BOOL CDlgColorHistory::OnInitDialog()
{
	CString sTxt;
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD]				= theApp.GetProfileInt(registrySectionColor, registryColorHistoryUpload, COLOR_HISTORY_UPLOAD);
	m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD_GPU]			= theApp.GetProfileInt(registrySectionColor, registryColorHistoryUploadGPU, COLOR_HISTORY_UPLOAD_GPU);
	m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT]		= theApp.GetProfileInt(registrySectionColor, registryColorHistoryReadyToReport, COLOR_HISTORY_READY_TO_REPORT);
	m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU]	= theApp.GetProfileInt(registrySectionColor, registryColorHistoryReadyToReportGPU, COLOR_HISTORY_READY_TO_REPORT_GPU);
	m_colorHistory[COLOR_HISTORY_ARRAY_ABORT]				= theApp.GetProfileInt(registrySectionColor, registryColorHistoryAbort, COLOR_HISTORY_ABORT);
	m_colorHistory[COLOR_HISTORY_ARRAY_ERROR]				= theApp.GetProfileInt(registrySectionColor, registryColorHistoryError, COLOR_HISTORY_ERROR);
	m_colorHistory[COLOR_HISTORY_ARRAY_ERROR_GPU]			= theApp.GetProfileInt(registrySectionColor, registryColorHistoryErrorGPU, COLOR_HISTORY_ERROR_GPU);
	m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED]			= theApp.GetProfileInt(registrySectionColor, registryColorHistoryReported, COLOR_HISTORY_REPORTED);
	m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_GPU]		= theApp.GetProfileInt(registrySectionColor, registryColorHistoryReportedGPU, COLOR_HISTORY_REPORTED_GPU);
	m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR]		= theApp.GetProfileInt(registrySectionColor, registryColorHistoryReportedError, COLOR_HISTORY_REPORTED_ERROR);
	m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU]	= theApp.GetProfileInt(registrySectionColor, registryColorHistoryReportedErrorGPU, COLOR_HISTORY_REPORTED_ERROR_GPU);
	m_colorHistory[COLOR_HISTORY_ARRAY_MISSED]				= theApp.GetProfileInt(registrySectionColor, registryColorHistoryMissed, COLOR_HISTORY_MISSED);
	m_colorHistory[COLOR_HISTORY_ARRAY_MISSED_GPU]			= theApp.GetProfileInt(registrySectionColor, registryColorHistoryMissedGpu, COLOR_HISTORY_MISSED_GPU);
	m_bColorWindowUpdate= true;

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleHistory];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonUpload.SetWindowText(gszTranslation[PosDialogColorUpload]);
	m_buttonUploadGpu.SetWindowText(gszTranslation[PosDialogColorUploadGpu]);
	m_buttonReadyToReport.SetWindowText(gszTranslation[PosDialogColorReadyToReport]);
	m_buttonReadyToReportGpu.SetWindowText(gszTranslation[PosDialogColorReadyToReportGpu]);
	m_buttonAbort.SetWindowText(gszTranslation[PosDialogColorAbort]);
	m_buttonError.SetWindowText(gszTranslation[PosDialogColorError]);
	m_buttonErrorGpu.SetWindowText(gszTranslation[PosDialogColorErrorGpu]);
	m_buttonReported.SetWindowText(gszTranslation[PosDialogColorReported]);
	m_buttonReportedGpu.SetWindowText(gszTranslation[PosDialogColorReportedGpu]);
	m_buttonReportedError.SetWindowText(gszTranslation[PosDialogColorReportedError]);
	m_buttonReportedErrorGpu.SetWindowText(gszTranslation[PosDialogColorReportedErrorGpu]);
	m_buttonMissed.SetWindowText(gszTranslation[PosDialogColorMissed]);
	sTxt = gszTranslation[PosDialogColorMissed];
	sTxt+= " Gpu";
	m_buttonMissedGpu.SetWindowText(sTxt);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
// Translation

	SetTimer(UTIMER_DLG_COLOR_HISTORY, 1000, 0);	// interval 1 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

COLORREF *CDlgColorHistory::GetColorArray()
{
	return &m_colorHistory[0];
}

void CDlgColorHistory::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryUpload , m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryUploadGPU , m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReadyToReport , m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReadyToReportGPU , m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryAbort , m_colorHistory[COLOR_HISTORY_ARRAY_ABORT]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryError , m_colorHistory[COLOR_HISTORY_ARRAY_ERROR]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryErrorGPU , m_colorHistory[COLOR_HISTORY_ARRAY_ERROR_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReported , m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReportedGPU, m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReportedError , m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReportedErrorGPU , m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryMissed , m_colorHistory[COLOR_HISTORY_ARRAY_MISSED]);
	theApp.WriteProfileInt(registrySectionColor,registryColorHistoryMissedGpu , m_colorHistory[COLOR_HISTORY_ARRAY_MISSED_GPU]);

	m_bColorWindowUpdate= true;
}

void CDlgColorHistory::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_COLOR_UPLOAD, m_wndHistory[COLOR_HISTORY_ARRAY_UPLOAD]);
	DDX_Control(pDX, IDC_WND_COLOR_UPDOWN_GPU, m_wndHistory[COLOR_HISTORY_ARRAY_UPLOAD_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_READY, m_wndHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT]);
	DDX_Control(pDX, IDC_WND_COLOR_READY_GPU, m_wndHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_ABORT, m_wndHistory[COLOR_HISTORY_ARRAY_ABORT]);
	DDX_Control(pDX, IDC_WND_COLOR_ERROR, m_wndHistory[COLOR_HISTORY_ARRAY_ERROR]);
	DDX_Control(pDX, IDC_WND_COLOR_ERROR_GPU, m_wndHistory[COLOR_HISTORY_ARRAY_ERROR_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_REPORTED, m_wndHistory[COLOR_HISTORY_ARRAY_REPORTED]);
	DDX_Control(pDX, IDC_WND_COLOR_REPORTED_GPU, m_wndHistory[COLOR_HISTORY_ARRAY_REPORTED_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_REPORTED_ERROR, m_wndHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR]);
	DDX_Control(pDX, IDC_WND_COLOR_REPORTED_ERROR_GPU, m_wndHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_MISSED, m_wndHistory[COLOR_HISTORY_ARRAY_MISSED]);
	DDX_Control(pDX, IDC_WND_COLOR_MISSED_GPU, m_wndHistory[COLOR_HISTORY_ARRAY_MISSED_GPU]);

	DDX_Control(pDX, IDC_BUTTON_COLOR_UPLOAD, m_buttonUpload);
	DDX_Control(pDX, IDC_BUTTON_COLOR_UPLOAD_GPU, m_buttonUploadGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_READY, m_buttonReadyToReport);
	DDX_Control(pDX, IDC_BUTTON_COLOR_READY_GPU, m_buttonReadyToReportGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_ABORT, m_buttonAbort);
	DDX_Control(pDX, IDC_BUTTON_COLOR_ERROR, m_buttonError);
	DDX_Control(pDX, IDC_BUTTON_COLOR_ERROR_GPU, m_buttonErrorGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_REPORTED, m_buttonReported);
	DDX_Control(pDX, IDC_BUTTON_COLOR_REPORTED_GPU, m_buttonReportedGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_REPORTED_ERROR, m_buttonReportedError);
	DDX_Control(pDX, IDC_BUTTON_COLOR_REPORTED_ERROR_GPU, m_buttonReportedErrorGpu);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MISSED, m_buttonMissed);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MISSED_GPU, m_buttonMissedGpu);
}


BEGIN_MESSAGE_MAP(CDlgColorHistory, CPropertyPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COLOR_UPLOAD, &CDlgColorHistory::OnBnClickedButtonColorUpload)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_UPLOAD_GPU, &CDlgColorHistory::OnBnClickedButtonColorUploadGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_READY, &CDlgColorHistory::OnBnClickedButtonColorReady)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_READY_GPU, &CDlgColorHistory::OnBnClickedButtonColorReadyGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ABORT, &CDlgColorHistory::OnBnClickedButtonColorAbort)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ERROR, &CDlgColorHistory::OnBnClickedButtonColorError)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ERROR_GPU, &CDlgColorHistory::OnBnClickedButtonColorErrorGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_REPORTED, &CDlgColorHistory::OnBnClickedButtonColorReported)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_REPORTED_GPU, &CDlgColorHistory::OnBnClickedButtonColorReportedGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_REPORTED_ERROR, &CDlgColorHistory::OnBnClickedButtonColorReportedError)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_REPORTED_ERROR_GPU, &CDlgColorHistory::OnBnClickedButtonColorReportedErrorGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MISSED, &CDlgColorHistory::OnBnClickedButtonColorMissed)
	ON_BN_CLICKED(IDOK, &CDlgColorHistory::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MISSED_GPU, &CDlgColorHistory::OnBnClickedButtonColorMissedGpu)
END_MESSAGE_MAP()


// CDlgColorHistory message handlers

void CDlgColorHistory::OnTimer(UINT_PTR nIDEvent)
{
	int iCount;
	if (m_bColorWindowUpdate)
	{
		m_bColorWindowUpdate = false;
		for (iCount = 0; iCount < COLOR_ARRAY_NUMBER_HISTORY; iCount++)
		{
			m_wndHistory[iCount].SetColor(m_colorHistory[iCount]);
			m_wndHistory[iCount].Invalidate(TRUE);
		}
	}
}

void CDlgColorHistory::OnBnClickedButtonColorUpload()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryUpload , m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorUploadGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryUploadGPU , m_colorHistory[COLOR_HISTORY_ARRAY_UPLOAD_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorReady()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReadyToReport , m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorReadyGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReadyToReportGPU , m_colorHistory[COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorAbort()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_ABORT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_ABORT] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryAbort , m_colorHistory[COLOR_HISTORY_ARRAY_ABORT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorError()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_ERROR],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_ERROR] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryError , m_colorHistory[COLOR_HISTORY_ARRAY_ERROR]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorErrorGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_ERROR_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_ERROR_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryErrorGPU , m_colorHistory[COLOR_HISTORY_ARRAY_ERROR_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorReported()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReported , m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorReportedGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReportedGPU, m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorReportedError()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReportedError , m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorReportedErrorGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryReportedErrorGPU , m_colorHistory[COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorMissed()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_MISSED],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_MISSED] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryMissed , m_colorHistory[COLOR_HISTORY_ARRAY_MISSED]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorHistory::OnBnClickedButtonColorMissedGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorHistory[COLOR_HISTORY_ARRAY_MISSED_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorHistory[COLOR_HISTORY_ARRAY_MISSED_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorHistoryMissedGpu , m_colorHistory[COLOR_HISTORY_ARRAY_MISSED_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

COLORREF CDlgColorHistory::Correct(COLORREF color)
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
void CDlgColorHistory::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}




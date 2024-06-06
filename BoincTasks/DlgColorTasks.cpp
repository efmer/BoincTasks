// DlgColorTasks.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorTasks.h"
#include "Translation.h"

// CDlgColorTasks dialog

IMPLEMENT_DYNAMIC(CDlgColorTasks, CPropertyPage)

CDlgColorTasks::CDlgColorTasks()
	: CPropertyPage(CDlgColorTasks::IDD)
{

}

CDlgColorTasks::~CDlgColorTasks()
{
}

BOOL CDlgColorTasks::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	m_colorTasks[COLOR_TASKS_ARRAY_ABORT]				= theApp.GetProfileInt(registrySectionColor, registryColorTasksAbort, COLOR_TASKS_ABORT);
	m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD]		= theApp.GetProfileInt(registrySectionColor, registryColorTasksUploadDownload, COLOR_TASKS_UPLOAD_DOWNLOAD);
	m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU]	= theApp.GetProfileInt(registrySectionColor, registryColorTasksUploadDownloadGPU, COLOR_TASKS_UPLOAD_DOWNLOAD_GPU);
	m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT]		= theApp.GetProfileInt(registrySectionColor, registryColorTasksReadyToReport, COLOR_TASKS_READY_TO_REPORT);
	m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU]	= theApp.GetProfileInt(registrySectionColor, registryColorTasksReadyToReportGPU, COLOR_TASKS_READY_TO_REPORT_GPU);
	m_colorTasks[COLOR_TASKS_ARRAY_RUNNING]				= theApp.GetProfileInt(registrySectionColor, registryColorTasksRunning, COLOR_TASKS_RUNNING);
	m_colorTasks[COLOR_TASKS_ARRAY_RUNNING_GPU]			= theApp.GetProfileInt(registrySectionColor, registryColorTasksRunningGPU, COLOR_TASKS_RUNNING_GPU);
	m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY]		= theApp.GetProfileInt(registrySectionColor, registryColorTasksHighPriority, COLOR_TASKS_HIGH_PRIORITY);
	m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU]	= theApp.GetProfileInt(registrySectionColor, registryColorTasksHighPriorityGpu, COLOR_TASKS_HIGH_PRIORITY_GPU);
	m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN]		= theApp.GetProfileInt(registrySectionColor, registryColorTasksWaitingToRun, COLOR_TASKS_WAITING_TO_RUN);
	m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU]	= theApp.GetProfileInt(registrySectionColor, registryColorTasksWaitingToRunGPU, COLOR_TASKS_WAITING_TO_RUN_GPU);
	m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START]		= theApp.GetProfileInt(registrySectionColor, registryColorTasksReadyToStart, COLOR_TASKS_READY_TO_START);
	m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START_GPU]	= theApp.GetProfileInt(registrySectionColor, registryColorTasksReadyToStartGPU, COLOR_TASKS_READY_TO_START_GPU);
	m_colorTasks[COLOR_TASKS_ARRAY_ERROR]				= theApp.GetProfileInt(registrySectionColor, registryColorTasksError, COLOR_TASKS_ERROR);
	m_colorTasks[COLOR_TASKS_ARRAY_ERROR_GPU]			= theApp.GetProfileInt(registrySectionColor, registryColorTasksErrorGPU, COLOR_TASKS_ERROR_GPU);
	m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED]			= theApp.GetProfileInt(registrySectionColor, registryColorTasksSuspended, COLOR_TASKS_SUSPENDED);
	m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED_GPU]		= theApp.GetProfileInt(registrySectionColor, registryColorTasksSuspendedGPU, COLOR_TASKS_SUSPENDED_GPU);

	m_bColorWindowUpdate= true;

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleTasks];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonUploadDownload.SetWindowText(gszTranslation[PosDialogColorUploadDownload]);
	m_buttonUploadDownloadGpu.SetWindowText(gszTranslation[PosDialogColorUploadDownloadGpu]);
	m_buttonReadyToReport.SetWindowText(gszTranslation[PosDialogColorReadyToReport]);
	m_buttonReadyToReportGpu.SetWindowText(gszTranslation[PosDialogColorReadyToReportGpu]);
	m_buttonRunning.SetWindowText(gszTranslation[PosDialogColorRunning]);
	m_buttonRunningGpu.SetWindowText(gszTranslation[PosDialogColorRunningGpu]);
	m_buttonHighPriority.SetWindowText(gszTranslation[PosDialogColorHighPriority]);
	m_buttonHighPriorityGpu.SetWindowText(gszTranslation[PosDialogColorHighPriorityGpu]);
	m_buttonAbort.SetWindowText(gszTranslation[PosDialogColorAbort]);
	m_buttonWaitingToRun.SetWindowText(gszTranslation[PosDialogColorWaitingToRun]);
	m_buttonWaitingToRunGpu.SetWindowText(gszTranslation[PosDialogColorWaitingToRunGpu]);
	m_buttonReadyToStart.SetWindowText(gszTranslation[PosDialogColorReadyToStart]);
	m_buttonReadyToStartGpu.SetWindowText(gszTranslation[PosDialogColorReadyToStartGpu]);
	m_buttonError.SetWindowText(gszTranslation[PosDialogColorError]);
	m_buttonErrorGpu.SetWindowText(gszTranslation[PosDialogColorErrorGpu]);
	m_buttonSuspended.SetWindowText(gszTranslation[PosDialogColorSuspended]);
	m_buttonSuspendedGpu.SetWindowText(gszTranslation[PosDialogColorSuspendedGpu]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
// Translation

	SetTimer(UTIMER_DLG_COLOR_TASKS, 1000, 0);	// interval 1 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

COLORREF *CDlgColorTasks::GetColorArray()
{
	return &m_colorTasks[0];
}

void CDlgColorTasks::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksUploadDownload , m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksUploadDownloadGPU , m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToReport , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToReportGPU , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksRunning , m_colorTasks[COLOR_TASKS_ARRAY_RUNNING]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksRunningGPU , m_colorTasks[COLOR_TASKS_ARRAY_RUNNING_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksHighPriority , m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksHighPriorityGpu , m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksWaitingToRun , m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksWaitingToRunGPU , m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToStart , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToStartGPU , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksAbort , m_colorTasks[COLOR_TASKS_ARRAY_ABORT]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksError , m_colorTasks[COLOR_TASKS_ARRAY_ERROR]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksErrorGPU , m_colorTasks[COLOR_TASKS_ARRAY_ERROR_GPU]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksSuspended , m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTasksSuspendedGPU , m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED_GPU]);
	m_bColorWindowUpdate= true;
}

void CDlgColorTasks::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_COLOR_ABORT, m_wndTasks[COLOR_TASKS_ARRAY_ABORT]);
	DDX_Control(pDX, IDC_WND_COLOR_UPDOWN, m_wndTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD]);
	DDX_Control(pDX, IDC_WND_COLOR_UPDOWN_GPU, m_wndTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_READY, m_wndTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT]);
	DDX_Control(pDX, IDC_WND_COLOR_READY_GPU, m_wndTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_RUNNING, m_wndTasks[COLOR_TASKS_ARRAY_RUNNING]);
	DDX_Control(pDX, IDC_WND_COLOR_RUNNING_CUDA, m_wndTasks[COLOR_TASKS_ARRAY_RUNNING_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_HIGHPRIORITY, m_wndTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY]);
	DDX_Control(pDX, IDC_WND_COLOR_HIGHPRIORITY_GPU, m_wndTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_WAITINGTORUN, m_wndTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN]);
	DDX_Control(pDX, IDC_WND_COLOR_WAITINGTORUN_CUDA, m_wndTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_READYTOSTART, m_wndTasks[COLOR_TASKS_ARRAY_READY_TO_START]);
	DDX_Control(pDX, IDC_WND_COLOR_READYTOSTART_GPU, m_wndTasks[COLOR_TASKS_ARRAY_READY_TO_START_GPU]);
	DDX_Control(pDX, IDC_WND_COLOR_ERROR, m_wndTasks[COLOR_TASKS_ARRAY_ERROR]);
	DDX_Control(pDX, IDC_WND_COLOR_ERROR_GPU, m_wndTasks[COLOR_TASKS_ARRAY_ERROR_GPU]);

	DDX_Control(pDX, IDC_WND_COLOR_SUSPENDED, m_wndTasks[COLOR_TASKS_ARRAY_SUSPENDED]);
	DDX_Control(pDX, IDC_WND_COLOR_SUSPENDED_GPU, m_wndTasks[COLOR_TASKS_ARRAY_SUSPENDED_GPU]);


	DDX_Control(pDX, IDC_BUTTON_COLOR_UPDOWN, m_buttonUploadDownload);
	DDX_Control(pDX, IDC_BUTTON_COLOR_UPDOWN_GPU, m_buttonUploadDownloadGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_READY, m_buttonReadyToReport);
	DDX_Control(pDX, IDC_BUTTON_COLOR_READY_GPU, m_buttonReadyToReportGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_RUNNING, m_buttonRunning);
	DDX_Control(pDX, IDC_BUTTON_COLOR_RUNNING_GPU, m_buttonRunningGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_HIGHPRIORITY, m_buttonHighPriority);
	DDX_Control(pDX, IDC_BUTTON_COLOR_HIGHPRIORITY_GPU, m_buttonHighPriorityGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_ABORT, m_buttonAbort);
	DDX_Control(pDX, IDC_BUTTON_COLOR_WAITINGTORUN, m_buttonWaitingToRun);
	DDX_Control(pDX, IDC_BUTTON_COLOR_WAITINGTORUN_CUDA, m_buttonWaitingToRunGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_READYTOSTART, m_buttonReadyToStart);
	DDX_Control(pDX, IDC_BUTTON_COLOR_READYTOSTART_GPU, m_buttonReadyToStartGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_ERROR, m_buttonError);
	DDX_Control(pDX, IDC_BUTTON_COLOR_ERROR_GPU, m_buttonErrorGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_SUSPENDED, m_buttonSuspended);
	DDX_Control(pDX, IDC_BUTTON_COLOR_SUSPENDED_GPU, m_buttonSuspendedGpu);
	DDX_Control(pDX, IDOK, m_buttonOk);
}

BEGIN_MESSAGE_MAP(CDlgColorTasks, CPropertyPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COLOR_UPDOWN, &CDlgColorTasks::OnBnClickedButtonColorUpdown)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_UPDOWN_GPU, &CDlgColorTasks::OnBnClickedButtonColorUpdownGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_READY, &CDlgColorTasks::OnBnClickedButtonColorReady)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_READY_GPU, &CDlgColorTasks::OnBnClickedButtonColorReadyGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_RUNNING, &CDlgColorTasks::OnBnClickedButtonColorRunning)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_RUNNING_GPU, &CDlgColorTasks::OnBnClickedButtonColorRunningGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_HIGHPRIORITY, &CDlgColorTasks::OnBnClickedButtonColorHighpriority)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_HIGHPRIORITY_GPU, &CDlgColorTasks::OnBnClickedButtonColorHighpriorityGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_WAITINGTORUN, &CDlgColorTasks::OnBnClickedButtonColorWaitingtorun)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_WAITINGTORUN_CUDA, &CDlgColorTasks::OnBnClickedButtonColorWaitingtorunCuda)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_READYTOSTART, &CDlgColorTasks::OnBnClickedButtonColorReadytostart)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_READYTOSTART_GPU, &CDlgColorTasks::OnBnClickedButtonColorReadytostartGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ERROR, &CDlgColorTasks::OnBnClickedButtonColorError)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ERROR_GPU, &CDlgColorTasks::OnBnClickedButtonColorErrorGpu)
//	ON_COMMAND(ID_COLORS_TASK, &CDlgColor::OnColorsTask)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ABORT, &CDlgColorTasks::OnBnClickedButtonColorAbort)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_SUSPENDED, &CDlgColorTasks::OnBnClickedButtonColorSuspended)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_SUSPENDED_GPU, &CDlgColorTasks::OnBnClickedButtonColorSuspendedGpu)
	ON_BN_CLICKED(IDOK, &CDlgColorTasks::OnBnClickedOk)

END_MESSAGE_MAP()

// CDlgColorTasks message handlers

void CDlgColorTasks::OnTimer(UINT_PTR nIDEvent)
{
	int iCount;
	if (m_bColorWindowUpdate)
	{
		m_bColorWindowUpdate = false;
		for (iCount = 0; iCount < COLOR_ARRAY_NUMBER_TASKS; iCount++)
		{
			m_wndTasks[iCount].SetColor(m_colorTasks[iCount]);
			m_wndTasks[iCount].Invalidate(TRUE);
		}
//		::PostMessage(m_hWnd, WM_GRAPHIC_COLOR_CHANGED, 0, 0);
	}
}

void CDlgColorTasks::OnBnClickedButtonColorUpdown()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksUploadDownload , m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorUpdownGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksUploadDownloadGPU , m_colorTasks[COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorReady()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToReport , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorReadyGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToReportGPU , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorRunning()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_RUNNING],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_RUNNING] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksRunning , m_colorTasks[COLOR_TASKS_ARRAY_RUNNING]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorRunningGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_RUNNING_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_RUNNING_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksRunningGPU , m_colorTasks[COLOR_TASKS_ARRAY_RUNNING_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorHighpriority()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksHighPriority , m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorHighpriorityGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksHighPriorityGpu , m_colorTasks[COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorWaitingtorun()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksWaitingToRun , m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorWaitingtorunCuda()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksWaitingToRunGPU , m_colorTasks[COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorReadytostart()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToStart , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorReadytostartGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksReadyToStartGPU , m_colorTasks[COLOR_TASKS_ARRAY_READY_TO_START_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorAbort()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_ABORT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_ABORT] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksAbort , m_colorTasks[COLOR_TASKS_ARRAY_ABORT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorError()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_ERROR],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_ERROR] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksError , m_colorTasks[COLOR_TASKS_ARRAY_ERROR]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorErrorGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_ERROR_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_ERROR_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksErrorGPU , m_colorTasks[COLOR_TASKS_ARRAY_ERROR_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorSuspended()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksSuspended , m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTasks::OnBnClickedButtonColorSuspendedGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED_GPU],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED_GPU] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTasksSuspendedGPU , m_colorTasks[COLOR_TASKS_ARRAY_SUSPENDED_GPU]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

COLORREF CDlgColorTasks::Correct(COLORREF color)
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
void CDlgColorTasks::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}



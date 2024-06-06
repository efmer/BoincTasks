#pragma once

#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"

#define COLOR_TASKS_UPLOAD_DOWNLOAD		RGB(255,255,255)
#define COLOR_TASKS_UPLOAD_DOWNLOAD_GPU	RGB(198,227,251)
#define COLOR_TASKS_READY_TO_REPORT		RGB(255,255,255)
#define COLOR_TASKS_READY_TO_REPORT_GPU	RGB(198,227,251)
#define COLOR_TASKS_RUNNING				RGB(120,252,159)
#define COLOR_TASKS_RUNNING_GPU			RGB(120,252,196)
#define COLOR_TASKS_HIGH_PRIORITY		RGB(255,128,30)
#define COLOR_TASKS_HIGH_PRIORITY_GPU	RGB(255,128,30)
#define COLOR_TASKS_WAITING_TO_RUN		RGB(255,255,255)
#define COLOR_TASKS_WAITING_TO_RUN_GPU	RGB(198,227,251)
#define COLOR_TASKS_READY_TO_START		RGB(255,255,255)
#define COLOR_TASKS_READY_TO_START_GPU	RGB(198,227,251)
#define COLOR_TASKS_ERROR				RGB(251,183,185)
#define COLOR_TASKS_ERROR_GPU			RGB(251,183,185)
#define COLOR_TASKS_ABORT				RGB(251,183,185)
#define COLOR_TASKS_SUSPENDED			RGB(255,255,255)
#define COLOR_TASKS_SUSPENDED_GPU		RGB(255,255,255)
#define COLOR_TASKS_ERROR_RATIO			RGB(251,183,185)

#define COLOR_TASKS_ARRAY_ABORT					0
#define COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD		1
#define COLOR_TASKS_ARRAY_UPLOAD_DOWNLOAD_GPU	2
#define COLOR_TASKS_ARRAY_READY_TO_REPORT		3
#define COLOR_TASKS_ARRAY_READY_TO_REPORT_GPU	4
#define COLOR_TASKS_ARRAY_RUNNING				5
#define COLOR_TASKS_ARRAY_RUNNING_GPU			6
#define COLOR_TASKS_ARRAY_HIGH_PRIORITY			7
#define COLOR_TASKS_ARRAY_HIGH_PRIORITY_GPU		8
#define COLOR_TASKS_ARRAY_WAITING_TO_RUN		9
#define COLOR_TASKS_ARRAY_WAITING_TO_RUN_GPU	10
#define COLOR_TASKS_ARRAY_READY_TO_START		11
#define COLOR_TASKS_ARRAY_READY_TO_START_GPU	12
#define COLOR_TASKS_ARRAY_ERROR					13
#define COLOR_TASKS_ARRAY_ERROR_GPU				14
#define COLOR_TASKS_ARRAY_SUSPENDED				15
#define COLOR_TASKS_ARRAY_SUSPENDED_GPU			16

#define COLOR_ARRAY_NUMBER_TASKS				17

// CDlgColorTasks dialog

class CDlgColorTasks : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorTasks)

public:
	CDlgColorTasks();
	virtual ~CDlgColorTasks();
	virtual BOOL OnInitDialog();
	COLORREF *GetColorArray();
	void	SaveRegistry();

// Dialog Data
	enum { IDD = IDD_DIALOG_COLOR_TASKS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	COLORREF	Correct(COLORREF color);
	bool		m_bColorWindowUpdate;
	CWndColor	m_wndTasks[COLOR_ARRAY_NUMBER_TASKS+1];
	COLORREF	m_colorTasks[COLOR_ARRAY_NUMBER_TASKS+1];

	DECLARE_MESSAGE_MAP()

    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonColorUpdown();
	afx_msg void OnBnClickedButtonColorUpdownGpu();
	afx_msg void OnBnClickedButtonColorReady();
	afx_msg void OnBnClickedButtonColorReadyGpu();
	afx_msg void OnBnClickedButtonColorRunning();
	afx_msg void OnBnClickedButtonColorRunningCuda();
	afx_msg void OnBnClickedButtonColorHighpriority();
	afx_msg void OnBnClickedButtonColorHighpriorityGpu();
	afx_msg void OnBnClickedButtonColorWaitingtorun();
	afx_msg void OnBnClickedButtonColorWaitingtorunCuda();
	afx_msg void OnBnClickedButtonColorReadytostart();
	afx_msg void OnBnClickedButtonColorReadytostartGpu();
	afx_msg void OnBnClickedButtonColorError();
	afx_msg void OnBnClickedButtonColorErrorGpu();
//	afx_msg void OnColorsTask();
	afx_msg void OnBnClickedButtonColorAbort();
	afx_msg void OnBnClickedButtonColorSuspended();
	afx_msg void OnBnClickedButtonColorSuspendedGpu();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonColorRunningGpu();
	CButton m_buttonUploadDownload;
	CButton m_buttonUploadDownloadGpu;
	CButton m_buttonReadyToReport;
	CButton m_buttonReadyToReportGpu;
	CButton m_buttonRunning;
	CButton m_buttonRunningGpu;
	CButton m_buttonHighPriority;
	CButton m_buttonHighPriorityGpu;
	CButton m_buttonAbort;
	CButton m_buttonWaitingToRun;
	CButton m_buttonWaitingToRunGpu;
	CButton m_buttonReadyToStart;
	CButton m_buttonReadyToStartGpu;
	CButton m_buttonError;
	CButton m_buttonErrorGpu;
	CButton m_buttonSuspended;
	CButton m_buttonSuspendedGpu;
	CButton m_buttonOk;
};

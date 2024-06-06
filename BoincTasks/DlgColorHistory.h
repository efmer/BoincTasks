#pragma once
#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"

#define COLOR_HISTORY_UPLOAD				RGB(255,255,255)
#define COLOR_HISTORY_UPLOAD_GPU			RGB(198,227,251)
#define COLOR_HISTORY_READY_TO_REPORT		RGB(255,255,255)
#define COLOR_HISTORY_READY_TO_REPORT_GPU	RGB(198,227,251)
#define COLOR_HISTORY_ABORT					RGB(251,183,185)
#define COLOR_HISTORY_ERROR					RGB(251,183,185)
#define COLOR_HISTORY_ERROR_GPU				RGB(251,183,185)
#define COLOR_HISTORY_REPORTED				RGB(120,252,159)
#define COLOR_HISTORY_REPORTED_GPU			RGB(120,252,196)
#define COLOR_HISTORY_REPORTED_ERROR		RGB(251,183,185)
#define COLOR_HISTORY_REPORTED_ERROR_GPU	RGB(251,183,185)
#define COLOR_HISTORY_MISSED				RGB(100,232,139)
#define COLOR_HISTORY_MISSED_GPU			RGB(100,232,139)

#define COLOR_HISTORY_ARRAY_UPLOAD				0
#define COLOR_HISTORY_ARRAY_UPLOAD_GPU			1
#define COLOR_HISTORY_ARRAY_READY_TO_REPORT		2
#define COLOR_HISTORY_ARRAY_READY_TO_REPORT_GPU	3
#define COLOR_HISTORY_ARRAY_ABORT				4
#define COLOR_HISTORY_ARRAY_ERROR				5
#define COLOR_HISTORY_ARRAY_ERROR_GPU			6
#define COLOR_HISTORY_ARRAY_REPORTED			7
#define COLOR_HISTORY_ARRAY_REPORTED_GPU		8
#define COLOR_HISTORY_ARRAY_REPORTED_ERROR		9
#define COLOR_HISTORY_ARRAY_REPORTED_ERROR_GPU	10
#define COLOR_HISTORY_ARRAY_MISSED				11
#define COLOR_HISTORY_ARRAY_MISSED_GPU			12

#define COLOR_ARRAY_NUMBER_HISTORY				13

// CDlgColorHistory dialog

class CDlgColorHistory : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorHistory)

public:
	CDlgColorHistory();
	virtual ~CDlgColorHistory();
	virtual BOOL OnInitDialog();
	COLORREF *GetColorArray();
	void	SaveRegistry();

// Dialog Data
	enum { IDD = IDD_DIALOG_COLOR_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	COLORREF	Correct(COLORREF color);
	bool		m_bColorWindowUpdate;
	CWndColor	m_wndHistory[COLOR_ARRAY_NUMBER_HISTORY+1];
	COLORREF	m_colorHistory[COLOR_ARRAY_NUMBER_HISTORY+1];

	DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonColorUpload();
	afx_msg void OnBnClickedButtonColorUploadGpu();
	afx_msg void OnBnClickedButtonColorReady();
	afx_msg void OnBnClickedButtonColorReadyGpu();
	afx_msg void OnBnClickedButtonColorAbort();
	afx_msg void OnBnClickedButtonColorError();
	afx_msg void OnBnClickedButtonColorErrorGpu();
	afx_msg void OnBnClickedButtonColorReported();
	afx_msg void OnBnClickedButtonColorReportedGpu();
	afx_msg void OnBnClickedButtonColorReportedError();
	afx_msg void OnBnClickedButtonColorReportedErrorGpu();
	afx_msg void OnBnClickedButtonColorMissed();


	afx_msg void OnBnClickedOk();
	CButton m_buttonUpload;
	CButton m_buttonUploadGpu;
	CButton m_buttonReadyToReport;
	CButton m_buttonReadyToReportGpu;
	CButton m_buttonAbort;
	CButton m_buttonError;
	CButton m_buttonErrorGpu;
	CButton m_buttonReported;
	CButton m_buttonReportedGpu;
	CButton m_buttonReportedError;
	CButton m_buttonReportedErrorGpu;
	CButton m_buttonOk;
	CButton m_buttonMissed;
	CButton m_buttonMissedGpu;
	afx_msg void OnBnClickedButtonColorMissedGpu();
};

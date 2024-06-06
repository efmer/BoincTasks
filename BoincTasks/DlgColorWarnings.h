#pragma once

#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"

#define COLOR_WARNINGS_ERROR_RATIO			RGB(248,118,121)
#define COLOR_WARNINGS_ERROR_RATIO_GPU		RGB(248,118,121)
#define COLOR_WARNINGS_DEADLINE				RGB(248,118,121)
#define COLOR_WARNINGS_TASKS				RGB(248,118,121)
#define COLOR_WARNINGS_CHECKPOINT			RGB(248,118,121)

#define COLOR_WARNINGS_ARRAY_ERROR_RATIO		0
#define COLOR_WARNINGS_ARRAY_ERROR_RATIO_GPU	1
#define COLOR_WARNINGS_ARRAY_DEADLINE			2
#define COLOR_WARNINGS_ARRAY_DEADLINE_RUNNING	3
#define COLOR_WARNINGS_ARRAY_TASKS				4
#define COLOR_WARNINGS_ARRAY_CHECKPOINT			5

#define COLOR_ARRAY_NUMBER_WARNINGS				6

// CDlgColorWarnings dialog

class CDlgColorWarnings : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorWarnings)

public:
	CDlgColorWarnings();
	virtual ~CDlgColorWarnings();
	virtual BOOL OnInitDialog();
	COLORREF *GetColorArray();
	void	SaveRegistry();

// Dialog Data
	enum { IDD = IDD_DIALOG_COLOR_WARNINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	COLORREF	Correct(COLORREF color);
	bool		m_bColorWindowUpdate;
	CWndColor	m_wndWarnings[COLOR_ARRAY_NUMBER_WARNINGS+1];
	COLORREF	m_colorWarnings[COLOR_ARRAY_NUMBER_WARNINGS+1];

	DECLARE_MESSAGE_MAP()

    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonColorErrorRatio();
public:
	afx_msg void OnBnClickedButtonColorErrorRatioGpu();
	afx_msg void OnBnClickedButtonColorDeadlineWarning();
	afx_msg void OnBnClickedOk();
	CButton m_buttonErrorCpuRatio;
	CButton m_buttonErrorGpuRatio;
	CButton m_buttonDeadlineWarning;
	CButton m_buttonTaskWarning;
	CButton m_buttonCheckpointWarning;

	CButton m_buttonOk;
	afx_msg void OnBnClickedButtonColorDeadlineWarningRunning();
	CButton m_buttonDeadlineWarningRunning;
	afx_msg void OnBnClickedButtonColorCountWarning();
	afx_msg void OnBnClickedButtonColorCheckpointWarning();
};

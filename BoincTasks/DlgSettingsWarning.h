#pragma once
#include "afxwin.h"

// CDlgSettingsWarning dialog

class CDlgSettingsWarning : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsWarning)

public:
	CDlgSettingsWarning();
	virtual ~CDlgSettingsWarning();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

//	int	m_iEfficiencyCpuBelow;
//	int	m_iEfficiencyGpuAbove;

	double m_fWarningCpuLow1, m_fWarningCpuHigh1;
	double m_fWarningCpuLow2, m_fWarningCpuHigh2;
	double m_fWarningCpuLow3, m_fWarningCpuHigh3;
	double m_fWarningCpuLow4, m_fWarningCpuHigh4;

	double m_fWarningGpuLow1, m_fWarningGpuHigh1;
	double m_fWarningGpuLow2, m_fWarningGpuHigh2;
	double m_fWarningGpuLow3, m_fWarningGpuHigh3;
	double m_fWarningGpuLow4, m_fWarningGpuHigh4;

	double m_fWarningPercLow1, m_fWarningPercHigh1;
	double m_fWarningPercLow2, m_fWarningPercHigh2;
	double m_fWarningPercLow3, m_fWarningPercHigh3;
	double m_fWarningPercLow4, m_fWarningPercHigh4;

	int		m_iWarningCpuTasks1;
	int		m_iWarningCpuTasks2;
	int		m_iWarningCpuTasks3;
	int		m_iWarningCpuTasks4;

	int		m_iWarningGpuTasks1;
	int		m_iWarningGpuTasks2;
	int		m_iWarningGpuTasks3;
	int		m_iWarningGpuTasks4;

	CString m_sProject1;
	CString m_sProject2;
	CString m_sProject3;
	CString m_sProject4;

	CString m_sComputer1;
	CString m_sComputer2;
	CString m_sComputer3;
	CString m_sComputer4;

	int		m_iDeadlineTime;

	bool	m_bCheckpoint;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_ALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CBitmap m_bmp;


	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_deadlineDays;
	CComboBox m_deadlineHours;
//	CEdit m_efficiencyCpuBelow;
//	CEdit m_efficiencyGpuAbove;
	afx_msg void OnBnClickedOk();
//	afx_msg void OnEnChangeEditWarningCpuPerc();
//	afx_msg void OnEnChangeEditWarningGpuPerc();
	afx_msg void OnCbnSelchangeComboDeadlineDay();
	afx_msg void OnCbnSelchangeComboDeadlineHour();
	afx_msg void OnEnChangeEditWarningGpuHigh1();
	CEdit m_cpuLow1;
	CEdit m_cpuLow2;
	CEdit m_cpuLow4;
	CEdit m_cpuHigh1;
	CEdit m_cpuHigh2;
	CEdit m_cpuHigh3;
	CEdit m_cpuHigh4;
	CEdit m_cpuLow3;
	CEdit m_gpuLow1;
	CEdit m_gpuLow2;
	CEdit m_gpuLow3;
	CEdit m_gpuLow4;
	CEdit m_gpuHigh1;
	CEdit m_gpuHigh2;
	CEdit m_gpuHigh3;
	CEdit m_gpuHigh4;
	CEdit m_percLow1;
	CEdit m_percLow2;
	CEdit m_percLow3;
	CEdit m_percLow4;
	CEdit m_percHigh1;
	CEdit m_percHigh2;
	CEdit m_percHigh3;
	CEdit m_percHigh4;
	afx_msg void OnEnChangeEditWarningCpuLow1();
	afx_msg void OnEnChangeEditWarningCpuLow2();
	afx_msg void OnEnChangeEditWarningCpuLow3();
	afx_msg void OnEnChangeEditWarningCpuLow4();
	afx_msg void OnEnChangeEditWarningCpuHigh1();
	afx_msg void OnEnChangeEditWarningCpuHigh2();
	afx_msg void OnEnChangeEditWarningCpuHigh3();
	afx_msg void OnEnChangeEditWarningCpuHigh4();
	afx_msg void OnEnChangeEditWarningGpuLow1();
	afx_msg void OnEnChangeEditWarningGpuLow2();
	afx_msg void OnEnChangeEditWarningGpuLow3();
	afx_msg void OnEnChangeEditWarningGpuLow4();
	afx_msg void OnEnChangeEditWarningGpuHigh2();
	afx_msg void OnEnChangeEditWarningGpuHigh3();
	afx_msg void OnEnChangeEditWarningGpuHigh4();
	afx_msg void OnEnChangeEditWarningUseLow1();
	afx_msg void OnEnChangeEditWarningUseLow2();
	afx_msg void OnEnChangeEditWarningUseLow3();
	afx_msg void OnEnChangeEditWarningUseLow4();
	afx_msg void OnEnChangeEditWarningUseHigh1();
	afx_msg void OnEnChangeEditWarningUseHigh2();
	afx_msg void OnEnChangeEditWarningUseHigh3();
	afx_msg void OnEnChangeEditWarningUseHigh4();
	CButton m_buttonOk;
	CButton m_checkCheckpoint;
	CStatic m_textWarning;
	CStatic m_textDaysAnd;
	CStatic m_textHours;
	CStatic m_textWarningIsActive;
//	CStatic m_textCpuBetween1;
//	CStatic m_textCpuBetween2;
//	CStatic m_textCpuBetween3;
//	CStatic m_textCpuBetween4;
//	CStatic m_textGpuBetween1;
//	CStatic m_textGpuBetween2;
//	CStatic m_textGpuBetween3;
//	CStatic m_textGpuBetween4;
//	CStatic m_textCpuPBetween1;
//	CStatic m_textCpuPBetween2;
//	CStatic m_textCpuPBetween3;
//	CStatic m_textCpuPBetween4;
	CEdit m_cpuTasks1;
	CEdit m_cpuTasks2;
	CEdit m_cpuTasks3;
	CEdit m_cpuTasks4;
//	CStatic m_textCpuTasks1;
	CEdit m_gpuTasks1;
	CEdit m_gpuTasks2;
	CEdit m_gpuTasks3;
	CEdit m_gpuTasks4;
	CEdit m_project1;
	CEdit m_project2;
	CEdit m_project3;
	CEdit m_project4;
	CEdit m_computer1;
	CEdit m_computer2;
	CEdit m_computer3;
	CEdit m_computer4;
	CStatic m_groupGpuTasks;
	CStatic m_groupCpuTasks;
	CStatic m_groupProject;
	CStatic m_groupComputer;
	CStatic m_groupCpuBetween;
	CStatic m_groupGpuBetween;
	CStatic m_groupCpuPercentageBetween;
	afx_msg void OnEnChangeEditWarningCpuTasks1();
	afx_msg void OnEnChangeEditWarningCpuTasks2();
	afx_msg void OnEnChangeEditWarningCpuTasks3();
	afx_msg void OnEnChangeEditWarningCpuTasks4();
	afx_msg void OnEnChangeEditWarningGpuTasks1();
	afx_msg void OnEnChangeEditWarningGpuTasks2();
	afx_msg void OnEnChangeEditWarningGpuTasks3();
	afx_msg void OnEnChangeEditWarningGpuTasks4();
	afx_msg void OnEnChangeEditWarningProject1();
	afx_msg void OnEnChangeEditWarningProject2();
	afx_msg void OnEnChangeEditWarningProject3();
	afx_msg void OnEnChangeEditWarningProject4();
	afx_msg void OnEnChangeEditWarningCpuComputer1();
	afx_msg void OnEnChangeEditWarningCpuComputer2();
	afx_msg void OnEnChangeEditWarningCpuComputer3();
	afx_msg void OnEnChangeEditWarningCpuComputer4();

	afx_msg void OnBnClickedCheckWarningCheckpoint();
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
};

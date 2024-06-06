#pragma once
#include "afxwin.h"
#include "TimeString.h"
#include "WndScheduler.h"

// CDlgPrefProcessor dialog

class CDlgPrefMain;
class CRpcPreferenceInfo;
class CDlgPrefProcessor : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgPrefProcessor)

public:
	CDlgPrefProcessor();
	virtual ~CDlgPrefProcessor();
	virtual BOOL OnInitDialog();
			void FillDialog(CRpcPreferenceInfo *pPreferenceInfo);
			bool ReadDialog(CRpcPreferenceInfo *pPreferenceInfo);
			void ShowGraphic(bool bEnable);
// Dialog Data
	enum { IDD = IDD_DIALOG_PREFERENCE_PROCESSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void ShowHideDay(int iDay);

	CTimeString	timeStringDouble;
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnSetGraphic(WPARAM parm1, LPARAM parm2);
public:
	CDlgPrefMain *m_pDlgPrefMain;

	CButton CheckComputingOnBatteries;
	CButton CheckComputingInUse;
	CButton CheckComputingInUseGpu;
	CEdit ValueComputingIdleMin;
	CEdit ValueOtherSwitchEveryMin;
	CEdit ValueOtherMultiprocPerc;
	CEdit ValueOtherUseAtMost;
	CEdit ValueWhileProcessorUsageIsLessThan;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CButton m_checkDay[8];
	CEdit m_editDayOfWeek[8][2];

	CEdit m_editTimeFrom;
	CEdit m_editTimeTo;
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedCheckComputingMonday();
	afx_msg void OnBnClickedCheckComputingTuesday();
	afx_msg void OnBnClickedCheckComputingWednesday();
	afx_msg void OnBnClickedCheckComputingThursday();
	afx_msg void OnBnClickedCheckComputingFriday();
	afx_msg void OnBnClickedCheckComputingSaturday();
	afx_msg void OnBnClickedCheckComputingSunday();
	CButton m_buttonClear;
	CButton m_buttonOk;
	CButton m_buttonCancel;
	CStatic m_textComputingAllowed;
	CStatic m_textOnlyAfterComputerBeenIdleFor;
	CStatic m_textMinutes1;
	CStatic m_textMinutes2;
	CStatic m_textEverydayBetweenTheHoursOf;
	CStatic m_textAnd;
	CStatic m_textNoRestrictionsIfEqual;
	CStatic m_textDayOfWeekOverride;
	CStatic m_textHourMinutes;
	CStatic m_textOtherOptions;
	CStatic m_textSwitchBetweenApplicationsEvery;
	CStatic m_textOnMultiprocessorSystemsUseAtMost;
	CStatic m_textPercOfTheProcessors;
	CStatic m_textUseAtMost;
	CStatic m_textPercCpuTime;
	CStatic m_textWhileProcessorUsageIsLessThan;
	CStatic m_textPerc0MeansNoRestrictions;

	afx_msg void OnBnClickedCheckComputingInUseGpu();
	afx_msg void OnBnClickedAdvancedScheduler();
	CWndScheduler m_window;
	CButton m_checkGraphic;
	afx_msg void OnBnClickedCheckGraphical();
	CStatic m_divider1;
	CStatic m_divider2;
	CStatic m_divider3;
	CStatic m_divider4;
	CStatic m_divider5;
	CStatic m_divider6;
	CStatic m_divider7;
	afx_msg void OnStnClickedWndProcessorScheduler();
};

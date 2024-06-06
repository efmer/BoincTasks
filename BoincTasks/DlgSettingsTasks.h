#pragma once

#include "TemplateRemoteTaskView.h"
#include "afxwin.h"


#define STATUS_MIN (PosViewTasksStatusComputation)


// CDlgSettingsTasks dialog

class CDlgSettingsTasks : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsTasks)

public:
	CDlgSettingsTasks();
	virtual ~CDlgSettingsTasks();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	void	SetUserFriendly();

	bool	m_bUseCondense;
	bool	m_bRatioLongTerm;
	bool	m_bUserFriendly;
	bool	m_bDeadlineShowRemaining;
	bool	m_bSortOnApplicationNr;

	int		m_iMaxUpdateTime;

	int		m_iCpuDigits;
	int		m_iProgressDigits;

	bool	m_bCheckpoint;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_TASKS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void	DefaultStatusList();
	void	FillStatusList();
	void	SortStatusList();
	void	OffsetStatusList();
	void	ReadStatusList();
	int		GetOffsetText(CString *psTxt);
	bool	ValidStatusList();
	void	WriteStatusListRegistry();
	bool	ReadStatusListRegistry();
	void	SendStatusList();

	void	CheckIntegrity();
	void	ReadColumsFromRegistry();
	void	WriteColumsToRegistry();
	void	TaskColumns();
	void	EnableCheck();
	void	SetColumn(int iColumn, int iCheck);
	void	SetColumnHide(int iColumn, int iCheck);

	CButton		m_apply;

	int			m_iActiveColumns;
	int			m_iTasksColumnIn[NUM_REMOTE_TASK_COLUMNS+1];
	int			m_iTasksColumn[NUM_REMOTE_TASK_COLUMNS+1];
	int			m_iTasksColumnLookup[NUM_REMOTE_TASK_COLUMNS+1];

	std::deque<int>	m_lListStatusSorting;

	CBitmap m_bmp;

	afx_msg		LRESULT OnSettingsTasksGet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsTasksSet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsTasksWidth(WPARAM parm1, LPARAM parm2);

	afx_msg		LRESULT OnSettingsTasksSortingStatus(WPARAM parm1, LPARAM parm2);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();

	afx_msg void OnBnClickedOk();
	CButton m_useCondense;
	afx_msg void OnBnClickedCheckUseCondense();
	CButton m_buttonOk;
	CButton m_buttonDefault;
	CStatic m_textSequence;
	CButton m_checkRatioLongTerm;
	CButton m_checkDeadlineShowRemaining;
	CButton m_checkUserFriendly;
	CStatic m_textStatusSorting;
	CButton m_buttonDefaultSorting;

	afx_msg void OnBnClickedCheckRatioLongTerm();
	afx_msg void OnBnClickedCheckUserFriendly();

	CButton m_checkShow[NUM_REMOTE_TASK_COLUMNS+1];
	afx_msg void OnBnClickedCheckTasksColumn0();
	afx_msg void OnBnClickedCheckTasksColumn1();
	afx_msg void OnBnClickedCheckTasksColumn2();
	afx_msg void OnBnClickedCheckTasksColumn3();
	afx_msg void OnBnClickedCheckTasksColumn4();
	afx_msg void OnBnClickedCheckTasksColumn5();
	afx_msg void OnBnClickedCheckTasksColumn6();
	afx_msg void OnBnClickedCheckTasksColumn7();
	afx_msg void OnBnClickedCheckTasksColumn8();
	afx_msg void OnBnClickedCheckTasksColumn9();
	afx_msg void OnBnClickedCheckTasksColumn10();
	afx_msg void OnBnClickedCheckTasksColumn11();
	afx_msg void OnBnClickedCheckTasksColumn12();
	afx_msg void OnBnClickedCheckTasksColumn13();
	afx_msg void OnBnClickedCheckTasksColumn14();
	afx_msg void OnBnClickedCheckTasksColumn15();

	afx_msg void OnBnClickedCheckDeadlineRemaining();

	CListBox m_listTasksStatusSorting;
	CButton m_buttonTasksUp;
	CButton m_buttonTasksDown;
	afx_msg void OnBnClickedButtonTasksUp();
	afx_msg void OnLbnSelchangeListTasksStatusSorting();

	afx_msg void OnBnClickedButtonTasksDown();

	afx_msg void OnBnClickedTasksDefaultSorting();
	CStatic m_textMaxUpdate;
	CEdit m_editMaxUpdate;
	CStatic m_textSeconds;
	afx_msg void OnEnChangeEditTasksMaxUpdate();
	CStatic m_textCpuDigits;
	CStatic m_textProgressDigits;
	CEdit m_editCpuDigits;
	CEdit m_editProgressDigits;
	afx_msg void OnEnChangeEditTasksDigitsCpu();
	afx_msg void OnEnChangeEditTasksDigitsProgress();
	CButton m_buttonHelp;
	afx_msg void OnBnClickedHelp();
	afx_msg void OnBnClickedCheckTasksColumn16();
	afx_msg void OnBnClickedCheckTasksColumn17();
	afx_msg void OnBnClickedCheckTasksColumn18();
	CButton m_checkSortOnNr;
	afx_msg void OnBnClickedCheckSortApplicationNr();
	afx_msg void OnBnClickedCheckTasksColumn19();
};

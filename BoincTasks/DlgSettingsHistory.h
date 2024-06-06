#pragma once
#include "afxwin.h"

// CDlgSettingsHistory dialog

class CDlgSettingsHistory : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsHistory)

public:
	CDlgSettingsHistory();
	virtual ~CDlgSettingsHistory();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	void	SetColumnDefault();
	void	ReadColumsFromRegistry();
	void	WriteColumsToRegistry();

	void	HistoryColumns();
	void	EnableCheck();

	bool	m_bHistoryEnabled;
	bool	m_bHistorySmartMode;
	bool	m_bHistoryLessAccurate;
	int		m_iHistoryTime;
	int		m_iHistorySettingDays;
	int		m_iHistorySettingHours;

	bool	m_bHistoryForceUpdate;
	int		m_iHistoryMaxUpdateTime;
	int		m_iHistoryMinUpdateTime;

	bool	m_bMoveToLongHistory;
	int		m_iHistoryLongSettingDays;
	int		m_iHistoryLongSettingHours;
	int		m_iHistoryLongTime;

	bool	m_bBackupHistory;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg		LRESULT OnSettingsHistoryGet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsHistorySet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsHistoryWidth(WPARAM parm1, LPARAM parm2);

	CButton		m_apply;

	int			m_iActiveColumns;
	int			m_iHistoryColumnIn[NUM_REMOTE_HISTORY_COLUMNS+1];
	int			m_iHistoryColumn[NUM_REMOTE_HISTORY_COLUMNS+1];
	int			m_iHistoryColumnLookup[NUM_REMOTE_HISTORY_COLUMNS+1];

	void	GetHistoryTime();
	void	GetHistoryLongTime();
	void	LogHistory();
	void	SetColumn(int iColumn, int iCheck);
	void	SetColumnHide(int iColumn, int iCheck);

	CButton m_enableHistoryLogging;
	CButton m_enableHistorySmartMode;

	CBitmap m_bmp;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();

	CButton m_checkShow[NUM_REMOTE_HISTORY_COLUMNS+1];
	afx_msg void OnBnClickedCheckHistoryColumn0();
	afx_msg void OnBnClickedCheckHistoryColumn1();
	afx_msg void OnBnClickedCheckHistoryColumn2();
	afx_msg void OnBnClickedCheckHistoryColumn3();
	afx_msg void OnBnClickedCheckHistoryColumn4();
	afx_msg void OnBnClickedCheckHistoryColumn5();
	afx_msg void OnBnClickedCheckHistoryColumn6();
	afx_msg void OnBnClickedCheckHistoryColumn7();
	afx_msg void OnBnClickedCheckHistoryColumn8();
	afx_msg void OnBnClickedCheckHistoryColumn9();
	afx_msg void OnBnClickedCheckHistoryColumn10();
	afx_msg void OnBnClickedCheckHistoryColumn11();

	afx_msg void OnBnClickedCheckHistoryLogging();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboHistoryDeleteDay();
	CComboBox m_historyDays;
	CButton m_buttonOk;
	CStatic m_textRemoveAfter;
	CStatic m_textRemoveDays;
	CComboBox m_historyHours;
	CStatic m_textRemoveHours;
	CStatic m_textMaxUpdateTime;
	CStatic m_textMaxUpdateTimeSeconds;
	CEdit m_editMaxUpdateTime;
	afx_msg void OnCbnSelchangeComboHistoryDeleteHour();

	afx_msg void OnBnClickedCheckHistorySmart();

	afx_msg void OnEnChangeEditHistoryMaxUpdate();

	CStatic m_textMinUpdateTime;
	CEdit m_editMinUpdateTime;
	CStatic m_textMinUpdateTimeSeconds;
	afx_msg void OnEnChangeEditHistoryMinUpdate();
	CButton m_checkLong;
	afx_msg void OnBnClickedCheckHistoryLong();
	CComboBox m_historyLongHours;
	CStatic m_textMoveHours;
	afx_msg void OnCbnSelchangeComboHistoryLongHour();
	CComboBox m_historyLongDays;
	afx_msg void OnCbnSelchangeComboHistoryLongDay();
	CStatic m_textMoveDays;
	CButton m_checkAccurate;
	afx_msg void OnBnClickedCheckHistoryNotAccurate();
	afx_msg void OnBnClickedCheckHistoryBackup();
	CButton m_checkBackup;
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
	CStatic m_textSequence;
};

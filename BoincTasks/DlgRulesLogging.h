#pragma once
#include "afxwin.h"

#define LOGGING_RULES_MAX_BUFFER  20000

// DlgRulesLogging dialog

class CDlgRulesLogging : public CDialog
{
	DECLARE_DYNAMIC(CDlgRulesLogging)

public:
	CDlgRulesLogging(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRulesLogging();
	void	SafeWindowPlacement();
	void	Translate();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOG_RULES };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void AddLoggingDebug(CString sFrom, CString sLog, bool bTime);
	void AddLogging(CString sFrom, CString sLog, bool bTime = true, bool bFileOnly = false);
	void ShowLoggingText();
	void AddLoggingToFile(CString *psTxt);
	void AddLoggingInfo();

	CString GetLogName();
	void	CheckDayChanged();
	void	OpenLog();

	CString *m_psLogging;
	bool	m_bDebugMode;

	CString m_sLogFileName;
	CFile	m_file;
	bool	m_bFileOpen;

	HICON	m_hIcon;

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnLoggingText(WPARAM parm1, LPARAM parm2);
	afx_msg void OnBnClickedClear();
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
public:
	CButton m_buttonClear;
	CButton m_buttonOk;
	CButton m_buttonCancel;
	CEdit m_logging;
	afx_msg void OnBnClickedCheckDebug();
	CButton m_checkEnableDebug;
};

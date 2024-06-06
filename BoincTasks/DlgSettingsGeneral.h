#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Label.h"

#define MAX_LANGUAGES	20

#define OS_SELECTED_WINDOWS	0
#define OS_SELECTED_LINUX	1
#define OS_SELECTED_MAC		2

// CDlgSettingsGeneral dialog

class CThreadBoincClient;
class CDlgSettingsGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsGeneral)

public:
	CDlgSettingsGeneral();
	virtual ~CDlgSettingsGeneral();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	bool	m_bConnecTthrottle;
	bool	m_bHideAtStartup;
	CString m_sPassword;
	int		m_iOs;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_GENERAL };

protected:
	bool StartupProgram(bool startup);
	bool SetKey(HKEY key, CString sKeyPath, CString sKeyName, CString sKeyValue, bool bSetKey);
	bool CreateKey(HKEY key, CString sSubKey);
	bool ReadKey(HKEY key, CString sKeyPath, CString sKeyName, CString *psKeyValue);
	bool ReadKey(HKEY key, CString sKeyPath, CString sKeyName, DWORD *pdKeyValue);

	void CheckHotKeysChanged();
	CStringA GetKey();
	BOOL RegisterHotkey(char *charHotkeyId, char *charRegister, CHotKeyCtrl *memberHotKey);

//	CThreadBoincClient *m_pThreadBoincClient;
	int		m_iStartBoincClientAtLoginDelay;

	CButton m_startAtLogin;
	CButton m_startBoincClientAtLogin;
	CButton m_connectTthrottle;
	CButton m_checkHideAtStartup;

	char	m_cPasswordChar;

	int	m_iLanguageCodeList[MAX_LANGUAGES+1];
	int m_iLanguageCodeListCount;

	int	m_hotKeyBoincTasksMainIDNow;

	bool	m_bStartBoincClientAtLogin;
	bool	m_bStartBoincClientAtLoginRunning;
	bool	m_bStartBoincClientMessageShown;
	int		m_iCheckBoincManager;

	CBitmap m_bmp;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnBnClickedCheckStartAtLogin();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckStartBoincAtLogin();

	afx_msg void OnBnClickedCheckTthrottle();
	CComboBox m_comboLanguage;
	afx_msg void OnCbnSelchangeComboLanguage();
	CButton m_buttonOk;
	CStatic m_textLanguage;
	CHotKeyCtrl m_hotkeyShowAndHideBoincTasks;
	CStatic m_tekstHotKeyShowHide;

	afx_msg void OnBnClickedCheckHideAtStartup();
	CStatic m_textOs;
	CComboBox m_comboOs;
	afx_msg void OnCbnSelchangeComboGeneralOs();


	CButton m_stopBoincClientOnExit;
	afx_msg void OnBnClickedCheckStopBoinc();
	afx_msg void OnBnClickedCheckGeneralShow();
	afx_msg void OnEnChangeEditGeneralPasswod();
//	afx_msg void OnHelpManual();
	CStatic m_textPassword;
	CEdit m_editPassword;
	CButton m_checkShow;
	CLabel m_textManager;
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
	CStatic m_textStartDelay;
	CEdit m_editStartDelay;
	afx_msg void OnEnChangeEditGeneralStartDelay();
	CStatic m_textStartDelaySeconds;
};

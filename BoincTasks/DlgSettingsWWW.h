#pragma once
#include "afxwin.h"


// CDlgSettingsWWW dialog

class CDlgSettingsWWW : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsWWW)

public:
	CDlgSettingsWWW();
	virtual ~CDlgSettingsWWW();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	int		m_iRefreshTime;
	bool	m_bWWWEnable;

	bool	m_bCloudEnable;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_WWW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void ShowRefresh(int iCheck);
	void ShowCloud(int iCheck);
	CStringA GetKey();
	void ValidEmailPassword();
	void StartStopThread();
	void StopThread();
	void SendEmailPassword();

	CString m_sServer;
	CString	m_sCloudEmail;
	CString	m_sCloudPassword;
	bool	m_bValidEmailPassword;
	bool	m_bValidEmailPasswordSend;
	bool	m_bInitRunning;

	CBitmap m_bmp;

	DECLARE_MESSAGE_MAP()
public:
	CButton m_checkEnableWWW;
	CStatic m_staticRefresh;
	CEdit m_editRefresh;
	CStatic m_staticSeconds;
	CButton m_buttonOk;
	afx_msg void OnBnClickedCheckWwwEnable();
	afx_msg void OnEnChangeEditWwwRefresh();
	afx_msg void OnBnClickedOk();

	afx_msg LRESULT OnStartCloud(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadCloudTimeout(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadCloudQuit(WPARAM wParam, LPARAM lParam);



	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
//	afx_msg void OnBnClickedButtonCloudFolder();
	CButton m_checkEnableCloud;
	afx_msg void OnBnClickedCheckCloudEnable();
//	CEdit m_editCloudFolder;
//	CButton m_buttonCloudFolder;
	afx_msg void OnEnChangeEditWwwEmail();
	afx_msg void OnEnChangeEditWwwPassword();
	afx_msg void OnBnClickedCheckWwwShow();
	CEdit m_editPassword;
	CEdit m_editEmail;
	CEdit m_editStatus;
	CStatic m_staticEmail;
	CStatic m_staticPassword;
	CStatic m_textGroupFtp;
	CStatic m_staticServer;
	CStatic m_staticStatus;
	CEdit m_editServer;
	CButton m_buttonDefault;
	afx_msg void OnEnChangeEditWwwServer();
	CButton m_checkUseLocalhost;
	afx_msg void OnBnClickedWwwDefaultServer();
	afx_msg void OnBnClickedCheckWwwDebug();
	CStatic m_imageLock;
	CStatic m_static_https;
};

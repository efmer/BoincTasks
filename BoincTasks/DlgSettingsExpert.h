#pragma once
#include "afxwin.h"

#define COMPUTER_DUMMY_SET_TIME	"__set_time__"


// CDlgSettingsExpert dialog

class CDlgSettingsExpert : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsExpert)

public:
	CDlgSettingsExpert();
	virtual ~CDlgSettingsExpert();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	CString m_sPath;
//	int		m_iQuickAndDirty;
	bool	m_bUseEncryption;
	bool	m_bUseEncryptionChanged;
	int		m_iThreadTimeout;
	DWORD	m_dwPriority;

	bool	m_bCpidManual;
	std::string	m_sCpid;
	std::string	m_sCpid2;


	bool	m_bEnableThreadGraph;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_EXPERT };

protected:
	void SetReconnect(int iTime);
	void SendCPID();
	void ShowCPID(int iCheck);
	int	m_iReconnectTime;
	CEdit m_xmlPath;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CBitmap m_bmp;

//	bool	m_bCPIDChanged;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnGiveReconnectTime(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckXmlPath();
	CButton m_checkPath;
//	afx_msg void OnBnClickedCheckQuickAndDirty();
//	CButton m_quick;
	CButton m_buttonOk;
	CButton m_checkEncryption;
	afx_msg void OnBnClickedCheckEncryption();
	afx_msg void OnEnChangeEditReconnectTime();
	afx_msg void OnEnChangeEditThreadTimeout();
	CEdit m_editReconnect;
	CStatic m_textReconnect;
	CEdit m_editThreadTimeout;
	CStatic m_textThreadTimeout;

	CButton m_checkSilent;
	afx_msg void OnBnClickedCheckSilentReboot();
	afx_msg void OnCbnSelchangeComboPriority();
	CComboBox m_comboPriority;
	CStatic m_textPriority;
	CButton m_checkEnableThreadGraph;
	afx_msg void OnBnClickedCheckThreadTimes();
	CButton m_buttonThreadGraph;
	afx_msg void OnBnClickedButtonThreadGraphic();
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
	CButton m_checkCPID;
	CStatic m_textCPID;
	CEdit m_editCPID;
	CEdit m_editCPID2;
	afx_msg void OnEnChangeEditCpid();
	afx_msg void OnEnChangeEditCpid2();
	afx_msg void OnBnClickedCheckCpidManual();
};

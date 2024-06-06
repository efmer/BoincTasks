#pragma once
#include "afxwin.h"

#define MOBILE_FONT_HEADER	0
#define MOBILE_FONT_TEXT	1

// CDlgSettingsMobile dialog

class CDlgSettingsMobile : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsMobile)

public:
	CDlgSettingsMobile();
	virtual ~CDlgSettingsMobile();
	virtual BOOL OnInitDialog();
	BOOL OnHelpInfo( HELPINFO* HelpInfo);
	void GiveWarningDeactivate();

	CStatic m_textPassword;
	CEdit m_editPassword;
	CButton m_checkShow;

	bool	m_bMobileActive;
	bool	m_bMobileAutoDeactivate;
	UINT	m_iWebServerPort;

	bool	m_bMobileAlternativeSocket;

	CString m_sPassword;

	int		m_iHeaderFont;
	int		m_iFont;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_MOBILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CStringA GetKey();

	CButton m_checkMobileActive;
	CEdit	m_editMobilePort;

	CButton m_buttonOk;
	CButton m_buttonHelp;

	CBitmap m_bmp;

	char	m_cPasswordChar;

	afx_msg void OnBnClickedCheckWebserver();
	afx_msg void OnEnChangeEditWebserver();
	afx_msg void OnBnClickedCheckWwwShow();
	afx_msg void OnEnChangeEditWwwPassword();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHelp();

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_textHeaderFont;
	CStatic m_textFont;
	CComboBox m_comboHeaderFont;
	CComboBox m_comboFont;
	afx_msg void OnCbnSelchangeComboMobileHeaderFont();
	afx_msg void OnCbnSelchangeComboMobileFont();

	afx_msg LRESULT OnMobile(WPARAM wParam,LPARAM lParam);
};

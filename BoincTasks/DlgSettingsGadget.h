#pragma once
#include "afxwin.h"

#define RADIO_SHOW_ALWAYS_ON_TOP	0
#define RADIO_SHOW_ALWAYS			1
#define RADIO_SHOW_ICON				2


// CDlgSettingsGadget dialog

class CDlgSettingsGadget : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsGadget)

public:
	CDlgSettingsGadget();
	virtual ~CDlgSettingsGadget();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	int		m_iTime;
	int		m_iTimeAfter;
	int		m_iRefreshTime;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_GADGET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		void SendGadget();

	int		m_iRadio;
	int		m_iMode;

	CBitmap m_bmp;

	DECLARE_MESSAGE_MAP()
public:

	CButton m_radioShowAlwaysOnTop;
	CButton m_radioShowAlways;
	CButton m_radioShowOverIcon;


	CButton m_alwaysOnTop;
	CButton m_buttonOk;
	CStatic m_textSeconds1;
	CStatic m_textSeconds2;

	CEdit m_editTime;
	afx_msg void OnEnChangeEditIconTime();
	afx_msg void OnBnClickedRadioGadgetOnTop();
	afx_msg void OnBnClickedRadioGadgetShowAlways();
	afx_msg void OnBnClickedRadioGadgetOnIcon();
	CEdit	m_editRefreshEvery;
	afx_msg void OnEnChangeEditRefreshTime();
	afx_msg void OnBnClickedOk();
	CStatic m_textRefreshEvery;
	CEdit m_editTimeAfter;
	CStatic m_textSecondsFor;
	afx_msg void OnEnChangeEditIconTimeAfter();
	afx_msg void OnCbnSelchangeComboGadgetType();

	afx_msg LRESULT OnGetFloaterData(WPARAM wParam,LPARAM lParam);

	CComboBox m_listType;
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
};

#pragma once
#include "afxwin.h"


// CDlgSettingsView dialog

class CDlgSettingsView : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsView)

public:
	CDlgSettingsView();
	virtual ~CDlgSettingsView();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	int		m_iTime;
	int		m_iRefresh;
	int		m_iRefreshManual;

	bool	m_bUseHorizontalGrid;
	bool	m_bUseVerticalGrid;
	bool	m_bUseSkin;
	int		m_iUseSkin;
	bool	m_bAlternatingStriped;
	bool	m_bPercRect;
	bool	m_bAdjustTime;
	bool	m_bBoincTranslatable;

	int		m_iTimeDhms;
	CString m_sTimeFormat;

	char	m_cComma;
	char	m_cThousend;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_VIEW };

protected:
			void CheckRefreshManualEnabled();
			void CheckTimeFormatEnabled();
			bool CheckValidtime(CString sTimeToCheck, CString *psTime);

			void DecimalPointAndThousandSep(CString *psDefault);

	afx_msg void OnCbnSelchangeComboTime1224();
	afx_msg void OnCbnSelchangeComboRefreshrate();
	afx_msg void OnEnChangeEditRefreshrateManual();

	CString m_sSeperators;

	CBitmap m_bmp;

	CComboBox	m_time;
	CComboBox	m_timeDhms;
	CComboBox	m_refresh;
	CComboBox	m_skinNr;
	CEdit		m_refreshManual;
	CEdit		m_editTimeFormat;
	CStatic		m_refreshManualText;
	CStatic		m_TextNumberFormatTime;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CButton m_buttonOk;
	CStatic m_textTimeFormat;
	CStatic m_textRefreshRate;
	CButton m_checkHorizontalGrid;
	CButton m_checkVerticalGrid;
	afx_msg void OnBnClickedCheckHorizontalLines();
	afx_msg void OnBnClickedCheckVerticalLines();
	afx_msg void OnBnClickedCheckStripes();
	CButton m_checkAlternatingStripes;
	CStatic m_TextNumberFormat;
	CStatic m_TextNumberFormatExample;
	CEdit m_editNumberFormat;
	CButton m_checkPercRect;
	CButton m_checkTranslatable;

	afx_msg void OnEnChangeEditNumberFormat();
	afx_msg void OnCbnSelchangeComboTimeFormat();
	afx_msg void OnEnChangeEditTimeUserDefined();
	afx_msg void OnBnClickedCheckBoincTranslatable();

	CButton m_checkSkin;
	afx_msg void OnBnClickedCheckSkin();

	afx_msg void OnCbnSelchangeComboSkin();
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
	afx_msg void OnBnClickedCheckPercRect();
	CButton m_checkAdjustTime;
	afx_msg void OnBnClickedCheckAdjustTime();
};

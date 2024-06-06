#pragma once
#include "afxwin.h"
#include "ColorListBox.h"
#include "DlgNotices.h"

#define NOTICES_HIDE_AFTER_DAYS 60
#define NOTICES_SHOW_ALERT_EVERY 30
#define NOTICES_CHECK_EVERY 30

// CDlgSettingsNotices dialog

class CDlgSettingsNotices : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsNotices)

public:
	CDlgSettingsNotices();
	virtual ~CDlgSettingsNotices();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	int		m_iHideNoticeAfter;
	int		m_iAlertEvery;
	int		m_iCheckEvery;

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_NOTICES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	void	SetNoticesAfter(int iCheck);
	void	SetNoticesAlert(int iCheck);

	void	SortingSetDefault();
	void	SortingUpdate();
	void	SortingWriteRegistry();

	CNoticeSorting	m_sorting[NOTICES_SORT_COUNT+1];

	CBitmap m_bmp;

	DECLARE_MESSAGE_MAP()
public:
	CButton m_buttonOk;
	CButton m_checkHideNotices;
	CButton m_CheckAlert;
	CStatic m_textDays;
	CStatic m_textCheckEvery;
	CEdit m_editHideAfter;
	CEdit m_editAlert;
	CEdit m_editCheckEvery;
	CStatic m_textMinutes1;
	CStatic m_textMinutes2;
	afx_msg void OnBnClickedCheckNoticesShowAfterDays();
	afx_msg void OnBnClickedCheckNoticesShowAlert();
	afx_msg void OnEnChangeEditNoticesShowAfter();
	afx_msg void OnEnChangeEditNoticesAlert();
	afx_msg void OnEnChangeEditNoticesCheck();
	afx_msg void OnBnClickedOk();
	CColorListBox m_listNoticesSort;
//	afx_msg void OnLbnSelchangeListNoticesSorting();
	afx_msg void OnBnClickedButtonNoticesUp();
//	afx_msg void OnBnClickedButtonNoticesDown();

	afx_msg LRESULT OnChangedSelection(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnNoticesSort(WPARAM parm1, LPARAM parm2);

	CButton m_buttonUp;
	CButton m_buttonDown;
	CStatic m_textSorting;
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
};

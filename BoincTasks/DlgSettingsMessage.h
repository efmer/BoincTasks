#pragma once
#include "afxwin.h"
#include "DlgColorMessages.h"

#define DEFAULT_KEEP_MESSAGES	5000

// CDlgSettingsMessage dialog

class CDlgSettingsMessage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsMessage)

public:
	CDlgSettingsMessage();
	virtual ~CDlgSettingsMessage();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

	void	ReadColumsFromRegistry();
	void	WriteColumsToRegistry();
	void	MessageColumns();
	void	InvalidateMessages();

	bool	m_bMessagesKeep;
	int		m_iKeepMessages;

	CString m_sHighlight[COLOR_ARRAY_HIGHLIGHT_MAX+1];
	CString m_sHighlightProject[COLOR_ARRAY_HIGHLIGHT_MAX+1];

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_MESSAGE };	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg		LRESULT OnSettingsMessageGet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsMessageSet(WPARAM parm1, LPARAM parm2);

	int			m_iActiveColumns;
	int			m_iMessageColumnIn[NUM_REMOTE_MESSAGE_COLUMNS+1];
	int			m_iMessageColumn[NUM_REMOTE_MESSAGE_COLUMNS+1];
	int			m_iMessageColumnLookup[NUM_REMOTE_MESSAGE_COLUMNS+1];

	CButton		m_buttonOk;

	CBitmap m_bmp;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedOk();

public:
	CStatic m_textMessagesKeep;
	afx_msg void OnEnChangeEditMessagesKeep();
	CEdit m_editKeepMessages;
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
	CStatic m_textMessagesHl1;
	CStatic m_textMessagesHl2;
	CStatic m_textMessagesHl3;
	CStatic m_textMessagesHl4;
	CStatic m_textMessagesHl5;
	CStatic m_textMessagesHl6;
	CStatic m_textMessagesHl7;
	CStatic m_textMessagesHl8;
	CStatic m_textMessagesHl9;
	CStatic m_textMessagesHl10;

	CStatic m_textMessagesProject;
	CStatic m_textMessagesMessage;
	CEdit m_editProjectHl1;
	CEdit m_editProjectHl2;
	CEdit m_editProjectHl3;
	CEdit m_editProjectHl4;
	CEdit m_editProjectHl5;
	CEdit m_editProjectHl6;
	CEdit m_editProjectHl7;
	CEdit m_editProjectHl8;
	CEdit m_editProjectHl9;
	CEdit m_editProjectHl10;

	CEdit m_editHl1;
	CEdit m_editHl2;
	CEdit m_editHl3;
	CEdit m_editHl4;
	CEdit m_editHl5;
	CEdit m_editHl6;
	CEdit m_editHl7;
	CEdit m_editHl8;
	CEdit m_editHl9;
	CEdit m_editHl10;

	afx_msg void OnEnChangeEditMessagesHl1();
	afx_msg void OnEnChangeEditMessagesHl2();
	afx_msg void OnEnChangeEditMessagesHl3();
	afx_msg void OnEnChangeEditMessagesHl4();
	afx_msg void OnEnChangeEditMessagesHl5();
	afx_msg void OnEnChangeEditMessagesHl6();
	afx_msg void OnEnChangeEditMessagesHl7();
	afx_msg void OnEnChangeEditMessagesHl8();
	afx_msg void OnEnChangeEditMessagesHl9();
	afx_msg void OnEnChangeEditMessagesHl10();

	afx_msg void OnEnChangeEditMessagesHl1Project();
	afx_msg void OnEnChangeEditMessagesHl1Project2();
	afx_msg void OnEnChangeEditMessagesHl1Project3();
	afx_msg void OnEnChangeEditMessagesHl1Project4();
	afx_msg void OnEnChangeEditMessagesHl1Project5();
	afx_msg void OnEnChangeEditMessagesHl1Project6();
	afx_msg void OnEnChangeEditMessagesHl1Project7();
	afx_msg void OnEnChangeEditMessagesHl1Project8();
	afx_msg void OnEnChangeEditMessagesHl1Project9();
	afx_msg void OnEnChangeEditMessagesHl1Project10();
};

#pragma once
#include "afxwin.h"


// CDlgSettingsProject dialog

class CDlgSettingsProject : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsProject)

public:
	CDlgSettingsProject();
	virtual ~CDlgSettingsProject();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_PROJECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void	ReadColumsFromRegistry();
	void	WriteColumsToRegistry();
	void	CheckIntegrity();
	void	ProjectsColumns();
	void	EnableCheck();
	void	SetColumn(int iColumn, int iCheck);
	void	SetColumnHide(int iColumn, int iCheck);

	CButton		m_buttonOk;
	CButton		m_apply;
	CStatic		m_textGroupShow;

	int			m_iActiveColumns;
	int			m_iProjectsColumnIn[NUM_REMOTE_PROJECT_COLUMNS+1];
	int			m_iProjectsColumn[NUM_REMOTE_PROJECT_COLUMNS+1];
	int			m_iProjectsColumnLookup[NUM_REMOTE_PROJECT_COLUMNS+1];

	CBitmap m_bmp;

	afx_msg		LRESULT OnSettingsProjectsGet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsProjectsSet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsProjectsWidth(WPARAM parm1, LPARAM parm2);

	DECLARE_MESSAGE_MAP()
public:
	CButton m_checkShow[NUM_REMOTE_PROJECT_COLUMNS+1];
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedCheckProjectsColumn0();
	afx_msg void OnBnClickedCheckProjectsColumn1();
	afx_msg void OnBnClickedCheckProjectsColumn2();
	afx_msg void OnBnClickedCheckProjectsColumn3();
	afx_msg void OnBnClickedCheckProjectsColumn4();
	afx_msg void OnBnClickedCheckProjectsColumn5();
	afx_msg void OnBnClickedCheckProjectsColumn6();
	afx_msg void OnBnClickedCheckProjectsColumn7();
	afx_msg void OnBnClickedCheckProjectsColumn8();
	afx_msg void OnBnClickedCheckProjectsColumn9();
	afx_msg void OnBnClickedCheckProjectsColumn10();
	afx_msg void OnBnClickedCheckProjectsColumn11();
	afx_msg void OnBnClickedCheckProjectsColumn12();
	afx_msg void OnBnClickedCheckProjectsColumn13();
	afx_msg void OnBnClickedCheckProjectsColumn14();
	afx_msg void OnBnClickedCheckProjectsColumn15();
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
	afx_msg void OnBnClickedCheckProjectsColumn16();
};

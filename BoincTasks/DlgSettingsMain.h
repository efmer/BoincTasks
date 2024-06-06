#pragma once



// CDlgSettingsMain

class CDlgSettingsMain : public CPropertySheet
{
	DECLARE_DYNAMIC(CDlgSettingsMain)

public:
	CDlgSettingsMain(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDlgSettingsMain(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDlgSettingsMain();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);
	void	SafeWindowPlacement();
protected:

	HICON	m_hIcon;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};



#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrlRules.h"
#include "DlgRules.h"

#define NUM_RULES_COLUMNS	4

//#define STATE_ACTIVE
//#define STATE_INACTIVE




// DlgSettingsRules dialog

class CDlgSettingsRules : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsRules)

public:
	CDlgSettingsRules();
	virtual ~CDlgSettingsRules();
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_RULES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void AddColumns();
	void EnableButtons(bool bEnable);
	void CheckEnable();
	int	ReadXml(void);
	void WriteXml(void);
	void DialogToItem(CDlgRules *pDlg, int iItem, CRuleItem *pRuleItem);
	void AddRule(CString *psComputer, CString *psProject, CString *psApplication);

    afx_msg void OnTimer(UINT_PTR nIDEvent);

	CImageList m_StateImageList;
	bool		m_bButtonsEnabled;

//	CList <CRuleItem*,CRuleItem*> m_lRules;
	std::deque<CRuleItem*> m_lRules;

	bool		m_bRulesChanged;
	bool		m_bRulesLocked;

	CBitmap m_bmp;

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnAddRule(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCopy(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonRulesAdd();
	CListCtrlRules m_listRules;
	CButton m_buttonDelete;
	CButton m_buttonEdit;
	CButton m_buttonAdd;
	afx_msg void OnBnClickedButtonRulesDelete();
	afx_msg void OnBnClickedButtonRulesEdit();
	afx_msg void OnBnClickedOk();
	CButton m_buttonOk;

	afx_msg void OnLvnItemchangedListRules(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListRules(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedHelp();
	CButton m_buttonHelp;
};

#pragma once

#include "ThreadRpc.h"

#define DLG_PREF_EXCLUSIVE_TAB	3

// CDlgPrefMain


class CDlgPrefDisk;
class CDlgPrefNetwork;
class CDlgPrefProcessor;
class CDgPrefExclusive;
class CDlgPrefMain : public CPropertySheet
{
	DECLARE_DYNAMIC(CDlgPrefMain)

public:
	CDlgPrefMain(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDlgPrefMain(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDlgPrefMain();
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnInitDialog();

//	void	Init(int iThreadId);

	void	Ok(void);
	void	Cancel(void);
	void	Clear(void);

	CDlgPrefDisk			*m_pDlgPrefDisk;
	CDlgPrefNetwork			*m_pDlgPrefNetwork;
	CDlgPrefProcessor		*m_pDlgPrefProcessor;
	CDgPrefExclusive		*m_pDlgPrefExclusive;

protected:
	void	FillDialog();
	void	SafeWindowPlacement();
	HICON	m_hIcon;
	int		m_iThreadId;

	int 	m_iThreadBusy;

	int		m_iLastActive;
	bool	m_bDisabled;

	CRpcPreferenceInfo	m_preferenceInfo;
	CRpcConfig			m_config;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	afx_msg LRESULT OnPreferenceOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnPreferenceReadReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnGetConfigReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSetConfigReady(WPARAM parm1, LPARAM parm2);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};



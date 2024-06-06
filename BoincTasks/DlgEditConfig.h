#pragma once
#include "resource.h"
#include "DlgEditConfigBase.h"

// CDlgEditConfig dialog

class CDlgEditConfig : public DlgEditConfigBase
{
	DECLARE_DYNAMIC(CDlgEditConfig)

public:
	CDlgEditConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditConfig();
	virtual BOOL OnInitDialog();

	void SafeWindowPlacement();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT_CONFIG};

protected:
	CRpcConfig	m_config;

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnEditConfigOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnConfigReadReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnConfigWriteReady(WPARAM parm1, LPARAM parm2);
	afx_msg void OnBnClickedApply();

};

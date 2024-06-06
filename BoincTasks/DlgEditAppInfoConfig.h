#pragma once
#include "resource.h"
#include "DlgEditConfigBase.h"

// CDlgEditAppInfoConfig dialog

class CDlgEditAppInfoConfig : public DlgEditConfigBase
{
	DECLARE_DYNAMIC(CDlgEditAppInfoConfig)

public:
	CDlgEditAppInfoConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditAppInfoConfig();
	virtual BOOL OnInitDialog();

	void SafeWindowPlacement();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT_APP_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int Parse(XML_PARSER& xp, MSG_VEC& mv);
	void DeleteNew();

	CRpcAppInfo	m_AppConfig;

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnEditConfigOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnConfigReadReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnConfigWriteReady(WPARAM parm1, LPARAM parm2);

	afx_msg void OnBnClickedApply();
};

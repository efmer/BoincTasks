#pragma once

#include "ThreadRpc.h"

// CDlgProxyMain

class CDlgProxyMain : public CPropertySheet
{
	DECLARE_DYNAMIC(CDlgProxyMain)

public:
	CDlgProxyMain(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDlgProxyMain(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDlgProxyMain();
	virtual BOOL OnInitDialog();


	void	Ok(void);
	void	Cancel(void);
protected:
	void	FillDialog();

	HICON	m_hIcon;
	int		m_iThreadId;

	CRpcProxyInfo m_proxyInfo;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	afx_msg LRESULT OnProxyOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProxyReadReady(WPARAM parm1, LPARAM parm2);

	DECLARE_MESSAGE_MAP()
};



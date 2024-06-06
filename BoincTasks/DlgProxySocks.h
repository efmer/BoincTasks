#pragma once
#include "afxwin.h"


// CDlgProxySocks dialog

class GR_PROXY_INFO;
class CDlgProxySocks : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgProxySocks)

public:
	CDlgProxySocks();
	virtual ~CDlgProxySocks();
	virtual BOOL OnInitDialog();

	void	FillDialog(GR_PROXY_INFO *pGrProxyInfo);
	bool	ReadDialog(GR_PROXY_INFO *pGrProxyInfo);
	void	Enable(bool bEnable);
	bool	m_bNoProxy;
// Dialog Data
	enum { IDD = IDD_DIALOG_PROXY_SOCKS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckProxySocksUse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditProxySocksServer();
	afx_msg void OnEnChangeEditProxySocksPort();
	afx_msg void OnEnChangeEditProxySocksNoProxy();
	afx_msg void OnEnChangeEditProxySocksUser();
	afx_msg void OnEnChangeEditProxySocksPassword();
	CStatic m_textProxySocksGroupHttp;
	CStatic m_textProxySocksServer;
	CStatic m_textProxySocksPort;
	CStatic m_textProxySocksNoFor;
	CStatic m_textProxySocksUser;
	CStatic m_textProxySocksGroupLeaveEmpty;
	CStatic m_textProxySocksPassword;
	CEdit m_editProxySocksServer;
	CEdit m_editProxySocksPort;
	CEdit m_editProxySocksNoProxy;
	CEdit m_editProxySocksUser;
	CEdit m_editProxySocksPassword;
	CButton m_buttonOk;
	CButton m_buttonCancel;
	CButton m_checkProxyUse;
};

#pragma once
#include "afxwin.h"


// CDlgProxyHttp dialog

class GR_PROXY_INFO;
class CDlgProxyHttp : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgProxyHttp)

public:
	CDlgProxyHttp();
	virtual ~CDlgProxyHttp();
	virtual BOOL OnInitDialog();

	void	FillDialog(GR_PROXY_INFO *pGrProxyInfo);
	bool	ReadDialog(GR_PROXY_INFO *pGrProxyInfo);
	BOOL	PreTranslateMessage(MSG* pMsg);
	void	Enable(bool bEnable);
	bool	m_bNoProxy;

// Dialog Data
	enum { IDD = IDD_DIALOG_PROXY_HTTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckProxyHttpUse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditProxyHttpServer();
	afx_msg void OnEnChangeEditProxyHttpPort();
	afx_msg void OnEnChangeEditProxyHttpNoProxy();
	afx_msg void OnEnChangeEditProxyHttpUser();
	afx_msg void OnEnChangeEditProxyHttpPassword();
	CStatic m_textProxyHttpGroupHttp;
	CStatic m_textProxyHttpServer;
	CStatic m_textProxyHttpPort;
	CStatic m_textProxyHttpNoFor;
	CStatic m_textProxyHttpUser;
	CStatic m_textProxyHttpGroupLeaveEmpty;
	CStatic m_textProxyHttpPassword;
	CEdit m_editProxyHttpServer;
	CEdit m_editProxyHttpPort;
	CEdit m_editProxyHttpNoProxy;
	CEdit m_editProxyHttpUser;
	CEdit m_editProxyHttpPassword;
	CButton m_buttonOk;
	CButton m_buttonCancel;
	CButton m_checkProxyUse;
};

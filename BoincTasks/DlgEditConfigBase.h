#pragma once
#include "afxwin.h"
#include "stdafx.h"
#include "ThreadRpc.h"
#include "RichEditCtrlXml.h"
#include "afxcmn.h"

// DlgEditConfigBase dialog

class CRemoteCombinedDoc;

class DlgEditConfigBase : public CDialogEx
{
	DECLARE_DYNAMIC(DlgEditConfigBase)

public:
	DlgEditConfigBase(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgEditConfigBase();
	void InitialUpdate(CRemoteCombinedDoc *pDoc, CString sFile);

	CRichEditCtrlXml m_window;
	afx_msg void OnEnChangeWndEditConfig();
	CButton m_buttonApply;
	CButton m_buttonCheckSyntax;
	afx_msg void OnBnClickedCheck();

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedCheckIgnoreSyntax();
	CButton m_checkIgnoreSyntax;

// Dialog Data
	UINT IDD;

//	enum { IDD = IDD_DLGEDITCONFIGBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void Check(void);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);

	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CRemoteCombinedDoc *m_pDoc;

	LONG	m_lLastWidth;
	LONG	m_lLastHeight;
	int		m_iWindowInitialLeftMargin;
	int		m_iWindowInitialTopMargin;

	int		m_iThreadId;
	bool	m_bThreadBusy;

	bool	m_bNeedSyntaxCheck;
	bool	m_bIgnoreSyntax;

	HICON		m_hIcon;

	CString		m_sFileXml;


	DECLARE_MESSAGE_MAP()

};

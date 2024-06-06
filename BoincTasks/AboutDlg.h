#pragma once
#include "afxwin.h"
#include "Label.h"
#include "TextBox.h"

// CAboutDlg dialog

#define ABOUT_COLOR	COLORREF(RGB(0, 0, 255))


class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnTimer(UINT_PTR nIDEvent);

	HICON	m_hIcon;

	HCURSOR m_hCursor;
	double	m_dDeflateHeight;
	double	m_dDeflateWidth;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus);

	CTextBox m_text;
	afx_msg void OnStnClickedEfmerHttp();
	afx_msg void OnStnClickedSnlHttp();
	CLabel m_efmerHttp;
	CLabel m_snlHttp;
	CEdit m_version;
	CStatic m_textTranslatedBy;
	CLabel m_textTranslatedTeam;
	afx_msg void OnStnClickedTextTranslatedTeam();
//	CTextBox m_textWindow;
	CStatic m_bmpSnl;
};

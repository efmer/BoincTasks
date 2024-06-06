#pragma once
#include "afxwin.h"


// CDlgSearch dialog

class CDlgSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSearch)

public:
	CDlgSearch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSearch();

	CString	m_sSearch;

// Dialog Data
	enum { IDD = IDD_DIALOG_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_buttonCancel;
	CButton m_buttonOk;
	afx_msg void OnBnClickedOk();
	CEdit m_editSearch;
};

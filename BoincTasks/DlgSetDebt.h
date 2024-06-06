#pragma once
#include "afxwin.h"


// CDlgSetDebt dialog


class CRpcProjectsDiskUsage;
class PROJECTS;
class CDlgSetDebt : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetDebt)

public:
	CDlgSetDebt(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetDebt();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_SET_DEBT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetCurrentSelection();

	void ApplyButton();

	CRpcProjectsDiskUsage *m_pProjectsDiskUsage;

	int				m_iThreadId;
	CString 		m_sProject;
	CString			m_sComputer;
	PROJECTS		*m_pProject;

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnOpenDialog(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSetThreadId(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnReadProject(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSetProjectDebtReady(WPARAM parm1, LPARAM parm2);

public:
	CStatic m_textSetDebtCpuLong;
	CStatic m_textSetDebtNVidiaLong;
	CStatic m_textSetDebtAtiLong;
	CStatic m_textSetDebtCpuShort;
	CStatic m_textSetDebtNVidiaShort;
	CStatic m_textSetDebtAtiShort;
	CEdit m_editSetDebtCpuLong;
	CEdit m_editSetDebtCpuLongSet;
	CEdit m_editSetDebtNVidiaLong;
	CEdit m_editSetDebtNVidiaLongSet;
	CEdit m_editSetDebtAtiLong;
	CEdit m_editSetDebtAtiLongSet;
	CEdit m_editSetDebtCpuShort;
	CEdit m_editSetDebtCpuShortSet;
	CEdit m_editSetDebtNVidiaShort;
	CEdit m_editSetDebtNVidiaShortSet;
	CEdit m_editSetDebtAtiShort;
	CEdit m_editSetDebtAtiShortSet;
	CButton m_buttonApply;
	CButton m_buttonCancel;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnSelchangeListSetDebtProjects();
	CListBox m_listProjects;
	afx_msg void OnEnChangeEditSetDebtCpuLongSet();
	afx_msg void OnEnChangeEditSetDebtCpuShortSet();
};

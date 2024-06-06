#pragma once
#include "afxwin.h"

#define EXCLUSIVE_TEXT_GPU		"Gpu"
#define EXCLUSIVE_TEXT_CPU_GPU	"Cpu & Gpu"

// CDgPrefExclusiveAdd dialog

class CDgPrefExclusiveAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CDgPrefExclusiveAdd)

public:
	CDgPrefExclusiveAdd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgPrefExclusiveAdd();
	virtual BOOL OnInitDialog();

	bool	m_bSet;
	CString m_sCpuGpu;
	CString m_sExe;

// Dialog Data
	enum { IDD = IDD_DIALOG_PREFERENCE_EXCLUSIVE_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL	BrowseForFolder(HWND hwnd, LPCTSTR szCurrent, LPTSTR szPath);
	bool	GetProcess();
	CString	GetProcessFileName(CString sProcess);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckGpu();
	CButton m_checkGpu;
	afx_msg void OnEnChangeEditProgram();
	CEdit m_editProgram;
	afx_msg void OnBnClickedOpenFile();
	CButton m_buttonOk;
	CButton m_buttonCancel;
	CComboBox m_combo_process;
	CEdit m_edit_process;
	afx_msg void OnCbnSelchangeComboProcess();
};

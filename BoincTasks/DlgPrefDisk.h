#pragma once
#include "afxwin.h"


// CDlgPrefDisk dialog

class CDlgPrefMain;
class CRpcPreferenceInfo;
class CDlgPrefDisk : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgPrefDisk)

public:
	CDlgPrefDisk();
	virtual ~CDlgPrefDisk();
	virtual BOOL OnInitDialog();

	void	FillDialog(CRpcPreferenceInfo *pPreferenceInfo);
	void	ReadDialog(CRpcPreferenceInfo *pPreferenceInfo);

// Dialog Data
	enum { IDD = IDD_DIALOG_PREFERENCE_DISK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDlgPrefMain *m_pDlgPrefMain;

	CEdit ValueDiskUseAtMost;
	CEdit ValueDiskLeaveAtLeast;
	CEdit ValueDiskUseAtMostPerc;
	CEdit ValueDiskWriteToDiskEvery;
	CEdit ValueDiskUseAtMostPageFile;
	CEdit ValueMemoryUseWhenInUse;
	CEdit ValueMemoryUseWhenIdle;
	CButton m_checkMemoryLeaveApplicationInMemory;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClear();
	CStatic m_textDiskUsage;
	CStatic m_textMemoryUsage;
	CStatic m_textUseAtMost1;
	CStatic m_textUseAtMost2;
	CStatic m_textUseAtMost3;
	CStatic m_textUseAtMost4;
	CStatic m_textUseAtMost5;
	CStatic m_textLeaveAtLeast;
	CStatic m_textWriteToDiskAtMostEvery;
	CStatic m_textGigabyteDiskSpace;
	CStatic m_textGigabyteDiskSpaceFree;
	CStatic m_textPercOfTotalDiskSpace;
	CStatic m_textSeconds;
	CStatic m_textPercOfPageFile;
	CStatic m_textPercWhenComputerIsInUse;
	CStatic m_textPercWhenComputerIsIdle;
	CButton m_buttonClear;
	CButton m_buttonOk;
	CButton m_buttonCancel;
};

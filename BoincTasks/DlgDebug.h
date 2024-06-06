#pragma once
#include "afxwin.h"


class CDlgLogging;

// CDlgDebug dialog

class CDlgDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDebug)

public:
	CDlgDebug(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDebug();

	CDlgLogging *m_pLogging;

// Dialog Data
	enum { IDD = IDD_DIALOG_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckDebugNotices();
	afx_msg void OnBnClickedCheckDebugNoticesTimer();

	CButton m_checkNotices;
	CButton m_checkNoticesTimer;
	CButton m_checkConnecting;
	CButton m_checkClosing;
	CButton m_checkSearchComputers;
	CButton m_checkTemperatureGpu;
	CButton m_checkTThrottle;
	CButton m_checkTThrottleData;
	CButton m_checkTThrottleRead;
	CButton m_checkTThrottleGraph;
	CButton m_checkSnooze;
	CButton m_checkHistoryFetching;
	CButton m_checkTasks;
	CButton m_checkDirectoryLocal;
	CButton m_checkDirectory;
	CButton m_checkState;
	CButton m_checkMobile;
	CButton m_checkMobileScript;
	CButton m_checkMobileConnect;
	CButton m_checkMobileConnectDump;
	CButton m_checkCloud;
	CButton m_checkHeap;
	CButton m_checkFileOnly;
	afx_msg void OnBnClickedCheckDebugConnecting();
	afx_msg void OnBnClickedCheckDebugComputerSearch();
	afx_msg void OnBnClickedCheckDebugClosing();
	afx_msg void OnBnClickedCheckDebugTempGpu();
	afx_msg void OnBnClickedCheckDebugTthrottle();
	afx_msg void OnBnClickedCheckDebugTthrottleData();
	afx_msg void OnBnClickedCheckDebugTthrottleRead();
	afx_msg void OnBnClickedCheckDebugTthrottleGraph();
	afx_msg void OnBnClickedCheckDebugSnooze();
	afx_msg void OnBnClickedCheckDebugHistoryFetch();
	afx_msg void OnBnClickedCheckDebugTasks();
	afx_msg void OnBnClickedCheckDebugLocalDirectory();
	afx_msg void OnBnClickedCheckDebugDirectory();
	afx_msg void OnBnClickedCheckDebugState();
	afx_msg void OnBnClickedCheckDebugMobile();
	afx_msg void OnBnClickedCheckDebugMobileScript();
	afx_msg void OnBnClickedCheckDebugMobileConnect();
	afx_msg void OnBnClickedCheckDebugMobileConnectDump();
	afx_msg void OnBnClickedCheckDebugCloud();
	afx_msg void OnBnClickedCheckDebugHeap();
	afx_msg void OnBnClickedCheckDebugFileOnly();
	CButton m_checkTasksShow;
	afx_msg void OnBnClickedCheckDebugTasksShow();
};

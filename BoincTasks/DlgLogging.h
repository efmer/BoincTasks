#pragma once
#include "afxwin.h"

#define LOGGING_MAX_BUFFER  40000
#define LOGGING_NORMAL			1
#define LOGGING_DEBUG			2
#define LOGGING_DEBUGFULL		3
//#define LOGGING_DEBUG_TASKS		4
//#define LOGGING_DEBUG_TTHROTTLE	5

#define MAX_MEMORY_TEST			32
#define MEMORY_TEST_ALLOCATION	256000000

#define LOGGING_FOLDER		"\\log\\"

class CDlgDebug;

class CLoggingMessage
{
public:
	char *m_pFrom;
	char *m_pText;
};


// CDlgLogging dialog

class CDlgLogging : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogging)

public:
	CDlgLogging(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLogging();
	void	SafeWindowPlacement();
	void	AddLoggingInfo();

	bool	m_bDebugMode;

	bool	m_bLogDebugNotices;
	bool	m_bLogDebugNoticesTimer;
	bool	m_bLogDebugTemperatureGpu;
	bool	m_bLogDebugTThrottle;
	bool	m_bLogDebugTThrottleData;
	bool	m_bLogDebugTThrottleRead;
	bool	m_bLogDebugTThrottleGraphic;
	bool	m_bLogDebugSnooze;
	bool	m_bLogDebugDirectory;
	bool	m_bLogDebugHistoryFetching;
	bool	m_bLogDebugSearchComputers;
	bool	m_bLogDebugConnecting;
	bool	m_bLogDebugState;
	bool	m_bLogDebugClosing;
	bool	m_bLogDebugMobile;
	bool	m_bLogDebugMobileConnect;
	bool	m_bLogDebugMobileConnectDump;
	bool	m_bLogDebugCloud;
	bool	m_bLogDebugLocalDirectory;
	bool	m_bLogDebugHeapCheck;
	bool	m_bLogDebugMobileScript;
	bool	m_bLogDebugTasks;
	bool	m_bLogDebugTasksShow;
	bool	m_bLogDebugFileOnly;

// Dialog Data
	enum { IDD = IDD_DIALOG_LOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void AddLogging(CString sFrom, CString sLog, bool bTime = true, bool bFileOnly = false);
	void AddLoggingDebug(CString sFrom, CString sLog, bool bTime = true);
	void AddLoggingDebugFull(CString sFrom, CString sLog, bool bTime = true);
	void ShowLoggingText();
	void AddLoggingToFile(CString *psTxt);


	void MemoryTestClear(bool bInit = false);
	void MemoryTest();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CString GetLogName();
	void	CheckDayChanged();
	void	OpenLog(bool bFirstTime);
	void	CloseLog();

	CString *m_psLogging;

	CString m_sLogFileName;
	CFile	m_file;
	bool	m_bFileOpen;

	bool	m_bLogDebugForce;

	bool	m_bDebugFullMode;

	bool	m_bFirstTimeMemoryLog;
	bool	m_bMemoryTest;
	int		m_iMemoryTest;
	char	*m_pcMemoryTest[MAX_MEMORY_TEST+1];

	HICON	m_hIcon;


	CString m_sMagic;
	CString m_sSilver;
	CString m_sMemory;

	CEdit m_logging;

	CDlgDebug *m_pDlgDebug;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditLogging();
	CButton m_checkEnableDebug;
	afx_msg void OnBnClickedCheckDebug();
	afx_msg LRESULT OnLoggingText(WPARAM parm1, LPARAM parm2);
	CStatic m_fullDebug;
	CButton m_buttonOk;
	CButton m_buttonClear;
	afx_msg void OnBnClickedClear();
	CButton m_buttonCancel;
	afx_msg void OnBnClickedFlags();
	CButton m_buttonSetDebug;
	void Translate();
};

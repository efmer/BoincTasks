#pragma once
#include "afxwin.h"

#define TTHROTTLE_UPDATE_NUMBERS 3

// CDlgTThrottle dialog

class CRpcInfoClient;
class CRemoteCombinedDoc;
class CDlgTThrottle : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTThrottle)

public:
	CDlgTThrottle(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTThrottle();

	void	SafeWindowPlacement();
	void	InitialUpdate(CRemoteCombinedDoc *pDoc);
	void	Init();
	void	ComputerSelectionChanged();
	void	InvalidReading();
	void	ButtonsActive(BOOL bActive);

	BOOL PreTranslateMessage(MSG* pMsg);

// Dialog Data
	enum { IDD = IDD_DIALOG_TTHROTTLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool		m_bTimerActive;
	int			m_iSelectedComputers;
	bool		m_bComputerSelectionChanged;
	int			m_iUpdateNumbers;
	CString		m_sSelectedComputer;
	int			m_iSelectedThreadId;

//	bool		m_bUpdateNeeded;
	bool		m_bUpdateApply;
	bool		m_bAutoActiveEnabled;

	bool		m_bThreadLocked;
	int			m_iThreadLockedTimeout;

	std::deque<CRpcConnectionItem*>	m_lListConnectionsItems;
	std::deque<CRpcInfoClient*>		m_lRpcInfoClient;

	CRemoteCombinedDoc *m_pDoc;
	HICON		m_hIcon;

	bool		m_bInit;

	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg LRESULT OnRpcConnection(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReadyRpcClient(WPARAM parm1, LPARAM parm2);

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listComputers;
	afx_msg void OnBnClickedCheckTthrottleAuto();
	afx_msg void OnEnChangeEditTthrottleCoreSet();
	afx_msg void OnEnChangeEditTthrottleGpuSet();
	afx_msg void OnEnChangeEditTthrottleCpuMax();
	afx_msg void OnEnChangeEditTthrottleCpuMin();
	afx_msg void OnLbnSelchangeListTthrottleComputers();
	CEdit m_editCore;
	CStatic m_textCore;
	CEdit m_editSetCore;
	CStatic m_textSetCore;
	CEdit m_editGpu;
	CStatic m_textGpu;
	CEdit m_editSetGpu;
	CStatic m_textSetGpu;
	CEdit m_editRunningCpu;
	CStatic m_textRunningCpu;
	CEdit m_editRunningGpu;
	CStatic m_textRunningGpu;
	CEdit m_editMaxCpu;
	CStatic m_textMaxCpu;
	CEdit m_editMinCpu;
	CStatic m_textMinCpu;
	CButton m_checkAutoActive;
	CEdit m_editInfo;
	CButton m_buttonApply;
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedOk();
	CStatic m_textComputers;
	CStatic m_textTemperature;
	CStatic m_textRunPercentage;
};

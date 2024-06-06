#pragma once


#define  NUM_REMOTE_COMPUTER_COLUMNS	11

class CThreadRpc;

// CRemoteCombinedComputerView view

class CThreadRpc;
class CRemoteCombinedDoc;
class CRemoteCombinedComputerView : public CTemplateRemoteComputerView
{
	friend class CRemoteCombinedDoc;
	DECLARE_DYNCREATE(CRemoteCombinedComputerView)

public:
	CRemoteCombinedComputerView();           // protected constructor used by dynamic creation
	virtual ~CRemoteCombinedComputerView();
	virtual void OnInitialUpdate();				// called first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView );

	void	InitialUpdate(CRemoteCombinedDoc* pDoc);
	bool	ListChanged(bool bEncrypted);
	void	SaveComputerList();

	bool	m_bDocumentIsClosing;

//	std::deque<CComputerComputerItem*>	m_lComputers;

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	void	ReadAllColumnWidth();
	int		ReadColumnWidth(int iColumn, int iDefault);
	void	CheckColumnWidth();
	CString GetComputerStatus(int iStatus);
	void	FillList();
	CString RemoveHidden(CString sTxt);
	CString ExtractHidden(CString *psTxt, CString *psTxtHidden);
	void	RenameFile(CString sNow, CString sNew, bool bHistoryLong);

	void	SendTThrottleRequest(int iNrTThrottleRpc, bool bExcludeFromList = true);

//	void	DisplayRpc(CThreadRpc *pThreadRpc, int iNrTransfer, int *piRow, CString *psComputerId);

	void	PostTransferOperation(int iOperation);

	int			m_iTimerComputer;
	
	time_t		m_LastSwitchedTime;

	int			m_iNrColomns;

	int			m_iThreadLocked;
	bool		m_bThreadBusy;
	bool		m_bFirstTime;


	CImageList	m_StateImageList;

//	HWND			m_hWndPreference;

	CRemoteCombinedDoc	*m_pDoc;

//	HWND				m_hWndPreference;
	HWND				m_hWndProxy;

	int		m_iColumnWidth[NUM_REMOTE_COMPUTER_COLUMNS+1];		// for xchange only
	int		m_iColumnOrder[NUM_REMOTE_COMPUTER_COLUMNS+1]; 

	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg LRESULT OnReadyRpc(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnReadyRpcClient(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColumnChanged(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnTimerComputers(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnSettingsTasks(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSwitch(WPARAM parm1, LPARAM parm2);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg LRESULT OnPreferenceDialog(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProxyDialog(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelTabView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnFloaterData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeCpu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetSnoozeGpu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsTasksSetWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsProjectSetWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingsHistorySetWidth(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnBoincTasksVersion(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBoincTasksRule(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchComputerReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebItemSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebSort(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnFontChangeAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontHasChanged(WPARAM wParam, LPARAM lParam);

	afx_msg void OnComputersAdd();
	afx_msg void OnComputersRemove();
	afx_msg void OnComputersDetect();

	afx_msg void OnComputerProperties();
	afx_msg void OnUpdateComputerProperties(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
};



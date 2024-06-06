#pragma once

#include <deque>
#include "history.h"
#include "ContextMenu.h"

// CRemoteCombinedHistoryView view

class CRuleItem;
class CActiveRuleItem;
class COperationItem;
class CFloaterItem;
class CHistory;
class CRemoteCombinedDoc;
class CRemoteCombinedHistoryView : public CTemplateRemoteHistoryView
{
	DECLARE_DYNCREATE(CRemoteCombinedHistoryView)

public:
	CRemoteCombinedHistoryView();           // protected constructor used by dynamic creation
	virtual ~CRemoteCombinedHistoryView();
	virtual void OnInitialUpdate();				// called first time after construct
	void	InitialUpdate(CRemoteCombinedDoc* pDoc);

	bool	m_bDocumentIsClosing;


public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	bool	GetOperationFromQue();
	void	RulesCheckAll();
	void	RulesWallClock();
	void	RulesStatus();
	void	RulesConnection();
	void	RulesTimeLeft();
	bool	RulesTripped(CHistoryItem *pHistoryItem);
	bool	RulesCheck(CHistoryItem *pHistoryItem, CRuleItem *pRuleItem, CString *pTxt);
	bool	RuleCheck(int iRuleItem, CHistoryItem *pHistoryItem, CRuleItem *pRuleItem, CString *pTxt);	
	void	RulesActiveReset();
	void	RulesActiveAdd(CRuleItem *pRuleItem, CString *psTxt, bool bTimeNull, bool bProject);
	void	RulesActiveAdd(CHistoryItem *pHistoryItem, CRuleItem *pRuleItem, CString *psTxt, bool bTimeNul = false);
	void	RulesActiveRemove();
	void	RulesActiveEvent();

	void	SendWuRequest(int iNrRpc, bool bExcludeFromList);
	void	SendTThrottleRequest(int iNrTThrottleRpc);

	void	ClearFloaterList();
	void	TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore, char *pAfter);
	void	CheckWarning(CHistoryCountItem *pHistoryCountItem, CString *psComputerWarning, CString *psProjectWarning, int iWarningCpuTasks, int iWarningGpuTasks1, bool *pbWarning);
	void	Sort(CListCtrl *plistCtrl);

	bool	m_bThreadBusy;
	int		m_iThreadLocked;
	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;
	int		m_iAddedRpcCount;

	int		m_iHistoryStateReadyCount;


	CHistory		*m_pHistory;

	int		m_iTimeLeftLowest;
	int		m_iTimeLeftLowestPrevious;

	COperationItem *m_pOperationItemToThread;
	bool	m_bOperationsQueLocked;
	bool	m_bRuleTemperature;
	bool	m_bRuleWallClock;
	bool	m_bRuleStatus;
	bool	m_bRuleConnection;
	bool	m_bRuleTimeLeft;

public:

protected:
	bool	m_bTooLargeShown;

	CRemoteCombinedDoc	*m_pDoc;

	int					m_iColumnWidth[NUM_REMOTE_HISTORY_COLUMNS+1];		// for xchange only
	int					m_iNrColomns;

	std::deque<CFloaterItem*>  m_lFloater;
//	CList				<CFloaterItem*,CFloaterItem*> m_lFloater;
	CString				m_sTip;

//	HWND				m_hWndPreference;
	HWND				m_hWndProxy;

	bool				m_bRequestHistoryLong;
	HWND				m_hWndRequestHistoryLong;
	
	std::deque<CHistoryItem*> *m_plHistoryLong;

	CContextMenu		m_contextMenu;
	afx_msg LRESULT OnReadyProject(WPARAM parm1, LPARAM parm2);

	afx_msg void OnWww1();
	afx_msg void OnWww2();
	afx_msg void OnWww3();
	afx_msg void OnWww4();
	afx_msg void OnWww5();
	afx_msg void OnWww6();
	afx_msg void OnWww7();
	afx_msg void OnWww8();
	afx_msg void OnWww9();
	afx_msg void OnWww10();
	afx_msg void OnWww11();
	afx_msg void OnWww12();
	afx_msg void OnWww13();
	afx_msg void OnWww14();
	afx_msg void OnWww15();
	afx_msg void OnWww16();
	afx_msg void OnWww17();
	afx_msg void OnWww18();
	afx_msg void OnWww19();
	afx_msg void OnWww20();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg LRESULT OnOperationReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSettingsTasks(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnPreferenceDialog(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProxyDialog(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnSwitch(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSomethingHappened(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelTabView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerBarSelectionChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnTimerHistory(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRpcHistoryReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReadyRpcClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFloaterData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloudData(WPARAM wParam, LPARAM lParam);
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

	afx_msg LRESULT OnRequestLongHistory(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnSetProjectList(WPARAM wParam, LPARAM lParam);

	afx_msg void OnComputersDetect();

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) ;
	afx_msg void OnHeaderSetItems();

	afx_msg void OnCopyalltoclipboard();
	afx_msg void OnCopyselectedtoclipboard();
};



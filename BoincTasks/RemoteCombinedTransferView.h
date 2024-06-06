#pragma once

//#include "listviewex.h"



class CThreadRpc;


// CRemoteCombinedTransferView view

class CThreadRpc;
class CRemoteCombinedDoc;
class CRemoteCombinedTransferView : public CTemplateRemoteTransferView
{
	DECLARE_DYNCREATE(CRemoteCombinedTransferView)

public:
	CRemoteCombinedTransferView();           // protected constructor used by dynamic creation
	virtual ~CRemoteCombinedTransferView();
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
//	void	ReadAllColumnWidth();
//	int		ReadColumnWidth(int iColumn, int iDefault);
//	void	CheckColumnWidth();

//	void	DisplayRpc(CThreadRpc *pThreadRpc, int iNrTransfer, int *piRow, CString *psComputerId);

	void	PostTransferOperation(int iOperation);

	CString			m_sSelectedTransferItemsComputer[MAX_TRANSFER];
	CString			m_sSelectedTransferItemsName[MAX_TRANSFER];
	int				m_iSelectedTransferItems;

	int				m_iNrColomns;

	int				m_iThreadLocked;
	bool			m_bThreadBusy;
	bool			m_bFirstTime;

//	HWND			m_hWndPreference;
	HWND			m_hWndProxy;

	CRemoteCombinedDoc	*m_pDoc;

//	CString				m_sId;
	CContextMenu m_contextMenu;

//	int		m_iColumnWidth[NUM_REMOTE_TRANSFER_COLUMNS+1];		// for xchange only

	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg LRESULT OnReadyRpc(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColumnChanged(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnTimerTransfer(WPARAM parm1, LPARAM parm2);

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
	afx_msg void OnHeaderSetItems();

	afx_msg LRESULT OnFontChangeAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontHasChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnCopyalltoclipboard();
	afx_msg void OnCopyselectedtoclipboard();

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
public:
	afx_msg void OnTransferRetry();
	afx_msg void OnTransferRetryall();
	afx_msg void OnTransferAbort();

	afx_msg LRESULT OnWebOperation(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebItemSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebSort(WPARAM wParam, LPARAM lParam);

	afx_msg void OnComputersDetect();
};



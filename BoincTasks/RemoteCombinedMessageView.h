#pragma once

//#include "RemoteDoc.h"

// CRemoteCombinedMessageView view
class CRpcThreadInfo;
class CThreadRpc;
class CRemoteCombinedDoc;
class CRemoteCombinedMessageView : public CTemplateRemoteMessageView
{
	friend class CRemoteCombinedDoc;
	DECLARE_DYNCREATE(CRemoteCombinedMessageView)

public:
//protected:	
	CRemoteCombinedMessageView();				// protected constructor used by dynamic creation
	virtual ~CRemoteCombinedMessageView();
	void	ClearProjectFilterList();
	virtual void OnInitialUpdate();				// called first time after construct

	void	InitialUpdate(CRemoteCombinedDoc* pDoc);

	void	NoSelection(void);

	bool	m_bDocumentIsClosing;

//	void	SetDocument(CRemoteDoc *pDoc);

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext & dc) const;
#endif
#endif

protected:
	int		m_iThreadLocked;
	bool	m_bThreadBusy;
	bool	m_bFirstTime;

	void	StartupDisplay();
//	bool	DisplayRpc(CThreadRpc *pThreadRpc, int iNrMessages, int iLastReadMessages, CString *psComputerId);

	CThreadRpc			*m_pThreadRpc;
	CRemoteCombinedDoc	*m_pDoc;

	CString				m_sId;

	int					m_iSelected;
	int					m_iThreadRpcId;
	bool				m_bThreadRpcInit;

//	HWND				m_hWndPreference;
	HWND				m_hWndProxy;

	int					m_iWaitingTurner;

	void				ComputerSelect(int iSelected);
	void				FilterProject(int iFilter);

	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg LRESULT OnReadyRpc(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnTimerDlg(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSettingsTasks(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSwitch(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColumnChanged(WPARAM parm1, LPARAM parm2);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

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
	afx_msg LRESULT OnMessagesInvalidate(WPARAM wParam, LPARAM lParam);

	afx_msg void OnComputersDetect();

	afx_msg LRESULT OnFontChangeAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontHasChanged(WPARAM wParam, LPARAM lParam);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClipBoardSelected();
	afx_msg void OnClipBoardAll();
	afx_msg void OnSearch();

	afx_msg void OnFilterProjectNone();
	afx_msg void OnUpdateFilterProjectNone(CCmdUI* pCmdUI);
	afx_msg void OnFilterProjectGeneral();
	afx_msg void OnUpdateFilterProjectGeneral(CCmdUI* pCmdUI);

	afx_msg void OnFilterProject0();
	afx_msg void OnFilterProject1();
	afx_msg void OnFilterProject2();
	afx_msg void OnFilterProject3();
	afx_msg void OnFilterProject4();
	afx_msg void OnFilterProject5();
	afx_msg void OnFilterProject6();
	afx_msg void OnFilterProject7();
	afx_msg void OnFilterProject8();
	afx_msg void OnFilterProject9();
	afx_msg void OnFilterProject10();
	afx_msg void OnFilterProject11();
	afx_msg void OnFilterProject12();
	afx_msg void OnFilterProject13();
	afx_msg void OnFilterProject14();
	afx_msg void OnFilterProject15();
	afx_msg void OnFilterProject16();
	afx_msg void OnFilterProject17();
	afx_msg void OnFilterProject18();
	afx_msg void OnFilterProject19();
	afx_msg void OnFilterProject20();
	afx_msg void OnFilterProject21();
	afx_msg void OnFilterProject22();
	afx_msg void OnFilterProject23();
	afx_msg void OnFilterProject24();
	afx_msg void OnFilterProject25();
	afx_msg void OnFilterProject26();
	afx_msg void OnFilterProject27();
	afx_msg void OnFilterProject28();
	afx_msg void OnFilterProject29();
	afx_msg void OnFilterProject30();
	afx_msg void OnFilterProject31();
	afx_msg void OnFilterProject32();
	afx_msg void OnFilterProject33();
	afx_msg void OnFilterProject34();
	afx_msg void OnFilterProject35();
	afx_msg void OnFilterProject36();
	afx_msg void OnFilterProject37();
	afx_msg void OnFilterProject38();
	afx_msg void OnFilterProject39();
	afx_msg void OnFilterProject40();
	afx_msg void OnFilterProject41();
	afx_msg void OnFilterProject42();
	afx_msg void OnFilterProject43();
	afx_msg void OnFilterProject44();
	afx_msg void OnFilterProject45();
	afx_msg void OnFilterProject46();
	afx_msg void OnFilterProject47();
	afx_msg void OnFilterProject48();
	afx_msg void OnFilterProject49();
	afx_msg void OnFilterProject50();
	afx_msg void OnFilterProject51();
	afx_msg void OnFilterProject52();
	afx_msg void OnFilterProject53();
	afx_msg void OnFilterProject54();
	afx_msg void OnFilterProject55();
	afx_msg void OnFilterProject56();
	afx_msg void OnFilterProject57();
	afx_msg void OnFilterProject58();
	afx_msg void OnFilterProject59();
	afx_msg void OnFilterProject60();
	afx_msg void OnFilterProject61();
	afx_msg void OnFilterProject62();
	afx_msg void OnFilterProject63();
	afx_msg void OnFilterProject64();
	afx_msg void OnFilterProject65();
	afx_msg void OnFilterProject66();
	afx_msg void OnFilterProject67();
	afx_msg void OnFilterProject68();
	afx_msg void OnFilterProject69();
	afx_msg void OnFilterProject70();
	afx_msg void OnFilterProject71();
	afx_msg void OnFilterProject72();
	afx_msg void OnFilterProject73();
	afx_msg void OnFilterProject74();
	afx_msg void OnFilterProject75();
	afx_msg void OnFilterProject76();
	afx_msg void OnFilterProject77();
	afx_msg void OnFilterProject78();
	afx_msg void OnFilterProject79();
	
	afx_msg void OnComputerSelected0();
	afx_msg void OnComputerSelected1();
	afx_msg void OnComputerSelected2();
	afx_msg void OnComputerSelected3();
	afx_msg void OnComputerSelected4();
	afx_msg void OnComputerSelected5();
	afx_msg void OnComputerSelected6();
	afx_msg void OnComputerSelected7();
	afx_msg void OnComputerSelected8();
	afx_msg void OnComputerSelected9();
	afx_msg void OnComputerSelected10();
	afx_msg void OnComputerSelected11();
	afx_msg void OnComputerSelected12();
	afx_msg void OnComputerSelected13();
	afx_msg void OnComputerSelected14();
	afx_msg void OnComputerSelected15();
	afx_msg void OnComputerSelected16();
	afx_msg void OnComputerSelected17();
	afx_msg void OnComputerSelected18();
	afx_msg void OnComputerSelected19();
	afx_msg void OnComputerSelected20();
	DECLARE_MESSAGE_MAP()
};



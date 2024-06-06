#pragma once

#include "DlgUpdate.h"



#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 



// CRemoteCombinedNoticesView html view

class CRemoteCombinedDoc;
class CRemoteCombinedNoticesView : public CHtmlView
{
	DECLARE_DYNCREATE(CRemoteCombinedNoticesView)

public:
	CRemoteCombinedNoticesView();           // protected constructor used by dynamic creation
	virtual ~CRemoteCombinedNoticesView();
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void	Destroy(void);
	virtual HRESULT OnTranslateUrl( DWORD dwTranslate,  OLECHAR* pchURLIn,  OLECHAR** ppchURLOut );
	virtual void OnInitialUpdate();		
	void	InitialUpdate(CRemoteCombinedDoc* pDoc);
	void	NavigateHtml(CString *psHtml);

	bool	m_bDocumentIsClosing;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void	SendLogging(char *pcFrom, char *pcMessage, int iDebug);

	CRemoteCombinedDoc	*m_pDoc;

	afx_msg void OnSize(UINT nType, int cx, int cy );

	afx_msg LRESULT OnSwitch(WPARAM parm1, LPARAM parm2);

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

	afx_msg LRESULT OnFontChangeAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontHasChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnComputersDetect();
};



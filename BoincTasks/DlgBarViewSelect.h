#pragma once
#include "afxcmn.h"
#include "TabCtrlView.h"

#define BAR_SELECTED_COMPUTERS	0
#define BAR_SELECTED_PROJECTS	1
#define BAR_SELECTED_TASKS		2
#define BAR_SELECTED_TRANSFERS	3
#define BAR_SELECTED_MESSAGES	4
#define BAR_SELECTED_HISTORY	5
#define BAR_SELECTED_NOTICES	6

// DlgBarViewSelect dialog

class CDlgBarViewSelect : public CDialogBar
{

public:
	CDlgBarViewSelect();   // standard constructor
//	DlgBarViewSelect(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID);
	virtual ~CDlgBarViewSelect();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR_TABS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnInitDialog ( WPARAM, LPARAM );
	afx_msg LRESULT OnSelChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnBarViewOpen(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBarViewClose(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnBarViewSetTab(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBarViewSetNoticedCount(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnKey(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	BOOL	m_bEnable;
	int		m_iOpenWindows;

	int		m_iNoticesCount;

	//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	void	OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrlView m_tab;

	CImageList m_imageList;



};

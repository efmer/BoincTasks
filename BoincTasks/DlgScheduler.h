#pragma once
#include "afxcmn.h"


// CDlgScheduler dialog

#include "WndScheduler.h"

class CDlgScheduler : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScheduler)

public:
	CDlgScheduler(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgScheduler();
	void	 SafeWindowPlacement();
	void	Init(CWnd *pWnd, std::deque<CRuleIntervalItem *> *plInterval);

	std::deque<CRuleIntervalItem *>		*m_plInterval;

	CWndScheduler m_window;

// Dialog Data
	enum { IDD = IDD_DIALOG_ADVANCED_SCHEDULER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void  OnMouseMove(UINT, CPoint);

	HICON		m_hIcon;

	int			m_iWindowInitialLeftMargin;
	int			m_iWindowInitialTopMargin;
	LONG		m_lLastWidth;
	LONG		m_lLastHeight;
	bool		m_bInvalidate;

	CWnd		*m_pWndOwner;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
};

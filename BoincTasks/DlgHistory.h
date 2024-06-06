#pragma once
#include "afxcmn.h"
#include "ListCtrlHistory.h"
#include "ColorListBox.h"
#include "afxwin.h"
#include "afxdtctl.h"

#define MAX_HISTORY_DLG_TIMER 60

// CDlgHistory dialog

class CRemoteCombinedDoc;

class CDlgHistory : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHistory)

public:
	CDlgHistory(CWnd* pParent = NULL);   // standard constructor

	virtual ~CDlgHistory();

	void InitialUpdate(CRemoteCombinedDoc *pDoc);
	void SafeWindowPlacement();

// Dialog Data
	enum { IDD = IDD_DIALOG_HISTORY };

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void ClearHistory(std::deque<CHistoryItem*> *plHistoryLong);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
	void RestoreWindowPlacement();
	int  ReadColumnWidth(int iColumn, int iDefault);
	void SafeColumnWidth();
	void AddAllItems();
	void AddItem(CHistoryItem *pHistoryItem, int iPosAdd);
	void ClipBoard(bool bAll);

	HICON	m_hIcon;
	bool	m_bUpdateWindow;

	int		m_iTimer;
	int		m_iMaxTimer;
	bool	m_bThreadLocked;
	bool	m_bDraw;

	int		m_iWindowInitialLeftMargin;
	int		m_iWindowInitialTopMargin;

	int		m_iColumnWidth[NUM_REMOTE_HISTORY_COLUMNS+1];

	CRemoteCombinedDoc *m_pDoc;

	std::deque<CHistoryItem*> *m_plHistoryLong;
	std::deque<CHistoryItem*> *m_plHistoryLongFetch;


	afx_msg LRESULT OnLongHistory(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnChangedFont(WPARAM wParam,LPARAM lParam);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlHistory m_list;
	afx_msg void OnLvnColumnclickListHistory(NMHDR *pNMHDR, LRESULT *pResult);
	CColorListBox m_listComputers;
	CColorListBox m_listProjects;
	CButton m_checkSingleMultiple;
	afx_msg void OnBnClickedCheckGraphicSingleMultiple();
	afx_msg void OnLbnSelchangeListHistoryComputers();
	afx_msg void OnLbnSelchangeListHistoryProjects();
	CDateTimeCtrl m_dateTimeFrom;
	CDateTimeCtrl m_dateTimeTo;
	afx_msg void OnDtnDatetimechangeHistoryDatetimeFrom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeHistoryDatetimeTo(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClipBoardAll();
	afx_msg void OnClipBoardSelected();
	afx_msg void OnNMRClickListHistory(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_textComputers;
	CStatic m_textProjects;
};

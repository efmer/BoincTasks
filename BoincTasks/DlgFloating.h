#pragma once

#include "ListCtrlFloater.h"

#define DEFAULT_LABEL_LEFT		400
#define DEFAULT_LABEL_TOP		400
#define DEFAULT_LABEL_WIDTH		620
#define DEFAULT_LABEL_HEIGHT	200

#define FLOATER_COLUMNS			4

#define FLOATER_TOOLTIP_INFO	0
#define FLOATER_FULL_INFO		1


#define FLOATER_MODE_UNDEFINED	-1
// numbers must bue the same as the items in the settings->Gadget dialog
#define FLOATER_MODE_TASKS		0
#define FLOATER_MODE_COMPUTERS	1

// CDlgFloating dialog

class CDlgFloating : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFloating)

public:
	CDlgFloating(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFloating();
	virtual BOOL OnInitDialog();
	virtual void OnClose();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
// Dialog Data
	enum { IDD = IDD_DIALOG_FLOATING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			void AddColumnsTasks();
			void AddColumnsComputers();
			void Refresh();
			void GetPlacement();
			void SavePlacement(void);

			int ReadColumnWidth(int iColumn, int iDefault, int iMinimum);
			void CheckAndSaveColumnsWidth();

			void SetMode(int iNewMode);

	int		m_iColumnWidth[FLOATER_COLUMNS+1];

	int		m_iStartupTimer;
	int		m_iStartupTry;
	int		m_iTimer;
	bool	m_bUpdateWindow;
	CListCtrlFloater m_list;
	int		m_iMoveTimer;
	bool	m_bOnTop;

	bool	m_bRefreshBusy;
	int		m_iRefreshBusy;
	int		m_iRefreshTime;

	int		m_iStayOnTop;

	bool	m_bMoved;

	int		m_iRadio;

	int		m_iMode;

	CFont	*m_pFont;


	time_t	tLastRequest;

	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnShowWindow(BOOL bShow,  UINT nStatus);

	afx_msg LRESULT OnGetFloaterData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetFloaterDataReady(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
//	afx_msg LRESULT OnOverIcon(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetFloater(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetFloaterMode(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnChangedFont(WPARAM wParam,LPARAM lParam);


};

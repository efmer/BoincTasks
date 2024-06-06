#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "ListCtrlFloater.h"

#define MAX_PROPERTIES_COLUMNS 2

// CDlgProperties dialog

class CDlgProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgProperties)

public:
	CDlgProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProperties();
	void	SafeWindowPlacement();
	void	RestoreWindowPlacement();
	void	SafeColumn();


// Dialog Data
	enum { IDD = IDD_DIALOG_PROPERTIES };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	void SetText(CString *psTxt);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
	void ClipBoard(bool bAll);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClipBoardAll();

	HICON	m_hIcon;
	bool	m_bUpdateWindow;

	CString	m_sRegistryPrefix;

	CFont	*m_pFont;

	afx_msg LRESULT OnSetup(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAddRow(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnChangedFont(WPARAM wParam,LPARAM lParam);

public:
	CListCtrl m_listProperties;
};

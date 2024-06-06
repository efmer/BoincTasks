#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "listviewex.h"
#include "selection.h"


#define NUM_YESNO_COLUMNS 6

class CColumnTextAdd
{
public:
	CString m_sTxt[NUM_YESNO_COLUMNS+1];
};

// CDlgYesNo dialog

class CDlgYesNo : public CDialog
{
	DECLARE_DYNAMIC(CDlgYesNo)

public:
	CDlgYesNo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgYesNo();
	void	AddColumns();
	void	AddColumn(CString sTxt1, CString sTxt2="", CString sTxt3="", CString sTxt4="", CString sTxt5="", CString sTxt6="");
	int		ReadColumnWidth(int iColumn, int iDefault);
	void	SafeColumnWidth();

	CString m_sTxt;

	CString		m_sColumn[NUM_YESNO_COLUMNS+1];
	int			m_iColumnWidth[NUM_YESNO_COLUMNS+1];

	CString		m_sRegistryId;

	bool		bColumnMode;

// Dialog Data
	enum { IDD = IDD_DIALOG_YESNO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void	SafeWindowPlacement();
	void	AddColumnReal(int iPos, CString sTxt1, CString sTxt2, CString sTxt3, CString sTxt4, CString sTxt5, CString sTxt6);
	void	SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
//	int		m_iInsertPos;

//	CString m_sColumnText[MAX_SELECTED_PROPERTIES+10][NUM_YESNO_COLUMNS+1];

	std::deque<CColumnTextAdd*> m_lColumnText;

	bool	m_bUpdateWindow;
	int		m_iWindowInitialBottomMargin;

	int		m_iWindowInitialYesNoButton;
	int		m_iWidthYesNoButton;
	int		m_iHeightYesNoButton;

	HICON	m_hIcon;

	CFont	*m_pFont;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedYes();
	CEdit m_text;
	CListCtrl m_list;
	afx_msg void OnLvnItemchangedListYesno(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);

	afx_msg LRESULT OnChangedFont(WPARAM wParam,LPARAM lParam);

	CButton m_buttonYes;
	CButton m_buttonNo;
};

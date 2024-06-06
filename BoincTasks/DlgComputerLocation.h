#pragma once
#include "afxcmn.h"
#include "ListCtrlEdit.h"

// CDlgComputerLocation dialog

#define MAX_COMPUTER_LOCATION_COLUMNS	2

class CDlgComputerLocation : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgComputerLocation)

public:
	CDlgComputerLocation(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgComputerLocation();

	void	SafeWindowPlacement();
	void	SafeColumn();


// Dialog Data
	enum { IDD = IDD_DIALOG_COMPUTER_LOCATION };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int		ReadXml(void);
	void	WriteXml(void);
	bool	CheckDouble(CString *psText);
	void	WriteLocationList();
	void	RenameFile(CString sNow, CString sOld);
	void	SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
	void	RestoreWindowPlacement();

	CFont	*m_pFont;

	HICON	m_hIcon;
	bool	m_bUpdateWindow;

	bool	m_bListChanged;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);

	afx_msg LRESULT OnChangedFont(WPARAM wParam,LPARAM lParam);

	afx_msg void OnBnClickedComputerLocationAdd();
	afx_msg void OnBnClickedComputerLocationRemove();
	CListCtrlEdit m_listLocations;
	afx_msg void OnLvnItemchangedListLocations(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnSetup(WPARAM parm1, LPARAM parm2);
	afx_msg void OnBnClickedCancel();
};

#pragma once

#include "ListCtrlComputer.h"
#include "DlgBarSide.h"



#define BAR_COMPUTER_MAKE_ROOM	400

class	CComputerSetItem
{
public:
	CString		m_sGroup;
	CString		m_sComputer;
	bool		m_bSet;
};

// CDlgBarComputer dialog

class CDlgBarComputer : public CDlgBarSide
{

public:
	CDlgBarComputer();   // standard constructor

//	CDlgBarComputer(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID, BOOL = TRUE);
	virtual ~CDlgBarComputer();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );



	void	AddComputer(CString sGroup, CString sComputerId);
	void	AddComputerComplete(void);
	void	SelectAllComputer(void);
	int		SelectGroup(int iGroup);
	void	SelectAllComputersInGroup(int iGroup);
	void	CheckGroupSelected(void);
	void	CheckAllItemsSelected(void);
	void	CheckGroupExpanded(CString *psGroup);

	CString GetComputerSelected(void);

	CSize m_sizeFloating;
	BOOL m_bChangeDockedSize;   // Indicates whether to keep a default size for docking

	bool	m_bWebSingleSelection;

// Dialog Data
	enum { IDD = IDD_DIALOGBAR_COMPUTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			void ResetScaling();
	afx_msg LRESULT OnInitDialog ( WPARAM, LPARAM );
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnCheckComputerSelected ( WPARAM, LPARAM );
	afx_msg LRESULT OnGetComputerSelected ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerConnected ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerSelect ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectAllComputers ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerTree ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComputerGetTree ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewSize ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickedOnIcon ( WPARAM wParam, LPARAM lParam);


	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	CImageList			m_imageList;


	bool	m_bSizingEnded;
	int		m_iNoLongerSizing;
	
//	int		m_iDockedWidth;
//	int		m_iDockedHeight;

	bool	m_bMouseKeyReleased;
	bool	m_bMouseKeyPressedSecondTime;

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLbnSelchangeListBarComputer();
//	afx_msg void OnLvnItemchangedListBarComputers(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnOdstatechangedListBarComputers(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnItemActivateListBarComputers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListBarComputers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListBarComputers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

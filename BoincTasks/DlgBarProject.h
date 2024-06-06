#pragma once

#include "ListCtrlComputer.h"
#include "DlgBarSide.h"


#define BAR_PROJECT_MAKE_ROOM	400

class CProjectBarList
{
public:
	std::string	m_sProjectName;
	std::string	m_sProjectUrl;
	bool		m_bFound;
};

// CDlgBarProject dialog

class CDlgBarProject : public CDlgBarSide
{

public:
	CDlgBarProject();   // standard constructor

	virtual BOOL Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID, BOOL = TRUE);
	virtual ~CDlgBarProject();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );

	void	DeleteProjectList();

	void	AddProject(CString sGroup, CString sProjectId);
	void	AddProjectComplete(void);
	void	SelectAllProject(void);
	int		SelectGroup(int iGroup);
	void	SelectAllProjectsInGroup(int iGroup);
	void	CheckGroupSelected(void);
	void	CheckAllItemsSelected(void);
	void	CheckGroupExpanded(CString *psGroup);
	void	ComputerSelected(CString *psComputer, int iState);
	void	RequestProjectList();
	void	SendSelectedProjectList();

	CSize m_sizeFloating;
	BOOL m_bChangeDockedSize;   // Indicates whether to keep a default size for docking

	bool	m_bWebSingleSelection;

// Dialog Data
	enum { IDD = IDD_DIALOGBAR_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			void ResetScaling();

	afx_msg LRESULT OnThreadProjectList(WPARAM, LPARAM);
	afx_msg LRESULT OnInitDialog(WPARAM, LPARAM);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNewSize ( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickedOnIcon ( WPARAM wParam, LPARAM lParam);


	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);


	CImageList			m_imageList;

	bool				m_bThreadBusy;
	int					m_iThreadCount;
	int					m_iProjectRefresh;
	int					m_iProjectRefreshInterval;
	bool				m_bSelectAllProjects;

	bool				m_bAddProjects;

	std::string			m_sProjectsSelectedUrl;
	std::string			m_sProjectsSelected;

	std::deque<CProjectBarList *> m_lProjects;
	int					m_iSendRefresh;
	bool				m_bSendRefresh;

	bool	m_bSizingEnded;
	
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
	afx_msg void OnNMClickListBarProjects(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListBarComputers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

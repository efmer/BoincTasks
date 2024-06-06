#pragma once
#include "ListCtrlComputer.h"

#define SIDEBAR_COMPUTER 0
#define SIDEBAR_PROJECT 0

#define BAR_COMPUTER_TOP_BORDER 10
#define BAR_COMPUTER_BOTTOM_BORDER 4
#define BAR_COMPUTER_LEFT_BORDER 2
#define BAR_COMPUTER_RIGHT_BORDER 2

#define BAR_TREE_GROUP		0
#define BAR_TREE_ITEM		1


class	CBarTreeItem
{
public:
	CString		m_sComputer;
	int			m_iImage;
	int			m_iState;
};

class	CBarTreeGroupItem
{
public:
	CString		m_sGroup;
	bool		m_bExpand;
	bool		m_bSelected;
	std::deque<CBarTreeItem*>	m_lList;
};

// CDlgBarSide

class CDlgBarSide : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarSide)

public:
	CDlgBarSide();
	virtual ~CDlgBarSide();

	void	Init(int iTypeSidebar);
	void	ScaleList(CSize size);

	void	ClickedOnIcon(int iItem);
	void	RemoveTree(void);
	void	MakeTree();
	void	Sort(void);

	CSize	m_sizeDocked;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnMoving(UINT nSide, LPRECT lpRect);

	bool	m_bInvalidate;

	int		m_iTypeSideBar;
	bool	m_bSizing;
	int		m_iMakeRoom;
	int		m_iNoLongerSizing;

	CListCtrlComputer	m_list;

	std::deque<CBarTreeGroupItem*>	m_lListBarTree;
};



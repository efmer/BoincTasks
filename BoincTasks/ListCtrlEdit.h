#pragma once


//Inheriting CEdit class.
class cMyEdit: public CEdit
{
public:
	int m_x;
	int m_y;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

// CListCtrlEdit

class CListCtrlEdit : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEdit)

public:
	CListCtrlEdit();
	virtual ~CListCtrlEdit();

	//This CEdit is placed on the top of the subItem whenever editing is        required.
	cMyEdit m_editWnd;
	//It contains the current Item of which subItem is being edited.
	int m_item;
	//This is the subItem that is being edited.
	int m_subitem;

	//Total Events that has to be overridden.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT    *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();


protected:
	DECLARE_MESSAGE_MAP()
};



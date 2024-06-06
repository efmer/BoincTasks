#pragma once


// CTextBox

class CTextBox : public CEdit
{
	DECLARE_DYNAMIC(CTextBox)

public:
	CTextBox();
	virtual ~CTextBox();

protected:

	int			m_iHeight;
	int			m_iWidth;
	COLORREF	m_crText;

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg LRESULT OnNewSize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewColor(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};



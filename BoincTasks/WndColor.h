#pragma once

// CWndColor

class CWndColor : public CWnd
{
	DECLARE_DYNAMIC(CWndColor)

public:
	CWndColor();
	virtual ~CWndColor();
	void SetText(CString sTxt, int iFont);
	void SetColor(COLORREF color);

protected:

	COLORREF m_color;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

};



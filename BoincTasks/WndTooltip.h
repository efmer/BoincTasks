#pragma once


// CWndTooltip

class CWndTooltip : public CWnd
{
	DECLARE_DYNAMIC(CWndTooltip)

public:
	CWndTooltip();
	virtual ~CWndTooltip();
	void SetText(CString sTxt);
	CSize GetTextSize();

	CSize	m_sizeText;

protected:
	DECLARE_MESSAGE_MAP()

	CString m_sTxt;

	afx_msg void OnPaint();
};



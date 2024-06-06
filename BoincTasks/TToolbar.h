#pragma once


// CTToolbar

class CTToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CTToolBar)

public:
	CTToolBar();
	virtual ~CTToolBar();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
	DECLARE_MESSAGE_MAP()
};



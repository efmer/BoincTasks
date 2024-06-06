#pragma once


// CSkinHeader

class CSkinHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CSkinHeaderCtrl)

public:
	CSkinHeaderCtrl();
	virtual ~CSkinHeaderCtrl();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	CString m_sFont;


//	afx_msg void OnPaint();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};



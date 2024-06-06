#pragma once

#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"
//#include "Label.h"

#define COLOR_GENERAL_GRID			RGB(100,100,100)
#define COLOR_GENERAL_FONT			RGB(000,000,000)
#define COLOR_GENERAL_BAR			RGB(96,196,249)
#define COLOR_GENERAL_BAR_OUTLINE	RGB(128,128,128)



#define COLOR_GENERAL_ARRAY_GRID				0
#define COLOR_GENERAL_ARRAY_FONT				1
#define COLOR_GENERAL_ARRAY_FONT_SELECTED		2
#define COLOR_GENERAL_ARRAY_BACK_SELECTED		3
#define COLOR_GENERAL_ARRAY_BAR					4
#define COLOR_GENERAL_ARRAY_BAR_OUTLINE			5

#define COLOR_ARRAY_NUMBER_GENERAL				6

// CDlgColorGeneral dialog

class CDlgColorGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorGeneral)

public:
	CDlgColorGeneral();
	virtual ~CDlgColorGeneral();
	virtual BOOL OnInitDialog();

	COLORREF *GetColorArray();
	void	SaveRegistry();

// Dialog Data
	enum { IDD = IDD_DIALOG_COLOR_GENERAL };

protected:
	COLORREF	Correct(COLORREF color);


	bool		m_bColorWindowUpdate;
	CWndColor	m_wndGeneral[COLOR_ARRAY_NUMBER_GENERAL+1];
	COLORREF	m_colorGeneral[COLOR_ARRAY_NUMBER_GENERAL+1];

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonColorGrid();
	afx_msg void OnBnClickedButtonColorFont();

	CButton m_buttonGrid;
	CButton m_buttonFont;
	CButton m_buttonFontSelected;
	CButton m_buttonBackSelected;
	CButton m_buttonBar;
	CButton m_buttonBarOutline;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonColorBar();
	afx_msg void OnBnClickedButtonColorBarOutline();
	afx_msg void OnBnClickedButtonColorFontSelected();
	afx_msg void OnBnClickedButtonColorBackSelected();


	CButton m_buttonOk;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

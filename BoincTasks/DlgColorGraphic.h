#pragma once
#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"

#define COLOR_G_BACKGROUND		RGB(206,227,251)
#define COLOR_G_PROJECT_TEXT	RGB(000,000,000)
#define COLOR_G_SCALE_TEXT		RGB(000,000,000)
#define COLOR_G_GRID			RGB(000,100,000)
#define COLOR_G_GRID_FINE		RGB(200,200,200)
#define COLOR_G_AVERAGE			RGB(84 ,175,237)

#define COLOR_G_COMPUTER1		RGB(255,000,000)
#define COLOR_G_COMPUTER2		RGB(000,255,000)
#define COLOR_G_COMPUTER3		RGB(000,000,255)
#define COLOR_G_COMPUTER4		RGB(255,000,000)
//l geel
#define COLOR_G_COMPUTER5		RGB(255,255,128)
//l blauw
#define COLOR_G_COMPUTER6		RGB(128,255,255)
//l paars
#define COLOR_G_COMPUTER7		RGB(255,128,255)
//l grijs
#define COLOR_G_COMPUTER8		RGB(192,192,192)
//oranje
#define COLOR_G_COMPUTER9		RGB(255,128,0)
//d blauw
#define COLOR_G_COMPUTER10		RGB(0,255,255)
//d paars
#define COLOR_G_COMPUTER11		RGB(128,0,255)

#define COLOR_G_COMPUTER12		RGB(000,000,000)
#define COLOR_G_COMPUTER13		RGB(000,000,000)
#define COLOR_G_COMPUTER14		RGB(000,000,000)
#define COLOR_G_COMPUTER15		RGB(000,000,000)
#define COLOR_G_COMPUTER16		RGB(000,000,000)
#define COLOR_G_COMPUTER17		RGB(000,000,000)
#define COLOR_G_COMPUTER18		RGB(000,000,000)
#define COLOR_G_COMPUTER19		RGB(000,000,000)
#define COLOR_G_COMPUTER20		RGB(000,000,000)


#define COLOR_ARRAY_G_NUMBER_COMPUTERS 20
#define COLOR_ARRAY_G_NUMBER COLOR_ARRAY_G_NUMBER_COMPUTERS+6

#define COLOR_ARRAY_BACKGROUND		0
#define COLOR_ARRAY_PROJECT_TEXT	1
#define COLOR_ARRAY_SCALE_TEXT		2
#define COLOR_ARRAY_GRID			3
#define COLOR_ARRAY_GRID_FINE		4
#define COLOR_ARRAY_AVERAGE			5
#define COLOR_ARRAY_COMPUTER1		6
#define COLOR_ARRAY_COMPUTER2		7
#define COLOR_ARRAY_COMPUTER3		8
#define COLOR_ARRAY_COMPUTER4		9
#define COLOR_ARRAY_COMPUTER5		10
#define COLOR_ARRAY_COMPUTER6		11
#define COLOR_ARRAY_COMPUTER7		12
#define COLOR_ARRAY_COMPUTER8		13
#define COLOR_ARRAY_COMPUTER9		14
#define COLOR_ARRAY_COMPUTER10		15
#define COLOR_ARRAY_COMPUTER11		16
#define COLOR_ARRAY_COMPUTER12		17
#define COLOR_ARRAY_COMPUTER13		18
#define COLOR_ARRAY_COMPUTER14		19
#define COLOR_ARRAY_COMPUTER15		20
#define COLOR_ARRAY_COMPUTER16		21
#define COLOR_ARRAY_COMPUTER17		22
#define COLOR_ARRAY_COMPUTER18		23
#define COLOR_ARRAY_COMPUTER19		24
#define COLOR_ARRAY_COMPUTER20		25

// CDlgColorGraphic dialog

class CDlgColorGraphic : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorGraphic)

public:
//	CDlgColorGraphic(CWnd* pParent = NULL);   // standard constructor
	CDlgColorGraphic();
	virtual ~CDlgColorGraphic();
	virtual BOOL OnInitDialog();

	COLORREF GetColor(int iPos);
	COLORREF GetColorComputer(int iPos, int *piRepeat);
	COLORREF *GetColorArray();
	bool	GetFixedColors();
	void SaveRegistry();
// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPHIC_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool		m_bColorWindowUpdate;
	CWndColor	m_wnd[COLOR_ARRAY_G_NUMBER+1];
	COLORREF	m_color[COLOR_ARRAY_G_NUMBER+1];
	bool		m_bFixedColors;

	HICON	m_hIcon;

	HWND		m_hWndGraphic;
	HWND		m_hWndGraphicTasks;
	HWND		m_hWndGraphicDeadline;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnColorInit(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnColorInit2(WPARAM parm1, LPARAM parm2);
	afx_msg void OnBnClickedButtonGraphicBackground();
	afx_msg void OnBnClickedButtonGraphicProjectText();
	afx_msg void OnBnClickedButtonGraphicScaleText();
	afx_msg void OnBnClickedButtonGraphicGrid();
	afx_msg void OnBnClickedButtonGraphicComputer1();
	afx_msg void OnBnClickedButtonGraphicComputer2();
	afx_msg void OnBnClickedButtonGraphicComputer3();
	afx_msg void OnBnClickedButtonGraphicComputer4();
	afx_msg void OnBnClickedButtonGraphicComputer5();
	afx_msg void OnBnClickedButtonGraphicComputer6();
	afx_msg void OnBnClickedButtonGraphicComputer7();
	afx_msg void OnBnClickedButtonGraphicComputer8();
	afx_msg void OnBnClickedButtonGraphicComputer9();
	afx_msg void OnBnClickedButtonGraphicComputer10();
	afx_msg void OnBnClickedButtonGraphicComputer11();
	afx_msg void OnBnClickedButtonGraphicComputer12();
	afx_msg void OnBnClickedButtonGraphicComputer13();
	afx_msg void OnBnClickedButtonGraphicComputer14();
	afx_msg void OnBnClickedButtonGraphicComputer15();
	afx_msg void OnBnClickedButtonGraphicComputer16();
	afx_msg void OnBnClickedButtonGraphicComputer17();
	afx_msg void OnBnClickedButtonGraphicComputer18();
	afx_msg void OnBnClickedButtonGraphicComputer19();
	afx_msg void OnBnClickedButtonGraphicComputer20();
	CButton m_buttonBackground;
	CButton m_buttonProjectText;
	CButton m_buttonScaleText;
	CButton m_buttonGrid;
	CButton m_buttonGridFine;
	CButton m_buttonComputer[20];
	CButton m_buttonOk;
	afx_msg void OnBnClickedButtonGraphicGridFine();
	CButton m_buttonAverage;
	afx_msg void OnBnClickedButtonGraphicAverage();
	CButton m_checkFixedColors;
	afx_msg void OnBnClickedCheckGraphicFixed();
};

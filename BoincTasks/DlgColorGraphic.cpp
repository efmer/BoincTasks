// DlgGraphicColor.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorGraphic.h"
#include "Translation.h"


// CDlgColorGraphic dialog

IMPLEMENT_DYNAMIC(CDlgColorGraphic, CPropertyPage)


CDlgColorGraphic::CDlgColorGraphic()
	: CPropertyPage(CDlgColorGraphic::IDD)
{

	m_hWndGraphic		= NULL;
	m_hWndGraphicTasks	= NULL;
	m_hWndGraphicDeadline	= NULL;
}

/*
CDlgColorGraphic::CDlgColorGraphic(CWnd* pParent )
	: CPropertyPage(CDlgColorGraphic::IDD, pParent)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
*/

CDlgColorGraphic::~CDlgColorGraphic()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
}

BOOL CDlgColorGraphic::OnInitDialog()
{
	CPropertySheet *pSheet;
	CString sFormat;
	int		iCheck;

	CPropertyPage::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogGraphicColorsTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonBackground.SetWindowText(gszTranslation[PosDialogGraphicColorsButtonBackground]);
	m_buttonProjectText.SetWindowText(gszTranslation[PosDialogGraphicColorsButtonProjectText]);
	m_buttonScaleText.SetWindowText(gszTranslation[PosDialogGraphicColorsButtonScaleText]);
	m_buttonGrid.SetWindowText(gszTranslation[PosDialogGraphicColorsButtonGrid]);
	m_buttonGridFine.SetWindowText(gszTranslation[PosDialogColorTemperatureGridFine]);
	m_buttonAverage.SetWindowText(gszTranslation[PosDialogGraphicColorsAverage]);
	
	m_checkFixedColors.SetWindowText(gszTranslation[PosDialogGraphicColorFixed]);

	for (int iCount = 0; iCount < 20; iCount++)
	{
		sFormat.Format(" %d", iCount+1);
		sFormat = gszTranslation[PosDialogGraphicColorsButtonColor] + sFormat;
		m_buttonComputer[iCount].SetWindowTextA(sFormat);
	}

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

// Translation

	m_color[COLOR_ARRAY_BACKGROUND]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorBackground, COLOR_G_BACKGROUND);
	m_color[COLOR_ARRAY_PROJECT_TEXT]	= theApp.GetProfileInt(registrySectionColorGraphic, registryColorProjectText, COLOR_G_PROJECT_TEXT);
	m_color[COLOR_ARRAY_SCALE_TEXT]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorScaleText, COLOR_G_SCALE_TEXT);
	m_color[COLOR_ARRAY_GRID]			= theApp.GetProfileInt(registrySectionColorGraphic, registryColorGrid, COLOR_G_GRID);
	m_color[COLOR_ARRAY_GRID_FINE]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorGridFine, COLOR_G_GRID_FINE);
	m_color[COLOR_ARRAY_AVERAGE]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorAverage, COLOR_G_AVERAGE);

	m_color[COLOR_ARRAY_COMPUTER1]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer1, COLOR_G_COMPUTER1);
	m_color[COLOR_ARRAY_COMPUTER2]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer2, COLOR_G_COMPUTER2);
	m_color[COLOR_ARRAY_COMPUTER3]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer3, COLOR_G_COMPUTER3);
	m_color[COLOR_ARRAY_COMPUTER4]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer4, COLOR_G_COMPUTER4);
	m_color[COLOR_ARRAY_COMPUTER5]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer5, COLOR_G_COMPUTER5);
	m_color[COLOR_ARRAY_COMPUTER6]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer6, COLOR_G_COMPUTER6);
	m_color[COLOR_ARRAY_COMPUTER7]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer7, COLOR_G_COMPUTER7);
	m_color[COLOR_ARRAY_COMPUTER8]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer8, COLOR_G_COMPUTER8);
	m_color[COLOR_ARRAY_COMPUTER9]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer9, COLOR_G_COMPUTER9);
	m_color[COLOR_ARRAY_COMPUTER10]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer10, COLOR_G_COMPUTER10);
	m_color[COLOR_ARRAY_COMPUTER11]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer11, COLOR_G_COMPUTER11);
	m_color[COLOR_ARRAY_COMPUTER12]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer12, COLOR_G_COMPUTER12);
	m_color[COLOR_ARRAY_COMPUTER13]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer13, COLOR_G_COMPUTER13);
	m_color[COLOR_ARRAY_COMPUTER14]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer14, COLOR_G_COMPUTER14);
	m_color[COLOR_ARRAY_COMPUTER15]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer15, COLOR_G_COMPUTER15);
	m_color[COLOR_ARRAY_COMPUTER16]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer16, COLOR_G_COMPUTER16);
	m_color[COLOR_ARRAY_COMPUTER17]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer17, COLOR_G_COMPUTER17);
	m_color[COLOR_ARRAY_COMPUTER18]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer18, COLOR_G_COMPUTER18);
	m_color[COLOR_ARRAY_COMPUTER19]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer19, COLOR_G_COMPUTER19);
	m_color[COLOR_ARRAY_COMPUTER20]		= theApp.GetProfileInt(registrySectionColorGraphic, registryColorComputer20, COLOR_G_COMPUTER20);

	m_bColorWindowUpdate= true;

	iCheck = theApp.GetProfileInt(registrySectionColorGraphic, registryColorFixed, 1);
	m_checkFixedColors.SetCheck(iCheck);
	m_bFixedColors = (iCheck != 0);

	SetTimer(UTIMER_DLG_COLOR_GRAPHIC, 1000, 0);	// interval 1 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

COLORREF CDlgColorGraphic::GetColor(int iPos)
{
	if (iPos >= COLOR_ARRAY_G_NUMBER) return 0;
	return m_color[iPos];
}

COLORREF CDlgColorGraphic::GetColorComputer(int iPos, int *piRepeat)
{
	*piRepeat = 0;

	while (iPos >= COLOR_ARRAY_G_NUMBER_COMPUTERS)
	{
		(*piRepeat)++;
		iPos -= COLOR_ARRAY_G_NUMBER_COMPUTERS;
	}
	return m_color[COLOR_ARRAY_COMPUTER1+iPos];
}

COLORREF *CDlgColorGraphic::GetColorArray()
{
	return &m_color[0];
}

bool CDlgColorGraphic::GetFixedColors()
{
	return m_bFixedColors;
}

void CDlgColorGraphic::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorBackground , m_color[COLOR_ARRAY_BACKGROUND]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorProjectText , m_color[COLOR_ARRAY_PROJECT_TEXT]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorScaleText , m_color[COLOR_ARRAY_SCALE_TEXT]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorGrid , m_color[COLOR_ARRAY_GRID]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorGridFine, m_color[COLOR_ARRAY_GRID_FINE]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorAverage, m_color[COLOR_ARRAY_AVERAGE]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer2 , m_color[COLOR_ARRAY_COMPUTER1]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer2 , m_color[COLOR_ARRAY_COMPUTER2]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer3 , m_color[COLOR_ARRAY_COMPUTER3]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer4 , m_color[COLOR_ARRAY_COMPUTER4]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer5 , m_color[COLOR_ARRAY_COMPUTER5]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer6 , m_color[COLOR_ARRAY_COMPUTER6]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer7 , m_color[COLOR_ARRAY_COMPUTER7]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer8 , m_color[COLOR_ARRAY_COMPUTER8]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer9 , m_color[COLOR_ARRAY_COMPUTER9]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer10 , m_color[COLOR_ARRAY_COMPUTER10]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer11 , m_color[COLOR_ARRAY_COMPUTER11]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer12 , m_color[COLOR_ARRAY_COMPUTER12]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer13 , m_color[COLOR_ARRAY_COMPUTER13]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer14 , m_color[COLOR_ARRAY_COMPUTER14]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer15 , m_color[COLOR_ARRAY_COMPUTER15]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer16 , m_color[COLOR_ARRAY_COMPUTER16]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer17 , m_color[COLOR_ARRAY_COMPUTER17]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer18 , m_color[COLOR_ARRAY_COMPUTER18]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer19 , m_color[COLOR_ARRAY_COMPUTER19]);
	theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer20 , m_color[COLOR_ARRAY_COMPUTER20]);

	m_bColorWindowUpdate= true;
}


void CDlgColorGraphic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_BACKGROUND, m_wnd[COLOR_ARRAY_BACKGROUND]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_TEXT		, m_wnd[COLOR_ARRAY_PROJECT_TEXT]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_SCALE_TEXT, m_wnd[COLOR_ARRAY_SCALE_TEXT]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_GRID		, m_wnd[COLOR_ARRAY_GRID]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_GRID_FINE, m_wnd[COLOR_ARRAY_GRID_FINE]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_AVERAGE,	  m_wnd[COLOR_ARRAY_AVERAGE]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER1, m_wnd[COLOR_ARRAY_COMPUTER1]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER2, m_wnd[COLOR_ARRAY_COMPUTER2]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER3, m_wnd[COLOR_ARRAY_COMPUTER3]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER4, m_wnd[COLOR_ARRAY_COMPUTER4]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER5, m_wnd[COLOR_ARRAY_COMPUTER5]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER6, m_wnd[COLOR_ARRAY_COMPUTER6]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER7, m_wnd[COLOR_ARRAY_COMPUTER7]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER8, m_wnd[COLOR_ARRAY_COMPUTER8]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER9, m_wnd[COLOR_ARRAY_COMPUTER9]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER10, m_wnd[COLOR_ARRAY_COMPUTER10]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER11, m_wnd[COLOR_ARRAY_COMPUTER11]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER12, m_wnd[COLOR_ARRAY_COMPUTER12]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER13, m_wnd[COLOR_ARRAY_COMPUTER13]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER14, m_wnd[COLOR_ARRAY_COMPUTER14]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER15, m_wnd[COLOR_ARRAY_COMPUTER15]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER16, m_wnd[COLOR_ARRAY_COMPUTER16]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER17, m_wnd[COLOR_ARRAY_COMPUTER17]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER18, m_wnd[COLOR_ARRAY_COMPUTER18]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER19, m_wnd[COLOR_ARRAY_COMPUTER19]);
	DDX_Control(pDX, IDC_GRAPHIC_COLOR_COMPUTER20, m_wnd[COLOR_ARRAY_COMPUTER20]);

	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER1, m_buttonComputer[0]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER2, m_buttonComputer[1]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER3, m_buttonComputer[2]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER4, m_buttonComputer[3]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER5, m_buttonComputer[4]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER6, m_buttonComputer[5]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER7, m_buttonComputer[6]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER8, m_buttonComputer[7]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER9, m_buttonComputer[8]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER10, m_buttonComputer[9]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER11, m_buttonComputer[10]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER12, m_buttonComputer[11]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER13, m_buttonComputer[12]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER14, m_buttonComputer[13]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER15, m_buttonComputer[14]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER16, m_buttonComputer[15]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER17, m_buttonComputer[16]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER18, m_buttonComputer[17]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER19, m_buttonComputer[18]);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_COMPUTER20, m_buttonComputer[19]);

	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_BACKGROUND, m_buttonBackground);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_PROJECT_TEXT, m_buttonProjectText);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_SCALE_TEXT, m_buttonScaleText);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_GRID, m_buttonGrid);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_GRID_FINE, m_buttonGridFine);
	DDX_Control(pDX, IDC_BUTTON_GRAPHIC_AVERAGE, m_buttonAverage);
	DDX_Control(pDX, IDOK, m_buttonOk);

	DDX_Control(pDX, IDC_CHECK_GRAPHIC_FIXED, m_checkFixedColors);
}


BEGIN_MESSAGE_MAP(CDlgColorGraphic, CPropertyPage)
	ON_WM_TIMER()
	ON_MESSAGE(UWM_GRAPHIC_COLOR_INIT,OnColorInit)
	ON_MESSAGE(UWM_GRAPHIC_COLOR_INIT2,OnColorInit2)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_BACKGROUND, &CDlgColorGraphic::OnBnClickedButtonGraphicBackground)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_PROJECT_TEXT, &CDlgColorGraphic::OnBnClickedButtonGraphicProjectText)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_SCALE_TEXT, &CDlgColorGraphic::OnBnClickedButtonGraphicScaleText)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_GRID, &CDlgColorGraphic::OnBnClickedButtonGraphicGrid)
	ON_BN_CLICKED(IDOK, &CDlgColorGraphic::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER1, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer1)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER2, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer2)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER3, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer3)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER4, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer4)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER5, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer5)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER6, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer6)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER7, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer7)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER8, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer8)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER9, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer9)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER10, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer10)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER11, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer11)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER12, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer12)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER13, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer13)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER14, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer14)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER15, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer15)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER16, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer16)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER17, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer17)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER18, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer18)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER19, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer19)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_COMPUTER20, &CDlgColorGraphic::OnBnClickedButtonGraphicComputer20)

	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_GRID_FINE, &CDlgColorGraphic::OnBnClickedButtonGraphicGridFine)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_AVERAGE, &CDlgColorGraphic::OnBnClickedButtonGraphicAverage)
	ON_BN_CLICKED(IDC_CHECK_GRAPHIC_FIXED, &CDlgColorGraphic::OnBnClickedCheckGraphicFixed)
END_MESSAGE_MAP()


// CDlgColorGraphic message handlers

LRESULT CDlgColorGraphic::OnColorInit(WPARAM parm1, LPARAM parm2)
{
	m_hWndGraphic = (HWND) parm1;
	m_hWndGraphicTasks = (HWND) parm2;

	return 0;
}

LRESULT CDlgColorGraphic::OnColorInit2(WPARAM parm1, LPARAM parm2)
{
	m_hWndGraphicDeadline = (HWND) parm1;
	return 0;
}

void CDlgColorGraphic::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bColorWindowUpdate)
	{
		m_bColorWindowUpdate = false;
		for (int iCount = 0; iCount < COLOR_ARRAY_G_NUMBER; iCount++)
		{
			m_wnd[iCount].SetColor(m_color[iCount]);
			m_wnd[iCount].Invalidate(TRUE);
		}

		::PostMessage(m_hWndGraphic, UWM_GRAPHIC_COLOR_CHANGED, 0, 0);
		::PostMessage(m_hWndGraphicTasks, UWM_GRAPHIC_COLOR_CHANGED, 0, 0);
		::PostMessage(m_hWndGraphicDeadline,  UWM_GRAPHIC_COLOR_CHANGED, 0, 0);
	}
}

void CDlgColorGraphic::OnBnClickedButtonGraphicBackground()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_BACKGROUND],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_BACKGROUND] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorBackground , m_color[COLOR_ARRAY_BACKGROUND]);
	}

}

void CDlgColorGraphic::OnBnClickedButtonGraphicProjectText()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_PROJECT_TEXT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_PROJECT_TEXT] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorProjectText , m_color[COLOR_ARRAY_PROJECT_TEXT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicScaleText()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_SCALE_TEXT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_SCALE_TEXT] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorScaleText , m_color[COLOR_ARRAY_SCALE_TEXT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicGrid()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_GRID],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_GRID] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorGrid , m_color[COLOR_ARRAY_GRID]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicGridFine()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_GRID_FINE],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_GRID_FINE] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorGridFine , m_color[COLOR_ARRAY_GRID_FINE]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorGraphic::OnBnClickedButtonGraphicAverage()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_AVERAGE],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_AVERAGE] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorAverage , m_color[COLOR_ARRAY_AVERAGE]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer1()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER1],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER1] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer1 , m_color[COLOR_ARRAY_COMPUTER1]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer2()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER2],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER2] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer2 , m_color[COLOR_ARRAY_COMPUTER2]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer3()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER3],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER3] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer3 , m_color[COLOR_ARRAY_COMPUTER3]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer4()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER4],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER4] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer4 , m_color[COLOR_ARRAY_COMPUTER4]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer5()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER5],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER5] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer5 , m_color[COLOR_ARRAY_COMPUTER5]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer6()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER6],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER6] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer6 , m_color[COLOR_ARRAY_COMPUTER6]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer7()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER7],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER7] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer7 , m_color[COLOR_ARRAY_COMPUTER7]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer8()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER8],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER8] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer8 , m_color[COLOR_ARRAY_COMPUTER8]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer9()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER9],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER9] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer9 , m_color[COLOR_ARRAY_COMPUTER9]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer10()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER10],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER10] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer10 , m_color[COLOR_ARRAY_COMPUTER10]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer11()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER11],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER11] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer11 , m_color[COLOR_ARRAY_COMPUTER11]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer12()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER12],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER12] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer12 , m_color[COLOR_ARRAY_COMPUTER12]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer13()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER13],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER13] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer13 , m_color[COLOR_ARRAY_COMPUTER13]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer14()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER14],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER14] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer14 , m_color[COLOR_ARRAY_COMPUTER14]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer15()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER15],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER15] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer15 , m_color[COLOR_ARRAY_COMPUTER15]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer16()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER16],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER16] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer16 , m_color[COLOR_ARRAY_COMPUTER16]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer17()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER17],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER17] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer17 , m_color[COLOR_ARRAY_COMPUTER17]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer18()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER18],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER18] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer18 , m_color[COLOR_ARRAY_COMPUTER18]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer19()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER19],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER19] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer19 , m_color[COLOR_ARRAY_COMPUTER19]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedButtonGraphicComputer20()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_color[COLOR_ARRAY_COMPUTER20],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color[COLOR_ARRAY_COMPUTER20] = dlg.GetColor();
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColorGraphic,registryColorComputer20 , m_color[COLOR_ARRAY_COMPUTER20]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGraphic::OnBnClickedCheckGraphicFixed()
{
	int iCheck;

	iCheck = m_checkFixedColors.GetCheck();
	theApp.WriteProfileInt(registrySectionColorGraphic, registryColorFixed, iCheck);
	m_bFixedColors = (iCheck != 0);
}

void CDlgColorGraphic::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}
// DlgColorGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"

#include "DlgColorMain.h"
#include "DlgColorGeneral.h"
#include "Translation.h"

// CDlgColorGeneral dialog

IMPLEMENT_DYNAMIC(CDlgColorGeneral, CPropertyPage)


CDlgColorGeneral::CDlgColorGeneral()
	: CPropertyPage(CDlgColorGeneral::IDD)
{

}

CDlgColorGeneral::~CDlgColorGeneral()
{

}



void CDlgColorGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_COLOR_RASTER, m_wndGeneral[COLOR_GENERAL_ARRAY_GRID]);
	DDX_Control(pDX, IDC_WND_COLOR_FONT, m_wndGeneral[COLOR_GENERAL_ARRAY_FONT]);
	DDX_Control(pDX, IDC_WND_COLOR_FONT_SELECTED, m_wndGeneral[COLOR_GENERAL_ARRAY_FONT_SELECTED]);
	DDX_Control(pDX, IDC_WND_COLOR_BACK_SELECTED, m_wndGeneral[COLOR_GENERAL_ARRAY_BACK_SELECTED]);
	DDX_Control(pDX, IDC_WND_COLOR_BAR, m_wndGeneral[COLOR_GENERAL_ARRAY_BAR]);
	DDX_Control(pDX, IDC_WND_COLOR_BAR_OUTLINE, m_wndGeneral[COLOR_GENERAL_ARRAY_BAR_OUTLINE]);

	DDX_Control(pDX, IDC_BUTTON_COLOR_RASTER, m_buttonGrid);
	DDX_Control(pDX, IDC_BUTTON_COLOR_FONT, m_buttonFont);
	DDX_Control(pDX, IDC_BUTTON_COLOR_FONT_SELECTED, m_buttonFontSelected);
	DDX_Control(pDX, IDC_BUTTON_COLOR_BACK_SELECTED, m_buttonBackSelected);
	DDX_Control(pDX, IDC_BUTTON_COLOR_BAR, m_buttonBar);
	DDX_Control(pDX, IDC_BUTTON_COLOR_BAR_OUTLINE, m_buttonBarOutline);

	DDX_Control(pDX, IDOK, m_buttonOk);
}

BOOL CDlgColorGeneral::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	m_colorGeneral[COLOR_GENERAL_ARRAY_GRID]			= theApp.GetProfileInt(registrySectionColor, registryColorGeneralGrid, COLOR_GENERAL_GRID);
	m_colorGeneral[COLOR_GENERAL_ARRAY_FONT]			= theApp.GetProfileInt(registrySectionColor, registryColorGeneralFont, COLOR_GENERAL_FONT);
	m_colorGeneral[COLOR_GENERAL_ARRAY_FONT_SELECTED]	= theApp.GetProfileInt(registrySectionColor, registryColorGeneralFontSelected, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
	m_colorGeneral[COLOR_GENERAL_ARRAY_BACK_SELECTED]	= theApp.GetProfileInt(registrySectionColor, registryColorGeneralBackSelected, ::GetSysColor(COLOR_HIGHLIGHT));
	m_colorGeneral[COLOR_GENERAL_ARRAY_BAR]				= theApp.GetProfileInt(registrySectionColor, registryColorGeneralBar, COLOR_GENERAL_BAR);
	m_colorGeneral[COLOR_GENERAL_ARRAY_BAR_OUTLINE]		= theApp.GetProfileInt(registrySectionColor, registryColorGeneralBarOutline, COLOR_GENERAL_BAR_OUTLINE);

	m_bColorWindowUpdate= true;

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleGeneral];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonGrid.SetWindowText(gszTranslation[PosDialogColorGeneralGrid]);
	m_buttonFont.SetWindowText(gszTranslation[PosDialogColorGeneralFont]);
	m_buttonFontSelected.SetWindowText(gszTranslation[PosDialogColorGeneralFontSelected]);
	m_buttonBackSelected.SetWindowText(gszTranslation[PosDialogColorGeneralBackSelected]);
	m_buttonBar.SetWindowText(gszTranslation[PosDialogColorGeneralBar]);
	m_buttonBarOutline.SetWindowText(gszTranslation[PosDialogColorGeneralBarOutline]);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
// Translation



//	SendFont();		// don't send the data let them ask for it!

	SetTimer(UTIMER_DLG_COLOR_GENERAL, 1000, 0);	// interval 1 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

COLORREF *CDlgColorGeneral::GetColorArray()
{
	return &m_colorGeneral[0];
}

void CDlgColorGeneral::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColor,registryColorGeneralGrid , m_colorGeneral[COLOR_GENERAL_ARRAY_GRID]);
	theApp.WriteProfileInt(registrySectionColor,registryColorGeneralFont , m_colorGeneral[COLOR_GENERAL_ARRAY_FONT]);
	theApp.WriteProfileInt(registrySectionColor,registryColorGeneralFontSelected , m_colorGeneral[COLOR_GENERAL_ARRAY_FONT_SELECTED]);
	theApp.WriteProfileInt(registrySectionColor,registryColorGeneralBackSelected , m_colorGeneral[COLOR_GENERAL_ARRAY_BACK_SELECTED]);
	theApp.WriteProfileInt(registrySectionColor,registryColorGeneralBar , m_colorGeneral[COLOR_GENERAL_ARRAY_BAR]);
	theApp.WriteProfileInt(registrySectionColor,registryColorGeneralBarOutline , m_colorGeneral[COLOR_GENERAL_ARRAY_BAR_OUTLINE]);
	m_bColorWindowUpdate= true;
}

BEGIN_MESSAGE_MAP(CDlgColorGeneral, CPropertyPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COLOR_RASTER, &CDlgColorGeneral::OnBnClickedButtonColorGrid)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_FONT, &CDlgColorGeneral::OnBnClickedButtonColorFont)
	ON_BN_CLICKED(IDOK, &CDlgColorGeneral::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_BAR, &CDlgColorGeneral::OnBnClickedButtonColorBar)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_BAR_OUTLINE, &CDlgColorGeneral::OnBnClickedButtonColorBarOutline)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_FONT_SELECTED, &CDlgColorGeneral::OnBnClickedButtonColorFontSelected)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_BACK_SELECTED, &CDlgColorGeneral::OnBnClickedButtonColorBackSelected)
END_MESSAGE_MAP()


// CDlgColorGeneral message handlers

void CDlgColorGeneral::OnTimer(UINT_PTR nIDEvent)
{
	int iCount;
	if (m_bColorWindowUpdate)
	{
		m_bColorWindowUpdate = false;
		for (iCount = 0; iCount < COLOR_ARRAY_NUMBER_GENERAL; iCount++)
		{
			m_wndGeneral[iCount].SetColor(m_colorGeneral[iCount]);
			m_wndGeneral[iCount].Invalidate(TRUE);
		}
//		::PostMessage(m_hWnd, WM_GRAPHIC_COLOR_CHANGED, 0, 0);
	}
}

void CDlgColorGeneral::OnBnClickedButtonColorGrid()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorGeneral[COLOR_GENERAL_ARRAY_GRID],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorGeneral[COLOR_GENERAL_ARRAY_GRID] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorGeneralGrid , m_colorGeneral[COLOR_GENERAL_ARRAY_GRID]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGeneral::OnBnClickedButtonColorFont()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorGeneral[COLOR_GENERAL_ARRAY_FONT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorGeneral[COLOR_GENERAL_ARRAY_FONT] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorGeneralFont , m_colorGeneral[COLOR_GENERAL_ARRAY_FONT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorGeneral::OnBnClickedButtonColorFontSelected()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorGeneral[COLOR_GENERAL_ARRAY_FONT_SELECTED],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorGeneral[COLOR_GENERAL_ARRAY_FONT_SELECTED] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorGeneralFontSelected , m_colorGeneral[COLOR_GENERAL_ARRAY_FONT_SELECTED]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGeneral::OnBnClickedButtonColorBackSelected()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorGeneral[COLOR_GENERAL_ARRAY_BACK_SELECTED],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorGeneral[COLOR_GENERAL_ARRAY_BACK_SELECTED] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorGeneralBackSelected , m_colorGeneral[COLOR_GENERAL_ARRAY_BACK_SELECTED]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGeneral::OnBnClickedButtonColorBar()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorGeneral[COLOR_GENERAL_ARRAY_BAR],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorGeneral[COLOR_GENERAL_ARRAY_BAR] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorGeneralBar , m_colorGeneral[COLOR_GENERAL_ARRAY_BAR]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorGeneral::OnBnClickedButtonColorBarOutline()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	CColorDialog dlg(m_colorGeneral[COLOR_GENERAL_ARRAY_BAR_OUTLINE],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorGeneral[COLOR_GENERAL_ARRAY_BAR_OUTLINE] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorGeneralBarOutline , m_colorGeneral[COLOR_GENERAL_ARRAY_BAR_OUTLINE]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

COLORREF CDlgColorGeneral::Correct(COLORREF color)
{
	int r,g,b;

	r = color >> 16; 
	g = (color >> 8) & 0xff;
	b = color & 0xff;

	if ((r) < 20) r = 20;
	if ((g) < 20) g = 20;
	if ((b) < 20) b = 20;

	color = (r << 16) | (g << 8) | b;
	return color;

}

void CDlgColorGeneral::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}

BOOL CDlgColorGeneral::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::PreTranslateMessage(pMsg);
}

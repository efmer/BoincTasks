// DlgFont.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgFont.h"
#include "afxdialogex.h"
#include "DlgColorMain.h"
#include "DlgProperties.h"
#include "DlgComputerLocation.h"
#include "DlgYesNo.h"
#include "DlgFloating.h"
#include "Translation.h"

// CDlgFont dialog

IMPLEMENT_DYNAMIC(CDlgFont, CPropertyPage)

CDlgFont::CDlgFont()
	: CPropertyPage(CDlgFont::IDD)
{
	m_pFont = new CFont;
	m_pFontGadget = new CFont;

	memset(&m_logFont, 0, sizeof(LOGFONT));
	memset(&m_logFontGadget, 0, sizeof(LOGFONT));
}

CDlgFont::~CDlgFont()
{
	delete m_pFont;
	delete m_pFontGadget;
}

void CDlgFont::SendFont()
{
	CView		*pView;
	CDocument	*pDoc;
	POSITION	posDoc, posView;
	LOGFONT		*pLogFont;

	if (m_bSystemFont) pLogFont = NULL;
	else pLogFont = &m_logFont;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			posView = pDoc->GetFirstViewPosition();
			while (1)
			{
				pView = pDoc->GetNextView(posView);
				if (pView == NULL) break;
				pView->PostMessage(UWM_MSG_FONT_CHANGE_ALL, (WPARAM) pLogFont, 0);
			}
		}
	}
	if (theApp.m_pDlgProperties != NULL)
	{
		theApp.m_pDlgProperties->PostMessage(UWM_MSG_FONT_CHANGE_ALL, (WPARAM) pLogFont, 0);
	}
	if (theApp.m_pDlgComputerLocation != NULL)
	{
		theApp.m_pDlgComputerLocation->PostMessage(UWM_MSG_FONT_CHANGE_ALL, (WPARAM) pLogFont, 0);
	}
}

void CDlgFont::SendFontGadget()
{
	if (theApp.m_pDlgFloating != NULL)
	{
		if (m_bSystemFont) theApp.m_pDlgFloating->PostMessage(UWM_MSG_FONT_CHANGE_ALL, (WPARAM) NULL, 0);
		else theApp.m_pDlgFloating->PostMessage(UWM_MSG_FONT_CHANGE_ALL, (WPARAM) &m_logFontGadget, 0);
	}
}

void CDlgFont::SetFont(bool bSystemFont, BYTE *pcBuffer, BYTE *pcBufferGadget)
{
	int iCheck;
	m_bSystemFont = bSystemFont;
	iCheck = bSystemFont;

	ShowHideSystem();
	theApp.WriteProfileInt(registrySectionFont,registryFontUseSystem , iCheck);
	m_checkUsesSystem.SetCheck(iCheck);

	memcpy((LOGFONT *) &m_logFont, pcBuffer, sizeof(LOGFONT));
	memcpy((LOGFONT *) &m_logFontGadget, pcBufferGadget, sizeof(LOGFONT));

	ProcessFont();
	ProcessFontGadget();
}

void CDlgFont::ProcessFont()
{
	theApp.WriteProfileBinary(registrySectionFont, registryFontLf, (LPBYTE) &m_logFont, sizeof(LOGFONT));
	SetEditFont(&m_editFont, &m_logFont, &m_pFont);
	m_buttonGeneral.SetWindowText(m_logFont.lfFaceName);
	SendFont();
}

void CDlgFont::ProcessFontGadget()
{
	theApp.WriteProfileBinary(registrySectionFont, registryFontLfGadget, (LPBYTE) &m_logFontGadget, sizeof(LOGFONT));
	SetEditFont(&m_editFontGadget, &m_logFontGadget, &m_pFontGadget);
	m_buttonGadget.SetWindowText(m_logFontGadget.lfFaceName);
	SendFontGadget();
}

void CDlgFont::ShowHideSystem()
{
	int		iShow;

	if (m_bSystemFont) iShow = SW_HIDE;
	else iShow = SW_SHOW;

	m_editFont.ShowWindow(iShow);
	m_editFontGadget.ShowWindow(iShow);
	m_textGroupGeneral.ShowWindow(iShow);
	m_textGroupGadget.ShowWindow(iShow);
	m_buttonGeneral.ShowWindow(iShow);
	m_buttonGadget.ShowWindow(iShow);
}

void CDlgFont::SetEditFont(CEdit *pEdit,LOGFONT *pLf, CFont **pFont)
{
	CFont *pFontTemp;

	pEdit->SetWindowText("AbCd aBcD");

	pFontTemp = new CFont;
	pFontTemp->CreateFontIndirect(pLf);
	pEdit->SetFont(pFontTemp,TRUE);

	delete *pFont;
	*pFont = pFontTemp;
}

void CDlgFont::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_FONT, m_editFont);
	DDX_Control(pDX, IDC_EDIT_FONT_GADGET, m_editFontGadget);
	DDX_Control(pDX, IDC_FONT_GROUP_GENERAL, m_textGroupGeneral);
	DDX_Control(pDX, IDC_FONT_GROUP_GADGET, m_textGroupGadget);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_FONT_GENERAL, m_buttonGeneral);
	DDX_Control(pDX, IDC_FONT_GADGET, m_buttonGadget);
	DDX_Control(pDX, IDC_CHECK_FONT_SYSTEM, m_checkUsesSystem);
}


BEGIN_MESSAGE_MAP(CDlgFont, CPropertyPage)
	ON_MESSAGE(UWM_MSG_FONT_INITIAL,OnChangedFontInitial)
	ON_BN_CLICKED(IDOK, &CDlgFont::OnBnClickedOk)
	ON_BN_CLICKED(IDC_FONT_GENERAL, &CDlgFont::OnBnClickedFontGeneral)
	ON_BN_CLICKED(IDC_FONT_GADGET, &CDlgFont::OnBnClickedFontGadget)
	ON_BN_CLICKED(IDC_CHECK_FONT_SYSTEM, &CDlgFont::OnBnClickedCheckFontSystem)
END_MESSAGE_MAP()


// CDlgFont message handlers

BOOL CDlgFont::OnInitDialog()
{
	LOGFONT *plfBuffer;
	CPropertySheet *pSheet;
	CString		sNr;

//	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleFont];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_textGroupGeneral.SetWindowText(gszTranslation[PosDialogColorFontGroupGeneral]);
	m_textGroupGadget.SetWindowText(gszTranslation[PosDialogColorFontGroupGadget]);
	m_checkUsesSystem.SetWindowText(gszTranslation[PosDialogColorFontUseSystem]);

	// translations

	int iSystemFont;
	iSystemFont = theApp.GetProfileInt(registrySectionFont, registryFontUseSystem, TRUE);
	m_bSystemFont = (iSystemFont == TRUE);
	m_checkUsesSystem.SetCheck(iSystemFont);

	UINT iRead = 0;
	memset(&m_logFont, 0, sizeof(LOGFONT));
	theApp.GetProfileBinary(registrySectionFont, registryFontLf, (LPBYTE *) &plfBuffer, &iRead);
	if (iRead == sizeof(LOGFONT))
	{
		memcpy((LOGFONT *) &m_logFont, plfBuffer, sizeof(LOGFONT));
		delete [] plfBuffer;
	}

	memset(&m_logFontGadget, 0, sizeof(LOGFONT));
	theApp.GetProfileBinary(registrySectionFont, registryFontLfGadget, (LPBYTE *) &plfBuffer, &iRead);
	if (iRead == sizeof(LOGFONT))
	{
		memcpy((LOGFONT *) &m_logFontGadget, plfBuffer, sizeof(LOGFONT));
		delete [] plfBuffer;
	}

	BOOL fResult;

	NONCLIENTMETRICS ncm;

	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	fResult = SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
		
	if (fResult)
	{
		if (*m_logFont.lfFaceName == 0)
		{
			memcpy((LOGFONT *) &m_logFont, (LOGFONT *) &ncm.lfMenuFont, sizeof(LOGFONT));
		}
		if (*m_logFontGadget.lfFaceName == 0)
		{
			memcpy((LOGFONT *) &m_logFontGadget, (LOGFONT *) &ncm.lfMenuFont, sizeof(LOGFONT));
		}
	}	

	ShowHideSystem();

	SetEditFont(&m_editFont, &m_logFont, &m_pFont);
	SetEditFont(&m_editFontGadget, &m_logFontGadget, &m_pFontGadget);

	m_buttonGeneral.SetWindowText(m_logFont.lfFaceName);
	m_buttonGadget.SetWindowText(m_logFontGadget.lfFaceName);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


LRESULT CDlgFont::OnChangedFontInitial(WPARAM wParam,LPARAM lParam)
{
	int		iCommand;
	LOGFONT	*pLogFont;

	if (m_bSystemFont) pLogFont = NULL;
	else pLogFont = &m_logFont;

	iCommand = (int) lParam;
	if (iCommand == FONT_TYPE_GENERAL)
	{
		::PostMessage((HWND) wParam,UWM_MSG_FONT_HAS_CHANGED, (WPARAM) pLogFont, 0);
		return 0;
	}

	::PostMessage((HWND) wParam,UWM_MSG_FONT_HAS_CHANGED, (WPARAM) pLogFont, 0);
	
	return 0;
}

/*
void CDlgFont::OnCbnSelchangeComboFont()
{
	int		iSelection;
	CFont	font;

	iSelection = m_comboFont.GetCurSel();

	m_comboFont.GetLBText(iSelection,m_sFont);
	m_sFont = m_sFont.Left(LF_FACESIZE-1);

	m_editFont.SetWindowText("AbCd aBcD");

	delete m_pFont;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = m_iFontSize;

	strcpy_s(lf.lfFaceName, LF_FACESIZE, m_sFont.GetBuffer()); // "Sevenet 7");
	m_sFont.ReleaseBuffer();

	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(&lf);

	m_editFont.SetFont(m_pFont,TRUE);

	theApp.WriteProfileString(registrySectionFont,registryFontDefault , m_sFont);

	SendFont();
}
*/

/*
void CDlgFont::OnCbnSelchangeComboFontSize()
{
	int iSelection;
	CString sSelection;

	iSelection = m_comboSize.GetCurSel();
	m_comboSize.GetLBText(iSelection,sSelection);

	m_iFontSize =  atoi(sSelection);
	if (m_iFontSize <= 0) m_iFontSize = 16;
	theApp.WriteProfileInt(registrySectionFont,registryFontDefaultSize , m_iFontSize);

	OnCbnSelchangeComboFont();
}
*/
/*
void CDlgFont::OnCbnSelchangeComboFontGadget()
{
	int		iSelection;
	CFont	font;

	iSelection = m_comboFontGadget.GetCurSel();

	m_comboFontGadget.GetLBText(iSelection,m_sFontGadget);
	m_sFontGadget = m_sFontGadget.Left(LF_FACESIZE-1);

	m_editFontGadget.SetWindowText("AbCd aBcD");

	delete m_pFontGadget;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = m_iFontSizeGadget;

	strcpy_s(lf.lfFaceName, LF_FACESIZE, m_sFontGadget.GetBuffer()); // "Sevenet 7");
	m_sFont.ReleaseBuffer();

	m_pFontGadget = new CFont;
	m_pFontGadget->CreateFontIndirect(&lf);

	m_editFontGadget.SetFont(m_pFontGadget,TRUE);

	theApp.WriteProfileString(registrySectionFont,registryFontGadget , m_sFontGadget);

	SendFontGadget();
}


void CDlgFont::OnCbnSelchangeComboFontGadgetSize()
{
	int iSelection;
	CString sSelection;

	iSelection = m_comboSizeGadget.GetCurSel();
	m_comboSizeGadget.GetLBText(iSelection,sSelection);

	m_iFontSizeGadget =  atoi(sSelection);
	if (m_iFontSizeGadget <= 0) m_iFontSizeGadget = 14;
	theApp.WriteProfileInt(registrySectionFont,registryFontGadgetSize , m_iFontSizeGadget);

	OnCbnSelchangeComboFontGadget();
}
*/

void CDlgFont::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}

void CDlgFont::OnBnClickedFontGeneral()
{
	CFontDialog dlg(&m_logFont,CF_SCREENFONTS|CF_LIMITSIZE);
	dlg.m_cf.nSizeMin = 4;
	dlg.m_cf.nSizeMax = 22;
	if (dlg.DoModal() == IDOK) ProcessFont();
}


void CDlgFont::OnBnClickedFontGadget()
{
	CFontDialog dlg(&m_logFontGadget,CF_SCREENFONTS|CF_LIMITSIZE);
	dlg.m_cf.nSizeMin = 4;
	dlg.m_cf.nSizeMax = 22;
	if (dlg.DoModal() == IDOK)	ProcessFontGadget();
}


void CDlgFont::OnBnClickedCheckFontSystem()
{
	int iCheck;

	iCheck = m_checkUsesSystem.GetCheck();

	m_bSystemFont = (iCheck == TRUE);
	ShowHideSystem();

	SendFont();
	SendFontGadget();

	theApp.WriteProfileInt(registrySectionFont,registryFontUseSystem , iCheck);
}

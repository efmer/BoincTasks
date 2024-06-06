// DlgYesNo.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgFont.h"
#include "DlgYesNo.h"
#include "Translation.h"

// CDlgYesNo dialog

IMPLEMENT_DYNAMIC(CDlgYesNo, CDialog)

CDlgYesNo::CDlgYesNo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgYesNo::IDD, pParent)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int iCount = 0; iCount < NUM_YESNO_COLUMNS; iCount++)
	{
		m_sColumn[iCount] = "";
		m_iColumnWidth[iCount] = 0;
	}

	bColumnMode = false;
//	m_iInsertPos = 0;
	m_bUpdateWindow = false;
	m_iWindowInitialBottomMargin = -1;
	m_iWindowInitialYesNoButton = -1;

	m_pFont = new CFont;
}

CDlgYesNo::~CDlgYesNo()
{
	delete m_pFont;

	if (!m_hIcon)
	{
		delete m_hIcon;
	}
}

BOOL CDlgYesNo::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);	// request new font.

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryYesNoPosLeft+m_sRegistryId, 20);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryYesNoPosTop+m_sRegistryId, 20);
	int width = theApp.GetProfileInt(registrySectionWindowPosition, registryYesNoPosRight+m_sRegistryId, 20+850) - left;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, registryYesNoPosBottom+m_sRegistryId, 20+500) - top;

	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER );


	//translate
	m_buttonYes.SetWindowText(gszTranslation[PosDialogCommonButtonsYesButton]);
	m_buttonNo.SetWindowTextA(gszTranslation[PosDialogCommonButtonsNoButton]);
	//translate

	if (bColumnMode)
	{
		m_text.ShowWindow(SW_HIDE);
		AddColumns();
		for (int iPos = 0; iPos < (int) m_lColumnText.size(); iPos++)
		{
			AddColumnReal(iPos, m_lColumnText.at(iPos)->m_sTxt[0], m_lColumnText.at(iPos)->m_sTxt[1], m_lColumnText.at(iPos)->m_sTxt[2], m_lColumnText.at(iPos)->m_sTxt[3], m_lColumnText.at(iPos)->m_sTxt[4], m_lColumnText.at(iPos)->m_sTxt[5]);
		}

	}
	else
	{
		m_list.ShowWindow(SW_HIDE);
		m_text.SetWindowText(m_sTxt);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#define TOP_MARGIN_YES_NO	10
#define LEFT_MARGIN_YES_NO 0
#define BOTTOM_MARGIN_YES_NO 10

void CDlgYesNo::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
{
	LONG lTopMargin, lBottomMargin, lLeftMargin, lRightMargin, lWidthGraphic, lHeightGraphic;

	// get initial position of the graph window.
	if (m_iWindowInitialBottomMargin == -1)
	{
		if (m_list.m_hWnd != NULL)
		{
			WINDOWPLACEMENT wp;
			m_list.GetWindowPlacement(&wp);
			m_iWindowInitialBottomMargin = lHeight - wp.rcNormalPosition.bottom;
		}
	}

	if (m_iWindowInitialYesNoButton == -1)
	{
		if (m_buttonYes.m_hWnd != NULL)
		{
			WINDOWPLACEMENT wp;
			m_buttonYes.GetWindowPlacement(&wp);
			m_iWindowInitialYesNoButton = lHeight - wp.rcNormalPosition.top;
			m_iWidthYesNoButton = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
			m_iHeightYesNoButton = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		}

	}

	switch (nType)
	{
		case SIZE_MAXIMIZED:
			lTopMargin		= TOP_MARGIN_YES_NO;
			lBottomMargin	= 10;
			lLeftMargin		= 10+LEFT_MARGIN_YES_NO;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin)) - (m_iHeightYesNoButton + BOTTOM_MARGIN_YES_NO);
		break;
		case SIZE_RESTORED:
			lTopMargin		= TOP_MARGIN_YES_NO;
			lBottomMargin	= 10;
			lLeftMargin		= 5+LEFT_MARGIN_YES_NO;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin)) - (m_iHeightYesNoButton + BOTTOM_MARGIN_YES_NO);
		break;
		default:
			lTopMargin		= TOP_MARGIN_YES_NO;
			lBottomMargin	= 10;
			lLeftMargin		= 5+LEFT_MARGIN_YES_NO;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin)) - (m_iHeightYesNoButton + BOTTOM_MARGIN_YES_NO);
	}

// position graphic relative to window NOT the screen

	if (m_list.m_hWnd != NULL)
	{
		m_list.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
		m_list.Invalidate(TRUE);
	}
	if (m_text.m_hWnd != NULL)
	{
		m_text.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
		m_text.Invalidate(TRUE);
	}

	if (m_buttonYes.m_hWnd != NULL)
	{
		m_buttonYes.MoveWindow(10, lHeight-m_iHeightYesNoButton-BOTTOM_MARGIN_YES_NO, m_iWidthYesNoButton, m_iHeightYesNoButton,FALSE);
		m_buttonYes.Invalidate(TRUE);
	}

	if (m_buttonNo.m_hWnd != NULL)
	{
		m_buttonNo.MoveWindow(lWidth-m_iWidthYesNoButton-BOTTOM_MARGIN_YES_NO,  lHeight-m_iHeightYesNoButton-10, m_iWidthYesNoButton, m_iHeightYesNoButton,FALSE);
		m_buttonNo.Invalidate(TRUE);
	}

	// set timer to update window

	m_bUpdateWindow = true;
	this->Invalidate();
}

void CDlgYesNo::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryYesNoPosLeft+m_sRegistryId, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryYesNoPosTop+m_sRegistryId, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryYesNoPosRight+m_sRegistryId, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryYesNoPosBottom+m_sRegistryId, wp.rcNormalPosition.bottom);
}

int CDlgYesNo::ReadColumnWidth(int iColumn, int iDefault)
{
	CString	sTxt, sRegistry;
	char	cChar;
	int		iWidth;

	sRegistry = registryColumnYesNo;
	sRegistry+= m_sRegistryId;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
//	if (iWidth < 20) return iDefault;
	return iWidth;
}

void CDlgYesNo::SafeColumnWidth()
{
	int		iWidth;
	CString sRegistry;
	char	cChar;

	for (int iCount = 0; iCount < NUM_YESNO_COLUMNS; iCount++)
	{
		iWidth = m_list.GetColumnWidth(iCount);
		if (m_iColumnWidth[iCount] != iWidth)
		{
			sRegistry = registryColumnYesNo;
			sRegistry+= m_sRegistryId;
			cChar = iCount + 'a';
			sRegistry = sRegistry + "_" + cChar; 
			// update new width in registry
			theApp.WriteProfileInt(registrySectionColumn, sRegistry, iWidth);
		}
	}
}


void CDlgYesNo::AddColumns()
{
	LV_COLUMN	lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iCount = 0; iCount < NUM_YESNO_COLUMNS; iCount++)
	{
		lvc.iSubItem = iCount;
		lvc.pszText = m_sColumn[iCount].GetBuffer(); 
		lvc.cx = ReadColumnWidth(iCount,m_iColumnWidth[iCount]);
		lvc.fmt = LVCFMT_LEFT;
		m_list.InsertColumn(iCount,&lvc);
		m_sColumn[iCount].ReleaseBuffer();
	}
}

void CDlgYesNo::AddColumnReal(int iPos, CString sTxt1, CString sTxt2, CString sTxt3, CString sTxt4, CString sTxt5, CString sTxt6)
{
	m_list.InsertItem(iPos,"");

	m_list.SetItemText(iPos,0,sTxt1);
	m_list.SetItemText(iPos,1,sTxt2);
	m_list.SetItemText(iPos,2,sTxt3);
	m_list.SetItemText(iPos,3,sTxt4);
	m_list.SetItemText(iPos,4,sTxt5);
	m_list.SetItemText(iPos,5,sTxt6);
}

void CDlgYesNo::AddColumn(CString sTxt1, CString sTxt2, CString sTxt3, CString sTxt4, CString sTxt5, CString sTxt6)
{
	CColumnTextAdd *pColumnTextAdd;

	pColumnTextAdd = new CColumnTextAdd;

	pColumnTextAdd->m_sTxt[0] = sTxt1;
	pColumnTextAdd->m_sTxt[1] = sTxt2;
	pColumnTextAdd->m_sTxt[2] = sTxt3;
	pColumnTextAdd->m_sTxt[3] = sTxt4;
	pColumnTextAdd->m_sTxt[4] = sTxt5;
	pColumnTextAdd->m_sTxt[5] = sTxt6;

	m_lColumnText.push_back(pColumnTextAdd);

//	m_sColumnText[m_iInsertPos][0] = sTxt1;
//	m_sColumnText[m_iInsertPos][1] = sTxt2;
//	m_sColumnText[m_iInsertPos][2] = sTxt3;
//	m_sColumnText[m_iInsertPos][3] = sTxt4;
//	m_sColumnText[m_iInsertPos][4] = sTxt5;
//	m_sColumnText[m_iInsertPos][5] = sTxt6;
//	m_iInsertPos++;
}

void CDlgYesNo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_YESNO, m_text);
	DDX_Control(pDX, IDC_LIST_YESNO, m_list);
	DDX_Control(pDX, IDYES, m_buttonYes);
	DDX_Control(pDX, IDNO, m_buttonNo);
}


BEGIN_MESSAGE_MAP(CDlgYesNo, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_BN_CLICKED(IDNO, &CDlgYesNo::OnBnClickedNo)
	ON_BN_CLICKED(IDYES, &CDlgYesNo::OnBnClickedYes)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_YESNO, &CDlgYesNo::OnLvnItemchangedListYesno)

	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnChangedFont)
	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnChangedFont)

END_MESSAGE_MAP()


// CDlgYesNo message handlers

void CDlgYesNo::OnBnClickedNo()
{
	SafeWindowPlacement();
	SafeColumnWidth();
	EndDialog(IDNO);
}

void CDlgYesNo::OnBnClickedYes()
{
	SafeWindowPlacement();
	SafeColumnWidth();
	EndDialog(IDYES);
}

void CDlgYesNo::OnClose() 
{
	SafeWindowPlacement();
	SafeColumnWidth();
	CDialog::OnClose();
}

void CDlgYesNo::OnLvnItemchangedListYesno(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgYesNo::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

void CDlgYesNo::OnSizing(UINT fwSide, LPRECT pRect)
{
	LONG lTop, lBottom,lLeft, lRight, lWidth, lHeight;
	bool bSetWindow = false;
	bool bSetWindowWidth = false;
	bool bSetWindowHeight = false;

// limit the minimum size of the dialog

	lTop	= pRect->top;
	lBottom = pRect->bottom;
	lLeft	= pRect->left;
	lRight	= pRect->right;
	lWidth	= lRight - lLeft;
	lHeight	= lBottom - lTop;

	if (lWidth < 200)
	{
		bSetWindow = true;
		bSetWindowWidth = true;
	}
	if (lHeight < 200)
	{
		bSetWindow = true;
		bSetWindowHeight = true;
	}

	if (bSetWindowWidth)
	{
		pRect->right  = pRect->left+ 201;
	}
	if (bSetWindowHeight)
	{
		pRect->bottom = pRect->top + 201;
	}

	CDialog::OnSizing(fwSide, pRect);
}

LRESULT CDlgYesNo::OnChangedFont(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLp;
	pLp = (LOGFONT *) wParam;

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLp);

	m_list.SetFont(m_pFont,TRUE);
	m_list.Invalidate(TRUE);

	return 0;
}


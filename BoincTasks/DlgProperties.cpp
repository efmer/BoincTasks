// DlgProperties.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgProperties.h"
#include "DlgFont.h"
#include "Translation.h"


// CDlgProperties dialog

IMPLEMENT_DYNAMIC(CDlgProperties, CDialog)

CDlgProperties::CDlgProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProperties::IDD, pParent)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bUpdateWindow = false;

	m_pFont = new CFont;
}

CDlgProperties::~CDlgProperties()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
	delete m_pFont;
}

BOOL CDlgProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

//	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	RestoreWindowPlacement();

	m_listProperties.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/*
void CDlgProperties::SetText(CString *psTxt)
{
	CString sTxt;
	sTxt = *psTxt;

	m_progress.SetWindowText(sTxt);
	m_progress.Invalidate(TRUE);
	m_progress.SetFocus();
}
*/

#define TOP_MARGIN_PROGRESS	10
#define LEFT_MARGIN_PROGRESS 0

void CDlgProperties::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
{
	LONG lTopMargin, lBottomMargin, lLeftMargin, lRightMargin, lWidthGraphic, lHeightGraphic;

	switch (nType)
	{
		case SIZE_MAXIMIZED:
			lTopMargin		= TOP_MARGIN_PROGRESS;
			lBottomMargin	= 10;
			lLeftMargin		= 10+LEFT_MARGIN_PROGRESS;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
		break;
		case SIZE_RESTORED:
			lTopMargin		= TOP_MARGIN_PROGRESS;
			lBottomMargin	= 10;
			lLeftMargin		= 5+LEFT_MARGIN_PROGRESS;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
		break;
		default:
			lTopMargin		= TOP_MARGIN_PROGRESS;
			lBottomMargin	= 10;
			lLeftMargin		= 5+LEFT_MARGIN_PROGRESS;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
	}

// position graphic relative to window NOT the screen

	if (m_listProperties.m_hWnd != NULL)
	{
		m_listProperties.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
		m_listProperties.Invalidate(TRUE);
	}

	// set timer to update window

	m_bUpdateWindow = true;
	this->Invalidate();
}

void CDlgProperties::RestoreWindowPlacement()
{
	CString sRegistry;

	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosLeft;
	int left = theApp.GetProfileInt(registrySectionWindowPosition, sRegistry, 100);
	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosTop;
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, sRegistry, 100);
	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosRight;
	int width = theApp.GetProfileInt(registrySectionWindowPosition, sRegistry, 100+600) - left;
	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosBottom;
	int height = theApp.GetProfileInt(registrySectionWindowPosition, sRegistry, 100+500) - top;
	theApp.ValidScreenPosition(&left, &top, &width, &height);
 	SetWindowPos(&CWnd::wndBottom, left, top, width, height, SWP_NOOWNERZORDER );
}

void CDlgProperties::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	CString sRegistry;

	GetWindowPlacement(&wp);
	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosLeft;
	theApp.WriteProfileInt(registrySectionWindowPosition, sRegistry, wp.rcNormalPosition.left);
	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosTop;
	theApp.WriteProfileInt(registrySectionWindowPosition, sRegistry, wp.rcNormalPosition.top);
	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosRight;
	theApp.WriteProfileInt(registrySectionWindowPosition, sRegistry, wp.rcNormalPosition.right);
	sRegistry = m_sRegistryPrefix + "_" + registryPropertiesPosBottom;
	theApp.WriteProfileInt(registrySectionWindowPosition, sRegistry, wp.rcNormalPosition.bottom);
	SafeColumn();
}

void CDlgProperties::SafeColumn()
{
	if (m_listProperties.GetItemCount() > 0)
	{
		int iColumnCount = m_listProperties.GetHeaderCtrl()->GetItemCount();
		if (iColumnCount > MAX_PROPERTIES_COLUMNS) iColumnCount = MAX_PROPERTIES_COLUMNS;

		CString sRegistry;
		char cChar;
		int	iWidth;
		for (int iCount = 0; iCount < iColumnCount; iCount++)
		{
			iWidth = m_listProperties.GetColumnWidth(iCount);

			sRegistry = registryColumnProperties;
			cChar = iCount + 'a';
			sRegistry = sRegistry + "_" + cChar; 
			sRegistry = m_sRegistryPrefix + "_" + sRegistry;
			theApp.WriteProfileInt(registrySectionColumn, sRegistry, iWidth);
		}
	}
}

void CDlgProperties::ClipBoard(bool bAll)
{
    HGLOBAL		hglbCopy; 
    LPTSTR		lptstrCopy; 
	CString		sTxt,sClipBoard;
	int			iItems;
	bool		bSelected;

	if ( !OpenClipboard() ) return;
	if( !EmptyClipboard() ) return;
	
	iItems = m_listProperties.GetItemCount();

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		bSelected = false;
		if (m_listProperties.GetItemState(iCount,LVIS_SELECTED)) bSelected = true;
		if (bAll) bSelected = true;
		if (bSelected)
		{
			sTxt = m_listProperties.GetItemText(iCount,0);
			sClipBoard += sTxt;
			sClipBoard += "\t";
			sTxt = m_listProperties.GetItemText(iCount,1);
			sClipBoard += sTxt;
			sClipBoard += "\r\n";
		}
	}

	// Allocate a global memory object for the text. 
 	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (sClipBoard.GetLength() + 1) * sizeof(char)); 
	if (hglbCopy == NULL) 
    { 
		CloseClipboard(); 
        return; 
	} 
 
	// Lock the handle and copy the text to the buffer. 
    lptstrCopy = (LPTSTR) GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, sClipBoard.GetBuffer(), sClipBoard.GetLength() * sizeof(char)); 
	lptstrCopy[sClipBoard.GetLength()] = (char) 0;    // null character 
	GlobalUnlock(hglbCopy); 
 
	// set the locale
	HANDLE hLC=GlobalAlloc(GMEM_MOVEABLE,sizeof(LCID));
	if (hLC)
	{
		PLCID pLc=(PLCID)GlobalLock(hLC);
		if (pLc)
		{
			*pLc = theApp.m_pTranslation->m_lcid;
			GlobalUnlock(hLC);
			if (!SetClipboardData(CF_LOCALE, pLc))	GlobalFree(hLC);
		}
		else
		{
			GlobalFree(hLC);
		}
	}

	// Place the handle on the clipboard. 
 	if (!SetClipboardData(CF_TEXT, hglbCopy)) GlobalFree(hglbCopy);; 

	CloseClipboard();
}

void CDlgProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_EDIT_PROPERTIES, m_progress);
	DDX_Control(pDX, IDC_LIST_PROPERTIES, m_listProperties);
}

BEGIN_MESSAGE_MAP(CDlgProperties, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(UWM_MSG_DLG_PROPERTIES_SETUP,OnSetup)
	ON_MESSAGE(UWM_MSG_DLG_PROPERTIES_ADD_ROW,OnAddRow)

	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnChangedFont)
	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnChangedFont)

	ON_COMMAND(ID_PROPERTIES_COPYALLTOCLIPBOARD, OnClipBoardAll)
END_MESSAGE_MAP()


// CDlgProperties message handlers

LRESULT CDlgProperties::OnSetup(WPARAM parm1, LPARAM parm2)
{
	int		iColumns, iWidth;
	CString sRegistry;
	char	cChar,*pcRegistry;

	iColumns = (int) parm1;

	// safe everthing first
	SafeColumn();
	SafeWindowPlacement();

	pcRegistry = (char *) parm2;
	m_sRegistryPrefix = pcRegistry;
	RestoreWindowPlacement();

	m_listProperties.DeleteAllItems();

	for(int iCount = 0; iCount < 10; iCount++)
	{
		m_listProperties.DeleteColumn(0);
	}

	LV_COLUMN	lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iColumn = 0; iColumn < iColumns; iColumn++)
	{
		lvc.pszText = "";
		lvc.iSubItem = iColumn;

		lvc.fmt = LVCFMT_LEFT;

		sRegistry = registryColumnProperties;
		cChar = iColumn + 'a';
		sRegistry = sRegistry + "_" + cChar; 
		sRegistry = m_sRegistryPrefix + "_" + sRegistry;
		iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, 250);

		lvc.cx = iWidth;
		m_listProperties.InsertColumn(iColumn,&lvc);
	}

	m_listProperties.SetFocus();

	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);	// request new font.

	this->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CDlgProperties::OnAddRow(WPARAM parm1, LPARAM parm2)
{
	int		iItems;
	COLORREF crColor;

	CString *pColumn0, *pColumn1;

	pColumn0 = (CString *) parm1;
	pColumn1 = (CString *) parm2;

	iItems = m_listProperties.GetItemCount();
	m_listProperties.InsertItem(iItems,"x");
	m_listProperties.SetItemText(iItems,0,*pColumn0);
	m_listProperties.SetItemText(iItems,1,*pColumn1);

	crColor = RGB(20,220,20);
	m_listProperties.SetItemData(iItems,crColor);

	return 0;
}

void CDlgProperties::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

void CDlgProperties::OnSizing(UINT fwSide, LPRECT pRect)
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

	if (lWidth < 100)
	{
		bSetWindow = true;
		bSetWindowWidth = true;
	}
	if (lHeight < 100)
	{
		bSetWindow = true;
		bSetWindowHeight = true;
	}

	if (bSetWindowWidth)
	{
		pRect->right  = pRect->left+ 101;
	}
	if (bSetWindowHeight)
	{
		pRect->bottom = pRect->top + 101;
	}

	CDialog::OnSizing(fwSide, pRect);
}

void CDlgProperties::OnContextMenu(CWnd* pWnd, CPoint point) 
{ 
	//our menu
	CMenu contextMenu; 
	//the actual popup menu which is the first
	//sub menu item of IDR_CONTEXT
	CMenu* tracker; 

	CPoint startPoint;

	// check if there are any selected items.
//	if (m_pSelection->SelectionNumber() == 0) return;

	startPoint = point;
	startPoint.y += 10;

	//at first we'll load our menu
	contextMenu.LoadMenu(IDR_MENU_PROPERTIES); 
 
	//we have to translate the mouse coordinates first:
	//these are relative to the window (or view) but the context menu
	//needs coordinates that are relative to the whole screen
	ClientToScreen(&point);
 
	//tracker now points to IDR_CONTEXT's first submenu 
	//GetSubMenu(0) -&gt; returns the first submenu,
	// GetSubMenu(1)-&gt; second one etc..
	tracker = contextMenu.GetSubMenu(0); 

//Translate
	tracker->ModifyMenu(ID_PROPERTIES_COPYALLTOCLIPBOARD,MF_STRING,ID_PROPERTIES_COPYALLTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopyAll]);
//	tracker->ModifyMenu(ID_HISTORY_COPYSELECTEDTOCLIPBOARD,MF_STRING,ID_HISTORY_COPYSELECTEDTOCLIPBOARD,gszTranslation[PosPopUpMessagesCopySelected]);

//Translate

	//show the context menu
	tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, startPoint.x , startPoint.y ,this);//; AfxGetMainWnd()); 

	//has always to be called (function of base class)
//   CListViewEx::OnRButtonDown(nFlags, point); 
}

void CDlgProperties::OnClipBoardAll()
{
	ClipBoard(true);
}

LRESULT CDlgProperties::OnChangedFont(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLp;
	pLp = (LOGFONT *) wParam;

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLp);

	m_listProperties.SetFont(m_pFont,TRUE);
	m_listProperties.Invalidate(TRUE);

	return 0;
}
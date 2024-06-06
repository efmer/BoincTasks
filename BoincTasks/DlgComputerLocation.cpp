// DlgComputerLocation.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgFont.h"
#include "DlgComputerLocation.h"
#include "afxdialogex.h"
#include "Xml.h"


// CDlgComputerLocation dialog

IMPLEMENT_DYNAMIC(CDlgComputerLocation, CDialogEx)

CDlgComputerLocation::CDlgComputerLocation(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgComputerLocation::IDD, pParent)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bUpdateWindow = false;
	m_bListChanged = false;

	m_pFont = new CFont;
}

CDlgComputerLocation::~CDlgComputerLocation()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}
	delete m_pFont;
}

BOOL CDlgComputerLocation::OnInitDialog()
{
	CString sRegistry;
	char	cChar;
	int		iWidth;

	CDialog::OnInitDialog();

//	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	RestoreWindowPlacement();

	m_listLocations.DeleteAllItems();

	for(int iCount = 0; iCount < 10; iCount++)
	{
		m_listLocations.DeleteColumn(0);
	}

	LV_COLUMN	lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iColumn = 0; iColumn < MAX_COMPUTER_LOCATION_COLUMNS; iColumn++)
	{
		switch (iColumn)
		{
		case 0: 
			lvc.pszText = "Location";
		break;
		default:
			lvc.pszText = "";
		}

		lvc.iSubItem = iColumn;

		lvc.fmt = LVCFMT_LEFT;

		sRegistry = registryColumnProperties;
		cChar = iColumn + 'a';
		sRegistry = sRegistry + "_" + cChar; 
		sRegistry = registryColumnLocation + sRegistry;
		iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, 250);

		lvc.cx = iWidth;
		m_listLocations.InsertColumn(iColumn,&lvc);
	}

	ReadXml();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#define TOP_MARGIN_COMPUTER_LOCATION	10
#define LEFT_MARGIN_COMPUTER_LOCATION 0

void CDlgComputerLocation::SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType)
{
	LONG lTopMargin, lBottomMargin, lLeftMargin, lRightMargin, lWidthGraphic, lHeightGraphic;

	switch (nType)
	{
		case SIZE_MAXIMIZED:
			lTopMargin		= TOP_MARGIN_COMPUTER_LOCATION;
			lBottomMargin	= 10;
			lLeftMargin		= 10+LEFT_MARGIN_COMPUTER_LOCATION;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
		break;
		case SIZE_RESTORED:
			lTopMargin		= TOP_MARGIN_COMPUTER_LOCATION;
			lBottomMargin	= 10;
			lLeftMargin		= 5+LEFT_MARGIN_COMPUTER_LOCATION;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
		break;
		default:
			lTopMargin		= TOP_MARGIN_COMPUTER_LOCATION;
			lBottomMargin	= 10;
			lLeftMargin		= 5+LEFT_MARGIN_COMPUTER_LOCATION;
			lRightMargin	= 5;
			lWidthGraphic	= lWidth - (lLeftMargin + lRightMargin);
			lHeightGraphic	= (lHeight- (lTopMargin + lBottomMargin));
	}

// position graphic relative to window NOT the screen

	if (m_listLocations.m_hWnd != NULL)
	{
		m_listLocations.MoveWindow(lLeftMargin, lTopMargin, lWidthGraphic, lHeightGraphic,FALSE);
		m_listLocations.Invalidate(TRUE);
	}

	// set timer to update window

	m_bUpdateWindow = true;
	this->Invalidate();
}

void CDlgComputerLocation::RestoreWindowPlacement()
{
	CRect rect;
	int iWidth, iHeight;

	int left = theApp.GetProfileInt(registrySectionWindowPosition, registryLocationPosLeft, 100);
	int top  = theApp.GetProfileInt(registrySectionWindowPosition, registryLocationPosTop, 100);

	GetWindowRect(rect);
	iWidth = rect.Width();
	iHeight = rect.Height();
	theApp.ValidScreenPosition(&left, &top, &iWidth, &iHeight);
 	SetWindowPos(&CWnd::wndBottom, left, top, iWidth, iHeight, SWP_NOOWNERZORDER );
}

void CDlgComputerLocation::SafeWindowPlacement()
{
	WINDOWPLACEMENT wp;
	CString sRegistry;

	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLocationPosLeft, wp.rcNormalPosition.left);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLocationPosTop, wp.rcNormalPosition.top);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLocationPosRight, wp.rcNormalPosition.right);
	theApp.WriteProfileInt(registrySectionWindowPosition, registryLocationPosBottom, wp.rcNormalPosition.bottom);
	SafeColumn();
}

void CDlgComputerLocation::SafeColumn()
{
	if (m_listLocations.GetItemCount() > 0)
	{
		int iColumnCount = m_listLocations.GetHeaderCtrl()->GetItemCount();
		if (iColumnCount > MAX_COMPUTER_LOCATION_COLUMNS) iColumnCount = MAX_COMPUTER_LOCATION_COLUMNS;

		CString sRegistry;
//		char cChar;
		int	iWidth;
		for (int iCount = 0; iCount < iColumnCount; iCount++)
		{
			iWidth = m_listLocations.GetColumnWidth(iCount);
			theApp.WriteProfileInt(registrySectionColumn, registryColumnLocation, iWidth);
		}
	}
}

LRESULT CDlgComputerLocation::OnSetup(WPARAM parm1, LPARAM parm2)
{
//	int		iColumns, iWidth;
	CString sRegistry;
//	char	cChar,*pcRegistry;

	m_listLocations.SetFocus();

	theApp.m_pDlgFont->PostMessage(UWM_MSG_FONT_INITIAL, (WPARAM) this->m_hWnd,FONT_TYPE_GENERAL);	// request new font.

	this->ShowWindow(SW_SHOW);
	return 0;
}

bool CDlgComputerLocation::CheckDouble(CString *psTxt)
{
	int iItems;
	CString sItem,sItem2;

	iItems = m_listLocations.GetItemCount();
	if (iItems > 0)
	{
		for (int iCount = 0; iCount < iItems; iCount++)
		{
			sItem = m_listLocations.GetItemText(iCount,0);
			sItem.MakeLower();
			for (int iCount2 = iCount+1; iCount2 < iItems; iCount2++)
			{
				sItem2 = m_listLocations.GetItemText(iCount2,0);
				*psTxt = sItem2;
				sItem2.MakeLower();
				if (sItem == sItem2) return true;
			}
		}
	}
	return false;
}

void CDlgComputerLocation::WriteLocationList()
{
	if (m_bListChanged)
	{
		WriteXml();
		m_bListChanged = false;
	}
}

void CDlgComputerLocation::RenameFile(CString sNow, CString sOld)
{

}

int CDlgComputerLocation::ReadXml(void)
{
	CString		sLocation, sDescription;
	CStringA	sAComputerPassword, sUseEncryption;
	int			iItems;

	iItems = 0;
	
	CXml xml("locations.xml",true,false);
	if (xml.ReadStart("locations"))
	{
		while (xml.ReadGroup("location"))
		{
			xml.ReadItem("location",&sLocation);
			xml.ReadItem("description",&sDescription);

			m_listLocations.InsertItem(iItems,"x");
			m_listLocations.SetItemText(iItems,0,sLocation);
			m_listLocations.SetItemText(iItems,1,sDescription);
			iItems++;
		}
	}

	return iItems;
}

void CDlgComputerLocation::WriteXml(void)
{
	CString		sLocation, sDescription;
	int			iItems;

	iItems = m_listLocations.GetItemCount();
	if (iItems == 0) return;

	CXml xml("locations.xml");
	if (xml.WriteStart("locations"))
	{
		for(int i = 0; i < iItems; i++)
		{
			xml.WriteBeginGroup("location");

			sLocation = m_listLocations.GetItemText( i, 0);
			sDescription = m_listLocations.GetItemText( i, 1);

			xml.WriteItem("location",sLocation);
			xml.WriteItem("description",sDescription);

			xml.WriteEndGroup();
		}
		xml.WriteEnd();
	}
}

void CDlgComputerLocation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOCATIONS, m_listLocations);
}


BEGIN_MESSAGE_MAP(CDlgComputerLocation, CDialogEx)
	ON_WM_SYSCOMMAND()	
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_MESSAGE(UWM_MSG_DLG_COMPUTER_LOCATION_SETUP,OnSetup)

	ON_BN_CLICKED(ID_COMPUTER_LOCATION_ADD, &CDlgComputerLocation::OnBnClickedComputerLocationAdd)
	ON_BN_CLICKED(ID_COMPUTER_LOCATION_REMOVE, &CDlgComputerLocation::OnBnClickedComputerLocationRemove)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOCATIONS, &CDlgComputerLocation::OnLvnItemchangedListLocations)
	ON_BN_CLICKED(IDOK, &CDlgComputerLocation::OnBnClickedOk)

	ON_MESSAGE(UWM_MSG_FONT_HAS_CHANGED,OnChangedFont)
	ON_MESSAGE(UWM_MSG_FONT_CHANGE_ALL,OnChangedFont)
	ON_BN_CLICKED(IDCANCEL, &CDlgComputerLocation::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgComputerLocation message handlers

void CDlgComputerLocation::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		// close window = cancel
		OnBnClickedOk();
		return;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

void CDlgComputerLocation::OnSize(UINT nType, int cx, int cy )
{
	SetWindowPosition(cx, cy, nType);
}

void CDlgComputerLocation::OnSizing(UINT fwSide, LPRECT pRect)
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

	CDialogEx::OnSizing(fwSide, pRect);
}

void CDlgComputerLocation::OnBnClickedComputerLocationAdd()
{
	int iItems;
	CString sTxt;

	iItems = m_listLocations.GetItemCount();
	sTxt.Format("location %d", iItems);
	m_listLocations.InsertItem(iItems,"x");
	m_listLocations.SetItemText(iItems,0,sTxt);
	m_listLocations.SetItemText(iItems,1,"");
}


void CDlgComputerLocation::OnBnClickedComputerLocationRemove()
{
	int iItem;

	iItem = m_listLocations.GetSelectionMark();
	if (iItem >= 0)
	{
		m_listLocations.DeleteItem(iItem);
//		xx item removed, remove from the list as well.
	}
}


void CDlgComputerLocation::OnLvnItemchangedListLocations(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgComputerLocation::OnBnClickedOk()
{
	int iItems;
	CString sDouble;

	if (CheckDouble(&sDouble))
	{
		sDouble = "Identical item found: " + sDouble;
		AfxMessageBox(sDouble,MB_ICONEXCLAMATION);
		this->SetForegroundWindow();
		return;
	}

	iItems = m_listLocations.GetItemCount();

	// check if the name has changed
//	for (int iCountList = 0; iCountList < iNrComputers; iCountList++)
//	{
//		sLocation =  listCtrl.GetItemText( iCountList, 0);
//		if (sLocation != m_sLocation[iCountList])xxxxx
//		{
			m_bListChanged = true;
//			RenameFile(sComputerId, sComputerIdHidden);
//		}
//	}

	WriteLocationList();

	this->ShowWindow(SW_HIDE);

//	CDialogEx::OnOK();
}

LRESULT CDlgComputerLocation::OnChangedFont(WPARAM wParam,LPARAM lParam)
{
	LOGFONT	*pLp;
	pLp = (LOGFONT *) wParam;

	delete m_pFont;
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(pLp);

	m_listLocations.SetFont(m_pFont,TRUE);
	m_listLocations.Invalidate(TRUE);

	return 0;
}

void CDlgComputerLocation::OnBnClickedCancel()
{
	this->ShowWindow(SW_HIDE);

//	CDialogEx::OnCancel();
}

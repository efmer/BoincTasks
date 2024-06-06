// AboutDlg.cpp : implementation file
//

#include "stdafx.h"

#include "Label.h"
#include "BoincTasks.h"
#include "AboutDlg.h"
#include "Translation.h"

// CAboutDlg dialog
#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif


IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dDeflateHeight = 350;
	m_dDeflateWidth = 350;
}

CAboutDlg::~CAboutDlg()
{
	if (!m_hIcon)
	{
		delete m_hIcon;
	}

	if (!m_hCursor)
	{
		delete m_hCursor;
	}
}

BOOL CAboutDlg::OnInitDialog()
{
	CString str;
	CString csEntry;
	CString csRet;
	CString str1;
	CString	sDisplayText;
	CString sVersion, sTxtId;
//	int iLangIdSelected;

	CDialog::OnInitDialog();

//	m_text.SetBkColor(RGB(255,255,255));
//	m_text.SetTextColor(RGB(0,0,255));

	LCID langsys = GetSystemDefaultLCID ();
	LCID languser = GetUserDefaultLCID();

	// translations

	sDisplayText = gszTranslation[PosDialogAboutTitle];
	sDisplayText+= " BoincTasks";
	SetWindowText(sDisplayText);

	sDisplayText = "\r\n";
	sDisplayText+= gszTranslation[PosDialogAboutText1];
	sDisplayText+= gszTranslation[PosDialogAboutTextProgrammer];
	sDisplayText+= " Fred Melgert";
	sDisplayText = sDisplayText + gszTranslation[PosDialogAboutTextTesters];
	sDisplayText+= "Corsair, Lammert, idahofisherman, Pepo, jjwhalen, wicked, glennaxl, Sekerob, Torsten, BeemerBiker, archae86, tlsi2000, John C, BobCat13,\r\n";
	sDisplayText+= " Beyond, SuperJoker, John McLeod VII, dskrbt, indians, coolstream, Bernt, Norman, thebestjaspreet, gubment.cheez, Mellowman, Colin, ElGuillermo,\r\n";
	sDisplayText+= " ReBoRnE33, BilBg, Tex1954, dougjj, Gord Sipko, brbecker, Ytterbium, GSD, TeeVeeEss, HighTech67, szopler, hucker.\r\n";
	sDisplayText+= "\r\n";
	sDisplayText+= gszTranslation[PosDialogAboutTextSupport];
	sDisplayText+= "eFMer, Mark James (6), David Anderson (3), Armand Medeiros (3), Art Brown (3), Sylvester Rademaker (2), Carl Caamano (2), John Kerr,\r\n";
	sDisplayText+= " Paul York, Peter Hardy, Teemu Mannermaa, Joseph Stateson, Peter Stoll, Richard Furnari, John Chastain, Yves Trehot, Tobias Greifzu,\r\n";
	sDisplayText+= " JoaoLuis Pena, Peter Slacik, Rick Goddard,Lars Jensen, Martin Duszynski Jr, MEB Consulting Services Inc., Jose Ernani, Polak Filho,\r\n";
	sDisplayText+= " Philipp Bliedung, Jan Ingmar Karlsson, Charles Cybulski, Tom Milinovich, Jefferson Lawrence, Peter Scott, Joseph Watterson,\r\n";
	sDisplayText+= " Bengt-Åke Johansson, Rob Beentjes, Earl T Koskie Jr, David Barstow, Mark Ritter, john michel, Yu-Hsien Ko, BOINC Poland,\r\n";
	sDisplayText+= " Thomas Koch, Alan Hanson, Ashley Wright, Fred Evans, Mike Green, Scott Ritchie, Piotr Wieczorek, Hartmut Schmoldt, Paul Bowyer,\r\n";
	sDisplayText+= " Charles Elliott, Trevor vanRijn, John McClelland, Ytterbium, Jose Ramirez, Jonathon G Morris, William White, Thosten Sorge (2), Lionel Cador, \r\n";
	sDisplayText+= " Step Williamson, Anthony Welch, John Janoch, Alan Hanson, S@NL-JBG, Robert Carter, Loren Vanstone, David McMillan, Douglas Orlowsky,\r\n";
	sDisplayText+= " Stephen Ellison, Greg Staunton - JugNut (3), William Plachy, SekeRob2, R.A.Hicks, Rodney Duane, Matthew Belben (2), Sean Humphries, Michael Hartsell,\r\n";
	sDisplayText+= " Lushchay Vladimir, Alain Denos, Steven Pepperell (2), Richard Roberts, Yeti, Daniel Wimmer, Richard Bassett, Wendell Dunn Iii, Damien Healy (6),\r\n";
	sDisplayText+= " John Burton, P McVeigh, Abhinav Singh, TeeVeeEss, Giles Averay-Jones, Thorsten Sambale, Jeremy Hanson, William Johnston, kiberdweller.\r\n";
	sDisplayText+= " Charles Elliott, Peter Wright, Mark Whitmore, Michael D Henderson (2), Joerg Steinmetz, Kari Hyvönen, Erik Veit (9), David Huffman-Gottschling.\r\n";
	sDisplayText+= " MWG Creations, Mark Hudson, Alexander Lagerberg, Mikhail Romanov, Time Logic Systems, Inc, Dirk Gudenschwager, Daniel McArdell.\r\n";
	sDisplayText+= " Websters computers, David Chapman.\r\n";
// updated upto 8-june-2023

	sDisplayText = sDisplayText + gszTranslation[PosDialogAboutText2];

	m_textTranslatedBy.SetWindowText(gszTranslation[PosDialogAboutCredits]);
	m_textTranslatedTeam.SetWindowText(gszTranslation[PosDialogAboutCreditsLink]);
	// translations

	m_hCursor = AfxGetApp()->LoadIcon(IDR_ICON_LINK_HAND);
	m_efmerHttp.SetLink(TRUE,FALSE);
	m_efmerHttp.SetTextColor(RGB(0,0,255));
	m_efmerHttp.SetFontUnderline(TRUE);
	m_efmerHttp.SetLinkCursor(m_hCursor);

	m_snlHttp.SetLink(TRUE,FALSE);
	m_snlHttp.SetTextColor(RGB(0,0,255));
	m_snlHttp.SetFontUnderline(TRUE);
	m_snlHttp.SetLinkCursor(m_hCursor);

	CString sTxt;
	sTxt = gszTranslation[PosDialogAboutCreditsLinkHttp];
	if (sTxt.GetLength() > 1)
	{
		m_textTranslatedTeam.SetLink(TRUE,FALSE);
		m_textTranslatedTeam.SetTextColor(RGB(0,0,255));
		m_textTranslatedTeam.SetFontUnderline(TRUE);
		m_textTranslatedTeam.SetLinkCursor(m_hCursor);
	}

	sVersion.LoadString(IDS_PROGRAM_VERSION);
	m_version.SetWindowText(sVersion);

	CRect rectText;
	m_text.GetClientRect(&rectText);
	m_dDeflateHeight = rectText.Height();
	m_dDeflateWidth = rectText.Width();

//	m_text.ShowWindow(SW_HIDE);

//	m_textPointer = 0;
//	m_textLen = m_displayText.GetLength();

	m_text.SetWindowText(sDisplayText);

	if (theApp.m_pTranslation->m_lcid == 1043)
	{
		m_bmpSnl.ShowWindow(SW_SHOW);
		m_snlHttp.ShowWindow(SW_SHOW);
	}

	SetTimer(UTIMER_DLG_ABOUT, 100, 0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ABOUT, m_text);
	DDX_Control(pDX, IDC_EFMER_HTTP, m_efmerHttp);
	DDX_Control(pDX, IDC_SNL_HTTP, m_snlHttp);
	DDX_Control(pDX, IDC_EDIT_VERSION, m_version);
	DDX_Control(pDX, IDC_TEXT_TRANSLATED_BY, m_textTranslatedBy);
	DDX_Control(pDX, IDC_TEXT_TRANSLATED_TEAM, m_textTranslatedTeam);
	DDX_Control(pDX, IDC_BITMAP_SNL, m_bmpSnl);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_STN_CLICKED(IDC_EFMER_HTTP, OnStnClickedEfmerHttp)
	ON_STN_CLICKED(IDC_SNL_HTTP, OnStnClickedSnlHttp)
	ON_STN_CLICKED(IDC_TEXT_TRANSLATED_TEAM, &CAboutDlg::OnStnClickedTextTranslatedTeam)
END_MESSAGE_MAP()


// CAboutDlg message handlers

void CAboutDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}

void CAboutDlg::OnStnClickedEfmerHttp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks/";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

void CAboutDlg::OnStnClickedSnlHttp()
{
	CString strLink;
	strLink = "https://seti.nl/nl/bij-ons-team-aansluiten/";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	CWnd *pWnd;
	CString sDisplay;
	pWnd = this;
	COLORREF	cr;

	if (m_dDeflateHeight > 0)
	{
		if (!pWnd->IsWindowVisible())
		{
			m_dDeflateHeight = 0;
		}
		m_dDeflateHeight -= 0.5;
	}

	if (m_dDeflateWidth > 0)
	{
		if (!pWnd->IsWindowVisible())
		{
			m_dDeflateWidth = 0;
		}
		m_dDeflateWidth -= 3.5;
	}

	if (m_dDeflateWidth > 0 || m_dDeflateHeight > 0)
	{
		if (!pWnd->IsWindowVisible())
		{
			m_dDeflateHeight = 0;
			m_dDeflateWidth = 0;
		}

		cr = COLORREF(RGB(0, 0, 255));
		m_text.PostMessage(UWM_MSG_TEXT_NEW_COLOR, cr, 0);

		int iWidth, iHeight;

		iHeight = (int) m_dDeflateHeight;
		iWidth = (int) m_dDeflateWidth;

		m_text.PostMessage(UWM_MSG_TEXT_NEW_SIZE, iWidth, iHeight);
	}
	else
	{
		CRect rectText;
		m_text.GetClientRect(&rectText);
		m_dDeflateWidth = rectText.Width();
		m_dDeflateHeight = rectText.Height();

		m_text.PostMessage(UWM_MSG_TEXT_NEW_SIZE, 0, 0);

		KillTimer(UTIMER_DLG_ABOUT);
	}


}

void CAboutDlg::OnShowWindow( BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		SetTimer(UTIMER_DLG_ABOUT, 25, 0);
	}
	else
	{
//		m_text.PostMessage(UWM_MSG_TEXT_NEW_HEIGHT,0, m_iDeflateHeight);
	}
}

void CAboutDlg::OnStnClickedTextTranslatedTeam()
{
	CString strLink;
	strLink = gszTranslation[PosDialogAboutCreditsLinkHttp];

	if (strLink.GetLength() > 1) ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

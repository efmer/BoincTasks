// DlgColorMessages.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorMessages.h"
#include "Translation.h"

// CDlgColorMessages dialog

IMPLEMENT_DYNAMIC(CDlgColorMessages, CPropertyPage)

CDlgColorMessages::CDlgColorMessages()
	: CPropertyPage(CDlgColorMessages::IDD)
{

}

CDlgColorMessages::~CDlgColorMessages()
{
}

BOOL CDlgColorMessages::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	m_colorMessages[COLOR_MESSAGES_ARRAY_1]	= theApp.GetProfileInt(registrySectionColor, registryColorMessages1, COLOR_MESSAGES_1);
	m_colorMessages[COLOR_MESSAGES_ARRAY_2]	= theApp.GetProfileInt(registrySectionColor, registryColorMessages2, COLOR_MESSAGES_2);
	m_colorMessages[COLOR_MESSAGES_ARRAY_3]	= theApp.GetProfileInt(registrySectionColor, registryColorMessages3, COLOR_MESSAGES_3);

	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_1]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl1, COLOR_MESSAGES_H1);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_2]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl2, COLOR_MESSAGES_H2);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_3]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl3, COLOR_MESSAGES_H3);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_4]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl4, COLOR_MESSAGES_H4);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_5]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl5, COLOR_MESSAGES_H5);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_6]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl6, COLOR_MESSAGES_H6);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_7]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl7, COLOR_MESSAGES_H7);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_8]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl8, COLOR_MESSAGES_H8);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_9]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl9, COLOR_MESSAGES_H9);
	m_colorMessages[COLOR_MESSAGES_ARRAY_HL_10]	= theApp.GetProfileInt(registrySectionColor, registryColorMessagesHl10,COLOR_MESSAGES_H10);

	m_bColorWindowUpdate= true;

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleMessages];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonNormal.SetWindowText(gszTranslation[PosDialogColorMessageNormal]);
	m_buttonHigh.SetWindowText(gszTranslation[PosDialogColorMessageHigh]);
	m_buttonHighest.SetWindowText(gszTranslation[PosDialogColorMessageHighest]);
	m_buttonHighlight1.SetWindowText(gszTranslation[PosDialogColorMessageHighlight1]);
	m_buttonHighlight2.SetWindowText(gszTranslation[PosDialogColorMessageHighlight2]);
	m_buttonHighlight3.SetWindowText(gszTranslation[PosDialogColorMessageHighlight3]);
	m_buttonHighlight4.SetWindowText(gszTranslation[PosDialogColorMessageHighlight4]);
	m_buttonHighlight5.SetWindowText(gszTranslation[PosDialogColorMessageHighlight5]);
	m_buttonHighlight6.SetWindowText(gszTranslation[PosDialogColorMessageHighlight6]);
	m_buttonHighlight7.SetWindowText(gszTranslation[PosDialogColorMessageHighlight7]);
	m_buttonHighlight8.SetWindowText(gszTranslation[PosDialogColorMessageHighlight8]);
	m_buttonHighlight9.SetWindowText(gszTranslation[PosDialogColorMessageHighlight9]);
	m_buttonHighlight10.SetWindowText(gszTranslation[PosDialogColorMessageHighlight10]);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
// Translation

	SetTimer(UTIMER_DLG_COLOR_MESSAGES, 1000, 0);	// interval 1 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

COLORREF *CDlgColorMessages::GetColorArray()
{
	return &m_colorMessages[0];
}

void CDlgColorMessages::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColor,registryColorMessages1 , m_colorMessages[COLOR_MESSAGES_ARRAY_1]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessages2 , m_colorMessages[COLOR_MESSAGES_ARRAY_2]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessages3 , m_colorMessages[COLOR_MESSAGES_ARRAY_3]);

	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl1 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_1]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl2 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_2]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl3 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_3]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl4 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_4]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl5 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_5]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl6 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_6]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl7 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_7]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl8 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_8]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl9 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_9]);
	theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl10, m_colorMessages[COLOR_MESSAGES_ARRAY_HL_10]);

	m_bColorWindowUpdate= true;
}

void CDlgColorMessages::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_1, m_wndMessages[COLOR_MESSAGES_ARRAY_1]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_2, m_wndMessages[COLOR_MESSAGES_ARRAY_2]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_3, m_wndMessages[COLOR_MESSAGES_ARRAY_3]);

	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_1, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_1]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_2, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_2]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_3, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_3]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_4, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_4]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_5, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_5]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_6, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_6]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_7, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_7]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_8, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_8]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_9, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_9]);
	DDX_Control(pDX, IDC_WND_COLOR_MESSAGES_HL_10, m_wndMessages[COLOR_MESSAGES_ARRAY_HL_10]);

	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_1, m_buttonNormal);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_2, m_buttonHigh);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_3, m_buttonHighest);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_1, m_buttonHighlight1);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_2, m_buttonHighlight2);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_3, m_buttonHighlight3);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_4, m_buttonHighlight4);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_5, m_buttonHighlight5);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_6, m_buttonHighlight6);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_7, m_buttonHighlight7);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_8, m_buttonHighlight8);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_9, m_buttonHighlight9);
	DDX_Control(pDX, IDC_BUTTON_COLOR_MESSAGES_HL_10, m_buttonHighlight10);
}


BEGIN_MESSAGE_MAP(CDlgColorMessages, CPropertyPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_1, &CDlgColorMessages::OnBnClickedButtonColorMessages1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_2, &CDlgColorMessages::OnBnClickedButtonColorMessages2)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_3, &CDlgColorMessages::OnBnClickedButtonColorMessages3)
	ON_BN_CLICKED(IDOK, &CDlgColorMessages::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_1, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_2, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl2)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_3, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl3)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_4, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl4)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_5, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl5)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_6, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl6)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_7, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl7)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_8, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl8)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_9, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl9)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_MESSAGES_HL_10, &CDlgColorMessages::OnBnClickedButtonColorMessagesHl10)
END_MESSAGE_MAP()

void CDlgColorMessages::OnTimer(UINT_PTR nIDEvent)
{
	int iCount;
	if (m_bColorWindowUpdate)
	{
		m_bColorWindowUpdate = false;
		for (iCount = 0; iCount < COLOR_ARRAY_NUMBER_MESSAGES; iCount++)
		{
			m_wndMessages[iCount].SetColor(m_colorMessages[iCount]);
			m_wndMessages[iCount].Invalidate(TRUE);
		}
//		::PostMessage(m_hWnd, WM_GRAPHIC_COLOR_CHANGED, 0, 0);
	}
}

// CDlgColorMessages message handlers

void CDlgColorMessages::OnBnClickedButtonColorMessages1()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_1],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_1] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessages1 , m_colorMessages[COLOR_MESSAGES_ARRAY_1]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessages2()
{	
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_2],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_2] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessages2 , m_colorMessages[COLOR_MESSAGES_ARRAY_2]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessages3()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_3],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_3] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessages3 , m_colorMessages[COLOR_MESSAGES_ARRAY_3]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl1()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_1],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_1] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl1 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_1]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorMessages::OnBnClickedButtonColorMessagesHl2()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_2],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_2] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl2 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_2]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorMessages::OnBnClickedButtonColorMessagesHl3()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_3],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_3] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl3 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_3]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl4()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_4],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_4] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl4 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_4]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl5()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_5],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_5] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl5 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_5]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl6()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_6],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_6] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl6 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_6]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl7()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_7],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_7] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl7 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_7]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl8()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_8],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_8] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl8 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_8]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl9()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_9],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_9] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl9 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_9]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorMessages::OnBnClickedButtonColorMessagesHl10()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorMessages[COLOR_MESSAGES_ARRAY_HL_10],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorMessages[COLOR_MESSAGES_ARRAY_HL_10] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorMessagesHl10 , m_colorMessages[COLOR_MESSAGES_ARRAY_HL_10]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

COLORREF CDlgColorMessages::Correct(COLORREF color)
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
void CDlgColorMessages::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}




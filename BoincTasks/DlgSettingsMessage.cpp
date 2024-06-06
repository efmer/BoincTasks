// DlgSettingsHistory.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"	
#include "TemplateRemoteMessageView.h"
#include "RemoteCombinedMessageView.h"
#include "RemoteDocTemplate.h"
#include "RemoteCombinedDoc.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsMessage.h"
#include "DlgRulesLogging.h"
#include "DlgLogging.h"
#include "ThreadRpc.h"
#include "Translation.h"

// CDlgSettingsMessage dialog

IMPLEMENT_DYNAMIC(CDlgSettingsMessage, CPropertyPage)

CDlgSettingsMessage::CDlgSettingsMessage()
	: CPropertyPage(CDlgSettingsMessage::IDD)
{

}

CDlgSettingsMessage::~CDlgSettingsMessage()
{
}

BOOL CDlgSettingsMessage::OnInitDialog()
{
	CPropertySheet *pSheet;
	int iNr;
	CString sTxt;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	iNr = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsMessageTitle];
    pTabCtrl->SetItem(iNr,&tcItem);

	m_textMessagesKeep.SetWindowText(gszTranslation[PosGroupDialogSettingsMessagesKeep]);
	m_textMessagesProject.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageProject]);
	m_textMessagesMessage.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageMessage]);
	m_textMessagesHl1.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight1]);
	m_textMessagesHl2.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight2]);
	m_textMessagesHl3.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight3]);
	m_textMessagesHl4.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight4]);
	m_textMessagesHl5.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight5]);
	m_textMessagesHl6.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight6]);
	m_textMessagesHl7.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight7]);
	m_textMessagesHl8.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight8]);
	m_textMessagesHl9.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight9]);
	m_textMessagesHl10.SetWindowText(gszTranslation[PosGroupDialogSettingsMessageHighlight10]);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	ReadColumsFromRegistry();

	m_iKeepMessages = theApp.GetProfileInt(registrySectionSettings, registryMessagesKeep, DEFAULT_KEEP_MESSAGES);
	if (m_iKeepMessages <= 0) m_iKeepMessages = DEFAULT_KEEP_MESSAGES;
	sTxt.Format("%d", m_iKeepMessages);
	m_editKeepMessages.SetWindowText(sTxt);
	if (m_iKeepMessages > 2000) m_bMessagesKeep = true;
	else m_bMessagesKeep = false;

	m_sHighlightProject[0] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl1, "");m_editProjectHl1.SetWindowText(m_sHighlightProject[0]);m_sHighlightProject[0].MakeLower();
	m_sHighlightProject[1] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl2, "");m_editProjectHl2.SetWindowText(m_sHighlightProject[1]);m_sHighlightProject[1].MakeLower();
	m_sHighlightProject[2] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl3, "");m_editProjectHl3.SetWindowText(m_sHighlightProject[2]);m_sHighlightProject[2].MakeLower();
	m_sHighlightProject[3] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl4, "");m_editProjectHl4.SetWindowText(m_sHighlightProject[3]);m_sHighlightProject[3].MakeLower();
	m_sHighlightProject[4] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl5, "");m_editProjectHl5.SetWindowText(m_sHighlightProject[4]);m_sHighlightProject[4].MakeLower();
	m_sHighlightProject[5] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl6, "");m_editProjectHl6.SetWindowText(m_sHighlightProject[5]);m_sHighlightProject[5].MakeLower();
	m_sHighlightProject[6] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl7, "");m_editProjectHl7.SetWindowText(m_sHighlightProject[6]);m_sHighlightProject[6].MakeLower();
	m_sHighlightProject[7] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl8, "");m_editProjectHl8.SetWindowText(m_sHighlightProject[7]);m_sHighlightProject[7].MakeLower();
	m_sHighlightProject[8] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl9, "");m_editProjectHl9.SetWindowText(m_sHighlightProject[8]);m_sHighlightProject[8].MakeLower();
	m_sHighlightProject[9] = theApp.GetProfileString(registrySectionSettings, registryMessagesProjectHl10, "");m_editProjectHl10.SetWindowText(m_sHighlightProject[9]);m_sHighlightProject[9].MakeLower();

	m_sHighlight[0] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl1, "");m_editHl1.SetWindowText(m_sHighlight[0]);m_sHighlight[0].MakeLower();
	m_sHighlight[1] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl2, "");m_editHl2.SetWindowText(m_sHighlight[1]);m_sHighlight[1].MakeLower();
	m_sHighlight[2] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl3, "");m_editHl3.SetWindowText(m_sHighlight[2]);m_sHighlight[2].MakeLower();
	m_sHighlight[3] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl4, "");m_editHl4.SetWindowText(m_sHighlight[3]);m_sHighlight[3].MakeLower();
	m_sHighlight[4] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl5, "");m_editHl5.SetWindowText(m_sHighlight[4]);m_sHighlight[4].MakeLower();
	m_sHighlight[5] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl6, "");m_editHl6.SetWindowText(m_sHighlight[5]);m_sHighlight[5].MakeLower();
	m_sHighlight[6] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl7, "");m_editHl7.SetWindowText(m_sHighlight[6]);m_sHighlight[6].MakeLower();
	m_sHighlight[7] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl8, "");m_editHl8.SetWindowText(m_sHighlight[7]);m_sHighlight[7].MakeLower();
	m_sHighlight[8] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl9, "");m_editHl9.SetWindowText(m_sHighlight[8]);m_sHighlight[8].MakeLower();
	m_sHighlight[9] = theApp.GetProfileString(registrySectionSettings, registryMessagesHl10, "");m_editHl10.SetWindowText(m_sHighlight[9]);m_sHighlight[9].MakeLower();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsMessage::ReadColumsFromRegistry()
{
	int		iNr;

	iNr = theApp.GetProfileInt(registrySectionColumn, registryMessageColumn0, COLUMN_MESSAGE_NR+1);		m_iMessageColumnIn[0] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryMessageColumn1, COLUMN_MESSAGE_PROJECT+1);m_iMessageColumnIn[1] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryMessageColumn2, COLUMN_MESSAGE_TIME+1);	m_iMessageColumnIn[2] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryMessageColumn3, COLUMN_MESSAGE_MESSAGE+1);m_iMessageColumnIn[3] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryMessageColumn4, COLUMN_MESSAGE_COMPUTER+1);m_iMessageColumnIn[4] = iNr;
	MessageColumns();
}

void CDlgSettingsMessage::WriteColumsToRegistry()
{
	theApp.WriteProfileInt(registrySectionColumn, registryMessageColumn0, m_iMessageColumnIn[0]);
	theApp.WriteProfileInt(registrySectionColumn, registryMessageColumn1, m_iMessageColumnIn[1]);
	theApp.WriteProfileInt(registrySectionColumn, registryMessageColumn2, m_iMessageColumnIn[2]);
	theApp.WriteProfileInt(registrySectionColumn, registryMessageColumn3, m_iMessageColumnIn[3]);
	theApp.WriteProfileInt(registrySectionColumn, registryMessageColumn4, m_iMessageColumnIn[4]);
}

void CDlgSettingsMessage::MessageColumns()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
	{
		m_iMessageColumnLookup[iCount] = -1;
		m_iMessageColumn[iCount] = -1;
	}
	m_iActiveColumns = 0;
	for (iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
	{
		if (m_iMessageColumnIn[iCount] > 0)
		{
			if (m_iMessageColumnIn[iCount] <= NUM_REMOTE_MESSAGE_COLUMNS)
			{
				m_iMessageColumn[m_iActiveColumns] = m_iMessageColumnIn[iCount] -1;
				m_iActiveColumns++;
			}
		}
	}	
	for (iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
	{
		iNr = m_iMessageColumn[iCount];
		if (iNr >=0) m_iMessageColumnLookup[iNr] = iCount;
	}
}

void CDlgSettingsMessage::InvalidateMessages()
{
	CRemoteCombinedDoc	*pDoc;
	POSITION	posDoc;

	posDoc = theApp.m_pDocTemplateComputerCombinedRemote->GetFirstDocPosition();
	if (posDoc != NULL)
	{
		pDoc = (CRemoteCombinedDoc *) theApp.m_pDocTemplateComputerCombinedRemote->GetNextDoc(posDoc);
		if (pDoc != NULL)
		{
			pDoc->m_pMessageView->PostMessage(UWM_MSG_DLG_SETTINGS_MESSAGES_INVALIDATE,0,0);
		}
	}
}

void CDlgSettingsMessage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_KEEP, m_textMessagesKeep);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_KEEP, m_editKeepMessages);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_1, m_textMessagesHl1);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_2, m_textMessagesHl2);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_3, m_textMessagesHl3);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_4, m_textMessagesHl4);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_5, m_textMessagesHl5);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_6, m_textMessagesHl6);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_7, m_textMessagesHl7);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_8, m_textMessagesHl8);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_9, m_textMessagesHl9);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_HL_10, m_textMessagesHl10);

	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1, m_editHl1);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_2, m_editHl2);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_3, m_editHl3);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_4, m_editHl4);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_5, m_editHl5);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_6, m_editHl6);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_7, m_editHl7);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_8, m_editHl8);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_9, m_editHl9);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_10, m_editHl10);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_PROJECT, m_textMessagesProject);
	DDX_Control(pDX, IDC_TEXT_MESSAGES_MESSAGE, m_textMessagesMessage);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT, m_editProjectHl1);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT2, m_editProjectHl2);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT3, m_editProjectHl3);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT4, m_editProjectHl4);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT5, m_editProjectHl5);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT6, m_editProjectHl6);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT7, m_editProjectHl7);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT8, m_editProjectHl8);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT9, m_editProjectHl9);
	DDX_Control(pDX, IDC_EDIT_MESSAGES_HL_1_PROJECT10, m_editProjectHl10);
}


BEGIN_MESSAGE_MAP(CDlgSettingsMessage, CPropertyPage)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_MESSAGES_GET, OnSettingsMessageGet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_MESSAGES_SET, OnSettingsMessageSet) 
	ON_BN_CLICKED(IDOK, &CDlgSettingsMessage::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_KEEP, &CDlgSettingsMessage::OnEnChangeEditMessagesKeep)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsMessage::OnBnClickedHelp)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_2, &CDlgSettingsMessage::OnEnChangeEditMessagesHl2)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_3, &CDlgSettingsMessage::OnEnChangeEditMessagesHl3)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_4, &CDlgSettingsMessage::OnEnChangeEditMessagesHl4)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_5, &CDlgSettingsMessage::OnEnChangeEditMessagesHl5)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_6, &CDlgSettingsMessage::OnEnChangeEditMessagesHl6)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_7, &CDlgSettingsMessage::OnEnChangeEditMessagesHl7)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_8, &CDlgSettingsMessage::OnEnChangeEditMessagesHl8)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_9, &CDlgSettingsMessage::OnEnChangeEditMessagesHl9)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_10, &CDlgSettingsMessage::OnEnChangeEditMessagesHl10)

	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT2, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project2)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT3, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project3)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT4, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project4)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT5, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project5)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT6, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project6)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT7, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project7)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT8, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project8)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT9, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project9)
	ON_EN_CHANGE(IDC_EDIT_MESSAGES_HL_1_PROJECT10, &CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project10)
END_MESSAGE_MAP()

 
// CDlgSettingsMessage message handlers

LRESULT CDlgSettingsMessage::OnSettingsMessageGet(WPARAM parm1, LPARAM parm2)
{
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
	{
		*pArrayColumn = m_iMessageColumn[iCount];
		pArrayColumn++;
		*pArrayLookup = m_iMessageColumnLookup[iCount];
		*pArrayLookup++;
	}

	return m_iActiveColumns;
}

LRESULT CDlgSettingsMessage::OnSettingsMessageSet(WPARAM parm1, LPARAM parm2)
{
//	int iNr;
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
	{
		m_iMessageColumnIn[iCount] = (*pArrayColumn)+1;
		pArrayColumn++;
	}
	WriteColumsToRegistry();
	MessageColumns();

	return 0;
}

void CDlgSettingsMessage::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesKeep()
{
	CString sValue;

	m_editKeepMessages.GetWindowText(sValue);
	m_iKeepMessages = atoi(sValue);

	if (m_iKeepMessages < 100) m_iKeepMessages = 100;

	if (m_iKeepMessages <= 0) m_iKeepMessages = DEFAULT_KEEP_MESSAGES;
	theApp.WriteProfileInt(registrySectionSettings, registryMessagesKeep, m_iKeepMessages);

	if (m_iKeepMessages > 2000) m_bMessagesKeep = true;
	else m_bMessagesKeep = false;
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project()
{
	m_editProjectHl1.GetWindowText(m_sHighlightProject[0]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl1, m_sHighlightProject[0]);m_sHighlightProject[0].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1()
{
	m_editHl1.GetWindowText(m_sHighlight[0]);
	theApp.WriteProfileString(registrySectionSettings, registryMessagesHl1, m_sHighlight[0]);m_sHighlight[0].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project2()
{
	m_editProjectHl2.GetWindowText(m_sHighlightProject[1]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl2, m_sHighlightProject[1]);m_sHighlightProject[1].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl2()
{
	m_editHl2.GetWindowText(m_sHighlight[1]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl2, m_sHighlight[1]);m_sHighlight[1].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project3()
{
	m_editProjectHl3.GetWindowText(m_sHighlightProject[2]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl3, m_sHighlightProject[2]);m_sHighlightProject[2].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl3()
{
	m_editHl3.GetWindowText(m_sHighlight[2]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl3, m_sHighlight[2]);m_sHighlight[2].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project4()
{
	m_editProjectHl4.GetWindowText(m_sHighlightProject[3]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl4, m_sHighlightProject[3]);m_sHighlightProject[3].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl4()
{
	m_editHl4.GetWindowText(m_sHighlight[3]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl4, m_sHighlight[3]);m_sHighlight[3].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project5()
{
	m_editProjectHl5.GetWindowText(m_sHighlightProject[4]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl5, m_sHighlightProject[4]);m_sHighlightProject[4].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl5()
{
	m_editHl5.GetWindowText(m_sHighlight[4]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl5, m_sHighlight[4]);m_sHighlight[4].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project6()
{
	m_editProjectHl6.GetWindowText(m_sHighlightProject[5]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl6, m_sHighlightProject[5]);m_sHighlightProject[5].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl6()
{
	m_editHl6.GetWindowText(m_sHighlight[5]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl6, m_sHighlight[5]);m_sHighlight[5].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project7()
{
	m_editProjectHl7.GetWindowText(m_sHighlightProject[6]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl7, m_sHighlightProject[6]);m_sHighlightProject[6].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl7()
{
	m_editHl7.GetWindowText(m_sHighlight[6]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl7, m_sHighlight[6]);m_sHighlight[6].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project8()
{
	m_editProjectHl8.GetWindowText(m_sHighlightProject[7]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl8, m_sHighlightProject[7]);m_sHighlightProject[7].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl8()
{
	m_editHl8.GetWindowText(m_sHighlight[7]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl8, m_sHighlight[7]);m_sHighlight[7].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project9()
{
	m_editProjectHl9.GetWindowText(m_sHighlightProject[8]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl9, m_sHighlightProject[8]);m_sHighlightProject[8].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl9()
{
	m_editHl9.GetWindowText(m_sHighlight[8]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl9, m_sHighlight[8]);m_sHighlight[8].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl1Project10()
{
	m_editProjectHl10.GetWindowText(m_sHighlightProject[9]);theApp.WriteProfileString(registrySectionSettings, registryMessagesProjectHl10, m_sHighlightProject[9]);m_sHighlightProject[9].MakeLower();
	InvalidateMessages();
}

void CDlgSettingsMessage::OnEnChangeEditMessagesHl10()
{
	m_editHl10.GetWindowText(m_sHighlight[9]);theApp.WriteProfileString(registrySectionSettings, registryMessagesHl10, m_sHighlight[9]);m_sHighlight[9].MakeLower();
	InvalidateMessages();
}

BOOL CDlgSettingsMessage::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsMessage::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#messages";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}













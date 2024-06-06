// DlgSettingsHistory.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"	
#include "TemplateRemoteTransferView.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsTransfer.h"
#include "DlgRulesLogging.h"
#include "DlgLogging.h"
#include "ThreadRpc.h"
#include "Translation.h"

// CDlgSettingsTransfer dialog

IMPLEMENT_DYNAMIC(CDlgSettingsTransfer, CPropertyPage)

CDlgSettingsTransfer::CDlgSettingsTransfer()
	: CPropertyPage(CDlgSettingsTransfer::IDD)
{

}

CDlgSettingsTransfer::~CDlgSettingsTransfer()
{
}

BOOL CDlgSettingsTransfer::OnInitDialog()
{
	CPropertySheet *pSheet;
//	int iCheck;
	int iNr;
	CString sFormat;

	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	iNr = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsTransferTitle];
    pTabCtrl->SetItem(iNr,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	// translations

//	BOOL bStatus;
//	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
//	m_buttonHelp.SetBitmap(m_bmp);

	ReadColumsFromRegistry();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsTransfer::ReadColumsFromRegistry()
{
	int		iNr;

	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn0, COLUMN_TRANSFER_PROJECT+1);	m_iTransferColumnIn[0] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn1, COLUMN_TRANSFER_FILE+1);		m_iTransferColumnIn[1] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn2, COLUMN_TRANSFER_PROGRESS+1);	m_iTransferColumnIn[2] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn3, COLUMN_TRANSFER_SIZE+1);		m_iTransferColumnIn[3] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn4, COLUMN_TRANSFER_ELAPSED+1);	m_iTransferColumnIn[4] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn5, COLUMN_TRANSFER_SPEED+1);		m_iTransferColumnIn[5] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn6, COLUMN_TRANSFER_STATUS+1);	m_iTransferColumnIn[6] = iNr;
	iNr = theApp.GetProfileInt(registrySectionColumn, registryTransferColumn7, COLUMN_TRANSFER_COMPUTER+1);	m_iTransferColumnIn[7] = iNr;
	TransferColumns();
//	EnableCheck();
}

void CDlgSettingsTransfer::WriteColumsToRegistry()
{
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn0, m_iTransferColumnIn[0]);
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn1, m_iTransferColumnIn[1]);
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn2, m_iTransferColumnIn[2]);
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn3, m_iTransferColumnIn[3]);
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn4, m_iTransferColumnIn[4]);
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn5, m_iTransferColumnIn[5]);
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn6, m_iTransferColumnIn[6]);
	theApp.WriteProfileInt(registrySectionColumn, registryTransferColumn7, m_iTransferColumnIn[7]);
}

void CDlgSettingsTransfer::TransferColumns()
{
	int iCount, iNr;

	for (iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
	{
		m_iTransferColumnLookup[iCount] = -1;
		m_iTransferColumn[iCount] = -1;
	}
	m_iActiveColumns = 0;
	for (iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
	{
		if (m_iTransferColumnIn[iCount] > 0)
		{
			if (m_iTransferColumnIn[iCount] <= NUM_REMOTE_TRANSFER_COLUMNS)
			{
				m_iTransferColumn[m_iActiveColumns] = m_iTransferColumnIn[iCount] -1;
				m_iActiveColumns++;
			}
		}
	}	
	for (iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
	{
		iNr = m_iTransferColumn[iCount];
		if (iNr >=0) m_iTransferColumnLookup[iNr] = iCount;
	}
}

void CDlgSettingsTransfer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_buttonOk);
}


BEGIN_MESSAGE_MAP(CDlgSettingsTransfer, CPropertyPage)
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TRANSFERS_GET, OnSettingsTransferGet) 
	ON_MESSAGE (UWM_MSG_DLG_SETTINGS_TRANSFERS_SET, OnSettingsTransferSet) 

	ON_BN_CLICKED(IDOK, &CDlgSettingsTransfer::OnBnClickedOk)
END_MESSAGE_MAP()

 
// CDlgSettingsTransfer message handlers

LRESULT CDlgSettingsTransfer::OnSettingsTransferGet(WPARAM parm1, LPARAM parm2)
{
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
	{
		*pArrayColumn = m_iTransferColumn[iCount];
		pArrayColumn++;
		*pArrayLookup = m_iTransferColumnLookup[iCount];
		*pArrayLookup++;
	}

	return m_iActiveColumns;
}

LRESULT CDlgSettingsTransfer::OnSettingsTransferSet(WPARAM parm1, LPARAM parm2)
{
//	int iNr;
	int *pArrayLookup = (int *) parm1;
	int *pArrayColumn = (int *) parm2;

	for (int iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
	{
		m_iTransferColumnIn[iCount] = (*pArrayColumn)+1;
		pArrayColumn++;
	}
	WriteColumsToRegistry();
	TransferColumns();

	return 0;
}

void CDlgSettingsTransfer::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

// DgPrefExclusive.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgPrefExclusive.h"
#include "DlgPrefMain.h"
#include "DgPrefExclusiveAdd.h"
#include "afxdialogex.h"

#include "Translation.h"

// CDgPrefExclusive dialog

IMPLEMENT_DYNAMIC(CDgPrefExclusive, CPropertyPage)


static int _gnRulesFmt[NUM_EXCLUSIVE_COLUMNS] =
{
	LVCFMT_LEFT,	LVCFMT_LEFT,	LVCFMT_LEFT
};

static int _gnRulesWidth[NUM_EXCLUSIVE_COLUMNS] =
{
	100,			420,			0	
};

CDgPrefExclusive::CDgPrefExclusive()
	: CPropertyPage(CDgPrefExclusive::IDD)
{

}

CDgPrefExclusive::~CDgPrefExclusive()
{

}

BOOL CDgPrefExclusive::OnInitDialog()
{
	CDialog::OnInitDialog();


	CPropertySheet *pSheet;

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogBoincSettingsExclusiveTitle];
	pTabCtrl->SetItem(iSel,&tcItem);


	m_buttonEdit.SetWindowText(gszTranslation[PosDialogCommonButtonsEdit]);
	m_buttonAdd.SetWindowText(gszTranslation[PosDialogCommonButtonsAdd]);
	m_buttonDelete.SetWindowText(gszTranslation[PosDialogCommonButtonsDelete]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
	// translations
	AddColumns();

//	m_StateImageList.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
//	m_list.SetImageList(&m_StateImageList, LVSIL_STATE);

	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	
	m_bChanged = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

bool CDgPrefExclusive::ReadDialog(CRpcConfig *pConfig)
{
	int iItem, iItemCount;
	CString sTxtCpuGpu, sTxtExe;

	if (m_bChanged)
	{
		pConfig->m_pConfig->exclusive_apps.clear();
		pConfig->m_pConfig->exclusive_gpu_apps.clear();
		iItemCount = m_list.GetItemCount();

		for (iItem = 0; iItem < iItemCount; iItem++)
		{
			sTxtCpuGpu = m_list.GetItemText(iItem,0);
			sTxtExe = m_list.GetItemText(iItem,1);
			if (sTxtCpuGpu == EXCLUSIVE_TEXT_CPU_GPU)
			{
				std::string s = sTxtExe;
				pConfig->m_pConfig->exclusive_apps.push_back(s);
			}
			else
			{
				std::string s = sTxtExe;
				pConfig->m_pConfig->exclusive_gpu_apps.push_back(s);
			}
		}
		m_bChanged = false;
		return true;
	}
	m_bChanged = false;
	return false;
}

void CDgPrefExclusive::AddColumns()
{
	LV_COLUMN	lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iCount = 0; iCount < NUM_EXCLUSIVE_COLUMNS; iCount++)
	{
		lvc.iSubItem = iCount;
		switch (iCount)
		{
			case 0:
				lvc.pszText	= "";
			break;
			case 1:
				lvc.pszText	= "";
			break;
			case 2:
				lvc.pszText = "";
			break;
			case 3:
				lvc.pszText = "";
			break;

		}
		lvc.fmt = _gnRulesFmt[iCount];
		lvc.cx = _gnRulesWidth[iCount];
		m_list.InsertColumn(iCount,&lvc);
	}
}

void CDgPrefExclusive::EnableButtons(BOOL bEnable)
{
	m_buttonDelete.EnableWindow(bEnable);
	m_buttonEdit.EnableWindow(bEnable);;
}

void CDgPrefExclusive::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXCLUDE, m_list);
	DDX_Control(pDX, ID_BUTTON_ADD_EXCLUSIVE, m_buttonAdd);
	DDX_Control(pDX, ID_BUTTON_DELETE_EXCLUSIVE, m_buttonDelete);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, ID_BUTTON_EDIT_EXCLUSIVE, m_buttonEdit);
}


BEGIN_MESSAGE_MAP(CDgPrefExclusive, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CDgPrefExclusive::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_ADD_EXCLUSIVE, &CDgPrefExclusive::OnBnClickedButtonAddExclusive)
	ON_BN_CLICKED(ID_BUTTON_DELETE_EXCLUSIVE, &CDgPrefExclusive::OnBnClickedButtonDeleteExclusive)
	ON_BN_CLICKED(IDCANCEL, &CDgPrefExclusive::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BUTTON_EDIT_EXCLUSIVE, &CDgPrefExclusive::OnBnClickedButtonEditExclusive)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EXCLUDE, &CDgPrefExclusive::OnLvnItemchangedListExclude)
	ON_MESSAGE (UWM_MSG_DIALOG_BOINC_SETTINGS_FILL, OnFillDialog) 


END_MESSAGE_MAP()


// CDgPrefExclusive message handlers


LRESULT CDgPrefExclusive::OnFillDialog(WPARAM parm1, LPARAM parm2)
{
	int iPos, iItemCount;
	CRpcConfig *pRpcConfig;

	pRpcConfig = (CRpcConfig *) parm2;

	m_list.DeleteAllItems();

	if (pRpcConfig->m_bValid)
	{
		for (iPos = 0; iPos < (int) pRpcConfig->m_pConfig->exclusive_apps.size(); iPos++)
		{
			iItemCount = m_list.GetItemCount();
			m_list.InsertItem(iItemCount," ");
			m_list.SetItemState(iItemCount,INDEXTOSTATEIMAGEMASK(1),LVIS_STATEIMAGEMASK);
			std::string sText = pRpcConfig->m_pConfig->exclusive_apps.at(iPos);
			m_list.SetItemText(iItemCount,0,EXCLUSIVE_TEXT_CPU_GPU);
			m_list.SetItemText(iItemCount,1,sText.c_str());
		}
		for (iPos = 0; iPos < (int) pRpcConfig->m_pConfig->exclusive_gpu_apps.size(); iPos++)
		{
			iItemCount = m_list.GetItemCount();
			m_list.InsertItem(iItemCount," ");
			m_list.SetItemState(iItemCount,INDEXTOSTATEIMAGEMASK(1),LVIS_STATEIMAGEMASK);
			std::string sText = pRpcConfig->m_pConfig->exclusive_gpu_apps.at(iPos);
			m_list.SetItemText(iItemCount,0,EXCLUSIVE_TEXT_GPU);
			m_list.SetItemText(iItemCount,1,sText.c_str());
		}
	}
	return 0;
}

void CDgPrefExclusive::OnBnClickedButtonEditExclusive()
{
	int iResult, iItem;

	CDgPrefExclusiveAdd dlgPrefExclusiveAdd;
	dlgPrefExclusiveAdd.m_bSet = false;

	POSITION pos;
	pos = m_list.GetFirstSelectedItemPosition();
	if (pos)
	{
		iItem = m_list.GetNextSelectedItem(pos);
		if (iItem >= 0)
		{
			dlgPrefExclusiveAdd.m_sCpuGpu = m_list.GetItemText(iItem,0);
			dlgPrefExclusiveAdd.m_sExe = m_list.GetItemText(iItem,1);
			dlgPrefExclusiveAdd.m_bSet = true;
		}
	}

	iResult = (int) dlgPrefExclusiveAdd.DoModal();
	if (iResult == IDOK)
	{
		if (dlgPrefExclusiveAdd.m_sExe.GetLength() > 0)
		{
			m_list.SetItemState(iItem,INDEXTOSTATEIMAGEMASK(1),LVIS_STATEIMAGEMASK);
			m_list.SetItemText(iItem,0,dlgPrefExclusiveAdd.m_sCpuGpu);
			m_list.SetItemText(iItem,1,dlgPrefExclusiveAdd.m_sExe);
		}
	}
	EnableButtons(FALSE);
	m_bChanged = true;
}

void CDgPrefExclusive::OnBnClickedButtonAddExclusive()
{
	int iResult;

	CDgPrefExclusiveAdd dlgPrefExclusiveAdd;

	dlgPrefExclusiveAdd.m_bSet = false;

	iResult = (int) dlgPrefExclusiveAdd.DoModal();
	if (iResult == IDOK)
	{
		if (dlgPrefExclusiveAdd.m_sExe.GetLength() > 0)
		{
			int iItemCount;
			iItemCount = m_list.GetItemCount();
			m_list.InsertItem(iItemCount," ");
			m_list.SetItemState(iItemCount,INDEXTOSTATEIMAGEMASK(1),LVIS_STATEIMAGEMASK);
			m_list.SetItemText(iItemCount,0,dlgPrefExclusiveAdd.m_sCpuGpu);
			m_list.SetItemText(iItemCount,1,dlgPrefExclusiveAdd.m_sExe);
		}
	}
	EnableButtons(FALSE);
	m_bChanged = true;
}


void CDgPrefExclusive::OnBnClickedButtonDeleteExclusive()
{
	int iItem;
	POSITION pos;

	pos = m_list.GetFirstSelectedItemPosition();
	if (pos)
	{
		iItem = m_list.GetNextSelectedItem(pos);
		m_list.DeleteItem(iItem);	
	}
	EnableButtons(FALSE);
	m_bChanged = true;
}

void CDgPrefExclusive::OnBnClickedOk()
{
	m_pDlgPrefMain->Ok();
}

void CDgPrefExclusive::OnBnClickedCancel()
{
	m_pDlgPrefMain->Cancel();
}


void CDgPrefExclusive::OnLvnItemchangedListExclude(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

//	int iItem;
	POSITION pos;

	pos = m_list.GetFirstSelectedItemPosition();
	if (pos)
	{
		EnableButtons(TRUE);
	}
	else
	{
		EnableButtons(FALSE);
	}

	*pResult = 0;
}


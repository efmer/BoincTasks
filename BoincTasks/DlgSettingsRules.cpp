// DlgSettingsRules.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsMain.h"
#include "DlgSettingsRules.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "Translation.h"
#include "rules.h"

// DlgSettingsRules dialog

static int _gnRulesFmt[NUM_RULES_COLUMNS] =
{
	LVCFMT_LEFT,	LVCFMT_LEFT,	LVCFMT_LEFT,	LVCFMT_LEFT	
};

static int _gnRulesWidth[NUM_RULES_COLUMNS] =
{
	100,			100,			100,			250		
};



IMPLEMENT_DYNAMIC(CDlgSettingsRules, CPropertyPage)

CDlgSettingsRules::CDlgSettingsRules()
	: CPropertyPage(CDlgSettingsRules::IDD)
{

}

CDlgSettingsRules::~CDlgSettingsRules()
{
	int		iSize, iPos;
	CRules	rules;

	iSize = (int) m_lRules.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		rules.DeleteInterval(&m_lRules.at(iPos)->m_plInterval);
		delete m_lRules.at(iPos);
	}
	m_lRules.clear();
}

BOOL CDlgSettingsRules::OnInitDialog()
{
	int				iSize, iPos;
	CPropertySheet *pSheet;
	int iItemCount;
	CRuleItem	*pRuleItem;

	CDialog::OnInitDialog();
	
	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosGroupDialogSettingsRulesTitle];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);

	m_buttonDelete.SetWindowText(gszTranslation[PosDialogCommonButtonsDelete]);
	m_buttonEdit.SetWindowText(gszTranslation[PosDialogCommonButtonsEdit]);
	m_buttonAdd.SetWindowText(gszTranslation[PosDialogCommonButtonsAdd]);
	// translations

	BOOL bStatus;
	bStatus = m_bmp.LoadBitmap(IDB_BITMAP_HELP);
	m_buttonHelp.SetBitmap(m_bmp);

	AddColumns();

	m_StateImageList.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
	m_listRules.SetImageList(&m_StateImageList, LVSIL_STATE);

	ReadXml();

	iSize = (int) m_lRules.size();
	for (iPos = 0; iPos < iSize; iPos++)
	{
		pRuleItem = m_lRules.at(iPos);
		iItemCount = m_listRules.GetItemCount();
		m_listRules.InsertItem(iItemCount,pRuleItem->m_sComputer);
		m_listRules.SetItemText(iItemCount,1,pRuleItem->m_sProject);
		m_listRules.SetItemText(iItemCount,2,pRuleItem->m_sApplication);
		m_listRules.SetItemText(iItemCount,3,pRuleItem->m_sName);
		if (pRuleItem->m_iEnable)  m_listRules.SetItemState(iItemCount,INDEXTOSTATEIMAGEMASK(2),LVIS_STATEIMAGEMASK);
		else m_listRules.SetItemState(iItemCount,INDEXTOSTATEIMAGEMASK(1),LVIS_STATEIMAGEMASK);
	}

	SetTimer(UTIMER_DLG_SETTING_RULES, 200, 0);

	m_bButtonsEnabled = false;
	m_bRulesChanged = true;
	m_bRulesLocked = false;

	m_listRules.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSettingsRules::AddColumns()
{
	LV_COLUMN	lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int iCount = 0; iCount < NUM_RULES_COLUMNS; iCount++)
	{
		lvc.iSubItem = iCount;
		switch (iCount)
		{
			case 0:
				lvc.pszText	= gszTranslation[PosGroupDialogSettingsRulesColumnComputer];
			break;
			case 1:
				lvc.pszText	= gszTranslation[PosGroupDialogSettingsRulesColumnProject];
			break;
			case 2:
				lvc.pszText = gszTranslation[PosGroupDialogSettingsRulesColumnApplication];
			break;
			case 3:
				lvc.pszText = gszTranslation[PosGroupDialogSettingsRulesColumnRule];
			break;

		}
		lvc.fmt = _gnRulesFmt[iCount];
		lvc.cx = _gnRulesWidth[iCount];
		m_listRules.InsertColumn(iCount,&lvc);
	}
}

void CDlgSettingsRules::EnableButtons(bool bEnable)
{
	if (m_bButtonsEnabled != bEnable)
	{
		m_buttonDelete.EnableWindow(bEnable);
		m_buttonEdit.EnableWindow(bEnable);
		m_bButtonsEnabled = bEnable;
	}
}

void CDlgSettingsRules::CheckEnable()
{
	UINT		iPos;
	int			iItemCount;
	CString		sName;
	int			iEnabled;
	CRuleItem	*pRuleItem;

	iItemCount = m_listRules.GetItemCount();
	for (int iCount = 0; iCount < iItemCount; iCount++)
	{
		if (m_listRules.GetItemState(iCount,LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))	iEnabled = TRUE;
		else iEnabled = FALSE;

		sName = m_listRules.GetItemText(iCount,3);

		for (iPos = 0; iPos < m_lRules.size(); iPos++)
		{
			pRuleItem = m_lRules.at(iPos);
			if (sName == pRuleItem->m_sName)
			{
				if (pRuleItem->m_iEnable != iEnabled)
				{
					pRuleItem->m_iEnable = iEnabled;
					m_bRulesChanged = true;
				}
			}
		}
	}

}

int CDlgSettingsRules::ReadXml(void)
{
	CString				sTemp, sNr;
	int					iNrRules, iPos;
	CRuleItem			*pRuleItem;
	CRuleIntervalItem	*pRuleIntervalItem;

	iNrRules = 0;

	CXml xml("rules.xml",true,false);
	if (xml.ReadStart("rules"))
	{
		while (xml.ReadGroup("rule"))
		{
			pRuleItem = new CRuleItem;

			xml.ReadItem("active",&sTemp);
			if (sTemp == "no") pRuleItem->m_iEnable = FALSE;
			else pRuleItem->m_iEnable = TRUE;

			xml.ReadItem("name",&pRuleItem->m_sName);

			xml.ReadItem("computer",&pRuleItem->m_sComputer);
			xml.ReadItem("project",&pRuleItem->m_sProject);
			xml.ReadItem("appliation",&pRuleItem->m_sApplication);		// keep the typo before 1.77
			xml.ReadItem("application", &pRuleItem->m_sApplication);

			xml.ReadItem("type0",&sTemp);
			pRuleItem->m_iType[0] = atoi(sTemp);
			xml.ReadItem("type1",&sTemp);
			pRuleItem->m_iType[1] = atoi(sTemp);
			xml.ReadItem("type2",&sTemp);
			pRuleItem->m_iType[2] = atoi(sTemp);

			xml.ReadItem("operator0",&sTemp);
			pRuleItem->m_iOperator[0] = atoi(sTemp);
			xml.ReadItem("operator1",&sTemp);
			pRuleItem->m_iOperator[1] = atoi(sTemp);
			xml.ReadItem("operator2",&sTemp);
			pRuleItem->m_iOperator[2] = atoi(sTemp);

			xml.ReadItem("ivalue0",&sTemp);
			pRuleItem->m_iValue[0] = atoi(sTemp);
			xml.ReadItem("ivalue1",&sTemp);
			pRuleItem->m_iValue[1] = atoi(sTemp);
			xml.ReadItem("ivalue2",&sTemp);
			pRuleItem->m_iValue[2] = atoi(sTemp);

			xml.ReadItem("isnooze", &sTemp);
			pRuleItem->m_iSnooze = atoi(sTemp);

			xml.ReadItem("dvalue0",&sTemp);
			pRuleItem->m_dValue[0] = atof(sTemp);
			xml.ReadItem("dvalue1",&sTemp);
			pRuleItem->m_dValue[1] = atof(sTemp);
			xml.ReadItem("dvalue2",&sTemp);
			pRuleItem->m_dValue[2] = atof(sTemp);

			xml.ReadItem("itime",&sTemp);
			pRuleItem->m_iTime = atoi(sTemp);
		
			xml.ReadItem("color",&sTemp);
			pRuleItem->m_color = atoi(sTemp);

			xml.ReadItem("event_show",&sTemp);
			pRuleItem->m_iEventShow = atoi(sTemp);

			xml.ReadItem("event_internal", &sTemp);
			pRuleItem->m_iEventTypeInternal = atoi(sTemp);
			xml.ReadItem("event_external", &sTemp);
			pRuleItem->m_iEventTypeExternal = atoi(sTemp);

			xml.ReadItem("event_parameters", &pRuleItem->m_sEvent);

			pRuleItem->m_plInterval = NULL;

			int iInterval = 0;
			while (1)
			{
				sNr.Format("interval%d", iInterval);
				xml.ReadItem(sNr, &sTemp);
				if (sTemp == "")
				{
					break;
				}

				iPos = sTemp.Find(",");
				if (iPos <= 0)
				{
					break;
				}
				pRuleIntervalItem = new CRuleIntervalItem;

				pRuleIntervalItem->m_iStartTime = atoi(sTemp);
				sTemp = sTemp.Mid(iPos+1);
				pRuleIntervalItem->m_iStopTime = atoi(sTemp);

				if (pRuleItem->m_plInterval == NULL)
				{
					pRuleItem->m_plInterval = new std::deque<CRuleIntervalItem *>;
				}

				pRuleItem->m_plInterval->push_back(pRuleIntervalItem);

				iInterval++;
			}

			iNrRules++;
			m_lRules.push_back(pRuleItem);
		}
	}

	return iNrRules;
}


void CDlgSettingsRules::WriteXml(void)
{
	int			iSize, iPos;
	CString		sTxt, sAValue, sAComputerPassword;
	int			iNrRules;
	CRuleItem	*pRuleItem;
//	XOSITION	pos;

	CXml xml("rules.xml");
	if (xml.WriteStart("rules"))
	{
		iSize = (int) m_lRules.size();
		iPos = 0;
//		iNrRules = (int) m_lRules.GetCount();
		iNrRules = (int) m_lRules.size();
//		pos = m_lRules.GetHeadPosition();

		for(int i = 0; i < iNrRules; i++)
		{
			xml.WriteBeginGroup("rule");
//			pRuleItem = m_lRules.GetNext(pos);
			pRuleItem = m_lRules.at(iPos);
			iPos++;

			if (pRuleItem->m_iEnable) sTxt = "yes";
			else sTxt = "no";
			xml.WriteItem("active",sTxt,1);

			xml.WriteItem("name",pRuleItem->m_sName,1);
			xml.WriteItem("computer",pRuleItem->m_sComputer,1);
			xml.WriteItem("project",pRuleItem->m_sProject,1);
			xml.WriteItem("application",pRuleItem->m_sApplication,1);

			sTxt.Format("%d",pRuleItem->m_iType[0]);
			xml.WriteItem("type0",sTxt,1);
			sTxt.Format("%d",pRuleItem->m_iType[1]);
			xml.WriteItem("type1",sTxt,1);
			sTxt.Format("%d",pRuleItem->m_iType[2]);
			xml.WriteItem("type2",sTxt,1);

			sTxt.Format("%d",pRuleItem->m_iOperator[0]);
			xml.WriteItem("operator0",sTxt,1);
			sTxt.Format("%d",pRuleItem->m_iOperator[1]);
			xml.WriteItem("operator1",sTxt,1);
			sTxt.Format("%d",pRuleItem->m_iOperator[2]);
			xml.WriteItem("operator2",sTxt,1);

			sTxt.Format("%d",pRuleItem->m_iValue[0]);
			xml.WriteItem("ivalue0",sTxt,1);
			sTxt.Format("%d",pRuleItem->m_iValue[1]);
			xml.WriteItem("ivalue1",sTxt,1);
			sTxt.Format("%d",pRuleItem->m_iValue[2]);
			xml.WriteItem("ivalue2",sTxt,1);

			sTxt.Format("%d", pRuleItem->m_iSnooze);
			xml.WriteItem("isnooze", sTxt, 1);

			sTxt.Format("%f",pRuleItem->m_dValue[0]);
			xml.WriteItem("dvalue0",sTxt,1);
			sTxt.Format("%f",pRuleItem->m_dValue[1]);
			xml.WriteItem("dvalue1",sTxt,1);
			sTxt.Format("%f",pRuleItem->m_dValue[2]);
			xml.WriteItem("dvalue2",sTxt,1);

			sTxt.Format("%d",pRuleItem->m_iTime);
			xml.WriteItem("itime",sTxt,1);

			sTxt.Format("%d",pRuleItem->m_color);
			xml.WriteItem("color",sTxt,1);

			sTxt.Format("%d",pRuleItem->m_iEventShow);
			xml.WriteItem("event_show",sTxt,1);

			sTxt.Format("%d",pRuleItem->m_iEventTypeInternal);
			xml.WriteItem("event_internal",sTxt,1);
			sTxt.Format("%d",pRuleItem->m_iEventTypeExternal);
			xml.WriteItem("event_external",sTxt,1);

			xml.WriteItem("event_parameters",pRuleItem->m_sEvent,1);

			if (pRuleItem->m_plInterval != NULL)
			{
				for (int iInterval = 0; iInterval < (int) pRuleItem->m_plInterval->size(); iInterval++)
				{
					sTxt.Format("%d,%d",pRuleItem->m_plInterval->at(iInterval)->m_iStartTime, pRuleItem->m_plInterval->at(iInterval)->m_iStopTime);
					sAValue.Format("interval%d", iInterval);
					xml.WriteItem(sAValue,sTxt,1);
				}
			}

			xml.WriteEndGroup();
		}
		xml.WriteEnd();
	}
}

void CDlgSettingsRules::DialogToItem(CDlgRules *pDlg, int iItem, CRuleItem *pRuleItem)
{
	CRules rules;
	CString sTxt;

	m_listRules.SetItemText(iItem,1,pDlg->m_sProject);
	m_listRules.SetItemText(iItem,2,pDlg->m_sApplication);

	sTxt = pDlg->m_sName;
	m_listRules.SetItemText(iItem,3,sTxt);

	pRuleItem->m_sName = pDlg->m_sName;
	pRuleItem->m_sComputer = pDlg->m_sComputer;
	pRuleItem->m_sProject = pDlg->m_sProject;
	pRuleItem->m_sApplication = pDlg->m_sApplication;

	if (pRuleItem->m_sApplication.FindOneOf("?*") >= 0)
	{
		pRuleItem->m_bApplicationWc = true;
	}
	else
	{
		pRuleItem->m_bApplicationWc = false;
	}

	for (int iCount = 0; iCount < RULE_ITEMS; iCount++)
	{
		pRuleItem->m_iType[iCount] = pDlg->m_iType[iCount];
		pRuleItem->m_iOperator[iCount] = pDlg->m_iOperator[iCount];
		pRuleItem->m_iValue[iCount] = pDlg->m_iValue[iCount];
		pRuleItem->m_dValue[iCount] = pDlg->m_dValue[iCount];
	}

	pRuleItem->m_iSnooze = pDlg->m_iSnooze;

	pRuleItem->m_iTime = pDlg->m_iTime;
	pRuleItem->m_color = pDlg->m_color;

	pRuleItem->m_iEventShow = pDlg->m_iEventShow;

	pRuleItem->m_iEventTypeInternal = pDlg->m_iEventInternal;
	pRuleItem->m_iEventTypeExternal = pDlg->m_iEventExternal;

	pRuleItem->m_sEvent = pDlg->m_sEvent;

	if (pRuleItem->m_plInterval != pDlg->m_plInterval)
	{
		if (pRuleItem->m_plInterval)
		{
			rules.DeleteInterval(&pRuleItem->m_plInterval);
		}
	}
	pRuleItem->m_plInterval = pDlg->m_plInterval;
	if (pRuleItem->m_iType[0] != RULES_TYPE_TIME_INTERVAL)
	{
		rules.DeleteInterval(&pRuleItem->m_plInterval);
	}
}

void CDlgSettingsRules::AddRule(CString *psComputer, CString *psProject, CString *psApplication)
{
	int iResult;
	CString sTxt;
	CRuleItem *pRuleItem;

	m_bRulesLocked = true;

	CDlgRules dlgRules;
	dlgRules.m_bSetup = false;
	dlgRules.m_bEdit = false;
	dlgRules.m_plRules = &m_lRules; 

	dlgRules.m_sComputer = *psComputer;
	dlgRules.m_sProject = *psProject;
	dlgRules.m_sApplication = *psApplication;

	dlgRules.m_pRuleItem = NULL;
	dlgRules.m_plInterval = NULL;

	iResult = (int) dlgRules.DoModal();
	if (iResult == IDOK)
	{
		int iItemCount;
		iItemCount = m_listRules.GetItemCount();
		m_listRules.InsertItem(iItemCount,dlgRules.m_sComputer);
		m_listRules.SetItemState(iItemCount,INDEXTOSTATEIMAGEMASK(1),LVIS_STATEIMAGEMASK);
		pRuleItem = new CRuleItem;
		pRuleItem->m_plInterval = NULL;

		DialogToItem(&dlgRules, iItemCount, pRuleItem);

//		m_lRules.AddTail(pRuleItem);
		m_lRules.push_back(pRuleItem);

		CheckEnable();
		WriteXml();
		m_bRulesChanged = true;
	}
	m_bRulesLocked = false;
}

void CDlgSettingsRules::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RULES, m_listRules);
	DDX_Control(pDX, ID_BUTTON_RULES_DELETE, m_buttonDelete);
	DDX_Control(pDX, ID_BUTTON_RULES_EDIT, m_buttonEdit);
	DDX_Control(pDX, ID_BUTTON_RULES_ADD, m_buttonAdd);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, ID_HELP, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(CDlgSettingsRules, CPropertyPage)
	ON_WM_TIMER()
	ON_MESSAGE(UWM_MSG_RULES_ADD,OnAddRule)
	ON_MESSAGE(UWM_MSG_RULES_GET_COPY,OnGetCopy)
	ON_BN_CLICKED(ID_BUTTON_RULES_ADD, &CDlgSettingsRules::OnBnClickedButtonRulesAdd)
	ON_BN_CLICKED(ID_BUTTON_RULES_DELETE, &CDlgSettingsRules::OnBnClickedButtonRulesDelete)
	ON_BN_CLICKED(ID_BUTTON_RULES_EDIT, &CDlgSettingsRules::OnBnClickedButtonRulesEdit)
	ON_BN_CLICKED(IDOK, &CDlgSettingsRules::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RULES, &CDlgSettingsRules::OnLvnItemchangedListRules)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RULES, &CDlgSettingsRules::OnNMDblclkListRules)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(ID_HELP, &CDlgSettingsRules::OnBnClickedHelp)
END_MESSAGE_MAP()

// DlgSettingsRules message handlers

void CDlgSettingsRules::OnTimer(UINT_PTR nIDEvent)
{
	if (this->IsWindowVisible())
	{
		int iItemCount;
		bool	bChecked;
		bChecked = false;

		iItemCount = m_listRules.GetItemCount();
		for (int iCount = 0; iCount < iItemCount; iCount++)
		{
			POSITION pos = m_listRules.GetFirstSelectedItemPosition();
			if (pos)
//			if (m_listRules.GetItemState(iCount,LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))
			{
				bChecked = true;
				break;
			}
		}
		EnableButtons(bChecked);
	}

	if (m_listRules.m_bCheckChanged)
	{
		m_listRules.m_bCheckChanged = false;
		m_bRulesChanged = true;
		CheckEnable();
		WriteXml();
	}
}

LRESULT CDlgSettingsRules::OnGetCopy(WPARAM wParam, LPARAM lParam)
{
	int			iSize, iPos, iCopy, iCopyEnd;
	CRuleItem	*pRuleItem, *pRuleItemHistory;
	std::deque<CRuleItem*> *plRulesHistoryCopy;
	CRuleIntervalItem	*pRuleIntervalItem;
	bool		bBuildNew = true, bChanged, bDouble = false;
	CRules		rules;

	bChanged = false;

	if (m_bRulesLocked)
	{
		return bChanged;
	}
	plRulesHistoryCopy  = (std::deque<CRuleItem*> *) wParam;

	if (m_bRulesChanged)	// delete any old rules in history
	{
		bChanged = true;
		iSize = (int) plRulesHistoryCopy->size();
		for (iPos = 0; iPos < iSize; iPos++)
		{
			rules.DeleteInterval(&plRulesHistoryCopy->at(iPos)->m_plInterval);
			delete plRulesHistoryCopy->at(iPos);
		}
		plRulesHistoryCopy->clear();

		// now copy everything;

		iSize = (int) m_lRules.size();
		for (iPos = 0; iPos < iSize; iPos++)
		{
			pRuleItem = m_lRules.at(iPos);

			if (pRuleItem->m_iEnable)		// only add enabled rules.
			{
				if (pRuleItem->m_iType[0] == RULES_TYPE_TIME_INTERVAL)
				{
					bDouble = true;
					iCopyEnd = 2;
				}
				else
				{
					iCopyEnd = 1;
					bDouble = false;
				}

				iCopy = 1;
				while (iCopy <= iCopyEnd)
				{
					pRuleItemHistory = new CRuleItem;
					pRuleItemHistory->m_iBackoffUntill = 0;

					pRuleItemHistory->m_sName = pRuleItem->m_sName;
					pRuleItemHistory->m_sComputer = pRuleItem->m_sComputer;
					pRuleItemHistory->m_sProject = pRuleItem->m_sProject;
					pRuleItemHistory->m_sApplication = pRuleItem->m_sApplication;
	
					for (int iCount = 0; iCount < RULE_ITEMS; iCount++)
					{
						pRuleItemHistory->m_iType[iCount] =  pRuleItem->m_iType[iCount];
						pRuleItemHistory->m_iOperator[iCount] =	pRuleItem->m_iOperator[iCount];
						pRuleItemHistory->m_iValue[iCount] = pRuleItem->m_iValue[iCount];
						pRuleItemHistory->m_dValue[iCount] = pRuleItem->m_dValue[iCount];
					}
					pRuleItemHistory->m_iSnooze = pRuleItem->m_iSnooze;
					pRuleItemHistory->m_iTime =	pRuleItem->m_iTime;
					pRuleItemHistory->m_color =	pRuleItem->m_color;
					pRuleItemHistory->m_iEventShow = pRuleItem->m_iEventShow;

					pRuleItemHistory->m_iEventTypeInternal = pRuleItem->m_iEventTypeInternal;
					pRuleItemHistory->m_iEventTypeExternal = pRuleItem->m_iEventTypeExternal;

					pRuleItemHistory->m_sEvent = pRuleItem->m_sEvent;

					if (iCopy == 2)
					{
						if (bDouble) pRuleItemHistory->m_sName += " Deactive";
						switch (pRuleItemHistory->m_iEventTypeInternal)
						{
						case EVENT_TYPE_SUSPEND_PROJECT:
							pRuleItemHistory->m_iEventTypeInternal = EVENT_TYPE_RESUME_PROJECT;
						break;
						case EVENT_TYPE_NO_NEW_WORK:
							pRuleItemHistory->m_iEventTypeInternal = EVENT_TYPE_ALLOW_NEW_WORK;
						break;
						case EVENT_TYPE_SNOOZE:
							pRuleItemHistory->m_iEventTypeInternal = EVENT_TYPE_CANCEL_SNOOZE;
						break;
						case EVENT_TYPE_SNOOZE_GPU:
							pRuleItemHistory->m_iEventTypeInternal = EVENT_TYPE_CANCEL_SNOOZE_GPU;
						break;
						case EVENT_TYPE_SUSPEND_NETWORK:
							pRuleItemHistory->m_iEventTypeInternal = EVENT_TYPE_RESUME_NETWORK;
							break;
						}

					}
					else
					{
						if (bDouble) pRuleItemHistory->m_sName += " Active";
					}

					pRuleItemHistory->m_plInterval = NULL;

					if (pRuleItem->m_plInterval != NULL)
					{
						for (int iInterval = 0; iInterval < (int) pRuleItem->m_plInterval->size(); iInterval++)
						{
							pRuleIntervalItem = new CRuleIntervalItem;
							pRuleIntervalItem->m_iStartTime = pRuleItem->m_plInterval->at(iInterval)->m_iStartTime;
							pRuleIntervalItem->m_iStopTime = pRuleItem->m_plInterval->at(iInterval)->m_iStopTime;
							if (iCopy == 1) pRuleIntervalItem->m_bInvers = false;
							else pRuleIntervalItem->m_bInvers = true;
							if (pRuleItemHistory->m_plInterval == NULL)
							{
								pRuleItemHistory->m_plInterval = new (std::deque<CRuleIntervalItem *>);
							}
							pRuleItemHistory->m_plInterval->push_back(pRuleIntervalItem);
						}
					}
					plRulesHistoryCopy->push_back(pRuleItemHistory);
					iCopy++;
				}
			}
		}
	}
	m_bRulesChanged = false;
	return bChanged;
}

LRESULT CDlgSettingsRules::OnAddRule(WPARAM wParam, LPARAM lParam)
{
	CRuleItem *pRuleItem;

	pRuleItem = (CRuleItem *) wParam;

	theApp.m_pDlgSettingsMain->ShowWindow(SW_SHOW);
	theApp.m_pDlgSettingsMain->SetActivePage(this);


	AddRule(&pRuleItem->m_sComputer, &pRuleItem->m_sProject, &pRuleItem->m_sApplication);

	return 0;
}

void CDlgSettingsRules::OnBnClickedButtonRulesAdd()
{
	CString sComputer, sProject, sApplication;

	AddRule(&sComputer, &sProject, &sApplication);
}

void CDlgSettingsRules::OnBnClickedButtonRulesDelete()
{
	UINT		iPos;
	int			iItem;
	CString		sName, sTxt;
	POSITION	pos;
	CRuleItem	*pRuleItem;
	int			iResult;
	CRules		rules;

	m_bRulesLocked = true;

	int iRuleCount;

	iRuleCount = m_listRules.GetItemCount();

	pos = m_listRules.GetFirstSelectedItemPosition();
	if (pos)
	{
		iItem = m_listRules.GetNextSelectedItem(pos);
		sTxt = gszTranslation[PosGroupDialogSettingsRulesDeleteYesNo];
		sTxt+= " : ";
		sName = m_listRules.GetItemText(iItem,3);
		sTxt = sTxt + sName;
		iResult = AfxMessageBox(sTxt,MB_YESNO|MB_ICONEXCLAMATION);
		if (iResult != IDYES) return;

		m_listRules.DeleteItem(iItem);	
	}

	for (iPos = 0; iPos < m_lRules.size(); iPos++)
	{
		pRuleItem = m_lRules.at(iPos);
		if (pRuleItem->m_sName == sName)
		{
			rules.DeleteInterval(&pRuleItem->m_plInterval);
			delete pRuleItem;
			pRuleItem = NULL;
			m_lRules.erase(m_lRules.begin() + iPos);
		}
	}

	CheckEnable();
	WriteXml();
	m_bRulesChanged = true;
	m_bRulesLocked = false;
}

void CDlgSettingsRules::OnBnClickedButtonRulesEdit()
{
	UINT		iPos;
	int			iResult, iItemFound, iItem;
	CString		sTxt, sName;
	CRuleItem	*pRuleItem, *pRuleItemFound;
	POSITION	pos;
	
	m_bRulesLocked = true;

	CDlgRules dlgRules;
	dlgRules.m_bSetup = true;
	dlgRules.m_bEdit = true;
	dlgRules.m_plRules = &m_lRules; 

	pRuleItemFound = NULL;

	pos = m_listRules.GetFirstSelectedItemPosition();
	if (pos)
	{
		iItem = m_listRules.GetNextSelectedItem(pos);
		sName = m_listRules.GetItemText(iItem,3);

		for (iPos = 0; iPos < m_lRules.size(); iPos++)
		{
			pRuleItem = m_lRules.at(iPos);
			if (sName == pRuleItem->m_sName)
			{
				pRuleItemFound = pRuleItem;
				iItemFound = iItem;
			}
		}
	}

	if (pRuleItemFound == NULL) return;

	dlgRules.m_pRuleItem = pRuleItemFound;
	dlgRules.m_plInterval = pRuleItemFound->m_plInterval;

	iResult = (int) dlgRules.DoModal();
	if (iResult == IDOK)
	{
		// found selected one
		m_listRules.SetItemText(iItemFound,0,dlgRules.m_sComputer);

		DialogToItem(&dlgRules, iItemFound, pRuleItemFound);

		CheckEnable();
		WriteXml();
		m_bRulesChanged = true;
	}
	m_bRulesLocked = false;
}


void CDlgSettingsRules::OnBnClickedOk()
{
	theApp.m_pDlgSettingsMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
	theApp.m_pDlgSettingsMain->SafeWindowPlacement();
}

void CDlgSettingsRules::OnLvnItemchangedListRules(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	UINT uFlags = 0;

	*pResult = 0;
}


void CDlgSettingsRules::OnNMDblclkListRules(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OnBnClickedButtonRulesEdit();

}

BOOL CDlgSettingsRules::OnHelpInfo( HELPINFO* HelpInfo)
{
	OnBnClickedHelp();
	return TRUE;
}

void CDlgSettingsRules::OnBnClickedHelp()
{
	CString strLink;
	strLink = "https://efmer.com/boinctasks_settings#rules";

	ShellExecute(NULL,_T("open"),strLink.IsEmpty() ? strLink : strLink,NULL,NULL,SW_SHOWNORMAL);
}

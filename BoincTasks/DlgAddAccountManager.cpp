// DlgAccountManager.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgAddAccountManager.h"
#include "afxdialogex.h"
#include "Translation.h"
#include "ThreadRpc.h"
#include "RemoteCombinedDoc.h"

// CDlgAccountManager dialog

IMPLEMENT_DYNAMIC(CDlgAddAccountManager, CDialogEx)

CDlgAddAccountManager::CDlgAddAccountManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAddAccountManager::IDD, pParent)
{
//	m_iThreadId = 0;
//	m_iNrOfStrings = 0;

	m_pAccountManagerInfo = new ACCT_MGR_INFO;
	m_pAccountManagerAttach = new CRpcManagerAttach;
	m_pAccountMangerReply = new ACCT_MGR_RPC_REPLY;

 	m_bAccountManagerFound = false;

	m_hWnd = NULL;

}

CDlgAddAccountManager::~CDlgAddAccountManager()
{
	delete m_pAccountManagerInfo;
	delete m_pAccountManagerAttach;
	delete m_pAccountMangerReply;
}

BOOL CDlgAddAccountManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

// Translation
	
	m_textUserName.SetWindowText(gszTranslation[PosDialogAddAccountManagerUserName]);
	m_textPassword.SetWindowText(gszTranslation[PosDialogAddAccountManagerPassword]);

	m_buttonWebsite.SetWindowText(gszTranslation[PosDialogAddAccountManagerButtonWeb]);
	m_buttonAdd.SetWindowText(gszTranslation[PosDialogAddAccountManagerButtonAdd]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);

// Translation

	Populate();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgAddAccountManager::Populate()
{
	m_sAccountManager[0][0] = ACCOUNT_MANAGER_0;
	m_list.AddString(ACCOUNT_MANAGER_0);
	m_sAccountManager[0][1] = ACCOUNT_MANAGER_ULR_0;

	m_sAccountManager[1][0] = ACCOUNT_MANAGER_1;
	m_list.AddString(ACCOUNT_MANAGER_1);
	m_sAccountManager[1][1] = ACCOUNT_MANAGER_ULR_1;
	
	m_sAccountManager[2][0] = ACCOUNT_MANAGER_2;
	m_list.AddString(ACCOUNT_MANAGER_2);
	m_sAccountManager[2][1] = ACCOUNT_MANAGER_ULR_2;
}

void CDlgAddAccountManager::GetErrorString(int iError, CString *psError)
{
	switch (iError)
	{
		case ERR_GETHOSTBYNAME:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorUrl];					//"Url name not valid";
		break;
		case ERR_ATTACH_FAIL_SERVER_ERROR:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorFailedContact];		//"Failed to contact the project server";
		break;
		case ERR_BAD_PASSWD:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorPassword];				//"Bad password";
		break;
		case ERR_ALREADY_ATTACHED:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorAlreadyAtt];			//"Already attached to this project";
		break;
		case ERR_DB_NOT_FOUND:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorNotDatabase];			//"Not found in the database";
		break;
		case ERR_CONNECT:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorUnableConnect];		//"Unable to connect";
		break;
		case ERR_NOT_FOUND:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorNotFound];				//"Not found";
		break;
		case ERR_BAD_EMAIL_ADDR:
			*psError = gszTranslation[PosDialogAddProjectMsgErrorEmail];
		break;

		default:
			psError->Format("AddProject: Error %d",iError);
	}	
}

void CDlgAddAccountManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACCOUNT_MANAGER, m_list);
	DDX_Control(pDX, IDC_EDIT_SELECT_ACCOUNT_MANAGER, m_editUrl);
	DDX_Control(pDX, IDC_EDIT_CCOUNT_MANAGER_ERROR, m_editError);
	DDX_Control(pDX, IDC_TEXT_ACCOUNT_MANAGER_USERNAME, m_textUserName);
	DDX_Control(pDX, IDC_TEXT_ACCOUNT_MANAGER_PASSWORD, m_textPassword);
	DDX_Control(pDX, IDOK, m_buttonAdd);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDWWW, m_buttonWebsite);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT_MANAGER_USER_NAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT_MANAGER_LOGIN_PASSWRD, m_editPassword);
	DDX_Control(pDX, IDC_LIST_ACCOUNT_MANAGER_COMPUTERS, m_listComputers);
}


BEGIN_MESSAGE_MAP(CDlgAddAccountManager, CDialogEx)
	ON_MESSAGE(UWM_MSG_ADD_ACCOUNTMANAGER_OPEN_DIALOG, OnAddComputerOpen)
//	ON_MESSAGE(UWM_MSG_ADD_ACCOUNTMANAGER_SET_STRING_ID, OnSetComputerStringId)
//	ON_MESSAGE(UWM_MSG_ADD_ACCOUNTMANAGER_SET_THREAD_ID, OnSetComputerThreadId)

	ON_MESSAGE(UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO_READY, OnGetAccountManagerReady)
	ON_MESSAGE(UWM_MSG_THREAD_ACCOUNTMANAGER_ATTACH_READY, OnCreateAccountReady)
	ON_MESSAGE(UWM_MSG_THREAD_ACCOUNTMANAGER_POLL_READY, OnAccountPollReady)

	ON_LBN_SELCHANGE(IDC_LIST_ACCOUNT_MANAGER, &CDlgAddAccountManager::OnLbnSelchangeListAccountManager)
	ON_BN_CLICKED(IDCANCEL, &CDlgAddAccountManager::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgAddAccountManager::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_SELECT_ACCOUNT_MANAGER, &CDlgAddAccountManager::OnEnChangeEditSelectAccountManager)
	ON_BN_CLICKED(IDWWW, &CDlgAddAccountManager::OnBnClickedWww)
	ON_LBN_SELCHANGE(IDC_LIST_ACCOUNT_MANAGER_COMPUTERS, &CDlgAddAccountManager::OnLbnSelchangeListAccountManagerComputers)
END_MESSAGE_MAP()


// CDlgAccountManager message handlers

LRESULT CDlgAddAccountManager::OnAddComputerOpen(WPARAM parm1, LPARAM parm2)
{
	char	*pcComputerId;
	CString sTxt;
	int		iThreadId, iItems;
	CAccountManagerData *pAccountManagerData;

	pAccountManagerData = (CAccountManagerData *) parm1;

	m_accountManagerData.m_hWndTasks = pAccountManagerData->m_hWndTasks;
	m_accountManagerData.m_iThreadId = pAccountManagerData->m_iThreadId;
	m_accountManagerData.m_pDoc = pAccountManagerData->m_pDoc;
	m_accountManagerData.m_sComputer = pAccountManagerData->m_sComputer;

	sTxt = gszTranslation[PosDialogAddAccountManagerTitle];
	sTxt+= m_accountManagerData.m_sComputer;
	this->SetWindowText(sTxt);
	m_list.SetCurSel(-1);		// no selection

	m_listComputers.ResetContent();
	int iThreadCount = m_accountManagerData.m_pDoc->GetRpcSelectedComputers();
	for (int iCount = 0; iCount < iThreadCount; iCount++)
	{
		m_accountManagerData.m_pDoc->GetComputerIds(&pcComputerId, iCount);
		m_listComputers.AddString(pcComputerId);
	}

// find the right computer name
	iItems = m_accountManagerData.m_pDoc->GetRpcSelectedComputers();
	for (int iCount = 0; iCount < iItems; iCount++)
	{
		m_accountManagerData.m_pDoc->GetRpcThreadId(&iThreadId, iCount);
		if (m_accountManagerData.m_iThreadId == iThreadId)
		{
			sTxt = gszTranslation[PosDialogAddAccountManagerTitle];
			m_accountManagerData.m_pDoc->GetComputerIds(&pcComputerId, iCount);
			sTxt+= pcComputerId;
			this->SetWindowText(sTxt);
			m_listComputers.SelectString(0, pcComputerId);
			break;
		}
	}

	if (iThreadCount > 1)	m_listComputers.ShowWindow(SW_SHOW);
	else m_listComputers.ShowWindow(SW_HIDE);

	this->ShowWindow(SW_SHOW);
	::PostThreadMessage(m_accountManagerData.m_iThreadId,UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO,(WPARAM) this->m_hWnd, (LPARAM) m_pAccountManagerInfo);
	return 0;
}


LRESULT CDlgAddAccountManager::OnGetAccountManagerReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt;
	int iResult;
	BOOL	bEnable;

	iResult = (int) parm1;

	if (iResult==0)
	{
		if (strlen(m_pAccountManagerInfo->acct_mgr_name.c_str()) > 1)
		{
			m_editUrl.SetWindowText(m_pAccountManagerInfo->acct_mgr_url.c_str());
			m_bAccountManagerFound = true;
			sTxt = gszTranslation[PosDialogAddAccountManagerAlready];
			sTxt+= ": ";
			sTxt+= m_pAccountManagerInfo->acct_mgr_name.c_str();
			m_editError.SetWindowText(sTxt);
			bEnable = FALSE;
			m_buttonAdd.SetWindowText(gszTranslation[PosDialogAddAccountManagerButtonRemove]);	
		}
		else
		{
			m_bAccountManagerFound = false;
			m_editError.SetWindowText("");
			bEnable = TRUE;
			m_buttonAdd.SetWindowText(gszTranslation[PosDialogAddAccountManagerButtonAdd]);
		}
	}
	else
	{
		m_bAccountManagerFound = false;
		bEnable = TRUE;
		m_editError.SetWindowText("");
		m_buttonAdd.SetWindowText(gszTranslation[PosDialogAddAccountManagerButtonAdd]);
	}

	m_editUrl.EnableWindow(bEnable);
	m_editUserName.EnableWindow(bEnable);
	m_editPassword.EnableWindow(bEnable);
	m_list.EnableWindow(bEnable);

	if (!m_bAccountManagerFound)
	{
		OnEnChangeEditSelectAccountManager();
	}
	else
	{
		m_buttonAdd.EnableWindow(TRUE);
	}

	// something changed, so tell it the tasks view.
	::PostMessage(m_accountManagerData.m_hWndTasks, UWM_MSG_ADD_ACCOUNTMANAGER_CHANGED, 0, 0);

	m_listComputers.EnableWindow(TRUE);

	return 0;
}


void CDlgAddAccountManager::OnLbnSelchangeListAccountManager()
{
	int iSelected, iSizeManagers;
	CString sSelected, sTxt, sTxtBreakup;

	m_editError.SetWindowText("");

	iSelected = m_list.GetCurSel();
	m_list.GetText(iSelected,sSelected);

	iSizeManagers = ACCOUNT_MANAGERS_TOTAL;

	for (int iCount = 0; iCount < iSizeManagers; iCount++)
	{
		if (sSelected == m_sAccountManager[iCount][0])
		{
			m_editUrl.SetWindowText(m_sAccountManager[iCount][1]);
			break;
		}
	}
}


void CDlgAddAccountManager::OnBnClickedCancel()
{
	this->ShowWindow(SW_HIDE);
}


void CDlgAddAccountManager::OnBnClickedOk()
{
	CString sTxt;
	char cBuffer[100];

	if (m_bAccountManagerFound)
	{
		cBuffer[0] = 0;
		strcpy_s (m_cAccountUrl,99, cBuffer);	
		strcpy_s (m_cAccountEmailOrUser,99, cBuffer);	
		strcpy_s (m_cAccountPassWrd,99, cBuffer);	
	}
	else
	{
		m_editUrl.GetWindowText(cBuffer,99);
		strcpy_s (m_cAccountUrl,99, cBuffer);	
		m_editUserName.GetWindowText(cBuffer,99);
		strcpy_s (m_cAccountEmailOrUser,99, cBuffer);	
		m_editPassword.GetWindowText(cBuffer,99);
		strcpy_s (m_cAccountPassWrd,99, cBuffer);

		if (strlen(m_cAccountUrl) == 0) return;
		if (strlen(m_cAccountEmailOrUser) == 0) return;
		if (strlen(m_cAccountPassWrd) == 0) return;
	}
	m_pAccountManagerAttach->m_sUrl = m_cAccountUrl;
	m_pAccountManagerAttach->m_sName = m_cAccountEmailOrUser;
	m_pAccountManagerAttach->m_sAuth = m_cAccountPassWrd;

	sTxt = gszTranslation[PosDialogAddProjectMsgBusy];
	m_editError.SetWindowText(sTxt);	

	m_buttonAdd.EnableWindow(FALSE);
	m_listComputers.EnableWindow(FALSE);
	::PostThreadMessage(m_accountManagerData.m_iThreadId,UWM_MSG_THREAD_ACCOUNTMANAGER_ATTACH,(WPARAM) this->m_hWnd, (LPARAM) m_pAccountManagerAttach);

}


void CDlgAddAccountManager::OnEnChangeEditSelectAccountManager()
{
	CString sTxt;
	BOOL	bEnable;

	m_editUrl.GetWindowText(sTxt);

	if (sTxt.GetLength() > 10)
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	m_buttonAdd.EnableWindow(bEnable);
	m_buttonWebsite.EnableWindow(bEnable);
}


void CDlgAddAccountManager::OnBnClickedWww()
{
	CString sUrl;

	m_editUrl.GetWindowText(sUrl);

	if (sUrl.GetLength() > 0) ShellExecute(NULL,_T("open"), sUrl,NULL,NULL,SW_SHOWNORMAL);
}

LRESULT CDlgAddAccountManager::OnCreateAccountReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt;
	int iStatus;

	iStatus = (int) parm1;

	if (iStatus>=0)
	{
	}
	else
	{
		sTxt = gszTranslation[PosDialogAddProjectMsgErrorCreating];										//"Error in creating account"
		m_editError.SetWindowText(sTxt);
		m_buttonAdd.EnableWindow(TRUE);
		m_listComputers.EnableWindow(TRUE);
		return 0;
	}

	::PostThreadMessage(m_accountManagerData.m_iThreadId,UWM_MSG_THREAD_ACCOUNTMANAGER_POLL,(WPARAM) this->m_hWnd, (LPARAM) m_pAccountMangerReply);

	return 0;

}

LRESULT CDlgAddAccountManager::OnAccountPollReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt;
	int iStatus;

	iStatus = (int) parm1;

	if (iStatus>=0)
	{
		if (m_pAccountMangerReply->error_num != BOINC_SUCCESS )			//??
		{
			GetErrorString(m_pAccountMangerReply->error_num, &sTxt);
			sTxt += "\r\n";

			for (int iCount = 0; iCount < (int) m_pAccountMangerReply->messages.size(); iCount++)
			{
				sTxt += m_pAccountMangerReply->messages[iCount].c_str();
				sTxt += "\r\n";
			}

			m_editError.SetWindowText(sTxt);
			m_buttonAdd.EnableWindow(TRUE);
		}
		else
		{
//			sTxt += gszTranslation[PosDialogAddAccountManagerAdded];
//			for (int iCount = 0; iCount < m_pAccountMangerReply->messages.size(); iCount++)
//			{
//				sTxt += m_pAccountMangerReply->messages[iCount].c_str();
//				sTxt += "\r\n";
//			}			
			m_editError.SetWindowText("");
			m_buttonAdd.EnableWindow(FALSE);

			::PostThreadMessage(m_accountManagerData.m_iThreadId,UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO,(WPARAM) this->m_hWnd, (LPARAM) m_pAccountManagerInfo);
			return 0;
		}
	}
	else
	{
		m_editError.SetWindowText("");
	}
	m_buttonAdd.EnableWindow(TRUE);
	m_listComputers.EnableWindow(TRUE);
	return 0;
}

void CDlgAddAccountManager::OnLbnSelchangeListAccountManagerComputers()
{
	int		iCurSel;
	int		iThreadId, iThreadCount;
	char	*pcComputerId;
	CString sSelection, sTxt;

	iCurSel = m_listComputers.GetCurSel();
	if (iCurSel >= 0)
	{
		m_listComputers.GetText(iCurSel,sSelection);
		iThreadCount = m_accountManagerData.m_pDoc->GetRpcSelectedComputers();
		for (int iCount = 0; iCount < iThreadCount; iCount++)
		{
			m_accountManagerData.m_pDoc->GetComputerIds(&pcComputerId, iCount);
			if (sSelection == pcComputerId)
			{
				m_accountManagerData.m_pDoc->GetRpcThreadId(&iThreadId, iCount);
				m_accountManagerData.m_iThreadId = iThreadId;
				sTxt = gszTranslation[PosDialogAddAccountManagerTitle];
				sTxt+= sSelection;
				this->SetWindowText(sTxt);
				m_buttonAdd.EnableWindow(FALSE);
				m_listComputers.EnableWindow(FALSE);
				sTxt = gszTranslation[PosDialogAddProjectMsgBusy];
				m_editError.SetWindowText(sTxt);	
				::PostThreadMessage(m_accountManagerData.m_iThreadId,UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO,(WPARAM) this->m_hWnd, (LPARAM) m_pAccountManagerInfo);
				return;

			}
		}
	}

}

// DlgAddProject.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgAddProject.h"
#include "RemoteCombinedDoc.h"
#include "Translation.h"

// CDlgAddProject dialog

IMPLEMENT_DYNAMIC(CDlgAddProject, CDialog)

CDlgAddProject::CDlgAddProject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddProject::IDD, pParent)
{
	m_pDoc = NULL;
	m_iThreadId = 0;
	m_iNrOfStrings = 0;
	m_iThreadIdItems = 0;
}

CDlgAddProject::~CDlgAddProject()
{
}

BOOL CDlgAddProject::OnInitDialog()
{
	CDialog::OnInitDialog();

// Translation
	
	m_textAddMain.SetWindowText(gszTranslation[PosDialogAddProjectMain]);
	m_textLogin.SetWindowText(gszTranslation[PosDialogAddProjectLoginName]);
	m_textPassword.SetWindowText(gszTranslation[PosDialogAddProjectPassword]);
	m_staticUserName.SetWindowText(gszTranslation[PosDialogAddProjectUserName]);
	m_checkCreate.SetWindowText(gszTranslation[PosDialogAddProjectCreateWhile]);
	m_checkComputers.SetWindowText(gszTranslation[PosDialogAddProjectComputers]);

	m_buttonWebsite.SetWindowText(gszTranslation[PosDialogAddProjectButtonWeb]);
	m_buttonAddProject.SetWindowText(gszTranslation[PosDialogAddProjectButtonAdd]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
// Translation

	m_editLoginPasswrd2.ShowWindow(SW_HIDE);
	m_editUserName.ShowWindow(SW_HIDE);
	m_staticUserName.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

bool CDlgAddProject::CheckValidPlatform(int iItem)
{
	CString sPlatform;

	int iSize = (int) m_allProjectsList.projects[iItem]->platforms.size();

	if (iSize == 0) return true;	// projects that forgot the platform

	for (int iCount1 = 0; iCount1 < iSize ; iCount1++)
	{
		sPlatform = m_allProjectsList.projects[iItem]->platforms[iCount1].c_str();
//		int iBracket;
//		iBracket = sPlatform.Find("[");
//		if (iBracket)
//		{
//			sPlatform = sPlatform.Mid(0,iBacket);
//		}

		for (int iCount2 = 0; iCount2 < m_iNrOfStrings; iCount2++)
		{
			if (sPlatform.Find(m_returnStringNr[iCount2].m_sStr))
			{
				return true;
			}
		}
	}
	return false;
}

void CDlgAddProject::BreakUpText(CString *pTxt, int iBreak)
{
	CString sTxt, sNewTxt;
	int iPosition, iPositionOk, iTimeout;

	sTxt = *pTxt;
	iPositionOk = 0;
	iPosition = 0;
	iTimeout = 0;

	while (sTxt.GetLength() > iBreak-1)
	{
		if (iTimeout++ > 10000)
		{
			sNewTxt += "\r\n";
			sNewTxt += sTxt;
			sTxt = "";
		}
		iPosition = sTxt.Find(' ',iPosition+1);
		if (iPosition < iBreak)
		{
			iPositionOk = iPosition;
		}
		else
		{
			sNewTxt += "\r\n";
			sNewTxt += sTxt.Left(iPositionOk);
			sTxt = sTxt.Mid(iPositionOk+1);
			iPosition = 0;
		}
	}
	sNewTxt += "\r\n";
	sNewTxt += sTxt;	

	*pTxt = sNewTxt;
}

void CDlgAddProject::UpdateProjectList()
{
	m_editError.SetWindowText("");
	m_editSelectProject.SetWindowText("");
	m_editAddProjectsDescription.SetWindowText("");

	m_bGotAllProjectsList = false;
	m_iNrOfStrings = 0;
	m_listAddProjects.ResetContent();
	for (int iCount = 0; iCount < PLATFORM_LIST; iCount++)
	{
		m_returnStringNr[iCount].m_iNr = iCount;
		m_returnStringNr[iCount].m_sStr[0] = 0; 
		::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_GETPLATFORM,(WPARAM) this->m_hWnd, (LPARAM) &m_returnStringNr[iCount]);
	}
}

void CDlgAddProject::SetPlatformIcons(std::vector<std::string> *pPlatforms)
{
	std::string sPlatform;
	std::size_t found;

	PlatformIconsHide();

	for (int iCount = 0; iCount < (int) pPlatforms->size(); iCount++)
	{
		sPlatform = pPlatforms->at(iCount).c_str();
		found = sPlatform.find("windows");
		if (found != std::string::npos) 	m_process_windows.ShowWindow(SW_SHOW);
		found = sPlatform.find("apple-darwin");
		if (found != std::string::npos)  m_process_mac.ShowWindow(SW_SHOW);

		found = sPlatform.find("pc-linux");
		if (found != std::string::npos)  m_process_linux.ShowWindow(SW_SHOW);

		found = sPlatform.find("freebsd");
		if (found != std::string::npos)  m_process_freebsd.ShowWindow(SW_SHOW);

		found = sPlatform.find("box");
		if (found != std::string::npos)  m_process_virtual.ShowWindow(SW_SHOW);

		found = sPlatform.find("ati");
		if (found != std::string::npos)  m_process_ati.ShowWindow(SW_SHOW);

		bool bNvidia = false;

		found = sPlatform.find("nvidia");
		if (found != std::string::npos) bNvidia = true;
		found = sPlatform.find("cuda");
		if (found != std::string::npos) bNvidia = true;

		if (bNvidia) m_process_nvidia.ShowWindow(SW_SHOW);

		found = sPlatform.find("arm-android");
		if (found != std::string::npos)  m_process_android.ShowWindow(SW_SHOW);
	}
}

void CDlgAddProject::PlatformIconsHide()
{
	m_process_windows.ShowWindow(SW_HIDE);
	m_process_mac.ShowWindow(SW_HIDE);
	m_process_linux.ShowWindow(SW_HIDE);
	m_process_freebsd.ShowWindow(SW_HIDE);
	m_process_virtual.ShowWindow(SW_HIDE);
	m_process_ati.ShowWindow(SW_HIDE);
	m_process_nvidia.ShowWindow(SW_HIDE);
	m_process_android.ShowWindow(SW_HIDE);
}

void CDlgAddProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADD_PROJECTS, m_listAddProjects);
	DDX_Control(pDX, IDC_EDIT_PROJECT_INFO, m_editAddProjectsDescription);
	DDX_Control(pDX, IDC_EDIT_PROJECT_LOGIN_NAME, m_editLoginName);
	DDX_Control(pDX, IDC_EDIT_PROJECT_LOGIN_PASSWRD, m_editLoginPasswrd);
	DDX_Control(pDX, IDC_EDIT_SELECT_PROJECTS, m_editSelectProject);
	DDX_Control(pDX, IDADD, m_buttonAddProject);
	DDX_Control(pDX, IDC_EDIT_PROJECT_ERROR, m_editError);
	DDX_Control(pDX, IDC_EDIT_PROJECT_LOGIN_PASSWRD2, m_editLoginPasswrd2);
	DDX_Control(pDX, IDC_CHECK_CREATE_ACCOUNT, m_checkCreate);
	DDX_Control(pDX, IDC_EDIT_PROJECT_USER_NAME, m_editUserName);
	DDX_Control(pDX, IDC_STATIC_USERNAME, m_staticUserName);
	DDX_Control(pDX, IDC_TEXT_ADD_PROJECT_LOGIN, m_textLogin);
	DDX_Control(pDX, IDC_TEXT_ADD_PROJECT_PASSWORD, m_textPassword);
	DDX_Control(pDX, IDWWW, m_buttonWebsite);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_CHECK_CREATE_PROJECT_COMPUTERS, m_checkComputers);
	DDX_Control(pDX, IDC_LIST_ADD_PROJECTS_COMPUTERS, m_listAddComputers);
	DDX_Control(pDX, IDC_TEXT_ADD_PROJECT_MAIN, m_textAddMain);
	DDX_Control(pDX, IDC_LIST_ADD_PROJECTS_COMPUTERS_MAIN, m_listAddMainComputer);
	DDX_Control(pDX, IDC_BITMAP_P_WINDOWS, m_process_windows);
	DDX_Control(pDX, IDC_BITMAP_P_MAC, m_process_mac);
	DDX_Control(pDX, IDC_BITMAP_P_LINUX, m_process_linux);
	DDX_Control(pDX, IDC_BITMAP_P_VIRTUAL, m_process_virtual);
	DDX_Control(pDX, IDC_BITMAP_P_ATI, m_process_ati);
	DDX_Control(pDX, IDC_BITMAP_P_ANDROID, m_process_android);
	DDX_Control(pDX, IDC_BITMAP_P_NVIDIA, m_process_nvidia);
	DDX_Control(pDX, IDC_BITMAP_P_FREEBSD, m_process_freebsd);
}

BEGIN_MESSAGE_MAP(CDlgAddProject, CDialog)
	ON_MESSAGE(UWM_MSG_ADDPROJECT_OPEN_DIALOG, OnAddComputerOpen)
//	ON_MESSAGE(UWM_MSG_ADDPROJECT_SET_STRING_ID, OnSetComputerStringId)
	ON_MESSAGE(UWM_MSG_ADDPROJECT_SET_THREAD_ID, OnSetComputerThreadId)


	ON_MESSAGE(UWM_MSG_THREAD_GET_ALL_PROJECTS_LIST_READY, OnGetAllProjectsList)
	ON_MESSAGE(UWM_MSG_THREAD_PROJECT_CONFIG_READY, OnProjectConfigReady)
	ON_MESSAGE(UWM_MSG_THREAD_PROJECT_CONFIG_POLL_READY, OnProjectConfigPollReady)

	ON_MESSAGE(UWM_MSG_THREAD_ACCOUNT_LOOKUP_READY, OnAccountLookupReady)
	ON_MESSAGE(UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL_READY, OnAccountLookupPollReady)

	ON_MESSAGE(UWM_MSG_THREAD_PROJECT_ATTACH_READY, OnProjectAttachReady)


	ON_MESSAGE(UWM_MSG_THREAD_CREATE_ACCOUNT_READY, OnCreateAccountReady)
	ON_MESSAGE(UWM_MSG_THREAD_CREATE_ACCOUNT_POLL_READY, OnCreateAccountPollReady)

	ON_MESSAGE(UWM_MSG_THREAD_GETPLATFORM_READY, OnGetPlatform)

	ON_LBN_SELCHANGE(IDC_LIST_ADD_PROJECTS, &CDlgAddProject::OnLbnSelchangeListAddProjects)
	ON_BN_CLICKED(IDADD, &CDlgAddProject::OnBnClickedAdd)
	ON_BN_CLICKED(IDWWW, &CDlgAddProject::OnBnClickedWww)
	ON_BN_CLICKED(IDC_CHECK_CREATE_ACCOUNT, &CDlgAddProject::OnBnClickedCheckCreateAccount)
	ON_BN_CLICKED(IDC_CHECK_CREATE_PROJECT_COMPUTERS, &CDlgAddProject::OnBnClickedCheckCreateProjectComputers)
	ON_LBN_SELCHANGE(IDC_LIST_ADD_PROJECTS_COMPUTERS_MAIN, &CDlgAddProject::OnLbnSelchangeListAddProjectsComputersMain)
END_MESSAGE_MAP()

// CDlgAddProject message handlers

LRESULT CDlgAddProject::OnAddComputerOpen(WPARAM parm1, LPARAM parm2)
{
	CString *psComputer;
	CString sTxt;

	m_iThreadId = (int) parm1;
	m_pDoc = (CRemoteCombinedDoc *) parm1;
	psComputer = (CString *) parm2;

	sTxt = gszTranslation[PosDialogAddProjectTitle];
	sTxt+= *psComputer;

	this->SetWindowText(sTxt);

	UpdateProjectList();

	PlatformIconsHide();

	return 0;
}

LRESULT CDlgAddProject::OnSetComputerThreadId(WPARAM parm1, LPARAM parm2)
{
	char	*pcComputerId;
	int		iThreadId;
	CString	sTxt;

	m_pDoc = (CRemoteCombinedDoc *) parm2;

//	m_pThreadId = (int *) parm2;
	m_iThreadIdItems = m_pDoc->GetRpcSelectedComputers();

	m_listAddComputers.ResetContent();
	m_listAddMainComputer.ResetContent();

	int iItems = m_pDoc->GetRpcSelectedComputers();

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		m_pDoc->GetComputerIds(&pcComputerId, iCount);
		m_listAddComputers.AddString(pcComputerId);
		m_listAddMainComputer.AddString(pcComputerId);
	}

	// find the right computer name
	{
		for (int iCount = 0; iCount < iItems; iCount++)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCount);
			if (m_iThreadId == iThreadId)
			{
				sTxt = gszTranslation[PosDialogAddProjectTitle];
				m_pDoc->GetComputerIds(&pcComputerId, iCount);
				sTxt+= pcComputerId;
				this->SetWindowText(sTxt);
				m_listAddMainComputer.SelectString(0, pcComputerId);
				break;
			}
		}
	}

	return 0;
}



LRESULT CDlgAddProject::OnGetPlatform(WPARAM parm1, LPARAM parm2)
{
	int iStatus;
	bool bFound;


	iStatus = (int) parm1;
	if (iStatus < 0) return 0;

	bFound = (parm2 == TRUE);

	if (!bFound)
	{
		if (m_bGotAllProjectsList) return 0;	// skip the rest
		m_bGotAllProjectsList = true;
		::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_GET_ALL_PROJECTS_LIST,(WPARAM) this->m_hWnd, (LPARAM) &m_allProjectsList);
	}
	else m_iNrOfStrings++;

	return 0;
}

LRESULT CDlgAddProject::OnGetAllProjectsList(WPARAM parm1, LPARAM parm2)
{
	int iStatus;
	int iItems;

	iStatus = (int) parm1;
	if (iStatus < 0) return 0;

	this->ShowWindow(SW_SHOW);
	this->SetForegroundWindow();

	iItems = (int) m_allProjectsList.projects.size();

	for (int iCount = 0; iCount < iItems; iCount++)
	{
//		if (CheckValidPlatform(iCount))	
		m_listAddProjects.AddString(m_allProjectsList.projects[iCount]->name.c_str());
	}

	return 0;
}

LRESULT CDlgAddProject::OnProjectConfigReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt;
	int iStatus;
	
	iStatus = (int) parm1;

	if (iStatus<0) 
	{
		sTxt.Format("Error %d",iStatus);
		m_editError.SetWindowText(sTxt);
		m_buttonAddProject.EnableWindow(TRUE);
		return 0;
	}

	::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PROJECT_CONFIG_POLL,(WPARAM) this->m_hWnd, (LPARAM) &m_projectConfig);
	return 0;
}

LRESULT CDlgAddProject::OnProjectConfigPollReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt;
	int iStatus, iCheckCreate;

	iStatus = (int) parm1;

	if (iStatus>=0)
	{
		m_accountIn.clear();
		m_accountIn.url		= m_cAccountUrl;
//		if (m_projectConfig.uses_username) m_accountIn.user_name = m_cAccountEmailOrUser;
		//else m_accountIn.email_addr = m_cAccountEmailOrUser;
		m_accountIn.email_addr = m_cAccountEmailOrUser;
		m_accountIn.passwd	= m_cAccountPassWrd;
		m_accountIn.user_name = m_cAccountCreateUser;

		if (m_projectConfig.error_num != BOINC_SUCCESS )
		{
			GetErrorString(m_projectConfig.error_num, &sTxt);
			m_editError.SetWindowText(sTxt);
			m_buttonAddProject.EnableWindow(TRUE);
			return 0;
		}
	}

	iCheckCreate = m_checkCreate.GetCheck();
	if (iCheckCreate)
	{
		::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_CREATE_ACCOUNT,(WPARAM) this->m_hWnd, (LPARAM) &m_accountIn);	
	}
	else ::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_ACCOUNT_LOOKUP,(WPARAM) this->m_hWnd, (LPARAM) &m_accountIn);

	return 0;
}

LRESULT CDlgAddProject::OnAccountLookupReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt;
	int iStatus;

	iStatus = (int) parm1;

	if (iStatus>=0)
	{
		::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL,(WPARAM) this->m_hWnd, (LPARAM) &m_accountOut);
	}
	else
	{
		GetErrorString(iStatus, &sTxt);
		sTxt += "\r\n";
		sTxt += m_accountOut.error_msg.c_str();
		m_editError.SetWindowText(sTxt);
		m_buttonAddProject.EnableWindow(TRUE);
	}
	return 0;
}

LRESULT CDlgAddProject::OnAccountLookupPollReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt, sComputer;
	char	*pcComputerId;
	int iStatus, iThreadId;

	if (m_pDoc == NULL) return 0;

	iStatus = (int) parm1;

	if (iStatus>=0)
	{
		if (m_accountOut.error_num != BOINC_SUCCESS )
		{
			GetErrorString(m_accountOut.error_num, &sTxt);
			sTxt += "\r\n";
			sTxt += m_accountOut.error_msg.c_str();
			m_editError.SetWindowText(sTxt);
			m_buttonAddProject.EnableWindow(TRUE);
		}
		else
		{
			m_projectAttach.m_sAuth = (char *) m_accountOut.authenticator.c_str();
			m_projectAttach.m_sUrl = m_cAccountUrl;
			::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PROJECT_ATTACH,(WPARAM) this->m_hWnd, (LPARAM) &m_projectAttach);

			if (m_checkComputers.GetCheck() == TRUE)		// add on additional computers
			{
	
				int iItems = m_listAddComputers.GetSelCount();
				CArray<int,int> aryListBoxSel;
				aryListBoxSel.SetSize(iItems);
				m_listAddComputers.GetSelItems(iItems, aryListBoxSel.GetData());

				for (int iCount = 0; iCount < iItems; iCount++)
				{
					m_listAddComputers.GetText(aryListBoxSel[iCount],sComputer);
					for (int iCountFind = 0; iCountFind < m_iThreadIdItems; iCountFind++)
					{
						m_pDoc->GetComputerIds(&pcComputerId, iCountFind);
						if (pcComputerId  == sComputer)
						{
							m_pDoc->GetRpcThreadId(&iThreadId, iCountFind);
							if (iThreadId != m_iThreadId)
							{
								::PostThreadMessage(iThreadId,UWM_MSG_THREAD_PROJECT_ATTACH,(WPARAM) this->m_hWnd, (LPARAM) &m_projectAttach);
							}
						}
					}
				}

			}

		}
	}
	return 0;
}

LRESULT CDlgAddProject::OnProjectAttachReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt, sTxtNow;
	char	*pcComputer;

	int iStatus;

	iStatus = (int) parm1;
	pcComputer = (char *) parm2;

	m_buttonAddProject.EnableWindow(TRUE);

	if (iStatus>=0)
	{
		sTxt = gszTranslation[PosDialogAddProjectMsgAddedSuccessfully] ;							//"Project added successfully"
		sTxt+= " (";
		sTxt+= pcComputer;
		sTxt+= ")\r\n";

		if (!m_bFirstStatusText) m_editError.GetWindowText(sTxtNow);
		sTxtNow+= sTxt;
		m_bFirstStatusText = false;
		m_editError.SetWindowText(sTxtNow);
	}
	else
	{
		GetErrorString(iStatus, &sTxt);
		sTxt+= " (";
		sTxt+= pcComputer;
		sTxt+= ")\r\n";
		if (!m_bFirstStatusText) m_editError.GetWindowText(sTxtNow);
		sTxtNow+= sTxt;
		m_bFirstStatusText = false;
		m_editError.SetWindowText(sTxtNow);
	}
	return 0;
}

LRESULT CDlgAddProject::OnCreateAccountReady(WPARAM parm1, LPARAM parm2)
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
		m_buttonAddProject.EnableWindow(TRUE);
		return 0;
	}

	::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_CREATE_ACCOUNT_POLL,(WPARAM) this->m_hWnd, (LPARAM) &m_accountOut);

	return 0;

}

LRESULT CDlgAddProject::OnCreateAccountPollReady(WPARAM parm1, LPARAM parm2)
{
	CString sTxt;
	int iStatus;

	iStatus = (int) parm1;

	if (iStatus>=0)
	{
		if (m_accountOut.error_num != BOINC_SUCCESS )
		{
			GetErrorString(m_accountOut.error_num, &sTxt);
			sTxt += "\r\n";
			sTxt += m_accountOut.error_msg.c_str();
			m_editError.SetWindowText(sTxt);
			m_buttonAddProject.EnableWindow(TRUE);
		}
		else
		{
			m_projectAttach.m_sAuth = (char *) m_accountOut.authenticator.c_str();
			m_projectAttach.m_sUrl = m_cAccountUrl;
			::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PROJECT_ATTACH,(WPARAM) this->m_hWnd, (LPARAM) &m_projectAttach);
		}
	}
	return 0;
}


void CDlgAddProject::GetErrorString(int iError, CString *psError)
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
		default:
			psError->Format("AddProject: Error %d",iError);
	}	
}

void CDlgAddProject::OnLbnSelchangeListAddProjects()
{
	int iSelected, iSizeProjects;
	CString sSelected, sTxt, sTxtBreakup;

	m_editError.SetWindowText("");

	iSelected = m_listAddProjects.GetCurSel();
	m_listAddProjects.GetText(iSelected,sSelected);

	iSizeProjects = (int) m_allProjectsList.projects.size();

	for (int iCount = 0; iCount < iSizeProjects; iCount++)
	{
		if (sSelected == m_allProjectsList.projects[iCount]->name.c_str())
		{
			m_editSelectProject.SetWindowText(m_allProjectsList.projects[iCount]->url.c_str());

			sTxt = m_allProjectsList.projects[iCount]->home.c_str();
			sTxt+= "\r\n\r\n";
			sTxt+= m_allProjectsList.projects[iCount]->specific_area.c_str();
			sTxt+= "\r\n\r\n";
			sTxtBreakup = m_allProjectsList.projects[iCount]->description.c_str();
			BreakUpText(&sTxtBreakup,55);
			sTxt+= sTxtBreakup;
			m_editAddProjectsDescription.SetWindowText(sTxt);
			SetPlatformIcons(&m_allProjectsList.projects[iCount]->platforms);
		}
	}

	m_editUserName.SetWindowText("");

//	m_editAddProjects
}

void CDlgAddProject::OnBnClickedAdd()
{
	CString sLogin, sPasswrd, sPasswrd2, sUrl, sUserName, sTextNow;

	m_editLoginName.GetWindowText(sLogin);
	m_editLoginPasswrd.GetWindowText(sPasswrd);
	m_editLoginPasswrd2.GetWindowText(sPasswrd2);
	m_editUserName.GetWindowText(sUserName);
	m_editError.SetWindowText("");

	if ((sLogin.GetLength() < 4) || sPasswrd.GetLength() < 4)
	{
		m_editError.SetWindowText(gszTranslation[PosDialogAddProjectMsgInvalidLoginName]);		//"Invalid login name, \r\nemail address or password.\r\nToo short!"
		return;
	}
	
	if (m_checkCreate.GetCheck())
	{
		if (sPasswrd != sPasswrd2)
		{
			m_editError.SetWindowText(gszTranslation[PosDialogAddProjectMsgPasswordsEqual]);				//"Passwords must be equal!"
			return;
		}
		if (sUserName.GetLength() <= 0)
		{
			m_editError.SetWindowText(gszTranslation[PosDialogAddProjectMsgUsernameMissing]);								//"Missing user name!"
			return;
		}

	}

	// add the project

	m_editSelectProject.GetWindowText(sUrl);
	if ((sUrl.GetLength() < 4))
	{
		m_editError.SetWindowText(gszTranslation[PosDialogAddProjectMsgProjectUrlShort]);							//"Invalid project url.\r\nToo short!"
		return;
	}

	m_bAccountEmail = true;

	strcpy_s (m_cAccountUrl,99,sUrl.GetBuffer());	
	strcpy_s (m_cAccountEmailOrUser,99,sLogin.GetBuffer());	
	strcpy_s (m_cAccountPassWrd,99,sPasswrd.GetBuffer());	
	strcpy_s (m_cAccountCreateUser,99,sUserName);
	

	m_buttonAddProject.EnableWindow(FALSE);
	sTextNow = gszTranslation[PosDialogAddProjectMsgBusy];
	sTextNow += "\r\n"; //"Busy....."
	m_editError.SetWindowText(sTextNow);						
	m_bFirstStatusText = true;

	::PostThreadMessage(m_iThreadId,UWM_MSG_THREAD_PROJECT_CONFIG,(WPARAM) this->m_hWnd, (LPARAM) &m_cAccountUrl);
}

void CDlgAddProject::OnBnClickedWww()
{
	CString sUrl;

	m_editSelectProject.GetWindowText(sUrl);

	if (sUrl.GetLength() > 0) ShellExecute(NULL,_T("open"), sUrl,NULL,NULL,SW_SHOWNORMAL);
}

void CDlgAddProject::OnBnClickedCheckCreateAccount()
{
	int iCheck;

	iCheck = m_checkCreate.GetCheck();

	if (iCheck)
	{
		m_editLoginPasswrd2.ShowWindow(SW_SHOW);
		m_editUserName.ShowWindow(SW_SHOW);
		m_staticUserName.ShowWindow(SW_SHOW);
	}
	else
	{
		m_editLoginPasswrd2.ShowWindow(SW_HIDE);
		m_editUserName.ShowWindow(SW_HIDE);
		m_staticUserName.ShowWindow(SW_HIDE);
	}
}


void CDlgAddProject::OnBnClickedCheckCreateProjectComputers()
{
	int iCheck;

	iCheck = m_checkComputers.GetCheck();

	if (iCheck) m_listAddComputers.ShowWindow(SW_SHOW);
	else  m_listAddComputers.ShowWindow(SW_HIDE);
}


void CDlgAddProject::OnLbnSelchangeListAddProjectsComputersMain()
{
	int iSelected;
	CString sSelected, sTxt;
	char	*pcComputerId;
	int		iThreadId;

	iSelected = m_listAddMainComputer.GetCurSel();
	m_listAddMainComputer.GetText(iSelected,sSelected);

//	pcComputerId = m_pcComputerId;
//	pThreadId = m_pThreadId;
	for (int iCountFind = 0; iCountFind < m_iThreadIdItems; iCountFind++)
	{
		m_pDoc->GetComputerIds(&pcComputerId, iCountFind);
		if (pcComputerId  == sSelected)
		{
			m_pDoc->GetRpcThreadId(&iThreadId, iCountFind);
			m_iThreadId = iThreadId;
			sTxt = gszTranslation[PosDialogAddProjectTitle];
			sTxt+= pcComputerId;
			this->SetWindowText(sTxt);
			UpdateProjectList();
			break;
		}
	}
}

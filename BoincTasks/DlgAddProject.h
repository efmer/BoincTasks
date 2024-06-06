#pragma once

#include "ThreadRpc.h"
#include "afxwin.h"

/*
Add button
UWM_MSG_THREAD_PROJECT_CONFIG
GetProjectConfig(pUrl)
UWM_MSG_THREAD_PROJECT_CONFIG_READY

OnProjectConfigReady
UWM_MSG_THREAD_PROJECT_CONFIG_POLL
GetProjectConfigPoll(pPc)
UWM_MSG_THREAD_PROJECT_CONFIG_POLL_READY

OnProjectConfigPollReady

if create new account
{
	UWM_MSG_THREAD_CREATE_ACCOUNT
	CreateAccount(pAi)
	UWM_MSG_THREAD_CREATE_ACCOUNT_READY

	OnCreateAccountReady
	UWM_MSG_THREAD_CREATE_ACCOUNT_POLL
	CreateAccountPoll(pAo);
	UWM_MSG_THREAD_CREATE_ACCOUNT_POLL_READY		
}

UWM_MSG_THREAD_ACCOUNT_LOOKUP
LookupAccount(pAi);
UWM_MSG_THREAD_ACCOUNT_LOOKUP_READY


OnAccountLookupReady
UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL
LookUpAccountPoll(pAo);
UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL_READY

OnAccountLookupPollReady
UWM_MSG_THREAD_PROJECT_ATTACH
ProjectAttach(pPa);
UWM_MSG_THREAD_PROJECT_ATTACH_READY

OnProjectAttachReady
Ready.........
*/

#define PLATFORM_LIST	20

// CDlgAddProject dialog
class CRemoteCombinedDoc;
class CDlgAddProject : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddProject)

public:
	CDlgAddProject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddProject();
	virtual BOOL OnInitDialog();


// Dialog Data
	enum { IDD = IDD_DIALOG_ADD_PROJECT1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool				m_bFirstStatusText;

	int					m_iThreadId;
	CRpcReturnStringWithNr m_returnStringNr[PLATFORM_LIST+1];
	int					m_iNrOfStrings;
	bool				m_bGotAllProjectsList;

	ALL_PROJECTS_LIST	m_allProjectsList;
	PROJECT_CONFIG		m_projectConfig;
//	HOST_INFO			m_hostInfo;
	bool				m_bValidPlatform ;

	ACCOUNT_IN			m_accountIn;
	ACCOUNT_OUT			m_accountOut;

	CRpcProjectAttach	m_projectAttach;

	char				m_cAccountUrl[100];
	bool				m_bAccountEmail;
	char				m_cAccountEmailOrUser[100];
	char				m_cAccountPassWrd[100];
	char				m_cAccountCreateUser[100];

	int					m_iThreadIdItems;

	CRemoteCombinedDoc	*m_pDoc;

//	int					*m_pThreadId;
//	char				**m_pcComputerId;//[MAX_COMPUTERS_REMOTE+1];

	void UpdateProjectList();
	void SetPlatformIcons(std::vector<std::string> *pPlatforms);
	void PlatformIconsHide();
	bool CheckValidPlatform(int iItem);
	void BreakUpText(CString *pTxt, int iBreak);
	void GetErrorString(int iError, CString *psError);

	afx_msg LRESULT OnAddComputerOpen(WPARAM parm1, LPARAM parm2);
//	afx_msg LRESULT OnSetComputerStringId(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSetComputerThreadId(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnGetPlatform(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnGetAllProjectsList(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProjectConfigReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnProjectConfigPollReady(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnAccountLookupReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAccountLookupPollReady(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnProjectAttachReady(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnCreateAccountReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnCreateAccountPollReady(WPARAM parm1, LPARAM parm2);

	DECLARE_MESSAGE_MAP()
public:

	CListBox m_listAddProjects;
	CListBox m_listAddComputers;
	afx_msg void OnLbnSelchangeListAddProjects();
	CEdit m_editAddProjectsDescription;
	afx_msg void OnBnClickedAdd();
	CEdit m_editLoginName;
	CEdit m_editLoginPasswrd;
	CEdit m_editLoginPasswrd2;
	CEdit m_editSelectProject;
	CButton m_buttonAddProject;
	CEdit m_editError;
	afx_msg void OnBnClickedWww();

	afx_msg void OnBnClickedCheckCreateAccount();
	CButton m_checkCreate;
	CEdit m_editUserName;
	CStatic m_staticUserName;
	CStatic m_textLogin;
	CStatic m_textPassword;
	CButton m_buttonWebsite;
	CButton m_buttonCancel;
	CButton m_checkComputers;

	afx_msg void OnBnClickedCheckCreateProjectComputers();

	CStatic m_textAddMain;

	CListBox m_listAddMainComputer;
	afx_msg void OnLbnSelchangeListAddProjectsComputersMain();

	CStatic m_process_windows;
	CStatic m_process_mac;
	CStatic m_process_linux;
	CStatic m_process_freebsd;
	CStatic m_process_virtual;
	CStatic m_process_ati;
	CStatic m_process_nvidia;
	CStatic m_process_android;
};

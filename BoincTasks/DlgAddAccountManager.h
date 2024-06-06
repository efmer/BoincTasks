#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define ACCOUNT_MANAGERS_TOTAL	3

#define ACCOUNT_MANAGER_0		"BOINCstats BAM!"
#define ACCOUNT_MANAGER_ULR_0	"http://bam.boincstats.com/"
#define ACCOUNT_MANAGER_1		"Extremadura@home"
#define ACCOUNT_MANAGER_ULR_1	"http://boinc.unex.es/extremadurathome"
#define ACCOUNT_MANAGER_2		"GridRepublic"
#define ACCOUNT_MANAGER_ULR_2	"http://www.gridrepublic.org/"

class CRemoteCombinedDoc;
class CAccountManagerData
{
public:
	CString m_sComputer;
//	char	**m_pcComputerId;

	int		m_iThreadId;
//	int		m_iThreadCount;
//	int		*m_piThreadId;

	CRemoteCombinedDoc	*m_pDoc;

	HWND	m_hWndTasks;
};


// CDlgAccountManager dialog

class CRpcManagerAttach;
struct ACCT_MGR_INFO;
struct ACCT_MGR_RPC_REPLY;
class CDlgAddAccountManager : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddAccountManager)

public:
	CDlgAddAccountManager(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddAccountManager();
	virtual BOOL OnInitDialog();

	bool	m_bAccountManagerFound;


// Dialog Data
	enum { IDD = IDD_DIALOG_ACCOUNT_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void	Populate();
	void	GetErrorString(int iError, CString *psError);

	CAccountManagerData m_accountManagerData;

//	int			m_iThreadId;

//	int			m_iThreadIdItems;
//	int			*m_pThreadId;
//	char		**m_pcComputerId;//[MAX_COMPUTERS_REMOTE+1];

//	HWND		m_hWndTasks;

	CString		m_sAccountManager[3][2];

    ACCT_MGR_INFO  *m_pAccountManagerInfo;
	ACCT_MGR_RPC_REPLY *m_pAccountMangerReply;

	CRpcManagerAttach *m_pAccountManagerAttach;

	char			m_cAccountUrl[100];
	char			m_cAccountEmailOrUser[100];
	char			m_cAccountPassWrd[100];

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnAddComputerOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnGetAccountManagerReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnCreateAccountReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAccountPollReady(WPARAM parm1, LPARAM parm2);

public:
	CListBox m_list;
	CListBox m_listComputers;

	CEdit m_editUrl;
	CEdit m_editUserName;
	CEdit m_editPassword;	
	CEdit m_editError;

	CStatic m_textUserName;
	CStatic m_textPassword;
	CButton m_buttonAdd;
	CButton m_buttonCancel;
	CButton m_buttonWebsite;

	afx_msg void OnLbnSelchangeListAccountManager();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();

	afx_msg void OnEnChangeEditSelectAccountManager();
	afx_msg void OnBnClickedWww();


	afx_msg void OnLbnSelchangeListAccountManagerComputers();
};

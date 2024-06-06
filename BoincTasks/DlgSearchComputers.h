#pragma once
#include "afxcmn.h"
//#include "winsock2.h"
#include "ListCtrlFindComputers.h"
#include "ThreadRpcClient.h"
#include "afxwin.h"

#include "ThreadHostSearch.h"

#define	MAX_SEARCH_COMPUTERS			300

#define STATE_SEARCH_STOPPED			-1
#define STATE_SEARCH_ADDRESS_RANGE		1
#define STATE_SEARCH_FINDNAME			2


#define	SEARCH_COLUMN_COMPUTER			0
#define	SEARCH_COLUMN_IP				1
#define	SEARCH_COLUMN_MAC				2
#define	SEARCH_COLUMN_BOINC				3
#define	SEARCH_COLUMN_TTHROTTLE			4
#define	SEARCH_COLUMN_TOOLBOX			5
#define	SEARCH_COLUMN_PASSWORD			6

//#define STATE_SEARCH_IDLE				-1
//#define STATE_SEARCH_FINDNAME			1
//#define STATE_SEARCH_FINDADDRESS		2
//#define STATE_SEARCH_BOINC_TTHROTTLE	3
//#define STATE_SEARCH_FOUNDADDRESS		4

#define NUM_SEARCH_COMPUTERS_COLUMNS	7

class CListComputerFindItem
{
public:
	CString	m_sIp;
	CString	m_sMAC;
	CString m_sName;
	CString m_sPassword;
};

// CDlgSearchComputers dialog

class CListComputerFindItem;
class CLoggingMessage;
class CThreadHostSearch;
class CThreadRpcClient;
class CRpcInfoClientSearch;
class CDlgSearchComputers : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchComputers)

public:
	CDlgSearchComputers(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSearchComputers();
	virtual BOOL OnInitDialog();

	void	AddColumns();
	void	StartFullAddressScan();	
	void	Search();
	bool	CheckConnection(CString sName, bool *pbBoinc, bool *pbTThrottle);

// Dialog Data
	enum { IDD = IDD_DIALOG_FIND_COMPUTER };

protected:
	void	CheckIllegal();
	void	DisableButtons();
	void	EnableButtons();
	CString ConnectionStatus(int iConnectionStatus);
	void	ClipBoard(int iColumn);

	int		m_iSearchState;
	int		m_iConnectionNumber;

	bool	m_bValidAddress;
	in_addr	m_address;
	in_addr	m_addressFrom;
	in_addr	m_addressTo;

	CImageList m_StateImageList;

	CLoggingMessage *m_pLogging;
	bool	m_bLog;

	CFont	*m_pFont;

	CRpcInfoClientSearch	*m_pThreadSearchTThrottleBoinc;
	CRpcInfoClientSearch	*m_pThreadInfo[255+2];

	int					m_iThreadHostSearchID;

	CHostSearchInfo		m_searchInfo;
	
	//	bool				m_bThreadbusy;
	int					m_iThreadBusy;

	CString				m_sAddressSearch;
	int					m_iThreadResult;
	int					m_iProgress;

	bool				m_bCancel;

	bool				m_bButtonsEnabled;


	CString				m_sLocalHostIp;
	CString				m_sLocalHostName;

	std::deque<CListComputerFindItem*> *m_plListComputerFind;
	HWND				m_hMainWwd;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnCheckTThrottleBoincReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnClientReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnHostSearchReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnHostSearchQuit(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnChangedFont(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSearchComputerStart(WPARAM wParam,LPARAM lParam);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnClipBoardIp();
	afx_msg void OnClipBoardMac();
	afx_msg void OnClipBoardAll();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
public:
	CListCtrlFindComputers m_listComputers;
	afx_msg void OnBnClickedSearchagain();

	afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_addressFromCtrl;

//	CString m_sName[MAX_SEARCH_COMPUTERS+1];
//	CString m_sIp[MAX_SEARCH_COMPUTERS+1];
	CString	m_sPassWord;

	int		m_iComputersSelected;

	CEdit m_editAddressTo;
	afx_msg void OnBnClickedAddress();
	CButton m_buttonAddSelected;
	CButton m_buttonSearchAgain;
	CButton m_buttonSearchRange;
	CButton m_buttonCancel;

	CStatic m_textAddress;
	afx_msg void OnBnClickedCancel();
	CProgressCtrl m_barProgress;
	CStatic m_textPassword;
	CEdit m_editPassword;
	afx_msg void OnEnChangeEditSearchComputerPassword();
	CEdit m_editThreadLeft;
};

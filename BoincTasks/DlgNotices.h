#pragma once

#define NOTICES_PUSH_INTERVAL			86400

#define	NOTICES_LOAD_ANYWAY				"BT Load Anyway"
#define	NOTICES_NEW_BOINC_VERSION		"BOINC new version"

//#define NOTICES_DETECT_NEW_BOINC_VERSION "http://boinc.berkeley.edu/download.php>"


#define NOTICES_UPDATE_VISIBLE		60

#define NOTICES_ID_BT_VERSION		0
#define NOTICES_ID_TT_VERSION		2
#define NOTICES_ID_BT_RULES			3
#define NOTICES_ID_FIRST			10

#define NOTICES_SORT_CATEGORY	0
#define NOTICES_SORT_PROJECT	1
#define NOTICES_SORT_CREATION	2
#define NOTICES_SORT_ARRIVAL	3
#define NOTICES_SORT_NOTUSED1	4
#define NOTICES_SORT_NOTUSED2	5

#define NOTICES_SORT_COUNT		6
#define NOTICES_SORT_MAX		NOTICES_SORT_NOTUSED2

// ^ a-z 0-1 up
// v z-a 9-1 down

#define	NOTICES_SORT_OPERATOR_UP		1
#define NOTICES_SORT_OPERATOR_DOWN		2
#define NOTICES_SORT_OPERATOR_NO		0

#define NOTICES_SORT_OPERATOR_MAX		NOTICES_SORT_OPERATOR_DOWN

// for every computer
class	CRpcInfoClient;
class	CRpcThreadInfoNotices;
class CComputerNoticesListItem
{


};

class CNoticeSorting
{
public:
	DWORD		m_dwSortOn;
	DWORD		m_dwSortOperator;
};

class CNoticeBoincTasksUpdate
{
public:
	bool		m_bRead;
	int			m_iCheckBeta;

	int			m_iType;

	double		m_dCurrentVersion;
	double		m_dVersion;
	double		m_dVersionBeta;

	CString		m_sVersionExe;
	CString		m_sVersionBetaExe;

	double		m_dTThrottleVersion;
	double		m_dTThrottleVersionBeta;

	CString		m_sTThrottleVersionExe;
	CString		m_sTThrottleVersionBetaExe;

};

class CNoticeBoincTasksRules
{
public:
	int			m_iActiveNew;
	int			m_iActive;
	_int64		m_iTime;
	CString		m_sRuleText;
};

class NOTICE;
class CNoticesInfoItem
{
public:
	UINT	m_iID;

	NOTICE	*m_pNotice;

	CString	sProject;
	CString sTitle;

	UINT	m_iChecksum;
	bool	m_bCollapsed;
	bool	m_bDeleted;

	double	m_dTime;

	bool	m_bAllowDelete;

	std::deque<CString *> m_lComputer;
};

class CRpcInfoClient;
class CRpcInfoNoticesItem
{
public:	
	char		*m_pcComputerId;
	CThreadRpc	*m_pThread;
	int			m_iStatusRpc;
};

// CDlgNotices dialog

class CRemoteCombinedDoc;
class CDlgNotices : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNotices)

public:
	CDlgNotices(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNotices();

	void	InitialUpdate(CRemoteCombinedDoc* pDoc);
	int		ReadXmlPush(CStringA *psATxt);

	bool	m_bDocumentIsClosing;

// Dialog Data
	enum { IDD = IDD_DIALOG_NOTICES_DUMMY };

	std::deque<NOTICE*> m_lNoticesPush;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int		ReadXml(void);
	void	WriteXml(void);

	void	EmptyNotice(CString sTxt);

	UINT	CheckSumDescription(char *pDescription);
	void	AddNoticesItem(NOTICE *pNoticeClient, int iIDNotices, bool bFirst=false);
	void	CopyNotice(NOTICE *pNoticeTo, NOTICE *pNoticeFrom);
	int		Compare(CNoticesInfoItem *pNoticesInfoItem1, CNoticesInfoItem *pNoticesInfoItem2, int iCountCompare);
	void	SortNotices();

	void	SendLogging(char *pcFrom, char *pcMessage, int iDebug);

// computer

	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;

	std::deque<CRpcInfoNoticesItem*>	m_lRpcInfoNotices;

	std::deque<CRpcThreadInfoNotices*>	m_lComputerRpcInfo;
	std::deque<CRpcInfoClient*>			m_lComputerRpcInfoClient;

//	CRpcThreadInfoNotices	*m_pthreadNotices;
//	CRpcInfoClient			*m_pRpcInfoClient;

// computer

	CRemoteCombinedDoc	*m_pDoc;

	bool	m_bThreadBusy;
	int		m_iThreadLocked;

	int		m_iThreadRpcId ;
	time_t	m_tLastUpdate;

	bool	m_bNavigate2Done;

	CNoticeBoincTasksUpdate m_noticeBoincTasksUpdate;
	CNoticeBoincTasksRules m_noticeBoincTasksRules;
	CNoticesInfoItem	*m_pNoticeUpdate;
	CNoticesInfoItem	*m_pNoticeUpdateTThrottle;
	CNoticesInfoItem	*m_pNoticeRules;

	bool				m_bCheckTThrottle;


	CString				m_sNoticesText;

	CNoticeSorting		m_sorting[NOTICES_SORT_COUNT+1];

	std::deque<CNoticesInfoItem*> m_lNotices;
	UINT	m_iIDNotices;
	int		m_iNewNotices;
	bool	m_bNoticesCollapseDeleteChanged;

	bool	m_bDisabled;

	time_t	m_tLastNoticeAlert;
	time_t	m_tLastPushNotices;
	time_t m_tPushNoticesInterval;
	bool	m_bPushNotices;

	afx_msg LRESULT OnTimerNotices(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnReadyRpc(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnReadyRpcClient(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnBoincTasksVersion(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnBoincTasksRule(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetCollapseOrDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:

};

#pragma once

#define NUM_REMOTE_MESSAGE_COLUMNS 5

#define MESSAGE_COLOR_NORMAL	0
#define MESSAGE_COLOR_PRIORITY	1

#define MESSAGE_COLOR_NORMAL_LINE	2
#define MESSAGE_COLOR_PRIORITY_LINE	3

#define	MESSAGE_COLOR_1			0
#define MESSAGE_COLOR_1_LINE	1
#define	MESSAGE_COLOR_2			2
#define MESSAGE_COLOR_2_LINE	3
#define	MESSAGE_COLOR_3			4
#define MESSAGE_COLOR_3_LINE	5
#define	MESSAGE_COLOR_HL_1		6
#define MESSAGE_COLOR_HL_1_LINE	7 + COLOR_ARRAY_HIGHLIGHT_MAX
//#define	MESSAGE_COLOR_HL_2		8
//#define MESSAGE_COLOR_HL_2_LINE	9
//#define	MESSAGE_COLOR_HL_3		10
//#define MESSAGE_COLOR_HL_3_LINE	11

#define MAX_MESSAGE_PROJECT_FILTER 100

#define MESSAGE_INIT_TEXT		"\003_"

class CProjectFilterItem
{
public:
	CString m_sProject;
	bool	m_bSelected;
};


class CThreadRpc;
class CRpcInfoMessageItem
{
public:
	int		m_iNrMessages;
	int		m_iLastReadMessages;
	int		m_iThreadId;
	char	*m_pcComputerId;
	CThreadRpc *m_pThread;
	int		m_iStatusRpc;
	int		m_iInit;
	int		m_iSortIndex;
};

// CRemoteCombinedMessageView view
class CRpcThreadInfo;
class CThreadRpc;
class CRemoteCombinedDoc;
class CTemplateRemoteMessageView : public CListViewEx
{
	friend class CRemoteCombinedDoc;
	DECLARE_DYNCREATE(CTemplateRemoteMessageView)

public:
//protected:	
	CTemplateRemoteMessageView();				// protected constructor used by dynamic creation
	virtual ~CTemplateRemoteMessageView();
	virtual void OnInitialUpdate();
	void	ClipBoard(bool bAll);
public:

	bool m_bMessagesCorrupt;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	bool IsFiltered(CString *psProject);
	bool CheckIfMessagesCorrupt(CListCtrl *pListCtrl);
	void SetSortIndicator(int iColumn, int iSorting);

	CListData m_data[30];	// warning numer is color index not row number

	bool	m_bAlwaysShow;
	bool	m_bDraged;
	bool	m_bForceRefreshAll;

	int		m_iColumnOrder[NUM_REMOTE_MESSAGE_COLUMNS+1]; 
	int		m_iColumnWidth[NUM_REMOTE_MESSAGE_COLUMNS+1];
	int		m_iNrColomns;

	bool	DisplayRpc(CThreadRpc *pThreadRpc, int iNrMessages, int iNrFiltered, int iLastReadMessages, char *pcComputerId);
	void	AddAllColumns(bool bAdd = true);
	void	ReadAllColumnWidth();
	int		ReadColumnWidth(int iColumn, int iDefault);
	void	CheckAndSaveColumnsWidth();

	void	CheckIfLastIsShown(void);
	void	StripingSkin(COLORREF *crColor);

// computer
	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;
	int		m_iMessagesTotal;

	std::deque<CRpcInfoMessageItem*>	m_lComputerRpcInfo;
// computer

//	CString	m_sProjectFilter;
//	CString	m_sProjectFilterList[MAX_MESSAGE_PROJECT_FILTER+1];


	std::deque<CProjectFilterItem *>	m_lProjectFilter;

	bool	m_bFilterAllProjects;
	bool	m_bFilterGeneral;
	bool	m_bFilterChanged;
	bool	m_bWebserverNeedsUpdate;
	bool	m_bCloudserverNeedsUpdate;

	int		m_iSortingPrimary;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags,  short zDelta, CPoint pt);
	afx_msg BOOL OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHeaderChanged(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) ;
};



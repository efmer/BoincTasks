#pragma once
#include <deque>
#include "ThreadRpc.h"
#include "ThreadRpcClient.h"

//#define VALID_MAC_COLOR RGB(233,255,233)

//#define MAX_COMPUTERS_IN_LIST	1000

// for every computer
class CRpcInfoComputerItem
{
public:
	char			*m_pcComputerId;
	CThreadRpc		*m_pThread;
	int				m_iStatusRpc;
	int				m_iSortIndex;
	CString			m_sBoincVersion;
	CString			m_sBoincPlatform;
	CString			m_sTThrottleVersion;
	TIME_STATS		m_timeStats;
};

class CListComputerFindItem;
class CThreadRpc;
class CDlgSearchComputers;
class CSortColumn;
class CSelection;
class CTemplateRemoteComputerView : public CListViewEx
{
	DECLARE_DYNCREATE(CTemplateRemoteComputerView)

public:
	CTemplateRemoteComputerView();           // protected constructor used by dynamic creation
	virtual ~CTemplateRemoteComputerView();
	virtual void OnInitialUpdate();				// called first time after construct
			void DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin);
			void StartSorting()	;
			void DeleteComputerFind();
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	void CheckItem(int nNewCheckedItem);

	std::deque<CListData*> m_lListData;
	CListData m_dataDummy;
	std::deque<CListComputerFindItem*> m_lListComputerFind;

	CImageList m_StateImageList;
	BOOL m_bStateIcons;
	int m_nCheckedItem;
	bool	m_bChanged;

	CDlgSearchComputers *m_pDlgSearchComputer;

// computer
	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;

	std::deque<CRpcInfoComputerItem*>	m_lComputerRpcInfo;
	std::deque<CRpcInfoClient*>			m_lComputerRpcInfoClient;
// computer
	
	CSortColumn	*m_pSortColumn;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnViewStateIcons();
	afx_msg void OnUpdateViewStateIcons(CCmdUI* pCmdUI);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



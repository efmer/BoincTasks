#pragma once
#include <deque>
#include "history.h"
#include "ThreadRpc.h"
#include "ThreadRpcClient.h"

// for every computer

class CRuleItem;
class CActiveRuleItem;
class COperationItem;

class CHistoryRpc;
class CRpcInfoHistoryItem
{
public:
	int		m_iSendRpcThreadId;
	int		m_iReadyRpcThreadId;
	int		m_iReadyRpcFoundCount;
	int		m_iReadyRpcConnectionStatus;
	char	*m_pcReadyRpcComputerId;
	bool	m_bReadyRpcSelected;

};

//class CComputerHistoryItems
//{
//public:
//	std::deque<CHistoryItem*>		*m_plHistory;				// only copies of the actual
//	std::deque<CHistoryCountItem*>	*m_plHistoryCount;			// only copies of the actual
//};

#define NUM_REMOTE_HISTORY_COLUMNS 12
class CThreadRpc;
class CComputerRemoteDoc;
class CSortColumn;
class CSelection;
class CTemplateRemoteHistoryView : public CListViewEx
{
	friend class CRemoteCombinedDoc;
	DECLARE_DYNCREATE(CTemplateRemoteHistoryView)

public:
	CTemplateRemoteHistoryView();           // protected constructor used by dynamic creation
	virtual ~CTemplateRemoteHistoryView();
	virtual void OnInitialUpdate();				// called first time after construct
	void	InitialUpdate(CComputerRemoteDoc* pDoc);
	void	AddAllColumns(bool bAdd = true);
	void	ReadAllColumnWidth();
	int		ReadColumnWidth(int iColumn, int iDefault);
	void	CheckColumnWidth();
	std::deque<CHistoryRpc*> *GetComputerList();

	int		m_iColumnOrder[NUM_REMOTE_HISTORY_COLUMNS+1]; 
	int		m_iColumnWidth[NUM_REMOTE_HISTORY_COLUMNS+1];		// for xchange only
	bool	m_bApplyChangesOnColumn;
	int		m_iNrColomns;

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

// computer
	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;

	std::deque<CRpcInfoHistoryItem*>	m_lComputerHistoryItem;
//	std::deque<CHistoryRpc*>			m_lComputerHistoryRpc;				// warning this list only gets the copy of the history pointers!
	std::deque<CRpcInfoClient*>			m_lComputerRpcInfoClient;
	std::deque<CHistoryRpc*>			m_lComputer;
// computer



	CListData			m_dataDummy;
	CListData			m_dataWarning;

	std::deque<CListData*> m_lListData;
	CSortColumn			*m_pSortColumn;
	CSelection			*m_pSelection;

	time_t				m_tNextHistoryWrite;
	time_t				m_tNextHistoryUpdateTime;
	time_t				m_LastSwitchedTime;
	bool				m_bHistoryForcedUpdate;

	int					m_iSortingChanged;

	bool				m_bAddedFromFile;

	void				DisplayRpc(CListCtrl *pListCtrl, std::deque<CHistoryItem*> *plHistory, int *piRow);
	void				DisplaySingleItem(CListCtrl *pListCtrl, CHistoryItem *pHistoryItem, int *piRow);
	void				DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin);

	void				ClipBoard(bool bAll);

	void				TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore, char *pAfter);

	bool				m_bDraged;

	std::string			m_sProjectsSelected;
	bool				m_bAllProjectsSelected;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg BOOL OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHeaderChanged(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSettingsApplyChangesHistory(WPARAM parm1, LPARAM parm2);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:

	std::deque<CRuleItem*>				m_lRules;
	std::deque<CActiveRuleItem*>		m_lRulesActive;
	std::deque<COperationItem*>			m_lOperationsQue;

};



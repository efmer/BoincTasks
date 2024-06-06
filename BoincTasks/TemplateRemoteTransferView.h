#pragma once

#include "ThreadRpc.h"
//#include "computerDoc.h"

#define NUM_REMOTE_TRANSFER_COLUMNS 8

#define TRANSFER_COLOR_NORMAL		0
#define TRANSFER_COLOR_NORMAL_LINE	1

class CRpcInfoTransferItem
{
public:
	int		m_iNrTransfer;
	int		m_iThreadId;
	char	*m_pcComputerId;
	CThreadRpc *m_pThread;
	int		m_iStatusRpc;
	int		m_iSortIndex;
};

// CTemplateRemoteTransferView view

class CThreadRpc;
class CRemoteCombinedDoc;
class CSortColumn;
class CSelection;
class CTemplateRemoteTransferView : public CListViewEx
{
	friend class CRemoteCombinedDoc;
	DECLARE_DYNCREATE(CTemplateRemoteTransferView)

public:
	CTemplateRemoteTransferView();           // protected constructor used by dynamic creation
	virtual ~CTemplateRemoteTransferView();
	virtual void OnInitialUpdate();				// called first time after construct
			void DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin);
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	std::deque<CListData*> m_lListData;
//	CListData *m_pData[MAX_TRANSFER_DATA+1];
	CListData m_dataDummy;

	int		m_iColumnOrder[NUM_REMOTE_TRANSFER_COLUMNS+1]; 
	int		m_iColumnWidth[NUM_REMOTE_TRANSFER_COLUMNS+1];
	int		m_iNrColomns;

	void	AddAllColumns(bool bAdd = true);
	void	ReadAllColumnWidth();
	int		ReadColumnWidth(int iColumn, int iDefault);
	void	CheckAndSaveColumnsWidth();
//
	void	DisplayRpc(CThreadRpc *pThreadRpc, int iNrTransfer, int *piRow, char *pcComputerId, int iComputerId);

// computer
	int		m_iSendRpcCount;
	int		m_iReadyRpcCount;
	int		m_iTransferTotal;
	std::deque<CRpcInfoTransferItem*> m_lRpcInfo;
// computer

	CSortColumn	*m_pSortColumn;

	CSelection *m_pSelection;

	bool	m_bDraged;

	int		m_iWaitingTurner;

//	std::string m_sProjectsSelected;
//	bool	m_bAllProjectsSelected;

	DECLARE_MESSAGE_MAP()


	afx_msg void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg BOOL OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHeaderChanged(UINT, NMHDR* pNMHDR, LRESULT* pResult);

	void				ClipBoard(bool bAll);

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};



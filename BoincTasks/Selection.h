#pragma once
#include <deque>

class CSelectionItem
{
public:
	CSelectionItem();
	virtual ~CSelectionItem();

	void	AddComputer(char *pcComputer);
	void	AddTask(char *pcTask);
	void	Add(char **pcDest, char *pcSource);

	char	*m_pcComputer;
	char	*m_pcTask;
};

//#define MAX_SELECTED			200

class CSelection
{
public:
	CSelection(void);
	~CSelection(void);
	void ClearSelection();
	void Setup(CListCtrl *pListCtrl, std::deque<CListData*> *plData, int iColumnFollowTask, int iColumnFollowComputer, int iCount, int *piOrder);
	int	 SelectionNumber();
	void SelectionSetRight(int iItemSelected);
	void SelectionSet(int iItemSelected, int iOption=0);
	bool SelectionAdd(int iItemSelected, char *pcSelectedTask, char *pcSelectedComputer);
	bool SelectionRemoveAll();
//	void ClearList();
	void SelectionLastRemove();
	void SelectionLast(char *pcSelectedTask, char *pcSelectedComputer);
	int  SelectionGetLast(CListCtrl *pListCtrl);
	void SelectionSelectFromTo(CListCtrl *pListCtrl,int iFirst, int iLast);
	bool SelectionSelected(CListCtrl *pListCtrl, int iItem);
	void SelectionDelete(CListCtrl *pListCtrl, int iItemSelected);

	bool Follow(char *pcName, char *pcComputerName);
	void SelectionMoveDown(CListCtrl *pListCtrl, int iTaskColumn, int iComputerColumn, int iNrLines);
	void SelectionMoveUp(CListCtrl *pListCtrl, int iTaskColumn, int iComputerColumn, int iNrLines);
	
	void SelectionBegin(CListCtrl *pListCtrl);
	void SelectionEnd(CListCtrl *pListCtrl);

	CString GetLastSelected();


protected:

	CListCtrl	*m_pListCtrl;
	int			m_iColumnFollowTask;
	int			m_iColumnFollowComputer;
	int			m_iColumnFollow2;
	int			m_iColumnFollow3;

	int			m_iColumnOrder[20];

	std::deque<CSelectionItem*> m_lSelected;

//	CString		m_sSelectedTask[MAX_SELECTED+1];
//	CString		m_sSelectedComputer[MAX_SELECTED+1];

	CString		m_sSelectedLastTask;
	CString		m_sSelectedLastComputer;

//	int			m_iMaxData;
//	CListData	**m_pData;
	std::deque<CListData*> *m_plData;


};

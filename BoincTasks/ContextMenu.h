#pragma once

#define CONTEXT_MENU_PROJECTS	0
#define CONTEXT_MENU_TASKS		1
#define CONTEXT_MENU_TRANSFERS	2
#define CONTEXT_MENU_HISTORY	3

#define URL_PROJECTS_MAX 20

//#include "ThreadRpc.h"

// CContextMenu command target

class CSelection;
class CRemoteCombinedDoc;
class CContextMenu : public CObject
{
public:
	CContextMenu();
	virtual ~CContextMenu();
	void SetColumnOrder(int iOrderCount, int *pOrder);
	void ContextMenuCombined(int iWhatContectMenu, CRemoteCombinedDoc *pDoc, CListCtrl *pListCtrl, CSelection *pSelection, CPoint point, CWnd * pWnd, int iMenuId);
//	void ContextMenu(int iWhatContectMenu, CThreadRpc *pThread, CListCtrl *pListCtrl, CSelection *pSelection, CPoint point, CWnd * pWnd, int iMenuId);
	void Ready(int iStatus);
	void WWW(int iNr);
	int	FindMenuItem(CMenu *pMenu, LPCSTR MenuString);

protected:
	int			m_iColumnOrder[20];
	CPoint		m_startPoint;
	CWnd		*m_pWnd;

	CString		m_sComputer;
	CString		m_sProject;
	bool		m_bFilter;

	bool		m_bThreadLocked;

	char		*m_psUrl[URL_PROJECTS_MAX+1];
	char		*m_psMenu[URL_PROJECTS_MAX + 1];

	std::string	m_sForum;

	CMenu		m_menu;
	int			m_iMenuId;
	int			m_iWhatContectMenu;

//	CWaitCursor *m_pWaitCursor;

	CRpcProjectsDiskUsage m_projectsDiskUsage;
	PROJECTS	m_projects;

};


/*
class CContextMenuWww
{
public:
	CMenu	m_contextMenu;
	CWnd	*m_pWnd;
	CPoint	m_point;
	CString m_sComputer;
	CString m_sProject;
};
*/
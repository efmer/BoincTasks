#pragma once

//#include "workunit.h"

// CThreadRead

class CWorkUnit;
class CTasksDoc;
class CThreadTasksRead : public CWinThread
{
	DECLARE_DYNCREATE(CThreadTasksRead)

protected:
	CThreadTasksRead();           // protected constructor used by dynamic creation
	virtual ~CThreadTasksRead();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	int		Start(CTasksDoc *pDoc, CList <CWorkUnit*,CWorkUnit*> *pWu);

protected:
	bool		m_bStart;
	bool		m_bExit;
	CTasksDoc	*m_pDoc;

	CList <CWorkUnit*,CWorkUnit*> *m_pWu;

	DECLARE_MESSAGE_MAP()
};



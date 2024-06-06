#pragma once

class CThreadGarbageCollectorData
{
public:
	CString	m_sFolder;
	CString m_sFind;
	CString m_sLog;
	DWORD	m_dThreadId;
	int		m_iTimeToRemove;
	bool	m_bReadOnly;
};


// CThreadGarbageCollector

class CThreadGarbageCollector : public CWinThread
{
	DECLARE_DYNCREATE(CThreadGarbageCollector)

protected:
	CThreadGarbageCollector();           // protected constructor used by dynamic creation
	virtual ~CThreadGarbageCollector();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	void DeleteFiles(CThreadGarbageCollectorData *pThreadGarbageCollectorData);
	CString GetTimeString();

	DECLARE_MESSAGE_MAP()
};



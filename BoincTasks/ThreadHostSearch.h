#pragma once


class CHostSearchInfo
{
public:
	CHostSearchInfo(void);
	~CHostSearchInfo(void);
	void Clear();
	void AddHostName(char *pcHostName);
	void AddHostNameFull(char *pcHostNameFull);
	void Add(char **pcDest, char *pcSource);

	char *m_pcHostName;
	char *m_pcHostNameFull;
};

// CHostSearch

class CThreadHostSearch : public CWinThread
{
	DECLARE_DYNCREATE(CThreadHostSearch)

protected:
	CThreadHostSearch();           // protected constructor used by dynamic creation
	virtual ~CThreadHostSearch();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:

	bool	m_bExit;
	bool	m_bWaitForContinue;

	DECLARE_MESSAGE_MAP()
};



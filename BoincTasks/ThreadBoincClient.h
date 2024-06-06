#pragma once



// CThreadBoincClient

class CThreadBoincClient : public CWinThread
{
	DECLARE_DYNCREATE(CThreadBoincClient)

protected:
	CThreadBoincClient();           // protected constructor used by dynamic creation
	virtual ~CThreadBoincClient();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};



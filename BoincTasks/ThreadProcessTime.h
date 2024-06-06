#pragma once



// CThreadProcessTime

class CThreadProcessTime : public CWinThread
{
	DECLARE_DYNCREATE(CThreadProcessTime)

protected:
	CThreadProcessTime();           // protected constructor used by dynamic creation
	virtual ~CThreadProcessTime();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};



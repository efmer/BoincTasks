#pragma once

class CRpcInfoClientSearch;
class CRpcClient;

// CThreadRpcClientSearch

class CThreadRpcClientSearch : public CWinThread
{
	DECLARE_DYNCREATE(CThreadRpcClientSearch)

protected:
	CThreadRpcClientSearch();           // protected constructor used by dynamic creation
	virtual ~CThreadRpcClientSearch();

	bool		m_bExit;

	CRpcClient	*m_pRpcClient;
	int			m_iPort;

	CRpcInfoClientSearch *m_pInfo;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};



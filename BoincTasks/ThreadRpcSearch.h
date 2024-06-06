#pragma once


#include <vector>
#include <string>
#include "gui_rpc_client.h"
#include "error_numbers.h"
#include "util.h"
#include "str_util.h"
#include "version.h"
#include "common_defs.h"
#include "string.h"

#define RPC_STATUS_NONE					0
#define RPC_STATUS_ERROR_CANNOT_CONNECT	1
#define RPC_STATUS_ERROR_READ			2
#define RPC_STATUS_ERROR_AUTHORIZATION	3
#define RPC_STATUS_ERROR_READING_STATE	4
#define RPC_STATUS_ERROR_READING_HOSTID 5
#define RPC_STATUS_ERROR_LOST_CONNECTION 6
#define RPC_STATUS_CONNECTED			10

class CThreadRpcSearch : public CWinThread
{
	DECLARE_DYNCREATE(CThreadRpcSearch)

protected:
	CThreadRpcSearch();           // protected constructor used by dynamic creation
	virtual ~CThreadRpcSearch();
	int			GetError(int retval, int iFallback);

	bool		m_bExit;
	RPC_CLIENT	*m_pRpc;
	CC_STATE	*m_pState;
	HOST_INFO	*m_pHi;

	int			m_iConnectionStatus;
public:

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	bool		Connect();

	std::string m_sComputerId;
	std::string	m_sHostName;
	std::string	m_sPassword;
	int			m_iPort;


	DECLARE_MESSAGE_MAP()
};



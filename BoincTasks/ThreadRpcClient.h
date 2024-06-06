#pragma once

#define MAX_CPU 12
#define MAX_GPU 8

#define BIT_TRUE_BOINC		1
#define BIT_TRUE_TTHROTTLE	2
#define BIT_TRUE_TOOLBOX	4

#define STOP_TEMPERATURE	-1

#define THREAD_RPC_BUFFER	1024*32

class	CTemperatureOneCore
{
public:
		std::deque<float>	m_lTemperatureCore;
};

class	CTemperatureGraphItem
{
public:
		std::deque<CTemperatureOneCore *>	m_lTemperatureCores;
		time_t		m_tLastAddTime;
};

class	CTemperatureGraphBuffer
{
public:
		UINT		*m_piTemperatureBuffer;
		time_t		m_tLastAddTime;
};

class CThreadRpcClient;
class CRpcInfoClientSearch
{
public:
	CRpcInfoClientSearch();
	virtual ~CRpcInfoClientSearch();
	void	Clear();
//	void	AddIp(char *pcIp);
//	void	AddHostID(char *pcAddHostID);
//	void	AddPassWord(char *pcPassWord);
//	void	AddFoundHostName(char *pcFoundHostName);
//	void	Add(char **pcDest, char *pcSource);

	int			m_iThreadID;
	int			m_iPort;
	int			m_iConnectionStatus;
	std::string	m_sIp;
	std::string	m_sHostID;
	std::string	m_sPassWord;
	std::string	m_sFoundHostName;
	bool		m_bAutoExit;

	int			m_iTimeOut;
};

class CRpcInfoClient
{
public:
	bool	m_bReady;
	bool	m_bConnectionLogged;
	double	m_fCpuThrottle;
	double	m_fGpuThrottle;
	double	m_fTopCpuTemperature;
	double	m_fTopGpuTemperature;
	double	m_fGpuTemperature[MAX_GPU+1];

	int		m_iNrNVIDIA;
	int		m_iNrATI;

	bool	m_bReadyCopy;
	double	m_fCpuThrottleCopy;
	double	m_fGpuThrottleCopy;
	double	m_fTopCpuTemperatureCopy;
	double	m_fTopGpuTemperatureCopy;
	double	m_fGpuTemperatureCopy[MAX_GPU+1];

	CString m_sTxt;
	int		m_iSetCore;
	int		m_iSetGpu;
	int		m_iMinCpu;
	int		m_iMaxCpu;
	int		m_iActivive;

	std::string m_sComputerId;
	std::string	m_sHostName;
	std::string	m_sPort;
	int		m_iPort;
	double	m_fVersion;
	CString	m_sKey;

	int		m_iStatus;
	int		m_iLostCount;
};

// CThreadRpcClient

class CRpcInfoClientSearch;
class CThreadRpcSearch;
class CRpcInfoClient;
class CRpcClient;
class CThreadRpcClient : public CWinThread
{
	DECLARE_DYNCREATE(CThreadRpcClient)

protected:
	CThreadRpcClient();           // protected constructor used by dynamic creation
	virtual ~CThreadRpcClient();

	int		SendReceive(HWND *phWnd, CRpcInfoClient *pInfoClient, bool bSet);
	int		ReadMessage(char *pcBuffer, CRpcInfoClient *pInfoClient);
	int		ReadGraphic(BYTE *pcBuffer, UINT *piBufferReturn, int *piItemCount, char *pcError, time_t *ptLastAddTime);
	bool	FindNumber(char **pcBuffer, char *pcCompare, double *pfNumber);
	bool	FindNumber(char **pcBuffer, char *pcCompare, int *piNumber);
	bool	FindString(char **pcBuffer, char *pcCompare, CString *psTxt);
	bool	FindTemperature(BYTE **pcBuffer, UINT *piTemperature, int *piItems, int iVersionBufferLen);
//	void	SendLogging(CStringA *psFrom, CStringA *psMessage, int iDebug);
	void	SendLogging(char *pcFrom, char *pcMessage, int iDebug);

	char	*m_pcBuffer;

	CStringA	m_sKey;
	int		m_iBoincTThrottle;
	CRpcInfoClientSearch *m_pInfo;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	bool		m_bExit;

	std::string	m_sHostName;
	std::string m_sComputerId;
	std::string m_sPassword;
	std::string	m_sPort;

//	char		*m_pcPassword;
//	char		*m_pcComputerId;
//	char		*m_pcPort;
	int			m_iPort;

	float		m_fCpuTemperature[9];
	float		m_fGpuTemperature[5];

//	CRpcClientReturn	m_rpcClientReturn;
protected:

	CRpcClient			*m_pRpcClient;
	CThreadRpcSearch	*m_pSearchBoinc;

//	time_t				m_tLastAddTime;

	DECLARE_MESSAGE_MAP()
};



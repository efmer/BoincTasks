#pragma once

#define CLIENT_MANAGER_BOINC_SHUTTING_DOWN	"The BOINC client is shutting down"

// CBoincClientManager command target

class CLoggingMessage;
class CBoincClientManager : public CObject
{
public:
	CBoincClientManager();
	virtual ~CBoincClientManager();

	bool	StartupBoincCore(int iLocalHostThreadId);
	bool	Close(int iLocalHostThreadId, CLoggingMessage *psLog);
	bool	KillClient(DWORD *pExitCode);
//	int		IsBOINCConfiguredAsDaemon();
	bool	IsRunning();
	bool	CheckProcess(CString sProcess, bool bKill=false);
	bool	CheckProcess(DWORD dwPID);
};



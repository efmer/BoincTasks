#pragma once
class CTasksProperties
{
public:
	CTasksProperties(void);
	~CTasksProperties(void);

	void	Clear();
	void	Delete();

	void		Add(char **pcDest, char *pcSource);

	void		AddName(char *pcName);
	void		AddApplicationName(char *pcApplicationName);
	void		AddWuName(char *pcWuName);
	void		AddProjectUrl(char *pcProjectUrl);
	void		AddResources(char *pcResources);
	void		AddPlanClass(char *pcPlanClass);

	bool		m_bValid;

	CString		m_sComputer;
	CString		m_sProject;
//	CString		m_sName;
	int			m_iThreadId;

	char		*m_pcName;
	char		*m_pcApplicationName;
	char		*m_pcWuName;
	char		*m_pcProjectUrl;
	char		*m_pcResources;
	char		*m_pcPlanClass;

	int			m_iVersion;
	double		m_dDeadline;
	double		m_dRecieved;
	int			m_iState;
	int			m_iStateDebug;
	int			m_iSchedulerDebug;

	bool		m_bActive;
	int			m_iSlot;
	int			m_iPid;
	double		m_dCheckpointCpuTime;
	double		m_dCurrentCpuTime;
	double		m_dFractionDone;
	double		m_dElapsedTime;
	double		m_dSwapSize;
	double		m_dWorkingSetSize;
	double		m_dEstimateCpuTimeRemaining;

	double		m_dFlops;
	double		m_dRscFpopsEst;

	bool		m_bSuspendedViaGui;
	bool		m_bProjectSuspendedViaGui;
	int			m_iSuspendStatus;
	int			m_iSuspendReason;
};


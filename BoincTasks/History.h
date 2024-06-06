#pragma once
#include <deque>

#define HISTORY_FILENAME "history"
#define HISTORY_FILENAME_LONG "history_long"
#define HISTORY_ID_BEGIN "BoincTasks History"
#define HISTORY_ID_END	 "Check!"

#define HISTORY_FOLDER			"\\history\\"
#define HISTORY_FOLDER_BACKUP	"backup\\"

#define HISTORY_STATUS_OK				1
#define HISTORY_STATUS_ERROR			2
#define HISTORY_STATUS_FILE_NOT_FOUND	3


#define HISTORY_BACKUP_TIME	3600

class CHistoryRunItem
{
public:
	int		m_iPrev1ElapsedTimeCpu;
	int		m_iPrev1ElapsedTimeGpu;
	int		m_iPrev2ElapsedTimeCpu;
	int		m_iPrev2ElapsedTimeGpu;
	int		m_iPrev3ElapsedTimeCpu;
	int		m_iPrev3ElapsedTimeGpu;
	int		m_iPrev4ElapsedTimeCpu;
	int		m_iPrev4ElapsedTimeGpu;
	double	m_dPrev1Fraction_done;
	double	m_dPrev2Fraction_done;
	double	m_dPrev3Fraction_done;
	double	m_dPrev4Fraction_done;
};

class CHistoryItem
{
public:
	CHistoryItem();
	virtual ~CHistoryItem();

	void	AddComputer(char *pcComputer);
	void	AddProject(char *pcProject);
	void	AddApplication(char *pcApplication);
	void	AddName(char *pcName);
	void	AddPlanClass(char *pcPlanClass);
	void	AddUse(char *pcUse);
	void	Add(char **pcDest, char *pcSource);
	void	CopyItem(CHistoryItem *pHistoryItemFrom);

	char	*m_pcComputer;
	char	*m_pcProject;
	char	*m_pcApplication;
	char	*m_pcName;
	char	*m_pcPlanClass;
	char	*m_pcUse;

	int		m_iVersion;
	int		m_iElapsedTimeCpu;
	int		m_iElapsedTimeGpu;
	int		m_iDeadline;
	int		m_iTimeLeft;
	int		m_iState;
	__int64	m_i64ReportedTime;
	__int64 m_i64CompletedTime;
	int		m_iExitStatus;
	bool	m_bWuGone;
	double	m_dFraction_done;

	int		m_iCheckpoint;	// no double for compares
	int		m_iCheckpoints;

	double	m_dReceivedTime;

	double	m_dVirtualMemory;
	double	m_dMemory;

	CHistoryRunItem	*m_pHistoryRunItem;
};

class CHistoryCountItem
{
public:
	CHistoryCountItem();
	virtual ~CHistoryCountItem();

	void	AddComputer(char *pcComputer);
	void	AddProject(char *pcProject);
	void	Add(char **pcDest, char *pcSource);

	char	*m_pcComputer;
	char	*m_pcProject;
	int		m_iTaskCountCpu;
	int		m_iTaskCountGpu;
	int		m_iTaskCountLow;
	__int64  m_iTimeLeftCpu;
	__int64  m_iTimeLeftGpu;
	int		m_iCoresCpu;
	int		m_iCoresGpu;
	int		m_iReadyCount;
};

// 60*24
#define MAX_HISTORY_STATISTICS_LEN	(1440)
#define MAX_DEADLINE_LEN			(1440)
#define HOUR_SECONDS				(3600)

class CHistoryProjectTasksItem
{
public:
	CHistoryProjectTasksItem();
	virtual ~CHistoryProjectTasksItem();

	void	AddProject(char *pcProject);
	void	Add(char **pcDest, char *pcSource);

	char	*m_pcProject;

	time_t	m_tTimeBaseline;
	int		m_iReceivedCpu[MAX_HISTORY_STATISTICS_LEN+2];	// warning +1 is used!
	int		m_iReceivedGpu[MAX_HISTORY_STATISTICS_LEN+2];

	bool	m_bDeleted;
};

class CHistoryProjectDeadlineItem
{
public:
	CHistoryProjectDeadlineItem();
	virtual ~CHistoryProjectDeadlineItem();

	void	AddProject(char *pcProject);
	void	Add(char **pcDest, char *pcSource);

	char	*m_pcProject;

	time_t	m_tTimeBaseline;
	double	m_dReceivedCpu[MAX_DEADLINE_LEN+2];		// warning +1 is used!
	double	m_dReceivedGpu[MAX_DEADLINE_LEN+2];

	int		m_iCpuItems;
	int		m_iGpuItems;

	bool	m_bDeleted;

	int		m_iCpu, m_iGpu;
};

class CHistoryRpc
{
public:
	std::deque<CHistoryItem*> m_lHistory;
	std::deque<CHistoryItem*> m_lHistoryLong;
	bool	m_bHistoryLongNeedsWrite;
	std::deque<CHistoryCountItem*> m_lHistoryCount;
	UINT	m_iDeleteTime;
	bool	m_bSmartMode;
	bool	m_bHistoryLessAccurate;
	UINT	m_iUpdateMaxTime;
	bool	m_bHistoryForceUpdate;

	bool	m_bMoveToLongHistory;
	int		m_iMovetoLongHistoryTime;

	bool	m_bAdjustTime;

	std::string m_sComputerId;
};

// CHistory command target

class CHistoryItem;
class CHistory : public CObject
{
public:
	CHistory();
	virtual ~CHistory();

	void	CleanupLong(std::deque<CHistoryItem*> *pHistory);
	bool	Write(std::deque<CHistoryItem*> *plHistory, char *pcComputerName, bool bLong = false);
	bool	Read(char  *pcComputerName, std::deque<CHistoryItem*> *plHistory, bool bLong = false);

private:
	void	AddString(CString *psHistory, CString *psAdd, bool bFirst = false);
	void	AddString(CString *psHistory, char *pcAdd, bool bFirst = false);
	bool	WriteFile(CString *pFileName, CString *psFileText, char *pcComputerName);

//	bool	ReadFileAndCHeck(char *pcComputerName, CString *pFileName,  CString *psFileText, CList <CHistoryItem*,CHistoryItem*> *plHistory );
	int		ReadFileAndCHeck(char *pcComputerName, CString *pFileName,  CString *psFileText, std::deque<CHistoryItem*> *plHistory );
//	bool	ProcessHistory(char *pcComputerName, CString *psFileText, CList <CHistoryItem*,CHistoryItem*> *plHistory);
	int		ProcessHistory(char *pcComputerName, CString *psFileText, std::deque<CHistoryItem*> *plHistory);
	bool	ProcessHistoryGetOneLine(int *piPos, CString *psFileText, CString *psLineText, char cStop);
//	bool	AddHistoryItem(int iPos, char *psComputerName, CString *psLineText, CList <CHistoryItem*,CHistoryItem*> *plHistory, bool *pbEnd );
	bool	AddHistoryItem(int iPos, char *psComputerName, CString *psLineText, std::deque<CHistoryItem*> *plHistory, bool *pbEnd );
	CFile	m_file;

	CWnd	*m_pWndLogging;

};



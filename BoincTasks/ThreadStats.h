#pragma once

#define STATS_FOLDER			"\\stats\\"
#define STATS_FILE				"cpid_"

#define	STATS_ERROR				0
#define	STATS_OK_WEB			1
#define STATS_OK_FILE			2

// CThreadStats

class CThreadStats : public CWinThread
{
	DECLARE_DYNCREATE(CThreadStats)

protected:
	CThreadStats();           // protected constructor used by dynamic creation
	virtual ~CThreadStats();
	bool NeedToUpdate(CString *psCPID);
	bool ReadStatsWeb(CString *psCPID, CString *psStats);
	bool ReadStatsFile(CString *psCPID, CString *psStats);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};



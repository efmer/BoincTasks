#pragma once

// CWorkUnit command target

#define WU_STATUS_UNKNOWN					0
#define WU_STATUS_PROGRESS					1
#define WU_STATUS_COMPLETED_VALID			2
#define WU_STATUS_COMPLETED_WAITING			3
#define WU_STATUS_COMPLETED_INCONCLUSIVE	4
#define WU_STATUS_ERROR						5
#define WU_STATUS_ERROR_ABORTED				6
#define WU_STATUS_ERROR_DETACHED			7
#define WU_STATUS_ERROR_TIMED_OUT			8

class CWorkUnit : public CObject
{
public:
	CWorkUnit();
//	DECLARE_DYNCREATE(CWorkUnit)
	virtual ~CWorkUnit();

public:

	int		m_iWorkUnit;
	int		m_iTaskId;
	CString m_sTimeSend;
	CString m_sTimeReported;

	float	m_fRunTime;
	float	m_fClaimedCredit;
	float	m_fGrantedCredit;
	
	int		m_iStatus;

};



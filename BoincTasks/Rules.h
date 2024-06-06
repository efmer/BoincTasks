#pragma once

#define	RULE_ITEMS					3

#define RULES_TYPE_NONE				0
#define RULES_TYPE_ELAPSED			1
#define RULES_TYPE_CPUP				2
#define RULES_TYPE_PROGRESS			3
#define RULES_TYPE_PROGRESS_DELTA	4
#define RULES_TYPE_TIMELEFT			5
#define RULES_TYPE_CONNECTION		6
#define RULES_TYPE_USE				7
#define RULES_TYPE_TEMPERATURE		8
#define RULES_TYPE_STATUS			9
#define RULES_TYPE_TIME_INTERVAL	10
#define RULES_TYPE_DEADLINE			11
#define RULES_TYPE_TIMELEFT_PROJECT	12

#define	RULE_TYPE_CLASS_UNKNOWN			0
#define	RULE_TYPE_CLASS_TIME_DAY		1
#define RULE_TYPE_CLASS_INTERVAL		2
#define	RULE_TYPE_CLASS_PERCENTAGE		3
#define	RULE_TYPE_CLASS_USE				4
#define	RULE_TYPE_CLASS_TEMPERATURE		5
#define RULE_TYPE_CLASS_STATUS			6
#define RULE_TYPE_CLASS_CONNECTION		7

#define RULE_OPERATOR_NONE				0
#define RULE_OPERATOR_IS				1
#define RULE_OPERATOR_MORE				2
#define RULE_OPERATOR_LESS				3
#define RULE_OPERATOR_NOTEQUAL			4
#define RULE_OPERATOR_NOLONGEREQUAL		5

#define EVENT_NONE						0
#define EVENT_TYPE_SUSPEND_PROJECT		1
#define EVENT_TYPE_SUSPEND_TASK			2
#define EVENT_TYPE_PROGRAM				3
#define EVENT_TYPE_RESUME_PROJECT		4
#define EVENT_TYPE_NO_NEW_WORK			5
#define EVENT_TYPE_ALLOW_NEW_WORK		6
#define EVENT_TYPE_SNOOZE				7
#define EVENT_TYPE_SNOOZE_GPU			8
#define EVENT_TYPE_CANCEL_SNOOZE		9
#define EVENT_TYPE_CANCEL_SNOOZE_GPU	10
#define EVENT_TYPE_SUSPEND_NETWORK		11
#define EVENT_TYPE_RESUME_NETWORK		12


#define EVENT_CLASS_UNKNOWN				0
#define EVENT_CLASS_INTERNAL			1
#define EVENT_CLASS_EXTERNAL			2

#define EVENT_SHOW_NONE					0
#define EVENT_SHOW_LOGGING				1
#define EVENT_SHOW_NOTICE				2


#define INTERVAL_INSIDE					0
#define INTERVAL_OUTSIDE				1

class CRuleIntervalItem
{
public:
	int m_iStartTime;
	int m_iStopTime;
	int	m_bInvers;
};

class CRuleItem
{
public:
	CString m_sName;
	CString m_sComputer;
	CString m_sProject;
	CString m_sApplication;
	bool	m_bApplicationWc;

	int		m_iType[RULE_ITEMS];
	int		m_iOperator[RULE_ITEMS];

	_int64	m_iSnooze;

	_int64	m_iValue[RULE_ITEMS];
	double	m_dValue[RULE_ITEMS];
	std::deque<CRuleIntervalItem *> *m_plInterval;

	_int64	m_iTime;

	int		m_iEventTypeInternal;
	int		m_iEventTypeExternal;
	int		m_iEventShow;

	CString	m_sEvent;

	COLORREF m_color;

	int		m_iEnable;
	_int64	m_iBackoffUntill;		
};

class CActiveRuleItem
{
public:
	bool	m_bActive;
	bool	m_bEventTriggered;

//	bool	m_bFailed;
//	int		m_iBackoff;

	CString m_sRuleName;

	CString m_sComputer;
	CString m_sProject;
	CString m_sApplication;
	CString m_sTaskName;

	CString m_sRuleText;

	_int64	m_iElapsedTime;
	double	m_fProgress;
	int		m_iTime;

	_int64	m_iSnooze;

	_int64	m_iTriggerTime;
	COLORREF m_color;

	int		m_iEventTypeInternal;
	int		m_iEventTypeExternal;
	int		m_iEventShow;

	CString	m_sEvent;

};

// CRules command target

class CRules : public CObject
{
public:
	CRules();
	virtual ~CRules();


	int Type(CString *psType);
	void Type(int iType, CString *psType);
	int TypeClass(int iType);

	int EventType(CString *psEvent);
	void EventType(int iType, CString *psType);
	int EventClass(int iType);

	void Operator(int iType, int iOperator, CString *psOperator);

	void Value(int iTypeClass, _int64 iValue, double dValue, CString *psTxt);
	void TimeString(__int64 iTime, CString *pTime);
	void TimeExtract(__int64 iTime, int *piDay, int *piHour, int *piMinute, int *piSecond);
	
	void EventExtract(CString *psEvent, CString *psProgram, CString *psParameters);
	void EventProgramFullPath(CString *psProgram, CString *psFullPath);
	bool EventProgramCheckValid(CString *psProgramFull);
	bool Event(int iEvent, CString *psEvent, CString *psResult);
	bool InInterval(int iDay, int iHour, int iMinute, CRuleIntervalItem *pRuleIntervalItem);

	void IntervalString(std::deque<CRuleIntervalItem *> *plInterval, CString *psText);
	void IntervalStringSingle(CRuleIntervalItem *pRuleIntervalItem, CString *psText);

	void DeleteInterval(std::deque<CRuleIntervalItem *> **plInterval);
	void ShowRule(	CRuleItem *pRuleItem, CString *psRule);

};



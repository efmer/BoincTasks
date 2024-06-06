#pragma once

struct CC_STATUS;
class COperationItem
{
public:
	COperationItem();
	virtual ~COperationItem();

	void	AddDescription(char *pcDescription);
	void	AddComputer(char *pcComputer);
	void	AddProject(char *pcProject);
	void	AddTask(char *pcTask);

	void	Add(char **pcDest, char *pcSource);


	char	*m_pcDescription;
	char	*m_pcComputer;
	char	*m_pcProject;
	char	*m_pcTask;

	int		m_iTypeOperation;
	int		m_iOperation;
	int		m_iTime;

	int		m_iEventTypeInternal;

	CC_STATUS	*pC_STATUS;

	BOOL	m_bResult;
//	std::string m_sLog;
};


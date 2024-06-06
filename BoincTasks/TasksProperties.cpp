#include "StdAfx.h"
#include "TasksProperties.h"


CTasksProperties::CTasksProperties(void)
{
	Clear();
}


CTasksProperties::~CTasksProperties(void)
{
	if (m_pcName != NULL)			delete m_pcName;
	if (m_pcApplicationName != NULL)delete m_pcApplicationName;
	if (m_pcWuName != NULL)			delete m_pcWuName;
	if (m_pcProjectUrl != NULL)		delete m_pcProjectUrl;
	if (m_pcResources != NULL)		delete m_pcResources;
	if (m_pcPlanClass != NULL)		delete m_pcPlanClass;
}

void CTasksProperties::Clear()
{
	m_pcName = NULL;
	m_pcApplicationName = NULL;
	m_pcWuName = NULL;
	m_pcProjectUrl = NULL;
	m_pcResources = NULL;
	m_pcPlanClass = NULL;
}

void CTasksProperties::Delete()
{
	if (m_pcName != NULL)			delete m_pcName;
	if (m_pcApplicationName != NULL)delete m_pcApplicationName;
	if (m_pcWuName != NULL)			delete m_pcWuName;
	if (m_pcProjectUrl != NULL)		delete m_pcProjectUrl;
	if (m_pcResources != NULL)		delete m_pcResources;
	if (m_pcPlanClass != NULL)		delete m_pcPlanClass;
	Clear();
}

void CTasksProperties::AddName(char *pcName)
{
	Add(&m_pcName, pcName);
}
void CTasksProperties::AddApplicationName(char *pcApplicationName)
{
	Add(&m_pcApplicationName, pcApplicationName);
}
void CTasksProperties::AddWuName(char *pcWuName)
{
	Add(&m_pcWuName, pcWuName);
}
void CTasksProperties::AddProjectUrl(char *pcProjectUrl)
{
	Add(&m_pcProjectUrl, pcProjectUrl);
}
void CTasksProperties::AddResources(char *pcResources)
{
	Add(&m_pcResources, pcResources);
}
void CTasksProperties::AddPlanClass(char *pcPlanClass)
{
	Add(&m_pcPlanClass, pcPlanClass);
}


void CTasksProperties::Add(char **pcDest, char *pcSource)
{
	int iLen;

	if (*pcDest != NULL)
	{
		delete *pcDest;
		*pcDest = NULL;
	}

	if (pcSource == NULL) return;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
}
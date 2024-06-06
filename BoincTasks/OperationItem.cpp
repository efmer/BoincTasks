#include "StdAfx.h"
#include "OperationItem.h"


COperationItem::COperationItem()
{
	m_pcDescription = NULL;
	m_pcComputer = NULL;
	m_pcProject = NULL;
	m_pcTask = NULL;
}


COperationItem::~COperationItem()
{
	if (m_pcDescription != NULL)	delete m_pcDescription;
	if (m_pcComputer != NULL)		delete m_pcComputer;
	if (m_pcProject != NULL)		delete m_pcProject;
	if (m_pcTask != NULL)			delete m_pcTask;
}

void COperationItem::AddDescription(char *pcDescription)
{
	Add(&m_pcDescription, pcDescription);
}

void COperationItem::AddComputer(char *pcComputer)
{
	Add(&m_pcComputer, pcComputer);
}

void COperationItem::AddProject(char *pcProject)
{
	Add(&m_pcProject, pcProject);
}

void COperationItem::AddTask(char *pcTask)
{
	Add(&m_pcTask, pcTask);
}


void COperationItem::Add(char **pcDest, char *pcSource)
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
// CreateHtmlWebServerProperties.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "AnsiToUtf8.h"
#include "CreateHtmlWebServerProperties.h"


// CCreateHtmlWebServerProperties

CCreateHtmlWebServerProperties::CCreateHtmlWebServerProperties()
{
}

CCreateHtmlWebServerProperties::~CCreateHtmlWebServerProperties()
{
}


// CCreateHtmlWebServerProperties member functions

void CCreateHtmlWebServerProperties::Add(CString *psColumn1, CString *psColumn2)
{
	CAnsiToUtf8			AnsiToUtf8;
	m_sTxt += "<tr><td>";
	m_sTxt += AnsiToUtf8.ToUtf8(psColumn1);
	m_sTxt += "</td>";
	m_sTxt += "<td>";
	m_sTxt += AnsiToUtf8.ToUtf8(psColumn2);
	m_sTxt += "</td></tr>";
}

void CCreateHtmlWebServerProperties::Finish()
{
	char *pcNew;

	if (g_pcWebServerProperties) delete g_pcWebServerProperties;
	g_pcWebServerProperties = NULL;

	int iLen = m_sTxt.GetLength()+1;
	pcNew = new char [iLen];
	strcpy_s(pcNew, iLen, m_sTxt.GetBuffer());
	g_pcWebServerProperties = pcNew;
}
	

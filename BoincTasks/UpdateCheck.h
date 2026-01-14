#pragma once

// CUpdateCheck command target

class CUpdateVersion
{
public:
	int		m_iVersion;
	CString	m_sVersionHttp;

	int		m_iVersionBeta;
	CString	m_sVersionBetaHttp;

	int		m_iCurrentVersion;
	CString m_sCurrentVersion;


	CString	m_sVersionExe;

	CString	m_sVersionBetaExe;

};



class CUpdateCheck : public CObject
{
public:
	CUpdateCheck();
	virtual ~CUpdateCheck();

	bool GetLatestVersion(char * pcServer, char *pcHeaderType, char *pcLocation, char *pcFile, CUpdateVersion *pUpdateVersion);

//	CString m_versionHttp;
//	CString m_versionExe;
//	CString m_versionBetaHttp;
//	CString m_versionBetaExe;
};



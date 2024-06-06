#pragma once

// CUpdateCheck command target

class CUpdateVersion
{
public:
	double m_dVersion;
	double m_dVersionBeta;
	double m_dCurrentVersion;

	CString		m_sVersionHttp;
	CString		m_sVersionExe;
	CString		m_sVersionBetaHttp;
	CString		m_sVersionBetaExe;

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



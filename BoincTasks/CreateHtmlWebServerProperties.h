#pragma once

// CCreateHtmlWebServerProperties command target

extern char	*g_pcWebServerProperties;

class CCreateHtmlWebServerProperties : public CObject
{
public:
	CCreateHtmlWebServerProperties();
	virtual ~CCreateHtmlWebServerProperties();

	void Add(CString *psColumn1, CString *psColumn2);
	void Finish();

private:

	CString m_sTxt;
};



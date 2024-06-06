#pragma once

#define		MAX_NUM_LCID	10

// CMultiLanguage command target

class CMultiLanguage : public CObject
{
public:
	CMultiLanguage();
	virtual		~CMultiLanguage();

	CString		FindLangFile(LPCTSTR szModuleName, LANGID langUpdateId, LCID *pLcid, CString *psLog);
	CString		GetLanguageFile(LPCTSTR szModuleName, LPCTSTR szLangCode, CString *psLog);
	bool		GetLoadedLibrary(CString *psTxt);


	LCID		AddLangId( LANGID langidIn );
	LCID		AddLocale( LCID lcidIn );

	LANGID		DetectLangID(LANGID *plIdUser, LANGID *plIdSystem);


	int			m_iLocales;
	LCID		m_alcidSearch[MAX_NUM_LCID];

	CString		m_sLanguageId;

};



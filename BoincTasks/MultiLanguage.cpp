// MultiLanguage.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "MultiLanguage.h"


// CMultiLanguage

CMultiLanguage::CMultiLanguage()
{
	m_iLocales = 0;
}

CMultiLanguage::~CMultiLanguage()
{

}


// CMultiLanguage member functions

LCID	CMultiLanguage::AddLangId( LANGID langidIn )
{
	LCID lcidIn = MAKELCID(langidIn, SORT_DEFAULT);
	return AddLocale( lcidIn );
}

LCID	CMultiLanguage::AddLocale( LCID lcidIn )
{
	for ( int iLocale = 0; iLocale < m_iLocales; iLocale++ )
	{
		if ( m_alcidSearch[iLocale] == lcidIn )
		{	// Do not store duplicated lcid.
			return lcidIn;
		}
	}
	if ( m_iLocales < MAX_NUM_LCID )
	{
		LANGID langid = LANGIDFROMLCID(lcidIn);
		int nPrimaryLang = PRIMARYLANGID(langid);
		//int nSubLang = SUBLANGID(langid);
		LCID lcid = lcidIn;
		m_alcidSearch[m_iLocales] = ::ConvertDefaultLocale(lcid);
		m_iLocales++;

		if ( m_iLocales < MAX_NUM_LCID )
		{
			lcid = MAKELCID(MAKELANGID(nPrimaryLang, SUBLANG_NEUTRAL), SORT_DEFAULT);
			m_alcidSearch[m_iLocales] = ::ConvertDefaultLocale(lcid);
			m_iLocales++;
		} else {
			// Failed. Too may locale added
			return MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT);
		}
		// Success return
		return MAKELCID(langid, SORT_DEFAULT);
	} else {
		// Failed. Too may locale added
		return MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT);
	}
}

LANGID CMultiLanguage::DetectLangID(LANGID *plIdUser, LANGID *plIdSystem)
{
	LANGID langid = 0;
	int nPrimaryLang = 0;
	int nSubLang = 0;
	LCID lcid = 0;
	int nLocales = 0;

	langid = GetUserDefaultLangID();				// WinNT3.1/95 and later
	if (langid == 0) langid = 1033;					// nothing found = english
	AddLangId( langid );
	*plIdUser = langid;

	LANGID langSysid = GetSystemDefaultLangID();	// WinNT3.1/95 and later
	if (langid == 0) langSysid = 1033;				// nothing found = english
	AddLangId( langSysid );
	*plIdSystem = langSysid;

	return langid;
}

CString CMultiLanguage::FindLangFile(LPCTSTR szModuleName, LANGID langId, LCID *pLcid, CString *psLog)
{

	TCHAR		szLangCode[4+1];
//	LCID		lcid;
	int			iLocale;
	CString		sFile, sPath;
//	int			iFoundPos;

	if (langId != 0)
	{
		m_iLocales = 1;
		m_alcidSearch[0] = langId;
	}
	for ( iLocale = 0; iLocale < m_iLocales; iLocale++ )
	{
		*pLcid = m_alcidSearch[iLocale];
		int nResult = ::GetLocaleInfo(*pLcid, LOCALE_SABBREVLANGNAME, szLangCode, 4);
		if ( nResult == 0 )
		{
			*psLog += "FindLangFile, fallback on ENU\r\n";
			strcpy_s(szLangCode,4,"ENU");
		}
		sFile = GetLanguageFile(szModuleName, szLangCode, psLog);
		if (sFile != "") return sFile;
	}
	return "";
}

CString CMultiLanguage::GetLanguageFile(LPCTSTR szModuleName, LPCTSTR szLangCode, CString *psLog)
{
	CFile		file;
	CString		sFile, sPath;
	int			iFoundPos;
	int			iLength;

	char		strExePath[MAX_PATH];

	sFile = szModuleName;
	sFile += "_";
	sFile += szLangCode;
	sFile += ".btlang";

	iLength = GetModuleFileName (NULL, strExePath, MAX_PATH);
	strExePath[iLength] = 0;										// XP doesn't terminate it with a 0.
	sPath = strExePath;
	iFoundPos = sPath.ReverseFind('\\');

	if (iFoundPos > 0)
	{
		sPath = sPath.Mid(0,iFoundPos);
		sFile = sPath + "\\lang\\" + sFile;
		if (file.Open(sFile, CFile::modeRead))
		{
			file.Close();
			m_sLanguageId = szLangCode;
			return sFile;
		}
		Sleep(100);
		if (file.Open(sFile, CFile::modeRead))
		{
			file.Close();
			m_sLanguageId = szLangCode;
			return sFile;
		}
	}

	*psLog += "GetLanguageFile, unable to open: ";
	*psLog += sFile;
	return "";
}
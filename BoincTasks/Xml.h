#pragma once

#define XML_ERROR	0
#define XML_OK		1
#define XML_GROUP	2

#define MAX_ITEMS	2000

// CXml command target

class CXml : public CObject
{
public:
	CXml(CStringA sFileName, bool bUserDirectory = true, bool bMakeLower = true, bool bStripPercentage = true);
	CXml(bool bMakeLower,  bool bStripPercentage);
	virtual		~CXml();

	CStringA	GetDirectory(void);
	CStringA	GetUserDirectory(CString sPath, bool bLog = true);
	CStringA	GetLocalDirectory(void);

	bool		WriteStart(CStringA sAMainTag);
	void		WriteBeginGroup(CStringA sAGroup);
	void		WriteEndGroup(void);
	void		WriteItem(CStringA sATag, CStringA sAValue, int iTab=0);
	void		WriteItemBin(CStringA sATag,BYTE *pBuffer, int iSize);
	void		WriteEnd(void);

	bool		ReadStart(CStringA sGroup);
	bool		ReadStart(CStringA *psData, CStringA sMainTag);
	bool		ReadGroup(CStringA sAGroupTag, bool bFind = false, bool bScan = false);
	bool		ReadItem(CStringA sAId, CString *psAValue);
	bool		ReadItemBin(CStringA sAId, BYTE *pBuffer, int *piSize);
	bool		ConvertHexDec(BYTE cHigh, BYTE cLow, BYTE *pcValue);
	int			ReadCommandSequence(CStringA *psGroup, CStringA *psAItem, CStringA *psAValue);
	int			ReadCommand(CStringA *psCommand);

	CStringA	m_sLog;
protected:
	int			ReadFile(CStringA *pAXml);
	void		RemoveComment();

	bool		m_bUserDirectory;
	bool		m_bMakeLower;
	bool		m_bStripPercentage;

	bool		m_bLogLocalDirectoryOnce;
	CWnd		*m_pWndLogging;

	CFile		m_file;
	int			m_iPos;

	int			m_iItems;
	CStringA	m_sItem[MAX_ITEMS];
	CStringA	m_sItemValue[MAX_ITEMS];

	CStringA	m_sAFileName;

	CStringA	m_sAMainTag;
	CStringA	m_sAgroup;

	CStringA	m_sAXmlDataLower;
	CStringA	m_sAXmlData;
};



#pragma once

#include <deque>

#define PARSER_STATE_NONE		0
#define PARSER_STATE_BEGIN		1
#define PARSER_STATE_END		2
#define PARSER_INVALID_BEGIN	3
#define PARSER_VALID_TAG		4
#define PARSER_VALID_ENDTAG		5
#define PARSER_INVALID_TAG		6
#define PARSER_INVALID_CLOSE	7
#define PARSER_INVALID_CHAR		8
#define PARSER_INVALID_VALUE	9
#define PARSER_EOL				10

class CXmlParserSyntax
{
public:
	CString m_sName;
	CString m_sParent;
	CString m_sDefault;
};


class CXmlParserInfo
{
public:
	int	m_iPosBegin;
	int m_iPosEnd;
	int	m_iPosBeginValue;
	int	m_iState;
	int m_iStatus;
	bool m_bComment;
	CString m_sTag;
	CString m_sTagPrev;
	CString m_sValueTemp;
	CString m_sValue;
};


class CXmlParser
{
public:
	CXmlParser(CString sFile);
	~CXmlParser(void);
	void Start(CXmlParserInfo *pParserInfo);
	bool Parse(int *piPos, CString sParse,  CXmlParserInfo *pParserReturn);
	bool CheckSyntax(CXmlParserInfo *pParserInfo);
	bool CheckDefaultValue(CXmlParserInfo *pParserInfo);
	CString NotClosed();

private:
	bool GetTag(int *piPos, CString *psParse, CXmlParserInfo *pParserReturn);
	void ReadSyntaxXml(CString sFile);

	std::deque<CXmlParserSyntax*> m_lParseSyntax;

	std::deque<CString> m_lParsed;

	bool m_bBoincClientV6;

};


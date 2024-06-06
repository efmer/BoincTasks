#include "StdAfx.h"
#include "Xml.h"
#include "XmlParser.h"


CXmlParser::CXmlParser(CString sFile)
{
	ReadSyntaxXml(sFile);
}


CXmlParser::~CXmlParser(void)
{
	for (int iPos = 0; iPos < (int) m_lParseSyntax.size(); iPos++)
	{
		delete m_lParseSyntax.at(iPos);
	}
}

void CXmlParser::Start(CXmlParserInfo *pParserInfo)
{
	m_lParsed.clear();
	pParserInfo->m_iState = PARSER_STATE_NONE;
	pParserInfo->m_sValueTemp = "";
	pParserInfo->m_bComment = false;
}

bool CXmlParser::Parse(int *piPos, CString sParse,  CXmlParserInfo *pParserInfo)
{	
	pParserInfo->m_iPosBegin = -1;
	pParserInfo->m_iPosEnd = -1;
	pParserInfo->m_sTag = "";
	pParserInfo->m_sTagPrev = "";
	pParserInfo->m_iStatus = PARSER_INVALID_TAG;

	return GetTag(piPos, &sParse,  pParserInfo);
}

bool CXmlParser::GetTag(int *piPos, CString *psParse, CXmlParserInfo *pParserInfo)
{
	char	c;
	int		iLen;
//	int iState = PARSER_STATE_NONE;

	if (psParse->Find("<!--") != -1)
	{
		pParserInfo->m_bComment = true;
		return false;
	}

	if (psParse->Find("-->") != -1)
	{
		pParserInfo->m_bComment = false;
		return false;
	}

	if (pParserInfo->m_bComment)
	{
		return false;
	}

	iLen = psParse->GetLength();
	c = 1;
	while ((c != 0) && (*piPos < iLen))
	{
		c = psParse->GetAt(*piPos);
		if (c == 0xd || c == ' ')
		{
			(*piPos)++;
			continue;	// skip lf and space
		}

		if (c == 0)
		{
			if (pParserInfo->m_iState == PARSER_STATE_NONE || pParserInfo->m_iState == PARSER_STATE_END)
			{
				if (pParserInfo->m_sValueTemp.GetLength() != 0)
				{
					pParserInfo->m_iPosBegin = pParserInfo->m_iPosBeginValue;
					pParserInfo->m_iPosEnd = *piPos;
					pParserInfo->m_iStatus = PARSER_INVALID_VALUE;
					return true;
				}
				return false;
			}
			else
			{
				pParserInfo->m_iPosBegin = *piPos;
				pParserInfo->m_iPosEnd = *(piPos + 1);
				pParserInfo->m_iStatus = PARSER_EOL;
				return true;
			}
		}

		if (c == '<')
		{
			if (pParserInfo->m_iState == PARSER_STATE_NONE || pParserInfo->m_iState == PARSER_STATE_END)
			{
				pParserInfo->m_iPosBegin = (*piPos)+1;
				pParserInfo->m_iState = PARSER_STATE_BEGIN;
				pParserInfo->m_sValue = "";
			}
			else
			{
				pParserInfo->m_iPosBegin = *piPos;
				pParserInfo->m_iPosEnd = (*piPos) + 1;
				pParserInfo->m_iStatus = PARSER_INVALID_BEGIN;
				return true;
			}
		}
		else
		{
			if ((pParserInfo->m_iState == PARSER_STATE_END))	// End of <tag> is a value if the next tag is the same.
			{
				if (pParserInfo->m_sValueTemp.GetLength() == 0)
				{
					pParserInfo->m_iPosBeginValue = *piPos;
				}
				pParserInfo->m_sValueTemp += c;
			}
			if (pParserInfo->m_iState == PARSER_STATE_NONE)
			{
				pParserInfo->m_iPosBegin = *piPos;
				pParserInfo->m_iPosEnd = (*piPos)+1;
				pParserInfo->m_iStatus = PARSER_INVALID_CHAR;
				return true;
			}
		}

		pParserInfo->m_iPosEnd = *piPos;

		if (c == '>')
		{
			if (pParserInfo->m_iState == PARSER_STATE_BEGIN)
			{
				pParserInfo->m_iState = PARSER_STATE_END;
				pParserInfo->m_sTag = psParse->Mid(pParserInfo->m_iPosBegin, pParserInfo->m_iPosEnd - pParserInfo->m_iPosBegin);
				if (m_lParsed.size() > 0)
				{
					pParserInfo->m_sTagPrev = m_lParsed.at(m_lParsed.size()-1);
				}
				else
				{
					pParserInfo->m_sTagPrev = "";
				}

				if (pParserInfo->m_sTag.GetAt(0) == '/')	
				{
					if (m_lParsed.size() == 0)
					{
						pParserInfo->m_iStatus = PARSER_INVALID_CLOSE;
						(*piPos)++;
						return true;
					}
					pParserInfo->m_sTag = pParserInfo->m_sTag.Mid(1);
					if (pParserInfo->m_sTag == pParserInfo->m_sTagPrev)
					{
						pParserInfo->m_iState = PARSER_STATE_NONE;
						pParserInfo->m_sValue = pParserInfo->m_sValueTemp;
						pParserInfo->m_sValueTemp = "";
						m_lParsed.pop_back();	// move back
						pParserInfo->m_iStatus = PARSER_VALID_ENDTAG;
						(*piPos)++;
						return true;
					}
					else
					{
						pParserInfo->m_iStatus = PARSER_INVALID_CLOSE;
						(*piPos)++;
						return true;
					}
				}
				else
				{
					if (pParserInfo->m_sValueTemp.GetLength() != 0)
					{
						pParserInfo->m_iPosEnd = pParserInfo->m_iPosBegin-1;
						pParserInfo->m_iPosBegin = pParserInfo->m_iPosBeginValue;
						pParserInfo->m_iStatus = PARSER_INVALID_VALUE;
						return true;
					}
					m_lParsed.push_back(pParserInfo->m_sTag);
				}

				pParserInfo->m_iStatus = PARSER_VALID_TAG;
				(*piPos)++;
				return true;
			}
			else
			{
//				pParserReturn->m_iPosEnd = (*piPos)-1;
				pParserInfo->m_iStatus = PARSER_INVALID_TAG;
				(*piPos)++;
				return true;
			}
		}

		(*piPos)++;
	}

	if (pParserInfo->m_iState == PARSER_STATE_BEGIN)
	{
		pParserInfo->m_iStatus = PARSER_INVALID_TAG;
		(*piPos)++;
		return true;
	}

	return false;
}

bool CXmlParser::CheckSyntax(CXmlParserInfo *pParserInfo)
{
	for (int iPos = 0; iPos < (int) m_lParseSyntax.size(); iPos++)
	{
		if (pParserInfo->m_sTag == m_lParseSyntax.at(iPos)->m_sName)
		{
			if (pParserInfo->m_sTagPrev == m_lParseSyntax.at(iPos)->m_sParent)
			{
				return true;
			}
		}
	}
	return false;
}

bool CXmlParser::CheckDefaultValue(CXmlParserInfo *pParserInfo)
{
	if (pParserInfo->m_sValue.GetLength() == 0) return false;

	for (int iPos = 0; iPos < (int) m_lParseSyntax.size(); iPos++)
	{
		if (pParserInfo->m_sTag == m_lParseSyntax.at(iPos)->m_sName)
		{
			if (pParserInfo->m_sValue == m_lParseSyntax.at(iPos)->m_sDefault)
			{
				return true;
			}
		}
	}
	return false;
}

void CXmlParser::ReadSyntaxXml(CString sFile)
{
//	int			iPos;
	CString		sName, sParent;
	CXmlParserSyntax *pXmlParserSyntax;

//	sFile = "config_editor.xml";
	CXml xml(sFile,false,true);
	if (xml.ReadStart("config_editor"))
	{
		while (xml.ReadGroup("tag"))
		{
			pXmlParserSyntax = new CXmlParserSyntax;

			xml.ReadItem("name",&pXmlParserSyntax->m_sName);
			xml.ReadItem("parent",&pXmlParserSyntax->m_sParent);
			xml.ReadItem("default",&pXmlParserSyntax->m_sDefault);
			m_lParseSyntax.push_back(pXmlParserSyntax);
		}
	}

}

CString CXmlParser::NotClosed()
{
	if (m_lParsed.size() > 0)
	{
		return m_lParsed[0];
	}
	return "";
}
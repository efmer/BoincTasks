// Json.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "Json.h"


// CJson

CJson::CJson()
{

}

CJson::~CJson()
{
}

// validate on: http://jsonlint.com/

// CJson member functions

void CJson::Array(char *psId, std::deque<std::string> *pListItems, std::deque<std::string> *pList)
{
	int	iSizeItems, iSize, iPos;

	iSizeItems = (int) pListItems->size();
	iSize = (int) pList->size();

	AddString(psId);
	m_sJson += ":[";

	iPos = 0;

	while (iPos < iSize)
	{
		if (iPos > 0) m_sJson += ",";
		m_sJson += "{";
		for (int iPosItem = 0; iPosItem < iSizeItems; iPosItem++)
		{
			if (iPosItem > 0) m_sJson += ",";
			Add((char *) pListItems->at(iPosItem).c_str(), (char *) pList->at(iPos).c_str());
			iPos++;
		}
		m_sJson += "}";
	}
	m_sJson += "]";	
}
void CJson::Add(char *psId, char *psString)
{
	AddString(psId);
	m_sJson += ":";
	AddString(psString);
}

void CJson::AddString(char *psString)
{
	m_sJson += "\x22";
	m_sJson += psString;
	m_sJson += "\x22";
}

std::string CJson::Get()
{
	std::string sData;

	if (m_sJson.length() > 0)
	{
		sData = "{";
		sData+= m_sJson;
		sData+= "}";
	}
	return sData;
}

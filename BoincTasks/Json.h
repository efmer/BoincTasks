#pragma once

// CJson command target

class CJson : public CObject
{
public:
	CJson();
	virtual ~CJson();

	void	Array(char *psId, std::deque<std::string> *pListItems, std::deque<std::string> *pList);
	void	Add(char *psId, char *psString);
	std::string Get();

private:
	void	AddString(char *psString);

	std::string	m_sJson;

};



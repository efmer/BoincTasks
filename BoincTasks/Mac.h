#pragma once

#include <deque>
#include <string>

class CMac
{
public:
	CMac(void);
	~CMac(void);

	int	Extract(char *psMac, std::deque<std::string> *plMac = NULL);
	int	IsValidMacAddress(const char* mac);
};


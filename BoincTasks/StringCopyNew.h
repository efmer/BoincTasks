#pragma once
class CStringCopyNew
{
public:
	CStringCopyNew(char **pcDest, char *pcSource, bool bDelete = true);
	virtual ~CStringCopyNew(void);
};


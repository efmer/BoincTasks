#pragma once

class CPushCheck : public CObject
{
public:
	CPushCheck();
	virtual ~CPushCheck();

	bool CPushCheck::ReadPushNotice(CStringA *psTxt);
	bool CPushCheck::ReadFile(char * pcServer, char *pcHeaderType, char *pcLocation, char *pcFile, CStringA *psAXml);

private:

};



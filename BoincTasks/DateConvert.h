#pragma once

// CDateConvert command target

class CDateConvert : public CObject
{
public:
	CDateConvert();
	virtual ~CDateConvert();

	void Convert(time_t tmtime, char *pBuffer);

};



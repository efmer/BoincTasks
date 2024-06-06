#pragma once

// CTimeString command target

class CTimeString : public CObject
{
public:
	CTimeString();
	virtual ~CTimeString();

	bool TimeStringToDouble(CString sTime, double *pdTime);
	CString DoubleToTimeString(double dt);
	void  IllegalTime(CString sTxt1, CString sTxt2);
	void TimeString(int iTime, char *pBuffer, int iNrBuffer,  char *pBefore=NULL, char *pAfter=NULL, bool bShowZero=false, bool bAllowNegative = false);
};



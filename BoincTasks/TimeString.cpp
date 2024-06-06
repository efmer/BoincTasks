// TimeStringDouble.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsView.h"
#include "TimeString.h"


// CTimeString

CTimeString::CTimeString()
{
}

CTimeString::~CTimeString()
{
}


// convert string HH:MM into double and check if valid
bool CTimeString::TimeStringToDouble(CString sTime, double *pdTime)
{
	double dHour;
	double dMinutes;
	CString sHour, sMinutes;
	int iSeperatorPos;
	
	if (sTime == "" || sTime == "0")		// shortcuts
	{
		sTime = "00:00";
	}
	iSeperatorPos = sTime.Find(':',0);
	if (iSeperatorPos < 0) return false;

	sHour = sTime.Mid(0,iSeperatorPos);
	if (sHour.GetLength() != 2) return false;
	sMinutes  = sTime.Mid(iSeperatorPos+1);
	if (sMinutes.GetLength() != 2) return false;

	dHour = atof(sHour);
	if (dHour < 0) return false;
	if (dHour > 24) return false;

	dMinutes = atof(sMinutes);
	if (dMinutes < 0) return false;
	if (dMinutes > 59) return false;

	dMinutes = dMinutes / 60.0;
	*pdTime = dHour + dMinutes;
	return true;
}

// convert a double into a timestring HH:MM
CString CTimeString::DoubleToTimeString(double dt)
{
	CString sFormat;

	int hour = (int)dt;
	int minutes = (int)(60.0 * (dt - hour)+.5);
	sFormat.Format("%02d:%02d",hour,minutes);

	return sFormat;
}


void  CTimeString::IllegalTime(CString sTxt1, CString sTxt2)
{
	CString sTxt;

	sTxt = "Illegal time: format = HH:MM in: ";
	sTxt+= sTxt1;
	sTxt+= "\r\nFound: ";
	sTxt+= sTxt2;

	AfxMessageBox(sTxt);
}

void CTimeString::TimeString(int iTime, char *pBuffer, int iNrBuffer, char *pBefore, char *pAfter, bool bShowZero, bool bAllowNegative)
{
	unsigned int iDay, iHour, iMinute, iSecond, iTimeNew;
	char		cBufferSign[2];

	cBufferSign[0] = 0;
	cBufferSign[1] = 0;
	*pBuffer = 0;

	if (iTime < 0)
	{
		if (!bAllowNegative)
		{
			strcpy_s(pBuffer,10,INVALID_TIME_STRING);
			return;
		}
		iTime = abs(iTime);
		cBufferSign[0] = '-';
	}

	if (iTime > 0)
	{
		iDay = iTime / (60*60*24);
		iTimeNew = iTime - (iDay*60*60*24);
		iHour = iTimeNew / (60*60);
		iTimeNew = iTimeNew - (iHour*60*60);
		iMinute = iTimeNew / 60;
		iTimeNew = iTimeNew - (iMinute * 60);
		iSecond = iTimeNew;

//		iMinute = iTime / 60;
//		iSecond = iTime - iMinute*60 ;
//		iHour	= iMinute / 60;
//		iMinute = iMinute - iHour *60;
//		iDay	= iHour / 24;
//		iHour	= iHour - iDay * 24;
	}
	else
	{
		iMinute = 0;
		iSecond = 0;
		iHour	= 0;
		iMinute = 0;
		iDay	= 0;
		iHour	= 0;
	}

	if (theApp.m_pDlgSettingsView->m_iTimeDhms == TIME_DHMS)
	{
		if (iTime > 0)
		{
			if (iDay > 0) _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%s%02dd,%02d:%02d:%02d%s",cBufferSign,pBefore,iDay,iHour,iMinute,iSecond,pAfter);
			else _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%s%02d:%02d:%02d%s",cBufferSign, pBefore, iHour,iMinute,iSecond, pAfter);
		}
		else
		{
			if (bShowZero) _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02d:%02d:%02d%s", pBefore, 0,0,0, pAfter);
			else _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s-%s",pBefore, pAfter);
		}
	}
	else
	{
		if (iTime > 0)
		{
			iHour += iDay * 24;
			_snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%s%02d:%02d:%02d%s",cBufferSign, pBefore, iHour,iMinute,iSecond, pAfter);
		}
		else
		{
			if (bShowZero) _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s%02d:%02d:%02d%s", pBefore, 0,0,0, pAfter);
			else _snprintf_s(pBuffer,iNrBuffer,_TRUNCATE,"%s-%s",pBefore, pAfter);
		}
	}
}
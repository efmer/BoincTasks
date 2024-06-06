// DateConvert.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "DlgSettingsView.h"
#include "DateConvert.h"


// CDateConvert

CDateConvert::CDateConvert()
{
}

CDateConvert::~CDateConvert()
{
}


// CDateConvert member functions


void CDateConvert::Convert(time_t tConvert, char *pBuffer)
{
	struct tm   tmtime;
	CString		sTimeFormat;
	int			iErr, iTime;

	iErr = localtime_s(&tmtime, &tConvert);
	iTime = theApp.m_pDlgSettingsView->m_iTime;
	sTimeFormat = theApp.m_pDlgSettingsView->m_sTimeFormat;
	*pBuffer = 0;

	if (iErr)
	{
		strcpy_s(pBuffer,10,INVALID_TIME_STRING);
		return;
	}

	if (tConvert <= 0)
	{
		strcpy_s(pBuffer,10,INVALID_TIME_STRING);
		return;
	}

	switch (iTime)
	{
		case TIME_24:
			strftime(pBuffer, 40, "%d-%m-%Y %H:%M", &tmtime);
		break;
		case TIME_12:
			if (tmtime.tm_hour >= 12)	strftime(pBuffer, 40, "%d-%m-%Y %I:%M PM", &tmtime);
			else 						strftime(pBuffer, 40, "%d-%m-%Y %I:%M AM", &tmtime);
		break;
		case TIME_USER:
			strftime(pBuffer, 40, sTimeFormat, &tmtime);
		break;
		default:
			strftime(pBuffer, 40, "%x %X", &tmtime);
	}
}
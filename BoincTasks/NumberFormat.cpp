// TimeStringDouble.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsView.h"
#include "NumberFormat.h"


// CNumberFormat

CNumberFormat::CNumberFormat()
{
}

CNumberFormat::~CNumberFormat()
{
}

CString CNumberFormat::FormatNumberNT(double dNr, int iNrComma, bool bAllowNegative)
{
	return FormatNumber(dNr, iNrComma, bAllowNegative, false);
}

CString CNumberFormat::FormatNumber(double dNr, int iNrComma, bool bAllowNegative, bool bThousand)
{
	int		iLen;
	CString	sNr, sLeft, sRight, sSeperators;
	char	cComma, cThousend;

	if (dNr < 0 && !bAllowNegative) 
	{
		return "-";
	}

	cThousend = theApp.m_pDlgSettingsView->m_cThousend;
	cComma = theApp.m_pDlgSettingsView->m_cComma;

	switch(iNrComma)
	{
		case 10:
			sNr.Format("%.10f",dNr);
			iLen = -9;
		break;
		case 3:
			sNr.Format("%.3f",dNr);
			iLen = -2;
		break;
		case 2:
			sNr.Format("%.2f",dNr);
			iLen = -1;
		break;
		case 1:
			sNr.Format("%.1f",dNr);
			iLen = 0;
		break;
		default:
			sNr.Format("%.0f",dNr);
			iLen = 2;
	}

	sNr.Replace('.', cComma);

	if (bThousand)
	{
		if (cThousend != 0)
		{
			iLen+= sNr.GetLength();
			iLen-= 2+3;					//xxx.x

			while (iLen > 0)
			{
				sLeft = sNr.Left(iLen);
				if (sLeft == '-') return sNr;	// oeps the minus abort
				sRight= sNr.Mid(iLen);
				sNr = sLeft + cThousend + sRight;
				iLen-=3;
			}
		}
	}
	return sNr;
}

double CNumberFormat::atofLocaleWithThousand(CString sTxt)
{
	sTxt.Remove(theApp.m_pDlgSettingsView->m_cThousend);
	sTxt.Replace(theApp.m_pDlgSettingsView->m_cComma, '.');
	return atof(sTxt);
}

double CNumberFormat::atofLocale(CString sTxt)
{
// don't remove thousand as this is also a decimal point.
	sTxt.Replace(theApp.m_pDlgSettingsView->m_cComma, '.');
	return atof(sTxt);
}
#pragma once

// CNumberFormat command target

class CNumberFormat : public CObject
{
public:
	CNumberFormat();
	virtual ~CNumberFormat();

	CString FormatNumberNT(double dNr, int iNrComma, bool bAllowNegative=false);
	CString FormatNumber(double dNr, int iNrComma, bool bAllowNegative=false, bool bThousand=true);
	double	atofLocale(CString sTxt);
	double	atofLocaleWithThousand(CString sTxt);
};



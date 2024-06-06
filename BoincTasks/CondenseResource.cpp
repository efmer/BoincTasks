// TimeStringDouble.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "CondenseResource.h"


// CCondenseResource

CCondenseResource::CCondenseResource()
{
}

CCondenseResource::~CCondenseResource()
{
}

void CCondenseResource::Condense(CString *psCondense)
{
	CString sToCondense, sLeft, sRight;
	int		iFound;

	sToCondense = *psCondense;
	iFound = sToCondense.Find(" CPU");
	sToCondense.Replace(" CPUs","C");
	sToCondense.Replace(" CPU", "C");

	if (iFound >= 0)
	{
		sLeft = sToCondense.Left(iFound);
		sRight= sToCondense.Mid(iFound);

		sRight.Replace(".00","");
		sRight.Replace(" NVIDIA GPUs","NV");
		sRight.Replace(" NVIDIA GPU","NV");
		sRight.Replace(" Nvidia GPU", "NV");
		sRight.Replace(" ATI GPUs","AMD");
		sRight.Replace(" AMD/ATI GPU", "AMD");
		sRight.Replace(" AMD / ATI GPU", "AMD");

		sRight.Replace(" intel GPU", "INT");
		sRight.Replace(" intel_gpu GPU","INT");
		sRight.Replace(" Intel GPU", "INT");
		sRight.Replace("device ","d");
		sRight.Replace("Device ", "d");
		*psCondense = sLeft + sRight;
	}
	else
	{
		iFound = sToCondense.Find(" Apple");
		if (iFound >= 0)
		{
			sLeft = sToCondense.Left(iFound);
			sRight = sToCondense.Mid(iFound);
			sRight.Replace("Apple ", "A ");
			*psCondense = sLeft + sRight;
		}
		else
		{
			*psCondense = sToCondense;
		}
	}

}
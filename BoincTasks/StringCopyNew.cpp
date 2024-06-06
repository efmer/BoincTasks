#include "StdAfx.h"
#include "StringCopyNew.h"


CStringCopyNew::CStringCopyNew(char **pcDest, char *pcSource, bool bDelete)
{
	int iLen;

	if (bDelete)
	{
		if (*pcDest != NULL)
		{
			delete *pcDest;
			*pcDest = NULL;
		}
	}

	if (pcSource == NULL) return;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
	return;
}

CStringCopyNew::~CStringCopyNew(void)
{
}

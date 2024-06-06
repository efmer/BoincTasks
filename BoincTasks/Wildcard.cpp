#include "StdAfx.h"
#include "Wildcard.h"


CWildcard::CWildcard(void)
{
}


CWildcard::~CWildcard(void)
{
}

bool CWildcard::Compare(char *pcWc, char *pcOther)
{
	while (1)
	{
		if (*pcWc == '?')
		{
			pcWc++;
			pcOther++;
			continue;
		}
		if (*pcWc == '*')
		{
			return true;
		}
		if (*pcWc != *pcOther)
		{
			return false;
		}

		pcWc++;
		pcOther++;

		if (*pcWc == 0)
		{
			if (*pcOther == 0)
			{
				return true;
			}
			return false;
		}
		if (*pcOther == 0)
		{
			return false;
		}
	}
}

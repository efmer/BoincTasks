#include "StdAfx.h"
#include "Mac.h"


CMac::CMac(void)
{
}


CMac::~CMac(void)
{
}

int CMac::Extract(char *pcMac, std::deque<std::string> *plMac)
{
	int iPos, iValid;
	char	*pcFound;
	std::string sMac1, sMac2;

	pcFound = (char *) strstr(pcMac, ";");

	if (pcFound == NULL)
	{
		iValid = IsValidMacAddress(pcMac);
		if (iValid)
		{
			sMac1 = pcMac;
			if (plMac != NULL) plMac->push_back(sMac1);
		}
		return iValid;
	}
	sMac1 = pcMac;
	if (sMac1.length() < 6) return 0;
	iPos = (int) (pcFound - pcMac);
	sMac2 = sMac1.substr(iPos+1, 32);
	sMac1 = sMac1.substr(0,iPos);

	iValid = IsValidMacAddress(sMac1.c_str());
	if (iValid == 0) return 0;
	iValid = IsValidMacAddress(sMac2.c_str());
	if (iValid == 0) return 0;

	if (plMac != NULL)
	{
		plMac->push_back(sMac1);
		plMac->push_back(sMac2);
	}
	return 2;
}


int CMac::IsValidMacAddress(const char* mac)
{
    int i = 0;
    int s = 0;

    while (*mac)
	{
       if (isxdigit(*mac))
	   {
          i++;
       }
       else if (*mac == '-')
	   {

          if (i == 0 || i / 2 - 1 != s)
            break;
          ++s;
       }
       else
	   {
           s = -1;
       }

       ++mac;
    }

    return (i == 12 && (s == 5 || s == 0));
}
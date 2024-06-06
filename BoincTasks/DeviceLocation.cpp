// DeviceLocation.cpp : implementation file
//

#include "stdafx.h"
#include "Setupapi.h"
#include "deviceLocation.h"
#include "deviceLocationInformationList.h"

//#include <C:\WinDDK\6001_18002\inc\ddk\ndisguid.h>

// CDeviceLocation

CDeviceLocation::CDeviceLocation()
{
//	m_hDevInfo	= NULL;
//	m_sList		= "";

}

CDeviceLocation::~CDeviceLocation()
{
//	if(m_hDevInfo != NULL)
//	{
//		 SetupDiDestroyDeviceInfoList(m_hDevInfo);
//	}
}


// CDeviceLocation member functions


int CDeviceLocation::GetList(CString *psTxt, bool bShow, bool bDump, int iBytes)
{
	CDeviceLocationInformation *pItemLocation;
	CString	sTemp;
	int iItems = 0;

	CDeviceLocationInformationList	dlistLocation;
	iItems = dlistLocation.Collect(bShow, bDump, iBytes);
	dlistLocation.EnumReset();

	while( (pItemLocation = dlistLocation.EnumNext()) )
	{
		iItems++;
		sTemp.Format( L"Bus %i, Device %i (%x), Function %i\r\n", pItemLocation->m_iBus, pItemLocation->m_iDevice,  pItemLocation->m_iDevice, pItemLocation->m_iFunction );	
		*psTxt += sTemp;
		sTemp.Format( L"    PDO Name=%s\r\n", (LPCTSTR) pItemLocation->m_sPdoName );
		*psTxt += sTemp;
		sTemp.Format( L"    Description=%s\r\n", (LPCTSTR) pItemLocation->m_sDescription );
		*psTxt += sTemp;
		sTemp.Format( L"    Pnp Id=%s\r\n", (LPCTSTR) pItemLocation->m_sPnpInstanceID);
		*psTxt += sTemp;
		*psTxt += pItemLocation->m_sDump;


		sTemp.Format( L"\r\n\r\n" );
		*psTxt += sTemp;
	}

	return iItems;
}
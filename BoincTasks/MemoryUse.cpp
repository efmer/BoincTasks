// MemoryUse.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "Psapi.h"
#include "MemoryUse.h"


// CMemoryUse

CMemoryUse::CMemoryUse()
{
}

CMemoryUse::~CMemoryUse()
{
}

void CMemoryUse::GetCurrentUsage(CString *psTxt)
{
	HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;
	CString sFormat;

	hProcess = GetCurrentProcess();
	
	if ( GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)) )
    {
		sFormat.Format( "\r\n\tPageFaultCount\t\t: 0x%08X\t%uK\r\n", pmc.PageFaultCount, pmc.PageFaultCount/1000 );
		*psTxt += sFormat;
        sFormat.Format( "\tPeakWorkingSetSize\t: 0x%08X\t%uK\r\n", pmc.PeakWorkingSetSize, pmc.PeakWorkingSetSize/1000  );
		*psTxt += sFormat;
        sFormat.Format( "\tWorkingSetSize\t\t: 0x%08X\t%uK\r\n", pmc.WorkingSetSize, pmc.WorkingSetSize/1000  );
		*psTxt += sFormat;
        sFormat.Format( "\tQuotaPeakPagedPool\t: 0x%08X\t%uK\r\n", pmc.QuotaPeakPagedPoolUsage, pmc.QuotaPeakPagedPoolUsage/1000  );
		*psTxt += sFormat;
        sFormat.Format( "\tQuotaPagedPool\t\t: 0x%08X\t%uK\r\n", pmc.QuotaPagedPoolUsage, pmc.QuotaPagedPoolUsage/1000  );
		*psTxt += sFormat;
        sFormat.Format( "\tQuotaPeakNonPagedPool\t: 0x%08X\t%uK\r\n", pmc.QuotaPeakNonPagedPoolUsage, pmc.QuotaPeakNonPagedPoolUsage/1000  );
		*psTxt += sFormat;
        sFormat.Format( "\tQuotaNonPagedPool\t: 0x%08X\t%uK\r\n", pmc.QuotaNonPagedPoolUsage, pmc.QuotaNonPagedPoolUsage/1000  );
		*psTxt += sFormat;
        sFormat.Format( "\tPagefile\t\t\t: 0x%08X\t%uK\r\n", pmc.PagefileUsage, pmc.PagefileUsage/1000  ); 
		*psTxt += sFormat;
        sFormat.Format( "\tPeakPagefile\t\t: 0x%08X\t%uK\r\n", pmc.PeakPagefileUsage, pmc.PeakPagefileUsage/1000  );
		*psTxt += sFormat;
    }

    CloseHandle( hProcess );


HANDLE WINAPI GetCurrentProcess(void);
}


// CMemoryUse member functions

// UseCpuGpu.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "UseCpuGpu.h"
#include "NumberFormat.h"


// CUseCpuGpu

IMPLEMENT_DYNAMIC(CUseCpuGpu, CWnd)

CUseCpuGpu::CUseCpuGpu()
{

}

CUseCpuGpu::~CUseCpuGpu()
{
}

bool CUseCpuGpu::GetCpuGpu(char * pszResources, double *pdCpu, double *pdGpu, int *piGPU, bool *pbCuda, int *piTypeGpu)
{
	CNumberFormat numberFormat;
	char *pszFound;
	bool	bFound = false;

	*pdGpu = 0;
	*pdCpu = 1;
	*piTypeGpu = DETECTED_NONE;
	*pbCuda = false;
	*piGPU = 0;
	if (pszResources == NULL) return false;

//	pszResources = "3 CPU";	// test only

	// BEGIN make sure this is identical to RPC_CLIENT::get_results_CPA
	pszFound = strstr(pszResources, "GPU");
	if (pszFound == NULL) pszFound = strstr(pszResources, "CUDA");
	if (pszFound == NULL) pszFound = strstr(pszResources, "Apple");
	// END make sure this is identical to RPC_CLIENT::get_results_CPA

	if (pszFound != NULL)
	{
		bFound = true;
		*pdCpu = numberFormat.atofLocale(pszResources);
		// older version (6.6.37) use (0.04 CPUs, 1 CUDA) , as seperator
		// newer version (6.10.18) use 0.04 CPUs + 1.00 NVIDIA GPUs (device 0)  + as seperator and device only if there are more than one.
		pszFound = strstr(pszResources, "+");
		if (pszFound == NULL) pszFound = strstr(pszResources, ",");		
		if (pszFound != NULL)
		{
			pszFound += 1;
			*pdGpu = atof(pszFound);
//			if (*pdGpu > 0)
			{
				*pbCuda = true;
				*piGPU = 1;

				pszFound = strstr(pszResources, "NVIDIA");
				if (pszFound != NULL)
				{
					*piTypeGpu = DETECTED_NV;
					return bFound;
				}
				pszFound = strstr(pszResources, "CUDA");
				if (pszFound != NULL)
				{
					*piTypeGpu = DETECTED_NV;
					return bFound;
				}
				pszFound = strstr(pszResources, "ATI");
				if (pszFound != NULL)
				{
					*piTypeGpu = DETECTED_ATI;
					return bFound;
				}
				pszFound = strstr(pszResources, "Apple");
				if (pszFound != NULL)
				{
					*piTypeGpu = DETECTED_APPLE;
					return bFound;
				}
				*piTypeGpu = DETECTED_INTEL;
				return bFound;
			}
		}
	}
	pszFound = strstr(pszResources, "CPU");
	// check for number of CPU used
	if (pszFound != NULL)
	{
		*pdCpu = numberFormat.atofLocale(pszResources);
		if (*pdCpu < 1) *pdCpu = 1;	// at least one CPU.
	}

	return bFound;
}



BEGIN_MESSAGE_MAP(CUseCpuGpu, CWnd)
END_MESSAGE_MAP()



// CUseCpuGpu message handlers



#pragma once

#define DETECTED_NONE	0
#define DETECTED_NV		1
#define DETECTED_ATI	2
#define DETECTED_APPLE	3
#define DETECTED_INTEL	4


// CUseCpuGpu

class CUseCpuGpu : public CWnd
{
	DECLARE_DYNAMIC(CUseCpuGpu)

public:
	CUseCpuGpu();
	virtual ~CUseCpuGpu();

	bool	GetCpuGpu(char * pszResources, double *pdCpu, double *pdGpu, int *piGPU, bool *pbCuda, int *piType);

protected:
	DECLARE_MESSAGE_MAP()
};



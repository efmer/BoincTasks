#pragma once

// CMemoryUse command target

class CMemoryUse : public CObject
{
public:
	CMemoryUse();
	virtual ~CMemoryUse();

	void GetCurrentUsage(CString *psTxt);

};



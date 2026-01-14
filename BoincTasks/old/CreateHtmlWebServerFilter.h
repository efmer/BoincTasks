#pragma once

// CreateHtmlWebServerFilter command target

class CreateHtmlWebServerFilter : public CObject
{
public:
	CreateHtmlWebServerFilter();
	virtual ~CreateHtmlWebServerFilter();

	CString Create(CString sFilter, int iFilterId, int iFilter);

};



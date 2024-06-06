#pragma once

// CreateHtmlWebServerGraphProjectSelect command target

class CreateHtmlWebServerGraphProjectSelect : public CObject
{
public:
	CreateHtmlWebServerGraphProjectSelect();
	virtual ~CreateHtmlWebServerGraphProjectSelect();

	bool Create(CRemoteCombinedDoc	*pDoc, CString *psData, bool bSingle);

};


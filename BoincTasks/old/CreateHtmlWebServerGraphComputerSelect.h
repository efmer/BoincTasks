#pragma once

// CreateHtmlWebServerGraphComputerSelect command target

class CRemoteCombinedDoc;
class CreateHtmlWebServerGraphComputerSelect : public CObject
{
public:
	CreateHtmlWebServerGraphComputerSelect();
	virtual ~CreateHtmlWebServerGraphComputerSelect();

	bool Create(CRemoteCombinedDoc *pDoc, CString *psData);
};



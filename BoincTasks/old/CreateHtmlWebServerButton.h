#pragma once


// CreateHtmlWebServerButton command target

class CreateHtmlWebServerButton : public CObject
{
public:
	CreateHtmlWebServerButton();
	virtual ~CreateHtmlWebServerButton();
	CString Create(char *pcName, char *pcActionName, char *pcAction, char *pcTheme, bool bSelected);

private:
	CString m_sButtonTemplate;

};





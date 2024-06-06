// CreateHtmlWebServerButton.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "CreateHtmlWebServerButton.h"


// CreateHtmlWebServerButton

CreateHtmlWebServerButton::CreateHtmlWebServerButton()
{
	m_sButtonTemplate = "<a href='#' data-role='button' id='<__ActionName__>' data-theme='__Theme__' data-mini='true' data-icon='__checked__' onclick= '__Action__'  >__Name__</a>";
//'buttonSelectProject(\x22__ActionName__\x22)'
}

CreateHtmlWebServerButton::~CreateHtmlWebServerButton()
{
}


// CreateHtmlWebServerButton member functions

CString CreateHtmlWebServerButton::Create(char *pcName, char *pcActionName, char *pcAction, char *pcTheme, bool bSelected)
{
	CString sButton, sCheck;

	sButton = m_sButtonTemplate;
	sButton.Replace("__Theme__", pcTheme);
	sButton.Replace("__Action__", pcAction);
	sButton.Replace("__ActionName__", pcActionName);
	sButton.Replace("__Name__", pcName);
	if (bSelected) sCheck = "check";
	else sCheck = "checkbox-off";
	sButton.Replace("__checked__" , sCheck);

	return sButton;
}

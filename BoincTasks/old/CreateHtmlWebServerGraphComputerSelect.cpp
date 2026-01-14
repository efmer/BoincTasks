// CreateHtmlWebServerGraphComputerSelect.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "RemoteCombinedDoc.h"
#include "DlgGraphic.h"
#include "CreateHtmlWebServerButton.h"
#include "CreateHtmlWebServerGraphComputerSelect.h"


// CreateHtmlWebServerGraphComputerSelect

CreateHtmlWebServerGraphComputerSelect::CreateHtmlWebServerGraphComputerSelect()
{
}

CreateHtmlWebServerGraphComputerSelect::~CreateHtmlWebServerGraphComputerSelect()
{
}


// CreateHtmlWebServerGraphComputerSelect member functions

bool CreateHtmlWebServerGraphComputerSelect::Create(CRemoteCombinedDoc *pDoc, CString *psData)
{
	std::deque<CSelectedItem*>				*plComputer;
	CString				sButtonTemplate, sButton, sCheck;

	CreateHtmlWebServerButton createHtmlWebServerButton;
	plComputer = (std::deque<CSelectedItem*>*) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_COMPUTER, 0);
	for (int iCount = 0; iCount < (int) plComputer->size(); iCount++)
	{
		sButton = createHtmlWebServerButton.Create(plComputer->at(iCount)->m_sName.GetBuffer(), plComputer->at(iCount)->m_sName.GetBuffer(), "buttonSelectComputer(\x22__ActionName__\x22)", "b", plComputer->at(iCount)->m_bSelected);
		plComputer->at(iCount)->m_sName.ReleaseBuffer();
		plComputer->at(iCount)->m_sName.ReleaseBuffer();
		(*psData) += sButton;
	}
	return true;
}
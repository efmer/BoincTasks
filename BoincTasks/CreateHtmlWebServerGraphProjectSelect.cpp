// CreateHtmlWebServerGraphProjectSelect.cpp : implementation file
//


#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadRpc.h"
#include "RemoteCombinedDoc.h"
#include "DlgGraphic.h"
#include "CreateHtmlWebServerButton.h"
#include "CreateHtmlWebServerGraphProjectSelect.h"
#include "Translation.h"
#include "AnsiToUtf8.h"



// CreateHtmlWebServerGraphProjectSelect

CreateHtmlWebServerGraphProjectSelect::CreateHtmlWebServerGraphProjectSelect()
{
}

CreateHtmlWebServerGraphProjectSelect::~CreateHtmlWebServerGraphProjectSelect()
{
}

// CreateHtmlWebServerGraphProjectSelect member functions


bool CreateHtmlWebServerGraphProjectSelect::Create(CRemoteCombinedDoc *pDoc, CString *psData, bool bSingle)
{
	std::deque<CSelectedItem*>				*plProject;
	CString				sButtonTemplate, sButton, sCheck;
	CAnsiToUtf8			ansiToUtf8;

	CreateHtmlWebServerButton createHtmlWebServerButton;
	plProject = (std::deque<CSelectedItem*>*) pDoc->m_pDlgGraphic->SendMessage(UWM_MSG_GRAPHIC_GET, GRAPHIC_GET_PROJECT, 0);
	for (int iCount = 0; iCount < (int) plProject->size(); iCount++)
	{
		sButton = createHtmlWebServerButton.Create(plProject->at(iCount)->m_sName.GetBuffer(), plProject->at(iCount)->m_sName.GetBuffer(), "buttonSelectProject(\x22__ActionName__\x22)", "b", plProject->at(iCount)->m_bSelected);
		plProject->at(iCount)->m_sName.ReleaseBuffer();
		plProject->at(iCount)->m_sName.ReleaseBuffer();
		(*psData) += sButton;
	}

	sButton = createHtmlWebServerButton.Create(ansiToUtf8.ToUtf8(gszTranslation[PosGroupMobileSingleSelection]).GetBuffer(), "SS", "buttonSingleProject()", "c", bSingle);
	(*psData) += sButton;

	return true;
}

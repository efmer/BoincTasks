#pragma once

#include "TemplateRemoteTaskView.h"

// CCreateHtml command target

class CBarTreeGroupItem;
class CCreateHtmlWebServerComputerSelect : public CObject
{
public:
	CCreateHtmlWebServerComputerSelect();
	virtual ~CCreateHtmlWebServerComputerSelect();

	void Create(std::deque<CBarTreeGroupItem*> *pComputerTreeGroupItem, CString *psData);
};



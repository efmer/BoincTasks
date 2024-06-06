#pragma once

//#include "gui_rpc_client.h"

// CreateHtmlWebServerGraph command target

#define GRAPH_ITEM_FIRST 0
#define GRAPH_ITEM_NORMAL 1
#define GRAPH_ITEM_LAST 2

class CComputerGraphicType;
class CreateHtmlWebServerGraph : public CObject
{
public:
	CreateHtmlWebServerGraph();
	virtual ~CreateHtmlWebServerGraph();

	bool Create(std::string *psHtml);
	bool Json(CRemoteCombinedDoc *pDoc, std::string *psJson);
	void AddProject(CString *psGroup, CString *psProjectName, std::vector<DAILY_STATS> *pStatistics,  CComputerGraphicType *pWebType);

	CString CreateHtmlWebServerGraph::CreateTypeData(CComputerGraphicType *pwebType);
};



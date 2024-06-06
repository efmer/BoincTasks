#pragma once

#include "TemplateRemoteTaskView.h"


// CCreateHtml command target

class CRemoteCombinedTaskView;
class CCreateHtmlWebServer : public CObject
{
public:
	CCreateHtmlWebServer();
	virtual ~CCreateHtmlWebServer();

	bool Create(int iPosHeader, CListViewEx *pView, CListCtrl *pListCtrl, int *piColomnOrder, int iColumnsMax, CString *psSelectedColumns, int iPosTranslation, char **pcHtml);

	COLORREF Rgb(COLORREF color);

private:
	void RemoveComment(CString *psHtml);

//	void ReplaceRowTasks(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1], int iItem, int iRow, CString *psInsertRow);
//	void ReplaceHeaderTasks(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1], int iRow, CString *psInsertRow);
};



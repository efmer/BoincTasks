#pragma once

#include "TemplateRemoteTaskView.h"

// CCreateHtml command target

class CRemoteCombinedTaskView;
class CCreateHtml : public CObject
{
public:
	CCreateHtml();
	virtual ~CCreateHtml();

	bool CreateMultiple(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1]);
	bool Create(CString *psFile, CString *psDest, CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1]);

	bool ReplaceRow(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1], int iItem, int iRow, CString *psInsertRow);
	void ReplaceHeader(CRemoteCombinedTaskView *pRemoteTaskView, CListCtrl *pListCtrl, int m_iColumnOrder[NUM_REMOTE_TASK_COLUMNS+1], int iRow, CString *psInsertRow);

	bool StartFtp();

private:
	int		m_iChanged;
	CString	m_sChanged;

};



#pragma once


// CWebRead command target

class CComputer;
class CWorkUnit;
class CWebRead : public CObject
{
public:
	CWebRead();
	virtual ~CWebRead();
	int		ReadTasksPage(CString sHostId, int iPage, int *piRead, CList <CWorkUnit*,CWorkUnit*> *plWorkunit);
	int		ReadTasksInfoPage(CString sTaskId, CWorkUnit *plWorkunit);
	int		ReadComputerPage(CString sUserId,int iPage,int *piRead, CList <CComputer*,CComputer*> *m_pComputers);

protected:
	int		Login(CString sAccountKey, CFile *pFile);
	int		LoginForm(CString sAccountKey, CString *sPageBuffer);
	int		ReadWebPage(CString sHostId, CString sUrl, int iPage, CStringA *psPageBuffer);

	bool	GetTasksResultTotalLine(int *piPos,int *piRead, CStringA *psPageBuffer, CWorkUnit *pWu);
	bool	GetTasksInfoResultTotalLine(CStringA *psPageBuffer, CWorkUnit *pWu);
	bool	GetComputerTotalLine(int *piPos, int *piRead, CStringA *psPageBuffer, CComputer *pComputer);
	bool	GetResult(CStringA sFind, CStringA sDelimeter, int *piPos,CStringA *psPageBuffer, CStringA *psValue);
	void	Strip(CStringA *psValue);

};



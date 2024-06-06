#pragma once

#include <afxinet.h>

#define EXEPTION_TEXT "Exeption"

class CInternetSession;

// CHtmlConnect command target

class CInternetSession;

class CHtmlConnect : public CObject
{
public:
	CHtmlConnect();
	virtual ~CHtmlConnect();
	void SetServer(CString *psServer, int iPort);
	void SessionBegin();
	void SessionEnd();

	CString	PostData(CString sPhp, CString *psData, CString sId);
	CString	PostDataJson(CString sPhp, CString *psData, bool bVersionFolder = true);

private:
	CString	MakeRequestHeadersJson(CString& sBoundary);
	CString	MakeRequestHeaders(CString& sBoundary);
	CString	MakePreFileData(CString& strBoundary, CString sId, CString sToken, CString& strFileName);
	CString	MakePostFileData(CString& sBoundary);

	DWORD m_dwLastTick;

	CString m_sVersion;

	CString m_sServer;
	int		m_iPort;

	CInternetSession* m_pSession;
};



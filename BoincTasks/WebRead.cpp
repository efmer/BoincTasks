// WebRead.cpp : implementation file
//

#include "stdafx.h"

#include <afxinet.h>
#include "BoincTasks.h"
#include "WebRead.h"
#include "workunit.h"
#include "computer.h"

#include <direct.h>

// CWebRead

CWebRead::CWebRead()
{
}

CWebRead::~CWebRead()
{
}

int CWebRead::Login(CString sAccountKey, CFile *pFile)
{
	CString sPageBuffer;	

	for (int iTry = 0; iTry < 4; iTry++)
	{
		if (LoginForm(sAccountKey, &sPageBuffer) == TRUE)
		{
			pFile->Write(sPageBuffer, sPageBuffer.GetLength());
			return TRUE;
		}
	}
	return FALSE;


}


int CWebRead::ReadTasksPage(CString sHostId, int iPage,int *piRead, CList <CWorkUnit*,CWorkUnit*> *plWorkunit)
{
	CStringA	sPageBuffer;
	CString		sUrl;
	int			iPos, iReturn = FALSE;
	CWorkUnit	wu;

	sUrl = L"results.php?hostid=";
	for (int iTry = 0; iTry < 4; iTry++)
	{
		if (ReadWebPage(sHostId, sUrl, iPage, &sPageBuffer) == TRUE)
		{
			iPos = 0;
			while (GetTasksResultTotalLine(&iPos, piRead, &sPageBuffer, &wu))
			{
				CWorkUnit *pWu			= new CWorkUnit;
				pWu->m_iWorkUnit		= wu.m_iWorkUnit;
				pWu->m_iTaskId			= wu.m_iTaskId;
				pWu->m_sTimeSend		= wu.m_sTimeSend;
				pWu->m_sTimeReported	= wu.m_sTimeReported;
				pWu->m_iStatus			= wu.m_iStatus;
				pWu->m_fClaimedCredit	= wu.m_fClaimedCredit;
				pWu->m_fGrantedCredit	= wu.m_fGrantedCredit;
				pWu->m_fRunTime			= wu.m_fRunTime;
				plWorkunit->AddTail(pWu);
				iReturn = TRUE;
			}
			return iReturn;
		}
	}
	return FALSE;
}

int CWebRead::ReadTasksInfoPage(CString sTaskId, CWorkUnit *pWorkunit)
{
	CStringA	sPageBuffer;
	CString		sUrl;
	int			iPos, iReturn = FALSE;

	sUrl = L"result.php?resultid=";
	for (int iTry = 0; iTry < 4; iTry++)
	{
		if (ReadWebPage(sTaskId, sUrl, -1, &sPageBuffer) == TRUE)
		{
			iPos = 0;
			if (GetTasksInfoResultTotalLine(&sPageBuffer, pWorkunit))
			{
				iReturn = TRUE;
			}
		}
	}
	return FALSE;
}

int CWebRead::ReadComputerPage(CString sAccountKey,int iPage,int *piRead, CList <CComputer*,CComputer*> *m_plComputers)
{
	CStringA	sPageBuffer;
	CString		sUrl;
	int			iPos;
	CComputer	computer;

	bool		bResult;
	CFile		file;
	WCHAR		wcBuffer[256];
	DWORD		dwLen;
	CString		sUserDirectory, sFileName, sUserName, sFileWrite, sError, sTLS;

	// setup file write
	wcBuffer[0] = 0;
	GetUserNameW(wcBuffer,&dwLen);

	bResult = false;

	sUserName = wcBuffer;

	sUserDirectory = "C:\\documents and settings\\" + sUserName + "\\Application Data\\eFMer";
	_mkdir(sUserDirectory);
	sUserDirectory = "C:\\documents and settings\\" + sUserName + "\\Application Data\\eFMer\\BoincTasks";
	_mkdir(sUserDirectory);

	sFileName = sUserDirectory + "\\computers.txt";

	file.Open(sFileName,CFile::modeCreate | CFile::modeWrite);


	Login(sAccountKey,&file);

//	sUserId = "";
	sUrl = L"hosts_user.php";
//	sUrl = L"hosts_user.php?userid=";
	for (int iTry = 0; iTry < 4; iTry++)
	{
		if (ReadWebPage("", sUrl, iPage, &sPageBuffer) == TRUE)
		{
			file.Write(sPageBuffer, sPageBuffer.GetLength());

			iPos = 0;
			while (GetComputerTotalLine(&iPos, piRead, &sPageBuffer, &computer))
			{
				CComputer *pComputer = new CComputer;
				pComputer->m_iComputerId = computer.m_iComputerId;
				pComputer->m_sComputerId = computer.m_sComputerId;
				m_plComputers->AddTail(pComputer);
//				iReturn = TRUE;
			}
			file.Close();
			return TRUE;
		}
	}
	file.Close();
	return FALSE;
}

int CWebRead::LoginForm(CString sAccountKey, CString *psPageBuffer)
{
	bool		bReturn = FALSE;
	CWorkUnit	wu;
	CString		sUrl;

	//the verb we will be using for this connection
	//if NULL then GET is assumed
	LPCTSTR pstrVerb = _T("GET");
	//the address of the url in the request was obtained from
	LPCTSTR pstrReferer = NULL;
	LPCTSTR pstrAcceptTypes = NULL;
	//Http version we are using; NULL = HTTP/1.0
	LPCTSTR pstrVersion = NULL;
	CString szHeaders = _T("Accept: text/html, text/htm\r\n");

	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_DONT_CACHE;
	DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
	//Proxy setting if we need them
	LPCTSTR pstrProxyName = NULL;
	LPCTSTR pstrProxyBypass = NULL;
	LPCTSTR pstrName = _T("BoincTasks");
	LPCTSTR lpstrServer = _T("setiathome.berkeley.edu");

//	sUrl = "login_action.php?key=7a240d0a2fcb9a8921c5768114a94d9f";
	sUrl = "login_action.php?key=";

	sUrl+= sAccountKey;

	LPCTSTR strUrl = sUrl.GetBuffer();

	DWORD dwFlags = NULL;

	CInternetSession	session(pstrName, 1, INTERNET_FLAG_TRANSFER_ASCII);
	
	int ntimeOut = 30;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,1000* ntimeOut);
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,1000);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES,1);

	//Enable or disable status callbacks
	session.EnableStatusCallback(TRUE);

	CHttpConnection*	pServer = NULL;   
	CHttpFile* pFile = NULL;
	DWORD dwRet;
	try {		

		pServer = session.GetHttpConnection(lpstrServer, 80, "",""); //, pstrName);
		pFile = pServer->OpenRequest(pstrVerb, strUrl, pstrReferer, 1, &pstrAcceptTypes, pstrVersion, dwHttpRequestFlags);

		pFile->AddRequestHeaders(szHeaders);
		pFile->AddRequestHeaders("User-Agent: GetWebFile/1.0\r\n", HTTP_ADDREQ_FLAG_ADD_IF_NEW);

		pFile->SendRequest();

		pFile->QueryInfoStatusCode(dwRet);//Check wininet.h for info
										  //about the status codes

//		CGetWebFileDlg * pWebFileDlg = (CGetWebFileDlg*) AfxGetMainWnd();
		if (dwRet == HTTP_STATUS_DENIED)
		{
	//		return false;
		}

		if(dwRet == HTTP_STATUS_OK)
		{
			// warning these are bytes 
			int len = (int) pFile->GetLength();
			char buf[40000];
			int numread;
			buf[0] = '\0';
			char *pBuf = buf;

			if ((numread = pFile->Read(buf,sizeof(buf)-1)) > 0)
			{
				buf[numread] = '\0';
				*psPageBuffer = buf;				// ascii   buffer
				bReturn = TRUE;
			}

		}
		pFile->Close();      
		delete pFile;
		pServer->Close();
		delete pServer;
		session.Close();
	}
	catch (CInternetException* pEx) 
	{
      // catch any exceptions from WinINet      
		TCHAR szErr[1024];
		szErr[0] = '\0';
        if(!pEx->GetErrorMessage(szErr, 1024)) strcpy_s(szErr,_T("Update unknown error"));
		TRACE(_T("File transfer failed!! - %s"),szErr); 

		pEx->Delete();
		if(pFile)
			delete pFile;
		if(pServer)
			delete pServer;
		session.Close(); 
		return false;
	}

	return bReturn;
	return 0;
}


int CWebRead::ReadWebPage(CString sId, CString sUrl, int iPage, CStringA *psPageBuffer)
{
	bool		bReturn = FALSE;
	CWorkUnit	wu;

	//the verb we will be using for this connection
	//if NULL then GET is assumed
	LPCTSTR pstrVerb = _T("GET");
	//the address of the url in the request was obtained from
	LPCTSTR pstrReferer = NULL;
	LPCTSTR pstrAcceptTypes = NULL;
	//Http version we are using; NULL = HTTP/1.0
	LPCTSTR pstrVersion = NULL;
	CString szHeaders = _T("Accept: text/html, text/htm\r\n");

	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_DONT_CACHE;
	DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
	//Proxy setting if we need them
	LPCTSTR pstrProxyName = NULL;
	LPCTSTR pstrProxyBypass = NULL;
	LPCTSTR pstrName = _T("BoincTasks");
	LPCTSTR lpstrServer = _T("setiathome.berkeley.edu");

	sUrl += sId;
	CString sPage, sTxt;

	if (iPage >=0)
	{
		iPage *= 20;								// 20 on a page
		if (iPage != 0)
		{
			sPage.Format("&offset=%d", iPage);
			sUrl = sUrl + sPage;
		}
	}

	LPCTSTR strUrl = sUrl.GetBuffer();

	DWORD dwFlags = NULL;

	CInternetSession	session(pstrName, 1, INTERNET_FLAG_TRANSFER_ASCII);
	
	int ntimeOut = 30;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,1000* ntimeOut);
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,1000);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES,1);

	//Enable or disable status callbacks
	session.EnableStatusCallback(TRUE);

	CHttpConnection*	pServer = NULL;   
	CHttpFile* pFile = NULL;
	DWORD dwRet;
	try {		

		pServer = session.GetHttpConnection(lpstrServer, 80, "",""); //, pstrName);
		pFile = pServer->OpenRequest(pstrVerb, strUrl, pstrReferer, 1, &pstrAcceptTypes, pstrVersion, dwHttpRequestFlags);

		pFile->AddRequestHeaders(szHeaders);
		pFile->AddRequestHeaders("User-Agent: GetWebFile/1.0\r\n", HTTP_ADDREQ_FLAG_ADD_IF_NEW);

		pFile->SendRequest();

		pFile->QueryInfoStatusCode(dwRet);//Check wininet.h for info
										  //about the status codes

//		CGetWebFileDlg * pWebFileDlg = (CGetWebFileDlg*) AfxGetMainWnd();
		if (dwRet == HTTP_STATUS_DENIED)
		{
	//		return false;
		}

		if(dwRet == HTTP_STATUS_OK)
		{
			// warning these are bytes 
			int len = (int) pFile->GetLength();
			char buf[40000];
			int numread;
			buf[0] = '\0';
			char *pBuf = buf;

			if ((numread = pFile->Read(buf,sizeof(buf)-1)) > 0)
			{
				buf[numread] = '\0';
				*psPageBuffer = buf;				// ascii   buffer
				bReturn = TRUE;
			}

		}
		pFile->Close();      
		delete pFile;
		pServer->Close();
		delete pServer;
		session.Close();
	}
	catch (CInternetException* pEx) 
	{
      // catch any exceptions from WinINet      
		TCHAR szErr[1024];
		szErr[0] = '\0';
        if(!pEx->GetErrorMessage(szErr, 1024)) strcpy_s(szErr,_T("Update unknown error"));
		TRACE(_T("File transfer failed!! - %s"),szErr); 

		pEx->Delete();
		if(pFile)
			delete pFile;
		if(pServer)
			delete pServer;
		session.Close(); 
		return false;
	}

	return bReturn;
}


bool CWebRead::GetTasksResultTotalLine(int *piPos, int *piRead, CStringA *psPageBuffer, CWorkUnit *pWu)
{
	CStringA sFind;
	CStringA sValue;

	if (GetResult("result.php?resultid=","\"", piPos, psPageBuffer, &sValue))	// Task ID
	{
		pWu->m_iTaskId = atoi(sValue);
		if (GetResult("workunit.php?wuid=","\"", piPos, psPageBuffer, &sValue))	// Work unit ID
		{
			pWu->m_iWorkUnit = atoi(sValue);
			if (GetResult("<td>","</td>", piPos, psPageBuffer, &sValue))		// Time Send
			{
				pWu->m_sTimeSend = sValue;
				if (GetResult("<td>","</td>", piPos, psPageBuffer, &sValue))	// Time Reported
				{
					pWu->m_sTimeReported = sValue;
					if (GetResult("<td>","</td>", piPos, psPageBuffer, &sValue))// Status
					{
						pWu->m_iStatus = WU_STATUS_UNKNOWN;
						sValue.MakeLower();
						if (sValue.Find("error") >=0)
						{
							pWu->m_iStatus = WU_STATUS_ERROR;
						}
						if (sValue.Find("progress") >=0)
						{
							pWu->m_iStatus = WU_STATUS_PROGRESS;
						}
						if (sValue.Find("completed") >=0)
						{
							if (sValue.Find("waiting" >=0))
							{
								pWu->m_iStatus = WU_STATUS_COMPLETED_WAITING;
							}
							if (sValue.Find("validated") >=0)
							{
								pWu->m_iStatus = WU_STATUS_COMPLETED_VALID;
							}
							if (sValue.Find("inconclusive") >=0)
							{
								pWu->m_iStatus = WU_STATUS_COMPLETED_INCONCLUSIVE;
							}
						}
						if (sValue.Find("aborted") >=0)
						{
							pWu->m_iStatus = WU_STATUS_ERROR_ABORTED;
						}
						if (sValue.Find("detached") >=0)
						{
							pWu->m_iStatus = WU_STATUS_ERROR_DETACHED;
						}
						if (sValue.Find("timed out") >=0)
						{
							pWu->m_iStatus = WU_STATUS_ERROR_TIMED_OUT;
						}

						if (pWu->m_iStatus == WU_STATUS_UNKNOWN)
						{
							pWu->m_iStatus = WU_STATUS_UNKNOWN;
						}

						if (GetResult("<td align=right>","</td>", piPos, psPageBuffer, &sValue))// Run time
						{
							sValue.Replace(",","");
							if (sValue == "---") pWu->m_fRunTime = -1;
							else pWu->m_fRunTime = (float) atof(sValue);
							if (GetResult("<td align=right>","</td>", piPos, psPageBuffer, &sValue))// Claimed
							{
								sValue.Replace(",","");
								if (sValue == "---") pWu->m_fClaimedCredit = -1;
								else pWu->m_fClaimedCredit = (float) atof(sValue);
								if (GetResult("<td align=right>","</td>", piPos, psPageBuffer, &sValue))// Claimed
								{
									sValue.Replace(",","");
									if (sValue == "---") pWu->m_fGrantedCredit = -1;
									else
									{
										if (sValue == "pending") pWu->m_fGrantedCredit = -2;
										else pWu->m_fGrantedCredit = (float) atof(sValue);						
									}
								}
							}
						}
						(*piRead)++;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool CWebRead::GetTasksInfoResultTotalLine(CStringA *psPageBuffer, CWorkUnit *pWu)
{
//	CStringA sFind;
	CStringA sValue;
	int		 iPos;

	iPos = 0;
	if (GetResult("Claimed credit</td><td class=fieldvalue>","<", &iPos, psPageBuffer, &sValue)) pWu->m_fClaimedCredit = (float) atof(sValue);
	else pWu->m_fClaimedCredit = -1;
	
	iPos = 0;
	if (GetResult("Granted credit</td><td class=fieldvalue>","<", &iPos, psPageBuffer, &sValue))
	{
		pWu->m_fGrantedCredit = (float) atof(sValue);
	}
	else pWu->m_fGrantedCredit = -1;

	return false;
}

bool CWebRead::GetComputerTotalLine(int *piPos, int *piRead, CStringA *psPageBuffer, CComputer *pComputer)
{
	CStringA sFind;
	CStringA sValue;

	if (GetResult("<a href=show_host_detail.php?hostid=",">", piPos, psPageBuffer, &sValue))	// Comptuer ID
	{
		pComputer->m_iComputerId = atoi(sValue);
		if (GetResult("<td>","<", piPos, psPageBuffer, &sValue))								// Comptuer String
		{
			pComputer->m_sComputerId= sValue;
			return true;
		}
	}
	return false;
}

bool CWebRead::GetResult(CStringA sFind, CStringA sDelimiter, int *piPos, CStringA *psPageBuffer, CStringA *psValue)
{
	int iEnd, iLen;

	CStringA sFindBuffer;

	sFindBuffer = *psPageBuffer;//->Mid(*piPos);
	sFindBuffer.MakeLower();

	*piPos = sFindBuffer.Find(sFind,*piPos);
	if (*piPos > 0)
	{
		iLen = sFind.GetLength();
		*piPos += iLen;
		iEnd = sFindBuffer.Find(sDelimiter, *piPos);
		if (iEnd > 0)
		{
			iLen =  iEnd - *piPos;
			*psValue = sFindBuffer.Mid(*piPos,iLen);
			CString sTest = sFindBuffer.Mid(*piPos,200);
			Strip(psValue);
			*piPos += iLen;

			return true;
		}
	}
	return false;
}

// Strip font

void CWebRead::Strip(CStringA *psValue)
{
	int iPos = 0, iEnd, iLen;

	iPos = psValue->Find("<font",iPos);
	if (iPos >= 0)
	{
		iEnd = psValue->Find(">",iPos);
		iLen = iEnd - iPos;
		*psValue = psValue->Mid(iEnd+1);
		psValue->Replace("</font>","");
	}
}